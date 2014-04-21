#include "tcp_server.h"

inline void safe_select(uint32_t socket, fd_set* fds) {
 
  timeval time;
  time.tv_sec = 0;
  time.tv_usec = 100000;
 
  if (select(socket, fds, NULL, NULL, &time) < 0) {
    perror("select");
    // TODO what to do here
    //exit(-1);
  }
}

TcpServer::TcpServer(unsigned short port) :
   m_port(port) 
{
  setup_socket();
  m_highest_socket = m_socket_number;

  FD_SET(m_socket_number, &m_server_set);
}

void TcpServer::close() {
  //TODO
  m_open = false;

  // wait until the server stops listening
  while (isListening()) {
    // spin until the server stops listening
  }

  printf("TcpServer: closing all ports\n");

  if (::close(m_socket_number) < 0) {
    perror("TcpServer: close");
  }

  std::map<int, ConnectionToClient*>::iterator it;

  for (it = m_clients.begin(); it != m_clients.end(); ++it) {
    if (::close(it->second->sock_fd) < 0) {
      perror("TcpServer: close");
    }
  }

  serverClosed();
}

int TcpServer::getNumberOfClients() {

  return m_clients.size();
}

int TcpServer::getPort() {

  return m_port;
}

void TcpServer::setup_socket() {

  m_socket_length = sizeof(struct sockaddr_in);

  if ((m_socket_number = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket call");
    //exit(-1);
  }

  // Name the socket
  m_address.sin_family = AF_INET;
  m_address.sin_addr.s_addr = INADDR_ANY;
  m_address.sin_port = htons(m_port);

  if (bind(m_socket_number, (struct sockaddr *) &m_address, m_socket_length) < 0) {
    perror("bind call");
    //exit(-1);
  }

  if (getsockname(m_socket_number, (struct sockaddr *) &m_address, &m_socket_length)) {
    perror("getsockname call");
    //exit(-1);
  }

  printf("TcpServer: using port %d\n", ntohs(m_address.sin_port));
  // TODO where should sequence number be kept track of, currently this is in NetworkPacket extended class  
  //sequence_number = 1;
  listen(m_socket_number, BACKLOG);
  m_open = true;
}

bool TcpServer::isListening() {

  return m_listening;
}

void TcpServer::run() {

  m_listening = true;

  while (m_listening && m_open) {

    printf("TcpServer: spinning\n");
    check_for_clients(); // new connections
    check_for_messages(); // messages from clients
  }

  serverStopped();
  m_listening = false;
}

void TcpServer::check_for_clients() {

  fd_set tmp_set;

  // Check if the Server has any new clients
  memcpy(&tmp_set, &m_server_set, sizeof(fd_set));
  safe_select(m_highest_socket+1, &tmp_set);

  if (FD_ISSET(m_socket_number, &tmp_set)) {
    // add a client
    ConnectionToClient* client = new ConnectionToClient();
    client->socket_len = sizeof(struct sockaddr_in);

    client->sock_fd = accept(m_socket_number, (sockaddr*)&client->addr, &client->socket_len);
    client->pending = 1;

    if (client->sock_fd < 0) {
      perror("accept");
      return;
    }

    if (client->sock_fd > m_highest_socket) {
      m_highest_socket = client->sock_fd;
    }

    printf("TcpServer: a client joined and was assigned fd: %d\n", client->sock_fd);
    m_clients[client->sock_fd] = client;
    FD_SET(client->sock_fd, &m_client_set);
    clientConnected(client);
  }
}

void TcpServer::check_for_messages() {

  fd_set tmp_set;
  std::map<int, ConnectionToClient*>::iterator it;
  int len_read;
  ConnectionToClient* client;

  int max_len = 65536;
  uint8_t* buf = (uint8_t*)malloc(max_len);

  memcpy(&tmp_set, &m_client_set, sizeof(fd_set));
  safe_select(m_highest_socket+1, &tmp_set);

  for (it = m_clients.begin(); it != m_clients.end(); ++it) {

    client = it->second;
    if (FD_ISSET(client->sock_fd, &tmp_set)) {
      len_read = recv(client->sock_fd, buf, max_len, 0);

      if (len_read > 0) {
        handleMessageFromClient(buf, len_read, client);
      } else if (len_read == 0) {
        printf("TcpServer: length 0 message received from a client.");
      } else {
        perror("TcpServer read message");
      }
    }
  }
}

void TcpServer::stopListening() {

  m_listening = false;
}
