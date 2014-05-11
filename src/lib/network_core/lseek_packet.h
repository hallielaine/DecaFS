#ifndef _LSEEK_PACKET_H_
#define _LSEEK_PACKET_H_

#include "network_packets.h"

class LseekPacket : public Packet {

  private:
    static const int dataSize = 3*sizeof(int32_t);

  protected:
    virtual std::ostream& print(std::ostream&) const;

  public:
    LseekPacket(int32_t fd, int32_t offset, int32_t whence);
    LseekPacket(void* buf, ssize_t length);

    int32_t fd;
    int32_t offset;
    int32_t whence;

    friend std::ostream& operator<<(std::ostream& stream, const LseekPacket &packet);
};

#endif // _LSEEK_PACKET_H_
