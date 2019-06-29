#ifndef __MENTOC_MODS_OBJECT_SOURCE__
#define  __MENTOC_MODS_OBJECT_SOURCE__

#include "object.hpp"
#include "../structs.h"

namespace mods {
	object::~object(){
	}
	object::object(){
		init();
	}
	object& object::add_type(object_type_enum_t type){
		m_type.emplace_back(type);
		return *this;
	}
	object& object::remove_type(object_type_enum_t type){
		if(m_type.size()){
		std::vector<object_type_enum_t> final_version;
		final_version.reserve(m_type.size());
		std::copy_if(m_type.begin(),m_type.end(),final_version.begin(),
				[type](object_type_enum_t obj_type) -> bool {
					return type != obj_type;
				});
		m_type = final_version;
		}
		return *this;
	}
	void object::init(){
		m_shared_ptr = std::make_shared<obj_data>();
	}
	std::shared_ptr<obj_data> object::get_shared_ptr(){
		return m_shared_ptr;
	}
	object& object::set_shared_ptr(std::shared_ptr<obj_data>& ptr){
		m_shared_ptr = ptr;
		return *this;
	}
	object::object(mods::object* ptr) {
		init();
		m_type = ptr->m_type;
		if(ptr->m_shared_ptr){
			set_shared_ptr(ptr->m_shared_ptr);
		}
	}

};//end namespace
