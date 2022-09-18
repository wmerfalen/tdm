#ifndef __MENTOC_MODS_ORM_locker_HEADER__
#define __MENTOC_MODS_ORM_locker_HEADER__

#include "orm-base.hpp"

namespace mods::orm {
	using strmap_t = std::map<std::string,std::string>;
	using sql_compositor = mods::sql::compositor<mods::pq::transaction>;

#define LOCKER_ORM_MEMBERS ( \
    (uint64_t,id,0,1,null,1), \
    (std::string,l_type,"",3,null,1), \
    (std::string,l_yaml,"",3,null,1), \
    (room_vnum,l_room_vnum,0,1,null,1), \
    (uint16_t,l_count,0,1,null,1))

#define LOCKER_INITIALIZE_ROW_MEMBERS \



	struct locker : public mods::orm::orm_base<locker,std::string> {
		static constexpr std::string_view TABLE_NAME = "locker";
		static constexpr std::array<std::string_view,4> TYPES = {
			"UNK",
			"WPN",
			"ARM",
			"AMU",
		};
		enum type_t : uint8_t {
			UNKNOWN = 0,
			WEAPON,
			ARMOR,
			AMMO,
		};
		static std::vector<std::string> list_locker_by_type(std::string_view type,const room_vnum& room);
		static std::string_view to_type(std::string_view t) {
			if(t.compare("weapon") == 0) {
				return TYPES[type_t::WEAPON];
			}
			if(t.compare("armor") == 0) {
				return TYPES[type_t::ARMOR];
			}
			if(t.compare("ammo") == 0) {
				return TYPES[type_t::AMMO];
			}
			return TYPES[type_t::UNKNOWN];
		}
		static constexpr uint16_t default_weapon_count = 3;
		static constexpr uint16_t default_ammo_count = 3;
		static constexpr uint16_t default_armor_count = 3;
		using status_t = std::pair<bool,std::string>;
		static status_t place_locker(std::string_view type,const room_vnum& room, const std::vector<std::string>& args);
		static status_t place_locker_item(std::string_view type,const room_vnum& room, std::string_view yaml,uint16_t count);
		static status_t set_quota(std::string_view type,const room_vnum& room,uint16_t count);
		static status_t remove_locker(std::string_view type,const room_vnum& room);
		static status_t remove_item_by_id(const uint64_t& id);
		MENTOC_ORM_CLASS(LOCKER_ORM_MEMBERS,"locker");
		auto vnum() {
			return id;
		}

		locker() {
			this->init();
			loaded = 0;
			id = 0;
		}
		~locker() = default;

		MENTOC_ORM_PRIMARY_KEY_FUNCTIONS();
		MENTOC_ORM_FEED_MULTI(LOCKER_ORM_MEMBERS,locker);

		std::deque<locker> rows;
		auto destroy() {
			return  remove(this);
		}
		auto save() {
			return std::get<0>(this->update<locker>(this));
		}
		std::vector<std::tuple<int16_t,std::string>> insert_many(const room_vnum& room,std::string_view type,const std::vector<std::string>& items,const uint16_t& counts);
		std::vector<std::tuple<int16_t,std::string>> insert_many(const room_vnum& room,std::string_view type,const std::vector<std::pair<uint16_t,std::string>>& items);

#ifdef LOCKER_INITIALIZE_ROW_MEMBERS
		MENTOC_ORM_INITIALIZE_ROW_USING(LOCKER_INITIALIZE_ROW_MEMBERS);
#endif
	};
	struct locker_status_t {
		std::tuple<int16_t,std::string> orm_status;
		std::unique_ptr<locker> record;
		locker_status_t() = default;
		locker_status_t(locker_status_t&& other) : orm_status(other.orm_status),
			record(std::move(other.record)) {}
	};

	using locker_list_t = std::deque<std::shared_ptr<locker>>;

	locker_list_t& locker_list();

	locker_list_t load_all_locker_list();
};

#endif
