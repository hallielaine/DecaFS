#ifndef _NETWORK_CORE_H
#define _NETWORK_CORE_H

#include <stdlib.h>
#include <string.h>
#include "network.h"

#define MAX_CLIENTS 100

int network_add_client(char*);
ssize_t network_read_chunk(int fd, int file_id, int node_id, int stripe_id, int chunk_num, 
                           int offset, void* buf, int count);
ssize_t network_write_chunk(int fd, int file_id, int node_id, int stripe_id, int chunk_num, 
                           int offset, void* buf, int count);
int network_delete_chunk(int node_id, int file_id, int stripe_id, int chunk_num);

#endif
