#ifndef __MENTOC_MODS_PLAYER_SOURCE__
#define  __MENTOC_MODS_PLAYER_SOURCE__

#include "player.hpp"
#include "../structs.h"
#include "../utils.h"
#include "weapon.hpp"
#include "../globals.hpp"
#include "acl/color.hpp"
#include "prefs.hpp"
#include <chrono>
#include <bitset>
#include <algorithm>
#include "util.hpp"
#include "scan.hpp"
#include "date-time.hpp"
#include <chrono>
#include "object-utils.hpp"
#include "damage-event.hpp"
#include "armor/basic-protection.hpp"
#include "classes/super-user-fiddler.hpp"
#include "classes/sniper.hpp"
#include "demolitions.hpp"
#include "contract-types.hpp"
#include "player-contract-instance.hpp"
#include "contract-events.hpp"

/**
 * TODO: All these stc* functions need to be altered to accomodate
 * the new player_type_enum_t values. If output is to be muted, then
 * we can stop the output from this class and avoid the overhead of
 * calling whatever functions the send_to_char function calls to queue
 * output.
 */

#define __MENTOC_SHOW_EQUIP_DEBUG_OUTPUT__
namespace mods::stat_bonuses {
	extern void player_equip(uuid_t player_uuid,uuid_t object_uuid);
	extern void player_unequip(uuid_t player_uuid,uuid_t object_uuid);
};
namespace mods::rooms {
	extern std::string word_wrap_description(room_rnum,int width);
};
namespace mods::classes {
	extern void unblock_event(uint32_t,uuid_t);
};
namespace mods::levels {
	extern std::array<uint8_t,5> get_triads_by_class(player_class_t);
};
extern size_t vwrite_to_output(mods::descriptor_data& t, const char *format, va_list args);
extern int write_to_descriptor(socket_t desc, const char *txt);
extern void	send_to_room(room_rnum room, const char *messg, ...) __attribute__((format(printf, 2, 3)));
extern void send_to_room_except(room_rnum room, std::vector<uuid_t> except, const char *messg, ...);
extern void do_auto_exits(char_data *ch);
extern mods::player::descriptor_data_t descriptor_list;
extern mods::scan::find_results_t mods::scan::los_find(chptr hunter,chptr hunted);
extern void obj_to_room(obj_ptr_t in_object, room_rnum room);
enum histfile_type_t {
	HISTFILE_FILE = 1, HISTFILE_LMDB = 2, HISTFILE_DUAL = 3
};
static constexpr histfile_type_t HISTFILE_STRATEGY = histfile_type_t::HISTFILE_FILE;

namespace mods::orm::inventory::lmdb {
	extern void add_player_wear(uint64_t player_db_id, uint64_t object_db_id, uint8_t obj_type, uint8_t position);
	extern void remove_player_wear(uint64_t player_db_id, uint8_t position);
	extern void add_player_inventory(uint64_t player_db_id, uint64_t object_db_id, uint16_t obj_type);
	extern void remove_player_inventory(uint64_t player_db_id, uint64_t object_db_id);
};
namespace mods::orm::inventory {
	extern int16_t flush_player_by_uuid(uuid_t);
	extern int16_t feed_player_by_uuid(uuid_t);
};

namespace mods {
	using task_t = mods::contracts::contract_step::task_type_t;
	using target_t = mods::contracts::contract_step::task_target_t;

#ifdef __MENTOC_NPC_SEND_DEBUG_OUTPUT__
#define NPC_SEND_DEBUG(a){ std::cerr << "[player-output--NPC_SEND_DEBUG--][NPC:" << this->name().c_str() << "][uuid:" << this->uuid() << "]->'" << a <<"'\n"; }
#else
#define NPC_SEND_DEBUG(a)
#endif
#define MENTOC_NPC_CHECK(str) if(IS_NPC(cd())){ NPC_SEND_DEBUG(str); return; }
#define MENTOC_NPC_CHECK_0(str) if(IS_NPC(cd())){ NPC_SEND_DEBUG(str); return 0; }
	using mask_t = mods::weapon::mask_type;
	using lmdb_db = db_handle;


	void player::set_shared_ptr(player_ptr_t& self_ptr) {
#ifdef __MENTOC_SHOW_SET_SHARED_PTR_DEPRECATED__
		std::cerr << "[deprecated] set_shared_ptr\n";
#endif
		return;
		/*
			 m_self_ptr = self_ptr;
			 cd()->player_ptr = self_ptr;
			 */
	}
	void player::m_set_time() {
		//m_time = std::chrono::system_clock::now();
		m_time = ::time(nullptr);
	}

	player::player(player_type_enum_t type) {
		this->init();
		set_god_mode(false);
		set_bui_mode(false);
		set_imp_mode(false);
		m_set_time();
		switch(type) {
			case DRONE:
				player();
				break;
			default:
				player();
				break;
		}
		set_type(type);
		m_quitting = 0;
		visibility() = 1;
	}
	void player::set_type(player_type_enum_t type) {
		/**
		 * FIXME: take appropriate action on a per type basis.
		 * TODO: If it's a drone, then the appropriate drone flags need to be set
		 * TODO: if it's a mob, then it needs to have IS_NPC return true.. so, set that flag
		 * TODO: if it's a player, set the IS_NPC flag to false. Also, figure out how to handle descriptor_data
		 */
		m_type = type;
		switch(type) {
			case player_type_enum_t::PLAYER:
			case player_type_enum_t::DRONE:
				if(m_desc) {
					m_desc->set_queue_behaviour(mods::descriptor_data::queue_behaviour_enum_t::NORMAL);
				}
				break;
			case player_type_enum_t::MOB:
				SET_BIT(cd()->char_specials.saved.act, MOB_ISNPC);
			case player_type_enum_t::PLAYER_MUTED_DESCRIPTOR:
			case player_type_enum_t::MOB_MUTED_DESCRIPTOR:
			case player_type_enum_t::DRONE_MUTED_DESCRIPTOR:
				if(m_desc) {
					m_desc->set_queue_behaviour(mods::descriptor_data::queue_behaviour_enum_t::IGNORE_ALL);
				}
				break;
		}
	}
	player::player() {
		this->init();
		set_god_mode(false);
		set_bui_mode(false);
		set_imp_mode(false);
		m_set_time();
		descriptor_list.emplace_back();
		auto descriptor = descriptor_list.end()-1;
		m_shared_ptr = std::make_shared<char_data>();
		m_char_data = m_shared_ptr.get();
		descriptor->character  = this->cd();
		set_desc(descriptor);
		set_char_on_descriptor(descriptor);
		/** I don't like this class call FIXME */
		m_player_specials = std::make_shared<player_special_data>();
		m_char_data->player_specials = m_player_specials;
		/** Need a better uuid generator than this */
		/** FIXME: this is not how uuid's should be generated */
		m_char_data->uuid = mods::globals::player_uuid();
		m_char_data->next = character_list;
		character_list = m_char_data;
		/** FIXME: need to set the m_char_data->desc */
		m_page = 0;
		m_current_page = 0;
		m_do_paging = false;
		m_current_page_fragment = "";
		m_capture_output = false;
		m_executing_js = false;
		set_type(player_type_enum_t::PLAYER);
		this->set_overhead_map_width(16);
		this->set_overhead_map_height(10);
		m_quitting = 0;
		std::fill(m_misc_pref.begin(),m_misc_pref.end(),false);
		m_sync_equipment();
		visibility() = 1;
		this->clear_all_affected();
		this->clear_all_affected_plr();
	}
	player::player(mods::player* ptr) {
		this->init();
		m_incendiary_resistance_percent = ptr->m_incendiary_resistance_percent;
		m_explosive_resistance_percent = ptr->m_explosive_resistance_percent;
		m_shrapnel_resistance_percent = ptr->m_shrapnel_resistance_percent;
		m_corrosive_resistance_percent = ptr->m_corrosive_resistance_percent;
		m_cryogenic_resistance_percent = ptr->m_cryogenic_resistance_percent;
		m_radiation_resistance_percent = ptr->m_radiation_resistance_percent;
		m_emp_resistance_percent = ptr->m_emp_resistance_percent;
		m_shock_resistance_percent = ptr->m_shock_resistance_percent;
		m_anti_matter_resistance_percent = ptr->m_anti_matter_resistance_percent;
		m_incendiary_damage_percent = ptr->m_incendiary_damage_percent;
		m_explosive_damage_percent = ptr->m_explosive_damage_percent;
		m_shrapnel_damage_percent = ptr->m_shrapnel_damage_percent;
		m_corrosive_damage_percent = ptr->m_corrosive_damage_percent;
		m_cryogenic_damage_percent = ptr->m_cryogenic_damage_percent;
		m_radiation_damage_percent = ptr->m_radiation_damage_percent;
		m_emp_damage_percent = ptr->m_emp_damage_percent;
		m_shock_damage_percent = ptr->m_shock_damage_percent;
		m_anti_matter_damage_percent = ptr->m_anti_matter_damage_percent;
		m_char_data = ptr->m_char_data;
		/**TODO: should we set the queue_behaviour flags on the descriptor data items on *this? */
		std::fill(m_misc_pref.begin(),m_misc_pref.end(),false);
		m_misc_pref = ptr->m_misc_pref;
		m_shared_ptr = ptr->m_shared_ptr;
		m_page = 0;
		m_current_page = 0;
		m_do_paging = false;
		m_current_page_fragment = "";
		m_capture_output = false;
		m_executing_js = false;
		m_time = ptr->time();
		set_god_mode(ptr->god_mode());
		set_bui_mode(ptr->builder_mode());
		set_imp_mode(ptr->implementor_mode());
		/** TODO: investigate this function. I have a feeling that m_desc needs to be updated here */
		m_quitting = 0;
		m_equipment = ptr->m_equipment;
		m_sync_equipment();
		m_class = ptr->m_class;
	}
	void player::capture_output(bool capture_status) {
		m_capture_output = capture_status;
	}

	std::string_view player::get_captured_output() {
		return m_captured_output;
	}

	void player::clear_captured_output() {
		m_captured_output = "";
	}

