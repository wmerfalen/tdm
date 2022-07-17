#include "drops.hpp"
#include "super-users.hpp"
#include "interpreter.hpp"
#include "rand.hpp"
#include "str.hpp"
#include "filesystem.hpp"

#define __MENTOC_SHOW_MODS_drops_DEBUG_OUTPUT__
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

	static constexpr mob_vnum TENGU_SCOUT = 713;
	static constexpr mob_vnum TENGU_LANCER = 714;
	static constexpr mob_vnum TENGU_TROOPER = 715;
	static constexpr mob_vnum TENGU_EVISCERATOR = 716;

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

	static constexpr std::string_view hn3clay  =  "explosive/hn3-claymore-mine.yml";
	static constexpr std::string_view hn3shgren = "explosive/hn3-shock-grenade.yml";
	static constexpr std::string_view neumine =   "explosive/neutron-claymore-mine.yml";
	static constexpr std::string_view prflash =   "explosive/prador-flashbang-grenade.yml";

	static constexpr std::string_view acryo = "rifle/aug-cryo-mod.yml";
	static constexpr std::string_view amenace = "rifle/aug-menace.yml";
	static constexpr std::string_view avoidex = "rifle/aug-voidex.yml";
	static constexpr std::string_view bf39 = "rifle/bf-39-shotgun.yml";
	static constexpr std::string_view deffmg9 = "rifle/defiler-fmg9.yml";
	static constexpr std::string_view defscar = "rifle/defiler-scarh.yml";
	static constexpr std::string_view dst7a = "rifle/dst7a.yml";
	static constexpr std::string_view l96aw = "rifle/l96aw.yml";
	static constexpr std::string_view orthaug = "rifle/orthos-aug-a3.yml";
	static constexpr std::string_view orthl96 = "rifle/orthos-l96aw.yml";
	static constexpr std::string_view orthscar = "rifle/orthos-scarh.yml";
	static constexpr std::string_view orthsentscar = "rifle/orthos-sentinel-scarh.yml";
	static constexpr std::string_view tensaiga = "rifle/tengu-radon-saiga.yml";
	static constexpr std::string_view tenax13 = "rifle/tengu-vulkan-ax13.yml";
	static constexpr std::string_view umpvulture = "rifle/ump45-vulture-modded.yml";
	static constexpr std::string_view vc88 = "rifle/vc88.yml";
	static constexpr std::string_view xm109 = "rifle/xm109.yml";
	static constexpr std::string_view xtarar = "rifle/xtar21-assault-rifle.yml";
	static constexpr std::string_view xtareagle = "rifle/xtar21-eagle.yml";
	static constexpr std::string_view xtarsmg = "rifle/xtar21-vector-smg.yml";

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

	static std::vector<std::string> common_armors;
	static std::vector<std::string> uncommon_armors;
	static std::vector<std::string> rare_armors;
	static std::vector<std::string> legendary_armors;
	static std::vector<std::string> god_tier_armors;

	static std::vector<std::string> common_rifles;
	static std::vector<std::string> uncommon_rifles;
	static std::vector<std::string> rare_rifles;
	static std::vector<std::string> legendary_rifles;
	static std::vector<std::string> god_tier_rifles;

	static std::vector<std::string> common_explosives;
	static std::vector<std::string> uncommon_explosives;
	static std::vector<std::string> rare_explosives;
	static std::vector<std::string> legendary_explosives;
	static std::vector<std::string> god_tier_explosives;

	void create_common_object_into(obj_ptr_t& container) {
		static std::vector<std::string> types = {"R","A","E"};
		auto c = str(rand_item(types));
		std::string yaml;
		if(c.is("R")) {
			yaml = rand_item(common_rifles);
		} else if(c.is("A")) {
			yaml = rand_item(common_armors);
		} else if(c.is("E")) {
			yaml = rand_item(common_explosives);
		} else {
			log("Warning: this should never happen. Resorted to common_rifle");
			yaml = rand_item(common_rifles);
		}
		log("Dropping common object: %s",yaml.c_str());
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
					std::cerr << green_str("Dropping item for player: ") << drop.yaml << "\n";
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
		drops.emplace_back(mob,item,COMMON_DROP_DICE_COUNT(),COMMON_DROP_DICE_SIDES(),COMMON_DROP_DICE_THRESHOLD());
	}
	void less_common_drop_item_for_mob(std::string_view item, mob_vnum mob) {
		drops.emplace_back(mob,item,UNCOMMON_DROP_DICE_COUNT(),UNCOMMON_DROP_DICE_SIDES(),UNCOMMON_DROP_DICE_THRESHOLD());
	}
	void rare_drop_item_for_mob(std::string_view item, mob_vnum mob) {
		drops.emplace_back(mob,item,RARE_DROP_DICE_COUNT(),RARE_DROP_DICE_SIDES(),RARE_DROP_DICE_THRESHOLD());
	}
	void legendary_drop_item_for_mob(std::string_view item, mob_vnum mob) {
		drops.emplace_back(mob,item,LEGENDARY_DROP_DICE_COUNT(),LEGENDARY_DROP_DICE_SIDES(),LEGENDARY_DROP_DICE_THRESHOLD());
	}
	void godtier_drop_item_for_mob(std::string_view item, mob_vnum mob) {
		drops.emplace_back(mob,item,GOD_TIER_DROP_DICE_COUNT(),GOD_TIER_DROP_DICE_SIDES(),GOD_TIER_DROP_DICE_THRESHOLD());
	}
	void easy_drop_item_for_mob(std::string_view item,mob_vnum mob) {
		drops.emplace_back(mob, item, EASY_DROP_DICE_COUNT(),EASY_DROP_DICE_SIDES(),EASY_DROP_DICE_THRESHOLD());
	}
	/**
	 * Reads all yaml files and extracts rarities of items.
	 */
	std::vector<obj_ptr_t> parse_yamls_in_directory(std::string_view type) {
		auto directory = mods::yaml::directory(type);
		m_debug("directory: " << directory);
		std::vector<obj_ptr_t> objects;
		for(const auto& f : mods::util::glob(directory)) {
			if(mods::filesystem::is_directory(f)) {
				continue;
			}
			std::string file = str::alpha_only(type) + "/" + f;
			m_debug("file: " << file);
			auto obj = create_object(file);
			objects.emplace_back(obj);
		}
		return objects;
	}
	static constexpr std::array<std::string_view,18> overpowered_rifles = {
		acryo,
		amenace,
		avoidex,
		bf39,
		deffmg9,
		defscar,
		dst7a,
		orthaug,
		orthl96,
		orthscar,
		orthsentscar,
		tensaiga,
		tenax13,
		umpvulture,
		vc88,
		xtarar,
		xtareagle,
		xtarsmg,
	};

	void index_yaml_files() {
		drops.clear();
		common_drop_item_for_mob(frag,INFECTED_DRONE);
		common_drop_item_for_mob(claymore,INFECTED_DRONE);
		common_drop_item_for_mob(sensor,ADAPTED_DRONE);

		less_common_drop_item_for_mob(smoke,SHOPLIFTER);
		less_common_drop_item_for_mob(incendiary,SHOPLIFTER);
		less_common_drop_item_for_mob(smoke,SHOPLIFTER_2);
		less_common_drop_item_for_mob(incendiary,SHOPLIFTER_2);

		rare_drop_item_for_mob(tshin,DEFILER);
		for(const auto& item : overpowered_rifles) {
			godtier_drop_item_for_mob(item,DEFILER);
			godtier_drop_item_for_mob(item,ORTHOS_GUARD);
			godtier_drop_item_for_mob(item,ORTHOS_MENACE);
			godtier_drop_item_for_mob(item,ORTHOS_SENTINEL);
			godtier_drop_item_for_mob(item,ORTHOS_SNIPER);
			godtier_drop_item_for_mob(item,TENGU_SCOUT);
			godtier_drop_item_for_mob(item,TENGU_LANCER);
			godtier_drop_item_for_mob(item,TENGU_TROOPER);
			godtier_drop_item_for_mob(item,TENGU_EVISCERATOR);
		}
		for(const auto& item : {
		            hn3clay,
		            hn3shgren,
		            neumine,
		            prflash,
		        }) {
			legendary_drop_item_for_mob(item,DEFILER);
			legendary_drop_item_for_mob(item,ORTHOS_GUARD);
			legendary_drop_item_for_mob(item,ORTHOS_MENACE);
			legendary_drop_item_for_mob(item,ORTHOS_SENTINEL);
			legendary_drop_item_for_mob(item,ORTHOS_SNIPER);
			godtier_drop_item_for_mob(item,TENGU_SCOUT);
			less_common_drop_item_for_mob(item,TENGU_LANCER);
			common_drop_item_for_mob(item,TENGU_TROOPER);
			easy_drop_item_for_mob(item,TENGU_EVISCERATOR);
		}

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
			godtier_drop_item_for_mob(item,TENGU_SCOUT);
			less_common_drop_item_for_mob(item,TENGU_LANCER);
			common_drop_item_for_mob(item,TENGU_TROOPER);
			easy_drop_item_for_mob(item,TENGU_EVISCERATOR);
		}


		auto before_size = obj_list.size();

