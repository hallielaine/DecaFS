#ifndef __VOLATILE_METADATA_H__
#define __VOLATILE_METADATA_H__

#include <stdint.h>

using namespace std;

class Volatile_Metadata {
  private:
    uint32_t chunk_size;
    uint32_t stripe_size;
    // TODO: add file cursors and node map

  public:
    Volatile_Metadata();

    uint32_t get_chunk_size ();
    void set_chunk_size (uint32_t size);

    uint32_t get_stripe_size();
    void set_stripe_size (uint32_t size);
}; 
#endif
