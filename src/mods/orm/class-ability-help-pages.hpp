#ifndef __MENTOC_MODS_ORM_class_ability_help_pages_HEADER__
#define __MENTOC_MODS_ORM_class_ability_help_pages_HEADER__

#include "orm-base.hpp"

namespace mods::orm {
	using strmap_t = std::map<std::string,std::string>;
	using sql_compositor = mods::sql::compositor<mods::pq::transaction>;

#define CLASS_ABILITY_HELP_PAGES_ORM_MEMBERS ( \
(uint64_t,id,0,1,null,1), \
(std::string,ch_class,"",3,null,1), \
(std::string,ch_title,"",3,null,1), \
(std::string,ch_ability,"",3,null,1), \
(std::string,ch_usage,"",3,null,1), \
(std::string,ch_description,"",3,null,1), \
(std::string,ch_examples,"",3,null,1), \
(std::string,ch_keywords,"",3,null,1))

#define CLASS_ABILITY_HELP_PAGES_INITIALIZE_ROW_MEMBERS \
(ch_class) \
(ch_title) \
(ch_ability) \
(ch_usage) \
(ch_description) \
(ch_examples) \
(ch_keywords) \
 


	struct class_ability_help_pages : public mods::orm::orm_base<class_ability_help_pages,std::string> {
		MENTOC_ORM_CLASS(CLASS_ABILITY_HELP_PAGES_ORM_MEMBERS,"class_ability_help_pages");
		auto vnum() {
			return id;
		}
	
		class_ability_help_pages() {
			this->init();
			loaded = 0;
			id = 0;
		}
		~class_ability_help_pages() = default;
	
		MENTOC_ORM_PRIMARY_KEY_FUNCTIONS();
		MENTOC_ORM_FEED_MULTI(CLASS_ABILITY_HELP_PAGES_ORM_MEMBERS,class_ability_help_pages);
	
		std::deque<class_ability_help_pages> rows;
		auto destroy() {
			return  remove(this);
		}
		auto save() {
			return std::get<0>(this->update<class_ability_help_pages>(this));
		}
	#ifdef CLASS_ABILITY_HELP_PAGES_INITIALIZE_ROW_MEMBERS
		MENTOC_ORM_INITIALIZE_ROW_USING(CLASS_ABILITY_HELP_PAGES_INITIALIZE_ROW_MEMBERS);
	#endif
	};
	struct class_ability_help_pages_status_t {
		std::tuple<int16_t,std::string> orm_status;
		std::unique_ptr<class_ability_help_pages> record;
		class_ability_help_pages_status_t() = default;
		class_ability_help_pages_status_t(class_ability_help_pages_status_t&& other) : orm_status(other.orm_status),
			record(std::move(other.record)) {}
	};
	
	using class_ability_help_pages_list_t = std::deque<std::shared_ptr<class_ability_help_pages>>;
	
	class_ability_help_pages_list_t& class_ability_help_pages_list();
	
	class_ability_help_pages_list_t load_all_class_ability_help_pages_list();
};

#endif
