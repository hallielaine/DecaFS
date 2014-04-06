#include "locking_strategy.h"

#include <cassert>
#include <cstdint>

#include <chrono>
#include <mutex>
#include <set>
#include <unordered_map>
#include <utility>


struct file_lock {
  uint32_t owner;
  uint32_t ex_proc;
  std::set<int> sh_procs;
  file_lock() : owner(0), ex_proc(0), sh_procs() {}
};

struct meta_lock {
  std::mutex mtx;
  uint32_t owner;
  uint32_t proc;
  meta_lock() : mtx(), owner(0), proc(0) {}
};

static std::mutex file_m;
static std::unordered_map<uint32_t, file_lock> file_locks;
static std::mutex meta_m;
static std::unordered_map<uint32_t, meta_lock> meta_locks;

extern "C"
int get_exclusive_lock(uint32_t user_id, uint32_t proc_id, uint32_t file_id) {
  std::lock_guard<std::mutex> lk(file_m);
  file_lock &lock = file_locks[file_id];

  if (lock.owner)
    return -1;

  lock.owner = user_id;
  lock.ex_proc = proc_id;
  return 0;
}

extern "C"
int get_shared_lock(uint32_t user_id, uint32_t proc_id, uint32_t file_id) {
  std::lock_guard<std::mutex> lk(file_m);
  file_lock &lock = file_locks[file_id];

  if (lock.owner != user_id) {
    if (lock.owner)
      return -1;

    lock.owner = user_id;
  }

  auto insertion = lock.sh_procs.insert(proc_id);
  return insertion.second ? 0 : -1;
}

extern "C"
int release_lock(uint32_t user_id, uint32_t proc_id, uint32_t file_id) {
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
int has_exclusive_lock(uint32_t user_id, uint32_t proc_id, uint32_t file_id) {
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
int has_shared_lock(uint32_t user_id, uint32_t proc_id, uint32_t file_id) {
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

extern "C"
int get_metadata_lock(uint32_t user_id, uint32_t proc_id, uint32_t file_id) {
  meta_lock *m_lk = NULL;
  {
    std::lock_guard<std::mutex> lk(meta_m);
    m_lk = &meta_locks[file_id];
  }

  m_lk->mtx.lock();

  if (m_lk->owner || m_lk->proc) {
    m_lk->mtx.unlock();
    return -1;
  }

  m_lk->owner = user_id;
  m_lk->proc = proc_id;
  return 0;
}

extern "C"
int release_metadata_lock(uint32_t user_id, uint32_t proc_id, uint32_t file_id) {
  std::unordered_map<uint32_t, meta_lock>::iterator meta_it;
  {
    std::lock_guard<std::mutex> lk(meta_m);
    meta_it = meta_locks.find(file_id);
  }

  if (meta_it == meta_locks.end() ||
      meta_it->second.owner != user_id ||
      meta_it->second.proc != proc_id) {
    return -1;
  }

  meta_it->second.owner = 0;
  meta_it->second.proc = 0;
  meta_it->second.mtx.unlock();
  return 0;
}

extern "C"
int has_metadata_lock(uint32_t user_id, uint32_t proc_id, uint32_t file_id) {
  std::lock_guard<std::mutex> lk(meta_m);
  auto meta_it = meta_locks.find(file_id);

  return meta_it != meta_locks.end() &&
         meta_it->second.owner == user_id &&
         meta_it->second.proc == proc_id;
}
