#pragma once
#ifndef MAINMENU_H
#define MAINMENU_H

#include "CommonFunc.h"
#include "LTexture.h"
#include "LButton.h"
#include "Help.h"
#include "MenuStart.h"

class MainMenu {
public:
    MainMenu();
    ~MainMenu();
    bool init();
    bool loadMedia();
    void close();
    void run();

private:
    TTF_Font* gFont;
    SDL_Color mTextColor;

    LTexture MTexture; //Background
    LTexture Start; //Start Button
    LTexture BHelp; //Help Button
    LTexture Exit; //Exit Button
    SDL_Rect mPlayButton;
    SDL_Rect mInstructionsButton;
    SDL_Rect mExitButton;

    LButton gButtons[TOTAL_BUTTONS];

    bool mQuit;

    enum MenuButtons {
        BUTTON_PLAY = 0,
        BUTTON_INSTRUCTIONS = 1,
        BUTTON_EXIT = 2,
        TOTAL_BUTTONS
    };

    SDL_Rect gSpriteClips[BUTTON_SPRITE_TOTAL];

    void renderText(const std::string& text, int x, int y);
};
#endif // MAINMENU_H
