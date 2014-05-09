#ifndef _ESPRESSO_PACKET_PROCESSOR_H_
#define _ESPRESSO_PACKET_PROCESSOR_H_

#include "network_packets.h"
#include "espresso_storage/espresso_storage.h"

Packet* process_read_packet(const ReadChunkRequest& request);
Packet* process_write_packet(const WriteChunkRequest& request);
Packet* process_delete_packet(const DeleteChunkRequest& request);

#endif // _ESPRESSO_PACKET_PROCESSOR_H_