#define CRAWL_FOR(type) \
		for(auto& type : parse_yamls_in_directory(#type)) {\
			if(mods::rarity::is_common(type->type()->attributes->rarity)) {\
				BOOST_PP_CAT(BOOST_PP_CAT(common_,type),s).emplace_back(type->feed_file());\
			}\
			if(mods::rarity::is_uncommon(type->type()->attributes->rarity)) {\
				BOOST_PP_CAT(BOOST_PP_CAT(uncommon_,type),s).emplace_back(type->feed_file());\
			}\
			if(mods::rarity::is_rare(type->type()->attributes->rarity)) {\
				BOOST_PP_CAT(BOOST_PP_CAT(rare_,type),s).emplace_back(type->feed_file());\
			}\
			if(mods::rarity::is_legendary(type->type()->attributes->rarity)) {\
				BOOST_PP_CAT(BOOST_PP_CAT(legendary_,type),s).emplace_back(type->feed_file());\
			}\
			if(mods::rarity::is_god_tier(type->type()->attributes->rarity)) {\
				BOOST_PP_CAT(BOOST_PP_CAT(god_tier_,type),s).emplace_back(type->feed_file());\
			}\
			mods::globals::dispose_object(type->uuid);\
		}

		CRAWL_FOR(rifle);
		CRAWL_FOR(armor);
		CRAWL_FOR(explosive);

#undef CRAWL_FOR

		auto after_size = obj_list.size();

		assert(before_size == after_size);
	}

	SUPERCMD(do_reindex_yaml) {
		ADMIN_REJECT();
		player->sendln("[+] Starting re-index of yaml files...");
		index_yaml_files();
		player->sendln("[+] DONE");
	}


	void init() {
		index_yaml_files();
		ADD_BUILDER_COMMAND("admin:yaml:re-index",do_reindex_yaml);
	}

};
