#ifndef _BARISTA_NETWORK_HELPER_
#define _BARISTA_NETWORK_HELPER_

#include <stdio.h>

#include "decafs_types/file_types.h"

#include "net_tcp/tcp_server.h"
#include "packet_flags.h"
#include "network_packets.h"
#include "barista_server.h"
#include "open_packet_response.h"
#include "write_response_packet.h"
#include "read_response_packet.h"
#include "close_response_packet.h"
#include "lseek_response_packet.h"
#include "opendir_response_packet.h"
#include "file_storage_stat_response_packet.h"
#include "mkdir_response_packet.h"
#include "remove_response_packet.h"

// pulled from network_core module architecture
// modified to suit a non-blocking model

// sends a read chunk request to an espresso node
// returns -1 on error
int network_read_chunk(int32_t id, int fd, int file_id, int node_id, int stripe_id,
 int chunk_num, int offset, int count);

// sends a write chunk request to client
// returns -1 on error
int network_write_chunk(int32_t id, int fd, int file_id, int node_id, int stripe_id,
 int chunk_num, int offset, void* buf, int count);

// sends a delete chunk request to client
// returns -1 on error
int network_delete_chunk(int32_t id, int file_id, int node_id, int stripe_id, int chunk_num);

//int network_flush(ConnectionToClient* client);

int send_open_result(struct client c, int result);
int send_read_result(struct client c, int fd, ssize_t count, void* buf);
int send_write_result(struct client c, int fd, ssize_t count);
int send_close_result(struct client c, int result);
int send_remove_result(struct client c, int result);
int send_seek_result(struct client c, int result);
int send_opendir_result(struct client c, decafs_dir* dirp); 
int send_file_storage_stat_result(struct client c, const char* data);
int send_mkdir_result(struct client c, int result);

#endif // _BARISTA_NETWORK_HELPER_
