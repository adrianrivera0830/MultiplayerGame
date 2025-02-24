//
// Created by Cuent on 2/23/2025.
//

#include "NetworkManager.h"

void NetworkManager::Send(char* buffer, int size) {
    int bytes_sent = m_player->SendTo(
    buffer,size,
    (sockaddr *)&m_opponent,
    m_opponentLen
    );

    if (bytes_sent < 0) return;

}

void NetworkManager::Receive() {
    Buffer receiveBuffer(1024);

    sockaddr_in from;   // Crear estructura para almacenar la dirección del remitente
    socklen_t fromLen = sizeof(from); // Inicializar con el tamaño correcto

    int bytesReceived = m_player->ReceiveFrom((char *)receiveBuffer.m_buffer
        , receiveBuffer.m_size
        , (sockaddr *) &from
        , &fromLen
        );

    if (bytesReceived < 0) return;

    m_opponent = from;
    m_opponentLen = fromLen;

    PacketHeader received;
    received.ReadFromBufferToStruct(receiveBuffer);

    SetPacket(received);

}

void NetworkManager::CommunicationLoop() {
    while (true) {
        if (!toSendPackets.empty()) {
            PacketHeader toSendHeader = toSendPackets.front();
            toSendPackets.pop();

            Buffer write(1024);
            toSendHeader.WriteFromStructToBuffer(write);
            Send((char *)write.m_buffer,write.m_size);
        }

        Receive();
    }
}

void NetworkManager::AddPacketToSend(PacketHeader header) {
    toSendPackets.push(header);
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
