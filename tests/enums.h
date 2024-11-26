#pragma once

#include "testinghelper.h"

enum class Color {Blue, Red, Yellow};
void PrintTo(const Color& p, std::ostream* os) {
    auto func = [](const auto& e){
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
    };
    *os << func(p);
}

// no printTo defined
enum class Direction {Up, Right, Bottom, Left};

