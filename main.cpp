//#include "Game.h"
#include "Gameplay.h"
using namespace std;
Gameplay* game;

int main(int argc, char* argv[])
{
    Uint32 frameStart;
    int frameTime;

    game = new Gameplay(3);
    game->init("PUZZLE", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 768, 768, false);
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
    //int N;
    //cout << "N = ";
    //cin >> N;
    //Gameplay* gameplay = new Gameplay(N);
    //gameplay->setA();
    //gameplay->setGoal();
    //gameplay->Random();
    //gameplay->display(gameplay->getA());
    //gameplay->AuToRun();
    //cout << "HELLO WORLD!" << endl;
	return 0;
}