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
	template <typename TClassType,typename TPrimaryType>
	struct orm_base {
		template <typename TClass>
		static inline std::tuple<int16_t,std::string,uint64_t> create(TClass* c){
			auto status = mods::orm::util::insert_returning<TClass,sql_compositor>(c,c->primary_key_name());
			if(ORM_FAILURE(status)){
				std::cerr << "[orm_base::insert] failed: '" << std::get<1>(status) << "'\n";
			}
			return status;
		}
		template <typename TClass>
		std::tuple<int16_t,std::string> read(TClass* c){
			auto status = mods::orm::util::load_by_pkid<TClass,sql_compositor>(c);
			if(ORM_NO_RESULTS(status)){
				std::cout << "[load_by_pkid] no results.\n";
			}
			if(ORM_FAILURE(status)){
				std::cerr << "[load_by_pkid] failed: '" << std::get<1>(status) << "'\n";
			}
			return status;
		}
		template <typename TClass>
		std::tuple<int16_t,std::string> read_normalized(TClass* c,std::string column,std::string value){
			auto status = mods::orm::util::load_multi_by_column<TClass,sql_compositor>(c,column,value);
			if(ORM_NO_RESULTS(status)){
				std::cout << "[read_normalized] no results.\n";
			}
			if(ORM_FAILURE(status)){
				std::cerr << "[read_normalized] failed: '" << std::get<1>(status) << "'\n";
			}
			return status;
		}
		template <typename TClass>
		std::tuple<int16_t,std::string> read(TClass* c,std::string_view column,std::string_view value){
			auto status = mods::orm::util::load_by_column<TClass,sql_compositor>(c,column,value);
			if(ORM_NO_RESULTS(status)){
				std::cout << "[mods::orm::orm_base::read] no results.\n";
			}
			if(ORM_FAILURE(status)){
				std::cerr << "[mods::orm::orm_base::read] failed: '" << std::get<1>(status) << "'\n";
			}
			return status;
		}
		template <typename TClass>
		static inline std::tuple<int16_t,std::string> update(TClass* c){
			auto status = mods::orm::util::update<TClass,sql_compositor>(c);
			if(ORM_FAILURE(status)){
				std::cerr << "[orm_base::update] failed: '" << std::get<1>(status) << "'\n";
			}
			return status;
		}
		std::tuple<int16_t,std::string> remove(){
			auto status = mods::orm::util::delete_from<orm_base,sql_compositor>(this);
			if(ORM_FAILURE(status)){
				std::cerr << "[orm_base::remove] failed: '" << std::get<1>(status) << "'\n";
			}
			return status;
		}

		virtual std::string table_name() { return "player"; }
		virtual std::string column_prefix(){ return "player_"; }
		virtual std::string id_column(){ return "id"; }
		using primary_choice_t = TPrimaryType;

		virtual std::string primary_key_name() { return id_column(); }
		virtual std::string primary_key_value(){ return std::to_string(this->id); }
		virtual TPrimaryType primary_type(){ return (TPrimaryType)0; }
		virtual int16_t feed(const pqxx::result::reference &){
			return 0;
		}
		virtual strmap_t export_class(){
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
