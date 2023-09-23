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

extern SDL_Window* gWindow;
extern SDL_Renderer* gRenderer;
extern SDL_Event* g_Event;


//Screen

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 600;
const int SCREEN_BPP = 32;

const int COLOR_KEY_R = 167;
const int COLOR_KEY_G = 175;
const int COLOR_KEY_B = 180;

const int FPS = 60;
const int frameDelay = 1000 / FPS;

#endif