#ifndef __VOLATILE_METADATA_H__
#define __VOLATILE_METADATA_H__

#include <stdint.h>

#include "limits.h"

#define SIZE_ALREADY_SET -1
#define SIZE_INVALID -2

using namespace std;

class Volatile_Metadata {
  private:
    uint32_t chunk_size;
    uint32_t stripe_size;
    // TODO: add file cursors and node map

  public:
    Volatile_Metadata();
    std::map<string, int> ip_to_node_map;

    uint32_t get_chunk_size ();
    int set_chunk_size (uint32_t size);

    uint32_t get_stripe_size();
    int set_stripe_size (uint32_t size);

}; 
#endif
