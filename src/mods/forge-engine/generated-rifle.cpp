#include "generated-rifle.hpp"
#include "generator.hpp"
#include "../interpreter.hpp"
#include "../super-users.hpp"
#include "../filesystem.hpp"
#include "util.hpp"
#include "../orm/rifle-index.hpp"

#define __MENTOC_SHOW_MODS_FORGE_ENGINE_DEBUG_OUTPUT__
#ifdef __MENTOC_SHOW_MODS_FORGE_ENGINE_DEBUG_OUTPUT__
#define m_debug(MSG) mentoc_prefix_debug("[mods::forge_engine::generated_rifle]")  << MSG << "\n";
#else
#define m_debug(MSG) ;;
#endif

extern std::deque<char_data> mob_proto;

namespace mods::forge_engine {
	extern generator item_generator;
	static bool refresh_rifle_index = true;
	static std::vector<std::string> sub_machine_guns;
	static std::vector<std::string> sniper_rifles;
	static std::vector<std::string> light_machine_guns;
	static std::vector<std::string> pistols;
	static std::vector<std::string> assault_rifles;
	static std::vector<std::string> shotguns;
	static std::vector<std::string> machine_pistols;
	static std::vector<std::string> empty;

	using generated_attributes_t = std::vector<std::pair<rifle_attributes_t, std::variant<uint32_t, float>>>;

	/*
	 * Any time the rifle indexes change, you need to call this function.
	 */
	void rifle_index_changed() {
		refresh_rifle_index = true;
	}

	/**
	 * Forward declare this so we can call it from load_from_sql()
	 */
	void ral_loader();

	/**
	 * This loads the following items from sql:
	 * - the valid yaml files for all rifle types
	 * - rifle attribute limits (via ral_loader())
	 */
	void generated_rifle_t::load_from_sql() {
		static std::map<std::string,std::vector<std::string>> data;
		if(refresh_rifle_index) {
			mods::orm::load_all_rifle_index_data(&data);
			refresh_rifle_index = false;
		}
		for(auto& row : data) {
			if(row.first.compare("ar") == 0) {
				assault_rifles = row.second;
			}
			if(row.first.compare("smg") == 0) {
				sub_machine_guns = row.second;
			}
			if(row.first.compare("sniper") == 0) {
				sniper_rifles = row.second;
			}
			if(row.first.compare("lmg") == 0) {
				light_machine_guns = row.second;
			}
			if(row.first.compare("pistol") == 0) {
				pistols = row.second;
			}
			if(row.first.compare("shotgun") == 0) {
				shotguns = row.second;
			}
			if(row.first.compare("mp") == 0) {
				machine_pistols = row.second;
			}
		}
		ral_loader();
	}

	/**
	 * Pass in a rifle type, and it will return the list of
	 * yaml files that are valid for that rifle type.
	 */
	const std::vector<std::string>& generated_rifle_t::yaml_list(rifle_types_t t) {
		switch(t) {
			case RIFLE_TYPE_SHOTGUN:
				return shotguns;
			case RIFLE_TYPE_ASSAULT_RIFLE:
				return assault_rifles;
			case RIFLE_TYPE_SUB_MACHINE_GUN:
				return  sub_machine_guns;
			case RIFLE_TYPE_SNIPER:
				return sniper_rifles;
			case RIFLE_TYPE_HANDGUN:
			case RIFLE_TYPE_PISTOL:
				return pistols;
			case RIFLE_TYPE_MACHINE_PISTOL:
				return machine_pistols;
			case RIFLE_TYPE_LIGHT_MACHINE_GUN:
				return light_machine_guns;
			default:
				std::cerr << "[WARNING]: unknown rifle type at '" << __FILE__ << ":" << __LINE__ << ": type: '" << std::to_string(t) << "'\n";
				return empty;
		}
	}


	/**
	 * Initializes the object. Called from constructors
	 */
	void generated_rifle_t::init() {
		m_force_type = false;
		m_player = {};
		m_type = RIFLE_TYPE_SNIPER;
		m_requirements = {};
		m_attributes.clear();
		m_elemental_damages.clear();
		m_stat_boosts.clear();
		m_instance = nullptr;
	}

	/**
	 * Mostly used when converting rifle types from the varchar
	 * inside postgres on the rifle attribute limits table.
	 */
	rifle_types_t to_rifle_type_from_string(std::string_view t) {
		if(t.compare("SHOTGUN") == 0) {
			return rifle_types_t::RIFLE_TYPE_SHOTGUN;
		}
		if(t.compare("ASSAULT_RIFLE") == 0) {
			return rifle_types_t::RIFLE_TYPE_ASSAULT_RIFLE;
		}
		if(t.compare("SUB_MACHINE_GUN") == 0) {
			return rifle_types_t::RIFLE_TYPE_SUB_MACHINE_GUN;
		}
		if(t.compare("SNIPER") == 0) {
			return rifle_types_t::RIFLE_TYPE_SNIPER;
		}
		if(t.compare("HANDGUN") == 0) {
			return rifle_types_t::RIFLE_TYPE_HANDGUN;
		}
		if(t.compare("PISTOL") == 0) {
			return rifle_types_t::RIFLE_TYPE_PISTOL;
		}
		if(t.compare("MACHINE_PISTOL") == 0) {
			return rifle_types_t::RIFLE_TYPE_MACHINE_PISTOL;
		}
		if(t.compare("LIGHT_MACHINE_GUN") == 0) {
			return rifle_types_t::RIFLE_TYPE_LIGHT_MACHINE_GUN;
		}
		return rifle_types_t::RIFLE_TYPE_SNIPER;
	}

