#ifndef _NETWORK_PACKETS_H_
#define _NETWORK_PACKETS_H_

#include <stdint.h>
#include <stdlib.h>

#include "packet_flags.h"
#include "net_tcp/tcp_server.h"

class NetworkPacket {

  public:

    static const int header_size = 2*sizeof(uint32_t); // seq_num and flag
    int data_size;

    int packet_size;
    void *packet;

    uint32_t seq_num;
    uint32_t flag;

    NetworkPacket(uint32_t flag);
    NetworkPacket(void* buf, ssize_t count);

    void allocatePacket();
    virtual void printInfo() = 0;
};

class ReadChunkPacket : public NetworkPacket {

  public:

    ReadChunkPacket(void* buf, ssize_t packet_size);
    ReadChunkPacket(uint32_t fd, uint32_t file_id, uint32_t stripe_id, uint32_t chunk_num,
      uint32_t offset, uint32_t count);

    void printInfo();

    uint32_t fd;
    uint32_t file_id;
    uint32_t stripe_id;
    uint32_t chunk_num;
    uint32_t offset;
    uint32_t count;
};

#endif // _NETWORK_PACKETS_H_
