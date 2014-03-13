CXX := g++
CXXFLAGS := -std=c++11 -g -O3 --coverage \
            -Wall -Wextra -Weffc++ -Werror

.PHONY: all clean

BINS := libespresso_storage.a

OBJS := espresso_data.o \
        espresso_state.o \
        espresso_storage.o

all : $(BINS)

libespresso_storage.a: espresso_storage.o espresso_data.o espresso_state.o
	ar rcs $@ $^

clean:
	rm -f $(BINS) $(OBJS) *.gcno *.gcda *.gcov

%.o: %.cc
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ -c $<

espresso_data.o: espresso_data.cc espresso_data.h espresso_types.h \
  espresso_state.h
espresso_state.o: espresso_state.cc espresso_state.h espresso_types.h
espresso_storage.o: espresso_storage.cc espresso_storage.h \
  espresso_types.h espresso_state.h espresso_data.h
