#include "drops.hpp"
#include "super-users.hpp"
#include "interpreter.hpp"
#include "rand.hpp"

//#define __MENTOC_SHOW_MODS_drops_DEBUG_OUTPUT__
#ifdef __MENTOC_SHOW_MODS_drops_DEBUG_OUTPUT__
#define m_debug(MSG) mentoc_prefix_debug("[mods::drops::debug]")  << MSG << "\n";
#define m_error(MSG) mentoc_prefix_debug(red_str("[mods::drops::ERROR]"))  << MSG << "\n";
#else
#define m_debug(MSG) ;;
#define m_error(MSG) ;;
#endif

extern void object_list_new_owner(struct obj_data *list, char_data *ch);
extern void	obj_to_obj(obj_ptr_t from_object, obj_ptr_t to_object);
extern void obj_from_room(obj_ptr_t in_object);

namespace mods::drops {
	/**
	 * SQL black magic:

	 select CONCAT('static constexpr mob_vnum ',CONCAT(CONCAT(REPLACE(UPPER(mob_name),' ','_'),' = ',mob_virtual_number),';')) from mobile where mob_scalable != '1';

	 */

	/**
	 * Vim sort sorcery:
	 :r ! cat % | grep 'static constexpr mob_vnum' |  sed -E 's|\{/?[A-Z]+\}||gi' | sed -E 's| _|_|gi' | sed -E 's|_ |_|gi' | sed -e 's|  | |gi' | sed -E 's| AN_| |gi' | sed -E 's| A_| |gi' | uniq | sort
	 */


	/** Hostile NPCS */
	static constexpr mob_vnum ADAPTED_DRONE = 115;
	static constexpr mob_vnum INFECTED_DRONE = 114;
	static constexpr mob_vnum MILITARY_POLICE_ENFORCER = 500;
	static constexpr mob_vnum MILITARY_POLICE_SHOTGUNNER = 501;
	static constexpr mob_vnum SHOPLIFTER = 104;
	static constexpr mob_vnum SHOPLIFTER_2 = 110;
	static constexpr mob_vnum ORTHOS_GUARD = 503;
	static constexpr mob_vnum ORTHOS_MENACE = 505;
	static constexpr mob_vnum ORTHOS_SENTINEL = 669;
	static constexpr mob_vnum ORTHOS_SNIPER = 504;
	static constexpr mob_vnum ARMED_SECURITY_GUARD = 106;
	static constexpr mob_vnum BALLISTICS_EXPERT = 603;
	static constexpr mob_vnum BANK_TELLER = 107;
	static constexpr mob_vnum CHAOTIC_METH_ADDICT = 103;
	static constexpr mob_vnum CHEF_EMPLOYEE = 1;
	static constexpr mob_vnum CRACKHEAD = 105;
	static constexpr mob_vnum DEFILER = 666;
	static constexpr mob_vnum EXTREMIST = 670;
	static constexpr mob_vnum KIDNAPPER = 102;
	static constexpr mob_vnum MAINTENANCE_GRUNT = 668;
	static constexpr mob_vnum MAINTENANCE_GUARD = 667;
	static constexpr mob_vnum OPS_SHIELD_SHOTGUNNER = 502;
	static constexpr mob_vnum PETTY_THIEF = 101;
	static constexpr mob_vnum RETAIL_ASSOCIATE = 109;
	static constexpr mob_vnum ROGUE_MILITARY_POLICE_SHOTGUNNER = 407;
	static constexpr mob_vnum SUSPICIOUS_LOOKING_CAR_THIEF = 406;

	/** Non-hostile */
	static constexpr mob_vnum CORPORAL_JAMES_TAGGART = 410;
	static constexpr mob_vnum INVENTORY_GRUNT = 113;
	static constexpr mob_vnum INVENTORY_SPECIALIST = 112;
	static constexpr mob_vnum TRITON_LABS_FIELD_SURGEON = 602;
	static constexpr mob_vnum TRITON_LABS_SCIENTIST = 600;
	static constexpr mob_vnum VOLUNTEER_PATIENT = 601;

	///

//

	struct randomized_drop_t {
		mob_vnum mob;
		std::string yaml;
		std::tuple<uint16_t,uint16_t> dice;
		uint16_t greater_than;

