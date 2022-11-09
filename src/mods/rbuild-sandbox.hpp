#ifndef __MENTOC_MODS_RBUILD_SANDBOX_HEADER__
#define __MENTOC_MODS_RBUILD_SANDBOX_HEADER__
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
extern int get_raid_id(player_ptr_t player);

namespace mods::builder {
	int16_t delete_all_zone_data();
	std::tuple<bool,std::string> delete_by_mob_vnum(std::string_view mob_vnum);
	std::tuple<int8_t,std::string> pave_on(player_ptr_t& player,std::string_view sandbox_name);
	std::tuple<int8_t,std::string> pave_continue(player_ptr_t& player);
	std::tuple<int8_t,std::string> pave_off(player_ptr_t& player);
	bool currently_paving(player_ptr_t& player);
	void initialize_builder(player_ptr_t& player);

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
};

void r_error(const player_ptr_t& player,std::string_view msg);
void r_success(const player_ptr_t& player,std::string_view msg);
void r_status(const player_ptr_t& player,std::string_view msg);
#define ENCODE_RESPONSE(__ENC_RESPONSE) mods::builder::encode_scripted_response(player, __ENC_RESPONSE)
#define ENCODE_STR(__ENC_RESPONSE) mods::builder::encode_scripted_response(player, std::to_string(__ENC_RESPONSE))
#define ENCODE_R(__ENC_RESPONSE) mods::builder::encode_scripted_response(player, __ENC_RESPONSE)
#define ENCODE_INIT(__ENC_RESPONSE) mods::builder::encode_scripted_response(player, "!")
#define ENCODE_OK() mods::builder::encode_scripted_response(player, "ok")
#define ENCODE_MAP(__ENC_RESPONSE) mods::builder::encode_scripted_response(player, __ENC_RESPONSE)
#define ENCODE_SAFE(__ENC_RESPONSE) mods::builder::encode_scripted_response_safe(player, __ENC_RESPONSE)


#endif
