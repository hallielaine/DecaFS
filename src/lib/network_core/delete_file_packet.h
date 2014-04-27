#ifndef _DELETE_FILE_PACKET_H_
#define _DELETE_FILE_PACKET_H_

#include "network_packets.h"

class DeleteFilePacket : public Packet {

  protected:
    virtual int dataSize();
    virtual std::ostream& print(std::ostream&) const;

  public:

    char* pathname;

    DeleteFilePacket(void* buf, ssize_t size);
    DeleteFilePacket(char* pathname);

    friend std::ostream& operator<<(std::ostream& stream, const DeleteFilePacket &packet);
};

#endif //  _DELETE_FILE_PACKET_H_
