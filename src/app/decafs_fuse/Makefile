BIN := decafs_fuse
OBJS := decafs_fuse.o

DECAFS_LIBS := network_core/libnetcore.a \
               net_tcp/libnettcp.a \

EXTRA_CFLAGS := $(shell pkg-config fuse --cflags)
EXTRA_LIBS := $(shell pkg-config fuse --libs)

include ../../app.mk
