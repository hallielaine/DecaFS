#ifndef _READ_RESPONSE_PACKET_H_
#define _READ_RESPONSE_PACKET_H_

#include "network_packets.h"

class ReadResponsePacket : public FileDataPacket {

  protected:
    virtual std::ostream& print(std::ostream&) const;

  public:
    ReadResponsePacket(void* buf, ssize_t size);
    ReadResponsePacket(uint32_t fd, uint32_t count, uint8_t* buf);

    friend std::ostream& operator<<(std::ostream& stream, const ReadResponsePacket &req);
};

#endif // _READ_RESPONSE_PACKET_H_
