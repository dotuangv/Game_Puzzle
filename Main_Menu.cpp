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
        gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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
    else if (!Help.loadFromFile("IMG/Help.PNG")) {
        success = false;
    }
    else if (!Exit.loadFromFile("IMG/Exit.PNG")) {
        success = false;
    }
    else {
        MTexture.Resize(SCREEN_WIDTH, SCREEN_HEIGHT);
        for (int i = 0; i < BUTTON_SPRITE_TOTAL; ++i) {
            gSpriteClips[i].x = 0;
            gSpriteClips[i].y = 93 * i + 10;
            gSpriteClips[i].w = BUTTON_WIDTH;
            gSpriteClips[i].h = BUTTON_HEIGHT;
        }

        mPlayButton = { (SCREEN_WIDTH - BUTTON_WIDTH) / 2, 3 * SCREEN_HEIGHT / 5 , BUTTON_WIDTH, BUTTON_HEIGHT};
        mInstructionsButton = { (SCREEN_WIDTH - BUTTON_WIDTH) / 2, 3 * SCREEN_HEIGHT / 5 + 92 , BUTTON_WIDTH, BUTTON_HEIGHT };
        mExitButton = { (SCREEN_WIDTH - BUTTON_WIDTH) / 2, 3 * SCREEN_HEIGHT / 5 + 2 * 92, BUTTON_WIDTH, BUTTON_HEIGHT };

        gButtons[BUTTON_PLAY].SetPosition(mPlayButton.x, mPlayButton.y);
        gButtons[BUTTON_INSTRUCTIONS].SetPosition(mInstructionsButton.x, mInstructionsButton.y);
        gButtons[BUTTON_EXIT].SetPosition(mExitButton.x, mExitButton.y);
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
                gButtons[BUTTON_PLAY].render(Start, gSpriteClips);
                gButtons[BUTTON_INSTRUCTIONS].render(Help, gSpriteClips);
                gButtons[BUTTON_EXIT].render(Exit, gSpriteClips);
                SDL_RenderPresent(gRenderer);

                // Xác định nút nào được nhấn và thực hiện tác vụ tương ứng
                if (e.type == SDL_MOUSEBUTTONDOWN && gButtons[i].getCurrentSprite() == BUTTON_SPRITE_MOUSE_DOWN) {
                    switch (i) {
                    case BUTTON_PLAY:
                        // Thực hiện hành động khi nút PLAY_BUTTON được nhấn
                        Gameplay* game;
                        game = new Gameplay();
                            game->SetStart();
                            while (game->GetRunning2())
                            {
                                game->StartUpdate();
                                game->StartRenderer();
                                game->StartEvents();
                                if (!game->GetRunning2())
                                {
                                    game->SetUpGame(SCREEN_HEIGHT);
                                }
                            }
                            //game->SolveGame();
                            game->Play();
                            game->clean();
                            game->Clear();
                        delete game; // Giải phóng bộ nhớ sau khi trò chơi kết thúc
                        game = nullptr;
                        break;
                    case BUTTON_INSTRUCTIONS:
                        // Thực hiện hành động khi nút INSTRUCTIONS_BUTTON được nhấn
                        //Chuyển đến màn hình hướng dẫn
                        break;
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
        gButtons[BUTTON_INSTRUCTIONS].render(Help, gSpriteClips);
        gButtons[BUTTON_EXIT].render(Exit, gSpriteClips);
        SDL_RenderPresent(gRenderer);
    }

    close();
}