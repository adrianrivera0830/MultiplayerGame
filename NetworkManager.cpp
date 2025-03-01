//
// Created by Cuent on 2/23/2025.
//

#include "NetworkManager.h"
#include <bitset>


void NetworkManager::Send(char* buffer, int size) {
    int bytes_sent = m_player->SendTo(
    buffer,size,
    (sockaddr *)&m_opponent,
    m_opponentLen
    );

    if (bytes_sent < 0) return;

}

void NetworkManager::printPendingToAck() {
    std::cout << "\nin ack bitset: ";
    for (int elem : pendingToAckSenderPackets) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
}

void NetworkManager::printNotACKEDSentPacketsKeys() {
    std::cout << "\nSent packets still not acked: ";
    for (const auto& pair : sentNotACKEDPackets) {
        std::cout << pair.first << " ";
    }
    std::cout << std::endl;
}

void NetworkManager::SetPacketAckBitfield(PacketHeader &packet_header) {
    int mostRecentSeq = packet_header.GetMostRecentACK();
    if (mostRecentSeq == 0) {return;}


     for (auto it = pendingToAckSenderPackets.begin(); it != pendingToAckSenderPackets.end(); ) {
         if (*it == mostRecentSeq) {break;}
         int position =  (mostRecentSeq - 1) - *it;

         if (position >= BITFIELD_CAPACITY) {
             it = pendingToAckSenderPackets.erase(it);
         } else {
             packet_header.SetACKBit(position);
             ++it;  // Solo avanzar cuando no eliminamos
         }
     }

}

bool NetworkManager::IsPacketPendingAck(int sequence) {
    return sentNotACKEDPackets.find(sequence) != sentNotACKEDPackets.end();

}


void NetworkManager::Receive() {
    Buffer receiveBuffer(1024);

    sockaddr_in from;   // Crear estructura para almacenar la dirección del remitente
    socklen_t fromLen = sizeof(from); // Inicializar con el tamaño correcto

    int bytesReceived = m_player->ReceiveFrom((char *)receiveBuffer.m_buffer
        , receiveBuffer.m_size
        , (sockaddr *) &from
        , &fromLen
        );

    if (bytesReceived < 0) return;

    m_opponent = from;
    m_opponentLen = fromLen;

    PacketHeader received;
    received.ReadFromBufferToStruct(receiveBuffer);


    //
    // pendingToAckSenderPackets.push_back(received.GetPacketSequence());
    //
    // if (pendingToAckSenderPackets.size() > BITFIELD_CAPACITY) {
    //     pendingToAckSenderPackets.pop_front();    }
    SetPacket(received);
    //
    // if (received.GetPacketSequence() > opponentSeqNumber) {
    //     opponentSeqNumber = received.GetPacketSequence();
    // }

    //
    // if (sentNotACKEDPackets.find(received.GetMostRecentACK()) != sentNotACKEDPackets.end()) {
    //     sentNotACKEDPackets.erase(received.GetMostRecentACK());
    // }
    //
    //  for (uint8_t i = 0; i < 32; i++) {
    //      if (received.GetAckBitfield() & (1U << i)) { // Verifica si el bit en posición 'i' está encendido
    //          int pos = (received.GetMostRecentACK() - 1)- i;
    //          if (sentNotACKEDPackets.find(pos) != sentNotACKEDPackets.end()) {
    //              sentNotACKEDPackets.erase(pos);
    //          }
    //      }
    //  }

}



void NetworkManager::PreparePacket() {

    //Obtener el paquete mas reciente por enviar y eliminarlo de paquetes por enviar
    PacketHeader toSendHeader = toSendPackets.front();
    toSendPackets.pop();

    // //Asignamos la ultima secuencia que recibimos del oponente
    // toSendHeader.SetMostRecentACK(opponentSeqNumber);
    //
    // //Asignamos el bitfield en relacion a nuestro ultimo paquete de secuencia recibido
    // //(El primer bit seteado es  n-1) donde n es nuestra ultima secuencia recibida
    // SetPacketAckBitfield(toSendHeader);
    //
    //
    // //Guardamos el tiempo en el que se envio el paquete
    // auto timestamp = std::chrono::high_resolution_clock::now();
    //
    // //Guardamos el paquete que acabmos de enviar a paquetes esperando por confirmacion
    // sentNotACKEDPackets.insert_or_assign(toSendHeader.GetPacketSequence(),std::make_pair(toSendHeader,timestamp));


    Buffer write(1024);
    toSendHeader.WriteFromStructToBuffer(write);
    Send((char *)write.m_buffer,write.m_size);



}

void NetworkManager::CommunicationLoop() {
    while (true) {
        if (!toSendPackets.empty()) {
            PreparePacket();
        }

        Receive();

        //IMPORTANTE QUE EL METODO UpdateNotAckedTimers VAYA DESPUES DE RECEIVE
        //Receive borra paquetes de la lista de paquetes no ackeados, esto es importante para que no se iteren sobre ellos despues de borrarlos

        //UpdateNotAckedTimers();

    }

}

void NetworkManager::AddPacketToSend(PacketHeader& header) {
    header.SetPacketSequence(mySeqNumber);
    mySeqNumber++;

    toSendPackets.push(header);
}


bool IsSockaddrValid(const sockaddr_in &addr) {
    if (addr.sin_family != AF_INET) return false;
    if (addr.sin_addr.s_addr == INADDR_ANY) return false;
    if (ntohs(addr.sin_port) == 0) return false;

    return true;
}

bool NetworkManager::IsOpponentConnected() {
    if (!IsSockaddrValid(m_opponent)) {
        return false;
    }
    return true;
}

void NetworkManager::SetOpponent(sockaddr_in opponent) {
    if (!IsSockaddrValid(opponent)) {
        throw std::runtime_error("No es valido el sockaddr_in");
    }
    m_opponent = opponent;
    m_opponentLen = sizeof(m_opponent);
}

void NetworkManager::UpdateNotAckedTimers() {
    auto now = std::chrono::high_resolution_clock::now();

    for (auto it = sentNotACKEDPackets.begin(); it != sentNotACKEDPackets.end(); ) {
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - it->second.second).count();

        if (elapsed > 5000) { // Más de 1 segundo (1000ms)
            PacketHeader packet_header = it->second.first;
            std::cout << "Ha pasado más de 5s desde que se insertó el paquete con Seq Number: " << it->first << std::endl;

            it = sentNotACKEDPackets.erase(it); // Eliminar y avanzar el iterador correctamente
            if (packet_header.GetPriority() == 1) {
                AddPacketToSend(packet_header);
            }
        } else {
            it++;
        }
    }
}


NetworkManager::NetworkManager() {
    try {
        m_player = std::make_unique<Peer>();  // Creación dinámica
    } catch (const std::exception& e) {
        std::cerr << "Error al crear Peer: " << e.what() << std::endl;
        throw;
    }
}
