#ifndef __IO_MANAGER_H__
#define __IO_MANAGER_H__

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "limits.h"
#include "file_types.h"
#include "persistent_metadata.h"
#include "volatile_metadata.h"
#include "distribution_strategy.h"
#include "replication_strategy.h"
#include "access/access.h"

#define CHUNK_NOT_FOUND -1
#define NODE_NOT_FOUND -2
#define REPLICA_CHUNK_NOT_FOUND -3
#define REPLICA_NODE_NOT_FOUND -4

#define CHUNK_ID_INIT 1

using namespace std;

class IO_Manager {
  private:
    // Variables
    std::map<struct file_chunk, int> chunk_to_node;
    std::map<struct file_chunk, int> chunk_to_replica_node;
    
    // Helper Functions
    bool chunk_exists (struct file_chunk);
    bool chunk_replica_exists (struct file_chunk);
    //Given a file_id, list all chunks that are part of the file
    std::vector<struct file_chunk> get_all_chunks (uint32_t file_id);
    // Given offset (a stripe offset) sets id to be the chunk id
    // that the offset it is, and chunk_offset to the offset within
    // the chunk
    void get_first_chunk (uint32_t *id, uint32_t chunk_size, int *chunk_offset, int offset);

  public:
    IO_Manager();

    /*
     *	Translates a read request from the stripe level to the chunk level.
     *	The correct behavior of this function depends on the
     *	Distribution and Replication strategies that are in place.
     */
    ssize_t process_read_stripe (uint32_t file_id, char *pathname,
                                 uint32_t stripe_id, uint32_t stripe_size,
                                 uint32_t chunk_size, const void *buf,
                                 int offset, size_t count);

    /*
     *	Translates a write request into a series of chunk writes and handles
     *	replication. 
     *	The correct behavior of this function depends on the
     *	Distribution and Replication strategies that are in place.
     */
    ssize_t process_write_stripe (uint32_t file_id, char *pathname,
                                  uint32_t stripe_id, uint32_t stripe_size,
                                  uint32_t chunk_size, const void *buf,
                                  int offset, size_t count);
    
    /*
     *   Delete all chunks and replicas for a given file.
     */
    void process_delete_file (uint32_t file_id);

    /*
     *	Set the storage location (node id) for a given chunk of a file.
     *   @return the node id
     *   @return NODE_NOT_FOUND if the node does not exist
     */
    int set_node_id (uint32_t file_id, uint32_t stripe_id, uint32_t chunk_num,
                     uint32_t node_id);

    /*
     *	Get the storage location (node id) for a given chunk of a file.
     *   @return CHUNK_NOT_FOUND if the chunk hasn't been stored <properly>
     */
    int get_node_id (uint32_t file_id, uint32_t stripe_id, uint32_t chunk_num);

    /*
     *	Set the storage location (node id) for a given replica of a
     *	chunk of a file.
     */
    int set_replica_node_id (uint32_t file_id, uint32_t stripe_id,
                             uint32_t chunk_num, uint32_t node_id);

    /*
     *	Get the storage location (node id) for a given replica of a
     *	chunk of a file.
     */
    int get_replica_node_id (uint32_t file_id, uint32_t stripe_id, uint32_t chunk_num);

    /*
     *	Fill in struct decafs_file_stat structure that provides information
     *	about where the chunks live for a specific file.
     */
    int stat_file_name (char *pathname, struct decafs_file_stat *buf);
    int stat_file_id (uint32_t file_id, struct decafs_file_stat *buf);

    /*
     *	Fill in struct decafs_file_stat structure that provides information
     *	about where the stripes live for a specific file.
     */
    int stat_replica_name (char *pathname, struct decafs_file_stat *buf);
    int stat_replica_id (uint32_t file_id, struct decafs_file_stat *buf);

    /*
     *	Ensure that all filedata is written to disk.
     */
     void sync();

};
    
/*
 *	Translates a read request from the stripe level to the chunk level.
 *	The correct behavior of this function depends on the
 *	Distribution and Replication strategies that are in place.
 */
extern "C" ssize_t process_read_stripe (uint32_t file_id, char *pathname,
                                        uint32_t stripe_id, uint32_t stripe_size,
                                        uint32_t chunk_size, const void *buf,
                                        int offset, size_t count);


/*
 *	Translates a write request into a series of chunk writes and handles
 *	replication. 
 *	The correct behavior of this function depends on the
 *	Distribution and Replication strategies that are in place.
 */
extern "C" ssize_t process_write_stripe (uint32_t file_id, char *pathname,
                                         uint32_t stripe_id, uint32_t stripe_size,
                                         uint32_t chunk_size, const void *buf,
                                         int offset, size_t count);
    
/*
 *   Delete all chunks and replicas for a given file.
 */
extern "C" void process_delete_file (uint32_t file_id);

/*
 *	Set the storage location (node id) for a given chunk of a file.
 */
extern "C" int set_node_id (uint32_t file_id, uint32_t stripe_id,
                            uint32_t chunk_num, uint32_t node_id);

/*
 *	Get the storage location (node id) for a given chunk of a file.
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