	/**
	 * Various functions to fill an object from the stats
	 * that were generated
	 */
	void generated_rifle_t::fill(obj_ptr_t& obj) {
		fill_attributes(obj);
		fill_elemental(obj);
		fill_stats(obj);
	}

	void generated_rifle_t::fill_stats(obj_ptr_t& obj) {
		fill_stats_generic(obj,m_stat_boosts,obj->rifle()->attributes.get());
	}
	void generated_rifle_t::fill_attributes(obj_ptr_t& obj) {
		fill_rifle_attributes_generic(obj,m_attributes,obj->rifle()->attributes.get());
	}
	void generated_rifle_t::fill_elemental(obj_ptr_t& obj) {
		fill_elemental_damage_generic(obj,m_elemental_damages,obj->rifle()->attributes.get());
	}

	/** -- end fill functions */


	/**
	 * -- constructors --
	 *
	 */
	/**
	 * pass the kill and the type of rifle to generate
	 */
	generated_rifle_t::generated_rifle_t (kill_t& kill,std::string_view type) {
		this->init();
		m_force_type = true;
		m_type = to_rifle_type_from_string(type);
		m_player = kill;
		load_from_sql();
	}
	/**
	 * pass the kill
	 */
	generated_rifle_t::generated_rifle_t (kill_t& kill) {
		this->init();
		m_player = kill;
		load_from_sql();
	}
	/**
	 * -- end constructors
	 */

	bool generated_rifle_t::compute_weapon_cost() {
		m_player.find_mob();
		/**
		 * Find the mob by the VNUM
		 */
		auto opt_mob = m_player.cd();
		if(opt_mob.has_value() == false) {
			::log("SYSERR: couldn't find mob[vnum:'%d'] to reward player[name:'%s']!", m_player.victim, m_player.killer->name().c_str());
			m_player.killer->sendln(
			    CAT("{red}######################################################{/red}\r\n",
			        "{red}Not rewarding a weapon because we couldn't find  {/red}\r\n",
			        "{red}the NPC you killed!                              {/red}\r\n",
			        "{red}Please report this to an admin.                  {/red}\r\n",
			        "{red}NPC vnum:",m_player.victim,"{/red}\r\n",
			        "{red}######################################################{/red}\r\n"
			       )
			);
			return false;
		}
		char_data& mob = opt_mob.value();
		/**
		 * Compute the cost of the weapon
		 */
		/**
		 * Player level multiplied by 10 is the base cost in MP
		 */
		m_player.set_cost(m_player.killer->level() * 10);

		/**
		 * How much exp does the NPC have?
		 */
		m_player.add_cost(mob.mob_specials.experience);

		/**
		 * How much MP does the NPC have?
		 */
		m_player.add_cost(mob.mob_specials.mp);

		/**
		 * How many hitpoints does the NPC have?
		 */
		m_player.add_cost(mob.points.max_hit / m_player.killer->max_hp());

		/**
		 * How high of a level is the NPC to the killer?
		 */
		m_player.add_cost(mob.player.level - m_player.killer->level());

		/**
		 * How much mana does the NPC have compared to the killer?
		 */
		m_player.add_cost(mob.points.max_mana - m_player.killer->max_mana());

		return true;
	}

	/**
	 * #########################################################################
	 * Giant Rifle attributes randomization code
	 * #########################################################################
	 */
	static const std::vector<rifle_attributes_t> higher_is_better = {
		RIFLE_ATTRIBUTES_AMMO_MAX,
		RIFLE_ATTRIBUTES_CHANCE_TO_INJURE,
		RIFLE_ATTRIBUTES_CLIP_SIZE,
		RIFLE_ATTRIBUTES_CRITICAL_CHANCE,
		RIFLE_ATTRIBUTES_BASE_DAMAGE,
		RIFLE_ATTRIBUTES_DISORIENT_AMOUNT,
		RIFLE_ATTRIBUTES_HEADSHOT_BONUS,
		RIFLE_ATTRIBUTES_RANGE_MULTIPLIER,
		RIFLE_ATTRIBUTES_ROUNDS_PER_MINUTE,
		RIFLE_ATTRIBUTES_MUZZLE_VELOCITY,
		RIFLE_ATTRIBUTES_DAMAGE_DICE_COUNT,
		RIFLE_ATTRIBUTES_DAMAGE_DICE_SIDES
	};

