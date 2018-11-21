#include "flags.hpp"
namespace mods::flags {
	mods::flags::plr_container_t plr_flags = {
		{mods::flags::plr::KILLER,mods::flags::LEGACY_PLR_KILLER},
		{mods::flags::plr::THIEF,mods::flags::LEGACY_PLR_THIEF},
		{mods::flags::plr::FROZEN,mods::flags::LEGACY_PLR_FROZEN},
		{mods::flags::plr::DONTSET,mods::flags::LEGACY_PLR_DONTSET},
		{mods::flags::plr::WRITING,mods::flags::LEGACY_PLR_WRITING},
		{mods::flags::plr::MAILING,mods::flags::LEGACY_PLR_MAILING},
		{mods::flags::plr::CRASH,mods::flags::LEGACY_PLR_CRASH},
		{mods::flags::plr::SITEOK,mods::flags::LEGACY_PLR_SITEOK},
		{mods::flags::plr::NOSHOUT,mods::flags::LEGACY_PLR_NOSHOUT},
		{mods::flags::plr::NOTITLE,mods::flags::LEGACY_PLR_NOTITLE},
		{mods::flags::plr::DELETED,mods::flags::LEGACY_PLR_DELETED},
		{mods::flags::plr::LOADROOM,mods::flags::LEGACY_PLR_LOADROOM},
		{mods::flags::plr::NOWIZLIST,mods::flags::LEGACY_PLR_NOWIZLIST},
		{mods::flags::plr::INVSTART,mods::flags::LEGACY_PLR_INVSTART},
		{mods::flags::plr::CRYO,mods::flags::LEGACY_PLR_CRYO},
		{mods::flags::plr::NOTDEADYET,mods::flags::LEGACY_PLR_NOTDEADYET}
	};
	mods::flags::aff_container_t aff_flags = {
		{mods::flags::aff::BLIND,mods::flags::LEGACY_AFF_BLIND},
		{mods::flags::aff::INVISIBLE,mods::flags::LEGACY_AFF_INVISIBLE},
		{mods::flags::aff::DETECT_ALIGN,mods::flags::LEGACY_AFF_DETECT_ALIGN},
		{mods::flags::aff::DETECT_INVIS,mods::flags::LEGACY_AFF_DETECT_INVIS},
		{mods::flags::aff::DETECT_MAGIC,mods::flags::LEGACY_AFF_DETECT_MAGIC},
		{mods::flags::aff::SENSE_LIFE,mods::flags::LEGACY_AFF_SENSE_LIFE},
		{mods::flags::aff::WATERWALK,mods::flags::LEGACY_AFF_WATERWALK},
		{mods::flags::aff::SANCTUARY,mods::flags::LEGACY_AFF_SANCTUARY},
		{mods::flags::aff::GROUP,mods::flags::LEGACY_AFF_GROUP},
		{mods::flags::aff::CURSE,mods::flags::LEGACY_AFF_CURSE},
		{mods::flags::aff::INFRAVISION,mods::flags::LEGACY_AFF_INFRAVISION},
		{mods::flags::aff::POISON,mods::flags::LEGACY_AFF_POISON},
		{mods::flags::aff::PROTECT_EVIL,mods::flags::LEGACY_AFF_PROTECT_EVIL},
		{mods::flags::aff::PROTECT_GOOD,mods::flags::LEGACY_AFF_PROTECT_GOOD},
		{mods::flags::aff::SLEEP,mods::flags::LEGACY_AFF_SLEEP},
		{mods::flags::aff::NOTRACK,mods::flags::LEGACY_AFF_NOTRACK},
		{mods::flags::aff::UNUSED16,mods::flags::LEGACY_AFF_UNUSED16},
		{mods::flags::aff::UNUSED17,mods::flags::LEGACY_AFF_UNUSED17},
		{mods::flags::aff::SNEAK,mods::flags::LEGACY_AFF_SNEAK},
		{mods::flags::aff::HIDE,mods::flags::LEGACY_AFF_HIDE},
		//{mods::flags::aff::UNUSED20,mods::flags::LEGACY_AFF_UNUSED20},
		{mods::flags::aff::CHARM,mods::flags::LEGACY_AFF_CHARM}
	};
};//end namespace mods::flags
