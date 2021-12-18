#include "generator.hpp"
#include "../interpreter.hpp"
#include "../super-users.hpp"
#include "util.hpp"
#include "generated-rifle.hpp"
#include "../orm/rifle-instance.hpp"

#ifdef __MENTOC_SHOW_MODS_FORGE_ENGINE_DEBUG_OUTPUT__
#define m_debug(MSG) mentoc_prefix_debug("[mods::forge_engine::generator]")  << MSG << "\n";
#else
#define m_debug(MSG) ;;
#endif

namespace mods::forge_engine {
	mods::forge_engine::generator item_generator;

	static constexpr float ATTRIBUTE_LOW = 1.0;
	static constexpr float ATTRIBUTE_HIGH= 10000.0;
	static constexpr uint8_t MAX_ATTRIBUTES = 35;
	static constexpr float STAT_BOOST_LOW = 1.0;
	static constexpr float STAT_BOOST_HIGH= 10000.0;
	static constexpr uint8_t MAX_RIFLE_STAT_BOOSTS = 35;

	template <>
	bool generator::roll<bool>() {
		return rand_xoroshiro() & 1;
	}
	generator::generator() {
		this->init();
	}
	void generator::init() {
#ifdef MENTOC_DEBUG
		std::cerr << "init ";
#endif
	}
	generator::~generator() {
#ifdef MENTOC_DEBUG
		std::cerr << "[generator::~generator]\n";
#endif
	}

	rifle_attributes_t from_string(std::string t) {
		if(t.compare("RIFLE_ATTRIBUTES_AMMO_MAX") == 0) {
			return rifle_attributes_t::RIFLE_ATTRIBUTES_AMMO_MAX;
		}
		if(t.compare("RIFLE_ATTRIBUTES_CHANCE_TO_INJURE") == 0) {
			return rifle_attributes_t::RIFLE_ATTRIBUTES_CHANCE_TO_INJURE;
		}
		if(t.compare("RIFLE_ATTRIBUTES_CLIP_SIZE") == 0) {
			return rifle_attributes_t::RIFLE_ATTRIBUTES_CLIP_SIZE;
		}
		if(t.compare("RIFLE_ATTRIBUTES_COOLDOWN_BETWEEN_SHOTS") == 0) {
			return rifle_attributes_t::RIFLE_ATTRIBUTES_COOLDOWN_BETWEEN_SHOTS;
		}
		if(t.compare("RIFLE_ATTRIBUTES_CRITICAL_CHANCE") == 0) {
			return rifle_attributes_t::RIFLE_ATTRIBUTES_CRITICAL_CHANCE;
		}
		if(t.compare("RIFLE_ATTRIBUTES_CRITICAL_RANGE") == 0) {
			return rifle_attributes_t::RIFLE_ATTRIBUTES_CRITICAL_RANGE;
		}
		if(t.compare("RIFLE_ATTRIBUTES_BASE_DAMAGE") == 0) {
			return rifle_attributes_t::RIFLE_ATTRIBUTES_BASE_DAMAGE;
		}
		if(t.compare("RIFLE_ATTRIBUTES_DISORIENT_AMOUNT") == 0) {
			return rifle_attributes_t::RIFLE_ATTRIBUTES_DISORIENT_AMOUNT;
		}
		if(t.compare("RIFLE_ATTRIBUTES_HEADSHOT_BONUS") == 0) {
			return rifle_attributes_t::RIFLE_ATTRIBUTES_HEADSHOT_BONUS;
		}
		if(t.compare("RIFLE_ATTRIBUTES_MAX_RANGE") == 0) {
			return rifle_attributes_t::RIFLE_ATTRIBUTES_MAX_RANGE;
		}
		if(t.compare("RIFLE_ATTRIBUTES_RANGE_MULTIPLIER") == 0) {
			return rifle_attributes_t::RIFLE_ATTRIBUTES_RANGE_MULTIPLIER;
		}
		if(t.compare("RIFLE_ATTRIBUTES_RELOAD_TIME") == 0) {
			return rifle_attributes_t::RIFLE_ATTRIBUTES_RELOAD_TIME;
		}
		if(t.compare("RIFLE_ATTRIBUTES_ROUNDS_PER_MINUTE") == 0) {
			return rifle_attributes_t::RIFLE_ATTRIBUTES_ROUNDS_PER_MINUTE;
		}
		if(t.compare("RIFLE_ATTRIBUTES_MUZZLE_VELOCITY") == 0) {
			return rifle_attributes_t::RIFLE_ATTRIBUTES_MUZZLE_VELOCITY;
		}
		if(t.compare("RIFLE_ATTRIBUTES_EFFECTIVE_FIRING_RANGE") == 0) {
			return rifle_attributes_t::RIFLE_ATTRIBUTES_EFFECTIVE_FIRING_RANGE;
		}
		if(t.compare("RIFLE_ATTRIBUTES_DAMAGE_DICE_COUNT") == 0) {
			return rifle_attributes_t::RIFLE_ATTRIBUTES_DAMAGE_DICE_COUNT;
		}
		if(t.compare("RIFLE_ATTRIBUTES_DAMAGE_DICE_SIDES") == 0) {
			return rifle_attributes_t::RIFLE_ATTRIBUTES_DAMAGE_DICE_SIDES;
		}
		return rifle_attributes_t::__NO_ATTRIBUTE__;
	}




