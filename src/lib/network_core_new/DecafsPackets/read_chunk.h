#ifndef _READ_CHUNK_
#define _READ_CHUNK_

#include <stdlib.h>

#include "network_packet.h"
#include "packet_types.h"

class ReadChunk : public NetworkPacket {

   struct read_chunk_data {
      // information to file chunk
      int file_id;
      int stripe_id;
      int chunk_num;

      // informaiton within chunk
      int offset;
      int count;
   };

   private:
      static const int packet_type = READ_CHUNK;

   public:
      ReadChunk(int sequence_num, int file_id, int stripe_id, int chunk_num, int offset, int count);
};

#endif // _READ_CHUNK_
