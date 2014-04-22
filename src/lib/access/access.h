#include <sys/types.h>

#define DATA_NOT_FOUND -1

/*
 *	Read data from a chunk at a specific offset.
 *	If you are implementing this function: 
 *		If data is being read from an Espresso node, Network
 *		Layer network_read_chunk() must be called.
 */
ssize_t process_read_chunk (int fd, int file_id, int node_id, 
     int stripe_id, int chunk_num, int offset, 
     void *buf, int count);

/*
 *	Write data to a chunk at a specific offset.
 *	If you are implementing this function: 
 *		If data is being written to an Espresso node, Network
 *		Layer network_write_chunk() must be called.
 */
ssize_t process_write_chunk (int fd, int file_id, int node_id, 
 int stripe_id, int chunk_num, int offset, 
 void *buf, int count);
