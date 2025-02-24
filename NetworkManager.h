//
// Created by Cuent on 2/23/2025.
//

#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <memory>

#include "NetworkHeaders.h"
#include "Peer.h"

class NetworkManager {
private:
    void Send();
    void Receive();
private:
    std::unique_ptr<Peer> m_player;
public:
    NetworkManager();
};



#endif //NETWORKMANAGER_H
