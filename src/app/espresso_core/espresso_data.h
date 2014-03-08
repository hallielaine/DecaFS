#ifndef ESPRESSO_DATA_H_
#define ESPRESSO_DATA_H_

#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Reads data from the specified chunk. Fails if the chunk doesn't
 * exist, or if the range [offset, offset+count) falls outside the
 * bounds of the chunk.
 *
 * Returns the size read, as reported by read(2), or -1 on error.
 */
ssize_t read_data(int fd, int file_id, int stripe_id, int chunk_num,
    int offset, void *buf, int count);

/*
 * Writes data to the specified chunk. Fails if the chunk doesn't exist,
 * or if the range [offset, offset+count) falls outside the bounds of
 * the chunk.
 *
 * Returns the size written, as reported by write(2), or -1 on error.
 */
ssize_t write_data(int fd, int file_id, int stripe_id, int chunk_num,
    int offset, void *buf, int count);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // ESPRESSO_DATA_H_
