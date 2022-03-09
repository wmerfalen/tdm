#ifndef __MENTOC_MODS_FORGE_ENGINE_GENERATED_RIFLE_HEADER__
#define __MENTOC_MODS_FORGE_ENGINE_GENERATED_RIFLE_HEADER__
#include "../../structs.h"
#include "../pq.hpp"
#include "../sql.hpp"
#include "value-scaler.hpp"
#include "requirements.hpp"
#include "generator.hpp"
#include "../orm/admin/rifle-attribute-limits.hpp"
#include <functional>
#include "../player.hpp"

#ifdef __MENTOC_SHOW_MODS_FORGE_ENGINE_DEBUG_OUTPUT__
#define m_debug(MSG) mentoc_prefix_debug("[mods::forge_engine::generated_rifle]")  << MSG << "\n";
#else
#define m_debug(MSG) ;;
#endif


namespace mods::forge_engine {
	using sql_compositor = mods::sql::compositor<mods::pq::transaction>;
	void rifle_index_changed();
	using kill_t = mods::forge_engine::kill_t;
	using variant_t = std::variant<uint32_t,float>;

	using attributes_t = std::vector<std::pair<rifle_attributes_t,variant_t>>;
	struct attribute_limits_t {
		rifle_attributes_t attribute;
		int low;
		int high;
		int overpowered;
	};
	using attribute_limits_orm_t = mods::orm::admin::rifle_attribute_limits;
	template <typename T>
	struct damage_clamps {
		uint8_t range_start;
		uint8_t range_end;
		T value;
	};
	using dc_t = damage_clamps<uint16_t>;
	static constexpr std::size_t DAMAGE_CLAMP_SIZE = 15;
	using clamp_list_t = std::array<dc_t,DAMAGE_CLAMP_SIZE>;

	struct generated_rifle_t {
			using attr_pair_t = std::pair<rifle_attributes_t,variant_t>;
			generated_rifle_t() = delete;
			generated_rifle_t (kill_t& player);
			generated_rifle_t (kill_t& player,std::string_view type);
			obj_ptr_t roll();
			void send_stats_to_player(player_ptr_t& player);
			std::string get_dump();
			void fill(obj_ptr_t& rifle);
			void fill_attributes(obj_ptr_t& rifle);
			void fill_elemental(obj_ptr_t& rifle);
			void fill_stats(obj_ptr_t& rifle);
			/**
			 * Balancing rifle attributes
			 */
			void balance_rifle_attributes();

			/**
			 * Mainly used so that values aren't game breaking...
			 * i.e.: critical_range: 1014
			 */
			void clamp_rifle_attributes();

			const std::vector<std::string>& yaml_list(rifle_types_t);
			void load_from_sql();
			attributes_t generate_rifle_attributes();
			attribute_limits_t fetch_limits(const rifle_attributes_t& which);
			void init();
			std::pair<bool,variant_t> get_attribute(rifle_attributes_t attr) {
				for(auto p : m_attributes) {
					if(p.first == attr) {
						return {true,p.second};
					}
				}
				return {false,(uint32_t)0};
			}


