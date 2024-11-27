#pragma once

#include <string>
#include <vector>
#include <numeric>

namespace testing
{
    namespace meta {

        template<typename T, bool = std::is_enum_v<T>>
        struct select_underlying_t {
            using type = T;
        };
        template<typename T>
        struct select_underlying_t<T, true> {
            using type = typename std::underlying_type<T>::type;
        };

        template <typename, template <typename> class, typename = void>
        struct is_detected : std::false_type {};
        template <typename T, template <typename> class Op>
        struct is_detected<T, Op, std::void_t<Op<T>>> : std::true_type {};

        template <typename T, typename = void>
        struct has_str_conv : std::false_type {};
        template <typename T>
        struct has_str_conv<T, std::void_t<decltype(static_cast<std::string>(std::declval<T const>()))>> : std::true_type {};

        template <typename T, typename = void>
        struct is_to_string_compatible : std::false_type {};
        template <typename T>
        struct is_to_string_compatible<T, std::void_t<decltype(std::to_string(std::declval<T>()))>> : std::true_type {};

        template <typename T, typename = void>
        struct is_print_to_compatible : std::false_type {};
        template <typename T>
        struct is_print_to_compatible<T, std::void_t<decltype(PrintTo(std::declval<T const&>(), std::declval<std::ostream*>()))>> : std::true_type {};
    }

    namespace naming {
        template<typename T,
            bool = meta::is_print_to_compatible<T>::value,
            bool = meta::has_str_conv<T>::value,
            bool = meta::is_to_string_compatible<T>::value>
            struct print_name {
            std::string operator()(const T& t) const {
                return "NONAME";
            }
        };
        template<typename T, bool Conv, bool ToString>
        struct print_name<T, true, Conv, ToString> {
            std::string operator()(const T& t) const {
                std::ostringstream oss;
                PrintTo(t, &oss);
                return oss.str();
            }
        };
        template<typename T, bool ToString>
        struct print_name<T, false, true, ToString> {
            std::string operator()(const T& t) const {
                return static_cast<std::string>(t);
            }
        };
        template<typename T>
        struct print_name<T, false, false, true> {
            std::string operator()(const T& t) const {
                return std::to_string(t);
            }
        };

        template<typename TV>
        std::string operator+(const std::string& s, const std::vector<TV>& items) {
            static print_name<TV> print_name;
            auto joined = std::accumulate(items.begin(), items.end(), std::string(),
                [](const std::string& a, const auto& item) {
                    return a + "_" + print_name(item);
                });
            if (!joined.empty()) {
                return s + joined.substr(1);
            }
            else {
                return s + "NOTHING";
            }
        }

        std::string w2s(const std::wstring w) {
            return std::string(w.begin(), w.end());
        }

        std::string operator+(const std::string& lhs, const std::wstring& rhs) {
            return lhs + w2s(rhs);
        }
    }

    namespace pred {
        using namespace naming;

        template<typename T1, typename T2>
        struct eq {
            bool operator() (const T1& t1, const T2& t2) {
                return static_cast<T2>(t1) == t2;
            }
        };

        template<typename T>
        struct eq<T, T> {
            bool operator() (const T& t1, const T& t2) {
                return t1 == t2;
            }
        };

        template<typename TC, typename TV>
        struct collection_pred {
        private:
            using this_t = collection_pred<TC, TV>;
            using raw_t = TV;
            using compare_func = std::function<bool(const raw_t&, typename TC::const_reference)>;
            std::string name;
            std::function<bool(const TC&, compare_func)> func;
            collection_pred(std::string name, std::function<bool(const TC&, compare_func)> func) : name(name), func(func) {}

        public:
            bool operator() (const TC& collection, compare_func compare = eq<raw_t, typename TC::value_type>()) const {
                return func(collection, compare);
            }

