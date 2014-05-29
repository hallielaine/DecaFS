#include "persistent_metadata/persistent_metadata.h"
#include "persistent_metadata/persistent_metadata_impl.h"

Persistent_Metadata* Persistent_Metadata::get_instance() {
  #ifdef MOCK_PERSISTENT_METADATA
    return (Persistent_Metadata *)new Persistent_Metadata_Mock();    
  #else
    return (Persistent_Metadata *)new Persistent_Metadata_Impl();
  #endif
} 

