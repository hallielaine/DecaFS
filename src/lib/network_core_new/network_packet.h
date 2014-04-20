#ifndef _NETWORK_PACKET_
#define _NETWORK_PACKET_

#define SEQUENCE_NUM_START 1

class NetworkPacket {

   private:
      int m_sequence_num;

   protected:
      int m_size;
      char* m_packet_data;
      int m_packet_type;

   public:
      NetworkPacket(int sequence_num); // default constructor sets sequence_num
      NetworkPacket(int sequence_num, int packet_type);
      NetworkPacket(int sequence_num, int packet_type, int size);

      // Handles formatting of entire packet
      // packet checksum and header formatting
      void send_packet(int client);
      
      // virtual functions
      // do we need any?
};

#endif // _NETWORK_PACKET_
