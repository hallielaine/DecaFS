#include "network_core/barista_server.h"
#include "network_core/espresso_client.h"
#include "../decafs_barista/decafs_barista.h"

#include <stdio.h>
#include <thread>
#include "network_core/barista_network_helper.h"

int main(int argc, char** argv) {

  // init barista core information
  barista_core_init (argc, argv);
  printf ("Barista is initialized.\n");
  printf ("\tstripe_size: %d\n\tchunk_size: %d\n\n", get_stripe_size(),
           get_chunk_size());

  int port = 1034;

  // initialize the barista server
  BaristaServer* barista_server = BaristaServer::init(port);
  std::thread barista_thread(&BaristaServer::run, barista_server); 

  // create an espresso node
  EspressoClient espresso("localhost", port, 1);
  espresso.setName("espresso_1");
  
  EspressoClient espresso2("localhost", port, 2);
  espresso2.setName("espresso_2");

  EspressoClient espresso3("localhost", port, 3);
  espresso3.setName("espresso_3");

  EspressoClient espresso4("localhost", port, 4);
  espresso4.setName("espresso_4");


  sleep(1);

  espresso.openConnection();
  espresso2.openConnection();
  espresso3.openConnection();
  espresso4.openConnection();
  std::thread espresso_thread(&EspressoClient::run, &espresso);
  std::thread espresso_thread2(&EspressoClient::run, &espresso2);
  std::thread espresso_thread3(&EspressoClient::run, &espresso3);
  std::thread espresso_thread4(&EspressoClient::run, &espresso4);

  sleep(1);

  //ConnectionToClient *client = barista_server->getEspressoNode();
  //printf("client is: %p\n", (void*)client);

  int sent;

  // call open read write ...
 /* struct ip_address ip;
  ConnectionToClient *ctc = new ConnectionToClient();
  struct client default_client = {ip, 1, ctc};
  int count, fd = open_file ("new_file.txt", O_RDWR, default_client);
  char buf[] = "1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63 64 65 66 67 68 69 70 71 72 73 74 75 76 77 78 79 80 81 82 83 84 85 86 87 88 89 90 91 92 93 94 95 96 97 98 99 100";
  char *read_buf = (char *)malloc (1000);
  memset (read_buf, '\0', 1000);
  printf ("\n\n ------------------------------FIRST WRITE------------------------------\n");
  count = write_file (fd, buf, strlen (buf), default_client);
  printf ("(\n(BARISTA) wrote %d bytes.\n", count);
  close_file (fd, default_client);
*/
  /*sent = network_write_chunk(client, 1, 2, 3, 4, 5, (void*)"hello!", 6);
  printf("write sent: %d bytes\n", sent);

  sent = network_read_chunk(client, 1, 2, 3, 4, 5, 6);
  printf("read sent: %d bytes\n", sent);

  sent = network_delete_chunk(client, 1, 2, 3); 
  printf("delete sent: %d bytes\n", sent);
*/
  sleep(1);

  barista_server->close();
  barista_thread.join();

  sleep(1);

  espresso_thread.join();

  return 0;
}
