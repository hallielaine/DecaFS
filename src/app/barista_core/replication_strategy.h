#ifndef __REPLICATION_STRATEGY_H__
#define __REPLICATION_STRATEGY_H__

#include "../../lib/ip_address/ip_address.h"
#include "volatile_metadata.h"

#define FIRST_REPLICA 3
#define SECOND_REPLICA 4

#define CANNOT_PUT_REPLICA -1

/*
 *	Determine which node a given chunk’s replica should be sent to.
 */
extern "C" int put_replica (uint32_t file_id, char *pathname,
                                  uint32_t stripe_id, uint32_t chunk_num,
                                  uint32_t node_id);

#endif
