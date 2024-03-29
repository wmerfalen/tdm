#include "corpse.hpp"
#include "fire.hpp"
#include "shrapnel.hpp"
#include "affects.hpp"
#include "skills.hpp"
#include "armor.hpp"
#include "levels.hpp"
#include "calc-visibility.hpp"
#include "no-drop.hpp"

#include "rooms.hpp"
#include "drops.hpp"
#include "injure.hpp"
#include "doors.hpp"
#include "super-users.hpp"
#include "interpreter.hpp"
#include "weapons/damage-types.hpp"
#include "rand.hpp"
#include "projectile.hpp"
#include "weapons/damage-calculator.hpp"
#include "weapons/elemental.hpp"

extern void act(const std::string& str, int hide_invisible, char_data *ch, obj_data *obj, void *vict_obj, int type);

#define __MENTOC_SHOW_MODS_CORPSE_DEBUG_OUTPUT__
#ifdef __MENTOC_SHOW_MODS_CORPSE_DEBUG_OUTPUT__
	#define m_debug(MSG) mentoc_prefix_debug("[mods::corpse::debug]")  << MSG << "\n";
	#define m_error(MSG) mentoc_prefix_debug(red_str("[mods::corpse::ERROR]"))  << MSG << "\n";
#else
	#define m_debug(MSG) ;;
	#define m_error(MSG) ;;
#endif

extern void object_list_new_owner(struct obj_data *list, char_data *ch);
extern void	obj_to_obj(obj_ptr_t from_object, obj_ptr_t to_object);
extern void obj_from_room(obj_ptr_t in_object);

