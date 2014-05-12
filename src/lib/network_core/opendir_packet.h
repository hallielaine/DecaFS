#ifndef _OPENDIR_PACKET_H_
#define _OPENDIR_PACKET_H_

#include "network_packets.h"

class OpendirPacket : public Packet {

  private:

  protected:
    virtual std::ostream& print(std::ostream&) const;

  public:
    OpendirPacket(const char* pathname);
    OpendirPacket(void* buf, ssize_t length);

    char* filepath;

    friend std::ostream& operator<<(std::ostream& stream, const OpendirPacket &packet);
};

#endif // _OPENDIR_PACKET_H_
