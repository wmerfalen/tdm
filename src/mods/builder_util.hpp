#ifndef __MENTOC_MODS_BUILDER_UTIL_HEADER__
#define __MENTOC_MODS_BUILDER_UTIL_HEADER__
#include "builder.hpp"
#include "quests.hpp"
#include "pq.hpp"
#include "util.hpp"
#include "object.hpp"
#include <stdlib.h> //For itoa
#include "sql.hpp"
#include "../structs.h"
#include "extern.hpp"
#include <functional>
#include "player.hpp"

//using objtype = mods::object::type;
using args_t = std::vector<std::string>;
typedef mods::sql::compositor<mods::pq::transaction> sql_compositor;
namespace mods::builder_util {
	typedef std::function<void(sql_compositor::value_map&)> value_callback;
	typedef std::function<std::pair<bool,std::string>()> post_modify_callback;
	template <typename PKValueType>
	std::pair<bool,std::string> save_to_db(
	    const std::string& table_name,
	    const std::string& primary_key,
	    const PKValueType& pk_value,
	    value_callback fill_value_function,
	    post_modify_callback post_modify_values_function
	) {
		try {
			auto check_txn = txn();
			auto check_sql = sql_compositor(table_name,&check_txn)
			                 .select("COUNT(*)")
			                 .from(table_name)
			                 .where(primary_key,"=",pk_value)
			                 .sql();
			auto check_result = mods::pq::exec(check_txn,check_sql);
			mods::pq::commit(check_txn);
			sql_compositor::value_map values;
			fill_value_function(values);

			if(check_result.size()) {
				/* update the record */
				auto t = txn();
				auto sql = sql_compositor(table_name,&t)
				           .update(table_name)
				           .set(values)
				           .where(primary_key,"=",pk_value)
				           .sql();
				mods::pq::exec(t,sql);
				mods::pq::commit(t);
			} else {
				/* insert the record */
				auto t = txn();
				auto sql = sql_compositor(table_name,&t)
				           .insert()
				           .into(table_name)
				           .values(values)
				           .sql();
				mods::pq::exec(t,sql);
				mods::pq::commit(t);
			}

			return post_modify_values_function();
		} catch(const std::exception& e) {
			return {false,e.what()};
		}

		return {true,"success"};
	}

	template <typename FlagType>
	void flag_help(
	    mods::player * player,
	    const std::vector<std::string>& vec_args,
	    std::string_view key_name,
	    const FlagType& builder_flags
	) {
		if(vec_args.size() == 2 && vec_args[0].compare("help") == 0
		        && vec_args[1].compare(key_name.data()) == 0) {
			player->pager_start();
			*player << "possible flags:\r\n";

			for(auto& flag : builder_flags) {
				*player << flag.second << "\r\n";
			}

			*player << "\r\n";
			player->pager_end();
			player->page(0);
			return;
		}
	}
	template <typename VectorObjectType,typename NameType>
	void list_object_vector(
	    player_ptr_t& player,
	    const std::string& argument,
	    VectorObjectType& vector_objects,
	    std::function<NameType(typename VectorObjectType::value_type&)> data_extractor
	) {
		auto args = mods::util::subcmd_args<5,args_t>(argument,"list");

		if(args.has_value()) {
			mods::builder::report_status<player_ptr_t>(player,"listing...");
			unsigned vo_id = 0;
			player->pager_start();

			for(auto& vo_reference : vector_objects) {
				*player << "{gld}[" << vo_id++ << "]{/gld} :->{red} [" << data_extractor(vo_reference) << "]{/red}\r\n";
			}

			player->pager_end();
			player->page(0);
			return;
		}
	}

	template <typename VectorObjectType>
	void save_object_vector(
	    player_ptr_t& player,
	    const std::string& argument,
	    VectorObjectType& vector_objects,
	    std::function<std::pair<bool,std::string>(typename VectorObjectType::value_type&)> save_function
	) {
		auto args = mods::util::subcmd_args<5,args_t>(argument,"save");

		if(args.has_value()) {
			auto arg_vec = args.value();

			if(arg_vec.size() < 2) {
				mods::builder::report_error<player_ptr_t>(player,"Invalid number of arguments");
				return;
			}

			auto index = mods::util::stoi(arg_vec[1]);
			std::size_t i = index.value();

			if(!index.has_value() || i >= vector_objects.size()) {
				mods::builder::report_error<player_ptr_t>(player,"Invalid index");
				return;
			}

			auto return_pair = save_function(vector_objects[index.value()]);

			if(!return_pair.first) {
				mods::builder::report_error<player_ptr_t>(player,return_pair.second);
				return;
			}

			mods::builder::report_success<player_ptr_t>(player,"Object saved.");
			return;
		}
	}

