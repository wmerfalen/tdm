#ifndef __MENTOC_MODS_ORM_DOOR_EVENT_HEADER__
#define __MENTOC_MODS_ORM_DOOR_EVENT_HEADER__

#include "orm-base.hpp"

namespace mods::orm {
	using strmap_t = std::map<std::string,std::string>;
	using sql_compositor = mods::sql::compositor<mods::pq::transaction>;

	struct door_event : public mods::orm::orm_base<door_event,std::string> {
		using primary_choice_t = std::string;
		std::string table_name() const { return "door_event"; }
		std::string column_prefix() const { return "event_"; }
		std::string id_column() const { return "event_id"; }
		door_event() : id(0) {
			this->init(); loaded = 0;
		}
		~door_event() = default;

		std::string primary_key_name() { return id_column(); }
		std::string primary_key_value(){ return std::to_string(this->id); }

		uint64_t initialize_row();
		int16_t feed(const pqxx::result::reference &);
		void init();

		strmap_t export_class();
		//int16_t				load_by_player(uint64_t);
		int16_t save(){
			return std::get<0>(this->update<door_event>(this));
		}
		using id_type_t = uint64_t;
		using event_vnum_t = uint32_t;
		using room_vnum_t = uint32_t;
		using event_order_t = uint8_t;

		id_type_t id;
		id_type_t event_id;
		std::string event_type;
		event_vnum_t event_vnum;
		room_vnum_t event_room_vnum;
		event_order_t event_order;
		long created_at;
		long updated_at;
		bool loaded;
	};

};

#endif
