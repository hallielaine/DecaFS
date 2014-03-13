#include "esp_svc.h"

read_data_callback read_cb = NULL;
write_data_callback write_cb = NULL;
delete_data_callback delete_cb = NULL;

int result;
read_rtn read_result;

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

   if (read_result.buf == NULL) { // hasnt been mallocd
      read_result.buf = (char*)malloc(65536*sizeof(char));
   }

   if (read_cb != NULL) {
      read_result.length = read_cb(info->fd, info->file_id, info->stripe_id,
       info->chunk_num, info->offset, read_result.buf, info->count);
   }
   else {
      fprintf(stderr, "a read call has been received, but a read_data callback has not been registered\n");
      fprintf(stderr, "failing ");
      read_result.length = -1;
   }

   return &read_result;
}

int* rpc_write_data_1_svc(write_params *info, struct svc_req *req) {

   if (write_cb != NULL) {
      result = write_cb(info->fd, info->file_id, info->stripe_id, info->chunk_num,
       info->offset, info->buf, info->count);
   }
   else {
      fprintf(stderr, "a write call has been received, but a write_data callback has not been registered\n");
      fprintf(stderr, "failing ");
      result = -1;
   }

   return &result;
}

int* rpc_delete_data_1_svc(delete_params *info, struct svc_req *req) {

   if (delete_cb != NULL) {
      result = delete_cb(info->fd, info->file_id, info->stripe_id, info->chunk_num);
   }
   else {
      fprintf(stderr, "a delete call has been received, but a delete_data callback has not been registered\n");
      fprintf(stderr, "failing ");
      result = -1;
   }

   return &result;
}
