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
  std::condition_variable ex_cv;
  std::condition_variable sh_cv;
  int owner;
  int ex_intent;
  int ex_proc;
  int sh_intent;
  std::set<int> sh_procs;

  file_lock() : owner(-1), ex_intent(0), ex_proc(-1), sh_intent(0) {}
};

static std::mutex reg_m;
static std::unordered_map<int, file_lock> reg_locks;
static std::mutex meta_m;
static std::unordered_map<int, file_lock> meta_locks;

file_lock& reg_fl(int file_id) {
  std::lock_guard<std::mutex> lk(reg_m);
  return reg_locks[file_id];
}

file_lock& meta_fl(int file_id) {
  std::lock_guard<std::mutex> lk(meta_m);
  return meta_locks[file_id];
}

inline int ex_lock(file_lock &fl, int user_id, int proc_id, int timeout) {
  if (timeout < 0) {
    // acquire mutation lock for this file_lock
    std::unique_lock<std::mutex> lk(fl.m);

    if (user_id != fl.owner) {
      // wait for the owner to release the file or another process of this
      // user_id to acquire it
      fl.ex_intent++;
      fl.ex_cv.wait(lk, [&fl, user_id]{ return fl.owner == -1 ||
                                               fl.owner == user_id; });
      fl.ex_intent--;

      if (fl.owner == -1) {
        // an unowned file should not have active locks...
        assert(fl.ex_proc == -1);
        assert(fl.sh_procs.size() == 0);

        // acquire ownership
        fl.owner = user_id;
      }
    }

    if (fl.ex_proc != -1 || fl.sh_procs.size() != 0) {
      // wait for all locks to be released
      fl.ex_intent++;
      fl.ex_cv.wait(lk, [&fl]{ return fl.ex_proc == -1 &&
                                      fl.sh_procs.size() == 0; });
      fl.ex_intent--;
    }

    // acquire exclusive lock
    fl.ex_proc = proc_id;
    return 0;
  }

  return -1;
}

inline int sh_lock(file_lock &fl, int user_id, int proc_id, int timeout) {
  if (timeout < 0) {
    // acquire mutation lock for this file_lock
    std::unique_lock<std::mutex> lk(fl.m);

    if (user_id != fl.owner) {
      // wait for the owner to release the file or another process of this
      // user_id to acquire it
      fl.sh_intent++;
      fl.sh_cv.wait(lk, [&fl, user_id]{ return fl.owner == -1 ||
                                               fl.owner == user_id; });
      fl.sh_intent--;

      if (fl.owner == -1) {
        // an unowned file should not have active locks...
        assert(fl.ex_proc == -1);
        assert(fl.sh_procs.size() == 0);

        // acquire ownership
        fl.owner = user_id;
      }
    }

    if (fl.ex_proc != -1) {
      // wait for the exclusive lock to be released
      fl.sh_intent++;
      fl.sh_cv.wait(lk, [&fl]{ return fl.ex_proc == -1; });
      fl.sh_intent--;
    }

    // acquire shared lock
    auto r = fl.sh_procs.insert(proc_id);
    return r.second ? 0 : -1;
  }


  return -1;
}

static int exsh_unlock(file_lock &fl, int user_id, int proc_id) {
  std::unique_lock<std::mutex> lk(fl.m);
  if (user_id != fl.owner)
    return -1;
  if (proc_id == fl.ex_proc) {
    fl.ex_proc = -1;
  } else {
    const auto flit = fl.sh_procs.find(proc_id);
    if (flit == fl.sh_procs.end())
      return -1;
    fl.sh_procs.erase(flit);
  }

  if (fl.ex_intent > 0) {
    fl.ex_cv.notify_one();
  } else if (fl.sh_intent > 0) {
    fl.sh_cv.notify_all();
  } else {
    // TODO: delete the record instead
    fl.owner = -1;
  }
  return 0;
}

extern "C"
int get_exclusive_lock(int user_id, int proc_id, int file_id, int timeout) {
  if (user_id < 0 || proc_id < 0 || file_id < 0) return -1;
  return ex_lock(reg_fl(file_id), user_id, proc_id, timeout);
}

extern "C"
int get_shared_lock(int user_id, int proc_id, int file_id, int timeout) {
  if (user_id < 0 || proc_id < 0 || file_id < 0) return -1;
  return sh_lock(reg_fl(file_id), user_id, proc_id, timeout);
}

extern "C"
int release_lock(int user_id, int proc_id, int file_id) {
  if (user_id < 0 || proc_id < 0 || file_id < 0) return -1;
  return exsh_unlock(reg_fl(file_id), user_id, proc_id);
}

extern "C"
int has_exclusive_lock(int user_id, int proc_id, int file_id) {
  if (user_id < 0 || proc_id < 0 || file_id < 0) return -1;
  return -1;
}

extern "C"
int has_shared_lock(int user_id, int proc_id, int file_id) {
  if (user_id < 0 || proc_id < 0 || file_id < 0) return -1;
  return -1;
}

extern "C"
int get_exclusive_metadata_lock(int user_id, int proc_id, int file_id, int timeout) {
  if (user_id < 0 || proc_id < 0 || file_id < 0) return -1;
  return ex_lock(meta_fl(file_id), user_id, proc_id, timeout);
}

extern "C"
int get_shared_metadata_lock(int user_id, int proc_id, int file_id, int timeout) {
  if (user_id < 0 || proc_id < 0 || file_id < 0) return -1;
  return sh_lock(meta_fl(file_id), user_id, proc_id, timeout);
}

extern "C"
int release_metadata_lock(int user_id, int proc_id, int file_id) {
  if (user_id < 0 || proc_id < 0 || file_id < 0) return -1;
  return exsh_unlock(meta_fl(file_id), user_id, proc_id);
}

extern "C"
int has_exclusive_metadata_lock(int user_id, int proc_id, int file_id) {
  if (user_id < 0 || proc_id < 0 || file_id < 0) return -1;
  return -1;
}

extern "C"
int has_shared_metadata_lock(int user_id, int proc_id, int file_id) {
  if (user_id < 0 || proc_id < 0 || file_id < 0) return -1;
  return -1;
}
