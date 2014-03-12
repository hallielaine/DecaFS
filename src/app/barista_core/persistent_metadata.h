#ifndef __PERSISTENT_METADATA_H__
#define __PERSISTENT_METADATA_H__

#include <stdint.h>
#include <sys/statvfs.h>

#include <cstring>
#include <map>
#include <string>

#include "limits.h"
#include "file_types.h"

#define P_META_SUCCESS 0

#define FILE_NOT_FOUND -1

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
    bool metadata_contains (char *pathname);
    bool file_id_exists (int id);

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
     *  @return the number of files stored in filenames array
     */
    int get_filenames (char *filenames[MAX_FILENAME_LENGTH], int size);
    
    /*
     *  Fill in system stat structure with information 
     *  about one file.
     */
    
    int decafs_file_stat (char *pathname, struct decafs_file_stat *buf);
    /*
     *  Fill in system stat structure with information 
     *  about entire mounted DecaFS.
     */
    int decafs_stat (char *pathname, struct statvfs *buf);
    
    /*
     *  Updates the access time of the file.
     */
    int set_access_time (file_instance inst, struct timeval time);
};

#endif
