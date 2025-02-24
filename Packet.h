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

#endif //PACKET_H
