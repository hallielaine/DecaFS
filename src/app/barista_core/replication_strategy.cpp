#include "replication_strategy.h"

extern "C" int put_chunk_replica (uint32_t file_id, char *pathname,
                                  uint32_t stripe_id, uint32_t chunk_num,
                                  uint32_t node_id) {
  return 0;
}

extern "C" int get_chunk_replica (uint32_t file_id, char *pathname,
                                  uint32_t stripe_id, uint32_t chunk_num,
                                  uint32_t node_id) {
  return 0;
}

