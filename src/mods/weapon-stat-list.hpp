#ifndef __MENTOC_MODS_WEAPON_STAT_LIST_HEADER__
#define __MENTOC_MODS_WEAPON_STAT_LIST_HEADER__

	static inline weapon_stat_list_t* weapon_stats(int type){
		static weapon_stat_list_t shotgun = {
			{1,80,80},
			{1,15,15},
			{0,0,0},
			{0,0,0},
			{0,0,0}
		};
		static weapon_stat_list_t ar = {
			{1,40,40},
			{1,40,40},
			{1,40,40},
			{0,0,0},
			{0,0,0}
		};
		static weapon_stat_list_t smg = {
			{1,50,50},
			{0,0,0},
			{0,0,0},
			{0,0,0},
			{0,0,0}
		};
		static weapon_stat_list_t machine_pistol = {
			{1,35,35},
			{0,0,0},
			{0,0,0},
			{0,0,0},
			{0,0,0}
		};
		static weapon_stat_list_t handgun = {
			{1,15,15},
			{0,0,0},
			{0,0,0},
			{0,0,0},
			{0,0,0}
		};
		static weapon_stat_list_t lmg = {
			{1,40,40},
			{1,35,35},
			{1,30,30},
			{0,0,0},
			{0,0,0}
		};
		static weapon_stat_list_t sniper = {
			{1,10,10},
			{1,35,35},
			{1,60,60},
			{1,75,75},
			{1,75,75}
		};
		static weapon_stat_list_t empty = {
			{0,0,0},
			{0,0,0},
			{0,0,0},
			{0,0,0},
			{0,0,0}
		};
		switch(static_cast<weapon_rifle_t>(type)){
			case weapon_rifle_t::HANDGUN:
			case weapon_rifle_t::PISTOL:
				return &handgun;
			case weapon_rifle_t::SHOTGUN: 
				return &shotgun;
			case weapon_rifle_t::ASSAULT_RIFLE:
				return &ar;
			case weapon_rifle_t::SUB_MACHINE_GUN:
				return &smg;
			case weapon_rifle_t::MACHINE_PISTOL:
				return &machine_pistol;
			case weapon_rifle_t::LIGHT_MACHINE_GUN:
				return &lmg;
			case weapon_rifle_t::SNIPER:
				return &sniper;
			default:
				return &empty;
		}
	}//end stat list func
#endif
