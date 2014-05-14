#ifndef _MKDIR_PACKET_H_
#define _MKDIR_PACKET_H_

#include "network_packets.h"

class MkdirPacket : public Packet {

  private:
    const static int dataSize = sizeof(mode_t);

  protected:
    virtual std::ostream& print(std::ostream&) const;

  public:
    MkdirPacket(const char* pathname, mode_t mode);
    MkdirPacket(void* buf, ssize_t length);

    mode_t mode;
    char* filepath;

    friend std::ostream& operator<<(std::ostream& stream, const MkdirPacket &packet);
};

#endif // _MKDIR_PACKET_H_
