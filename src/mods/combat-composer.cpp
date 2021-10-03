#if 0
#include "combat-composer.hpp"
#include "fire.hpp"
#include "shrapnel.hpp"
#include "affects.hpp"
#include "skills.hpp"
#include "armor.hpp"
#include "levels.hpp"
#include "calc-visibility.hpp"

#include "rooms.hpp"
#include "injure.hpp"
#include "doors.hpp"
#include "super-users.hpp"
#include "interpreter.hpp"
#include "weapons/combat-types.hpp"
#include "rand.hpp"
#include "projectile.hpp"
#include "weapons/combat-calculator.hpp"
#include "weapons/elemental.hpp"

extern void act(const std::string& str, int hide_invisible, char_data *ch, obj_data *obj, void *vict_obj, int type);

#define __MENTOC_SHOW_MODS_DAMAGE_COMPOSER_DEBUG_OUTPUT__
#ifdef __MENTOC_SHOW_MODS_DAMAGE_COMPOSER_DEBUG_OUTPUT__
#define m_debug(MSG) mentoc_prefix_debug("[mods::combat_composer::debug]")  << MSG << "\n";
#define m_error(MSG) mentoc_prefix_debug(red_str("[mods::combat_composer::ERROR]"))  << MSG << "\n";
#else
#define m_debug(MSG) ;;
#define m_error(MSG) ;;
#endif

extern void object_list_new_owner(struct obj_data *list, char_data *ch);
extern void	obj_to_obj(obj_ptr_t from_object, obj_ptr_t to_object);
extern void obj_from_room(obj_ptr_t in_object);

namespace mods::combat_composer {
	struct combat_composer_explosive_t {
		uint32_t id;
		combat_composer_explosive_t() = delete;
		combat_composer_explosive_t(
		    player_ptr_t& attacker_ptr,
		    obj_ptr_t& combat_composer_ptr,
		    const uint16_t& in_base_combat,
		    std::string_view yaml) :
			base_combat(in_base_damage),
			combat_composer(damage_composer_ptr),
			charge(create_object(ITEM_EXPLOSIVE,yaml.data())),
			countdown(CORPSE_EXPLOSION_TICK_COUNTDOWN()),
			room_id(attacker_ptr->room()),
			attacker(attacker_ptr->uuid()) {

			m_debug("Corpse explosive created in room: " << room_id);
			charge->set_owner(attacker);
			id = ++combat_composer_id;
			mods::globals::defer_queue->push_ticks_event(countdown, {id, mods::deferred::EVENT_CORPSE_EXPLODE});
		}
		~combat_composer_explosive_t() = default;
		uint16_t base_combat;
		obj_ptr_t combat_composer;
		obj_ptr_t charge;
		uint16_t countdown;
		room_rnum room_id;
		uuid_t attacker;
	};
	std::tuple<bool,std::string,obj_ptr_t> pick_combat_composer_from_room_by_argument(player_ptr_t& player, std::string_view in_argument) {
		const char* argument = in_argument.substr(1).data();
		auto list = world[player->room()].contents;
		for(auto i = list; i; i = i->next_content) {
			if(!i) {
				break;
			}
			if(CAN_SEE_OBJ(player->cd(), i)) {
				auto item = optr(i);
				auto s = mods::calc_visibility::can_see_object(player,item);
				if(!std::get<0>(s)) {
					continue;
				} else {
					/** TODO: test for this syntax: 3.combat_composer */
					if(mods::util::fuzzy_match(argat(1),i->name.str()) && mods::object_utils::is_combat_composer(item)) {
						return {1,"",item};
					}
				}
			}
		}
		return {0,"Couldn't find a combat_composer",nullptr};
	}
	uint16_t get_combat_composer_weight(obj_ptr_t& damage_composer) {
		/**
		 * We really only want to catch instances of special combat_composers.
		 * For the most part, we return zero unless it's a combat_composer
		 * that belongs to a very high level NPC or boss mob.
		 */
		return 0;
	}
	std::tuple<int16_t,std::string> drag_combat_composer(player_ptr_t& dragger,obj_ptr_t& damage_composer,const direction_t& direction,const uint16_t& force) {
		if(force < get_combat_composer_weight(damage_composer)) {
			return {0,"You try to but the combat_composer just weighs too much!"};
		}
		/** 1) Pick up combat_composer */
		/** 2) block dragger for N ticks */
		/** 3) act("$n picks up $i and starts dragging it..."); */
		/** 4) act("$n takes $i <direction>..."); */
		/** 5) move direction */
		/** 6) drop combat_composer. unblock event */
		act(CAT("$n picks up a combat_composer and starts dragging it ",dirstr(direction),"...").c_str(),TRUE,dragger->cd(),0,0,TO_ROOM);
		if(world[dragger->room()].dir_option == nullptr) {
			return {0,"That direction does not exist in this room."};
		}
		if(!mods::doors::is_open(dragger->room(),direction)) {
			return {0,"You cannot go that direction. Is it open?"};
		}
		auto room = world[dragger->room()].dir_option[direction]->to_room;
		obj_from_room(combat_composer);
		/** If returns zero, it failed miserably */
		if(0 == do_simple_move(dragger->cd(),direction,0)) {
			obj_to_room(combat_composer.get(),room);
			return {0,"You failed to move."};
		}
		act(CAT("$n drags a combat_composer into the room from the ",dirstr(OPPOSITE_DIR(direction)),"...").c_str(),TRUE,dragger->cd(),0,0,TO_ROOM);
		obj_to_room(combat_composer.get(),room);
		return {1,CAT("You drag a combat_composer ",dirstr(direction))};
	}
	using combat_composer_explosions_t = std::forward_list<damage_composer_explosive_t>;
	static combat_composer_explosions_t damage_composer_explosions;

