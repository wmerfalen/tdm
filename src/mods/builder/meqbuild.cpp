#include "meqbuild.hpp"
#include "../interpreter.hpp"
#include "../util.hpp"
#include "../builder_util.hpp"
#include <map>
#include "../orm/mob-equipment.hpp"
#include "../orm/mob-equipment-map.hpp"
#include "../orm/base.hpp"
#include "../mob-equipment.hpp"

#include "slotted-builder.hpp"

namespace mods::builder::meqbuild {


	/** ===================================================================================== */
	/** ===================================================================================== */
	/** ===================================================================================== */
	using meqbuild_vnum_t = uint64_t;
	using meqbuild_orm_type = mods::orm::mob_equipment;
	struct meqbuild_interface : public slotted_builder<meqbuild_vnum_t,meqbuild_orm_type> {
		/** ======== */
		/** required */
		/** ======== */
		bool delete_by_vnum(meqbuild_vnum_t vnum) {
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

		/** ======== */
		/** required */
		/** ======== */
		/** will optionally return the orm entity that has the given vnum */
		std::optional<std::shared_ptr<meqbuild_orm_type>> by_vnum(meqbuild_vnum_t vnum) {
			for(const auto& m : mods::orm::mob_equipment_list()) {
				if(m->meq_vnum == vnum) {
					return m;
				}
			}
			return std::nullopt;
		}

		meqbuild_interface() {
			/** ======== */
			/** required */
			/** ======== */
			/** set available slots */
			set_slot_list(meqbuild_orm_type::get_slot_list());
			/** point the class to our base orm structure */
			set_orm_list(&mods::orm::mob_equipment_list());
			/** set the base command */
			set_base_command("meqbuild");
			clear();
			load_all();
		}

		/** ======== */
		/** required */
		/** ======== */
		/** in order for the "new" command to work, we must implement this */
		status_response_t dispatch_new_command(const std::vector<std::string>& cmd_args,std::string argument) {
			/**
			 * cmd_args: [0] => "new" [1] => profile-name [2] => vnum
			 */
			if(cmd_args.size() < 3) {
				return {0,"Error: not enough arguments"};
			}
			auto vnum = extract_int<int>("new",argument.c_str(),2).value_or(-1);
			if(vnum < 0) {
				return {0,"vnum must be a positive number"};
			}
			auto meq = std::make_shared<mods::orm::mob_equipment>();
			auto id = meq->initialize_row(cmd_args[1],vnum);
			if(id <= 0) {
				return {0,"Couldn't initialize row"};
			}
			mods::orm::mob_equipment_list().emplace_back(std::move(meq));
			return {1,"Created row."};
		}
		/** ======== */
		/** required */
		/** ======== */
		void clear() {
			mods::orm::mob_equipment_list().clear();
			mods::orm::mob_equipment_map_list().clear();
			mods::mob_equipment::refresh_mappings();
		}
		/** ======== */
		/** required */
		/** ======== */
		void load_all() {
			mods::orm::mob_equipment_list() = std::move(mods::orm::load_all_by_table<mods::orm::mob_equipment>());
			mods::orm::mob_equipment_map_list() = std::move(mods::orm::load_all_by_table<mods::orm::mob_equipment_map>());
			mods::mob_equipment::refresh_mappings();
		}
	};

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

	meqbuild_interface& meqbuilder(player_ptr_t player) {
		static meqbuild_interface interface;
		interface.set_builder(player);
		return interface;
	}

	ACMD(do_meqbuild) {
		mods::builder::initialize_builder(player);
#if 0
		if(vec_args.size() == 0 || vec_args[0].compare("help") == 0) {
			player->pager_start();
			*player << "usage: \r\n" <<
			        "  {gld}|:: -:[positions]:-{/gld}\r\n" <<
			        wear_flag_list() <<

			        /** remove */
			        " {grn}meqbuild{/grn} {red}remove <virtual_number> <position>{/red}\r\n" <<
			        "  |--> removes the slot at 'position' on profile 'virtual_number'\r\n" <<
			        "  {grn}|____[example]{/grn}\r\n" <<
			        "  |:: {wht}meqbuild{/wht} {grn}remove 3 wield{/grn}\r\n" <<
			        "  |--> clears the wield slot for profile 3\r\n" <<

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


			        "\r\n";
			player->pager_end();
			player->page(0);
			return;
		}
#endif

		if(meqbuilder(player).handle_input(argument)) {
			return;
		}

		auto vec_args = mods::util::arglist<std::vector<std::string>>(std::string(argument));
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
		meqbuilder(nullptr);
	}
};