		randomized_drop_t(auto m, auto y, auto dc, auto ds, auto gt) :
			mob(m),
			yaml(y), dice( {
			dc,ds
		}), greater_than(gt) {}
		randomized_drop_t(const randomized_drop_t& other) {
			mob = other.mob;
			yaml = other.yaml;
			dice = other.dice;
			greater_than = other.greater_than;
		}

	};
	bool roll(const randomized_drop_t& drop) {
		return dice(drop.dice) > drop.greater_than;
	}


	static constexpr std::string_view frag = "explosive/frag-grenade.yml";
	static constexpr std::string_view sensor = "explosive/sensor-grenade.yml";
	static constexpr std::string_view smoke = "explosive/smoke-grenade.yml";
	static constexpr std::string_view incendiary = "explosive/incendiary-grenade.yml";
	static constexpr std::string_view breach = "explosive/breach-charge.yml";
	static constexpr std::string_view claymore = "explosive/claymore-mine.yml";
	static constexpr std::string_view corpse_charge = "explosive/corpse-charge.yml";
	static constexpr std::string_view cormine = "explosive/corrosive-claymore-mine.yml";
	static constexpr std::string_view flash = "explosive/flashbang-grenade.yml";
	static constexpr std::string_view hellfirecc = "explosive/hellfire-corpse-charge.yml";
	static constexpr std::string_view shrapmine = "explosive/shrapnel-claymore-mine.yml";
	static constexpr std::string_view shrapcorc = "explosive/shrapnel-corpse-charge.yml";

	//
	// GOD_TIER
	static constexpr std::string_view tshould = "armor/titan-shoulder-pads.yml";
	static constexpr std::string_view vshould = "armor/viper-shoulder-pads.yml";
	static constexpr std::string_view pk3vest = "armor/vulture-pk3-ballistic-vest.yml";
	static constexpr std::string_view xmshould = "armor/xm-scorpio-shoulder-pads.yml";
	static constexpr std::string_view xscorpvest = "armor/xm-scorpio-slotted-vest-pack.yml";
	static constexpr std::string_view xscorpgloves = "armor/xm-scorpio-tactical-gloves.yml";
	static constexpr std::string_view xtgaunt = "armor/xtv89-gauntlets.yml";

	// LEGENDARY
	static constexpr std::string_view fshould = "armor/falcon-shoulder-pads.yml";
	static constexpr std::string_view xm3gloves = "armor/forge-xm3-gloves.yml";
	static constexpr std::string_view xmvboots = "armor/xm607-vulture-boots.yml";

	// COMMON
	static constexpr std::string_view fbvest = "armor/falcon-ballistic-vest.yml";
	static constexpr std::string_view mpegloves = "armor/mp-enforcer-gloves.yml";
	static constexpr std::string_view mpepants= "armor/mp-enforcer-pants.yml";
	static constexpr std::string_view p5boots = "armor/p5-boots.yml";
	static constexpr std::string_view p5pants = "armor/p5-pants.yml";
	static constexpr std::string_view p5vest = "armor/p5-vest.yml";
	static constexpr std::string_view ruback = "armor/raven-ultralight-backpack.yml";
	static constexpr std::string_view tgask = "armor/tactical-gas-mask.yml";
	static constexpr std::string_view therm = "armor/thermal-goggles.yml";
	static constexpr std::string_view tgaunt = "armor/titan-gauntlets.yml";
	static constexpr std::string_view tshin = "armor/titan-shin-guards.yml";
	static constexpr std::string_view vleg = "armor/viper-leg-guards.yml";
	static constexpr std::string_view xmboots = "armor/xm50-ultralight-boots.yml";
	static constexpr std::string_view x8boots = "armor/xm8-panama-combat-boots.yml";
	static constexpr std::string_view baklave = "armor/baklava.yml";
	static constexpr std::string_view bvest = "armor/basic-ballistic-vest.yml";
	static constexpr std::string_view bboots = "armor/basic-boots.yml";
	static constexpr std::string_view dtneck = "armor/dark-throne-necklace.yml";
	static constexpr std::string_view nvgoggles = "armor/night-vision-goggles.yml";
	static constexpr std::string_view rgpswatch = "armor/razor-gps-wrist-watch.yml";

	// RARE
	static constexpr std::string_view rbvest = "armor/raven-ballistic-vest.yml";
	static constexpr std::string_view teguards = "armor/titan-elbow-guards.yml";
	static constexpr std::string_view vbvest = "armor/vulture-ballistic-vest.yml";
	static constexpr std::string_view xmsbelt = "armor/xm-scorpio-belt.yml";