	player::player(char_data* ch) {
		this->init();
		m_char_data = ch;
		m_executing_js = false;
		m_do_paging = false;
		m_page = 0;
		m_current_page = 0;
		m_current_page_fragment = "";
		m_set_time();
		m_quitting = 0;
		visibility() = 1;
	};
	bool player::can_snipe(player_ptr_t target) {
		return mods::scan::los_find(
		           std::make_shared<mods::player>(cd()),
		           target
		       ).found;
	}
	void player::page(int pg) {
		assert(pg >= 0);
		if(m_pages.size() == 0 || static_cast<std::size_t>(pg * mods::player::PAGE_SIZE) >= m_pages.size()) {
			return;
		}

		std::string acc = "";
		unsigned i = pg * mods::player::PAGE_SIZE;

		for(; i < m_pages.size() && i < static_cast<std::size_t>((pg + 1) * mods::player::PAGE_SIZE); i++) {
			acc += m_pages[i];
		}

		if(i < m_pages.size()) {
			acc += std::string("--[ page number ") + std::to_string(pg +1)  + "/";
			acc += std::to_string((m_pages.size() / mods::player::PAGE_SIZE) + 1) + \
			       ":: press (q) to quit or page number ]--";
		} else {
			pager_clear();
			pager_end();
			m_current_page = 0;
		}

		if(acc.length()) {
			stc(acc);
			m_current_page = pg;
		}
	}
	void player::pager_next_page() {
		if((m_current_page +1) * mods::player::PAGE_SIZE >= m_pages.size()) {
			pager_clear();
			pager_end();
			return;
		}

		page(m_current_page +1);
	}
	obj_ptr_t player::equipment(int pos) {
		if(pos >= NUM_WEARS || pos < 0) {
			return nullptr;
		}
		//this->m_sync_equipment();
		return m_equipment[pos];
	}
	void player::equip(obj_ptr_t in_object,int pos) {
		if(!m_basic_protection) {
			m_basic_protection = std::make_shared<mods::armor::basic_protection>(uuid());
			m_advanced_protection = std::make_shared<mods::armor::advanced_protection>(uuid());
			m_elite_protection = std::make_shared<mods::armor::elite_protection>(uuid());
		}
		std::cerr << green_str("mods::player::equip(obj_ptr_t,pos):") << in_object->name.c_str() << "', pos:" << pos << "\n";
		if(pos < NUM_WEARS) {
			if(pos == WEAR_WIELD || pos == WEAR_PRIMARY || pos == WEAR_SECONDARY) {
				this->m_weapon_flags = in_object->obj_flags.weapon_flags;
			}
			GET_EQ(m_char_data, pos) = in_object.get();
			in_object->worn_by = this->cd();
			in_object->worn_on = pos;
			m_equipment[pos] = in_object;
#ifdef __MENTOC_USE_DEFAULT_INVENTORY_FLUSH__
			mods::orm::inventory::lmdb::add_player_wear(this->db_id(),in_object->db_id(),in_object->type,pos);
#else
			mods::orm::inventory::flush_player_by_uuid(uuid());
#endif
#ifdef __MENTOC_PLAYER_DEBUG__
			std::cerr << "[stub][player.cpp]-> perform equip calculations\n";
#endif
			perform_equip_calculations(pos,true);
			this->m_sync_equipment();
			mods::stat_bonuses::player_equip(uuid(),in_object->uuid);
		}
	}
	void player::equip(uuid_t obj_uuid,int pos) {
#ifdef __MENTOC_SHOW_EQUIP_DEBUG_OUTPUT__
		std::cerr << "player::equip(uuid): " << obj_uuid << "\n";
#endif
		auto obj = optr_by_uuid(obj_uuid);
		if(!obj) {
			exit(3);
		}
		this->equip(obj,pos);
	}
	void player::unequip(int pos) {
		if(pos < NUM_WEARS && m_equipment[pos]) {
			mods::stat_bonuses::player_unequip(uuid(),m_equipment[pos]->uuid);
			auto item = m_equipment[pos];
			if(pos == WEAR_WIELD) {
				/** FIXME: this needs to negate the bit */
				m_weapon_flags = 0;//m_equipment[pos]->obj_flags.weapon_flags;
			}
#ifdef __MENTOC_SHOW_EQUIP_DEBUG_OUTPUT__
			std::cerr << "[stub][player.cpp]-> perform equip calculations\n";
#endif
			perform_equip_calculations(pos,false);
			m_equipment[pos]->worn_by = nullptr;
			m_equipment[pos]->worn_on = -1;
			m_equipment[pos] = nullptr;

#ifdef __MENTOC_USE_DEFAULT_INVENTORY_FLUSH__
			mods::orm::inventory::lmdb::remove_player_wear(this->db_id(),pos);
#else
			mods::orm::inventory::flush_player_by_uuid(uuid());
#endif
			this->m_sync_equipment();
		}
	}
	void player::unequip_into_inventory(int pos) {
		if(pos < NUM_WEARS && m_equipment[pos]) {
			auto item = m_equipment[pos];
			this->unequip(pos);
			this->carry(std::move(item));
		}
	}
	void player::perform_equip_calculations(int pos,bool equip) {
		/**
		 * TODO: honor basic armor protection
		 * TODO: honor advanced armor protection
		 * TODO: honor elite armor protection
		 * TODO: if item is a deep object, calculate buffs/nerfs
		 * TODO: if item has buffs to specific skills (i.e.: strength or intelligence) store them
		 */
		const auto& item = m_equipment[pos];
		/** TODO melee */
		if(item->has_rifle()) {
			aff_abils().str += (equip ? 1 : -1) * item->rifle()->attributes->stat_strength;
			aff_abils().intel += (equip ? 1 : -1) * item->rifle()->attributes->stat_intelligence;
			aff_abils().wis +=(equip ? 1 : -1) *  item->rifle()->attributes->stat_wisdom;
			aff_abils().dex += (equip ? 1 : -1) * item->rifle()->attributes->stat_dexterity;
			aff_abils().con += (equip ? 1 : -1) * item->rifle()->attributes->stat_constitution;
			aff_abils().electronics +=(equip ? 1 : -1) *  item->rifle()->attributes->stat_electronics;
			aff_abils().armor += (equip ? 1 : -1) * item->rifle()->attributes->stat_armor;
			aff_abils().marksmanship +=(equip ? 1 : -1) *  item->rifle()->attributes->stat_marksmanship;
			aff_abils().sniping +=(equip ? 1 : -1) *  item->rifle()->attributes->stat_sniping;
			aff_abils().demolitions +=(equip ? 1 : -1) *  item->rifle()->attributes->stat_demolitions;
			aff_abils().chemistry +=(equip ? 1 : -1) *  item->rifle()->attributes->stat_chemistry;
			aff_abils().weapon_handling +=(equip ? 1 : -1) *  item->rifle()->attributes->stat_weapon_handling;
			aff_abils().strategy +=(equip ? 1 : -1) *  item->rifle()->attributes->stat_strategy;
			aff_abils().medical +=(equip ? 1 : -1) *  item->rifle()->attributes->stat_medical;
			auto rifle = mods::rifle_attachments::by_uuid(item->uuid);
			if(rifle) {
				m_incendiary_damage_percent += (equip ? 1 : -1) * rifle->incendiary_damage_percent;
				m_explosive_damage_percent += (equip ? 1 : -1) * rifle->explosive_damage_percent;
				m_shrapnel_damage_percent += (equip ? 1 : -1) *  rifle->shrapnel_damage_percent;
				m_corrosive_damage_percent += (equip ? 1 : -1) * rifle->corrosive_damage_percent;
				m_cryogenic_damage_percent += (equip ? 1 : -1) * rifle->cryogenic_damage_percent;
				m_radiation_damage_percent += (equip ? 1 : -1) * rifle->radiation_damage_percent;
				m_emp_damage_percent += (equip ? 1 : -1) * rifle->emp_damage_percent;
				m_shock_damage_percent += (equip ? 1 : -1) * rifle->shock_damage_percent;
				m_anti_matter_damage_percent += (equip ? 1 : -1) * rifle->anti_matter_damage_percent;
			}
		}
		if(item->has_armor()) {
			//thac0
			switch(item->armor()->attributes->speed_profile) {
				case 0:
					/** unhindered */
					break;
				case 1:
					/** fast */
					break;
				case 2:
					/** hindered */
					break;
				case 3:
					/** slow */
					break;
				case 4:
					/** sluggish */
					break;
			}
			if(item->armor()->attributes->classification.compare("BASIC") == 0) {
				if(equip) {
					m_basic_protection->equip(pos,item->uuid);
				} else {
					m_basic_protection->unequip(pos);
				}
			}
			if(item->armor()->attributes->classification.compare("ADVANCED") == 0) {
				if(equip) {
					m_advanced_protection->equip(pos,item->uuid);
				} else {
					m_advanced_protection->unequip(pos);
				}
			}
			if(item->armor()->attributes->classification.compare("ELITE") == 0) {
				if(equip) {
					m_elite_protection->equip(pos,item->uuid);
				} else {
					m_elite_protection->unequip(pos);
				}
			}
			aff_abils().str += (equip ? 1 : -1) * item->armor()->attributes->stat_strength;
			aff_abils().intel += (equip ? 1 : -1) * item->armor()->attributes->stat_intelligence;
			aff_abils().wis +=(equip ? 1 : -1) *  item->armor()->attributes->stat_wisdom;
			aff_abils().dex += (equip ? 1 : -1) * item->armor()->attributes->stat_dexterity;
			aff_abils().con += (equip ? 1 : -1) * item->armor()->attributes->stat_constitution;
			aff_abils().electronics +=(equip ? 1 : -1) *  item->armor()->attributes->stat_electronics;
			aff_abils().armor += (equip ? 1 : -1) * item->armor()->attributes->stat_armor;
			aff_abils().marksmanship +=(equip ? 1 : -1) *  item->armor()->attributes->stat_marksmanship;
			aff_abils().sniping +=(equip ? 1 : -1) *  item->armor()->attributes->stat_sniping;
			aff_abils().demolitions +=(equip ? 1 : -1) *  item->armor()->attributes->stat_demolitions;
			aff_abils().chemistry +=(equip ? 1 : -1) *  item->armor()->attributes->stat_chemistry;
			aff_abils().weapon_handling +=(equip ? 1 : -1) *  item->armor()->attributes->stat_weapon_handling;
			aff_abils().strategy +=(equip ? 1 : -1) *  item->armor()->attributes->stat_strategy;
			aff_abils().medical +=(equip ? 1 : -1) *  item->armor()->attributes->stat_medical;
			/** TODO honor thac0 */
			/** TODO honor weight_in_lbs */
			m_incendiary_resistance_percent += (equip ? 1 : -1) * item->armor()->attributes->incendiary_resistance_percent;
			m_explosive_resistance_percent += (equip ? 1 : -1) * item->armor()->attributes->explosive_resistance_percent;
			m_shrapnel_resistance_percent += (equip ? 1 : -1) * item->armor()->attributes->shrapnel_resistance_percent;
			m_corrosive_resistance_percent += (equip ? 1 : -1) * item->armor()->attributes->corrosive_resistance_percent;
			m_cryogenic_resistance_percent += (equip ? 1 : -1) * item->armor()->attributes->cryogenic_resistance_percent;
			m_radiation_resistance_percent += (equip ? 1 : -1) * item->armor()->attributes->radiation_resistance_percent;
			m_emp_resistance_percent += (equip ? 1 : -1) * item->armor()->attributes->emp_resistance_percent;
			m_shock_resistance_percent += (equip ? 1 : -1) * item->armor()->attributes->shock_resistance_percent;
			m_anti_matter_resistance_percent += (equip ? 1 : -1) * item->armor()->attributes->anti_matter_resistance_percent;
			/**
			 * TODO: honor these armor attributes
			offensive_damage_amount
			fire_resistance_percent,
			balistic_resistance_percent,
			speed_profile,
			durability_profile,
			hp,
			classification,
			*/
		}
		if(m_equipment[pos]->obj_flags.type_flag == ITEM_ARMOR) {
			int factor = 1;
			switch(pos) {
				case WEAR_BODY:
					factor = 3;
					break;			/* 30% */

				case WEAR_HEAD:
					factor = 2;
					break;			/* 20% */

				case WEAR_LEGS:
					factor = 2;
					break;			/* 20% */

				default:
					factor = 1;
					break;			/* all others 10% */
			}
			armor() += factor * m_equipment[pos]->obj_flags.value[0];
		}
		int8_t light = 0;
		//TODO: FIXME: perform updated modern logic of the below code
		if(pos == WEAR_LIGHT && m_equipment[pos]->obj_flags.type_flag == ITEM_LIGHT) {
			if(equip) {
				if(m_equipment[pos]->obj_flags.value[2]) {	/* if light is ON */
					light = 1;
				}
			}
			if(!equip) {
				if(m_equipment[pos]->obj_flags.value[2]) {	/* if light is ON */
					light = -1;
				}
			}
			if(light != 0) {
				mods::globals::affect_room_light(room(),light);
			}
		}
		this->m_sync_equipment();

	}
	bool player::has_weapon_capability(uint8_t type) {
		auto w = rifle();

		if(w &&  w->type == type) {
			return true;
		}
		return false;
	}
	bool player::has_inventory_capability(int type) {
		return true; //FIXME:
	}
	void player::weapon_cooldown_start(uint16_t duration,weapon_set set) {
		m_weapon_cooldown[set] = duration + mods::globals::current_tick;
	}
	void player::weapon_cooldown_start(uint16_t duration,obj_ptr_t& weapon) {
		m_weapon_cooldown[weapon->uuid] = duration + mods::globals::current_tick;
	}
	bool player::weapon_cooldown_expired(weapon_set set) {
		return mods::globals::current_tick >= m_weapon_cooldown[set];
	}
	bool player::weapon_cooldown_expired(obj_ptr_t& weapon) {
		return mods::globals::current_tick >= m_weapon_cooldown[weapon->uuid];
	}
	bool player::carrying_ammo_of_type(const weapon_type_t& type) {
		if(m_char_data->carrying == 0) {
			return false;
		}
		for(auto item = m_char_data->carrying; item->next; item = item->next) {
			if(item->obj_flags.is_ammo && type == item->obj_flags.type) {
				return true;
			}
		}

		return false;
	}
	/** TODO: do this */
	void player::carry(obj_ptr_t obj) {
#define dbg(A) std::cerr << red_str("[CARRY]dbug:") << A << "\n";
		dbg("carry entry");
		if(obj == nullptr) {
			dbg("obj is nullptr, returning early!");
			m_char_data->m_carrying.clear();
			m_char_data->carrying = nullptr;
			return;
		}
		dbg("setting next_content");
		obj->next_content = carrying();
		obj->carried_by = cd();
		IN_ROOM(obj) = NOWHERE;
		carry_weight() += GET_OBJ_WEIGHT(obj.get());
		carry_items()++;
		m_char_data->m_carrying.emplace_back(obj);
		m_char_data->carrying = obj.get();
		dbg("checking rifle has attachment");
		dbg("short circuited");
#ifdef __MENTOC_USE_DEFAULT_INVENTORY_FLUSH__
		dbg("adding...via lmdb");
		mods::orm::inventory::lmdb::add_player_inventory(this->db_id(), obj->db_id(), obj->type);
#else
		mods::orm::inventory::flush_player_by_uuid(uuid());
		dbg("flushing by uuid");
#endif
#undef dbg
	}
	void player::uncarry(obj_ptr_t obj) {
#define dbg(A) std::cerr << red_str("dbug:") << A << "\n";

		dbg("entrance");
		dbg("trimming rifle attachments");
		auto obj_uuid = obj->uuid;
		if(obj->has_armor()) {
			carry_weight() -= obj->armor()->attributes->weight_in_lbs;
		}
		if(obj->has_consumable()) {
			carry_weight() -= obj->consumable()->attributes->weight_in_lbs;
		}
		for(const auto& item : this->m_equipment) {
			if(!item) {
				continue;
			}
			dbg("checking item");
			if(item->uuid == obj_uuid) {
				dbg("found item");
				break;
			}
		}
		dbg("uuid fetched");
		obj_data* temp = 0;
		REMOVE_FROM_LIST(obj.get(), obj->carried_by->carrying,next_content);
		IS_CARRYING_W(obj->carried_by) -= GET_OBJ_WEIGHT(obj);
		IS_CARRYING_N(obj->carried_by)--;
		obj->carried_by = nullptr;
		obj->next_content = nullptr;
		auto it = std::find(m_char_data->m_carrying.begin(),m_char_data->m_carrying.end(),obj);
		if(it != m_char_data->m_carrying.end()) {
			m_char_data->m_carrying.erase(it);
			if(m_char_data->carrying == obj.get()) {
				if(m_char_data->m_carrying.size()) {
					m_char_data->carrying = m_char_data->m_carrying[0].get();
				} else {
					m_char_data->carrying = nullptr;
				}
			}
		}
		dbg("carrying erased");
#ifdef __MENTOC_USE_DEFAULT_INVENTORY_FLUSH__
		dbg("remove player inv");
		mods::orm::inventory::lmdb::remove_player_inventory(this->db_id(), obj->db_id());
#else
		dbg("flush player by uuid");
		mods::orm::inventory::flush_player_by_uuid(uuid());
#endif
#undef dbg
	}
	std::vector<obj_data*> player::vcarrying() {
		std::vector<obj_data*> list;
		if(!m_char_data) {
			return list;
		}
		for(auto item : m_char_data->m_carrying) {
			if(item) {
				list.emplace_back(item.get());
			}
		}
		return list;
	}
	obj_data* player::carrying() {
		return m_char_data->carrying;
	}
	std::string player::js_object() {
		std::string obj = std::string("{ 'name': '") + std::string(cd()->player.name) + std::string("','uuid':");
		return obj + std::to_string(cd()->uuid) + "}";
	}
	bool player::is_weapon_loaded() {
		auto w = rifle();

		if(!w) {
			return false;
		}

		return true;
	}
	bool player::has_ammo() {
		return true; //FIXME
	}
	bool player::has_thermite() {
		/** TODO: FIXME */
		return false;
	}

