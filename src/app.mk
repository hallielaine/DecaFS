include ../../common.mk

LDLIBS := $(addprefix ../../lib/, $(DECAFS_LIBS)) $(EXTRA_LIBS)

ifeq ($(CXX),g++)
	LDLIBS_GROUP := -Wl,--start-group $(LDLIBS) -Wl,--end-group
else
   LDLIBS_GROUP := $(LDLIBS)
endif

$(BIN): $(LDLIBS) $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS_GROUP)

%.a:
	make -C $(dir $@)
