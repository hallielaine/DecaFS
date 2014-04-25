#ifndef _WRITE_RESPONSE_PACKET_H_
#define _WRITE_RESPONSE_PACKET_H_

#include "network_packets.h"

class WriteResponsePacket : public FilePacket {

  protected: 
    virtual std::ostream& print(std::ostream&) const;

  public:
    WriteResponsePacket(void* buf, ssize_t size);
    WriteResponsePacket(uint32_t fd, uint32_t file_id, uint32_t stripe_id, 
     uint32_t chunk_num, uint32_t offset, uint32_t count);

    friend std::ostream& operator<<(std::ostream& stream, const WriteResponsePacket &res);
};

#endif // _WRITE_RESPONSE_PACKET_H_
