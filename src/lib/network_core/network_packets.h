#ifndef _NETWORK_PACKETS_H_
#define _NETWORK_PACKETS_H_

#include <stdint.h>
#include <stdlib.h>
#include <iostream>

#include "packet_flags.h"
#include "net_tcp/tcp_server.h"

// -------------------------------- Packet -------------------------
class Packet {

  private:
    static const int header_size = 3*sizeof(uint32_t); // packet_size, seq_num and flag

  protected:
    // returns how much of the packet buffer is filled by this class
    virtual int dataSize();
    virtual std::ostream& print(std::ostream&) const;

  public:

    int packet_size;
    void *packet;

    uint32_t seq_num;
    uint32_t flag;

    Packet(uint32_t flag, int derived_size);
    Packet(void* buf, ssize_t count);

    friend std::ostream& operator<<(std::ostream& stream, const Packet &packet);
};

// -------------------------- FilePacket -------------------------------
class FilePacket : public Packet {

  private:
    static const int data_size = 6*sizeof(uint32_t);

  protected:
    virtual int dataSize();
    virtual std::ostream& print(std::ostream&) const;

  public:

    uint32_t fd;
    uint32_t file_id;
    uint32_t stripe_id;
    uint32_t chunk_num;
    uint32_t offset;
    uint32_t count;

    FilePacket(void* buf, ssize_t size);
    FilePacket(int flag, int derived_size, uint32_t fd, uint32_t file_id, 
     uint32_t stripe_id, uint32_t chunk_num, uint32_t offset, uint32_t count);

    friend std::ostream& operator<<(std::ostream& stream, const FilePacket &packet);
};

// ---------------------------------- FileDataPacket ---------------------------
class FileDataPacket : public FilePacket {

  private:
    int data_size;

  protected:
    virtual int dataSize();
    virtual std::ostream& print(std::ostream&) const;

  public:

    uint8_t* data_buffer;

    FileDataPacket(void* buf, ssize_t size);
    FileDataPacket(int flag, int derived_size, uint32_t fd, uint32_t file_id, 
     uint32_t stripe_id, uint32_t chunk_num, uint32_t offset, uint32_t count,
     uint8_t* buf);

    friend std::ostream& operator<<(std::ostream& stream, const FileDataPacket &packet);
};

// ------------------------------------ ReadChunkRequest --------------------------
class ReadChunkRequest : public FilePacket {

  protected: 
    virtual std::ostream& print(std::ostream&) const;

  public:
    ReadChunkRequest(void* buf, ssize_t size);
    ReadChunkRequest(uint32_t fd, uint32_t file_id, uint32_t stripe_id, 
     uint32_t chunk_num, uint32_t offset, uint32_t count);

    friend std::ostream& operator<<(std::ostream& stream, const ReadChunkRequest &req);
};

// ------------------------------------ WriteChunkResponse --------------------------
class WriteChunkResponse : public FilePacket {

  protected: 
    virtual std::ostream& print(std::ostream&) const;

  public:
    WriteChunkResponse(void* buf, ssize_t size);
    WriteChunkResponse(uint32_t fd, uint32_t file_id, uint32_t stripe_id, 
     uint32_t chunk_num, uint32_t offset, uint32_t count);

    friend std::ostream& operator<<(std::ostream& stream, const WriteChunkResponse &res);
};

// ---------------------------------- WriteChunkRequest ---------------------------
class WriteChunkRequest : public FileDataPacket {

  protected:
    virtual std::ostream& print(std::ostream&) const;

  public:
    WriteChunkRequest(void* buf, ssize_t size);
    WriteChunkRequest(uint32_t fd, uint32_t file_id, uint32_t stripe_id, 
     uint32_t chunk_num, uint32_t offset, uint32_t count, uint8_t* buf);

    friend std::ostream& operator<<(std::ostream& stream, const WriteChunkRequest &req);
};

// ---------------------------------- ReadChunkResponse ---------------------------
class ReadChunkResponse : public FileDataPacket {

  protected:
    virtual std::ostream& print(std::ostream&) const;

  public:
    ReadChunkResponse(void* buf, ssize_t size);
    ReadChunkResponse(uint32_t fd, uint32_t file_id, uint32_t stripe_id, 
     uint32_t chunk_num, uint32_t offset, uint32_t count, uint8_t* buf);

    friend std::ostream& operator<<(std::ostream& stream, const ReadChunkResponse &req);
};

// ---------------------------------- DeleteChunkRequest ----------------------------

class DeleteChunkRequest : public FilePacket {

  protected: 
    virtual std::ostream& print(std::ostream&) const;

  public:
    DeleteChunkRequest(void* buf, ssize_t size);
    DeleteChunkRequest(uint32_t file_id, uint32_t stripe_id, uint32_t chunk_num);

    friend std::ostream& operator<<(std::ostream& stream, const DeleteChunkRequest &req);
};

// ---------------------------------- DeleteChunkResponse ----------------------------

class DeleteChunkResponse : public FilePacket {

  protected: 
    virtual std::ostream& print(std::ostream&) const;

  public:
    DeleteChunkResponse(void* buf, ssize_t size);
    DeleteChunkResponse(uint32_t file_id, uint32_t stripe_id, uint32_t chunk_num);

    friend std::ostream& operator<<(std::ostream& stream, const DeleteChunkResponse &req);
};

#endif // _NETWORK_PACKETS_H_
