#ifndef __MENTOC_MODS_BUILDER_HEADER__
#define __MENTOC_MODS_BUILDER_HEADER__
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

void r_error(const player_ptr_t& player,std::string_view msg);
void r_success(const player_ptr_t& player,std::string_view msg);
void r_status(const player_ptr_t& player,std::string_view msg);
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
	constexpr static int ROOM_NUMBER_OUT_OF_RANGE = -1;
	constexpr static int ROOM_NAME_EMPTY = -2;
	constexpr static int ROOM_DESC_EMPTY = -3;
	constexpr static unsigned int RNUMLIST_MAX_PER_CALL = 10000;
	extern std::array<std::pair<int,std::string>,17> extra_flags;
	extern std::array<std::pair<int,std::string>,15> wear_flags;
	extern std::array<std::pair<int,std::string>,38> affected_flags;
	extern std::array<std::pair<int,std::string>,24> type_flags;
	extern std::array<std::pair<int,std::string>,3> type2_flags;

	constexpr static uint64_t HAS_SANDBOX = (1 << 0);

	struct sandbox_data_t {
			sandbox_data_t();
			~sandbox_data_t() = default;
			sandbox_data_t(
			    player_ptr_t player,
			    std::string_view name,
			    int start,
			    int zone_virtual_number);
			int8_t new_sandbox(
			    player_ptr_t player,
			    std::string_view name,
			    int start,
			    int zone_virtual_number);
			std::string_view name() const;
			void set_name(std::string_view n);
			std::shared_ptr<builder_data_t> builder_data() {
				return m_builder_data;
			}
		protected:
			std::shared_ptr<builder_data_t> m_builder_data;
			std::string m_name;
			player_ptr_t m_player;
	};

	using sandbox_list_t = std::unordered_map<std::string,std::deque<sandbox_data_t>> ;
	extern sandbox_list_t sandboxes;
	/* Factory method to generate a room for us */
	room_data new_room(player_ptr_t player,int direction);
	bool flush_to_db(char_data *ch,int room);
	bool title(room_rnum room,std::string_view str_title);
	bool description(room_rnum room,std::string_view str_description);
	std::optional<std::string> dir_option(room_rnum,int direction,std::optional<std::string_view> description,
	                                      std::optional<std::string_view> keywords,
	                                      std::optional<int> exit_info,
	                                      std::optional<int> key,
	                                      std::optional<room_rnum> to_room
	                                     );
	bool create_direction(room_rnum room_id,byte direction,room_rnum room);
	bool destroy_direction(room_rnum,int direction);
	int save_to_db(room_rnum in_room);
	int import_room(struct room_data*);
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
	void add_room_to_pavements(player_ptr_t& player, int room_id);
	/** if executing js, encode it */
	void encode_scripted_response(player_ptr_t& player, std::string_view encoded);
};

void r_error(const player_ptr_t& player,std::string_view msg);
void r_success(const player_ptr_t& player,std::string_view msg);
void r_status(const player_ptr_t& player,std::string_view msg);
#define ENCODE_RESPONSE(__ENC_RESPONSE) mods::builder::encode_scripted_response(player, __ENC_RESPONSE)
#define ENCODE_STR(__ENC_RESPONSE) mods::builder::encode_scripted_response(player, std::to_string(__ENC_RESPONSE))
#define ENCODE_R(__ENC_RESPONSE) mods::builder::encode_scripted_response(player, __ENC_RESPONSE)
#define ENCODE_INIT(__ENC_RESPONSE) mods::builder::encode_scripted_response(player, "!")
//ACMD(do_rbuild);
//ACMD(do_rbuild_sandbox);
//ACMD(do_zbuild);
//ACMD(do_obuild);
//ACMD(do_sbuild);
//ACMD(do_mbuild);
#endif
