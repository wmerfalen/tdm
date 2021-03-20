#include "rifle-attachments.hpp"
#include "interpreter.hpp"
#include "orm/rifle-attachment.hpp"

extern void obj_to_char(obj_ptr_t  object, player_ptr_t player);
extern void obj_from_char(obj_ptr_t in_object);
namespace mods {
	const std::string example_grammar = "g36c.yml{sight:acog.yml,muzzle:compensator.yml,under_barrel:gm32grenadelauncher.yml}";
	using counter_t = uint16_t;
	namespace rifle_attachments {
		std::map<uuid_t,std::string>& uuid_schema_list() {
			static std::map<uuid_t,std::string> list;
			return list;
		}

		ACMD(do_instantiate_rifle_attachment) {
			ADMIN_REJECT();
			DO_HELP("instantiate_rifle_attachment");
			auto vec_args = PARSE_ARGS();
			if(vec_args.size() == 0) {
				player->errorln("not enough arguments");
				return;
			}
			for(const auto& str : vec_args) {
				player->rifle_attachments().emplace_back(std::make_shared<mods::rifle_attachments_t>(str));
				player->carry(player->rifle_attachments().back()->base_object);
				player->sendln("[+] Done");
			}
			ADMIN_DONE();
		}
		ACMD(do_load_my_rifle_attachments) {
			player->sendln("Loading...");
			mods::orm::load_player_rifle_attachments(player);
			player->sendln("[+] Done");
		}
		void init() {
			mods::interpreter::add_command("instantiate_rifle_attachment", POS_RESTING, do_instantiate_rifle_attachment, LVL_BUILDER,0);
			mods::interpreter::add_command("load_my_rifle_attachments", POS_RESTING, do_load_my_rifle_attachments, 0,0);
		}
	};

	void rifle_attachments_t::import_objects(const encoding_t& line) {
		auto map = m_parser.extract_line_items(line,mods::util::slot_names_for_type("rifle"));
		m_rifle_yaml_file = map["base"];
		base_object = create_object(ITEM_RIFLE,map["base"]);
		base_object->rifle()->attributes->is_rifle_attachment = true;
		for(auto& pair : map) {
			if(pair.first.compare("sight") == 0) {
				sight = create_object(ITEM_ATTACHMENT,pair.second);
			}
			if(pair.first.compare("under_barrel") == 0) {
				under_barrel = create_object(ITEM_ATTACHMENT,pair.second);
			}
			if(pair.first.compare("grip") == 0) {
				grip = create_object(ITEM_ATTACHMENT,pair.second);
			}
			if(pair.first.compare("barrel") == 0) {
				barrel = create_object(ITEM_ATTACHMENT,pair.second);
			}
			if(pair.first.compare("muzzle") == 0) {
				muzzle = create_object(ITEM_ATTACHMENT,pair.second);
			}
			if(pair.first.compare("magazine") == 0) {
				magazine = create_object(ITEM_ATTACHMENT,pair.second);
			}
			if(pair.first.compare("stock") == 0) {
				stock = create_object(ITEM_ATTACHMENT,pair.second);
			}
			if(pair.first.compare("strap") == 0) {
				strap = create_object(ITEM_ATTACHMENT,pair.second);
			}
		}
		mods::rifle_attachments::uuid_schema_list()[base_object->uuid] = line;
		base_object->action_description = this->examine();
		std::cerr << green_str("examine action desc: '") << base_object->action_description.c_str() << "'\n";
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
		return s;
	}
	std::string rifle_attachments_t::examine() {
		std::string s;
		s = CAT("{grn}[rifle]:{/grn}{yel}",base_object->rifle()->attributes->manufacturer," ",base_object->rifle()->attributes->name,"{/yel}\r\n");
		for(auto& pair : friendly_map()) {
			s += CAT("{grn}[",pair.first,"]:{/grn} {yel}",pair.second,"{/yel}\r\n");
		}
		return s;
	}
};
