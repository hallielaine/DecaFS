lib_network := src/lib/network_core
lib_espresso_storage := src/lib/espresso_storage
lib_ip_address := src/lib/ip_address
lib_access := src/lib/access
lib_nettcp := src/lib/net_tcp

libraries := $(lib_network) $(lib_espresso_storage) $(lib_ip_address) $(lib_access) $(lib_nettcp)

decafs_barista := src/app/barista_core
decafs_espresso := src/app/espresso_core
apps := $(decafs_barista) $(decafs_espresso)

dirs := $(libraries) $(apps)

.PHONY: all $(apps) $(libraries)
all: $(apps) cp

$(apps): 
	$(MAKE) --directory=$@

$(libraries):
	$(MAKE) --directory=$@
	mkdir -p ./lib
	cp $@/*.a ./lib

cp:
	cp src/app/barista_core/decafs_barista bin
	cp src/app/espresso_core/decafs_espresso bin

$(lib_access): $(lib_network)
$(apps): $(libraries)

clean:
	rm -f bin/* lib/*
	for DIR in ${dirs}; do \
	  make -C $${DIR} clean; \
	done
