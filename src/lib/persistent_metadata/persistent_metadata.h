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

#include "decafs_types/file_types.h"

using namespace std;

class Persistent_Metadata {
  public:
    virtual ~Persistent_Metadata() {}

    virtual void init (char *metadata_path) = 0;
    
    /*
     *  Return the number of files that exist in DecaFS.
     */
    virtual int get_num_files () = 0;

    /*
     *  Provide a list of filenames that exist in DecaFS.
     *  filenames must have space to hold the number of filenames
     *  returned by get_num_files().
     *  @param size number of file names of length MAX_FILENAME_LENGTH
     *      that fit in filenames.
     *  @return the number of files stored in filenames array in
     *      alphabetical order
     */
    virtual int get_filenames (char *filenames[MAX_FILENAME_LENGTH], int size) = 0;
    
    /*
      *  Fill in system stat structure with information 
     *  about one file.
     *  @ return 0 on success
     *           FILE_NOT_FOUND on failure
     */
    
    virtual int decafs_file_sstat (char *pathname, struct decafs_file_stat *buf) = 0;
    virtual int decafs_file_stat (uint32_t file_id, struct decafs_file_stat *buf) = 0;
    
    /*
     *  Fill in system stat structure with information 
     *  about entire mounted DecaFS.
     */
    virtual int decafs_stat (char *pathname, struct statvfs *buf) = 0;
    
    /*
     *  Updates the access time of the file.
     *  @ return 0 on success
                 FILE_NOT_FOUND on error
     */
    virtual int set_access_time (file_instance inst, struct timeval time) = 0;

    /*
     *  Add a file to the DecaFS metadata.
     *  @return file_id on success
     *          FILE_EXISTS if filename already exists in DecaFS
     *          FILENAME_INVALID if filename is too long
     */
    virtual int add_file (char *pathname, uint32_t stripe_size, uint32_t chunk_size,
                          uint32_t replica_size, struct timeval time) = 0;
    
    /*
     * Removes a file from DecaFS metadata.
     * @ return 0 on success
     *          FILE_NOT_FOUND on error
     */
    virtual int delete_file_contents (uint32_t file_id) = 0;

    /*
     *  Update the size (add or remove bytes to a file) of an existing file.
     *  @return the size of the new file on success
     *          FILE_NOT_FOUND on failure
     */
    virtual int update_file_size (uint32_t file_id, int size_delta) = 0; 
};

#endif
