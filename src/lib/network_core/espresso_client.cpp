#include "espresso_client.h"

EspressoClient::EspressoClient(std::string hostname, unsigned short port) :
  TcpClient(hostname, port) 
{
  
}

void EspressoClient::connectionClosed() {

  printf("EspressoClient: connection to Barista closed!\n");
}

void EspressoClient::connectionEstablished() {

  printf("EspressoClient: connection to Barista established!\n");
}

void EspressoClient::handleMessageFromServer(int socket) {

  printf("EspressoClient: message received from Barista!\n");

  uint32_t packet_size, flag, bytes_read;
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
