src = .
builddir = build
#CC = /usr/bin/clang++-8
CC = /usr/bin/g++-8
BINDIR = ../bin
DATA_DIR = "$(CURDIR)/../lib/"
JS_PROFILES_PATH = "$(HOME)/code/code/siege-mud/lib/js/profiles/"
LMDBNAME = bnull
LIB_DIR="$(HOME)/code/code/siege-mud/lib"
POST_BUILD_SCRIPT="$(CURDIR)/scripts/post-build.sh"

lmdbdir = $(CURDIR)/liblmdb
lmdb = liblmdb/liblmdb.so
duktapeout = $(builddir)/duktape
duktape = duktape.o
openssl = -lssl -lcrypto
yamlcpp = $(CURDIR)/yaml-cpp/include
yamllib = $(CURDIR)/yaml-cpp/build
yamlcpp_object = yaml-cpp
boostdir = $(CURDIR)/boost
thirdparty = $(duktape) $(lmdb)
FLAGS = -ggdb -g -O2 -Wall -Wno-unused-result \
	-Wno-char-subscripts -Wcomment \
	-Wno-format-zero-length -Wno-format-security\
	-std=c++2a -fconcepts -fpermissive -MMD \
	-I$(boostdir) -Imods/include -DLMDB_DB_DIRECTORY=\"$(DATA_DIR)\" \
	-DLMDB_DB_NAME=\"$(LMDBNAME)\" -D__MUD_BUILD_BNULL__  \
	-DJS_PROFILES_PATH=\"$(JS_PROFILES_PATH)\"  \
	-DMENTOC_NO_CONSTEXPR_CONF=1 \
	-D__MENTOC_USE_LMDB=1 \
	-Wno-sign-compare -I$(yamlcpp) -L$(yamllib) \
	-fPIC -DMENTOC_CURRENT_WORKING_DIR=\"$(LIB_DIR)\" \
	-D__MENTOC_SHOW_SNIPE_HIT_STATS__=1 \
	-DMENTOC_LIB_DIR=\"$(CURDIR)/../lib/\" \
	-D__MENTOC_MUTE_BEHAVIOUR_TREE_OUTPUT__=1
	#-D__MENTOC_SHOW_DISPOSE_PLAYER_DEBUG_OUTPUT__=1 \
	-D__MENTOC_SHOW_LMDB_DEBUG__=1 \
	#-DMENTOC_NO_CONSTEXPR_CONF=1 \
	#-D__MENTOC_USE_PQXX_RESULT__=1 
LIBS =  -lc -lm -lpqxx -lpq $(openssl) -l$(yamlcpp_object)
buildobj = act.informative act.item act.medic \
					 act.movement act.offensive act.other act.social\
					 act.wizard act.demolitions alias ban boards bsd-snprintf castle \
					 class comm config constants db fight globals graph\
					 handler house interpreter limits magic mail mobact\
					 modify objsave olc random spec_assign spec_procs\
					 spell_parser spells structs weather mods/ai_state\
					 mods/behaviour_tree_impl mods/lmdb\
					 mods/chat mods/conf mods/deferred mods/drone mods/immortal\
					 mods/js mods/player mods/prefs mods/pregame\
					 mods/projectile mods/quests mods/scan mods/sql mods/util\
					 mods/weapon mods/string mods/loops\
					 mods/hell mods/db mods/testing/lmdb/player-syncing\
					 mods/testing/lmdb/db mods/player-scaffolding sql/port\
					 mods/meta_utils mods/acl_list mods/flags mods/js-player-scaffolding \
					 mods/builder mods/pq mods/crypto utils mods/macro_impl \
					 act.comm mods/extra_desc_data mods/overhead_map mods/npc \
					 mods/randomized-items mods/auto-login mods/date-time \
					 mods/world-configuration mods/debug act.debug mods/pfind \
					 mods/orm/chargen mods/orm/inventory \
					 mods/orm/room \
					 mods/weapons/pistol-czp10 \
					 mods/weapons/smg-mp5 mods/weapons/shotgun-sasg12 \
					 mods/orm/class-sniper mods/classes/sniper \
					 mods/orm/class-marine mods/classes/marine \
					 mods/orm/class-sentinel mods/classes/sentinel \
					 mods/orm/class-contagion mods/classes/contagion \
					 mods/orm/class-engineer mods/classes/engineer \
					 mods/orm/class-medic mods/classes/medic \
					 mods/orm/class-psyop mods/classes/psyop \
					 mods/orm/class-support mods/classes/support \
					 mods/classes/base \
					 mods/weapons/sniper-rifle-psg1 mods/weapons/sniper-rifle-l96aw \
					 mods/orm/quotes act.builder mods/orm/base mods/yaml \
					 mods/affects mods/item mods/weapon-types mods/sensor-grenade \
					 mods/calc_visibility mods/events mods/chargen \
					 mods/object-utils mods/camera mods/orm/shop shop mods/shop mods/rand \
					 mods/help mods/rooms mods/weapons/damage-types mods/injure mods/skills \
					 mods/mobs/mini-gunner mods/mobs/extended-types mods/values \
					 mods/mobs/room-watching mods/mobs/mini-gunner-behaviour-tree \
					 mods/armor/basic-protection mods/armor/advanced-protection mods/armor/elite-protection \
					 mods/players/db-load mods/bugs-fixtures mods/player-registration \
					 mods/super-users mods/builder/object-placement mods/integral-objects \
					 mods/orm/integral-object mods/orm/camera-feed mods/orm/door-event \
					 mods/mini-games mods/orm/mini-game