	std::string& player::name() {
		return m_name;
	}
	std::string player::ucname() {
		return m_ucname;
	}
	void player::set_name(std::string n) {
		cd()->player.name.assign(n);
		m_name = n;
		m_ucname = m_name;
		if(m_name[0] > 96) {
			m_ucname[0] = (char)(m_name[0] - 32);
		}
		for(std::size_t i=1; i < m_name.length(); i++) {
			if(m_name[i] > 96) {
				m_ucname[i] = m_name[i];
				continue;
			}
			if(m_name[i] > 64 && m_name[i] < 91) {
				m_ucname[i] = (char)(m_name[i] + 32);
				continue;
			}
			m_ucname[i] = m_name[i];
		}
	}
	bool player::has_class_capability(class_type type) {
		return !(std::find(m_class_capability.begin(),m_class_capability.end(),type) == m_class_capability.end());
	}

	bool player::has_equipment_tag(const std::string& tag) {
		if(!m_char_data->carrying) {
			return false;
		}

		/* TODO: find a better way to do this. this loops through all objects in the game and checks if the person carrying it is the current player */
		for(auto item = m_char_data->carrying; item->next; item = item->next) {
			if(std::string(item->name).find(std::string("[") + std::string(tag) + "]") != std::string::npos
			        &&
			        m_char_data == item->carried_by
			  ) {
				return true;
			}
		}

		return false;
	}
	void player::ammo_adjustment(int increment) {
		set_ammo(ammo() + increment);
	}
	player& player::pager_start() {
		m_do_paging = true;
		m_pages.clear();
		m_page = 0;
		return *this;
	}
	player& player::pager_end() {
		m_do_paging = false;

		if(m_current_page_fragment.length()) {
			if(m_pages.begin() == m_pages.end()) {
				m_pages.push_back(m_current_page_fragment);
			} else {
				auto it = m_pages.end() -1;
				*it += m_current_page_fragment;
			}
		}

		m_current_page_fragment = "";
		return *this;
	}
	void player::executing_js(bool true_false) {
		m_executing_js = true_false;
	}
	bool player::is_executing_js() const {
		return m_executing_js;
	}
	void player::pager_clear() {
		m_pages.clear();
	}
	obj_data* player::get_first_ammo_of_type(const weapon_type_t& type) const {
		for(auto item = m_char_data->carrying; item->next; item = item->next) {
			if(item->obj_flags.is_ammo &&
			        item->obj_flags.type == type && m_char_data == item->carried_by) {
				return item;
			}
		}

		return nullptr;
	}
	/* returns:
	 * 	the `ammo` trait of the obj_data struct after the increment has been applied.
	 * if 0 (zero) is returned, then nothing was subtraced/added from the user's
	 * ammo supply. This means the user doesn't have that ammo type
	 */
	int player::ammo_type_adjustment(int increment,const weapon_type_t& type) {
		if(!m_char_data->carrying) {
			return 0;
		}

		for(auto item = m_char_data->carrying; item->next; item = item->next) {
			if(item->obj_flags.is_ammo &&
			        item->obj_flags.type == type && m_char_data == item->carried_by) {
				item->obj_flags.ammo += increment;
				return item->obj_flags.ammo;
			}
		}
		return 0;
	}
	void player::send(const std::vector<std::string>& list) {
		for(auto& item : list) {
			if(m_do_paging) {
				queue_page_fragment(item.c_str());
			} else {
				sendln(item);
			}
		}
	}
	void player::psendln(std::string_view str) {
		MENTOC_NPC_CHECK(str.data());
		if(desc().has_prompt) {
			write_to_char("\r\n",0,0);
		}
		write_to_char(str,1,1);
		desc().has_prompt = 0;
	}
	void player::psendln(mods::string& str) {
		MENTOC_NPC_CHECK(str.c_str());
		if(desc().has_prompt) {
			write_to_char("\r\n",0,0);
		}
		write_to_char(str.view(),1,1);
		desc().has_prompt = 0;
	}
	void player::sendln(mods::string& str) {
		MENTOC_NPC_CHECK(str.str());
		if(desc().has_prompt) {
			write_to_char("\r\n",0,0);
		}
		write_to_char(str.view(), 1,0);
		desc().has_prompt = 0;
	}
	void player::sendln(std::string_view str) {
		MENTOC_NPC_CHECK(str.data());
		if(desc().has_prompt) {
			write_to_char("\r\n",0,0);
		}
		write_to_char(str, 1,0);
		desc().has_prompt = 0;
	}
	void player::stc_room(const room_rnum& rnum) {
		MENTOC_NPC_CHECK(std::to_string(rnum));
		if(rnum < 0 || std::size_t(rnum) >= world.size()) {
			return;
		}
		raw_send(world[rnum].name);
		//if(world[rnum].name) {
		//	/** note: using 1 for the plain parameter since colr eval is done on room load from postgres */
		//	std::cerr << __FILE__ << "|" << __LINE__ << "-> name:'" << world[rnum].name.c_str() << "'\n";
		//	//write_to_char( world[rnum].name.view(),1,1);
		//}
		if(builder_mode()) {
			write_to_char((std::string("[room_id:") + std::to_string(rnum) + "|number:" +
			               std::to_string(world[rnum].number) + "|zone:" +
			               std::to_string(world[rnum].zone)
			              ),1,1
			             );
		}
		desc().has_prompt = 0;
	}
	void player::stc(const char* m) {
		MENTOC_NPC_CHECK(std::string(m));
		/* FIXME: this does not scale */
		if(m_capture_output) {
			m_captured_output += m;
		}
		write_to_char(m,0,0);
		desc().has_prompt = 0;
	}
	void player::stc(const mods::string& m) {
		MENTOC_NPC_CHECK(m.str());
		write_to_char(m.view(),0,0);
		desc().has_prompt = 0;
	}
	void player::stc(std::string_view sview) {
		MENTOC_NPC_CHECK(sview.data());
		write_to_char(sview,0,0);
		desc().has_prompt = 0;
	}
	void player::stc(const std::string m) {
		MENTOC_NPC_CHECK(m);
		/* FIXME: this does not scale */
		if(m_capture_output) {
			m_captured_output += m;
		}
		write_to_char(m,0,0);
		desc().has_prompt = 0;
	}
	void player::stc(int m) {
		MENTOC_NPC_CHECK(m);
		if(m_capture_output) {
			m_captured_output += std::to_string(m);
		}

		/** note, using 1 for plain parameter */
		write_to_char(std::to_string(m).c_str(),0,1);
		desc().has_prompt = 0;
	}
	void player::raw_send(const mods::string& str) {
		MENTOC_NPC_CHECK(str.c_str());
		if(m_do_paging) {
			queue_page_fragment(str.c_str());
		} else {
			write_to_descriptor(m_desc->descriptor,str.c_str());
		}
		//desc().has_prompt = 0;
	}
	mods::string player::weapon_name() {
		return GET_EQ(m_char_data, WEAR_WIELD)->name;
	}
	rifle_data_t* player::rifle() {
		return GET_EQ(m_char_data, WEAR_WIELD)->rifle();
	}
	obj_data* player::get_ammo(const weapon_type_t& type) {
		for(auto item = m_char_data->carrying; item->next; item = item->next) {
			if(item->obj_flags.is_ammo &&
			        m_char_data == item->carried_by &&
			        type == item->obj_flags.type
			  ) {
				return item;
			}
		}

		return nullptr;
	}
	void player::set_char_on_descriptor(std::deque<descriptor_data>::iterator it) {
		it->character = this->cd();
		it->character->desc = m_desc;
		it->character->has_desc = true;
	}
	void player::init() {
		m_contract_size = 0;
		m_contract = 0;
		m_incendiary_resistance_percent = 0;
		m_explosive_resistance_percent = 0;
		m_shrapnel_resistance_percent = 0;
		m_corrosive_resistance_percent = 0;
		m_cryogenic_resistance_percent = 0;
		m_radiation_resistance_percent = 0;
		m_emp_resistance_percent = 0;
		m_shock_resistance_percent = 0;
		m_anti_matter_resistance_percent = 0;
		m_incendiary_damage_percent = 0;
		m_explosive_damage_percent = 0;
		m_shrapnel_damage_percent = 0;
		m_corrosive_damage_percent = 0;
		m_cryogenic_damage_percent = 0;
		m_radiation_damage_percent = 0;
		m_emp_damage_percent = 0;
		m_shock_damage_percent = 0;
		m_anti_matter_damage_percent = 0;
		m_db_id = 0;
		m_ada = false;
		m_hacking_row = 0;
		m_currently_hacking = 0;
		m_class = player_class_t::CLASS_UNDEFINED;
		m_blocked_until = 0;
		m_has_block_event = false;
		m_camera = nullptr;
		m_camera_viewing = false;
		m_quitting = false;
		m_histfile_on = false;
		m_histfile_fp = nullptr;
		m_weapon_type = 0;
		m_weapon_flags = 0;
		m_authenticated = false;
		set_god_mode(false);
		set_imp_mode(false);
		set_bui_mode(false);
		if(m_desc) {
			m_desc.reset();
		}
		m_name.clear();
		m_class = CLASS_UNDEFINED;
		m_class_capability = {};
		m_executing_js = false;
		if(m_char_data) {
			m_char_data = nullptr;
		}
		std::fill(m_weapon_cooldown.begin(),m_weapon_cooldown.end(),0);
		m_weapon_set = {};
		m_do_paging = m_capture_output = false;
		m_captured_output.clear();
		m_page = 0;
		m_current_page = 0;
		m_current_page_fragment.clear();
		m_pages.clear();
		//m_class_info = {};
		if(m_shared_ptr) {
			m_shared_ptr.reset();
		}
		m_lense_type = NORMAL_SIGHT;
		for(unsigned i=0; i < NUM_WEARS; i++) {
			m_equipment[i] = nullptr;
		}
		std::fill(m_misc_pref.begin(),m_misc_pref.end(),false);
		this->m_sync_equipment();
		m_class = CLASS_UNDEFINED;
		m_incendiary_resistance_percent = 0;
		m_explosive_resistance_percent = 0;
		m_shrapnel_resistance_percent = 0;
		m_corrosive_resistance_percent = 0;
		m_cryogenic_resistance_percent = 0;
		m_radiation_resistance_percent = 0;
		m_emp_resistance_percent = 0;
		m_shock_resistance_percent = 0;
		m_anti_matter_resistance_percent = 0;
	}
	void player::set_cd(char_data* ch) {
		m_char_data = ch;
	}
	player::time_type_t player::time() const {
		return m_time;
	}
	void player::set_desc(std::deque<descriptor_data>::iterator it) {
		m_desc = std::make_shared<mods::descriptor_data>(*it);
		set_type(m_type);	//This will indirectly call the set_queue_behaviour function on our new descriptor_data object
	}
	void player::set_desc(std::shared_ptr<descriptor_data> it) {
		m_desc = it;
		set_type(m_type);	//This will indirectly call the set_queue_behaviour function on our new descriptor_data object
	}
	descriptor_data& player::desc() {
		if(m_desc) {
			return *m_desc;
		} else {
			set_desc(std::make_shared<mods::descriptor_data>());
			return *m_desc;
		}
	}


