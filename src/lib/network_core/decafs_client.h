#ifndef _DECAFS_CLIENT_H_
#define _DECAFS_CLIENT_H_

#include <iostream>
#include <dirent.h>
#include <sys/statvfs.h>

#include "net_tcp/tcp_client.h"
#include "espresso_packet_processor.h"

// NETWORK PACKETS
#include "network_packets.h"

#include "open_packet.h"
#include "open_packet_response.h"

#include "write_packet.h"
#include "write_response_packet.h"

#include "close_packet.h"
#include "close_response_packet.h"

#include "read_packet.h"
#include "read_response_packet.h"

#include "lseek_packet.h"
#include "lseek_response_packet.h"

#include "opendir_packet.h"
#include "opendir_response_packet.h"

#include "file_storage_stat_packet.h"
#include "file_storage_stat_response_packet.h"

class DecafsClient : public TcpClient {

  private:
    uint32_t user_id;

  public:
    DecafsClient(std::string hostname, unsigned short port, uint32_t user_id);

    void connectionClosed();
    void connectionEstablished();
    void handleMessageFromServer(int socket);

    int open(const char* pathname, int flags);
    ssize_t read(int fd, void* buf, ssize_t count);
    ssize_t write(int fd, void* buf, ssize_t count);
    int close(int fd);
    off_t lseek(int fd, off_t offset, int whence);
    void delete_file(char* pathname);
    void sync();
    int file_stat(const char* path, struct stat *buf);
    int file_fstat(int fd, struct stat *buf);
    void statfs(char *pathname, struct statvfs*);
    int mkdir(const char* dirname);
    decafs_dir* opendir(const char* name);

    // NON POSIX MIMICING METHODS
    FileStorageStatResponsePacket* file_storage_stat(const char* name);
};

#endif // _ESPRESSO_CLIENT_H_
