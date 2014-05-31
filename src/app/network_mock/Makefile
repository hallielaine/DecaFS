BIN := nettest
OBJS := main.o network_fake.o

DECAFS_LIBS := decafs_types/libdecafs_types.a \
               barista_core/libbarista_core.a \
               locking_strategy/liblocking_strategy.a \
               volatile_metadata/libvolatile_metadata.a \
               persistent_metadata/libpersistent_metadata.a \
               distribution_strategy/libdistribution_strategy.a \
               replication_strategy/libreplication_strategy.a \
               monitored_strategy/libmonitored?strategy.a \
               io_manager/libio_manager.a \
               access/libaccess.a \
               network_core/libnetcore.a \
               #net_tcp/libnettcp.a \
               #espresso_storage/libespresso_storage.a

include ../../app.mk
