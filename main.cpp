#include <iostream>

#include "LobbyManager.h"
#include "NetworkManager.h"

int main(int argc, char* argv[]) {

    std::shared_ptr<NetworkManager> networkManager;
    std::shared_ptr<LobbyManager> lobby_manager;

    try {
        networkManager = std::make_shared<NetworkManager>();  // Creación dinámica
    } catch (const std::exception& e) {
        std::cerr << "Error al inicializar NetworkManager: " << e.what() << std::endl;
        return -1;
    }

    try {
        lobby_manager = std::make_shared<LobbyManager>(networkManager);  // Creación dinámica
        lobby_manager->Init(lobby_manager);
    } catch (const std::exception& e) {
        std::cerr << "Error al inicializar LobbyManager: " << e.what() << std::endl;
        return -1;
    }

    int opcion = -1;
    std::cout << "Seleccione una opcion: \n";
    std::cout << "0 - Hostear un lobby\n";
    std::cout << "1 - Unirse a un lobby\n";
    std::cout << "Ingrese su eleccion (0 o 1): ";
    std::cin >> opcion;

    if (opcion == 0) {
        lobby_manager->Host();  // Llamar al método Host
    } else if (opcion == 1) {
        lobby_manager->Join();  // Llamar al metodo Join
    } else {
        std::cerr << "Opción invalida. Saliendo del programa." << std::endl;
        return -1;
    }
    return 0;
}