#ifndef _OPENDIR_RESPONSE_PACKET_H_
#define _OPENDIR_RESPONSE_PACKET_H_

#include "network_packets.h"
#include "decafs_types/file_types.h"

class OpendirResponsePacket : public Packet {

  private:

  protected:
    virtual std::ostream& print(std::ostream&) const;

  public:
    decafs_dir* dirp;

    OpendirResponsePacket(const decafs_dir* dirp);
    OpendirResponsePacket(void* buf, ssize_t length);

    friend std::ostream& operator<<(std::ostream& stream, const OpendirResponsePacket &packet);
};

#endif // _OPENDIR_RESPONSE_PACKET_H_
