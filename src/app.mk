include ../../common.mk

LDLIBS := $(addprefix ../../lib/, $(DECAFS_LIBS))

$(BIN): $(LDLIBS) $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

%.a:
	make -C $(dir $@)
