#include "rifle-stats.hpp"
#include "stats.hpp"
#include "str.hpp"
#define __MENTOC_SHOW_MODS_STATS_RIFLE_DEBUG_OUTPUT__
#ifdef __MENTOC_SHOW_MODS_STATS_RIFLE_DEBUG_OUTPUT__
#define m_debug(MSG) mentoc_prefix_debug("[mods::rifle-stats::debug]")  << MSG << "\n";
#define m_error(MSG) mentoc_prefix_debug(red_str("[mods::rifle-stats::ERROR]"))  << MSG << "\n";
#else
#define m_debug(MSG) ;;
#define m_error(MSG) ;;
#endif


namespace mods::rifle_stats {
	static constexpr std::string_view accuracy_map = "{grn}accuracy{/grn}: ";
	static constexpr std::string_view ammo = "{grn}ammo{/grn}: ";
	static constexpr std::string_view base_damage = "{grn}base{/grn}: ";
	static constexpr std::string_view critical= "{grn}critical{/grn}: ";
	static constexpr std::string_view injure= "{grn}injure{/grn}: ";
	static constexpr std::string_view cooldown = "{grn}cooldown{/grn}: ";
	static constexpr std::string_view damage = "{grn}damage{/grn}: ";

	enum e : uint16_t {
		i_manufacturer,
		i_title,
		i_hr,
		i_base_damage,
		i_damage,
		i_critical,
		i_injure,
		i_ammo,
		i_cooldown,
		i_accuracy,
		i_effective_range,
		i_max_range,
		i_range_mult,
		i_hs_bonus,
		i_a_map,
		i_ele_incendiary_damage,
		i_ele_explosive_damage,
		i_ele_shrapnel_damage,
		i_ele_corrosive_damage,
		i_ele_cryogenic_damage,
		i_ele_radioactive_damage,
		i_ele_emp_damage,
		i_ele_shock_damage,
		i_ele_anti_matter_damage,
		i_disorient,
		i_stat_armor,
		i_stat_chemistry,
		i_stat_constitution,
		i_stat_demolitions,
		i_stat_dexterity,
		i_stat_electronics,
		i_stat_intelligence,
		i_stat_marksmanship,
		i_stat_medical,
		i_stat_sniping,
		i_stat_strategy,
		i_stat_strength,
		i_stat_wh,
		i_stat_wisdom,
		i_etc,
		__i_END,
	};
	static constexpr std::size_t i_max_count = __i_END;

	static constexpr const char* NL = "\r\n";

