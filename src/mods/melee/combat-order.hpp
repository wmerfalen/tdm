#ifndef __MENTOC_MODS_MELEE_combat_order_HEADER__
#define __MENTOC_MODS_MELEE_combat_order_HEADER__

#include "../../globals.hpp"
#include "../damage-event.hpp"
#include "../interpreter.hpp"

namespace mods::melee::main {
#define MENTOC_TECHNIQUES_SEQUENCE \
		(JAB) \
		(CROSS) \
		(UPPERCUT) \
		(LEFT_ELBOW) \
		(RIGHT_ELBOW) \
		(RIGHT_UPWARD_ELBOW) \
		(LEFT_UPWARD_ELBOW) \
		(RIGHT_OBLIQUE) \
		(LEFT_OBLIQUE) \
		(LEFT_TEEP) \
		(RIGHT_TEEP) \
		(LEFT_FRONT_KICK) \
		(RIGHT_FRONT_KICK) \
		(LEFT_KNEE_TO_HEAD) \
		(RIGHT_KNEE_TO_HEAD) \
		(LEFT_KNEE_TO_BODY) \
		(RIGHT_KNEE_TO_BODY) \
		(KNIFE_DISARM) \
		(PISTOL_DISARM) \
		(RIGHT_LEG_KICK) \
		(LEFT_LEG_KICK) \
		(RIGHT_KICK_TO_HEAD) \
		(LEFT_KICK_TO_HEAD) \
		(RIGHT_STOMP_TO_HEAD) \
		(LEFT_STOMP_TO_HEAD)

#define MENTOC_TECHNIQUE_ENUM_IMPL(r,data,ENUM_TYPE) ENUM_TYPE BOOST_PP_COMMA()

	enum technique_t : uint16_t {
		NONE = 0,
		BOOST_PP_SEQ_FOR_EACH(MENTOC_TECHNIQUE_ENUM_IMPL,~,MENTOC_TECHNIQUES_SEQUENCE)
	};
	using func_t = void(*)(player_ptr_t&,player_ptr_t&);

	/**
	 * Converting enum to string
	 */
#define MENTOC_ENUM_TO_STRING_IMPL(r,data,ENUM_TYPE) \
	if(technique_t::ENUM_TYPE == (technique_t)technique){ return BOOST_PP_STRINGIZE(ENUM_TYPE); }

	static inline std::string to_string(technique_t technique) {
		BOOST_PP_SEQ_FOR_EACH(MENTOC_ENUM_TO_STRING_IMPL,~,MENTOC_TECHNIQUES_SEQUENCE);
		return "<unknown>";
	}

	/** Converting from string to enum */
#define MENTOC_STR_TO_ENUM_IMPL(r,data,ENUM_TYPE) \
	if(mods::util::is_lower_match(technique, BOOST_PP_STRINGIZE(ENUM_TYPE))) { std::cerr << "found for: " << BOOST_PP_STRINGIZE(ENUM_TYPE) << "|TECHNIQUE: '" << technique << "'\n"; return technique_t::ENUM_TYPE; }


	static inline uint16_t to_technique_enum(std::string technique) {
		BOOST_PP_SEQ_FOR_EACH(MENTOC_STR_TO_ENUM_IMPL,~,MENTOC_TECHNIQUES_SEQUENCE);
		return 0;
	}

	/** The list of moves available */
	namespace moves {
#define MENTOC_FORWARD_DECLARE_FUNCTIONS_IMPL(r,data,ENUM_TYPE) \
	void ENUM_TYPE (player_ptr_t& attacker,player_ptr_t& victim);

		BOOST_PP_SEQ_FOR_EACH(MENTOC_FORWARD_DECLARE_FUNCTIONS_IMPL,~,MENTOC_TECHNIQUES_SEQUENCE);
	};


	/**
	 * Get the technique function ptr from technique enum
	 */
#define MENTOC_ENUM_TO_FUNCTION_IMPL(r,data,ENUM_TYPE) \
	if(technique == technique_t::ENUM_TYPE){ return moves::ENUM_TYPE; }

	static inline std::optional<func_t> get_technique_function(technique_t technique) {
		BOOST_PP_SEQ_FOR_EACH(MENTOC_ENUM_TO_FUNCTION_IMPL,~,MENTOC_TECHNIQUES_SEQUENCE);
		return std::nullopt;
	}

};//end combat order namespace

namespace mods::melee::combat_order {
	using technique_t = mods::melee::main::technique_t;
	using func_t = mods::melee::main::func_t;
};
#undef MENTOC_FORWARD_DECLARE_FUNCTIONS_IMPL
#undef MENTOC_TECHNIQUE_ENUM_IMPL
#undef MENTOC_STR_TO_ENUM_IMPL
#undef MENTOC_ENUM_TO_FUNCTION_IMPL
#undef MENTOC_ENUM_TO_STRING_IMPL
#endif

