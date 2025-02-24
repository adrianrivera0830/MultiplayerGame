//
// Created by Cuent on 2/23/2025.
//

#ifndef PEER_H
#define PEER_H

#include "NetworkHeaders.h"

class Peer {
public:
    Peer();
    ~Peer();

    int SendTo(char *buffer, int bufferLen, sockaddr *to, socklen_t toLen);
    int ReceiveFrom(char *buffer, int bufferLen, sockaddr *from, socklen_t *fromLen);

private:
    SOCKET m_socket;
private:
    bool InitializeSocket();
    bool Bind();

};



#endif //PEER_H
