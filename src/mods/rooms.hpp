#ifndef __MENTOC_MODS_ROOMS_HEADER__
#define __MENTOC_MODS_ROOMS_HEADER__
#include "../globals.hpp"
extern obj_ptr_t optr_by_uuid(uuid_t);
// For our dissolver
#include "affects.hpp"

#define mra_debug(a) std::cerr << "[mods::affects]" << __FILE__ << "|" << __LINE__ << "->" << a << "\n";
namespace mods::rooms {
	using fire_status_t = room_data::fire_status_t;
	static inline bool is_peaceful(room_rnum room){
		return world[room].room_flags & ROOM_PEACEFUL;
	}

	namespace affects {
		using affect_t = room_data::texture_type_t;
		using affect_amount_t = uint32_t;
		using affect_map_t = std::map<affect_t,affect_amount_t>;
		using entity_type_t = room_rnum;
		using dissolver_t = mods::affects::dissolver<affect_t,std::vector<affect_t>,affect_map_t,entity_type_t>;
		using callback_t = std::function<void(entity_type_t,affect_t,affect_amount_t)>;

		/**
		 * @brief room_rnum -> dissolver_t
		 */
		static std::map<entity_type_t,dissolver_t> needs_dissolve;
		void process();
		/**
		 * @brief add a dissolve affect to a room
		 *
		 * @param room
		 * @param items
		 */
		static inline void add_room_dissolve_affect(room_rnum room, affect_map_t& items){
			mra_debug("Added mapped items to " << room);
			needs_dissolve[room].entity_id = room;
			needs_dissolve[room].affect_map(items);
		}
		/*! Add a specific affect onto a room, except only process it ever N ticks
		 * @param room_rnum room
		 * @param affect_t affect
		 * @param affect_amount_t amount
		 * @param uint32_t How many ticks before we process it
		 */
		static inline void add_room_dissolve_affect_every_n_tick(room_rnum room, affect_t affect, affect_amount_t amt, uint32_t n_ticks){
			needs_dissolve[room].entity_id = room;
			needs_dissolve[room].affect_every_n_ticks(affect,amt,n_ticks);
			mra_debug("Added room dissolve using ever n (" << n_ticks << ") ticks... to " << room);
		}
		/**
		 * @brief set direction to increment
		 *
		 * @param room
		 * @param affect
		 */
		static inline void set_affect_to_increment(room_rnum room,affect_t affect){
			needs_dissolve[room].set_direction(affect,true);
			mra_debug("Set direction to increment for affect: " << affect);
		}
		/**
		 * @brief change direction to decrememnt (this is the default)
		 *
		 * @param room
		 * @param affect
		 */
		static inline void set_affect_to_decrement(room_rnum room,affect_t affect){
			needs_dissolve[room].set_direction(affect,false);
			mra_debug("Set direction to decrement for affect: " << affect);
		}
		/**
		 * @brief sets maximum amount on incrementing affect to amount
		 *
		 * @param room
		 * @param affect
		 * @param amount
		 */
		static inline void set_affect_max_amount(room_rnum room,affect_t affect,affect_amount_t amount){
			needs_dissolve[room].set_max_amount(affect,amount);
			mra_debug("Set direction to increment for affect: " << affect);
		}
		/**
		 * @brief remove any dissolver affect
		 *
		 * @param room
		 * @param item
		 */
		static inline void remove_room_dissolve_affect(room_rnum room, affect_t item){
			needs_dissolve.erase(room);
		}

		/**
		 * @brief set the callback when the affect changes values
		 *
		 * @param room
		 * @param affect
		 * @param f
		 */
		static inline void add_callback(room_rnum room, affect_t affect, callback_t f){
			needs_dissolve[room].set_callback(affect,f);
		}

		static inline void on_room_fire_changed_affect(
				room_rnum room,
				affect_t affect, /* will always be ON_FIRE */
				affect_amount_t amount
				){
			fire_status_t fire_status = (fire_status_t)amount;
			world[room].fire_status() = fire_status;
			switch(fire_status){
				case fire_status_t::KINDLING:
					send_to_room(room, "\r\n{red}A fire begins to form!{/red}\r\n");
					break;
				case fire_status_t::COMPLETELY_ON_FIRE:
					send_to_room(room, "\r\n{red}The room is {yel}TOTALLY ENGULFED{/yel}{red} in fire!{/red}\r\n");
					break;
				case fire_status_t::SMOLDERING:
					send_to_room(room, "\r\n{red}The fire loses some intesity and smoke begins to fill the room.{/red}\r\n");
					break;
				case fire_status_t::SMOKING:
					send_to_room(room, "\r\n{red}The room is filled with smoke as the fire dies...{/red}\r\n");
					break;
				case fire_status_t::OUT:
					send_to_room(room, "\r\n{red}The fire is put out.{/red}\r\n");
					break;
				default:
					log((std::string("Invalid fire status: ") + std::to_string(fire_status) + std::string("on room:") + std::to_string(room)).c_str());
					break;
			}
		}
		static inline void trigger_callback_for_affect(room_rnum room,affect_t affect){
			needs_dissolve[room].trigger_callback(affect);
		}
	};//end affects namespace



