//
// Created by Cuent on 2/23/2025.
//

#include "Peer.h"

Peer::Peer() {
    try {
        if (!InitializeSocketLibrary()) {
            throw std::runtime_error("No se pudo inicializar con la librería de Winsock");
        }
        if (!InitializeSocket()) {
            throw std::runtime_error("No se pudo crear el socket");
        }
        if (!Bind()) {
            throw std::runtime_error("No se pudo hacer bind");
        }
    } catch (const std::exception& e) {
        std::cerr << "Error en Peer: " << e.what() << std::endl;
        CleanupSockets();  // Liberamos recursos antes de lanzar la excepción
        throw;  // Relanzamos la excepción para que el código externo la maneje
    }
}

bool Peer::InitializeSocket() {
    m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (!ISVALIDSOCKET(m_socket)) return false;
    if (!SetSocketNonBlocking(m_socket)) return false;

    return true;

}

bool Peer::Bind() {
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(0); // Puerto aleatorio

    if (bind(m_socket, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) == SOCKET_ERROR) return false;

    // Obtener IP y puerto al que se enlazó el socket
    sockaddr_in boundedAddress;
    socklen_t boundedLen = sizeof(boundedAddress);
    if (getsockname(m_socket, reinterpret_cast<sockaddr *>(&boundedAddress), &boundedLen) == -1) {
        std::cerr << "Error en getsockname\n";
        return false;
    }

    char ipStr[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &boundedAddress.sin_addr, ipStr, sizeof(ipStr));

    std::cout << "IP: " << ipStr << "\n";
    std::cout << "Puerto: " << ntohs(boundedAddress.sin_port) << "\n";

    return true;

}

Peer::~Peer() {
    CLOSESOCKET(m_socket);
    CleanupSockets();
}

int Peer::SendTo(char *buffer, int bufferLen, sockaddr *to, socklen_t toLen) {
    int bytes_sent = sendto(m_socket, buffer, bufferLen, 0, to, toLen);

    if (bytes_sent == SOCKET_ERROR) {
        int error = GETSOCKETERRNO();
        std::cerr << "Error en sendto(): " << error << std::endl;
        return -1; // Indica error
    }

    return bytes_sent;
}

int Peer::ReceiveFrom(char *buffer, int bufferLen, sockaddr *from, socklen_t *fromLen) {
    int bytes_received = recvfrom(m_socket, buffer, bufferLen, 0, from, fromLen);

    if (bytes_received == SOCKET_ERROR) {
        int error = GETSOCKETERRNO();

        if (error == WOULD_BLOCK) {
            // No hay datos disponibles en un socket no bloqueante, no es un error crítico

            return -2; // Indica que no hay datos disponibles (pero sigue conectado)
        }

        std::cerr << "Error en recvfrom(): " << error << std::endl;
        return -1; // Indica error general
    }

    if (bytes_received == 0) {
        std::cerr << "El socket remoto se ha cerrado\n";
        return 0; // Indica cierre de conexión
    }

    // Asegurar que la cadena terminada en null no sobrescriba datos válidos
    if (bytes_received > 0 && bytes_received < bufferLen) {
        buffer[bytes_received] = '\0';
    }

    return bytes_received;
}