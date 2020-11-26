#include "player-base-ability.hpp"
#include "../interpreter.hpp"

namespace mods::orm {
	uint64_t player_base_ability::initialize_row(player_ptr_t &player) {
		init();
		pba_player_id = player->db_id();
		auto status = this->create<player_base_ability>(this);
		if(ORM_SUCCESS(status)){
			updated_at = created_at = time(nullptr);
			loaded = 1;
			id = pba_id = std::get<2>(status);
		}
		return id;
	}
	strmap_t player_base_ability::export_class() {
		strmap_t values;
		values["pba_id"] = std::to_string(pba_id);
		values["pba_player_id"] = std::to_string(pba_player_id);
		values["pba_str"] = std::to_string(pba_str);
		values["pba_str_add"] = std::to_string(pba_str_add);
		values["pba_intel"] = std::to_string(pba_intel);
		values["pba_wis"] = std::to_string(pba_wis);
		values["pba_dex"] = std::to_string(pba_dex);
		values["pba_con"] = std::to_string(pba_con);
		values["pba_cha"] = std::to_string(pba_cha);
		values["pba_electronics"] = std::to_string(pba_electronics);
		values["pba_armor"] = std::to_string(pba_armor);
		values["pba_marksmanship"] = std::to_string(pba_marksmanship);
		values["pba_sniping"] = std::to_string(pba_sniping);
		values["pba_demolitions"] = std::to_string(pba_demolitions);
		values["pba_chemistry"] = std::to_string(pba_chemistry);
		values["pba_weapon_handling"] = std::to_string(pba_weapon_handling);
		values["pba_strategy"] = std::to_string(pba_strategy);
		values["pba_medical"] = std::to_string(pba_medical);
		//values["created_at"] = std::to_string(created_at);
		//values["updated_at"] = std::to_string(updated_at);
		return std::move(values);
	}
	int16_t player_base_ability::load_by_player(uint64_t player_id){
		return std::get<0>(this->read<player_base_ability>(this,"pba_player_id",std::to_string(player_id)));
	}
	bool player_base_ability::player_row_exists(uint64_t player_id){
		return !(ORM_NO_RESULTS(this->read<player_base_ability>(this,"pba_player_id",std::to_string(player_id))));
	}
	void player_base_ability::feed_ability_data(char_ability_data& data){
		data.str = pba_str;
		data.str_add = pba_str_add;
		data.intel = pba_intel;
		data.wis = pba_wis;
		data.dex = pba_dex;
		data.con = pba_con;
		data.cha = pba_cha;
		data.electronics = pba_electronics;
		data.armor = pba_armor;
		data.marksmanship = pba_marksmanship;
		data.sniping = pba_sniping;
		data.demolitions = pba_demolitions;
		data.chemistry = pba_chemistry;
		data.weapon_handling = pba_weapon_handling;
		data.strategy = pba_strategy;
		data.medical = pba_medical;
	}
	void player_base_ability::feed_player(player_ptr_t& player){
		this->load_by_player(player->db_id());
		this->feed_ability_data(player->real_abils());
	}
	int16_t player_base_ability::feed(const pqxx::result::reference & row){
		init();
		loaded = 0;
		id = pba_id = row["pba_id"].as<uint64_t>();
		pba_player_id = row["pba_player_id"].as<uint64_t>();
		pba_str = row["pba_str"].as<player_base_ability::skill_measurement_t>();
		pba_str_add = row["pba_str_add"].as<player_base_ability::skill_measurement_t>();
		pba_intel = row["pba_intel"].as<player_base_ability::skill_measurement_t>();
		pba_wis = row["pba_wis"].as<player_base_ability::skill_measurement_t>();
		pba_dex = row["pba_dex"].as<player_base_ability::skill_measurement_t>();
		pba_con = row["pba_con"].as<player_base_ability::skill_measurement_t>();
		pba_cha = row["pba_cha"].as<player_base_ability::skill_measurement_t>();
		pba_electronics = row["pba_electronics"].as<player_base_ability::skill_measurement_t>();
		pba_armor = row["pba_armor"].as<player_base_ability::skill_measurement_t>();
		pba_marksmanship = row["pba_marksmanship"].as<player_base_ability::skill_measurement_t>();
		pba_sniping = row["pba_sniping"].as<player_base_ability::skill_measurement_t>();
		pba_demolitions = row["pba_demolitions"].as<player_base_ability::skill_measurement_t>();
		pba_chemistry = row["pba_chemistry"].as<player_base_ability::skill_measurement_t>();
		pba_weapon_handling = row["pba_weapon_handling"].as<player_base_ability::skill_measurement_t>();
		pba_strategy = row["pba_strategy"].as<player_base_ability::skill_measurement_t>();
		pba_medical = row["pba_medical"].as<player_base_ability::skill_measurement_t>();
		created_at = mods::util::pg_timestamp_to_long(row["created_at"].c_str());
		updated_at = mods::util::pg_timestamp_to_long(row["updated_at"].c_str());
		return 0;
	}
	int16_t player_base_ability::save_by_player(player_ptr_t& player){
		bool insert = !this->player_row_exists(player->db_id());
		if(insert){
			this->init();
			auto values = this->export_class();
			values.erase("pba_id");
			values.erase("created_at");
			values.erase("updated_at");
			loaded = 0;
		}
		pba_player_id = player->db_id();
		pba_str = player->real_abils().str;
		pba_str_add = player->real_abils().str_add;
		pba_intel = player->real_abils().intel;
		pba_wis = player->real_abils().wis;
		pba_dex = player->real_abils().dex;
		pba_con = player->real_abils().con;
		pba_cha = player->real_abils().cha;
		pba_electronics = player->real_abils().electronics;
		pba_armor = player->real_abils().armor;
		pba_marksmanship = player->real_abils().marksmanship;
		pba_sniping = player->real_abils().sniping;
		pba_demolitions = player->real_abils().demolitions;
		pba_chemistry = player->real_abils().chemistry;
		pba_weapon_handling = player->real_abils().weapon_handling;
		pba_strategy = player->real_abils().strategy;
		pba_medical = player->real_abils().medical;
		this->dump();
		if(insert){
			return std::get<0>(this->create(this));
		}
		return std::get<0>(this->update(this));
	}
	void player_base_ability::init(){
		id = pba_id = 0;
		pba_player_id = 0;
		pba_str = 0;
		pba_str_add = 0;
		pba_intel = 0;
		pba_wis = 0;
		pba_dex = 0;
		pba_con = 0;
		pba_cha = 0;
		pba_electronics = 0;
		pba_armor = 0;
		pba_marksmanship = 0;
		pba_sniping = 0;
		pba_demolitions = 0;
		pba_chemistry = 0;
		pba_weapon_handling = 0;
		pba_strategy = 0;
		pba_medical = 0;
		updated_at = created_at = 0;
		loaded = 0;
	}
	void player_base_ability::dump(){
		std::cerr << "id->" << id << "\n";
		std::cerr << "pba_id->" << pba_id << "\n";
		std::cerr << "pba_player_id->" << pba_player_id << "\n";
		std::cerr << "pba_str->" << pba_str << "\n";
		std::cerr << "pba_str_add->" << pba_str_add << "\n";
		std::cerr << "pba_intel->" << pba_intel << "\n";
		std::cerr << "pba_wis->" << pba_wis << "\n";
		std::cerr << "pba_dex->" << pba_dex << "\n";
		std::cerr << "pba_con->" << pba_con << "\n";
		std::cerr << "pba_cha->" << pba_cha << "\n";
		std::cerr << "pba_electronics->" << pba_electronics << "\n";
		std::cerr << "pba_armor->" << pba_armor << "\n";
		std::cerr << "pba_marksmanship->" << pba_marksmanship << "\n";
		std::cerr << "pba_sniping->" << pba_sniping << "\n";
		std::cerr << "pba_demolitions->" << pba_demolitions << "\n";
		std::cerr << "pba_chemistry->" << pba_chemistry << "\n";
		std::cerr << "pba_weapon_handling->" << pba_weapon_handling << "\n";
		std::cerr << "pba_strategy->" << pba_strategy << "\n";
		std::cerr << "pba_medical->" << pba_medical << "\n";
		std::cerr << "created_at->" << created_at << "\n";
		std::cerr << "updated_at->" << updated_at << "\n";
		std::cerr << "loaded->" << loaded << "\n";
	};

	namespace pba {
	ACMD(do_save_my_pba){
		/** code here */
		auto vec_args = PARSE_ARGS();
		mods::orm::player_base_ability pba;
		auto status = pba.load_by_player(player->db_id());
		player->send("Status: %d\r\n",status);
		pba.dump();
		status = pba.save_by_player(player);
		player->send("SVP status: %d\r\n",status);
		status = pba.load_by_player(player->db_id());
		player->send("LVP Status: %d\r\n",status);
		pba.dump();
	};
	
	void init(){
		mods::interpreter::add_command("save_my_pba", POS_RESTING, do_save_my_pba, LVL_BUILDER,0);
	}
	};

};

