#ifndef __IP_ADDRESS_H__
#define __IP_ADDRESS_H__

#include <string.h>

#define IP_LENGTH 16

struct ip_address {
  char addr[IP_LENGTH];

  bool operator ==(const ip_address & other) const {
    return (strcmp (this->addr, other.addr) == 0);
  }
  
  bool operator <(const ip_address & other) const {
    return (strcmp (this->addr, other.addr) <= 0);
  }
};

void init_ip (struct ip_address *ip);

#endif
