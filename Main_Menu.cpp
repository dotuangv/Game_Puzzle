#include "Main_Menu.h"

MainMenu::MainMenu() {
    gWindow = NULL;
    gRenderer = NULL;
    gFont = NULL;
    mQuit = false;
}

MainMenu::~MainMenu() {
    close();
}

bool MainMenu::init() {
    bool success = true;

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        success = false;
    }
    else {
        gWindow = SDL_CreateWindow("Main Menu", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL) {
            success = false;
        }
        else {
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if (gRenderer == NULL) {
                success = false;
            }
            else {
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags)) {
                    success = false;
                }

                if (TTF_Init() == -1) {
                    success = false;
                }

                if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
                    success = false;
                }
            }
        }
    }

    return success;
}

bool MainMenu::loadMedia() {
    bool success = true;

    if (!MTexture.loadFromFile("IMG/Begin.PNG")) {
        success = false;
    }
    else if (!Start.loadFromFile("IMG/Start.PNG")) {
        success = false;
    }
    else if (!BHelp.loadFromFile("IMG/Help.PNG")) {
        success = false;
    }
    else if (!Exit.loadFromFile("IMG/Exit.PNG")) {
        success = false;
    }
    else {
        MTexture.Resize(SCREEN_WIDTH, SCREEN_HEIGHT);
        for (int i = 0; i < BUTTON_SPRITE_TOTAL; ++i) {
            gSpriteClips[i].x = 1;
            gSpriteClips[i].y = 93 * i + 10;
            gSpriteClips[i].w = BUTTON_WIDTH;
            gSpriteClips[i].h = BUTTON_HEIGHT;
        }

        mPlayButton = { (SCREEN_WIDTH - BUTTON_WIDTH) / 2, 3 * SCREEN_HEIGHT / 5 , BUTTON_WIDTH, BUTTON_HEIGHT};
        mInstructionsButton = { (SCREEN_WIDTH - BUTTON_WIDTH) / 2, 3 * SCREEN_HEIGHT / 5 + 92 , BUTTON_WIDTH, BUTTON_HEIGHT };
        mExitButton = { (SCREEN_WIDTH - BUTTON_WIDTH) / 2, 3 * SCREEN_HEIGHT / 5 + 2 * 92, BUTTON_WIDTH, BUTTON_HEIGHT };

        gButtons[BUTTON_PLAY].SetAllValue(mPlayButton.x, mPlayButton.y, BUTTON_WIDTH, BUTTON_HEIGHT);
        gButtons[BUTTON_INSTRUCTIONS].SetAllValue(mInstructionsButton.x, mInstructionsButton.y, BUTTON_WIDTH, BUTTON_HEIGHT);
        gButtons[BUTTON_EXIT].SetAllValue(mExitButton.x, mExitButton.y, BUTTON_WIDTH, BUTTON_HEIGHT);
    }

    return success;
}

void MainMenu::close() {
    MTexture.free();

    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    if (gFont != NULL) {
        TTF_CloseFont(gFont);
        gFont = NULL;
    }

    Mix_Quit();
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}

void MainMenu::run() {
    if (!init()) {
        return;
    }

    if (!loadMedia()) {
        return;
    }

    SDL_Event e;

    while (!mQuit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                mQuit = true;
            }

            for (int i = 0; i < TOTAL_BUTTONS; ++i) {
                gButtons[i].HandleEvent(&e);
                // Xác định nút nào được nhấn và thực hiện tác vụ tương ứng
                if (e.type == SDL_MOUSEBUTTONDOWN && gButtons[i].getCurrentSprite() == BUTTON_SPRITE_MOUSE_DOWN) {
                    switch (i) {
                    case BUTTON_PLAY:
                    {
                        // Thực hiện hành động khi nút PLAY_BUTTON được nhấn
                        MenuStart startgame;
                        startgame.run();
                        Gameplay* game;
                        game = new Gameplay();
                        game->Run(FALSE);
                        break;
                    }
                    case BUTTON_INSTRUCTIONS:
                    {
                        // Thực hiện hành động khi nút INSTRUCTIONS_BUTTON được nhấn
                        //Chuyển đến màn hình hướng dẫn
                        Help gamehelp;
                        gamehelp.run();
                        break;
                    }
                    case BUTTON_EXIT:
                        // Thực hiện hành động khi nút EXIT_BUTTON được nhấn
                        mQuit = true; // Kết thúc game
                        break;
                    default:
                        break;
                    }
                }
            }
        }

        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(gRenderer);

        MTexture.render(0, 0);

        gButtons[BUTTON_PLAY].render(Start, gSpriteClips);
        gButtons[BUTTON_INSTRUCTIONS].render(BHelp, gSpriteClips);
        gButtons[BUTTON_EXIT].render(Exit, gSpriteClips);
        SDL_RenderPresent(gRenderer);
    }

    close();
}