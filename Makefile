SHELL := /bin/sh

lib_network := src/lib/network_core
lib_espresso_storage := src/lib/espresso_storage
lib_ip_address := src/lib/ip_address
lib_access := src/lib/access
lib_locking := src/lib/locking_strategy
lib_nettcp := src/lib/net_tcp

libraries := $(lib_network) $(lib_espresso_storage) $(lib_ip_address) $(lib_access) $(lib_locking) $(lib_nettcp)

decafs_barista := src/app/barista_core
decafs_espresso := src/app/espresso_core

apps := $(decafs_barista) $(decafs_espresso)

dirs := $(libraries) $(apps)

.PHONY: all cp_bins $(apps) $(libraries)

all: $(apps) cp_bins

$(apps): $(libraries)
	$(MAKE) -C $@

$(libraries):
	$(MAKE) -C $@
	mkdir -p ./lib
	cp $@/*.a ./lib

cp_bins:
	mkdir -p ./bin
	cp src/app/barista_core/decafs_barista bin
	cp src/app/espresso_core/decafs_espresso bin

clean:
	$(RM) bin/* lib/*
	for DIR in ${dirs}; do make -C $${DIR} clean; done
