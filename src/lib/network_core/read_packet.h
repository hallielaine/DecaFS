#ifndef READ_PACKET
#define READ_PACKET

#include "network_packets.h"

class ReadRequest : public FilePacket {

  protected: 
    virtual std::ostream& print(std::ostream&) const;

  public:
    ReadRequest(void* buf, ssize_t size);
    ReadRequest(uint32_t fd, uint32_t count);

    friend std::ostream& operator<<(std::ostream& stream, const ReadRequest &req);
};

#endif // READ_PACKET
