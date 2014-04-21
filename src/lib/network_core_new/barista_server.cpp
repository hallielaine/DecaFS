#include "barista_server.h"

BaristaServer::BaristaServer(unsigned short port) : TcpServer(port) {

}

void BaristaServer::clientConnected(ConnectionToClient* client) {

  printf("BaristaServer: a client connected!\n");
}

void BaristaServer::clientDisconnected(ConnectionToClient* client) {

  printf("BaristaServer: a client disconnected!\n");
}

void BaristaServer::handleMessageFromClient(void* buf, ssize_t length, ConnectionToClient* client) {

  printf("BaristaServer: a client sent a message!\n");
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
