CXX := g++
CXXFLAGS := -g -O3 \
            -Wall -Wextra -Weffc++ -Werror

.PHONY: all clean

BINS := libip_address.a

OBJS := ip_address.o 

all : $(BINS)

libip_address.a: $(OBJS)
	ar rcs $@ $^

clean:
	rm -f $(BINS) $(OBJS) *.gcno *.gcda *.gcov

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ -c $<

ip_address.o: ip_address.cpp ip_address.h
