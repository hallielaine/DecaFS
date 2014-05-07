#ifndef _BARISTA_SERVER_H_
#define _BARISTA_SERVER_H_

#include <map>
#include <stdio.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "network_packets.h"
#include "open_packet.h"
#include "write_packet.h"
#include "read_packet.h"
#include "close_packet.h"
#include "lseek_packet.h"

#include "net_tcp/tcp_server.h"
#include "volatile_metadata/volatile_metadata_c_api.h"
#include "barista_core/barista_core.h"

class BaristaServer : public TcpServer {

  private:

    static BaristaServer* instance;

    // private because its a singleton
    BaristaServer(unsigned short port);

    uint32_t next_node_num;

    // this contains the espresso nodes and decafs client connections until the
    // connections send their initialization information to the server
    std::map<ConnectionToClient*, int> m_pending_clients; 
    std::map<int, ConnectionToClient*> m_espresso_nodes;
    std::map<ConnectionToClient*, int> m_espresso_nodes_rev;
    std::map<ConnectionToClient*, struct client> m_decafs_clients;

    void addEspressoNode(EspressoInit espresso_node, ConnectionToClient* ctc);
    void addDecafsClient(DecafsClientInit decafs_client, ConnectionToClient* ctc);

  public:
    static BaristaServer* init(int port); 
    static BaristaServer* get();

    void clientConnected(ConnectionToClient* client);
    void clientDisconnected(ConnectionToClient* client);
    void handleMessageFromClient(ConnectionToClient* client);
    void serverClosed();
    void serverStarted();
    void serverStopped();

    int numEspressoNodes();
    int sendToEspresso(int node_id, Packet packet); 
}; 

#endif // _BARISTA_SERVER_H_
