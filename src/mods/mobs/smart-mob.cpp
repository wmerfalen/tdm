#include "smart-mob.hpp"
#include "../rooms.hpp"
#include "helpers.hpp"
#include "../mob-equipment.hpp"

#ifdef  __MENTOC_MODS_MOBS_LOWLY_SECURITY_SHOW_DEBUG_OUTPUT__
#define sm_debug(a) mentoc_prefix_debug("mods::mobs::smart_mob") << a << "\n";
#else
#define sm_debug(a) ;;
#endif
namespace mods::mobs {
	smart_mob::smart_mob() {
		this->init();
	}
	/**
	 * @brief set variation of mg.
	 *
	 * @param v valid types: "sentinel"
	 */
	void smart_mob::set_variation(std::string v) {
		this->variation = v;
		if(v.compare("sentinel") == 0) {
			auto row = db_get_by_meta("smart_mob_sentinel","mgs_mob_vnum",std::to_string(this->cd()->nr));
			if(row.size() == 0) {
				sm_debug("[smart_mob][set_variation]-> cannot load data from postgres...");
				return;
			}
#define MG_REPORT(A)\
	sm_debug("[[[[ MINI GUNNER SENTINEL DUMP ]]]]" << \
	#A << ": '" << row[0][#A].c_str() << "'" << \
	"[[[[ -- MINI GUNNER SENTINEL DUMP -- ]]]]");

			sm_debug("[status][smart_mob][setting variation data]->");
			MG_REPORT(mgs_face_direction);
			MG_REPORT(mgs_room_vnum);
			MG_REPORT(mgs_mob_vnum);
#undef MG_REPORT

			this->set_heading(mods::globals::dir_int(row[0]["mgs_face_direction"].c_str()[0]));
			char_to_room(this->cd(),real_room(row[0]["mgs_room_vnum"].as<int>()));
		}
	}
	/**
	 * @brief wear a piece of eq
	 *
	 * @param where
	 * @param yaml
	 */
	void smart_mob::wear(uint8_t where,std::string_view yaml) {
		std::tuple<int,std::string> yaml_tuple = mods::util::extract_yaml_info_from_path(yaml);
		if(std::get<0>(yaml_tuple) < 0) {
			return;
		}
		if(!mods::util::yaml_file_exists(yaml.data())) {
			return;
		}
		auto obj = create_object(std::get<0>(yaml_tuple),std::get<1>(yaml_tuple));
		this->player_ptr->equip(obj,where);
	}
	int8_t smart_mob::wear_all() {
		int8_t items = 0;
		for(auto i = 0; i < wear_list.size(); i++) {
			if(wear_list[i].length() == 0) {
				continue;
			}
			this->wear(i,wear_list[i]);
			items++;
		}
		return items;
	}
	void smart_mob::populate_from_meqbuild_profile() {
		this->wear_list = std::move(mods::mob_equipment::fetch_list_by_mob_vnum(this->cd()->nr));
	}
	void smart_mob::bootstrap_equipment() {
		this->populate_from_meqbuild_profile();
		this->wear_all();
	}
	/**
	 * @brief preferred constructor method
	 *
	 * @param mob_uuid
	 * @param variation
	 */
	smart_mob::smart_mob(uuid_t mob_uuid, std::string variation) {
		this->init();
		this->uuid = mob_uuid;
		auto p = ptr_by_uuid(mob_uuid);
		if(!p) {
			log("SYSERR: did not find player to populate smart_mob with: %d",mob_uuid);
			this->loaded = false;
			this->error = true;
			return;
		}
		this->player_ptr = p;
		auto ch = p->cd();
		ch->mob_specials.extended_mob_type = mob_special_data::extended_mob_type_t::LOWLY_SECURITY;
		this->set_behaviour_tree("smart_mob_roam");
		/** TODO: wear all equipment as per the list setup in meqbuild command */
		this->setup_damage_callbacks();
		this->loaded = true;
		this->error = false;
		this->set_variation(variation);
		this->populate_from_meqbuild_profile();
		this->wear_all();
	}
	smart_mob::~smart_mob() {
		this->uuid = 0;
	}
	/**
	 * @brief initialize
	 */
	void smart_mob::init() {
		this->uuid = 0;
		this->loaded = false;
		this->weapon_heat =0;
		this->player_ptr = nullptr;
		this->heading = NORTH;
		this->spray_direction = NORTH;
	};
	/**
	 * @brief set heading
	 *
	 * @param dir
	 */
	void smart_mob::set_heading(uint8_t dir) {
		this->heading = this->cd()->mob_specials.heading = dir;
	}
	/**
	 * @brief yell. stub
	 *
	 * @param msg
	 */
	void smart_mob::shout(std::string_view msg) {
		sm_debug("[stub]shout:'" << msg.data() << "'");
		act(CAT("$n shouts '",msg.data(),"'").c_str(), TRUE, this->cd(), 0, 0, TO_ROOM);
	}
	/**
	 * @brief set behaviour tree
	 *
	 * @param name
	 */
	void smart_mob::set_behaviour_tree(std::string_view name) {
		sm_debug("Setting behaviour tree to: '" << name << "'");
		this->cd()->mob_specials.set_behaviour_tree(name);
	}
	/**
	 * @brief callback when someone spotted
	 *
	 * @param room
	 * @param player
	 */
	void smart_mob::enemy_spotted(room_rnum room,uuid_t player) {
		sm_debug("##################################################################################" <<
		         "[smart_mob] enemy spotted:" << room << "\n" <<
		         "##################################################################################");
		//this->spray(this->player_ptr->get_watching());
		this->last_seen[player] = CURRENT_TICK();
	}
	/**
	 * @brief set hunting uuid_t list
	 *
	 * @param hunting
	 */
	void smart_mob::set_hunting(const uuidvec_t& hunting) {
		this->hunting = hunting;
	}
	/**
	 * @brief get hunting uuid's
	 *
	 * @return
	 */
	uuidvec_t& smart_mob::get_hunting() {
		return this->hunting;
	}
	/**
	 * @brief get last seen
	 *
	 * @param player
	 *
	 * @return
	 */
	tick_t smart_mob::get_last_seen(uuid_t player) {
		return this->last_seen[player];
	}
	/**
	 * @brief get the last tiem in ticks when we saw that player
	 *
	 * @param player
	 *
	 * @return
	 */
	tick_t smart_mob::get_last_seen_diff(uuid_t player) {
		auto t = this->last_seen[player];
		return t - CURRENT_TICK();
	}
	/**
	 * @brief same as mob forget
	 *
	 * @param player
	 */
	void smart_mob::forget(uuid_t player) {
		this->last_seen.erase(player);
		mods::util::vector_erase(this->hunting,player);
	}
	/**
	 * @brief char data ptr
	 *
	 * @return
	 */
	char_data* smart_mob::cd() {
		return this->player_ptr->cd();
	}
	/**
	 * @brief which room
	 *
	 * @return
	 */
	room_rnum smart_mob::room() {
		return IN_ROOM(this->cd());
	}
	void smart_mob::watch_directions(vec_t<uint8_t> directions) {
		mods::mobs::helpers::watch_multiple(directions,this->cd(),LOWLY_SECURITY_SCAN_DEPTH());
	}
	void smart_mob::watch_heading() {
		this->watch(this->heading);
	}
	void smart_mob::save_targets(vec_t<uuid_t>& t) {
		this->targeting = t;
	}
	void smart_mob::save_hostile_targets(vec_t<uuid_t>& t) {
		this->hostile_targets = t;
	}
	void smart_mob::watch_nothing() {
		mods::mobs::helpers::clear_watching(this->uuid);
	}
	/**
	 * @brief watch acertain dir
	 *
	 * @param direction
	 */
	void smart_mob::watch(uint8_t direction) {
		this->watching = direction;
		sm_debug("[smart_mob] watching:" << dirstr(direction) << "uuid:" << this->uuid);
		mods::mobs::helpers::watch(direction,this->cd(),LOWLY_SECURITY_SCAN_DEPTH());
	}
	obj_ptr_t smart_mob::primary() {
		return this->player_ptr->primary();
	}
	const vec_t<uuid_t> smart_mob::get_hostile_targets() const {
		return hostile_targets;
	}
	void smart_mob::hunt_hostile_targets() {
		hunting = hostile_targets;
	}
	void smart_mob::setup_damage_callbacks() {

	}
};
#if 0
#endif