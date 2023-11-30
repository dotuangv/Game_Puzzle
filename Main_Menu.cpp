#include "Main_Menu.h"
#include "GetPlayerName.h"

MainMenu::MainMenu() {
    mQuit = false;
}

MainMenu::~MainMenu() {
    //close();
}

bool MainMenu::init() {
    bool success = true;

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        success = false;
    }
    else {
        gWindow = SDL_CreateWindow("SLIDING PUZZLE", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == nullptr) {
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
                if (!(gFont = TTF_OpenFont("Font//Snacko Trial.otf", 56))) {
                    std::cout << "Can't Initialize gFont TTF_Error: " << TTF_GetError() << std::endl;
                    success = false;
                }
                
                if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
                    success = false;
                }
            }
        }
    }
    isInit = success;
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
            gSpriteClips[i].y = 93 * i + 6;
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
    MTexture.free();
    Start.free();
    BHelp.free();
    Exit.free();
}

void MainMenu::run() {
    if (outGame) return;
    if (!isInit)
    {
        if (!init()) {
            return;
        }
    }
    if (!loadMedia()) {
        return;
    }
    bool isBackButtonClick = false;
    SDL_Event e;
    while (!mQuit && !outGame) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                mQuit = true;
                outGame = true;
                isBackButtonClick = false;
            }

            for (int i = 0; i < TOTAL_BUTTONS; ++i) {
                gButtons[i].HandleEvent(&e);
                // Xác định nút nào được nhấn và thực hiện tác vụ tương ứng
                if (isBackButtonClick || e.type == SDL_MOUSEBUTTONDOWN && gButtons[i].getCurrentSprite() == BUTTON_SPRITE_MOUSE_DOWN) {
                    if (isBackButtonClick && !outGame) i = BUTTON_PLAY;
                    switch (i) {
                    case BUTTON_PLAY:
                    {
                        GetPlayerName GetPlayerName;
                        if (!GetPlayerName.run())
                        {
                            cout << "Player Name is Empty " << endl;
                            break;
                        }
                        isBackButtonClick = false;
                        // Thực hiện hành động khi nút PLAY_BUTTON được nhấn
                        MenuStart startgame;
                        do {
                            startgame.run();
                        } while (!startgame.getIsChooseMode() && !outGame && !startgame.getIsOut());
                        if (!outGame && !startgame.getIsOut())
                        {
                            Gameplay* game;
                            game = new Gameplay();
                            game->Run();
                            if (!outGame) isBackButtonClick = true;
                        }
                        else if (startgame.getIsOut())
                        {
                            break;
                        }
                        else
                        {
                            mQuit = true;
                        }
                        break;
                    }
                    case BUTTON_INSTRUCTIONS:
                        // Thực hiện hành động khi nút INSTRUCTIONS_BUTTON được nhấn
                        //Chuyển đến màn hình hướng dẫn
                        if (!outGame)
                        {
                            Help gamehelp;
                            gamehelp.run();
                            if (gamehelp.getIsBackHelp())
                            {
                                outGame = false;
                                SDL_Delay(100);
                                while (SDL_PollEvent(&e));
                                break;
                            }
                        }
                        break;
                    case BUTTON_EXIT:
                        // Thực hiện hành động khi nút EXIT_BUTTON được nhấn
                        mQuit = true; // Kết thúc game
                        outGame = true;
                        break;
                    }
                }
            }
        }
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(gRenderer);
        if (!outGame)
        {

            MTexture.render(0, 0);
            gButtons[BUTTON_PLAY].render(Start, gSpriteClips);
            gButtons[BUTTON_INSTRUCTIONS].render(BHelp, gSpriteClips);
            gButtons[BUTTON_EXIT].render(Exit, gSpriteClips);
            SDL_RenderPresent(gRenderer);
        }
    }

    close();
}