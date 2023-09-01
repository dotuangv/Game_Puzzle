#include "Game.h"

using namespace std;
Game* game;

int main(int argc, char* argv[])
{
    Uint32 frameStart;
    int frameTime;

    game = new Game();
    game->init("PUZZLE", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, false);
    while (game->running()) {
        frameStart = SDL_GetTicks();

            game->handleEvents();
            game->update();
            game->render();

            frameTime = SDL_GetTicks() - frameStart;

            if (frameDelay > frameTime) {
                SDL_Delay(frameDelay - frameTime);
            }

        }
        game->clean();
    cout << "HELLO WORLD!" << endl;
	return 0;
}