	/**
	 * =========================
	 * =========================
	 * =========================
	 * Affect flag manipulation
	 * =========================
	 * =========================
	 * =========================
	 *
	 */
	void player::affect(uint64_t flag) {
		SET_BIT(AFF_FLAGS(cd()), flag);
		set_flag(mods::flags::chunk_type_t::LEGACY_AFF,flag);
	}
	//void player::affect(mods::flags::aff flag){
	//	SET_BIT(AFF_FLAGS(cd()), mods::util::aff2legacy(flag));
	//	set_flag(mods::flags::chunk_type_t::LEGACY_AFF,flag);
	//}
	void player::remove_affect(uint64_t flag) {
		REMOVE_BIT(AFF_FLAGS(cd()), flag);
		remove_flag(mods::flags::chunk_type_t::LEGACY_AFF,flag);
	}
	//void player::remove_affect(mods::flags::aff flag){
	//	REMOVE_BIT(AFF_FLAGS(cd()), mods::util::aff2legacy(flag));
	//	remove_flag(mods::flags::chunk_type_t::LEGACY_AFF,flag);
	//}
	void player::clear_all_affected() {
		m_flags[mods::flags::chunk_type_t::LEGACY_AFF] = 0;
		AFF_FLAGS(cd()) = 0;
	}
	aligned_int_t player::get_affected() {
		return AFF_FLAGS(cd());
		return get_chunk(mods::flags::chunk_type_t::LEGACY_AFF);
	}
	/**
	 * =============================
	 * =============================
	 * =============================
	 * PLR Affect flag manipulation
	 * =============================
	 * =============================
	 * =============================
	 *
	 */
	void player::affect_plr(uint64_t flag) {
		SET_BIT(PLR_FLAGS(cd()), flag);
		set_flag(mods::flags::chunk_type_t::LEGACY_PLR,flag);
	}
	//void player::affect_plr(mods::flags::plr flag){
	//	SET_BIT(PLR_FLAGS(cd()), mods::util::plr2legacy(flag));
	//	set_flag(mods::flags::chunk_type_t::LEGACY_PLR,flag);
	//}
	void player::remove_affect_plr(uint64_t flag) {
		REMOVE_BIT(PLR_FLAGS(cd()), flag);
		remove_flag(mods::flags::chunk_type_t::LEGACY_PLR,flag);
	}
	//void player::remove_affect_plr(mods::flags::plr flag){
	//	REMOVE_BIT(PLR_FLAGS(cd()), mods::util::plr2legacy(flag));
	//	remove_flag(mods::flags::chunk_type_t::LEGACY_PLR,flag);
	//}
	void player::clear_all_affected_plr() {
		PLR_FLAGS(cd()) = 0;
	}
	aligned_int_t player::get_affected_plr() {
		return PLR_FLAGS(cd());
		return get_chunk(mods::flags::chunk_type_t::LEGACY_PLR);
	}

