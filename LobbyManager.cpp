//
// Created by Cuent on 2/23/2025.
//

#include "LobbyManager.h"

LobbyManager::LobbyManager(std::shared_ptr<NetworkManager> network_manager) {
    if (!network_manager) {
        throw std::runtime_error("Referencia invalida a NetworkManager");
    }
    m_nM = network_manager;

    m_nM->Attach(shared_from_this());
}

void LobbyManager::Join() {
    //m_nM->SetOpponent()
}

void LobbyManager::Host() {
}

void LobbyManager::Update(PacketHeader packet_header) {
    std::cout << "Me llego algo\n";
}
