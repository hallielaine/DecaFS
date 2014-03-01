#include "barista_core.h"

Volatile_Metadata::Volatile_Metadata() {
  // initialize volatile metadata map for file cursor

  // intialize node map
  
  // Initialize stripe and chunk size
  chunk_size = 0;
  stripe_size = 0;
}

void Volatile_Metadata::set_chunk_size (uint32_t size) {
  // Set the chunk size if it hasn't been set before
  if (chunk_size == 0) {
    chunk_size = size;
  }
}

uint32_t Volatile_Metadata::get_chunk_size() {
  return chunk_size;
}

void Volatile_Metadata::set_stripe_size (uint32_t size) {
  // Set the stripe size if it hasn't been set before
  if (stripe_size == 0) {
    stripe_size = size;
  }
}

uint32_t Volatile_Metadata::get_stripe_size() {
  return stripe_size;
}
