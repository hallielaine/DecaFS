#ifndef _FILE_STORAGE_STAT_PACKET_H_
#define _FILE_STORAGE_STAT_PACKET_H_

#include "network_packets.h"

class FileStorageStatPacket : public Packet {

  private:

  protected:
    virtual std::ostream& print(std::ostream&) const;

  public:
    FileStorageStatPacket(const char* pathname);
    FileStorageStatPacket(void* buf, ssize_t length);

    char* filename;

    friend std::ostream& operator<<(std::ostream& stream, const FileStorageStatPacket &packet);
};

#endif // _FILE_STORAGE_STAT_PACKET_H_
