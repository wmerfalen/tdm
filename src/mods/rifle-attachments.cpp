#include "rifle-attachments.hpp"
#include "interpreter.hpp"
#include "orm/rifle-attachment.hpp"
#include "query-objects.hpp"

//#define __MENTOC_SHOW_MODS_RIFLE_ATTACHMENT_DEBUG_OUTPUT__
#ifdef __MENTOC_SHOW_MODS_RIFLE_ATTACHMENT_DEBUG_OUTPUT__
	#define m_debug(MSG) mentoc_prefix_debug("[mods::rifle_attachments]")  << MSG << "\n";
#else
	#define m_debug(MSG) ;;
#endif
#ifdef __MENTOC_PARSER_DEBUG__
#define mdo_debug(A)\
	std::cerr << "[mods::deep_object_parser:" << A << "\n";
#else
#define mdo_debug(A) /**-*/
#endif

namespace mods {

	extern void obj_to_char(obj_ptr_t  object, player_ptr_t player);
	extern void obj_from_char(obj_ptr_t in_object);
	rifle_attachments_t::rifle_attachments_t(const encoding_t& line) {
		m_rifle_yaml_file = extract_base_yaml_file(line);
		int16_t i = import_objects(line);
		if(i != IMPORT_OKAY) {
			log("WARNING: import_objects failed using line: '%s', error code: %d",line.c_str(),i);
		}
		mdo_debug("base yaml file:'" << base_yaml_file() << "'");
	}
	rifle_attachments_t::rifle_attachments_t(encoding_t& line) {
		m_rifle_yaml_file = extract_base_yaml_file(line);
		int16_t i = import_objects(line);
		if(i != IMPORT_OKAY) {
			log("WARNING: import_objects failed using line: '%s', error code: %d",line.c_str(),i);
		}
		mdo_debug("base yaml file:'" << base_yaml_file() << "'");
	}
	rifle_attachments_t::rifle_attachments_t(obj_ptr_t& weapon) {
		m_rifle_yaml_file = weapon->feed_file();
		int16_t i = import_objects(m_rifle_yaml_file);
		if(i != IMPORT_OKAY) {
			log("WARNING: import_objects failed using line: '%s', error code: %d",m_rifle_yaml_file.c_str(),i);
		}
		mdo_debug("base yaml file:'" << base_yaml_file() << "'");
	}
	namespace rifle_attachments {
		/**
		 * @brief gets all rifle attachment objects (as shared_ptr) associated with a player
		 *
		 * @param player
		 *
		 * @return
		 */
		std::vector<std::shared_ptr<mods::rifle_attachments_t>> by_player(player_ptr_t& player) {
			std::vector<std::shared_ptr<mods::rifle_attachments_t>> list;
			std::vector<uuid_t> uuid_list;
			for(const auto item : player->equipment()) {
				if(!item) {
					continue;
				}
				auto ptr = by_uuid(item->uuid);
				if(ptr) {
					if(std::find(uuid_list.cbegin(),uuid_list.cend(),item->uuid) == uuid_list.cend()) {
						list.emplace_back(ptr);
						uuid_list.emplace_back(item->uuid);
					}
				}
			}
			for(const auto item : player->vcarrying()) {
				if(!item) {
					continue;
				}
				auto ptr = by_uuid(item->uuid);
				if(ptr) {
					if(std::find(uuid_list.cbegin(),uuid_list.cend(),item->uuid) == uuid_list.cend()) {
						list.emplace_back(ptr);
						uuid_list.emplace_back(item->uuid);
					}
				}
			}
			return list;
		}
		/**
		 * @brief erases rifle attachments by their base_object's uuid
		 *
		 * @param uuid
		 */
		void erase(const uuid_t& uuid) {
			std::cerr << green_str("erasing uuid from rifle attachments:") << uuid << "\n";
			uuid_schema_list().erase(uuid);
			global_list().erase(uuid);
		}
		/**
		 * @brief optionally fetches a rifle attachment by it's uuid
		 *
		 * @param uuid
		 *
		 * @return the shared_ptr to the rifle attachment whose base_object->uuid is uuid. returns nullptr if none exist with that uuid
		 */
		std::shared_ptr<mods::rifle_attachments_t> by_uuid(const uuid_t& uuid) {
			if(global_list().find(uuid) == global_list().end()) {
				return nullptr;
			}
			return global_list()[uuid];
		}
		/**
		 * @brief schema for each rifle attachment are stored in a map of uuid_t to string. this returns that container
		 *
		 * @return
		 */
		std::map<uuid_t,std::string>& uuid_schema_list() {
			static std::map<uuid_t,std::string> list;
			return list;
		}
		/**
		 * @brief like the schema list except this stores the uuid_t to shared_ptr list.
		 *
		 * @return
		 */
		std::map<uuid_t,std::shared_ptr<mods::rifle_attachments_t>>& global_list() {
			static std::map<uuid_t,std::shared_ptr<mods::rifle_attachments_t>> list;
			return list;
		}
		/**
		 * @brief THE preferred way to create a rifle attachment. Instantiating a rifle_attachments_t object alone will not work and you will have issues when it comes to (un)equipping/(un)carrying the rifle attachment.
		 *
		 * @param str
		 *
		 * @return a shared_ptr to the created rifle_attachment. This function _DOES NOT_ deduplicate.
		 */
		std::shared_ptr<mods::rifle_attachments_t> make(std::string_view str) {
			auto ptr = std::make_shared<mods::rifle_attachments_t>(str.data());
			global_list()[ptr->base_object->uuid] = ptr;
			uuid_schema_list()[ptr->base_object->uuid] = str;
			return ptr;
		}
		/**
		 * @brief The purpose of this function is to create a rifle attachment from a regular rifle type weapon (where weapon->has_rifle() is true). It would make sense to have rifle attachments work for both PC's and NPC's but the code simply isn't there yet for that to happen. Ideally, all rifle type weapons should be rifle attachments, but since rifle attachments are a feature that came way after the concept of rifle type weapons, there simply isn't assurance that that particular strategy would work.
		 *
		 * @param weapon
		 *
		 * @return nullptr if `weapon` isn't a rifle type weapon. Returns a shared_ptr<rifle_attachments_t> if imported successfully.
		 */
		std::shared_ptr<mods::rifle_attachments_t> make_from(obj_ptr_t& weapon) {
			if(!weapon->has_rifle()) {
				log("SYSERR: trying to make rifle attachment from a weapon that isn't a rifle type");
				return nullptr;
			}
			auto ptr = std::make_shared<mods::rifle_attachments_t>(weapon);
			global_list()[ptr->base_object->uuid] = ptr;
			uuid_schema_list()[ptr->base_object->uuid] = ptr->export_objects();
			return ptr;
		}


