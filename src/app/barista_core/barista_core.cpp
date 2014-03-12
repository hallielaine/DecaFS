#include "barista_core.h"

#define MIN_ARGS 5
#define STRIPE_SIZE 1
#define CHUNK_SIZE 2
#define BARISTA 3
#define ESPRESSO 4

Volatile_Metadata volatile_metadata;
Persistent_Metadata persistent_metadata;

int main (int argc, char *argv[]) {
  process_arguments (argc, argv);

  // TODO: Recreate metadata from file

  // Wait for Espresso Nodes to finish startup

  // TODO: Change to proper logging
  printf ("Barista is initialized.\n");
  printf ("\tstripe_size: %d\n\tchunk_size: %d\n", 
           volatile_metadata.get_stripe_size(), volatile_metadata.get_chunk_size());
  return 0;
}

void process_arguments (int argc, char *argv[]) {
  int ret;
  if (argc < MIN_ARGS) {
    exit_failure (USAGE_ERROR);
  }
  
  ret = volatile_metadata.set_stripe_size (atoi(argv[STRIPE_SIZE]));
  if (ret < 0) {
    exit_failure (get_size_error_message ("stripe", argv[STRIPE_SIZE]));
  }
  ret = volatile_metadata.set_chunk_size (atoi(argv[CHUNK_SIZE]));
  if (ret < 0) {
    exit_failure (get_size_error_message ("chunk", argv[CHUNK_SIZE]));
  }

  // Add barista node to volatile_metadata
  // BARISTA param is log file

  // Add all espresso nodes to volatile_metadata
  for (int i = ESPRESSO; i < argc; i++) {
    int res = network_add_client(argv[i]);
    if (res >= 0) {
      volatile_metadata.add_node(argv[i], res);
      network_write_chunk(1, 1, res, 1, 1, 1, argv[i], 2);
    }
    else {
      fprintf(stderr, "Failed to connect to espresso node: %sn", argv[i]);
    }
  }
}

const char *get_size_error_message (const char *type, const char *value) {
  std::string msg = "Invalid size ";
  msg += value;
  msg += " for ";
  msg += type;

  return msg.c_str();
}

void exit_failure (const char *message) {
  fprintf (stderr, "%s\n", message);
  exit (EXIT_FAILURE);
}
