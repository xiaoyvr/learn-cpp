#include <iostream>
#include <string>
#include "pred.h"

enum class Color {Blue, Red, Yellow};

enum class Direction {Up, Righ, Bottom, Left};

struct ColorW : public naming::named<Color> {
    ColorW(Color e): naming::named<Color>(e){}

    operator std::string () const override {
        switch (e)
        {
        case Color::Blue:
            return "Blue";
        case Color::Red:
            return "Red";
        case Color::Yellow:
            return "Yellow";
        default:
            return "Unknown";
        }
    }
    operator Color () const override{
        return e;
    }
};

ColorW _n(typename ColorW::unnamed_t e){
    return ColorW(e);
}

std::ostream& operator<<(std::ostream& s, const std::string& str)
{
    return s << str.c_str();
}


int main(int argc, char const *argv[]) {

    {
        auto contain = pred::vec::contains(1);
        auto result = contain(std::vector<int> {0,1,2,3});        
        std::cout<< contain << " is " << (result? "true" : "false") << std::endl;
    }
    {
        auto contain = pred::vec::not_contains(1);
        auto result = contain(std::vector<int> {0,1,2,3});        
        std::cout<< contain << " is " << (result? "true" : "false") << std::endl;
    }
    {
        auto contain = pred::vec::contains(_n(Color::Red));
        auto colors = std::vector<Color> {Color::Red,Color::Yellow, Color::Blue};
        auto result = contain(colors);
        std::cout<< contain << " is " << (result? "true" : "false") << std::endl;
    }
    {

        auto contain = pred::vec::not_contains_underlying(_n(Color::Red));
        auto result = contain(std::vector<int> {0,1,2,3});
        std::cout<< contain << " is " << (result? "true" : "false") << std::endl;
    }
    {
        auto includes_all = pred::vec::includes_all(std::vector<int>{2,3});
        auto collection = std::vector<int> {0,1, 2,4};
        auto result = includes_all(collection);        
        std::cout<< includes_all << " is " << (result? "true" : "false") << std::endl;
    }

    {
        auto excludes_all = pred::vec::excludes_all(std::vector<int>{2,3});
        auto collection = std::vector<int> {0,1, 5,4};
        auto result = excludes_all(collection);        
        std::cout<< excludes_all << " is " << (result? "true" : "false") << std::endl;
    }
    {
        auto contain = pred::vec::contains(Direction::Up);
        auto directions = std::vector<Direction> {Direction::Up, Direction::Bottom};
        auto result = contain(directions);
        std::cout<< contain << " is " << (result? "true" : "false") << std::endl;
    }

    {
        auto contain = pred::vec::not_contains_underlying(Direction::Up);
        auto directions = std::vector<int> {0, 1, 2};
        auto result = contain(directions);
        std::cout<< contain << " is " << (result? "true" : "false") << std::endl;
    }

    {
        auto includes_all = pred::vec::includes_all_underlying<ColorW>({_n(Color::Red), _n(Color::Yellow)});
        auto collection = std::vector<int> {0,1};
        auto result = includes_all(collection);        
        std::cout<< includes_all << " is " << (result? "true" : "false") << std::endl;
    }

    return 0;
}
