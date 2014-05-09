#include "barista_core.h"

#define MIN_ARGS 6
#define STRIPE_SIZE 1
#define CHUNK_SIZE 2
#define METADATA 3
#define NUM_ESPRESSOS 4
#define PORT 5

IO_Manager io_manager;
Persistent_Metadata persistent_metadata;
Volatile_Metadata volatile_metadata;

std::map<uint32_t, struct read_request_info> active_read_requests;

std::map<uint32_t, struct write_request> write_request_lookups;
std::map<struct write_request, struct write_request_info> active_write_requests;

std::map<uint32_t, struct request_info> active_delete_requests;

// ------------------------IO Manager Call Throughs---------------------------
extern "C" uint32_t process_read_stripe (uint32_t request_id, uint32_t file_id,
                                         char *pathname, uint32_t stripe_id,
                                         uint32_t stripe_size, uint32_t chunk_size,
                                         const void *buf, int offset,
                                         size_t count) {
    return io_manager.process_read_stripe (request_id, file_id, pathname,
                                           stripe_id, stripe_size, chunk_size,
                                           buf, offset, count);
}

extern "C" void process_write_stripe (uint32_t request_id, uint32_t replica_request_id,
                                      uint32_t *chunks_written,
                                      uint32_t *replica_chunks_written,
                                      uint32_t file_id, char *pathname,
                                      uint32_t stripe_id, uint32_t stripe_size,
                                      uint32_t chunk_size, const void *buf,
                                      int offset, size_t count) {
  io_manager.process_write_stripe (request_id, replica_request_id,
                                   chunks_written, replica_chunks_written,
                                   file_id, pathname,
                                   stripe_id, stripe_size, chunk_size,
                                   buf, offset, count);
}

extern "C" uint32_t process_delete_file (uint32_t request_id, uint32_t file_id) {
  return io_manager.process_delete_file (request_id, file_id);
}

extern "C" int set_node_id (uint32_t file_id, uint32_t stripe_id,
                            uint32_t chunk_num, uint32_t node_id) {
  return io_manager.set_node_id (file_id, stripe_id, chunk_num, node_id);
}

extern "C" int get_node_id (uint32_t file_id, uint32_t stripe_id, uint32_t chunk_num) {
  return io_manager.get_node_id (file_id, stripe_id, chunk_num);
}

extern "C" int set_replica_node_id (uint32_t file_id, uint32_t stripe_id, uint32_t chunk_num,
                                    uint32_t node_id) {
  return io_manager.set_replica_node_id (file_id, stripe_id, chunk_num, node_id);
}

extern "C" int get_replica_node_id (uint32_t file_id, uint32_t stripe_id,
                                    uint32_t chunk_num) {
  return io_manager.get_replica_node_id (file_id, stripe_id, chunk_num);
}

extern "C" int stat_file_name (char *pathname, struct decafs_file_stat *buf) {
  return io_manager.stat_file_name (pathname, buf);
}

extern "C" int stat_file_id (uint32_t file_id, struct decafs_file_stat *buf) {
  return io_manager.stat_file_id (file_id, buf);
}

extern "C" int stat_replica_name (char *pathname, struct decafs_file_stat *buf) {
  return io_manager.stat_replica_name (pathname, buf);
}

extern "C" int stat_replica_id (uint32_t file_id, struct decafs_file_stat *buf) {
  return io_manager.stat_replica_id (file_id, buf);
}

extern "C" void sync () {
  return io_manager.sync();
}

// ------------------------Persistent Metadata Call Throughs---------------------------
extern "C" int get_num_files (struct client client) {
  int res;
  if (get_metadata_lock (client) < 0) {
    return NO_METADATA_LOCK;
  }
  res = persistent_metadata.get_num_files();
  release_metadata_lock (client);
  return res;
}

extern "C" int get_filenames (char *filenames[MAX_FILENAME_LENGTH], int size,
                              struct client client) {
  int res;
  if (get_metadata_lock (client) < 0) {
    return NO_METADATA_LOCK;
  }
  res = persistent_metadata.get_filenames(filenames, size);
  release_metadata_lock (client);
  return res;
}

extern "C" int decafs_file_sstat (char *pathname, struct decafs_file_stat *buf,
                                 struct client client) {
  int res;
  if (get_metadata_lock (client) < 0) {
    return NO_METADATA_LOCK;
  }
  res = persistent_metadata.decafs_file_sstat (pathname, buf);
  release_metadata_lock (client);
  return res;
}

