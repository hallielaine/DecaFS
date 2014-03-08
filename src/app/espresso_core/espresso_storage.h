#ifndef ESPRESSO_STORAGE_H_
#define ESPRESSO_STORAGE_H_

#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Reads *count* bytes from the chunk at offset *offset* into *buf.
 * Fails if the chunk doesn't exist, or if the range [offset,
 * offset+count) falls outside the bounds of the chunk.
 *
 * Returns the size read, as reported by read(2), or -1 on error.
 */
ssize_t read_chunk(int fd, int file_id, int stripe_id, int chunk_num,
    int offset, void *buf, int count);

/*
 * Writes *count* bytes from *buf* to the chunk at offset *offset*.
 * Creates a new chunk if it doesn't exist, and resizes the chunk if the
 * range [offset, offset+count) falls outside the exsiting bounds of
 * the chunk.
 *
 * Returns the size written, as reported by write(2), or -1 on error.
 */
ssize_t write_chunk(int fd, int file_id, int stripe_id, int chunk_num,
    int offset, void *buf, int count);

/*
 * Deletes a chunk, freeing the space it occupied for future use. Fails
 * if the chunk doesn't exist.
 *
 * Returns 0 on success, or -1 on error.
 */
int delete_chunk(int fd, int file_id, int stripe_id, int chunk_num);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // ESPRESSO_STORAGE_H_