		private:
#define IS_SNIPER m_type == rifle_types_t::RIFLE_TYPE_SNIPER
#define IS_AR m_type == rifle_types_t::RIFLE_TYPE_ASSAULT_RIFLE
#define IS_PISTOL m_type == rifle_types_t::RIFLE_TYPE_PISTOL
#define IS_MP m_type == rifle_types_t::RIFLE_TYPE_MACHINE_PISTOL
#define IS_SHOT m_type == rifle_types_t::RIFLE_TYPE_SHOTGUN
#define IS_LMG m_type == rifle_types_t::RIFLE_TYPE_LIGHT_MACHINE_GUN
#define IS_SMG m_type == rifle_types_t::RIFLE_TYPE_SUB_MACHINE_GUN
			auto ammo_max() {
				static constexpr clamp_list_t sniper = {
					dc_t{1,5,10},
					dc_t{6,10,20},
					dc_t{11,15,33},
					dc_t{16,25,54},
					dc_t{26,30,68},
					dc_t{31,40,90},
					dc_t{41,48,170},
					dc_t{49,59,280},
					dc_t{60,65,390},
					dc_t{66,70,410},
					dc_t{71,80,590},
					dc_t{81,85,690},
					dc_t{86,90,850},
					dc_t{91,100,1150},
				};
				if(IS_SNIPER) {
					return sniper;
				}
				static constexpr clamp_list_t ar = {
					dc_t{1,5,30},
					dc_t{6,10,30},
					dc_t{11,15,33},
					dc_t{16,25,54},
					dc_t{26,30,58},
					dc_t{31,40,60},
					dc_t{41,48,90},
					dc_t{49,59,110},
					dc_t{60,65,110},
					dc_t{66,70,120},
					dc_t{71,80,120},
					dc_t{81,85,120},
					dc_t{86,90,150},
					dc_t{91,100,250},
				};
				if(IS_AR) {
					return ar;
				}
				static constexpr clamp_list_t smg = {
					dc_t{1,5,20},
					dc_t{6,10,20},
					dc_t{11,15,30},
					dc_t{16,25,33},
					dc_t{26,30,44},
					dc_t{31,40,44},
					dc_t{41,48,55},
					dc_t{49,59,55},
					dc_t{60,65,55},
					dc_t{66,70,55},
					dc_t{71,80,80},
					dc_t{81,85,90},
					dc_t{86,90,110},
					dc_t{91,100,210},
				};
				if(IS_SMG) {
					return smg;
				}
				static constexpr clamp_list_t shot = {
					dc_t{1,5,6},
					dc_t{6,10,6},
					dc_t{11,15,6},
					dc_t{16,25,9},
					dc_t{26,30,9},
					dc_t{31,40,9},
					dc_t{41,48,12},
					dc_t{49,59,12},
					dc_t{60,65,12},
					dc_t{66,70,15},
					dc_t{71,80,15},
					dc_t{81,85,20},
					dc_t{86,90,23},
					dc_t{91,100,30},
				};
				if(IS_SHOT) {
					return shot;
				}
				static constexpr clamp_list_t pistol = {
					dc_t{1,5,9},
					dc_t{6,10,9},
					dc_t{11,15,9},
					dc_t{16,25,12},
					dc_t{26,30,12},
					dc_t{31,40,12},
					dc_t{41,48,18},
					dc_t{49,59,18},
					dc_t{60,65,18},
					dc_t{66,70,21},
					dc_t{71,80,21},
					dc_t{81,85,22},
					dc_t{86,90,25},
					dc_t{91,100,30},
				};
				if(IS_PISTOL) {
					return pistol;
				}
				static constexpr clamp_list_t mp = {
					dc_t{1,5,13},
					dc_t{6,10,13},
					dc_t{11,15,13},
					dc_t{16,25,18},
					dc_t{26,30,18},
					dc_t{31,40,18},
					dc_t{41,48,28},
					dc_t{49,59,28},
					dc_t{60,65,28},
					dc_t{66,70,31},
					dc_t{71,80,31},
					dc_t{81,85,42},
					dc_t{86,90,55},
					dc_t{91,100,62},
				};
				if(IS_MP) {
					return mp;
				}
				static constexpr clamp_list_t lmg = {
					dc_t{1,5,93},
					dc_t{6,10,93},
					dc_t{11,15,93},
					dc_t{16,25,118},
					dc_t{26,30,158},
					dc_t{31,40,198},
					dc_t{41,48,198},
					dc_t{49,59,298},
					dc_t{60,65,298},
					dc_t{66,70,381},
					dc_t{71,80,381},
					dc_t{81,85,420},
					dc_t{86,90,550},
					dc_t{91,100,620},
				};
				if(IS_LMG) {
					return lmg;
				}

				return sniper;
			}
			auto clip_size() {
				static constexpr clamp_list_t sniper = {
					dc_t{1,5,5},
					dc_t{6,10,5},
					dc_t{11,15,7},
					dc_t{16,25,7},
					dc_t{26,30,9},
					dc_t{31,40,12},
					dc_t{41,48,12},
					dc_t{49,59,15},
					dc_t{60,65,15},
					dc_t{66,70,15},
					dc_t{71,80,21},
					dc_t{81,85,25},
					dc_t{86,90,30},
					dc_t{91,100,50},
				};
				return sniper;
			}
			auto critical_chance() {
				static constexpr clamp_list_t critical_chance_clamps = {
					dc_t{1,5,10},
					dc_t{6,10,10},
					dc_t{11,15,20},
					dc_t{16,25,20},
					dc_t{26,30,29},
					dc_t{31,40,34},
					dc_t{41,48,45},
					dc_t{49,59,58},
					dc_t{60,65,65},
					dc_t{66,70,110},
					dc_t{71,80,240},
					dc_t{81,85,380},
					dc_t{86,90,450},
					dc_t{91,100,550},
				};
				return critical_chance_clamps;
			}
			auto disorient_amount() {
				static constexpr clamp_list_t disorient_amount_clamps = {
					dc_t{1,5,5},
					dc_t{6,10,18},
					dc_t{11,15,23},
					dc_t{16,25,26},
					dc_t{26,30,29},
					dc_t{31,40,35},
					dc_t{41,48,47},
					dc_t{49,59,58},
					dc_t{60,65,69},
					dc_t{66,70,81},
					dc_t{71,80,99},
					dc_t{81,85,119},
					dc_t{86,90,125},
					dc_t{91,100,135},
				};
				return disorient_amount_clamps;
			}
			auto headshot_bonus() {
				static constexpr uint8_t s0 = 20;
				static constexpr uint8_t s1 = 20;
				static constexpr uint8_t s2 = 23;
				static constexpr uint8_t s3 = 26;
				static constexpr uint8_t s4 = 29;
				static constexpr uint8_t s5 = 35;
				static constexpr uint8_t s6 = 47;
				static constexpr uint8_t s7 = 58;
				static constexpr uint8_t s8 = 69;
				static constexpr uint8_t s9 = 81;
				static constexpr uint8_t s10 = 99;
				static constexpr uint8_t s11 = 119;
				static constexpr uint8_t s12 = 125;
				static constexpr uint8_t s13 = 135;
				static constexpr clamp_list_t sniper_hsb_clamps = {
					dc_t{1,5,   s0},
					dc_t{6,10,  s1},
					dc_t{11,15, s2},
					dc_t{16,25, s3},
					dc_t{26,30, s4},
					dc_t{31,40, s5},
					dc_t{41,48, s6},
					dc_t{49,59, s7},
					dc_t{60,65, s8},
					dc_t{66,70, s9},
					dc_t{71,80, s10},
					dc_t{81,85, s11},
					dc_t{86,90, s12},
					dc_t{91,100,s13},
				};
				if(IS_SNIPER) {
					return sniper_hsb_clamps;
				}
				static constexpr float AR_PERC = 0.5;
				static constexpr float PISTOL_PERC = 0.25;
				static constexpr float M_PISTOL_PERC = 0.375;
				static constexpr float SMG_PERC= 0.40;
				static constexpr float LMG_PERC= 0.75;
				static constexpr clamp_list_t machine_pistol_hsb_clamps = {
					/**
					 * In general, a machine pistol should do 1.5 (rounding down)
					 * the damage of a pistol
					 */
					dc_t{1,5, (uint16_t)(s0  * M_PISTOL_PERC) },
					dc_t{6,10, (uint16_t)(s1  * M_PISTOL_PERC) },
					dc_t{11,15, (uint16_t)(s2  * M_PISTOL_PERC) },
					dc_t{16,25, (uint16_t)(s3  * M_PISTOL_PERC) },
					dc_t{26,30, (uint16_t)(s4  * M_PISTOL_PERC) },
					dc_t{31,40, (uint16_t)(s5  * M_PISTOL_PERC) },
					dc_t{41,48, (uint16_t)(s6  * M_PISTOL_PERC) },
					dc_t{49,59, (uint16_t)(s7  * M_PISTOL_PERC) },
					dc_t{60,65, (uint16_t)(s8  * M_PISTOL_PERC) },
					dc_t{66,70, (uint16_t)(s9  * M_PISTOL_PERC) },
					dc_t{71,80, (uint16_t)(s10 * M_PISTOL_PERC) },
					dc_t{81,85, (uint16_t)(s11 * M_PISTOL_PERC) },
					dc_t{86,90, (uint16_t)(s12 * M_PISTOL_PERC) },
					dc_t{91,100,(uint16_t)(s13 * M_PISTOL_PERC) },
				};
				if(IS_MP) {
					return machine_pistol_hsb_clamps;
				}
				static constexpr clamp_list_t pistol_hsb_clamps = {
					/**
					 * In general, a pistol should do half (rounding down)
					 * the damage of an assault rifle
					 */
					dc_t{1,5, (uint16_t)(s0  * PISTOL_PERC) },
					dc_t{6,10, (uint16_t)(s1  * PISTOL_PERC) },
					dc_t{11,15, (uint16_t)(s2  * PISTOL_PERC) },
					dc_t{16,25, (uint16_t)(s3  * PISTOL_PERC) },
					dc_t{26,30, (uint16_t)(s4  * PISTOL_PERC) },
					dc_t{31,40, (uint16_t)(s5  * PISTOL_PERC) },
					dc_t{41,48, (uint16_t)(s6  * PISTOL_PERC) },
					dc_t{49,59, (uint16_t)(s7  * PISTOL_PERC) },
					dc_t{60,65, (uint16_t)(s8  * PISTOL_PERC) },
					dc_t{66,70, (uint16_t)(s9  * PISTOL_PERC) },
					dc_t{71,80, (uint16_t)(s10 * PISTOL_PERC) },
					dc_t{81,85, (uint16_t)(s11 * PISTOL_PERC) },
					dc_t{86,90, (uint16_t)(s12 * PISTOL_PERC) },
					dc_t{91,100,(uint16_t)(s13 * PISTOL_PERC) },
				};
				if(IS_PISTOL) {
					return pistol_hsb_clamps;
				}
				static constexpr clamp_list_t ar_hsb_clamps = {
					/**
					 * In general, an AR should do half (rounding down)
					 * the damage of a sniper rifle
					 */
					dc_t{1,5, (uint16_t)(s0  * AR_PERC) },
					dc_t{6,10, (uint16_t)(s1  * AR_PERC) },
					dc_t{11,15, (uint16_t)(s2  * AR_PERC) },
					dc_t{16,25, (uint16_t)(s3  * AR_PERC) },
					dc_t{26,30, (uint16_t)(s4  * AR_PERC) },
					dc_t{31,40, (uint16_t)(s5  * AR_PERC) },
					dc_t{41,48, (uint16_t)(s6  * AR_PERC) },
					dc_t{49,59, (uint16_t)(s7  * AR_PERC) },
					dc_t{60,65, (uint16_t)(s8  * AR_PERC) },
					dc_t{66,70, (uint16_t)(s9  * AR_PERC) },
					dc_t{71,80, (uint16_t)(s10 * AR_PERC) },
					dc_t{81,85, (uint16_t)(s11 * AR_PERC) },
					dc_t{86,90, (uint16_t)(s12 * AR_PERC) },
					dc_t{91,100,(uint16_t)(s13 * AR_PERC) },
				};
				if(IS_AR) {
					return ar_hsb_clamps;
				}

				static constexpr clamp_list_t smg_hsb_clamps = {
					/**
					 * In general, a SMG should do 4/5ths (rounding down)
					 * the damage of an AR
					 */
					dc_t{1,5, (uint16_t)(s0  * SMG_PERC)    },
					dc_t{6,10, (uint16_t)(s1  * SMG_PERC)    },
					dc_t{11,15, (uint16_t)(s2  * SMG_PERC)    },
					dc_t{16,25, (uint16_t)(s3  * SMG_PERC)    },
					dc_t{26,30, (uint16_t)(s4  * SMG_PERC)    },
					dc_t{31,40, (uint16_t)(s5  * SMG_PERC)    },
					dc_t{41,48, (uint16_t)(s6  * SMG_PERC)    },
					dc_t{49,59, (uint16_t)(s7  * SMG_PERC)    },
					dc_t{60,65, (uint16_t)(s8  * SMG_PERC)    },
					dc_t{66,70, (uint16_t)(s9  * SMG_PERC)    },
					dc_t{71,80, (uint16_t)(s10 * SMG_PERC)    },
					dc_t{81,85, (uint16_t)(s11 * SMG_PERC)    },
					dc_t{86,90, (uint16_t)(s12 * SMG_PERC)    },
					dc_t{91,100,(uint16_t)(s13 * SMG_PERC)    },
				};
				if(IS_SMG) {
					return smg_hsb_clamps;
				}
				static constexpr clamp_list_t lmg_hsb_clamps = {
					/**
					 * In general, a LMG should do 75% (rounding down)
					 * the damage of a sniper rifle
					 */
					dc_t{1,5, (uint16_t)(s0  * LMG_PERC)    },
					dc_t{6,10, (uint16_t)(s1  * LMG_PERC)    },
					dc_t{11,15, (uint16_t)(s2  * LMG_PERC)    },
					dc_t{16,25, (uint16_t)(s3  * LMG_PERC)    },
					dc_t{26,30, (uint16_t)(s4  * LMG_PERC)    },
					dc_t{31,40, (uint16_t)(s5  * LMG_PERC)    },
					dc_t{41,48, (uint16_t)(s6  * LMG_PERC)    },
					dc_t{49,59, (uint16_t)(s7  * LMG_PERC)    },
					dc_t{60,65, (uint16_t)(s8  * LMG_PERC)    },
					dc_t{66,70, (uint16_t)(s9  * LMG_PERC)    },
					dc_t{71,80, (uint16_t)(s10 * LMG_PERC)    },
					dc_t{81,85, (uint16_t)(s11 * LMG_PERC)    },
					dc_t{86,90, (uint16_t)(s12 * LMG_PERC)    },
					dc_t{91,100,(uint16_t)(s13 * LMG_PERC)    },
				};
				if(IS_LMG) {
					return lmg_hsb_clamps;
				}
				log("SYSERR: UNKNOWN TYPE IN %s:%s! Returning pistol type!",__FILE__,__FUNCTION__);
				return pistol_hsb_clamps;
			}
			auto reload_time() {
				static constexpr uint16_t s0 = 50;
				static constexpr uint16_t s1 = 45;
				static constexpr uint16_t s2 = 40;
				static constexpr uint16_t s3 = 40;
				static constexpr uint16_t s4 = 35;
				static constexpr uint16_t s5 = 35;
				static constexpr uint16_t s6 = 25;
				static constexpr uint16_t s7 = 20;
				static constexpr uint16_t s8 = 18;
				static constexpr uint16_t s9 = 15;
				static constexpr uint16_t s10 = 12;
				static constexpr uint16_t s11 = 9;
				static constexpr uint16_t s12 = 6;
				static constexpr uint16_t s13 = 3;
				static constexpr clamp_list_t sniper_reload_time_clamps = {
					dc_t{1,5, (s0)   },
					dc_t{6,10, (s1)   },
					dc_t{11,15, (s2)   },
					dc_t{16,25, (s3)   },
					dc_t{26,30, (s4)   },
					dc_t{31,40, (s5)   },
					dc_t{41,48, (s6)   },
					dc_t{49,59, (s7)   },
					dc_t{60,65, (s8)   },
					dc_t{66,70, (s9)   },
					dc_t{71,80, (s10)   },
					dc_t{81,85, (s11)   },
					dc_t{86,90, (s12)   },
					dc_t{91,100,(s13)   },
				};
				static constexpr float MP_PERC = 0.50;
				static constexpr clamp_list_t mp_reload_time_clamps = {
					dc_t{1,5, (uint16_t)(s0 * MP_PERC)   },
					dc_t{6,10, (uint16_t)(s1 * MP_PERC)   },
					dc_t{11,15, (uint16_t)(s2 * MP_PERC)   },
					dc_t{16,25, (uint16_t)(s3 * MP_PERC)   },
					dc_t{26,30, (uint16_t)(s4 * MP_PERC)   },
					dc_t{31,40, (uint16_t)(s5 * MP_PERC)   },
					dc_t{41,48, (uint16_t)(s6 * MP_PERC)   },
					dc_t{49,59, (uint16_t)(s7 * MP_PERC)   },
					dc_t{60,65, (uint16_t)(s8 * MP_PERC)   },
					dc_t{66,70, (uint16_t)(s9 * MP_PERC)   },
					dc_t{71,80, (uint16_t)(s10 * MP_PERC)   },
					dc_t{81,85, (uint16_t)(s11 * MP_PERC)   },
					dc_t{86,90, (uint16_t)(s12 * MP_PERC)   },
					dc_t{91,100,(uint16_t)(s13 * MP_PERC)   },
				};
				static constexpr float PISTOL_PERC = 0.20;
				static constexpr clamp_list_t pistol_reload_time_clamps = {
					dc_t{1,5, (uint16_t)(s0 * PISTOL_PERC)   },
					dc_t{6,10, (uint16_t)(s1 * PISTOL_PERC)   },
					dc_t{11,15, (uint16_t)(s2 * PISTOL_PERC)   },
					dc_t{16,25, (uint16_t)(s3 * PISTOL_PERC)   },
					dc_t{26,30, (uint16_t)(s4 * PISTOL_PERC)   },
					dc_t{31,40, (uint16_t)(s5 * PISTOL_PERC)   },
					dc_t{41,48, (uint16_t)(s6 * PISTOL_PERC)   },
					dc_t{49,59, (uint16_t)(s7 * PISTOL_PERC)   },
					dc_t{60,65, (uint16_t)(s8 * PISTOL_PERC)   },
					dc_t{66,70, (uint16_t)(s9 * PISTOL_PERC)   },
					dc_t{71,80, (uint16_t)(s10 * PISTOL_PERC)   },
					dc_t{81,85, (uint16_t)(s11 * PISTOL_PERC)   },
					dc_t{86,90, (uint16_t)(s12 * PISTOL_PERC)   },
					dc_t{91,100,(uint16_t)(s13 * PISTOL_PERC)   },
				};
				static constexpr float LMG_PERC = 3.0;
				static constexpr clamp_list_t lmg_reload_time_clamps = {
					dc_t{1,5, (uint16_t)(s0 * LMG_PERC)   },
					dc_t{6,10, (uint16_t)(s1 * LMG_PERC)   },
					dc_t{11,15, (uint16_t)(s2 * LMG_PERC)   },
					dc_t{16,25, (uint16_t)(s3 * LMG_PERC)   },
					dc_t{26,30, (uint16_t)(s4 * LMG_PERC)   },
					dc_t{31,40, (uint16_t)(s5 * LMG_PERC)   },
					dc_t{41,48, (uint16_t)(s6 * LMG_PERC)   },
					dc_t{49,59, (uint16_t)(s7 * LMG_PERC)   },
					dc_t{60,65, (uint16_t)(s8 * LMG_PERC)   },
					dc_t{66,70, (uint16_t)(s9 * LMG_PERC)   },
					dc_t{71,80, (uint16_t)(s10 * LMG_PERC)   },
					dc_t{81,85, (uint16_t)(s11 * LMG_PERC)   },
					dc_t{86,90, (uint16_t)(s12 * LMG_PERC)   },
					dc_t{91,100,(uint16_t)(s13 * LMG_PERC)   },
				};
				switch(m_type) {
					case rifle_types_t::RIFLE_TYPE_ASSAULT_RIFLE:
					case rifle_types_t::RIFLE_TYPE_SNIPER:
					case rifle_types_t::RIFLE_TYPE_SHOTGUN:
					case rifle_types_t::RIFLE_TYPE_SUB_MACHINE_GUN:
						return sniper_reload_time_clamps;
					case rifle_types_t::RIFLE_TYPE_MACHINE_PISTOL:
						return mp_reload_time_clamps;
					case rifle_types_t::RIFLE_TYPE_PISTOL:
					case rifle_types_t::RIFLE_TYPE_HANDGUN:
						return pistol_reload_time_clamps;
					case rifle_types_t::RIFLE_TYPE_LIGHT_MACHINE_GUN:
						return lmg_reload_time_clamps;
					default:
						log("SYSERR: UNKNOWN TYPE IN %s:%s! Returning sniper type!",__FILE__,__FUNCTION__);
						return sniper_reload_time_clamps;
				}
			}
			auto rounds_per_minute() {
				static constexpr clamp_list_t rounds_per_minute_clamps = {
					dc_t{1,5,    60   },
					dc_t{6,10,   80  },
					dc_t{11,15,  110  },
					dc_t{16,25,  160  },
					dc_t{26,30,  190  },
					dc_t{31,40,  210  },
					dc_t{41,48,  270  },
					dc_t{49,59,  380  },
					dc_t{60,65,  420  },
					dc_t{66,70,  550  },
					dc_t{71,80,  650  },
					dc_t{81,85,  780},
					dc_t{86,90,  825},
					dc_t{91,100, 1350},
				};
				return rounds_per_minute_clamps;
			}
			auto muzzle_velocity() {
				static constexpr clamp_list_t muzzle_velocity_clamps = {
					dc_t{1,5,110},
					dc_t{6,10,180},
					dc_t{11,15,230},
					dc_t{16,25,260},
					dc_t{26,30,290},
					dc_t{31,40,350},
					dc_t{41,48,470},
					dc_t{49,59,580},
					dc_t{60,65,690},
					dc_t{66,70,810},
					dc_t{71,80,990},
					dc_t{81,85,1190},
					dc_t{86,90,1250},
					dc_t{91,100,1350},
				};
				return muzzle_velocity_clamps;
			}
			auto effective_firing_range() {
				static constexpr clamp_list_t effective_firing_range = {
					dc_t{1,5,    3   },
					dc_t{6,10,   3   },
					dc_t{11,15,  6   },
					dc_t{16,25,  6   },
					dc_t{26,30,  7   },
					dc_t{31,40,  7   },
					dc_t{41,48,  8   },
					dc_t{49,59,  8   },
					dc_t{60,65,  9   },
					dc_t{66,70,  10  },
					dc_t{71,80,  11  },
					dc_t{81,85,  12  },
					dc_t{86,90,  13  },
					dc_t{91,100, 14  },
				};
				return effective_firing_range;
			}
			auto incendiary_damage() {
				static constexpr clamp_list_t incendiary_damage_clamps = {
					dc_t{1,5,110},
					dc_t{6,10,180},
					dc_t{11,15,230},
					dc_t{16,25,260},
					dc_t{26,30,290},
					dc_t{31,40,350},
					dc_t{41,48,470},
					dc_t{49,59,580},
					dc_t{60,65,690},
					dc_t{66,70,810},
					dc_t{71,80,990},
					dc_t{81,85,1190},
					dc_t{86,90,1250},
					dc_t{91,100,1350},
				};
				return incendiary_damage_clamps;
			}
			auto explosive_damage() {
				static constexpr clamp_list_t explosive_damage_clamps = {
					dc_t{1,5,    0},
					dc_t{6,10,   0},
					dc_t{11,15,  0},
					dc_t{16,25,  0},
					dc_t{26,30,  290},
					dc_t{31,40,  350},
					dc_t{41,48,  470},
					dc_t{49,59,  580},
					dc_t{60,65,  690},
					dc_t{66,70,  810},
					dc_t{71,80,  990},
					dc_t{81,85,  1190},
					dc_t{86,90,  1250},
					dc_t{91,100, 1350},
				};
				return explosive_damage_clamps;
			}
			auto shrapnel_damage() {
				static constexpr clamp_list_t shrapnel_damage_clumps = {
					dc_t{1,5,    50},
					dc_t{6,10,   80},
					dc_t{11,15,  90},
					dc_t{16,25,  160},
					dc_t{26,30,  190},
					dc_t{31,40,  250},
					dc_t{41,48,  270},
					dc_t{49,59,  380},
					dc_t{60,65,  390},
					dc_t{66,70,  410},
					dc_t{71,80,  590},
					dc_t{81,85,  690},
					dc_t{86,90,  725},
					dc_t{91,100, 1350},
				};
				return shrapnel_damage_clumps;
			}
			auto corrosive_damage() {
				static constexpr clamp_list_t corrosive_damage_clamps = {
					dc_t{1,5,    0},
					dc_t{6,10,   0},
					dc_t{11,15,  0},
					dc_t{16,25,  0},
					dc_t{26,30,  0},
					dc_t{31,40,  350},
					dc_t{41,48,  470},
					dc_t{49,59,  580},
					dc_t{60,65,  690},
					dc_t{66,70,  810},
					dc_t{71,80,  990},
					dc_t{81,85,  1190},
					dc_t{86,90,  1250},
					dc_t{91,100, 1350},
				};
				return corrosive_damage_clamps;
			}
			auto cryogenic_damage() {
				static constexpr clamp_list_t cryogenic_damage_clamps = {
					dc_t{1,5,    0 },
					dc_t{6,10,   0 },
					dc_t{11,15,  0 },
					dc_t{16,25,  0 },
					dc_t{26,30,  0 },
					dc_t{31,40,  0 },
					dc_t{41,48,  470 },
					dc_t{49,59,  580 },
					dc_t{60,65,  690 },
					dc_t{66,70,  810 },
					dc_t{71,80,  990 },
					dc_t{81,85,  1190},
					dc_t{86,90,  1250},
					dc_t{91,100, 1350},
				};
				return cryogenic_damage_clamps;
			}
			auto radioactive_damage() {
				static constexpr clamp_list_t radioactive_damage_clamps = {
					dc_t{1,5,    0 },
					dc_t{6,10,   0 },
					dc_t{11,15,  0 },
					dc_t{16,25,  0 },
					dc_t{26,30,  0 },
					dc_t{31,40,  0 },
					dc_t{41,48,  0 },
					dc_t{49,59,  0 },
					dc_t{60,65,  690 },
					dc_t{66,70,  810 },
					dc_t{71,80,  990 },
					dc_t{81,85,  1190},
					dc_t{86,90,  1250},
					dc_t{91,100, 1350},
				};
				return radioactive_damage_clamps;
			}
			auto emp_damage() {
				static constexpr clamp_list_t emp_damage_clamps = {
					dc_t{1,5,110},
					dc_t{6,10,180},
					dc_t{11,15,230},
					dc_t{16,25,260},
					dc_t{26,30,290},
					dc_t{31,40,350},
					dc_t{41,48,470},
					dc_t{49,59,580},
					dc_t{60,65,690},
					dc_t{66,70,810},
					dc_t{71,80,990},
					dc_t{81,85,1190},
					dc_t{86,90,1250},
					dc_t{91,100,1350},
				};
				return emp_damage_clamps;
			}
			auto shock_damage() {
				static constexpr clamp_list_t shock_damage_clamps = {
					dc_t{1,5,    0  },
					dc_t{6,10,   0  },
					dc_t{11,15,  0  },
					dc_t{16,25,  0  },
					dc_t{26,30,  0  },
					dc_t{31,40,  0  },
					dc_t{41,48,  0  },
					dc_t{49,59,  0  },
					dc_t{60,65,  0  },
					dc_t{66,70,  810  },
					dc_t{71,80,  990  },
					dc_t{81,85,  1190 },
					dc_t{86,90,  1250 },
					dc_t{91,100, 1350 },
				};
				return shock_damage_clamps;
			}
			auto anti_matter_damage() {
				static constexpr clamp_list_t anti_matter_clamps = {
					dc_t{1,5,    0  },
					dc_t{6,10,   0  },
					dc_t{11,15,  0  },
					dc_t{16,25,  0  },
					dc_t{26,30,  0  },
					dc_t{31,40,  0  },
					dc_t{41,48,  0  },
					dc_t{49,59,  0  },
					dc_t{60,65,  0  },
					dc_t{66,70,  0  },
					dc_t{71,80,  990  },
					dc_t{81,85,  1190 },
					dc_t{86,90,  1250 },
					dc_t{91,100, 1350 },
				};
				return anti_matter_clamps;
			}
			auto base_damage() {
				static constexpr uint16_t s0 = 110;
				static constexpr uint16_t s1 = 180;
				static constexpr uint16_t s2 =  230;
				static constexpr uint16_t s3 =  260;
				static constexpr uint16_t s4 =  290;
				static constexpr uint16_t s5 =  350;
				static constexpr uint16_t s6 =  470;
				static constexpr uint16_t s7 =  580;
				static constexpr uint16_t s8 =  690;
				static constexpr uint16_t s9 =  810;
				static constexpr uint16_t s10 =  990;
				static constexpr uint16_t s11 = 1190;
				static constexpr uint16_t s12 = 1250;
				static constexpr uint16_t s13 = 1350;
				static constexpr clamp_list_t sniper_base_damage_clamps = {
					dc_t{1,5,   s0},
					dc_t{6,10,  s1},
					dc_t{11,15, s2},
					dc_t{16,25, s3},
					dc_t{26,30, s4},
					dc_t{31,40, s5},
					dc_t{41,48, s6},
					dc_t{49,59, s7},
					dc_t{60,65, s8},
					dc_t{66,70, s9},
					dc_t{71,80, s10},
					dc_t{81,85, s11},
					dc_t{86,90, s12},
					dc_t{91,100,s13},
				};
				static constexpr float SHOT_PERC = 0.45;
				static constexpr float AR_PERC = 0.5;
				static constexpr float PISTOL_PERC = 0.25;
				static constexpr float M_PISTOL_PERC = 0.375;
				static constexpr float SMG_PERC= 0.40;
				static constexpr float LMG_PERC= 0.75;
				static constexpr clamp_list_t smg_base_damage_clamps = {
					dc_t{1,5, (uint16_t)(s0  * SMG_PERC)   },
					dc_t{6,10, (uint16_t)(s1  * SMG_PERC)   },
					dc_t{11,15, (uint16_t)(s2  * SMG_PERC)   },
					dc_t{16,25, (uint16_t)(s3  * SMG_PERC)   },
					dc_t{26,30, (uint16_t)(s4  * SMG_PERC)   },
					dc_t{31,40, (uint16_t)(s5  * SMG_PERC)   },
					dc_t{41,48, (uint16_t)(s6  * SMG_PERC)   },
					dc_t{49,59, (uint16_t)(s7  * SMG_PERC)   },
					dc_t{60,65, (uint16_t)(s8  * SMG_PERC)   },
					dc_t{66,70, (uint16_t)(s9  * SMG_PERC)   },
					dc_t{71,80, (uint16_t)(s10 * SMG_PERC)   },
					dc_t{81,85, (uint16_t)(s11 * SMG_PERC)   },
					dc_t{86,90, (uint16_t)(s12 * SMG_PERC)   },
					dc_t{91,100,(uint16_t)(s13 * SMG_PERC)   },
				};
				static constexpr clamp_list_t ar_base_damage_clamps = {
					dc_t{1,5, (uint16_t)(s0  *   AR_PERC)   },
					dc_t{6,10, (uint16_t)(s1  *   AR_PERC)   },
					dc_t{11,15, (uint16_t)(s2  * AR_PERC)   },
					dc_t{16,25, (uint16_t)(s3  * AR_PERC)   },
					dc_t{26,30, (uint16_t)(s4  * AR_PERC)   },
					dc_t{31,40, (uint16_t)(s5  * AR_PERC)   },
					dc_t{41,48, (uint16_t)(s6  * AR_PERC)   },
					dc_t{49,59, (uint16_t)(s7  * AR_PERC)   },
					dc_t{60,65, (uint16_t)(s8  * AR_PERC)   },
					dc_t{66,70, (uint16_t)(s9  * AR_PERC)   },
					dc_t{71,80, (uint16_t)(s10 * AR_PERC)   },
					dc_t{81,85, (uint16_t)(s11 * AR_PERC)   },
					dc_t{86,90, (uint16_t)(s12 * AR_PERC)   },
					dc_t{91,100,(uint16_t)(s13 * AR_PERC)   },
				};
				static constexpr clamp_list_t lmg_base_damage_clamps = {
					dc_t{1,5, (uint16_t)(s0  * LMG_PERC)   },
					dc_t{6,10, (uint16_t)(s1  * LMG_PERC)   },
					dc_t{11,15, (uint16_t)(s2  * LMG_PERC)   },
					dc_t{16,25, (uint16_t)(s3  * LMG_PERC)   },
					dc_t{26,30, (uint16_t)(s4  * LMG_PERC)   },
					dc_t{31,40, (uint16_t)(s5  * LMG_PERC)   },
					dc_t{41,48, (uint16_t)(s6  * LMG_PERC)   },
					dc_t{49,59, (uint16_t)(s7  * LMG_PERC)   },
					dc_t{60,65, (uint16_t)(s8  * LMG_PERC)   },
					dc_t{66,70, (uint16_t)(s9  * LMG_PERC)   },
					dc_t{71,80, (uint16_t)(s10 * LMG_PERC)   },
					dc_t{81,85, (uint16_t)(s11 * LMG_PERC)   },
					dc_t{86,90, (uint16_t)(s12 * LMG_PERC)   },
					dc_t{91,100,(uint16_t)(s13 * LMG_PERC)   },
				};
				static constexpr clamp_list_t pistol_base_damage_clamps = {
					dc_t{1,5, (uint16_t)(s0  * PISTOL_PERC)   },
					dc_t{6,10, (uint16_t)(s1  * PISTOL_PERC)   },
					dc_t{11,15, (uint16_t)(s2  * PISTOL_PERC)   },
					dc_t{16,25, (uint16_t)(s3  * PISTOL_PERC)   },
					dc_t{26,30, (uint16_t)(s4  * PISTOL_PERC)   },
					dc_t{31,40, (uint16_t)(s5  * PISTOL_PERC)   },
					dc_t{41,48, (uint16_t)(s6  * PISTOL_PERC)   },
					dc_t{49,59, (uint16_t)(s7  * PISTOL_PERC)   },
					dc_t{60,65, (uint16_t)(s8  * PISTOL_PERC)   },
					dc_t{66,70, (uint16_t)(s9  * PISTOL_PERC)   },
					dc_t{71,80, (uint16_t)(s10 * PISTOL_PERC)   },
					dc_t{81,85, (uint16_t)(s11 * PISTOL_PERC)   },
					dc_t{86,90, (uint16_t)(s12 * PISTOL_PERC)   },
					dc_t{91,100,(uint16_t)(s13 * PISTOL_PERC)   },
				};
				static constexpr clamp_list_t mp_base_damage_clamps = {
					dc_t{1,5, (uint16_t)(s0  * M_PISTOL_PERC)   },
					dc_t{6,10, (uint16_t)(s1  * M_PISTOL_PERC)   },
					dc_t{11,15, (uint16_t)(s2  * M_PISTOL_PERC)   },
					dc_t{16,25, (uint16_t)(s3  * M_PISTOL_PERC)   },
					dc_t{26,30, (uint16_t)(s4  * M_PISTOL_PERC)   },
					dc_t{31,40, (uint16_t)(s5  * M_PISTOL_PERC)   },
					dc_t{41,48, (uint16_t)(s6  * M_PISTOL_PERC)   },
					dc_t{49,59, (uint16_t)(s7  * M_PISTOL_PERC)   },
					dc_t{60,65, (uint16_t)(s8  * M_PISTOL_PERC)   },
					dc_t{66,70, (uint16_t)(s9  * M_PISTOL_PERC)   },
					dc_t{71,80, (uint16_t)(s10 * M_PISTOL_PERC)   },
					dc_t{81,85, (uint16_t)(s11 * M_PISTOL_PERC)   },
					dc_t{86,90, (uint16_t)(s12 * M_PISTOL_PERC)   },
					dc_t{91,100,(uint16_t)(s13 * M_PISTOL_PERC)   },
				};
				static constexpr clamp_list_t shot_base_damage_clamps = {
					dc_t{1,5, (uint16_t)(s0  * SHOT_PERC)   },
					dc_t{6,10, (uint16_t)(s1  * SHOT_PERC)   },
					dc_t{11,15, (uint16_t)(s2  * SHOT_PERC)   },
					dc_t{16,25, (uint16_t)(s3  * SHOT_PERC)   },
					dc_t{26,30, (uint16_t)(s4  * SHOT_PERC)   },
					dc_t{31,40, (uint16_t)(s5  * SHOT_PERC)   },
					dc_t{41,48, (uint16_t)(s6  * SHOT_PERC)   },
					dc_t{49,59, (uint16_t)(s7  * SHOT_PERC)   },
					dc_t{60,65, (uint16_t)(s8  * SHOT_PERC)   },
					dc_t{66,70, (uint16_t)(s9  * SHOT_PERC)   },
					dc_t{71,80, (uint16_t)(s10 * SHOT_PERC)   },
					dc_t{81,85, (uint16_t)(s11 * SHOT_PERC)   },
					dc_t{86,90, (uint16_t)(s12 * SHOT_PERC)   },
					dc_t{91,100,(uint16_t)(s13 * SHOT_PERC)   },
				};
				if(IS_SNIPER) {
					return sniper_base_damage_clamps;
				}
				if(IS_AR) {
					return ar_base_damage_clamps;
				}
				if(IS_SMG) {
					return smg_base_damage_clamps;
				}
				if(IS_LMG) {
					return lmg_base_damage_clamps;
				}
				if(IS_PISTOL) {
					return pistol_base_damage_clamps;
				}
				if(IS_MP) {
					return mp_base_damage_clamps;
				}
				if(IS_SHOT) {
					return shot_base_damage_clamps;
				}
				log("SYSERR: UNKNOWN TYPE IN %s:%s! Returning pistol type!",__FILE__,__FUNCTION__);
				return pistol_base_damage_clamps;
			}

