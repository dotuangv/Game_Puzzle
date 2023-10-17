#include "Main_Menu.h"

using namespace std;

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

TTF_Font* gFont = NULL;

SDL_Texture* MainTexture;
vector<SDL_Texture*> MainTextures;

int n = 0;
int Order = 0;

int main(int argc, char* argv[])
{
    MainMenu a;
    a.run();
	return 0;
}