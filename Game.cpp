//
// Created by Cuent on 2/28/2025.
//

#include "Game.h"

void Game::Init() {
    // Inicializar SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        throw std::runtime_error("Error al inicializar SDL: " + std::string(SDL_GetError()));
    }

    // Inicializar SDL_image para manejar imágenes PNG
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        SDL_Quit();
        throw std::runtime_error("Error al inicializar SDL_image: " + std::string(IMG_GetError()));
    }

    // Inicializar SDL_ttf para manejo de fuentes
    if (TTF_Init() == -1) {
        IMG_Quit();
        SDL_Quit();
        throw std::runtime_error("Error al inicializar SDL_ttf: " + std::string(TTF_GetError()));
    }

    // Crear una ventana
    window = SDL_CreateWindow("Ventana SDL2",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              800, 600, SDL_WINDOW_SHOWN);
    if (!window) {
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        throw std::runtime_error("Error al crear la ventana: " + std::string(SDL_GetError()));
    }

    // Crear el renderizador
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_DestroyWindow(window);
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        throw std::runtime_error("Error al crear el renderizador: " + std::string(SDL_GetError()));
    }
}
#define SQUARE_SIZE 20
void Game::Clean() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

    void  Game::Update() {
        Uint32 lastTime = SDL_GetTicks(); // Tiempo del último frame
        running = true;

        SDL_Event event;
        int mouseX, mouseY;

        while (running) {
            Uint32 currentTime = SDL_GetTicks();
            deltaTime = (currentTime - lastTime) / 1000.0f; // Convertir a segundos
            lastTime = currentTime;

            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    running = false;
                }
            }

            // Obtener la posición del mouse
            SDL_GetMouseState(&mouseX, &mouseY);
            playerMallet->SetPos(mouseX,mouseY);
            // Definir el cuadrado en la posición del mouse
           // SDL_Rect square = { mouseX - SQUARE_SIZE / 2, mouseY - SQUARE_SIZE / 2, SQUARE_SIZE, SQUARE_SIZE };
            // Renderizar
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderClear(renderer);

            playerMallet->Render();
            opponentMallet->Render();
            SDL_RenderPresent(renderer);

            // Control de FPS
            Uint32 frameTime = SDL_GetTicks() - currentTime;
            if (frameTime < FRAME_DELAY) {
                SDL_Delay(FRAME_DELAY - frameTime);
            }
        }
    }

Game::Game() {
    try {
        Init();
        playerMallet = new Mallet(renderer,50,1);
        opponentMallet = new Mallet(renderer,50,1);

    } catch (const std::exception& e) {
        std::cerr << "Error al iniciar el juego: " << e.what() << std::endl;
        running = false; // Marcar el juego como inactivo si hay un error
        throw;
    }
}

Game::~Game() {
    Clean();
}

void Game::StartGame() {
    Update();
}

void Game::UpdateGameState(GameState game_state) {
    opponentMallet->SetPos(game_state.playerPos.x,game_state.playerPos.y);
}

GameState Game::GetGameState() {
    GameState game_state;
    game_state.playerPos = playerMallet->GetPosition();
    game_state.playerVel = playerMallet->GetVelocity();
}
