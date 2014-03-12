#include "persistent_metadata.h"

Persistent_Metadata::Persistent_Metadata() {
}

int Persistent_Metadata::get_num_files() {
  return metadata.size();
}

int Persistent_Metadata::get_filenames (char *filenames[MAX_FILENAME_LENGTH], int size) {
  int current = 0;
  std::map<string, struct persistent_metadata_info>::iterator it;

  it = metadata.begin();

  while (current < size && it != metadata.end()) {
    std::strcpy (filenames[current++], (it->first).c_str());
    it++;
  }

  return current;
}

int Persistent_Metadata::decafs_file_stat (char *pathname, struct decafs_file_stat *buf) {
  if (metadata_contains (pathname)) {
    struct persistent_metadata_info info = metadata[pathname];
    buf->file_id = info.file_id;
    buf->size = info.size;
    buf->stripe_size = info.stripe_size;
    buf->chunk_size = info.chunk_size;
    buf->replica_size = info.replica_size;
    
    // TODO: call function to populate node ids from Distribution Strategy

    // TODO: call function to populate replica ids from Replication Strategy

    return P_META_SUCCESS;
  }
  return FILE_NOT_FOUND;
}

int Persistent_Metadata::decafs_stat (char *pathname, struct statvfs *buf) {
  // TODO
  return 0;
}

int Persistent_Metadata::set_access_time (file_instance inst, struct timeval time) {
  if (!file_id_exists (inst.file_id)) {
    return FILE_NOT_FOUND;
  }
  
  string pathname = file_id_to_pathname[inst.file_id];

  if (!metadata_contains ((char *)pathname.c_str())) {
    return FILE_NOT_FOUND;
  }

  metadata[pathname].last_access_time = time;
  
  return P_META_SUCCESS;
}

bool Persistent_Metadata::metadata_contains (char *pathname) {
  return (metadata.find (pathname) != metadata.end());
}

bool Persistent_Metadata::file_id_exists (int id) {
  return (file_id_to_pathname.find (id) != file_id_to_pathname.end());
}
