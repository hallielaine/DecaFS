lib_network := src/lib/network_core
libraries := $(lib_network)

decafs_barista := src/app/barista_core
decafs_espresso := src/app/espresso_core
apps := $(decafs_barista) $(decafs_espresso)

dirs := $(libraries) $(apps)

.PHONY: all $(apps) $(libraries)
all: $(apps) cp

$(apps) $(libraries):
	$(MAKE) --directory=$@

cp:
	cp src/app/barista_core/decafs_barista bin
	cp src/app/espresso_core/decafs_espresso bin

$(apps): $(libraries)

clean:
	rm -f bin/*
	for DIR in ${dirs}; do \
	  make -C $${DIR} clean; \
	done
