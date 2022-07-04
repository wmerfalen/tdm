#include "mob-scaler.hpp"
#include "../interpreter.hpp"
#include "../util.hpp"
#include "../builder_util.hpp"
#include <map>

#include "../orm/raid.hpp"
#include "slotted-builder.hpp"
#include "../../db.h"
#include "../user-feedback.hpp"

namespace mods::builder {
	using args_t = std::vector<std::string>;
	extern std::tuple<bool,std::string,uint64_t> save_player(char_data* obj);

	std::tuple<bool,std::string,int> scale_mob(mob_rnum id,int level, uint64_t raid_id,mob_vnum* vnum) {
		if(id <= 0 || id == NOBODY || id >= mob_proto.size()) {
			return {false,"mob id is invalid",-1};
		}
		auto& orig = mob_proto[id];
		mob_proto.push_back(mob_proto[id]);
		auto new_id = mob_proto.size() - 1;
		auto obj = &mob_proto[new_id];

		float orig_level = orig.player.level;

		obj->player.level = level;

		float exp = orig.points.exp;
		float mana = orig.points.mana;
		float max_mana = orig.points.max_mana;
		float hit = orig.points.hit;
		float max_hit = orig.points.max_hit;
		float move = orig.points.move;
		float max_move = orig.points.max_move;
		float gold = orig.points.gold;
		float hitroll = orig.points.hitroll;
		float damroll = orig.points.damroll;

		exp /= orig_level;
		mana = mana / orig_level;
		max_mana /= orig_level;
		hit /= orig_level;
		max_hit /= orig_level;
		move /= orig_level;
		max_move /= orig_level;
		gold /= orig_level;
		hitroll /= orig_level;
		damroll /= orig_level;

		obj->points.exp = exp * level;
		obj->points.mana = mana * level;
		obj->points.max_mana = max_mana * level;
		obj->points.hit = hit * level;
		obj->points.max_hit = max_hit * level;
		obj->points.move = move * level;
		obj->points.max_move = max_move * level;
		obj->points.gold = gold * level;
		obj->points.hitroll = hitroll * level;
		obj->points.damroll = damroll * level;

		/** Ignoring weight and height */

		float electronics = orig.real_abils.electronics ;
		float armor = orig.real_abils.armor ;
		float marksmanship = orig.real_abils.marksmanship ;
		float sniping = orig.real_abils.sniping ;
		float demolitions = orig.real_abils.demolitions ;
		float chemistry = orig.real_abils.chemistry ;
		float weapon_handling = orig.real_abils.weapon_handling ;
		float strategy = orig.real_abils.strategy ;
		float medical = orig.real_abils.medical ;
		float str = orig.real_abils.str ;
		float str_add = orig.real_abils.str_add ;
		float intel = orig.real_abils.intel ;
		float wis = orig.real_abils.wis ;
		float dex = orig.real_abils.dex ;
		float con = orig.real_abils.con ;
		float cha = orig.real_abils.cha ;
		float damnodice = orig.mob_specials.damnodice ;
		float attack_type = orig.mob_specials.attack_type ;
		float damsizedice = orig.mob_specials.damsizedice ;
		float experience = orig.mob_specials.experience ;

		electronics /= orig_level;
		armor /= orig_level;
		marksmanship /= orig_level;
		sniping /= orig_level;
		demolitions /= orig_level;
		chemistry /= orig_level;
		weapon_handling /= orig_level;
		strategy /= orig_level;
		medical /= orig_level;
		str /= orig_level;
		str_add /= orig_level;
		intel /= orig_level;
		wis /= orig_level;
		dex /= orig_level;
		con /= orig_level;
		cha /= orig_level;
		damnodice /= orig_level;
		attack_type /= orig_level;
		damsizedice /= orig_level;
		experience /= orig_level;

		obj->real_abils.electronics = electronics * level;
		obj->real_abils.armor = armor * level;
		obj->real_abils.marksmanship = marksmanship * level;
		obj->real_abils.sniping = sniping * level;
		obj->real_abils.demolitions = demolitions * level;
		obj->real_abils.chemistry = chemistry * level;
		obj->real_abils.weapon_handling = weapon_handling * level;
		obj->real_abils.strategy = strategy * level;
		obj->real_abils.medical = medical * level;
		obj->real_abils.str = str * level;
		obj->real_abils.str_add = str_add * level;
		obj->real_abils.intel = intel * level;
		obj->real_abils.wis = wis * level;
		obj->real_abils.dex = dex * level;
		obj->real_abils.con = con * level;
		obj->real_abils.cha = cha * level;
		obj->mob_specials.damnodice = damnodice * level;
		obj->mob_specials.damsizedice = damsizedice * level;
		obj->mob_specials.attack_type = attack_type * level;
		obj->mob_specials.damsizedice = damsizedice * level;
		obj->mob_specials.experience = experience * level;

		obj->mob_specials.raid_id = raid_id;
		obj->mob_specials.vnum = next_mob_number();
		if(vnum) {
			*vnum = obj->mob_specials.vnum;
		}
		obj->mob_specials.extended_mob_type = orig.mob_specials.extended_mob_type;

		auto s = mods::builder::save_player(obj);
		if(!std::get<0>(s)) {
			return {0,std::get<1>(s),0};
		}
		return {1,"okay",std::get<2>(s)};
	}

};
