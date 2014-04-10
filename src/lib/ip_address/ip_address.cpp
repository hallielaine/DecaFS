#include "ip_address.h"

bool ip_is_null (struct ip_address ip) {
  return (strlen (ip.addr) == 0);
}

bool client_is_null (struct client client) {
  return ((ip_is_null (client.ip)) &&
         client.user_id == 0 && client.proc_id == 0);
}
