#include "testinghelper.h"

enum class Color {Blue, Red, Yellow};
struct ColorN : public testing::naming::named<Color> {
    ColorN(Color e): testing::naming::named<Color>(e){}

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
};

ColorN _n(typename ColorN::unnamed_t e){
    return ColorN(e);
}
enum class Direction {Up, Right, Bottom, Left};

