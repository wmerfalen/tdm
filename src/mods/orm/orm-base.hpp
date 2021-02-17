#ifndef __MENTOC_MODS_ORM_BASE_REWRITE_HEADER__
#define __MENTOC_MODS_ORM_BASE_REWRITE_HEADER__

#include <iostream>
#include <vector>
#include "../../globals.hpp"
#include "../pq.hpp"
#include "../sql.hpp"
#include "util.hpp"

#define ORM_SUCCESS(a) std::get<0>(a) == 0
#define ORM_NO_RESULTS(a) std::get<0>(a) == mods::orm::util::NO_RESULTS
#define ORM_FAILURE(a) std::get<0>(a) < 0
namespace mods::orm {
	using strmap_t = std::map<std::string,std::string>;
	using sql_compositor = mods::sql::compositor<mods::pq::transaction>;
	template <typename TObject>
	static inline std::deque<std::shared_ptr<TObject>> load_all_by_table() {
		std::deque<std::shared_ptr<TObject>> list;
		for(auto&& row : db_get_all(TObject::table_name_value)) {
			auto s = std::make_shared<TObject>();
			s->feed(row);
			list.emplace_back(std::move(s));
		}
		return std::move(list);
	}
	template <typename TClassType,typename TPrimaryType>
	struct orm_base {
		std::string dump() {
			std::string dump;
			dump = CAT("[",primary_key_name().data(),"]:->",primary_key_value().data(),"\r\n");
			for(const auto& pair : export_class()) {
				dump += CAT("[",pair.first,"]:->",pair.second,"\r\n");
			}
			return dump;
		}
		std::string dump_fields(const std::vector<std::string_view>& field_list) {
			std::string dump;
			auto data = export_class();
			std::size_t ctr = 0;
			for(const auto& field : field_list) {
				if(field.compare(primary_key_name().data()) == 0) {
					dump += CAT("[",field.data(),"]:->",primary_key_value().data(),"\r\n");
				} else {
					dump += CAT("[",field.data(),"]:->",data[field.data()].data(),"\r\n");
				}
				if(++ctr == data.size()) {
					break;
				}
				dump += ",";
			}
			return dump;
		}
		std::string encode() {
			std::string dump;
			auto data = export_class();
			std::size_t ctr = 0;
			dump = "[";
			dump += CAT("{klen:",primary_key_name().length(),",key:\"",primary_key_name().data(),"\",vlen:",primary_key_value().data(),",value:\"",primary_key_value().data(),"\"},");
			for(const auto& pair : data) {
				dump += CAT("{klen:",pair.first.length(),",key:\"",pair.first,"\",vlen:",pair.second.length(),",value:\"",pair.second,"\"}");
				if(++ctr == data.size()) {
					break;
				}
				dump += ",";
			}
			dump += "]";
			return dump;
		}
		std::string encode_fields(const std::vector<std::string_view>& field_list) {
			std::string dump;
			auto data = export_class();
			std::size_t ctr = 0;
			dump = "[";
			for(const auto& field : field_list) {
				if(field.compare(primary_key_name().data()) == 0) {
					dump += CAT("{klen:",primary_key_name().length(),",key:\"",primary_key_name().data(),"\",vlen:",primary_key_value().data(),",value:\"",primary_key_value().data(),"\"}");
				} else {
					dump += CAT("{klen:",field.length(),",key:\"",field.data(),"\",vlen:",data[field.data()].length(),",value:\"",data[field.data()].data(),"\"}");
				}
				if(++ctr == data.size()) {
					break;
				}
				dump += ",";
			}
			dump += "]";
			return dump;
		}
		template <typename TClass>
		static inline std::tuple<int16_t,std::string,uint64_t> create(TClass* c) {
			auto status = mods::orm::util::insert_returning<TClass,sql_compositor>(c,c->primary_key_name());
			if(ORM_FAILURE(status)) {
				std::cerr << "[orm_base::insert] failed: '" << std::get<1>(status) << "'\n";
			}
			return status;
		}
		template <typename TClass>
		std::tuple<int16_t,std::string> read(TClass* c) {
			auto status = mods::orm::util::load_by_pkid<TClass,sql_compositor>(c);
			if(ORM_NO_RESULTS(status)) {
				std::cout << "[load_by_pkid] no results.\n";
			}
			if(ORM_FAILURE(status)) {
				std::cerr << "[load_by_pkid] failed: '" << std::get<1>(status) << "'\n";
			}
			return status;
		}
		template <typename TClass>
		std::tuple<int16_t,std::string> read_normalized(TClass* c,std::string column,std::string value) {
			auto status = mods::orm::util::load_multi_by_column<TClass,sql_compositor>(c,column,value);
			if(ORM_NO_RESULTS(status)) {
				std::cout << "[read_normalized] no results.\n";
			}
			if(ORM_FAILURE(status)) {
				std::cerr << "[read_normalized] failed: '" << std::get<1>(status) << "'\n";
			}
			return status;
		}
		template <typename TClass>
		std::tuple<int16_t,std::string> read_all(TClass* c) {
			auto status = mods::orm::util::load_all<TClass,sql_compositor>(c);
			if(ORM_NO_RESULTS(status)) {
				std::cout << "[mods::orm::orm_base::read] no results.\n";
			}
			if(ORM_FAILURE(status)) {
				std::cerr << "[mods::orm::orm_base::read] failed: '" << std::get<1>(status) << "'\n";
			}
			return status;
		}
		template <typename TClass>
		std::tuple<int16_t,std::string> read(TClass* c,std::string_view column,std::string_view value) {
			auto status = mods::orm::util::load_by_column<TClass,sql_compositor>(c,column,value);
			if(ORM_NO_RESULTS(status)) {
				std::cout << "[mods::orm::orm_base::read] no results.\n";
			}
			if(ORM_FAILURE(status)) {
				std::cerr << "[mods::orm::orm_base::read] failed: '" << std::get<1>(status) << "'\n";
			}
			return status;
		}
		template <typename TClass>
		static inline std::tuple<int16_t,std::string> update(TClass* c) {
			auto status = mods::orm::util::update<TClass,sql_compositor>(c);
			if(ORM_FAILURE(status)) {
				std::cerr << "[orm_base::update] failed: '" << std::get<1>(status) << "'\n";
			}
			return status;
		}
		std::tuple<int16_t,std::string> remove() {
			auto status = mods::orm::util::delete_from<orm_base,sql_compositor>(this);
			if(ORM_FAILURE(status)) {
				std::cerr << "[orm_base::remove] failed: '" << std::get<1>(status) << "'\n";
			}
			return status;
		}

		virtual std::string table_name() {
			return "player";
		}
		virtual std::string column_prefix() {
			return "player_";
		}
		virtual std::string id_column() {
			return "id";
		}
		using primary_choice_t = TPrimaryType;

		virtual std::string primary_key_name() {
			return id_column();
		}
		virtual std::string primary_key_value() {
			return std::to_string(this->id);
		}
		virtual TPrimaryType primary_type() {
			return (TPrimaryType)0;
		}
		virtual int16_t feed(const pqxx::result::reference&) {
			return 0;
		}
		virtual strmap_t export_class() {
			strmap_t f;
			f["id"] = std::to_string(id);
			return f;
		}

		bool loaded;
		uint64_t id;
		long created_at;
		long updated_at;

	};
};

#endif