extern "C" int decafs_file_stat (uint32_t file_id, struct decafs_file_stat *buf,
                                 struct client client) {
  int res;
  if (get_metadata_lock (client) < 0) {
    return NO_METADATA_LOCK;
  }
  res = persistent_metadata.decafs_file_stat (file_id, buf);
  release_metadata_lock (client);
  return res;
}

extern "C" int decafs_stat (char *pathname, struct statvfs *buf,
                            struct client client) {
  int res;
  if (get_metadata_lock (client) < 0) {
    return NO_METADATA_LOCK;
  }
  res = persistent_metadata.decafs_stat (pathname, buf);
  release_metadata_lock (client);
  return res;
}

extern "C" int set_access_time (file_instance inst, struct timeval time,
                                struct client client) {
  int res;
  if (get_metadata_lock (client) < 0) {
    return NO_METADATA_LOCK;
  }
  res = persistent_metadata.set_access_time (inst, time);
  release_metadata_lock (client);
  return res;
}

extern "C" int add_file (char *pathname,
                         uint32_t stripe_size, uint32_t chunk_size,
                         uint32_t replica_size, struct timeval time,
                         struct client client) {
  int res;
  if (get_metadata_lock (client) < 0) {
    return NO_METADATA_LOCK;
  }
  res = persistent_metadata.add_file (pathname, stripe_size,
                                      chunk_size, replica_size, time);
  release_metadata_lock (client);
  return res;
}

extern "C" int delete_file_contents (uint32_t file_id, struct client client) {
  int res;
  if (get_metadata_lock (client) < 0) {
    return NO_METADATA_LOCK;
  }
  res = persistent_metadata.delete_file_contents (file_id);
  release_metadata_lock (client);
  return res;
}

extern "C" int update_file_size (uint32_t file_id, int size_delta,
                                 struct client client) {
  int res;
  if (get_metadata_lock (client) < 0) {
    return NO_METADATA_LOCK;
  }
  res = persistent_metadata.update_file_size (file_id, size_delta);
  release_metadata_lock (client);
  return res;
}

// ------------------------Volatile Metadata Call Throughs---------------------------
extern "C" uint32_t get_chunk_size () {
  return volatile_metadata.get_chunk_size();
}

extern "C" int set_chunk_size (uint32_t size) {
  return volatile_metadata.set_chunk_size (size);
}

extern "C" uint32_t get_stripe_size () {
  return volatile_metadata.get_stripe_size ();
}

extern "C" int set_stripe_size (uint32_t size) {
  return volatile_metadata.set_stripe_size(size);
}

extern "C" uint32_t get_num_espressos () {
  return volatile_metadata.get_num_espressos();
}

extern "C" int set_num_espressos (uint32_t num_espressos) {
  return volatile_metadata.set_num_espressos (num_espressos);
}

extern "C" uint32_t set_node_down (uint32_t node_number) {
  return volatile_metadata.set_node_down (node_number);
}

extern "C" uint32_t set_node_up (uint32_t node_number) {
  return volatile_metadata.set_node_up (node_number);
}

extern "C" bool is_node_up (uint32_t node_number) {
  return volatile_metadata.is_node_up (node_number);
}

extern "C" int get_active_node_count() {
  return volatile_metadata.get_active_node_count();
}

extern "C" struct active_nodes get_active_nodes () {
  return volatile_metadata.get_active_nodes ();
}

extern "C" int new_file_cursor (uint32_t file_id, struct client client) {
  return volatile_metadata.new_file_cursor (file_id, client);
}

extern "C" int close_file_cursor (uint32_t fd, struct client client) {
  return volatile_metadata.close_file_cursor (fd, client);
}

extern "C" int get_file_cursor (uint32_t fd) {
  return volatile_metadata.get_file_cursor (fd);
}

extern "C" int set_file_cursor (uint32_t fd, uint32_t offset, struct client client) {
  return volatile_metadata.set_file_cursor (fd, offset, client);
}

extern "C" struct file_instance get_file_info (uint32_t fd) {
  return volatile_metadata.get_file_info (fd);
}

extern "C" uint32_t get_new_request_id () {
  return volatile_metadata.get_new_request_id ();
}
// ------------------------Helper Functions-------------------------
/*
 * Determines the first stripe and stripe offset required for processing based
 * on the global offset, in context of stripe size.
 */
