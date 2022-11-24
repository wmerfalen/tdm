#include "rifle-attachments.hpp"
#include "interpreter.hpp"
#include "orm/rifle-attachment.hpp"
#include "query-objects.hpp"

#define __MENTOC_SHOW_MODS_RIFLE_ATTACHMENT_DEBUG_OUTPUT__
#ifdef __MENTOC_SHOW_MODS_RIFLE_ATTACHMENT_DEBUG_OUTPUT__
	#define m_debug(MSG) mentoc_prefix_debug("[mods::orm::admin::rifle_attribute_limits]")  << MSG << "\n";
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
					if(std::find(uuid_list.begin(),uuid_list.end(),item->uuid) == uuid_list.end()) {
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
					if(std::find(uuid_list.begin(),uuid_list.end(),item->uuid) == uuid_list.end()) {
						list.emplace_back(ptr);
						uuid_list.emplace_back(item->uuid);
					}
				}
			}
			return list;
		}
		void erase(const uuid_t& uuid) {
#ifdef __MENTOC_SHOW_RIFLE_DEBUG__
			std::cerr << green_str("erasing uuid from rifle attachments:") << uuid << "\n";
#endif
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
		ACMD(do_forge_colon_add) {
			static constexpr std::string_view usage = "Usage: forge:add <attachment> <weapon>\r\n"
			    "Please note: weapons must be in your inventory.\r\n";
			const auto vec_args = PARSE_ARGS();
			if(argshave()->size_gt(1)->passed() == false) {
				player->sendln("Not enough arguments");
				player->sendln(usage);
				return;
			}
			std::vector<uuid_t> attachments =  mods::query_objects::query_inventory_for_object(player,vec_args[0]);
			std::vector<uuid_t> weapons =  mods::query_objects::query_inventory_for_object(player,vec_args[1]);
			if(attachments.size() == 0) {
				player->sendln("Couldn't find attachment in your inventory!");
				return;
			}
			if(weapons.size() == 0) {
				player->sendln("Couldn't find weapon in your inventory!");
				return;
			}
			auto a = optr_by_uuid(attachments[0]);
			auto w = optr_by_uuid(weapons[0]);
			if(!a) {
				player->sendln("Attachment is invalid!");
				return;
			}
			if(!w) {
				player->sendln("Weapon is invalid!");
				return;
			}
			if(a->has_attachment() == false) {
				player->sendln(CAT(a->name.c_str()," cannot be attached to a weapon!"));
				return;
			}
			if(w->has_rifle() == false) {
				/**
				 * FIXME: we should allow for attachments to melee weapons
				 */
				player->sendln(CAT(w->name.c_str()," is not a rifle!"));
				return;
			}
			auto list = by_player(player);
			if(list.size() == 0) {
				player->sendln("You don't have any weapons in your inventory that can be modified");
				return;
			}
			for(const auto& rifle : list) {
				if(rifle->base_object && rifle->base_object->uuid == w->uuid) {
					player->sendln(CAT("Found base_object match: '",rifle->base_object->name.c_str(),"' uuid:'",w->uuid,"'"));
					rifle->attach_from_inventory(player,a->attachment()->type,a);
					return;
				}
			}
			player->sendln("You could not modify any weapon in your inventory.");
		}
		ACMD(do_forge_colon_remove) {
			static constexpr std::string_view usage = "Usage: forge:remove <attachment> <weapon>\r\n"
			    "Please note: weapons must be in your inventory.\r\n";
			const auto vec_args = PARSE_ARGS();
			if(argshave()->size_gt(1)->passed() == false) {
				player->sendln("Not enough arguments");
				player->sendln(usage);
				return;
			}
			std::vector<uuid_t> attachments =  mods::query_objects::query_inventory_for_object(player,vec_args[0]);
			std::vector<uuid_t> weapons =  mods::query_objects::query_inventory_for_object(player,vec_args[1]);
			if(attachments.size() == 0) {
				player->sendln("Couldn't find attachment in your inventory!");
				return;
			}
			if(weapons.size() == 0) {
				player->sendln("Couldn't find weapon in your inventory!");
				return;
			}
			auto a = optr_by_uuid(attachments[0]);
			auto w = optr_by_uuid(weapons[0]);
			if(!a) {
				player->sendln("Attachment is invalid!");
				return;
			}
			if(!w) {
				player->sendln("Weapon is invalid!");
				return;
			}

		}
		ACMD(do_forge_colon_strip) {
			static constexpr std::string_view usage = "Usage: forge:strip <weapon>\r\n"
			    "Please note: weapons must be in your inventory.\r\n";
			const auto vec_args = PARSE_ARGS();
			if(argshave()->size_gt(0)->passed() == false) {
				player->sendln("Not enough arguments");
				player->sendln(usage);
				return;
			}

			std::vector<uuid_t> weapons =  mods::query_objects::query_inventory_for_object(player,vec_args[0]);
			if(weapons.size() == 0) {
				player->sendln("Couldn't find weapon in your inventory!");
				return;
			}
			auto obj = optr_by_uuid(weapons[0]);
			if(!obj) {
				player->sendln("Object is invalid!");
				return;
			}
			player->sendln(CAT("You begin field stripping your {grn}",obj->name.c_str(),"{/grn}..."));
			/** TODO: remove all attachments from weapon and place them in inventory */
			player->sendln(CAT("You finish field stripping your {grn}",obj->name.c_str(),"{/grn}."));
		}
		void init() {
			mods::interpreter::add_user_command("vcarrying", do_vcarrying);
			mods::interpreter::add_user_command("load_my_rifle_attachments",  do_load_my_rifle_attachments);
			mods::interpreter::add_user_command("rifle-attachments:load",  do_load_my_rifle_attachments);

			mods::interpreter::add_builder_command("instantiate_rifle_attachment", do_instantiate_rifle_attachment);
			mods::interpreter::add_builder_command("list_rifle_attachments",  do_list_rifle_attachments);

			mods::interpreter::add_builder_command("rifle-attachments:instantiate", do_instantiate_rifle_attachment);
			mods::interpreter::add_builder_command("rifle-attachments:list",  do_list_rifle_attachments);

			mods::interpreter::add_user_command("forge:add", do_forge_colon_add);
			mods::interpreter::add_user_command("forge:remove", do_forge_colon_remove);
			mods::interpreter::add_user_command("forge:strip", do_forge_colon_strip);
		}
	};

	/**
	 * Returns zero if no item is in the slot. Returns 1 if item removed from slot and put in inventory
	 */
	int16_t rifle_attachments_t::detach_into_inventory(player_ptr_t& player,slot_t slot) {
		obj_ptr_t obj = nullptr;
		switch(slot) {
			case SLOT_SIGHT:
				if(!sight) {
					return 0;
				}
				obj = sight;
				sight = nullptr;
				break;
			case SLOT_UNDERBARREL:
				if(!under_barrel) {
					return 0;
				}
				obj = under_barrel;
				under_barrel = nullptr;
				break;
			case SLOT_GRIP:
				if(!grip) {
					return 0;
				}
				obj = grip;
				grip = nullptr;
				break;
			case SLOT_BARREL:
				if(!barrel) {
					return 0;
				}
				obj = barrel;
				barrel = nullptr;
				break;
			case SLOT_MUZZLE:
				if(!muzzle) {
					return 0;
				}
				obj = muzzle;
				muzzle = nullptr;
				break;
			case SLOT_MAGAZINE:
				if(!magazine) {
					return 0;
				}
				obj = magazine;
				magazine = nullptr;
				break;
			case SLOT_STOCK:
				if(!stock) {
					return 0;
				}
				obj = stock;
				stock = nullptr;
				break;
			case SLOT_STRAP:
				if(!strap) {
					return 0;
				}
				obj = strap;
				strap = nullptr;
			default:
				obj = nullptr;
				break;
		}
		if(obj) {
			player->carry(obj);
			remove_stats(obj);
			return 1;
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
		base_object->action_description = this->examine();
	}
	obj_ptr_t rifle_attachments_t::get_slot(slot_t slot) {
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
				return nullptr;
		}
	}
	obj_ptr_t rifle_attachments_t::set_slot(slot_t slot,obj_ptr_t& obj) {
		switch(slot) {
			case SLOT_SIGHT:
				return sight = obj;
			case SLOT_UNDERBARREL:
				return under_barrel = obj;
			case SLOT_GRIP:
				return grip = obj;
			case SLOT_BARREL:
				return barrel = obj;
			case SLOT_MUZZLE:
				return muzzle = obj;
			case SLOT_MAGAZINE:
				return magazine = obj;
			case SLOT_STOCK:
				return stock = obj;
			case SLOT_STRAP:
				return strap = obj;
			default:
				return nullptr;
		}
	}
	int16_t rifle_attachments_t::attach_from_inventory(player_ptr_t& player,slot_t slot,obj_ptr_t& attachment) {

		if(!attachment) {
			return -1;
		}
		if(!attachment->has_attachment()) {
			return -2;
		}
		obj_ptr_t existing = get_slot(slot);
		if(existing) {
			if(detach_into_inventory(player,slot)) {
				player->sendln(CAT("You remove {grn}",existing->name.c_str(),"{/grn} from ",base_object->name.c_str()));
				player->sendln(CAT("You carry {grn}",existing->name.c_str(),"{/grn}"));
			}
		}
		player->sendx(CAT("You grab ",attachment->name.c_str()).c_str());
		player->uncarry(attachment);
		player->sendln(CAT(" and attach it to",base_object->name.c_str(),"..."));
		add_stats(attachment);
		return 0;
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
#ifdef __MENTOC_ATTACHMENT_DEBUG__
			std::cerr << red_str("item doesnt have attachment!") << "\n";
#endif
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
