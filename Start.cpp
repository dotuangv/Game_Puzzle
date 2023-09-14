//#include "Start.h"
//
//
//SDL_Event Start::event;
////SDL_Texture* PlayerTex;
////SDL_Rect srcR, destR;
//
//
//void Start::StartInit(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
//{
//    int flags = 0;
//
//    if (fullscreen) {
//        flags = SDL_WINDOW_FULLSCREEN;
//    }
//
//    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
//        cout << "Subsystems Initialised!..." << endl;
//        window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
//        if (window) {
//            cout << "Window created!" << endl;
//        }
//
//        renderer = SDL_CreateRenderer(window, -1, 0);
//        if (renderer) {
//            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
//            cout << "Renderer created!" << endl;
//        }
//
//        isRunning = true;
//    }
//    else {
//        isRunning = false;
//    }
//}
//
