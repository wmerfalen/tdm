#include "hqbuild.hpp"
#include "../interpreter.hpp"
#include "../builder_util.hpp"
#include "../orm/hq.hpp"
#include "../affiliations.hpp"

namespace mods::builder::hqbuild {
	void clear() {
		mods::orm::hq_list().clear();
	}
	void load_all() {
		mods::orm::hq_list() = std::move(mods::orm::load_all_by_table<mods::orm::hq>());
	}

	bool delete_by_room_vnum(const room_vnum vnum) {
		bool deleted = false;
		auto& r = mods::orm::hq_list();
		r.erase(
		std::remove_if(r.begin(),r.end(),[&vnum,&deleted](auto& r) -> bool {
			if(r->hq_room_vnum == vnum) {
				r->destroy();
				deleted = true;
				return true;
			}
			return false;
		}),r.end());
		return deleted;
	}
	std::optional<std::shared_ptr<mods::orm::hq>> by_vnum(const uint32_t& vnum) {
		for(const auto& m : mods::orm::hq_list()) {
			if(m->hq_room_vnum == vnum) {
				return m;
			}
		}
		return std::nullopt;
	}


	std::string& affiliation_list() {
		static std::string list;
		if(list.length()) {
			return list;
		}
		for(const auto& pair : mods::affiliation_map()) {
			list += CAT("  {gld}|:: ",pair.first.data(),"{/gld}\r\n");
		}
		return list;
	}
	bool valid_affiliation(std::string_view aff) {
		for(const auto& pair : mods::affiliation_map()) {
			if(pair.first.compare(aff.data()) == 0) {
				return true;
			}

		}
		return false;
	}
	std::string& attribute_list() {
		static std::string list;
		if(list.length()) {
			return list;
		}
		for(const auto& item : {
		            "affiliation",
		            "basic_mob_count",
		            "advanced_mob_count",
		            "elite_mob_count",
		            "suv_count",
		            "sedan_count",
		            "armored_van_count",
		            "replenish_ticks",
		            "replenish_basic_count",
		            "replenish_advanced_count",
		            "replenish_elite_count",
		        }) {
			list += CAT("  {gld}|:: ",item,"{/gld}\r\n");
		}
		return list;
	}

	using args_t = std::vector<std::string>;

