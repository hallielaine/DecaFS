#ifndef _OPEN_PACKET_H_
#define _OPEN_PACKET_H_

#include "network_packets.h"

class OpenPacket : public Packet {

  private:

  protected:
    virtual std::ostream& print(std::ostream&) const;

  public:
    OpenPacket(int flags, char* pathname);
    OpenPacket(void* buf, ssize_t length);

    int32_t open_flags;
    char* filepath;

    friend std::ostream& operator<<(std::ostream& stream, const OpenPacket &packet);
};

#endif // _OPEN_PACKET_H_
