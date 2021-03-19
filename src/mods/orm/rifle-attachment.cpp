#include "rifle-attachment.hpp"
#include "../rifle-attachments.hpp"
#include "util.hpp"
#include "../player.hpp"

namespace mods::orm {
	uint64_t rifle_attachment::initialize_row(const uint64_t& player_id,const std::vector<std::string>& list) {
		rows.clear();
		init();
		for(auto& desc : list) {
			id = 0;
			loaded = 0;
			rifle_data = desc;
			rifle_player_id = player_id;
			auto status = this->create<rifle_attachment>(this);
			if(ORM_SUCCESS(status)) {
				updated_at = created_at = time(nullptr);
				loaded = 1;
				id = std::get<2>(status);
				rows.emplace_back();
				auto& r = rows.back();
				r.id = id;
				r.rifle_data = desc;
				r.rifle_player_id = player_id;
			}
		}
		return id;
	}
	int16_t rifle_attachment::feed(const pqxx::result::reference& row) {
		init();
		id = row["id"].as<uint64_t>();
		rifle_data = row["rifle_data"].c_str();
		rifle_player_id  = row["rifle_player_id"].as<uint64_t>();
		loaded = 1;
		return 0;
	}
	std::tuple<int16_t,std::string> rifle_attachment::delete_by_player_id(const uint64_t& player_id) {
		return mods::orm::util::delete_where<rifle_attachment,sql_compositor>(
		           table_name(),
		           "rifle_player_id",
		           "=",
		           std::to_string(player_id)
		       );
	}

	int16_t rifle_attachment::save() {
		for(auto& record : rows) {
			id = record.id;
			rifle_data = record.rifle_data;
			rifle_player_id = record.rifle_player_id;
			std::tuple<int16_t,std::string,uint64_t> insert_result = mods::orm::util::insert_returning<rifle_attachment_record_t,sql_compositor>(&record, "id");
			if(!ORM_SUCCESS(insert_result)) {
				std::cerr << red_str("Issue saving rifle_attachment:'") << std::get<1>(insert_result) << "'\n";
			}
		}
		return 0;
	}
	strmap_t rifle_attachment::export_class() {
		strmap_t v;
		v["rifle_data"] = (rifle_data);
		v["rifle_player_id"] = std::to_string(rifle_player_id);
		return std::move(v);
	}


	void rifle_attachment::init() {
		id = 0;
		loaded = 0;
		rifle_data.clear();
		rifle_player_id = 0;
		created_at = 0;
		updated_at = 0;
	}

	std::tuple<int16_t,std::string> load_player_rifle_attachments(player_ptr_t player) {
		if(!player) {
			std::cerr << red_str("invalid player ptr passed in to load_player_rifle_attachments") << "\n";
			return {0,"invalid ptr"};
		}
		player->sendln("checking rifle attachment table...");
		auto result = db_get_by_meta(rifle_attachment_table_name.data(),"rifle_player_id",std::to_string(player->db_id()));
		player->sendln(CAT("found ",result.size()," results"));
		if(result.size() == 0) {
			player->sendln("no results! is that normal for you?");
			return {0,"no results"};
		}

		for(auto&& row : result) {
			std::cerr << green_str("rifle_attachment filling rifle:") << row["rifle_data"].c_str() << "\n";
			player->sendln("Giving you a rifle...");
			player->rifle_attachments().emplace_back(std::make_shared<mods::rifle_attachments_t>(row["rifle_data"].c_str()));
			player->carry(player->rifle_attachments().back()->base_object);
		}
		return {result.size(),"loaded"};
	}
	std::tuple<int16_t,std::string> flush_player_rifle_attachments(player_ptr_t player) {
		if(!player) {
			std::cerr << red_str("invalid player ptr passed in to flush_player_rifle_attachments") << "\n";
			return {0,"invalid ptr"};
		}
		//auto result = db_get_by_meta(rifle_attachment_table_name.data(),"rifle_player_id",std::to_string(player->db_id()));
		//if(player->rifle_attachments().size() == result.size()) {
		//	player->sendln("looks like we already have your rifle attachments saved.");
		//	return {0,"already saved"};
		//}
		rifle_attachment r;
		r.delete_by_player_id(player->db_id());
		std::vector<std::string> encodings;
		for(auto& row : player->rifle_attachments()) {
			std::cerr << green_str("rifle_attachment saving rifle:") << row->export_objects() << "\n";
			encodings.emplace_back(row->export_objects());
		}
		return {r.initialize_row(player->db_id(),encodings),"saved"};
	}
};
