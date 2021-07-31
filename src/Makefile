src = .
builddir = build
#CC = /usr/bin/clang++-8
CC = /usr/bin/g++-8
BINDIR = ../bin
DATA_DIR = "$(CURDIR)/../lib/"
JS_PROFILES_PATH = "$(DATA_DIR)js/profiles/"
LMDBNAME = bnull
LIB_DIR="$(DATA_DIR)"
POST_BUILD_SCRIPT="$(CURDIR)/scripts/post-build.sh"
SQL_MIGRATIONS_DIR="$(CURDIR)/sql/migrations"

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
#gdbflags = -ggdb -g
post_build_args = strip-all
FLAGS = $(gdbflags) -O2 -Wall -Wno-unused-result \
	-Wuninitialized \
	-Wno-char-subscripts -Wcomment \
	-Wno-format-zero-length -Wno-format-security\
	-std=c++2a -fconcepts -fpermissive -MMD \
	-I$(boostdir) -Imods/include -DLMDB_DB_DIRECTORY=\"$(DATA_DIR)\" \
	-Imods/catch2/Catch2/src -Lmods/catch2/Catch2/build/src \
	-DLMDB_DB_NAME=\"$(LMDBNAME)\" -D__MUD_BUILD_BNULL__  \
	-DJS_PROFILES_PATH=\"$(JS_PROFILES_PATH)\"  \
	-DMENTOC_NO_CONSTEXPR_CONF=1 \
	-D__MENTOC_USE_LMDB=1 \
	-Wno-sign-compare -I$(yamlcpp) -L$(yamllib) \
	-fPIC -DMENTOC_CURRENT_WORKING_DIR=\"$(LIB_DIR)\" \
	-D__MENTOC_SHOW_SNIPE_HIT_STATS__=1 \
	-DMENTOC_LIB_DIR=\"$(CURDIR)/../lib/\" \
	-D__MENTOC_MUTE_BEHAVIOUR_TREE_OUTPUT__=1 \
	-D__MENTOC_SQL_MIGRATIONS_DIR=\"$(SQL_MIGRATIONS_DIR)\" \
	-D__MENTOC_PREDEFINED_RIFLE_PKID__=1 \
	-D__MENTOC_SHOW_BEHAVIOUR_TREE_LOWLY_SECURITY_BTREE_DEBUG_OUTPUT__=1 \
	-D__MENTOC_SHOW_CONTRACT_OUTPUT__=1 \
	-D__MENTOC_SHOW_GAIN_EXP_DEBUG_OUTPUT__=1 \
	-D__MENTOC_DISABLE_INJURE_DYNAMICS__=1 \
	-D__MENTOC_SHOW_IS_CORPSE_DEBUG_OUTPUT__=1 \
	-D__MENTOC_DEBUG_EXTRACT_OBJ_OUTPUT__=1

 	#-D__MENTOC_MODS_MOBS_chaotic_meth_addict_SHOW_DEBUG_OUTPUT__=1
	#-D__MENTOC_CAN_CONTINUE_DEBUG__=1
	#-D__MENTOC_SHOW_DAMAGE_TYPES_DEBUG__=1
	#-D__MENTOC_MODS_MOBS_car_thief_SHOW_DEBUG_OUTPUT__=1
	#-D__MENTOC_SHOW_MELEE_HIT_STATS__=1
	#-D__MENTOC_SHOW_COOLDOWN_DEBUG__=1
	#-D__MENTOC_SHOW_MELEE_HIT_STATS__=1
	#-D__MENTOC_PLAYER_SKILL_USAGE_DEBUG_OUTPUT__=1
	#-D__MENTOC_ORM_BASE_DEBUG_OUTPUT__
	#-D__MENTOC_SHOW_MODS_PLAYERS_DB_LOAD_DEBUG_OUTPUT__=1
	#-D__MENTOC_ORM_UTIL_DEBUG_OUTPUT__=1
	#-D__MENTOC_ENCODED_MESSAGE_DEBUG_OUTPUT__=1
	#-D__MENTOC_MODS_ZONE_DEBUG__=1
	#-D__MENTOC_OBJ_DATA_DEBUG__=1
	#-D__MENTOC_MODS_INTEGRAL_OBJECTS_DEBUG__=1
	#-D__MENTOC_DEBUG_EVENT_MATCHES__=1
	#-D__MENTOC_RUN_WRAPPER_PREGAME__=1
	#-D__MENTOC_SHOW_ORM_INVENTORY_FLUSH_OUTPUT__=1
#-D__MENTOC_MODS_AFFECTS_SHOW_DEBUG_OUTPUT__=1 \
#-D__MENTOC_SHOW_MODS_CALC_VISIBILITY_DEBUG_OUTPUT__=1 \
#-D__MENTOC_SHOW_CALC_VISIBILITY_DUMP_OUTPUT__=1 \
	#-D__MENTOC_SHOW_YAML_FIELD_BEING_FED__=1 \
	#-D__MENTOC_SHOW_LMDB_DEBUG__=1
	#-D__MENTOC_SHOW_DISPOSE_PLAYER_DEBUG_OUTPUT__=1 \
	-D__MENTOC_SHOW_LMDB_DEBUG__=1 \
	#-DMENTOC_NO_CONSTEXPR_CONF=1 \
	#-D__MENTOC_USE_PQXX_RESULT__=1 
