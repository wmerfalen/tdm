#ifndef __MENTOC_MODS_ORM_UTIL_HEADER__
#define __MENTOC_MODS_ORM_UTIL_HEADER__

namespace mods::orm::util {
		template <typename TObject,typename sql_compositor>
			static inline std::tuple<int16_t,std::string,uint64_t> insert_returning(TObject* obj, std::string_view returning_field){
				try{
					auto insert_transaction = txn();
					auto mapped = obj->export_class();
					mapped.erase(obj->primary_key_name().data());
					sql_compositor comp(obj->table_name(),&insert_transaction);
					auto up_sql = comp
						.insert()
						.into(obj->table_name())
						.values(mapped)
						.returning(returning_field)
						.sql();
					auto record = mods::pq::exec(insert_transaction,up_sql);
					mods::pq::commit(insert_transaction);
					if(record.size()){
						auto i_value = mods::util::stoi<uint64_t>(record[0][returning_field.data()].c_str());
						return {0,"okay",i_value};
					}
				}catch(std::exception& e){
					return {-1,e.what(),0};
				}
				return {-2,"unknown",0};
			}

		template <typename TObject,typename sql_compositor>
			static inline std::tuple<int16_t,std::string> delete_from(TObject* obj){
				try{
					auto del_txn = txn();
					sql_compositor comp(obj->table_name(),&del_txn);
					auto up_sql = comp
						.del()
						.from(obj->table_name())
						.where(obj->primary_key_name(),"=",obj->primary_key_value())
						.sql();
					auto record = mods::pq::exec(del_txn,up_sql);
					mods::pq::commit(del_txn);
					return {0,"okay"};
				}catch(std::exception& e){
					return {-1,e.what()};
				}
				return {-2,"unknown"};
			}
		template <typename TObject,typename sql_compositor>
			static inline std::tuple<int16_t,std::string> delete_where(
					std::string_view table_name,
					std::string_view where, 
					std::string_view sql_operator,
					std::string_view value) {
				try{
					auto del_txn = txn();
					sql_compositor comp(table_name.data(),&del_txn);
					auto up_sql = comp
						.del()
						.from(table_name.data())
						.where(where.data(), sql_operator.data(),value.data())
						.sql();
					mods::pq::exec(del_txn,up_sql);
					mods::pq::commit(del_txn);
					return {0,"okay"};
				}catch(std::exception& e){
					return {-1,e.what()};
				}
				return {-2,"unknown"};
			}
		template <typename TObject,typename sql_compositor>
			static inline std::tuple<int16_t,std::string> update(TObject* s){
				try{
					auto up_txn = txn();
					sql_compositor comp(s->table_name(),&up_txn);
					auto up_sql = comp
						.update(s->table_name())
						.set(s->export_class())
						.where(s->primary_key_name(),"=",s->primary_key_value())
						.sql();
					mods::pq::exec(up_txn,up_sql);
					mods::pq::commit(up_txn);
					return {0,"okay"};
				}catch(std::exception& e){
					return {-1,e.what()};
				}
				return {-2,"unknown"};
			}
		template <typename TContainerObject,typename sql_compositor>
			static inline std::tuple<int16_t,std::string> load_where(TContainerObject& s,
					std::string_view where,
					std::string_view sql_operator,
					std::string_view value){
				try{
					auto select_transaction = txn();
					sql_compositor comp(s[0].table_name(),&select_transaction);
					auto player_sql = comp.select("*")
						.from(s[0].table_name())
						.where(where.data(), sql_operator.data(), value.data())
						.sql();
					auto player_record = mods::pq::exec(select_transaction,player_sql);
					mods::pq::commit(select_transaction);
					for(auto row : player_record){
						s.emplace_back(row);
					}
					return {0,"okay"};
				}catch(std::exception& e){
					return {-1,e.what()};
				}
				return {-2,"unknown"};
			}
		template <typename TObject,typename sql_compositor>
			static inline std::tuple<int16_t,std::string> load_by_column(TObject* s,std::string_view column,std::string_view value){
				try{
					auto select_transaction = txn();
					sql_compositor comp(s->table_name(),&select_transaction);
					auto player_sql = comp.select("*")
						.from(s->table_name())
						.where(column.data(),"=",value.data())
						.sql();
					auto player_record = mods::pq::exec(select_transaction,player_sql);
					if(player_record.size()){
						s->feed(player_record[0]);
						return {0,"okay"};
					}
				}catch(std::exception& e){
					return {-1,e.what()};
				}
				return {-2,"unknown"};
			}
		template <typename TObject,typename sql_compositor>
			static inline std::tuple<int16_t,std::string> load_by_pkid(TObject* s){
				try{
					auto select_transaction = txn();
					sql_compositor comp(s->table_name(),&select_transaction);
					auto player_sql = comp.select("*")
						.from(s->table_name())
						.where(s->primary_key_name(),"=",s->primary_key_value())
						.sql();
					auto player_record = mods::pq::exec(select_transaction,player_sql);
					if(player_record.size()){
						s->feed(player_record[0]);
						return {0,"okay"};
					}
				}catch(std::exception& e){
					return {-1,e.what()};
				}
				return {-2,"unknown"};
			}

}; //end namespace
#endif
