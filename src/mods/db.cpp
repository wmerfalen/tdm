#include "db.hpp"
#include "lmdb.hpp"
#include "meta_utils.hpp"
#include "player.hpp"
#include "pq.hpp"

namespace mods::db {

aligned_int_t save_record_get_id(
		const std::string& table,
		mutable_map_t* values){
	auto status = mods::db::save_record(table,values,(*values)["id"]);
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
	std::string id_list = "";
	id_list = db_get(db_key({table,"id_list"}));
	//std::cout << "id_list: '" << id_list << "' .size(): " << id_list.length() << "\n";
	//std::cout << "debug: id_list buffer: '" << id_list << "'\n";
	aligned_int_t next_id = 0;
	std::vector<aligned_int_t> deserialized_id_list;
	if(id_list.length() == 0){
		deserialized_id_list.push_back(0);
		next_id = 0;
	}
	else{
		std::copy(id_list.begin(),id_list.end(),std::back_inserter(deserialized_id_list));
		next_id = ++deserialized_id_list[0];
	}
	id_list.clear();
	deserialized_id_list.push_back(next_id);
	debug("saving deserialized_id_list with the next_id: "<< next_id << " and a new size of: " << deserialized_id_list.size());
	std::string str_buffer{""};
	std::copy(deserialized_id_list.begin(),deserialized_id_list.end(),
			std::back_inserter(str_buffer));
	auto ret = ptr_db->put(db_key({table,"id_list"}),str_buffer);
	if(ret < 0){
		std::cerr << "failed to place id_list!!!\n";
		return 0;
	}

	ret = ptr_db->put(db_key({table,"id",std::to_string(next_id)}),"active");
	ret = ptr_db->put(db_key({table,"active",std::to_string(next_id)}),"active");
	if(ret < 0){
		std::cerr << "failed to place active key!!!\n";
		return 0;
	}
	auto tuple_return = ptr_db->commit();
	if(!std::get<0>(tuple_return)){
		std::cerr << "error: commit failed (initialize_row)\n";
		return 0;
	}
	return next_id;
}

tuple_status_t new_record(const std::string& table,mutable_map_t* values){
	auto pk_id = mods::db::initialize_row(table);
	//std::cout << "debug: new pk_id: '" << pk_id << "'\n";
	return save_record(table,values,std::to_string(pk_id));
}

tuple_status_t save_record(const std::string& table,mutable_map_t* values,std::string pk_id){
	mods::globals::db->renew_txn();
	for(auto & meta_key : mods::meta_utils::get_all_meta_values(table,values)){
		//std::cout << "debug: save_record. Putting: '" << meta_key << " as '" << pk_id << "'\n";
		db_put(meta_key,pk_id);
	}
		auto write_status =  lmdb_write_values(
				table,
				values,pk_id);
		if(!std::get<0>(write_status)){
			std::cerr << "error: lmdb_write_values failed with: '" << std::get<1>(write_status) << "'\n";
		}
	auto tuple_status = mods::globals::db->commit();
	if(std::get<0>(tuple_status)){
		return {true,"saved",mods::util::stoi<aligned_int_t>(pk_id)};
	}else{
		return {false,std::get<1>(tuple_status),0};
	}
}

tuple_status_t save_char(
		std::shared_ptr<mods::player> player_ptr){
	mutable_map_t values;
	lmdb_export_char(player_ptr,values);
	return save_record("player",&values,std::to_string(player_ptr->get_db_id()));
}
tuple_status_t save_new_char(
		std::shared_ptr<mods::player> player_ptr){
	std::cerr << "[DEPRECATED]: calls to mods::lmdb::save_new_char will be ignored!\n";
	//player_ptr->set_db_id(initialize_row("player"));
	mutable_map_t values;
	lmdb_export_char(player_ptr,values);
	return save_record("player",&values,values["id"]);
}

tuple_status_t lmdb_write_values(
		const std::string& table,
		mutable_map_t* values,std::string pk_id){
	mods::globals::db->renew_txn();
	for(auto & [key,value] : *values){
		 db_put(db_key({table,key,pk_id}),value);
	}
	return {true,"saved",0};
}

void lmdb_export_char(std::shared_ptr<mods::player> player_ptr, mutable_map_t &values){
	/** TODO: instead of using the char_data accesses, create functions(or use existing ones) on mods::player object */
	auto ch = player_ptr->cd();
		values["player_password"] = player_ptr->password();
		values["player_affection_plr_bitvector"] = std::to_string(player_ptr->get_affected_plr());
		values["player_affection_bitvector"] = std::to_string(player_ptr->get_affected());

		if(values["player_affection_plr_bitvector"].length() == 0){
			values.erase("player_affection_plr_bitvector");
		}
		if(values["player_affection_bitvector"].length() == 0){
			values.erase("player_affection_bitvector");
		}

		values["player_name"] = player_ptr->name().c_str();
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
		values["player_max_hitpoints"] = (std::to_string(player_ptr->max_hp()));
		values["player_max_mana"] = (std::to_string(player_ptr->max_mana()));
		values["player_max_move"] = (std::to_string(player_ptr->max_move()));
		values["player_gold"] = (std::to_string(player_ptr->gold()));
		values["player_exp"] = (std::to_string(player_ptr->exp()));
		values["player_sex"] = player_ptr->sex() == SEX_MALE ? std::string("M") : std::string("F");
		values["player_hitpoints"] = (std::to_string(player_ptr->hp()));
		values["player_mana"] = (std::to_string(player_ptr->mana()));
		values["player_move"] = (std::to_string(player_ptr->move()));
		values["player_damroll"] = (std::to_string(player_ptr->damroll()));
		values["player_weight"] = (std::to_string(player_ptr->weight()));
		values["player_height"] = (std::to_string(player_ptr->height()));
		values["player_class"] = (std::to_string(player_ptr->cd()->player.chclass));
		values["player_title"] = std::to_string(player_ptr->title());
		values["player_hometown"] = (std::to_string(player_ptr->hometown()));
		values["player_damnodice"] = std::string("0");
		values["player_damsizedice"] = std::string("0");
		values["player_attack_type"] = std::string("0");
		values["player_type"] = std::string("PC");
		values["player_alignment"] = std::to_string(
			ch->char_specials.saved.alignment);
		values["player_level"] = std::to_string(player_ptr->level());
		values["player_hitroll"] = std::to_string(player_ptr->cd()->points.hitroll);
		values["player_armor"] = std::to_string(player_ptr->cd()->points.armor);
		if(player_ptr->get_prefs() == 0){
			values["player_preferences"] = "0";
		}else{
			values["player_preferences"] = std::to_string(player_ptr->get_prefs());
		}
		return;
}

int load_record(const std::string& table, aligned_int_t pk, mutable_map_t& values){
	auto str_pk = std::to_string(pk);
	return load_record(table,str_pk,values);
}
int load_record(const std::string& table, const std::string& pk, mutable_map_t& values){
	std::string value;
	int count = 0;
	for(auto & key : mods::globals::db->fields_to_grab(table)){
		value = db_get(db_key({table,key,pk}));
		//std::cout << "debug: load_record key: '" << key << "' value: '" << value << "'\n";
		values[key] = value;
		++count;
	}
	return count;
}


int load_record_by_meta(const std::string& table, mutable_map_t* values,mutable_map_t& out_record){
	if(values == nullptr){
		std::cerr << "error: values is a nullptr in load_record_by_meta\n";
		return -1;
	}
	int count = 0;
	for(auto & meta_key : mods::meta_utils::get_all_meta_values(table,values)){
		std::string pk_id;
		pk_id = db_get(meta_key);
		auto opt_pk = mods::util::stoi_optional<aligned_int_t>(pk_id);
		if(opt_pk.has_value() && opt_pk.value() != 0){
			count = 0;
			//std::cout << "debug: found pk: '" << opt_pk.value() << "'\n";
			out_record["id"] = pk_id;
			std::vector<std::string> fields_to_grab = mods::schema::db[table];
			if(mods::globals::db->is_using_pluck_filter()){
				fields_to_grab = mods::globals::db->fields_to_grab(table);
			}
			for(auto & key : fields_to_grab){
				std::string fetched = db_get(db_key({table,key,pk_id}));
				//std::cout << "debug: load_record key: '" << key << "' value: '" << fetched << "'\n";
				out_record[key] = fetched;
				count++;
			}
			return count;
		}
	}
	return count;
}


};
