#ifndef __BARISTA_CORE_H__
#define __BARISTA_CORE_H__

#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "decafs_types/limits.h"
#include "decafs_types/file_types.h"
#include "decafs_types/messages.h"
#include "decafs_types/ip_address.h"

#include "io_manager/io_manager.h"
#include "io_manager/io_manager_c_api.h"

#include "persistent_metadata/persistent_metadata.h"
#include "persistent_metadata/persistent_metadata_c_api.h"

#include "volatile_metadata/volatile_metadata.h"
#include "volatile_metadata/volatile_metadata_c_api.h"

#include "network_core/barista_network_helper.h"
#include "network_core/network_packets.h"

#include "access/access.h"
#include "locking_strategy/locking_strategy.h"

#define FILE_IN_USE -1
#define FILE_NOT_OPEN_FOR_WRITE -2
#define FILE_NOT_OPEN_FOR_READ -3
#define UNABLE_TO_STAT_FILE -4
#define FILE_NOT_FOUND_FOR_READING -5

#define STRIPE_ID_INIT 1

struct request_info {
  uint32_t chunks_expected;
  uint32_t chunks_received;
  struct client client;

  request_info() : chunks_expected (0), chunks_received (0) {}
  request_info (struct client client) {
      this->chunks_expected = 0;
      this->chunks_received = 0;
      this->client = client;
  }
};

struct read_request_info {
  struct request_info info;
  int fd;
  uint8_t *buf;
  std::map<struct file_chunk, ReadChunkResponse *> response_packets;

  read_request_info() : info (request_info()), fd (0) {}
  read_request_info (struct client client, int fd, uint8_t *buf) {
    this->info = request_info (client);
    this->fd = fd;
    this->buf = buf;
  }
};

struct write_request {
  uint32_t request_id;
  uint32_t replica_request_id;

  bool operator <(const write_request &other) const {
    return ((this->request_id < other.request_id) ? true :
                (this->replica_request_id < other.replica_request_id) ? 
                 true : false);
  }
};

struct write_request_info {
  struct request_info info;
  struct request_info replica_info;
  int fd;
  int count;

  write_request_info() : info (request_info()), replica_info (request_info()),
                         fd (0), count (0) {}
  write_request_info (struct client client, int fd) {
    this->info = request_info (client);
    this->replica_info = request_info (client);
    this->fd = fd;
    this->count = 0;
  }
};

extern "C" const char *get_size_error_message (const char *type, const char *value);

extern "C" void exit_failure (const char *message);

/*
 * Initialize barista core
 */
extern "C" void barista_core_init (int argc, char *argv[]);

/*
 *	Open a file for read or write access.
 *
 * Flags:
 *   O_RDONLY open a file for reading
 *   O_RDWR open a file for both reading and writing
 *   O_APPEND start the file cursor at the end of the file
 *
 *	@post
 *     open_file sends the file id for the newly opened file (non-zero)
 *     to the client or FILE_IN_USE if the proper lock cannot be obtained
 */
extern "C" void open_file (const char *pathname, int flags, struct client client);

/*
 *	If the process has a lock on the file, complete the read.
 *	Translates read request into chunks of requests to Espresso 
 *	nodes.
 */
extern "C" void read_file (int fd, size_t count, struct client client);

/*
 * Aggregates the read_file futures and determines when the read is complete.
 * Upon completion of a read, this function returns read information to the
 * Network Layer.
 */
extern "C" void read_response_handler (ReadChunkResponse *read_response);

/*
 *	If the process has an exclusive lock on the file, complete the
 *	write.
 *	Translate write requests into chunks of requests to Espresso
 *	nodes.
 */
extern "C" void write_file (int fd, const void *buf, size_t count, struct client client);

/*
 * Aggregates the write_file futures and determines when the write is complete.
 * Upon completion of a write, this function returns write information to the
 * Network Layer.
 */
extern "C" void write_response_handler (WriteChunkResponse *write_response);

/*
 *	Release locks associate with a fd.
 */
extern "C" void close_file (int fd, struct client client);

/*
 *	Removes a file from DecaFS.
 *	@ return >= 0 success, < 0 failure
 */
extern "C" void delete_file (char *pathname, struct client client);

/*
 * Aggregates the delete_file futures and determines when the delete is complete.
 * Upon completion of a delete, this function returns delete information to the
 * Network Layer.
 */
extern "C" void delete_response_handler (DeleteChunkResponse *delete_response);

/*
 * Moves the file cursor to the location specificed by whence, plus offset
 * bytes.
 *
 * If the whence and offset cause the cursor to be set past the end of the file
 * it will be set to the end of the file.
 *
 * whence:
 *   SEEK_SET move to offset from the beginning of the file
 *   SEEK_CUR move to offset from the current location of the fd
 *
 * @return the cursor's new location on success and < 0 on failure
 *          
 */
extern "C" int file_seek (int fd, uint32_t offset, int whence, struct client client);

/*
 *	Fills struct stat with file info.
 */
extern "C" int file_stat (const char *path, struct stat *buf);
extern "C" int file_fstat (int fd, struct stat *buf);

/*
 *  Collects information about a mounted filesystem. 
 *  path is the pathname of any file within the mounted
 *  filesystem.
 */
extern "C" void statfs (char *pathname, struct statvfs * stat);

/*
 *	Register a module to be called with a specific timeout, 
 *	repeatedly throughout DecaFS execution.
 */
extern "C" void register_monitor_module (void (*monitor_module), 
                                         struct timeval timeout);
/*
 *	Register a function to be called on node failure.
 */
extern "C" void register_node_failure_handler (void (*failure_handler));

/*
 *	Register a function to be called on startup to recover chunk metadata.
 */
extern "C" void register_chunk_metadata_handler (void (*metadata_handler));

/*
 *	Register a function to be called on startup to recover chunk replica
 *	metadata.
 */
extern "C" void register_chunk_replica_metadata_handler (void (*metadata_handler));

/*
 *	Move an existing chunk to a different Espresso node in the system. 
 */
extern "C" int move_chunk (const char* pathname, uint32_t stripe_id, uint32_t chunk_num, 
                           uint32_t dest_node, struct client client);
extern "C" int fmove_chunk (uint32_t file_id, uint32_t stripe_id, uint32_t chunk_num,
                            uint32_t dest_node, struct client client);

/*
 *	Move a chunk’s replica to a different Espresso node in the system. 
 */
extern "C" int move_chunk_replica (const char* pathname, uint32_t stripe_id, 
                                   uint32_t chunk_num, uint32_t dest_node,
                                   struct client client);
extern "C" int fmove_chunk_replica (uint32_t file_id, uint32_t stripe_id,
                                    uint32_t chunk_num, uint32_t dest_node,
                                    struct client client);

/*
 *	creates a directory in the DecaFS instance.
 */
extern "C" int mk_decafs_dir (const char* dirname);

/*
 *	opens a directory stream corresponding to the directory name.
 */
extern "C" DIR* open_decafs_dir (const char* name);

/*
 *	returns a pointer to a dirent structure representing the next directory   
 *	entry in the directory stream pointed to by dirp.
 */
extern "C" struct dirent* read_decafs_dir (DIR *dirp);

#endif
