#ifndef __MENTOC_MODS_MOBS_MINI_GUNNER_HEADER__
#define  __MENTOC_MODS_MOBS_MINI_GUNNER_HEADER__

#include "../../globals.hpp"
#include "../damage-event.hpp"

namespace mods::mobs {
	struct mini_gunner {
		static void create(uuid_t uuid, std::string);
		static void free_mob(uuid_t uuid);
		void init();
		mini_gunner() = delete;
		~mini_gunner();
		mini_gunner(uuid_t mob_uuid,std::string variation);
		uuid_t uuid;
		void wear(int where,std::string_view yaml);
		void set_heading(int dir);
		int get_heading() const { return heading; }
		feedback_t& spray(int dir);
		void setup_damage_callbacks();
		int get_weapon_heat() const { return weapon_heat; }
		void set_behaviour_tree(std::string_view name);
		void shout(std::string);
		void set_variation(std::string);
		void enemy_spotted(room_rnum room,uuid_t player);
		str_map_t report();
		private:
		std::string variation;
		int weapon_heat;
		int heading;
		int spray_direction;
		player_ptr_t player_ptr;
		bool loaded;
		bool error;
		feedback_t last_attack;
	};
	extern std::map<uuid_t,std::shared_ptr<mini_gunner>> mg_map;
	namespace mg::orm {
		static inline std::string face_direction(std::string value){
			if(value.length() == 0){
				std::cerr << "[WARNING] face_direction is of zero length! Defaulting to NORTH\n";
				return "N";
			}
			return value.substr(0,1);
		}
	template <typename sql_compositor>
	static inline bool db_exists(mob_vnum mob_vnum){
			try{
				auto up_txn = txn();
				sql_compositor comp("mini_gunner_sentinel",&up_txn);
				auto up_sql = comp.select("id")
					.from("mini_gunner_sentinel")
					.where("mgs_mob_vnum","=",std::to_string(mob_vnum))
					.sql();
				auto rows = mods::pq::exec(up_txn,up_sql);
				mods::pq::commit(up_txn);
				return rows.size() > 0;
			}catch(std::exception& e){
				std::cerr << "error updating mini_gunner_sentinel in db: '" << e.what() << "'\n";
				return -1;
			}
	}
	template <typename sql_compositor>
	static inline int db_update(mob_vnum mob_vnum,str_map_t values){
		values["mgs_mob_vnum"] = std::to_string(mob_vnum);
			try{
		values["mgs_face_direction"] = face_direction(values["mgs_face_direction"]);
				auto up_txn = txn();
				sql_compositor comp("mini_gunner_sentinel",&up_txn);
				auto up_sql = comp
					.update("mini_gunner_sentinel")
					.set(values)
					.where("mgs_mob_vnum","=",std::to_string(mob_vnum))
					.sql();
				mods::pq::exec(up_txn,up_sql);
				mods::pq::commit(up_txn);
				return 0;
			}catch(std::exception& e){
				std::cerr << "error updating mini_gunner_sentinel in db: '" << e.what() << "'\n";
				return -1;
			}
	}
	template <typename sql_compositor>
	static inline int db_create(mob_vnum mob_vnum,str_map_t values){
		values["mgs_mob_vnum"] = std::to_string(mob_vnum);
		values["mgs_face_direction"] = face_direction(values["mgs_face_direction"]);
		{
			try {
			auto txn2 = txn();
			sql_compositor comp("mini_gunner_sentinel",&txn2);
			auto sql = comp
				.insert()
				.into("mini_gunner_sentinel")
				.values(values)
				.sql();
			auto row = mods::pq::exec(txn2,sql);
			mods::pq::commit(txn2);
			return 0;
			}catch(std::exception& e){
				std::cerr << "error updating mini_gunner_sentinel in db: '" << e.what() << "'\n";
				return -1;
			}
		}
	}
	};//end namespace
};

static inline bool has_mini_gunner_ptr(uuid_t u){
	return mods::mobs::mg_map.find(u) != mods::mobs::mg_map.end();
}

static inline std::shared_ptr<mods::mobs::mini_gunner> mini_gunner_ptr(uuid_t u){
	return mods::mobs::mg_map[u];
}
#endif