LIBS =  -lc -lm -lpqxx -lpq $(openssl) -l$(yamlcpp_object) -lCatch2
buildobj = act.informative act.item act.medic \
					 act.movement act.offensive act.other act.social\
					 act.wizard act.demolitions alias ban boards bsd-snprintf castle \
					 class comm config constants db fight globals graph\
					 handler house interpreter limits magic mail mobact\
					 modify objsave olc random spec_assign spec_procs\
					 spell_parser spells mods/builder-data structs weather mods/ai_state\
					 mods/behaviour_tree_impl mods/lmdb\
					 mods/chat mods/conf mods/deferred mods/drone mods/immortal\
					 mods/js mods/player mods/prefs mods/pregame\
					 mods/projectile mods/contracts mods/scan mods/sql mods/util\
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
					 mods/orm/sniper mods/classes/sniper \
					 mods/orm/marine mods/classes/marine \
					 mods/orm/breacher mods/classes/breacher \
					 mods/orm/engineer mods/classes/engineer \
					 mods/orm/ghost mods/classes/ghost \
					 mods/orm/medic mods/classes/medic \
					 mods/orm/support mods/classes/support \
					 mods/classes/base mods/classes/super-user-fiddler \
					 mods/weapons/sniper-rifle-psg1 mods/weapons/sniper-rifle-l96aw \
					 mods/orm/quotes act.builder mods/orm/base mods/yaml \
					 mods/affects mods/item mods/weapon-types mods/sensor-grenade \
					 mods/calc-visibility mods/events mods/chargen \
					 mods/object-utils mods/camera mods/orm/shop shop mods/shop mods/rand \
					 mods/weapons/damage-types mods/weapons/elemental mods/weapons/heat \
					 mods/injure mods/skills  mods/help mods/rooms \
					 mods/mobs/mini-gunner mods/mobs/extended-types mods/values \
					 mods/mobs/room-watching mods/mobs/mini-gunner-behaviour-tree \
					 mods/mobs/smart-mob \
					 mods/mobs/lowly-security mods/mobs/lowly-security-behaviour-tree \
					 mods/mobs/mp-shotgunner mods/mobs/mp-shotgunner-behaviour-tree \
					 mods/armor/basic-protection mods/armor/advanced-protection mods/armor/elite-protection \
					 mods/players/db-load mods/bugs-fixtures mods/player-registration \
					 mods/super-users mods/builder/object-placement \
					 mods/orm/mob-equipment mods/builder/meqbuild \
					 mods/orm/mob-equipment-map \
					 mods/orm/hq mods/builder/hqbuild \
					 mods/integral-objects mods/integral-objects-db \
					 mods/orm/integral-object mods/orm/camera-feed mods/orm/door-event \
					 mods/mini-games/line-up mods/orm/mini-game mods/mini-games \
					 mods/mini-games/wires mods/migrations mods/interpreter \
					 mods/radio mods/zone mods/query-objects mods/deep-object-parser \
					 mods/rifle-attachments mods/examine \
					 mods/target-practice mods/target-practice-db mods/demolitions \
					 mods/replenish mods/players/destroy-player mods/rate-limiting \
					 mods/levels mods/orm/player-base-ability \
					 mods/class-abilities \
					 mods/orm/elevator mods/elevator \
					 mods/orm/mob-roam mods/mob-roam mods/unit-tests/mob-roam \
					 mods/forge-engine/generator mods/forge-engine/value-scaler \
					 mods/loot mods/forge-engine/generated-rifle mods/orm/rifle-index \
					 mods/forge-engine/generated-armor mods/orm/armor-index \
					 mods/forge-engine/util mods/stat-bonuses \
					 mods/orm/skill-points mods/unit-tests/skill-points \
					 mods/orm/player-skill-points mods/unit-tests/damage-types \
					 mods/unit-tests/elemental mods/unit-tests/elemental-armor-resistances \
					 mods/unit-tests/armor-stats mods/unit-tests/visibility \
					 mods/skill-orm-adaptor mods/orm/skill-trees \
					 mods/orm/rifle-instance mods/unit-tests/rifle-instance \
					 mods/orm/player-skill-usage \
					 mods/fluxkraft/generator mods/fluxkraft/shape-dictionary \
					 mods/fluxkraft/arcon-metropolitan mods/fluxkraft/arcon-bar \
					 mods/response-team/radio mods/response-team/hq-locations \
					 mods/mob-equipment mods/builder/encode mods/orm/contract-steps \
					 mods/orm/contracts mods/orm/player-contract-state \
					 mods/unit-tests/contracts mods/unit-tests/player-contract-instance \
					 mods/contract-events mods/player-contract-instance \
					 mods/players/messages mods/builder/conbuild \
					 mods/contract-steps mods/orm/rifle-attachment mods/unit-tests/rifle-attachments \
					 mods/builder/bookmarks \
					 mods/movement mods/weapons/damage-calculator mods/weapons/reload \
					 mods/scripted-sequences mods/scripted-step mods/builder/seqbuild \
					 mods/orm/scripted-step mods/contract-step-callback \
					 mods/scripted-sequence-events mods/scripted-sequence-runner \
					 mods/doors mods/runners/movement mods/runners/items \
					 mods/runners/communication mods/mobs/damage-event \
					 mods/weapons/armor-calculator \
					 mods/mobs/car-thief mods/mobs/car-thief-behaviour-tree \
					 mods/players/banish mods/medic \
					 mods/mobs/generic-thief mods/mobs/generic-thief-behaviour-tree \
					 mods/mobs/chaotic-meth-addict mods/mobs/chaotic-meth-addict-behaviour-tree \
					 mods/weapons/attachment-shotgun-underbarrel \
					 mods/memory mods/weapons/attachment-frag-underbarrel mods/unit-tests/frag-underbarrel \
					 mods/boosters/adrenaline-shot mods/unit-tests/class-sniper

