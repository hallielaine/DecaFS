#include "decafs_client.h"

DecafsClient::DecafsClient(std::string hostname, unsigned short port, uint32_t user_id) :
  TcpClient(hostname, port), user_id(user_id)
{
  
}

void DecafsClient::connectionClosed() {

  printf("DecafsClient: connection to Barista closed!\n");
}

void DecafsClient::connectionEstablished() {

  printf("DecafsClient: connection to Barista established!\n");
  DecafsClientInit init(user_id);
  sendToServer(init.packet, init.packet_size);
  printf("DecafsClient: DecafsClientInit has been sent!\n");
}

int DecafsClient::open(const char* pathname, int flags) {

  char tmppath[256];
  strncpy(tmppath, pathname, 256);
  OpenPacket op(flags, tmppath); 
  // TODO need to check for errors
  sendToServer(op.packet, op.packet_size);

  fd_set tmp_set;
  FD_ZERO(&tmp_set);
  FD_SET(m_socket_number, &tmp_set);
  if (select(m_socket_number+1, &tmp_set, NULL, NULL, NULL) < 0) {
    perror("select");
  }

  // recv the packet
  // check for length of packet
  int32_t packet_size; 
  if (recv(m_socket_number, (void*)&packet_size, sizeof(packet_size), MSG_PEEK) != sizeof(packet_size)) {

  }

  char* buffer = (char*) malloc(packet_size);
  // TODO check for errors
  recv(m_socket_number, buffer, packet_size, 0);
 
  int32_t flag = ((uint32_t*)buffer)[2];
  if (flag != OPEN_RESPONSE) {
    // error, should only receive response here
  }

  OpenPacketResponse orp(buffer, packet_size);
  std::cout << orp << std::endl;
  return orp.response;
}

off_t DecafsClient::lseek(int fd, off_t offset, int whence) {

  LseekPacket sp(fd, offset, whence);
  sendToServer(sp.packet, sp.packet_size);

  fd_set tmp_set;
  FD_ZERO(&tmp_set);
  FD_SET(m_socket_number, &tmp_set);
  if (select(m_socket_number+1, &tmp_set, NULL, NULL, NULL) < 0) {
    perror("select");
  }

  // recv the packet
  // check for length of packet
  int32_t packet_size; 
  if (recv(m_socket_number, (void*)&packet_size, sizeof(packet_size), MSG_PEEK) != sizeof(packet_size)) {

  }

  char* buffer = (char*) malloc(packet_size);
  // TODO check for errors
  recv(m_socket_number, buffer, packet_size, 0);
 
  int32_t flag = ((uint32_t*)buffer)[2];
  if (flag != LSEEK_RESPONSE) {
    // error, should only receive response here
  }

  LseekResponsePacket lrp(buffer, packet_size);
  std::cout << lrp << std::endl;
  return lrp.result;
}

int DecafsClient::close(int fd) {

  ClosePacket cp(fd); 
  // TODO need to check for errors
  sendToServer(cp.packet, cp.packet_size);

  fd_set tmp_set;
  FD_ZERO(&tmp_set);
  FD_SET(m_socket_number, &tmp_set);
  if (select(m_socket_number+1, &tmp_set, NULL, NULL, NULL) < 0) {
    perror("select");
  }

  // recv the packet
  // check for length of packet
  int32_t packet_size; 
  if (recv(m_socket_number, (void*)&packet_size, sizeof(packet_size), MSG_PEEK) != sizeof(packet_size)) {

  }

  char* buffer = (char*) malloc(packet_size);
  // TODO check for errors
  recv(m_socket_number, buffer, packet_size, 0);
 
  int32_t flag = ((uint32_t*)buffer)[2];
  if (flag != CLOSE_RESPONSE) {
    // error, should only receive response here
  }

  CloseResponsePacket crp(buffer, packet_size);
  std::cout << crp << std::endl;
  return crp.result;
}

