src = .
builddir = build
CC = g++-7
BINDIR = ../bin

lmdbout = mods/lib
lmdb = $(lmdbout)/lmdb.so
duktapeout = $(builddir)/duktape
duktape = $(duktapeout)/duktape.o

thirdparty = $(duktape) $(lmdb)
FLAGS = -g -O2 -Wall -Wno-unused-result \
	-Wno-char-subscripts    -Wcomment \
	-Wno-format-zero-length -Wno-format-security\
	-A -da -std=c++17       -fpermissive -MMD\
	-L$(CURDIR)/$(lmdbout)  -Lboost-cpp/build/lib\
	-Imods/include -Iboost-cpp\
LIBS = -lcrypt -lpqxx -lpq -llmdb
buildobj = act.comm act.informative act.item act.medic \
					 act.movement act.offensive act.other act.social\
					 act.wizard alias ban boards bsd-snprintf castle \
					 class comm config constants db fight globals graph\
					 handler house interpreter limits magic mail mobact\
					 modify objsave olc random shop spec_assign spec_procs\
					 spell_parser spells utils weather mods/ai_state\
					 mods/behaviour_tree_impl mods/acl/config-parser mods/builder \
					 mods/chat mods/conf mods/deferred mods/drone mods/immortal\
					 mods/js mods/player mods/pq mods/prefs mods/pregame\
					 mods/projectile mods/quests mods/scan mods/sql mods/util\
					 mods/weapon mods/classes/medic
objects = $(patsubst %,$(builddir)/%.o,$(buildobj))
default: all
all: $(builddir) $(objects) $(thirdparty)
	$(CC) $(FLAGS) -o $(BINDIR)/circle $(objects) $(thirdparty) $(LIBS)
clean:
	rm -f $(addprefix $(builddir)/,*.o *.d mods/*.o \
		mods/*.d mods/classes/*.o mods/classes/*.d)

$(builddir):
	mkdir -p $(builddir)/mods/classes $(duktapeout) $(lmdbout)

$(builddir)/%.o: %.cpp
	$(CC) -c $(FLAGS) -o $@ $<

-include $(buildobj:=.d)

$(duktape): duktape-2.2.0/src/duktape.h duktape-2.2.0/src/duktape.c
	$(CC) -c $(FLAGS) duktape-2.2.0/src/duktape.c -o $(duktape)

$(lmdb):
	make mods/lmdb/liblmdb/liblmdb.so ; cp mods/lmdb/liblmdb/liblmdb.so $(lmdb)
