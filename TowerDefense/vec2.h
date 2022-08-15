#pragma once
#include<cmath>
#include<SFML/System/Vector2.hpp>

class vec2
{
public:
    float x = 0.0;
    float y = 0.0;

    vec2() = default;
    vec2(float _x, float _y);
    vec2(const sf::Vector2f& vector);

    float GetLength()const;
    vec2 GetNormalized()const;

    operator sf::Vector2f()const;

    vec2 operator+(const vec2& other)const;
    vec2 operator-(const vec2& other)const;
    vec2 operator-()const;
    vec2 operator+=(const vec2& other);
    vec2 operator-=(const vec2& other);
    vec2 operator*(const vec2& other)const;
    vec2 operator*(float scalar)const;
    vec2 operator / (float scalar)const;
    auto operator<=>(const vec2& other)const = default;
};


class vec2i
{
public:
    int x = 0;
    int y = 0;

    vec2i() = default;
    vec2i(int _x, int _y);
    vec2i(const sf::Vector2i& vector);
    vec2i(const sf::Vector2u& vector);

    int GetLength()const;
    vec2i GetNormalized()const;

    operator vec2()const;
    operator sf::Vector2i()const;

    vec2i operator+(const vec2i& other)const;
    vec2i operator-(const vec2i& other)const;
    vec2i operator-()const;
    vec2i operator+=(const vec2i& other);
    vec2i operator-=(const vec2i& other);
    vec2i operator*(const vec2i& other)const;
    vec2i operator*(int scalar)const;
    vec2i operator / (int scalar)const;
    auto operator<=>(const vec2i& other)const = default;
};

