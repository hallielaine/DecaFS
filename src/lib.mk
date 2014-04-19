include ../../common.mk

$(LIB): $(OBJS)
	ar rcs $@ $^
