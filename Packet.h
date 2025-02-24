//
// Created by Cuent on 2/23/2025.
//

#ifndef PACKET_H
#define PACKET_H
#include <cstdint>

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

};
#endif //PACKET_H