	static constexpr std::size_t BONE_SHARD_MINIMUM = 3;
	static constexpr std::size_t BONE_SHARD_MAXIMUM = 33;
	bool perform_combat_composer_blast_radius(player_ptr_t& victim,obj_ptr_t& device,uint8_t blast_count,const direction_t& from_direction) {
		feedback_t f;
		f.from_direction = from_direction;
		f.hits = rand_number(BONE_SHARD_MINIMUM,BONE_SHARD_MAXIMUM);
		f.combat = roll(device->explosive());
		if(blast_count) {
			f.combat /=  blast_count;
		}
		f.attacker = device->get_owner();
		f.combat_event = HIT_BY_TEETH_AND_BONES;
		victim->hp() -= f.combat;
		victim->combat_event(f);

		update_pos(victim->cd());
		bool dead = victim->position() == POS_DEAD;
		auto attacker = ptr_by_uuid(device->get_owner());
		if(!attacker) {
			return dead;
		}

		f.from_direction = NORTH;
		f.combat_event = YOU_INFLICTED_CORPSE_EXPLOSION_DAMAGE;
		attacker->combat_event(f);
		if(dead) {
			mods::weapons::combat_types::legacy::player_died(attacker,victim);
		}
		return dead;
	}

	bool deal_combat_composer_explosion_damage_to(player_ptr_t& victim,obj_ptr_t& device) {
		feedback_t f;
		f.from_direction = NORTH;
		f.hits = rand_number(BONE_SHARD_MINIMUM,BONE_SHARD_MAXIMUM);
		f.combat = roll(device->explosive());
		f.attacker = device->get_owner();
		f.combat_event = HIT_BY_TEETH_AND_BONES;
		victim->combat_event(f);

		if(mods::object_utils::is_hellfire_combat_composer_charge(device)) {
			mods::fire::deploy_fire_combat_to_victim_via_device(victim,device);
			mods::fire::set_fire_to_room_via_device(victim->room(),device);
		}
		if(mods::object_utils::is_shrapnel_combat_composer_charge(device)) {
			mods::shrapnel::deploy_shrapnel_combat_to(victim,device);
		}
		update_pos(victim->cd());
		bool dead = victim->position() == POS_DEAD;
		auto attacker = ptr_by_uuid(device->get_owner());
		if(!attacker) {
			return dead;
		}

		f.from_direction = NORTH;
		f.combat_event = YOU_INFLICTED_CORPSE_EXPLOSION_DAMAGE;
		attacker->combat_event(f);
		if(dead) {
			mods::weapons::combat_types::legacy::player_died(attacker,victim);
		}
		return dead;
	}
	obj_ptr_t make_combat_composer(player_ptr_t& victim) {
		char buf2[MAX_NAME_LENGTH + 64];
		obj_data *o;
		int i;

		auto combat_composer = blank_object();

		memset(buf2,0,sizeof(buf2));
		snprintf(buf2, sizeof(buf2), "The combat_composer of %s is lying here.", victim->name().c_str());
		combat_composer->description.assign(buf2);
		combat_composer->name.assign(damage_composer->description.str());

		memset(buf2,0,sizeof(buf2));
		snprintf(buf2, sizeof(buf2), "The combat_composer of %s", victim->name().c_str());
		combat_composer->short_description.assign(buf2);

		GET_OBJ_TYPE(combat_composer) = ITEM_CONTAINER;
		GET_OBJ_WEAR(combat_composer) = ITEM_WEAR_TAKE;
		GET_OBJ_EXTRA(combat_composer) = ITEM_NODONATE;
		GET_OBJ_VAL(combat_composer, 0) = 0;	/* You can't store stuff in a damage_composer */
		GET_OBJ_VAL(combat_composer, 3) = 1;	/* damage_composer identifier */
		GET_OBJ_WEIGHT(combat_composer) = victim->weight() + victim->carry_weight();
		GET_OBJ_RENT(combat_composer) = 100000;

		if(victim->is_npc()) {
			GET_OBJ_TIMER(combat_composer) = max_npc_damage_composer_time;
		} else {
			GET_OBJ_TIMER(combat_composer) = max_pc_damage_composer_time;
		}

		if(victim->is_npc()) {
			/* transfer character's inventory to the combat_composer */
			combat_composer->contains = victim->carrying();
		}

		for(o = combat_composer->contains; o != NULL; o = o->next_content) {
			o->in_obj = combat_composer.get();	/** FIXME mods::weapons::damage_types::legacy */
		}

		object_list_new_owner(combat_composer.get(), NULL);/** FIXME mods::weapons::damage_types::legacy */

		/* transfer character's equipment to the combat_composer */
		if(victim->is_npc()) {
			for(i = 0; i < NUM_WEARS; i++) {
				auto obj = victim->equipment(i);
				if(obj) {
					victim->unequip(i);
					obj_to_obj(obj, combat_composer);
				}
			}

			victim->carry(nullptr);
			victim->carry_items() = 0;
			victim->carry_weight() = 0;
		}

		combat_composer->is_damage_composer = true;
		obj_to_room(combat_composer.get(), victim->room());
		return combat_composer;
	}

