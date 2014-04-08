.SUFFIXES:
.SUFFIXES: .cpp .o
BARISTA = decafs_barista
CFLAGS = -g -Wall
INCDIR = ../../lib
LIBDIRS = ../../../lib 
LIBS = -lnetcore -lip_address -laccess -llocking_strategy --coverage

OBJS = barista_core.o volatile_metadata.o persistent_metadata.o io_manager.o distribution_strategy.o replication_strategy.o

.cpp.o:; g++ -c -o $@ $(CFLAGS) -I$(INCDIR) $<

default: $(BARISTA)

$(BARISTA): $(OBJS)
	g++ -o $(BARISTA) $(OBJS) -L$(LIBDIRS) $(LIBS)

clean:
	rm *.o $(BARISTA)
	rm -i *~
	rm core
