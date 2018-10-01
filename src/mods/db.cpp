#include "lmdb.hpp"
#include "db.hpp"

namespace mods::db {
tuple_status_t lmdb_commit(db_handle* ptr_db,std::string_view from_where){
	lmdb_return(ptr_db->commit(),"lmdb_commit");
}
tuple_status_t lmdb_save_char(
		std::string_view player_name,
		char_data * ch,
		db_handle* ptr_db){
	mutable_map_t values;
	lmdb_export_char(ch,values);

	std::string_view table = "player";
	std::string existing_user_id;
	aligned_int_t user_id = 0;
	lmdb_get(ptr_db,
		db_key({table,"meta","name",player_name.data()}),
		existing_user_id,
		{
			std::stringstream stream;
			stream << existing_user_id;
			stream >> user_id;
		},{}
	);

	/**
	 * If the user id was successfully parsed, it would *not* be zero.
	 * So this next if statement is checking if we want to update or not
	 */
	if(user_id != 0){
		lmdb_return_if_fail(lmdb_update_char(player_name,ptr_db,values,existing_user_id),"lmdb_write_meta_values");
	}else{
		lmdb_return_if_fail(lmdb_insert_char(player_name,ptr_db,values),"lmdb_write_meta_values");
	}
	return lmdb_commit(ptr_db,"lmdb_save_char");
}

tuple_status_t lmdb_update_char(
		std::string_view player_name,
		db_handle* ptr_db,
		const mutable_map_t &values,
		std::string_view str_id){
	std::string table = "player";
	lmdb_return_if_fail(lmdb_write_meta_values(table,ptr_db,{
			{std::string("name|") + player_name.data(),str_id.data()}
	}),"lmdb_update_char");
	return lmdb_write_values(player_name,
			table,
			ptr_db,
			values
	);
}

tuple_status_t lmdb_insert_char(
		std::string_view player_name,
		db_handle* ptr_db,
		const mutable_map_t &values){
	std::string table = "player";
	bool init_error = 0;
	auto id = initialize_row(table,ptr_db,init_error);
	if(init_error){
		return {false,"Couldn't initialize row",0};
	}
	lmdb_write_meta_values(table,ptr_db,{
			{std::string("name|") + player_name.data(),std::to_string(id)}
	});
	return lmdb_write_values(std::to_string(id),
			"player",
			ptr_db,
			values
	);
}

tuple_status_t lmdb_write_meta_values(
		std::string_view table,
		db_handle* ptr_db,
		std::map<std::string,std::string> meta_values){
	if(meta_values.size()){
		ptr_db->renew_txn();
		for(auto & [key,value] : meta_values){
			lmdb_put(
					ptr_db,db_key({table,"meta",key}),value,
					{ ;; },{
						ptr_db->dump_status();
						ptr_db->abort_txn();
						return tuple_status_t(true,"lmdb_write_meta_values: Failed to place key in db",0);
					};
			);
		}
		return lmdb_commit(ptr_db,"lmdb_write_meta_values");
	}else{
		return {false,"empty map",0};
	}
	return {false,"unreachable code reached. How?",0};
}

tuple_status_t lmdb_load_by_meta(
		std::string_view meta_key,
		std::string_view meta_value,
		std::string_view table,
		db_handle* ptr_db,
		mutable_map_t& values
		){
	std::string user_id = "0";
	ptr_db->renew_txn();
	auto ret = ptr_db->get(db_key(
				{table.data(),"meta",meta_key.data(),meta_value.data()}),
			user_id);
	if(db_handle::KEY_FETCHED_OKAY != ret){
		ptr_db->abort_txn();
		return {false,"Meta value doesn't exist",0};
	}
	aligned_int_t meta_int_id = 0;
	std::stringstream stream;
	stream << user_id;
	stream >> meta_int_id;
	if(meta_int_id == 0){
		return {false,"Couldn't unserialize meta id",0};
	}
	assert(mods::schema::db.find(std::string(table.data())) != 
			mods::schema::db.end());
	std::string value;
	for(auto & column : mods::schema::db[std::string(table)]){
		value = "";
		ret = ptr_db->get(db_key(
				{table.data(),column,user_id}),
			value);
		if(db_handle::KEY_FETCHED_OKAY == ret){
			values[column] = value; 
		}else{
			values[column] = "";
		}
	}
	return {true,"Fetched",meta_int_id};
}

tuple_status_t lmdb_write_values(
		std::string_view str_id,
		std::string_view table,
		db_handle* ptr_db,
		const mutable_map_t& values){
	ptr_db->renew_txn();
	for(auto & [key,value] : values){
		auto ptr_if = std::get_if<std::string>(&value);
		if(ptr_if == nullptr){
			continue;
		}
		auto ret = ptr_db->put(db_key({table,key,str_id}),*ptr_if);
		if(ret){
			ptr_db->dump_status();
			ptr_db->abort_txn();
			return {false,"Failed to place key in db",0};
		}
	}
	return {true,"",0};
}

void lmdb_export_char(char_data* ch, mutable_map_t &values){
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
