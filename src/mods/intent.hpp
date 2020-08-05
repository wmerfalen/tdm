#ifndef __MENTOC_MODS_WEAPONS_INTENT_HEADER__
#define __MENTOC_MODS_WEAPONS_INTENT_HEADER__

#include "../globals.hpp"

namespace mods::intent {
	enum intent_to_t {
		ATTACK = 1,
		FLEE
	};
	struct intent_t {
		intent_to_t 				wants_to;
		uuid_t 							player;
		std::vector<uuid_t> targets;
		uint8_t 						direction;
		room_rnum						from_room_id;
		room_rnum						target_room_id;
		uint64_t						created_at;
		uint64_t						timeout;
		std::vector<std::string> blockers;
		std::vector<std::string> solution;
	};

	void register_intent_to_attack(
			uuid_t player,
			std::vector<uuid_t> targets,
			uint8_t direction
	);
	void register_intent_to_flee(
			uuid_t player
	);
	void run_intents();
};

#endif
