.SUFFIXES:
.SUFFIXES: .cpp .o
ESPRESSO = decafs_espresso
CFLAGS = -g -Wall
INCDIRS = ../../lib
LIBDIRS = ../../lib/network_core
LIBS = -lnetcore

OBJS = espresso_core.o 

.cpp.o:; g++ -c -o $@ $(CFLAGS) -I$(INCDIRS) $<

default: $(ESPRESSO)

$(ESPRESSO): $(OBJS)
	g++ -o $(ESPRESSO) $(OBJS) -L$(LIBDIRS) $(LIBS)

clean:
	rm *.o $(ESPRESSO)
	rm -i *~
	rm core