			auto cooldown_between_shots() {
				static constexpr uint16_t s0 = 23;
				static constexpr uint16_t s1 = 23;
				static constexpr uint16_t s2 = 19;
				static constexpr uint16_t s3 = 13;
				static constexpr uint16_t s4 = 12;
				static constexpr uint16_t s5 = 12;
				static constexpr uint16_t s6 = 9;
				static constexpr uint16_t s7 = 9;
				static constexpr uint16_t s8 = 9;
				static constexpr uint16_t s9 = 6;
				static constexpr uint16_t s10 = 6;
				static constexpr uint16_t s11 = 6;
				static constexpr uint16_t s12 = 3;
				static constexpr uint16_t s13 = 3;
				static constexpr float SMG_PERC = 0.30;
				static constexpr float AR_PERC = 0.50;
				static constexpr float LMG_PERC = 3.50;
				static constexpr float PISTOL_PERC = 0.10;
				static constexpr float M_PISTOL_PERC = 0.12;
				static constexpr float SHOT_PERC = 0.45;
				static constexpr clamp_list_t smg_cooldown_between_shots_clamps = {
					dc_t{1,5, (uint16_t)(s0  * SMG_PERC)   },
					dc_t{6,10, (uint16_t)(s1  * SMG_PERC)   },
					dc_t{11,15, (uint16_t)(s2  * SMG_PERC)   },
					dc_t{16,25, (uint16_t)(s3  * SMG_PERC)   },
					dc_t{26,30, (uint16_t)(s4  * SMG_PERC)   },
					dc_t{31,40, (uint16_t)(s5  * SMG_PERC)   },
					dc_t{41,48, (uint16_t)(s6  * SMG_PERC)   },
					dc_t{49,59, (uint16_t)(s7  * SMG_PERC)   },
					dc_t{60,65, (uint16_t)(s8  * SMG_PERC)   },
					dc_t{66,70, (uint16_t)(s9  * SMG_PERC)   },
					dc_t{71,80, (uint16_t)(s10 * SMG_PERC)   },
					dc_t{81,85, (uint16_t)(s11 * SMG_PERC)   },
					dc_t{86,90, (uint16_t)(s12 * SMG_PERC)   },
					dc_t{91,100,(uint16_t)(s13 * SMG_PERC)   },
				};
				static constexpr clamp_list_t ar_cooldown_between_shots_clamps = {
					dc_t{1,5, (uint16_t)(s0  *   AR_PERC)   },
					dc_t{6,10, (uint16_t)(s1  *   AR_PERC)   },
					dc_t{11,15, (uint16_t)(s2  * AR_PERC)   },
					dc_t{16,25, (uint16_t)(s3  * AR_PERC)   },
					dc_t{26,30, (uint16_t)(s4  * AR_PERC)   },
					dc_t{31,40, (uint16_t)(s5  * AR_PERC)   },
					dc_t{41,48, (uint16_t)(s6  * AR_PERC)   },
					dc_t{49,59, (uint16_t)(s7  * AR_PERC)   },
					dc_t{60,65, (uint16_t)(s8  * AR_PERC)   },
					dc_t{66,70, (uint16_t)(s9  * AR_PERC)   },
					dc_t{71,80, (uint16_t)(s10 * AR_PERC)   },
					dc_t{81,85, (uint16_t)(s11 * AR_PERC)   },
					dc_t{86,90, (uint16_t)(s12 * AR_PERC)   },
					dc_t{91,100,(uint16_t)(s13 * AR_PERC)   },
				};
				static constexpr clamp_list_t lmg_cooldown_between_shots_clamps = {
					dc_t{1,5, (uint16_t)(s0  * LMG_PERC)   },
					dc_t{6,10, (uint16_t)(s1  * LMG_PERC)   },
					dc_t{11,15, (uint16_t)(s2  * LMG_PERC)   },
					dc_t{16,25, (uint16_t)(s3  * LMG_PERC)   },
					dc_t{26,30, (uint16_t)(s4  * LMG_PERC)   },
					dc_t{31,40, (uint16_t)(s5  * LMG_PERC)   },
					dc_t{41,48, (uint16_t)(s6  * LMG_PERC)   },
					dc_t{49,59, (uint16_t)(s7  * LMG_PERC)   },
					dc_t{60,65, (uint16_t)(s8  * LMG_PERC)   },
					dc_t{66,70, (uint16_t)(s9  * LMG_PERC)   },
					dc_t{71,80, (uint16_t)(s10 * LMG_PERC)   },
					dc_t{81,85, (uint16_t)(s11 * LMG_PERC)   },
					dc_t{86,90, (uint16_t)(s12 * LMG_PERC)   },
					dc_t{91,100,(uint16_t)(s13 * LMG_PERC)   },
				};
				static constexpr clamp_list_t pistol_cooldown_between_shots_clamps = {
					dc_t{1,5, (uint16_t)(s0  * PISTOL_PERC)   },
					dc_t{6,10, (uint16_t)(s1  * PISTOL_PERC)   },
					dc_t{11,15, (uint16_t)(s2  * PISTOL_PERC)   },
					dc_t{16,25, (uint16_t)(s3  * PISTOL_PERC)   },
					dc_t{26,30, (uint16_t)(s4  * PISTOL_PERC)   },
					dc_t{31,40, (uint16_t)(s5  * PISTOL_PERC)   },
					dc_t{41,48, (uint16_t)(s6  * PISTOL_PERC)   },
					dc_t{49,59, (uint16_t)(s7  * PISTOL_PERC)   },
					dc_t{60,65, (uint16_t)(s8  * PISTOL_PERC)   },
					dc_t{66,70, (uint16_t)(s9  * PISTOL_PERC)   },
					dc_t{71,80, (uint16_t)(s10 * PISTOL_PERC)   },
					dc_t{81,85, (uint16_t)(s11 * PISTOL_PERC)   },
					dc_t{86,90, (uint16_t)(s12 * PISTOL_PERC)   },
					dc_t{91,100,(uint16_t)(s13 * PISTOL_PERC)   },
				};
				static constexpr clamp_list_t mp_cooldown_between_shots_clamps = {
					dc_t{1,5, (uint16_t)(s0  * M_PISTOL_PERC)   },
					dc_t{6,10, (uint16_t)(s1  * M_PISTOL_PERC)   },
					dc_t{11,15, (uint16_t)(s2  * M_PISTOL_PERC)   },
					dc_t{16,25, (uint16_t)(s3  * M_PISTOL_PERC)   },
					dc_t{26,30, (uint16_t)(s4  * M_PISTOL_PERC)   },
					dc_t{31,40, (uint16_t)(s5  * M_PISTOL_PERC)   },
					dc_t{41,48, (uint16_t)(s6  * M_PISTOL_PERC)   },
					dc_t{49,59, (uint16_t)(s7  * M_PISTOL_PERC)   },
					dc_t{60,65, (uint16_t)(s8  * M_PISTOL_PERC)   },
					dc_t{66,70, (uint16_t)(s9  * M_PISTOL_PERC)   },
					dc_t{71,80, (uint16_t)(s10 * M_PISTOL_PERC)   },
					dc_t{81,85, (uint16_t)(s11 * M_PISTOL_PERC)   },
					dc_t{86,90, (uint16_t)(s12 * M_PISTOL_PERC)   },
					dc_t{91,100,(uint16_t)(s13 * M_PISTOL_PERC)   },
				};
				static constexpr clamp_list_t shot_cooldown_between_shots_clamps = {
					dc_t{1,5, (uint16_t)(s0  * SHOT_PERC)   },
					dc_t{6,10, (uint16_t)(s1  * SHOT_PERC)   },
					dc_t{11,15, (uint16_t)(s2  * SHOT_PERC)   },
					dc_t{16,25, (uint16_t)(s3  * SHOT_PERC)   },
					dc_t{26,30, (uint16_t)(s4  * SHOT_PERC)   },
					dc_t{31,40, (uint16_t)(s5  * SHOT_PERC)   },
					dc_t{41,48, (uint16_t)(s6  * SHOT_PERC)   },
					dc_t{49,59, (uint16_t)(s7  * SHOT_PERC)   },
					dc_t{60,65, (uint16_t)(s8  * SHOT_PERC)   },
					dc_t{66,70, (uint16_t)(s9  * SHOT_PERC)   },
					dc_t{71,80, (uint16_t)(s10 * SHOT_PERC)   },
					dc_t{81,85, (uint16_t)(s11 * SHOT_PERC)   },
					dc_t{86,90, (uint16_t)(s12 * SHOT_PERC)   },
					dc_t{91,100,(uint16_t)(s13 * SHOT_PERC)   },
				};
				static constexpr clamp_list_t sniper_cooldown_between_shots_clamps = {
					dc_t{1,5,   s0},
					dc_t{6,10,  s1},
					dc_t{11,15, s2},
					dc_t{16,25, s3},
					dc_t{26,30, s4},
					dc_t{31,40, s5},
					dc_t{41,48, s6},
					dc_t{49,59, s7},
					dc_t{60,65, s8},
					dc_t{66,70, s9},
					dc_t{71,80, s10},
					dc_t{81,85, s11},
					dc_t{86,90, s12},
					dc_t{91,100,s13},
				};
				if(IS_SNIPER) {
					return sniper_cooldown_between_shots_clamps;
				}
				if(IS_AR) {
					return ar_cooldown_between_shots_clamps;
				}
				if(IS_SMG) {
					return smg_cooldown_between_shots_clamps;
				}
				if(IS_LMG) {
					return lmg_cooldown_between_shots_clamps;
				}
				if(IS_PISTOL) {
					return pistol_cooldown_between_shots_clamps;
				}
				if(IS_MP) {
					return mp_cooldown_between_shots_clamps;
				}
				if(IS_SHOT) {
					return shot_cooldown_between_shots_clamps;
				}
				log("SYSERR: UNKNOWN TYPE IN %s:%s! Returning pistol type!",__FILE__,__FUNCTION__);
				return pistol_cooldown_between_shots_clamps;
			}
			auto damage_dice_count() {
				static constexpr uint16_t s0 = 5;
				static constexpr uint16_t s1 = 7;
				static constexpr uint16_t s2 = 10;
				static constexpr uint16_t s3 = 13;
				static constexpr uint16_t s4 = 15;
				static constexpr uint16_t s5 = 17;
				static constexpr uint16_t s6 = 19;
				static constexpr uint16_t s7 = 21;
				static constexpr uint16_t s8 = 22;
				static constexpr uint16_t s9 = 24;
				static constexpr uint16_t s10 = 26;
				static constexpr uint16_t s11 = 28;
				static constexpr uint16_t s12 = 30;
				static constexpr uint16_t s13 = 35;
				static constexpr clamp_list_t sniper_dice_count_clamps = {
					dc_t{1,5,   s0},
					dc_t{6,10,  s1},
					dc_t{11,15, s2},
					dc_t{16,25, s3},
					dc_t{26,30, s4},
					dc_t{31,40, s5},
					dc_t{41,48, s6},
					dc_t{49,59, s7},
					dc_t{60,65, s8},
					dc_t{66,70, s9},
					dc_t{71,80, s10},
					dc_t{81,85, s11},
					dc_t{86,90, s12},
					dc_t{91,100,s13},
				};
				if(IS_SNIPER) {
					return sniper_dice_count_clamps;
				}
				static constexpr float AR_PERC = 0.5;
				static constexpr float PISTOL_PERC = 0.25;
				static constexpr float M_PISTOL_PERC = 0.375;
				static constexpr float SMG_PERC= 0.40;
				static constexpr float LMG_PERC= 0.75;
				static constexpr clamp_list_t machine_pistol_dice_count_clamps = {
					/**
					 * In general, a machine pistol should do 1.5 (rounding down)
					 * the damage of a pistol
					 */
					dc_t{1,5, (uint16_t)(s0  * M_PISTOL_PERC) },
					dc_t{6,10, (uint16_t)(s1  * M_PISTOL_PERC) },
					dc_t{11,15, (uint16_t)(s2  * M_PISTOL_PERC) },
					dc_t{16,25, (uint16_t)(s3  * M_PISTOL_PERC) },
					dc_t{26,30, (uint16_t)(s4  * M_PISTOL_PERC) },
					dc_t{31,40, (uint16_t)(s5  * M_PISTOL_PERC) },
					dc_t{41,48, (uint16_t)(s6  * M_PISTOL_PERC) },
					dc_t{49,59, (uint16_t)(s7  * M_PISTOL_PERC) },
					dc_t{60,65, (uint16_t)(s8  * M_PISTOL_PERC) },
					dc_t{66,70, (uint16_t)(s9  * M_PISTOL_PERC) },
					dc_t{71,80, (uint16_t)(s10 * M_PISTOL_PERC) },
					dc_t{81,85, (uint16_t)(s11 * M_PISTOL_PERC) },
					dc_t{86,90, (uint16_t)(s12 * M_PISTOL_PERC) },
					dc_t{91,100,(uint16_t)(s13 * M_PISTOL_PERC) },
				};
				if(IS_MP) {
					return machine_pistol_dice_count_clamps;
				}
				static constexpr clamp_list_t pistol_dice_count_clamps = {
					/**
					 * In general, a pistol should do half (rounding down)
					 * the damage of an assault rifle
					 */
					dc_t{1,5, (uint16_t)(s0  * PISTOL_PERC) },
					dc_t{6,10, (uint16_t)(s1  * PISTOL_PERC) },
					dc_t{11,15, (uint16_t)(s2  * PISTOL_PERC) },
					dc_t{16,25, (uint16_t)(s3  * PISTOL_PERC) },
					dc_t{26,30, (uint16_t)(s4  * PISTOL_PERC) },
					dc_t{31,40, (uint16_t)(s5  * PISTOL_PERC) },
					dc_t{41,48, (uint16_t)(s6  * PISTOL_PERC) },
					dc_t{49,59, (uint16_t)(s7  * PISTOL_PERC) },
					dc_t{60,65, (uint16_t)(s8  * PISTOL_PERC) },
					dc_t{66,70, (uint16_t)(s9  * PISTOL_PERC) },
					dc_t{71,80, (uint16_t)(s10 * PISTOL_PERC) },
					dc_t{81,85, (uint16_t)(s11 * PISTOL_PERC) },
					dc_t{86,90, (uint16_t)(s12 * PISTOL_PERC) },
					dc_t{91,100,(uint16_t)(s13 * PISTOL_PERC) },
				};
				if(IS_PISTOL) {
					return pistol_dice_count_clamps;
				}
				static constexpr clamp_list_t ar_dice_count_clamps = {
					/**
					 * In general, an AR should do half (rounding down)
					 * the damage of a sniper rifle
					 */
					dc_t{1,5, (uint16_t)(s0  * AR_PERC) },
					dc_t{6,10, (uint16_t)(s1  * AR_PERC) },
					dc_t{11,15, (uint16_t)(s2  * AR_PERC) },
					dc_t{16,25, (uint16_t)(s3  * AR_PERC) },
					dc_t{26,30, (uint16_t)(s4  * AR_PERC) },
					dc_t{31,40, (uint16_t)(s5  * AR_PERC) },
					dc_t{41,48, (uint16_t)(s6  * AR_PERC) },
					dc_t{49,59, (uint16_t)(s7  * AR_PERC) },
					dc_t{60,65, (uint16_t)(s8  * AR_PERC) },
					dc_t{66,70, (uint16_t)(s9  * AR_PERC) },
					dc_t{71,80, (uint16_t)(s10 * AR_PERC) },
					dc_t{81,85, (uint16_t)(s11 * AR_PERC) },
					dc_t{86,90, (uint16_t)(s12 * AR_PERC) },
					dc_t{91,100,(uint16_t)(s13 * AR_PERC) },
				};
				if(IS_AR) {
					return ar_dice_count_clamps;
				}

				static constexpr clamp_list_t smg_dice_count_clamps = {
					/**
					 * In general, a SMG should do 4/5ths (rounding down)
					 * the damage of an AR
					 */
					dc_t{1,5, (uint16_t)(s0  * SMG_PERC)    },
					dc_t{6,10, (uint16_t)(s1  * SMG_PERC)    },
					dc_t{11,15, (uint16_t)(s2  * SMG_PERC)    },
					dc_t{16,25, (uint16_t)(s3  * SMG_PERC)    },
					dc_t{26,30, (uint16_t)(s4  * SMG_PERC)    },
					dc_t{31,40, (uint16_t)(s5  * SMG_PERC)    },
					dc_t{41,48, (uint16_t)(s6  * SMG_PERC)    },
					dc_t{49,59, (uint16_t)(s7  * SMG_PERC)    },
					dc_t{60,65, (uint16_t)(s8  * SMG_PERC)    },
					dc_t{66,70, (uint16_t)(s9  * SMG_PERC)    },
					dc_t{71,80, (uint16_t)(s10 * SMG_PERC)    },
					dc_t{81,85, (uint16_t)(s11 * SMG_PERC)    },
					dc_t{86,90, (uint16_t)(s12 * SMG_PERC)    },
					dc_t{91,100,(uint16_t)(s13 * SMG_PERC)    },
				};
				if(IS_SMG) {
					return smg_dice_count_clamps;
				}
				static constexpr clamp_list_t lmg_dice_count_clamps = {
					/**
					 * In general, a LMG should do 75% (rounding down)
					 * the damage of a sniper rifle
					 */
					dc_t{1,5, (uint16_t)(s0  * LMG_PERC)    },
					dc_t{6,10, (uint16_t)(s1  * LMG_PERC)    },
					dc_t{11,15, (uint16_t)(s2  * LMG_PERC)    },
					dc_t{16,25, (uint16_t)(s3  * LMG_PERC)    },
					dc_t{26,30, (uint16_t)(s4  * LMG_PERC)    },
					dc_t{31,40, (uint16_t)(s5  * LMG_PERC)    },
					dc_t{41,48, (uint16_t)(s6  * LMG_PERC)    },
					dc_t{49,59, (uint16_t)(s7  * LMG_PERC)    },
					dc_t{60,65, (uint16_t)(s8  * LMG_PERC)    },
					dc_t{66,70, (uint16_t)(s9  * LMG_PERC)    },
					dc_t{71,80, (uint16_t)(s10 * LMG_PERC)    },
					dc_t{81,85, (uint16_t)(s11 * LMG_PERC)    },
					dc_t{86,90, (uint16_t)(s12 * LMG_PERC)    },
					dc_t{91,100,(uint16_t)(s13 * LMG_PERC)    },
				};
				if(IS_LMG) {
					return lmg_dice_count_clamps;
				}
				log("SYSERR: UNKNOWN TYPE IN %s:%s! Returning pistol type!",__FILE__,__FUNCTION__);
				return pistol_dice_count_clamps;
			}
			auto damage_dice_sides() {
				static constexpr uint16_t s0 = 9;
				static constexpr uint16_t s1 = 11;
				static constexpr uint16_t s2 = 14;
				static constexpr uint16_t s3 = 15;
				static constexpr uint16_t s4 = 16;
				static constexpr uint16_t s5 = 18;
				static constexpr uint16_t s6 = 20;
				static constexpr uint16_t s7 = 21;
				static constexpr uint16_t s8 = 24;
				static constexpr uint16_t s9 = 24;
				static constexpr uint16_t s10 = 25;
				static constexpr uint16_t s11 = 28;
				static constexpr uint16_t s12 = 30;
				static constexpr uint16_t s13 = 35;
				static constexpr clamp_list_t sniper_dice_sides_clamps = {
					dc_t{1,5,   s0},
					dc_t{6,10,  s1},
					dc_t{11,15, s2},
					dc_t{16,25, s3},
					dc_t{26,30, s4},
					dc_t{31,40, s5},
					dc_t{41,48, s6},
					dc_t{49,59, s7},
					dc_t{60,65, s8},
					dc_t{66,70, s9},
					dc_t{71,80, s10},
					dc_t{81,85, s11},
					dc_t{86,90, s12},
					dc_t{91,100,s13},
				};
				if(IS_SNIPER) {
					return sniper_dice_sides_clamps;
				}
				static constexpr float AR_PERC = 0.5;
				static constexpr float PISTOL_PERC = 0.25;
				static constexpr float M_PISTOL_PERC = 0.375;
				static constexpr float SMG_PERC= 0.40;
				static constexpr float LMG_PERC= 0.75;
				static constexpr clamp_list_t machine_pistol_dice_sides_clamps = {
					/**
					 * In general, a machine pistol should do 1.5 (rounding down)
					 * the damage of a pistol
					 */
					dc_t{1,5, (uint16_t)(s0  * M_PISTOL_PERC) },
					dc_t{6,10, (uint16_t)(s1  * M_PISTOL_PERC) },
					dc_t{11,15, (uint16_t)(s2  * M_PISTOL_PERC) },
					dc_t{16,25, (uint16_t)(s3  * M_PISTOL_PERC) },
					dc_t{26,30, (uint16_t)(s4  * M_PISTOL_PERC) },
					dc_t{31,40, (uint16_t)(s5  * M_PISTOL_PERC) },
					dc_t{41,48, (uint16_t)(s6  * M_PISTOL_PERC) },
					dc_t{49,59, (uint16_t)(s7  * M_PISTOL_PERC) },
					dc_t{60,65, (uint16_t)(s8  * M_PISTOL_PERC) },
					dc_t{66,70, (uint16_t)(s9  * M_PISTOL_PERC) },
					dc_t{71,80, (uint16_t)(s10 * M_PISTOL_PERC) },
					dc_t{81,85, (uint16_t)(s11 * M_PISTOL_PERC) },
					dc_t{86,90, (uint16_t)(s12 * M_PISTOL_PERC) },
					dc_t{91,100,(uint16_t)(s13 * M_PISTOL_PERC) },
				};
				if(IS_MP) {
					return machine_pistol_dice_sides_clamps;
				}
				static constexpr clamp_list_t pistol_dice_sides_clamps = {
					/**
					 * In general, a pistol should do half (rounding down)
					 * the damage of an assault rifle
					 */
					dc_t{1,5, (uint16_t)(s0  * PISTOL_PERC) },
					dc_t{6,10, (uint16_t)(s1  * PISTOL_PERC) },
					dc_t{11,15, (uint16_t)(s2  * PISTOL_PERC) },
					dc_t{16,25, (uint16_t)(s3  * PISTOL_PERC) },
					dc_t{26,30, (uint16_t)(s4  * PISTOL_PERC) },
					dc_t{31,40, (uint16_t)(s5  * PISTOL_PERC) },
					dc_t{41,48, (uint16_t)(s6  * PISTOL_PERC) },
					dc_t{49,59, (uint16_t)(s7  * PISTOL_PERC) },
					dc_t{60,65, (uint16_t)(s8  * PISTOL_PERC) },
					dc_t{66,70, (uint16_t)(s9  * PISTOL_PERC) },
					dc_t{71,80, (uint16_t)(s10 * PISTOL_PERC) },
					dc_t{81,85, (uint16_t)(s11 * PISTOL_PERC) },
					dc_t{86,90, (uint16_t)(s12 * PISTOL_PERC) },
					dc_t{91,100,(uint16_t)(s13 * PISTOL_PERC) },
				};
				if(IS_PISTOL) {
					return pistol_dice_sides_clamps;
				}
				static constexpr clamp_list_t ar_dice_sides_clamps = {
					/**
					 * In general, an AR should do half (rounding down)
					 * the damage of a sniper rifle
					 */
					dc_t{1,5, (uint16_t)(s0  * AR_PERC) },
					dc_t{6,10, (uint16_t)(s1  * AR_PERC) },
					dc_t{11,15, (uint16_t)(s2  * AR_PERC) },
					dc_t{16,25, (uint16_t)(s3  * AR_PERC) },
					dc_t{26,30, (uint16_t)(s4  * AR_PERC) },
					dc_t{31,40, (uint16_t)(s5  * AR_PERC) },
					dc_t{41,48, (uint16_t)(s6  * AR_PERC) },
					dc_t{49,59, (uint16_t)(s7  * AR_PERC) },
					dc_t{60,65, (uint16_t)(s8  * AR_PERC) },
					dc_t{66,70, (uint16_t)(s9  * AR_PERC) },
					dc_t{71,80, (uint16_t)(s10 * AR_PERC) },
					dc_t{81,85, (uint16_t)(s11 * AR_PERC) },
					dc_t{86,90, (uint16_t)(s12 * AR_PERC) },
					dc_t{91,100,(uint16_t)(s13 * AR_PERC) },
				};
				if(IS_AR) {
					return ar_dice_sides_clamps;
				}

				static constexpr clamp_list_t smg_dice_sides_clamps = {
					/**
					 * In general, a SMG should do 4/5ths (rounding down)
					 * the damage of an AR
					 */
					dc_t{1,5, (uint16_t)(s0  * SMG_PERC)    },
					dc_t{6,10, (uint16_t)(s1  * SMG_PERC)    },
					dc_t{11,15, (uint16_t)(s2  * SMG_PERC)    },
					dc_t{16,25, (uint16_t)(s3  * SMG_PERC)    },
					dc_t{26,30, (uint16_t)(s4  * SMG_PERC)    },
					dc_t{31,40, (uint16_t)(s5  * SMG_PERC)    },
					dc_t{41,48, (uint16_t)(s6  * SMG_PERC)    },
					dc_t{49,59, (uint16_t)(s7  * SMG_PERC)    },
					dc_t{60,65, (uint16_t)(s8  * SMG_PERC)    },
					dc_t{66,70, (uint16_t)(s9  * SMG_PERC)    },
					dc_t{71,80, (uint16_t)(s10 * SMG_PERC)    },
					dc_t{81,85, (uint16_t)(s11 * SMG_PERC)    },
					dc_t{86,90, (uint16_t)(s12 * SMG_PERC)    },
					dc_t{91,100,(uint16_t)(s13 * SMG_PERC)    },
				};
				if(IS_SMG) {
					return smg_dice_sides_clamps;
				}
				static constexpr clamp_list_t lmg_dice_sides_clamps = {
					/**
					 * In general, a LMG should do 75% (rounding down)
					 * the damage of a sniper rifle
					 */
					dc_t{1,5, (uint16_t)(s0  * LMG_PERC)    },
					dc_t{6,10, (uint16_t)(s1  * LMG_PERC)    },
					dc_t{11,15, (uint16_t)(s2  * LMG_PERC)    },
					dc_t{16,25, (uint16_t)(s3  * LMG_PERC)    },
					dc_t{26,30, (uint16_t)(s4  * LMG_PERC)    },
					dc_t{31,40, (uint16_t)(s5  * LMG_PERC)    },
					dc_t{41,48, (uint16_t)(s6  * LMG_PERC)    },
					dc_t{49,59, (uint16_t)(s7  * LMG_PERC)    },
					dc_t{60,65, (uint16_t)(s8  * LMG_PERC)    },
					dc_t{66,70, (uint16_t)(s9  * LMG_PERC)    },
					dc_t{71,80, (uint16_t)(s10 * LMG_PERC)    },
					dc_t{81,85, (uint16_t)(s11 * LMG_PERC)    },
					dc_t{86,90, (uint16_t)(s12 * LMG_PERC)    },
					dc_t{91,100,(uint16_t)(s13 * LMG_PERC)    },
				};
				if(IS_LMG) {
					return lmg_dice_sides_clamps;
				}
				log("SYSERR: UNKNOWN TYPE IN %s:%s! Returning pistol type!",__FILE__,__FUNCTION__);
				return pistol_dice_sides_clamps;
			}
			auto critical_range() {
				static constexpr uint16_t s0 = 1;
				static constexpr uint16_t s1 = 2;
				static constexpr uint16_t s2 = 3;
				static constexpr uint16_t s3 = 3;
				static constexpr uint16_t s4 = 4;
				static constexpr uint16_t s5 = 5;
				static constexpr uint16_t s6 = 6;
				static constexpr uint16_t s7 = 7;
				static constexpr uint16_t s8 = 8;
				static constexpr uint16_t s9 = 9;
				static constexpr uint16_t s10 = 9;
				static constexpr uint16_t s11 = 11;
				static constexpr uint16_t s12 = 12;
				static constexpr uint16_t s13 = 13;
				static constexpr clamp_list_t sniper_crange_clamps = {
					dc_t{1,5,   s0},
					dc_t{6,10,  s1},
					dc_t{11,15, s2},
					dc_t{16,25, s3},
					dc_t{26,30, s4},
					dc_t{31,40, s5},
					dc_t{41,48, s6},
					dc_t{49,59, s7},
					dc_t{60,65, s8},
					dc_t{66,70, s9},
					dc_t{71,80, s10},
					dc_t{81,85, s11},
					dc_t{86,90, s12},
					dc_t{91,100,s13},
				};
				if(IS_SNIPER) {
					return sniper_crange_clamps;
				}
				static constexpr float AR_PERC = 0.25;
				static constexpr float PISTOL_PERC = 0.10;
				static constexpr float M_PISTOL_PERC = 0.15;
				static constexpr float SMG_PERC= 0.50;
				static constexpr float LMG_PERC= 1.75;
				static constexpr clamp_list_t machine_pistol_crange_clamps = {
					/**
					 * In general, a machine pistol should do 1.5 (rounding down)
					 * the damage of a pistol
					 */
					dc_t{1,5, (uint16_t)(s0  * M_PISTOL_PERC) },
					dc_t{6,10, (uint16_t)(s1  * M_PISTOL_PERC) },
					dc_t{11,15, (uint16_t)(s2  * M_PISTOL_PERC) },
					dc_t{16,25, (uint16_t)(s3  * M_PISTOL_PERC) },
					dc_t{26,30, (uint16_t)(s4  * M_PISTOL_PERC) },
					dc_t{31,40, (uint16_t)(s5  * M_PISTOL_PERC) },
					dc_t{41,48, (uint16_t)(s6  * M_PISTOL_PERC) },
					dc_t{49,59, (uint16_t)(s7  * M_PISTOL_PERC) },
					dc_t{60,65, (uint16_t)(s8  * M_PISTOL_PERC) },
					dc_t{66,70, (uint16_t)(s9  * M_PISTOL_PERC) },
					dc_t{71,80, (uint16_t)(s10 * M_PISTOL_PERC) },
					dc_t{81,85, (uint16_t)(s11 * M_PISTOL_PERC) },
					dc_t{86,90, (uint16_t)(s12 * M_PISTOL_PERC) },
					dc_t{91,100,(uint16_t)(s13 * M_PISTOL_PERC) },
				};
				if(IS_MP) {
					return machine_pistol_crange_clamps;
				}
				static constexpr clamp_list_t pistol_crange_clamps = {
					/**
					 * In general, a pistol should do half (rounding down)
					 * the damage of an assault rifle
					 */
					dc_t{1,5, (uint16_t)(s0  * PISTOL_PERC) },
					dc_t{6,10, (uint16_t)(s1  * PISTOL_PERC) },
					dc_t{11,15, (uint16_t)(s2  * PISTOL_PERC) },
					dc_t{16,25, (uint16_t)(s3  * PISTOL_PERC) },
					dc_t{26,30, (uint16_t)(s4  * PISTOL_PERC) },
					dc_t{31,40, (uint16_t)(s5  * PISTOL_PERC) },
					dc_t{41,48, (uint16_t)(s6  * PISTOL_PERC) },
					dc_t{49,59, (uint16_t)(s7  * PISTOL_PERC) },
					dc_t{60,65, (uint16_t)(s8  * PISTOL_PERC) },
					dc_t{66,70, (uint16_t)(s9  * PISTOL_PERC) },
					dc_t{71,80, (uint16_t)(s10 * PISTOL_PERC) },
					dc_t{81,85, (uint16_t)(s11 * PISTOL_PERC) },
					dc_t{86,90, (uint16_t)(s12 * PISTOL_PERC) },
					dc_t{91,100,(uint16_t)(s13 * PISTOL_PERC) },
				};
				if(IS_PISTOL) {
					return pistol_crange_clamps;
				}
				static constexpr clamp_list_t ar_crange_clamps = {
					/**
					 * In general, an AR should do half (rounding down)
					 * the damage of a sniper rifle
					 */
					dc_t{1,5, (uint16_t)(s0  * AR_PERC) },
					dc_t{6,10, (uint16_t)(s1  * AR_PERC) },
					dc_t{11,15, (uint16_t)(s2  * AR_PERC) },
					dc_t{16,25, (uint16_t)(s3  * AR_PERC) },
					dc_t{26,30, (uint16_t)(s4  * AR_PERC) },
					dc_t{31,40, (uint16_t)(s5  * AR_PERC) },
					dc_t{41,48, (uint16_t)(s6  * AR_PERC) },
					dc_t{49,59, (uint16_t)(s7  * AR_PERC) },
					dc_t{60,65, (uint16_t)(s8  * AR_PERC) },
					dc_t{66,70, (uint16_t)(s9  * AR_PERC) },
					dc_t{71,80, (uint16_t)(s10 * AR_PERC) },
					dc_t{81,85, (uint16_t)(s11 * AR_PERC) },
					dc_t{86,90, (uint16_t)(s12 * AR_PERC) },
					dc_t{91,100,(uint16_t)(s13 * AR_PERC) },
				};
				if(IS_AR) {
					return ar_crange_clamps;
				}

				static constexpr clamp_list_t smg_crange_clamps = {
					/**
					 * In general, a SMG should do 4/5ths (rounding down)
					 * the damage of an AR
					 */
					dc_t{1,5, (uint16_t)(s0  * SMG_PERC)    },
					dc_t{6,10, (uint16_t)(s1  * SMG_PERC)    },
					dc_t{11,15, (uint16_t)(s2  * SMG_PERC)    },
					dc_t{16,25, (uint16_t)(s3  * SMG_PERC)    },
					dc_t{26,30, (uint16_t)(s4  * SMG_PERC)    },
					dc_t{31,40, (uint16_t)(s5  * SMG_PERC)    },
					dc_t{41,48, (uint16_t)(s6  * SMG_PERC)    },
					dc_t{49,59, (uint16_t)(s7  * SMG_PERC)    },
					dc_t{60,65, (uint16_t)(s8  * SMG_PERC)    },
					dc_t{66,70, (uint16_t)(s9  * SMG_PERC)    },
					dc_t{71,80, (uint16_t)(s10 * SMG_PERC)    },
					dc_t{81,85, (uint16_t)(s11 * SMG_PERC)    },
					dc_t{86,90, (uint16_t)(s12 * SMG_PERC)    },
					dc_t{91,100,(uint16_t)(s13 * SMG_PERC)    },
				};
				if(IS_SMG) {
					return smg_crange_clamps;
				}
				static constexpr clamp_list_t lmg_crange_clamps = {
					/**
					 * In general, a LMG should do 75% (rounding down)
					 * the damage of a sniper rifle
					 */
					dc_t{1,5, (uint16_t)(s0  * LMG_PERC)    },
					dc_t{6,10, (uint16_t)(s1  * LMG_PERC)    },
					dc_t{11,15, (uint16_t)(s2  * LMG_PERC)    },
					dc_t{16,25, (uint16_t)(s3  * LMG_PERC)    },
					dc_t{26,30, (uint16_t)(s4  * LMG_PERC)    },
					dc_t{31,40, (uint16_t)(s5  * LMG_PERC)    },
					dc_t{41,48, (uint16_t)(s6  * LMG_PERC)    },
					dc_t{49,59, (uint16_t)(s7  * LMG_PERC)    },
					dc_t{60,65, (uint16_t)(s8  * LMG_PERC)    },
					dc_t{66,70, (uint16_t)(s9  * LMG_PERC)    },
					dc_t{71,80, (uint16_t)(s10 * LMG_PERC)    },
					dc_t{81,85, (uint16_t)(s11 * LMG_PERC)    },
					dc_t{86,90, (uint16_t)(s12 * LMG_PERC)    },
					dc_t{91,100,(uint16_t)(s13 * LMG_PERC)    },
				};
				if(IS_LMG) {
					return lmg_crange_clamps;
				}
				log("SYSERR: UNKNOWN TYPE IN %s:%s! Returning pistol type!",__FILE__,__FUNCTION__);
				return pistol_crange_clamps;
			}
			auto range_multiplier() {
				static constexpr clamp_list_t range_multiplier_clamps = {
					dc_t{1,5,   0 },
					dc_t{6,10,  0 },
					dc_t{11,15, 2 },
					dc_t{16,25, 2 },
					dc_t{26,30, 3 },
					dc_t{31,40, 3 },
					dc_t{41,48, 3 },
					dc_t{49,59, 3 },
					dc_t{60,65, 4 },
					dc_t{66,70, 4},
					dc_t{71,80, 4},
					dc_t{81,85, 6},
					dc_t{86,90, 6},
					dc_t{91,100,12},
				};
				return range_multiplier_clamps;
			}
			auto max_range() {
				static constexpr clamp_list_t maxrange_clamps = {
					dc_t{1,5,   1 },
					dc_t{6,10,  2 },
					dc_t{11,15, 3 },
					dc_t{16,25, 3 },
					dc_t{26,30, 4 },
					dc_t{31,40, 5 },
					dc_t{41,48, 6 },
					dc_t{49,59, 7 },
					dc_t{60,65, 8 },
					dc_t{66,70, 9},
					dc_t{71,80, 9},
					dc_t{81,85, 11},
					dc_t{86,90, 12},
					dc_t{91,100,13},
				};
				return maxrange_clamps;
			}
			auto chance_to_injure() {
				// chance to injure
				static constexpr clamp_list_t cti_clamps = {
					dc_t{1,5,   8 },
					dc_t{6,10,  12 },
					dc_t{11,15, 15 },
					dc_t{16,25, 20 },
					dc_t{26,30, 24 },
					dc_t{31,40, 35 },
					dc_t{41,48, 40 },
					dc_t{49,59, 50 },
					dc_t{60,65, 80 },
					dc_t{66,70, 90},
					dc_t{71,80, 110},
					dc_t{81,85, 140},
					dc_t{86,90, 180},
					dc_t{91,100,280},
				};
				return cti_clamps;
			}
			uint16_t clamp_using(const uint16_t& current_attr,const clamp_list_t& clamps) {
				const auto& player = m_player.killer;
				const auto level = player->level();
				for(const auto& dc : clamps) {
					if(dc.range_start <= level && dc.range_end >= level) {
						if(current_attr >= dc.value) {
							if(dc.value) {
								if((current_attr % dc.value) > 0) {
									return current_attr % dc.value;
								} else {
									return current_attr / dc.value;
								}
							}
							return dc.value;
						}
						if(current_attr <= dc.value) {
							return current_attr;
						}
					}
				}
				return 0;
			}
#undef IS_SNIPER
#undef IS_AR
#undef IS_PISTOL
#undef IS_MP
#undef IS_SHOT
#undef IS_LMG
#undef IS_SMG
			uint16_t clamp_critical_range();
			uint16_t clamp_range_multiplier();
			uint16_t clamp_chance_to_injure();
			uint16_t clamp_cooldown_between_shots();
			uint16_t clamp_damage_dice_count();
			uint16_t clamp_damage_dice_sides();
			uint16_t clamp_ammo_max();
			uint16_t clamp_clip_size();
			uint16_t clamp_critical_chance();
			uint16_t clamp_base_damage();
			uint16_t clamp_disorient_amount();
			uint16_t clamp_headshot_bonus();
			uint16_t clamp_max_range();
			uint16_t clamp_reload_time();
			uint16_t clamp_rounds_per_minute();
			uint16_t clamp_muzzle_velocity();
			uint16_t clamp_effective_firing_range();
			uint16_t clamp_incendiary_damage();
			uint16_t clamp_explosive_damage();
			uint16_t clamp_shrapnel_damage();
			uint16_t clamp_corrosive_damage();
			uint16_t clamp_cryogenic_damage();
			uint16_t clamp_radioactive_damage();
			uint16_t clamp_emp_damage();
			uint16_t clamp_shock_damage();
			uint16_t clamp_anti_matter_damage();

