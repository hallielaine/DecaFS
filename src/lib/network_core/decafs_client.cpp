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

// want spin / close 
// need to modify run loop in tcp server

int DecafsClient::open(const char* pathname, int flags) {

  char tmppath[256];
  strncpy(tmppath, pathname, 256);
  // create open packet
  OpenPacket op(flags, tmppath); 
  // send the packet
  // TODO need to check for errors
  sendToServer(op.packet, op.packet_size);

  // wait for open response packet
  //recv(
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
  return orp.response;
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