	/********************/
	/** rifle functions */
	/********************/
	rifle_types_t generator::random_rifle_type() {
		uint32_t size = std::distance(valid_rifle_types.begin(), valid_rifle_types.end());
		return valid_rifle_types.at(this->roll<uint32_t>() % size);
	}



	std::vector<std::pair<stat_types_t, std::variant<uint32_t, float>>> generator::generate_rifle_stat_boosts(
	    kill_t& player) {
		return generate_random_mixed<stat_types_t, uint32_t> (valid_rifle_stats_boosts, player);
	}



	rifle_attributes_t generator::random_rifle_attribute() {
		uint32_t size = std::distance(valid_rifle_attributes.begin(), valid_rifle_attributes.end());
		return valid_rifle_attributes.at(this->roll<uint32_t>() % size);
	}




	std::vector<std::pair<elemental_types_t, std::variant<uint32_t, float>>> generator::generate_rifle_elemental_boosts(
	    kill_t& player) {
		return generate_random_mixed<elemental_types_t, uint32_t> (valid_elemental_types, player);
	}


	/********************/
	/** armor functions */
	/********************/
	std::vector<std::pair<armor_attributes_t, std::variant<uint32_t, float>>> generator::generate_armor_attributes(
	    kill_t& player) {
		return generate_random_mixed<armor_attributes_t, uint32_t> (valid_armor_attributes, player);
	}
	std::vector<std::pair<stat_types_t, std::variant<uint32_t, float>>> generator::generate_armor_stat_boosts(
	    kill_t& player) {
		return generate_random_mixed<stat_types_t, uint32_t> (valid_armor_stats, player);
	}
	std::vector<std::pair<elemental_types_t, std::variant<uint32_t, float>>> generator::generate_armor_elemental_boosts(
	    kill_t& player) {
		return generate_random_mixed<elemental_types_t, uint32_t> (valid_elemental_types, player);
	}
	std::vector<std::pair<elemental_types_t, std::variant<uint32_t, float>>> generator::generate_armor_elemental_resistances(
	    kill_t& player) {
		return generate_random_mixed<elemental_types_t, uint32_t> (valid_elemental_types, player);
	}

	/************************/
	/** explosive functions */
	/************************/
	std::vector<std::pair<explosive_attributes_t, std::variant<uint32_t, float>>> generator::generate_explosive_attributes(
	    kill_t& player) {
		return generate_random_mixed<explosive_attributes_t, uint32_t> (valid_explosive_attributes, player);
	}


	/** generic item functions */
	item_types_t generator::random_item_type() {
		uint32_t size = std::distance(active_item_types.begin(), active_item_types.end());
		return active_item_types.at(this->roll<uint32_t>() % size);
	}


	requirements_t generator::generate_requirements(kill_t& kill) {
		auto player = kill.killer;
		requirements_t reqs;
		reqs.minimum_player_level = 0;

		/** TODO: game balancing: these values need to be tweaked */
		if(this->roll<bool>()) {
			uint8_t max_rifle_reqs = std::clamp((uint8_t)(player->level() / 30), (uint8_t)1, (uint8_t)(player->level() / 10));
			reqs.stats = generate_random<stat_types_t, uint32_t, 2> (valid_requirements, max_rifle_reqs);
		}

		if(this->roll<bool>()) {
			/** TODO: mess with this a bit */
			reqs.minimum_player_level = player->level();
		}

		if(this->roll<bool>()) {
			reqs.player_classes.emplace_back(generate_random_class(player));
		}

		return reqs;
	}

	obj_ptr_t reward_player_with(std::string_view type,player_ptr_t& player, mob_vnum victim) {
		kill_t k(player,victim);
		auto rifle = mods::forge_engine::generated_rifle_t(k,type).roll();
		mods::orm::rifle_instance orm_layer(rifle);
		return rifle;
	}

	/** wrapper function that ultimately decides the player's randomized item */
	obj_ptr_t reward_player(player_ptr_t& player, mob_vnum victim) {
		kill_t k(player,victim);
		auto rifle = mods::forge_engine::generated_rifle_t(k).roll();
		mods::orm::rifle_instance orm_layer(rifle);
		return rifle;
#ifdef __MENTOC_REWARD_ALL_TYPES__
		item_types_t type = item_generator.random_item_type();

		switch(type) {
			case item_types_t::TYPE_ITEM_EXPLOSIVE:
			/** TODO */
			case item_types_t::TYPE_ITEM_GADGET:
			case item_types_t::TYPE_ITEM_DRONE:
			case item_types_t::TYPE_ITEM_WEAPON:
			case item_types_t::TYPE_ITEM_ATTACHMENT:
			case item_types_t::TYPE_ITEM_ARMOR:
			/** TODO */
			case item_types_t::TYPE_ITEM_CONSUMABLE:
			case item_types_t::TYPE_ITEM_TRAP:
			case item_types_t::TYPE_ITEM_RIFLE:
			default:
				mods::forge_engine::generated_rifle_t rifle(player);
				return rifle.roll();
		}
#endif
	}
};//end namespace mods::forge_engine
#undef m_debug