	/** TODO: fill these at runtime and allow admins to trigger a re-read */
	static std::vector<std::string> common_explosive = {
		"explosive/breach-charge.yml",
		"explosive/claymore-mine.yml",
		"explosive/corpse-charge.yml",
		"explosive/corrosive-claymore-mine.yml",
		"explosive/emp-grenade.yml",
		"explosive/engineer-drone-bomb.yml",
		"explosive/flashbang-grenade.yml",
		"explosive/frag-grenade.yml",
		"explosive/hellfire-corpse-charge.yml",
		"explosive/incendiary-grenade.yml",
		"explosive/sensor-grenade.yml",
		"explosive/shrapnel-claymore-mine.yml",
		"explosive/shrapnel-corpse-charge.yml",
		"explosive/smoke-grenade.yml",
		"explosive/thermite-charge.yml",
	};

	/** TODO: fill these at runtime and allow admins to trigger a re-read */
	static std::vector<std::string> rare_explosive = {
		"explosive/hellfire-claymore-mine.yml",
	};

	/** TODO: fill these at runtime and allow admins to trigger a re-read */
	static std::vector<std::string> legendary_explosive = {

	};

	/** TODO: fill these at runtime and allow admins to trigger a re-read */
	static std::vector<std::string> godtier_explosive = {


	};

	/**
	 * To generate this list, run:
	 * $MUD_ROOT/armor-by COMMON > loot-armor.common
	 * then read it :)
	 */
	/** TODO: fill these at runtime and allow admins to trigger a re-read */
	static std::vector<std::string> common_armor = {
		"armor/baklava.yml",
		"armor/basic-ballistic-vest.yml",
		"armor/basic-boots.yml",
		"armor/brown-leather-gloves.yml",
		"armor/dark-throne-necklace.yml",
		"armor/falcon-ballistic-vest.yml",
		"armor/leather-trenchcoat.yml",
		"armor/mp-enforcer-gloves.yml",
		"armor/mp-enforcer-pants.yml",
		"armor/night-vision-goggles.yml",
		"armor/p5-boots.yml",
		"armor/p5-pants.yml",
		"armor/p5-vest.yml",
		"armor/raven-ultralight-backpack.yml",
		"armor/razor-gps-wrist-watch.yml",
		"armor/tactical-gas-mask.yml",
		"armor/thermal-goggles.yml",
		"armor/titan-gauntlets.yml",
		"armor/titan-shin-guards.yml",
		"armor/viper-leg-guards.yml",
		"armor/xm50-ultralight-boots.yml",
		"armor/xm8-panama-combat-boots.yml",
	};

	/**
	 * To generate, run:
	 * :r ! ./rifle-by RARE
	 */
	/** TODO: fill these at runtime and allow admins to trigger a re-read */
	static std::vector<std::string> rare_rifles = {
		"rifle/belt-fed-minigun.yml",
		"rifle/czp10.yml",
		"rifle/defiler-scarh.yml",
		"rifle/desert-eagle.yml",
		"rifle/glock.yml",
		"rifle/magnum-revolver.yml",
		"rifle/ppk.yml",
		"rifle/ump45-vulture-modded.yml",
		"rifle/uzi.yml",
	};

	/**
	 * To generate, run:
	 * :r ! ./rifle-by COMMON
	 */
	/** TODO: fill these at runtime and allow admins to trigger a re-read */
	static std::vector<std::string> common_rifles = {
		"rifle/552-commando.yml",
		"rifle/aug-a3.yml",
		"rifle/augpara.yml",
		"rifle/famas.yml",
		"rifle/fmg9.yml",
		"rifle/g36c.yml",
		"rifle/hk21.yml",
		"rifle/hk45.yml",
		"rifle/l96aw.yml",
		"rifle/m16a4.yml",
		"rifle/m3.yml",
		"rifle/m4.yml",
		"rifle/mk46.yml",
		"rifle/mp5.yml",
		"rifle/mp9.yml",
		"rifle/p90.yml",
		"rifle/psg1.yml",
		"rifle/saiga12.yml",
		"rifle/sasg12.yml",
		"rifle/scarh.yml",
		"rifle/tar21.yml",
		"rifle/ump45.yml",
		"rifle/vc88.yml",
		"rifle/xm109.yml",
	};

