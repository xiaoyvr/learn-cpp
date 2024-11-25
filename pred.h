#include <string>
#include <numeric>
#include <algorithm>

namespace meta {

    template<typename T, bool = std::is_enum_v<T>>
    struct select_underlying_t
    {
        using type = T;
    };
    template<typename T>
    struct select_underlying_t<T, true>
    {
        using type = typename std::underlying_type<T>::type;
    };

    template <typename, template <typename> class, typename = void>
    struct is_detected : std::false_type {};
    template <typename T, template <typename> class Op>
    struct is_detected<T, Op, std::void_t<Op<T>>> : std::true_type {};

    template <typename T>
    using unnamed_t_detector = typename T::unnamed_t;
    template<typename T, bool = is_detected<T, unnamed_t_detector>::value>
    struct select_unnamed_t
    {
        using type = T;
    };
    template<typename T>
    struct select_unnamed_t<T, true>
    {
        using type = typename T::unnamed_t;
    };

    template <typename T, typename = void>
    struct has_str_conv : std::false_type {};
    template <typename T>
    struct has_str_conv<T, std::void_t<decltype(static_cast<std::string>(std::declval<T const>()))>> : std::true_type {};

    template <typename T, typename = void>
    struct is_to_string_compatible : std::false_type {};
    template <typename T>
    struct is_to_string_compatible<T, std::void_t<decltype(std::to_string(std::declval<T>()))>> : std::true_type {};
}

namespace naming {

    template<typename T, bool = meta::has_str_conv<T>::value, bool = meta::is_to_string_compatible<T>::value>
    struct to_string 
    {
        static std::string invoke(const T& t){ return "NONAME"; }
    };
    template<typename T, bool B>
    struct to_string <T, true, B>
    {
        static std::string invoke(const T& t){ return std::string(t); }
    };
    template<typename T, bool B>
    struct to_string <T, B, true>
    {
        static std::string invoke(const T& t){ return std::to_string(t); }
    };

    template<typename TV>
    std::string operator+(const std::string& s, const std::vector<TV>& items)
    {
        auto joined = std::accumulate(items.begin(), items.end(), std::string(), [](const std::string& a, const auto& item){
            return a + "_" + to_string<TV>::invoke(item);
        });
        if (!joined.empty())
        {
            return s + joined.substr(1);   
        } else {
            return s + "EMPTY";
        }
    }

    template<typename T>
    struct named {
        using unnamed_t = T;
        named(T e) : e(e) {}
        virtual operator std::string () const = 0; 
        virtual operator T () const = 0;
        protected:
        T e;
    };
}

namespace pred {
    using namespace naming;
    struct static_cast_eq{
        template<typename T1, typename T2>
        struct functor{
            bool operator() (const T1& t1, const T2& t2){
                return static_cast<T2>(t1) == t2;
            }
        };
    };

    struct eq{
        template<typename T, typename T1>
        struct functor{
            bool operator() (const T& t1, const T1& t2){
                return t1 == t2;
            }
        };
    };

    template<typename TC, typename TV, typename TEQ>
    struct collection_pred {
        private:
        using this_t = collection_pred<TC, TV, TEQ>;
        using raw_t = typename meta::select_unnamed_t<TV>::type;
        using compare_func = std::function<bool(const raw_t&, typename TC::const_reference)>;
        using functor_t = typename TEQ::template functor<raw_t, typename TC::value_type>;
        std::string name;
        std::function<bool(const TC&, compare_func)> func; 
        collection_pred(std::string name, std::function<bool(const TC&, compare_func)> func): name(name), func(func){}
        
        public:
        bool operator() (const TC& collection, compare_func compare = functor_t() ){
            return func(collection, compare);
        }

        operator std::string () const {
            return name;
        }

        static this_t contains(const TV& item){
            return this_t {
                "contains_" + to_string<TV>::invoke(item),
                [item](const TC& collection, compare_func compare) {
                    auto it = std::find_if(collection.begin(), collection.end(), 
                        [&item, compare](const auto& i){
                            return compare(static_cast<raw_t>(item), i);
                        });
                    return it != collection.end();
                }
            };
        }

        static this_t not_contains(const TV& item){
            return this_t {
                "not_contains_" + to_string<TV>::invoke(item),
                [item](const TC& collection, compare_func compare) {
                    auto it = std::find_if(collection.begin(), collection.end(), 
                        [&item, compare](const auto& i){
                            return compare(static_cast<raw_t>(item), i);
                        });
                    return it == collection.end();
                }
            };
        }


        static this_t includes_all(const std::vector<TV>& items){
            return this_t {
                "includes_" + items,
                [items](const TC& collection, compare_func compare) {
                    return std::all_of(items.begin(), items.end(), [&collection, compare](const auto& item){
                        auto it = std::find_if(collection.begin(), collection.end(), 
                            [item, compare](const auto& i){
                                return compare(static_cast<raw_t>(item), i);
                            });
                        return it != collection.end();
                    });
                }
            };
        }
        static this_t excludes_all(const std::vector<TV>& items){
            return this_t {
                "excludes_" + items,
                [items](const TC& collection, compare_func compare) {
                    return std::all_of(items.begin(), items.end(), [&collection, compare](const auto& item){
                        auto it = std::find_if(collection.begin(), collection.end(), 
                            [item, compare](const auto& i){
                                return compare(static_cast<raw_t>(item), i);
                            });
                        return it == collection.end();
                    });
                }
            };
        }
    };

    namespace vec {
        template<typename T>
        using pred_t = collection_pred<std::vector<typename meta::select_unnamed_t<T>::type>, T, eq>;
        template<typename T>
        pred_t<T> contains(T item){
            return pred_t<T>::contains(item);
        }

        template<typename T>
        pred_t<T> not_contains(T item){
            return pred_t<T>::not_contains(item);
        }

        template<typename T>
        pred_t<T> includes_all(std::vector<T> items){
            return pred_t<T>::includes_all(items);
        }

        template<typename T>
        pred_t<T> excludes_all(std::vector<T> items){
            return pred_t<T>::excludes_all(items);
        }

        template<typename T>
        using pred_u_t = collection_pred<std::vector<typename meta::select_underlying_t<typename meta::select_unnamed_t<T>::type>::type>, T, static_cast_eq>;

        template<typename T>
        pred_u_t<T> contains_underlying(T item){
            return pred_u_t<T>::contains(item);
        }

        template<typename T>
        pred_u_t<T> not_contains_underlying(T item){
            return pred_u_t<T>::not_contains(item);
        }

        template<typename T>
        pred_u_t<T> includes_all_underlying(std::vector<T> items){
            return pred_u_t<T>::includes_all(items);
        }

        template<typename T>
        pred_u_t<T> excludes_all_underlying(std::vector<T> items){
            return pred_u_t<T>::excludes_all(items);
        }
    }
}
