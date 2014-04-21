#ifndef _BARISTA_SERVER_H_
#define _BARISTA_SERVER_H_

#include <stdio.h>

#include "net_tcp/tcp_server.h"

class BaristaServer : public TcpServer {

  public:
    BaristaServer(unsigned short port);

    virtual void clientConnected(ConnectionToClient* client);
    virtual void clientDisconnected(ConnectionToClient* client);
    virtual void handleMessageFromClient(void* buf, ssize_t length, ConnectionToClient* client);
    virtual void serverClosed();
    virtual void serverStarted();
    virtual void serverStopped();
}; 

#endif // _BARISTA_SERVER_H_
