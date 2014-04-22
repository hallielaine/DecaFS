#ifndef LOCKING_STRATEGY_H_
#define LOCKING_STRATEGY_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "decafs_types/ip_address.h"

/*
 * Tries to acquire an exclusive lock for a process. Fails if the lock cannot
 * be acquired.
 *
 * Returns 0 on success, or negative on error.
 */
int get_exclusive_lock(struct client client, uint32_t file_id);

/*
 * Tries to acquire a shared lock for a process. Fails if the lock cannot be
 * acquired.
 *
 * Returns 0 on success, or negative on error.
 */
int get_shared_lock(struct client client, uint32_t file_id);

/*
 * Releases a lock, either exclusive or shared. The lock released is whatever
 * kind of lock the process had on the file. Fails if the lock is not owned.
 *
 * Returns 0 on success, or negative on error.
 */
int release_lock(struct client client, uint32_t file_id);

/*
 * Checks whether a process has an exclusive lock. Specifying a negative value
 * for *user_id* or *proc_id* is like a wildcard, and will return whether any
 * *user_id* or *proc_id* has the lock.
 *
 * Returns positive if the lock is held, 0 if not, or negative on error.
 */
int has_exclusive_lock(struct client client, uint32_t file_id);

/*
 * Checks whether a process has a shared lock. Specifying a negative value for
 * *user_id* or *proc_id* is like a wildcard, and will return whether any
 * *user_id* or *proc_id* has the lock.
 *
 * Returns positive if the lock is held, 0 if not, or negative on error.
 */
int has_shared_lock(struct client client, uint32_t file_id);

/*
 * Tries to acquire an exclusive metadata lock for a process. Blocks until the
 * lock can be acquired.
 *
 * Returns 0 on success, or negative on error.
 */
int get_metadata_lock(struct client client);

/*
 * Releases a metadata lock. Fails if the lock is not owned.
 *
 * Returns 0 on success, or negative on error.
 */
int release_metadata_lock(struct client client);

/*
 * Checks whether a process has an exclusive metadata lock. Specifying a
 * negative value for *user_id* or *proc_id* is like a wildcard, and will
 * return whether any *user_id* or *proc_id* has the lock.
 *
 * Returns positive if the lock is held, 0 if not, or negative on error.
 */
int has_metadata_lock(struct client client);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // LOCKING_STRATEGY_H_
