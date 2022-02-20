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


class vec2i
{
public:
    int x = 0;
    int y = 0;

    vec2i() = default;
    vec2i(int _x, int _y);

    int GetLength()const;
    vec2i GetNormalized()const;

    operator vec2()const;

    vec2i operator+(const vec2i& other)const;
    vec2i operator-(const vec2i& other)const;
    vec2i operator-()const;
    vec2i operator*(const vec2i& other)const;
    vec2i operator*(int scalar)const;
    vec2i operator / (int scalar)const;
};

