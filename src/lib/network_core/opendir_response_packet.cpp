#include "opendir_response_packet.h"

OpendirResponsePacket::OpendirResponsePacket(decafs_dir* dirents) 
 : Packet(0, OPENDIR_RESPONSE, 2*sizeof(int32_t) + dirents->total*sizeof(decafs_dirent)) {



  dirp = (decafs_dir*)(((uint8_t*)packet) + Packet::dataSize());
  dirp->current = dirents->current;
  dirp->total = dirents->total;
  memcpy(dirp->entries, dirents->entries, dirp->total*sizeof(decafs_dirent));
}

OpendirResponsePacket::OpendirResponsePacket(void* buf, ssize_t length) : Packet(buf, length) {

  dirp = (decafs_dir*)(&((char*)packet)[Packet::dataSize()]);
}

std::ostream& OpendirResponsePacket::print(std::ostream &stream) const {

  //stream << "OpendirResponsePacket" << std::endl;
  //stream << "\tfilepath: ";
  //stream.write(filepath, strlen(filepath));//strlen(filepath));
  //stream << std::endl;
  return Packet::print(stream);
}

std::ostream& operator <<(std::ostream& stream, const OpendirResponsePacket &packet) {
  return packet.print(stream);
}
