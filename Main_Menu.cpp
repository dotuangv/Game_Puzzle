#include "Main_Menu.h"

MainMenu::MainMenu() {
    TTF_Init();
    mFont = TTF_OpenFont("Font/Mooli-Regular.ttf", 36); // Đổi tên font tùy theo tên font của bạn

    mTextColor = { 255, 255, 255 }; // Màu chữ trắng

    // Kích thước và vị trí của các nút
    mPlayButton = { 100, 100, 200, 50 };
    mInstructionsButton = { 100, 200, 300, 50 };
    mExitButton = { 100, 300, 150, 50 };

    mQuit = false;
}

MainMenu::~MainMenu() {
    TTF_CloseFont(mFont);
    TTF_Quit();
    close();
}

bool MainMenu::init() {
    // Initialization flag
    bool success = true;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        success = false;
    }
    else {
        // Set texture filtering to linear
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
            printf("Warning: Linear texture filtering not enabled!");
        }
        //Create window
        gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL)
        {
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = false;
        }
        // Create renderer for window
        gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
        if (gRenderer == NULL) {
            printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
            success = false;
        }
        else {
            // Initialize renderer color
            SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

            // Initialize PNG loading
            int imgFlags = IMG_INIT_PNG;
            if (!(IMG_Init(imgFlags) & imgFlags)) {
                printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                success = false;
            }
        }
    }

    return success;
}

bool MainMenu::loadMedia() {
    // Loading success flag
    bool success = true;

    // Load texture
    if (!MTexture.loadFromFile("12_color_modulation/colors.png")) {
        printf("Failed to load colors texture!\n");
        success = false;
    }

    return success;
}

void MainMenu::close() {
    // Free loaded images
    MTexture.free();

    // Destroy renderer
    SDL_DestroyRenderer(gRenderer);
    gRenderer = NULL;

    // Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

//void MainMenu::run() {
//    if (!init()) {
//        printf("Failed to initialize!\n");
//    }
//    else {
//        if (!loadMedia()) {
//            printf("Failed to load media!\n");
//        }
//        else {
//            bool quit = false;
//            SDL_Event e;
//
//            Uint8 r = 255;
//            Uint8 g = 255;
//            Uint8 b = 255;
//
//            while (!quit) {
//                while (SDL_PollEvent(&e) != 0) {
//                    if (e.type == SDL_QUIT) {
//                        quit = true;
//                    }
//                    else if (e.type == SDL_KEYDOWN) {
//                        switch (e.key.keysym.sym) {
//                        case SDLK_q:
//                            r += 3;
//                            break;
//                        case SDLK_w:
//                            g += 3;
//                            break;
//                        case SDLK_e:
//                            b += 3;
//                            break;
//                        case SDLK_a:
//                            r -= 3;
//                            break;
//                        case SDLK_s:
//                            g -= 3;
//                            break;
//                        case SDLK_d:
//                            b -= 3;
//                            break;
//                        }
//                    }
//                }
//
//                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
//                SDL_RenderClear(gRenderer);
//
//                MTexture.SetColor(r, g, b);
//                MTexture.render(0, 0);
//
//                SDL_RenderPresent(gRenderer);
//            }
//        }
//    }
//
//    close();
//}

void MainMenu::run() {
    if (!init()) {
       printf("Failed to initialize!\n");
    }
    else
    while (!mQuit) {
        handleEvents();

        SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
        SDL_RenderClear(gRenderer);

        // Hiển thị các nút và văn bản tương ứng
        SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255); // Màu đỏ cho nút "Chơi"
        SDL_RenderFillRect(gRenderer, &mPlayButton);
        renderText("START AND CHOOSE ", mPlayButton.x + 10, mPlayButton.y + 10);

        SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255); // Màu xanh cho nút "Hướng dẫn"
        SDL_RenderFillRect(gRenderer, &mInstructionsButton);
        renderText("HELP", mInstructionsButton.x + 10, mInstructionsButton.y + 10);

        SDL_SetRenderDrawColor(gRenderer, 0, 0, 255, 255); // Màu xanh dương cho nút "Thoát"
        SDL_RenderFillRect(gRenderer, &mExitButton);
        renderText("EXIT", mExitButton.x + 10, mExitButton.y + 10);

        SDL_RenderPresent(gRenderer);
    }
}

void MainMenu::renderText(const std::string& text, int x, int y) {
    SDL_Surface* surface = TTF_RenderText_Solid(mFont, text.c_str(), mTextColor);
    if (surface == nullptr) {
        std::cerr << "TTF_RenderText_Solid error: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(gRenderer, surface);
    if (texture == nullptr) {
        std::cerr << "SDL_CreateTextureFromSurface error: " << SDL_GetError() << std::endl;
        return;
    }
    SDL_Rect destRect = { x, y, surface->w, surface->h };
    SDL_RenderCopy(gRenderer, texture, nullptr, &destRect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void MainMenu::handleEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            mQuit = true;
        }
        else if (e.type == SDL_MOUSEBUTTONDOWN) {
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);

            // Kiểm tra xem người dùng có nhấp vào các nút không
            if (mouseX >= mPlayButton.x && mouseX <= mPlayButton.x + mPlayButton.w &&
                mouseY >= mPlayButton.y && mouseY <= mPlayButton.y + mPlayButton.h) {
                // Xử lý lựa chọn "Chơi" ở đây
                std::cout << "Lựa chọn: Chơi" << std::endl;
            }
            else if (mouseX >= mInstructionsButton.x && mouseX <= mInstructionsButton.x + mInstructionsButton.w &&
                mouseY >= mInstructionsButton.y && mouseY <= mInstructionsButton.y + mInstructionsButton.h) {
                // Xử lý lựa chọn "Hướng dẫn game" ở đây
                std::cout << "Lựa chọn: Hướng dẫn game" << std::endl;
            }
            else if (mouseX >= mExitButton.x && mouseX <= mExitButton.x + mExitButton.w &&
                mouseY >= mExitButton.y && mouseY <= mExitButton.y + mExitButton.h) {
                mQuit = true;
            }
        }
    }
}
