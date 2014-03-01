#include "barista_core.h"

#define MIN_ARGS 4
#define STRIPE_SIZE 1
#define CHUNK_SIZE 2
#define BARISTA 3

Volatile_Metadata v_meta;

int main (int argc, char *argv[]) {
  process_arguments (argc, argv);

  // TODO: Recreate metadata from file

  // Wait for Espresso Nodes to finish startup

  // TODO: Change to proper logging
  printf ("Barista is initialized.\n");
  printf ("\tstripe_size: %d\n\tchunk_size: %d\n", 
           v_meta.get_stripe_size(), v_meta.get_chunk_size());
  return 0;
}

void process_arguments (int argc, char *argv[]) {
  if (argc < MIN_ARGS) {
    exit_failure (USAGE_ERROR);
  }
  
  v_meta.set_stripe_size (atoi(argv[STRIPE_SIZE]));
  v_meta.set_chunk_size (atoi(argv[CHUNK_SIZE]));

  // Add barista node to v_meta
  // Add all espresso nodes to v_meta
}

void exit_failure (const char *message) {
  perror (message);
  exit (EXIT_FAILURE);
}
