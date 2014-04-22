#include "volatile_metadata.h"

Volatile_Metadata::Volatile_Metadata() {
  // initialize volatile metadata map for file cursor
  last_fd = FD_NOT_SET;

  // Initialize stripe and chunk size
  chunk_size = 0;
  stripe_size = 0;
}

int Volatile_Metadata::set_chunk_size (uint32_t size) {
  // Set the chunk size if it hasn't been set before
  if (chunk_size == 0) {
    if (size >= MIN_CHUNK && size <= MAX_CHUNK) {
      chunk_size = size;
      return 1;
    }
    return SIZE_INVALID;
  }
  return SIZE_ALREADY_SET;
}

uint32_t Volatile_Metadata::get_chunk_size() {
  return chunk_size;
}

int Volatile_Metadata::set_stripe_size (uint32_t size) {
  // Set the stripe size if it hasn't been set before
  if (stripe_size == 0) {
    if (size >= MIN_STRIPE && size <= MAX_STRIPE) {
      stripe_size = size;
      return 1;
    }
    return SIZE_INVALID;
  }
  return SIZE_ALREADY_SET;
}

uint32_t Volatile_Metadata::get_stripe_size() {
  return stripe_size;
}

uint32_t Volatile_Metadata::set_node_down (uint32_t node_number) {
  if (up_nodes_contains (node_number)) {
    printf ("(BARISTA) Setting node %d to DOWN\n", node_number);
    up_nodes.remove (node_number);
    return V_META_SUCCESS;
  }
  return NODE_NUMBER_NOT_FOUND;
}

uint32_t Volatile_Metadata::set_node_up (uint32_t node_number) {
  if (!up_nodes_contains (node_number)) {
    printf ("(BARISTA) Setting node %d to UP\n", node_number);
    up_nodes.push_back (node_number);
    return V_META_SUCCESS;
  }
  return NODE_ALREADY_UP;
}

bool Volatile_Metadata::is_node_up (uint32_t node_number) {
  if (up_nodes_contains (node_number)) {
    return true;
  }
  return false;
}

int Volatile_Metadata::get_active_node_count() {
  return up_nodes.size();
}

struct active_nodes Volatile_Metadata::get_active_nodes () {
  int count = 0;
  struct active_nodes nodes;

  up_nodes.sort();

  for (std::list<uint32_t>::iterator it = up_nodes.begin(); 
       it != up_nodes.end(); it++) {
    nodes.node_numbers[count++] = *it;
  }
  nodes.active_node_count = count;

  return nodes;
}

int Volatile_Metadata::new_file_cursor (uint32_t file_id,
                                        struct client client) {
  uint32_t fd = get_new_fd();
  struct file_instance inst = {client, file_id, 0};
  file_cursors[fd] = inst;
  return fd;
}

int Volatile_Metadata::close_file_cursor (uint32_t fd,
                                          struct client client) {
  struct file_instance file;
  if (file_cursors_contains (fd)) {
    file = file_cursors[fd];
    if (file.client_id == client) {
      file_cursors.erase(fd);
      return file.file_id;
    }
    else {
      return WRONG_CLIENT;
    }
  }
  return INSTANCE_NOT_FOUND;
}

int Volatile_Metadata::get_file_cursor (uint32_t fd) {
  if (file_cursors_contains (fd)) {
    return file_cursors[fd].offset;
  }
  return INSTANCE_NOT_FOUND;
}

int Volatile_Metadata::set_file_cursor (uint32_t fd, uint32_t offset,
                                        struct client client) {
  struct file_instance inst;
  struct decafs_file_stat info;
  
  if (file_cursors_contains (fd)) {
    if (file_cursors[fd].client_id == client) {
      // Ensure that the file cursor is set to end of file at most
      inst = get_file_info (fd);
      decafs_file_stat (inst.file_id, &info, client);
      if (offset > info.size) {
        offset = info.size;
      }

      file_cursors[fd].offset = offset;
      return offset;
    }
    else {
      return WRONG_CLIENT;
    }
  }
  return INSTANCE_NOT_FOUND;
}
 
struct file_instance Volatile_Metadata::get_file_info (uint32_t fd) {
  struct file_instance inst;
  if (file_cursors_contains (fd)) {
    return file_cursors[fd];
  }
  return inst;
}

bool Volatile_Metadata::up_nodes_contains (uint32_t node_number) {
  return (std::find (up_nodes.begin(), up_nodes.end(), node_number)
          != up_nodes.end());
}

bool Volatile_Metadata::file_cursors_contains (uint32_t fd) {
  return (file_cursors.find (fd) != file_cursors.end());
}

uint32_t Volatile_Metadata::get_new_fd() {
  uint32_t new_fd;

  fd_mutex.lock();
  // If we don't know the current fd, find the max
  if (last_fd == FD_NOT_SET) {
    if (!file_cursors.empty()) {
      last_fd = file_cursors.rbegin()->first;
    }
  }
  
  new_fd = ++last_fd;
  fd_mutex.unlock();

  return new_fd;
}
