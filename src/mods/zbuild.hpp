#ifndef __MENTOC_MODS_ZBUILD_HEADER__
#define __MENTOC_MODS_ZBUILD_HEADER__
#define __MENTOC_BUILDER_UTILS_ACCEPT_ZERO_INDEX__
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
#include <unordered_map>
#include "flags.hpp"
#include "rbuild.hpp"

void r_error(const player_ptr_t& player,std::string_view msg);
void r_success(const player_ptr_t& player,std::string_view msg);
void r_status(const player_ptr_t& player,std::string_view msg);
extern int next_room_number();
extern int next_obj_number();
extern int next_zone_number();
extern int next_mob_number();

namespace mods::builder {
	std::tuple<int8_t,std::string> pave_on(player_ptr_t& player,std::string_view sandbox_name);
	std::tuple<int8_t,std::string> pave_continue(player_ptr_t& player);
	std::tuple<int8_t,std::string> pave_off(player_ptr_t& player);
	bool currently_paving(player_ptr_t& player);
	void initialize_builder(player_ptr_t& player);
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
	//  14    char_data *people;    /* List of NPC / PC in room           */
	//  15 };
	//  16
	extern std::array<std::pair<int,std::string>,17> extra_flags;
	extern std::array<std::pair<int,std::string>,15> wear_flags;
	extern std::array<std::pair<int,std::string>,38> affected_flags;
	extern std::array<std::pair<int,std::string>,24> type_flags;
	extern std::array<std::pair<int,std::string>,3> type2_flags;

	using zone_pkid_t = int;
	std::tuple<bool,zone_pkid_t> save_zone_to_db(
	    int virtual_number,
	    std::string_view name,
	    int room_start,
	    int room_end,
	    int lifespan,
	    int reset_mode
	);
	std::pair<bool,std::string> zone_place(int zone_id,std::string_view command, std::string_view if_flag,std::string_view arg1, std::string_view arg2,std::string_view arg3);
	std::optional<obj_data*> instantiate_object_by_index(int index);
	std::optional<obj_data*> instantiate_object_by_vnum(int vnum);
	void report_error(mods::player* player,std::string_view message);
	void report_status(mods::player* player,std::string_view message);
	void report_success(mods::player* player,std::string_view message);
	void encode_scripted_response(player_ptr_t& player, std::string_view encoded);
	int next_zone_vnum();
	void disable_zone_resets() ;
	void enable_zone_resets() ;
	void blacklist_zone(int zone);
	void release_zone(int zone);
};

void r_error(const player_ptr_t& player,std::string_view msg);
void r_success(const player_ptr_t& player,std::string_view msg);
void r_status(const player_ptr_t& player,std::string_view msg);
#ifndef ENCODE_RESPONSE
	#define ENCODE_RESPONSE(__ENC_RESPONSE) mods::builder::encode_scripted_response(player, __ENC_RESPONSE)
	#define ENCODE_STR(__ENC_RESPONSE) mods::builder::encode_scripted_response(player, std::to_string(__ENC_RESPONSE))
	#define ENCODE_R(__ENC_RESPONSE) mods::builder::encode_scripted_response(player, __ENC_RESPONSE)
	#define ENCODE_INIT(__ENC_RESPONSE) mods::builder::encode_scripted_response(player, "!")
	#define ENCODE_OK() mods::builder::encode_scripted_response(player, "ok")
	#define ENCODE_MAP(__ENC_RESPONSE) mods::builder::encode_scripted_response(player, __ENC_RESPONSE)
	#define ENCODE_SAFE(__ENC_RESPONSE) mods::builder::encode_scripted_response_safe(player, __ENC_RESPONSE)
#endif


#endif
