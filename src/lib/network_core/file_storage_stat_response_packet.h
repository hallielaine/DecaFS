#ifndef _FILE_STORAGE_STAT_RESPONSE_PACKET_H_
#define _FILE_STORAGE_STAT_RESPONSE_PACKET_H_

#include "network_packets.h"

class FileStorageStatResponsePacket : public Packet {

  private:

  protected:
    virtual std::ostream& print(std::ostream&) const;

  public:
    FileStorageStatResponsePacket(const char* data);
    FileStorageStatResponsePacket(void* buf, ssize_t length);

    char* result;

    friend std::ostream& operator<<(std::ostream& stream, const FileStorageStatResponsePacket &packet);
};

#endif // _FILE_STORAGE_STAT_RESPONSE_PACKET_H_
