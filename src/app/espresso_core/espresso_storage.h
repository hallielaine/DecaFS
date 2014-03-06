#ifndef ESPRESSO_STORAGE_H_
#define ESPRESSO_STORAGE_H_

#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

ssize_t read_chunk(int fd, int file_id, int stripe_id, int chunk_num,
    int offset, void *buf, int count);

ssize_t write_chunk(int fd, int file_id, int stripe_id, int chunk_num,
    int offset, void *buf, int count);

int delete_chunk(int fd, int file_id, int stripe_id, int chunk_num);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // ESPRESSO_STORAGE_H_
