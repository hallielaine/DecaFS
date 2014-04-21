#ifndef __PERSISTENT_METADATA_H__
#define __PERSISTENT_METADATA_H__

#include <stdint.h>
#include <stdio.h>
#include <sys/statvfs.h>
#include <sys/time.h>

#include <cstring>
#include <map>
#include <mutex>
#include <string>

#include "decafs_types/limits.h"
#include "decafs_types/file_types.h"

#define P_META_SUCCESS 0

#define FILE_NOT_FOUND -1

#define FILE_EXISTS -2
#define FILENAME_INVALID -3

#define NO_METADATA_LOCK -4

#define ID_NOT_SET 0

struct persistent_metadata_info {
  uint32_t file_id;
  uint32_t size;
  uint32_t stripe_size;
  uint32_t chunk_size;
  uint32_t replica_size;
  struct timeval last_access_time;
};

using namespace std;

class Persistent_Metadata {
  private:
    // Variables
    std::map<int, string> file_id_to_pathname;
    std::map<string, struct persistent_metadata_info> metadata;

    // Helper Functions
    bool get_file_name (uint32_t file_id, string *name);
    bool metadata_contains (char *pathname);
    bool file_id_exists (int id);
    uint32_t get_new_file_id();
    
    uint32_t next_file_id;
    std::mutex file_id_mutex;

  public:
    Persistent_Metadata();

    /*
     *  Return the number of files that exist in DecaFS.
     */
    int get_num_files ();

    /*
     *  Provide a list of filenames that exist in DecaFS.
     *  filenames must have space to hold the number of filenames
     *  returned by get_num_files().
     *  @param size number of file names of length MAX_FILENAME_LENGTH
     *      that fit in filenames.
     *  @return the number of files stored in filenames array in
     *      alphabetical order
     */
    int get_filenames (char *filenames[MAX_FILENAME_LENGTH], int size);
    
    /*
      *  Fill in system stat structure with information 
     *  about one file.
     *  @ return 0 on success
     *           FILE_NOT_FOUND on failure
     */
    
    int decafs_file_sstat (char *pathname, struct decafs_file_stat *buf);
    int decafs_file_stat (uint32_t file_id, struct decafs_file_stat *buf);
    /*
     *  Fill in system stat structure with information 
     *  about entire mounted DecaFS.
     */
    int decafs_stat (char *pathname, struct statvfs *buf);
    
    /*
     *  Updates the access time of the file.
     *  @ return 0 on success
                 FILE_NOT_FOUND on error
     */
    int set_access_time (file_instance inst, struct timeval time);

    /*
     *  Add a file to the DecaFS metadata.
     *  @return file_id on success
     *          FILE_EXISTS if filename already exists in DecaFS
     *          FILENAME_INVALID if filename is too long
     */
    int add_file (char *pathname, uint32_t stripe_size, uint32_t chunk_size,
                  uint32_t replica_size, struct timeval time);
    
    /*
     * Removes a file from DecaFS metadata.
     * @ return 0 on success
     *          FILE_NOT_FOUND on error
     */
    int delete_file_contents (uint32_t file_id);

    /*
     *  Update the size (add or remove bytes to a file) of an existing file.
     *  @return the size of the new file on success
     *          FILE_NOT_FOUND on failure
     */
     int update_file_size (uint32_t file_id, int size_delta); 
};

#endif
