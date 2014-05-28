#ifndef __FILE_TYPES_H__
#define __FILE_TYPES_H__

#include <sys/time.h>

#include <string>

#include <stdint.h>

#include "limits.h"
#include "ip_address.h"

/*
 * DIR equivalent for DecaFS
 */
struct decafs_dir {
  int current;
  int total;
  struct decafs_dirent *entries;

  decafs_dir(int current, int total, decafs_dirent* entries) :
   current(current), total(total), entries(entries) {}
  decafs_dir(int total, decafs_dirent* entries) : decafs_dir(0, total, entries) {}

};

struct decafs_dirent {
  uint32_t file_id; // id unknown for directories
  unsigned char d_type; // 'f' file, 'd' directory
  char d_name[256]; // entry name

  decafs_dirent(uint32_t file_id, unsigned char d_type, char* name) :
   file_id(file_id), d_type(d_type) {
 
    memcpy(d_name, name, strlen(name) + 1);
  }
};

/*
 * File metadata information.
 */
struct persistent_metadata_info {
  uint32_t file_id;
  uint32_t size;
  uint32_t stripe_size;
  uint32_t chunk_size;
  uint32_t replica_size;
  std::string pathname;
  struct timeval last_access_time;

  persistent_metadata_info () : file_id (0), size (0), stripe_size (0),
                                chunk_size (0), replica_size (0) {}
  persistent_metadata_info (uint32_t file_id, uint32_t size,
                            uint32_t stripe_size, uint32_t chunk_size,
                            uint32_t replica_size, std::string pathname,
                            struct timeval last_access_time) {
    this->file_id = file_id;
    this->size = size;
    this->stripe_size = stripe_size;
    this->chunk_size = chunk_size;
    this->replica_size = replica_size;
    this->pathname = pathname;
    this->last_access_time = last_access_time;
  }
};


/*
 * Stores information about a specific instance of an open file in
 * DecaFS.
 */
struct file_instance {
  struct client client_id;
  uint32_t file_id;
  uint32_t offset;
  file_instance(): client_id (client()), file_id (0), offset (0) {}
  file_instance (struct client client, uint32_t file_id, uint32_t offset) {
    this->client_id = client;
    this->file_id = file_id;
    this->offset = offset;
  }

  bool operator ==(const file_instance & other) const {
    return (this->client_id == other.client_id &&
            this->file_id == other.file_id);
  }
  
  bool operator <(const file_instance &other) const {
    if (this->client_id != other.client_id) {
      return this->client_id < other.client_id;
    }
    return (this->file_id < other.file_id);
  }
};

/*
 * Distinctly idenfies a chunk of a file.
 */
struct file_chunk {
  uint32_t file_id;
  uint32_t stripe_id;
  uint32_t chunk_num;

  bool operator ==(const file_chunk & other) const {
    return (this->file_id == other.file_id &&
            this->stripe_id == other.stripe_id &&
            this->chunk_num == other.chunk_num);
  }
  
  friend bool operator <(const file_chunk &left, const file_chunk &right) {
    if (left.file_id != right.file_id) {
      return left.file_id < right.file_id;
    }
    if (left.stripe_id != right.stripe_id) {
      return left.file_id < right.stripe_id;
    }
    return left.chunk_num < right.chunk_num;
  }
};

/*
 *  Storage Information about one file in DecaFS.
 */
struct decafs_file_stat {
  uint32_t file_id; /* DecaFS file id for the file. */
  uint32_t size; /* Size of the file in bytes */
  uint32_t stripe_size; 
  uint32_t chunk_size;
  uint32_t replica_size;
  struct timeval last_access_time;
};


#endif
