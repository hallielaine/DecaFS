#include "connection_to_client.h"

ConnectionToClient::ConnectionToClient() {}

int ConnectionToClient::sendToClient(void* packet, int length) {

  printf("client sock fd is : %d\n", sock_fd);
  ssize_t rtn = send(sock_fd, packet, length, 0);

  if (rtn < 0) {
    perror("ConnectionToClient.send()");  
  }

  return rtn;
}
