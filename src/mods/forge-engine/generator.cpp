#include "generator.hpp"
#include "../interpreter.hpp"
#include "../super-users.hpp"
#include "util.hpp"

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

	std::vector<std::pair<elemental_types_t, std::variant<uint32_t, float>>> generator::generate_rifle_elemental_boosts(
	    player_ptr_t& player) {
		return generate_random_mixed<elemental_types_t, uint32_t> (valid_elemental_types, player);
	}
	item_types_t generator::random_item_type() {
		uint32_t size = std::distance(active_item_types.begin(), active_item_types.end());
		return active_item_types.at(this->roll<uint32_t>() % size);
	}

	/** admin utility function */
	void generated_rifle_t::fill(obj_ptr_t& obj) {
		fill_attributes(obj);
		fill_elemental(obj);
		fill_stats(obj);
	}

	void generated_rifle_t::fill_stats(obj_ptr_t& obj) {
		fill_stats_generic(obj,m_stat_boosts,obj->rifle()->attributes.get());
	}
	void generated_rifle_t::fill_attributes(obj_ptr_t& obj) {
		for(auto& attr : m_attributes) {
			switch(attr.first) {
				case RIFLE_ATTRIBUTES_AMMO_MAX:
					obj->rifle()->attributes->ammo_max = fetch(attr.second,obj->rifle()->attributes->ammo_max);
					break;
				case RIFLE_ATTRIBUTES_CHANCE_TO_INJURE:
					obj->rifle()->attributes->chance_to_injure = fetch(attr.second,obj->rifle()->attributes->chance_to_injure);
					break;
				case RIFLE_ATTRIBUTES_CLIP_SIZE:
					obj->rifle()->attributes->clip_size = fetch(attr.second,obj->rifle()->attributes->clip_size);
					break;
					obj->rifle()->attributes->cooldown_between_shots = fetch(attr.second,obj->rifle()->attributes->cooldown_between_shots);
					break;
				case RIFLE_ATTRIBUTES_CRITICAL_CHANCE:
					obj->rifle()->attributes->critical_chance = fetch(attr.second,obj->rifle()->attributes->critical_chance);
					break;
				case RIFLE_ATTRIBUTES_CRITICAL_RANGE:
					obj->rifle()->attributes->critical_range = fetch(attr.second,obj->rifle()->attributes->critical_range);
					break;
				case RIFLE_ATTRIBUTES_DAMAGE_PER_SECOND:
					obj->rifle()->attributes->damage_per_second = fetch(attr.second,obj->rifle()->attributes->damage_per_second);
					break;
				case RIFLE_ATTRIBUTES_DISORIENT_AMOUNT:
					obj->rifle()->attributes->disorient_amount = fetch(attr.second,obj->rifle()->attributes->disorient_amount);
					break;
				case RIFLE_ATTRIBUTES_HEADSHOT_BONUS:
					obj->rifle()->attributes->headshot_bonus = fetch(attr.second,obj->rifle()->attributes->headshot_bonus);
					break;
				case RIFLE_ATTRIBUTES_MAX_RANGE:
					obj->rifle()->attributes->max_range = fetch(attr.second,obj->rifle()->attributes->max_range);
					break;
				case RIFLE_ATTRIBUTES_RANGE_MULTIPLIER:
					obj->rifle()->attributes->range_multiplier = fetch(attr.second,obj->rifle()->attributes->range_multiplier);
					break;
				case RIFLE_ATTRIBUTES_RELOAD_TIME:
					obj->rifle()->attributes->reload_time = fetch(attr.second,obj->rifle()->attributes->reload_time);
					break;
				case RIFLE_ATTRIBUTES_ROUNDS_PER_MINUTE:
					obj->rifle()->attributes->rounds_per_minute = fetch(attr.second,obj->rifle()->attributes->rounds_per_minute);
					break;
				case RIFLE_ATTRIBUTES_MUZZLE_VELOCITY:
					obj->rifle()->attributes->muzzle_velocity = fetch(attr.second,obj->rifle()->attributes->muzzle_velocity);
					break;
				case RIFLE_ATTRIBUTES_EFFECTIVE_FIRING_RANGE:
					obj->rifle()->attributes->effective_firing_range = fetch(attr.second,obj->rifle()->attributes->effective_firing_range);
					break;
				case RIFLE_ATTRIBUTES_DAMAGE_DICE_COUNT:
					obj->rifle()->attributes->damage_dice_count = fetch(attr.second,obj->rifle()->attributes->damage_dice_count);
					break;
				case RIFLE_ATTRIBUTES_DAMAGE_DICE_SIDES:
					obj->rifle()->attributes->damage_dice_sides = fetch(attr.second,obj->rifle()->attributes->damage_dice_sides);
					break;
				default:
					break;
			}
		}
	}

	void generated_rifle_t::fill_elemental(obj_ptr_t& obj) {
		fill_elemental_damage_generic(obj,m_elemental_damages,obj->rifle()->attributes.get());
	}


	generated_rifle_t::generated_rifle_t (player_ptr_t& player) {
		m_player = player;
	}

	obj_ptr_t generated_rifle_t::roll() {
		m_type = mods::forge_engine::item_generator.random_rifle_type();
		m_requirements = mods::forge_engine::item_generator.generate_requirements(m_player);
		m_attributes = mods::forge_engine::item_generator.generate_rifle_attributes(m_player);
		m_elemental_damages = mods::forge_engine::item_generator.generate_rifle_elemental_boosts(m_player);
		m_stat_boosts = mods::forge_engine::item_generator.generate_rifle_stat_boosts(m_player);
		m_instance = create_object(ITEM_RIFLE, "g36c.yml");
		this->fill(m_instance);
		return m_instance;
	}

	void generated_rifle_t::send_stats_to_player(player_ptr_t& player) {
		player->send("%s\r\n", get_dump().c_str());
	}

	std::string generated_rifle_t::get_dump() {
		std::string output;
		output += "---- start of dump ----\r\n";
		output += CAT("random rifle type: ", to_string(m_type), "\r\n");

		for(auto& req : m_attributes) {
			if(std::holds_alternative<float> (req.second)) {
				output += CAT("attribute: ", to_string(req.first), ": ", std::to_string(std::get<float> (req.second)), "\r\n");
			} else {
				output += CAT("attribute: ", to_string(req.first), ": ", std::to_string(std::get<uint32_t> (req.second)), "\r\n");
			}
		}

		for(auto& req : m_stat_boosts) {
			if(std::holds_alternative<float> (req.second)) {
				output += CAT("stat_boost: ", to_string(req.first), ": ", std::to_string(std::get<float> (req.second)), "\r\n");
			} else if(std::holds_alternative<uint32_t> (req.second)) {
				output += CAT("stat_boost: ", to_string(req.first), ": ", std::to_string(std::get<uint32_t> (req.second)), "\r\n");
			}
		}

		for(auto& req : m_elemental_damages) {
			if(std::holds_alternative<float> (req.second)) {
				output += CAT("stat_boost: ", to_string(req.first), ": ", std::to_string(std::get<float> (req.second)), "\r\n");
			} else if(std::holds_alternative<uint32_t> (req.second)) {
				output += CAT("stat_boost: ", to_string(req.first), ": ", std::to_string(std::get<uint32_t> (req.second)), "\r\n");
			}
		}

		output += CAT("\r\n", get_requirements_string(m_requirements), "\r\n");
		output += "---- End of dump ----\r\n";
		return output;
	}

	obj_ptr_t reward_player(player_ptr_t& player) {
		item_types_t type = item_generator.random_item_type();

		switch(type) {
			case item_types_t::TYPE_ITEM_EXPLOSIVE:
			case item_types_t::TYPE_ITEM_GADGET:
			case item_types_t::TYPE_ITEM_DRONE:
			case item_types_t::TYPE_ITEM_WEAPON:
			case item_types_t::TYPE_ITEM_ATTACHMENT:
			case item_types_t::TYPE_ITEM_ARMOR:
			case item_types_t::TYPE_ITEM_CONSUMABLE:
			case item_types_t::TYPE_ITEM_TRAP:
			case item_types_t::TYPE_ITEM_RIFLE:
			default:
				generated_rifle_t rifle(player);
				return rifle.roll();
		}
	}
};//end namespace mods::forge_engine
#undef m_debug
