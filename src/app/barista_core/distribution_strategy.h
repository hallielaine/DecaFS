#ifndef __DISTRIBUTION_STRATEGY_H__
#define __DISTRIBUTION_STRATEGY_H__

#include "volatile_metadata.h"

/*
 *	Determine which node a given chunk from a stripe should be sent to.
 */
extern "C" int put_chunk (uint32_t file_id, char *pathname, uint32_t stripe_id,
                          uint32_t chunk_num);

/*
 *	Determine which node a given chunk is stored on.
 */
extern "C" int get_chunk (uint32_t file_id, char *pathname, uint32_t stripe_id,
                          uint32_t chunk_num);

#endif
