#include "distribution_strategy.h"

extern "C" int put_chunk (uint32_t file_id, char *pathname, uint32_t stripe_id,
                          uint32_t chunk_num) {
  int node_num;
  struct ip_address ip;

  if (chunk_num%2 == 1) {
    node_num = FIRST;  
  }
  else {
    node_num = SECOND;
  }
 
  ip = get_node_ip (node_num);
  
  printf ("Ip for node %d is %s\n", node_num, ip.addr);

  // If the ip address for the node cannot be found,
  // return an error
  if (is_ip_null (ip)) {
    return CANNOT_PUT_CHUNK;
  }

  if (is_node_up (ip.addr)) {
    return node_num;
  }
  return (node_num == FIRST) ? SECOND : FIRST; 
}