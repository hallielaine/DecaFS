#ifndef __VOLATILE_METADATA_C_API_H__
#define __VOLATILE_METADATA_C_API_H__

#include <stdint.h>

#include "decafs_types/file_types.h"
#include "decafs_types/ip_address.h"

/*
 * Returns the chunk size that is set for this instance of DecaFS.
 * If chunk size has not been set yet, this function returns 0.
 */
extern "C" uint32_t get_chunk_size ();

/*
 * Sets the chunk size for this instance of DecaFS.
 * If chunk size has already been set, SIZE_ALREADY_SET is returned.
 * If the chunk size provided is an invalid size SIZE_INVALID is returned.
 */
extern "C" int set_chunk_size (uint32_t size);

/*
 * Returns the stripe size that is set for this instance of DecaFS.
 * If stripe size has not been set yet, this function returns 0.
 */
extern "C" uint32_t get_stripe_size ();

/*
 * Sets the stripe size for this instance of DecaFS.
 * If stripe size has already been set, SIZE_ALREADY_SET is returned.
 * If the stripe size provided is an invalid size SIZE_INVALID is returned.
 */
extern "C" int set_stripe_size (uint32_t size);

/*
 * Returns the number of espresso nodes that should be connected for this
 * instance of DecaFS.
 */
extern "C" uint32_t get_num_espressos ();

/*
 * Sets the number of espresso nodes to expect for this instance of DecaFS.
 * If the number of espressos is already set, SIZE_ALREADY_SET is returned.
 */
extern "C" int set_num_espressos (uint32_t num_espressos);

/*
 * Set the node with the unique node_number to be "down" in the instance
 *   of DecaFS.
 * @return V_META_SUCCESS on success
 *         NODE_NUMBER_NOT_FOUND on failure
 */
extern "C" uint32_t set_node_down (uint32_t node_number);

/*
 * Set the node with the unique node_number to be "down" in the instance
 *   of DecaFS.
 * @return V_META_SUCCESS on success
 *         NODE_NOT_FOUND on failure
 */
extern "C" uint32_t set_node_up (uint32_t node_number);

/* 
 * Determines whether or not a specific node is "up"
 */
extern "C" bool is_node_up (uint32_t node_number);

/*
 * Returns the number of active nodes.
 */
extern "C" int get_active_node_count();

/*
 * Gives the "state" of the system.
 * Returns an active_nodes struct that represents the node numbers active
 *  in the current instance of DecaFS.
 */
extern "C" struct active_nodes get_active_nodes ();
  
/*
*   Start a new file cursor if one doesn’t exist already.
*  @return the fd
*/
extern "C" int new_file_cursor (uint32_t file_id, struct client client);

/*
*   Remove a file cursor for an open instance of a file.
*   @return id of the file closed on success
*   @return INSTANCE_NOT_FOUND if fd does not exist
*   @return WRONG_CLIENT if the client doesn't match the client who
*           opened the file
*/
extern "C" int close_file_cursor (uint32_t fd, struct client client);

/*
 * Provides information about the cursor for an instance of an open
 *   file.
 *  @return the current byte offset for a given fd
 *          if the fd does not exist, INSTANCE_NOT_FOUND is returned.
 */
extern "C" int get_file_cursor (uint32_t fd);

/*
 *   Set the cursor for an instance of an open file.
 *  @return the current byte offset for a given fd
 *          if the fd does not exist, INSTANCE_NOT_FOUND is returned.
 */
extern "C" int set_file_cursor (uint32_t fd, uint32_t offset, struct client client);
    
/*
 * Find the file_instance associated with a given fd.
 */
extern "C" struct file_instance get_file_info (uint32_t fd);

/*
 * Get a new request id for a client request.
 */
extern "C" uint32_t get_new_request_id();
#endif
