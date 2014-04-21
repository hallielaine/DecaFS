#ifndef _TCP_CLIENT_H_
#define _TCP_CLIENT_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <string>

class TcpClient {                          

  private:

    std::string m_name;

    std::string m_hostname;
    uint16_t m_port;
    uint32_t m_socket_number;
    sockaddr_in m_address;

    bool m_connection_open;

    uint32_t m_sequence_num;

    fd_set m_server_set;

    int connect_to_host();
    void listen(); 

  public:
    TcpClient(std::string host, unsigned short port);

    void closeConnection();
    virtual void connectionClosed() = 0;
    virtual void connectionEstablished() = 0;
    std::string getHost();
    int getPort();
    virtual void handleMessageFromServer(int socket) = 0;
    bool isConnected();
    int openConnection();
    void run();
    int sendToServer(void* packet, int length);
    void setHost(std::string hostname);
    void setPort(int port);
    void setName(std::string name);
};

#endif // _TCP_CLIENT_H_
