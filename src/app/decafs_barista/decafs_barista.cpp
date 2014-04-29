#include "decafs_barista.h"

int main (int argc, char *argv[]) {

  printf("DecafsBarista: initializing barista_core\n");
  barista_core_init (argc, argv);
  printf ("Barista is initialized.\n");
  printf ("\tstripe_size: %d\n\tchunk_size: %d\n\n", get_stripe_size(),
           get_chunk_size());
  printf("DecafsBarista: barista_core initialized\n");

  int port = 0;
  if (argc >= MIN_ARGS) {
    port = atoi(argv[PORT]); 
    printf("got port: %d\n", port);
  } else {
    fprintf(stderr, "port number not specified\n");
    exit(-1);
  }

  printf("DecafsBarista: initializing BaristaServer\n");
  BaristaServer *barista_server = BaristaServer::init(port);
  printf("DecafsBarista: BaristaServer initialized, running BaristaServer\n");
  barista_server->run();

  return 0;
}

