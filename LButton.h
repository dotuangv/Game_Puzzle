#pragma once
#ifndef  LBUTTON_H_
#define LBUTTON_H_
#include "LTexture.h"

class LButton {
public:
	//Initialize internal variable
	LButton();

	//Initialize Button with Texture
	LButton(const LTexture &Texture);

	//Sets Top Left Position
	void SetPosition(int x, int y);

	//Set Texture 
	void SetTexture(const LTexture& Texture) {
		mTexture = Texture;
	}

	//Handle Event 
	void HandleEvent(SDL_Event* e);

	//Shows button sprites
	void render(LTexture& gButtonSpriteSheetTexture, SDL_Rect gSpriteClips[]);
	LButtonSprite getCurrentSprite();
private:
	// Top left Position of Button
	SDL_Point mPosition;

	//Size of Button
	int ButtonWidth;
	int ButtonHeight;

	//Currently Used global sprite
	LButtonSprite mCurrentSprite;

	//Button
	LTexture mTexture;
};

#endif // ! LBUTTON_H_