	static const std::vector<rifle_attributes_t> lower_is_better = {
		RIFLE_ATTRIBUTES_COOLDOWN_BETWEEN_SHOTS,
		RIFLE_ATTRIBUTES_CRITICAL_RANGE,
		RIFLE_ATTRIBUTES_RELOAD_TIME,
		RIFLE_ATTRIBUTES_DAMAGE_DICE_COUNT,
		RIFLE_ATTRIBUTES_DAMAGE_DICE_SIDES
	};

	static const std::vector<rifle_attributes_t> balanced_is_better = {
		RIFLE_ATTRIBUTES_EFFECTIVE_FIRING_RANGE,
		RIFLE_ATTRIBUTES_MAX_RANGE,
	};



	static std::vector<attribute_limits_t> pistol_limits;
	static std::vector<attribute_limits_t> sniper_limits;
	static std::vector<attribute_limits_t> ar_limits;
	static std::vector<attribute_limits_t> smg_limits;
	static std::vector<attribute_limits_t> lmg_limits;
	static std::vector<attribute_limits_t> shotgun_limits;
	static std::vector<attribute_limits_t> machine_pistol_limits;
	static attribute_limits_t no_attribute = {__NO_ATTRIBUTE__,0,0,0};

	/**
	 * Call this to load all attr limit data from the db
	 */
	void ral_loader() {
		pistol_limits.clear();
		sniper_limits.clear();
		ar_limits.clear();
		smg_limits.clear();
		lmg_limits.clear();
		shotgun_limits.clear();
		machine_pistol_limits.clear();
		auto rows = mods::orm::admin::load_all_rifle_attribute_limits();
		for(const auto& r : rows) {
			attribute_limits_t limit;
			limit.attribute = from_string(r.attribute);
			limit.low = r.low;
			limit.high = r.high;
			limit.overpowered = r.overpowered;
			m_debug(r.attribute << " id: " << r.id << ", low:" << r.low << ", high:" << r.high << ", op:" << r.overpowered << ", type:'" << r.type << "'");
			if(r.type.compare("SHOTGUN") == 0) {
				shotgun_limits.emplace_back(limit);
			}
			if(r.type.compare("PISTOL") == 0) {
				pistol_limits.emplace_back(limit);
			}
			if(r.type.compare("MACHINE_PISTOL") == 0) {
				machine_pistol_limits.emplace_back(limit);
			}
			if(r.type.compare("AR") == 0) {
				ar_limits.emplace_back(limit);
			}
			if(r.type.compare("SMG") == 0) {
				smg_limits.emplace_back(limit);
			}
			if(r.type.compare("LMG") == 0) {
				lmg_limits.emplace_back(limit);
			}
			if(r.type.compare("SNIPER") == 0) {
				sniper_limits.emplace_back(limit);
			}
		}
	}
	bool is_no_attribute(const auto& m) {
		return m.attribute == __NO_ATTRIBUTE__;
	}
	attribute_limits_t fetch_attribute_limit(const std::vector<attribute_limits_t>& limits, const rifle_attributes_t& which) {
		for(const auto& limit : limits) {
			if(limit.attribute == which) {
				return limit;
			}
		}
		return no_attribute;
	}
	attribute_limits_t generated_rifle_t::fetch_limits(const rifle_attributes_t& which) {
		switch(m_type) {
			case rifle_types_t::RIFLE_TYPE_SHOTGUN:
				return fetch_attribute_limit(shotgun_limits,which);
			case rifle_types_t::RIFLE_TYPE_ASSAULT_RIFLE:
				return fetch_attribute_limit(ar_limits,which);
			case rifle_types_t::RIFLE_TYPE_SUB_MACHINE_GUN:
				return fetch_attribute_limit(smg_limits,which);
			case rifle_types_t::RIFLE_TYPE_SNIPER:
				return fetch_attribute_limit(sniper_limits,which);
			case rifle_types_t::RIFLE_TYPE_HANDGUN:
			case rifle_types_t::RIFLE_TYPE_PISTOL:
				return fetch_attribute_limit(pistol_limits,which);
			case rifle_types_t::RIFLE_TYPE_MACHINE_PISTOL:
				return fetch_attribute_limit(machine_pistol_limits,which);
			case rifle_types_t::RIFLE_TYPE_LIGHT_MACHINE_GUN:
				return fetch_attribute_limit(lmg_limits,which);
			default:
				::log("SYSERR: invalid rifle type('%d') given to fetch_limits()",m_type);
				break;
		}
		return no_attribute;
	}

