#ifndef _BAR_CLNT_H
#define _BAR_CLNT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "network.h"

#define MAX_CLIENTS 100

#ifdef __cplusplus
extern "C" {
#endif

extern int network_add_client(char*);
extern ssize_t network_read_chunk(int fd, int file_id, int node_id, int stripe_id, int chunk_num,
                           int offset, void* buf, int count);
extern ssize_t network_write_chunk(int fd, int file_id, int node_id, int stripe_id, int chunk_num,
                           int offset, const void* buf, int count);
extern int network_delete_chunk(int fd, int file_id, int node_id, int stripe_id, int chunk_num);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // _BAR_CLNT_H
