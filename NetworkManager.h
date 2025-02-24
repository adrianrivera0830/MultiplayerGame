//
// Created by Cuent on 2/23/2025.
//

#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <memory>

#include "NetworkHeaders.h"
#include "ObserverPattern.h"
#include "Peer.h"

class NetworkManager : public Subject{
private:
    void Send();
    void Receive();
private:
    std::unique_ptr<Peer> m_player;

    sockaddr_in m_opponent;
    socklen_t m_opponentLen;

public:
    NetworkManager();
    void SetOpponent(sockaddr_in opponent);

};



#endif //NETWORKMANAGER_H
