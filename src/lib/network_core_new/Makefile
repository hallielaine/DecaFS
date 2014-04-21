.SUFFIXES:
.SUFFIXES: .cpp .o

LIB = libnetcore.a
BIN = nettest

CFLAGS = -std=c++11 -g -Wall -Wl,--no-as-needed
INCDIR = ..
LIBDIRS = ../../../lib 

OBJS = barista_server.o espresso_client.o decafs_client.o network_packets.o barista_network_helper.o
MAIN = main.o

.cpp.o:; g++ -c -o $@ $(CFLAGS) -I$(INCDIR) $<

default: $(BIN) $(MAIN)

$(BIN): $(OBJS) $(MAIN)
	ar -cvq $(LIB) $(OBJS)
	g++ -pthread -o $(BIN) $(MAIN) $(LIB) ../net_tcp/libnettcp.a

clean:
	rm -rf *.o $(LIB) $(BIN)
	rm -i *~
	rm core
