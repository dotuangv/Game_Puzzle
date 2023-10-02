#include "Help.h"

void Help::run()
{
	if (!HTexture.loadFromFile("IMG//Help_Game.PNG")) {
		std::cout << "Can't not load Help.png : " << IMG_GetError();
	}
	else {
		isRunning = true;
		SDL_Event e;
		while (isRunning)
		{
			while (SDL_PollEvent(&e) != 0)
			{
				if (e.type == SDL_QUIT)
				{
					isRunning = false;
				}
			}
			SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_RenderClear(gRenderer);

			HTexture.Resize(SCREEN_WIDTH, SCREEN_HEIGHT);
			HTexture.render(0, 0);
			SDL_RenderPresent(gRenderer);
		}
	}
}
