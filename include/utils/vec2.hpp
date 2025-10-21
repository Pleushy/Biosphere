#pragma once

/** Simple struct of vector with 2 values and overrides */
/** Made by NightmarePog (check him out :3)*/
/** Modified by me to support int instead of float */
struct Vec2 {
    int x, y;

    Vec2(int x = 0, int y = 0) : x(x), y(y) {}

    Vec2 operator+(const Vec2& other) const {
        return Vec2(x + other.x, y + other.y);
    }

    Vec2 operator-(const Vec2& other) const {
        return Vec2(x - other.x, y - other.y);
    }

    Vec2 operator*(int scalar) const {
        return Vec2(x * scalar, y * scalar);
    }

    bool operator==(const Vec2& other) const {
        return x == other.x && y == other.y;
    }
};