	bool player::has_affect(uint64_t flag) {
		return has_flag(mods::flags::chunk_type_t::LEGACY_AFF,flag);
	}
	bool player::has_affect(mods::flags::aff flag) {
		return has_flag(mods::flags::chunk_type_t::LEGACY_AFF,flag);
	}
	bool player::has_affect_plr(uint64_t flag) {
		return has_flag(mods::flags::chunk_type_t::LEGACY_PLR,flag);
	}
	bool player::has_affect_plr(mods::flags::plr flag) {
		return has_flag(mods::flags::chunk_type_t::LEGACY_PLR,flag);
	}

	/**
	 * ======================
	 * ======================
	 * ======================
	 * END FLAG MANIPULATIONS
	 * ======================
	 * ======================
	 * ======================
	 *
	 */




	bool player::god_mode() const {
		return m_god_mode;
	}
	bool player::implementor_mode() const {
		return m_imp_mode;
	}
	bool player::builder_mode() const {
		return m_bui_mode;
	}
	void player::set_god_mode(bool b) {
		m_god_mode = b;
	}
	void player::set_imp_mode(bool b) {
		m_imp_mode = b;
	}
	void player::set_bui_mode(bool b) {
		m_bui_mode = b;
		if(m_bui_mode) {
			if(!builder_data) {
				builder_data = std::make_shared<builder_data_t>();
			}
		}
	}
	void player::done() {
		psendln("It has been done.");
	}
	bool player::has_builder_data() {
		if(builder_data) {
			return true;
		}
		return false;
	}
	bool player::room_pave_mode() {
		if(this->has_builder_data()) {
			return builder_data->room_pave_mode;
		}
		return false;
	}
	bool player::zone_pave_mode() {
		if(this->has_builder_data()) {
			return builder_data->zone_pave_mode;
		}
		return false;
	}
	void player::set_db_id(aligned_int_t id) {
		m_db_id = id;
	}
	aligned_int_t player::get_db_id() const {
		return m_db_id;
	}

	void player::set_password(std::string pw) {
		m_password = pw;
	}
	std::string player::get_password() {
		return m_password;
	}
	void player::set_bad_password_count(int i) {
		if(m_desc) {
			m_desc->bad_pws = i;
		} else {
			std::cerr << "warning: set_bad_password_count called but m_desc is null\n";
		}
	}
	void player::increment_bad_password_count() {
		if(m_desc) {
			++m_desc->bad_pws;
		} else {
			std::cerr << "warning: increment_bad_password_count called but m_desc is null\n";
		}
	}
	int player::get_bad_password_count() {
		if(m_desc) {
			return m_desc->bad_pws;
		} else {
			std::cerr << "warning: get_bad_password_count called but m_desc is null\n";
			return 0;
		}
	}
	void player::set_state(int state) {
		if(m_desc) {
			m_desc->connected = state;
		} else {
			std::cerr << "warning: set_state called but m_desc is null\n";
		}
	}

	int player::state() {
		if(m_desc) {
			return m_desc->connected;
		} else {
			std::cerr << "warning: state called but m_desc is null\n";
			if(cd()->desc) {
				return cd()->desc->connected;
			}
			return 0;
		}
	}
	void player::set_host(std::string host) {
		if(m_desc) {
			m_desc->host = host;
		} else {
			std::cerr << "warning: player::set_host called but m_desc is null\n";
		}
	}
	std::string player::host() const {
		if(m_desc) {
			return m_desc->host.c_str();
		} else {
			std::cerr << "warning: player::host called but m_desc is null\n";
		}
		return "unknown";
	}
	void player::deactivate_account() {
		/** TODO: what do we need to do to deactivate the account?
		 * for now, I think just setting an active flag for the player would be enough
		 */
		std::cout << "stub: player::deactivate_account\n";
	}
	void player::set_time_birth(time_t b) {
		this->cd()->player.time.birth = b;
	}
	time_t player::get_time_birth() const {
		return this->cd()->player.time.birth;
	}

	void player::set_time_logon(time_t t) {
		this->cd()->player.time.logon = t;
	}
	time_t player::get_time_login() const {
		return this->cd()->player.time.logon;
	}
	void player::set_time_played(int t) {
		this->cd()->player.time.played = t;
	}
	int player::get_time_played() const {
		return this->cd()->player.time.played;
	}

	/**  Affects */
	void player::set_socket(socket_t d) {
		this->desc().descriptor = d;
		this->cd()->desc->descriptor = d;
	}
	socket_t player::socket() {
		return this->desc().descriptor;
	}

	std::string grab_raw_histfile_seconds() {
		time_t seconds;
		seconds = time(nullptr);
		char buffer[sizeof(seconds)+1];
		bcopy(&seconds,buffer,sizeof(seconds));
		buffer[sizeof(seconds)] = 0;
		std::string data(sizeof(buffer),0);
		std::copy(buffer,buffer+sizeof(buffer), data.begin());
		return data;
	}

	enum histfile_opcode_t {
		HFO_INDEX =1,HFO_START=2,HFO_STOP=3,HFO_LOG=4
	};

	void player::start_histfile() {
		std::string lib_dir = MENTOC_CURRENT_WORKING_DIR;
		lib_dir += "/../log/";
		lib_dir += name();
		lib_dir += "-" + std::to_string(std::time(nullptr));
		lib_dir += ".log";
		m_histfile_fp = (FILE*)fopen(lib_dir.c_str(),"a+");
		if(!m_histfile_fp) {
			std::cerr << "can't open log file '" << lib_dir.c_str() << "'\n";
		}
	}
	void player::write_histfile(std::string_view line) {
		if(!m_histfile_fp) {
			return;
		}
		std::string prefix = "[";
		prefix += std::to_string(std::time(nullptr));
		prefix += "]->";
		fwrite(prefix.c_str(),sizeof(char),prefix.length(),m_histfile_fp);
		fwrite(line.data(),sizeof(char),line.length(),m_histfile_fp);
		fwrite("\n",sizeof(char),sizeof("\n"),m_histfile_fp);
		fflush(m_histfile_fp);
	}

	void player::stop_histfile() {
		if(!m_histfile_fp) {
			return;
		}
		fflush(m_histfile_fp);
		fclose(m_histfile_fp);
	}

	player::~player() {
#ifdef __MENTOC_SHOW_MODS_PLAYER__DESTRUCTOR_CALLS__
		std::cerr << "[~player]\n";
#endif
		if(m_histfile_on) {
			stop_histfile();
		}
		m_weapon_type = 0;
		m_weapon_flags = 0;
		m_authenticated = false;
		set_god_mode(false);
		set_imp_mode(false);
		set_bui_mode(false);
		if(m_desc) {
			m_desc.reset();
		}
		m_name.clear();
		m_class = CLASS_UNDEFINED;
		m_class_capability = {};
		m_executing_js = false;
		if(m_char_data) {
			m_char_data = nullptr;
		}
		std::fill(m_weapon_cooldown.begin(),m_weapon_cooldown.end(),0);
		m_weapon_set = {};
		m_do_paging = m_capture_output = false;
		m_captured_output.clear();
		m_page = 0;
		m_current_page = 0;
		m_current_page_fragment.clear();
		m_pages.clear();
		//m_class_info = {};
		if(m_shared_ptr) {
			m_shared_ptr.reset();
		}
		m_lense_type = NORMAL_SIGHT;
	}
	void player::queue_send_fragment(const char *message, ...) {
	}
	size_t player::send(const char *messg, ...) {
		MENTOC_NPC_CHECK_0(messg);
		if(desc().has_prompt) {
			write_to_char("\r\n",0,0);
		}
		if(m_do_paging) {
			static constexpr int txt_buffer_size_total = MAX_STRING_LENGTH;
			static constexpr int txt_buffer_size_allowable = txt_buffer_size_total - 12;
			std::array<char,txt_buffer_size_total> txt;
			std::fill(txt.begin(),txt.end(),0);
			const char *text_overflow = "**OVERFLOW**\r\n";
			int size = 0;
			va_list args;
			va_start(args, messg);
			size = vsnprintf(&txt[0], txt_buffer_size_allowable, messg, args);
			if(size == 0) {
				return 0;
			}
			if(size < 0) {
				size = txt_buffer_size_allowable;
				int offset = size - strlen(text_overflow);
				if(offset < 0) {
					offset = 0;
				}
				if(offset >= txt_buffer_size_total) {
					offset = txt_buffer_size_total - strlen(text_overflow);
				}
				strncpy(&txt[0] + offset, text_overflow,strlen(text_overflow)+1);
			} else {
				txt[std::min(size,txt_buffer_size_allowable)] = '\0';
			}
			va_end(args);
			queue_page_fragment(&txt[0]);
			return 0;
		}
		if(messg && *messg) {
			size_t left;
			va_list args;
			va_start(args, messg);
			left = vwrite_to_output(*(cd()->desc), messg, args);
			va_end(args);
			desc().has_prompt = 0;
			return left;
		}
		desc().has_prompt = 0;
		return 0;
	}

