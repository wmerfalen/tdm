#include "generator.hpp"
#include "../interpreter.hpp"
#include "../super-users.hpp"
#include "util.hpp"
#include "generated-rifle.hpp"

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
		std::cerr << "init ";
	}
	generator::~generator() {
		std::cout << "[generator::~generator]\n";
	}

	rifle_types_t generator::random_rifle_type() {
		uint32_t size = std::distance(valid_rifle_types.begin(), valid_rifle_types.end());
		return valid_rifle_types.at(this->roll<uint32_t>() % size);
	}
	rifle_attributes_t generator::random_rifle_attribute() {
		uint32_t size = std::distance(valid_rifle_attributes.begin(), valid_rifle_attributes.end());
		return valid_rifle_attributes.at(this->roll<uint32_t>() % size);
	}

	std::vector<std::pair<explosive_attributes_t, std::variant<uint32_t, float>>> generator::generate_explosive_attributes(
	    player_ptr_t& player) {
		return generate_random_mixed<explosive_attributes_t, uint32_t> (valid_explosive_attributes, player);
	}


	std::vector<std::pair<rifle_attributes_t, std::variant<uint32_t, float>>> generator::generate_rifle_attributes(
	    player_ptr_t& player) {
		return generate_random_mixed<rifle_attributes_t, uint32_t> (valid_rifle_attributes, player);
	}
	std::vector<std::pair<armor_attributes_t, std::variant<uint32_t, float>>> generator::generate_armor_attributes(
	    player_ptr_t& player) {
		return generate_random_mixed<armor_attributes_t, uint32_t> (valid_armor_attributes, player);
	}

	requirements_t generator::generate_requirements(player_ptr_t& player) {
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

	std::vector<std::pair<stat_types_t, std::variant<uint32_t, float>>> generator::generate_rifle_stat_boosts(
	    player_ptr_t& player) {
		return generate_random_mixed<stat_types_t, uint32_t> (valid_rifle_stats_boosts, player);
	}
	std::vector<std::pair<stat_types_t, std::variant<uint32_t, float>>> generator::generate_armor_stat_boosts(
	    player_ptr_t& player) {
		return generate_random_mixed<stat_types_t, uint32_t> (valid_armor_stats, player);
	}

	std::vector<std::pair<elemental_types_t, std::variant<uint32_t, float>>> generator::generate_armor_elemental_boosts(
	    player_ptr_t& player) {
		return generate_random_mixed<elemental_types_t, uint32_t> (valid_elemental_types, player);
	}
	std::vector<std::pair<elemental_types_t, std::variant<uint32_t, float>>> generator::generate_rifle_elemental_boosts(
	    player_ptr_t& player) {
		return generate_random_mixed<elemental_types_t, uint32_t> (valid_elemental_types, player);
	}
	item_types_t generator::random_item_type() {
		uint32_t size = std::distance(active_item_types.begin(), active_item_types.end());
		return active_item_types.at(this->roll<uint32_t>() % size);
	}

	obj_ptr_t reward_player(player_ptr_t& player) {
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
	}
};//end namespace mods::forge_engine
#undef m_debug
