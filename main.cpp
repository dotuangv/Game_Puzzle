//#include "Game.h"
#include "Gameplay.h"
//#include "Start.h"
using namespace std;
Gameplay* game;
//Start* start;
const int Width = 1000;
const int Height = 600;
int main(int argc, char* argv[])
{
    
    //start = new Start(4);
    //start->StartInit("PUZZLE", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Width, Height, false);
    //start->SetStart();
    game = new Gameplay();
    game->init("PUZZLE", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Width, Height, false);
    while (game->GetRunning2())
    {
        game->SetStart();
        game->StartUpdate();
        game->StartRenderer();
        game->StartEvents();
        //cout << "HEllo\n";
        if (!game->GetRunning2())
        {
            game->SetUpGame(Height);
            //game->Play();
        }
    }
    //game->SetUpGame(Height);
    game->Play();
    //game->handleEvents();
    //game->SolveGame();
    game->clean();
    game->Clear();
	return 0;
}