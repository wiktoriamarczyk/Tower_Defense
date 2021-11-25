#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<memory>
#include<fstream>
#include<stdlib.h>
#include<time.h>
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

enum : int
{
    SCREEN_WIDTH = 1920,
    SCREEN_HEIGHT = 1080,
    OBJECT_HEIGHT = 50,
    OBJECT_WIDTH = 50,
    BOSS_WIDTH = 200,
    BOSS_HEIGHT = 100,
    INVADER_SPEED = 2,
    GUN_SPEED = 500,
    BOSS_SPEED = 50,
    SHOT_SPEED = 500,
    SHOT_WIDTH = 3,
    SHOT_HEIGHT = 9,
};

enum class eStateID
{
    UNKNOWN = 0,
    MAINMENU = 1,
    INGAME = 2,
    VICTORY = 3,
    HOWTOPLAY = 4,
};

enum class eTowerID
{
    NONE = 0,
    SOME_TOWER = 1,
};


bool SDL_IsKeyPressed(SDL_Scancode Code);