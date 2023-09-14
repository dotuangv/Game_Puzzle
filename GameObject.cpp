#include "GameObject.h"
#include "TextureManager.h"

GameObject::GameObject(const char* texturesheet, SDL_Renderer* ren, int x, int y, int n)
{
	renderer = ren;
	objTexture = TextureManager::LoadTexture(texturesheet, ren);
	xpos = x;
	ypos = y;
	this->n = n;
}

void GameObject::Update(int x, int y, int X, int Y, int Width, int Height)
{
	xpos = x;
	ypos = y;

	srcRect.h = X;
	srcRect.w = Y;
	//srcRect.h = 512;
	//srcRect.w = 512;
	srcRect.x = 0;
	srcRect.y = 0;

	destRect.x = xpos;
	destRect.y = ypos;
	destRect.w = Width;
	destRect.h = Height;

}

void GameObject::Render()
{
	SDL_RenderCopy(renderer, objTexture, &srcRect, &destRect);
}