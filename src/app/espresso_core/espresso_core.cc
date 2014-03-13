#include "espresso_core.h"

int main(int argc, char** argv) {

  handle_cmd_args(argc, argv);

  load_persistent_metadata(argv[METADATA_FILE]);

  printf("persistent metadata on %s has been loaded, ", argv[NAME_ID]);
  printf("opening espresso node to network connections\n");

  // register espresso callback functions in network layer
  register_read_data_callback(read_data);
  register_write_data_callback(write_data);
  register_delete_data_callback(delete_data);

  // the svc_main_loop function lives in network core and
  // calls registered espresso functions as it receives the rpcs
  svc_main_loop(argc, argv);

  return 0;
}

ssize_t read_data (int fd, int file_id, int stripe_id, int chunk_num, int offset, void *buf, int count) {

  printf("read_data called, espresso_core\n");
  return read_chunk(fd, file_id, stripe_id, chunk_num, offset, buf, count);
}

ssize_t write_data (int fd, int file_id, int stripe_id, int chunk_num, int offset, void *buf, int count) {

  return write_chunk(fd, file_id, stripe_id, chunk_num, offset, buf, count);
}

int delete_data (int fd, int file_id, int stripe_id, int chunk_num) {

   printf("delete_data called, espresso_core\n");
  return delete_chunk(fd, file_id, stripe_id, chunk_num);
}

void load_persistent_metadata(char* metadata_path) {

   printf("loading metadata from: %s\n", metadata_path);
}

void handle_cmd_args(int argc, char** argv) {

  if (argc == NUM_EXPECTED_ARGS) {

  }
  else {
     fprintf(stderr, "%s exiting: invalid command line arguments provided", argv[0]);
     fprintf(stderr, "\nUsage: %s <id> <persistent metadata path>\n", argv[0]);
     exit(-1);
  }
}
