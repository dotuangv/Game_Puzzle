//#include "Game.h"
#include "Gameplay.h"
using namespace std;
Gameplay* game;
const int Width = 600;
const int Height = 600;
int main(int argc, char* argv[])
{
    //Uint32 frameStart;
    //int frameTime;

    game = new Gameplay(4);
    game->init("PUZZLE", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Width, Height, false);
    //game->Play();
    game->SolveGame();
    game->clean();
	return 0;
}