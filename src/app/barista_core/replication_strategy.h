#ifndef __REPLICATION_STRATEGY_H__
#define __REPLICATION_STRATEGY_H__

#include "volatile_metadata.h"

#define FIRST_REPLICA 3
#define SECOND_REPLICA 4

/*
 *	Determine which node a given chunk’s replica should be sent to.
 */
extern "C" int put_chunk_replica (uint32_t file_id, char *pathname,
                                  uint32_t stripe_id, uint32_t chunk_num,
                                  uint32_t node_id);

/*
 *	Determine which node a given stripe’s replica is stored on.
 */
extern "C" int get_chunk_replica (uint32_t file_id, char *pathname,
                                  uint32_t stripe_id, uint32_t chunk_num,
                                  uint32_t node_id);

#endif
