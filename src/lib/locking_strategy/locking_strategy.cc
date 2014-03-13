#include "locking_strategy.h"

#include <cassert>
#include <unordered_map>
#include <set>
#include <utility>
#include <mutex>
#include <chrono>


#define UNUSED __attribute__((unused));


struct file_lock {
  std::mutex m;
  std::condition_variable cv;
  int owner;
  int exclusive_proc;
  std::set<int> shared_procs;
  bool exclusive_intent;

  file_lock() : owner(-1), exclusive_proc(-1), exclusive_intent(false) {}
};

static std::mutex regular_m;
static std::unordered_map<int, file_lock> regular_locks;
static std::mutex metadata_m;
static std::unordered_map<int, file_lock> metadata_locks;

inline int exlock(std::mutex &m, std::unordered_map<int, file_lock> &locks,
    int user_id, int proc_id, int file_id, int timeout) {
  if (timeout < 0) {
    // lock around accessing the map
    file_lock *fl;
    {
      std::lock_guard<std::mutex> lk(m);
      fl = &locks[file_id];
    }

    // acquire mutation lock for this file_lock
    std::unique_lock<std::mutex> lk(fl->m);

    if (user_id != fl->owner) {
      fl->cv.wait(lk, [fl]{ return fl->owner == -1; });

      // if just gaining ownership, lock should be all clear
      assert(!fl->exclusive_intent);
      assert(fl->exclusive_proc == -1);
      assert(fl->shared_procs.size() == 0);

      fl->owner = user_id;
      fl->exclusive_proc = proc_id;
      return 0;
    }
    if (fl->exclusive_intent) {
      fl->cv.wait(lk, [fl]{ return !fl->exclusive_intent; });
      fl->exclusive_intent = true;
    }

    // at this point this user_id owns the file_lock and has exclusive_intent

    if (fl->exclusive_proc != -1 || fl->shared_procs.size() != 0) {
      fl->cv.wait(lk, [fl]{ return fl->exclusive_proc == -1 &&
          fl->shared_procs.size() == 0; });
    }

    fl->exclusive_proc = proc_id;
    fl->exclusive_intent = false;
    return 0;
  }

  return -1;
}

inline int shlock(std::mutex &m, std::unordered_map<int, file_lock> &locks,
    int user_id, int proc_id, int file_id, int timeout) {
  if (timeout < 0) {
    file_lock *fl;
    {
      std::lock_guard<std::mutex> lk(m);
      fl = &locks[file_id];
    }

    // acquire mutation lock for this file_lock
    std::unique_lock<std::mutex> lk(fl->m);

    if (user_id != fl->owner) {
      fl->cv.wait(lk, [fl]{ return fl->owner == -1; });

      // if just gaining ownership, lock should be all clear
      assert(!fl->exclusive_intent);
      assert(fl->exclusive_proc == -1);
      assert(fl->shared_procs.size() == 0);

      fl->owner = user_id;
      fl->shared_procs.insert(proc_id);
      return 0;
    }
    if (fl->exclusive_proc || fl->exclusive_intent) {
      fl->cv.wait(lk, [fl]{ return !fl->exclusive_proc &&
          !fl->exclusive_intent; });
    }

    auto r = fl->shared_procs.insert(proc_id);
    return r.second ? 0 : -1;
  }

  return -1;
}

static int rlock(std::mutex &m, std::unordered_map<int, file_lock> &locks,
    int user_id, int proc_id, int file_id) {
  file_lock *fl;
  {
    std::lock_guard<std::mutex> lk(m);
    fl = &locks[file_id];
  }

  std::unique_lock<std::mutex> lk(fl->m);
  if (user_id != fl->owner)
    return -1;
  if (proc_id == fl->exclusive_proc) {
    fl->exclusive_proc = -1;
  } else {
    const auto flit = fl->shared_procs.find(proc_id);
    if (flit == fl->shared_procs.end())
      return -1;
    fl->shared_procs.erase(flit);
  }

  lk.unlock();
  fl->cv.notify_all();
  return 0;
}

extern "C"
int get_exclusive_lock(int user_id, int proc_id, int file_id, int timeout) {
  return exlock(regular_m, regular_locks, user_id, proc_id, file_id, timeout);
}

extern "C"
int get_shared_lock(int user_id, int proc_id, int file_id, int timeout) {
  return shlock(regular_m, regular_locks, user_id, proc_id, file_id, timeout);
}

extern "C"
int release_lock(int user_id, int proc_id, int file_id) {
  return rlock(regular_m, regular_locks, user_id, proc_id, file_id);
}

extern "C"
int has_exclusive_lock(int user_id, int proc_id, int file_id) {
  return -1;
}

extern "C"
int has_shared_lock(int user_id, int proc_id, int file_id) {
  return -1;
}

extern "C"
int get_exclusive_metadata_lock(int user_id, int proc_id, int file_id, int timeout) {
  return exlock(metadata_m, metadata_locks, user_id, proc_id, file_id, timeout);
}

extern "C"
int get_shared_metadata_lock(int user_id, int proc_id, int file_id, int timeout) {
  return shlock(metadata_m, metadata_locks, user_id, proc_id, file_id, timeout);
}

extern "C"
int release_metadata_lock(int user_id, int proc_id, int file_id) {
  return rlock(metadata_m, metadata_locks, user_id, proc_id, file_id);
}

extern "C"
int has_exclusive_metadata_lock(int user_id, int proc_id, int file_id) {
  return -1;
}

extern "C"
int has_shared_metadata_lock(int user_id, int proc_id, int file_id) {
  return -1;
}
