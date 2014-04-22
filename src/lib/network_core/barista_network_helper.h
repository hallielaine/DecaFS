#ifndef _BARISTA_NETWORK_HELPER_
#define _bARISTA_NETWORK_HELPER_

#include "net_tcp/tcp_server.h"
#include "packet_flags.h"
#include "network_packets.h"
#include "barista_server.h"

// pulled from network_core module architecture
// modified to suit a non-blocking model

// sends a read chunk request to client
// returns -1 on error
int network_read_chunk(int fd, int file_id, int node_id, int stripe_id, 
 int chunk_num, int offset, int count);

// sends a write chunk request to client
// returns -1 on error
int network_write_chunk(int fd, int file_id, int node_id, int stripe_id,
 int chunk_num, int offset, void* buf, int count);

// sends a delete chunk request to client
// returns -1 on error
int network_delete_chunk(int file_id, int node_id, int stripe_id, int chunk_num);

//int network_flush(ConnectionToClient* client);

#endif // _BARISTA_NETWORK_HELPER_
