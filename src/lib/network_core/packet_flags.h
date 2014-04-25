#ifndef _PACKET_TYPES_
#define _PACKET_TYPES_

enum packet_flags {

   // Initialization Packets

   ESPRESSO_INIT = 1,
   DECAFS_CLIENT_INIT = 2,

   // Client -> Barista
   OPEN = 3,
   READ = 4,
   WRITE = 5,
   CLOSE = 6,
   DELETE_FILE = 7,
   SYNC = 8,
   STAT_FS = 9,
   MKDIR = 10,
   OPENDIR = 11,
   READDIR = 12,

   // Barista -> Client
   OPEN_RESPONSE = 13,
   READ_RESPONSE = 14,
   WRITE_RESPONSE = 15,
   CLOSE_RESPONSE = 16,
   DELETE_FILE_RESPONSE = 17,
   SYNC_RESPONSE = 18,
   STAT_FS_RESPONSE = 19,
   MKDIR_RESPONSE = 20,
   OPENDIR_RESPONSE = 21,
   READDIR_RESPONSE = 22,

   // Barista -> Espresso
   READ_CHUNK = 23,
   WRITE_CHUNK = 24,
   DELETE_CHUNK = 25,

   // Espresso -> Barista
   READ_CHUNK_RESPONSE = 26,
   WRITE_CHUNK_RESPONSE = 27,
   DELETE_CHUNK_RESPONSE = 28
};

#endif // _PACKET_TYPES_
