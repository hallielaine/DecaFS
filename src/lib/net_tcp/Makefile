.SUFFIXES:
.SUFFIXES: .cpp .o
BIN = libnettcp.a

CFLAGS = -std=c++11 -g -Wall -Wl,--no-as-needed
INCDIR = .
LIBDIRS = ../../../lib 

OBJS = tcp_server.o tcp_client.o connection_to_client.o

.cpp.o:; g++ -c -o $@ $(CFLAGS) -I$(INCDIR) $<

default: $(BIN)

$(BIN): $(OBJS) $(M1) $(M2)
	ar -cvq $(BIN) $(OBJS)

clean:
	rm -rf *.o $(BIN)
	rm -i *~
	rm core
