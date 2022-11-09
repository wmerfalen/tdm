#include "builder-flags.hpp"
#include "object.hpp"
namespace mods::builder {
	using objtype = mods::object::type;
#define MENTOC_WEAPON(name) {mods::weapon::name,#name}
	std::array<std::pair<int,std::string>,13> weapon_type_flags = { {
			MENTOC_WEAPON(SMG),
			MENTOC_WEAPON(SHOTGUN),
			MENTOC_WEAPON(SNIPE),
			MENTOC_WEAPON(GRENADE),
			MENTOC_WEAPON(ASSAULT_RIFLE),
			MENTOC_WEAPON(PISTOL),
			MENTOC_WEAPON(MACHINE_PISTOL),
			MENTOC_WEAPON(ENERGY),
			MENTOC_WEAPON(FUMES),
			MENTOC_WEAPON(FLAME),
			MENTOC_WEAPON(CLAYMORE),
			MENTOC_WEAPON(REMOTE_EXPLOSIVE),
			MENTOC_WEAPON(ATTACK_DRONE)
		}
	};
#undef MENTOC_WEAPON

	std::array<std::pair<int,std::string>,3> sex_flags = { {
			{SEX_NEUTRAL,"NEUTRAL"},
			{SEX_MALE,"MALE"},
			{SEX_FEMALE,"FEMALE"}
		}
	};

	std::array<std::pair<int,std::string>,20> mob_act_flags = { {
			{MOB_SPEC,"SPEC"},
			{MOB_SENTINEL,"SENTINEL"},
			{MOB_SCAVENGER,"SCAVENGER"},
			{MOB_ISNPC,"ISNPC"},
			{MOB_AWARE,"AWARE"},
			{MOB_AGGRESSIVE,"AGGRESSIVE"},
			{MOB_STAY_ZONE,"STAY_ZONE"},
			{MOB_WIMPY,"WIMPY"},
			{MOB_AGGR_EVIL,"AGGR_EVIL"},
			{MOB_AGGR_GOOD,"AGGR_GOOD"},
			{MOB_AGGR_NEUTRAL,"AGGR_NEUTRAL"},
			{MOB_MEMORY,"MEMORY"},
			{MOB_HELPER,"HELPER"},
			{MOB_NOCHARM,"NOCHARM"},
			{MOB_NOSUMMON,"NOSUMMON"},
			{MOB_NOSLEEP,"NOSLEEP"},
			{MOB_NOBASH,"NOBASH"},
			{MOB_NOBLIND,"NOBLIND"},
			{MOB_NOTDEADYET,"NOTDEADYET"},
			{MOB_NODROP,"NODROP"},
		}
	};

	std::array<std::pair<int,std::string>,22> aff_flags = { {
			{AFF_BLIND,"BLIND"},
			{AFF_INVISIBLE,"INVISIBLE"},
			{AFF_DETECT_ALIGN,"DETECT_ALIGN"},
			{AFF_DETECT_INVIS,"DETECT_INVIS"},
			{AFF_DETECT_MAGIC,"DETECT_MAGIC"},
			{AFF_SENSE_LIFE,"SENSE_LIFE"},
			{AFF_INTIMIDATED,"INTIMIDATED"},
			{AFF_SANCTUARY,"SANCTUARY"},
			{AFF_GROUP,"GROUP"},
			{AFF_CURSE,"CURSE"},
			{AFF_INFRAVISION,"INFRAVISION"},
			{AFF_POISON,"POISON"},
			{AFF_PROTECT_EVIL,"PROTECT_EVIL"},
			{AFF_PROTECT_GOOD,"PROTECT_GOOD"},
			{AFF_SLEEP,"SLEEP"},
			{AFF_NOTRACK,"NOTRACK"},
			{AFF_UNUSED16,"UNUSED16"},
			{AFF_UNUSED17,"UNUSED17"},
			{AFF_SNEAK,"SNEAK"},
			{AFF_HIDE,"HIDE"},
			{AFF_UNUSED20,"UNUSED20"},
			{AFF_CHARM,"CHARM"},
		}
	};

	/**
	 * !introspection It's questionable why this is here within the builder.cpp file...
	 * If there is a reason that we're setting position flags on a mob,
	 * then this is fine, but it smells of bad design. A position flag
	 * is set throughout battle and not as a mob's prototype. In rare
	 * cases this may make sense, i.e.: necromancers will have a default
	 * position of DEAD.
	 */
	std::array<std::pair<int,std::string>,9> position_flags = { {
			{POS_DEAD,"DEAD"},
			{POS_MORTALLYW,"MORTALLYW"},
			{POS_INCAP,"INCAP"},
			{POS_STUNNED,"STUNNED"},
			{POS_SLEEPING,"SLEEPING"},
			{POS_RESTING,"RESTING"},
			{POS_SITTING,"SITTING"},
			{POS_FIGHTING,"FIGHTING"},
			{POS_STANDING,"STANDING"}
		}
	};

