#ifndef ESPRESSO_TYPES_H_
#define ESPRESSO_TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 * The combination of these three identifiers uniquely identifies a
 * chunk stored on a single Espresso node.
 */
struct data_descriptor {
  int file_id;
  int stripe_id;
  int chunk_num;
};

/*
 * Addresses an extent of data within an Espresso storage file. The int
 * type has different sizes on different platforms, however it should be
 * assumed that these integers are no greater than 32 bits. Thus the
 * maximum offset is just short of 2GB.
 */
struct data_address {
  int offset;
  int size;
};

#ifdef __cplusplus
} // extern "C"
#endif

#endif // ESPRESSO_TYPES_H_
