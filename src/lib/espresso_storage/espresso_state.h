#ifndef ESPRESSO_STATE_H_
#define ESPRESSO_STATE_H_

#include <map>
#include <set>

#include "espresso_types.h"

/*
 * Global data required by the Espresso storage module.
 */
struct espresso_global_data_t {
  int fd;
  std::map<data_descriptor, data_address> metadata;
  std::set<data_address> free_extents;
};

extern espresso_global_data_t espresso_global_data;

/*
 * Initialize the Espresso storage module global data. The storage
 * module will use *fd* as the file descriptor for the data file, and
 * will not allow it to grow beyond *max_size* bytes.
 *
 * TODO: Once metadata is persistent, this will need to be split into
 * two functions, one for initializing an new file, and one for
 * restoring an existing one.
 */
void espresso_global_data_init(int fd, int max_size);

/*
 * Comparison operator for data_descriptors, for STL ordered
 * collections.
 */
inline bool operator< (const data_descriptor &d1, const data_descriptor &d2) {
  if (d1.file_id != d2.file_id)
    return d1.file_id < d2.file_id;
  if (d1.stripe_id != d2.stripe_id)
    return d1.stripe_id < d2.stripe_id;
  return d1.chunk_num < d2.chunk_num;
}

/*
 * Comparison operator for data_addresses, for STL ordered collections.
 * Size is not part of the comparison, as it would be meaningless to do
 * so.
 */
inline bool operator< (const data_address &a1, const data_address &a2) {
  return a1.offset < a2.offset;
}

#endif // ESPRESSO_STATE_H_
