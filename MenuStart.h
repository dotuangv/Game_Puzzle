#pragma once
#ifndef  START_H_
#define START_H_

#include "LTexture.h"
#include "LButton.h"

class MenuStart {
private:
    LTexture HTexture;
    vector<LTexture> MenuStartButton;
    bool isRunning;
    enum StartButton
    {
        BUTTON_MODE_3 = 0,
        BUTTON_MODE_4 = 1,
        BUTTON_MODE_5 = 2,
        BUTTON_MODE_6 = 3,
        BUTTON_TURN_LEFT = 4,
        BUTTON_TURN_RIGHT = 5,
        START_BUTTON_TOTAL = 6
    };
    SDL_Rect gSpriteClips[BUTTON_SPRITE_TOTAL];
    SDL_Rect gRect[START_BUTTON_TOTAL];
    LButton gButton[START_BUTTON_TOTAL];

public:
    MenuStart();
    ~MenuStart();

    bool init();
    bool loadMedia();
    void close();
    void run();
};


#endif // ! HELP_H