		/**
		 * @brief takes a deep object string and creates the rifle attachment object for you. The object will appear in your inventory via the player->carry() function.
		 *
		 * @param do_instantiate_rifle_attachment
		 */
		SUPERCMD(do_instantiate_rifle_attachment) {
			ADMIN_REJECT();
			DO_HELP("instantiate_rifle_attachment");
			auto vec_args = PARSE_ARGS();
			if(vec_args.size() == 0) {
				player->errorln("not enough arguments");
				return;
			}
			for(const auto& str : vec_args) {
				auto ptr = mods::rifle_attachments::make(str);
				player->carry(ptr->base_object);
				player->sendln("[+] Done");
			}
			ADMIN_DONE();
		}
		/**
		 * @brief allows you to manually call load_player_rifle_attachments. Only use this if you really know what you're doing.
		 *
		 * @param do_load_my_rifle_attachments
		 */
		SUPERCMD(do_load_my_rifle_attachments) {
			player->sendln("Loading...");
			mods::orm::load_player_rifle_attachments(player);
			player->sendln("[+] Done");
		}
		/**
		 * @brief Will print everything in your inventory
		 *
		 * @param do_vcarrying
		 */
		ACMD(do_vcarrying) {
			player->sendln("Schema list...");
			for(const auto& pair : player->vcarrying()) {
				player->sendln(CAT("Item:",pair->name.c_str()));
			}
			player->sendln("[+] Done");
		}
		/**
		 * @brief iterates through all schemas and the global list of shared_ptr's that contain every rifle attachment object in the game.
		 *
		 * @param do_list_rifle_attachments
		 */
		SUPERCMD(do_list_rifle_attachments) {
			player->sendln("Schema list...");
			for(const auto& pair : uuid_schema_list()) {
				player->sendln(CAT("Schema[",pair.first,"]:'",pair.second,"'"));
			}
			player->sendln("Globals list...");
			for(const auto& pair : global_list()) {
				player->sendln(CAT("Global[",pair.first,"]:"));
			}
			player->sendln("[+] Done");
		}
		/**
		 * @brief called by mods::init();
		 */
		void init() {
			mods::interpreter::add_user_command("vcarrying", do_vcarrying);
			mods::interpreter::add_user_command("load_my_rifle_attachments",  do_load_my_rifle_attachments);
			mods::interpreter::add_user_command("rifle-attachments:load",  do_load_my_rifle_attachments);

			mods::interpreter::add_builder_command("instantiate_rifle_attachment", do_instantiate_rifle_attachment);
			mods::interpreter::add_builder_command("list_rifle_attachments",  do_list_rifle_attachments);

			mods::interpreter::add_builder_command("rifle-attachments:instantiate", do_instantiate_rifle_attachment);
			mods::interpreter::add_builder_command("rifle-attachments:list",  do_list_rifle_attachments);
		}
	};

