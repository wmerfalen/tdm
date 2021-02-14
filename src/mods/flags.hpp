#ifndef __MENTOC_MODS_FLAGS_HEADER__
#define  __MENTOC_MODS_FLAGS_HEADER__
#include <vector>
#include <string>
namespace mods::flags {
	enum chunk_type_t {
		LEGACY_AFF = 0,
		LEGACY_PLR,
		VISION,
		BUILDER,
		FIRST = LEGACY_AFF,
		LAST = BUILDER
	};
	constexpr static uint32_t NOT_IMPL = (1 << 22);
	constexpr static uint32_t LEGACY_PLR_KILLER = (1 << 0);
	constexpr static uint32_t LEGACY_PLR_THIEF = (1 << 1);
	constexpr static uint32_t LEGACY_PLR_FROZEN = (1 << 2);
	constexpr static uint32_t LEGACY_PLR_DONTSET = (1 << 3);
	constexpr static uint32_t LEGACY_PLR_WRITING = (1 << 4);
	constexpr static uint32_t LEGACY_PLR_MAILING = (1 << 5);
	constexpr static uint32_t LEGACY_PLR_CRASH = (1 << 6);
	constexpr static uint32_t LEGACY_PLR_SITEOK = (1 << 7);
	constexpr static uint32_t LEGACY_PLR_NOSHOUT = (1 << 8);
	constexpr static uint32_t LEGACY_PLR_NOTITLE = (1 << 9);
	constexpr static uint32_t LEGACY_PLR_DELETED = (1 << 10);
	constexpr static uint32_t LEGACY_PLR_LOADROOM = (1 << 11);
	constexpr static uint32_t LEGACY_PLR_NOWIZLIST = (1 << 12);
	constexpr static uint32_t LEGACY_PLR_NODELETE = (1 << 13);
	constexpr static uint32_t LEGACY_PLR_INVSTART = (1 << 14);
	constexpr static uint32_t LEGACY_PLR_CRYO = (1 << 15);
	constexpr static uint32_t LEGACY_PLR_NOTDEADYET = (1 << 16);
	constexpr static uint32_t LEGACY_AFF_BLIND = (1 << 0);
	constexpr static uint32_t LEGACY_AFF_INVISIBLE = (1 << 1);
	constexpr static uint32_t LEGACY_AFF_DETECT_ALIGN = (1 << 2);
	constexpr static uint32_t LEGACY_AFF_DETECT_INVIS = (1 << 3);
	constexpr static uint32_t LEGACY_AFF_DETECT_MAGIC = (1 << 4);
	constexpr static uint32_t LEGACY_AFF_SENSE_LIFE = (1 << 5);
	constexpr static uint32_t LEGACY_AFF_WATERWALK = (1 << 6);
	constexpr static uint32_t LEGACY_AFF_SANCTUARY = (1 << 7);
	constexpr static uint32_t LEGACY_AFF_GROUP = (1 << 8);
	constexpr static uint32_t LEGACY_AFF_CURSE = (1 << 9);
	constexpr static uint32_t LEGACY_AFF_INFRAVISION = (1 << 10);
	constexpr static uint32_t LEGACY_AFF_POISON = (1 << 11);
	constexpr static uint32_t LEGACY_AFF_PROTECT_EVIL = (1 << 12);
	constexpr static uint32_t LEGACY_AFF_PROTECT_GOOD = (1 << 13);
	constexpr static uint32_t LEGACY_AFF_SLEEP = (1 << 14);
	constexpr static uint32_t LEGACY_AFF_NOTRACK = (1 << 15);
	constexpr static uint32_t LEGACY_AFF_UNUSED16 = (1 << 16);
	constexpr static uint32_t LEGACY_AFF_UNUSED17 = (1 << 17);
	constexpr static uint32_t LEGACY_AFF_SNEAK = (1 << 18);
	constexpr static uint32_t LEGACY_AFF_HIDE = (1 << 19);
	constexpr static uint32_t LEGACY_AFF_UNUSED20 = (1 << 20);
	constexpr static uint32_t LEGACY_AFF_CHARM = (1 << 21);
	enum plr {
		KILLER = 1,
		THIEF,
		FROZEN,
		DONTSET,
		WRITING,
		MAILING,
		CRASH,
		SITEOK,
		NOTDEADYET,
		NOSHOUT,
		NOTITLE,
		DELETED,
		LOADROOM,
		NOWIZLIST,
		INVSTART,
		CRYO,
		NODELETE,
		__PLR_FIRST = KILLER,
		__PLR_LAST = CRYO
	};
	enum aff {
		BLIND = 1,
		INVISIBLE,
		DETECT_ALIGN, 	// useless
		DETECT_INVIS,
		DETECT_MAGIC,		// useless
		SENSE_LIFE,
		WATERWALK,			// useless
		SANCTUARY,
		GROUP,
		CURSE,					// useless
		INFRAVISION,
		POISON,
		PROTECT_EVIL,		// useless
		PROTECT_GOOD,		// useless
		SLEEP,
		NOTRACK,
		UNUSED16,				// useless
		CHARM,
		UNUSED17,				// useless
		SNEAK,
		HIDE,
		THERMAL_VISION,
		NIGHT_VISION,
		NORMAL_SIGHT,
		__AFF_FIRST = BLIND,
		__AFF_LAST = NORMAL_SIGHT
	};
	constexpr static uint32_t PLR_FLAG_COUNT = plr::__PLR_LAST + 1;
	constexpr static uint32_t AFF_FLAG_COUNT = aff::__AFF_LAST + 1;
	enum flag_group {
		AFF,PLR
	};
	using plr_container_t = std::vector<std::pair<plr,uint32_t>>;
	using aff_container_t = std::vector<std::pair<aff,uint32_t>>;
	extern plr_container_t plr_flags;
	extern aff_container_t aff_flags;
	//template <typename PlayerPointerType>
	//	static inline void setter_function(PlayerPointerType player,
	//			mods::flags::aff f){
	//		player->affect(f);
	//	}
	//template <typename PlayerPointerType>
	//	static inline void setter_function(PlayerPointerType player,
	//			mods::flags::plr f){
	//		player->affect_plr(f);
	//	}
//	template <typename PlayerPointerType,typename FlagType>
//		static inline void load(PlayerPointerType player,const std::string& data){
//
//		}
//
};
#endif
