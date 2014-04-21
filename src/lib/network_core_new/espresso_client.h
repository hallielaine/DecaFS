#ifndef _ESPRESSO_CLIENT_H_
#define _ESPRESSO_CLIENT_H_

#include <iostream>

#include "net_tcp/tcp_client.h"
#include "network_packets.h"

class EspressoClient : public TcpClient {

  public:
    EspressoClient(std::string hostname, unsigned short port);

    void connectionClosed();
    void connectionEstablished();
    void handleMessageFromServer(int socket);

};

#endif // _ESPRESSO_CLIENT_H_
