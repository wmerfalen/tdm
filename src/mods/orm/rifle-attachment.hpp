#ifndef __MENTOC_MODS_ORM_rifle_attachment_HEADER__
#define __MENTOC_MODS_ORM_rifle_attachment_HEADER__

#include "../../globals.hpp"
#include "orm-base.hpp"
#include "../rifle-attachments.hpp"

namespace mods::orm {
	using strmap_t = std::map<std::string,std::string>;
	static constexpr std::string_view rifle_attachment_table_name = "rifle_attachment";
	struct rifle_attachment_record_t {
		uint64_t id;
		std::string rifle_data;
		uint64_t rifle_player_id;
		strmap_t export_class() {
			strmap_t v;
			v["rifle_data"] = (rifle_data);
			v["rifle_player_id"] = std::to_string(rifle_player_id);
			v["id"] = std::to_string(id);
			return std::move(v);
		}
		std::string table_name() const {
			return rifle_attachment_table_name.data();
		}
		std::string primary_key_name() {
			return "id";
		}
	};

	struct rifle_attachment : public mods::orm::orm_base<rifle_attachment,bool> {
		std::string table_name() {
			return rifle_attachment_table_name.data();
		}
		std::string column_prefix() {
			return "rifle_";
		}
		std::string id_column() {
			return "id";
		}
		std::string primary_key_name() {
			return this->id_column();
		}
		std::string primary_key_value() {
			return std::to_string(this->id);
		}

		rifle_attachment() {
			this->init();
			loaded = 0;
		}
		rifle_attachment(obj_ptr_t& obj);
		~rifle_attachment() = default;

		void import_object(obj_ptr_t& obj);
		uint64_t initialize_row(const uint64_t& player_id,const std::vector<std::string>& list);
		int16_t feed_multi(const pqxx::result::reference&);
		int16_t feed(const pqxx::result::reference&);
		std::tuple<int16_t,std::string> delete_by_player_id(const uint64_t& player_id);
		void init();
		strmap_t export_class();

		void populate_object(obj_ptr_t& obj);
		int16_t save();
		std::string& get_dump();

		int16_t				load_by_id(uint64_t);
		uint64_t id;
		std::string rifle_data;
		uint64_t rifle_player_id;
		long created_at;
		long updated_at;
		bool loaded;
		std::deque<rifle_attachment_record_t> rows;
	};
	std::tuple<int16_t,std::string> load_player_rifle_attachments(player_ptr_t player);
	std::tuple<int16_t,std::string> flush_player_rifle_attachments(player_ptr_t player);

};

#endif
