//
// Created by Cuent on 2/23/2025.
//

#include "Packet.h"

PacketID IntToID(int value) {
    switch (value) {
        case static_cast<int>(PacketID::CONNECT):
            return PacketID::CONNECT;
        case static_cast<int>(PacketID::DISCONNECT):
            return PacketID::DISCONNECT;
        case static_cast<int>(PacketID::START_GAME):
            return PacketID::START_GAME;
        case static_cast<int>(PacketID::GOAL):
            return PacketID::GOAL;
        case static_cast<int>(PacketID::END_GAME):
            return PacketID::END_GAME;
        case static_cast<int>(PacketID::MOVE):
            return PacketID::MOVE;
        case static_cast<int>(PacketID::DEBUG):
            return PacketID::DEBUG;
        default:
            throw std::invalid_argument("Valor inválido para PacketID");
    }
}

int PacketIDToInt(PacketID id) {
    return static_cast<int>(id);
}

void PacketHeader::WriteFromStructToBuffer(Buffer &buffer) {
    WriteIntData(buffer,packet_id);
    WriteIntData(buffer,packet_sequence);
    WriteIntData(buffer,priority);
    WriteIntData(buffer,lastSequenceReceived);

    WriteIntData(buffer,ack_bitfield);
    WriteIntData(buffer,payload_size);

}

void PacketHeader::ReadFromBufferToStruct(Buffer &buffer) {
    ReadIntData(buffer,packet_id);
    ReadIntData(buffer,packet_sequence);
    ReadIntData(buffer,priority);
    ReadIntData(buffer,lastSequenceReceived);
    ReadIntData(buffer,ack_bitfield);
    ReadIntData(buffer,payload_size);


}
