#include "espresso_state.h"

espresso_global_data_t espresso_global_data;

void espresso_global_data_init(int fd) {
    espresso_global_data.fd = fd;
}
