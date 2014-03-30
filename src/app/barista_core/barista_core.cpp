#include "barista_core.h"

#define MIN_ARGS 5
#define STRIPE_SIZE 1
#define CHUNK_SIZE 2
#define BARISTA 3
#define ESPRESSO 4

IO_Manager io_manager;
Persistent_Metadata persistent_metadata;
Volatile_Metadata volatile_metadata;

int main (int argc, char *argv[]) {
  process_arguments (argc, argv);

  // TODO: Recreate metadata from file

  // Wait for Espresso Nodes to finish startup

  // TODO: Change to proper logging
  printf ("Barista is initialized.\n");
  printf ("\tstripe_size: %d\n\tchunk_size: %d\n", 
           volatile_metadata.get_stripe_size(), volatile_metadata.get_chunk_size());
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

  // Add barista node to volatile_metadata
  // BARISTA param is log file

  //char bufer[100];

  // Add all espresso nodes to volatile_metadata
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

// ------------------------IO Manager Call Throughs ---------------------------
extern "C" ssize_t process_read_stripe (uint32_t file_id, char *pathname,
                                        uint32_t stripe_id, void *buf,
                                        size_t count) {
  return io_manager.process_read_stripe (file_id, pathname, stripe_id, buf, count);
}

extern "C" ssize_t process_write_stripe (uint32_t file_id, char *pathname,
                                         uint32_t stripe_id, void *buf,
                                         size_t count) {
  return io_manager.process_write_stripe (file_id, pathname, stripe_id, buf, count);
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

// ------------------------Persistent Metadata Call Throughs ---------------------------
extern "C" int get_num_files () {
  return persistent_metadata.get_num_files();
}

extern "C" int get_filenames (char *filenames[MAX_FILENAME_LENGTH], int size) {
  return persistent_metadata.get_filenames(filenames, size);
}

extern "C" int decafs_file_stat (char *pathname, struct decafs_file_stat *buf) {
  return persistent_metadata.decafs_file_stat (pathname, buf);
}

extern "C" int decafs_stat (char *pathname, struct statvfs *buf) {
  return persistent_metadata.decafs_stat (pathname, buf);
}

extern "C" int set_access_time (file_instance inst, struct timeval time) {
  return persistent_metadata.set_access_time (inst, time);
}

extern "C" int add_file (char *pathname, uint32_t file_id,
                         uint32_t stripe_size, uint32_t chunk_size,
                         uint32_t replica_size, struct timeval time) {
  return persistent_metadata.add_file (pathname, file_id, stripe_size,
                                       chunk_size, replica_size, time);
}

extern "C" int delete_file (uint32_t file_id) {
  return persistent_metadata.delete_file (file_id);
}

extern "C" int update_file_size (uint32_t file_id, int size_delta) {
  return persistent_metadata.update_file_size (file_id, size_delta);
}

// ------------------------Volatile Metadata Call Throughs ---------------------------
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

extern "C" int get_active_node_count() {
  return volatile_metadata.get_active_node_count();
}

extern "C" uint32_t get_active_nodes (char ***nodes) {
  return volatile_metadata.get_active_nodes (nodes);
}

extern "C" bool node_exists (uint32_t node_number) {
  return volatile_metadata.node_exists (node_number);
}

extern "C" int new_file_cursor (struct file_instance inst) {
  return volatile_metadata.new_file_cursor (inst);
}

extern "C" int close_file_cursor (struct file_instance inst) {
  return volatile_metadata.close_file_cursor (inst);
}

extern "C" int get_file_cursor (struct file_instance inst) {
  return volatile_metadata.get_file_cursor (inst);
}

extern "C" int set_file_cursor (struct file_instance inst, uint32_t offset) {
  return volatile_metadata.set_file_cursor (inst, offset);
}
