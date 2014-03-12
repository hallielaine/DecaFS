#include "persistent_metadata.h"

Persistent_Metadata::Persistent_Metadata() {
}

int Persistent_Metadata::get_num_files() {
  return metadata.size();
}

int Persistent_Metadata::get_filenames (char *filenames[MAX_FILENAME_LENGTH], int size) {
  return 0;
}

int Persistent_Metadata::decafs_file_stat (char *pathname, struct decafs_file_stat *buf) {
  return 0;
}

int Persistent_Metadata::decafs_stat (char *pathname, struct statvfs *buf) {
  return 0;
}

int Persistent_Metadata::set_access_time (file_instance inst, struct timeval) {
  return 0;
}
