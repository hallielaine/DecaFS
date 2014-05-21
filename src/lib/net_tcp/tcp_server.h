#ifndef _TCP_SERVER_H_
#define _TCP_SERVER_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <map>

#include "connection_to_client.h"

#define BACKLOG 5

class TcpServer {

   private:

      bool m_listening;
      bool m_open;

      socklen_t m_socket_length;
      uint32_t m_socket_number;
      uint32_t m_highest_socket; // used to keep track of sockets for clients
      sockaddr_in m_address;

      fd_set m_server_set;
      fd_set m_client_set;

      // client information
      int next_client;
      int next_pending_client;

      // maps port to ConnectionToClient
      std::map<int, ConnectionToClient*> m_clients;

      int m_port;

      void setup_socket();
      void check_for_clients();
      void check_for_messages();

   public:
      TcpServer(unsigned short port);
      virtual ~TcpServer();

      virtual void clientConnected(ConnectionToClient* client) = 0;
      virtual void clientDisconnected(ConnectionToClient* client) = 0;
      void close();
      // getClientConenctions
      int getNumberOfClients();
      int getPort();
      virtual void handleMessageFromClient(ConnectionToClient* client) = 0;
      //bool isClosed();
      bool isListening();
      void run();
      virtual void serverClosed() = 0;
      virtual void serverStarted() = 0;
      virtual void serverStopped() = 0;
      //void setBacklog(int backlog);
      //void setPort(int port);
      void stopListening();
};

#endif // _TCP_SERVER_H_