	/**
	 * @brief static function that transforms a string to the rifle_attachments_t::slot_t integral type
	 *
	 * @param str
	 *
	 * @return
	 */
	std::optional<rifle_attachments_t::slot_t> rifle_attachments_t::from_string_to_slot(std::string_view str) {
		m_debug("from_string_to_slot comparing: '" << str << "'");
		if(str.compare("SIGHT") == 0) {
			return SLOT_SIGHT;
		}
		if(str.compare("UNDER_BARREL") == 0) {
			m_debug("is UNDER_BARREL");
			return SLOT_UNDERBARREL;
		}
		if(str.compare("GRIP") == 0) {
			return SLOT_GRIP;
		}
		if(str.compare("BARREL") == 0) {
			return SLOT_BARREL;
		}
		if(str.compare("MUZZLE") == 0) {
			return SLOT_MUZZLE;
		}
		if(str.compare("MAGAZINE") == 0) {
			return SLOT_MAGAZINE;
		}
		if(str.compare("STOCK") == 0) {
			return SLOT_STOCK;
		}
		if(str.compare("STRAP") == 0) {
			return SLOT_STRAP;
		}
		return std::nullopt;
	}
	/**
	 * @brief sets the slot to nullptr
	 *
	 * @param slot
	 */
	void rifle_attachments_t::clear_slot(const slot_t& slot) {
		switch(slot) {
			case SLOT_SIGHT:
				sight = nullptr;
				break;
			case SLOT_UNDERBARREL:
				under_barrel = nullptr;
				break;
			case SLOT_GRIP:
				grip = nullptr;
				break;
			case SLOT_BARREL:
				barrel = nullptr;
				break;
			case SLOT_MUZZLE:
				muzzle = nullptr;
				break;
			case SLOT_MAGAZINE:
				magazine = nullptr;
				break;
			case SLOT_STOCK:
				stock = nullptr;
				break;
			case SLOT_STRAP:
				strap = nullptr;
				break;
			default:
				std::cerr << "warning: didn't clear a slot\n";
				return;
		}
		m_debug("Slot cleared: " << slot);
	}
	/**
	 * @brief sets the slot to the passed in object
	 *
	 * @param slot
	 * @param obj
	 */
	void rifle_attachments_t::set_slot(const slot_t& slot,obj_ptr_t& obj) {
		switch(slot) {
			case SLOT_SIGHT:
				sight = obj;
				break;
			case SLOT_UNDERBARREL:
				under_barrel = obj;
				break;
			case SLOT_GRIP:
				grip = obj;
				break;
			case SLOT_BARREL:
				barrel = obj;
				break;
			case SLOT_MUZZLE:
				muzzle = obj;
				break;
			case SLOT_MAGAZINE:
				magazine = obj;
				break;
			case SLOT_STOCK:
				stock = obj;
				break;
			case SLOT_STRAP:
				strap = obj;
				break;
			default:
				m_debug("not setting because unrecognized slot: " << slot);
				return;
		}
	}
	/**
	 * Returns number of items detached from the weapon.
	 */
	/**
	 * @brief removes the slot (if it has an object attached there) and places it in your inventory. this will print the removal and inventory modification strings to the user.
	 *
	 * @param player
	 * @param slot
	 *
	 * @return returns 1 if the slot is occupied and it's been flushed to the player's inventory. returns zero if nothing is occupying that slot (and thus nothing changed to the player's inventory for obvious reasons).
	 */
	int16_t rifle_attachments_t::detach_into_inventory(player_ptr_t& player,const slot_t& slot) {
		if(has_slot(slot)) {
			obj_ptr_t obj = get_slot(slot);
			clear_slot(slot);
			player->sendln(CAT("You remove ",obj->name.c_str()," from ",base_object->name.c_str()));
			player->sendln(CAT("You put ",obj->name.c_str()," into your inventory"));
			player->carry(obj);
			remove_stats(obj);
			return 1;
		}
		return 0;

	}
	/**
	 * @brief attaches the attachment to the specified slot, if one exists. This function sends all the inventory modification strings if and when objects are removed/carried/uncarried.
	 *
	 * @param player
	 * @param str
	 * @param attachment
	 *
	 * @return returns zero if the attachment is attached to the rifle attachment. returns -1 if the attachment is invalid (nullptr). returns -2 if the attachment passed in does not have an attachment_description_t ptr (->attachment()). returns -5 if the passed in 'str' doesn't match any recognized slots. Unrecognized slots do not modify the inventory or the rifle attachment.
	 */
	int16_t rifle_attachments_t::attach_from_inventory(player_ptr_t& player,std::string_view str,obj_ptr_t& attachment) {
		auto opt = from_string_to_slot(str);
		if(opt.has_value()) {
			return -1;
		}
		const slot_t& slot = (slot_t)opt.value();
		return attach_from_inventory(player,slot,attachment);
	}
	int16_t rifle_attachments_t::attach_from_inventory(player_ptr_t& player,const slot_t& slot,obj_ptr_t& attachment) {

		if(!attachment) {
			return -1;
		}
		if(!attachment->has_attachment()) {
			return -2;
		}
		obj_ptr_t existing = get_slot(slot);
		if(existing) {
			if(detach_into_inventory(player,slot)) {
				player->sendln(CAT("You remove a ",existing->name.c_str()," from a ",base_object->name.c_str()));
				player->sendln(CAT("You place a ",existing->name.c_str()," in your inventory."));
			}
		}
		player->sendx(CAT("You grab a ",attachment->name.c_str()).c_str());
		player->uncarry(attachment);
		player->sendln(CAT(" and attach it to a ",base_object->name.c_str(),"..."));
		set_slot(slot,attachment);
		add_stats(attachment);
		update_description();
		return 0;
	}
	/**
	 * @brief checks if the rifle attachment has an object in the specified slot
	 *
	 * @param slot
	 *
	 * @return true if an object occupies the slot, false if not.
	 */
	bool rifle_attachments_t::has_slot(const slot_t& slot) {
		switch(slot) {
			case SLOT_SIGHT:
				return !!sight;
			case SLOT_UNDERBARREL:
				return !!under_barrel;
			case SLOT_GRIP:
				return !!grip;
			case SLOT_BARREL:
				return !!barrel;
			case SLOT_MUZZLE:
				return !!muzzle;
			case SLOT_MAGAZINE:
				return !!magazine;
			case SLOT_STOCK:
				return !!stock;
			case SLOT_STRAP:
				return !!strap;
			default:
				return false;
		}

	}

