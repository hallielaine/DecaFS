#include "ip_address.h"

void init_ip (struct ip_address *ip) {
  memset (ip->addr, '\0', IP_LENGTH);
}
