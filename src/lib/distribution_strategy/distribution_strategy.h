#ifndef __DISTRIBUTION_STRATEGY_H__
#define __DISTRIBUTION_STRATEGY_H__

#include <stdio.h>

#include "decafs_types/ip_address.h"
#include "volatile_metadata/volatile_metadata_c_api.h"

#define CANNOT_PUT_CHUNK -1

#define FIRST 1
#define SECOND 2

/*
 *	Determine which node a given chunk from a stripe should be sent to.
 */
extern "C" int put_chunk (uint32_t file_id, char *pathname, uint32_t stripe_id,
                          uint32_t chunk_num);

#endif
