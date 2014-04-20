#ifndef _PACKET_TYPES_
#define _PACKET_TYPES_

namespace PacketTypes {
   // Client -> Barista
   #define OPEN 1
   #define READ 2
   #define WRITE 3
   #define CLOSE 4
   #define DELETE_FILE 5
   #define SYNC 6
   #define STAT_FS 7
   #define MKDIR 8
   #define OPENDIR 9
   #define READDIR 10

   // Barista -> Client
   #define OPEN_RESPONSE 11
   #define READ_RESPONSE 12
   #define WRITE_RESPONSE 13
   #define CLOSE_RESPONSE 14
   #define DELETE_FILE_RESPONSE 15
   #define SYNC_RESPONSE 16
   #define STAT_FS_RESPONSE 17
   #define MKDIR_RESPONSE 18
   #define OPENDIR_RESPONSE 19
   #define READDIR_RESPONSE 20

   // Barista -> Espresso
   #define READ_CHUNK 21
   #define WRITE_CHUNK 22
   #define DELETE_CHUNK 23

   // Espresso -> Barista (ACKS)
   #define READ_CHUNK_ACK 24
   #define WRITE_CHUNK_ACK 25
   #define DELETE_CHUNK_ACK 26

   // Espresso -> Barista
   #define READ_CHUNK_RESPONSE 27
   #define WRITE_CHUNK_RESPONSE 28
   #define DELETE_CHUNK_RESPONSE 29

   // Barista -> Espresso (ACKS)
   #define READ_CHUNK_RESPONSE_ACK 30
   #define WRITE_CHUNK_RESPONSE_ACK 31
   #define DELETE_CHUNK_RESPONSE_ACK 32
}

#endif // _PACKET_TYPES_
