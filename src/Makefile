src = .
builddir = build
#CC = g++-7
CC = /usr/bin/clang
BINDIR = ../bin
DATA_DIR = "$(CURDIR)/../lib/"
JS_PROFILES_PATH = "$(CURDIR)/../lib/js/profiles/"
LMDBNAME = bnull

lmdbdir = $(CURDIR)/liblmdb
lmdb = $(lmdbdir)/liblmdb.so
duktapeout = $(builddir)/duktape
duktape = $(duktapeout)/duktape.o
openssl = -lssl -lcrypto

thirdparty = $(duktape) $(lmdb)
FLAGS = -ggdb -g -O2 -Wall -Wno-unused-result \
	-Wno-char-subscripts -Wcomment \
	-Wno-format-zero-length -Wno-format-security\
	-std=c++17 -fpermissive -MMD \
	-Imods/include -DLMDB_DB_DIRECTORY=\"$(DATA_DIR)\" \
	-DLMDB_DB_NAME=\"$(LMDBNAME)\" -D__MUD_BUILD_BNULL__  \
	-DJS_PROFILES_PATH=\"$(JS_PROFILES_PATH)\" \
	-stdlib=libstdc++ 
#-A -da \
#	-Lboost-cpp/build/lib\ -Ipqxx 
#	-Imods/include -Iboost-cpp -DLMDB_DB_DIRECTORY=\"$(DATA_DIR)\" 
LIBS = -lpqxx $(openssl) -lstdc++ -lm 
buildobj = act.informative act.item act.medic \
					 act.movement act.offensive act.other act.social\
					 act.wizard alias ban boards bsd-snprintf castle \
					 class comm config constants db fight globals graph\
					 handler house interpreter limits magic mail mobact\
					 modify objsave olc random shop spec_assign spec_procs\
					 spell_parser spells structs weather mods/ai_state\
					 mods/behaviour_tree_impl mods/acl/config-parser mods/lmdb\
					 mods/chat mods/conf mods/deferred mods/drone mods/immortal\
					 mods/js mods/player mods/prefs mods/pregame\
					 mods/projectile mods/quests mods/scan mods/sql mods/util\
					 mods/weapon mods/classes/medic mods/string mods/loops\
					 mods/hell mods/db mods/testing/lmdb/player-syncing\
					 mods/testing/lmdb/db mods/player-scaffolding sql/port\
					 mods/meta_utils mods/acl_list mods/flags mods/js-player-scaffolding \
					 mods/builder mods/pq mods/crypto utils mods/macro_impl \
					 act.comm mods/extra_desc_data mods/overhead_map mods/npc

objects = $(patsubst %,$(builddir)/%.o,$(buildobj))
default: all
all: $(builddir) $(objects) $(thirdparty) #$(DATA_DIR)
	$(CC) $(FLAGS) -o $(BINDIR)/circle $(objects) $(thirdparty) $(LIBS)
clean:
	make -f $(lmdbdir)/Makefile clean ; \
		rm -f $(addprefix $(builddir)/,*.o *.d  mods/*.o \
		mods/*.d mods/classes/*.o mods/classes/*.d) \
		rm -f $(builddir)/mods/testing/lmdb/*.o \
		rm -f $(builddir)/mods/testing/lmdb/*.d 

$(builddir):
	mkdir -p $(builddir)/mods/classes $(duktapeout) $(builddir)/mods/acl/ $(builddir)/mods/testing/lmdb $(builddir)/sql

$(builddir)/%.o: %.cpp
	$(CC) -c $(FLAGS) -o $@ $<

-include $(objects:.o=.d)

$(duktape): duktape-2.2.0/src/duktape.h duktape-2.2.0/src/duktape.c
	$(CC) -c $(FLAGS) duktape-2.2.0/src/duktape.c  -o $(duktape)

$(lmdb):
	cd $(lmdbdir) && make clean ; make liblmdb.so && cd $(src)

$(DATA_DIR):
	mkdir $(DATA_DIR)
