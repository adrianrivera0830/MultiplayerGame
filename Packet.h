//
// Created by Cuent on 2/23/2025.
//

#ifndef PACKET_H
#define PACKET_H
#include <cstdint>
#include <cstdint>
#include <cstdlib>  // Para exit()
#include <iostream>
#include "NetworkHeaders.h"

struct Buffer {
    uint8_t *m_buffer;
    int index;
    int m_size;

    Buffer(int size) : m_size(size), index(0) {
        m_buffer = new uint8_t[size]();
    }

    ~Buffer() {
        delete[] m_buffer;
    }

    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;

};

struct PacketHeader {
    uint16_t packet_id = 0;
    uint32_t packet_sequence = 0;
    uint8_t priority = 0;
    uint32_t ack_bitfield = 0;
    uint16_t payload_size = 0;

    void WriteFromStructToBuffer(Buffer &buffer);
    void ReadFromBufferToStruct(Buffer &buffer);
};


template<typename T>
void WriteIntData(Buffer& buffer, T data) {
    T networkData;

    if constexpr (sizeof(T) == 1) {
        // uint8_t no necesita conversión
        networkData = data;
    } else if constexpr (sizeof(T) == 2) {
        networkData = htons(data);
    } else if constexpr (sizeof(T) == 4) {
        networkData = htonl(data);
    } else if constexpr (sizeof(T) == 8) {
        // Implementación manual de htonll (no existe en todos los sistemas)
        uint32_t high = htonl(static_cast<uint32_t>(data >> 32));
        uint32_t low  = htonl(static_cast<uint32_t>(data & 0xFFFFFFFF));
        networkData = (static_cast<uint64_t>(high) << 32) | low;
    } else {
        static_assert(sizeof(T) <= 8, "Unsupported integer size");
    }

    // Copiar el valor convertido al buffer
    memcpy(buffer.m_buffer + buffer.index, &networkData, sizeof(T));

    // Actualizar índice
    buffer.index += sizeof(T);
}

template<typename T>
void ReadIntData(Buffer& buffer, T& data) {
    T networkData;

    // Leer datos desde el buffer
    memcpy(&networkData, buffer.m_buffer + buffer.index, sizeof(T));

    if constexpr (sizeof(T) == 1) {
        // uint8_t no necesita conversión
        data = networkData;
    } else if constexpr (sizeof(T) == 2) {
        data = ntohs(networkData);
    } else if constexpr (sizeof(T) == 4) {
        data = ntohl(networkData);
    } else if constexpr (sizeof(T) == 8) {
        // Implementación manual de ntohll (no existe en todos los sistemas)
        uint32_t high, low;
        memcpy(&high, reinterpret_cast<uint8_t*>(&networkData), 4);
        memcpy(&low, reinterpret_cast<uint8_t*>(&networkData) + 4, 4);
        high = ntohl(high);
        low = ntohl(low);
        data = (static_cast<uint64_t>(high) << 32) | low;
    } else {
        static_assert(sizeof(T) <= 8, "Unsupported integer size");
    }

    // Actualizar índice
    buffer.index += sizeof(T);
}

#endif //PACKET_H
