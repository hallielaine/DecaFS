SHELL := /bin/sh

ifeq ($(shell uname),Darwin)
	CC := clang
	CXX := clang++
else
	CC := gcc
	CXX := g++
endif

#TODO: make code compile with -Werror
CFLAGS := -g -O3 -MMD -Wall -Wno-unused-parameter $(EXTRA_CFLAGS)
LDFLAGS := -pthread

ifeq ($(COVERAGE),yes)
	CFLAGS += --coverage
	LDFLAGS += --coverage
endif

CXXFLAGS := $(CFLAGS) -std=c++0x -fno-exceptions -fno-rtti

ifeq ($(CXX),clang++)
	LDFLAGS += -flto
	CXXFLAGS += -flto -Wunreachable-code
endif

INCLUDES := -I../../lib/ -I../../../test/gmock/include \
            -I../../../test/gtest/include

OUTFILE := $(BIN)$(LIB)

.PHONY: default clean

default: $(OUTFILE)

clean:
	$(RM) *.o *.gcno *.gcda $(OUTFILE)

-include *.d

%.o : %.c
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES)

%.o : %.cc
	$(CXX) $(CXXFLAGS) -o $@ -c $< $(INCLUDES)

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $< $(INCLUDES)