	/**
	 * @brief convenience wrapper function that ultimately calls detach_into_inventory(player_ptr_t&,const slot_t&).
	 *
	 * @param player
	 * @param str
	 *
	 * @return if 'str' is not recognized, returns zero. if the 'str' is recognized the return value is the same as detach_into_inventory(player_ptr_t&,const slot_t&)
	 */
	int16_t rifle_attachments_t::detach_into_inventory(player_ptr_t& player,std::string_view str) {
		auto opt = from_string_to_slot(str);
		if(opt.has_value()) {
			const slot_t& slot = (slot_t)opt.value();
			return detach_into_inventory(player,slot);
		}
		return 0;
	}
	/**
	 * @brief performs crude subtraction (without bounds checking or overflow detection)
	 *
	 * @param object
	 */
	void rifle_attachments_t::remove_stats(const obj_ptr_t& object) {
		incendiary_damage_percent -= object->attachment()->attributes->incendiary_damage_percent;
		explosive_damage_percent -= object->attachment()->attributes->explosive_damage_percent;
		shrapnel_damage_percent -= object->attachment()->attributes->shrapnel_damage_percent;
		corrosive_damage_percent -= object->attachment()->attributes->corrosive_damage_percent;
		cryogenic_damage_percent -= object->attachment()->attributes->cryogenic_damage_percent;
		radiation_damage_percent -= object->attachment()->attributes->radiation_damage_percent;
		emp_damage_percent -= object->attachment()->attributes->emp_damage_percent;
		shock_damage_percent -= object->attachment()->attributes->shock_damage_percent;
		anti_matter_damage_percent -= object->attachment()->attributes->anti_matter_damage_percent;
		zoom_multiplier -= object->attachment()->attributes->zoom_multiplier;
		ammunition_amount -= object->attachment()->attributes->ammunition_amount;
		damage_percent_bonus -= object->attachment()->attributes->damage_percent_bonus;
		durability_profile -= object->attachment()->attributes->durability_profile;
		incendiary_damage -= object->attachment()->attributes->incendiary_damage;
		radioactive_damage -= object->attachment()->attributes->radiation_damage;
		armor_penetration_amount -= object->attachment()->attributes->armor_penetration_amount;
		disorient_amount -= object->attachment()->attributes->disorient_amount;
		free_ammo_chance -= object->attachment()->attributes->free_ammo_chance;
		regenerate_ammo_chance -= object->attachment()->attributes->regenerate_ammo_chance;

		recoil_reduction -= object->attachment()->attributes->recoil_reduction;
		bleed_chance -= object->attachment()->attributes->bleed_chance;
		aimed_limb_accuracy_percent -= object->attachment()->attributes->aimed_limb_accuracy_percent;
		accuracy_points -= object->attachment()->attributes->accuracy_points;
		underbarrel_launcher_type = object->attachment()->attributes->underbarrel_launcher_type;
		loudness_reduction -= object->attachment()->attributes->loudness_reduction;

		thermal_range -= object->attachment()->attributes->thermal_range;
		night_vision_range -= object->attachment()->attributes->night_vision_range;
	}
	/**
	 * @brief performs crude addition to stats without bounds checking or clamping to prevent/detect overflow
	 *
	 * @param object
	 */
	void rifle_attachments_t::add_stats(const obj_ptr_t& object) {
		incendiary_damage_percent += object->attachment()->attributes->incendiary_damage_percent;
		explosive_damage_percent += object->attachment()->attributes->explosive_damage_percent;
		shrapnel_damage_percent += object->attachment()->attributes->shrapnel_damage_percent;
		corrosive_damage_percent += object->attachment()->attributes->corrosive_damage_percent;
		cryogenic_damage_percent += object->attachment()->attributes->cryogenic_damage_percent;
		radiation_damage_percent += object->attachment()->attributes->radiation_damage_percent;
		emp_damage_percent += object->attachment()->attributes->emp_damage_percent;
		shock_damage_percent += object->attachment()->attributes->shock_damage_percent;
		anti_matter_damage_percent += object->attachment()->attributes->anti_matter_damage_percent;
		zoom_multiplier += object->attachment()->attributes->zoom_multiplier;
		ammunition_amount += object->attachment()->attributes->ammunition_amount;
		damage_percent_bonus += object->attachment()->attributes->damage_percent_bonus;
		durability_profile += object->attachment()->attributes->durability_profile;
		incendiary_damage += object->attachment()->attributes->incendiary_damage;
		radioactive_damage += object->attachment()->attributes->radiation_damage;
		armor_penetration_amount += object->attachment()->attributes->armor_penetration_amount;
		disorient_amount += object->attachment()->attributes->disorient_amount;
		free_ammo_chance += object->attachment()->attributes->free_ammo_chance;
		regenerate_ammo_chance += object->attachment()->attributes->regenerate_ammo_chance;

		recoil_reduction += object->attachment()->attributes->recoil_reduction;
		bleed_chance += object->attachment()->attributes->bleed_chance;
		aimed_limb_accuracy_percent += object->attachment()->attributes->aimed_limb_accuracy_percent;
		accuracy_points += object->attachment()->attributes->accuracy_points;
		underbarrel_launcher_type = object->attachment()->attributes->underbarrel_launcher_type;
		loudness_reduction += object->attachment()->attributes->loudness_reduction;

		thermal_range += object->attachment()->attributes->thermal_range;
		night_vision_range += object->attachment()->attributes->night_vision_range;
	}
	/**
	 * @brief sets the base_object's action_description to the string based on all the attached objects to this rifle attachment. Call this function anytime you add/remove items from rifle attachment.
	 */
	void rifle_attachments_t::update_description() {
		base_object->action_description = examine();
	}
	/**
	 * @brief returns the entry at slot
	 *
	 * @param slot
	 *
	 * @return returns the entry at slot. if the slot is unrecognized, this returns the base_object instead. do not call this function without first checking that the slot exists by calling has_slot(const slot_t&)
	 */
	obj_ptr_t& rifle_attachments_t::get_slot(const slot_t& slot) {
		switch(slot) {
			case SLOT_SIGHT:
				return sight;
			case SLOT_UNDERBARREL:
				return under_barrel;
			case SLOT_GRIP:
				return grip;
			case SLOT_BARREL:
				return barrel;
			case SLOT_MUZZLE:
				return muzzle;
			case SLOT_MAGAZINE:
				return magazine;
			case SLOT_STOCK:
				return stock;
			case SLOT_STRAP:
				return strap;
			default:
				return base_object;
		}
	}
	/**
	 * @brief parses a deep object string and creates the base_object along with any attachments. this function returns early if the currently parsed attachment isn't found (meaning nullptr was returned by create_object). the base object must also be a valid object and this function will return early if it's not. once this function is called, you may also call `bool imported_okay()` to check if the line was successfully and fully parsed.
	 *
	 * @param line
	 *
	 * @return returns IMPORT_OKAY, otherwise returns one of the other import_status_t enums depending on where the import failed to create an attachment (or base object).
	 */
	int16_t rifle_attachments_t::import_objects(const encoding_t& line) {
		import_status = 0;
		auto map = m_parser.extract_line_items(line,mods::util::slot_names_for_type("rifle"));
		m_rifle_yaml_file = map["base"];
		m_debug("m_rifle_yaml_file: '" << m_rifle_yaml_file << "'");
		base_object = create_object(map["base"]);
		if(!base_object) {
			import_status = INVALID_BASE_OBJECT;
			return INVALID_BASE_OBJECT;
		}
		base_object->rifle()->attributes->is_rifle_attachment = true;
		base_damage = base_object->rifle()->attributes->base_damage;
		damage_dice_count = base_object->rifle()->attributes->damage_dice_count;
		damage_dice_sides = base_object->rifle()->attributes->damage_dice_sides;
		incendiary_damage = base_object->rifle()->attributes->incendiary_damage;
		explosive_damage = base_object->rifle()->attributes->explosive_damage;
		shrapnel_damage = base_object->rifle()->attributes->shrapnel_damage;
		corrosive_damage = base_object->rifle()->attributes->corrosive_damage;
		cryogenic_damage = base_object->rifle()->attributes->cryogenic_damage;
		radioactive_damage = base_object->rifle()->attributes->radioactive_damage;
		anti_matter_damage = base_object->rifle()->attributes->anti_matter_damage;
		emp_damage = base_object->rifle()->attributes->emp_damage;
		shock_damage = base_object->rifle()->attributes->shock_damage;
		cooldown_between_shots = base_object->rifle()->attributes->cooldown_between_shots;
		incendiary_damage_percent = 0;
		explosive_damage_percent = 0;
		shrapnel_damage_percent = 0;
		corrosive_damage_percent = 0;
		cryogenic_damage_percent = 0;
		radiation_damage_percent = 0;
		emp_damage_percent = 0;
		shock_damage_percent = 0;
		anti_matter_damage_percent = 0;

		zoom_multiplier = 0;
		ammunition_amount = 0;
		loudness_reduction = 0;
		durability_profile = 0;
		accuracy_points = 0;
		incendiary_damage = 0;
		radioactive_damage = 0;
		armor_penetration_amount = 0;
		disorient_amount = 0;
		recoil_reduction = 0;
		free_ammo_chance = 0;
		regenerate_ammo_chance = 0;
		bleed_chance = 0;
		aimed_limb_accuracy_percent = 0;
		underbarrel_launcher_type = "NONE";
		damage_percent_bonus = 0;
		thermal_range = 0;
		night_vision_range = 0;
		static const char* LEVEL_STR = "#level:";
		auto level_position = line.find(LEVEL_STR);
		if(level_position != std::string::npos) {
			auto s = line.substr(level_position + strlen(LEVEL_STR));
			auto i = mods::util::stoi<int>(s);
			set_level(i);
		} else {
			set_level(1);
		}

		for(const auto& pair : map) {
			obj_ptr_t object;
			object = nullptr;
			if(pair.first.compare("sight") == 0) {
				object = sight = create_object(ITEM_ATTACHMENT,pair.second);
				if(!sight) {
					import_status = INVALID_SIGHT;
					return INVALID_SIGHT;
				}
			}
			if(pair.first.compare("under_barrel") == 0) {
				object = under_barrel = create_object(ITEM_ATTACHMENT,pair.second);
				if(!under_barrel) {
					import_status = INVALID_UNDER_BARREL;
					return INVALID_UNDER_BARREL;
				}
			}
			if(pair.first.compare("grip") == 0) {
				object = grip = create_object(ITEM_ATTACHMENT,pair.second);
				if(!grip) {
					import_status = INVALID_GRIP;
					return INVALID_GRIP;
				}
			}
			if(pair.first.compare("barrel") == 0) {
				object = barrel = create_object(ITEM_ATTACHMENT,pair.second);
				if(!barrel) {
					import_status = INVALID_BARREL;
					return INVALID_BARREL;
				}
			}
			if(pair.first.compare("muzzle") == 0) {
				object = muzzle = create_object(ITEM_ATTACHMENT,pair.second);
				if(!muzzle) {
					import_status = INVALID_MUZZLE;
					return INVALID_MUZZLE;
				}
			}
			if(pair.first.compare("magazine") == 0) {
				object = magazine = create_object(ITEM_ATTACHMENT,pair.second);
				if(!magazine) {
					import_status = INVALID_MAGAZINE;
					return INVALID_MAGAZINE;
				}
			}
			if(pair.first.compare("stock") == 0) {
				object = stock = create_object(ITEM_ATTACHMENT,pair.second);
				if(!stock) {
					import_status = INVALID_STOCK;
					return INVALID_STOCK;
				}
			}
			if(pair.first.compare("strap") == 0) {
				object = strap = create_object(ITEM_ATTACHMENT,pair.second);
				if(!strap) {
					import_status = INVALID_STRAP;
					return INVALID_STRAP;
				}
			}
			if(pair.first.compare("level") == 0) {
				set_level(mods::util::stoi(pair.second).value_or(1));
			}
			if(object && object->has_attachment()) {
				add_stats(object);
			}
		}
		update_description();
		import_status = IMPORT_OKAY;
		return IMPORT_OKAY;
	}
	int16_t rifle_attachments_t::import_from_object_pointer(const obj_ptr_t& weapon) {
		import_status = 0;
		if(!weapon->has_rifle()) {
			import_status = INVALID_BASE_OBJECT;
			return import_status;
		}
		m_rifle_yaml_file = weapon->feed_file();
		m_debug("m_rifle_yaml_file: '" << m_rifle_yaml_file << "'");
		base_object = weapon;
		base_object->rifle()->attributes->is_rifle_attachment = true;
		base_damage = base_object->rifle()->attributes->base_damage;
		damage_dice_count = base_object->rifle()->attributes->damage_dice_count;
		damage_dice_sides = base_object->rifle()->attributes->damage_dice_sides;
		incendiary_damage = base_object->rifle()->attributes->incendiary_damage;
		explosive_damage = base_object->rifle()->attributes->explosive_damage;
		shrapnel_damage = base_object->rifle()->attributes->shrapnel_damage;
		corrosive_damage = base_object->rifle()->attributes->corrosive_damage;
		cryogenic_damage = base_object->rifle()->attributes->cryogenic_damage;
		radioactive_damage = base_object->rifle()->attributes->radioactive_damage;
		anti_matter_damage = base_object->rifle()->attributes->anti_matter_damage;
		emp_damage = base_object->rifle()->attributes->emp_damage;
		shock_damage = base_object->rifle()->attributes->shock_damage;
		cooldown_between_shots = base_object->rifle()->attributes->cooldown_between_shots;
		incendiary_damage_percent = 0;
		explosive_damage_percent = 0;
		shrapnel_damage_percent = 0;
		corrosive_damage_percent = 0;
		cryogenic_damage_percent = 0;
		radiation_damage_percent = 0;
		emp_damage_percent = 0;
		shock_damage_percent = 0;
		anti_matter_damage_percent = 0;

		zoom_multiplier = 0;
		ammunition_amount = 0;
		loudness_reduction = 0;
		durability_profile = 0;
		accuracy_points = 0;
		incendiary_damage = 0;
		radioactive_damage = 0;
		armor_penetration_amount = 0;
		disorient_amount = 0;
		recoil_reduction = 0;
		free_ammo_chance = 0;
		regenerate_ammo_chance = 0;
		bleed_chance = 0;
		aimed_limb_accuracy_percent = 0;
		underbarrel_launcher_type = "NONE";
		damage_percent_bonus = 0;
		thermal_range = 0;
		night_vision_range = 0;
		set_level(1);

		sight = nullptr;
		under_barrel = nullptr;
		grip = nullptr;
		barrel = nullptr;
		muzzle = nullptr;
		magazine = nullptr;
		stock = nullptr;
		strap = nullptr;
		update_description();
		import_status = IMPORT_OKAY;
		return IMPORT_OKAY;
	}
	/**
	 * @brief this can be called after import_objects(const encoding_t&) to check whether or not all objects have been successfully parsed and created.
	 *
	 * @return
	 */
	bool rifle_attachments_t::imported_okay() const {
		return import_status == IMPORT_OKAY;
	}
	/**
	 * @brief
	 *
	 * @param line
	 *
	 * @return returns the base object from the deep object string stored in 'line'
	 */
	std::string rifle_attachments_t::extract_base_yaml_file(const encoding_t& line) {
		auto map = m_parser.extract_line_items(line,mods::util::slot_names_for_type("rifle"));
		return map["base"];
	}
	/**
	 * @brief
	 *
	 * @return returns the already parsed base yaml file
	 */
	std::string& rifle_attachments_t::base_yaml_file() {
		return m_rifle_yaml_file;
	}
	/**
	 * @brief
	 *
	 * @return returns a map of key value pairs where the keys are the slot and the values are the return value of feed_file() for each attached object
	 */
	std::map<std::string,std::string> rifle_attachments_t::yaml_map() {
		std::map<std::string,std::string> files;
		if(sight) {
			files["sight"] = sight->feed_file();
		}
		if(under_barrel) {
			files["under_barrel"] = under_barrel->feed_file();
		}
		if(grip) {
			files["grip"] = grip->feed_file();
		}
		if(barrel) {
			files["barrel"] = barrel->feed_file();
		}
		if(muzzle) {
			files["muzzle"] = muzzle->feed_file();
		}
		if(magazine) {
			files["magazine"] = magazine->feed_file();
		}
		if(stock) {
			files["stock"] = stock->feed_file();
		}
		if(strap) {
			files["strap"] = strap->feed_file();
		}
		return files;
	}
	/**
	 * @brief
	 *
	 * @param in_a
	 *
	 * @return the manufacturer and name of the attachment (if it has an attachment). returns an empty string if the passed in object stored at `in_a` does not contain an attachment_description_t (via ->attachment())
	 */
	std::string short_desc(auto& in_a) {
		if(!in_a->has_attachment()) {
			return "";
		}
		auto& a = in_a->attachment()->attributes;
		return CAT(a->manufacturer," ",a->name);
	}
	/**
	 * @brief
	 *
	 * @param in_a
	 *
	 * @return returns a string of all the stats for the passed in object. does NOT do sanity checking on the ->attachment() call.
	 */
	std::string long_desc(auto& in_a) {
		auto& a = in_a->attachment()->attributes;
		return CAT(a->manufacturer," ",a->name,
		        "zoom_multiplier: ",a->zoom_multiplier,"\r\n",
		        "ammunition_amount: ",a->ammunition_amount,"\r\n",
		        "loudness_reduction: ",a->loudness_reduction,"\r\n",
		        "durability_profile: ",a->durability_profile,"\r\n",
		        "accuracy_points: ",a->accuracy_points,"\r\n",
		        "incendiary_damage: ",a->incendiary_damage,"\r\n",
		        "radiation_damage: ",a->radiation_damage,"\r\n",
		        "armor_penetration_amount: ",a->armor_penetration_amount,"\r\n",
		        "disorient_amount: ",a->disorient_amount,"\r\n",
		        "incendiary_damage_percent: ",a->incendiary_damage_percent,"\r\n",
		        "explosive_damage_percent: ",a->explosive_damage_percent,"\r\n",
		        "shrapnel_damage_percent: ",a->shrapnel_damage_percent,"\r\n",
		        "corrosive_damage_percent: ",a->corrosive_damage_percent,"\r\n",
		        "cryogenic_damage_percent: ",a->cryogenic_damage_percent,"\r\n",
		        "radiation_damage_percent: ",a->radiation_damage_percent,"\r\n",
		        "emp_damage_percent: ",a->emp_damage_percent,"\r\n",
		        "shock_damage_percent: ",a->shock_damage_percent,"\r\n",
		        "anti_matter_damage_percent: ",a->anti_matter_damage_percent,"\r\n",
		        "recoil_reduction: ",a->recoil_reduction,"\r\n",
		        "free_ammo_chance: ",a->free_ammo_chance,"\r\n",
		        "regenerate_ammo_chance: ",a->regenerate_ammo_chance,"\r\n",
		        "bleed_chance: ",a->bleed_chance,"\r\n",
		        "aimed_limb_accuracy_percent: ",a->aimed_limb_accuracy_percent,"\r\n",
		        "underbarrel_launcher_type: ",a->underbarrel_launcher_type,"\r\n",
		        "damage_percent_bonus: ",a->damage_percent_bonus,"\r\n",
		        "thermal_range: ",a->thermal_range,"\r\n",
		        "night_vision_range: ",a->night_vision_range,"\r\n",
		        "str_type: ",a->str_type,"\r\n");
	}
	/**
	 * @brief
	 *
	 * @return same as yaml_map() except short_desc() is called on each attached object.
	 */
	std::map<std::string,std::string> rifle_attachments_t::friendly_map() {
		std::map<std::string,std::string> files;
		if(sight) {
			files["sight"] = short_desc(sight);
		}
		if(under_barrel) {
			files["under_barrel"] = short_desc(under_barrel);
		}
		if(grip) {
			files["grip"] = short_desc(grip);
		}
		if(barrel) {
			files["barrel"] = short_desc(barrel);
		}
		if(muzzle) {
			files["muzzle"] = short_desc(muzzle);
		}
		if(magazine) {
			files["magazine"] = short_desc(magazine);
		}
		if(stock) {
			files["stock"] = short_desc(stock);
		}
		if(strap) {
			files["strap"] = short_desc(strap);
		}
		return files;
	}

