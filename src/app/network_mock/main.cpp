#include "barista_core/barista_core.h"
#include "decafs_types/ip_address.h"

#include "network_fake.h"

#define MIN_ARGS 6
#define STRIPE_SIZE 1
#define CHUNK_SIZE 2
#define METADATA 3
#define NUM_ESPRESSOS 4
#define PORT 5

int main(int argc, char** argv) {

   // barista_core_init()
   // set_node_up x 4 or whatever you want
   
   char* args[] = {(char*)"", (char*)"1024", (char*)"512", (char*)"barista_metadata", (char*)"4", (char*)"1024"};
   barista_core_init(6, args);

   set_node_up(1);
   set_node_up(2);
   set_node_up(3);
   set_node_up(4);
 
   struct ip_address ipaddr1 = ip_address((char*)"192.168.1.1"); 
   ConnectionToClient* ctc = NULL; 
   struct client client1 = client(ipaddr1, 1, ctc);

   //open_dir("hello.txt", client1);

   open_file("hello.txt", O_RDWR, client1);
   std::string teststr("testwrite");
   write_file(1, teststr.c_str(), teststr.size(), client1); 

   close_file(1, client1);
   open_file("hello.txt", O_RDWR, client1);
 
   read_file(2, teststr.size(), client1); 
}
