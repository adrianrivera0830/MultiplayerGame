// Created by Cuent on 2/23/2025.

#ifndef NETWORKHEADERS_H
#define NETWORKHEADERS_H

#include <iostream>
#include <cstring>

/// ==========================
/// 🔹 PLATAFORMA Y SOCKETS 🔹
/// ==========================

#if defined(_WIN32)  // Windows
    #ifndef _WIN32_WINNT
        #define _WIN32_WINNT 0x0601  // Windows Vista o superior
    #endif
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")  // Enlazar la librería Winsock automáticamente
#else  // Linux / MacOS (Unix-like)
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <netdb.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <errno.h>
#endif

/// ====================================
/// 🔹 DEFINICIONES MULTIPLATAFORMA 🔹
/// ====================================
#if defined(_WIN32)
    #define ISVALIDSOCKET(s) ((s) != INVALID_SOCKET)
    #define CLOSESOCKET(s) closesocket(s)
    #define GETSOCKETERRNO() (WSAGetLastError())
    typedef int socklen_t;  // Windows usa `int` en lugar de `socklen_t`
    #define WOULD_BLOCK WSAEWOULDBLOCK
    #define SLEEP(ms) Sleep(ms)  // Windows usa Sleep(ms)
#else
    #define ISVALIDSOCKET(s) ((s) >= 0)
    #define CLOSESOCKET(s) close(s)
    #define SOCKET int
    #define GETSOCKETERRNO() (errno)
    #define SOCKET_ERROR -1
    #define WOULD_BLOCK EWOULDBLOCK
    #define SLEEP(ms) usleep((ms) * 1000)  // Linux usa usleep(microsegundos)
#endif

/// ================================
/// 🔹 FUNCIONES DE INICIALIZACIÓN 🔹
/// ================================

/**
 * @brief Inicializa Winsock en Windows. No hace nada en Linux.
 * @return `true` si la inicialización fue exitosa, `false` en caso de error.
 */
inline bool InitializeSockets() {
#if defined(_WIN32)
    WSADATA wsaData;
    return WSAStartup(MAKEWORD(2, 2), &wsaData) == 0;
#else
    return true;  // En Linux no se necesita inicialización
#endif
}

/**
 * @brief Finaliza Winsock en Windows (no hace nada en Linux).
 */
inline void CleanupSockets() {
#if defined(_WIN32)
    WSACleanup();
#endif
}

/// =================================
/// 🔹 CONFIGURACIÓN DE SOCKETS 🔹
/// =================================

/**
 * @brief Configura un socket en modo no bloqueante.
 * @param socket El socket a modificar.
 * @return `true` si la operación fue exitosa, `false` en caso de error.
 */
inline bool SetSocketNonBlocking(SOCKET socket) {
#if defined(_WIN32)
    u_long mode = 1;
    return ioctlsocket(socket, FIONBIO, &mode) == 0;
#else
    int flags = fcntl(socket, F_GETFL, 0);
    if (flags == -1) {
        return false;
    }
    return fcntl(socket, F_SETFL, flags | O_NONBLOCK) != -1;
#endif
}

#endif // NETWORKHEADERS_H
