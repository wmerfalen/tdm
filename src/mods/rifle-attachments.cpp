#include "rifle-attachments.hpp"
#include "interpreter.hpp"
#include "orm/rifle-attachment.hpp"
#include "query-objects.hpp"

#define __MENTOC_SHOW_MODS_RIFLE_ATTACHMENT_DEBUG_OUTPUT__
#ifdef __MENTOC_SHOW_MODS_RIFLE_ATTACHMENT_DEBUG_OUTPUT__
	#define m_debug(MSG) mentoc_prefix_debug("[mods::rifle_attachments]")  << MSG << "\n";
#else
	#define m_debug(MSG) ;;
#endif

extern void obj_to_char(obj_ptr_t  object, player_ptr_t player);
extern void obj_from_char(obj_ptr_t in_object);
namespace mods {
	const std::string example_grammar = "rifle/g36c.yml{sight:acog.yml,muzzle:compensator.yml,under_barrel:gm32grenadelauncher.yml}";
	using counter_t = uint16_t;
	namespace rifle_attachments {
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
		void erase(const uuid_t& uuid) {
			std::cerr << green_str("erasing uuid from rifle attachments:") << uuid << "\n";
			uuid_schema_list().erase(uuid);
			global_list().erase(uuid);
		}
		std::shared_ptr<mods::rifle_attachments_t> by_uuid(const uuid_t& uuid) {
			if(global_list().find(uuid) == global_list().end()) {
				return nullptr;
			}
			return global_list()[uuid];
		}
		std::map<uuid_t,std::string>& uuid_schema_list() {
			static std::map<uuid_t,std::string> list;
			return list;
		}
		std::map<uuid_t,std::shared_ptr<mods::rifle_attachments_t>>& global_list() {
			static std::map<uuid_t,std::shared_ptr<mods::rifle_attachments_t>> list;
			return list;
		}
		std::shared_ptr<mods::rifle_attachments_t> make(std::string_view str) {
			auto ptr = std::make_shared<mods::rifle_attachments_t>(str.data());
			global_list()[ptr->base_object->uuid] = ptr;
			uuid_schema_list()[ptr->base_object->uuid] = str;
			return ptr;
		}
		void update_global_list(std::shared_ptr<mods::rifle_attachments_t>& ptr) {
			global_list()[ptr->base_object->uuid] = ptr;
			uuid_schema_list()[ptr->base_object->uuid] = ptr->export_objects();
		}

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
		SUPERCMD(do_load_my_rifle_attachments) {
			player->sendln("Loading...");
			mods::orm::load_player_rifle_attachments(player);
			player->sendln("[+] Done");
		}
		ACMD(do_vcarrying) {
			player->sendln("Schema list...");
			for(const auto& pair : player->vcarrying()) {
				player->sendln(CAT("Item:",pair->name.c_str()));
			}
			player->sendln("[+] Done");
		}
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
	void rifle_attachments_t::set_slot(const slot_t& slot,obj_ptr_t& obj) {
		switch(slot) {
			case SLOT_SIGHT:
				sight = obj;
				break;
			case SLOT_UNDERBARREL:
				under_barrel = obj;
				break;
			case SLOT_GRIP:
				m_debug("grip set to: '" << obj->name.c_str() << "'");
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
	int16_t rifle_attachments_t::attach_from_inventory(player_ptr_t& player,std::string_view str,obj_ptr_t& attachment) {
		auto opt = from_string_to_slot(str);
		if(opt.has_value()) {
			const slot_t& slot = (slot_t)opt.value();

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
		return -5;
	}
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
	 * Wrapper function that ultimately calls the real detach_into_inventory
	 */
	int16_t rifle_attachments_t::detach_into_inventory(player_ptr_t& player,std::string_view str) {
		auto opt = from_string_to_slot(str);
		if(opt.has_value()) {
			const slot_t& slot = (slot_t)opt.value();
			return detach_into_inventory(player,slot);
		}
		return 0;
	}
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
	void rifle_attachments_t::update_description() {
		yaml_map();
		friendly_map();
		base_object->action_description = examine();
	}
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
	int16_t rifle_attachments_t::import_objects(const encoding_t& line) {
		auto map = m_parser.extract_line_items(line,mods::util::slot_names_for_type("rifle"));
		m_rifle_yaml_file = map["base"];
		m_debug("m_rifle_yaml_file: '" << m_rifle_yaml_file << "'");
		base_object = create_object(map["base"]);
		if(!base_object) {
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
					return INVALID_SIGHT;
				}
			}
			if(pair.first.compare("under_barrel") == 0) {
				object = under_barrel = create_object(ITEM_ATTACHMENT,pair.second);
				if(!under_barrel) {
					return INVALID_UNDER_BARREL;
				}
			}
			if(pair.first.compare("grip") == 0) {
				object = grip = create_object(ITEM_ATTACHMENT,pair.second);
				if(!grip) {
					return INVALID_GRIP;
				}
			}
			if(pair.first.compare("barrel") == 0) {
				object = barrel = create_object(ITEM_ATTACHMENT,pair.second);
				if(!barrel) {
					return INVALID_BARREL;
				}
			}
			if(pair.first.compare("muzzle") == 0) {
				object = muzzle = create_object(ITEM_ATTACHMENT,pair.second);
				if(!muzzle) {
					return INVALID_MUZZLE;
				}
			}
			if(pair.first.compare("magazine") == 0) {
				object = magazine = create_object(ITEM_ATTACHMENT,pair.second);
				if(!magazine) {
					return INVALID_MAGAZINE;
				}
			}
			if(pair.first.compare("stock") == 0) {
				object = stock = create_object(ITEM_ATTACHMENT,pair.second);
				if(!stock) {
					return INVALID_STOCK;
				}
			}
			if(pair.first.compare("strap") == 0) {
				object = strap = create_object(ITEM_ATTACHMENT,pair.second);
				if(!strap) {
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
		return IMPORT_OKAY;
	}
	std::string rifle_attachments_t::extract_base_yaml_file(const encoding_t& line) {
		auto map = m_parser.extract_line_items(line,mods::util::slot_names_for_type("rifle"));
		return map["base"];
	}
	std::string& rifle_attachments_t::base_yaml_file() {
		return m_rifle_yaml_file;
	}
	std::map<std::string,std::string> rifle_attachments_t::yaml_map() {
		std::map<std::string,std::string> files;
		if(sight) {
			files["sight"] = sight->feed_file();
		}
		if(under_barrel) {
			files["under_barrel"] = under_barrel->feed_file();
		}
		if(grip) {
			m_debug("setting grip");
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
	std::string short_desc(auto& in_a) {
		if(!in_a->has_attachment()) {
			std::cerr << red_str("item doesnt have attachment!") << "\n";
			return "";
		}
		auto& a = in_a->attachment()->attributes;
		return CAT(a->manufacturer," ",a->name);
	}
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
	std::string rifle_attachments_t::examine() {
		std::string s;
		s = CAT("{grn}[rifle]:{/grn}{yel}",base_object->rifle()->attributes->manufacturer," ",base_object->rifle()->attributes->name.c_str(),"{/yel}\r\n");
		for(auto& pair : friendly_map()) {
			s += CAT("{grn}[",pair.first,"]:{/grn} {yel}",pair.second,"{/yel}\r\n");
		}
		return s;
	}
};
