#include "file_storage_stat_response_packet.h"

FileStorageStatResponsePacket::FileStorageStatResponsePacket(const char* data) 
 : Packet(0, FILE_STORAGE_STAT_RESPONSE, strlen(data) + 1) {

  result = (char*)(((uint8_t*)packet) + Packet::dataSize());

  memcpy(result, data, strlen(data) + 1);
}

FileStorageStatResponsePacket::FileStorageStatResponsePacket(void* buf, ssize_t length) : Packet(buf, length) {

  result = &((char*)packet)[Packet::dataSize()];
}

std::ostream& FileStorageStatResponsePacket::print(std::ostream &stream) const {

  stream << "FileStorageStatResponsePacket" << std::endl;
  stream << "\tresult: ";
  stream.write(result, strlen(result));
  stream << std::endl;

  return Packet::print(stream);
}

std::ostream& operator <<(std::ostream& stream, const FileStorageStatResponsePacket &packet) {
  return packet.print(stream);
}
