#ifndef _OPENDIR_RESPONSE_PACKET_H_
#define _OPENDIR_RESPONSE_PACKET_H_

#include "network_packets.h"

class OpendirResponsePacket : public Packet {

  private:

  protected:
    virtual std::ostream& print(std::ostream&) const;

  public:
    OpendirResponsePacket(int flags, char* pathname);
    OpendirResponsePacket(void* buf, ssize_t length);

    friend std::ostream& operator<<(std::ostream& stream, const OpendirResponsePacket &packet);
};

#endif // _OPENDIR_RESPONSE_PACKET_H_
