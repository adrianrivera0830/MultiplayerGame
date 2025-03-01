//
// Created by Cuent on 2/23/2025.
//

#include "LobbyManager.h"
#include <limits> // Para limpiar std::cin
#include <thread>

LobbyManager::LobbyManager(std::shared_ptr<NetworkManager> network_manager) {
    if (!network_manager) {
        throw std::runtime_error("Referencia invalida a NetworkManager");
    }
    m_nM = network_manager;





}

void LobbyManager::Init(std::shared_ptr<LobbyManager> self) {
    m_nM->Attach(self);

}

void LobbyManager::Join() {
    sockaddr_in opponent;
    std::string ip;
    int port;




    std::cout << "Ingrese la direccion IP del oponente: ";
    std::cin >> ip;

    std::cout << "Ingrese el puerto: ";
    std::cin >> port;

    if (inet_pton(AF_INET, ip.c_str(), &opponent.sin_addr) != 1) {
        throw std::invalid_argument("Direccion IP no valida.");
    }
    opponent.sin_family = AF_INET;
    opponent.sin_port = htons(port);
    m_nM->SetOpponent(opponent);

    std::thread thread(&NetworkManager::CommunicationLoop, m_nM.get());
    thread.detach();

    PacketHeader connect_header;
    connect_header.SetPacketID(PacketIDToInt(PacketID::CONNECT));

    m_nM->AddPacketToSend(connect_header);

    game_manager = std::make_shared<GameManager>(m_nM);
    game_manager->Init(game_manager);
    while (!game_manager->GetGameStarted()) {

    }

    game_manager->StartGame();
}

void LobbyManager::Host() {
    std::thread thread(&NetworkManager::CommunicationLoop, m_nM.get());
    thread.detach();

    std::cout << "Hosteando!" << std::endl;
    std::cout << "Esperando jugador...\n" << std::endl;

    PacketHeader startGameHeader;
    startGameHeader.SetPacketID(PacketIDToInt(PacketID::START_GAME));

    game_manager = std::make_shared<GameManager>(m_nM);
    game_manager->Init(game_manager);

    while (true) {
        if (m_nM->IsOpponentConnected()) {
            int option;
            std::cout << "Ingresa 1 para iniciar el juego o 0 para abandonar: ";
            std::cin >> option;

            if (option == 1) {
                m_nM->AddPacketToSend(startGameHeader);
                std::cout << "Empezando juego\n";
                game_manager->SetGameState(true);


                break;  // Salir del loop para continuar con la lógica del juego
            } else if (option == 0) {
                std::cout << "Saliendo del juego...\n";
                exit(0); // Terminar el programa
            } else {
                std::cout << "Opción inválida. Inténtalo de nuevo.\n";
            }
        }
    }


    game_manager->StartGame();


}

void LobbyManager::Update(PacketHeader packet_header) {
    if (IntToID(packet_header.GetPacketID()) == PacketID::START_GAME) {
        std::cout << "Empezando juego\n";
        game_manager->SetGameState(true);

    }
    if (IntToID(packet_header.GetPacketID()) == PacketID::CONNECT) {
        std::cout << "Jugador conectado.\n";

    }
}
