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
  int ret;
  if (argc < MIN_ARGS) {
    exit_failure (USAGE_ERROR);
  }
  
  ret = v_meta.set_stripe_size (atoi(argv[STRIPE_SIZE]));
  if (ret < 0) {
    exit_failure (get_size_error_message ("stripe", argv[STRIPE_SIZE]));
  }
  ret = v_meta.set_chunk_size (atoi(argv[CHUNK_SIZE]));
  if (ret < 0) {
    exit_failure (get_size_error_message ("chunk", argv[CHUNK_SIZE]));
  }

  // Add barista node to v_meta
  // Add all espresso nodes to v_meta
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
