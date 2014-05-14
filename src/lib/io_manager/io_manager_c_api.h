#ifndef __IO_MANAGER_H__
#define __IO_MANAGER_H__

#include "decafs_types/file_types.h"
    
/*
 *	Translates a read request from the stripe level to the chunk level.
 *	The correct behavior of this function depends on the
 *	Distribution and Replication strategies that are in place.
 *
 *   @return the number of chunks that participated in the read
 */
extern "C" uint32_t process_read_stripe (uint32_t request_id, uint32_t file_id,
                                         char *pathname, uint32_t stripe_id,
                                         uint32_t stripe_size, uint32_t chunk_size,
                                         const void *buf, int offset,
                                         size_t count);


/*
 *	Translates a write request into a series of chunk writes and handles
 *	replication. 
 *	The correct behavior of this function depends on the
 *	Distribution and Replication strategies that are in place.
 *
 *   All requests sent to the access module for primary storage writes must be
 *   send with request_id.
 *   All requests sent to the access module for replica writes must bes sent with
 *   replica_request_id.
 *
 *   The number of requests sent to the access module for primary storage writes
 *   must be returned in chunks_written.
 *   The number of requests sent to the access module for replica writes must
 *   be returned in replica_chunks_written.
 */
extern "C" void process_write_stripe (uint32_t request_id, uint32_t replica_request_id,
                                      uint32_t *chunks_written,
                                      uint32_t *replica_chunks_written,
                                      uint32_t file_id, char *pathname,
                                      uint32_t stripe_id, uint32_t stripe_size,
                                      uint32_t chunk_size, const void *buf,
                                      int offset, size_t count);


/*
 *   Delete all chunks and replicas for a given file.
 *
 *   @return the number of chunks that participated in the delete
 */
extern "C" uint32_t process_delete_file (uint32_t request_id, uint32_t file_id);

/*
 * Get information about the storage locations of chunks within a file.
 */
extern "C" char * process_file_storage_stat (struct file_storage_stat file_info);

/*
 *	Set the storage location (node id) for a given chunk of a file.
 *   @return the node id
 */
extern "C" int set_node_id (uint32_t file_id, uint32_t stripe_id,
                            uint32_t chunk_num, uint32_t node_id);

/*
 *	Get the storage location (node id) for a given chunk of a file.
 *   @return CHUNK_NOT_FOUND if the chunk hasn't been stored <properly>
 */
extern "C" int get_node_id (uint32_t file_id, uint32_t stripe_id, uint32_t chunk_num);

/*
 *	Set the storage location (node id) for a given replica of a
 *	chunk of a file.
 */
extern "C" int set_replica_node_id (uint32_t file_id, uint32_t stripe_id, uint32_t chunk_num,
                                    uint32_t node_id);

/*
 *	Get the storage location (node id) for a given replica of a
 *	chunk of a file.
 */
extern "C" int get_replica_node_id (uint32_t file_id, uint32_t stripe_id,
                                    uint32_t chunk_num);

/*
 *	Fill in struct decafs_file_stat structure that provides information
 *	about where the chunks live for a specific file.
 */
extern "C" int stat_file_name (char *pathname, struct decafs_file_stat *buf);
extern "C" int stat_file_id (uint32_t file_id, struct decafs_file_stat *buf);

/*
 *	Fill in struct decafs_file_stat structure that provides information
 *	about where the stripes live for a specific file.
 */
extern "C" int stat_replica_name (char *pathname, struct decafs_file_stat *buf);
extern "C" int stat_replica_id (uint32_t file_id, struct decafs_file_stat *buf);

/*
 *	Ensure that all filedata is written to disk.
 */
extern "C" void sync();

#endif
