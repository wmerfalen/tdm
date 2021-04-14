#ifndef __MENTOC_MODS_RIFLE_ATTACHMENTS_HEADER__
#define __MENTOC_MODS_RIFLE_ATTACHMENTS_HEADER__
#include "deep-object-parser.hpp"

#define mdo_debug(A)\
			std::cerr << "[mods::deep_object_parser:" << A << "\n";
namespace mods {
	using str_map_t = std::map<std::string,std::string>;
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
			obj_ptr_t base_object;
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

			std::map<std::string,std::string> yaml_map();
			std::map<std::string,std::string> friendly_map();
			std::string examine();

			std::string extract_base_yaml_file(const encoding_t& line);
			void import_objects(const encoding_t&);
			encoding_t export_objects();
			uuid_t owner() const {
				return m_owner_uuid;
			}
			void set_owner_uuid(uuid_t u) {
				m_owner_uuid = u;
			}
			int16_t incendiary_damage_percent;
			int16_t explosive_damage_percent;
			int16_t shrapnel_damage_percent;
			int16_t corrosive_damage_percent;
			int16_t cryogenic_damage_percent;
			int16_t radiation_damage_percent;
			int16_t emp_damage_percent;
			int16_t shock_damage_percent;
			int16_t anti_matter_damage_percent;

			/** --#=================================================#-- */
			/** --[ SECTION START: ((( Fairly easy to implement ))) ]-- */
			/** --#=================================================#-- */
			float zoom_multiplier;						/** honored [ 2021-04-13 ] */
			int16_t free_ammo_chance; 				/** honored [ 2021-04-13 ] */
			int16_t regenerate_ammo_chance; 	/** honored [ 2021-04-13 ] */
			int16_t incendiary_damage;				/** honored [ ? (see elemental.cpp) ] */
			int16_t radiation_damage;					/** honored [ ? (see elemental.cpp) ] */
			int16_t damage_percent_bonus;			/** honored [ 2021-04-13 ] */
			int16_t armor_penetration_amount;
			int16_t ammunition_amount;
			int16_t disorient_amount;
			int16_t durability_profile;
			/** --#=================================================#-- */
			/** --[ SECTION END:   ((( Fairly easy to implement ))) ]-- */
			/** --#=================================================#-- */



			/** --#=======================================#-- */
			/** --[ SECTION START: ((( Large Features ))) ]-- */
			/** --#=======================================#-- */
			int16_t accuracy_points;
			int16_t recoil_reduction;
			int16_t aimed_limb_accuracy_percent;
			int16_t bleed_chance;
			int16_t loudness_reduction;
			std::string underbarrel_launcher_type;
			/** --#======================================#-- */
			/** --[ SECTION END:  ((( Large Features ))) ]-- */
			/** --#=======================================#-- */
		private:
			deep_object_parser_t m_parser;
			std::string m_rifle_yaml_file;
			uuid_t m_owner_uuid;
	};
	namespace rifle_attachments {
		std::map<uuid_t,std::string>& uuid_schema_list();
	};
};

#undef mdo_debug

#endif
