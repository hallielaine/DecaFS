#ifndef __VOLATILE_METADATA_H__
#define __VOLATILE_METADATA_H__

#include <stdint.h>

#include <algorithm>
#include <list>
#include <cstring>

#include "limits.h"

#define V_META_SUCCESS 0

// Chunk and Stripe Size Errors
#define SIZE_ALREADY_SET -1
#define SIZE_INVALID -2

// Map Errors
#define IP_EXISTS -1
#define IP_NOT_FOUND -2

using namespace std;

class Volatile_Metadata {
  private:
    // Variables
    uint32_t chunk_size;
    uint32_t stripe_size;
    std::map<string, int> ip_to_node_map;
    std::list<string> up_nodes;
    // TODO: add file cursors and node map

    // Helper Functions
    bool ip_to_node_map_contains (char *ip);
    bool up_nodes_contains (char *ip);

  public:
    Volatile_Metadata();

    /*
     * Returns the chunk size that is set for this instance of DecaFS.
     * If chunk size has not been set yet, this function returns 0.
     */
    uint32_t get_chunk_size ();
    /*
     * Sets the chunk size for this instance of DecaFS.
     * If chunk size has already been set, SIZE_ALREADY_SET is returned.
     * If the chunk size provided is an invalid size SIZE_INVALID is returned.
     */
    int set_chunk_size (uint32_t size);

    /*
     * Returns the stripe size that is set for this instance of DecaFS.
     * If stripe size has not been set yet, this function returns 0.
     */
    uint32_t get_stripe_size ();
    /*
     * Sets the stripe size for this instance of DecaFS.
     * If stripe size has already been set, SIZE_ALREADY_SET is returned.
     * If the stripe size provided is an invalid size SIZE_INVALID is returned.
     */
    int set_stripe_size (uint32_t size);

    /*
     * Returns the node number for a specific IP address.
     * If barista_core was not started with the given IP address, IP_NOT_FOUND
     *   is returned.
     */
    uint32_t get_node_number (char *ip);
    /*
     * Add a node ip and node number pairing to the metadata for this instance
     *   of DecaFS.
     * If the ip address has already been assigned a node number, IP_EXISTS
     *   is returned.
     */
    int add_node (char *ip, uint32_t node_number);
    /*
     * Set the node with the specific ip address to be "down" 
     *   of DecaFS.
     * If barista_core was not started with the given IP address, IP_NOT_FOUND
     *   is returned.
     */
    uint32_t set_node_down (char *ip);
    /*
     * Set the node with the specific ip address to be "up" 
     *   of DecaFS.
     * If barista_core was not started with the given IP address, IP_NOT_FOUND
     *   is returned.
     */
    uint32_t set_node_up (char *ip);
    /*
     * Returns the number of active nodes.
     */
    int get_active_node_count();
    /*
     * Give the "state" of the system.
     * nodes is filled in with the ip addresses for the active nodes (nodes
     *   that were not set to "down").
     * IP addresses of each node will be returned in sorted order.
     * nodes will be reallocated to support the number of nodes that are up.
     * The number of active nodes in the system is returned.
     */
    uint32_t get_active_nodes (char ***nodes);
}; 
#endif
