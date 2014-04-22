#include "barista_server.h"

BaristaServer* barista_server = NULL;

BaristaServer* BaristaServer::init(int port) {

  static bool init = false;

  if (!init) {
    barista_server = new BaristaServer(port);
    init = true;
  }

  return barista_server;
}

BaristaServer* BaristaServer::get() {

  return barista_server;
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

  printf("client connected with ip_address: %s\n", ip_ptr);

  int node_id = next_node_num++;

  add_node(ip_ptr, node_id);
  m_espresso_nodes[node_id] = client;

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
