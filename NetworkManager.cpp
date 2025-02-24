//
// Created by Cuent on 2/23/2025.
//

#include "NetworkManager.h"

void NetworkManager::Send() {
}

void NetworkManager::Receive() {
}

bool IsSockaddrValid(const sockaddr_in &addr) {
    if (addr.sin_family != AF_INET) return false;
    if (addr.sin_addr.s_addr == INADDR_ANY) return false;
    if (ntohs(addr.sin_port) == 0) return false;

    return true;
}

void NetworkManager::SetOpponent(sockaddr_in opponent) {
    if (!IsSockaddrValid(opponent)) {
        throw std::runtime_error("No es valido el sockaddr_in");
    }
    m_opponent = opponent;
    m_opponentLen = sizeof(m_opponent);
}

NetworkManager::NetworkManager() {
    try {
        m_player = std::make_unique<Peer>();  // Creación dinámica
    } catch (const std::exception& e) {
        std::cerr << "Error al crear Peer: " << e.what() << std::endl;
        throw;
    }
}
