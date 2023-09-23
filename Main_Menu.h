    #ifndef MAINMENU_H
    #define MAINMENU_H

    #include "CommonFunc.h"
    #include "LTexture.h"

    class MainMenu {
    public:
        MainMenu();
        ~MainMenu();
        MainMenu(SDL_Renderer* renderer);
        bool init();
        bool loadMedia();
        void close();
        void run();
        // Hiển thị menu chính
        void showMainMenu();

    private:
        LTexture MTexture;
        TTF_Font* mFont;
        SDL_Rect mPlayButton;
        SDL_Rect mInstructionsButton;
        SDL_Rect mExitButton;
        SDL_Color mTextColor;
        bool mQuit;

        void renderText(const std::string& text, int x, int y);
        void handleEvents();
    };

    #endif // MAINMENU_H
