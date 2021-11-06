#include "vec2.h"


vec2::vec2(float _x, float _y) : x(_x), y(_y) {}


float vec2::GetLength()const
{
    return sqrt(x * x + y * y);
}

vec2 vec2::GetNormalized()const
{
    vec2 result;
    float length = GetLength();

    // nie mozna dzielic przez 0
    if (length == 0)
        return result;

    result.x = x / length;
    result.y = y / length;;

    return result;
}

vec2 vec2::operator+(const vec2& other)const
{
    vec2 result;

    result.x = x + other.x;
    result.y = y + other.y;

    return result;
}

vec2 vec2::operator-(const vec2& other)const
{
    vec2 result;

    result.x = x - other.x;
    result.y = y - other.y;

    return result;
}

vec2 vec2::operator-()const
{
    vec2 result;

    result.x = -x;
    result.y = -y;

    return result;
}

vec2 vec2::operator*(const vec2& other)const
{
    vec2 result;

    result.x = x * other.x;
    result.y = y * other.y;

    return result;
}

vec2 vec2::operator*(float scalar)const
{
    vec2 result;

    result.x = x * scalar;
    result.y = y * scalar;

    return result;
}

vec2 vec2::operator/(float scalar)const
{
    vec2 result;

    result.x = x / scalar;
    result.y = y / scalar;

    return result;
}