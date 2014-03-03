#include "bar_clnt.h"

CLIENT **g_clients = NULL;
int num_clients = 0;

/*int main() {

  int node_id = network_add_client("localhost");
  printf("node id: %d\n", node_id);

  char buf[100];

  network_write_chunk(1, 2, node_id, 4, 5, 6, "data to write", 8);
  int res = network_read_chunk(1, 2, node_id, 4, 5, 6, buf, 100); 

  printf("read data: %s\n", buf);

  return 0;
}*/

int network_add_client(char *hostname) {

  if (g_clients == NULL) {
    g_clients = (CLIENT**)malloc(sizeof(struct CLIENT*)*MAX_CLIENTS);
  }

  CLIENT* cl = clnt_create(hostname, DECAFS_NETWORK_CORE, DECAFS_NETWORK_CORE_V1, "tcp");
  
  if (cl == NULL) {
    fprintf(stderr, "Could not connect to %s\n", hostname);
    return -1; // could not connect to client
  }

  g_clients[num_clients++] = cl;
  printf("%s is now client %d\n", hostname, num_clients - 1);

  return num_clients - 1;
}

ssize_t network_read_chunk(int fd, int file_id, int node_id, int stripe_id,
                           int chunk_num, int offset, void *buf, int count) {

  read_params data;

  data.fd = fd;
  data.file_id = file_id;
  data.stripe_id = stripe_id;
  data.chunk_num = chunk_num;
  data.offset = offset;
  data.count = count;

  if (node_id >= 0 && node_id < num_clients) {
    read_rtn *ret = rpc_read_data_1(&data, g_clients[node_id]);
    strncpy(buf, ret->buf, ret->length);
    return ret->length;
  }  
  else {
    fprintf(stderr, "node_id %d does not exist\n", node_id); 
    return -1; // node_id does not exist
    // this may conflict with an error message from READ
  }
}

ssize_t network_write_chunk(int fd, int file_id, int node_id, int stripe_id, 
                            int chunk_num, int offset, const void* buf, int count) {

  write_params data;

  data.fd = fd;
  data.file_id = file_id;
  data.stripe_id = stripe_id;
  data.chunk_num = chunk_num;
  data.offset = offset;
  data.buf = buf;
  data.count = count;

  if (node_id >= 0 && node_id < num_clients) {
    int *ret = rpc_write_data_1(&data, g_clients[node_id]);
    return *ret;
  }  
  else {
    fprintf(stderr, "node_id %d does not exist\n", node_id); 
    return -1; // node_id does not exist
    // this may conflict with an error message from READ
  }
}

int network_delete_chunk(int node_id, int file_id, int stripe_id, int chunk_num) {

  return 1;
}
