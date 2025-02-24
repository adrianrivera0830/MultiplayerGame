//
// Created by Cuent on 2/23/2025.
//

#ifndef LOBBYMANAGER_H
#define LOBBYMANAGER_H
#include "NetworkManager.h"
#include "ObserverPattern.h"


class LobbyManager : public Observer{
private:
    std::shared_ptr<NetworkManager> m_nM;
public:
    LobbyManager(std::shared_ptr<NetworkManager> network_manager);
void Init(std::shared_ptr<LobbyManager> self);
    void Join();
    void Host();
    void Update(PacketHeader packet_header) override;
};



#endif //LOBBYMANAGER_H
