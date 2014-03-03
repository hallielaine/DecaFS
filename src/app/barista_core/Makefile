.SUFFIXES:
.SUFFIXES: .cpp .o
BARISTA = decafs_barista
CFLAGS = -g -Wall
LIBDIRS = ../../lib/network_core
LIBS = -lnetcore

OBJS = barista_core.o volatile_metadata.o 

.cpp.o:; g++ -c -o $@ $(CFLAGS) $<

default: $(BARISTA)

$(BARISTA): $(OBJS)
	g++ -o $(BARISTA) $(OBJS) -L$(LIBDIRS) $(LIBS)

clean:
	rm *.o $(BARISTA)
	rm -i *~
	rm core
