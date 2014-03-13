CXX := g++
CXXFLAGS := -std=c++11 -g -O3 --coverage \
            -Wall -Wextra -Weffc++ -Werror -Wno-unused-parameter

.PHONY: all clean

BINS := liblocking_strategy.a

OBJS := locking_strategy.o

all : $(BINS)

liblocking_strategy.a: $(OBJS)
	ar rcs $@ $^

clean:
	rm -f $(BINS) $(OBJS) *.gcno *.gcda *.gcov

%.o: %.cc
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ -c $<

locking_strategy.o: locking_strategy.cc locking_strategy.h
