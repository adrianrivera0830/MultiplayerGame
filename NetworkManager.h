//
// Created by Cuent on 2/23/2025.
//

#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <chrono>
#include <list>
#include <memory>
#include <queue>
#include <unordered_map>
#include <unordered_set>

#include "NetworkHeaders.h"
#include "ObserverPattern.h"
#include "Peer.h"

class NetworkManager :  public Subject{
private:
    void Send(char *buffer, int size);
    void Receive();

    void PreparePacket();


    void printPendingToAck();

    // Función para imprimir solo las claves del unordered_map
    void printNotACKEDSentPacketsKeys();
    //
    void SetPacketAckBitfield(PacketHeader& packet_header) {
        for (auto it = pendingToAck.begin(); it != pendingToAck.end(); ) {
            int position =  opponentSeqNumber - *it;
            if (position >= BITFIELD_CAPACITY) {  // Eliminar números pares
                it = pendingToAck.erase(it);  // erase() devuelve el siguiente iterador válido
            } else {
                packet_header.SetACKBit(position);
                ++it;  // Solo avanzar cuando no eliminamos
            }
        }
    }

private:

    int mySeqNumber = 0;
    int opponentSeqNumber = 0;
    std::unique_ptr<Peer> m_player;

    sockaddr_in m_opponent;
    socklen_t m_opponentLen;

    std::queue<PacketHeader> toSendPackets;
    std::list<int> pendingToAck;
    std::unordered_map<int, std::pair
    <
        PacketHeader, std::chrono::time_point<std::chrono::high_resolution_clock>>
    > notACKEDSentPackets;


public:
    NetworkManager();
    void SetOpponent(sockaddr_in opponent);
    void CommunicationLoop();
    void AddPacketToSend(PacketHeader header);

    bool IsOpponentConnected();
};



#endif //NETWORKMANAGER_H
