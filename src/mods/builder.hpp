#ifndef __MENTOC_MODS_BUILDER_HEADER__
#define __MENTOC_MODS_BUILDER_HEADER__
#include <time.h>
#include <netdb.h>
#include <sys/socket.h>
#include <cstdio>
typedef int socket_t;
#include "../structs.h"
#include "../utils.h"
#include "../conf.h"
#include "../sysdep.h"
#include <array>
#include "../mods/util.hpp"
#include "../globals.hpp"
#include "../duktape/src/duktape.h"
#include <sys/stat.h>
#include <string_view>
#include "../interpreter.h"
namespace mods::builder {
//struct room_data {
//   1    room_vnum number;        /* Rooms number (vnum)            */
//   2    zone_rnum zone;              /* Room zone (for resetting)          */
//   3    int  sector_type;            /* sector type (move/hide)            */
//   4    char *name;                  /* Rooms name 'You are ...'           */
//   5    char *description;           /* Shown when entered                 */
//   6    struct extra_descr_data *ex_description; /* for examine/look       */
//   7    struct room_direction_data *dir_option[NUM_OF_DIRS]; /* Directions */
//   8    int /*bitvector_t*/ room_flags;      /* DEATH,DARK ... etc */
//   9
//  10    byte light;                  /* Number of lightsources in room     */
//  11    SPECIAL(*func);
//  12
//  13    struct obj_data *contents;   /* List of items in room              */
//  14    struct char_data *people;    /* List of NPC / PC in room           */
//  15 };
//  16
	constexpr static int ROOM_NUMBER_OUT_OF_RANGE = -1;
	constexpr static int ROOM_NAME_EMPTY = -2;
	constexpr static int ROOM_DESC_EMPTY = -3;
	extern std::array<std::pair<int,std::string>,17> extra_flags;
	extern std::array<std::pair<int,std::string>,15> wear_flags;
	extern std::array<std::pair<int,std::string>,25> affected_flags;
	extern std::array<std::pair<int,std::string>,23> type_flags;
	extern std::array<std::pair<int,std::string>,3> type2_flags;
	/* Factory method to generate a room for us */
	struct room_data new_room(struct char_data* ch,int direction);
	bool flush_to_db(struct char_data *ch,room_vnum room);
	bool title(room_rnum room,std::string_view str_title);
	bool description(room_rnum room,std::string_view str_description);
	std::optional<std::string> dir_option(room_rnum,int direction,std::optional<std::string_view> description,
	                                      std::optional<std::string_view> keywords,
	                                      std::optional<int> exit_info,
	                                      std::optional<int> key,
	                                      std::optional<room_rnum> to_room
	                                     );
	bool create_direction(room_rnum room_id,int direction,room_rnum room);
	bool destroy_direction(room_rnum,int direction);
	int save_to_db(room_rnum in_room);
	int import_room(struct room_data*);
	bool save_zone_to_db(std::string_view name,int room_start,int room_end,int lifespan,int reset_mode);
	std::pair<bool,std::string> zone_place(int zone_id,std::string_view command, std::string_view if_flag,std::string_view arg1, std::string_view arg2,std::string_view arg3);
	void pave_to(std::shared_ptr<mods::player> ch,room_data* room,int direction);
	std::optional<obj_data*> instantiate_object_by_index(int index);
	std::optional<obj_data*> instantiate_object_by_vnum(obj_vnum vnum);
	void report_error(mods::player* player,std::string_view message);
	void report_status(mods::player* player,std::string_view message);
	void report_success(mods::player* player,std::string_view message);
	template <typename T>
	void report_error(T player,std::string_view message) {
		*player << "{red}[error]: {/red}" << message.data() << "\r\n";
	}
	template <typename T>
	void report_status(T player,std::string_view message) {
		*player << "{gld}[status]: {/gld}" << message.data() << "\r\n";
	}
	template <typename T>
	void report_success(T player,std::string_view message) {
		*player << "{grn}[success]: {/grn}" << message.data() << "\r\n";
	}
};

ACMD(do_rbuild);
ACMD(do_zbuild);
#endif
