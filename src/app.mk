SHELL := /bin/sh

ifeq ($(shell uname),Darwin)
	CC := clang
	CXX := clang++
else
	CC := gcc
	CXX := g++
endif

CFLAGS := -g -O3 -MMD --coverage #-Wall -Wextra -Werror -Wno-unused-parameter
CXXFLAGS := $(CFLAGS) -std=c++0x -fno-exceptions -fno-rtti
LDFLAGS := --coverage

ifeq ($(CXX),clang++)
	LDFLAGS += -flto
	CXXFLAGS += -flto -Wunreachable-code # gcc is not smart enough for this
endif

.PHONY: default clean

INCLUDES := -I../../lib/
LDLIBS := $(addprefix ../../lib/, $(DECAFS_LIBS))

LINK_RULE = $(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

default: $(LDLIBS) $(DECAFS_BIN)

clean:
	$(RM) *.o $(DECAFS_BIN)

%.o : %.c
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES)

%.o : %.cc
	$(CXX) $(CXXFLAGS) -o $@ -c $< $(INCLUDES)

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $< $(INCLUDES)

%.a:
	make -C $(dir $@)
