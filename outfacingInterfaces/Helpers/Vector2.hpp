//
// Created by jesse on 31/10/2023.
//

#ifndef BRACK_ENGINE_VECTOR2_HPP
#define BRACK_ENGINE_VECTOR2_HPP

#include "cmath"

class Vector2 {
public:
    Vector2() : x(0.0f), y(0.0f) {};

    Vector2(const float x, const float y) : x(x), y(y) {};

    ~Vector2() = default;

    float getX() const;

    float getY() const;

    void setX(const float newX);

    void setY(const float newY);

    float magnitude() const;

    Vector2 normalized() const;

    float dot(const Vector2 &other) const;

    float cross(const Vector2 &other) const;

    float distance(const Vector2 &other) const;

    float angel() const;

    Vector2 operator+(const Vector2 &other) const;

    Vector2 operator-(const Vector2 &other) const;

    Vector2 operator*(const float &other) const;

    Vector2 operator/(const float &other) const;

private:
    float x, y;
};


#endif //BRACK_ENGINE_VECTOR2_HPP