	static std::map<std::string,std::string> cache;
	std::string format_rifle_stats_page(std::map<std::string,std::string>* page) {
		//using namespace mods::util::str;
		std::vector<std::string> rifle_page;

		std::string manufacturer,name,
		    clip_size,ammo_max,
		    crit_chance,crit_range,
		    dice_count,dice_sides;
		rifle_page.resize(std::max((std::size_t)i_max_count,page->size()));
		//rifle_page[i_a_map] = accuracy_map.data();

		for(const auto& pair : *page) {
			const auto& f = pair.first;
			if(str::starts_with("rifle_name",f)) {
				rifle_page[i_title] = pair.second;
				continue;
			}
			if(str::starts_with("rifle_manufacturer",f)) {
				rifle_page[i_manufacturer] = pair.second;
				continue;
			}
			if(str::starts_with_any({
			"rifle_ammo_type",
			"rifle_damage_map_",
			"rifle_file",
			"rifle_muzzle_velocity",
			"rifle_name",
			"rifle_reload_time",
			"rifle_rounds_per_minute",
			"rifle_str_type",
			"rifle_type",
			"rifle_vnum",
		},f)) {
				continue;
			}
			std::string s = pair.second;

			if(str::ends_with(".000000",s)) {
				s = str::chop_after_2nd_place(s);
			}

			if(str::is_floatval(s)) {
				s = str::chop_after_2nd_place(s);
			}
			if(s.compare("0.00") == 0 || s.compare("0") == 0) {
				continue;
			}
			if(str::is("rifle_incendiary_damage",f)) {
				rifle_page[i_ele_incendiary_damage] = CAT("{grn}incendiary{/grn}: ",s);
				continue;
			}
			if(str::is("rifle_explosive_damage",f)) {
				rifle_page[i_ele_explosive_damage] = CAT("{grn}explosive{/grn}: ",s);
				continue;
			}
			if(str::is("rifle_shrapnel_damage",f)) {
				rifle_page[i_ele_shrapnel_damage] = CAT("{grn}shrapnel{/grn}: ",s);
				continue;
			}
			if(str::is("rifle_corrosive_damage",f)) {
				rifle_page[i_ele_corrosive_damage] = CAT("{grn}corrosive{/grn}: ",s);
				continue;
			}
			if(str::is("rifle_cryogenic_damage",f)) {
				rifle_page[i_ele_cryogenic_damage] = CAT("{grn}cryogenic{/grn}: ",s);
				continue;
			}
			if(str::is("rifle_radioactive_damage",f)) {
				rifle_page[i_ele_radioactive_damage] = CAT("{grn}radioactive{/grn}: ",s);
				continue;
			}
			if(str::is("rifle_emp_damage",f)) {
				rifle_page[i_ele_emp_damage] = CAT("{grn}emp{/grn}: ",s);
				continue;
			}
			if(str::is("rifle_shock_damage",f)) {
				rifle_page[i_ele_shock_damage] = CAT("{grn}shock{/grn}: ",s);
				continue;
			}
			if(str::is("rifle_anti_matter_damage",f)) {
				rifle_page[i_ele_anti_matter_damage] = CAT("{grn}anti-matter{/grn}: ",s);
				continue;
			}

			if(str::is("rifle_critical_chance",f)) {
				crit_chance = CAT(s,"%{grn}@{/grn}");
				continue;
			}
			if(str::is("rifle_critical_range",f)) {
				crit_range = s;
				continue;
			}

			if(str::is("rifle_ammo_max",f)) {
				ammo_max = CAT("{grn}/{/grn}",s);
				continue;
			}
			if(str::is("rifle_chance_to_injure",f)) {
				rifle_page[i_injure] = CAT(injure.data(),s,"%");
				continue;
			}
			if(str::is("rifle_clip_size",f)) {
				rifle_page[i_ammo] = CAT(ammo.data(),s,ammo_max);
				continue;
			}
			if(str::is("rifle_cooldown_between_shots",f)) {
				rifle_page[i_cooldown] = CAT(cooldown.data(),s," ticks");
				continue;
			}
			if(str::is("rifle_base_damage",f)) {
				rifle_page[i_base_damage] = CAT(base_damage.data(),s);
				continue;
			}
			if(str::is("rifle_damage_dice_count",f)) {
				dice_count = s;
				continue;
			}

			if(str::is("rifle_damage_dice_sides",f)) {
				dice_sides = s;
				continue;
			}

			if(str::is("rifle_disorient_amount",f)) {
				rifle_page[i_disorient] += CAT("{grn}disorient{/grn}: ",s);
				continue;
			}
			if(str::is("rifle_effective_firing_range",f)) {
				rifle_page[i_effective_range] = CAT("{grn}effective range{/grn}: ",s);
				continue;
			}
			if(str::is("rifle_max_range",f)) {
				rifle_page[i_max_range] += CAT("{grn}max range{/grn}: ",s);
				continue;
			}
			if(str::is("rifle_range_multiplier",f)) {
				rifle_page[i_range_mult] = CAT("{grn}range multipler{/grn}: ",s);
				continue;
			}
			if(str::is("rifle_headshot_bonus",f)) {
				rifle_page[i_hs_bonus] += CAT("{grn}headshot bonus{/grn}: {grn}+{/grn}",s);
				continue;
			}


			if(str::starts_with("rifle_stat_",f)) {
				if(s.compare("0") == 0) {
					continue;
				}
				if(str::is("rifle_stat_armor",f)) {
					rifle_page[i_stat_armor] += CAT("{grn}armor{/grn}: {grn}+{/grn}",s);
					continue;
				}
				if(str::is("rifle_stat_chemistry",f)) {
					rifle_page[i_stat_chemistry] += CAT("{grn}chemistry{/grn}: {grn}+{/grn}",s);
					continue;
				}
				if(str::is("rifle_stat_constitution",f)) {
					rifle_page[i_stat_constitution] += CAT("{grn}constitution{/grn}: {grn}+{/grn}",s);
					continue;
				}
				if(str::is("rifle_stat_demolitions",f)) {
					rifle_page[i_stat_demolitions] += CAT("{grn}demolitions{/grn}: {grn}+{/grn}",s);
					continue;
				}
				if(str::is("rifle_stat_dexterity",f)) {
					rifle_page[i_stat_dexterity] += CAT("{grn}dexterity{/grn}: {grn}+{/grn}",s);
					continue;
				}
				if(str::is("rifle_stat_electronics",f)) {
					rifle_page[i_stat_electronics] += CAT("{grn}electronics{/grn}: {grn}+{/grn}",s);
					continue;
				}
				if(str::is("rifle_stat_intelligence",f)) {
					rifle_page[i_stat_intelligence] += CAT("{grn}intelligence{/grn}: {grn}+{/grn}",s);
					continue;
				}
				if(str::is("rifle_stat_marksmanship",f)) {
					rifle_page[i_stat_marksmanship] += CAT("{grn}marksmanship{/grn}: {grn}+{/grn}",s);
					continue;
				}
				if(str::is("rifle_stat_medical",f)) {
					rifle_page[i_stat_medical] += CAT("{grn}medical{/grn}: {grn}+{/grn}",s);
					continue;
				}
				if(str::is("rifle_stat_sniping",f)) {
					rifle_page[i_stat_sniping] += CAT("{grn}sniping{/grn}: {grn}+{/grn}",s);
					continue;
				}
				if(str::is("rifle_stat_strategy",f)) {
					rifle_page[i_stat_strategy] += CAT("{grn}strategy{/grn}: {grn}+{/grn}",s);
					continue;
				}
				if(str::is("rifle_stat_strength",f)) {
					rifle_page[i_stat_strength] += CAT("{grn}strength{/grn}: {grn}+{/grn}",s);
					continue;
				}
				if(str::is("rifle_stat_weapon_handling",f)) {
					rifle_page[i_stat_wh] += CAT("{grn}weapon handling{/grn}: {grn}+{/grn}",s);
					continue;
				}
				if(str::is("rifle_stat_wisdom",f)) {
					rifle_page[i_stat_wisdom] += CAT("{grn}wisdom{/grn}: {grn}+{/grn}",s);
					continue;
				}
			}

			if(str::starts_with("rifle_accuracy_map_",f)) {
				if(rifle_page[i_a_map].length() == 0) {
					rifle_page[i_a_map] = accuracy_map.data();
				}
				rifle_page[i_a_map] += CAT(str::until('.',s)," ");
				continue;
			}


			if(str::starts_with("rifle_",f)) {
				rifle_page[i_etc] += CAT("{grn}",f.substr(strlen("rifle_")),"{/grn}: ",s,NL);
			} else {
				rifle_page[i_etc] += CAT("{grn}",f,"{/grn}: ",s,NL);
			}
		}
		if(rifle_page[i_title].length()) {
			rifle_page[i_hr] = "{grn}";
			for(auto i=0; i < 4 + rifle_page[i_title].length() + rifle_page[i_manufacturer].length(); ++i) {
				rifle_page[i_hr] += "-";
			}
			rifle_page[i_hr] += "{/grn}\r\n";
		}

		if(rifle_page[i_critical].length()) {
			rifle_page[i_critical] = CAT(critical.data(),crit_chance,crit_range);
		}
		rifle_page[i_damage] = CAT(damage.data(),dice_count,"{grn}d{/grn}",dice_sides);
		std::string final_page;
		std::size_t i = 0;
		for(const auto& line : rifle_page) {
			if(line.length() == 0) {
				continue;
			}
			switch(i) {
				case i_manufacturer:
					if(rifle_page[i_title].length()) {
						final_page += CAT("{grn}",line,"{/grn} :: ");
					}
					break;
				case i_etc:
					if(rifle_page[i_etc].length()) {
						final_page += line;
					}
					break;
				case i_hr:
					if(rifle_page[i_hr].length()) {
						final_page += line;
					}
					break;
				default:
					final_page += CAT(line,NL);
					break;
			}
			++i;
		}
		return final_page;
	}
	std::string format_rifle_stats_page(std::string_view feed_file,std::map<std::string,std::string>* page) {
		/**
		 * TODO: need cache invalidation
		 */
		if(cache[feed_file.data()].length()) {
			return cache[feed_file.data()];
		}
		cache[feed_file.data()] = format_rifle_stats_page(page);
		return cache[feed_file.data()];
	}
#undef m_error
#undef m_debug
};
