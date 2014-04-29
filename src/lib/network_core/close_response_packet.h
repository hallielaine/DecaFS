#ifndef _CLOSE_RESPONSE_PACKET_H_
#define _CLOSE_RESPONSE_PACKET_H_

#include "network_packets.h"

class CloseResponsePacket : public Packet {

  private:
    static const int data_size = sizeof(int32_t);

  protected:
    virtual std::ostream& print(std::ostream&) const;

  public:
    int32_t result;

    CloseResponsePacket(void*buf, ssize_t size);
    CloseResponsePacket(int result);

    friend std::ostream& operator<<(std::ostream& stream, const CloseResponsePacket &packet);
};

#endif // _CLOSE_RESPONSE_PACKET_H_
