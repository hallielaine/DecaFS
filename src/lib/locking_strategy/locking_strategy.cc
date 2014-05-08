#include "locking_strategy.h"

#include <cassert>
#include <cstdint>

#include <chrono>
#include <mutex>
#include <set>
#include <unordered_map>
#include <utility>


struct file_lock {
  uint32_t owner_uid;
  struct ip_address owner_ip;
  ConnectionToClient *ex_proc;
  std::set<ConnectionToClient*> sh_procs;
  file_lock() : owner_uid(0), ex_proc(NULL), sh_procs() {}
};

struct meta_lock {
  std::mutex mtx;
  uint32_t owner_uid;
  struct ip_address owner_ip;
  ConnectionToClient *proc;
  meta_lock() : mtx(), owner_uid(0), proc(NULL) {}
} meta_lock_singleton;

static std::mutex file_m;
static std::unordered_map<uint32_t, file_lock> file_locks;

extern "C"
int get_exclusive_lock(struct client client, uint32_t file_id) {
  assert(client.user_id != 0);

  std::lock_guard<std::mutex> lk(file_m);
  file_lock &lock = file_locks[file_id];

  if (lock.owner_uid != 0)
    return LOCK_ERROR;

  lock.owner_uid = client.user_id;
  lock.owner_ip = client.ip;
  lock.ex_proc = client.ctc;
  return LOCK_SUCCESS;
}

extern "C"
int get_shared_lock(struct client client, uint32_t file_id) {
  assert(client.user_id != 0);

  std::lock_guard<std::mutex> lk(file_m);
  file_lock &lock = file_locks[file_id];

  if (lock.owner_uid == 0) {
    lock.owner_uid = client.user_id;
    lock.owner_ip = client.ip;
  } else if (lock.owner_uid != client.user_id || lock.owner_ip != client.ip) {
    return LOCK_ERROR;
  }

  auto insertion = lock.sh_procs.insert(client.ctc);
  return insertion.second ? LOCK_SUCCESS : LOCK_ERROR;
}

extern "C"
int release_lock(struct client client, uint32_t file_id) {
  assert(client.user_id != 0);

  std::lock_guard<std::mutex> lk(file_m);
  file_lock &lock = file_locks[file_id];

  if (lock.owner_uid != client.user_id || lock.owner_ip != client.ip)
    return LOCK_ERROR;

  if (lock.ex_proc == client.ctc) {
    file_locks.erase(file_id);
  } else {
    auto erasure = lock.sh_procs.erase(client.ctc);
    if (erasure == 0)
      return LOCK_ERROR;

    if (lock.sh_procs.size() == 0) {
      file_locks.erase(file_id);
    }
  }
  return LOCK_SUCCESS;
}

extern "C"
int has_exclusive_lock(struct client client, uint32_t file_id) {
  assert(client.user_id != 0);

  std::lock_guard<std::mutex> lk(file_m);
  auto lock_it = file_locks.find(file_id);

  if (lock_it != file_locks.end() &&
      lock_it->second.owner_uid == client.user_id &&
      lock_it->second.owner_ip == client.ip &&
      lock_it->second.ex_proc == client.ctc)
    return 1;

  return 0;
}

extern "C"
int has_shared_lock(struct client client, uint32_t file_id) {
  assert(client.user_id != 0);

  std::lock_guard<std::mutex> lk(file_m);
  auto lock_it = file_locks.find(file_id);

  if (lock_it != file_locks.end() &&
      lock_it->second.owner_uid == client.user_id &&
      lock_it->second.owner_ip == client.ip &&
      lock_it->second.sh_procs.count(client.ctc) != 0)
    return 1;

  return 0;
}

extern "C"
int get_metadata_lock(struct client client) {
  assert(client.user_id != 0);

  std::lock_guard<std::mutex> lk(meta_lock_singleton.mtx);

  if (meta_lock_singleton.owner_uid == 0) {
    meta_lock_singleton.owner_uid = client.user_id;
    meta_lock_singleton.owner_ip = client.ip;
    meta_lock_singleton.proc = client.ctc;
    return LOCK_SUCCESS;
  }

  return LOCK_ERROR;
}

extern "C"
int release_metadata_lock(struct client client) {
  assert(client.user_id != 0);

  std::lock_guard<std::mutex> lk(meta_lock_singleton.mtx);

  if (meta_lock_singleton.owner_uid == client.user_id &&
      meta_lock_singleton.owner_ip == client.ip &&
      meta_lock_singleton.proc == client.ctc) {
    meta_lock_singleton.owner_uid = 0;
    return LOCK_SUCCESS;
  }

  return LOCK_ERROR;
}

extern "C"
int has_metadata_lock(struct client client) {
  assert(client.user_id != 0);

  std::lock_guard<std::mutex> lk(meta_lock_singleton.mtx);

  return meta_lock_singleton.owner_uid == client.user_id &&
         meta_lock_singleton.owner_ip == client.ip &&
         meta_lock_singleton.proc == client.ctc;
}
