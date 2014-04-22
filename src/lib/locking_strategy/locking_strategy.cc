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
  ConnectionToClient *ex_proc;
  std::set<ConnectionToClient *> sh_procs;
  file_lock() : owner(0), ex_proc(NULL), sh_procs() {}
};

struct meta_lock {
  std::mutex mtx;
  uint32_t owner;
  ConnectionToClient *proc;
  meta_lock() : mtx(), owner(0), proc(NULL) {}
} meta_lock_singleton;

static std::mutex file_m;
static std::unordered_map<uint32_t, file_lock> file_locks;

extern "C"
int get_exclusive_lock(struct client client, uint32_t file_id) {
  std::lock_guard<std::mutex> lk(file_m);
  file_lock &lock = file_locks[file_id];

  if (lock.owner)
    return -1;

  lock.owner = client.user_id;
  lock.ex_proc = client.ctc;
  return 0;
}

extern "C"
int get_shared_lock(struct client client, uint32_t file_id) {
  std::lock_guard<std::mutex> lk(file_m);
  file_lock &lock = file_locks[file_id];

  if (lock.owner != client.user_id) {
    if (lock.owner)
      return -1;

    lock.owner = client.user_id;
  }

  if (lock.ex_proc)
    return -1;

  auto insertion = lock.sh_procs.insert(client.ctc);
  return insertion.second ? 0 : -1;
}

extern "C"
int release_lock(struct client client, uint32_t file_id) {
  std::lock_guard<std::mutex> lk(file_m);
  file_lock &lock = file_locks[file_id];

  if (lock.owner != client.user_id)
    return -1;

  if (lock.ex_proc == client.ctc) {
    file_locks.erase(file_id);
  } else {
    auto erasure = lock.sh_procs.erase(client.ctc);
    if (erasure == 0)
      return -1;

    if (lock.sh_procs.size() == 0) {
      file_locks.erase(file_id);
    }
  }
  return 0;
}

extern "C"
int has_exclusive_lock(struct client client, uint32_t file_id) {
  std::lock_guard<std::mutex> lk(file_m);
  auto lock_it = file_locks.find(file_id);

  if (lock_it == file_locks.end())
    return 0;

  if (lock_it->second.owner != client.user_id)
    return 0;

  if (lock_it->second.ex_proc != client.ctc)
    return 0;

  return 1;
}

extern "C"
int has_shared_lock(struct client client, uint32_t file_id) {
  std::lock_guard<std::mutex> lk(file_m);
  auto lock_it = file_locks.find(file_id);

  if (lock_it == file_locks.end())
    return 0;

  if (lock_it->second.owner != client.user_id)
    return 0;

  auto sh_lock = lock_it->second.sh_procs.count(client.ctc);
  assert(sh_lock == 0 || sh_lock == 1);
  if (!sh_lock)
    return 0;

  return 1;
}

extern "C"
int get_metadata_lock(struct client client) {
  meta_lock_singleton.mtx.lock();

  assert(meta_lock_singleton.owner == 0);
  assert(meta_lock_singleton.proc == 0);

  meta_lock_singleton.owner = client.user_id;
  meta_lock_singleton.proc = client.ctc;
  return 0;
}

extern "C"
int release_metadata_lock(struct client client) {
  if (meta_lock_singleton.owner != client.user_id ||
      meta_lock_singleton.proc != client.ctc) {
    return -1;
  }

  meta_lock_singleton.owner = 0;
  meta_lock_singleton.proc = 0;
  meta_lock_singleton.mtx.unlock();
  return 0;
}

extern "C"
int has_metadata_lock(struct client client) {
  return meta_lock_singleton.owner == client.user_id &&
         meta_lock_singleton.proc == client.ctc;
}