ssize_t DecafsClient::write(int fd, void* buf, ssize_t count) {

  WritePacket wp(fd, count, (uint8_t*)buf);
  // TODO need to check for errors
  sendToServer(wp.packet, wp.packet_size);

  fd_set tmp_set;
  FD_ZERO(&tmp_set);
  FD_SET(m_socket_number, &tmp_set);
  if (select(m_socket_number+1, &tmp_set, NULL, NULL, NULL) < 0) {
    perror("select");
  }

  // recv the packet
  // check for length of packet
  int32_t packet_size; 
  if (recv(m_socket_number, (void*)&packet_size, sizeof(packet_size), MSG_PEEK) != sizeof(packet_size)) {

  }

  char* buffer = (char*) malloc(packet_size);
  // TODO check for errors
  recv(m_socket_number, buffer, packet_size, 0);
 
  int32_t flag = ((uint32_t*)buffer)[2];
  if (flag != WRITE_RESPONSE) {
    // error, should only receive response here
  }

  WriteResponsePacket wrp(buffer, packet_size);
  std::cout << wrp << std::endl;
  return wrp.count;
}

ssize_t DecafsClient::read(int fd, void* buf, ssize_t count) {

  ReadRequest rp(fd, count);
  // TODO need to check for errors
  sendToServer(rp.packet, rp.packet_size);

  fd_set tmp_set;
  FD_ZERO(&tmp_set);
  FD_SET(m_socket_number, &tmp_set);
  if (select(m_socket_number+1, &tmp_set, NULL, NULL, NULL) < 0) {
    perror("select");
  }

  // recv the packet
  // check for length of packet
  int32_t packet_size; 
  if (recv(m_socket_number, (void*)&packet_size, sizeof(packet_size), MSG_PEEK) != sizeof(packet_size)) {

  }

  char* buffer = (char*) malloc(packet_size);
  // TODO check for errors
  recv(m_socket_number, buffer, packet_size, 0);
 
  int32_t flag = ((uint32_t*)buffer)[2];
  if (flag != READ_RESPONSE) {
    // error, should only receive response here
  }

  std::cout << "DecafsClient: received ReadResponsePacket from Barista" << std::endl;
  ReadResponsePacket rrp(buffer, packet_size);

  // copy data to the users buffer
  if (rrp.count > 0) {
    memcpy(buf, rrp.data_buffer, rrp.count);
  }

  std::cout << rrp << std::endl;
  return rrp.count;
}

void DecafsClient::handleMessageFromServer(int socket) {

  printf("DecafsClient: message received from Barista!\n");

  uint32_t flag;
  int32_t packet_size;
  void* buffer_ptr;
  Packet *packet;

    // get the next packet size
    if (recv(socket, (void*)&packet_size, sizeof(packet_size), MSG_PEEK) <= 0) {
      // TODO error
    }

    buffer_ptr = (char*)malloc(packet_size);
    if (recv(socket, buffer_ptr, packet_size, 0) != packet_size) {
      // TODO error expected to read more bytes for a complete Packet message
    } 
   
    flag = ((uint32_t*)buffer_ptr)[2];

    switch (flag) {
      case (READ_CHUNK) : 
        {
          printf("\ngot a READ_CHUNK packet\n");
          ReadChunkRequest readRequest(buffer_ptr, packet_size);
          std::cout << readRequest << std::endl;
          packet = process_read_packet(readRequest);
          sendToServer(packet->packet, packet->packet_size);
        }
        break;
      case (WRITE_CHUNK) : 
        {
          printf("\ngot a WRITE_CHUNK packet\n");
          WriteChunkRequest writeRequest(buffer_ptr, packet_size);
          std::cout << writeRequest << std::endl;
          packet = process_write_packet(writeRequest);
          sendToServer(packet->packet, packet->packet_size); 
        }
        break;
      case (DELETE_CHUNK) :
        {
          printf("\ngot a DELETE_CHUNK packet\n");
          DeleteChunkRequest deleteRequest(buffer_ptr, packet_size);
          std::cout << deleteRequest << std::endl;
          packet = process_delete_packet(deleteRequest);
          sendToServer(packet->packet, packet->packet_size);
        }
    }
}
