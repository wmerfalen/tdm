#ifndef __MENTOC_MODS_ROOMS_HEADER__
#define __MENTOC_MODS_ROOMS_HEADER__
#include "../globals.hpp"
extern obj_ptr_t optr_by_uuid(uuid_t);
// For our dissolver
#include "affects.hpp"

#ifdef __MENTOC_SHOW_MODS_ROOMS_AFFECTS_DEBUG_OUTPUT__
	#define mra_debug(a) std::cerr << "[mods::affects]" << __FILE__ << "|" << __LINE__ << "->" << a << "\n";
#else
	#define mra_debug(a) /*-*/
#endif
namespace mods::rooms {
	using txt = room_data::texture_type_t;
	using fs = room_data::fire_status_t;
	using fire_status_t = room_data::fire_status_t;
	static const std::map<int,std::string> room_flag_strings = {
		{ROOM_DARK, "ROOM_DARK"},
		{ROOM_DEATH, "ROOM_DEATH"},
		{ROOM_NOMOB, "ROOM_NOMOB"},
		{ROOM_INDOORS, "ROOM_INDOORS"},
		{ROOM_PEACEFUL, "ROOM_PEACEFUL"},
		{ROOM_DMZ, "ROOM_DMZ"},
		{ROOM_SOUNDPROOF, "ROOM_SOUNDPROOF"},
		{ROOM_NOTRACK, "ROOM_NOTRACK"},
		{ROOM_NOMAGIC, "ROOM_NOMAGIC"},
		{ROOM_TUNNEL, "ROOM_TUNNEL"},
		{ROOM_PRIVATE, "ROOM_PRIVATE"},
		{ROOM_GODROOM, "ROOM_GODROOM"},
		{ROOM_HOUSE, "ROOM_HOUSE"},
		{ROOM_HOUSE_CRASH, "ROOM_HOUSE_CRASH"},
		{ROOM_ATRIUM, "ROOM_ATRIUM"},
		{ROOM_OLC, "ROOM_OLC"},
		{ROOM_BFS_MARK, "ROOM_BFS_MARK"}
	};
	static inline std::optional<int> room_flag_from_string(std::string& s){
#define MENTOC_LAZY(A) if(s.compare(#A) == 0){ return A; }
		MENTOC_LAZY(ROOM_DARK);
		MENTOC_LAZY(ROOM_DEATH);
		MENTOC_LAZY(ROOM_NOMOB);
		MENTOC_LAZY(ROOM_INDOORS);
		MENTOC_LAZY(ROOM_PEACEFUL);
		MENTOC_LAZY(ROOM_DMZ);
		MENTOC_LAZY(ROOM_SOUNDPROOF);
		MENTOC_LAZY(ROOM_NOTRACK);
		MENTOC_LAZY(ROOM_NOMAGIC);
		MENTOC_LAZY(ROOM_TUNNEL);
		MENTOC_LAZY(ROOM_PRIVATE);
		MENTOC_LAZY(ROOM_GODROOM);
		MENTOC_LAZY(ROOM_HOUSE);
		MENTOC_LAZY(ROOM_HOUSE_CRASH);
		MENTOC_LAZY(ROOM_ATRIUM);
		MENTOC_LAZY(ROOM_OLC);
		MENTOC_LAZY(ROOM_BFS_MARK);
#undef MENTOC_LAZY
		return std::nullopt;
	}

	static inline void set_flag(room_rnum room, int flag){
		world[room].room_flags |= flag;
	}
	static inline void remove_flag(room_rnum room, int flag){
		world[room].room_flags &= ~flag;
	}
	static inline std::vector<std::string> get_room_flag_strings(){
		std::vector<std::string> m;
		for(auto & pair : room_flag_strings){
			m.emplace_back(pair.second);
		}
		return m;
	}
	void set_flag_absolute(room_rnum room, int flag);

	static inline std::vector<std::string> get_room_flags_from_room(room_rnum r){
		std::vector<std::string> accumulator;
		for(auto & pair : room_flag_strings){
			if(world[r].room_flags & pair.first){
				accumulator.emplace_back(pair.second.substr(strlen("ROOM_")));
			}
		}
		return accumulator;
	}

