#include "barista_server.h"
#include "espresso_client.h"

#include <thread>

int main(int argc, char** argv) {

  int port = 1024;

  if (argc > 1) {
    port = atoi(argv[1]);
  }

  printf("main is using port: %d\n", port); 

  BaristaServer barista(port);
  EspressoClient client("localhost", port);

  std::thread barista_thread(&BaristaServer::run, &barista); 

  sleep(1);

  printf("Calling serverStopped\n"); 
  barista.serverStopped();
  printf("serverStopped finished\n");

  sleep(1);

  barista.close();
  barista_thread.join();

  return 0;
}