	std::array<std::pair<int,std::string>,3> type2_flags = { {
			{objtype::WEAPON,"WEAPON"},
			{objtype::CONTAINER,"CONTAINER"},
			{objtype::SENTINEL,"SENTINEL"}
		}
	};
	std::array<std::pair<int,std::string>,24> type_flags = { {
			{ITEM_LIGHT,"LIGHT"},
			{ITEM_SCROLL,"SCROLL"},
			{ITEM_WAND,"WAND"},
			{ITEM_STAFF,"STAFF"},
			{ITEM_WEAPON,"WEAPON"},
			{ITEM_FIREWEAPON,"FIREWEAPON"},
			{ITEM_MISSILE,"MISSILE"},
			{ITEM_TREASURE,"TREASURE"},
			{ITEM_ARMOR,"ARMOR"},
			{ITEM_POTION,"POTION"},
			{ITEM_WORN,"WORN"},
			{ITEM_OTHER,"OTHER"},
			{ITEM_TRASH,"TRASH"},
			{ITEM_TRAP,"TRAP"},
			{ITEM_CONTAINER,"CONTAINER"},
			{ITEM_NOTE,"NOTE"},
			{ITEM_DRINKCON,"DRINKCON"},
			{ITEM_KEY,"KEY"},
			{ITEM_FOOD,"FOOD"},
			{ITEM_MONEY,"MONEY"},
			{ITEM_PEN,"PEN"},
			{ITEM_BOAT,"BOAT"},
			{ITEM_FOUNTAIN,"FOUNTAIN"},
			{ITEM_WEAPON_ATTACHMENT,"WEAPON_ATTACHMENT"}
		}
	};

	std::array<std::pair<int,std::string>,17> extra_flags = { {
			{ITEM_GLOW,"GLOW"},
			{ITEM_HUM,"HUM"},
			{ITEM_NORENT,"NORENT"},
			{ITEM_NODONATE,"NODONATE"},
			{ITEM_NOINVIS,"NOINVIS"},
			{ITEM_INVISIBLE,"INVISIBLE"},
			{ITEM_MAGIC,"MAGIC"},
			{ITEM_NODROP,"NODROP"},
			{ITEM_BLESS,"BLESS"},
			{ITEM_ANTI_GOOD,"ANTI_GOOD"},
			{ITEM_ANTI_EVIL,"ANTI_EVIL"},
			{ITEM_ANTI_NEUTRAL,"ANTI_NEUTRAL"},
			{ITEM_ANTI_MAGIC_USER,"ANTI_MAGIC_USER"},
			{ITEM_ANTI_CLERIC,"ANTI_CLERIC"},
			{ITEM_ANTI_THIEF,"ANTI_THIEF"},
			{ITEM_ANTI_WARRIOR,"ANTI_WARRIOR"},
			{ITEM_NOSELL,"NOSELL"}
		}
	};
	std::array<std::pair<int,std::string>,38> affected_flags = { {
			{APPLY_NONE,"NONE"},
			{APPLY_STR,"STR"},
			{APPLY_DEX,"DEX"},
			{APPLY_INT,"INT"},
			{APPLY_WIS,"WIS"},
			{APPLY_CON,"CON"},
			{APPLY_CHA,"CHA"},
			{APPLY_CLASS,"CLASS"},
			{APPLY_LEVEL,"LEVEL"},
			{APPLY_AGE,"AGE"},
			{APPLY_CHAR_WEIGHT,"CHAR_WEIGHT"},
			{APPLY_CHAR_HEIGHT,"CHAR_HEIGHT"},
			{APPLY_MANA,"MANA"},
			{APPLY_HIT,"HIT"},
			{APPLY_MOVE,"MOVE"},
			{APPLY_GOLD,"GOLD"},
			{APPLY_EXP,"EXP"},
			{APPLY_AC,"AC"},
			{APPLY_HITROLL,"HITROLL"},
			{APPLY_DAMROLL,"DAMROLL"},
			{APPLY_SAVING_PARA,"SAVING_PARA"},
			{APPLY_SAVING_ROD,"SAVING_ROD"},
			{APPLY_SAVING_PETRI,"SAVING_PETRI"},
			{APPLY_SAVING_BREATH,"SAVING_BREATH"},
			{APPLY_SAVING_SPELL,"SAVING_SPELL"},
			{APPLY_SCOPE,"SCOPE"},
			{APPLY_RECOIL,"RECOIL"},
			{APPLY_BARREL,"BARREL"},
			{APPLY_MAGAZINE_SIZE,"MAGAZINE_SIZE"},
			{APPLY_INCENDIARY,"INCENDIARY"},
			{APPLY_SIDERAIL,"SIDERAIL"},
			{APPLY_UNDER_BARREL,"UNDER_BARREL"},
			{APPLY_PENETRATION,"PENETRATION"},
			{APPLY_RANGE,"RANGE"},
			{APPLY_CRITICAL_RANGE,"CRITICAL_RANGE"},
			{APPLY_OPTIMAL_RANGE,"OPTIMAL_RANGE"},
			{APPLY_VISION,"VISION"},
			{APPLY_STEALTH,"STEALTH"}

		}
	};
	std::array<std::pair<int,std::string>,15> wear_flags = { {
			{ITEM_WEAR_TAKE,"TAKE"},
			{ITEM_WEAR_FINGER,"FINGER"},
			{ITEM_WEAR_NECK,"NECK"},
			{ITEM_WEAR_BODY,"BODY"},
			{ITEM_WEAR_HEAD,"HEAD"},
			{ITEM_WEAR_LEGS,"LEGS"},
			{ITEM_WEAR_FEET,"FEET"},
			{ITEM_WEAR_HANDS,"HANDS"},
			{ITEM_WEAR_ARMS,"ARMS"},
			{ITEM_WEAR_SHIELD,"SHIELD"},
			{ITEM_WEAR_ABOUT,"ABOUT"},
			{ITEM_WEAR_WAIST,"WAIST"},
			{ITEM_WEAR_WRIST,"WRIST"},
			{ITEM_WEAR_WIELD,"WIELD"},
			{ITEM_WEAR_HOLD,"HOLD"}
		}
	};

};
