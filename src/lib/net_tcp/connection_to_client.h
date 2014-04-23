#ifndef _CONNECTION_TO_CLIENT_
#define _CONNECTION_TO_CLIENT_

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> 
#include <stdio.h>

struct ConnectionToClient {

    uint32_t sock_fd;
    uint8_t handle;
    sockaddr_in addr;
    uint8_t pending;
    socklen_t socket_len;

    ConnectionToClient();
    int sendToClient(void* packet, int length);
};


#endif // _CONNECTION_TO_CLIENT_