	generated_attributes_t generated_rifle_t::generate_rifle_attributes() {
		//auto victim = kill.victim;
		value_scaler scale(m_player.killer);
		m_attributes = {};
		if(valid_rifle_attributes.size() == 0) {
			return m_attributes;
		}
		uint8_t i = std::clamp((uint8_t)rand_xoroshiro(), (uint8_t)0, (uint8_t)(scale.max_stats));
		if(!i) {
			return m_attributes;
		}
		while(i-- > 0) {
			const auto& attr = valid_rifle_attributes.at((uint16_t)rand_xoroshiro() % valid_rifle_attributes.size());
			auto limit = this->fetch_limits(attr);
			auto low = limit.low;
			auto high = limit.high;
			auto op = limit.overpowered;

			if(roll_bool()) {
				auto float_roll = roll_float(scale.stat_low,scale.stat_high);
				if(is_no_attribute(limit)) {
					m_attributes.emplace_back(attr,float_roll);
					continue;
				}
				bool is_overpowered = roll_bool();
				if(is_overpowered) {
					m_attributes.emplace_back(attr,roll_float(scale.stat_low,roll_float(scale.stat_high,op)));
				} else {
					m_attributes.emplace_back(attr,std::clamp(float_roll,(float)low,(float)high));
				}
			} else {
				auto int_roll = roll_between<uint32_t> (scale.uint_low,scale.uint_high);
				if(is_no_attribute(limit)) {
					m_attributes.emplace_back(attr,int_roll);
					continue;
				}
				bool is_overpowered = roll_bool();
				if(is_overpowered) {
					m_attributes.emplace_back(attr,roll_between<uint32_t>(scale.uint_low,roll_between(scale.uint_high,(uint32_t)op)));
				} else {
					m_attributes.emplace_back(attr,std::clamp(int_roll,(uint32_t)low,(uint32_t)high));
				}
			}
		}
		return m_attributes;
	}

	void generated_rifle_t::log_section(std::string_view section) {
		m_section = section;
	}

	void generated_rifle_t::log_gen(std::string_view msg) {
		mods::filesystem::append_to_forge_engine_log_file(to_string(m_type), CAT("[",m_section,"]",msg.data()));
	}

	/**
	 * Roll an overpowered stat
	 */
	bool generated_rifle_t::roll_overpowered(std::string_view which_stat) {
		bool ok = roll_bool();
		if(ok) {
			log_gen(CAT("[",which_stat.data(),"]: Rolled successful overpowered!"));
		} else {
			log_gen(CAT("[",which_stat.data(),"]: Failed overpower roll."));
		}
		return ok;
	}

