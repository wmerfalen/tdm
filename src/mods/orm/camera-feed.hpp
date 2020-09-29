#ifndef __MENTOC_MODS_ORM_CAMERA_FEED_HEADER__
#define __MENTOC_MODS_ORM_CAMERA_FEED_HEADER__

#include "orm-base.hpp"

namespace mods::orm {
	using strmap_t = std::map<std::string,std::string>;
	using sql_compositor = mods::sql::compositor<mods::pq::transaction>;

	struct camera_feed : public mods::orm::orm_base<camera_feed,std::string> {
		using primary_choice_t = std::string;
		std::string table_name() const { return "camera_feed"; }
		std::string column_prefix() const { return "feed_"; }
		std::string id_column() const { return "feed_id"; }
		camera_feed() : id(0) {
			this->init(); loaded = 0;
		}
		~camera_feed() = default;

		std::string primary_key_name() { return id_column(); }
		std::string primary_key_value(){ return std::to_string(this->id); }

		uint64_t initialize_row();
		int16_t feed(const pqxx::result::reference &);
		void init();

		strmap_t export_class();
		//int16_t				load_by_player(uint64_t);
		int16_t save(){
			return std::get<0>(this->update<camera_feed>(this));
		}
		using id_type_t = uint64_t;
		using feed_vnum_t = uint32_t;
		using room_vnum_t = uint32_t;
		using feed_order_t = uint8_t;

		id_type_t id;
		id_type_t feed_id;
		std::string feed_type;
		feed_vnum_t feed_vnum;
		room_vnum_t feed_room_vnum;
		feed_order_t feed_order;
		long created_at;
		long updated_at;
		bool loaded;
	};

};

#endif
