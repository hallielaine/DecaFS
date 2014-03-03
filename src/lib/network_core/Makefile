.SUFFIXES:
.SUFFIXES: .c .o
NETWORK = libnetcore.a
CFLAGS = -g -Wall

OBJS = esp_svc.o network_svc.o network_xdr.o network_clnt.o bar_clnt.o

.c.o:; gcc -c -o $@ $(CFLAGS) $<

default: $(NETWORK)

$(NETWORK): $(OBJS)
	ar -cvq $(NETWORK) $(OBJS)
	#gcc -o $(NETWORK) $(OBJS)

clean:
	rm *.o $(NETWORK)
	rm -i *~
	rm core
