#ifndef __PERSISTENT_METADATA_C_API_H__
#define __PERSISTENT_METADATA_C_API_H__

#include <stdint.h>
#include <sys/statvfs.h>
#include <sys/time.h>

#include "decafs_types/file_types.h"

/*
 *  Return the number of files that exist in DecaFS.
 */
extern "C" int get_num_files (struct client client);

/*
 *  Provide a list of filenames that exist in DecaFS.
 *  filenames must have space to hold the number of filenames
 *  returned by get_num_files().
 *  @param size number of file names of length MAX_FILENAME_LENGTH
 *      that fit in filenames.
 *  @return the number of files stored in filenames array in
 *      alphabetical order
 */
extern "C" int get_filenames (char *filenames[MAX_FILENAME_LENGTH], int size,
                              struct client client);

/*
  *  Fill in system stat structure with information 
 *  about one file.
 *  @ return 0 on success
 *           FILE_NOT_FOUND on failure
 */

extern "C" int decafs_file_sstat (char *pathname, struct decafs_file_stat *buf,
                                 struct client client);
extern "C" int decafs_file_stat (uint32_t file_id, struct decafs_file_stat *buf,
                                 struct client client);

/*
 *  Fill in system stat structure with information 
 *  about entire mounted DecaFS.
 */
extern "C" int decafs_stat (char *pathname, struct statvfs *buf,
                            struct client client);

/*
 *  Updates the access time of the file.
 *  @ return 0 on success
             FILE_NOT_FOUND on error
 */
extern "C" int set_access_time (file_instance inst, struct timeval time,
                                struct client client);

/*
 *  Add a file to the DecaFS metadata.
 *  @return file_id on success
 *          FILE_EXISTS if filename already exists in DecaFS
 *          FILENAME_INVALID if filename is too long
 */
extern "C" int add_file (char *pathname,
                         uint32_t stripe_size, uint32_t chunk_size,
                         uint32_t replica_size, struct timeval time,
                         struct client client);

/*
 * Removes a file from DecaFS metadata.
 * @ return 0 on success
 *          FILE_NOT_FOUND on error
 */
extern "C" int delete_file (uint32_t file_id, struct client client);

/*
 *  Update the size (add or remove bytes to a file) of an existing file.
 *  @return the size of the new file on success
 *          FILE_NOT_FOUND on failure
 */
extern "C" int update_file_size (uint32_t file_id, int size_delta,
                                 struct client client); 

#endif
