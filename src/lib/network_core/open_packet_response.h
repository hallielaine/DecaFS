#ifndef _OPEN_PACKET_RESPONSE_H_
#define _OPEN_PACKET_RESPONSE_H_

#include "network_packets.h"

class OpenPacketResponse : public Packet {

  private:
    static const int data_size = sizeof(uint32_t);

  protected:
    virtual std::ostream& print(std::ostream&) const;

  public:
    uint32_t response; 

    OpenPacketResponse(void*buf, ssize_t size);
    OpenPacketResponse(int response);

    friend std::ostream& operator<<(std::ostream& stream, const OpenPacketResponse &packet);
};

#endif // OPEN_PACKET_RESPONSE