void get_first_stripe (uint32_t *id, int *stripe_offset, uint32_t stripe_size,
                       int offset) {
  int offset_remaining = offset;
  *id = STRIPE_ID_INIT;

  while (offset_remaining > (int)stripe_size) {
    (*id)++;
    offset_remaining -= stripe_size;
  }
  *stripe_offset = offset_remaining;
}

bool read_request_exists (uint32_t request_id) {
  return (active_read_requests.find (request_id) != active_read_requests.end());
}

bool write_request_exists (uint32_t request_id) {
  if (write_request_lookups.find (request_id) == write_request_lookups.end()) {
    return false;
  }
  struct write_request request = write_request_lookups[request_id];
  return (active_write_requests.find (request) != active_write_requests.end());
}

bool delete_request_exists (uint32_t request_id) {
  return (active_delete_requests.find (request_id) != active_delete_requests.end());
}

void check_read_complete (uint32_t request_id) {
  assert (read_request_exists (request_id));
  if (active_read_requests[request_id].info.chunks_expected == 0) {
    return;
  }

  if (active_read_requests[request_id].info.chunks_expected ==
      active_read_requests[request_id].info.chunks_received) {
    int count = 0;
    uint8_t *buffer_offset = active_read_requests[request_id].buf;
    std::map<struct file_chunk, ReadChunkResponse *> &packet_map = 
        active_read_requests[request_id].response_packets;
    std::map<struct file_chunk, ReadChunkResponse *>::iterator it = packet_map.begin();
    while (it != packet_map.end()) {
      ReadChunkResponse *cur_packet = it->second;
      memcpy (buffer_offset, cur_packet->data_buffer, cur_packet->count);
      buffer_offset += cur_packet->count;
      count += cur_packet->count;
      it++;
      delete (cur_packet);
    }
    if (send_read_result (active_read_requests[request_id].info.client,
                          active_read_requests[request_id].fd, count,
                          active_read_requests[request_id].buf) < 0) {
      printf ("\tRead result could not reach client.\n");
    }
    active_read_requests.erase (request_id);
  }
}

void check_write_complete (uint32_t request_id) {
  assert (write_request_exists (request_id));
  struct write_request request = write_request_lookups[request_id];
  
  if (active_write_requests[request].info.chunks_expected == 0) {
    return;
  }

  if (active_write_requests[request].info.chunks_expected ==
      active_write_requests[request].info.chunks_received &&
      active_write_requests[request].replica_info.chunks_expected ==
      active_write_requests[request].replica_info.chunks_received) {
    if (send_write_result (active_write_requests[request].info.client,
                           active_write_requests[request].fd,
                           active_write_requests[request].count) < 0) {
      printf ("\tWrite result could not reach client.\n");
    }
    active_write_requests.erase (request);
    write_request_lookups.erase (request.request_id);
    write_request_lookups.erase (request.replica_request_id);
  }
}

void check_delete_complete (uint32_t request_id) {
  assert (delete_request_exists (request_id));

  if (active_delete_requests[request_id].chunks_expected == 0) {
    return;
  }

  if (active_delete_requests[request_id].chunks_expected ==
      active_delete_requests[request_id].chunks_received) {
    uint32_t file_id = active_delete_requests[request_id].file_id;
    struct client client = active_delete_requests[request_id].client;

    if (send_delete_result (client, 0, 0) < 0) {
      printf ("\tDelete result could not reach client.\n");
    }
    
    while (delete_file_contents (file_id, client) == NO_METADATA_LOCK) {
      ; // retry metadata deletion until we succeed
    }
    
    // release the lock on the file
    release_lock (client, file_id);

    active_delete_requests.erase (request_id);
  
  }
}

// ------------------------Core Functions---------------------------
extern "C" void barista_core_init (int argc, char *argv[]) {
  int ret;
  if (argc < MIN_ARGS) {
    exit_failure (USAGE_ERROR);
  }

  io_manager.init (argv[METADATA]);
  persistent_metadata.init (argv[METADATA]);

  ret = set_stripe_size (atoi(argv[STRIPE_SIZE]));
  if (ret < 0) {
    exit_failure (get_size_error_message ("stripe", argv[STRIPE_SIZE]));
  }

  ret = set_chunk_size (atoi(argv[CHUNK_SIZE]));
  if (ret < 0) {
    exit_failure (get_size_error_message ("chunk", argv[CHUNK_SIZE]));
  }
 
  set_num_espressos (atoi(argv[NUM_ESPRESSOS]));
}

extern "C" const char *get_size_error_message (const char *type, const char *value) {
  std::string msg = "Invalid size ";
  msg += value;
  msg += " for ";
  msg += type;

  return msg.c_str();
}

