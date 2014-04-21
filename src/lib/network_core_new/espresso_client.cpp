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

void EspressoClient::handleMessageFromServer(void* packet, ssize_t len) {

  printf("EspressoClient: message received from Barista!\n");

  uint32_t flag = ((uint32_t*)packet)[1];

  if (flag == READ_CHUNK) {
    printf("got a READ_CHUNK packet\n");
    ReadChunkPacket read(packet, len);
    read.printInfo();
  }
}
