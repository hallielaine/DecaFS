#include <unistd.h>

#include <algorithm>

#include "espresso_data.h"
#include "espresso_types.h"
#include "espresso_state.h"


#define UNUSED __attribute__((unused))

extern "C"
ssize_t read_data(int fd UNUSED, int file_id, int stripe_id, int chunk_num,
        int offset, void *buf, int count) {
    if (count < 0)
        return -1;

    data_descriptor d = { .file_id = file_id,
                          .stripe_id = stripe_id,
                          .chunk_num = chunk_num };

    auto a = espresso_global_data.metadata.find(d);

    // chunk not found
    if (a == espresso_global_data.metadata.end())
        return -1;

    // do not read past the end of the chunk
    if (count > a->second.size - offset)
        return -1;

    // seek to chunk at offset
    if (lseek(espresso_global_data.fd, a->second.offset + offset, SEEK_SET) == -1)
        return -1;

    // read len bytes of chunk starting from offset
    return read(espresso_global_data.fd, buf, count);
}

extern "C"
ssize_t write_data(int fd UNUSED, int file_id, int stripe_id, int chunk_num,
        int offset, void *buf, int count) {
    if (count < 0)
        return -1;

    data_descriptor d = { .file_id = file_id,
                          .stripe_id = stripe_id,
                          .chunk_num = chunk_num };

    auto a = espresso_global_data.metadata.find(d);

    // chunk not found
    if (a == espresso_global_data.metadata.end())
        return -1;

    // do not write past the end of the chunk
    if (count > a->second.size - offset)
        return -1;

    // seek to chunk at offset
    if (lseek(espresso_global_data.fd, a->second.offset + offset, SEEK_SET) == -1)
        return -1;

    // write len bytes of chunk starting from offset
    return write(espresso_global_data.fd, buf, count);
}