extern "C" void exit_failure (const char *message) {
  fprintf (stderr, "%s\n", message);
  exit (EXIT_FAILURE);
}

extern "C" void open_file (const char *pathname, int flags, struct client client) {
  uint32_t file_id;
  struct decafs_file_stat stat;
  int cursor;

  // If the file does not exist
  if ((decafs_file_sstat ((char *)pathname, &stat, client)) == FILE_NOT_FOUND) {
    // If we are going to write to the file, create it
    if (flags & O_RDWR) {
      printf ("\tfile not found... creating now\n");  
      // Create the file
      struct timeval time;
      gettimeofday(&time, NULL);
                          // change 4th param to get_replica_size()
                          // implement in vmeta
      file_id = add_file ((char *)pathname, get_stripe_size(), get_chunk_size(),
                          get_chunk_size(), time, client);
    }
    // We can't read from nothing!
    else {
      if (send_open_result (client, FILE_NOT_FOUND_FOR_READING) < 0) {
        printf ("\tOpen result could not reach client.\n");
      }
      return;
   }
  }
  else {
    file_id = stat.file_id;
  }

  printf ("\tfile %s has id %d.\n", pathname, file_id);
  
  // If we're opening with read only, obtain a read lock
  if (flags & O_RDWR) {
    // if we can't get a write lock, return that the file is in use so we can't
    // open it
    if (get_exclusive_lock (client, file_id) < 0) {
      if (send_open_result (client, FILE_IN_USE) < 0) {
        printf ("\tOpen result could not reach client.\n");
      }
      return;
    }
    printf ("\tobtained a write lock.\n");
  }
  // obtain a write lock
  else {
    // if we can't get a read lock, return that the file is in use so we can't
    // open it
    if (get_shared_lock (client, file_id) < 0) {
      if (send_open_result (client, FILE_IN_USE) < 0) {
        printf ("\tOpen result could not reach client.\n");
      }
      return;
    }
    printf ("\tobtained a read lock.\n");
  }
  
  cursor = new_file_cursor (file_id, client);
  if (flags & O_APPEND) {
    printf ("\tfile opened with O_APPEND, moving cursor to EOF.\n");
    set_file_cursor (cursor, stat.size, client);
  }

  if (send_open_result (client, cursor) < 0) {
    printf ("\tOpen result could not reach client.\n");
  }
}

extern "C" void open_dir (const char* name, struct client client) {

}

extern "C" void read_file (int fd, size_t count, struct client client) {
  struct file_instance inst;
  struct decafs_file_stat stat;
  uint32_t stripe_id, num_chunks = 0;
  int file_offset, stripe_offset, bytes_read = 0, read_size = 0;
  uint8_t *buf;
  uint32_t request_id = get_new_request_id();

  assert (fd > 0);
  
  // Allocate space for the read request
  buf = (uint8_t *)malloc (count);

  inst = get_file_info((uint32_t)fd); 
  
  printf ("\n(BARISTA) Read request (%d bytes)\n", (int)count);
 
  // If the client does not have permission to read, return an error
  if (has_exclusive_lock (client, inst.file_id) <= 0) {
    if (has_shared_lock (client, inst.file_id) <= 0) {
      if (send_read_result (client, 0, FILE_NOT_OPEN_FOR_READ, NULL) < 0) {
        printf ("\tRead result could not reach client.\n");
      }
      return;
    }
  }
  
  if (decafs_file_stat (inst.file_id, &stat, client) < 0) {
    if (send_read_result (client, 0, UNABLE_TO_STAT_FILE, NULL) < 0) {
      printf ("\tRead result could not reach client.\n");
    }
    return;
  }
  
  if ((file_offset = get_file_cursor (fd)) < 0) {
    if (send_read_result (client, 0, FILE_NOT_OPEN_FOR_READ, NULL) < 0) {
      printf ("\tRead result could not reach client.\n");
    }
    return;
  }
  
  // TODO: make some assertion about max read size here
  // If we are trying to read past EOF, return 0 bytes read
  if (file_offset >= (int)stat.size) {
    if (send_read_result (client, fd, 0, NULL) < 0) {
      printf ("\tRead result could not reach client.\n");
    }
    return;
  }

  // Save the request id.
  active_read_requests[request_id] = read_request_info (client, inst.file_id,
                                                        fd, buf);  
  get_first_stripe (&stripe_id, &stripe_offset, stat.stripe_size,
                    file_offset);

  while (bytes_read < (int)count) {
    printf ("file cursor: %d\n", get_file_cursor(fd));
    if (count - bytes_read > stat.stripe_size - stripe_offset) {
      read_size = stat.stripe_size - stripe_offset;
    }
    else {
      read_size = count - bytes_read;
    }

    printf ("\t(request: %d) sending stripe (%d) information for processing (%d bytes)\n", 
               request_id, stripe_id, read_size);

    // TODO: add pathname here, get from persistent meta
    num_chunks += process_read_stripe (request_id, inst.file_id, (char *)"",
                                       stripe_id, stat.stripe_size,
                                       stat.chunk_size,
                                       (uint8_t *)buf + bytes_read,
                                       stripe_offset, read_size);

    set_file_cursor (fd, get_file_cursor (fd) + read_size, client);
    stripe_offset = 0;
    bytes_read += read_size;
    ++stripe_id;
  }
  
  assert (read_request_exists (request_id)); 
  active_read_requests[request_id].info.chunks_expected = num_chunks;
  check_read_complete(request_id);
}

