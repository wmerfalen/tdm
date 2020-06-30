#include "room.hpp"
#include <vector>
#include <string>

namespace mods::orm {
	using sql_compositor = mods::sql::compositor<mods::pq::transaction>;

	uint64_t room::initialize_row(){
		this->init();
		return 0;
	}
	sql_save_map_t room::export_insert() {
		auto values = export_class();
		auto it = values.find("id");
		values.erase(it);
		return std::move(values);
	}
	sql_save_map_t room::export_class() {
		sql_save_map_t values;
		values["id"] = std::to_string(id);
		values["room_number"] = std::to_string(room_number);
		values["zone"]= std::to_string(zone);
		values["sector_type"] = std::to_string(sector_type);
		values["name"] = name;
		values["description"] = description;
		values["ex_keyword"] = ex_keyword;
		values["ex_description"] = ex_description;
		values["light"] = std::to_string(light);
		values["room_flag"] = std::to_string(room_flag);
		return std::move(values);
	}
	int16_t room::feed(const pqxx::result::reference & row){
		this->init();
		this->loaded = 0;
		this->id = row["id"].as<uint64_t>();
		this->room_number = row["room_number"].as<room_vnum>();
		this->zone = row["zone"].as<zone_vnum>();
		this->sector_type = row["sector_type"].as<uint32_t>();
		this->name = row["name"].c_str();
		this->description = row["description"].c_str();
		this->ex_keyword =  row["ex_keyword"].c_str();
		this->ex_description =  row["ex_description"].c_str();
		this->light = row["light"].as<uint64_t>();
		this->room_flag = row["room_flag"].as<uint64_t>();
		return 0;
	}
	void room::init(){
		id = 0;
		room_number = 0;
		loaded = 0;
		zone = 0;
		sector_type = 0;
		name.clear();
		description.clear();
		ex_keyword.clear();
		ex_description.clear();
		light = 0;
		room_flag = 0;
	}

	void foobar(){
		mods::orm::room r;

		r.room_number = 500;
		r.zone = 1;
		r.sector_type = 0;
		r.name = "Plato's Weapons and Armor";
		r.description = "Weapons. Ammo. Armor. Explosives. Everything.";
		r.ex_keyword = "";
		r.ex_description = "";
		r.light = 0;
		r.room_flag = 0;

		auto status3 = mods::orm::base::insert_returing<mods::orm::room>(r, "id");
		if(std::get<0>(status3) < 0){
			std::cerr << "[error] insert_returning failed!:\n";
		}
		std::cerr << "returned id:" << std::get<1>(status3) << "\n";

		auto status = mods::orm::base::load_by_pkid(r,std::get<1>(status3));
		if(std::get<0>(status) < 0){
			std::cerr << "[error] load_by_pkid failed!: '" << std::get<1>(status) << "'\n";
		}
		std::cerr << ":)\n";
		r.id = 0;
		auto status2 = mods::orm::base::delete_from(r);
		if(std::get<0>(status2) < 0){
			std::cerr << "[error] delete_from failed!: '" << std::get<1>(status2) << "'\n";
		}
		std::cerr << ":)\n";


		/** load multiple into object container */
		using list_t = std::vector<mods::orm::room>;
		list_t result_set;
		mods::orm::base::load_where(result_set, "zone", "=", "7");
		for(auto & row : result_set){
			std::cerr << "[row]: " << row.dump_cols({"id","zone","sector_type","room_number"}) << "\n";
		}
		exit(0);
	}
	std::string room::dump_cols(){
		std::string buffer = "";
#define MENTOC_LAZY(A)\
		buffer += #A;\
		buffer += ": ";\
		buffer += std::to_string(A);\
		buffer += "\n";

#define MENTOC_S(A)\
		buffer += #A;\
		buffer += ": ";\
		buffer += A;\
		buffer += "\n";
		MENTOC_LAZY(loaded);
		MENTOC_LAZY(id);
		MENTOC_LAZY(room_number);
		MENTOC_LAZY(zone);
		MENTOC_LAZY(sector_type);
		MENTOC_S(name);
		MENTOC_S(description);
		MENTOC_S(ex_keyword);
		MENTOC_S(ex_description);
		MENTOC_LAZY(light);
		MENTOC_LAZY(room_flag);
#undef MENTOC_LAZY
#undef MENTOC_S
		return buffer;
	}
	std::string room::dump_cols(std::vector<std::string> columns){
		std::string buffer = "";
		for(auto c : columns){
#define MENTOC_LAZY(A)\
			if(c.compare(#A) == 0){\
				buffer += #A;\
				buffer += ": ";\
				buffer += std::to_string(A);\
				buffer += "\n";\
			}
#define MENTOC_S(A)\
			if(c.compare(#A) == 0){\
				buffer += #A;\
				buffer += ": ";\
				buffer += A;\
				buffer += "\n";\
			}
			MENTOC_LAZY(loaded);
			MENTOC_LAZY(id);
			MENTOC_LAZY(room_number);
			MENTOC_LAZY(zone);
			MENTOC_LAZY(sector_type);
			MENTOC_S(name);
			MENTOC_S(description);
			MENTOC_S(ex_keyword);
			MENTOC_S(ex_description);
			MENTOC_LAZY(light);
			MENTOC_LAZY(room_flag);
		}
#undef MENTOC_LAZY
#undef MENTOC_S
		return buffer;
	}
};

