#ifndef PERSISTENT_MAP_H_
#define PERSISTENT_MAP_H_

#include <cstring>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include <map>
#include <memory>
#include <set>
#include <utility>

template<typename Key, typename T>
class PersistentMap {
  std::map<Key, std::pair<Key, T>*> entries;
  std::set<std::pair<int, int> > unallocated_extents;
  std::pair<Key, T> *map;
  off_t map_size;
  int fd;

  std::pair<Key, T>* allocate() {
    std::set<std::pair<int, int> >::iterator begin = unallocated_extents.begin();
    if (begin != unallocated_extents.end()) {
      std::pair<Key, T> *allocation = map + begin->first;

      if (begin->first + 1 == begin->second) {
        unallocated_extents.erase(begin);
      } else {
        std::pair<int, int> new_extent =
          std::make_pair(begin->first + 1, begin->second);
        unallocated_extents.erase(begin);
        unallocated_extents.insert(new_extent);
      }

      return allocation;
    }

    int start_of_new = map_size / sizeof(std::pair<Key, T>);
    munmap(map, map_size);
    map_size += sizeof(std::pair<Key, T>) * 10;
    ftruncate(fd, map_size);
    map = static_cast<std::pair<Key, T>*>(mmap(NULL, map_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));
    unallocated_extents.insert(std::make_pair(start_of_new + 1, start_of_new + 10));
    return map + start_of_new;
  }

  void deallocate(std::pair<Key, T> *p) {
    int start = (p - map) / sizeof(std::pair<Key, T>);

    memset(&p->first, 0, sizeof(Key));

    if (unallocated_extents.empty()) {
      unallocated_extents.insert(std::make_pair(start, start + 1));
    } else {
      auto new_extent = std::make_pair(start, start + 1);
      auto after = unallocated_extents.lower_bound(new_extent);
      auto before = after;

      if (before != unallocated_extents.begin()) {
        --before;
        if (before->second == new_extent.first) {
          new_extent.first = before->first;
          unallocated_extents.erase(before);
        }
      }

      if (after != unallocated_extents.end()) {
        if (after->first == new_extent.second) {
          new_extent.second = after->second;
          unallocated_extents.erase(after);
        }
      }

      unallocated_extents.insert(new_extent);
    }
  }

public:
  explicit inline PersistentMap() : fd(-1) {};

  inline ~PersistentMap() {
    if (fd >= 0)
      close();
  }

  int open(const char *filename) {
    struct stat sd;

    fd = ::open(filename, O_RDWR | O_CREAT, 0600);
    if (fd < 0)
      return -1;
    if (fstat(fd, &sd) < 0)
      return -1;

    map_size = sd.st_size;
    map = static_cast<std::pair<Key, T>*>(mmap(NULL, map_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));
    if (map == MAP_FAILED)
      return -1;

    std::pair<Key, T> * const begin = map;
    std::pair<Key, T> * const end = map + map_size / sizeof(std::pair<Key, T>);
    static char zeros[sizeof(Key)];
    for (std::pair<Key, T> *entry = begin; entry != end; ++entry) {
      if (memcmp(&entry->first, zeros, sizeof(Key)) == 0) {
        deallocate(entry);
      } else {
        entries.insert(std::make_pair(entry->first, entry));
      }
    }

    return 0;
  }

  int flush() {
    return msync(map, map_size, MS_SYNC);
  }

  int close() {
    int err = 0;
    err |= flush();
    err |= ::munmap(map, map_size);
    err |= ::close(fd);
    entries.clear();
    unallocated_extents.clear();
    fd = -1;
    return err;
  }

  T& operator[](const Key& key) {
    std::pair<Key, T> *v = entries[key];
    if (v == nullptr) {
      v = allocate();
      new (v) std::pair<Key, T>(key, T());
    }
    return v->second;
  }

  struct iterator : public std::iterator<std::bidirectional_iterator_tag,
                                         std::pair<Key, T> > {
    explicit inline iterator() {}
    explicit inline iterator(typename std::map<Key, std::pair<Key, T>*>::iterator it) :
      internal_it(it) {}
    inline iterator(iterator &o) {
      internal_it = o.internal_it;
      return *this;
    }

    iterator inline operator++() { return iterator(++internal_it); }
    iterator inline operator++(int) { return iterator(internal_it++); }
    iterator inline operator--() { return iterator(--internal_it); }
    iterator inline operator--(int) { return iterator(internal_it--); }
    std::pair<Key, T>& operator*() const { return *(internal_it->second); }
    std::pair<Key, T>* operator->() const { return internal_it->second; }
    friend inline bool operator!=(const iterator &lhs, const iterator &rhs) {
      return lhs.internal_it != rhs.internal_it;
    }
    friend inline bool operator==(const iterator &lhs, const iterator &rhs) {
      return lhs.internal_it == rhs.internal_it;
    }

  private:
    friend PersistentMap;
    typename std::map<Key, std::pair<Key, T>*>::iterator internal_it;
  };

