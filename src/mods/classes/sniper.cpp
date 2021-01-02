#include "sniper.hpp"
#include "../orm/inventory.hpp"
#include "../weapons/damage-types.hpp"
#include "../interpreter.hpp"
#include "../help.hpp"
#include "../examine.hpp"

namespace mods::classes {
	sniper::sniper() {
		this->init();
	}
	sniper::sniper(player_ptr_t p) {
		this->init();
		load_by_player(p);
	}
	player_ptr_t 	sniper::player() {
		return m_player;
	}
	void sniper::init() {
		m_target = 0;
		m_engaged = 0;
		m_xray_shot_charges = 0;
		m_player = nullptr;
		m_claymore_instances.clear();
		m_tracking_shot_charges = 0;
		m_preferences["mute-replenish"] = true;
	}
	void sniper::use_claymore(uuid_t object_uuid) {
		auto it = std::find(m_claymore_instances.begin(),m_claymore_instances.end(),object_uuid);
		if(it != m_claymore_instances.end()) {
			m_claymore_instances.erase(it);
		}
	}
	void sniper::replenish_notify(std::string_view msg) {
		if(m_preferences["mute-replenish"]) {
			return;
		}
		m_player->sendln(msg.data());
	}

	uint8_t sniper::claymore_count() const {
		return m_claymore_instances.size();
	}
	void sniper::replenish() {
		static uint16_t call_count = 0;
		++call_count;
		auto tier = tier(m_player);
		if((call_count % SNIPER_REPLENISH_PULSE() / tier) == 0) {
			if(m_claymore_instances.size() < SNIPER_CLAYMORE_MAX_COUNT() * tier) {
				auto fatal = create_object(ITEM_EXPLOSIVE,"claymore-mine.yml");
				m_claymore_instances.emplace_back(fatal->uuid);
				m_player->carry(fatal);
				replenish_notify("{grn}A sniper class claymore mine has been regenerated.{/grn}");
			}
			if(m_xray_shot_charges < SNIPER_XRAY_SHOT_MAX_COUNT() * tier) {
				replenish_notify("{grn}A sniper class X-Ray shot charge has been regenerated.{/grn}");
				++m_xray_shot_charges;
			}
			if(m_tracking_shot_charges < SNIPER_TRACKING_SHOT_MAX_COUNT() * tier) {
				replenish_notify("{grn}A sniper class Tracking Shot charge has been regenerated.{/grn}");
				++m_tracking_shot_charges;
			}
#ifdef __MENTOC_SEND_SNIPER_PLAYER_REPLENISH_DEBUG_MESSAGE__
			replenish_notify("Replenish");
#endif
		}
	}
	int16_t sniper::new_player(player_ptr_t& player) {
		auto db_id = m_orm.initialize_row(player);
		if(db_id == 0) {
			report(CAT("unable to initialize row for player (sniper::new_player) ",(player->db_id()),".. player's database id is zero!"));
			return -2;
		}
		player->set_db_id(db_id);
		load_by_player(player);
		return 0;
	}
	int16_t sniper::load_by_player(player_ptr_t& player) {
		m_player = player;
		auto result = m_orm.load_by_player(player->db_id());
		if(result < 0) {
			report(CAT("unable to load sniper class by player id: ", player->db_id(), ".. return status: ", result));
			return -100 - result;
		}
		obj_ptr_t primary = nullptr;
		/** TODO: create catchy name using the deep object parser */
		primary = create_object(ITEM_RIFLE,"psg1.yml");
		player->equip(primary,WEAR_PRIMARY);
		player->equip(create_object(ITEM_RIFLE,"czp10.yml"),WEAR_SECONDARY);
		return result;
	}
	int16_t sniper::save() {
		mods::orm::inventory::flush_player(m_player);
		return m_orm.save();
	}
	/*
	- Ability: X-Ray Shot (can shoot through multiple layers of walls/doors)
		- Can snipe an enemy within a building
		- Can snipe an enemy through walls or doors
		- If enemy not behind cover, it causes 150% damage to target
		*/
	std::tuple<bool,std::string> sniper::xray_shot() {
		auto weapon = m_player->primary();
		if(!weapon || weapon->rifle()->attributes->type != mw_rifle::SNIPER) {
			return {0,"You must be wielding a sniper rifle!"};
		}
		if(!m_engaged) {
			return {0,"You must first use the 'engage' command"};
		}
		int distance = weapon->rifle()->attributes->critical_range;
		int direction = NORTH;
		if(m_target == 0) {
			return {0,"You have not marked anyone"};
		}
		auto victim = ptr_by_uuid(m_target);
		if(!victim || victim->visibility() == 0) {
			return {0,"Cannot find your victim!"};
		}
		auto feedback = mods::weapons::damage_types::rifle_attack_with_feedback(
		                    m_player,
		                    weapon,
		                    victim,
		                    distance,
		                    direction
		                );
		return {1,"stub"};
	}
	void sniper::target_died(uuid_t target) {
		m_target = 0;
		m_engaged = 0;
		m_player->sendln("Your target died.");
	}
	std::tuple<bool,std::string> sniper::mark_target(std::string_view target) {
		m_target = mods::examine::find_player_by_name(m_player, target);
		if(m_target == 0) {
			return {0,"Couldn't find a target that matches that string."};
		}
		return {1,"Marked target"};
	}

