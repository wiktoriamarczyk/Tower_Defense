#pragma once
#include<cmath>

class vec2
{
public:
    float x = 0;
    float y = 0;

    vec2() = default;
    vec2(float _x, float _y);

    float GetLength()const;
    vec2 GetNormalized()const;

    vec2 operator+(const vec2& other)const;
    vec2 operator-(const vec2& other)const;
    vec2 operator-()const;
    vec2 operator*(const vec2& other)const;
    vec2 operator*(float scalar)const;
    vec2 operator / (float scalar)const;
};

