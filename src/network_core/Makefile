.SUFFIXES:
.SUFFIXES: .c .o
CLNT = decafs_barista
SRVR = decafs_espresso
CFLAGS = -g -Wall

SRVR_OBJ = esp_svc.o network_xdr.o network_svc.o
CLNT_OBJ = network_xdr.o network_clnt.o bar_clnt.o

.c.o:; gcc -c -o $@ $(CFLAGS) $<

default: $(CLNT) $(SRVR)

$(CLNT): $(CLNT_OBJ)
	gcc -o $(CLNT) $(CLNT_OBJ)

$(SRVR): $(SRVR_OBJ)
	gcc -o $(SRVR) $(SRVR_OBJ)

clean:
	rm *.o $(CLNT) $(SRVR)
	rm -i *~
	rm core
