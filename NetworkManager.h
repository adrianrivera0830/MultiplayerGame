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


private:

    std::unique_ptr<Peer> m_player;

    sockaddr_in m_opponent;
    socklen_t m_opponentLen;
    std::queue<PacketHeader> toSendPackets;

public:
    NetworkManager();
    void SetOpponent(sockaddr_in opponent);

    void CommunicationLoop();
    void AddPacketToSend(PacketHeader &header);

    bool IsOpponentConnected();
};



#endif //NETWORKMANAGER_H
