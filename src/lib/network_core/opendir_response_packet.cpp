#include "opendir_response_packet.h"

OpendirResponsePacket::OpendirResponsePacket(const decafs_dir* dirents) 
 : Packet(0, OPENDIR_RESPONSE, sizeof(decafs_dir) + dirents->total*sizeof(decafs_dirent)) {

  char* front = (char*)packet;
  dirp = (decafs_dir*)(&(((char*)packet)[Packet::dataSize()]));
  //dirp = (decafs_dir*)(((uint8_t*)packet) + Packet::dataSize());
  dirp->current = dirents->current;
  dirp->total = dirents->total;
  //dirp->entries = (decafs_dirent*)(((uint8_t*)packet) + Packet::dataSize() + sizeof(decafs_dirent));
  dirp->entries = (decafs_dirent*)(&dirp[1]);//(decafs_dirent*)(&(((char*)packet)[Packet::dataSize() + sizeof(decafs_dirent)]));
  memcpy(dirp->entries, dirents->entries, dirp->total*sizeof(decafs_dirent));

  std::cout << "before serialization" << std::endl;
  //std::cout << *this << std::endl;
}

OpendirResponsePacket::OpendirResponsePacket(void* buf, ssize_t length) : Packet(buf, length) {

  dirp = (decafs_dir*)(&((char*)packet)[Packet::dataSize()]);
  //dirp->entries = (decafs_dirent*)(((char*)(&dirp->entries)) + 4);//(decafs_dirent*)(&((char*)packet)[Packet::dataSize() + sizeof(decafs_dir)]);
  dirp->entries = (decafs_dirent*)(&dirp[1]);//(decafs_dirent*)(((uint8_t*)packet) + Packet::dataSize() + sizeof(decafs_dirent));
}

std::ostream& OpendirResponsePacket::print(std::ostream &stream) const {

  std::cout << "ptrs" << std::endl;
  std::cout << " packet: " << packet << std::endl; 
  std::cout << "   dirp: " << dirp << std::endl; 
  std::cout << "entries: " << dirp->entries << std::endl; 

  stream << "OpendirResponsePacket" << std::endl;
  stream << "\tcurrent: " << dirp->current << std::endl;
  stream << "\ttotal: " << dirp->total << std::endl;

  for (int i = 0; i < dirp->total; i++) {
    stream << "\t\tdirent: " << i << std::endl;
    stream << "\t\tfile_id: " << dirp->entries[i].file_id << std::endl;
    stream << "\t\td_type: " << dirp->entries[i].d_type << std::endl;
    stream << "\t\td_name: ";
    stream.write(dirp->entries[i].d_name, strlen(dirp->entries[i].d_name));
    stream << std::endl;
  }
  return Packet::print(stream);
}

std::ostream& operator <<(std::ostream& stream, const OpendirResponsePacket &packet) {
  return packet.print(stream);
}
