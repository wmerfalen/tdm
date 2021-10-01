#include "contagion.hpp"
#include "../interpreter.hpp"
#include "../corpse.hpp"
#include "../object-utils.hpp"
#include "../calc-visibility.hpp"
#include "../orm/inventory.hpp"

namespace mods::classes {
	void contagion_event_over(const std::tuple<uuid_t,uint32_t>& s) {
		switch(std::get<1>(s)) {
			case deferred::EVENT_CONTAGION_MINOR_SHIELDING_OVER: {
					auto player = ptr_by_uuid(std::get<0>(s));
					if(!player) {
						std::cerr << "shielding over, but cannot find player by uuid: " << std::get<0>(s) << "\n";
						return;
					}
					if(!player->contagion()) {
						std::cerr << "attempted to do contagion event over on non-contagion player: " << std::get<0>(s) << "\n";
						return;
					}
					player->add_damage_nerf(-CONTAGION_MINOR_SHIELDING_DAMAGE_NERF_AMOUNT());
					player->sendln("You feel less protected as your {grn}MINOR SHIELDING{/grn} wears off...");
					break;
				}
			default:
				std::cerr << "unhandled contagion event...: " << std::get<1>(s) << "\n";
				break;
		}

	}
	contagion::contagion() {
		this->init();
	}
	contagion::contagion(player_ptr_t p) {
		this->init();
		load_by_player(p);
	}

	void contagion::init() {
		using skillset_t = ability_data_t::skillset_t;
		m_abilities = {
			{PATHOGEN_AMMUNITION,"patho","Pathogen Ammunition",skillset_t::INTELLIGENCE,&m_pathogen_ammunition},
			{GRIM_AURA,"aura","Grim Aura",skillset_t::INTELLIGENCE,&m_grim_aura},
			{MELT,"melt","Melt",skillset_t::INTELLIGENCE,&m_melt},
			{SUFFOCATE,"suff","Suffocate",skillset_t::INTELLIGENCE,&m_suffocate},
			{SHREDDED_CANTRIP,"shred","Shredded Cantrip",skillset_t::INTELLIGENCE,&m_shredded_cantrip},
			{MUSCLE_MEMORY,"mm","Muscle Memory",skillset_t::INTELLIGENCE,&m_muscle_memory},
			{HELLFIRE_CIRCLE,"hfc","Hellfire Circle",skillset_t::INTELLIGENCE,&m_hellfire_circle},
			{PARTICLE_DECELERATION,"pardec","Particle Deceleration",skillset_t::INTELLIGENCE,&m_particle_deceleration},
			{GHASTLY_DOUBLE,"gd","Ghastly Double",skillset_t::INTELLIGENCE,&m_ghastly_double},
			{MINOR_SHIELDING,"ms","Minor Shielding",skillset_t::INTELLIGENCE,&m_minor_shielding},
			{CURSED_BALLISTICS,"curb","Cursed Ballistics",skillset_t::INTELLIGENCE,&m_cursed_ballistics},
			{NEUTRON_SHIELD,"ns","Neutron Shield",skillset_t::INTELLIGENCE,&m_neutron_shield},
			{BLADED_ARRAY,"ba","Bladed Array",skillset_t::INTELLIGENCE,&m_bladed_array},
			{ROOTS_OF_MAYHEM,"rom","Roots of Mayhem",skillset_t::INTELLIGENCE,&m_roots_of_mayhem},
			{MORBID_DOUBT,"mord","Morbid Doubt",skillset_t::INTELLIGENCE,&m_morbid_doubt},
			{INTIMIDATE,"int","Intimidate",skillset_t::INTELLIGENCE,&m_intimidate},
			{FORCE_OUT,"force","Force Out",skillset_t::INTELLIGENCE,&m_force_out},
			{LEECH,"leech","Leech",skillset_t::INTELLIGENCE,&m_leech},
			{PARASITIC_CORPSE_TAP,"pctap","Parasitic Corpse Tap",skillset_t::INTELLIGENCE,&m_parasitic_corpse_tap},
			{EXTRACT_ORGANS,"extr","Extract Organs",skillset_t::INTELLIGENCE,&m_extract_organs},
			{RECRUIT,"rec","Recruit",skillset_t::INTELLIGENCE,&m_recruit},
			{DEMONIC_INCANTATION,"demi","Demonic Incantation",skillset_t::INTELLIGENCE,&m_demonic_incantation},
			{SHADOW_SIGHT,"ss","Shadow Sight",skillset_t::INTELLIGENCE,&m_shadow_sight},
			{MORBID_INSIGHT,"mi","Morbid Insight",skillset_t::INTELLIGENCE,&m_morbid_insight},
			{LIFE_TAP,"tap","Life Tap",skillset_t::INTELLIGENCE,&m_life_tap},
			{CORPSE_EXPLOSION,"ce","Corpse Explosion",skillset_t::DEMOLITIONS,&m_corpse_explosion},
			{SHRAPNEL_CORPSE_EXPLOSION,"sce","Shrapnel Corpse Explosion",skillset_t::DEMOLITIONS,&m_shrapnel_corpse_explosion},
			{CORROSIVE_CORPSE_EXPLOSION,"cce","Corrosive Corpse Explosion",skillset_t::DEMOLITIONS,&m_corrosive_corpse_explosion},
			{HELLFIRE_CORPSE_EXPLOSION,"hce","Hellfire Corpse Explosion",skillset_t::DEMOLITIONS,&m_hellfire_corpse_explosion},
			{ATTRACT,"at","Attract",skillset_t::STRATEGY,&m_attract},
			{CONFUSE,"cf","Confuse",skillset_t::STRATEGY,&m_confuse},
			{HELLFIRE_INCANTATION,"hell","Hellfire Incantation",skillset_t::DEMOLITIONS,&m_hellfire_incantation},
			{DETONATE_LIMB,"det","Detonate Limb",skillset_t::DEMOLITIONS,&m_detonate_limb},
			{DRAG_CORPSE,"drag","Drag Corpse",skillset_t::STRENGTH,&m_drag_corpse},
		};
	}
	void contagion::set_player(player_ptr_t p) {
		m_player = p;
	}

