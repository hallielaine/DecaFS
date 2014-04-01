.SUFFIXES:
.SUFFIXES: .cpp .o
BIN = libaccess.a
CFLAGS = -g -Wall
INCDIR = ../../lib
LIBDIRS = ../../lib/network_core
LIBS = -lnetcore

OBJS = access.o 

.cpp.o:; g++ -c -o $@ $(CFLAGS) -I$(INCDIR) $<

default: $(BIN)

$(BIN): $(OBJS)
	#g++ -o $(BIN) $(OBJS) -L$(LIBDIRS) $(LIBS)
	ar -cvq $(BIN) $(OBJS) 

clean:
	rm *.o $(BIN)
	rm -i *~
	rm core
