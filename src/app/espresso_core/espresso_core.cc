#include "espresso_core.h"

int main(int argc, char** argv) {

  handle_cmd_args(argc, argv);

  printf("metadata path is: %s\n", argv[METADATA]);
  espresso_global_data_init (argv[METADATA], NODE_STORAGE_SIZE);

  char* barista_hostname = argv[BARISTA_HOSTNAME];
  int barista_port = atoi(argv[BARISTA_PORT]);
  int node_id = atoi(argv[NODE_ID]);
  EspressoClient espresso(barista_hostname, barista_port, node_id);
  espresso.openConnection();
  espresso.run();

  return 0;
}

void handle_cmd_args(int argc, char** argv) {

  if (argc == NUM_EXPECTED_ARGS) {

  }
  else {
     fprintf(stderr, "%s exiting: invalid command line arguments provided", argv[0]);
     fprintf(stderr, "\nUsage: %s <node_id> <persistent metadata path> <data path> <barista hostname> <barista port>\n", argv[0]);
     exit(-1);
  }
}