	/**
	 * This function is ideally going to be the catalyst to what might
	 * be a domain specific language. Currently, I document the logic
	 * for this function at src/doc/balanc-op-loot-ebnf.txt.
	 * While it would be ideal to have this as a DSL for content creators,
	 * the need right now is not urgent, mainly because I'm the only one
	 * creating content. When we get more builders and need a DSL, then
	 * we can convert this logic into an interpretted grammar.
	 */
	void generated_rifle_t::balance_rifle_attributes() {
		// - sniper:
		// 	- (if high) EFR + ZMAG, then ->
		//			CBS (nerf raise) (op lower)

		if(m_type == rifle_types_t::RIFLE_TYPE_SNIPER) {
			/**
			 * Balance a sniper rifle
			 */
			/**
			 * Balance ratio for effective_firing_range + range_multiplier to
			 * cooldown_between_shots.
			 *
			 * This means cooldown_between_shots multiplied by COOLDOWN_BETWEEN_SHOTS_RATIO
			 * must always equal effective_firing_range + range_multiplier
			 * But __ONLY__ if we are balancing the rifle. If we are making the rifle
			 * overpowered, then this stat does not come into play.
			 */
			static constexpr float COOLDOWN_BETWEEN_SHOTS_LIMITER = 2.5;


			auto attr = m_instance->rifle()->attributes.get();
			log_section("cooldown 1");
			if(roll_overpowered("cooldown_between_shots")) {
				/**
				 * [ MAKE OVERPOWERED ]
				 * If the effective_firing_range or range_multiplier is high, then
				 * buff (decrease) the cooldown_between_shots
				 */
				log_gen(CAT(" generating OP stats for cooldown_between_shots"));
				if(attr->cooldown_between_shots >= (attr->effective_firing_range + attr->range_multiplier) / COOLDOWN_BETWEEN_SHOTS_LIMITER) {
					uint32_t deduction = 0;
					int cbs = attr->cooldown_between_shots - deduction;
					auto before = attr->cooldown_between_shots;
					if(cbs <= 0) {
						attr->cooldown_between_shots = 3;
					} else {
						attr->cooldown_between_shots -= deduction;
					}
					log_gen(CAT("{OP} cooldown_between_shots BEFORE:'",before,"' AFTER:'",attr->cooldown_between_shots,"'"));
				}
			} else {
				/**
				 * [ BALANCE ]
				 * If the effective_firing_range is high, or the range_multiplier is high,
				 * then nerf (increase) the cooldown_between_shots.
				 */
				log_gen(CAT("*NOT* generating OP stats for cooldown_between_shots"));
				auto before = attr->cooldown_between_shots;
				if(attr->cooldown_between_shots < (attr->effective_firing_range + attr->range_multiplier) / COOLDOWN_BETWEEN_SHOTS_LIMITER) {
					attr->cooldown_between_shots = (attr->effective_firing_range + attr->range_multiplier) / COOLDOWN_BETWEEN_SHOTS_LIMITER;
				}
				log_gen(CAT("{BALANCE} cooldown_between_shots BEFORE:'",before,"' AFTER:'",attr->cooldown_between_shots,"'"));
			}

			//	- (if has) ELEM_SHRAPNEL, then ->
			//			ZMAG (nerf zero) (op higher)
			//			MXRNG (nerf (clamp,3)) (op raise)

			log_section("shrapnel eq");
			if(attr->shrapnel_damage) {
				log_gen(CAT("sniper rifle has shrapnel_damage of:'",attr->shrapnel_damage,"'. balance/op rule in effect"));
				auto before = attr->range_multiplier;
				auto range_before = attr->max_range;
				if(roll_overpowered("range_multiplier")) {
					/**
					 * [ MAKE OVERPOWERED ]
					 * Add a range_multiplier between 2 and 4
					 */
					attr->range_multiplier += rand_number(2,4);
					log_gen(CAT("{OP} range_multiplier BEFORE:'",before,"' AFTER:'",attr->range_multiplier,"'"));
					attr->max_range += rand_number(1,6);
					log_gen(CAT("{OP} max_range BEFORE:'",range_before,"' AFTER:'",attr->max_range,"'"));
				} else {
					/**
					 * [ BALANCE ]
					 * If sniper rifle has shrapnel damage, nerf (turn to zero) the range_multiplier
					 * and nerf (clamp) max_range to 3
					 */
					if(attr->range_multiplier > 3) {
						attr->range_multiplier = 0;
					}
					if(attr->max_range > 3) {
						attr->max_range = 3;
					}
					log_gen(CAT("{BALANCE} range_multiplier BEFORE:'",before,"' AFTER:'",attr->range_multiplier,"'"));
					log_gen(CAT("{BALANCE} max_range BEFORE:'",range_before,"' AFTER:'",attr->max_range,"'"));
				}
			}
			//	- (if high) CLIPSZ (and low) CBS, then ->
			//			[*ALL-DMG-ATTRS*] (nerf lower) (op raise)
			static constexpr uint8_t CLIP_SIZE_THRESHOLD = 5;
			static constexpr uint8_t FAST_COOLDOWN_BETWEEN_SHOTS = 9;
			log_section("clip size 1");
			if(attr->clip_size > CLIP_SIZE_THRESHOLD && attr->cooldown_between_shots <= FAST_COOLDOWN_BETWEEN_SHOTS) {
				log_gen(CAT("detected low clip_size of: '",attr->clip_size,"' taking action.."));
				auto before = attr->clip_size;
				if(roll_overpowered("clip_size")) {
					/**
					 * [ MAKE OVERPOWERED ]
					 * If clip size is fairly low, to make this weapon overpowered, we
					 * increase the clip size
					 */
					attr->clip_size += rand_number(3,4);
					log_gen(CAT("{OP} clip_size BEFORE:'",before,"' AFTER:'",attr->clip_size,"'"));
					{
						auto base_dam_before = attr->base_damage;
						attr->base_damage += rand_number(15,120);
						log_gen(CAT("{OP} base_damage BEFORE:'",base_dam_before,"' AFTER:'",attr->base_damage,"'"));
					}
					{
						auto dds_before = attr->damage_dice_sides;
						attr->damage_dice_sides += rand_number(15,120);
						log_gen(CAT("{OP} damage_dice_sides BEFORE:'",dds_before,"' AFTER:'",attr->damage_dice_sides,"'"));
					}
					{
						auto ddc_before = attr->damage_dice_count;
						attr->damage_dice_count += rand_number(3,22);
						log_gen(CAT("{OP} damage_dice_count BEFORE:'",ddc_before,"' AFTER:'",attr->damage_dice_count,"'"));
					}
				} else {
					/**
					 * [ BALANCE ]
					 * High clip size and low cooldown need all damage output to be lowered.
					 * It's simply way too OP to have that combination with a high damage output.
					 */

					{
						/**
						 * [ FIRST ]
						 * Reduce base_damage
						 */
						auto before = attr->base_damage;
						attr->base_damage = (attr->clip_size * 10) + (rand_number(1,20));
						log_gen(CAT("{BALANCE} base_damage BEFORE:'",before,"' AFTER:'",attr->base_damage,"'"));
					}
					{
						/**
						 * [ SECOND ]
						 * Reduce damage_dice_count
						 */
						auto before = attr->damage_dice_count;
						attr->damage_dice_count = (attr->clip_size / 2.5) + (rand_number(1,8));
						log_gen(CAT("{BALANCE} damage_dice_count BEFORE:'",before,"' AFTER:'",attr->damage_dice_count,"'"));
					}
				}
			}

			//	- (if low) CLIPSZ, then ->
			//			[*ALL-DMG-ATTRS*] (nerf raise) (op (mult,2))
			//			CLIPSZ (op (mult,2))
			static constexpr uint8_t LOW_CLIP_SIZE_THRESHOLD = 5;
			log_section("clip size 2");
			if(attr->clip_size < LOW_CLIP_SIZE_THRESHOLD) {
				log_gen(CAT("detected low clip_size of: '",attr->clip_size,"' taking action.."));
				if(roll_overpowered("clip_size")) {
					auto before = attr->clip_size;
					/**
					 * [ MAKE OVERPOWERED ]
					 * If clip size is fairly low, to make this weapon overpowered, we
					 * increase the clip size
					 */
					attr->clip_size += rand_number(3,4);
					log_gen(CAT("{OP} clip_size BEFORE:'",before,"' AFTER:'",attr->clip_size,"'"));
				} else {
					/**
					 * [ BALANCE ]
					 * High clip size and low cooldown need all damage output to be lowered.
					 * It's simply way too OP to have that combination with a high damage output.
					 */

					{
						/**
						 * [ FIRST ]
						 * Reduce base_damage
						 */
						auto before = attr->base_damage;
						attr->base_damage = (attr->clip_size * 10) + (rand_number(1,20));
						log_gen(CAT("{BALANCE} base_damage BEFORE:'",before,"' AFTER:'",attr->base_damage,"'"));
					}
					{
						/**
						 * [ SECOND ]
						 * Reduce damage_dice_count
						 */
						auto before = attr->damage_dice_count;
						attr->damage_dice_count = (attr->clip_size / 2.5) + (rand_number(1,8));
						log_gen(CAT("{BALANCE} damage_dice_count BEFORE:'",before,"' AFTER:'",attr->damage_dice_count,"'"));
					}
				}
			}
			log_section("");
		}// End rifle type of sniper

		/**
		 * General rules around shotguns
		 *
		 * [ BALANCE ] if high damage, then CBS += rand()
		 * [ OP ] if high damage, then lower CBS
		 *
		 * [BALANCE] if high damage, then maxrange clamp(2)
		 * [OP] if high damage, then maxrange += rand(1,3)
		 *
		 * [BALANCE] if low damage, then lower CBS
		 * [OP] if low damage, then maxrange += rand(1,3)
		 *
		 * [BALANCE] if low CBS, then lower [*ALL-DAMAGES*]
		 * [OP] if low CBS, then raise [*ALL-DAMAGES*]
		 *
		 * [BALANCE] if low reloadtime, then lower clipsize
		 * [OP] if low reloadtime, then raise clipsize
		 *
		 * [BALANCE] if low efr, then raise dicecount, lower dicesides
		 * [OP] if low efr, then raise dicecount, raise dicesides
		 *
		 * [BALANCE] if critrange > 1, then maxrange clamp(2)
		 * [OP] if critrange > 1, then maxrange += rand(1,3)
		 *
		 * [LUCKY] if headshotbonus == 0, then headshotbonus = scaled_headshot_bonus(player)
		 * [BALANCE] if headshotbonus > 0, then critchance = headshotbonus / 2
		 * [OP] if headshotbonus > 0, then critchance = scaled_op_critchance(player)
		 *
		 * [LUCKY] if cryo == 0, then cryo = rand(50,350)
		 * [BALANCE] if incendiary > 0 and cryo > 0, then
		 * 		if (rand(1,10) % 2 == 0){
		 * 			incendiary = 0
		 * 		}else{
		 * 			cryo = 0
		 * 		}
		 * [BALANCE] if cryo > 0 and incendiary > 0, then incendiary /= 2
		 */
	}
	uint16_t generated_rifle_t::clamp_critical_range() {
		auto attr = m_instance->rifle()->attributes.get();
		return clamp_using(attr->critical_range,critical_range());
	}
	uint16_t generated_rifle_t::clamp_range_multiplier() {
		auto attr = m_instance->rifle()->attributes.get();
		return clamp_using(attr->range_multiplier,range_multiplier());
	}
	uint16_t generated_rifle_t::clamp_chance_to_injure() {
		auto attr = m_instance->rifle()->attributes.get();
		return clamp_using(attr->chance_to_injure,chance_to_injure());
	}
	uint16_t generated_rifle_t::clamp_cooldown_between_shots() {
		auto attr = m_instance->rifle()->attributes.get();
		return clamp_using(attr->cooldown_between_shots,cooldown_between_shots());
	}
	uint16_t generated_rifle_t::clamp_damage_dice_count() {
		auto attr = m_instance->rifle()->attributes.get();
		return clamp_using(attr->damage_dice_count,damage_dice_count());
	}
	uint16_t generated_rifle_t::clamp_damage_dice_sides() {
		auto attr = m_instance->rifle()->attributes.get();
		return clamp_using(attr->damage_dice_sides,damage_dice_sides());
	}
	uint16_t generated_rifle_t::clamp_ammo_max() {
		auto attr = m_instance->rifle()->attributes.get();
		return clamp_using(attr->ammo_max,ammo_max());
	}
	uint16_t generated_rifle_t::clamp_clip_size() {
		auto attr = m_instance->rifle()->attributes.get();
		return clamp_using(attr->clip_size,clip_size());
	}
	uint16_t generated_rifle_t::clamp_critical_chance() {
		auto attr = m_instance->rifle()->attributes.get();
		return clamp_using(attr->critical_chance,critical_chance());
	}
	uint16_t generated_rifle_t::clamp_disorient_amount() {
		auto attr = m_instance->rifle()->attributes.get();
		return clamp_using(attr->disorient_amount,disorient_amount());
	}
	uint16_t generated_rifle_t::clamp_headshot_bonus() {
		auto attr = m_instance->rifle()->attributes.get();
		return clamp_using(attr->headshot_bonus,headshot_bonus());
	}
	uint16_t generated_rifle_t::clamp_max_range() {
		auto attr = m_instance->rifle()->attributes.get();
		return clamp_using(attr->max_range,max_range());
	}
	uint16_t generated_rifle_t::clamp_reload_time() {
		auto attr = m_instance->rifle()->attributes.get();
		return clamp_using(attr->reload_time,reload_time());
	}
	uint16_t generated_rifle_t::clamp_rounds_per_minute() {
		auto attr = m_instance->rifle()->attributes.get();
		return clamp_using(attr->rounds_per_minute,rounds_per_minute());
	}
	uint16_t generated_rifle_t::clamp_muzzle_velocity() {
		auto attr = m_instance->rifle()->attributes.get();
		return clamp_using(attr->muzzle_velocity,muzzle_velocity());
	}
	uint16_t generated_rifle_t::clamp_effective_firing_range() {
		auto attr = m_instance->rifle()->attributes.get();
		return clamp_using(attr->effective_firing_range,effective_firing_range());
	}
	uint16_t generated_rifle_t::clamp_incendiary_damage() {
		auto attr = m_instance->rifle()->attributes.get();
		return clamp_using(attr->incendiary_damage,incendiary_damage());
	}
	uint16_t generated_rifle_t::clamp_explosive_damage() {
		auto attr = m_instance->rifle()->attributes.get();
		return clamp_using(attr->explosive_damage,explosive_damage());
	}
	uint16_t generated_rifle_t::clamp_shrapnel_damage() {
		auto attr = m_instance->rifle()->attributes.get();
		return clamp_using(attr->shrapnel_damage,shrapnel_damage());
	}
	uint16_t generated_rifle_t::clamp_corrosive_damage() {
		auto attr = m_instance->rifle()->attributes.get();
		return clamp_using(attr->corrosive_damage,corrosive_damage());
	}
	uint16_t generated_rifle_t::clamp_cryogenic_damage() {
		auto attr = m_instance->rifle()->attributes.get();
		return clamp_using(attr->cryogenic_damage,cryogenic_damage());
	}
	uint16_t generated_rifle_t::clamp_radioactive_damage() {
		auto attr = m_instance->rifle()->attributes.get();
		return clamp_using(attr->radioactive_damage,radioactive_damage());
	}
	uint16_t generated_rifle_t::clamp_emp_damage() {
		auto attr = m_instance->rifle()->attributes.get();
		return clamp_using(attr->emp_damage,emp_damage());
	}
	uint16_t generated_rifle_t::clamp_shock_damage() {
		auto attr = m_instance->rifle()->attributes.get();
		return clamp_using(attr->shock_damage,shock_damage());
	}
	uint16_t generated_rifle_t::clamp_anti_matter_damage() {
		auto attr = m_instance->rifle()->attributes.get();
		return clamp_using(attr->anti_matter_damage,anti_matter_damage());
	}
	uint16_t generated_rifle_t::clamp_base_damage() {
		auto attr = m_instance->rifle()->attributes.get();
		return clamp_using(attr->base_damage,base_damage());
	}