	/**
	 * To generate, run:
	 * :r ! ./rifle-by LEGENDARY
	 */
	/** TODO: fill these at runtime and allow admins to trigger a re-read */
	static std::vector<std::string> legendary_rifles = {
		"rifle/bf-39-shotgun.yml",
		"rifle/defiler-fmg9.yml",
		"rifle/dst7a.yml",
	};

	/**
	 * To generate, run:
	 * :r ! ./rifle-by GOD_TIER
	 */
	/** TODO: fill these at runtime and allow admins to trigger a re-read */
	static std::vector<std::string> godtier_rifles = {
		"rifle/orthos-aug-a3.yml",
		"rifle/orthos-l96aw.yml",
		"rifle/orthos-scarh.yml",
		"rifle/orthos-sentinel-scarh.yml",
	};
	static std::vector<std::string> all_commons;
	void create_common_object_into(obj_ptr_t& container) {
		auto yaml = all_commons.at(rand_number(0,all_commons.size()) % all_commons.size());
		m_debug("create_common_object_into placing '" << yaml << "'");
		create_object_into(yaml,container);
	}
	bool attacker_was_player(player_ptr_t& victim) {
		auto uuid = victim->get_attacker();
		auto p = ptr_by_uuid(uuid);
		if(!p) {
			return false;
		}
		return p->is_npc() == false;
	}
	static std::vector<randomized_drop_t> drops;
	void random_drop_to(player_ptr_t& victim,obj_ptr_t& corpse) {
		if(!attacker_was_player(victim)) {
			return;
		}
		m_debug("Attacker was player for:" << victim->name().c_str());
		for(const auto& drop : drops) {
			if(drop.mob == victim->cd()->mob_specials.vnum) {
				if(roll(drop)) {
					create_object_into(drop.yaml,corpse);
				}
			}
		}
		if(dice(COMMON_DROP_DICE_COUNT(),
		        COMMON_DROP_DICE_SIDES()) >
		        COMMON_DROP_DICE_THRESHOLD()) {
			create_common_object_into(corpse);
		}
	}
	void common_drop_item_for_mob(std::string_view item, mob_vnum mob) {
		drops.emplace_back(mob,item,2,6,10);
	}
	void less_common_drop_item_for_mob(std::string_view item, mob_vnum mob) {
		drops.emplace_back(mob,item,2,7,11);
	}
	void rare_drop_item_for_mob(std::string_view item, mob_vnum mob) {
		drops.emplace_back(mob,item,10,20,120);
	}
	void legendary_drop_item_for_mob(std::string_view item, mob_vnum mob) {
		drops.emplace_back(mob,item,10,20,140);
	}
	void godtier_drop_item_for_mob(std::string_view item, mob_vnum mob) {
		drops.emplace_back(mob,item,10,20,150);
	}
	void easy_drop_item_for_mob(std::string_view item,mob_vnum mob) {
		drops.emplace_back(mob, item, 10,20,80);
	}
	void init() {
		drops.clear();
		common_drop_item_for_mob(frag,INFECTED_DRONE);
		common_drop_item_for_mob(claymore,INFECTED_DRONE);
		common_drop_item_for_mob(sensor,ADAPTED_DRONE);

		less_common_drop_item_for_mob(smoke,SHOPLIFTER);
		less_common_drop_item_for_mob(incendiary,SHOPLIFTER);
		less_common_drop_item_for_mob(smoke,SHOPLIFTER_2);
		less_common_drop_item_for_mob(incendiary,SHOPLIFTER_2);

		rare_drop_item_for_mob(tshin,DEFILER);

		for(const auto item : {
		            // GOD TIER items
		            tshould,
		            vshould,
		            pk3vest,
		            xmshould,
		            xscorpvest,
		            xscorpgloves,
		            xtgaunt,

		            cormine, xmshould, tshin, tshould, tgaunt,
		        }) {
			easy_drop_item_for_mob(item,DEFILER);
			/**
			 * TODO: figure out how hard it is to kill each ORTHOS type and adjust drop rate
			 */
			easy_drop_item_for_mob(item,ORTHOS_GUARD);
			easy_drop_item_for_mob(item,ORTHOS_MENACE);
			easy_drop_item_for_mob(item,ORTHOS_SENTINEL);
			easy_drop_item_for_mob(item,ORTHOS_SNIPER);
		}

		all_commons.clear();
		all_commons = vec::append(all_commons,common_rifles,common_armor);

	}

#undef m_error
#undef m_debug
};
