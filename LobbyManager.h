//
// Created by Cuent on 2/23/2025.
//

#ifndef LOBBYMANAGER_H
#define LOBBYMANAGER_H
#include "NetworkManager.h"
#include "ObserverPattern.h"


class LobbyManager : public Observer, std::enable_shared_from_this<LobbyManager> {
private:
    std::shared_ptr<NetworkManager> m_nM;
public:
    LobbyManager(std::shared_ptr<NetworkManager> network_manager);
    void Join();
    void Host();
    void Update(PacketHeader packet_header) override;
};



#endif //LOBBYMANAGER_H
