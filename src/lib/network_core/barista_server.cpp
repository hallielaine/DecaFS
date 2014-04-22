#include "barista_server.h"

BaristaServer::BaristaServer(unsigned short port) 
 : TcpServer(port) 
{
  printf("%d\n", port);
  next_node_num = 1;
}

void BaristaServer::clientConnected(ConnectionToClient* client) {

  printf("BaristaServer: a client connected!\n");

  // get ip address of client

  //int ip_addr = client->addr.sin_addr.s_addr;
  //char ip[INET_ADDRSTRLEN];
  //inet_ntop(AF_INET, &ip_addr, ip, INET_ADDSTRLEN);
  char* ip_ptr = inet_ntoa(client->addr.sin_addr);

  printf("client connected with ip_address: %s\n", ip_ptr);

  add_node(ip_ptr, next_node_num++);

  cl = client;
}

void BaristaServer::clientDisconnected(ConnectionToClient* client) {

  printf("BaristaServer: a client disconnected!\n");
}

void BaristaServer::handleMessageFromClient(void* buf, ssize_t length, ConnectionToClient* client) {

  printf("BaristaServer: a client sent a message!\n");

  // can call in barista core
  // new_espresso_node
  // espresso_disconnected
  // read_chunk_response
  // write_chunk_response
  // delete_chunk_response 
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
