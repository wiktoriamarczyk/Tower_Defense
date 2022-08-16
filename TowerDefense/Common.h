#pragma once
#include<iostream>
#include<vector>
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
using std::none_of;

enum : int
{
    SCREEN_WIDTH = 1920,
    SCREEN_HEIGHT = 1080,
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