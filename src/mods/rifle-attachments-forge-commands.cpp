#include "rifle-attachments.hpp"
#include "interpreter.hpp"
#include "orm/rifle-attachment.hpp"
#include "query-objects.hpp"

#define __MENTOC_SHOW_MODS_RIFLE_ATTACHMENT_FORGE_COMMANDS_DEBUG_OUTPUT__
#ifdef __MENTOC_SHOW_MODS_RIFLE_ATTACHMENT_FORGE_COMMANDS_DEBUG_OUTPUT__
	#define m_debug(MSG) mentoc_prefix_debug("[mods::rifle_attachments]")  << MSG << "\n";
#else
	#define m_debug(MSG) ;;
#endif

namespace mods::rifle_attachments::forge_commands {
	ACMD(do_forge_colon_add) {
		static constexpr std::string_view usage = "Usage: forge:add <attachment> <weapon>\r\n"
		    "Please note: weapons must be in your inventory.\r\n";
		const auto vec_args = PARSE_ARGS();
		if(argshave()->size_gt(1)->passed() == false) {
			player->sendln("Not enough arguments");
			player->sendln(usage);
			return;
		}
		auto aopt =  mods::query_objects::inv::query_for_single_object(player,vec_args[0]);
		auto wopt =  mods::query_objects::inv::query_for_single_object(player,vec_args[1]);
		if(!aopt.has_value()) {
			player->sendln("Couldn't find attachment in your inventory!");
			return;
		}
		if(!wopt.has_value()) {
			player->sendln("Couldn't find weapon in your inventory!");
			return;
		}
		auto a = optr_by_uuid(aopt.value());
		auto w = optr_by_uuid(wopt.value());
		if(!a) {
			player->sendln("Attachment is invalid!");
			return;
		}
		if(!w) {
			player->sendln("Weapon is invalid!");
			return;
		}
		if(w->has_rifle() == false) {
			player->sendln(CAT(w->name.c_str()," is not a rifle!"));
			return;
		}
		auto list = by_player(player);
		if(list.size() == 0) {
			player->sendln("You don't have any weapons in your inventory that can be modified");
			return;
		}
		for(auto& rifle : list) {
			if(rifle->base_object && rifle->base_object->uuid == w->uuid) {
				m_debug(CAT("Found base_object match: '",rifle->base_object->name.c_str(),"' uuid:'",w->uuid,"'"));
				m_debug(CAT("attachment->type:'", a->attachment()->type,"', attr-type:'",a->attachment()->attributes->type,"'"));
				m_debug(CAT("attachment->str_type:'", a->attachment()->attributes->str_type,"'"));
				auto status = rifle->attach_from_inventory(player,a->attachment()->attributes->str_type,a);
				update_global_list(rifle);
				switch(status) {
					case -1:
						player->sendln("Invalid attachment object");
						return;
					case -2:
						player->sendln("Object is not an attachment");
						return;
					case 0:
						player->sendln(CAT("You successfully modify ",rifle->base_object->name.c_str()));
						return;
					default:
						player->sendln(CAT("You couldn't attach anything to a ",rifle->base_object->name.c_str()));
						return;
				}
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
		auto aopt =  mods::query_objects::inv::query_for_single_object(player,vec_args[0]);
		auto wopt =  mods::query_objects::inv::query_for_single_object(player,vec_args[1]);
		if(!aopt.has_value()) {
			player->sendln("Couldn't find attachment in your inventory!");
			return;
		}
		if(!wopt.has_value()) {
			player->sendln("Couldn't find weapon in your inventory!");
			return;
		}
		auto a = optr_by_uuid(aopt.value());
		auto w = optr_by_uuid(wopt.value());
		if(!a) {
			player->sendln("Attachment is invalid!");
			return;
		}
		if(!w) {
			player->sendln("Weapon is invalid!");
			return;
		}
		auto list = by_player(player);
		if(list.size() == 0) {
			player->sendln("You don't have any weapons in your inventory that can be modified");
			return;
		}
		for(auto& rifle : list) {
			if(rifle->base_object && rifle->base_object->uuid == w->uuid) {
				m_debug(CAT("Found base_object match: '",rifle->base_object->name.c_str(),"' uuid:'",w->uuid,"'"));
				m_debug(CAT("attachment->type:'", a->attachment()->type,"', attr-type:'",a->attachment()->attributes->type,"'"));
				m_debug(CAT("attachment->str_type:'", a->attachment()->attributes->str_type,"'"));
				auto status = rifle->detach_into_inventory(player,a->attachment()->attributes->str_type);
				if(status) {
					update_global_list(rifle);
				} else {
					player->sendln("It seems that slot is empty already.");
				}
				return;
			}
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
		mods::interpreter::add_user_command("forge:add", do_forge_colon_add);
		mods::interpreter::add_user_command("forge:remove", do_forge_colon_remove);
		mods::interpreter::add_user_command("forge:strip", do_forge_colon_strip);
	}
};
