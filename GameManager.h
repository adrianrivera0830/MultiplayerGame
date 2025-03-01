//
// Created by adrian on 2/28/25.
//

#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H
#include "NetworkManager.h"
#include "ObserverPattern.h"


class GameManager : public Observer {
private:
    bool game_started = false;
    std::shared_ptr<NetworkManager> m_nM;

public:
    void Init(std::shared_ptr<GameManager> self);

    GameManager(std::shared_ptr<NetworkManager> network_manager);




    void SetGameState(bool state);

    bool GetGameStarted() const;

    void StartGame();

    void Update(PacketHeader packet_header) override;
};



#endif //GAMEMANAGER_H
