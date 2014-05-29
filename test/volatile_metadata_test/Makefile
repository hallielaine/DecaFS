GTEST_DIR := ../gtest
GMOCK_DIR := ../gmock
USER_DIR := ../../src/lib/volatile_metadata

CXX := g++ -std=c++11
CXXFLAGS := -g -O3 -pthread -Wall -Wextra -Weffc++ \
            -isystem $(GTEST_DIR)/include -isystem $(GMOCK_DIR)/include

TESTS := volatile_metadata_unittest

GTEST_HEADERS := $(GTEST_DIR)/include/gtest/*.h \
                 $(GTEST_DIR)/include/gtest/internal/*.h \

GMOCK_HEADERS := $(GMOCK_DIR)/include/gmock/*.h \
                 $(GMOCK_DIR)/include/gmock/internal/*.h

.PHONY: all clean fullclean

all : $(TESTS)

GTEST_OBJS := gtest-all.o
GMOCK_OBJS := gmock-all.o gmock_main.o
USER_OBJS := volatile_metadata_unittest.o

clean :
	rm -f $(TESTS) $(USER_OBJS)

fullclean : clean
	rm -f $(GTEST_OBJS) $(GMOCK_OBJS) gmock_main.a


gmock_main.a : $(GTEST_OBJS) $(GMOCK_OBJS)
	ar rcs $@ $^

GTEST_SRCS := $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h \
              $(GTEST_HEADERS)

GMOCK_SRCS := $(GMOCK_DIR)/src/*.cc \
              $(GMOCK_HEADERS)

gtest-all.o : $(GTEST_SRCS)
	$(CXX) $(CXXFLAGS) -I$(GTEST_DIR) -c $(GTEST_DIR)/src/gtest-all.cc -o $@

gmock-all.o : $(GMOCK_SRCS)
	$(CXX) $(CXXFLAGS) -I$(GMOCK_DIR) -c $(GMOCK_DIR)/src/gmock-all.cc -o $@

gmock_main.o : $(GMOCK_SRCS)
	$(CXX) $(CXXFLAGS) -I$(GMOCK_DIR) -c $(GMOCK_DIR)/src/gmock_main.cc -o $@

volatile_metadata_unittest.o : volatile_metadata_unittest.cpp \
                              $(USER_DIR)/*.h \
                              $(GTEST_HEADERS) $(GMOCK_HEADERS)
	$(CXX) $(CXXFLAGS) -I$(USER_DIR)/.. -c $< -o $@

$(USER_DIR)/libvolatile_metadata.a : $(USER_DIR)/*.cpp $(USER_DIR)/*.h \
                                 $(USER_DIR)/Makefile
	make -C $(USER_DIR) libvolatile_metadata.a

volatile_metadata_unittest : $(USER_DIR)/libvolatile_metadata.a \
                            volatile_metadata_unittest.o gmock_main.a \
                             $(USER_DIR)/../*/*.a
	$(CXX) $(CXXFLAGS) -Wl,--start-group $^ -Wl,--end-group -o $@

