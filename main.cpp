//#include "Game.h"
#include "Gameplay.h"
#include "Main_Menu.h"
//#include "Start.h"
using namespace std;
//Start* start;

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

TTF_Font* gFont = NULL;



int main(int argc, char* argv[])
{
    MainMenu a;
    a.run();

    
	return 0;
}