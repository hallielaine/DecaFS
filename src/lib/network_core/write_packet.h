#ifndef _WRITE_PACKET_H_
#define _WRITE_PACKET_H_

#include "network_packets.h"

class WritePacket : public FileDataPacket {

  protected:
    virtual std::ostream& print(std::ostream&) const;

  public:
    WritePacket(void* buf, ssize_t size);
    WritePacket(uint32_t fd, uint32_t count, uint8_t* buf);

    friend std::ostream& operator<<(std::ostream& stream, const WritePacket &req);
};

#endif // _WRITE_REQUEST_H_