			bool roll_overpowered(std::string_view which_stat);
			void log_gen(std::string_view);
			void log_section(std::string_view);
			std::string m_section;

			bool m_force_type;
			kill_t m_player;
			rifle_types_t m_type;
			requirements_t m_requirements;
			std::vector<std::pair<rifle_attributes_t,variant_t>> m_attributes;
			std::vector<std::pair<elemental_types_t,variant_t>> m_elemental_damages;
			std::vector<std::pair<stat_types_t,variant_t>> m_stat_boosts;
			obj_ptr_t m_instance;
			bool compute_weapon_cost();
	};

	enum test_type_t {
		TEST_GREATER_THAN,
		TEST_LESS_THAN,
		TEST_GTE,
		TEST_LTE,
		TEST_EQUAL,

	};

	/**
	 * [ balancing ]
	 * [ RPM - balances - DMG ]
	 * ------------------------
	 * If cooldown between shots less than (high - low)
	 * 	THEN
	 * 	dmg dice count/sides should be nerfed
	 * 	base damage should be nerfed
	 */

	/**
	 * [ balancing ]
	 * [ CRIT % - balances - CBS ]
	 * If critical chance >= high
	 * 	THEN
	 *  cooldown between shots needs to be higher
	 */
	struct balancer_t {
		rifle_attributes_t attribute;
		test_type_t test;
		int value;
	};

