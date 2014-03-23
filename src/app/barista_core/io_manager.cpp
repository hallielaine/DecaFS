#include "io_manager.h"

IO_Manager::IO_Manager() {

}

ssize_t IO_Manager::process_read_stripe (uint32_t file_id, char *pathname,
                                         uint32_t stripe_id, void *buf,
                                         size_t count) {
  return 0;
}

ssize_t IO_Manager::process_write_stripe (uint32_t file_id, char *pathname,
                                          uint32_t stripe_id, void *buf,
                                          size_t count) {
  return 0;
}

int IO_Manager::set_node_id (uint32_t file_id, uint32_t stripe_id,
                             uint32_t chunk_num, uint32_t node_id) {
  return 0;
}

int IO_Manager::get_node_id (uint32_t file_id, uint32_t stripe_id,
                             uint32_t chunk_num) {
  return 0;
}

int IO_Manager::set_replica_node_id (uint32_t file_id, uint32_t stripe_id,
                                     uint32_t chunk_num, uint32_t node_id) {
  return 0;
}

int IO_Manager::get_replica_node_id (uint32_t file_id, uint32_t stripe_id,
                                     uint32_t chunk_num) {
  return 0;
}

int IO_Manager::stat_file_name (char *pathname, struct decafs_file_stat *buf) {
  return 0;
}

int IO_Manager::stat_file_id (uint32_t file_id, struct decafs_file_stat *buf) {
  return 0;
}

int IO_Manager::stat_replica_name (char *pathname, struct decafs_file_stat *buf) {
  return 0;
}

int IO_Manager::stat_replica_id (uint32_t file_id, struct decafs_file_stat *buf) {
  return 0;
}
