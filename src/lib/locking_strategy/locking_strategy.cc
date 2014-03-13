#include "locking_strategy.h"

#include <unordered_map>
#include <mutex>
#include <chrono>


#define UNUSED __attribute__((unused));

int get_exclusive_lock(int user_id, int proc_id, int file_id, int timeout) {
  return -1;
}

int get_shared_lock(int user_id, int proc_id, int file_id, int timeout) {
  return -1;
}
int release_lock(int user_id, int proc_id, int file_id) {
  return -1;
}

int has_exclusive_lock(int user_id, int proc_id, int file_id) {
  return -1;
}

int has_shared_lock(int user_id, int proc_id, int file_id) {
  return -1;
}

int get_shared_metadata_lock(int user_id, int proc_id, int file_id, int timeout) {
  return -1;
}

int release_metadata_lock(int user_id, int proc_id, int file_id) {
  return -1;
}

int has_exclusive_metadata_lock(int user_id, int proc_id, int file_id) {
  return -1;
}

int has_shared_metadata_lock(int user_id, int proc_id, int file_id) {
  return -1;
}
