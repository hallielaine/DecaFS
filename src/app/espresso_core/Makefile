CXX := g++
CXXFLAGS := -std=c++11 -g -O3 -Wall -Wextra -Weffc++ -Werror --coverage

.PHONY: all clean

BINS := espresso_storage.a

OBJS := espresso_data.o \
        espresso_state.o \
        espresso_storage.o

all : $(BINS)

espresso_storage.a: $(OBJS)
	ar rcs $@ $^

clean:
	rm -f $(BINS) $(OBJS)

espresso_data.o: espresso_data.cc espresso_data.h espresso_types.h \
  espresso_state.h
espresso_state.o: espresso_state.cc espresso_state.h espresso_types.h
espresso_storage.o: espresso_storage.cc espresso_storage.h \
  espresso_types.h espresso_state.h espresso_data.h
