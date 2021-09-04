#ifndef __MENTOC_MODS_FORGE_ENGINE_BOSS_DROP_RATES_HEADER__
#define __MENTOC_MODS_FORGE_ENGINE_BOSS_DROP_RATES_HEADER__
#include "../../structs.h"
#include "../pq.hpp"
#include "../sql.hpp"
#include <memory>
#include "../object-utils.hpp"
#include "../rand.hpp"
#include "value-scaler.hpp"
#include "requirements.hpp"
#include "elemental-enum.hpp"

#ifdef __MENTOC_SHOW_MODS_FORGE_ENGINE_DEBUG_OUTPUT__
#define m_debug(MSG) mentoc_prefix_debug("[mods::forge_engine::boss_drop_rates]")  << MSG << "\n";
#else
#define m_debug(MSG) ;;
#endif


namespace mods::forge_engine::boss_drop_rates {
	enum item_type_t {
		REWARD_NOTHING,
		REWARD_MISSION_POINTS,
		REWARD_RIFLE,
		REWARD_JUNK,

		/** Starting here, these items are the same percentage */
		REWARD_RING,
		REWARD_AMULET,
		REWARD_JEWEL,
		REWARD_RUNE,
		REWARD_GEM,
		/** Ending here */

		REWARD_UNIQUE_ITEM,
		REWARD_POTION,
		REWARD_SCROLL,
	};

#if 0

	static const float DEFILER_DROP_RATES[] = {
		18.0,	/** NOTHING - 18 */
		29.0, /** MISSION POINTS - 47 */
		25.0, /** RIFLE - 72 */
		4.0, /** JUNK - 76 */
		3.0, /** RING/AMULET/JEWEL/RUNE/GEM - 79 */
		0.001, /** UNIQUE - 80 */
		13.0, /** POTION - 93 */
		7.0, /** SCROLL - 100 */
	};
#endif

};

#endif
