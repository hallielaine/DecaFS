GTEST_DIR := ../gtest
USER_DIR := ../../src/lib/espresso_storage

CXX := g++
CXXFLAGS := -g -O3 -pthread -Wall -Wextra -Weffc++ \
            -isystem $(GTEST_DIR)/include

TESTS := espresso_storage_unittest

GTEST_HEADERS := $(GTEST_DIR)/include/gtest/*.h \
                 $(GTEST_DIR)/include/gtest/internal/*.h

.PHONY: all clean fullclean

all : $(TESTS)

GTEST_OBJS := gtest-all.o gtest_main.o
USER_OBJS := espresso_storage_unittest.o

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


espresso_storage_unittest.o : espresso_storage_unittest.cc \
                              $(USER_DIR)/*.h \
                              $(GTEST_HEADERS)
	$(CXX) $(CXXFLAGS) -I$(USER_DIR) -c $< -o $@

$(USER_DIR)/libespresso_storage.a : $(USER_DIR)/*.cc $(USER_DIR)/*.h \
                                 $(USER_DIR)/Makefile
	make -C $(USER_DIR) libespresso_storage.a

espresso_storage_unittest : $(USER_DIR)/libespresso_storage.a \
                            espresso_storage_unittest.o gtest_main.a
	$(CXX) $(CXXFLAGS) --coverage $^ -o $@

