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
  m_pending_clients[client] = 1;  

  // not necessary here
  char* ip_ptr = inet_ntoa(client->addr.sin_addr);
  printf("client connected from ip_address: %s\n", ip_ptr);
}

void BaristaServer::clientDisconnected(ConnectionToClient* client) {

  printf("BaristaServer: a client disconnected!\n");
  // TODO take care of client disconnections
}

void BaristaServer::addEspressoNode(EspressoInit espresso_node, ConnectionToClient* ctc) {

  if (m_pending_clients.count(ctc)) {
    m_pending_clients.erase(ctc);
    m_espresso_nodes[espresso_node.node_id] = ctc;
    set_node_up(espresso_node.node_id);
    printf("BaristaServer: espresso node: %d connected\n", espresso_node.node_id);
  }
}

void BaristaServer::addDecafsClient(DecafsClientInit decafs_client, ConnectionToClient* ctc) {

  if (m_pending_clients.count(ctc)) {
    m_pending_clients.erase(ctc);
    struct client dclient = client(inet_ntoa(ctc->addr.sin_addr), decafs_client.user_id, ctc);
    m_decafs_clients[ctc] = dclient;
    printf("a decafs client connected: ip<%s>, user_id:<%d>\n", dclient.ip.addr, dclient.user_id);
  }
}

void BaristaServer::handleMessageFromClient(ConnectionToClient* client) {


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
  printf("BaristaServer: received a message from a client, with flag %d!\n", flag);

  // TODO this should be split into types based on what type of client ctc is
  // ctc possibilities: pending, decafs_client, espresso_node
  switch (flag) {
    case (OPEN) :
    {
       printf("goat a OPEN packet!\n");
       OpenPacket op(buffer_ptr, packet_size);
       std::cout << op << std::endl;
       open_file(op.filepath, op.open_flags, m_decafs_clients[client]);
       break;
    }
    case (ESPRESSO_INIT) :
    {
      printf("got a ESPRESSO_INIT packet\n");
      EspressoInit espressoNode(buffer_ptr, packet_size);
      std::cout << espressoNode << std::endl;
      addEspressoNode(espressoNode, client);
      break;
    }
    case (DECAFS_CLIENT_INIT) :
    {
      printf("got a DECAFS_CLIENT_INIT packet\n");
      DecafsClientInit decafs_client(buffer_ptr, packet_size);
      std::cout << decafs_client << std::endl;
      addDecafsClient(decafs_client, client);
      break;
    }
    case (READ_CHUNK_RESPONSE) : 
    {
      printf("\ngot a READ_CHUNK_RESPONSE packet\n");
      ReadChunkResponse readResponse(buffer_ptr, packet_size);
      std::cout << readResponse << std::endl;
      read_response_handler(&readResponse);
      break;
    }
    case (WRITE_CHUNK_RESPONSE) : 
    {
      printf("\ngot a WRITE_CHUNK_RESPONSE packet\n");
      WriteChunkResponse writeResponse(buffer_ptr, packet_size);
      std::cout << writeResponse << std::endl;
      write_response_handler(&writeResponse);
      break;
    }
    case (DELETE_CHUNK_RESPONSE) :
    {
      printf("\ngot a DELETE_CHUNK_RESPONSE packet\n");
      DeleteChunkResponse deleteResponse(buffer_ptr, packet_size);
      std::cout << deleteResponse << std::endl;
      delete_response_handler(&deleteResponse);
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
