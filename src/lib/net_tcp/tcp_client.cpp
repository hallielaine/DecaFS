#include "tcp_client.h"

inline void safe_select(uint32_t socket, fd_set* fds) {

  timeval time;
  time.tv_sec = 0;
  time.tv_usec = 100000;

  if (select(socket, fds, NULL, NULL, &time) < 0) {

    printf("TcpClient failure\n");
    perror("select");
    // TODO what to do here
    //exit(-1);
  }
}   

TcpClient::TcpClient(std::string hostname, unsigned short port) :
  m_hostname(hostname),
  m_port(port),
  m_connection_open(0)
{

}

void TcpClient::closeConnection() {

  ::close(m_socket_number);
  m_connection_open = false;
  connectionClosed();
}

std::string TcpClient::getHost() {
   return m_hostname;
}

int TcpClient::getPort() {
  return m_port;
}

bool TcpClient::isConnected() {
   return m_connection_open;
}

int TcpClient::openConnection() {

  printf("TcpClient: %s openConnection called\n", m_name.c_str());

  socklen_t socket_length = sizeof(struct sockaddr_in);
  struct hostent *hp;

  if ((m_socket_number = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("TcpClient failure\n");
    perror("socket call");
    return 0;
  }
  
  memset(&m_address, 0, sizeof(sockaddr_in));
  m_address.sin_family = AF_INET;
  
  if ((hp = gethostbyname(m_hostname.c_str())) == NULL) {
    printf("TcpClient: %s error getting hostname: %s\n", m_name.c_str(), m_hostname.c_str());
    //exit(EXIT_FAILURE);
  }

  memcpy((uint8_t *)&(m_address.sin_addr.s_addr),
         (uint8_t *)hp->h_addr, hp->h_length);
  m_address.sin_port = htons(m_port);

  printf("TcpClient: %s connecting to port: %d\n", m_name.c_str(), ntohs(m_address.sin_port));
  if (connect(m_socket_number, (sockaddr*)&m_address, socket_length) < 0) {
    printf("TcpClient failure\n");
    perror("connect");
    return 0;
  }

  m_connection_open = true;

  return 1;
}

void TcpClient::run() {

  fd_set tmp_set;

  ssize_t max_len = 4;
  ssize_t len;

  uint32_t buf;

  while(m_connection_open) {

    //printf("TcpClient: %s spinning\n", m_name.c_str());

    // TODO implement scanning for messages from server
    FD_SET(m_socket_number, &tmp_set);
    safe_select(m_socket_number+1, &tmp_set);

    if (FD_ISSET(m_socket_number, &tmp_set)) {
      // receive message from server
      len = recv(m_socket_number, (void*)&buf, max_len, MSG_PEEK);

      if (len < 0) {
        printf("TcpClient failure\n");
        perror("Client recv");
      } else if (len == 0) {
        printf("TcpClient: %s received a msg len of 0 from the server\n", m_name.c_str());
        closeConnection();
      } else {
        printf("TcpClient: %s received a msg from the server\n", m_name.c_str());
        handleMessageFromServer(m_socket_number);
      } 
    }
  }
} 

int TcpClient::sendToServer(void* packet, int length) {

  int rtn = send(m_socket_number, packet, length, 0);

  if (rtn < 0) {
     printf("TcpClient failure\n");
     perror("TcpClient.sendToServer()");
     return rtn;
  }

  return rtn;
}

void TcpClient::setHost(std::string hostname) {

   m_hostname = hostname;
}

void TcpClient::setPort(int port) {

  m_port = port;
}

void TcpClient::setName(std::string name) {

  m_name = name;
}