namespace mods::corpse {
	static uint32_t corpse_id = 0;
	struct corpse_explosive_t {
		uint32_t id;
		corpse_explosive_t() = delete;
		corpse_explosive_t(
		    player_ptr_t& attacker_ptr,
		    obj_ptr_t& corpse_ptr,
		    const uint16_t& in_base_damage,
		    std::string_view yaml) :
			base_damage(in_base_damage),
			corpse(corpse_ptr),
			charge(create_object(ITEM_EXPLOSIVE,yaml.data())),
			countdown(CORPSE_EXPLOSION_TICK_COUNTDOWN()),
			room_id(attacker_ptr->room()),
			attacker(attacker_ptr->uuid()) {

			m_debug("Corpse explosive created in room: " << room_id);
			charge->set_owner(attacker);
			id = ++corpse_id;
			mods::globals::defer_queue->push_ticks_event(countdown, {id, mods::deferred::EVENT_CORPSE_EXPLODE});
		}
		~corpse_explosive_t() = default;
		uint16_t base_damage;
		obj_ptr_t corpse;
		obj_ptr_t charge;
		uint16_t countdown;
		room_rnum room_id;
		uuid_t attacker;
	};
	std::tuple<bool,std::string,obj_ptr_t> pick_corpse_from_room_by_argument(player_ptr_t& player, std::string_view in_argument) {
		if(in_argument.length() == 0) {
			return {0,"Specify a corpse",nullptr};
		}
		std::string corpse;
		if(in_argument.length() > 32) {
			corpse = in_argument.substr(0,31);
		} else {
			corpse = in_argument.data();
		}
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
					/** TODO: test for this syntax: 3.corpse */
					if(mods::util::fuzzy_match(corpse,i->name.str()) && mods::object_utils::is_corpse(item)) {
						return {1,"",item};
					}
				}
			}
		}
		return {0,"Couldn't find a corpse",nullptr};
	}
	uint16_t get_corpse_weight(obj_ptr_t& corpse) {
		/**
		 * We really only want to catch instances of special corpses.
		 * For the most part, we return zero unless it's a corpse
		 * that belongs to a very high level NPC or boss mob.
		 */
		return 0;
	}
	std::tuple<int16_t,std::string> drag_corpse(player_ptr_t& dragger,obj_ptr_t& corpse,const direction_t& direction,const uint16_t& force) {
		if(force < get_corpse_weight(corpse)) {
			return {0,"You try to but the corpse just weighs too much!"};
		}
		/** 1) Pick up corpse */
		/** 2) block dragger for N ticks */
		/** 3) act("$n picks up $i and starts dragging it..."); */
		/** 4) act("$n takes $i <direction>..."); */
		/** 5) move direction */
		/** 6) drop corpse. unblock event */
		act(CAT("$n picks up a corpse and starts dragging it ",dirstr(direction),"...").c_str(),TRUE,dragger->cd(),0,0,TO_ROOM);
		if(world[dragger->room()].dir_option == nullptr) {
			return {0,"That direction does not exist in this room."};
		}
		if(!mods::doors::is_open(dragger->room(),direction)) {
			return {0,"You cannot go that direction. Is it open?"};
		}
		auto room = world[dragger->room()].dir_option[direction]->to_room;
		obj_from_room(corpse);
		/** If returns zero, it failed miserably */
		if(0 == do_simple_move(dragger->cd(),direction,0)) {
			obj_to_room(corpse.get(),room);
			return {0,"You failed to move."};
		}
		act(CAT("$n drags a corpse into the room from the ",dirstr(OPPOSITE_DIR(direction)),"...").c_str(),TRUE,dragger->cd(),0,0,TO_ROOM);
		obj_to_room(corpse.get(),room);
		return {1,CAT("You drag a corpse ",dirstr(direction))};
	}
	using corpse_explosions_t = std::forward_list<corpse_explosive_t>;
	static corpse_explosions_t corpse_explosions;

	static constexpr std::size_t BONE_SHARD_MINIMUM = 3;
	static constexpr std::size_t BONE_SHARD_MAXIMUM = 33;
	bool perform_corpse_blast_radius(player_ptr_t& victim,obj_ptr_t& device,uint8_t blast_count,const direction_t& from_direction) {
		feedback_t f;
		f.from_direction = from_direction;
		f.hits = rand_number(BONE_SHARD_MINIMUM,BONE_SHARD_MAXIMUM);
		f.damage = roll(device->explosive());
		if(blast_count) {
			f.damage /=  blast_count;
		}
		f.attacker = device->get_owner();
		f.damage_event = HIT_BY_TEETH_AND_BONES;
		victim->hp() -= f.damage;
		victim->damage_event(f);

		update_pos(victim->cd());
		bool dead = victim->position() == POS_DEAD;
		auto attacker = ptr_by_uuid(device->get_owner());
		if(!attacker) {
			return dead;
		}

		f.from_direction = NORTH;
		f.damage_event = YOU_INFLICTED_CORPSE_EXPLOSION_DAMAGE;
		attacker->damage_event(f);
		if(dead) {
			mods::weapons::damage_types::legacy::player_died(attacker,victim);
		}
		return dead;
	}

	bool deal_corpse_explosion_damage_to(player_ptr_t& victim,obj_ptr_t& device) {
		feedback_t f;
		f.from_direction = NORTH;
		f.hits = rand_number(BONE_SHARD_MINIMUM,BONE_SHARD_MAXIMUM);
		f.damage = roll(device->explosive());
		f.attacker = device->get_owner();
		f.damage_event = HIT_BY_TEETH_AND_BONES;
		victim->damage_event(f);

		if(mods::object_utils::is_hellfire_corpse_charge(device)) {
			mods::fire::deploy_fire_damage_to_victim_via_device(victim,device);
			mods::fire::set_fire_to_room_via_device(victim->room(),device);
		}
		if(mods::object_utils::is_shrapnel_corpse_charge(device)) {
			mods::shrapnel::deploy_shrapnel_damage_to(victim,device);
		}
		update_pos(victim->cd());
		bool dead = victim->position() == POS_DEAD;
		auto attacker = ptr_by_uuid(device->get_owner());
		if(!attacker) {
			return dead;
		}

		f.from_direction = NORTH;
		f.damage_event = YOU_INFLICTED_CORPSE_EXPLOSION_DAMAGE;
		attacker->damage_event(f);
		if(dead) {
			mods::weapons::damage_types::legacy::player_died(attacker,victim);
		}
		return dead;
	}
	obj_ptr_t make_corpse(player_ptr_t& victim) {
		char buf2[MAX_NAME_LENGTH + 64];
		obj_data *o;
		int i;

		auto corpse = blank_object();

		memset(buf2,0,sizeof(buf2));
		snprintf(buf2, sizeof(buf2), "The corpse of %s is lying here.", victim->name().c_str());
		corpse->description.assign(buf2);
		corpse->name.assign(corpse->description.str());

		memset(buf2,0,sizeof(buf2));
		snprintf(buf2, sizeof(buf2), "The corpse of %s", victim->name().c_str());
		corpse->short_description.assign(buf2);

		GET_OBJ_TYPE(corpse) = ITEM_CONTAINER;
		GET_OBJ_WEAR(corpse) = ITEM_WEAR_TAKE;
		GET_OBJ_EXTRA(corpse) = ITEM_NODONATE;
		GET_OBJ_VAL(corpse, 0) = 0;	/* You can't store stuff in a corpse */
		GET_OBJ_VAL(corpse, 3) = 1;	/* corpse identifier */
		GET_OBJ_WEIGHT(corpse) = victim->weight() + victim->carry_weight();
		GET_OBJ_RENT(corpse) = 100000;

		if(victim->is_npc()) {
			GET_OBJ_TIMER(corpse) = max_npc_corpse_time;
		} else {
			GET_OBJ_TIMER(corpse) = max_pc_corpse_time;
		}

		if(victim->is_npc()) {
			/* transfer character's inventory to the corpse */
			corpse->contains = victim->carrying();
		}

		for(o = corpse->contains; o != NULL; o = o->next_content) {
			o->in_obj = corpse.get();	/** FIXME mods::weapons::damage_types::legacy */
		}

		object_list_new_owner(corpse.get(), NULL);/** FIXME mods::weapons::damage_types::legacy */

		/* transfer character's equipment to the corpse */
		if(victim->is_npc() && mods::no_drop::should_drop(victim)) {
			for(i = 0; i < NUM_WEARS; i++) {
				auto obj = victim->equipment(i);
				if(obj) {
					victim->unequip(i);
					obj_to_obj(obj, corpse);
				}
			}
			mods::drops::random_drop_to(victim,corpse);

			victim->carry(nullptr);
			victim->carry_items() = 0;
			victim->carry_weight() = 0;
		}

		corpse->is_corpse = true;
		obj_to_room(corpse.get(), victim->room());
		return corpse;
	}

	/**
	 * corpse damage against a player has the following effects:
	 * 	- melee attackers take 2 turns to move instead of 1
	 * 	- chance of melee attackers to drop their weapon
	 * 	- ranged attacks do N percent less damage
	 *  - continuous damage over N seconds
	 *  - blindness over N seconds
	 */
	ACMD(do_corpse_me) {
		ADMIN_REJECT();
		player->sendln("Creating corpse");
		auto obj = make_corpse(player);
		mods::globals::register_object(obj);
		player->sendln("There you go");
		//player->sendln(CAT("Current: ",std::distance(corpse_explosions.cbegin(),corpse_explosions.cend())));
		//queue_corpse_explode(corpse,player,damage);
		//player->sendln(CAT("After: ",std::distance(corpse_explosions.cbegin(),corpse_explosions.cend())));
	}
	void queue_hellfire_corpse_explode(obj_ptr_t& corpse,player_ptr_t& attacker,const uint16_t& damage) {
		corpse_explosions.push_front({attacker,corpse,damage,"hellfire-corpse-charge.yml"});
	}
	void queue_shrapnel_corpse_explode(obj_ptr_t& corpse,player_ptr_t& attacker,const uint16_t& damage) {
		corpse_explosions.push_front({attacker,corpse,damage,"shrapnel-corpse-charge.yml"});
	}
	void queue_corpse_explode(obj_ptr_t& corpse,player_ptr_t& attacker,const uint16_t& damage) {
		corpse_explosions.push_front({attacker,corpse,damage,"corpse-charge.yml"});
	}
	void explode(const uint64_t& id) {
		for(const auto& corpse : corpse_explosions) {
			if(id == corpse.id) {
				mods::projectile::explode(corpse.room_id,corpse.charge->uuid,corpse.attacker);
				obj_from_room(corpse.corpse);
				corpse_explosions.remove_if([id](const auto& c) -> bool { return c.id == id; });
				return;
			}
		}
	}
	void init() {
		mods::interpreter::add_user_command("corpse_me", do_corpse_me);

	}

#undef m_error
#undef m_debug
};
