#include "locking_strategy.h"

#include <cassert>
#include <unordered_map>
#include <set>
#include <utility>
#include <mutex>
#include <chrono>


#define UNUSED __attribute__((unused));


struct file_lock {
  int owner;
  int ex_proc;
  std::set<int> sh_procs;
  file_lock() : owner(-1), ex_proc(-1) {}
};

static std::mutex file_m;
static std::unordered_map<int, file_lock> file_locks;
static std::mutex meta_m;
static std::unordered_map<int, std::mutex> meta_locks;

extern "C"
int get_exclusive_lock(int user_id, int proc_id, int file_id) {
  std::lock_guard<std::mutex> lk(file_m);
  file_lock &lock = file_locks[file_id];

  if (lock.owner != -1)
    return -1;

  lock.owner = user_id;
  lock.ex_proc = proc_id;
  return 0;
}

extern "C"
int get_shared_lock(int user_id, int proc_id, int file_id) {
  std::lock_guard<std::mutex> lk(file_m);
  file_lock &lock = file_locks[file_id];

  if (lock.owner != user_id) {
    if (lock.owner != -1)
      return -1;

    lock.owner = user_id;
  }

  auto insertion = lock.sh_procs.insert(proc_id);
  return insertion.second ? 0 : -1;
}

extern "C"
int release_lock(int user_id, int proc_id, int file_id) {
  std::lock_guard<std::mutex> lk(file_m);
  file_lock &lock = file_locks[file_id];

  if (lock.owner != user_id)
    return -1;

  if (lock.ex_proc == proc_id) {
    file_locks.erase(file_id);
  } else {
    auto erasure = lock.sh_procs.erase(user_id);
    if (erasure == 0)
      return -1;

    if (lock.sh_procs.size() == 0) {
      file_locks.erase(file_id);
    }
  }
  return 0;
}

extern "C"
int has_exclusive_lock(int user_id, int proc_id, int file_id) {
  std::lock_guard<std::mutex> lk(file_m);
  auto lock_it = file_locks.find(file_id);

  if (lock_it == file_locks.end())
    return 0;

  if (lock_it->second.owner != user_id)
    return 0;

  if (lock_it->second.ex_proc != proc_id)
    return 0;

  return 1;
}

extern "C"
int has_shared_lock(int user_id, int proc_id, int file_id) {
  std::lock_guard<std::mutex> lk(file_m);
  auto lock_it = file_locks.find(file_id);

  if (lock_it == file_locks.end())
    return 0;

  if (lock_it->second.owner != user_id)
    return 0;

  auto sh_lock = lock_it->second.sh_procs.count(proc_id);
  assert(sh_lock == 0 || sh_lock == 1);
  if (!sh_lock)
    return 0;

  return 1;
}

