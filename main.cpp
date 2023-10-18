#include "Main_Menu.h"

using namespace std;

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

TTF_Font* gFont = NULL;


bool isUSE = true;
int n = 0;
int Order = 0;
bool isInit = false;

int main(int argc, char* argv[])
{
    while (isUSE)
    {
        isUSE = false;
        MainMenu a;
        a.run();
    }
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    if (gFont != NULL) {
        TTF_CloseFont(gFont);
        gFont = NULL;
    }

    Mix_Quit();
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
	return 0;
}