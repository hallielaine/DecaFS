#include "decafs_barista.h"

int main (int argc, char *argv[]) {
  barista_core_init (argc, argv);
  printf ("Barista is initialized.\n");
  printf ("\tstripe_size: %d\n\tchunk_size: %d\n\n", get_stripe_size(),
           get_chunk_size());

  int port = 0;
  if (argc >= MIN_ARGS) {
    port = atoi(argv[PORT]); 
    printf("got port: %d\n", port);
  } else {
    fprintf(stderr, "port number not specified\n");
    exit(-1);
  }

  BaristaServer *barista_server = BaristaServer::init(port);
  barista_server->run();

  /* TEST CODE */
  /*struct ip_address ip;
  struct client default_client = {ip, 1, 1};
  int count, fd = open_file ("new_file.txt", O_RDWR, default_client);
  char buf[] = "1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63 64 65 66 67 68 69 70 71 72 73 74 75 76 77 78 79 80 81 82 83 84 85 86 87 88 89 90 91 92 93 94 95 96 97 98 99 100";
  char *read_buf = (char *)malloc (1000);
  memset (read_buf, '\0', 1000);
  printf ("\n\n ------------------------------FIRST WRITE------------------------------\n");
  count = write_file (fd, buf, strlen (buf), default_client);
  printf ("(\n(BARISTA) wrote %d bytes.\n", count);
  
  close_file (fd, default_client);
  fd = open_file ("new_file.txt", O_RDONLY, default_client);

  printf ("\n\n ------------------------------FIRST READ--------------------------------\n");
  count = read_file (fd, read_buf, strlen (buf), default_client);
  printf ("\n(BARISTA) Read %d bytes.\n", count);
  printf ("(BARISTA) Buf is:\n%s\n", read_buf);
  
  close_file (fd, default_client);
  fd = open_file ("new_file.txt", O_RDWR | O_APPEND, default_client);

  printf ("\n\n ------------------------------SECOND WRITE------------------------------\n");
  write_file (fd, buf, strlen (buf), default_client);
  
  close_file (fd, default_client);
  fd = open_file ("new_file.txt", O_RDONLY, default_client);
  
  printf ("\n\n ------------------------------SECOND READ--------------------------------\n");
  count = read_file (fd, read_buf, 2*strlen (buf), default_client);
  printf ("\n(BARISTA) Read %d bytes.\n", count);
  printf ("(BARISTA) Buf is:\n%s\n", read_buf);*/
  return 0;
}

