#pragma once
#pragma once
#ifndef LTEXTURE_H
#define LTEXTURE_H

#include "CommonFunc.h"

class LTexture {
public:
    LTexture();
    ~LTexture();

    bool loadFromFile(std::string path);
    void free();
    void SetColor(Uint8 red, Uint8 green, Uint8 blue);
    void render(int x, int y, SDL_Rect* clip = NULL);
    int getWidth();
    int getHeight();

private:
    SDL_Texture* mTexture;
    int mWidth;
    int mHeight;
};

#endif // LTEXTURE_H
