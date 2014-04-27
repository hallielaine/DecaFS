#ifndef _CLOSE_PACKET_H_
#define _CLOSE_PACKET_H_

#include "network_packets.h"

class ClosePacket : public Packet {

  private:
    static const int data_size = sizeof(uint32_t);

  protected:
    virtual std::ostream& print(std::ostream&) const;

  public:
    uint32_t fd;

    ClosePacket(void*buf, ssize_t size);
    ClosePacket(int fd);

    friend std::ostream& operator<<(std::ostream& stream, const ClosePacket &packet);
};

#endif // _CLOSE_PACKET_H_
