#ifndef __ESPRESSO_CORE_H__
#define __ESPRESSO_CORE_H__

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "espresso_storage/espresso_state.h"
#include "espresso_storage/espresso_storage.h"
#include "network_core/espresso_client.h"

#define NUM_EXPECTED_ARGS 6
#define NODE_ID 1
#define METADATA 2
#define FILESYSTEM 3
#define BARISTA_HOSTNAME 4
#define BARISTA_PORT 5
#define NODE_STORAGE_SIZE 1000000000

void handle_cmd_args(int argc, char** argv);
void load_persistent_metadata(char* metadata_path);

#endif
