#ifndef _REMOVE_PACKET_H_
#define _REMOVE_PACKET_H_

#include "network_packets.h"

class RemovePacket : public Packet {

  private:

  protected:
    virtual std::ostream& print(std::ostream&) const;

  public:
    RemovePacket(const char* pathname);
    RemovePacket(void* buf, ssize_t length);

    char* filepath;

    friend std::ostream& operator<<(std::ostream& stream, const RemovePacket &packet);
};

#endif // _REMOVE_PACKET_H_
