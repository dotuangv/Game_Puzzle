//
// Created by ADMIN on 8/27/2023.
//

#ifndef PBL2_GAMEPUZZLE_GAME_H
#define PBL2_GAMEPUZZLE_GAME_H


#include "CommonFunc.h"

using namespace std;

class Game {
public:
    Game();
    ~Game();
    void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
    void handleEvents();
    void update();
    void render();
    void clean();
    bool running() {
        return isRunning;
    };
    int getCnt() {
        return cnt;
    }
private:
    int cnt = 0;
    bool isRunning;
    SDL_Window* window;
    SDL_Renderer* renderer;
};

#endif //PBL2_GAMEPUZZLE_GAME_H
