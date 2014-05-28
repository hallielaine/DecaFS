#ifndef __PERSISTENT_METADATA_IMPL_H__
#define __PERSISTENT_METADATA_IMPL_H__

#include <stdint.h>
#include <stdio.h>
#include <sys/statvfs.h>
#include <sys/time.h>

#include <cstring>
#include <map>
#include <mutex>
#include <string>

#include "persistent_metadata/persistent_metadata.h"

#include "decafs_types/limits.h"
#include "decafs_types/file_types.h"
#include "persistent_stl/persistent_map.h"

#define P_META_SUCCESS 0

#define FILE_NOT_FOUND -1

#define FILE_EXISTS -2
#define FILENAME_INVALID -3

#define NO_METADATA_LOCK -4

#define ID_NOT_SET 0

using namespace std;

class Persistent_Metadata_Impl : public Persistent_Metadata {
  private:
    // Variables
    PersistentMap<string, uint32_t> pathname_to_file_id;
    PersistentMap<uint32_t, struct persistent_metadata_info> metadata;
    //map<string, uint32_t> pathname_to_file_id;
    //map<uint32_t, struct persistent_metadata_info> metadata;

    const char *path_metadata_filename = ".file_id_to_pathname_metadata.dat";
    const char *persistent_metadata_filename = ".persistent_metadata.dat";

    // Helper Functions
    bool metadata_contains (uint32_t id);
    bool pathname_exists (char *pathname);
    uint32_t get_new_file_id();
    
    uint32_t next_file_id;
    mutex file_id_mutex;

  public:
    Persistent_Metadata_Impl();
    ~Persistent_Metadata_Impl () {}

    void init (char *metadata_path);
    int get_num_files ();
    int get_filenames (char *filenames[MAX_FILENAME_LENGTH], int size);
    int decafs_file_sstat (char *pathname, struct decafs_file_stat *buf);
    int decafs_file_stat (uint32_t file_id, struct decafs_file_stat *buf);
    int decafs_stat (char *pathname, struct statvfs *buf);
    int set_access_time (file_instance inst, struct timeval time);
    int add_file (char *pathname, uint32_t stripe_size, uint32_t chunk_size,
                  uint32_t replica_size, struct timeval time);
    int delete_file_contents (uint32_t file_id);
    int update_file_size (uint32_t file_id, int size_delta); 
};

#endif
