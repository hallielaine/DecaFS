#include "delete_file_packet.h"

DeleteFilePacket::DeleteFilePacket(void* buf, ssize_t size) : Packet(buf, size) {

  pathname = ((char*)buf) + Packet::dataSize();
}

DeleteFilePacket::DeleteFilePacket(char* path)
 : Packet(0, DELETE_FILE, strlen(path) + 1) {

  pathname = ((char*)packet) + Packet::dataSize();
  strncpy(pathname, path, strlen(path)+1);
}

int DeleteFilePacket::dataSize() {

  return Packet::dataSize() + strlen(pathname) + 1;
}

std::ostream& DeleteFilePacket::print(std::ostream& stream) const {
  stream << "DeleteFilePacket" << std::endl;
  stream << "pathname: ";
  stream.write((const char*)pathname, strlen(pathname));
  stream << std::endl;

  return Packet::print(stream);
}

std::ostream& operator<<(std::ostream& stream, const DeleteFilePacket &packet) {
  return packet.print(stream);
}

