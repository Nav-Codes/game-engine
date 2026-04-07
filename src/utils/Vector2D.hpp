//
// Created by navjo on 1/28/2026.
//

#ifndef VECTOR2D_HPP
#define VECTOR2D_HPP

#include <iostream>

class Vector2D {
public:
    float x = 0.0f;
    float y = 0.0f;

    Vector2D() {
        x = 0.0f;
        y = 0.0f;
    }

    Vector2D(float x, float y) : x(x), y(y) {}

    //member operator function
    //Vector2D has to be on left, float on right
    Vector2D operator*(float scaler) const;

    //Vector2D on right, float on left
    //friend makes it a non-member function
    friend Vector2D operator*(float scaler, Vector2D& vec);

    Vector2D operator/(float scaler) const;
    friend Vector2D operator/(float scaler, Vector2D& vec);

    Vector2D& operator+=(const Vector2D& vec);

    Vector2D& operator-=(const Vector2D& vec);

    Vector2D& operator*=(float scaler);
    friend Vector2D& operator*=(float scaler, Vector2D& vec);

    Vector2D& operator/=(float scaler);

    Vector2D operator+(float scaler);
    friend Vector2D operator+(float scaler, Vector2D& vec);

    Vector2D operator-(Vector2D& vec);

    bool operator==(const Vector2D& vec) const;

    bool operator!=(const Vector2D& vec) const;

    Vector2D& operator-();

    Vector2D& normalize();
};

inline std::ostream& operator<<(std::ostream& os, const Vector2D& vec) {
    os << "(" << static_cast<int>(vec.x) << ", " << static_cast<int>(vec.y) << ")";
    return os;
}

#endif //VECTOR2D_HPP
