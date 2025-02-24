#include <iostream>
#include "NetworkManager.h"
int main() {
    std::unique_ptr<NetworkManager> networkManager;

    try {
        networkManager = std::make_unique<NetworkManager>();  // Creación dinámica
    } catch (const std::exception& e) {
        std::cerr << "Error al inicializar NetworkManager: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}