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

enum class PacketID {
    CONNECT = 0,
    DISCONNECT = 1,
    START_GAME = 2,
    GOAL = 3,
    END_GAME = 4,
    MOVE = 5,
    DEBUG = 6,
    ACK = 7
};

PacketID IntToID(int value);

// Convierte un PacketID a int
int PacketIDToInt(PacketID id);

const int BITFIELD_CAPACITY = 32;
struct PacketHeader {
private:
    uint16_t packet_id = 0;
    uint32_t packet_sequence = 0;
    uint8_t priority = 0;
    uint32_t mostRecentACK = 0;
    //Si se cambia cambiar bitfield capacity
    uint32_t ack_bitfield = 0;
    uint16_t payload_size = 0;
public:
    void SetACKBit(int pos);
    void WriteFromStructToBuffer(Buffer &buffer);
    void ReadFromBufferToStruct(Buffer &buffer);

    uint16_t GetPacketID() const { return packet_id; }
    uint32_t GetPacketSequence() const { return packet_sequence; }
    uint8_t GetPriority() const { return priority; }
    uint32_t GetMostRecentACK() const { return mostRecentACK; }
    uint32_t GetAckBitfield() const { return ack_bitfield; }
    uint16_t GetPayloadSize() const { return payload_size; }

    // Setters (con validaciones básicas)
    void SetPacketID(uint16_t id) { packet_id = id; }
    void SetPacketSequence(uint32_t seq) { packet_sequence = seq; }
    void SetPriority(uint8_t prio) { priority = prio; }
    void SetMostRecentACK(uint32_t lastSeq) { mostRecentACK = lastSeq; }
    void SetPayloadSize(uint16_t size) { payload_size = size; }

};


inline void PacketHeader::SetACKBit(int pos) {
    ack_bitfield |= (1 << pos);
}

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
