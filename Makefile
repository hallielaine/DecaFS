SHELL := /bin/sh

lib_network := src/lib/network_core
lib_espresso_storage := src/lib/espresso_storage
lib_decafs_types := src/lib/decafs_types
lib_access := src/lib/access
lib_locking := src/lib/locking_strategy
lib_nettcp := src/lib/net_tcp
lib_persistent_metadata := src/lib/persistent_metadata
lib_volatile_metadata := src/lib/volatile_metadata
lib_io_manager := src/lib/io_manager
lib_distribution_strategy := src/lib/distribution_strategy
lib_replication_strategy := src/lib/replication_strategy
lib_barista_core := src/lib/barista_core
lib_monitored_strategy := src/lib/monitored_strategy

libraries := $(lib_network) $(lib_espresso_storage) $(lib_decafs_types)\
             $(lib_access) $(lib_locking) $(lib_persistent_metadata) \
             $(lib_volatile_metadata) $(lib_distribution_strategy) \
             $(lib_replication_strategy) $(lib_io_manager) \
             $(lib_barista_core) $(lib_nettcp) $(lib_monitored_strategy)

decafs_barista := src/app/decafs_barista
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
	cp src/app/decafs_barista/decafs_barista bin
	cp src/app/espresso_core/decafs_espresso bin

clean:
	$(RM) bin/* lib/*
	for DIR in ${dirs}; do make -C $${DIR} clean; done
