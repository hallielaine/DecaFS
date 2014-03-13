#include "esp_svc.h"

int result;

void register_read_data_callback(read_data_callback callback) {
  read_cb = callback;
}

void register_write_data_callback(write_data_callback callback) {
  write_cb = callback;
}

void register_delete_data_callback(delete_data_callback callback) {
  delete_cb = callback;
}

read_rtn* rpc_read_data_1_svc(read_params *info, struct svc_req *req) {

   printf("read data called\n");
   read_rtn result;

   result.buf = "this is the read data";
   result.length = strlen(result.buf);

   read_cb(1, 1, 1, 1, 1, NULL, 1);

   return &result;
}

int* rpc_write_data_1_svc(write_params *info, struct svc_req *req) {

   printf("write data called: %s\n", info->buf);

   result = 1;
   return &result;
}

int* rpc_delete_data_1_svc(delete_params *info, struct svc_req *req) {

   printf("delete data called\n");
   result = 1;

   return &result;
}