	int16_t contagion::new_player(player_ptr_t& player) {
		auto db_id = m_orm.initialize_row(player);
		if(db_id == 0) {
			return -2;
		}
		load_by_player(player);
		return 0;
	}
	int16_t contagion::load_by_player(player_ptr_t& player) {
		set_player(player);
		auto result = m_orm.load_by_player(player->db_id());
		if(result < 0) {
			report(CAT("contagion::load_by_player unable to load contagion class by player id: ",(player->db_id()),".. return status: ",(result)));
			return result;
		}
		initialize_skills_for_player(m_player);
		obj_ptr_t primary = nullptr;
		return result;
	}
	void contagion::replenish_notify(std::string_view v) {
		m_player->sendln(v.data());
	}
	int16_t contagion::save() {
		return m_orm.save();
	}
	std::shared_ptr<contagion> create_contagion(player_ptr_t& in_player) {
		return std::move(std::make_shared<contagion>(in_player));
	}
	std::pair<int16_t,std::string> contagion::cast_minor_shielding() {
		if(m_minor_shielding.not_learned()) {
			return {0,"It looks like you still need to train that skill"};
		}
		auto s = roll_skill_success(MINOR_SHIELDING);
		if(!std::get<0>(s)) {
			return {0,std::get<1>(s)};
		}
		uint16_t ticks = 0;
		if(m_minor_shielding.awful() || m_minor_shielding.terrible() || m_minor_shielding.okay()) {
			ticks = dice(10, 28) + 1 + (m_player->level() / 4);
		}
		if(m_minor_shielding.learned()) {
			ticks = dice(20, 28) + 3 + (m_player->level() / 4);
		}

		if(m_minor_shielding.mastered()) {
			ticks = 100 + dice(30, 28) + (m_player->level() *33);
		}
		if(m_minor_shielding.elite()) {
			ticks = 200 + dice(40, 28) + (m_player->level() *33);
		}
		m_player->sendln(CAT("Ticks: ",ticks));
		mods::globals::defer_queue->push_ticks_event(ticks, {m_player->uuid(), mods::deferred::EVENT_CONTAGION_MINOR_SHIELDING_OVER});
		m_player->add_damage_nerf(CONTAGION_MINOR_SHIELDING_DAMAGE_NERF_AMOUNT());
		m_player->hp() += CONTAGION_MINOR_SHIELDING_HP_LEVEL_MULTIPLIER() * m_player->level();

		return {1,CAT("You enable minor shielding for ",ticks," ticks")};
	}
	std::tuple<int16_t,std::string> contagion::drag_corpse(obj_ptr_t& corpse,const direction_t& direction) {
		if(m_drag_corpse.not_learned()) {
			return {0,"It looks like you still need to train that skill"};
		}
		if(m_player->mana() < DRAG_CORPSE_MANA_COST()) {
			return {0,"You don't have enough mana!"};
		}
		auto s = roll_skill_success(DRAG_CORPSE);
		if(!std::get<0>(s)) {
			return {0,std::get<1>(s)};
		}
		m_player->mana() -= DRAG_CORPSE_MANA_COST();
		uint16_t force = 0;
		if(m_drag_corpse.awful() || m_drag_corpse.terrible() || m_drag_corpse.okay()) {
			force = dice(10, 28) + 1 + (m_player->level() / 4);
		}
		if(m_drag_corpse.learned()) {
			force = dice(20, 28) + 3 + (m_player->level() / 4);
		}

		if(m_drag_corpse.mastered() || m_drag_corpse.elite()) {
			force = 100 + dice(30, 28);
		}
		return mods::corpse::drag_corpse(m_player,corpse,direction,force);
	}
	/** Contagion class abilities */
	std::pair<int16_t,std::string> contagion::cast_shrapnel_corpse_explosion(obj_ptr_t& corpse) {
		if(m_shrapnel_corpse_explosion.not_learned()) {
			return {0,"It looks like you still need to train that skill"};
		}
		if(m_player->mana() < HELLFIRE_CORPSE_EXPLOSION_MANA_COST()) {
			return {0,"You don't have enough mana!"};
		}
		auto s = roll_skill_success(HELLFIRE_CORPSE_EXPLOSION);
		if(!std::get<0>(s)) {
			return {0,std::get<1>(s)};
		}
		m_player->mana() -= HELLFIRE_CORPSE_EXPLOSION_MANA_COST();
		uint16_t damage = 0;
		if(m_shrapnel_corpse_explosion.awful() || m_shrapnel_corpse_explosion.terrible() || m_shrapnel_corpse_explosion.okay()) {
			damage = dice(10, 28) + 1 + (m_player->level() / 4);
		}
		if(m_shrapnel_corpse_explosion.learned()) {
			damage = dice(20, 28) + 3 + (m_player->level() / 4);
		}

		if(m_shrapnel_corpse_explosion.mastered() || m_shrapnel_corpse_explosion.elite()) {
			damage = 100 + dice(30, 28);
		}
		mods::corpse::queue_shrapnel_corpse_explode(corpse,m_player,damage);
		m_player->sendln(CAT("Damage: ",damage));
		return {1,"You rig a corpse to explode!"};
	}
	std::pair<int16_t,std::string> contagion::cast_hellfire_corpse_explosion(obj_ptr_t& corpse) {
		if(m_hellfire_corpse_explosion.not_learned()) {
			return {0,"It looks like you still need to train that skill"};
		}
		if(m_player->mana() < HELLFIRE_CORPSE_EXPLOSION_MANA_COST()) {
			return {0,"You don't have enough mana!"};
		}
		auto s = roll_skill_success(HELLFIRE_CORPSE_EXPLOSION);
		if(!std::get<0>(s)) {
			return {0,std::get<1>(s)};
		}
		m_player->mana() -= HELLFIRE_CORPSE_EXPLOSION_MANA_COST();
		uint16_t damage = 0;
		if(m_hellfire_corpse_explosion.awful() || m_hellfire_corpse_explosion.terrible() || m_hellfire_corpse_explosion.okay()) {
			damage = dice(10, 28) + 1 + (m_player->level() / 4);
		}
		if(m_hellfire_corpse_explosion.learned()) {
			damage = dice(20, 28) + 3 + (m_player->level() / 4);
		}

		if(m_hellfire_corpse_explosion.mastered() || m_hellfire_corpse_explosion.elite()) {
			damage = 100 + dice(30, 28);
		}
		mods::corpse::queue_hellfire_corpse_explode(corpse,m_player,damage);
		m_player->sendln(CAT("Damage: ",damage));
		return {1,"You rig a corpse to explode!"};
	}
	std::pair<int16_t,std::string> contagion::cast_corpse_explosion(obj_ptr_t& corpse) {
		if(m_corpse_explosion.not_learned()) {
			return {0,"It looks like you still need to train that skill"};
		}
		if(m_player->mana() < CORPSE_EXPLOSION_MANA_COST()) {
			return {0,"You don't have enough mana!"};
		}
		auto s = roll_skill_success(CORPSE_EXPLOSION);
		if(!std::get<0>(s)) {
			return {0,std::get<1>(s)};
		}
		m_player->mana() -= CORPSE_EXPLOSION_MANA_COST();
		uint16_t damage = 0;
		if(m_corpse_explosion.awful() || m_corpse_explosion.terrible() || m_corpse_explosion.okay()) {
			damage = dice(10, 28) + 1 + (m_player->level() / 4);
		}
		if(m_corpse_explosion.learned()) {
			damage = dice(20, 28) + 3 + (m_player->level() / 4);
		}

		if(m_corpse_explosion.mastered() || m_corpse_explosion.elite()) {
			damage = 100 + dice(30, 28);
		}
		mods::corpse::queue_corpse_explode(corpse,m_player,damage);
		m_player->sendln(CAT("Damage: ",damage));
		return {1,"You rig a corpse to explode!"};
	}
};
namespace mods::class_abilities::contagion {
	enum corpse_explosion_type_t : uint8_t {
		NORMAL_CORPSE_EXPLOSION,
		HELLFIRE_CORPSE_EXPLOSION,
		SHRAPNEL_CORPSE_EXPLOSION,
	};
	void do_corpse_explosion(player_ptr_t& player,auto argument,const corpse_explosion_type_t& type) {
		auto status = mods::corpse::pick_corpse_from_room_by_argument(player, argument);
		if(std::get<0>(status) == 0) {
			player->sendln(std::get<1>(status));
			return;
		}
		auto item = std::get<2>(status);
		switch(type) {
			default:
			case corpse_explosion_type_t::NORMAL_CORPSE_EXPLOSION: {
					auto cast_status = player->contagion()->cast_corpse_explosion(item);
					player->sendln(std::get<1>(cast_status));
					return;
				}
			case corpse_explosion_type_t::HELLFIRE_CORPSE_EXPLOSION: {
					auto cast_status = player->contagion()->cast_hellfire_corpse_explosion(item);
					player->sendln(std::get<1>(cast_status));
					return;
				}
			case corpse_explosion_type_t::SHRAPNEL_CORPSE_EXPLOSION: {
					auto cast_status = player->contagion()->cast_shrapnel_corpse_explosion(item);
					player->sendln(std::get<1>(cast_status));
					return;
				}
		}
	}
	ACMD(do_invoke) {
		if(!player->contagion()) {
			player->sendln("Invoke what? You're not a CONTAGION.");
			return;
		}
		/** This is the main entry point of all spells
		 * that the necro can cast. 'invoke' is a command
		 * that *only* a necro can call to use their spells
		 * and abilities.
		 */
		if(argshave()->first_is("corpse_explosion")->size_gt(1)->passed()) {
			do_corpse_explosion(player,argument,corpse_explosion_type_t::NORMAL_CORPSE_EXPLOSION);
			return;
		}
		if(argshave()->first_is("hellfire_corpse_explosion")->size_gt(1)->passed()) {
			do_corpse_explosion(player,argument,corpse_explosion_type_t::HELLFIRE_CORPSE_EXPLOSION);
			return;
		}
		if(argshave()->first_is("shrapnel_corpse_explosion")->size_gt(1)->passed()) {
			do_corpse_explosion(player,argument,corpse_explosion_type_t::SHRAPNEL_CORPSE_EXPLOSION);
			return;
		}
		if(argshave()->first_is("drag_corpse")->size_gt(2)->passed()) {
			auto status = mods::corpse::pick_corpse_from_room_by_argument(player, argument);
			if(std::get<0>(status) == 0) {
				player->sendln(std::get<1>(status));
				return;
			}
			auto opt_dir = mods::util::parse_direction_optional(argat(2));
			if(!opt_dir.has_value()) {
				player->sendln("Use a valid direction");
				return;
			}
			player->sendln(CAT("Direction: ",dirstr(opt_dir.value())));
			player->contagion()->drag_corpse(std::get<2>(status),opt_dir.value());
			return;
		}
		if(argshave()->first_is("minor_shielding")->size_gt(0)->passed()) {
			auto cast_status = player->contagion()->cast_minor_shielding();
			player->sendln(std::get<1>(cast_status));
			return;
		}
		if(argshave()->first_is("pathogen_ammunition")->passed()) {
			player->sendln("Pathogen ammunition!");
			return;
		}
		if(argshave()->first_is("grim_aura")->passed()) {
			player->sendln("grim aura!");
			return;
		}
		if(argshave()->first_is("melt")->size_gt(1)->passed()) {
			player->sendln("melt!");
			return;
		}
		if(argshave()->first_is("suffocate")->passed()) {
			player->sendln("suffocate!");
			return;
		}
		if(argshave()->first_is("shredded_cantrip")->size_gt(1)->passed()) {
			player->sendln("shredded_cantrip!");
			return;
		}
		if(argshave()->first_is("muscle_memory")->passed()) {
			player->sendln("muscle_memory!");
			return;
		}
		player->sendln("Invoke");
	}

	void init() {
		mods::interpreter::add_command("invoke", POS_RESTING, do_invoke, 0,0);
	}
};
