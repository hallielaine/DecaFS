#include "network_fake.h"

std::map<std::string, char*> chunk_map;

std::string chunk_id_formatter(int file_id, int node_id, int stripe_id, int chunk_num) {

  std::ostringstream stream;
  stream << node_id << "." << file_id << "." << stripe_id << "." << chunk_num;
  return stream.str();
}

// sends a read chunk request to an espresso node
// returns -1 on error
int network_read_chunk(int32_t id, int fd, int file_id, int node_id, int stripe_id,
 int chunk_num, int offset, int count) {

  std::string name = chunk_id_formatter(file_id, node_id, stripe_id, chunk_num);

  if (chunk_map.count(name)) {

    char* base = &((chunk_map[name])[offset]);

    ReadChunkResponse readResponse(id, fd, file_id, stripe_id, chunk_num, offset, count, (uint8_t*)base);
    printf("network_read_chunk: %s\n", base);
    
    read_response_handler(&readResponse);

    return 1;
  }
  else {
    return -1;
  }
}

// sends a write chunk request to client
// returns -1 on error
int network_write_chunk(int32_t id, int fd, int file_id, int node_id, int stripe_id,
 int chunk_num, int offset, void* buf, int count) {

  std::string name = chunk_id_formatter(file_id, node_id, stripe_id, chunk_num);

  if (!chunk_map.count(name)) { // name is not yet in the map

    chunk_map[name] = (char*) malloc(MAX_CHUNK);
  }

  char* base = &((chunk_map[name])[offset]);
  memcpy(base, buf, count);

  WriteChunkResponse writeResponse(id, fd, file_id, stripe_id, chunk_num, offset, count);
  write_response_handler(&writeResponse);

  return 1;
}

// sends a delete chunk request to client
// returns -1 on error
int network_delete_chunk(int32_t id, int file_id, int node_id, int stripe_id, int chunk_num) {

  std::string name = chunk_id_formatter(file_id, node_id, stripe_id, chunk_num);

  if (!chunk_map.count(name)) {
    
    char* data_ptr = chunk_map[name];

    chunk_map.erase(name);
    free(data_ptr);

    return 1;
  }

  // chunk didnt exist ???
  return -1;
}

//int network_flush(ConnectionToClient* client);

int send_open_result(struct client c, int result) {

  std::cout << "fake send_open_result called!" << std::endl; 
  std::cout << "assigned fd is: " << result << std::endl;

  return 1;
}

int send_read_result(struct client c, int fd, ssize_t count, void* buf) {

  std::cout << "fake send_read_result called!" << std::endl;
  std::cout << "read: ";
  std::cout.write((char*)buf, count);
  std::cout << std::endl;

  return 1;
}

int send_write_result(struct client c, int fd, ssize_t count) {

  std::cout << "fake send_write_result called!" << std::endl;
  return 1;
}

int send_close_result(struct client c, int result) {

  std::cout << "fake send_close_result called!" << std::endl;
  return 1;
}

int send_remove_result(struct client c, int result) {

  std::cout << "fake send_remove_result called!" << std::endl;
  return 1;
}

int send_seek_result(struct client c, int result) {

  std::cout << "fake send_seek_result called!" << std::endl;
  return 1;
}

int send_opendir_result(struct client c, decafs_dir* dirp) {

  std::cout << "fake send_opendir_result called!" << std::endl;
  return 1;
}

int send_file_storage_stat_result(struct client c, const char* data) {

  std::cout << "fake send_file_storage_stat_result called!" << std::endl;
  return 1;
}

int send_mkdir_result(struct client c, int result) {

  std::cout << "fake send_mkdir_result called!" << std::endl;
  return 1;
}
