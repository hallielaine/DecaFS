#include "network_core/barista_server.h"
#include "network_core/espresso_client.h"
#include "../decafs_barista/decafs_barista.h"

#include <stdio.h>
#include <thread>
#include "network_core/decafs_client.h"


// ----------------------- ESPRESSO STORAGE FAKE FUNCTIONS ------------------

ssize_t read_chunk(int fd, int file_id, int stripe_id, int chunk_num,
    int offset, void *buf, int count) {

  return count;
}

ssize_t write_chunk(int fd, int file_id, int stripe_id, int chunk_num,
    int offset, void *buf, int count) {

  return count;
}

int delete_chunk(int fd, int file_id, int stripe_id, int chunk_num) {

  return 0;
}

// ----------------------- BARISTA CORE FAKE FUNCTIONS ----------------------
// set_node_down
extern "C" uint32_t set_node_down (uint32_t node_number) {
 return 0;
}

// set_node_up
extern "C" uint32_t set_node_up (uint32_t node_number) {
  return 0;
}

// open_file 
extern "C" void open_file (const char *pathname, int flags, struct client client) {

}

// read_response_handler
extern "C" void read_response_handler (ReadChunkResponse *read_response) {

}

// write_response_handler
extern "C" void write_response_handler (WriteChunkResponse *write_response) {

}

// delete_response_handler
extern "C" void delete_response_handler (DeleteChunkResponse *delete_response) {

}

int main(int argc, char** argv) {

  int port = 12345;
  BaristaServer* barista_server = BaristaServer::init(port);
  std::thread barista_thread(&BaristaServer::run, barista_server);

  EspressoClient espresso("localhost", port, 1);
  std::thread espresso_thread(&EspressoClient::run, &espresso);

  // hostname, port, user_id
  DecafsClient dc("localhost", port, 2);
  dc.openConnection();

  sleep(1);

  // OPEN
  dc.open("test", 0);

  sleep(1);

  barista_server->close();
  barista_thread.join();
  espresso_thread.join();

  return 0;
}
