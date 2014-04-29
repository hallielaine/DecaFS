
#include "persistent_metadata.h"

Persistent_Metadata::Persistent_Metadata() {
  next_file_id = ID_NOT_SET;
}

void Persistent_Metadata::init(char *metadata_path) {
  string pathname_to_file = string (metadata_path) + 
                           string (path_metadata_filename);
  string metadata_file = string (metadata_path) + 
                         string (persistent_metadata_filename);
  //pathname_to_file_id.open(pathname_to_file.c_str());
  //metadata.open(metadata_file.c_str());
}

int Persistent_Metadata::get_num_files() {
  return metadata.size();
}

int Persistent_Metadata::get_filenames (char *filenames[MAX_FILENAME_LENGTH], int size) {
  int current = 0;
  //PersistentMap<string, uint32_t>::iterator it;
  map<string, uint32_t>::iterator it;

  it = pathname_to_file_id.begin();

  while (current < size && it != pathname_to_file_id.end()) {
    strcpy (filenames[current++], (it->first).c_str());
    it++;
  }

  return current;
}

int Persistent_Metadata::decafs_file_sstat (char *pathname, struct decafs_file_stat *buf) {
  int id;

  if (pathname_exists (pathname)) {
    return decafs_file_stat (pathname_to_file_id[string(pathname)], buf);
  }
  return FILE_NOT_FOUND;
}

int Persistent_Metadata::decafs_file_stat (uint32_t file_id, struct decafs_file_stat *buf) {
  if (metadata_contains (file_id)) {
    struct persistent_metadata_info info = metadata[file_id];
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
  if (metadata_contains (inst.file_id)) {
    metadata[inst.file_id].last_access_time = time;
    return P_META_SUCCESS;
  }
  return FILE_NOT_FOUND;
}
    
int Persistent_Metadata::add_file (char *pathname, uint32_t stripe_size,
                                   uint32_t chunk_size, uint32_t replica_size, struct timeval time) {
  if (pathname_exists (pathname)) {
    return FILE_EXISTS;
  }

  if (strlen (pathname) > MAX_FILENAME_LENGTH) {
    return FILENAME_INVALID;
  }
 
  string name (pathname);
  uint32_t file_id = get_new_file_id();
  
  printf("\n\n");
  printf ("Adding file %s to DecaFS.\n", pathname);
  struct persistent_metadata_info info = {file_id, 0, stripe_size, chunk_size, replica_size, name,
                       time};
  metadata[file_id] = info;
  printf ("\tid: %d,\n\tsize: %d\n",
           file_id, 0);
  printf ("\tstripe_size: %d,\n\tchunk_size: %d\n",
           stripe_size, chunk_size);
  printf ("Added\n");
  printf ("\tid: %d,\n\tsize: %d\n",
           metadata[file_id].file_id, metadata[file_id].size);
  printf ("\tstripe_size: %d,\n\tchunk_size: %d\n",
           metadata[file_id].stripe_size, metadata[file_id].chunk_size);
  printf ("\n\n");

  pathname_to_file_id[name] = file_id;
  return file_id;
}

int Persistent_Metadata::delete_file_contents (uint32_t file_id) {
  if (metadata_contains (file_id)) {
    pathname_to_file_id.erase (metadata[file_id].pathname);
    metadata.erase (file_id);
    return P_META_SUCCESS;
  }
  return FILE_NOT_FOUND;
}

int Persistent_Metadata::update_file_size (uint32_t file_id, int size_delta) {
  if (metadata_contains (file_id)) {
    struct persistent_metadata_info *info = &(metadata[file_id]);
    
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
     
bool Persistent_Metadata::metadata_contains (uint32_t id) {
  return (metadata.find (id) != metadata.end());
}

bool Persistent_Metadata::pathname_exists (char *pathname) {
  return (pathname_to_file_id.find (string(pathname)) !=
          pathname_to_file_id.end());
}
    
uint32_t Persistent_Metadata::get_new_file_id() {
  uint32_t new_file_id;
  
  file_id_mutex.lock();
  // If we don't know the file id, find the max
  if (next_file_id == ID_NOT_SET) {
    if (!metadata.empty()) {
      next_file_id = metadata.rbegin()->first;
    }
  }
  
  new_file_id = ++next_file_id;
  file_id_mutex.unlock();

  return new_file_id;
}
