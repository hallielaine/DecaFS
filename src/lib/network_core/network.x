struct read_params {
   int fd;
   int file_id;
   int stripe_id;
   int chunk_num;
   int offset;
   int count;
};

struct write_params {
   int fd;
   int file_id;
   int stripe_id;
   int chunk_num;
   int offset;
   string buf<>;
   int count;
};

struct delete_params {
   int file_id;
   int stripe_id;
   int chunk_num;
};

struct read_rtn {
   string buf<>;
   int length;
};

program DECAFS_NETWORK_CORE {
   version DECAFS_NETWORK_CORE_V1 {
      read_rtn RPC_READ_DATA(read_params) = 1;
      int RPC_WRITE_DATA(write_params) = 2;
      int RPC_DELETE_DATA(delete_params) = 3;
   } = 1;
} = 0x2fffffff;
