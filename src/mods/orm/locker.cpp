#include "locker.hpp"
#include "util.hpp"
#include "../player.hpp"
#include "../util-map.hpp"


#define __MENTOC_SHOW_MODS_ORM_LOCKER_DEBUG_OUTPUT__
#ifdef __MENTOC_SHOW_MODS_ORM_LOCKER_DEBUG_OUTPUT__
	#define m_debug(MSG) mentoc_prefix_debug("[mods::orm::locker::debug]")  << MSG << "\n";
	#define m_error(MSG) mentoc_prefix_debug(red_str("[mods::orm::locker::ERROR]"))  << MSG << "\n";
#else
	#define m_debug(MSG)
	#define m_error(MSG)
	#define lc_debug(MSG)
#endif

namespace mods::orm {
	bool locker::room_has_locker_by_type(std::string_view type,const room_vnum& room) {
		static constexpr std::string_view row_alias = "cnt";
		using statement = std::vector<mods::orm::util::statement_t>;
		using c = std::vector<pqxx::result::reference>;
		c container;
		statement statements;
		auto t = to_type(type);
		statements.emplace_back("l_type","=",t.data(),true,false);
		statements.emplace_back("l_room_vnum","=",std::to_string(room),false,false);
		mods::orm::util::load_count_where<c,sql_compositor,statement>(
		    container,
		    locker::TABLE_NAME.data(),
		    row_alias,
		    statements
		);
		for(const auto& c : container) {
			if(c[row_alias.data()].is_null()) {
				continue;
			}
			if(c[row_alias.data()].as<std::size_t>()) {
				return true;
			}
		}
		return false;
	}

	locker::status_t locker::remove_item_by_id(const uint64_t& id) {
		auto s = mods::orm::util::delete_where<locker,sql_compositor>(
		        locker::TABLE_NAME,
		        "id",
		        "=",
		        std::to_string(id).c_str()
		    );
		if(ORM_FAILURE(s)) {
			return {false,std::get<1>(s)};
		}
		return {true,""};
	}
	std::size_t locker::get_lockers_by_type(std::string_view type,const room_vnum& room,std::vector<locker>* storage) {
		using statement = std::vector<mods::orm::util::statement_t>;
		using c = std::vector<pqxx::result::reference>;
		c container;
		statement statements;
		auto t = to_type(type);
		statements.emplace_back("l_type","=",t.data(),true,false);
		statements.emplace_back("l_room_vnum","=",std::to_string(room),false,false);
		mods::orm::util::load_where<c,sql_compositor,statement>(
		    container,
		    locker::TABLE_NAME.data(),
		    statements
		);
		if(container.size() == 0) {
			return 0;
		}
		for(const auto& row : container) {
			storage->emplace_back(row);
		}
		return storage->size();
	}
	std::vector<std::string> locker::list_locker_by_type(std::string_view type,const room_vnum& room) {
		std::vector<std::string> list;
		std::vector<locker> storage;
		get_lockers_by_type(type,room,&storage);
		for(const auto& row : storage) {
			list.emplace_back(CAT("[id]:",row.id,"\r\n",
			        "[l_type]:'",row.l_type,"'\r\n",
			        "[l_room_vnum]:",row.l_room_vnum,"\r\n",
			        "[l_yaml]:'",row.l_yaml,"\r\n",
			        "[l_count]:'",row.l_count,"\r\n"
			    ));
		}
		return list;
	}
	locker::status_t locker::place_locker_item(std::string_view type,const room_vnum& room, std::string_view yaml,uint16_t count) {
		locker l;
		l.l_type = to_type(type);
		l.l_room_vnum = room;
		l.l_count = count;
		l.l_yaml = yaml;
		if(l.l_yaml.length() > 2048) {
			l.l_yaml = l.l_yaml.substr(0,2047);
		}
		auto res = l.create(&l);
		if(ORM_FAILURE(res)) {
			return {false,std::get<1>(res)};
		}
		return {true,std::get<1>(res)};
	}
	locker::status_t locker::place_locker(std::string_view type,const room_vnum& room, const std::vector<std::string>& args) {
		uint16_t count = locker::default_weapon_count;
		if(type.compare("weapon") == 0) {
			count = locker::default_weapon_count;
		}
		if(type.compare("ammo") == 0) {
			count = locker::default_ammo_count;
		}
		if(type.compare("armor") == 0) {
			count = locker::default_armor_count;
		}
		locker l;
		l.l_type = to_type(type);
		auto statuses = l.insert_many(room,type,args,count);
		std::string errors;
		for(const auto& s : statuses) {
			if(ORM_FAILURE(s)) {
				errors += std::get<1>(s);
				errors += "\r\n";
			}
		}
		return {errors.length(),errors};
	}
	locker::status_t locker::set_quota(std::string_view type,const room_vnum& room,uint16_t count) {
		return {true,"stub"};
	}
	locker::status_t locker::remove_locker(std::string_view type,const room_vnum& room) {
		std::vector<locker> storage;
		std::size_t ok = 0, error = 0;
		if(get_lockers_by_type(type,room,&storage)) {
			for(auto& row : storage) {
				auto s = remove_item_by_id(row.id);
				if(std::get<0>(s)) {
					++ok;
				} else {
					++error;
				}
			}
			if(error == 0) {
				return {true,CAT(ok," items deleted with zero errors")};
			}
			return {false,CAT(ok," items deleted with ",error," errors")};
		}
		return {false,"nothing to delete"};
	}
	std::vector<std::tuple<int16_t,std::string>> locker::insert_many(const room_vnum& room,std::string_view type,const std::vector<std::string>& items,const uint16_t& count) {
		this->l_type = to_type(type);
		this->l_count = count;
		this->l_room_vnum = room;
		std::vector<std::tuple<int16_t,std::string>> statuses;
		for(const auto& yaml : items) {
			this->l_yaml = yaml;
			if(this->l_yaml.length() > 2048) {
				this->l_yaml = this->l_yaml.substr(0,2047);
			}
			auto res = this->create(this);
			statuses.emplace_back(std::get<0>(res),std::get<1>(res));
		}
		return statuses;
	}

	std::vector<std::tuple<int16_t,std::string>> locker::insert_many(const room_vnum& room,std::string_view type,const std::vector<std::pair<uint16_t,std::string>>& items) {
		this->l_type = to_type(type);
		this->l_room_vnum = room;
		std::vector<std::tuple<int16_t,std::string>> statuses;
		for(const auto& pair: items) {
			this->l_count = pair.first;
			this->l_yaml = pair.second;
			if(this->l_yaml.length() > 2048) {
				this->l_yaml = this->l_yaml.substr(0,2047);
			}
			auto res = this->create(this);
			statuses.emplace_back(std::get<0>(res),std::get<1>(res));
		}
		return statuses;
	}



	locker_list_t& locker_list() {
		static locker_list_t list;
		return list;
	}

	locker_list_t load_all_locker_list() {
		locker_list_t list;
		locker r;
		r.read_all(&r);
		for(const auto& r : r.rows) {
			list.emplace_back(std::make_shared<locker>(r));
		}
		return list;
	}
};

#undef m_debug
#undef m_error
#undef lc_debug