	SUPERCMD(do_hqbuild) {

		mods::builder::initialize_builder(player);
		auto vec_args = mods::util::arglist<std::vector<std::string>>(std::string(argument));
		if(vec_args.size() == 0 || vec_args[0].compare("help") == 0) {
			player->pager_start();
			*player << "usage: \r\n" <<
			        /** help */
			        " {grn}hqbuild{/grn} {red}help{/red}\r\n" <<
			        "  |--> this help menu\r\n" <<

			        /** new */
			        " {grn}hqbuild{/grn} {red}new <affiliation> <level>{/red}\r\n" <<
			        "  |--> create a hq in the current room.\r\n" <<
			        "  {gld}|:: -:[affiliations]:-{/gld}\r\n" <<
			        affiliation_list() <<

			        /** set */
			        " {grn}hqbuild{/grn} {red}set <attribute> <value>{/red}\r\n" <<
			        "  |--> set attribute to value.\r\n" <<
			        "  {gld}|:: -:[attributes]:-{/gld}\r\n" <<
			        attribute_list() <<

			        /** save */
			        " {grn}hqbuild{/grn} {red}save <room_vnum>...<room_vnumN>{/red}\r\n" <<
			        "  |--> save all hq's within the room_vnums listed.\r\n" <<


			        /** delete */
			        " {grn}hqbuild{/grn} {red}delete <room_vnum>...<room_vnumN>{/red}\r\n" <<
			        "  |--> remove all hq's within the room_vnums listed.\r\n" <<

			        /** list */
			        " {grn}hqbuild{/grn} {red}list [room_vnum]...[room_vnumN]{/red}\r\n" <<
			        "  |--> lists all active hq's optionally with room vnums.\r\n" <<

			        /** reload-all */
			        " {grn}hqbuild{/grn} {red}reload-all{/red}\r\n" <<
			        "  |--> clears all hqbuild data and reloads from sql. Does not save before loading\r\n"

			        "\r\n";
			player->pager_end();
			player->page(0);
			return;
		}
		/** reload-all */
		{
			auto args = mods::util::subcmd_args<11,args_t>(argument,"reload-all");
			if(args.has_value()) {
				ENCODE_INIT();
				clear();
				load_all();
				r_success(player,"Reloaded");
				ENCODE_R("ok");
				return;
			}
		}
		/** set */
		{
			auto args = mods::util::subcmd_args<4,args_t>(argument,"set");
			if(args.has_value()) {
				ENCODE_INIT();
				auto opt_hq = by_vnum(player->vnum());
				if(!opt_hq.has_value()) {
					r_error(player,"no hq in this room");
					return;
				}
				auto& hq = opt_hq.value();
				auto cmd_args = args.value();
				/**
				 * cmd_args will be: [0] => set, [1] => attribute [2] => value
				 */
				if(cmd_args.size() < 3) {
					r_error(player,"Error: not enough arguments");
					return;
				}
				const auto& attribute = cmd_args[1];
				if(attribute.compare("affiliation") == 0) {
					if(!valid_affiliation(cmd_args[2])) {
						ENCODE_R("invalid");
						r_error(player,"Invalid affiliation");
						return;
					}
					hq->hq_affiliation = cmd_args[2];
					r_success(player,"Value set.");
					ENCODE_OK();
					return;
				}
				auto opt_value = mods::util::stoi(cmd_args[2]);
				if(!opt_value.has_value()) {
					r_error(player,"value must be a number");
					return;
				}
				if(attribute.compare("basic_mob_count") == 0) {
					hq->hq_basic_mob_count = opt_value.value();
				}
				if(attribute.compare("advanced_mob_count") == 0) {
					hq->hq_advanced_mob_count = opt_value.value();
				}
				if(attribute.compare("elite_mob_count") == 0) {
					hq->hq_elite_mob_count = opt_value.value();
				}
				if(attribute.compare("suv_count") == 0) {
					hq->hq_suv_count = opt_value.value();
				}
				if(attribute.compare("sedan_count") == 0) {
					hq->hq_sedan_count = opt_value.value();
				}
				if(attribute.compare("armored_van_count") == 0) {
					hq->hq_armored_van_count = opt_value.value();
				}
				if(attribute.compare("replenish_ticks") == 0) {
					hq->hq_replenish_ticks = opt_value.value();
				}
				if(attribute.compare("replenish_basic_count") == 0) {
					hq->hq_replenish_basic_count = opt_value.value();
				}
				if(attribute.compare("replenish_advanced_count") == 0) {
					hq->hq_replenish_advanced_count = opt_value.value();
				}
				if(attribute.compare("replenish_elite_count") == 0) {
					hq->hq_replenish_elite_count = opt_value.value();
				}

				r_success(player,"Value set.");
				ENCODE_OK();
				return;
			}
		}
		/** new */
		{
			auto args = mods::util::subcmd_args<4,args_t>(argument,"new");
			if(args.has_value()) {
				ENCODE_INIT();
				auto cmd_args = args.value();
				/**
				 * cmd_args will be: [0] => new, [1] => affiliation [2] => level
				 */
				if(cmd_args.size() < 3) {
					r_error(player,"Error: not enough arguments");
					return;
				}
				auto affiliation = cmd_args[1];
				auto level = mods::util::stoi(cmd_args[2]).value_or(-1);
				if(level < 0) {
					r_error(player,"level must be a positive number");
					return;
				}
				mods::orm::hq hq;
				auto id = hq.initialize_row(affiliation,player->vnum(),level);
				if(id <= 0) {
					r_error(player,"Couldn't initialize row");
					return;
				}
				r_success(player,"Created row.");
				ENCODE_STR(id);
				return;
			}
		}

		MENTOC_LIST_EXTRACT();
		MENTOC_PAGINATED_LIST(mods::orm::hq_list());
		/** list */
		{
			auto args = mods::util::subcmd_args<5,args_t>(argument,"list");
			if(args.has_value()) {
				ENCODE_INIT();
				auto cmd_args = args.value();
				std::string  encoded_response = "";
				if(cmd_args.size() == 0) {
					for(const auto& profile : mods::orm::hq_list()) {
						r_success(player, profile->dump());
						encoded_response += profile->encode() + "\r\n";
					}
					ENCODE_R(encoded_response);
					return;
				}
				for(unsigned i = 1; i < cmd_args.size(); ++i) {
					auto vnum = mods::util::stoi(cmd_args[i]).value_or(-1);
					if(vnum <= 0) {
						r_error(player,"vnum must be a positive number");
						continue;
					}
					auto opt_hq = by_vnum(vnum);
					if(!opt_hq.has_value()) {
						r_error(player,CAT("no hq in room_vnum : '",vnum,"'"));
						continue;
					}
					auto& profile = opt_hq.value();
					r_success(player, profile->dump());
					encoded_response += profile->encode() + "\r\n";
				}
				ENCODE_R(encoded_response);
				return;
			}
		}


		/** save */
		{
			auto args = mods::util::subcmd_args<5,args_t>(argument,"save");
			if(args.has_value()) {
				ENCODE_INIT();
				auto cmd_args = args.value();
				if(cmd_args.size() < 2) {
					r_error(player,"Error: not enough arguments");
					return;
				}
				std::string saved = "";
				for(unsigned i = 1; i < cmd_args.size(); ++i) {
					auto vnum = mods::util::stoi(cmd_args[i]).value_or(-1);
					if(vnum <= 0) {
						r_error(player,"vnum must be a positive number");
						continue;
					}
					auto profile = by_vnum(vnum);
					if(!profile.has_value()) {
						r_error(player,CAT("no hq in room_vnum : '",vnum,"'"));
						continue;
					}
					saved += std::to_string(profile.value()->save()) + "|";
				}
				ENCODE_R(saved);
				return;
			}
		}


		/** delete */
		{
			auto args = mods::util::subcmd_args<7,args_t>(argument,"delete");
			if(args.has_value()) {
				ENCODE_INIT();
				auto cmd_args = args.value();
				if(cmd_args.size() < 2) {
					r_error(player,"Not enough arguments");
					return;
				}
				/**
				 * cmd_args will be: [0] => delete, [1] => <vnum>, ... [N] => <vnumN>
				 */
				std::string deleted = "";
				for(unsigned i = 1; i < cmd_args.size(); ++i) {
					auto vnum = mods::util::stoi(cmd_args[i]).value_or(-1);
					if(vnum < 0) {
						r_error(player,"vnum must be a positive number");
						continue;
					}
					if(!delete_by_room_vnum(vnum)) {
						r_error(player,"No mob equipment profile exists with that vnum");
						continue;
					}
					deleted += CAT(vnum,"|");
				}
				ENCODE_R(deleted);
				r_success(player,"Profile(s) deleted");
				return;
			}
		}
	}	//end hqbuild
	void init() {
		mods::interpreter::add_command("hqbuild", POS_RESTING, do_hqbuild, LVL_BUILDER,0);
		clear();
		load_all();
	}
};
