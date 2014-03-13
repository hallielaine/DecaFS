#include <cassert>
#include <memory>
#include <utility>
#include <cstdint>

#include "espresso_storage.h"

#include "espresso_types.h"
#include "espresso_state.h"
#include "espresso_data.h"


#define UNUSED __attribute__((unused))

static int allocate_chunk(data_address *a, int size) {

  for (auto it = espresso_global_data.free_extents.begin();
      it != espresso_global_data.free_extents.end(); ++it) {
    if (size <= it->size) {
      a->offset = it->offset;
      a->size = size;

      if (size < it->size) {
        data_address extra;
        extra.offset = it->offset + size;
        extra.size = it->size - size;
        espresso_global_data.free_extents.insert(extra);
      }

      espresso_global_data.free_extents.erase(it);

      return 0;
    }
  }

  return -1;
}

static int deallocate_chunk(data_address a) {
  if (!espresso_global_data.free_extents.empty()) {
    auto after = espresso_global_data.free_extents.lower_bound(a);
    auto before = after;

    if (after != espresso_global_data.free_extents.end()) {
      if (after->offset == a.offset + a.size) {
        a.size += after->size;
        espresso_global_data.free_extents.erase(after);
      }
    }

    if (before != espresso_global_data.free_extents.begin()) {
      before--;
      if (before->offset + before->size == a.offset) {
        a.offset = before->offset;
        a.size += before->size;
        espresso_global_data.free_extents.erase(before);
      }
    }
  }

  espresso_global_data.free_extents.insert(a);

  return 0;
}

static int reallocate_chunk(data_address *a, int size) {
  deallocate_chunk(*a);

  if (allocate_chunk(a, size) == -1) {
    // need to clean up the mess a deallocation potentially made
    auto it = espresso_global_data.free_extents.lower_bound(*a);

    if (it == espresso_global_data.free_extents.end() ||
        it->offset > a->offset) {
      // deallocation merged before, and the lower_bound skipped it
      --it;
      assert(it->size > a->size);

      int free_before = a->offset - it->offset;
      if (it->size > free_before + a->size) {
        // deallocation also merged after
        data_address new_a = { .offset = a->offset + a->size,
                               .size = it->size - a->size - free_before};
        espresso_global_data.free_extents.insert(it, new_a);
      }

      data_address new_a = { .offset = it->offset, .size = free_before };
      espresso_global_data.free_extents.erase(it);
      espresso_global_data.free_extents.insert(new_a);
    } else if (it->size > a->size) {
      // deallocation merged after
      assert(it->offset == a->offset);

      data_address new_a = { .offset = a->offset + a->size,
                             .size = it->size - a->size};
      espresso_global_data.free_extents.insert(it, new_a);
      espresso_global_data.free_extents.erase(it);
    } else {
      // deallocation performed no merges
      assert(it->offset == a->offset);
      assert(it->size == a->size);
      espresso_global_data.free_extents.erase(it);
    }

    return -1;
  }

  return 0;
}

extern "C"
ssize_t read_chunk(int fd, int file_id, int stripe_id, int chunk_num,
    int offset, void *buf, int count) {
  return read_data(fd, file_id, stripe_id, chunk_num, offset, buf, count);
}

extern "C"
ssize_t write_chunk(int fd, int file_id, int stripe_id, int chunk_num,
    int offset, void *buf, int count) {
  if (count < 0)
    return -1;

  data_descriptor d = { .file_id = file_id,
                        .stripe_id = stripe_id,
                        .chunk_num = chunk_num };

  auto a = espresso_global_data.metadata.find(d);
  if (a == espresso_global_data.metadata.end()) {
    data_address new_a;
    if (allocate_chunk(&new_a, offset + count) == -1)
      return -1;
    espresso_global_data.metadata.insert(std::make_pair(d, new_a));
  } else if (offset + count > a->second.size) {
    std::unique_ptr<uint8_t[]> old_chunk(new uint8_t[a->second.size]);

    if (read_data(fd, file_id, stripe_id, chunk_num, 0, old_chunk.get(),
            a->second.size) == -1)
      return -1;

    if (reallocate_chunk(&a->second, offset + count) == -1)
      return -1;

    if (write_data(fd, file_id, stripe_id, chunk_num, 0, old_chunk.get(),
            a->second.size) == -1)
      return -1;
  }

  return write_data(fd, file_id, stripe_id, chunk_num, offset, buf, count);
}

extern "C"
int delete_chunk(int fd UNUSED, int file_id, int stripe_id, int chunk_num) {
  data_descriptor d = { .file_id = file_id,
                        .stripe_id = stripe_id,
                        .chunk_num = chunk_num };

  auto a = espresso_global_data.metadata.find(d);
  if (a == espresso_global_data.metadata.end())
    return -1;

  if (deallocate_chunk(a->second) == -1)
    return -1;
  espresso_global_data.metadata.erase(a);

  return 0;
}