	size_t player::godsend(const char *messg, ...) {
		MENTOC_NPC_CHECK_0(messg);
		if(!m_god_mode) {
			return 0;
		}
		if(messg && *messg) {
			size_t left;
			va_list args;

			va_start(args, messg);
			left = vwrite_to_output(*(cd()->desc), messg, args);
			va_end(args);
			desc().has_prompt = 0;
			return left;
		}

		desc().has_prompt = 0;
		return 0;
	}


	player::affect_dissolver_t& player::get_affect_dissolver() {
		return m_affects;
	}

	void player::m_sync_equipment() {
		if(!m_char_data) {
			return;
		}
		for(unsigned i=0; i < NUM_WEARS; i++) {
			if(m_equipment[i]) {
				m_char_data->equipment[i] = m_equipment[i].get();
			} else {
				m_char_data->equipment[i] = nullptr;
			}
		}
	}
	void player::set_camera_viewing(bool b) {
		m_camera_viewing = b;
	}
	void player::set_camera(obj_data_ptr_t obj) {
		m_camera = std::make_shared<mods::camera>(this->uuid(),obj->uuid);
	}
	void player::block_for(uint16_t ticks, uint32_t unblock_event,uuid_t optional_uuid) {
		m_block_data[unblock_event] = optional_uuid;
		m_block_event = mods::globals::defer_queue->push_ticks_event(ticks, {this->uuid(),unblock_event});
		m_has_block_event = true;
		m_blocked_until = unblock_event;
	}
	void player::cancel_block() {
		if(m_has_block_event) {
			mods::globals::defer_queue->cancel_event(m_block_event);
			m_has_block_event = false;
		}
		m_blocked_until = 0;
	}
	bool player::is_blocked() {
		return m_blocked_until != 0;
	}
	void player::set_position(byte pos) {
		m_char_data->char_specials.position = pos;
	}

	void player::unblock_event(uint32_t unblock) {
		d("[player::unblock_event]:" << unblock);
		uuid_t target = 0;
		target = m_block_data[unblock];
		switch(unblock) {
			case mods::deferred::EVENT_PLAYER_GOES_VISIBLE: {
					mods::classes::unblock_event(unblock,uuid());
					break;
				}
			case mods::deferred::EVENT_PLAYER_FINISHES_FEIGN_DEATH: {
					mods::classes::unblock_event(unblock,uuid());
					break;
				}
			case mods::deferred::EVENT_PLAYER_UNBLOCK_INSTALLATION: {
					auto obj = optr_by_uuid(target);
					if(!obj) {
						std::cerr << "[WARNING] got nullptr from EVENT_PLAYER_UNBLOCK_INSTALLATION\n";
						break;
					}
					obj_to_room(obj, obj->in_room);
					mods::object_utils::set_done_installing(obj);
					if(mods::object_utils::is_claymore(obj)) {
						mods::demolitions::set_done_installing(target,this->uuid());
					}
					if(m_char_data->contract) {
						contract_install_item(target);
					}
					this->send("\r\nYou successfully deploy a %s\r\n", obj->name.c_str());
					break;
				}
			case mods::deferred::EVENT_PLAYER_UNBLOCK_BREACH: {
					auto obj = optr_by_uuid(target);
					if(!obj) {
						std::cerr << "[WARNING] got nullptr from EVENT_PLAYER_UNBLOCK_INSTALLATION\n";
						break;
					}
					mods::object_utils::set_done_breaching(obj);
					break;
				}
			case mods::deferred::EVENT_PLAYER_REVIVE_SUCCESSFUL: {
					auto revive_target = ptr_by_uuid(target);
					if(!revive_target) {
						std::cerr << "[WARNING] got nullptr from EVENT_PLAYER_REVIVE_SUCCESSFUL\n";
						break;
					}
					revive_target->hp() = mods::values::REVIVE_HP();
					revive_target->position() = POS_STANDING;
					this->send("{grn}You revive %s!{/grn}\r\n",revive_target->name().c_str());
					if(!IS_NPC(revive_target->cd())) {
						revive_target->send("%s {grn}revives you!{/grn}\r\n", this->name().c_str());
						revive_target->sendln("{grn}You dust yourself off and get to your feet!");
					}
					send_to_room_except(this->room(), {this->uuid(),revive_target->uuid()},
					                    "%s is revived by %s!",this->name().c_str(),revive_target->name().c_str()
					                   );
					break;
				}
			default:
				std::cerr << "[WARNING] unhandled unblock event: " << unblock << "\n";
				break;
		}
		m_block_data.erase(unblock);
		m_blocked_until = 0;
		m_has_block_event = false;
	}
	uint32_t player::current_block() {
		return m_blocked_until;
	}
	bool player::camera_viewing() {
		return m_camera_viewing;
	}
	int player::viewing_room() {
		if(this->camera_viewing()) {
			return m_camera->room();
		}
		return this->room();
	}
	std::shared_ptr<mods::camera> player::get_camera() const {
		return m_camera;
	}
	void player::clear_camera() {
		m_camera_viewing = false;
		m_camera = nullptr;
	}

	obj_data_ptr_t player::primary() {
		return this->equipment(WEAR_PRIMARY);
	}
	obj_data_ptr_t player::secondary() {
		return this->equipment(WEAR_SECONDARY);
	}

	void player::set_attacking_with_primary() {
		m_attacking_with = this->primary();
	}
	void player::set_attacking_with_secondary() {
		m_attacking_with = this->secondary();
	}
	bool player::has_night_vision() const {
		/** TODO */
		return false;
	}
	bool player::has_thermal_vision() const {
		/** TODO */
		return false;
	}
	obj_ptr_t player::get_ammo_for(obj_ptr_t& weapon) {
		/** TODO: use a query based interface to get any ammunition from packs, backpack, inventory, etc */
		for(auto obj : m_char_data->m_carrying) {
			if(obj->obj_flags.is_ammo && obj->obj_flags.type == weapon->rifle()->attributes->type) {
				return obj;
			}
		}
		return nullptr;
	}
	void player::consume_from_carrying(obj_ptr_t& item) {
		auto it = std::find(m_char_data->m_carrying.begin(),m_char_data->m_carrying.end(),item);
		if(it != m_char_data->m_carrying.end()) {
			m_char_data->m_carrying.erase(it);
		}
	}
	std::string player::get_type_string() {
		switch(m_type) {
			case player_type_enum_t::PLAYER:
				return "PLAYER";
			case player_type_enum_t::DRONE:
				return "DRONE";
			case player_type_enum_t::MOB:
				return "MOB";
			case player_type_enum_t::PLAYER_MUTED_DESCRIPTOR:
				return "PLAYER_MUTED_DESCRIPTOR";
			case player_type_enum_t::MOB_MUTED_DESCRIPTOR:
				return "MOB_MUTED_DESCRIPTOR";
			case player_type_enum_t::DRONE_MUTED_DESCRIPTOR:
				return "DRONE_MUTED_DESCRIPTOR";
			default:
				return "<UNKNOWN>";
				break;
		}
	}
	void player::report() {
		std::vector<std::string> msg;
#define MR(MSG,VALUE) msg.push_back(std::string(MSG) + std::string(":") + std::to_string(VALUE));
#define MRC(MSG,VALUE) msg.push_back(std::string(MSG) + std::string(":") + (char*)VALUE);
		MRC("type",this->get_type_string().c_str());
		MRC("name",this->name().c_str());
		MR("uuid",this->uuid());
		MRC("char_data*",this->cd());
		MR("IS_NPC",(bool)IS_NPC(this->cd()));
		send_to_room(room(), "{yel}[report-start]{/yel}\r\n");
		for(const auto& m : msg) {
			send_to_room(room(), CAT("Report: '",m,"'\r\n").c_str());
		}
		send_to_room(room(), CAT(name(),"'s equipment:\r\n").c_str());
		for(unsigned i =0; i < NUM_WEARS; i++) {
			if(!m_equipment[i]) {
				continue;
			}
			send_to_room(room(), CAT("Equipment[",i,",]:'",m_equipment[i]->name.c_str(),"'\r\n").c_str());
		}
		if(IS_NPC(this->cd())) {
			this->cd()->mob_specials.report(this->room());
		}
		send_to_room(room(),CAT("m_incendiary_damage_percent :",m_incendiary_damage_percent,"\r\n").c_str());
		send_to_room(room(),CAT("m_explosive_damage_percent :",m_explosive_damage_percent,"\r\n").c_str());
		send_to_room(room(),CAT("m_shrapnel_damage_percent :",m_shrapnel_damage_percent,"\r\n").c_str());
		send_to_room(room(),CAT("m_corrosive_damage_percent :",m_corrosive_damage_percent,"\r\n").c_str());
		send_to_room(room(),CAT("m_cryogenic_damage_percent :",m_cryogenic_damage_percent,"\r\n").c_str());
		send_to_room(room(),CAT("m_radiation_damage_percent :",m_radiation_damage_percent,"\r\n").c_str());
		send_to_room(room(),CAT("m_emp_damage_percent :",m_emp_damage_percent,"\r\n").c_str());
		send_to_room(room(),CAT("m_shock_damage_percent :",m_shock_damage_percent,"\r\n").c_str());
		send_to_room(room(),CAT("m_anti_matter_damage_percent :",m_anti_matter_damage_percent,"\r\n").c_str());
		send_to_room(room(),CAT("real_abils().str :",real_abils().str,"\r\n").c_str());
		send_to_room(room(),CAT("real_abils().intel :",real_abils().intel,"\r\n").c_str());
		send_to_room(room(),CAT("real_abils().wis:",real_abils().wis,"\r\n").c_str());
		send_to_room(room(),CAT("real_abils().dex :",real_abils().dex,"\r\n").c_str());
		send_to_room(room(),CAT("real_abils().con :",real_abils().con,"\r\n").c_str());
		send_to_room(room(),CAT("real_abils().electronics:",real_abils().electronics,"\r\n").c_str());
		send_to_room(room(),CAT("real_abils().armor :",real_abils().armor,"\r\n").c_str());
		send_to_room(room(),CAT("real_abils().marksmanship:",real_abils().marksmanship,"\r\n").c_str());
		send_to_room(room(),CAT("real_abils().sniping:",real_abils().sniping,"\r\n").c_str());
		send_to_room(room(),CAT("real_abils().demolitions:",real_abils().demolitions,"\r\n").c_str());
		send_to_room(room(),CAT("real_abils().chemistry:",real_abils().chemistry,"\r\n").c_str());
		send_to_room(room(),CAT("real_abils().weapon_handling :",real_abils().weapon_handling,"\r\n").c_str());
		send_to_room(room(),CAT("real_abils().strategy:",real_abils().strategy,"\r\n").c_str());
		send_to_room(room(),CAT("real_abils().medical:",real_abils().medical,"\r\n").c_str());
		send_to_room(room(),CAT("m_incendiary_resistance_percent :",m_incendiary_resistance_percent,"\r\n").c_str());
		send_to_room(room(),CAT("m_explosive_resistance_percent :",m_explosive_resistance_percent,"\r\n").c_str());
		send_to_room(room(),CAT("m_shrapnel_resistance_percent :",m_shrapnel_resistance_percent,"\r\n").c_str());
		send_to_room(room(),CAT("m_corrosive_resistance_percent :",m_corrosive_resistance_percent,"\r\n").c_str());
		send_to_room(room(),CAT("m_cryogenic_resistance_percent :",m_cryogenic_resistance_percent,"\r\n").c_str());
		send_to_room(room(),CAT("m_radiation_resistance_percent :",m_radiation_resistance_percent,"\r\n").c_str());
		send_to_room(room(),CAT("m_emp_resistance_percent :",m_emp_resistance_percent,"\r\n").c_str());
		send_to_room(room(),CAT("m_shock_resistance_percent:",m_shock_resistance_percent,"\r\n").c_str());
		send_to_room(room(),CAT("m_anti_matter_resistance_percent:",m_anti_matter_resistance_percent,"\r\n").c_str());
		send_to_room(room(), "{yel}[report-end]{/yel}\r\n\r\n");
#undef MR
#undef MRC
	}

