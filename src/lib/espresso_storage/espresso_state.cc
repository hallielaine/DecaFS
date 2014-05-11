#include "espresso_state.h"

espresso_global_data_t espresso_global_data;

void espresso_global_data_init(std::string dir, int max_size) {
  std::string data_name = dir + "espresso_data.dat";
  std::string metadata_name = dir + "espresso_metadata.dat";
  std::string free_extents_name = dir + "espresso_free_extents.dat";

  espresso_global_data.fd = open(data_name.c_str(), O_RDWR | O_CREAT, 0600);
  espresso_global_data.metadata.open(metadata_name.c_str());
  espresso_global_data.free_extents.open(free_extents_name.c_str());

  if (espresso_global_data.free_extents.empty()) {
    data_address whole_file = { .offset = 0, .size = max_size };
    espresso_global_data.free_extents.insert(whole_file);
  }
}
