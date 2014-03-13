#ifndef _ESP_SVC_H
#define _ESP_SVC_H

#include <stdio.h>
#include <string.h>

#include "network.h"

int result;


#ifdef __cplusplus
extern "C" {
#endif

typedef ssize_t (*read_data_callback)(int, int, int, int, int, void*, int);
typedef ssize_t (*write_data_callback)(int, int, int, int, int, void*, int);
typedef int (*delete_data_callback)(int, int, int);

read_data_callback read_cb;
write_data_callback write_cb;
delete_data_callback delete_cb;

extern void register_read_data_callback(read_data_callback callback);
extern void register_write_data_callback(write_data_callback callback);
extern void register_delete_data_callback(delete_data_callback callback);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // _ESP_SVC_H
