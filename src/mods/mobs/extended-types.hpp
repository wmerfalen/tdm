#ifndef __MENTOC_MODS_MOBS_EXTENDED_TYPES_HEADER__
#define  __MENTOC_MODS_MOBS_EXTENDED_TYPES_HEADER__

#include <iostream>
#include <map>
#include <string>
#include <optional>
#include <vector>

namespace mods::mobs {
	using uuid_t = uint64_t;
	enum extended_types_t : uint16_t {
		NONE =0,
		MINI_GUNNER,
		MINI_GUNNER_SENTINEL,
		INNOCENT,
		SENTINEL,
		AGGRESSIVE_MELEE,
		AGGRESSIVE_RIFLER,
		LUNATIC,
		GRUNT,
		PRACTICE_DUMMY,
		LOWLY_SECURITY,
		MP_SHOTGUNNER,
		MP_ENFORCER,
		ROGUE_MP_SHOTGUNNER,
		CAR_THIEF,
		GENERIC_THIEF,
		CHAOTIC_METH_ADDICT,
		SHOPLIFTER,
		ARMED_GUARD,
	};
	//vim sorcery: s/\t\t\([A-Z_]\+\),/\t\t{\1,"\1"},/g
	static std::map<extended_types_t,std::string> ext_map = {
		{NONE,"NONE"},
		{MINI_GUNNER,"MINI_GUNNER"},
		{MINI_GUNNER_SENTINEL,"MINI_GUNNER_SENTINEL"},
		{INNOCENT,"INNOCENT"},
		{SENTINEL,"SENTINEL"},
		{AGGRESSIVE_MELEE,"AGGRESSIVE_MELEE"},
		{AGGRESSIVE_RIFLER,"AGGRESSIVE_RIFLER"},
		{LUNATIC,"LUNATIC"},
		{GRUNT,"GRUNT"},
		{PRACTICE_DUMMY,"PRACTICE_DUMMY"},
		{LOWLY_SECURITY,"LOWLY_SECURITY"},
		/**! @NEW_BEHAVIOUR_TREE@ !**/
		{MP_SHOTGUNNER,"MP_SHOTGUNNER"},
		{MP_ENFORCER,"MP_ENFORCER"},
		{ROGUE_MP_SHOTGUNNER,"ROGUE_MP_SHOTGUNNER"},
		{CAR_THIEF,"CAR_THIEF"},
		{GENERIC_THIEF,"GENERIC_THIEF"},
		{CHAOTIC_METH_ADDICT,"CHAOTIC_METH_ADDICT"},
		{SHOPLIFTER,"SHOPLIFTER"},
		{ARMED_GUARD,"ARMED_GUARD"},
	};
	void decorate(uuid_t mob_id);

	namespace desc {
		/**! @NEW_BEHAVIOUR_TREE@ !**/
		static const std::string mp_shotgunner = std::string("A military police guard armed with a BF-9 Shotgun. Relentless in pursuit and can call in backup via radio.");
		static const std::string lowly_security = std::string("A simple security guard that calls for backup when things get dicey.");
		static const std::string mini_gunner = std::string("The mob will have a backpack and minigun which he/she will use as the main weapon.") +
		                                       ("Mini gunner mobs are usually very slow and take a long time to adjust which direction ") +
		                                       ("they are attempting to spray toward. Once they have found a target, they will spray in that ") +
		                                       ("direction. There is one caveat with the mini-gunner and that is HEAT. Their mini-gun overheats ") +
		                                       ("frequently and requires their constant spraying to cease every once in awhile.");
		static const std::string mini_gunner_sentinel = std::string("The mob will have a backpack and minigun which he/she will use as the main weapon.","This mob stands guard in a single room and will generally face on direction (while watching each room in that direction for a number of rooms).");
		static const std::string innocent = std::string("Innocent mobs that wouldn't harm a fly (even when provoked). These mobs fulfill the ") +
		                                    ("victim archetype, often being in the middle of unfortunate situations despite their willingness ") +
		                                    ("to actively avoid any conflict. An innocent mob will do anything as long as it means they can ") +
		                                    ("avoid losing their life.");
	};
	static std::map<extended_types_t,std::string> description_strings = {
		{NONE,"NONE"},
		{MINI_GUNNER,desc::mini_gunner},
		{MINI_GUNNER_SENTINEL,desc::mini_gunner_sentinel},
		{INNOCENT,desc::innocent},
		{SENTINEL,"SENTINEL"},
		{AGGRESSIVE_MELEE,"AGGRESSIVE_MELEE"},
		{AGGRESSIVE_RIFLER,"AGGRESSIVE_RIFLER"},
		{LUNATIC,"LUNATIC"},
		{GRUNT,"GRUNT"},
		{PRACTICE_DUMMY,"PRACTICE_DUMMY"},
		{LOWLY_SECURITY,desc::lowly_security},
		/**! @NEW_BEHAVIOUR_TREE@ !**/
		{MP_SHOTGUNNER,desc::mp_shotgunner},
		{MP_ENFORCER,"MP_ENFORCER"},
		{ROGUE_MP_SHOTGUNNER,"ROGUE_MP_SHOTGUNNER"},
		{CAR_THIEF,"CAR_THIEF"},
		{GENERIC_THIEF,"GENERIC_THIEF"},
		{CHAOTIC_METH_ADDICT,"CHAOTIC_METH_ADDICT"},
		{SHOPLIFTER,"SHOPLIFTER"},
		{ARMED_GUARD,"ARMED_GUARD"},
	};
	namespace extended_types {
		using mob_rnum = uint32_t;
		std::vector<std::string> strings();
		std::optional<extended_types_t> from_string(std::string);
		std::string to_string(extended_types_t t);
		std::string description(std::string str);
		std::string description(extended_types_t type);
		void register_targets(const mob_rnum& vnum, std::string_view items);
		std::string_view get_mobs_targets(const mob_rnum& vnum);
	};
	void default_wear_strategy(int where, std::string_view path);
	std::tuple<int,std::string> extract_yaml_info_from_path(std::string_view path);
};
#endif
