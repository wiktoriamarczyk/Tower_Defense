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
#include<SDL.h>
#include<SDL_Mixer.h>
#include<SDL_image.h>


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

enum : int
{
    SCREEN_WIDTH = 1920,
    SCREEN_HEIGHT = 1080,
    GRID_ROWS = 17,
    GRID_COLS = 30,
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
    TOWER1 = '2',
    TOWER2 = '3',
};

enum class eTowerID
{
    NONE = 0,
    TOWER1 = 1,
    TOWER2 = 2,
};

enum class eTowerPrice
{
    TOWER1 = 100,
    TOWER2 = 250,
};

bool SDL_IsKeyPressed(SDL_Scancode Code);
