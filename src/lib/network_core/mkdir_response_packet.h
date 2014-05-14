#ifndef _MKDIR_RESPONSE_PACKET_H_
#define _MKDIR_RESPONSE_PACKET_H_

#include "network_packets.h"

class MkdirResponsePacket : public Packet {

  private:
    static const int data_size = sizeof(int32_t);

  protected:
    virtual std::ostream& print(std::ostream&) const;

  public:
    int32_t result;

    MkdirResponsePacket(void*buf, ssize_t size);
    MkdirResponsePacket(int result);

    friend std::ostream& operator<<(std::ostream& stream, const MkdirResponsePacket &packet);
};

#endif // _MKDIR_RESPONSE_PACKET_H_
