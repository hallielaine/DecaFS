#include "decafs_types/ip_address.h"
#include "decafs_types/file_types.h"
#include "decafs_types/limits.h"

#include "network_core/network_packets.h"
#include "barista_core/barista_core.h"

#include <string>
#include <sstream>
#include <iostream>
#include <map>
#include <string.h>
#include <stdio.h>

void set_chunk_size(int csize);
void set_stripe_size(int ssize);

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

