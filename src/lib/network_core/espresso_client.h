#ifndef _ESPRESSO_CLIENT_H_
#define _ESPRESSO_CLIENT_H_

#include <iostream>

#include "net_tcp/tcp_client.h"
#include "network_packets.h"
#include "espresso_packet_processor.h"

class EspressoClient : public TcpClient {

  private:
    uint32_t node_id;

  public:
    EspressoClient(std::string hostname, unsigned short port, uint32_t node_id);

    void connectionClosed();
    void connectionEstablished();
    void handleMessageFromServer(int socket);

};

#endif // _ESPRESSO_CLIENT_H_
