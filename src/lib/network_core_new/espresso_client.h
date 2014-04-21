#ifndef _ESPRESSO_CLIENT_H_
#define _ESPRESSO_CLIENT_H_

#include "net_tcp/tcp_client.h"

class EspressoClient : public TcpClient {

  public:
    EspressoClient(std::string hostname, unsigned short port);

    void connectionClosed();
    void connectionEstablished();
    void handleMessageFromServer(void* packet, ssize_t len);

};

#endif // _ESPRESSO_CLIENT_H_
