BIN := decafs_barista
OBJS := barista_core.o volatile_metadata.o persistent_metadata.o \
        io_manager.o distribution_strategy.o replication_strategy.o

DECAFS_LIBS := network_core/libnetcore.a \
               ip_address/libip_address.a \
               access/libaccess.a \
               locking_strategy/liblocking_strategy.a

include ../../app.mk
