#ifndef _LSEEK_RESPONSE_PACKET_H_
#define _LSEEK_RESPONSE_PACKET_H_

#include "network_packets.h"

class LseekResponsePacket : public Packet {

  private:
    static const int data_size = sizeof(int32_t);

  protected:
    virtual std::ostream& print(std::ostream&) const;

  public:
    int32_t result;

    LseekResponsePacket(void*buf, ssize_t size);
    LseekResponsePacket(int result);

    friend std::ostream& operator<<(std::ostream& stream, const LseekResponsePacket &packet);
};

#endif // _LSEEK_RESPONSE_PACKET_H_
