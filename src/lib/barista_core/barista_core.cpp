#include "barista_core.h"

#define MIN_ARGS 5
#define STRIPE_SIZE 1
#define CHUNK_SIZE 2
#define METADATA 3
#define ESPRESSO 4

IO_Manager io_manager;
Persistent_Metadata persistent_metadata;
Volatile_Metadata volatile_metadata;

// ------------------------IO Manager Call Throughs---------------------------
extern "C" ssize_t process_read_stripe (uint32_t file_id, char *pathname,
                                        uint32_t stripe_id, uint32_t stripe_size,
                                        uint32_t chunk_size, const void *buf,
                                        int offset, size_t count) {
    return io_manager.process_read_stripe (file_id, pathname, stripe_id,
                                           stripe_size, chunk_size, buf,
                                           offset, count);
}

extern "C" ssize_t process_write_stripe (uint32_t file_id, char *pathname,
                                         uint32_t stripe_id, uint32_t stripe_size,
                                         uint32_t chunk_size, const void *buf,
                                         int offset, size_t count) {
  return io_manager.process_write_stripe (file_id, pathname, stripe_id,
                                          stripe_size, chunk_size, buf,
                                          offset, count);
}

extern "C" void process_delete_file (uint32_t file_id) {
  io_manager.process_delete_file (file_id);
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

// ------------------------Helper Functions-------------------------
void get_first_stripe (uint32_t *id, int *stripe_offset, uint32_t stripe_size,
                       int offset) {
  *id = STRIPE_ID_INIT;
  while (offset > (int)stripe_size) {
    (*id)++;
    offset -= stripe_size;
  }
  *stripe_offset = offset;
}

// ------------------------Core Functions---------------------------
extern "C" void barista_core_init (int argc, char *argv[]) {
  int ret;
  if (argc < MIN_ARGS) {
    exit_failure (USAGE_ERROR);
  }

  io_manager.init (argv[3]);
  persistent_metadata.init (argv[3]);

  ret = set_stripe_size (atoi(argv[STRIPE_SIZE]));
  if (ret < 0) {
    exit_failure (get_size_error_message ("stripe", argv[STRIPE_SIZE]));
  }

  ret = set_chunk_size (atoi(argv[CHUNK_SIZE]));
  if (ret < 0) {
    exit_failure (get_size_error_message ("chunk", argv[CHUNK_SIZE]));
  }

  // TODO: Recreate metadata from file
  // path to metadata file is in argv[METADATA]

  // Add all espresso nodes to volatile_metadata
  // TODO: retry connections to espresso nodes on failure
  // TODO: 3 times with increased wait each time
  /*for (int i = ESPRESSO; i < argc; i++) {
    int res = network_add_client(argv[i]);
    if (res >= 0) {
      add_node(argv[i], res);
    }
    else {
      fprintf(stderr, "Failed to connect to espresso node: %sn", argv[i]);
    }
  }*/
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


extern "C" int open_file (const char *pathname, int flags, struct client client) {
  uint32_t file_id;
  struct decafs_file_stat stat;
  int cursor;

  // If the file does not exist
  if ((decafs_file_sstat ((char *)pathname, &stat, client)) == FILE_NOT_FOUND) {
    printf ("\tfile not found... creating now\n");  
    // Create the file
    struct timeval time;
    gettimeofday(&time, NULL);
                        // change 4th param to get_replica_size()
                        // implement in vmeta
    file_id = add_file ((char *)pathname, get_stripe_size(), get_chunk_size(),
                        get_chunk_size(), time, client);
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
      return FILE_IN_USE;
    }
    printf ("\tobtained a write lock.\n");
  }
  // obtain a write lock
  else {
    // if we can't get a read lock, return that the file is in use so we can't
    // open it
    if (get_shared_lock (client, file_id) < 0) {
      return FILE_IN_USE;
    }
    printf ("\tobtained a read lock.\n");
  }
  
  cursor = new_file_cursor (file_id, client);
  if (flags & O_APPEND) {
    printf ("\tfile opened with O_APPEND, moving cursor to EOF.\n");
    set_file_cursor (cursor, stat.size, client);
  }
  return cursor;
}

extern "C" ssize_t read_file (int fd, void *buf, size_t count, struct client client) {
  struct file_instance inst;
  struct decafs_file_stat stat;
  uint32_t stripe_id;
  int file_offset, stripe_offset, bytes_read = 0, read_size = 0;

  assert (fd > 0);
  inst = get_file_info((uint32_t)fd); 

  printf ("\n(BARISTA) Read request (%d bytes)\n", (int)count);
 
  // If the client does not have permission to read, return an error
  if (has_exclusive_lock (client, inst.file_id) <= 0) {
    if (has_shared_lock (client, inst.file_id) <= 0) {
      return FILE_NOT_OPEN_FOR_READ;
    }
  }
  
  decafs_file_stat (inst.file_id, &stat, client);
  
  if ((file_offset = get_file_cursor (fd)) < 0) {
    return FILE_NOT_OPEN_FOR_READ; 
  }
  
  // TODO: make some assertion about max read size here
  get_first_stripe (&stripe_id, &stripe_offset, stat.stripe_size, file_offset);

  while (bytes_read < (int)count) {
    printf ("file cursor: %d\n", get_file_cursor(fd));
    if (count - bytes_read > stat.stripe_size - stripe_offset) {
      read_size = stat.stripe_size - stripe_offset;
    }
    else {
      read_size = count - bytes_read;
    }

    printf ("\tsending stripe (%d) information for processing (%d bytes)\n", 
               stripe_id, read_size);

    // TODO: add pathname here, get from persistent meta
    process_read_stripe (inst.file_id, (char *)"", stripe_id,
                         stat.stripe_size, stat.chunk_size, (uint8_t *)buf + bytes_read,
                         stripe_offset, read_size);

    set_file_cursor (fd, get_file_cursor (fd) + read_size, client);
    stripe_offset = 0;
    bytes_read += read_size;
    ++stripe_id;
  }
    
  return bytes_read;
}

extern "C" ssize_t write_file (int fd, const void *buf, size_t count, struct client client) {
  struct file_instance inst;
  struct decafs_file_stat stat;
  uint32_t stripe_id;
  int file_offset, stripe_offset, bytes_written = 0, write_size = 0;
  
  assert (fd > 0);
  inst = get_file_info((uint32_t)fd); 

  printf ("\n(BARISTA) Write request (%d bytes)\n", (int)count);
  
  // If the client does not have permission to write, return an error
  if (has_exclusive_lock (client, inst.file_id) <= 0) {
    return FILE_NOT_OPEN_FOR_WRITE; 
  }
 
  decafs_file_stat (inst.file_id, &stat, client);
  
  if ((file_offset = get_file_cursor (fd)) < 0) {
    return FILE_NOT_OPEN_FOR_WRITE; 
  }

  // TODO: make some assertion about max write size here
  get_first_stripe (&stripe_id, &stripe_offset, stat.stripe_size, file_offset);
          
  while (bytes_written < (int)count) {
    printf ("file cursor: %d\n", get_file_cursor(fd));
    if (count - bytes_written > stat.stripe_size - stripe_offset) {
      write_size = stat.stripe_size - stripe_offset;
    }
    else {
      write_size = count - bytes_written;
    }

    printf ("\tsending stripe %d for processing (%d bytes)\n", 
               stripe_id, write_size);
    // TODO: add pathname here, get from persistent meta
    process_write_stripe (inst.file_id, (char *)"", stripe_id,
                          stat.stripe_size, stat.chunk_size,
                          (uint8_t *)buf + bytes_written, stripe_offset,
                          write_size);

    update_file_size (inst.file_id, write_size, client);
    set_file_cursor (fd, get_file_cursor (fd) + write_size, client);
    stripe_offset = 0;
    bytes_written += write_size;
    ++stripe_id;
  }
  
  return bytes_written;
}

extern "C" int close_file (int fd, struct client client) {
  int file_id = close_file_cursor (fd, client);
  // If we successfully closed the file, release the lock
  if (file_id > 0) {
    release_lock (client, file_id);
  }
  return file_id;
}

extern "C" int delete_file (char *pathname, struct client client) {
  struct decafs_file_stat file_info;
  
  // If the file doesn't exist
  if ((decafs_file_sstat (pathname, &file_info, client)) < 0) {
    return FILE_NOT_FOUND;
  }
  
  if (has_exclusive_lock (client, file_info.file_id) <= 0) {
    if (get_exclusive_lock (client, file_info.file_id) < 0) {
      return FILE_IN_USE;
    }
  }
 
  process_delete_file (file_info.file_id);
  release_lock (client, file_info.file_id);
  
  return file_info.file_id;
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
