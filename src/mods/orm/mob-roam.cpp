#include "mob-roam.hpp"
#include "util.hpp"

namespace mods::orm {
	void mob_roam::get_compound_list() {
		using statement = std::vector<mods::orm::util::statement_t>;
		using c = std::vector<pqxx::result::reference>;
		c container;
		statement statements;
		statements.emplace_back("mob_virtual_number","=","410",true,false);
		statements.emplace_back("profile_name","=","foobar",false,false);
		mods::orm::util::load_where<c,sql_compositor,statement>(
		    container,
		    this->table_name(),
		    statements
		);
		for(const auto& row : container) {
			std::cerr << green_str("mob_roam compound:") << row["profile_name"].c_str() << ", id:" << row["id"].as<int>() << "\n";
		}
	}
	void mob_roam::delete_foobar_profile() {
		using statement = std::vector<mods::orm::util::statement_t>;
		using c = std::vector<pqxx::result::reference>;
		c container;
		statement statements;
		statements.emplace_back("mob_virtual_number","=","410",true,false);
		statements.emplace_back("profile_name","=","foobar",false,false);
		mods::orm::util::delete_where<c,sql_compositor,statement>(
		    this->table_name(),
		    statements
		);
	}
	std::tuple<int16_t,std::string> mob_roam::delete_by_mob_vnum(mob_vnum mob) {
		return mods::orm::util::delete_where<mob_roam,sql_compositor>(
		           table_name(),
		           "mob_virtual_number",
		           "=",
		           std::to_string(mob)
		       );
	}
	std::tuple<int16_t,std::string> mob_roam::delete_by_profile_name(std::string_view profile) {
		return mods::orm::util::delete_where<mob_roam,sql_compositor>(
		           table_name(),
		           "profile_name",
		           "=",
		           profile
		       );
	}
	int16_t mob_roam::save() {
		for(auto& record : this->rows) {
			std::tuple<int16_t,std::string,uint64_t> insert_result = mods::orm::util::insert_returning<mob_roam_record_t,sql_compositor>(&record, "id");
			if(!ORM_SUCCESS(insert_result)) {
				std::cerr << red_str("Issue saving mob_roam:'") << std::get<1>(insert_result) << "'\n";
			}
		}
		return 0;
	}
	void mob_roam::populate(uint32_t mob_vnum, const std::vector<room_vnum>& rooms) {
		for(const auto& room : rooms) {
			mob_roam_record_t record;
			record.mob_virtual_number = mob_vnum;
			record.room_virtual_number = room;
			rows.emplace_back(record);
		}
	}
	strmap_t mob_roam::export_class() {
		strmap_t values;
		return std::move(values);
	}

	void mob_roam::init() {
		id = 0;
		loaded = 0;
	}

	void mob_roam::feed_multi(pqxx::result& in_rows) {
		mob_roam_record_t r;
		for(auto row : in_rows) {
			r.mob_virtual_number = row["mob_virtual_number"].as<int>();
			r.room_virtual_number = row["room_virtual_number"].as<int>();
			r.profile_name = row["profile_name"].c_str();
			rows.emplace_back(r);
		}
	}
	std::tuple<int16_t,std::string> mob_roam::load_all() {
		mods::orm::util::load_all<mob_roam,sql_compositor>(this);
		return {0,""};
	}

	std::tuple<int16_t,std::string> load_all_roaming_data(std::map<mob_vnum,std::vector<room_vnum>>* storage_ptr) {
		int16_t count = 0;
		std::string message = "";
		static mob_roam data;
		data.load_all();
		for(auto& row : data.rows) {
			(*storage_ptr)[row.mob_virtual_number].emplace_back(row.room_virtual_number);
		}
		return {count,message};
	}
};
