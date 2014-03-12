CXX := g++
CXXFLAGS := -std=c++11 -g -O3 --coverage \
            -Wall -Wextra -Weffc++ -Werror

.PHONY: all clean

BINS := decafs_espresso espresso_storage.a

OBJS := espresso_core.o \
        espresso_data.o \
        espresso_state.o \
        espresso_storage.o

DECAFS_LIBDIR := ../../lib

INCLUDE_PATHS := $(DECAFS_LIBDIR) \
                 ../../src

INCLUDES := $(addprefix -I, $(INCLUDE_PATHS))

LIBRARIES := #$(DECAFS_LIBDIR)/network_core/libnetcore.a

all : $(BINS)

decafs_espresso: espresso_core.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBRARIES)

espresso_storage.a: espresso_storage.o espresso_data.o espresso_state.o
	ar rcs $@ $^

clean:
	rm -f $(BINS) $(OBJS) *.gcno *.gcda *.gcov

%.o: %.cc
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ -c $<

espresso_core.o: espresso_core.cc espresso_core.h \
  ../../lib/network_core/network.h
espresso_data.o: espresso_data.cc espresso_data.h espresso_types.h \
  espresso_state.h
espresso_state.o: espresso_state.cc espresso_state.h espresso_types.h
espresso_storage.o: espresso_storage.cc espresso_storage.h \
  espresso_types.h espresso_state.h espresso_data.h
