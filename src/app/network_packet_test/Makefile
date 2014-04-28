BIN := nettest
OBJS := main.o

DECAFS_LIBS := network_core/libnetcore.a \
               net_tcp/libnettcp.a \
               decafs_types/libdecafs_types.a \
               #access/libaccess.a \
               locking_strategy/liblocking_strategy.a \
               volatile_metadata/libvolatile_metadata.a \
               persistent_metadata/libpersistent_metadata.a \
               distribution_strategy/libdistribution_strategy.a \
               replication_strategy/libreplication_strategy.a \
               io_manager/libio_manager.a \
               #barista_core/libbarista_core.a \
               #espresso_storage/libespresso_storage.a

include ../../app.mk
