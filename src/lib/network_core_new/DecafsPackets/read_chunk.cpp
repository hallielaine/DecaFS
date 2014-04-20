#include "read_chunk.h"

ReadChunk::ReadChunk(int sequence_num, int file_id, int stripe_id, int chunk_num, int offset, int count) : 
  NetworkPacket(sequence_num, packet_type, sizeof(read_chunk_data)) 
{

   read_chunk_data* data = (read_chunk_data*)malloc(sizeof(read_chunk_data));

   data->file_id = file_id;
   data->stripe_id = stripe_id;
   data->chunk_num = chunk_num;
   data->offset = offset;
   data->count = count;

   m_packet_data = (char*)data;
}