	template <typename VectorObjectType>
	void show_nonconst_vector(
	    player_ptr_t& player,
	    const std::string& argument,
	    VectorObjectType vector_objects,
	    std::function<void(typename VectorObjectType::value_type&)> show_function
	) {
		auto args = mods::util::subcmd_args<5,args_t>(argument,"show");

		if(args.has_value()) {
			auto arg_vec = args.value();

			if(arg_vec.size() < 2) {
				mods::builder::report_error<player_ptr_t>(player,"Invalid number of arguments");
				return;
			}

			auto index = mods::util::stoi(arg_vec[1]);

			if(!index.has_value() || index.value() < 0 ||
			        static_cast<std::size_t>(index.value()) >= vector_objects.size()) {
				mods::builder::report_error<player_ptr_t>(player,"Invalid index");
				return;
			}

			show_function(vector_objects[index.value()]);
		}

		return;
	}


	template <typename VectorObjectType>
	void show_object_vector(
	    player_ptr_t& player,
	    const std::string& argument,
	    VectorObjectType& vector_objects,
	    std::function<void(typename VectorObjectType::value_type&)> show_function
	) {
		auto args = mods::util::subcmd_args<5,args_t>(argument,"show");

		if(args.has_value()) {
			auto arg_vec = args.value();

			if(arg_vec.size() < 2) {
				mods::builder::report_error<player_ptr_t>(player,"Invalid number of arguments");
				return;
			}

			auto index = mods::util::stoi(arg_vec[1]);

			if(!index.has_value() || index.value() < 0 ||
			        static_cast<std::size_t>(index.value()) >= vector_objects.size()) {
				mods::builder::report_error<player_ptr_t>(player,"Invalid index");
				return;
			}

			show_function(vector_objects[index.value()]);
		}

		return;
	}

	template <typename VectorObjectType>
	void set_attribute_on_object_vector(
	    player_ptr_t& player,
	    const std::string& argument,
	    VectorObjectType& vector_objects,
	    std::function<void(typename VectorObjectType::value_type&,const std::string&,const std::string&)> set_function
	) {
		auto args = mods::util::subcmd_args<5,args_t>(argument,"attr");

		if(args.has_value()) {
			auto arg_vec = args.value();

			if(arg_vec.size() < 4) {
				mods::builder::report_error<player_ptr_t>(player,"Invalid number of arguments");
				return;
			}

			auto index = mods::util::stoi(arg_vec[1]);

			if(index.has_value() && index.value() < vector_objects.size()) {
				set_function(vector_objects[index.value()],arg_vec[2],arg_vec[3]);
			}
		}

		return;
	}


	template <typename VectorObjectType>
	void instantiate_object_vector(
	    player_ptr_t& player,
	    const std::string& argument,
	    VectorObjectType& vector_objects,
	    std::function<void(int)> instantiate_function
	) {
		auto args = mods::util::subcmd_args<12,args_t>(argument,"instantiate");

		if(args.has_value()) {
			auto arg_vec = args.value();
			auto i_value = mods::util::stoi(arg_vec[1]);

			if(!i_value.has_value()) {
				mods::builder::report_error<player_ptr_t>(player,"Please use a valid numeric value.");
			} else {
				auto index = i_value.value();

				if(index >= vector_objects.size()) {
					mods::builder::report_error<player_ptr_t>(player,"Out of bounds");
				}

				instantiate_function(index);
				return;
			}
		}

		return;
	}

	template <typename TPlayer,typename ObjectType>
	void list_line(TPlayer& player, const std::string& key, ObjectType& value) {
		*player << "{gld}[" << key << "]{/gld} :->{red} [" << value << "]{/red}\r\n";
	}
};
extern void r_error(const player_ptr_t& player,std::string_view msg);
extern void r_success(const player_ptr_t& player,std::string_view msg);
extern void r_status(const player_ptr_t& player,std::string_view msg);
#endif