	/** 
	 * \brief true if room is dark
	 * @param room_rnum room
	 */
	static inline bool is_dark(room_rnum room) {
		if(room >= world.size()){
			log("SYSERR: %d is out of bounds",room);
			return false;
		}
		if(world[room].light) {
			return false;
		}

		if(ROOM_FLAGGED(room, ROOM_DARK)) {
			return true;
		}

		if(SECT(room) == SECT_INSIDE || SECT(room) == SECT_CITY) {
			return false;
		}

		if(weather_info.sunlight == SUN_SET || weather_info.sunlight == SUN_DARK) {
			return true;
		}

		return false;
	}
	/**
	 * @brief returns true if room has dark flags
	 *
	 * @param room
	 *
	 * @return 
	 */
	inline bool is_dark(room_data& room) {
		return is_dark(real_room(room.number));
	}

	/**
	 * @brief returns true if room is smoking
	 *
	 * @tparam TRoom
	 * @param room
	 *
	 * @return bool
	 */
	template <typename TRoom>
	static inline bool is_smoked(TRoom room) {
		auto & textures = world[room].textures();
		if(textures.size() == 0){
			return false;
		}
		std::array<room_data::texture_type_t,2> smokes = {
			room_data::texture_type_t::NON_HAZARDOUS_SMOKE,
			room_data::texture_type_t::HAZARDOUS_SMOKE
		};
		return std::find_first_of(textures.begin(),textures.end(),smokes.begin(),smokes.end()) != textures.end();
	}
	/**
	 * @brief true if on fire
	 *
	 * @tparam TRoom
	 * @param room
	 *
	 * @return 
	 */
	template <typename TRoom>
	static inline bool is_on_fire(TRoom room) {
		auto & textures = world[room].textures();
		if(textures.size() == 0){
			return false;
		}
		return std::find(textures.begin(),textures.end(),room_data::texture_type_t::ON_FIRE) != textures.end();
	}
	/**
	 * @brief gets fire_status_t enum if room on fire
	 *
	 * @tparam TRoom
	 * @param room
	 *
	 * @return 
	 */
	template <typename TRoom>
	static inline fire_status_t get_fire_status(TRoom room) {
		return world[room].fire_status();
	}

	/**
	 * @brief completely stops a fire and removes texture from room
	 *
	 * @param room
	 */
	static inline void stop_fire_dissolver(room_rnum room) {
		if(room >= world.size()){
			return;
		}
		auto fire = room_data::texture_type_t::ON_FIRE;
		affects::remove_room_dissolve_affect(
				room,
				(affects::affect_t)fire
		);
		world[room].remove_texture(fire);
		maffects_debug("Removed fire texture from: " << room);
	}
	/**
	 * @brief entry point for setting a room on fire. use this
	 *
	 * @param room
	 */
	static inline void start_fire_dissolver(room_rnum room) {
		if(is_peaceful(room)){
			log("Prevented peaceful room to catch on fire (%d)", room);
			return;
		}
		auto fire = room_data::texture_type_t::ON_FIRE;
		if(room >= world.size()){
			return;
		}
		if(world[room].has_texture(fire) && affects::needs_dissolve.find(room) != affects::needs_dissolve.end()){
			log("Room already has fire dissolver running. Roomid: %d", room);
			return;
		}
		world[room].add_texture(fire);
		affects::add_room_dissolve_affect_every_n_tick(
				room,
				(affects::affect_t)fire,
				room_data::fire_status_t::KINDLING,
				FIRE_EVERY_N_TICKS
		);

		affects::set_affect_to_increment(
				room,
				fire
		);
		
		affects::set_affect_max_amount(
				room,
				fire,
				fire_status_t::OUT
		);

		affects::add_callback(room,
			fire,
			affects::on_room_fire_changed_affect
		);

		affects::trigger_callback_for_affect(room,fire);
	}

