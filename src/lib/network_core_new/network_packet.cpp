#include "network_packet.h"

NetworkPacket::NetworkPacket(int sequence_num) :
   NetworkPacket(sequence_num, -1, -1)
{
}

NetworkPacket::NetworkPacket(int sequence_num, int packet_type) :
   NetworkPacket(sequence_num, packet_type, -1)
{
}

NetworkPacket::NetworkPacket(int sequence_num, int packet_type, int size) :
   m_sequence_num(sequence_num),
   m_size(size),
   m_packet_type(packet_type)
{
}

void NetworkPacket::send_packet(int client) {
}
