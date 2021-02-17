#include "meqbuild.hpp"
#include "../interpreter.hpp"
#include "../util.hpp"
#include "../builder_util.hpp"
#include <map>
#include "../orm/mob-equipment.hpp"
#include "../orm/mob-equipment-map.hpp"
#include "../orm/base.hpp"
#include "../mob-equipment.hpp"

namespace mods::builder::meqbuild {
	static constexpr int MAX_MOB_EQUIPMENT_LIST = 1500;
	static constexpr int MEQBUILD_MAX_ITEMS_LISTED = 150;
	void clear() {
		mods::orm::mob_equipment_list().clear();
		mods::orm::mob_equipment_map_list().clear();
		mods::mob_equipment::refresh_mappings();
	}
	void load_all() {
		mods::orm::mob_equipment_list() = std::move(mods::orm::load_all_by_table<mods::orm::mob_equipment>());
		mods::orm::mob_equipment_map_list() = std::move(mods::orm::load_all_by_table<mods::orm::mob_equipment_map>());
		mods::mob_equipment::refresh_mappings();
	}

	namespace map {
		bool delete_by_vnum(const uint64_t& vnum) {
			std::deque<std::shared_ptr<mods::orm::mob_equipment_map>> list;
			bool deleted = false;
			for(auto& m : mods::orm::mob_equipment_map_list()) {
				if(m->mmap_mob_vnum == vnum) {
					m->destroy();
					deleted = true;
					continue;
				}
				list.emplace_back(std::move(m));
			}
			mods::orm::mob_equipment_map_list() = std::move(list);
			return deleted;
		}
		std::vector<std::shared_ptr<mods::orm::mob_equipment_map>> by_mob_vnum(const uint64_t& vnum) {
			std::vector<std::shared_ptr<mods::orm::mob_equipment_map>> list;
			for(const auto& m : mods::orm::mob_equipment_map_list()) {
				if(m->mmap_mob_vnum == vnum) {
					list.emplace_back(m);
				}
			}
			return list;
		}
		std::vector<std::shared_ptr<mods::orm::mob_equipment_map>> by_profile_vnum(const uint64_t& vnum) {
			std::vector<std::shared_ptr<mods::orm::mob_equipment_map>> list;
			for(const auto& m : mods::orm::mob_equipment_map_list()) {
				if(m->mmap_mob_equipment_vnum == vnum) {
					list.emplace_back(m);
				}
			}
			return list;
		}
	};
	bool delete_by_vnum(const uint64_t& vnum) {
		std::deque<std::shared_ptr<mods::orm::mob_equipment>> list;
		bool deleted = false;
		for(auto& m : mods::orm::mob_equipment_list()) {
			if(m->meq_vnum == vnum) {
				m->destroy();
				deleted = true;
				continue;
			}
			list.emplace_back(std::move(m));
		}
		mods::orm::mob_equipment_list() = std::move(list);
		return deleted;
	}
	std::optional<std::shared_ptr<mods::orm::mob_equipment>> by_vnum(const uint64_t& vnum) {
		for(const auto& m : mods::orm::mob_equipment_list()) {
			if(m->meq_vnum == vnum) {
				return m;
			}
		}
		return std::nullopt;
	}
	std::optional<std::shared_ptr<mods::orm::mob_equipment>> by_profile(std::string_view name) {
		for(const auto& m : mods::orm::mob_equipment_list()) {
			if(m->meq_profile_name.compare(name) == 0) {
				return m;
			}
		}
		return std::nullopt;
	}


	std::map<std::string,uint8_t> wear_flags_map = {
		{"light",WEAR_LIGHT},
		{"finger_r",WEAR_FINGER_R},
		{"finger_l",WEAR_FINGER_L},
		{"neck_1",WEAR_NECK_1},
		{"neck_2",WEAR_NECK_2},
		{"body",WEAR_BODY},
		{"head",WEAR_HEAD},
		{"legs",WEAR_LEGS},
		{"feet",WEAR_FEET},
		{"hands",WEAR_HANDS},
		{"arms",WEAR_ARMS},
		{"shield",WEAR_SHIELD},
		{"about",WEAR_ABOUT},
		{"waist",WEAR_WAIST},
		{"wrist_r",WEAR_WRIST_R},
		{"wrist_l",WEAR_WRIST_L},
		{"wield",WEAR_WIELD},
		{"primary",WEAR_PRIMARY},
		{"hold",WEAR_HOLD},
		{"secondary",WEAR_SECONDARY},
		{"shoulders_l",WEAR_SHOULDERS_L},
		{"shoulders_r",WEAR_SHOULDERS_R},
		{"backpack",WEAR_BACKPACK},
		{"goggles",WEAR_GOGGLES},
		{"vest_pack",WEAR_VEST_PACK},
		{"elbow_l",WEAR_ELBOW_L},
		{"elbow_r",WEAR_ELBOW_R}
	};
	std::string& wear_flag_list() {
		static std::string list;
		if(list.length()) {
			return list;
		}
		for(const auto& pair : wear_flags_map) {
			list += CAT("  {gld}|:: ",pair.first,"{/gld}\r\n");
		}
		return list;
	}