            this_t operator! () const {
                std::function<bool(const TC&, compare_func)> reverse_func = [f = func](const TC& collection, compare_func compare) {
                    return !f(collection, compare);
                };
                return this_t{
                    "not_" + this->name,
                    [func = func](const TC& collection, compare_func compare) {
                        return !func(collection, compare);
                    }
                };
            }

            operator std::string() const {
                return name;
            }

            static this_t contains(const TV& item) {
                static print_name<TV> print_name;
                return this_t{
                    "contains_" + print_name(item),
                    [item](const TC& collection, compare_func compare) {
                        auto it = std::find_if(collection.begin(), collection.end(),
                            [&item, compare](const auto& i) {
                                return compare(static_cast<raw_t>(item), i);
                            });
                        return it != collection.end();
                    }
                };
            }

            static this_t not_contains(const TV& item) {
                static print_name<TV> print_name;
                return this_t{
                    "not_contains_" + print_name(item),
                    [item](const TC& collection, compare_func compare) {
                        auto it = std::find_if(collection.begin(), collection.end(),
                            [&item, compare](const auto& i) {
                                return compare(static_cast<raw_t>(item), i);
                            });
                        return it == collection.end();
                    }
                };
            }


            static this_t includes_all(const std::vector<TV>& items) {
                return this_t{
                    "includes_" + items,
                    [items](const TC& collection, compare_func compare) {
                        return std::all_of(items.begin(), items.end(), [&collection, compare](const auto& item) {
                            auto it = std::find_if(collection.begin(), collection.end(),
                                [item, compare](const auto& i) {
                                    return compare(static_cast<raw_t>(item), i);
                                });
                            return it != collection.end();
                        });
                    }
                };
            }
            static this_t excludes_all(const std::vector<TV>& items) {
                return this_t{
                    "excludes_" + items,
                    [items](const TC& collection, compare_func compare) {
                        return std::all_of(items.begin(), items.end(), [&collection, compare](const auto& item) {
                            auto it = std::find_if(collection.begin(), collection.end(),
                                [item, compare](const auto& i) {
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
            using pred_t = collection_pred<std::vector<T>, T>;
            template<typename T>
            pred_t<T> contains(T item) {
                return pred_t<T>::contains(item);
            }

            template<typename T>
            pred_t<T> includes_all(std::vector<T> items) {
                return pred_t<T>::includes_all(items);
            }

            template <typename T, typename... Args>
            pred_t<T> includes_all(T first, Args... rest) {
                return pred_t<T>::includes_all(std::vector<T>{first, rest...});
            }

            template<typename T>
            pred_t<T> excludes_all(std::vector<T> items) {
                return pred_t<T>::excludes_all(items);
            }

            template <typename T, typename... Args>
            pred_t<T> excludes_all(T first, Args... rest) {
                return pred_t<T>::excludes_all(std::vector<T>{first, rest...});
            }


            template<typename T>
            using pred_u_t = collection_pred<std::vector<typename meta::select_underlying_t<T>::type>, T>;

            template<typename T>
            pred_u_t<T> contains_u(T item) {
                return pred_u_t<T>::contains(item);
            }

            template<typename T>
            pred_u_t<T> includes_all_u(std::vector<T> items) {
                return pred_u_t<T>::includes_all(items);
            }

            template<typename T>
            pred_u_t<T> excludes_all_u(std::vector<T> items) {
                return pred_u_t<T>::excludes_all(items);
            }
        }
        template<typename TC, typename TV>
        void PrintTo(const pred::collection_pred<TC, TV>& p, std::ostream* os) {
            *os << static_cast<std::string>(p);
        }
    }

    namespace naming {
        template<typename TC, typename TV>
        std::string operator+(const std::string& s, const pred::collection_pred<TC, TV>& p) {
            return s + static_cast<std::string>(p);
        }
        template<typename TC, typename TV>
        std::string operator+(const pred::collection_pred<TC, TV>& p, const std::string& s) {
            return static_cast<std::string>(p) + s;
        }
    }
}
