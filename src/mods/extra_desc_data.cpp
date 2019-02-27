#include "extra_desc_data.hpp"
//#include "string.hpp"
//#include <string_view>
namespace mods {
	extra_desc_data::extra_desc_data() : 
		keyword(""), description(""){ }
	extra_desc_data::extra_desc_data(
				std::string_view k,std::string_view d) : 
		keyword(k.data()), description(d.data()){ }
	
};
