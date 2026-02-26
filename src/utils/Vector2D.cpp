//
// Created by navjo on 1/28/2026.
//

#include "Vector2D.hpp"

#include <cmath>

Vector2D Vector2D::operator*(float scaler) const {
    return Vector2D(x * scaler, y * scaler);
}

//non-member operator function so we can make use of putting the Vector2D on the right hand side
Vector2D operator*(float scaler, Vector2D& vec) {
    return vec * scaler;
}

Vector2D &Vector2D::operator*=(float scaler) {
    this -> x *= scaler;
    this -> y *= scaler;
    return *this;
}

Vector2D& operator*=(float scaler, Vector2D& vec) {
    return vec *= scaler;
}

Vector2D Vector2D::operator/(float scaler) const {
    return Vector2D(x / scaler, y / scaler);
}

Vector2D operator/(float scaler, Vector2D& vec) {
    return Vector2D(vec.x / scaler, vec.y / scaler);
}

Vector2D &Vector2D::operator/=(float scaler) {
    this -> x /= scaler;
    this -> y /= scaler;
    return *this;
}

//member operator function so we use the Vector2D for +=
Vector2D &Vector2D::operator+=(const Vector2D &vec) {
    this -> x += vec.x;
    this -> y += vec.y;
    return *this;
}

Vector2D Vector2D::operator+(float scaler) {
    return Vector2D(x + scaler, y + scaler);
}

Vector2D operator+(float scaler, Vector2D& vec) {
    return vec + scaler;
}

Vector2D Vector2D::operator-(Vector2D& vec) {
    return Vector2D(x - vec.x, y - vec.y);
}

Vector2D &Vector2D::operator-=(const Vector2D &vec) {
    this -> x -= vec.x;
    this -> y -= vec.y;
    return *this;
}

bool Vector2D::operator==(const Vector2D& vec) const {
    return x == vec.x && y == vec.y;
}

bool Vector2D::operator!=(const Vector2D &vec) const {
    return x != vec.x && y != vec.y;
}

Vector2D &Vector2D::operator-() {
    this -> x = -x;
    this -> y = -y;
    return *this;
}

Vector2D &Vector2D::normalize() {
    float length = std::sqrt(x*x + y*y);

    //if the length was 4.4
    //x = x/4.4
    //y = y/4.4
    if (length > 0) {
        this->x /= length;
        this->y /= length;
    }

    return *this;
}

