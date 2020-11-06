#ifndef __MENTOC_MODS_RIFLE_ATTACHMENTS_HEADER__
#define __MENTOC_MODS_RIFLE_ATTACHMENTS_HEADER__
#include "deep-object-parser.hpp"

#define mdo_debug(A)\
			std::cerr << "[mods::deep_object_parser:" << A << "\n";
namespace mods {
	using str_map_t = std::map<std::string,std::string>;
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
		std::string& base_yaml_file();
		rifle_attachments_t() = delete;
		rifle_attachments_t(const encoding_t& line) {
			m_rifle_yaml_file = extract_base_yaml_file(line);
			import_objects(line);
			mdo_debug("base yaml file:'" << base_yaml_file() << "'");
		}
		rifle_attachments_t(encoding_t& line) {
			m_rifle_yaml_file = extract_base_yaml_file(line);
			import_objects(line);
			mdo_debug("base yaml file:'" << base_yaml_file() << "'");
		}
		~rifle_attachments_t() = default;

		std::string extract_base_yaml_file(const encoding_t& line);
		void import_objects(const encoding_t&);
		encoding_t export_objects();
		private:
			deep_object_parser_t m_parser;
			std::string m_rifle_yaml_file;
	};
};

#undef mdo_debug

#endif
