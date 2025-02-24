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

    while (true) {
        try {
            std::cout << "Ingrese la direccion IP del oponente: ";
            std::cin >> ip;

            std::cout << "Ingrese el puerto: ";
            std::cin >> port;

            // Validar IP
            if (inet_pton(AF_INET, ip.c_str(), &opponent.sin_addr) != 1) {
                throw std::invalid_argument("Direccion IP no valida.");
            }

            // Validar puerto
            if (port < 1024 || port > 65535) {
                throw std::out_of_range("El puerto debe estar entre 1024 y 65535.");
            }

            // Configurar sockaddr_in para UDP
            opponent.sin_family = AF_INET;
            opponent.sin_port = htons(port);

            // Crear socket UDP
            int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
            if (sock < 0) {
                throw std::runtime_error("No se pudo crear el socket UDP.");
            }

            std::cout << "Enviando mensaje de prueba al servidor en " << ip << ":" << port << "..." << std::endl;

            std::string message = "Hola servidor";
            int bytes_sent = sendto(sock, message.c_str(), message.size(), 0,
                                    (struct sockaddr*)&opponent, sizeof(opponent));

            if (bytes_sent < 0) {
                CLOSESOCKET(sock);
                throw std::runtime_error("Error al enviar mensaje al servidor.");
            }

            std::cout << "Mensaje enviado correctamente.\n";

            // Esperar respuesta del servidor (opcional)
            char buffer[1024];
            sockaddr_in serverAddr{};
            socklen_t serverLen = sizeof(serverAddr);

            int bytes_received = recvfrom(sock, buffer, sizeof(buffer) - 1, 0,
                                          (struct sockaddr*)&serverAddr, &serverLen);

            if (bytes_received > 0) {
                buffer[bytes_received] = '\0';
                std::cout << "Respuesta del servidor: " << buffer << std::endl;
            } else {
                std::cerr << "No se recibió respuesta del servidor." << std::endl;
            }

            CLOSESOCKET(sock);

            std::thread thread(&NetworkManager::CommunicationLoop, m_nM.get());
            thread.detach();




            break;  // Salimos del bucle al terminar

        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << ". Intentelo de nuevo.\n";
        }
    }

    while (true) {

    }
}

void LobbyManager::Host() {

    std::thread thread(&NetworkManager::CommunicationLoop, m_nM.get());
    thread.detach();

    while (true) {

    }
}

void LobbyManager::Update(PacketHeader packet_header) {
    std::cout << "Me llego algo\n";
}
