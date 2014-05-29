#include "gmock/gmock.h"

#include "persistent_metadata/persistent_metadata.h"

class Mock_Persistent_Metadata : public Persistent_Metadata {
  public:
    MOCK_METHOD0(get_num_files, int());
    
    MOCK_METHOD1(init, void(char *metadata_path));
    
    MOCK_METHOD2(get_filenames, int(char *filenames[MAX_FILENAME_LENGTH], int size));
    MOCK_METHOD2(decafs_file_sstat, int(char *pathname, struct decafs_file_stat *buf));
    MOCK_METHOD2(decafs_file_stat, int(uint32_t file_id, struct decafs_file_stat *buf));
    MOCK_METHOD2(decafs_stat, int(char *pathname, struct statvfs *buf));
};
