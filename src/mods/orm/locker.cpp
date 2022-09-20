//#define __MENTOC_SHOW_UTIL_MAP_DUMP_OUTPUT__
#include "locker.hpp"
#include "util.hpp"
#include "../player.hpp"
#include "../util-map.hpp"

#define lc_debug(MSG) mentoc_prefix_debug(red_str("[mods::orm::locker_cache::debug]"))  << MSG << "\n";

#ifdef __MENTOC_SHOW_MODS_ORM_NOTCH_DEBUG_OUTPUT__
	#define m_debug(MSG) mentoc_prefix_debug("[mods::orm::locker::debug]")  << MSG << "\n";
	#define m_error(MSG) mentoc_prefix_debug(red_str("[mods::orm::locker::ERROR]"))  << MSG << "\n";

#else
	#define m_debug(MSG)
	#define m_error(MSG)
#endif

namespace mods::orm {
	/**
	 * We will need some serious caching.
	 */
	namespace locker_cache {
		struct data {
			uint64_t id;
			uint16_t count;
			std::string yaml;
		};
		struct slim_locker {
			slim_locker() = delete;
			slim_locker(
			    const room_vnum& r,
			    locker::type_t t,
			    const std::vector<data>& d)  : room(r),type(t),rows(d) {
				ids.resize(d.size());
				contents.resize(d.size());
				lc_debug("slim_locker constructor");
				std::size_t i = 0;
				for(const auto& entry : d) {
					contents[i] = std::make_pair<>(entry.count,entry.yaml);
					ids[i] = entry.id;
					++i;
				}
			}

			room_vnum room;
			locker::type_t type;
			std::vector<uint64_t> ids;
			std::vector<data> rows;
			std::vector<std::pair<uint16_t,std::string>> contents;
		};
		static std::vector<uint64_t> removal_queue;
		static std::vector<std::pair<room_vnum,locker::type_t>> old;
		static std::vector<slim_locker> index;

		bool is_old(const room_vnum& room,locker::type_t type) {
			for(const auto& pair : old) {
				if(pair.first == room && pair.second == type) {
					return true;
				}
			}
			return false;
		}
		slim_locker& renew(const room_vnum& room,locker::type_t type,const std::vector<data>& list) {
			lc_debug("renew on " << room << " of type: " << type);
			old.erase(std::remove_if(old.begin(),old.end(),[&](const auto& pair) {
				return pair.first == room && pair.second == type;
			}),old.end());
			index.erase(std::remove_if(index.begin(),index.end(),[&](const auto& sl_locker) {
				return sl_locker.room == room && sl_locker.type == type;
			}),index.end());
			index.emplace_back(room,type,list);
			return index.back();
		}
		slim_locker& get_slim_locker(const room_vnum& room,locker::type_t type) {
			lc_debug("get_slim_locker on " << room << " of type: " << type);
			using statement = std::vector<mods::orm::util::statement_t>;
			using c = std::vector<pqxx::result::reference>;
			c container;
			statement statements;
			statements.emplace_back("l_type","=",locker::TYPES[type].data(),true,false);
			statements.emplace_back("l_room_vnum","=",std::to_string(room),false,false);
			mods::orm::util::load_where<c,sql_compositor,statement>(
			    container,
			    locker::TABLE_NAME.data(),
			    statements
			);
			std::vector<data> list;
			for(const auto& row : container) {
				data d;
				d.id = row["id"].as<uint64_t>();
				d.count = row["l_count"].as<uint16_t>();
				d.yaml = row["l_yaml"].c_str();
				list.emplace_back(d);
			}
			return renew(room,type,list);
		}
		void queue_removal_by_id(const uint64_t& id) {
			removal_queue.emplace_back(id);
		}
		void set_dirty(const room_vnum& room,locker::type_t type) {
			old.emplace_back(std::make_pair<>(room,type));
		}
	};
	bool locker::room_has_locker_by_type(std::string_view type,const room_vnum& room) {
		lc_debug("room_has_locker_by_type entry");
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
		lc_debug("room_has_locker_by_type");
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
	void locker::perform_cleanup() {
		lc_debug("perform_cleanup");
		for(auto& sl_locker : locker_cache::index) {
			if(std::find_first_of(
			        sl_locker.ids.cbegin(),
			        sl_locker.ids.cend(),
			        locker_cache::removal_queue.cbegin(),
			        locker_cache::removal_queue.cend()) != sl_locker.ids.cend()) {
				lc_debug("slim_locker perform_cleanup found some old entries");
				locker_cache::old.emplace_back(std::make_pair<>(sl_locker.room,sl_locker.type));
			}
		}
		locker_cache::removal_queue.clear();
		lc_debug("removal_queue cleared");
	}

	const std::vector<std::pair<uint16_t,std::string>>& locker::contents(std::string_view type,const room_vnum& room) {
		lc_debug("locker::contents entry");
		auto i_type = to_type_integral(type);
		if(locker_cache::is_old(room,i_type)) {
			lc_debug(room << "/" << type << " are old. Renewing cached entry");
			auto& locker = locker_cache::get_slim_locker(room,i_type);
			return locker.contents;
		}
		for(const auto& entry : locker_cache::index) {
			if(entry.room == room && entry.type == i_type) {
				lc_debug("Returning CACHED contents for " << room << "/" << type);
				return entry.contents;
			}
		}
		lc_debug("generating slim_locker for the first time for entry " << room << "/" << type);
		auto& locker = locker_cache::get_slim_locker(room,i_type);
		return locker.contents;
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
		locker_cache::queue_removal_by_id(id);
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
		storage->reserve(container.size());
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
		locker_cache::set_dirty(room,to_type_integral(type));
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
		// TODO
		locker_cache::set_dirty(room,to_type_integral(type));
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
		locker_cache::set_dirty(room,to_type_integral(type));
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
		locker_cache::set_dirty(room,to_type_integral(type));
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
#undef __MENTOC_SHOW_UTIL_MAP_DUMP_OUTPUT__
#undef __MENTOC_ORM_UTIL_DEBUG_OUTPUT__
};
