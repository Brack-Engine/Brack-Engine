//
// Created by jesse on 31/10/2023.
//

#include "Helpers/Vector2.hpp"

float Vector2::getX() const {
    return x;
}

float Vector2::getY() const {
    return y;
}

void Vector2::setX(const float newX) {
    x = newX;
}

void Vector2::setY(const float newY) {
    y = newY;
}

float Vector2::magnitude() const {
    return std::sqrt(x * x + y * y);
}

Vector2 Vector2::normalized() const {
    auto mag = magnitude();
    if (mag != 0)
        return Vector2(x / mag, y / mag);
    return *this;
}

float Vector2::dot(const Vector2 &other) const {
    return x * other.x + y * other.y;
}

float Vector2::cross(const Vector2 &other) const {
    return x * other.y - y * other.x;
}

float Vector2::distance(const Vector2 &other) const {
    auto dx = x - other.x;
    auto dy = y - other.y;
    return std::sqrt(dx * dx + dy * dy);
}

float Vector2::angel() const {
    return std::atan2(y, x);
}

Vector2 Vector2::operator+(const Vector2 &other) const {
    return {x + other.x, y + other.y};
}

Vector2 Vector2::operator-(const Vector2 &other) const {
    return {x - other.x, y - other.y};
}

Vector2 Vector2::operator*(const float &other) const {
    return {x * other, y * other};
}

Vector2 Vector2::operator/(const float &other) const {
    return {x / other, y / other};
}

bool Vector2::operator==(const Vector2 &other) const {
    return x == other.x && y == other.y;
}

bool Vector2::operator!=(const Vector2 &other) const {
    return x != other.x || y != other.y;
}

void Vector2::operator+=(const Vector2 &other) {
    x += other.x;
    y += other.y;
}

void Vector2::operator-=(const Vector2 &other) {
    x -= other.x;
    y -= other.y;
}

void Vector2::operator*=(const float &other) {
    x *= other;
    y *= other;
}

void Vector2::operator/=(const float &other) {
    x /= other;
    y /= other;
}

void Vector2::operator*=(const Vector2 &other) {
    x *= other.x;
    y *= other.y;
}

void Vector2::operator/=(const Vector2 &other) {
    x /= other.x;
    y /= other.y;
}

Vector2 Vector2::operator*(const Vector2 &other) const {
    return {x * other.x, y * other.y};
}

Vector2 Vector2::operator/(const Vector2 &other) const {
    return {y / other.x, y / other.y};
}