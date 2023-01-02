#ifndef __MENTOC_MODS_TARGET_ACQUISITION_HEADER__
#define __MENTOC_MODS_TARGET_ACQUISITION_HEADER__
#include "scan.hpp"
#include "player.hpp"
#include "rooms.hpp"
#include "object-utils.hpp"

namespace mods::target_acquisition {
	using vpd = mods::scan::vec_player_data;
	using de = damage_event_t;
	using namespace mods::object_utils;
	struct calculation_t {
		int16_t max_range;
		int16_t critical_range;
		std::pair<int16_t,int16_t> effective_range;
		uint16_t stat_boosts;
	};

	struct target_t {
		std::string target_name;
		direction_t direction;
		bool is_corpse;
		bool is_object;
		bool is_character;
		target_t(std::string_view _target_name,
		    direction_t _direction,
		    bool _is_corpse,
		    bool _is_object,
		    bool _is_character
		) : target_name(_target_name),
			direction(_direction),
			is_corpse(_is_corpse),
			is_object(_is_object),
			is_character(_is_character)
		{}
		target_t(std::string_view _target_name,
		    direction_t _direction) :
			target_name(_target_name),
			direction(_direction),
			is_corpse(0),
			is_object(0),
			is_character(1)
		{}
		target_t() = delete;
		~target_t() = default;
	};

	struct acquired_target_t {
		player_ptr_t target;
		direction_t direction;
		uint16_t distance;
		acquired_target_t() = default;
		acquired_target_t(std::optional<std::tuple<player_ptr_t,direction_t,uint16_t>> opt) {
			if(opt.has_value()) {
				auto t = opt.value();
				target = std::get<0>(t);
				direction = std::get<1>(t);
				distance = std::get<2>(t);
				return;
			}
			target = nullptr;
			direction = NORTH;
			distance = 0;
		}
	};

};//end namespace mods::target_acquisition
#endif