	void player::set_skill(int skill,uint16_t proficiency) {
		m_skills[skill] = proficiency;
	}
	uint16_t player::skill(int t) {
		return m_skills[t];
	}
	void player::register_damage_event_callback(damage_event_t e,damage_event_callback_t cb) {
		m_damage_event_callbacks[e] = cb;
	}
	void player::dispatch_event(feedback_t feedback) {
		if(m_damage_event_callbacks.find(feedback.damage_event) != m_damage_event_callbacks.end()) {
			m_damage_event_callbacks[feedback.damage_event](feedback,this->uuid());
		}
	}

	void player::damage_event(feedback_t feedback) {
		this->dispatch_event(feedback);
		switch(feedback.damage_event) {
			default:
#ifdef __MENTOC_SHOW_UNHANDLED_DAMAGE_EVENTS__
				std::cerr << "[damage_event]: UNHANDLED case!->'" << e << "'\n";
#endif
				break;
			case damage_event_t::YOU_GOT_HIT_BY_INCENDIARY_AMMO:
				this->queue_up(CAT(mods::values::MSG_YOU_GOT_HIT_BY_INCENDIARY_AMMO(),"[from:",dirstr(feedback.from_direction),"]"));
				break;
			case damage_event_t::YOU_INFLICTED_INCENDIARY_AMMO:
				this->queue_up(mods::values::MSG_YOU_INFLICTED_INCENDIARY_AMMO());
				break;
			case damage_event_t::ATTACKER_NARROWLY_MISSED_YOU_EVENT:
				this->queue_up(CAT(MSG_NARROWLY_MISSED_ME(),"[from:",dirstr(feedback.from_direction),"]"));
				break;
			case damage_event_t::YOU_ARE_DISORIENTED_EVENT:
				this->queue_up(mods::values::MSG_YOU_ARE_DISORIENTED());
				break;
			case damage_event_t::YOU_DISORIENTED_SOMEONE_EVENT:
				this->queue_up(mods::values::MSG_YOU_DISORIENT_SOMEONE());
				break;
			case damage_event_t::YOU_ARE_INJURED_EVENT:
				this->queue_up(MSG_YOU_ARE_INJURED());
				break;
			case damage_event_t::TARGET_DEAD_EVENT:
				this->queue_up(MSG_YOUR_TARGET_IS_DEAD());
				if(sniper()) {
					sniper()->target_died(feedback.attacker);
				}
				break;
			case damage_event_t::YOU_MISSED_YOUR_TARGET_EVENT:
				this->queue_up(MSG_MISSED_TARGET());
				break;
			case damage_event_t::HIT_BY_MELEE_ATTACK:
				this->queue_up(CAT(MSG_HIT_BY_MELEE_ATTACK(),"[",std::to_string(feedback.damage),"]"));
				break;
			case damage_event_t::HIT_BY_BLADED_MELEE_ATTACK:
				this->queue_up(CAT(MSG_HIT_BY_BLADED_MELEE_ATTACK(),"[",std::to_string(feedback.damage),"]"));
				break;
			case damage_event_t::HIT_BY_BLUNT_MELEE_ATTACK:
				this->queue_up(CAT(MSG_HIT_BY_BLUNT_MELEE_ATTACK(),"[",std::to_string(feedback.damage),"]"));
				break;
			case damage_event_t::HIT_BY_RIFLE_ATTACK:
				this->queue_up(CAT(MSG_HIT_BY_RIFLE_ATTACK(),"[",std::to_string(feedback.damage),"][from:",dirstr(feedback.from_direction),"]"));
				break;
			case damage_event_t::HIT_BY_SPRAY_ATTACK:
				/** FIXME: damage is overall, not the specific user's damage! */
				this->queue_up(CAT(MSG_HIT_BY_SPRAY_ATTACK(),"[",std::to_string(feedback.damage),"][from:",dirstr(feedback.from_direction),"]"));
				break;
			case damage_event_t::NO_PRIMARY_WIELDED_EVENT:
				this->queue_up(MSG_NO_PRIMARY_WIELDED());
				break;
			case damage_event_t::OUT_OF_AMMO_EVENT:
				this->queue_up(MSG_OUT_OF_AMMO());
				break;
			case damage_event_t::COOLDOWN_IN_EFFECT_EVENT:
				this->queue_up(MSG_COOLDOWN_IN_EFFECT());
				break;
			case damage_event_t::COULDNT_FIND_TARGET_EVENT:
				this->queue_up(MSG_COULDNT_FIND_TARGET());
				break;
			case damage_event_t::TARGET_IN_PEACEFUL_ROOM_EVENT:
				this->queue_up(MSG_TARGET_IN_PEACEFUL_ROOM());
				break;
			case damage_event_t::YOU_GOT_HEADSHOT_BY_SPRAY_ATTACK:
				break;
			case damage_event_t::YOU_GOT_HEADSHOT_BY_RIFLE_ATTACK:
				this->queue_up(CAT(MSG_HIT_BY_HEADSHOT(),"[",std::to_string(feedback.damage),"][from:",dirstr(feedback.from_direction),"]"));
				break;
			case damage_event_t::YOU_INJURED_SOMEONE_EVENT:
				this->queue_up(MSG_YOU_INJURED_SOMEONE());
			case damage_event_t::YOU_GOT_HIT_BY_REFLECTED_MUNITIONS_EVENT:
				sendln(CAT(MSG_YOU_GOT_HIT_BY_REFLECTED_MUNTIONS(),"[",feedback.damage,"][from:",dirstr(feedback.from_direction)));
				break;
			case damage_event_t::YOU_REFLECTED_MUNITIONS_EVENT:
				sendln(CAT(MSG_YOU_REFLECTED_MUNITIONS(),"[",feedback.damage,"][from:",dirstr(feedback.from_direction)));
				break;
			case damage_event_t::YOU_INFLICTED_AR_SHRAPNEL:
				sendln(CAT(MSG_YOU_INFLICTED_AR_SHRAPNEL(),"[",feedback.damage,"]"));
				break;
			case damage_event_t::YOU_GOT_HIT_BY_AR_SHRAPNEL:
				sendln(CAT(MSG_YOU_GOT_HIT_BY_AR_SHRAPNEL(),"[",feedback.damage,"][from:",dirstr(feedback.from_direction)));
				break;
		}
	}
	/*
	void player::m_sync_basic_equipment(){
		if(!m_basic_protection){
			m_basic_protection = std::make_shared<mods::armor::basic_protection>(uuid());
		}
		m_basic_protection->sync_equipment(uuid());
	}
	*/

