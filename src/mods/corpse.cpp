#include "corpse.hpp"
#include "affects.hpp"
#include "skills.hpp"
#include "armor.hpp"

#include "rooms.hpp"
#include "injure.hpp"
#include "doors.hpp"
#include "super-users.hpp"
#include "interpreter.hpp"
#include "weapons/damage-types.hpp"
#include "rand.hpp"
#include "projectile.hpp"
#include "weapons/damage-calculator.hpp"
#include "weapons/elemental.hpp"

#define __MENTOC_SHOW_MODS_BLEED_DEBUG_OUTPUT__
#ifdef __MENTOC_SHOW_MODS_BLEED_DEBUG_OUTPUT__
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
		    const uint16_t& in_base_damage) :
			base_damage(in_base_damage),
			corpse(corpse_ptr),
			charge(create_object(ITEM_EXPLOSIVE,"corpse-charge.yml")),
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
	using corpse_explosions_t = std::forward_list<corpse_explosive_t>;
	static corpse_explosions_t corpse_explosions;

	static constexpr std::size_t BONE_SHARD_MINIMUM = 3;
	static constexpr std::size_t BONE_SHARD_MAXIMUM = 33;
	void perform_corpse_blast_radius(player_ptr_t& victim,obj_ptr_t& device,uint8_t blast_count,const direction_t& from_direction) {
		feedback_t f;
		f.from_direction = from_direction;
		f.hits = rand_number(BONE_SHARD_MINIMUM,BONE_SHARD_MAXIMUM);
		f.damage = roll(device->explosive()) / blast_count;
		f.attacker = device->get_owner();
		f.damage_event = HIT_BY_TEETH_AND_BONES;
		victim->damage_event(f);

		auto attacker = ptr_by_uuid(device->get_owner());
		if(!attacker) {
			return;
		}
		f.from_direction = NORTH;
		f.damage_event = YOU_INFLICTED_CORPSE_EXPLOSION_DAMAGE;
		attacker->damage_event(f);
	}

	void deal_corpse_explosion_damage_to(player_ptr_t& victim,obj_ptr_t& device) {
		feedback_t f;
		f.from_direction = NORTH;
		f.hits = rand_number(BONE_SHARD_MINIMUM,BONE_SHARD_MAXIMUM);
		f.damage = roll(device->explosive());
		f.attacker = device->get_owner();
		f.damage_event = HIT_BY_TEETH_AND_BONES;
		victim->damage_event(f);

		auto attacker = ptr_by_uuid(device->get_owner());
		if(!attacker) {
			return;
		}
		f.from_direction = NORTH;
		f.damage_event = YOU_INFLICTED_CORPSE_EXPLOSION_DAMAGE;
		attacker->damage_event(f);
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
			o->in_obj = corpse.get();	/** FIXME legacy */
		}

		object_list_new_owner(corpse.get(), NULL);/** FIXME legacy */

		/* transfer character's equipment to the corpse */
		if(victim->is_npc()) {
			for(i = 0; i < NUM_WEARS; i++) {
				auto obj = victim->equipment(i);
				if(obj) {
					victim->unequip(i);
					obj_to_obj(obj, corpse);
				}
			}

			victim->carry(nullptr);
			victim->carry_items() = 0;
			victim->carry_weight() = 0;
		}

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
		player->sendln("Creating corpse");
		uint16_t damage = 250;
		auto corpse = make_corpse(player);
		player->sendln(CAT("Current: ",std::distance(corpse_explosions.cbegin(),corpse_explosions.cend())));
		queue_corpse_explode(corpse,player,damage);
		player->sendln(CAT("After: ",std::distance(corpse_explosions.cbegin(),corpse_explosions.cend())));
	}
	void queue_corpse_explode(obj_ptr_t& corpse,player_ptr_t& attacker,const uint16_t& damage) {
		corpse_explosions.push_front({attacker,corpse,damage});
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
