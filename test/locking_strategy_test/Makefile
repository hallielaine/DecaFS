GTEST_DIR := ../gtest
USER_DIR := ../../src/lib/locking_strategy

CXX := g++
CXXFLAGS := -std=c++11 -g -O3 -pthread -Wall -Wextra -Weffc++ \
            -isystem $(GTEST_DIR)/include

TESTS := locking_strategy_unittest

GTEST_HEADERS := $(GTEST_DIR)/include/gtest/*.h \
                 $(GTEST_DIR)/include/gtest/internal/*.h

.PHONY: all clean fullclean

all : $(TESTS)

GTEST_OBJS := gtest-all.o gtest_main.o
USER_OBJS := locking_strategy_unittest.o

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


locking_strategy_unittest.o : locking_strategy_unittest.cc \
                              $(USER_DIR)/*.h \
                              $(GTEST_HEADERS)
	$(CXX) $(CXXFLAGS) -I$(USER_DIR) -c $< -o $@

$(USER_DIR)/liblocking_strategy.a : $(USER_DIR)/*.cc $(USER_DIR)/*.h \
                                    $(USER_DIR)/Makefile
	make -C $(USER_DIR) liblocking_strategy.a

locking_strategy_unittest : locking_strategy_unittest.o gtest_main.a \
                            $(USER_DIR)/liblocking_strategy.a
	$(CXX) $(CXXFLAGS) --coverage $^ -o $@

