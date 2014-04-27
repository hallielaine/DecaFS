#ifndef PERSISTENT_SET_H_
#define PERSISTENT_SET_H_

#include <cstring>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include <map>
#include <memory>
#include <set>
#include <utility>

template<typename Entry>
class PersistentSet {
  std::map<Entry, Entry*> entries;
  std::set<std::pair<int, int> > unallocated_extents;
  Entry *map;
  off_t map_size;
  int fd;

  Entry* allocate() {
    std::set<std::pair<int, int> >::iterator begin = unallocated_extents.begin();
    if (begin != unallocated_extents.end()) {
      Entry *allocation = map + begin->first;

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

    int start_of_new = map_size / sizeof(Entry);
    munmap(map, map_size);
    map_size += sizeof(Entry) * 10;
    ftruncate(fd, map_size);
    map = static_cast<Entry*>(mmap(NULL, map_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));
    unallocated_extents.insert(std::make_pair(start_of_new + 1, start_of_new + 10));
    return map + start_of_new;
  }

  void deallocate(Entry *p) {
    int start = (p - map) / sizeof(Entry);

    memset(p, 0, sizeof(Entry));

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
  explicit inline PersistentSet() : fd(-1) {};

  inline ~PersistentSet() {
    if (fd >= 0) {
      flush();
      close();
    }
  }

  int open(const char *filename) {
    struct stat sd;

    fd = ::open(filename, O_RDWR | O_CREAT, 0600);
    if (fd < 0)
      return -1;
    if (fstat(fd, &sd) < 0)
      return -1;

    map_size = sd.st_size;
    map = static_cast<Entry*>(mmap(NULL, map_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));
    if (map == MAP_FAILED)
      return -1;

    Entry * const begin = map;
    Entry * const end = map + map_size / sizeof(Entry);
    static char zeros[sizeof(Entry)];
    for (Entry *entry = begin; entry != end; ++entry) {
      if (memcmp(entry, zeros, sizeof(Entry)) == 0) {
        deallocate(entry);
      } else {
        entries.insert(std::make_pair(*entry, entry));
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

  struct const_iterator : public std::iterator<std::bidirectional_iterator_tag,
                                               Entry> {
    explicit inline const_iterator() {}
    explicit inline const_iterator(typename std::map<Entry, Entry*>::const_iterator it) :
      internal_it(it) {}
    inline const_iterator(const const_iterator &o) {
      internal_it = o.internal_it;
    }

    const_iterator inline operator++() { return const_iterator(++internal_it); }
    const_iterator inline operator++(int) { return const_iterator(internal_it++); }
    const_iterator inline operator--() { return const_iterator(--internal_it); }
    const_iterator inline operator--(int) { return const_iterator(internal_it--); }
    const Entry& operator*() const { return *(internal_it->second); }
    const Entry* operator->() const { return internal_it->second; }
    friend inline bool operator!=(const const_iterator &lhs, const const_iterator &rhs) {
      return lhs.internal_it != rhs.internal_it;
    }
    friend inline bool operator==(const const_iterator &lhs, const const_iterator &rhs) {
      return lhs.internal_it == rhs.internal_it;
    }

  private:
    friend PersistentSet;
    typename std::map<Entry, Entry*>::const_iterator internal_it;
  };

  typedef const_iterator iterator;

  struct const_reverse_iterator : public std::iterator<std::bidirectional_iterator_tag,
                                               Entry> {
    explicit inline const_reverse_iterator() {}
    explicit inline const_reverse_iterator(typename std::map<Entry, Entry*>::const_reverse_iterator it) :
      internal_it(it) {}
    inline const_reverse_iterator(const const_reverse_iterator &o) {
      internal_it = o.internal_it;
    }

    const_reverse_iterator inline operator++() { return const_reverse_iterator(++internal_it); }
    const_reverse_iterator inline operator++(int) { return const_reverse_iterator(internal_it++); }
    const_reverse_iterator inline operator--() { return const_reverse_iterator(--internal_it); }
    const_reverse_iterator inline operator--(int) { return const_reverse_iterator(internal_it--); }
    const Entry& operator*() const { return *(internal_it->second); }
    const Entry* operator->() const { return internal_it->second; }
    friend inline bool operator!=(const const_reverse_iterator &lhs, const const_reverse_iterator &rhs) {
      return lhs.internal_it != rhs.internal_it;
    }
    friend inline bool operator==(const const_reverse_iterator &lhs, const const_reverse_iterator &rhs) {
      return lhs.internal_it == rhs.internal_it;
    }

  private:
    friend PersistentSet;
    typename std::map<Entry, Entry*>::const_reverse_iterator internal_it;
  };

  typedef const_reverse_iterator reverse_iterator;

  inline iterator begin() { return iterator(entries.begin()); }
  inline const_iterator begin() const { return const_iterator(entries.begin()); }
  inline const_iterator cbegin() const { return const_iterator(entries.cbegin()); }
  inline iterator end() { return iterator(entries.end()); }
  inline const_iterator end() const { return const_iterator(entries.end()); }
  inline const_iterator cend() const { return const_iterator(entries.cend()); }
  inline reverse_iterator rbegin() { return reverse_iterator(entries.rbegin()); }
  inline const_reverse_iterator rbegin() const { return const_reverse_iterator(entries.rbegin()); }
  inline const_reverse_iterator crbegin() const { return const_reverse_iterator(entries.crbegin()); }
  inline reverse_iterator rend() { return reverse_iterator(entries.rend()); }
  inline const_reverse_iterator rend() const { return const_reverse_iterator(entries.rend()); }
  inline const_reverse_iterator crend() const { return const_reverse_iterator(entries.crend()); }

  inline bool empty() const { return entries.empty(); }
  inline size_t size() const { return entries.size(); }

  /* can't return error, probably should avoid */
  void clear() {
    for (auto &entry : entries) {
      entry.second->~Entry();
    }
    entries.clear();
    munmap(map, map_size);
    ftruncate(fd, 0);
    map = static_cast<Entry*>(mmap(NULL, 0, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));
  }

  std::pair<iterator, bool> insert(const Entry& value) {
    std::pair<typename std::map<Entry, Entry*>::iterator, bool> insertion =
      entries.insert(std::make_pair(value, nullptr));
    if (insertion.second) {
      insertion.first->second = allocate();
      new (insertion.first->second) Entry(value);
      return std::make_pair(iterator(insertion.first), true);
    } else {
      return std::make_pair(iterator(insertion.first), false);
    }
  }

  iterator insert(const_iterator hint, const Entry& value) {
    typename std::map<Entry, Entry*>::iterator existing = entries.find(value);
    if (existing != entries.end()) {
      return iterator(existing);
    } else {
      Entry* new_element = allocate();
      new (new_element) Entry(value);
      return iterator(entries.insert(hint.internal_it, std::make_pair(value, new_element)));
    }
  }

  template<class InputIt>
  inline void insert(InputIt first, InputIt last) {
    while (first != last) {
      insert(*first);
      ++first;
    }
  }

  inline void insert(std::initializer_list<Entry> ilist) {
    insert(ilist.begin(), ilist.end());
  }

  iterator erase(const_iterator pos) {
    pos->~Entry();
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


  size_t erase(const Entry& key) {
    iterator search = find(key);
    if (search != end()) {
      erase(search);
      return 1;
    }
    return 0;
  }

  inline size_t count(const Entry &key) const { return entries.count(key); }

  inline iterator find(const Entry &key) { return iterator(entries.find(key)); }
  inline const_iterator find(const Entry &key) const { return const_iterator(entries.find(key)); }

  inline iterator lower_bound(const Entry &key) { return iterator(entries.lower_bound(key)); }
  inline const_iterator lower_bound(const Entry &key) const { return const_iterator(entries.lower_bound(key)); }
};

#endif // PERSISTENT_SET_H_