	player_class_t player::get_class() {
		return m_class;
	}
	void player::set_class(player_class_t c) {
#ifdef __MENTOC_PLAYER_DEBUG__
		std::cerr << "[mods::player] set class to: " << c << "\n";
#endif
		m_class = c;
		m_class_string = mods::util::player_class_to_string(c);
		m_triads = mods::levels::get_triads_by_class(m_class);
		GET_CLASS(m_char_data) = m_class;
	}
	int player::screen_width() {
		return mods::util::stoi(mods::prefs::dynamic_get("width","player",m_char_data)).value_or(80);
	}
	std::tuple<uint32_t,uint8_t> player::currently_hacking() {
		return {m_currently_hacking,m_hacking_row};
	}
	void player::set_currently_hacking(uint32_t id,uint8_t row) {
		m_currently_hacking = id;
		m_hacking_row = row;
	}
	bool player::can(std::string_view c) {
		auto class_name = mods::util::extract_until(c,'.');
		if(mods::util::is_lower_match(mods::util::player_class_to_string(get_class()),class_name)) {
			return true;
		}
		if(c.compare("heal") == 0) {
			switch(get_class()) {
				default:
					return false;
				case player_class_t::DEALER:
				case player_class_t::MALADY:
				case player_class_t::PYREXIA:
					return true;
			}
		}
		return false;
	}
	void player::set_ada_data(std::string_view key,std::string_view value) {
		m_ada_data[key.data()] = value.data();
	}
	std::map<std::string,std::string>& player::get_ada_data() {
		return m_ada_data;
	}
	std::shared_ptr<mods::classes::ghost>& player::ghost() {
		return m_ghost;
	}
	void player::set_ghost(std::shared_ptr<mods::classes::ghost> g) {
		m_ghost = g;
	}
	std::shared_ptr<mods::classes::pyrexia>& player::pyrexia() {
		return m_pyrexia;
	}
	void player::set_pyrexia(std::shared_ptr<mods::classes::pyrexia> g) {
		m_pyrexia = g;
	}
	std::shared_ptr<mods::classes::forge>& player::forge() {
		return m_forge;
	}
	void player::set_forge(std::shared_ptr<mods::classes::forge> f) {
		m_forge = f;
	}
	std::shared_ptr<mods::classes::syndrome>& player::syndrome() {
		return m_syndrome;
	}
	void player::set_syndrome(std::shared_ptr<mods::classes::syndrome> s) {
		m_syndrome = s;
	}
	char_data::visibility_t& player::visibility() {
		return m_char_data->visibility;
	}
	void player::write_to_char(std::string_view msg, bool newline,bool plain) {
		if(m_do_paging) {
			queue_page_fragment(msg.data());
			if(newline) {
				queue_page_fragment("\r\n");
			}
		} else {
			m_char_data->desc->queue_output(msg,newline,plain);
		}
	}
	mods::string player::get_class_string() const {
		return mods::util::player_class_to_string(m_class).c_str();
	}
	player::rate_limit_data_t& player::get_rate_limit_data(int action) {
		return m_rate_limits[action];
	}
	void player::rate_limit_hit(int action) {
		m_rate_limits[action].emplace_back(::time(nullptr));
	}
	player::access_level_t player::access_level() {
		return (player::access_level_t)m_char_data->player.level;
	}
	std::array<uint8_t,5> player::triads() {
		return m_triads;
	}
	void player::admin_success(std::string_view msg) {
		this->sendln(CAT("{grn}[Admin Success]:",msg.data(),"{/grn}"));
	}
	void player::admin_fail(std::string_view msg) {
		this->sendln(CAT("{red}[Admin Failure]:",msg.data(),"{/red}"));
	}
	std::shared_ptr<mods::classes::sniper>& player::sniper() {
		return m_sniper;
	}
	void player::set_sniper(std::shared_ptr<mods::classes::sniper> g) {
		m_sniper = g;
	}

	std::shared_ptr<mods::classes::marine>& player::marine() {
		return m_marine;
	}
	void player::set_marine(std::shared_ptr<mods::classes::marine> g) {
		m_marine = g;
	}

	std::shared_ptr<mods::classes::breacher>& player::breacher() {
		return m_breacher;
	}
	void player::set_breacher(std::shared_ptr<mods::classes::breacher> g) {
		m_breacher = g;
	}

	std::shared_ptr<mods::classes::engineer>& player::engineer() {
		return m_engineer;
	}
	void player::set_engineer(std::shared_ptr<mods::classes::engineer> g) {
		m_engineer = g;
	}

	std::shared_ptr<mods::classes::medic>& player::medic() {
		return m_medic;
	}
	void player::set_medic(std::shared_ptr<mods::classes::medic> g) {
		m_medic = g;
	}

	std::shared_ptr<mods::classes::support>& player::support() {
		return m_support;
	}
	void player::set_support(std::shared_ptr<mods::classes::support> g) {
		m_support = g;
	}
	uint8_t& player::level() {
		uint8_t level = mods::classes::mock_player_level(this->uuid(),m_char_data->player.level);
		m_char_data->player.level = level;
		return m_char_data->player.level;
	}
	int16_t& player::incendiary_resistance_percent() {
		return m_incendiary_resistance_percent;
	}
	int16_t& player::explosive_resistance_percent() {
		return m_explosive_resistance_percent;
	}
	int16_t& player::shrapnel_resistance_percent() {
		return m_shrapnel_resistance_percent;
	}
	int16_t& player::corrosive_resistance_percent() {
		return m_corrosive_resistance_percent;
	}
	int16_t& player::cryogenic_resistance_percent() {
		return m_cryogenic_resistance_percent;
	}
	int16_t& player::radiation_resistance_percent() {
		return m_radiation_resistance_percent;
	}
	int16_t& player::emp_resistance_percent() {
		return m_emp_resistance_percent;
	}
	int16_t& player::shock_resistance_percent() {
		return m_shock_resistance_percent;
	}
	int16_t& player::anti_matter_resistance_percent() {
		return m_anti_matter_resistance_percent;
	}
	room_vnum player::vnum() {
		return world[room()].number;
	}
	void player::set_fight_timestamp() {
		m_char_data->last_fight_timestamp = std::time(NULL);
	}
	bool player::is_npc() const {
		return IS_NPC(cd());
	}
	void player::contract_find_item(obj_ptr_t& object) {
		if(!m_contract) {
			return;
		}
		for(auto& c : contracts()) {
			if(c->is_find_item()) {
				c->find_item(object);
			}
		}
	}
	void player::contract_find_mob(player_ptr_t& mob) {
		if(!m_contract) {
			return;
		}
		for(auto& c : contracts()) {
			if(c->is_find_mob()) {
				c->find_mob(mob);
			}
		}
	}
	void player::contract_find_room(const room_rnum& room_id) {
		if(!m_contract) {
			return;
		}
		for(auto& c : contracts()) {
			if(c->is_find_room()) {
				c->find_room(room_id);
			}
		}
	}
	void player::contract_find_door(const room_rnum& room_id,const int8_t& direction) {
		if(!m_contract) {
			return;
		}
		for(auto& c : contracts()) {
			if(c->is_find_door()) {
				c->find_door(room_id,direction);
			}
		}
	}
	/** TODO: */
	void player::contract_destroy_item(obj_ptr_t& object) {
		if(!m_contract) {
			return;
		}
		for(auto& c : contracts()) {
			if(c->is_destroy_item()) {
				c->destroy_item(object);
			}
		}
	}
	/** TODO: */
	void player::contract_destroyed_door(const room_rnum& room_id,const int8_t& direction) {
		if(!m_contract) {
			return;
		}
		for(auto& c : contracts()) {
			if(c->is_destroy_door()) {
				c->destroy_door(room_id,direction);
			}
		}
	}
	void player::contract_retrieve_item(obj_ptr_t& object) {
		if(!m_contract) {
			return;
		}
		for(auto& c : contracts()) {
			if(c->is_retrieve_item()) {
				c->retrieve_item(object);
			}
		}
	}
	void player::contract_quota_item_find_increase(obj_ptr_t& item) {
		if(!m_contract) {
			return;
		}
		for(auto& c : contracts()) {
			if(c->goal() & task_t::GOAL_QUOTA && c->goal() & task_t::GOAL_FIND && c->target() == target_t::TARGET_ITEM) {
				c->quota_find_item(item);
			}
		}
	}
	/** TODO: */
	void player::contract_quota_kill_mob_increase(player_ptr_t& mob) {
		if(!m_contract) {
			return;
		}
		for(auto& c : contracts()) {
			if(c->is_quota_kill_mob()) {
				c->quota_kill_mob(mob);
			}
		}
	}
	/** TODO: */
	void player::contract_quota_destroyed_door(const room_rnum& room_id,const int8_t& direction) {
		if(!m_contract) {
			return;
		}
		for(auto& c : contracts()) {
			if(c->is_quota_destroy_door()) {
				c->quota_destroy_door(room_id,direction);
			}
		}
	}
	/** TODO: */
	void player::contract_kill_mob(player_ptr_t& mob) {
		if(!m_contract) {
			return;
		}
		for(auto& c : contracts()) {
			if(c->is_kill()) {
				c->kill(mob);
			}
		}
	}
	void player::contract_gain_entry(const room_rnum& room_id) {
		if(!m_contract) {
			return;
		}
		for(auto& c : contracts()) {
			if(c->is_gain_entry()) {
				c->gain_entry(room_id);
			}
		}
	}
	/** TODO: */
	void player::contract_talk_to(player_ptr_t& mob) {
		if(!m_contract) {
			return;
		}
		for(auto& c : contracts()) {
			if(c->is_talk_to()) {
				c->talk_to(mob);
			}
		}
	}
	void player::contract_install_item(const uuid_t& item_uuid) {
		if(!m_contract) {
			return;
		}
		for(auto& c : contracts()) {
			if(c->is_install_item()) {
				c->install_item(item_uuid);
			}
		}
	}
	void player::contract_give_item(obj_ptr_t& object,player_ptr_t& mob) {
		if(!m_contract) {
			return;
		}
		for(auto& c : contracts()) {
			if(c->is_give_item()) {
				c->give_item(object,mob);
			}
		}
	}
	void player::queue_up(std::string_view msg) {
		sendln(msg);
		//mods::players::messages::queue(db_id(),msg);
	}
	void player::update_contract_status() {
		m_contract = m_char_data->contract = m_contract_size;
	}
	bool& player::moving_to_room() {
		return m_moving_to_room;
	}
	player::contract_list_t& player::contracts() {
		return m_contracts;
	}
	void player::start_contract(const contract_vnum_t& contract_vnum) {
		m_contracts.emplace_front(std::make_shared<mods::contracts::player_contract_instance>(contract_vnum,db_id()));
		++m_contract_size;
		update_contract_status();
	}
	void player::stop_contract(const contract_vnum_t& contract_vnum) {
		m_contracts.remove_if([&](const auto& pcon_instance) -> bool {
			if(pcon_instance->vnum() == contract_vnum) {
				pcon_instance->stop_contract();
				if(m_contract_size) {
					--m_contract_size;
				}
				return true;
			}
			return false;
		});
		update_contract_status();
	}

};

#endif
