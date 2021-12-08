#ifndef __MENTOC_MODS_RIFLE_ATTACHMENTS_HEADER__
#define __MENTOC_MODS_RIFLE_ATTACHMENTS_HEADER__
#include "deep-object-parser.hpp"

#ifdef __MENTOC_PARSER_DEBUG__
#define mdo_debug(A)\
			std::cerr << "[mods::deep_object_parser:" << A << "\n";
#else
#define mdo_debug(A) /**-*/
#endif

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
			void set_level(uint16_t level) {
				m_rifle_level = level;
			}
			uint16_t get_level() const {
				return m_rifle_level;
			}
			int16_t base_damage;
			int16_t incendiary_damage_percent;
			int16_t explosive_damage_percent;
			int16_t shrapnel_damage_percent;
			int16_t corrosive_damage_percent;
			int16_t cryogenic_damage_percent;
			int16_t radiation_damage_percent;
			int16_t emp_damage_percent;
			int16_t shock_damage_percent;
			int16_t anti_matter_damage_percent;

			int16_t incendiary_damage;
			int16_t explosive_damage;
			int16_t shrapnel_damage;
			int16_t corrosive_damage;
			int16_t cryogenic_damage;
			int16_t radioactive_damage;
			int16_t anti_matter_damage;
			int16_t emp_damage;
			int16_t shock_damage;

			int16_t damage_dice_count;
			int16_t damage_dice_sides;

			/** --#=================================================#-- */
			/** --[ SECTION START: ((( Fairly easy to implement ))) ]-- */
			/** --#=================================================#-- */
			float zoom_multiplier;						/** honored [ 2021-04-13 ] */
			int16_t free_ammo_chance; 				/** honored [ 2021-04-13 ] */
			int16_t regenerate_ammo_chance; 	/** honored [ 2021-04-13 ] */
			int16_t damage_percent_bonus;			/** honored [ 2021-04-13 ] */
			int16_t ammunition_amount;				/** honored [ 2021-04-13 ] */
			int16_t armor_penetration_amount;	/** honored partially (adds 10% bonus if it's non-zero. doesn't affect if lte zero) */
			int16_t disorient_amount;
			int16_t durability_profile;
			int16_t cooldown_between_shots;
			int16_t thermal_range;
			int16_t night_vision_range;
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
			uint16_t m_rifle_level;
	};
	namespace rifle_attachments {
		std::map<uuid_t,std::string>& uuid_schema_list();
		std::map<uuid_t,std::shared_ptr<mods::rifle_attachments_t>>& global_list();
		std::shared_ptr<mods::rifle_attachments_t> make(std::string_view str);
		std::shared_ptr<mods::rifle_attachments_t> by_uuid(const uuid_t& uuid);
		std::vector<std::shared_ptr<mods::rifle_attachments_t>> by_player(player_ptr_t& player);
		void erase(const uuid_t& uuid);
	};
};

#undef mdo_debug

#endif