objects = $(patsubst %,$(builddir)/%.o,$(buildobj))
default: all
all: $(builddir) $(builddir)/mods/classes $(duktapeout) $(builddir)/mods/testing/lmdb $(builddir)/sql $(builddir)/mods/orm $(builddir)/mods/weapons $(builddir)/mods/mobs $(builddir)/mods/builder $(builddir)/mods/armor $(builddir)/mods/builder $(builddir)/mods/armor $(builddir)/mods/players $(objects) $(thirdparty) #$(DATA_DIR)
	$(CC) $(FLAGS) -o $(BINDIR)/circle $(objects) $(thirdparty) $(LIBS) && $(POST_BUILD_SCRIPT)

clean:
	make -f $(lmdbdir)/Makefile clean ; \
		rm -f $(addprefix $(builddir)/,*.o *.d  mods/*.o \
		mods/*.d mods/orm/*.o mods/orm/*.d \
		mods/classes/*.o mods/classes/*.d mods/mobs/*.o mods/mobs/*.d \
		mods/armor/*.o mods/armor/*.d mods/players/*.o mods/players/*.d) \
		rm -f $(builddir)/mods/testing/lmdb/*.o \
		rm -f $(builddir)/mods/testing/lmdb/*.d 

$(builddir)/mods/classes:
	mkdir -p $(builddir)/mods/classes
$(duktapeout): 
	mkdir -p $(duktapeout)
$(builddir)/mods/testing/lmdb:
	mkdir -p $(builddir)/mods/testing/lmdb
$(builddir)/sql:
	mkdir -p $(builddir)/sql
$(builddir)/mods/orm:
	mkdir -p $(builddir)/mods/orm
$(builddir)/mods/weapons:
	mkdir -p $(builddir)/mods/weapons
$(builddir)/mods/mobs:
	mkdir -p $(builddir)/mods/mobs
$(builddir)/mods/builder:
	mkdir -p $(builddir)/mods/builder
$(builddir)/mods/armor:
	mkdir -p $(builddir)/mods/armor
$(builddir)/mods/players:
	mkdir -p $(builddir)/mods/players

$(builddir)/%.o: %.cpp
	$(CC) -c $(FLAGS) -o $@ $<

-include $(objects:.o=.d)

$(duktape): duktape-2.2.0/src/duktape.h duktape-2.2.0/src/duktape.c
	$(CC) -c $(FLAGS) duktape-2.2.0/src/duktape.c  -o $(duktape)

$(lmdb):
	cd $(lmdbdir) && make clean ; make liblmdb.so && cp liblmdb.so $(src) && cd $(src) 

$(DATA_DIR):
	mkdir $(DATA_DIR)
