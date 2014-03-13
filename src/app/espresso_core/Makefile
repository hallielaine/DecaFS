CXX := g++
CXXFLAGS := -std=c++11 -g -O3 -Wall -Wextra -Weffc++

.PHONY: all clean

BINS := decafs_espresso

OBJS := espresso_core.o

DECAFS_LIBDIR := ../../lib

INCLUDE_PATHS := $(DECAFS_LIBDIR)

INCLUDES := $(addprefix -I, $(INCLUDE_PATHS))

LIBRARIES := $(DECAFS_LIBDIR)/espresso_storage/libespresso_storage.a \
             $(DECAFS_LIBDIR)/network_core/libnetcore.a

all : $(BINS)

decafs_espresso: espresso_core.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBRARIES)

clean:
	rm -f $(BINS) $(OBJS)

%.o: %.cc
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ -c $<

espresso_core.o: espresso_core.cc espresso_core.h \
  ../../lib/network_core/network.h \
  ../../lib/espresso_storage/espresso_storage.h
