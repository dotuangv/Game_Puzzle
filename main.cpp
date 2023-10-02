#include "Main_Menu.h"

using namespace std;

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

TTF_Font* gFont = NULL;

int n = 0;

int main(int argc, char* argv[])
{
    MainMenu a;
    a.run();
	return 0;
}