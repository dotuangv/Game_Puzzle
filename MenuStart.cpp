#include "MenuStart.h"

MenuStart::MenuStart() : isRunning(true), MenuStartButton(vector<LTexture>(6)) {
}

MenuStart::~MenuStart() {
    close();
}

bool MenuStart::init() {
    return true;
}

bool MenuStart::loadMedia() {
    bool success = true;

    if (!HTexture.loadFromFile("IMG//Start_Menu//ModeAndImage.PNG")) {
        std::cout << "Can't not load ModeAndImage.PNG : " << IMG_GetError();
        success = false;
    }
    else if (!MenuStartButton[BUTTON_MODE_3].loadFromFile("IMG//Start_Menu//3x3.png")) {
        std::cout << "Can't not load Mode img : \n" << IMG_GetError();
        success = false;
    }
    else if (!MenuStartButton[BUTTON_MODE_4].loadFromFile("IMG//Start_Menu//4x4.png")) {
        std::cout << "Can't not load Mode img : \n" << IMG_GetError();
        success = false;
    }
    else if (!MenuStartButton[BUTTON_MODE_5].loadFromFile("IMG//Start_Menu//5x5.png")) {
        std::cout << "Can't not load Mode img : \n" << IMG_GetError();
        success = false;
    }
    else if (!MenuStartButton[BUTTON_MODE_6].loadFromFile("IMG//Start_Menu//6x6.png")) {
        std::cout << "Can't not load Mode img : \n" << IMG_GetError();
        success = false;
    }
    else if (!MenuStartButton[BUTTON_TURN_LEFT].loadFromFile("IMG//Start_Menu//TurnLeft.png")) {
        std::cout << "Can't not load Mode img : \n" << IMG_GetError();
        success = false;
    }
    else if (!MenuStartButton[BUTTON_TURN_RIGHT].loadFromFile("IMG//Start_Menu//TurnRight.png")) {
        std::cout << "Can't not load Mode img : \n" << IMG_GetError();
        success = false;
    }
    else
    {
        for (int i = 0; i < BUTTON_SPRITE_TOTAL; ++i) {
            gSpriteClips[i].x = 12;
            gSpriteClips[i].y = 10 + 144 * i;
            gSpriteClips[i].w = 144;
            gSpriteClips[i].h = 140;
        }
        for (int i = 0; i < 4; ++i)
        {
            gButton[i].SetAllValue(568 + i * 181, 537, 144, 144);
        }
        for (int i = 4; i < START_BUTTON_TOTAL; ++i)
        {
            gButton[i].SetAllValue(617 + (i - 4) * 461, 285, 144, 144);
        }
    }
    return success;
}

void MenuStart::close() {
    HTexture.free(); // Giải phóng texture
    for (int i = 0; i < START_BUTTON_TOTAL; ++i)
    {
        MenuStartButton[i].free();
    }
    // Không cần giải phóng renderer và window ở đây vì chúng được quản lý trong MainMenu.

    // Đóng SDL_ttf và Mix
    //TTF_Quit();
    //Mix_Quit();
}

void MenuStart::run() {
    if (!init()) {
        return;
    }
    if (!loadMedia())
    {
        std::cout << "Fail to Load Media \n";
    }
    else
    {
        SDL_Event e;
        isRunning = true;
        while (isRunning) {
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    isRunning = false;
                }

                for (int i = 0; i < START_BUTTON_TOTAL; ++i) {
                    gButton[i].HandleEvent(&e);
                    for (int j = 0; j < START_BUTTON_TOTAL; ++j)
                        gButton[i].render(MenuStartButton[i], gSpriteClips);
                    SDL_RenderPresent(gRenderer);

                    // Xác định nút nào được nhấn và thực hiện tác vụ tương ứng
                    if (e.type == SDL_MOUSEBUTTONDOWN && gButton[i].getCurrentSprite() == BUTTON_SPRITE_MOUSE_DOWN) {
                        switch (i) {
                        case BUTTON_MODE_3:
                        {
                            //Ô 3 x 3
                            n = 3;
                            isRunning = false;
                            break;
                        }
                        case BUTTON_MODE_4:
                        {
                            //Ô 4 x 4
                            n = 4;
                            isRunning = false;
                            break;
                        }
                        case BUTTON_MODE_5:
                        {
                            //Ô 5 x 5
                            n = 5;
                            isRunning = false;
                            break;
                        }
                        case BUTTON_MODE_6:
                        {
                            //Ô 6 x 6
                            n = 6;
                            isRunning = false;
                            break;
                        }
                        case BUTTON_TURN_LEFT:
                        {
                            //Chuyển ảnh trái
                            
                            break;
                        }
                        case BUTTON_TURN_RIGHT:
                        {
                            //Chuyển ảnh phải

                            break;
                        }
                        default:
                            break;
                        }
                    }
                }
            }
            SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderClear(gRenderer);

            HTexture.render(0, 0);
            for (int i = 0; i < START_BUTTON_TOTAL; ++i)
            {
                gButton[i].render(MenuStartButton[i], gSpriteClips);
            }
            //MenuStartButton[BUTTON_TURN_LEFT].render(500, 500);
            //MenuStartButton[BUTTON_TURN_RIGHT].render(500, 500);

            SDL_RenderPresent(gRenderer);
        }
    }
    close();
}