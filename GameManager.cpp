//
// Created by adrian on 2/28/25.
//

#include "GameManager.h"

#include <thread>


void GameManager::GameManagerLoop() {
    while (true) {
        std::cout << "hola\n";
    }
}

void GameManager::Init(std::shared_ptr<GameManager> self) {
    m_nM->Attach(self);
}

GameManager::GameManager(std::shared_ptr<NetworkManager> network_manager) {
    if (!network_manager) {
        throw std::runtime_error("Referencia invalida a NetworkManager");
    }
    m_nM = network_manager;
    SetGameState(false);
}

void GameManager::SetGameState(bool state) {
    game_started = state;
}

bool GameManager::GetGameStarted() const {
    return game_started;
}

void GameManager::StartGame() {
    SetGameState(true);
    std::thread thread(&Game::StartGame,&game);
    thread.detach();
    GameManagerLoop();
}



void GameManager::Update(PacketHeader packet_header) {
}