	/**
	 * @brief convert fire_status_t enum to string
	 *
	 * @param stat
	 *
	 * @return std::string
	 */
	static inline std::string to_string(room_data::fire_status_t stat){
		switch(stat){
			case room_data::fire_status_t::NONE:
				return "NONE";
			case room_data::fire_status_t::KINDLING:
				return "KINDLING";
			case room_data::fire_status_t::COMPLETELY_ON_FIRE:
				return "COMPLETELY_ON_FIRE";
			case room_data::fire_status_t::SMOLDERING:
				return "SMOLDERING";
			case room_data::fire_status_t::SMOKING:
				return "SMOKING";
			case room_data::fire_status_t::OUT:
				return "OUT";
			default:
				return std::string("? -> ") + std::to_string(stat);
		}
	}

	/**
	 * @brief returns true if world[room].fire_status_t is a certain value
	 *
	 * @param room
	 *
	 * @return 
	 */
	static inline bool can_see_through_fire(room_rnum room) {
		switch(get_fire_status(room)){
			case mods::rooms::fire_status_t::COMPLETELY_ON_FIRE:
			case mods::rooms::fire_status_t::SMOLDERING:
			case mods::rooms::fire_status_t::SMOKING:
				return false;
			default: return true;
		}
	}

	/**
	 * @brief get the real room
	 *
	 * @param room_id
	 *
	 * @return 
	 */
	inline room_rnum real_room(room_vnum room_id){
		for(room_rnum i=0; i < world.size();++i){
			if(world[i].number == room_id){
				return i;
			}
		}
		return NOWHERE;
	}

	/**
	 * =============================
	 * = Methods reserved for gods =
	 * =============================
	 */
	namespace gods {
		/**
		 * @brief set flags for darkness in the room
		 *
		 * @tparam TRoom
		 * @param room
		 */
		template <typename TRoom>
			static inline void force_dark(TRoom room) {
				if(room >= world.size()){
						return;
				}
				world[room].light = false;
				SET_BIT(ROOM_FLAGS(room), ROOM_DARK);
				return;
			}
		/**
		 * @brief set flags for light in the room
		 *
		 * @tparam TRoom
		 * @param room
		 */
		template <typename TRoom>
			static inline void remove_dark(TRoom room) {
				if(room >= world.size()){
						return;
				}
				world[room].light = true;
				REMOVE_BIT(ROOM_FLAGS(room), ROOM_DARK);
				return;
			}

		/**
		 * @brief FOR DEBUGGING ONLY - debugging function used to set the fire texture and a fire_status_t level
		 *
		 * @param room
		 * @param on
		 * @param level
		 * @param player
		 */
			static inline void set_fire(room_rnum room,bool on,std::string& level,player_ptr_t& player) {
				std::string msg = "";
				bool found = false;
				if(room >= world.size()){
					msg = "invalid room id";
					goto cleanup;
				}
				if(on){
					msg += "\r\nsetting room to on fire\r\n";
					start_fire_dissolver(room);
				}else{
					msg += "\r\nsetting room to __NOT__ on fire\r\n";
					stop_fire_dissolver(room);
				}
				if(level.length() == 0){
					goto cleanup;
				}
				if(level.compare("NONE") == 0){
					world[room].fire_status() = room_data::fire_status_t::OUT;
					msg += "set to OUT...";
					found = true;
					goto cleanup;
				}
				if(level.compare("KINDLING") == 0){
					world[room].fire_status() = room_data::fire_status_t::KINDLING;
					msg += "set to kindling...";
					found = true;
					goto cleanup;
				}
				if(level.compare("COMPLETELY_ON_FIRE") == 0){
					world[room].fire_status() = room_data::fire_status_t::COMPLETELY_ON_FIRE;
					msg += "set to cof...";
					found = true;
					goto cleanup;
				}
				if(level.compare("SMOLDERING") == 0){
					world[room].fire_status() = room_data::fire_status_t::SMOLDERING;
					msg += "set to SMOLDERING...";
					found = true;
					goto cleanup;
				}
				if(level.compare("SMOKING") == 0){
					world[room].fire_status() = room_data::fire_status_t::SMOKING;
					msg += "set to SMOKING...";
					found = true;
					goto cleanup;
				}
				if(level.compare("OUT") == 0){
					world[room].fire_status() = room_data::fire_status_t::OUT;
					msg += "set to OUT...";
					found = true;
					goto cleanup;
				}
				if(!found){
					player->sendln("Options are KINDLING,COMPLETELY_ON_FIRE,SMOLDERING,SMOKING,OUT,NONE");
				}
				cleanup:
					msg += "\r\ncurrent fire status: '";
					msg += to_string(get_fire_status(room)) + "'\r\n";
					player->sendln(msg);
				return;
			}
	};//end gods
};//end namespace
#endif