objects = $(patsubst %,$(builddir)/%.o,$(buildobj))
default: all
all: $(builddir) $(builddir)/mods/classes $(duktapeout) $(builddir)/mods/testing/lmdb $(builddir)/sql $(builddir)/mods/orm $(builddir)/mods/weapons $(builddir)/mods/mobs $(builddir)/mods/builder $(builddir)/mods/armor $(builddir)/mods/builder $(builddir)/mods/armor $(builddir)/mods/players $(builddir)/mods/mini-games $(builddir)/mods/forge-engine $(objects) $(thirdparty) $(builddir)/mods/unit-tests $(builddir)/mods/fluxkraft $(builddir)/mods/response-team $(builddir)/mods/boosters
	$(CC) $(FLAGS) -o $(BINDIR)/circle $(objects) $(thirdparty) $(LIBS) && $(POST_BUILD_SCRIPT) $(post_build_args)

clean:
	make -f $(lmdbdir)/Makefile clean ; \
		rm -f $(addprefix $(builddir)/,*.o *.d  mods/*.o \
		mods/*.d mods/orm/*.o mods/orm/*.d \
		mods/classes/*.o mods/classes/*.d mods/mobs/*.o mods/mobs/*.d \
		mods/armor/*.o mods/armor/*.d mods/players/*.o mods/players/*.d) \
		rm -f $(builddir)/mods/testing/lmdb/*.o \
		rm -f $(builddir)/mods/testing/lmdb/*.d  \
		rm -f $(builddir)/mods/mini-games/*.o \
		rm -f $(builddir)/mods/mini-games/*.d \
		rm -f $(builddir)/mods/forge-engine/*.o \
		rm -f $(builddir)/mods/forge-engine/*.d \
		rm -f $(builddir)/mods/fluxkraft/*.o \
		rm -f $(builddir)/mods/fluxkraft/*.d \
		rm -f $(builddir)/mods/response-team/*.o \
		rm -f $(builddir)/mods/response-team/*.d \
		rm -f $(builddir)/mods/builder/*.o \
		rm -f $(builddir)/mods/builder/*.d \
		rm -f $(builddir)/mods/unit-tests/*.o \
		rm -f $(builddir)/mods/unit-tests/*.d 

$(builddir)/mods/response-team:
	mkdir -p $(builddir)/mods/response-team
$(builddir)/mods/fluxkraft:
	mkdir -p $(builddir)/mods/fluxkraft
$(builddir)/mods/forge-engine:
	mkdir -p $(builddir)/mods/forge-engine
$(builddir)/mods/mini-games:
	mkdir -p $(builddir)/mods/mini-games
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
$(builddir)/mods/boosters:
	mkdir -p $(builddir)/mods/boosters
$(builddir)/mods/mobs:
	mkdir -p $(builddir)/mods/mobs
$(builddir)/mods/builder:
	mkdir -p $(builddir)/mods/builder
$(builddir)/mods/armor:
	mkdir -p $(builddir)/mods/armor
$(builddir)/mods/players:
	mkdir -p $(builddir)/mods/players
$(builddir)/mods/unit-tests:
	mkdir -p $(builddir)/mods/unit-tests

$(builddir)/%.o: %.cpp
	$(CC) -c $(FLAGS) -o $@ $<

-include $(objects:.o=.d)

$(duktape): duktape-2.2.0/src/duktape.h duktape-2.2.0/src/duktape.c
	$(CC) -c $(FLAGS) duktape-2.2.0/src/duktape.c  -o $(duktape)

$(lmdb):
	cd $(lmdbdir) && make clean ; make liblmdb.so && cp liblmdb.so $(src) && cd $(src) 

$(DATA_DIR):
	mkdir $(DATA_DIR)
