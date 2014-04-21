#include "barista_server.h"
#include "espresso_client.h"

#include <stdio.h>
#include <thread>
#include "barista_network_helper.h"

int main(int argc, char** argv) {

  int port = 1024;

  if (argc > 1) {
    port = atoi(argv[1]);
  }

  printf("main is using port: %d\n", port); 

  BaristaServer barista(port);
  EspressoClient espresso("localhost", port);

  std::thread barista_thread(&BaristaServer::run, &barista); 

  sleep(1);

  espresso.openConnection();
  std::thread client_thread(&EspressoClient::run, &espresso);

  sleep(1);

  ConnectionToClient *client = barista.getEspressoNode();
  printf("client is: %p\n", (void*)client);

  int sent;

  sent = network_write_chunk(client, 1, 2, 3, 4, 5, (void*)"hello!", 6);
  printf("write sent: %d bytes\n", sent);

  sent = network_read_chunk(client, 1, 2, 3, 4, 5, 6);
  printf("read sent: %d bytes\n", sent);

  sent = network_delete_chunk(client, 1, 2, 3); 
  printf("delete sent: %d bytes\n", sent);

  sleep(1);

  barista.close();
  barista_thread.join();

  sleep(1);

  client_thread.join();

  return 0;
}
