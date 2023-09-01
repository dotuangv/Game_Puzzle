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

static SDL_Window* g_Window = NULL;
static SDL_Renderer* g_Screen = NULL;
static SDL_Event* g_Event = NULL;


//Screen

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 640;
const int SCREEN_BPP = 32;

const int COLOR_KEY_R = 167;
const int COLOR_KEY_G = 175;
const int COLOR_KEY_B = 180;

const int FPS = 60;
const int frameDelay = 1000 / FPS;

#endif