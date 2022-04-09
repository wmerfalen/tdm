#include "ensnare.hpp"
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
#include "weapons/damage-calculator.hpp"
#include "weapons/elemental.hpp"

#ifdef __MENTOC_SHOW_MODS_SLOWED_DEBUG_OUTPUT__
#define m_debug(MSG) mentoc_prefix_debug("[mods::ensnare::debug]")  << MSG << "\n";
#define m_error(MSG) mentoc_prefix_debug(red_str("[mods::ensnare::ERROR]"))  << MSG << "\n";
#else
#define m_debug(MSG) ;;
#define m_error(MSG) ;;
#endif


namespace mods::ensnare {
	static uint32_t ensnare_id = 0;
	struct ensnare_damage_t {
		uint32_t id;
		uuid_t attacker;
		uuid_t target;
		int16_t damage;
		bool cleanup;
		uuid_t barbed_wire;
		inline bool operator==(const ensnare_damage_t& lhs) {
			if(lhs.id == id) {
				return true;
			}
			return false;
		}
		std::string report() {
			return CAT("id:",id,"\nattacker:",attacker,"\ntarget:",target,"\ndamage:",damage,"\n");
		}
		ensnare_damage_t() = delete;
		ensnare_damage_t(const uuid_t& atk,
		                 const uuid_t& t,
		                 const int16_t& d,
		                 const uuid_t& barbs
		                )
			: id(++ensnare_id),
			  attacker(atk), target(t),damage(d),cleanup(false),barbed_wire(barbs) {
		}
	};
	using ensnare_list_t = std::forward_list<ensnare_damage_t>;

	static ensnare_list_t& ensnare_player_list() {
		static ensnare_list_t list;
		return list;
	}

	bool is_ensnared(player_ptr_t victim) {
		return victim && victim->get_affect_dissolver().has_affect(mods::affects::affect_t::SLOWED);
	}
	void ensnare_damage(
	    player_ptr_t attacker,
	    player_ptr_t victim,
	    obj_ptr_t item,
	    int damage) {
		auto ticks = DEFAULT_ENSNARE_TICKS();
		m_debug("ticks is positive number (after calculation):" << ticks);
		victim->ensnared_amount() = INITIAL_ENSNARED_AMOUNT();
		mods::affects::affect_player_for({mods::affects::affect_t::SLOWED},victim,ticks);
		m_debug("calling queue ensnare player");
		mods::ensnare::queue_ensnare_player(item,victim,damage);
	}

	void process_players() {
		std::vector<ensnare_damage_t> player_removals;
		for(const auto& entry : ensnare_player_list()) {
			if(entry.cleanup) {
				m_debug("cleanup requested for entry: " << entry.id);
				player_removals.emplace_back(entry);
				continue;
			}
			m_debug("victim: " << entry.target);
			auto victim = ptr_by_uuid(entry.target);
			if(!victim || victim->position() == POS_DEAD || !victim->get_affect_dissolver().has_affect(mods::affects::affect_t::SLOWED)) {
				player_removals.emplace_back(entry);
				continue;
			}
			auto dam = entry.damage;
			auto attacker = ptr_by_uuid(entry.attacker);
			if(!attacker || attacker->position() == POS_DEAD) {
				player_removals.emplace_back(entry);
				continue;
			}

			if(attacker) {
				attacker->sendln(CAT("{grn}[+",dam,"] ensnare damage to ",victim->name(),"{/grn}"));
			}
			victim->sendln(CAT("{grn}[-",dam,"] ensnare damage.{/grn}"));
			victim->hp() -= dam;
			mods::weapons::damage_types::update_and_process_death(attacker,victim);
		}
		for(const auto& entry : player_removals) {
			ensnare_player_list().remove(entry);
		}
	}
	void process_ensnare() {
		m_debug("process_ensnare");
		process_players();
	}
	void ensnare_clears_up(player_ptr_t victim) {
		return;
	}
	void queue_ensnare_player(obj_ptr_t device,player_ptr_t& victim,const int16_t& damage) {
		ensnare_player_list().emplace_front(device->get_owner(),
		                                    victim->uuid(),
		                                    damage,
		                                    device->uuid
		                                   );
	}
	void unqueue_player(const uuid_t& player_uuid) {
		for(auto& entry : ensnare_player_list()) {
			if(entry.target == player_uuid) {
				entry.cleanup = true;
			}
		}
	}
	ADMINCMD(do_ensnare_me) {
		player->sendln("Creating barbed wire...");
		int damage = 250;
		auto device = create_object(ITEM_GADGET,"demonic-snares.yml");
		ensnare_damage(player,player,device,damage);
	}
	void init() {
		ADD_ADMIN_COMMAND("admin:ensnare",do_ensnare_me);
	}

#undef m_error
#undef m_debug
};
