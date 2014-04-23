#include "barista_server.h"

BaristaServer* bs;

BaristaServer* BaristaServer::init(int port) {

  static bool init = false;

  if (!init) {
    bs = new BaristaServer(port);
    init = true;
  }

  return bs;
}

BaristaServer* BaristaServer::get() {

  return bs;
}

BaristaServer::BaristaServer(unsigned short port) 
 : TcpServer(port) 
{
  printf("%d\n", port);
  next_node_num = 1;
}

int BaristaServer::sendToEspresso(int node_id, Packet packet) {

  return m_espresso_nodes[node_id]->sendToClient(packet.packet, packet.packet_size); 
}

void BaristaServer::clientConnected(ConnectionToClient* client) {

  printf("BaristaServer: a client connected!\n");

  // get ip address of client

  //int ip_addr = client->addr.sin_addr.s_addr;
  //char ip[INET_ADDRSTRLEN];
  //inet_ntop(AF_INET, &ip_addr, ip, INET_ADDSTRLEN);
  char* ip_ptr = inet_ntoa(client->addr.sin_addr);

  int node_id = next_node_num++;

  ip_ptr[strlen(ip_ptr)-1] = (char)(node_id + 48);
  printf("client connected with ip_address: %s\n", ip_ptr);

  set_node_up(node_id);
  m_espresso_nodes[node_id] = client;

  cl = client;
}

void BaristaServer::clientDisconnected(ConnectionToClient* client) {

  printf("BaristaServer: a client disconnected!\n");
}

void BaristaServer::handleMessageFromClient(ConnectionToClient* client) {

  printf("BaristaServer: received a message from a client!\n");

  int32_t packet_size, flag;
  void* buffer_ptr;

  // get the next packet size
  if (recv(client->sock_fd, (void*)&packet_size, sizeof(packet_size), MSG_PEEK) <= 0) {
    // TODO error
  }

  buffer_ptr = (char*)malloc(packet_size);
  if (recv(client->sock_fd, buffer_ptr, packet_size, 0) != packet_size) {
    // TODO error expected to read more bytes for a complete Packet message
  } 
   
  flag = ((uint32_t*)buffer_ptr)[2];

  switch (flag) {
    case (READ_CHUNK_RESPONSE) : 
    {
      printf("\ngot a READ_CHUNK_RESPONSE packet\n");
      ReadChunkResponse readResponse(buffer_ptr, packet_size);
      std::cout << readResponse << std::endl;
      // TODO call back to barista core 
      break;
    }
    case (WRITE_CHUNK_RESPONSE) : 
    {
      printf("\ngot a WRITE_CHUNK_RESPONSE packet\n");
      WriteChunkResponse writeResponse(buffer_ptr, packet_size);
      std::cout << writeResponse << std::endl;
      // TODO call back to barista core
      break;
    }
    case (DELETE_CHUNK_RESPONSE) :
    {
      printf("\ngot a DELETE_CHUNK_RESPONSE packet\n");
      DeleteChunkResponse deleteResponse(buffer_ptr, packet_size);
      std::cout << deleteResponse << std::endl;
      // TODO call back to barista core
      break;
    }
  }
}

void BaristaServer::serverClosed() {

  printf("BaristaServer: the barista has been closed!\n");
}

void BaristaServer::serverStarted() {

  printf("BaristaServer: server started!\n");
}

void BaristaServer::serverStopped() {

  printf("BaristaServer: server stopped!\n");
}

int BaristaServer::numEspressoNodes() {

  return m_espresso_nodes.size();
}

// for testing only
ConnectionToClient* BaristaServer::getEspressoNode() {
  return cl;
}