	enum sector_type_t {
		NONE = 0,
		OUTSIDE_GRASSY,
		OUTSIDE_CEMENT,
		OUTSIDE_FOREST,
		OUTSIDE_WOODEN_WALLS,
		OUTSIDE_DIRT,
		OUTSIDE_DESERT,
		OUTSIDE_WATER,
		OUTSIDE_UNDERWATER,
		OUTSIDE_FROZEN,
		OUTSIDE_HILLS,
		OUTSIDE_MARKET_PLACE,
		OUTSIDE_METAL_HATCH,
		OUTSIDE_TREE,
		OUTSIDE_ROOFTOP,
		OUTSIDE_WOODEN_WALL,	/** rappelling */
		OUTSIDE_METAL_WALL,	/** rappelling */
		OUTSIDE_AIR,	/** on a rope hanging from a helicopter */
		INDOOR_WOODEN_WALLS,
		INDOOR_CEMENT,
		INDOOR_CEMENT_WOODEN_WALLS,
		INDOOR_CARPET_WOODEN_WALLS,
		INDOOR_TUNNEL,
		INDOOR_METAL_WALLS,
		INDOOR_SERVER_ROOM,
		INDOOR_SEWER,
		INDOOR_MARKET_PLACE,
		INDOOR_METAL_HATCH,
		INDOOR_ROOF,	/** attached to the roof like spiderman */
		INDOOR_ATTIC,
		INDOOR_BASEMENT
	};
	/**
	 * vim sorcery: :48,79s/^\t\t\([A-Z_]\+\),\?/\t\t{sector_type_t::\1,"\1"},/g
	 */
	static const std::map<sector_type_t,std::string> sector_strings {
		{sector_type_t::NONE,"NONE"},
			{sector_type_t::OUTSIDE_GRASSY,"OUTSIDE_GRASSY"},
			{sector_type_t::OUTSIDE_CEMENT,"OUTSIDE_CEMENT"},
			{sector_type_t::OUTSIDE_FOREST,"OUTSIDE_FOREST"},
			{sector_type_t::OUTSIDE_WOODEN_WALLS,"OUTSIDE_WOODEN_WALLS"},
			{sector_type_t::OUTSIDE_DIRT,"OUTSIDE_DIRT"},
			{sector_type_t::OUTSIDE_DESERT,"OUTSIDE_DESERT"},
			{sector_type_t::OUTSIDE_WATER,"OUTSIDE_WATER"},
			{sector_type_t::OUTSIDE_UNDERWATER,"OUTSIDE_UNDERWATER"},
			{sector_type_t::OUTSIDE_FROZEN,"OUTSIDE_FROZEN"},
			{sector_type_t::OUTSIDE_HILLS,"OUTSIDE_HILLS"},
			{sector_type_t::OUTSIDE_MARKET_PLACE,"OUTSIDE_MARKET_PLACE"},
			{sector_type_t::OUTSIDE_METAL_HATCH,"OUTSIDE_METAL_HATCH"},
			{sector_type_t::OUTSIDE_TREE,"OUTSIDE_TREE"},
			{sector_type_t::OUTSIDE_ROOFTOP,"OUTSIDE_ROOFTOP"},
			{sector_type_t::OUTSIDE_WOODEN_WALL,"OUTSIDE_WOODEN_WALL"},
			{sector_type_t::OUTSIDE_METAL_WALL,"OUTSIDE_METAL_WALL"},
			{sector_type_t::OUTSIDE_AIR,"OUTSIDE_AIR"},
			{sector_type_t::INDOOR_WOODEN_WALLS,"INDOOR_WOODEN_WALLS"},
			{sector_type_t::INDOOR_CEMENT,"INDOOR_CEMENT"},
			{sector_type_t::INDOOR_CEMENT_WOODEN_WALLS,"INDOOR_CEMENT_WOODEN_WALLS"},
			{sector_type_t::INDOOR_CARPET_WOODEN_WALLS,"INDOOR_CARPET_WOODEN_WALLS"},
			{sector_type_t::INDOOR_TUNNEL,"INDOOR_TUNNEL"},
			{sector_type_t::INDOOR_METAL_WALLS,"INDOOR_METAL_WALLS"},
			{sector_type_t::INDOOR_SERVER_ROOM,"INDOOR_SERVER_ROOM"},
			{sector_type_t::INDOOR_SEWER,"INDOOR_SEWER"},
			{sector_type_t::INDOOR_MARKET_PLACE,"INDOOR_MARKET_PLACE"},
			{sector_type_t::INDOOR_METAL_HATCH,"INDOOR_METAL_HATCH"},
			{sector_type_t::INDOOR_ROOF,"INDOOR_ROOF"},
			{sector_type_t::INDOOR_ATTIC,"INDOOR_ATTIC"},
			{sector_type_t::INDOOR_BASEMENT,"INDOOR_BASEMENT"}
	};

	static inline std::string texture_to_string(room_data::texture_type_t t){
#define MENTOC_LAZY(A) if(t == room_data::texture_type_t::A){ return #A; }
		MENTOC_LAZY(GRASS);
		MENTOC_LAZY(CEMENT);
		MENTOC_LAZY(OUTSIDE);
		MENTOC_LAZY(INSIDE);
		MENTOC_LAZY(SEWER);
		MENTOC_LAZY(RADIOACTIVE);
		MENTOC_LAZY(VOLATILE);
		MENTOC_LAZY(RUBBLE);
		MENTOC_LAZY(DIRT);
		MENTOC_LAZY(SHATTERED_GLASS);
		MENTOC_LAZY(LOW_ATMOSPHERE);
		MENTOC_LAZY(ON_FIRE);
		MENTOC_LAZY(NON_HAZARDOUS_SMOKE);
		MENTOC_LAZY(HAZARDOUS_SMOKE);
		MENTOC_LAZY(EMP);
		MENTOC_LAZY(TUNNEL);
		MENTOC_LAZY(LADDER);
		MENTOC_LAZY(ELEVATOR);
		MENTOC_LAZY(GLASS_WINDOWS);
		MENTOC_LAZY(SCANNED);
		MENTOC_LAZY(DAMP);
		MENTOC_LAZY(FOREST);
		MENTOC_LAZY(WOODEN_WALLS);
		MENTOC_LAZY(CARPET);
		MENTOC_LAZY(DRY);
		MENTOC_LAZY(DESERT);
		MENTOC_LAZY(WATER);
		MENTOC_LAZY(UNDERWATER);
		MENTOC_LAZY(METAL_HATCH);
		MENTOC_LAZY(METAL_WALL);
		MENTOC_LAZY(TREE);
		MENTOC_LAZY(ROOFTOP);
		MENTOC_LAZY(AIR);
		MENTOC_LAZY(FROZEN);
		MENTOC_LAZY(SERVER_ROOM);
#undef MENTOC_LAZY
		return "<unknown>";
	}

	void set_sector_type(room_rnum room_id, int sector_type);
	std::optional<sector_type_t> sector_from_string(std::string m);
	static inline bool is_peaceful(room_rnum room){
		return world[room].room_flags & ROOM_PEACEFUL;
	}
	bool has_textures(room_rnum r, std::vector<txt>);
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
	void start_fire_dissolver(room_rnum room);

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
