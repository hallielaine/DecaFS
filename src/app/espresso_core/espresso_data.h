#ifndef ESPRESSO_DATA_H_
#define ESPRESSO_DATA_H_

#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

ssize_t read_data(int fd, int file_id, int stripe_id, int chunk_num,
        int offset, void *buf, int count);

ssize_t write_data(int fd, int file_id, int stripe_id, int chunk_num,
        int offset, void *buf, int count);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // ESPRESSO_DATA_H_
