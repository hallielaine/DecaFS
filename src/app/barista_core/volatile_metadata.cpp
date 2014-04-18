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

uint32_t Volatile_Metadata::get_node_number (char *ip) {
  if (ip_to_node_map_contains (ip)) {
    return ip_to_node_map[ip];
  }
  return IP_NOT_FOUND;
}

struct ip_address Volatile_Metadata::get_node_ip (uint32_t node_number) {
  struct ip_address ip;
  
  for (std::map<string, int>::iterator it = ip_to_node_map.begin();
       it != ip_to_node_map.end(); it++) {
    if ((it->second) == (int)node_number) {
      strcpy (ip.addr, it->first.c_str());
      break;
    }
  }
  return ip;
}

int Volatile_Metadata::add_node (char *ip, uint32_t node_number) {
  if (!ip_to_node_map_contains (ip)) {
    ip_to_node_map[ip] = node_number;
    up_nodes.push_back (std::string(ip));
    return V_META_SUCCESS;
  }
  return IP_EXISTS;
}

uint32_t Volatile_Metadata::set_node_down (char *ip) {
  if (up_nodes_contains (ip)) {
    printf ("(BARISTA) Setting node %s to DOWN\n", ip);
    up_nodes.remove (ip);
    return V_META_SUCCESS;
  }
  return IP_NOT_FOUND;
}

uint32_t Volatile_Metadata::set_node_up (char *ip) {
  if (up_nodes_contains (ip)) {
    return V_META_SUCCESS;
  }
  else if (ip_to_node_map_contains (ip)) {
    printf ("(BARISTA) Setting node %s to UP\n", ip);
    up_nodes.push_back (ip);
    return V_META_SUCCESS;
  }
  return IP_NOT_FOUND;
}

bool Volatile_Metadata::is_node_up (char *ip) {
  if (up_nodes_contains (ip)) {
    return true;
  }
  return false;
}

int Volatile_Metadata::get_active_node_count() {
  return up_nodes.size();
}

uint32_t Volatile_Metadata::get_active_nodes (char ***nodes) {
  int count = 0;
  *nodes = (char **)(realloc (*nodes, up_nodes.size()));
 
  up_nodes.sort();

  for (std::list<string>::iterator it = up_nodes.begin(); 
       it != up_nodes.end(); it++) {
    (*nodes)[count] = (char *)(malloc ((*it).length() + 1));
    std::strcpy ((*nodes)[count++], ((*it).c_str()));
  }

  return count;
}

bool Volatile_Metadata::node_exists (uint32_t node_number) {
  for (std::map<string, int>::iterator it = ip_to_node_map.begin();
       it != ip_to_node_map.end(); it++) {
    if ((it->second) == (int)node_number) {
      return true;
    }
  }
  return false;
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
  if (file_cursors_contains (fd)) {
    if (file_cursors[fd].client_id == client) {
      file_cursors.erase(fd);
    }
    else {
      return WRONG_CLIENT;
    }
  }
  return V_META_SUCCESS;
}

int Volatile_Metadata::get_file_cursor (uint32_t fd) {
  if (file_cursors_contains (fd)) {
    return file_cursors[fd].offset;
  }
  return INSTANCE_NOT_FOUND;
}

int Volatile_Metadata::set_file_cursor (uint32_t fd, uint32_t offset,
                                        struct client client) {
  if (file_cursors_contains (fd)) {
    if (file_cursors[fd].client_id == client) {
      // TODO: Add error checking to ensure that only valid offsets can be set
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

bool Volatile_Metadata::ip_to_node_map_contains (char *ip) {
  return (ip_to_node_map.find (ip) != ip_to_node_map.end());
}

bool Volatile_Metadata::up_nodes_contains (char *ip) {
  return (std::find (up_nodes.begin(), up_nodes.end(), ip) != up_nodes.end());
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