	/**
	 * @brief
	 *
	 * @return deep object string representing the currently attached objects for this rifle attachment
	 */
	rifle_attachments_t::encoding_t rifle_attachments_t::export_objects() {
		std::string s;
		s = m_rifle_yaml_file;
		auto files = yaml_map();
		if(files.size() == 0) {
			return s;
		}
		s += "{";
		uint8_t i = 0;
		for(auto& pair : files) {
			s += CAT(pair.first,":",pair.second);
			if(i + 1 == files.size()) {
				break;
			}
			s += ",";
			++i;
		}
		s += "}";
		if(m_rifle_level) {
			s += CAT("#level:",m_rifle_level);
		}
		return s;
	}
	bool rifle_attachments_t::is_attachment(obj_ptr_t& obj) {
		return obj->rifle()->attributes->is_rifle_attachment;
	}
	std::shared_ptr<rifle_attachments_t> rifle_attachments_t::first_or_create(obj_ptr_t& obj) {
		if(is_attachment(obj)) {
			return rifle_attachments::by_uuid(obj->uuid);
		}
		return rifle_attachments::make_from(obj);
	}
	/**
	 * @brief
	 *
	 * @return the string to be displayed when a player examines this rifle attachment. does not cache the string and generates a new string each time called. the string represents the currently attached objects.
	 */
	std::string rifle_attachments_t::examine() {
		std::string s;
		s = CAT("{grn}[rifle]:{/grn}{yel}",base_object->rifle()->attributes->manufacturer," ",base_object->rifle()->attributes->name.c_str(),"{/yel}\r\n");
		for(auto& pair : friendly_map()) {
			s += CAT("{grn}[",pair.first,"]:{/grn} {yel}",pair.second,"{/yel}\r\n");
		}
		return s;
	}
};
