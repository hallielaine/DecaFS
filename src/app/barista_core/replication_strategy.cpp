#include "replication_strategy.h"

extern "C" int put_replica (uint32_t file_id, char *pathname,
                            uint32_t stripe_id, uint32_t chunk_num) {
  int node_num;
  struct ip_address ip;

  if (chunk_num%2 == 1) {
    node_num = FIRST_REPLICA;  
  }
  else {
    node_num = SECOND_REPLICA;
  }
 
  ip = get_node_ip (node_num);

  
  // If the ip address for the node cannot be found,
  // return an error
  if (is_ip_null (ip)) {
    return CANNOT_PUT_REPLICA;
  }

  if (is_node_up (ip.addr)) {
    return node_num;
  }
  return (node_num == FIRST_REPLICA) ? SECOND_REPLICA : FIRST_REPLICA; 
}

