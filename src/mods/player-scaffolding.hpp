#ifndef __MENTOC_MODS_PLAYER_SCAFFOLDING_HEADER__
#define  __MENTOC_MODS_PLAYER_SCAFFOLDING_HEADER__

#include "player.hpp"

namespace mods {
	struct player_scaffolding {
		//using player_ptr_t = player_ptr_t;
		//using points_map_t = std::map<std::string,int>;
		//using std::vector<player_ptr_t>  = std::vector<player_ptr_t>;
		constexpr static std::size_t MAIN_PLAYER_INDEX = 0;
		constexpr static std::size_t TARGET_PLAYER_INDEX = 1;
		constexpr static std::size_t OTHER_PLAYER_INDEX_START = 2;
			player_scaffolding();
			player_scaffolding(player_ptr_t);
			//TODO: player_scaffolding(std::string_view player_name);
			int8_t sniping(bool start_stop);
			int8_t breaching(bool start_stop,bool soft_walls);
			/**
			 * Initializes and bootstraps the player. 
			 * @param player ptr
			 * @param the room number to send the player to
			 * @returns -1 on error, call get_last_error() to get error strings. Returns 0 on success.
			 */
			int initialize_player(player_ptr_t player,int to_room);
			/**
			 * Creates main and target players by looking them up in lmdb. 
			 * Additionally, you can pass in multiple other names via the third param. These other players will be created and stored in the m_other_players protected member variable.
			 * @param main_player main player
			 * @param target_player target player
			 * @param optional_other_players
			 * @return std::vector<player_ptr_t> of all players. first two elements are main_player and target_player respectively. The rest (if any) are all other players you specified.
			 */
			std::vector<player_ptr_t>& set_players(std::string_view main_player,
			std::string_view target_player,const std::vector<std::string_view>& optional_other_players);
			/**
			 * Creates the main and target players by looking them up in lmdb
			 * @param main_player the main player. usually the one that is the core of this scaffolding experiment
			 * @param target_player the target player that is participating in this experiment
			 * @returns Returns -1 if the creation failed. get_last_error() can be called on player_scaffolding to find out what happened. Returns 0 on success. Both player_ptr_t objects are stored in the m_main_player and m_target_player protected member variables on this player_scaffolding object.
			 */
			int set_players(std::string_view main_player,std::string_view target_player);
			std::vector<std::string>* get_last_error();
			/**
			 * Sets main and target player
			 * @returns 0 always. Future code may make this react differently.
			 */
			int set_players(player_ptr_t main_player,player_ptr_t target_player);
			/**
			 * Creates a player by emplacing it onto the player list and just returning it. 
			 * @note this player will not have been initialized. 
			 * @note player is created using player_type_enum_t::PLAYER flag to the mods::player constructor
			 * @return player_ptr_t
			 */
			player_ptr_t create_player();
			/**
			 * Resets the passed in player's points (char_data->points.*)
			 *
			 * @param player The player_ptr_t that you want to modify
			 * @param points_map An optional player_scaffolding::points_map_t pointer of the values you want to set. The keys are the names of the functions on mods::player that affect the points of the player. If this is parameter is nullptr then this function will set the points to hardcoded values. See the function definition for what those values are.
			 * @return void
			 */
			void reset_points(player_ptr_t player, std::map<std::string_view,int>* points_map);
			/**
			 * Creates a typical target player that has his descriptor_data muted
			 * @return the player's pointer
			 */
			player_ptr_t create_muted_player_target();
			
		protected:
			std::vector<player_ptr_t> m_players;
			std::vector<std::string> m_errors;
			
	};
};

#endif
