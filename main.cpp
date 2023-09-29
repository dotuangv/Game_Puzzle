//#include "Game.h"
#include "Gameplay.h"
#include "Main_Menu.h"
//#include "Start.h"
using namespace std;
Gameplay* game;
//Start* start;


SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;

int main(int argc, char* argv[])
{
    
    start = new Start(4);
    start->StartInit("PUZZLE", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Width, Height, false);
    start->SetStart();
    game = new Gameplay();
    game->init("PUZZLE", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, false);
    while (game->GetRunning2())
    {
        game->SetStart();
        game->StartUpdate();
        game->StartRenderer();
        game->StartEvents();
        //cout << "HEllo\n";
        if (!game->GetRunning2())
        {
            game->SetUpGame(SCREEN_HEIGHT);
            //game->Play();
        }
    }
    //game->SetUpGame(Height);
    //game->Play();
    //game->handleEvents();
    game->SolveGame();
    //game->clean();
    //game->Clear();
    //MainMenu a;
    //a.run();
	return 0;
}