extern "C" void read_response_handler (ReadChunkResponse *read_response) {
  assert (read_request_exists (read_response->id));
  
  struct file_chunk chunk = {read_response->file_id, read_response->stripe_id,
                             read_response->chunk_num};
  
  active_read_requests[read_response->id].info.chunks_received++;
  active_read_requests[read_response->id].response_packets[chunk] = read_response;

  check_read_complete(read_response->id);
}

extern "C" void write_file (int fd, const void *buf, size_t count, struct client client) {
  struct file_instance inst;
  struct decafs_file_stat stat;
  uint32_t stripe_id, num_chunks = 0, num_replica_chunks = 0;
  uint32_t chunks_written, replica_chunks_written;
  int file_offset, stripe_offset, bytes_written = 0, write_size = 0;
  uint32_t request_id = get_new_request_id();
  uint32_t replica_request_id = get_new_request_id();
  struct write_request request = {request_id, replica_request_id};

  assert (fd > 0);
  
  inst = get_file_info((uint32_t)fd); 

  printf ("\n(BARISTA) Write request (%d bytes) from file %d\n",
             (int)count, (int)inst.file_id);
  
  // If the client does not have permission to write, return an error
  if (has_exclusive_lock (client, inst.file_id) <= 0) {
    if (send_write_result (client, 0, FILE_NOT_OPEN_FOR_WRITE) < 0) {
      printf ("\tWrite result could not reach client.\n");
    }
    return;
  }

  if (decafs_file_stat (inst.file_id, &stat, client) < 0) {
    if (send_write_result (client, 0, UNABLE_TO_STAT_FILE) < 0) {
      printf ("\tWrite result could not reach client.\n");
    }
    return;
  }
  
  if ((file_offset = get_file_cursor (fd)) < 0) {
    if (send_write_result (client, 0, FILE_NOT_OPEN_FOR_WRITE) < 0) {
      printf ("\tWrite result could not reach client.\n");
    }
    return;
  }
  
  // Save the request id
  write_request_lookups[request_id] = request;
  write_request_lookups[replica_request_id] = request;
  active_write_requests[request] = write_request_info (client, inst.file_id,
                                                       fd);  
  
  // TODO: make some assertion about max write size here
  get_first_stripe (&stripe_id, &stripe_offset, stat.stripe_size, file_offset);
          
  while (bytes_written < (int)count) {
    if (count - bytes_written > stat.stripe_size - stripe_offset) {
      write_size = stat.stripe_size - stripe_offset;
    }
    else {
      write_size = count - bytes_written;
    }

    printf ("\t(request: (%d,%d)) sending stripe %d for processing (%d bytes)\n", 
               request_id, replica_request_id, stripe_id, write_size);
    // TODO: add pathname here, get from persistent meta
    process_write_stripe (request_id, replica_request_id,
                          &chunks_written, &replica_chunks_written,
                          inst.file_id, (char *)"", stripe_id,
                          stat.stripe_size, stat.chunk_size,
                          (uint8_t *)buf + bytes_written, stripe_offset,
                          write_size);
    
    num_chunks += chunks_written;
    num_replica_chunks += replica_chunks_written;
    
    // TODO (?): Move the file size update and cursor to check_write_complete()
    update_file_size (inst.file_id, write_size, client);
    set_file_cursor (fd, get_file_cursor (fd) + write_size, client);
    stripe_offset = 0;
    bytes_written += write_size;
    ++stripe_id;
  }
  assert (write_request_exists (request_id)); 
  active_write_requests[request].info.chunks_expected = num_chunks;
  active_write_requests[request].replica_info.chunks_expected =
                                              num_replica_chunks;
  check_write_complete(request_id);
}