	/**
	 * combat_composer damage against a player has the following effects:
	 * 	- melee attackers take 2 turns to move instead of 1
	 * 	- chance of melee attackers to drop their weapon
	 * 	- ranged attacks do N percent less combat
	 *  - continuous combat over N seconds
	 *  - blindness over N seconds
	 */
	ACMD(do_combat_composer_me) {
		player->sendln("Creating combat_composer");
		auto obj = make_combat_composer(player);
		mods::globals::register_object(obj);
		player->sendln("There you go");
		//player->sendln(CAT("Current: ",std::distance(combat_composer_explosions.cbegin(),damage_composer_explosions.cend())));
		//queue_combat_composer_explode(damage_composer,player,damage);
		//player->sendln(CAT("After: ",std::distance(combat_composer_explosions.cbegin(),damage_composer_explosions.cend())));
	}
	void queue_hellfire_combat_composer_explode(obj_ptr_t& damage_composer,player_ptr_t& attacker,const uint16_t& damage) {
		combat_composer_explosions.push_front({attacker,damage_composer,damage,"hellfire-damage_composer-charge.yml"});
	}
	void queue_shrapnel_combat_composer_explode(obj_ptr_t& damage_composer,player_ptr_t& attacker,const uint16_t& damage) {
		combat_composer_explosions.push_front({attacker,damage_composer,damage,"shrapnel-damage_composer-charge.yml"});
	}
	void queue_combat_composer_explode(obj_ptr_t& damage_composer,player_ptr_t& attacker,const uint16_t& damage) {
		combat_composer_explosions.push_front({attacker,damage_composer,damage,"damage_composer-charge.yml"});
	}
	void explode(const uint64_t& id) {
		for(const auto& combat_composer : damage_composer_explosions) {
			if(id == combat_composer.id) {
				mods::projectile::explode(combat_composer.room_id,damage_composer.charge->uuid,damage_composer.attacker);
				obj_from_room(combat_composer.damage_composer);
				combat_composer_explosions.remove_if([id](const auto& c) -> bool { return c.id == id; });
				return;
			}
		}
	}
	void init() {
		mods::interpreter::add_user_command("combat_composer_me", do_damage_composer_me);

	}

#undef m_error
#undef m_debug
};
#endif
