//
// Created by Cuent on 2/23/2025.
//

#include "NetworkManager.h"

NetworkManager::NetworkManager() {
    try {
        m_player = std::make_unique<Peer>();  // Creación dinámica
    } catch (const std::exception& e) {
        std::cerr << "Error al crear Peer: " << e.what() << std::endl;
    }
}
