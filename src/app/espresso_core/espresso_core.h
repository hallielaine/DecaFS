#ifndef __ESPRESSO_CORE_H__
#define __ESPRESSO_CORE_H__

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "network_core/network.h"
#include "network_core/esp_svc.h"
#include "espresso_storage/espresso_state.h"
#include "espresso_storage/espresso_storage.h"

#define NUM_EXPECTED_ARGS 4
#define NAME 1
#define METADATA 2
#define FILESYSTEM 3
#define NODE_STORAGE_SIZE 1000000000

void handle_cmd_args(int argc, char** argv);
void load_persistent_metadata(char* metadata_path);

/*
 *	Read data from a chunk at a specific offset.
 */
ssize_t read_data (int fd, int file_id, int stripe_id, int chunk_num, int offset, void *buf, int count);

/*
 *	Write data to a chunk at a specific offset.
 */
ssize_t write_data (int fd, int file_id, int stripe_id, int chunk_num, int offset, void *buf, int count);

/*
 *	Delete a chunk with given file_id and stripe_id.
 */
int delete_data (int fd, int file_id, int stripe_id, int chunk_num);

#endif
