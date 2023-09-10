#include "GameObject.h"
#include "TextureManager.h"

GameObject::GameObject(const char* texturesheet, SDL_Renderer* ren, int x, int y, int length, int n)
{
	renderer = ren;
	objTexture = TextureManager::LoadTexture(texturesheet, ren);
	xpos = x;
	ypos = y;
	this->length = length;
	this->n = n;
}

void GameObject::Update(int x, int y)
{
	xpos = x;
	ypos = y;

	srcRect.h = 600/n;
	srcRect.w = 600/n;
	//srcRect.h = 512;
	//srcRect.w = 512;
	srcRect.x = 0;
	srcRect.y = 0;

	destRect.x = xpos;
	destRect.y = ypos;
	destRect.w = length;
	destRect.h = length;

}

void GameObject::Render()
{
	SDL_RenderCopy(renderer, objTexture, &srcRect, &destRect);
}