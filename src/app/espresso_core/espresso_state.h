#ifndef ESPRESSO_STATE_H_
#define ESPRESSO_STATE_H_

#include <map>
#include <set>

#include "espresso_types.h"

struct espresso_global_data_t {
    int fd;
    std::map<data_descriptor, data_address> metadata;
    std::set<data_address> free_extents;
};

void espresso_global_data_init(int fd);

inline bool operator< (const data_descriptor &d1, const data_descriptor &d2) {
    if (d1.file_id != d2.file_id)
        return d1.file_id < d2.file_id;
    if (d1.stripe_id != d2.stripe_id)
        return d1.stripe_id < d2.stripe_id;
    return d1.chunk_num < d2.chunk_num;
}

inline bool operator< (const data_address &a1, const data_address &a2) {
    return a1.offset < a2.offset;
}

extern espresso_global_data_t espresso_global_data;

#endif // ESPRESSO_STATE_H_
