#include "vec2.h"

namespace math
{
    static constexpr double PI = 3.141592653589793238462643383279502884;
}

vec2::vec2(float _x, float _y) : x(_x), y(_y) {}

vec2::vec2(const sf::Vector2f& vector)
{
    x = vector.x;
    y = vector.y;
}

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

float vec2::GetAngleFromVec()const
{
    if (x == 0 && y == 0)
        return 0;

    return float(atan2(y, x) / math::PI * 180);
}

vec2::operator sf::Vector2f() const
{
    sf::Vector2f vector;
    vector.x = x;
    vector.y = y;
    return vector;
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

vec2 vec2::operator+=(const vec2& other)
{
    x += other.x;
    y += other.y;

    return *this;
}

vec2 vec2::operator-=(const vec2& other)
{
    x -= other.x;
    y -= other.y;

    return *this;
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

vec2 vec2::operator / (const vec2& other)const
{
    vec2 result;

    result.x = x / other.x;
    result.y = y / other.y;

    return result;
}

//============================================
#include "vec2.h"

vec2i::vec2i(int _x, int _y) : x(_x), y(_y) {}

vec2i::vec2i(const sf::Vector2i& vector)
{
    x = vector.x;
    y = vector.y;
}

vec2i::vec2i(const sf::Vector2u& vector)
{
    x = int(vector.x);
    y = int(vector.y);
}

int vec2i::GetLength()const
{
    return sqrt(x * x + y * y);
}

vec2i vec2i::GetNormalized()const
{
    vec2i result;
    int length = GetLength();

    // nie mozna dzielic przez 0
    if (length == 0)
        return result;

    result.x = x / length;
    result.y = y / length;;

    return result;
}

vec2i vec2i::operator+(const vec2i& other)const
{
    vec2i result;

    result.x = x + other.x;
    result.y = y + other.y;

    return result;
}

vec2i vec2i::operator-(const vec2i& other)const
{
    vec2i result;

    result.x = x - other.x;
    result.y = y - other.y;

    return result;
}

vec2i vec2i::operator-()const
{
    vec2i result;

    result.x = -x;
    result.y = -y;

    return result;
}

vec2i vec2i::operator+=(const vec2i& other)
{
    x += other.x;
    y += other.y;

    return *this;
}

vec2i vec2i::operator-=(const vec2i& other)
{
    x -= other.x;
    y -= other.y;

    return *this;
}

vec2i vec2i::operator*(const vec2i& other)const
{
    vec2i result;

    result.x = x * other.x;
    result.y = y * other.y;

    return result;
}

vec2i vec2i::operator*(int scalar)const
{
    vec2i result;

    result.x = x * scalar;
    result.y = y * scalar;

    return result;
}

vec2i vec2i::operator/(int scalar)const
{
    vec2i result;

    result.x = x / scalar;
    result.y = y / scalar;

    return result;
}

vec2i::operator vec2()const
{
    return vec2(float(x),float(y));
}

vec2i::operator sf::Vector2i() const
{
    sf::Vector2i vector;
    vector.x = x;
    vector.y = y;
    return vector;
}
