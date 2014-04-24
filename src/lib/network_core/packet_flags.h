#ifndef _PACKET_TYPES_
#define _PACKET_TYPES_

enum packet_flags {

   // Initialization Packets

   ESPRESSO_INIT = 33,
   DECAFS_CLIENT_INIT = 34,

   // Client -> Barista
   OPEN = 1,
   READ = 2,
   WRITE = 3,
   CLOSE = 4,
   DELETE_FILE = 5,
   SYNC = 6,
   STAT_FS = 7,
   MKDIR = 8,
   OPENDIR = 9,
   READDIR = 10,

   // Barista -> Client
   OPEN_RESPONSE = 11,
   READ_RESPONSE = 12,
   WRITE_RESPONSE = 13,
   CLOSE_RESPONSE = 14,
   DELETE_FILE_RESPONSE = 15,
   SYNC_RESPONSE = 16,
   STAT_FS_RESPONSE = 17,
   MKDIR_RESPONSE = 18,
   OPENDIR_RESPONSE = 19,
   READDIR_RESPONSE = 20,

   // Barista -> Espresso
   READ_CHUNK = 21,
   WRITE_CHUNK = 22,
   DELETE_CHUNK = 23,

   // Espresso -> Barista (ACKS)
   //READ_CHUNK_ACK 24
   //WRITE_CHUNK_ACK 25
   //DELETE_CHUNK_ACK 26

   // Espresso -> Barista
   READ_CHUNK_RESPONSE = 27,
   WRITE_CHUNK_RESPONSE = 28,
   DELETE_CHUNK_RESPONSE = 29

   // Barista -> Espresso (ACKS)
   //READ_CHUNK_RESPONSE_ACK 30
   //WRITE_CHUNK_RESPONSE_ACK 31
   //DELETE_CHUNK_RESPONSE_ACK 32
};

#endif // _PACKET_TYPES_
