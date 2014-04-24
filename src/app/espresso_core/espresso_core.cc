#include "espresso_core.h"

int main(int argc, char** argv) {

  handle_cmd_args(argc, argv);

  // TODO: open filesystem data for when you need it
  // filepath is argv[FILESYSTEM]

  char storage_file[] = ".data_storage";
  char *file = (char *)malloc(strlen (argv[FILESYSTEM]) +
                              strlen (storage_file) + 1);
  strcpy (file, argv[FILESYSTEM]);
  strcat (file, storage_file);
  
  printf ("Opening storage file %s\n", file);
  
  int storage_file_fd;
  if ((storage_file_fd  = open (file, O_RDWR | O_CREAT,
                                S_IRUSR | S_IWUSR)) < 0) {
    perror("Unable to open data storage file.");
    exit (EXIT_FAILURE);
  }

  printf ("storage file open (%d)\n", storage_file_fd);
   
  espresso_global_data_init (storage_file_fd, NODE_STORAGE_SIZE);

  char* barista_hostname = argv[BARISTA_HOSTNAME];
  int barista_port = atoi(argv[BARISTA_PORT]);
  int node_id = atoi(argv[NODE_ID]);
  EspressoClient espresso(barista_hostname, barista_port, node_id);  
  espresso.openConnection();
  espresso.run();

  // the svc_main_loop function lives in network core and
  // calls registered espresso functions as it receives the rpcs
  //svc_main_loop(argc, argv);

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