  struct const_iterator : public std::iterator<std::bidirectional_iterator_tag,
                                               std::pair<Key, T> > {
    explicit inline const_iterator() {}
    explicit inline const_iterator(typename std::map<Key, std::pair<Key, T>*>::iterator it) :
      internal_it(it) {}
    inline const_iterator(iterator &it) : internal_it(it.internal_it) {}
    inline const_iterator(const_iterator &o) {
      internal_it = o.internal_it;
      return *this;
    }

    const_iterator inline operator++() { return const_iterator(++internal_it); }
    const_iterator inline operator++(int) { return const_iterator(internal_it++); }
    const_iterator inline operator--() { return const_iterator(--internal_it); }
    const_iterator inline operator--(int) { return const_iterator(internal_it--); }
    const std::pair<Key, T>& operator*() const { return *(internal_it->second); }
    const std::pair<Key, T>* operator->() const { return internal_it->second; }
    friend inline bool operator!=(const const_iterator &lhs, const const_iterator &rhs) {
      return lhs.internal_it != rhs.internal_it;
    }
    friend inline bool operator==(const const_iterator &lhs, const const_iterator &rhs) {
      return lhs.internal_it == rhs.internal_it;
    }

  private:
    friend PersistentMap;
    typename std::map<Key, std::pair<Key, T>*>::const_iterator internal_it;
  };

  inline iterator begin() { return iterator(entries.begin()); }
  inline const_iterator begin() const { return const_iterator(entries.begin()); }
  inline const_iterator cbegin() const { return const_iterator(entries.cbegin()); }
  inline iterator end() { return iterator(entries.end()); }
  inline const_iterator end() const { return const_iterator(entries.end()); }
  inline const_iterator cend() const { return const_iterator(entries.cend()); }
  inline iterator rbegin() { return iterator(entries.rbegin()); }
  inline const_iterator rbegin() const { return const_iterator(entries.rbegin()); }
  inline const_iterator crbegin() const { return const_iterator(entries.crbegin()); }
  inline iterator rend() { return iterator(entries.rend()); }
  inline const_iterator rend() const { return const_iterator(entries.rend()); }
  inline const_iterator crend() const { return const_iterator(entries.crend()); }

  inline bool empty() const { return entries.empty(); }
  inline size_t size() const { return entries.size(); }

  /* can't return error, probably should avoid */
  void clear() {
    for (auto &entry : entries) {
      using ::std::pair;
      entry.second->~pair<Key, T>();
    }
    entries.clear();
    munmap(map, map_size);
    ftruncate(fd, 0);
    map = static_cast<std::pair<Key, T>*>(mmap(NULL, 0, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));
  }

  std::pair<iterator, bool> insert(const std::pair<Key, T>& value) {
    std::pair<typename std::map<Key, std::pair<Key, T>*>::iterator, bool> insertion =
      entries.insert(std::make_pair(value.first, nullptr));
    if (insertion.second) {
      insertion.first->second = allocate();
      new (insertion.first->second) std::pair<Key, T>(value);
      return make_pair(iterator(insertion.first), true);
    } else {
      return make_pair(iterator(insertion.first), false);
    }
  }

  iterator insert(const_iterator hint, const std::pair<Key, T>& value) {
    typename std::map<Key, T*>::iterator existing = entries.find(value.first);
    if (existing != entries.end()) {
      return iterator(existing);
    } else {
      std::pair<Key, T>* new_element = allocate();
      new (new_element) std::pair<Key, T>(value);
      return iterator(entries.insert(hint.internal_it, make_pair(value.first, new_element)));
    }
  }

  template<class InputIt>
  inline void insert(InputIt first, InputIt last) {
    while (first != last) {
      insert(*first);
      ++first;
    }
  }

  inline void insert(std::initializer_list<std::pair<Key, T> > ilist) {
    insert(ilist.begin(), ilist.end());
  }

  iterator erase(const_iterator pos) {
    using ::std::pair;
    pos->~pair<Key, T>();
    return iterator(entries.erase(pos.internal_it));
  }

  iterator erase(const_iterator first, const_iterator last) {
    iterator ret;
    while (first != last) {
      iterator next = ++first;
      ret = erase(first);
      first = next;
    }
    return ret;
  }


  size_t erase(const Key& key) {
    iterator search = find(key);
    if (search != end()) {
      erase(search);
      return 1;
    }
    return 0;
  }

  inline size_t count(const Key &key) const { return entries.count(key); }

  inline iterator find(const Key &key) { return iterator(entries.find(key)); }
  inline const_iterator find(const Key &key) const { return const_iterator(entries.find(key)); }

  inline iterator lower_bound(const Key &key) { return iterator(entries.lower_bound(key)); }
  inline const_iterator lower_bound(const Key &key) const { return const_iterator(entries.lower_bound(key)); }
};

#endif // PERSISTENT_MAP_H_
