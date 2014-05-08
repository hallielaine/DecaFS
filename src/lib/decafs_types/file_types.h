#ifndef __FILE_TYPES_H__
#define __FILE_TYPES_H__

#include <sys/time.h>

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
    return ((this->client_id < other.client_id) ? true :
                (this->file_id < other.file_id) ? true : false);
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
  
  bool operator <(const file_chunk &other) const {
    return ((this->file_id < other.file_id) ? true :
               (this->stripe_id < other.stripe_id) ? true :
                   (this->chunk_num < other.chunk_num) ? 
                      true : false);
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
  uint32_t num_stripes;  /* Number of stripes in the file */
  uint32_t nodes[MAX_CHUNK]; /* Where each stripe is stored */
  uint32_t num_replica_chunks; /* Number of replica chunks in the file. */
  uint32_t replica_chunk_nodes[MAX_CHUNK]; /* Where each replica is stored. */
  struct timeval last_access_time;
};


#endif
