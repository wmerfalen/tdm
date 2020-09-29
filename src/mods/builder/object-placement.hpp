#ifndef __MENTOC_MODS_BUILDER_OBJECT_PLACEMENT_HEADER__
#define __MENTOC_MODS_BUILDER_OBJECT_PLACEMENT_HEADER__
#include "../builder.hpp"
#include "../pq.hpp"
#include "../util.hpp"
#include "../object.hpp"
#include "../../structs.h"
#include <string>
#include <optional>

namespace mods::builder::object_placement {
	using str_map_t = std::map<std::string,std::string>;
	str_map_t extract_line_items(const std::string& line,const std::vector<std::string>&);
	const std::vector<std::string> rifle_slot_names = {
		"sight",
		"under_barrel",
		"grip",
		"barrel",
		"muzzle",
		"magazine",
		"stock",
		"strap"
	};
	struct deep_object_parser_t {
		std::vector<obj_ptr_t>& parse_line_item(std::string line);
		private:
		std::vector<obj_ptr_t> m_line_items;
	};
	struct backpack_t {
		std::deque<obj_ptr_t> items;
	};
	struct holster_t {

	};
	struct rifle_attachments_t {
		using encoding_t = std::string;
		obj_ptr_t sight;
		obj_ptr_t under_barrel;
		obj_ptr_t grip;
		obj_ptr_t barrel;
		obj_ptr_t muzzle;
		obj_ptr_t magazine;
		obj_ptr_t stock;
		obj_ptr_t strap;
		rifle_attachments_t() = delete;
		rifle_attachments_t(const encoding_t& line) {
			import_objects(line);
		}
		rifle_attachments_t(encoding_t& line) {
			import_objects(line);
		}
		~rifle_attachments_t() = default;

		void import_objects(const encoding_t&);
		encoding_t export_objects();
	};
};

ACMD(do_place_object);

#endif