	using args_t = std::vector<std::string>;

	ACMD(do_meqbuild) {

		mods::builder::initialize_builder(player);
		auto vec_args = mods::util::arglist<std::vector<std::string>>(std::string(argument));
		if(vec_args.size() == 0 || vec_args[0].compare("help") == 0) {
			player->pager_start();
			*player << "usage: \r\n" <<
			        /** help */
			        " {grn}meqbuild{/grn} {red}help{/red}\r\n" <<
			        "  |--> this help menu\r\n" <<

			        /** new */
			        " {grn}meqbuild{/grn} {red}new <name> <virtual_number>{/red}\r\n" <<
			        "  |--> create a new mob equipment profile named 'name'\r\n" <<

			        /** save */
			        " {grn}meqbuild{/grn} {red}save <virtual_number>...<virtual_numberN>{/red}\r\n" <<
			        "  |--> create a new mob equipment profile named 'name'\r\n" <<

			        /** set */
			        " {grn}meqbuild{/grn} {red}set <virtual_number> <position> <yaml>{/red}\r\n" <<
			        "  |--> set the slot for the profile to 'yaml'\r\n" <<
			        "  {grn}|____[example]{/grn}\r\n" <<
			        "  |:: {wht}meqbuild{/wht} {grn}set 3 wield rifle/g36c.yml{/grn}\r\n" <<
			        "  |--> sets profile 3 to have a g36c on the wield slot\r\n" <<

			        "  {gld}|:: -:[positions]:-{/gld}\r\n" <<
			        wear_flag_list() <<

			        /** remove */
			        " {grn}meqbuild{/grn} {red}remove <virtual_number> <position>{/red}\r\n" <<
			        "  |--> removes the slot at 'position' on profile 'virtual_number'\r\n" <<
			        "  {grn}|____[example]{/grn}\r\n" <<
			        "  |:: {wht}meqbuild{/wht} {grn}remove 3 wield{/grn}\r\n" <<
			        "  |--> clears the wield slot for profile 3\r\n" <<

			        paginate_option("meqbuild","mob equipment profiles") <<
			        list_extract_option("meqbuild","mob equipment profiles") <<

			        /** delete */
			        " {grn}meqbuild{/grn} {red}delete <virtual_number>...<virtual_numberN>{/red}\r\n" <<
			        "  |--> deletes the profile\r\n" <<
			        "  {grn}|____[example]{/grn}\r\n" <<
			        "  |:: {wht}meqbuild{/wht} {grn}delete 3{/grn}\r\n" <<

			        /** list */
			        " {grn}meqbuild{/grn} {red}list [virtual_number]...[virtual_numberN]{/red}\r\n" <<
			        "  |--> lists all mob equipment profiles optionally with virtual numbers\r\n" <<

			        /** show */
			        " {grn}meqbuild{/grn} {red}show <virtual_number>{/red}\r\n" <<
			        "  |--> show a specific mob equipment profile\r\n" <<


			        /** map-assign */
			        " {grn}meqbuild{/grn} {red}map-assign <mob_vnum> <meq_vnum>{/red}\r\n" <<
			        "  |--> assigns the specific mob to have the equipment profile of 'meq_vnum'\r\n" <<
			        "  {grn}|____[example]{/grn}\r\n" <<
			        "  |:: {wht}meqbuild{/wht} {grn}map-assign 402 3{/grn}\r\n" <<

			        /** map-list */
			        " {grn}meqbuild{/grn} {red}map-list{/red}\r\n" <<
			        "  |--> lists the specific mob to equipment profiles\r\n" <<

			        /** map-delete */
			        " {grn}meqbuild{/grn} {red}map-delete <mob_vnum>...<mob-vnumN>{/red}\r\n" <<
			        "  |--> deletes the map-assign'd mob to meq vnum mapping \r\n" <<
			        "  |--> A mob without a meq vnum will not spawn with equipment\r\n" <<
			        "  {grn}|____[example]{/grn}\r\n" <<
			        "  |:: {wht}meqbuild{/wht} {grn}map-delete 402 401 403{/grn}\r\n" <<
			        "  |--> deletes all map-assign'd mob to meq vnum mappings specified by the list of mob vnums passed in\r\n" <<


			        /** reload-all */
			        " {grn}meqbuild{/grn} {red}reload-all{/red}\r\n" <<
			        "  |--> clears all meqbuild data and reloads from sql. Does not save before loading\r\n"

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
				mods::builder::meqbuild::clear();
				mods::builder::meqbuild::load_all();
				r_success(player,"Reloaded");
				ENCODE_R("ok");
				return;
			}
		}
		/** new */
		{
			auto args = mods::util::subcmd_args<4,args_t>(argument,"new");
			if(args.has_value()) {
				ENCODE_INIT();
				auto cmd_args = args.value();
				if(cmd_args.size() < 3) {
					r_error(player,"Error: not enough arguments");
					return;
				}
				auto profile_name = cmd_args[1];
				auto vnum = mods::util::stoi(cmd_args[2]).value_or(-1);
				if(vnum < 0) {
					r_error(player,"vnum must be a positive number");
					return;
				}
				mods::orm::mob_equipment meq;
				auto id = meq.initialize_row(profile_name,vnum);
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

		MENTOC_PAGINATED_LIST(mods::orm::mob_equipment_list());
		{
			auto args = mods::util::subcmd_args<5,args_t>(argument,"show");
			if(args.has_value()) {
				ENCODE_INIT();
				auto cmd_args = args.value();
				std::string encoded_response;
				if(cmd_args.size() < 2) {
					r_error(player,"atleast 1 vnum is needed");
					return;
				}
				auto opt = mods::util::stoi(cmd_args[1]).value_or(-1);
				if(opt < 0) {
					r_error(player,"vnum must be positive");
					return;
				}
				auto profile = by_vnum(opt);
				if(profile.has_value()) {
					r_success(player,profile.value()->dump());
					ENCODE_R(profile.value()->encode());
					return;
				}
				r_error(player,"no profile for that vnum");
				return;
			}
		}

		/** list */
		{
			auto args = mods::util::subcmd_args<5,args_t>(argument,"list");
			if(args.has_value()) {
				ENCODE_INIT();
				auto cmd_args = args.value();
				std::string encoded_response;
				if(cmd_args.size() > 1) {
					for(auto i = 1; i < cmd_args.size(); i++) {
						auto opt = mods::util::stoi(cmd_args[i]).value_or(-1);
						if(opt < 0) {
							r_error(player,CAT("vnum must be positive for argument number ",i,": '",cmd_args[i],"'"));
							continue;
						}
						auto profile = by_vnum(opt);
						if(profile.has_value()) {
							r_success(player,profile.value()->dump());
							encoded_response += profile.value()->encode();
						}
					}
					ENCODE_R(encoded_response);
					return;
				}
				int max_items = mods::orm::mob_equipment_list().size();
				if(mods::orm::mob_equipment_list().size() > MAX_MOB_EQUIPMENT_LIST) {
					r_error(player,CAT("WARNING: meqbuild data is very large. use paginate to see specific pages. listing the first ",MEQBUILD_MAX_ITEMS_LISTED));
					max_items = MEQBUILD_MAX_ITEMS_LISTED;
				}

				int ctr = 0;
				for(const auto& profile : mods::orm::mob_equipment_list()) {
					r_success(player,profile->dump());
					encoded_response += profile->encode();
					if(++ctr >= max_items) {
						break;
					}
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
						r_error(player,CAT("no profile associated with: '",vnum,"'"));
						continue;
					}
					saved += std::to_string(profile.value()->save()) + "|";
				}
				ENCODE_R(saved);
				return;
			}
		}


		/** set */
		{
			auto args = mods::util::subcmd_args<4,args_t>(argument,"set");

			if(args.has_value()) {
				ENCODE_INIT();
				auto cmd_args = args.value();
				if(cmd_args.size() < 4) {
					r_error(player,"Not enough arguments");
					return;
				}
				/**
				 * cmd_args will be: [0] => set, [1] => <vnum> [2] => <position> [3] => yaml
				 */
				auto vnum = mods::util::stoi(cmd_args[1]).value_or(-1);
				auto position = cmd_args[2];
				auto yaml = cmd_args[3];
				if(vnum <= 0) {
					r_error(player,"vnum must be a positive number");
					return;
				}
				auto item = by_vnum(vnum);
				if(!item.has_value()) {
					r_error(player,"No mob equipment profile exists with that vnum");
					return;
				}
				if(!item.value()->set_slot(position, yaml)) {
					r_error(player,"Invalid slot type");
					return;
				}
				ENCODE_R("ok");
				r_success(player,"Set slot");
				return;
			}//end pave on
		}
		/** remove  */
		{
			auto args = mods::util::subcmd_args<7,args_t>(argument,"remove");

			if(args.has_value()) {
				ENCODE_INIT();
				auto cmd_args = args.value();
				if(cmd_args.size() < 3) {
					r_error(player,"Not enough arguments");
					return;
				}
				/**
				 * cmd_args will be: [0] => remove, [1] => <vnum> [2] => <position>
				 */
				auto vnum = mods::util::stoi(cmd_args[1]).value_or(-1);
				auto position = cmd_args[2];
				if(vnum <= 0) {
					r_error(player,"vnum must be a positive number");
					return;
				}
				auto item = by_vnum(vnum);
				if(!item.has_value()) {
					r_error(player,"No mob equipment profile exists with that vnum");
					return;
				}
				if(!item.value()->set_slot(position, "")) {
					r_error(player,"Invalid slot type");
					return;
				}
				ENCODE_R("ok");
				r_success(player,"Slot cleared");
				return;
			}//end pave on
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
					if(!delete_by_vnum(vnum)) {
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
		/** map-list */
		{
			auto args = mods::util::subcmd_args<11,args_t>(argument,"map-list");

			if(args.has_value()) {
				ENCODE_INIT();
				/**
				 * cmd_args will be: [0] => map-list
				 */
				std::string list = "";
				for(const auto& m : mods::orm::mob_equipment_map_list()) {
					list += CAT("{mob_vnum:",m->mmap_mob_vnum,",meq_vnum:",m->mmap_mob_equipment_vnum,"}");
				}
				ENCODE_R(list);
				r_success(player,list);
				return;
			}//end pave on
		}
		/** map-assign */
		{
			auto args = mods::util::subcmd_args<11,args_t>(argument,"map-assign");

			if(args.has_value()) {
				ENCODE_INIT();
				auto cmd_args = args.value();
				if(cmd_args.size() < 3) {
					r_error(player,"Not enough arguments");
					return;
				}
				/**
				 * cmd_args will be: [0] => map-assign, [1] => <mob-vnum> [2] => <eq-vnum>
				 */
				auto mvn = mods::util::stoi(cmd_args[1]).value_or(-1);
				auto eq_vnum = mods::util::stoi(cmd_args[2]).value_or(-1);
				if(mvn <= 0) {
					r_error(player,"mob vnum must be a positive number");
					return;
				}
				if(eq_vnum <= 0) {
					r_error(player,"eq vnum must be a positive number");
					return;
				}
				for(auto& m : mods::orm::mob_equipment_map_list()) {
					if(m->mmap_mob_vnum == mvn) {
						auto backup = m->mmap_mob_equipment_vnum;
						m->mmap_mob_equipment_vnum = eq_vnum;
						if(m->save() < 0) {
							r_error(player,"Unable to save existing.");
							m->mmap_mob_equipment_vnum = backup;
							return;
						}
						r_success(player,"Saved existing.");
						ENCODE_R("ok");
						return;
					}
				}
				/** we've reached here which means no existing map exists. create one */
				auto ref = std::make_shared<mods::orm::mob_equipment_map>();
				ref->initialize_row(mvn,eq_vnum);
				ref->mmap_mob_vnum = mvn;
				ref->mmap_mob_equipment_vnum = eq_vnum;
				if(ref->save() < 0) {
					r_error(player,"Unable to save.");
					return;
				}
				mods::orm::mob_equipment_map_list().emplace_back(std::move(ref));
				r_success(player,"Created new mapping and saved");
				ENCODE_R("ok");
				return;
			}//end pave on
		}
		/** map-delete */
		{
			auto args = mods::util::subcmd_args<11,args_t>(argument,"map-delete");

			if(args.has_value()) {
				ENCODE_INIT();
				auto cmd_args = args.value();
				if(cmd_args.size() < 2) {
					r_error(player,"Not enough arguments");
					return;
				}
				/**
				 * cmd_args will be: [0] => map-delete, [1] => <mob-vnum> ... [N] => <mob-vnum-N>
				 */
				std::string list = "";
				std::deque<std::shared_ptr<mods::orm::mob_equipment_map>> after;
				for(unsigned i=1; i < cmd_args.size(); ++i) {
					auto mvn = mods::util::stoi(cmd_args[i]).value_or(-1);
					if(mvn < 0) {
						r_error(player,"mob vnum must be a positive number");
						continue;
					}
					for(auto& m : mods::orm::mob_equipment_map_list()) {
						if(m->mmap_mob_vnum == mvn) {
							m->destroy();
							m->destroyed = true;
							list += CAT("{",mvn,"}");
							continue;
						}
					}
				}
				for(auto& m : mods::orm::mob_equipment_map_list()) {
					if(m->destroyed) {
						continue;
					}
					after.emplace_back(std::move(m));
				}
				mods::orm::mob_equipment_map_list() = std::move(after);
				r_success(player,CAT("Deleted: ",list));
				ENCODE_R(list);
				return;
			}//end pave on
		}
	}	//end meqbuild
	void init() {
		mods::interpreter::add_command("meqbuild", POS_RESTING, do_meqbuild, LVL_BUILDER,0);
		mods::builder::meqbuild::clear();
		mods::builder::meqbuild::load_all();
	}
};
