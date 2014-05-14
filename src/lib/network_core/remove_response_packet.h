#ifndef _REMOVE_RESPONSE_PACKET_H_
#define _REMOVE_RESPONSE_PACKET_H_

#include "network_packets.h"

class RemoveResponsePacket : public Packet {

  private:
    static const int data_size = sizeof(int32_t);

  protected:
    virtual std::ostream& print(std::ostream&) const;

  public:
    int32_t result;

    RemoveResponsePacket(void*buf, ssize_t size);
    RemoveResponsePacket(int result);

    friend std::ostream& operator<<(std::ostream& stream, const RemoveResponsePacket &packet);
};

#endif // _REMOVE_RESPONSE_PACKET_H_
