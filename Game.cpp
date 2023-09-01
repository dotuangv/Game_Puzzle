////
//// Created by Tuan on 8/27/2023.
////

#include "Game.h"
#include "SDL_image.h"
#include "TextureManager.h"
#include "GameObject.h"
using namespace std;

Game::Game() {}

Game::~Game() {}

GameObject* Number1;
GameObject* Number2;
GameObject* Number3;

SDL_Texture *PlayerTex;
SDL_Rect srcR, destR;

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {
    int flags = 0;

    if (fullscreen) {
        flags = SDL_WINDOW_FULLSCREEN;
    }

    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        cout << "Subsystems Initialised!..." << endl;
        window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
        if (window) {
            cout << "Window created!" << endl;
        }

        renderer = SDL_CreateRenderer(window, -1, 0);
        if (renderer) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            cout << "Renderer created!" << endl;
        }

        isRunning = true;
    }
    else {
        isRunning = false;
    }

    Number1 = new GameObject("Data/Number1.png", renderer, 0, 0);
    Number2 = new GameObject("Data/Number2.png", renderer, 128, 0);
    Number3 = new GameObject("Data/Number3.png", renderer, 128, 128);
}

void Game::handleEvents() {
    SDL_Event event;
    SDL_PollEvent(&event);
    switch (event.type) {
    case SDL_QUIT:
        isRunning = false;
        break;
    default:
        break;
    }
}

void Game::update() {
    Number1->Update();
    Number2->Update();
    Number3->Update();
}

void Game::render() {
    SDL_RenderClear(renderer);
    Number1->Render();
    Number2->Render();
    Number3->Render();
    SDL_RenderPresent(renderer);
}

void Game::clean() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}
