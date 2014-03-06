#include "espresso_state.h"

espresso_global_data_t espresso_global_data;

void espresso_global_data_init(int fd, int max_size) {
    espresso_global_data.fd = fd;

    data_address whole_file = { .offset = 0, .size = max_size };
    espresso_global_data.free_extents.insert(whole_file);
}
