#pragma once
#include "Gameplay.h"

class GameObject {

public:
	GameObject(const char* texturesheet, SDL_Renderer* ren, int x, int y);
	~GameObject();

	void Update(int x, int y);
	void Render();


private:

	int xpos;
	int ypos;

	SDL_Texture* objTexture;
	SDL_Rect srcRect, destRect;
	SDL_Renderer* renderer;

};