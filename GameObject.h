#pragma once
#include "Gameplay.h"

class GameObject {

public:
	GameObject(const char* texturesheet, SDL_Renderer* ren, int x, int y, int n);
	~GameObject();

	void Update(int x, int y, int X, int Y, int Width, int Height);
	void Render();


private:

	int xpos;
	int ypos;
	int n;
	SDL_Texture* objTexture;
	SDL_Rect srcRect, destRect;
	SDL_Renderer* renderer;

};