	struct balance_ruleset_t {
			/**
			 * How to use this class.
			 * Load it up with tests and logic using the two parameter
			 * constructor. Then when you want to test a rifle, use
			 * the run(rifle*) function.
			 * If you do this correctly, and some(or all) of your tests pass,
			 * then your logic functions should be ran until either one of
			 * the following happens:
			 * 1) we reach the end of the logic functions you passed
			 * or
			 * 2) one of your logic functions returns false signalling to
			 * not call the next logic function in the deque
			 */
			using fixer_function_t = std::function<bool(void*,void*,void*)>;
			using test_list_t = std::deque<balancer_t>;
			using logic_list_t = std::deque<fixer_function_t>;
			balance_ruleset_t(test_list_t t, logic_list_t l) :
				tests(t), logic(l), rifle(nullptr)
			{}


			balance_ruleset_t() {
				this->__init();
			}
			~balance_ruleset_t() {
				tests.clear();
				logic.clear();
				rifle = nullptr;
			}
			void run(generated_rifle_t* r) {
				this->rifle = r;
				for(const auto& t : tests) {
					auto pair = this->rifle->get_attribute(t.attribute);
					if(pair.first == false) {
						continue;
					}
					auto vari = pair.second;
					int attr = 0;
					if(std::holds_alternative<uint32_t>(vari)) {
						attr = std::get<uint32_t>(vari);
					} else {
						attr = (int)std::get<float>(vari);
					}
					bool passed = false;
					switch(t.test) {
						case test_type_t::TEST_GREATER_THAN:
							passed = attr > t.value;
							break;
						case test_type_t::TEST_LESS_THAN:
							passed = attr < t.value;
							break;
						case test_type_t::TEST_GTE:
							passed = attr >= t.value;
							break;
						case test_type_t::TEST_LTE:
							passed = attr <= t.value;
							break;
						case test_type_t::TEST_EQUAL:
							passed = attr == t.value;
							break;
						default:
							break;
					}
					if(passed) {
						this->test_passed(t);
					}
					passed = false;
				}
				run_logic();
				rifle = nullptr;
			}
		private:
			test_list_t tests;
			test_list_t passed_tests;
			logic_list_t logic;
			generated_rifle_t* rifle;
			void __init() {
				tests.clear();
				logic.clear();
				rifle = nullptr;
			}
			void run_logic() {
				if(!rifle) {
					std::cerr << "rifle is a nullptr! --> " <<  __FUNCTION__ << "\n";
					return;
				}
				for(const auto func : logic) {
					if(!func((void*)&passed_tests,(void*)rifle,(void*)this)) {
						return;
					}
				}
			}
			void test_passed(const balancer_t& test) {
				passed_tests.emplace_back(test);
			}
	};//end balance_ruleset_t

	static inline void bal_loaders() {

	}

	/**
	 * A "weapon shaper" is a device in which we balance a
	 * weapon by it's weapon type to act within the limits
	 * of a predictable weapon of that type.
	 * This device is a molder, a limiter of sorts. The
	 * attributes of weapon are tamed so that a shotgun
	 * isn't doing insane damage at the range of a sniper
	 * rifle.
	 */
	struct weapon_shaper_t {
		/** Test if these attributes are high */
		std::vector<balancer_t> balancer_functions;
		rifle_attributes_t target_set;
		std::function<void(void*,balancer_t*)> balancer;
	};
	/*
	RIFLE_TYPE_SHOTGUN,
	RIFLE_TYPE_ASSAULT_RIFLE,
	RIFLE_TYPE_SUB_MACHINE_GUN,
	RIFLE_TYPE_SNIPER,
	RIFLE_TYPE_HANDGUN,
	RIFLE_TYPE_PISTOL,
	RIFLE_TYPE_MACHINE_PISTOL,
	RIFLE_TYPE_LIGHT_MACHINE_GUN,
	*/

};

#undef m_debug
#endif