	std::tuple<bool,std::string> sniper::engage() {
		if(m_target == 0) {
			return {0,"You have not marked a target yet!"};
		}
		auto ptr = ptr_by_uuid(m_target);
		if(!ptr) {
			m_target = 0;
			return {0,"That target doesn't exist anymore."};
		}
		m_engaged = 1;
		return {1,CAT("Engaged target: '",ptr->name().c_str(),"'")};
	}
	std::tuple<bool,std::string> sniper::disengage() {
		m_target = 0;
		m_engaged = 0;
		return {1,"You disengage with your target."};
	}

	std::tuple<bool,std::string> sniper::tracking_shot(std::string_view target, direction_t direction) {
		if(m_tracking_shot_charges) {
			int depth = 6;
			mods::scan::vec_player_data data;
			/*
			 * vec_player_data {
				*	char_data* ch;
				*	obj_data* obj;
				*	uint16_t distance;
				*	uuid_t uuid;
				*	uint8_t direction;
				*	uint32_t room_rnum;
				* }
			*/
			mods::scan::los_scan_direction(m_player->cd(),depth,&data,direction);
			for(const auto& result : data) {
				if(mods::util::fuzzy_match(target.data(),result.ch->player.name.c_str())) {
					auto victim = ptr(result.ch);
					if(!victim) {
						return {false,"You could not locate your victim! Are they dead?"};
					}
					feedback_t feedback = mods::weapons::damage_types::rifle_attack_with_feedback(
					                          m_player,
					                          m_player->primary(),
					                          victim,
					                          result.distance,
					                          direction
					                      );

					if(feedback.hits == 0) {
						return {false,"You missed your target!"};
					}

					--m_tracking_shot_charges;
					mods::affects::affect_player_for(mods::affects::to_affect({"tracked"}),ptr(result.ch),SKILL_SNIPER_TRACKING_SHOT_TICKS());
					return {1,"You tag your target!"};
				}
			}
		}
		return {0,"You couldn't tag your target!"};
	}
	std::shared_ptr<sniper> create_sniper(player_ptr_t& in_player) {
		return std::move(std::make_shared<sniper>(in_player));
	}
};

namespace mods::class_abilities::sniper {
	ACMD(do_xray_shot) {
		PLAYER_CAN("sniper.xray_shot");
		DO_HELP("xray_shot");
		auto status = player->sniper()->xray_shot();
		player->sendln(std::get<1>(status));
	};
	ACMD(do_mark_target) {
		PLAYER_CAN("sniper.mark_target");
		DO_HELP("mark_target");
		static constexpr const char* usage = "Usage: mark_target <target>";
		auto vec_args = PARSE_ARGS();
		if(vec_args.size() == 0) {
			player->sendln(usage);
			return;
		}
		auto status = player->sniper()->mark_target(vec_args[0]);
		player->sendln(std::get<1>(status));
	};
	ACMD(do_tracking_shot) {
		PLAYER_CAN("sniper.tracking_shot");
		DO_HELP("tracking_shot");
		static constexpr const char* usage = "Usage: tracking_shot <target> <direction>";
		auto vec_args = PARSE_ARGS();
		if(vec_args.size() < 2) {
			player->sendln(usage);
			return;
		}
		direction_t dir = mods::util::parse_direction(vec_args[1]);
		auto status = player->sniper()->tracking_shot(vec_args[0],dir);
		player->sendln(std::get<1>(status));
	};
	ACMD(do_engage) {
		PLAYER_CAN("sniper.engage");
		DO_HELP("engage");
		auto status = player->sniper()->engage();
		player->sendln(std::get<1>(status));
	};
	ACMD(do_disengage) {
		PLAYER_CAN("sniper.disengage");
		DO_HELP("disengage");
		auto status = player->sniper()->disengage();
		player->sendln(std::get<1>(status));
	};
	void init() {
		mods::interpreter::add_command("mark", POS_RESTING, do_mark_target, 0,0);
		mods::interpreter::add_command("mark_target", POS_RESTING, do_mark_target, 0,0);
		mods::interpreter::add_command("tracking_shot", POS_RESTING, do_tracking_shot, 0,0);
		mods::interpreter::add_command("engage", POS_RESTING, do_engage, 0,0);
		mods::interpreter::add_command("disengage", POS_RESTING, do_disengage, 0,0);
		mods::interpreter::add_command("xray_shot", POS_RESTING, do_xray_shot, 0,0);
	}
};
