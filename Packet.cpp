//
// Created by Cuent on 2/23/2025.
//

#include "Packet.h"

void PacketHeader::WriteFromStructToBuffer(Buffer &buffer) {
    WriteIntData(buffer,packet_id);
    WriteIntData(buffer,packet_sequence);
    WriteIntData(buffer,priority);
    WriteIntData(buffer,ack_bitfield);
    WriteIntData(buffer,payload_size);

}

void PacketHeader::ReadFromBufferToStruct(Buffer &buffer) {
    ReadIntData(buffer,packet_id);
    ReadIntData(buffer,packet_sequence);
    ReadIntData(buffer,priority);
    ReadIntData(buffer,ack_bitfield);
    ReadIntData(buffer,payload_size);

}
