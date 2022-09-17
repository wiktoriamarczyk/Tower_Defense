#pragma once
#include<iostream>
#include<vector>
#include<deque>
#include<string>
#include<fstream>
#include<memory>
#include<fstream>
#include<stdlib.h>
#include<time.h>
#include<algorithm>
#include<optional>
#include<functional>
#include"vec2.h"
#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>

using std::vector;
using std::deque;
using std::pair;
using std::make_pair;
using std::unique_ptr;
using std::make_unique;
using std::shared_ptr;
using std::make_shared;
using std::move;
using std::string;
using std::fstream;
using std::ifstream;
using std::sort;
using std::cout;
using std::optional;
using std::nullopt;
using std::function;
using std::max;

enum : int
{
    SCREEN_WIDTH = 1920,
    SCREEN_HEIGHT = 1080,
    MAP_WIDTH = 200,
    MAP_HEIGHT = 200,
    MAP_X = 1681,
    MAP_Y = 38,
    GRID_ROWS = 17,
    GRID_COLS = 30,
    CELL_SIZE = 64,
};

enum class eStateID
{
    UNKNOWN = 0,
    MAINMENU = 1,
    INGAME = 2,
    VICTORY = 3,
    HOWTOPLAY = 4,
};

enum class eGridValue : char
{
    BLOCKED = '0',
    FREE = '1',
    Tower1 = '2',
    Tower2 = '3',
    PATH = '4',
};

enum class eTowerID
{
    NONE = 0,
    Tower1 = 1,
    Tower2 = 2,
};

enum class eTowerPrice
{
    Tower1 = 100,
    Tower2 = 250,
};

enum class eDrawMode
{
    NORMAL,
    ADDITIVE,
};

enum class eGraphicLayer
{
    BACKGROUND,
    FOREGROUND,
    EFFECTS,
    OVERLAY,
    UI,
};

enum class eUIGroup
{
    UNKNOWN,
    TOWERS,
    SPELLS,
};

enum class eCursorType
{
    DEFAULT,
    BUILD,
    SWORD,
    HOURGLASS,
    SPELL,
};

namespace eFontStyle
{
    enum Type
    {
        REGULAR       = 0,      // 00000000
        BOLD          = 1 << 0, // 00000001
        ITALIC        = 1 << 1, // 00000010
        UNDERLINED    = 1 << 2, // 00000100
        STRIKE        = 1 << 3  // 00001000
    };
}

enum class eTextHorizontalAlign
{
    LEFT,
    CENTER,
    RIGHT,
};

struct DisplayParameters
{
    //optional<vec2i> DisplaySize;
    eDrawMode DrawMode = eDrawMode::NORMAL;
    vec2 DrawScale = vec2(1.0f, 1.0f);
    //vec2 SrcTopLeft = vec2(0, 0);
    //vec2 SrcSize = vec2(1, 1);
    vec2 Pivot = vec2(0, 0);
    sf::Color DrawColor = sf::Color::White;
    float Rotation = 0.0f;
};

struct DrawTextParameters
{
    string FontName = "Graph-35-pix";
    eFontStyle::Type FontStyle = eFontStyle::REGULAR;
    eTextHorizontalAlign HAlign = eTextHorizontalAlign::LEFT;
    sf::Color FontColor = sf::Color::White;
    bool ContinueLastLine = false;
    bool DisableDraw = false;
};

struct Damage
{
    float FireValue = 0;
    float LightningValue = 0;
    float IceValue = 0;
};

using Resistances = Damage;

struct TextLineData
{
    TextLineData()=default;
    TextLineData(string _Text, int _Size = 8, DrawTextParameters _Param = {})
        : Text(_Text), Size(_Size), Parameters(_Param) {}

    string Text;
    int    Size = 10;
    DrawTextParameters Parameters;
};

template <typename T>
string ToString(const T& value)
{
    return std::to_string(value);
}
string ToString(float value, int precision=1);