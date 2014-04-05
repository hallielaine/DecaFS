#ifndef __BARISTA_CORE_H__
#define __BARISTA_CORE_H__

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>

#include "limits.h"
#include "file_types.h"
#include "messages.h"
#include "io_manager.h"
#include "persistent_metadata.h"
#include "volatile_metadata.h"
#include "network_core/bar_clnt.h"
#include "access/access.h"

void process_arguments (int argc, char *argv[]);
void exit_failure (const char *message);

const char *get_size_error_message (const char *type, const char *value);

/*
 *	Open a file for read or write access.
 *	If a file is opened for read access, no exclusive locks are held 
 *	for the file and no other client has any lock on the file, a 
 *	shared lock is granted.
 *	If a file is opened for write access, and there are no locks on  
 *	the file, an exclusive lock is granted.
 *	@ return the file id for the newly opened file (non-zero)
 */
int open (const char *pathname, int flags);

/*
 *	If the process has a lock on the file, complete the read.
 *	Translates read request into chunks of requests to Espresso 
 *	nodes.
 */
ssize_t read (int fd, void *buf, size_t count);

/*
 *	If the process has an exclusive lock on the file, complete the
 *	write.
 *	Translate write requests into chunks of requests to Espresso
 *	nodes.
 */
ssize_t write (int fd, const void *buf, size_t count);

/*
 *	Release locks associate with a fd.
 */
int close (int fd);

/*
 *	Removes a file from DecaFS.
 *	@ return >= 0 success, < 0 failure
 */
void delete_file (char *pathname);

/*
 *	Ensure that all filedata is written to disk.
 */
void sync();

/*
 *	Fills struct stat with file info.
 */
int stat (const char *path, struct stat *buf);
int fstat (int fd, struct stat *buf);

/*
 *  Collects information about a mounted filesystem. 
 *  path is the pathname of any file within the mounted
 *  filesystem.
 */
void statfs (char *pathname, struct statvfs * stat);

/*
 *	Register a module to be called with a specific timeout, 
 *	repeatedly throughout DecaFS execution.
 */
void register_monitor_module (void (*monitor_module), 
                              struct timeval timeout);
/*
 *	Register a function to be called on node failure.
 */
void register_node_failure_handler (void (*failure_handler));

/*
 *	Register a function to be called on startup to recover chunk metadata.
 */
void register_chunk_metadata_handler (void (*metadata_handler));

/*
 *	Register a function to be called on startup to recover chunk replica
 *	metadata.
 */
void register_chunk_replica_metadata_handler (void (*metadata_handler));

/*
 *	Move an existing chunk to a different Espresso node in the system. 
 */
void move_chunk (const char* pathname, uint32_t stripe_id, uint32_t chunk_num, 
                 uint32_t dest_node);
void fmove_chunk (uint32_t file_id, uint32_t stripe_id, uint32_t chunk_num,
                  uint32_t dest_node);

/*
 *	Move a chunk’s replica to a different Espresso node in the system. 
 */
void move_chunk_replica (const char* pathname, uint32_t stripe_id, 
                         uint32_t chunk_num, uint32_t dest_node);
void fmove_chunk_replica (uint32_t file_id, uint32_t stripe_id,
                          uint32_t chunk_num, uint32_t dest_node);

/*
 *	creates a directory in the DecaFS instance.
 */
int mkdir (const char* dirname);

/*
 *	opens a directory stream corresponding to the directory name.
 */
DIR* opendir (const char* name);

/*
 *	returns a pointer to a dirent structure representing the next directory   
 *	entry in the directory stream pointed to by dirp.
 */
struct dirent* readdir (DIR *dirp);

#endif
