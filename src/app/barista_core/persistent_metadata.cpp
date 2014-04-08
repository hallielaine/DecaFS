#include "persistent_metadata.h"

Persistent_Metadata::Persistent_Metadata() {
  next_file_id = ID_NOT_SET;
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
    buf->last_access_time = info.last_access_time;
    
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
  string pathname;
  
  if (get_file_name (inst.file_id, &pathname)) {
    metadata[pathname].last_access_time = time;
    return P_META_SUCCESS;
  }
  return FILE_NOT_FOUND;
}
    
int Persistent_Metadata::add_file (char *pathname, uint32_t stripe_size,
                                   uint32_t chunk_size, uint32_t replica_size, struct timeval time) {
  if (metadata_contains (pathname)) {
    return FILE_EXISTS;
  }

  if (strlen (pathname) > MAX_FILENAME_LENGTH) {
    return FILENAME_INVALID;
  }
 
  string name(pathname);
  uint32_t file_id = get_new_file_id();
  struct persistent_metadata_info info = {file_id, 0, stripe_size, chunk_size, replica_size, time};

  metadata[name] = info;
  file_id_to_pathname[file_id] = name;
  return file_id;
}

int Persistent_Metadata::delete_file (uint32_t file_id) {
  string pathname;
  
  if (get_file_name (file_id, &pathname)) {
    metadata.erase (pathname);
    file_id_to_pathname.erase (file_id);
    return P_META_SUCCESS;
  }
  return FILE_NOT_FOUND;
}

int Persistent_Metadata::update_file_size (uint32_t file_id, int size_delta) {
  string pathname;

  if (get_file_name (file_id, &pathname)) {
    struct persistent_metadata_info *info = &(metadata[pathname]);
    
    if ((int)info->size + size_delta < 0) {
      info->size = 0;
    }
    else {
      info->size = info->size + size_delta;
    }
    return info->size;
  }
  return FILE_NOT_FOUND;
}
     
bool Persistent_Metadata::get_file_name (uint32_t file_id, string *name) {
  if (!file_id_exists (file_id)) {
    return false;
  }
  *name = file_id_to_pathname[file_id];
  if (!metadata_contains ((char *)(*name).c_str())) {
    return false;
  }
  return true;
}

bool Persistent_Metadata::metadata_contains (char *pathname) {
  return (metadata.find (pathname) != metadata.end());
}

bool Persistent_Metadata::file_id_exists (int id) {
  return (file_id_to_pathname.find (id) != file_id_to_pathname.end());
}
    
uint32_t Persistent_Metadata::get_new_file_id() {
  // If we don't know the file id, find the max
  if (next_file_id == ID_NOT_SET) {
    if (file_id_to_pathname.empty()) {
      next_file_id = 0;
    }
    else {
      next_file_id = file_id_to_pathname.rbegin()->first;
    }
  }
  return ++next_file_id;
}
