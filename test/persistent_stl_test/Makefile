GTEST_DIR := ../gtest
USER_DIR := ../../src/lib/persistent_stl

CXX := g++
CXXFLAGS := -g -O3 -std=c++11 -pthread -Wall -Wextra -Weffc++ \
            -isystem $(GTEST_DIR)/include

TESTS := persistent_map_unittest

GTEST_HEADERS := $(GTEST_DIR)/include/gtest/*.h \
                 $(GTEST_DIR)/include/gtest/internal/*.h

.PHONY: all clean fullclean

all : $(TESTS)

GTEST_OBJS := gtest-all.o gtest_main.o
USER_OBJS := persistent_map_unittest.o

clean :
	rm -f $(TESTS) $(USER_OBJS)

fullclean : clean
	rm -f $(GTEST_OBJS) gtest_main.a


gtest_main.a : $(GTEST_OBJS)
	ar rcs $@ $^

GTEST_SRCS := $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

gtest-all.o : $(GTEST_SRCS)
	$(CXX) $(CXXFLAGS) -I$(GTEST_DIR) -c $(GTEST_DIR)/src/gtest-all.cc -o $@

gtest_main.o : $(GTEST_SRCS)
	$(CXX) $(CXXFLAGS) -I$(GTEST_DIR) -c $(GTEST_DIR)/src/gtest_main.cc -o $@


persistent_map_unittest.o : persistent_map_unittest.cc \
                              $(USER_DIR)/persistent_map.h \
                              $(GTEST_HEADERS)
	$(CXX) $(CXXFLAGS) -I$(USER_DIR) -c $< -o $@

persistent_map_unittest : persistent_map_unittest.o gtest_main.a
	$(CXX) $(CXXFLAGS)  $^ -o $@