	void generated_rifle_t::clamp_rifle_attributes() {
		auto attr = m_instance->rifle()->attributes.get();
		//static constexpr uint16_t MAX_CRITICAL_RANGE = 16;
		log_section("clamp");
		log_gen("clamp check");
		log_gen(CAT("critical_range clamping in progress: ",attr->critical_range));
		attr->critical_range = clamp_critical_range();
		log_gen(CAT("range_multiplier clamping in progress: ",attr->range_multiplier));
		attr->range_multiplier = clamp_range_multiplier();
		log_gen(CAT("chance_to_injure clamping in progress: ",attr->chance_to_injure));
		attr->chance_to_injure = clamp_chance_to_injure();
		log_gen(CAT("cooldown_between_shots clamping in progress: ",attr->cooldown_between_shots));
		attr->cooldown_between_shots = clamp_cooldown_between_shots();
		log_gen(CAT("damage_dice_count clamping in progress: ",attr->damage_dice_count));
		attr->damage_dice_count = clamp_damage_dice_count();
		log_gen(CAT("damage_dice_sides clamping in progress: ",attr->damage_dice_sides));
		attr->damage_dice_sides = clamp_damage_dice_sides();
		log_gen(CAT("ammo_max clamping in progress: ",attr->ammo_max));
		attr->ammo_max = clamp_ammo_max();
		log_gen(CAT("clip_size clamping in progress: ",attr->clip_size));
		attr->clip_size = clamp_clip_size();
		log_gen(CAT("critical_chance clamping in progress: ",attr->critical_chance));
		attr->critical_chance = clamp_critical_chance();
		log_gen(CAT("base_damage clamping in progress: ",attr->base_damage));
		attr->base_damage = clamp_base_damage();
		log_gen(CAT("disorient_amount clamping in progress: ",attr->disorient_amount));
		attr->disorient_amount = clamp_disorient_amount();
		log_gen(CAT("headshot_bonus clamping in progress: ",attr->headshot_bonus));
		attr->headshot_bonus = clamp_headshot_bonus();
		log_gen(CAT("max_range clamping in progress: ",attr->max_range));
		attr->max_range = clamp_max_range();
		log_gen(CAT("reload_time clamping in progress: ",attr->reload_time));
		attr->reload_time = clamp_reload_time();
		log_gen(CAT("rounds_per_minute clamping in progress: ",attr->rounds_per_minute));
		attr->rounds_per_minute = clamp_rounds_per_minute();
		log_gen(CAT("muzzle_velocity clamping in progress: ",attr->muzzle_velocity));
		attr->muzzle_velocity = clamp_muzzle_velocity();
		log_gen(CAT("effective_firing_range clamping in progress: ",attr->effective_firing_range));
		attr->effective_firing_range = clamp_effective_firing_range();
		log_gen(CAT("incendiary_damage clamping in progress: ",attr->incendiary_damage));
		attr->incendiary_damage = clamp_incendiary_damage();
		log_gen(CAT("explosive_damage clamping in progress: ",attr->explosive_damage));
		attr->explosive_damage = clamp_explosive_damage();
		log_gen(CAT("shrapnel_damage clamping in progress: ",attr->shrapnel_damage));
		attr->shrapnel_damage = clamp_shrapnel_damage();
		log_gen(CAT("corrosive_damage clamping in progress: ",attr->corrosive_damage));
		attr->corrosive_damage = clamp_corrosive_damage();
		log_gen(CAT("cryogenic_damage clamping in progress: ",attr->cryogenic_damage));
		attr->cryogenic_damage = clamp_cryogenic_damage();
		log_gen(CAT("radioactive_damage clamping in progress: ",attr->radioactive_damage));
		attr->radioactive_damage = clamp_radioactive_damage();
		log_gen(CAT("emp_damage clamping in progress: ",attr->emp_damage));
		attr->emp_damage = clamp_emp_damage();
		log_gen(CAT("shock_damage clamping in progress: ",attr->shock_damage));
		attr->shock_damage = clamp_shock_damage();
		log_gen(CAT("anti_matter_damage clamping in progress: ",attr->anti_matter_damage));
		attr->anti_matter_damage = clamp_anti_matter_damage();

		log_section("");
	}
	obj_ptr_t generated_rifle_t::roll() {

		log_section("");
		log_gen("-[START]----------------------------------------------------------");
		if(!compute_weapon_cost()) {
			::log("WARNING: rolling for a piece of loot without sufficient cost adjustments");
		}

		if(!m_force_type) {
			m_type = mods::forge_engine::item_generator.random_rifle_type(m_player.killer);
		}
		m_requirements = mods::forge_engine::item_generator.generate_requirements(m_player);
		m_attributes = this->generate_rifle_attributes();
		m_elemental_damages = mods::forge_engine::item_generator.generate_rifle_elemental_boosts(m_player);
		m_stat_boosts = mods::forge_engine::item_generator.generate_rifle_stat_boosts(m_player);

		m_instance = create_object(ITEM_RIFLE,random_yaml(yaml_list(m_type)));
		this->fill(m_instance);
		this->balance_rifle_attributes();
		this->clamp_rifle_attributes();
		m_instance->forged = true;
		log_gen("-[ END ]----------------------------------------------------------");
		log_section("");
		return m_instance;
	}

