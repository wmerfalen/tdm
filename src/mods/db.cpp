#include "lmdb.hpp"
#include "db.hpp"
#include "meta_utils.hpp"
#include "player.hpp"

namespace mods::db {

aligned_int_t save_record_get_id(
		const std::string& table,
		mutable_map_t* values){
	auto status = mods::db::save_record(table,values);
	if(std::get<0>(status)){
		return std::get<2>(status);
	}
	else{
		return 0;
	}
}

aligned_int_t initialize_row(
		const std::string& table){
	auto ptr_db = mods::globals::db.get();
	ptr_db->renew_txn();
	std::string id_list;
	auto ret = ptr_db->get(db_key({table,"id_list"}),id_list);
	if(ret < 0){
		return 0;
	}

	aligned_int_t next_id = 0;
	std::vector<aligned_int_t> deserialized_id_list;
	if(id_list.begin() == id_list.end()){
		deserialized_id_list.push_back(0);
		next_id = 0;
	}
	else{
		std::copy(id_list.begin(),id_list.end(),std::back_inserter(deserialized_id_list));
	}
	++next_id;
	deserialized_id_list[0] = next_id;
	id_list.clear();
	deserialized_id_list.push_back(next_id);
	debug("saving deserialized_id_list with the next_id: "<< next_id << " and a new size of: " << deserialized_id_list.size());
	std::string str_buffer{""};
	std::copy(deserialized_id_list.begin(),deserialized_id_list.end(),
			std::back_inserter(str_buffer));
	ret = ptr_db->put(db_key({table,"id_list"}),str_buffer);
	if(ret < 0){
		return 0;
	}

	ret = ptr_db->put(db_key({table.data(),std::to_string(next_id)}),"active");
	if(ret < 0){
		return 0;
	}
	auto tuple_return = ptr_db->commit();
	if(!std::get<0>(tuple_return)){
		std::cerr << "error: commit failed (initialize_row)\n";
		return 0;
	}
	return next_id;
}

tuple_status_t save_record(const std::string& table,mutable_map_t* values){
	std::string existing_records_primary_key_id = "";
	aligned_int_t pk_id = 0;
		auto items_placed = mods::meta_utils::do_meta_easy(
				table,values,&pk_id);
		if(items_placed < 0){
			std::cout << "info: no meta values were placed\n";
		}
		if(items_placed >= 0){
			std::cout << "info: " << items_placed << " meta values placed\n";
		}
		std::cout << "debug: pk_id so far: '" << pk_id << "\n";

	if(pk_id != 0){
		std::cout << "debug: primary key 'id' good. Updating record\n";
	}else{
		pk_id = initialize_row(table);
		if(pk_id == 0){
			return {false,"Couldn't initialize row",0};
		}
	}
	assert(pk_id != 0);
	(*values)["id"] = pk_id;
		auto write_status =  lmdb_write_values(
				table,
				values);
		if(!std::get<0>(write_status)){
			std::cerr << "error: lmdb_write_values failed with: '" << std::get<1>(write_status) << "'\n";
		}else{
			std::cout << "debug: wrote record\n";
		}
	auto tuple_status = mods::globals::db->commit();
	if(std::get<0>(tuple_status)){
		return {true,"saved",pk_id};
	}else{
		return {false,std::get<1>(tuple_status),0};
	}
}
tuple_status_t save_char(
		std::shared_ptr<mods::player> player_ptr){
	mutable_map_t values;
	lmdb_export_char(player_ptr,values);
	return save_record("player",&values);
}

tuple_status_t lmdb_write_values(
		const std::string& table,
		mutable_map_t* values){
	auto ptr_db = mods::globals::db.get();
	ptr_db->renew_txn();
	uint64_t errors =0;
	std::string pk_id = (*values)["id"];
	for(auto & [key,value] : *values){
		auto ret = ptr_db->put(db_key({table,key,pk_id}),value);
		if(ret){
			++errors;
		}
	}
	if(errors){
		return {true,std::string("saved, but with ") + std::to_string(errors) + " errors",errors};
	}
	return {true,"saved",0};
}

void lmdb_export_char(std::shared_ptr<mods::player> player_ptr, mutable_map_t &values){
	/** TODO: instead of using the char_data accesses, create functions(or use existing ones) on mods::player object */
	auto ch = player_ptr->cd();
		values["id"] = std::to_string(ch->db_id());
		values["player_name"] = std::to_string(ch->player.name);
		values["player_short_description"] = std::to_string(ch->player.short_descr);
		values["player_long_description"] = std::to_string(ch->player.long_descr);
		values["player_action_bitvector"] = (std::to_string(ch->char_specials.saved.act));
		values["player_ability_strength"] = (std::to_string(ch->real_abils.str));
		values["player_ability_strength_add"] = (std::to_string(ch->real_abils.str_add));
		values["player_ability_intelligence"] = (std::to_string(ch->real_abils.intel));
		values["player_ability_wisdom"] = (std::to_string(ch->real_abils.wis));
		values["player_ability_dexterity"] = (std::to_string(ch->real_abils.dex));
		values["player_ability_constitution"] = (std::to_string(ch->real_abils.con));
		values["player_ability_charisma"] = (std::to_string(ch->real_abils.cha));
		values["player_ability_alignment"] = (std::to_string(ch->char_specials.saved.alignment));
		values["player_attack_type"] = (std::to_string(ch->real_abils.con));
		values["player_ability_constitution"] = (std::to_string(ch->real_abils.con));
		values["player_max_hitpoints"] = (std::to_string(ch->points.max_hit));
		values["player_max_mana"] = (std::to_string(ch->points.max_mana));
		values["player_max_move"] = (std::to_string(ch->points.max_move));
		values["player_gold"] = (std::to_string(ch->points.gold));
		values["player_exp"] = (std::to_string(ch->points.exp));
		values["player_sex"] = ch->player.sex == SEX_MALE ? std::string("M") : std::string("F");
		values["player_hitpoints"] = (std::to_string(ch->points.max_hit));
		values["player_mana"] = (std::to_string(ch->points.mana));
		values["player_move"] = (std::to_string(ch->points.move));
		values["player_damroll"] = (std::to_string(ch->points.damroll));
		values["player_weight"] = (std::to_string(ch->player.weight));
		values["player_height"] = (std::to_string(ch->player.height));
		values["player_class"] = (std::to_string(ch->player.chclass));
		values["player_title"] = std::to_string(ch->player.title);
		values["player_hometown"] = (std::to_string(ch->player.hometown));

		values["player_damnodice"] = std::string("0");
		values["player_damsizedice"] = std::string("0");
		values["player_attack_type"] = std::string("0");
		values["player_type"] = std::string("PC");
		values["player_alignment"] = std::to_string(
			ch->char_specials.saved.alignment);
		values["player_level"] = std::to_string(ch->player.level);
		values["player_hitroll"] = std::to_string(ch->points.hitroll);
		values["player_armor"] = std::to_string(ch->points.armor);
		return;
}

};
