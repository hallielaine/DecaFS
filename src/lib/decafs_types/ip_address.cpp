#include "ip_address.h"

bool is_ip_null (struct ip_address ip) {
  return (strlen (ip.addr) == 0);
}

bool is_client_null (struct client client) {
  return ((is_ip_null (client.ip)) ||
         client.user_id == 0 || client.ctc == NULL);
}
