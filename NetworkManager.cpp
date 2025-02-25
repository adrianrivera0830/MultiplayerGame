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

void NetworkManager::printPendingToAck() {
    std::cout << "Pending to ACK elements: ";
    for (int elem : pendingToAck) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
}

void NetworkManager::printNotACKEDSentPacketsKeys() {
    std::cout << "Not ACKED Sent Packets (keys): ";
    for (const auto& pair : notACKEDSentPackets) {
        std::cout << pair.first << " ";
    }
    std::cout << std::endl;
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

    pendingToAck.push_back(received.packet_sequence);

    if (pendingToAck.size() > BITFIELD_CAPACITY) {
        pendingToAck.pop_front();
    }
    SetPacket(received);

    if (received.packet_id > opponentSeqNumber) {
        opponentSeqNumber = received.packet_id;
    }

    for (uint8_t i = 0; i < 32; i++) {
        if (received.ack_bitfield & (1U << i)) { // Verifica si el bit en posición 'i' está encendido
            int ack = received.lastSequenceReceived - i;
            std::cout << "Se recibio el ack del paquete " << ack << std::endl;
        }
    }

    printPendingToAck();

}



void NetworkManager::PreparePacket() {
    PacketHeader toSendHeader = toSendPackets.front();
    toSendPackets.pop();
    toSendHeader.lastSequenceReceived = opponentSeqNumber;
    SetPacketAckBitfield(toSendHeader);
    if (toSendHeader.packet_sequence == 0) {
        toSendHeader.packet_sequence = mySeqNumber;
        mySeqNumber++;
    }

    Buffer write(1024);
    toSendHeader.WriteFromStructToBuffer(write);
    Send((char *)write.m_buffer,write.m_size);

    auto timestamp = std::chrono::high_resolution_clock::now();

    notACKEDSentPackets.insert_or_assign(toSendHeader.packet_sequence,std::make_pair(toSendHeader,timestamp));
    printNotACKEDSentPacketsKeys();
}

void NetworkManager::CommunicationLoop() {
    while (true) {
        if (!toSendPackets.empty()) {
            PreparePacket();
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

bool NetworkManager::IsOpponentConnected() {
    if (!IsSockaddrValid(m_opponent)) {
        return false;
    }
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
