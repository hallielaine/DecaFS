#include "barista_core.h"

Volatile_Metadata::Volatile_Metadata() {
  // initialize volatile metadata map for file cursor

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

int Volatile_Metadata::add_node (char *ip, uint32_t node_number) {
  if (!ip_to_node_map_contains (ip)) {
    ip_to_node_map[ip] = node_number;
    up_nodes.insert (ip);
    return SUCCESS;
  }
  return IP_EXISTS;
}

uint32_t Volatile_Metadata::set_node_down (char *ip) {
  if (up_nodes_contains (ip)) {
    up_nodes.remove (ip);
    return SUCCESS;
  }
  return IP_NOT_FOUND;
}

uint32_t Volatile_Metadata::get_active_nodes (char **nodes) {
  int count = 0;
  *nodes = realloc (up_nodes.size());

  for (std::list<string>::iterator it = up_nodes.being(); 
       it != up_nodes.end(); it++) {
    nodes[count] = malloc (*it.length());
    memcpy (nodes[count++], (*it.c_str()), *it.length());
  }

  return count;
}

bool Volatile_Metadata::ip_to_node_map_contains (char *ip) {
  return (ip_to_node_map.find (ip) == ip_to_node_map.end());
}

bool Volatile_Metadata::up_nodes_contains (char *ip) {
  return (std::find (up_nodes.begin(), up_nodes.end(), ip) != up_nodes.end());
}
