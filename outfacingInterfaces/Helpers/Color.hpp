//
// Created by jesse on 31/10/2023.
//

#ifndef BRACK_ENGINE_COLOR_HPP
#define BRACK_ENGINE_COLOR_HPP

#include <cstdint>

class Color {
public:
    Color() = default;

    Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : r(r), g(g), b(b), a(a) {};

    ~Color() = default;

    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

#endif //BRACK_ENGINE_COLOR_HPP
