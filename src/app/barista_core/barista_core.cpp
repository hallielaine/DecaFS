#include "barista_core.h"

#define MIN_ARGS 5
#define STRIPE_SIZE 1
#define CHUNK_SIZE 2
#define METADATA 3
#define ESPRESSO 4

IO_Manager io_manager;
Persistent_Metadata persistent_metadata;
Volatile_Metadata volatile_metadata;

int main (int argc, char *argv[]) {
  process_arguments (argc, argv);

  // TODO: Change to proper logging
  printf ("Barista is initialized.\n");
  printf ("\tstripe_size: %d\n\tchunk_size: %d\n\n", 
           volatile_metadata.get_stripe_size(), volatile_metadata.get_chunk_size());
   
  struct ip_address ip;
  struct client default_client = {ip, 1, 1};
  int fd = open ("new_file.txt", O_RDWR, default_client);
  char buf[] = "1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63 64 65 66 67 68 69 70 71 72 73 74 75 76 77 78 79 80 81 82 83 84 85 86 87 88 89 90 91 92 93 94 95 96 97 98 99 100";
  char read_buf[100];
  memset (read_buf, '\0', 100);
  write (fd, buf, strlen (buf), default_client);

  printf ("Sleeping for 10 seconds\n");
  sleep (10);
  printf ("Good morning.\n\n");

  int count = read (fd, read_buf, strlen (buf), default_client);
  printf ("\n(BARISTA) Read %d bytes.\n", count);
  printf ("(BARISTA) Buf is:\n%s\n", read_buf);
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

  // TODO: Recreate metadata from file
  // path to metadata file is in argv[METADATA]

  // Add all espresso nodes to volatile_metadata
  // TODO: retry connections to espresso nodes on failure
  // TODO: 3 times with increased wait each time
  for (int i = ESPRESSO; i < argc; i++) {
    int res = network_add_client(argv[i]);
    if (res >= 0) {
      volatile_metadata.add_node(argv[i], res);
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
int open (const char *pathname, int flags, struct client client) {
  uint32_t file_id;
  struct decafs_file_stat stat;
  
  printf ("\n(BARISTA) Opening file %s\n", pathname);

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
  if (flags == O_RDONLY) {
    // if we can't get a read lock, return that the file is in use so we can't
    // open it
    if (get_shared_lock (client.user_id, client.proc_id, file_id) < 0) {
      return FILE_IN_USE;
    }
  }
  // obtain a write lock
  else {
    // if we can't get a write lock, return that the file is in use so we can't
    // open it
    if (get_exclusive_lock (client.user_id, client.proc_id, file_id) < 0) {
      return FILE_IN_USE;
    }
  }
  
  return new_file_cursor (file_id, client);
}

ssize_t read (int fd, void *buf, size_t count, struct client client) {
  struct file_instance inst;
  struct decafs_file_stat stat;
  uint32_t stripe_id;
  int file_offset, stripe_offset, bytes_read = 0, read_size = 0;

  assert (fd > 0);
  inst = get_file_info((uint32_t)fd); 

  printf ("\n(BARISTA) Read request (%d bytes)\n", (int)count);
 
  // If the client does not have permission to read, return an error
  if (has_exclusive_lock (client.user_id, client.proc_id, inst.file_id) <= 0) {
    if (has_shared_lock (client.user_id, client.proc_id, inst.file_id) <= 0) {
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
                          stat.stripe_size, stat.chunk_size, buf,
                          stripe_offset, read_size);

    stripe_offset = 0;
    bytes_read += read_size;
    ++stripe_id;
  }
   
  return bytes_read;
}

ssize_t write (int fd, const void *buf, size_t count, struct client client) {
  struct file_instance inst;
  struct decafs_file_stat stat;
  uint32_t stripe_id;
  int file_offset, stripe_offset, bytes_written = 0, write_size = 0;
  
  assert (fd > 0);
  inst = get_file_info((uint32_t)fd); 

  printf ("\n(BARISTA) Write request (%d bytes)\n", (int)count);
  
  // If the client does not have permission to write, return an error
  if (has_exclusive_lock (client.user_id, client.proc_id, inst.file_id) <= 0) {
    return FILE_NOT_OPEN_FOR_WRITE; 
  }
 
  decafs_file_stat (inst.file_id, &stat, client);
  
  if ((file_offset = get_file_cursor (fd)) < 0) {
    return FILE_NOT_OPEN_FOR_WRITE; 
  }

  // TODO: make some assertion about max write size here
  get_first_stripe (&stripe_id, &stripe_offset, stat.stripe_size, file_offset);
          
  while (bytes_written < (int)count) {
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
                          stat.stripe_size, stat.chunk_size, buf,
                          stripe_offset, write_size);

    stripe_offset = 0;
    bytes_written += write_size;
    ++stripe_id;
  }
   
  return bytes_written;
}

int close (int fd, struct client client) {
  int file_id = close_file_cursor (fd, client);
  // If we successfully closed the file, release the lock
  if (file_id > 0) {
    release_lock (client.user_id, client.proc_id, file_id);
  }
  return file_id;
}

int delete_file (char *pathname, struct client client) {
  struct decafs_file_stat file_info;
  
  // If the file doesn't exist
  if ((decafs_file_sstat (pathname, &file_info, client)) < 0) {
    return FILE_NOT_FOUND;
  }

  if (get_exclusive_lock (client.user_id, client.proc_id,
                          file_info.file_id) < 0) {
    return FILE_IN_USE;
  }
 
  process_delete_file (file_info.file_id);
  release_lock (client.user_id, client.proc_id, file_info.file_id);
  
  return file_info.file_id;
}

int file_stat (const char *path, struct stat *buf) {

  return 0;
}

int file_fstat (int fd, struct stat *buf) {

  return 0;
}

void statfs (char *pathname, struct statvfs * stat) {

}

void register_monitor_module (void (*monitor_module), 
                              struct timeval timeout) {

}

void register_node_failure_handler (void (*failure_handler)) {

}

void register_chunk_metadata_handler (void (*metadata_handler)) {

}

void register_chunk_replica_metadata_handler (void (*metadata_handler)) {

}

int move_chunk (const char* pathname, uint32_t stripe_id, uint32_t chunk_num, 
                 uint32_t dest_node, struct client client) {

  return 0;
}

int fmove_chunk (uint32_t file_id, uint32_t stripe_id, uint32_t chunk_num,
                  uint32_t dest_node, struct client client) {

  return 0;
}

int move_chunk_replica (const char* pathname, uint32_t stripe_id, 
                         uint32_t chunk_num, uint32_t dest_node,
                         struct client client) {

  return 0;
}

int fmove_chunk_replica (uint32_t file_id, uint32_t stripe_id,
                          uint32_t chunk_num, uint32_t dest_node,
                          uint32_t user_id, uint32_t proc_id) {

  return 0;
}

int mkdir (const char* dirname) {

  return 0;
}

DIR* opendir (const char* name) {

  return NULL;
}

struct dirent* readdir (DIR *dirp) {

  return NULL;
}

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
  if (get_metadata_lock (client.user_id, client.proc_id) < 0) {
    return NO_METADATA_LOCK;
  }
  res = persistent_metadata.get_num_files();
  release_metadata_lock (client.user_id, client.proc_id);
  return res;
}

extern "C" int get_filenames (char *filenames[MAX_FILENAME_LENGTH], int size,
                              struct client client) {
  int res;
  if (get_metadata_lock (client.user_id, client.proc_id) < 0) {
    return NO_METADATA_LOCK;
  }
  res = persistent_metadata.get_filenames(filenames, size);
  release_metadata_lock (client.user_id, client.proc_id);
  return res;
}

extern "C" int decafs_file_sstat (char *pathname, struct decafs_file_stat *buf,
                                 struct client client) {
  int res;
  if (get_metadata_lock (client.user_id, client.proc_id) < 0) {
    return NO_METADATA_LOCK;
  }
  res = persistent_metadata.decafs_file_sstat (pathname, buf);
  release_metadata_lock (client.user_id, client.proc_id);
  return res;
}

extern "C" int decafs_file_stat (uint32_t file_id, struct decafs_file_stat *buf,
                                 struct client client) {
  int res;
  if (get_metadata_lock (client.user_id, client.proc_id) < 0) {
    return NO_METADATA_LOCK;
  }
  res = persistent_metadata.decafs_file_stat (file_id, buf);
  release_metadata_lock (client.user_id, client.proc_id);
  return res;
}

extern "C" int decafs_stat (char *pathname, struct statvfs *buf,
                            struct client client) {
  int res;
  if (get_metadata_lock (client.user_id, client.proc_id) < 0) {
    return NO_METADATA_LOCK;
  }
  res = persistent_metadata.decafs_stat (pathname, buf);
  release_metadata_lock (client.user_id, client.proc_id);
  return res;
}

extern "C" int set_access_time (file_instance inst, struct timeval time,
                                struct client client) {
  int res;
  if (get_metadata_lock (client.user_id, client.proc_id) < 0) {
    return NO_METADATA_LOCK;
  }
  res = persistent_metadata.set_access_time (inst, time);
  release_metadata_lock (client.user_id, client.proc_id);
  return res;
}

extern "C" int add_file (char *pathname,
                         uint32_t stripe_size, uint32_t chunk_size,
                         uint32_t replica_size, struct timeval time,
                         struct client client) {
  int res;
  if (get_metadata_lock (client.user_id, client.proc_id) < 0) {
    return NO_METADATA_LOCK;
  }
  res = persistent_metadata.add_file (pathname, stripe_size,
                                      chunk_size, replica_size, time);
  release_metadata_lock (client.user_id, client.proc_id);
  return res;
}

extern "C" int delete_file (uint32_t file_id, struct client client) {
  int res;
  if (get_metadata_lock (client.user_id, client.proc_id) < 0) {
    return NO_METADATA_LOCK;
  }
  res = persistent_metadata.delete_file (file_id);
  release_metadata_lock (client.user_id, client.proc_id);
  return res;
}

extern "C" int update_file_size (uint32_t file_id, int size_delta,
                                 struct client client) {
  int res;
  if (get_metadata_lock (client.user_id, client.proc_id) < 0) {
    return NO_METADATA_LOCK;
  }
  res = persistent_metadata.update_file_size (file_id, size_delta);
  release_metadata_lock (client.user_id, client.proc_id);
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

extern "C" uint32_t get_node_number (char *ip) {
  return volatile_metadata.get_node_number (ip);
}

extern "C" struct ip_address get_node_ip (uint32_t node_number) {
  return volatile_metadata.get_node_ip (node_number);
}

extern "C" int add_node (char *ip, uint32_t node_number) {
  return volatile_metadata.add_node (ip, node_number);
}

extern "C" uint32_t set_node_down (char *ip) {
  return volatile_metadata.set_node_down (ip);
}

extern "C" uint32_t set_node_up (char *ip) {
  return volatile_metadata.set_node_up (ip);
}

extern "C" bool is_node_up (char *ip) {
  return volatile_metadata.is_node_up (ip);
}

extern "C" int get_active_node_count() {
  return volatile_metadata.get_active_node_count();
}

extern "C" uint32_t get_active_nodes (char ***nodes) {
  return volatile_metadata.get_active_nodes (nodes);
}

extern "C" bool node_exists (uint32_t node_number) {
  return volatile_metadata.node_exists (node_number);
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