	void generated_rifle_t::send_stats_to_player(player_ptr_t& player) {
		player->sendln(get_dump().c_str());
	}

	std::string generated_rifle_t::get_dump() {
		std::string output;
		output += "---- start of dump ----\r\n";
		output += CAT("random rifle type: ", to_string(m_type), "\r\n");

		for(auto& req : m_attributes) {
			if(std::holds_alternative<float> (req.second)) {
				output += CAT("attribute: ", to_string(req.first), ": ", std::to_string(std::get<float> (req.second)), "\r\n");
			} else {
				output += CAT("attribute: ", to_string(req.first), ": ", std::to_string(std::get<uint32_t> (req.second)), "\r\n");
			}
		}

		for(auto& req : m_stat_boosts) {
			if(std::holds_alternative<float> (req.second)) {
				output += CAT("stat_boost: ", to_string(req.first), ": ", std::to_string(std::get<float> (req.second)), "\r\n");
			} else if(std::holds_alternative<uint32_t> (req.second)) {
				output += CAT("stat_boost: ", to_string(req.first), ": ", std::to_string(std::get<uint32_t> (req.second)), "\r\n");
			}
		}

		for(auto& req : m_elemental_damages) {
			if(std::holds_alternative<float> (req.second)) {
				output += CAT("stat_boost: ", to_string(req.first), ": ", std::to_string(std::get<float> (req.second)), "\r\n");
			} else if(std::holds_alternative<uint32_t> (req.second)) {
				output += CAT("stat_boost: ", to_string(req.first), ": ", std::to_string(std::get<uint32_t> (req.second)), "\r\n");
			}
		}

		output += CAT("\r\n", get_requirements_string(m_requirements), "\r\n");
		output += "---- End of dump ----\r\n";
		return output;
	}

};//end namespace mods::forge_engine
#undef m_debug
