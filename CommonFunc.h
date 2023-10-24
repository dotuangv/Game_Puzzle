#pragma once
#ifndef COMMON_FUNCTION_H_

#define COMMON_FUNCTION_H_

#include <windows.h>
#include <cstring>
#include <iostream>
#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <queue>
#include <vector>
#include <set>
#include <utility>
#include "GameObject.h"
#include <string>
#include "Gameplay.h"
#include <SDL_ttf.h>
#include <sstream>
#include <string>

extern SDL_Window* gWindow;
extern SDL_Renderer* gRenderer;
extern SDL_Event* g_Event;
extern TTF_Font* gFont;
//Screen

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int SCREEN_BPP = 32;

const int COLOR_KEY_R = 167;
const int COLOR_KEY_G = 175;
const int COLOR_KEY_B = 180;

const int FPS = 60;
const int frameDelay = 1000 / FPS;

const int TOTAL_IMAGE = 4;

//Button constants
const int BUTTON_WIDTH = 470;
const int BUTTON_HEIGHT = 93;
const int TOTAL_BUTTONS = 3;
extern int n;
extern int Mode;
extern int Order;
extern bool isUSE;
extern bool isInit;
enum LButtonSprite
{
    BUTTON_DEFAULT = -1,
    BUTTON_SPRITE_MOUSE_OUT = 0,
    BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
    BUTTON_SPRITE_MOUSE_DOWN = 2,
    BUTTON_SPRITE_MOUSE_UP = 3,
    BUTTON_SPRITE_TOTAL = 4
};

enum MenuButtons {
    //BUTTON_RELOAD = ,
    BUTTON_AUTO_RUN = 0,
    BUTTON_SELF_SOLVING,
    /*BUTTON_BACK,*/
    TOTAL_GAMEPLAY_BUTTONS = 2
};


#endif