extern "C" void write_response_handler (WriteChunkResponse *write_response) {
  assert (write_request_exists (write_response->id));
  struct write_request request = write_request_lookups[write_response->id];

  // If this is a primary chunk response
  if (write_response->id == request.request_id) {
    active_write_requests[request].info.chunks_received++;
    active_write_requests[request].count += write_response->count;
  }
  // Replica response
  else {
    active_write_requests[request].replica_info.chunks_received++;
  }
  check_write_complete(write_response->id);
}

extern "C" void close_file (int fd, struct client client) {
  int file_id = close_file_cursor (fd, client);
  
  // If we successfully closed the file, release the lock
  if (file_id > 0) {
    release_lock (client, file_id);
  }
  
  if (send_close_result (client, file_id) < 0) {
    printf ("\tClose result could not reach client.\n");
  }
}

extern "C" void delete_file (char *pathname, struct client client) {
  struct decafs_file_stat file_info;
  uint32_t num_chunks = 0, request_id = get_new_request_id();
  
  // If the file doesn't exist
  if ((decafs_file_sstat (pathname, &file_info, client)) < 0) {
    if (send_delete_result (client, 0, FILE_NOT_FOUND) < 0) {
      printf ("\tDelete result could not reach client.\n");
    }
    return;
  }
  
  if (get_exclusive_lock (client, file_info.file_id) < 0) {
    if (send_delete_result (client, 0, FILE_IN_USE) < 0) {
      printf ("\tDelete result could not reach client.\n");
    }
    return;
  }
 
  // Save the request id.
  active_delete_requests[request_id] = request_info (client, file_info.file_id); 
  printf ("(request: %d) processing delete file %s\n", request_id, pathname);
  num_chunks = process_delete_file (request_id, file_info.file_id);

  assert (delete_request_exists (request_id)); 
  active_delete_requests[request_id].chunks_expected = num_chunks;
  check_delete_complete(request_id);
}

extern "C" void delete_response_handler (DeleteChunkResponse *delete_response) {
  assert (delete_request_exists (delete_response->id));
  
  active_delete_requests[delete_response->id].chunks_received++;

  check_delete_complete(delete_response->id);
}

extern "C" int file_seek (int fd, uint32_t offset, int whence, struct client client) {
  int cursor_val;
  if (whence == SEEK_SET) {
    set_file_cursor (fd, 0, client);
  }
  if ((cursor_val = get_file_cursor (fd)) >= 0) {
    set_file_cursor (fd, cursor_val + offset, client);
  }
  return get_file_cursor (fd);
}

extern "C" int file_stat (const char *path, struct stat *buf) {

  return 0;
}

extern "C" int file_fstat (int fd, struct stat *buf) {

  return 0;
}

extern "C" void statfs (char *pathname, struct statvfs * stat) {

}

extern "C" void register_monitor_module (void (*monitor_module), 
                                         struct timeval timeout) {

}

extern "C" void register_node_failure_handler (void (*failure_handler)) {

}

extern "C" void register_chunk_metadata_handler (void (*metadata_handler)) {

}

extern "C" void register_chunk_replica_metadata_handler (void (*metadata_handler)) {

}

extern "C" int move_chunk (const char* pathname, uint32_t stripe_id, uint32_t chunk_num, 
                           uint32_t dest_node, struct client client) {

  return 0;
}

extern "C" int fmove_chunk (uint32_t file_id, uint32_t stripe_id, uint32_t chunk_num,
                            uint32_t dest_node, struct client client) {

  return 0;
}

extern "C" int move_chunk_replica (const char* pathname, uint32_t stripe_id, 
                                   uint32_t chunk_num, uint32_t dest_node,
                                   struct client client) {

  return 0;
}

extern "C" int fmove_chunk_replica (uint32_t file_id, uint32_t stripe_id,
                                    uint32_t chunk_num, uint32_t dest_node,
                                    struct client client) {

  return 0;
}

extern "C" int mk_decafs_dir (const char* dirname) {

  return 0;
}

extern "C" DIR* open_decafs_dir (const char* name) {

  return NULL;
}

extern "C" struct dirent* read_decafs_dir (DIR *dirp) {

  return NULL;
}
