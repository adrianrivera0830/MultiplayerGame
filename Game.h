//
// Created by Cuent on 2/28/2025.
//

#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <iostream>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "Mallet.h"


class Game {
private:


#define EDGE_MARGIN 30;
#define GOALPOAST_START 320
#define GOALPOAST_LENGTH 160

    static constexpr int SCREEN_WIDTH = 800;
    static constexpr int SCREEN_HEIGHT = 600;
    static constexpr int FPS = 60;
    static constexpr int FRAME_DELAY = 1000 / FPS;


    float deltaTime = 0.0f;

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    bool running = false;

    Mallet* playerMallet;
    Mallet* opponentMallet;

private:
    void Init();
    void Clean();
    void Update();
public:
    Game();
    ~Game();
    void StartGame();
    void UpdateGameState(GameState game_state);
    GameState GetGameState();
};



#endif //GAME_H
