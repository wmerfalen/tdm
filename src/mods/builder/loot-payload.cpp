#include "loot-payload.hpp"
#include "../interpreter.hpp"
#include "../util.hpp"
#include "../builder_util.hpp"
#include <map>
#include "../orm/mob-equipment.hpp"
#include "../orm/mob-equipment-map.hpp"
#include "../orm/base.hpp"
#include "../mob-equipment.hpp"

#include "slotted-builder.hpp"

#undef m_debug
#undef m_error
//#ifdef __MENTOC_SHOW_MODS_BLEED_DEBUG_OUTPUT__
#define m_debug(MSG) mentoc_prefix_debug("[mods::paybuild::debug]")  << MSG << "\n";
#define m_error(MSG) mentoc_prefix_debug(red_str("[mods::paybuild::ERROR]"))  << MSG << "\n";
//#else
//#define m_debug(MSG) ;;
//#define m_error(MSG) ;;
//#endif


namespace mods::builder::paybuild {


	/** ===================================================================================== */
	/** ===================================================================================== */
	/** ===================================================================================== */
	using paybuild_vnum_t = uint64_t;
	using paybuild_orm_type = mods::orm::mob_equipment;
	struct paybuild_interface : public slotted_builder<paybuild_vnum_t,paybuild_orm_type> {
		/** ======== */
		/** required */
		/** ======== */
		bool delete_by_vnum(paybuild_vnum_t vnum) {
			m_debug("delete by vnum");
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
		std::optional<std::shared_ptr<paybuild_orm_type>> by_vnum(paybuild_vnum_t vnum) {
			m_debug("by_vnum");
			for(const auto& m : mods::orm::mob_equipment_list()) {
				if(m->meq_vnum == vnum) {
					return m;
				}
			}
			return std::nullopt;
		}

		paybuild_interface() {
			m_debug("paybuild_interface");
			/** ======== */
			/** required */
			/** ======== */
			/** set available slots */
			set_slot_list(paybuild_orm_type::get_slot_list());
			/** point the class to our base orm structure */
			set_orm_list(&mods::orm::mob_equipment_list());
			/** set the base command */
			set_base_command("paybuild");
			clear();
			load_all();
			get_signatures()["map-assign"] =
			    "{grn}paybuild{/grn} {red}map-assign <mob-virtual-number> <meq-profile-vnum>{/red}\r\n"
			    "  |--> assigns the specific mob to have the equipment profile of 'meq_vnum'\r\n";
			get_signatures()["map-delete"] =
			    "{grn}paybuild{/grn} {red}map-delete <mob-virtual-number>...[mob-virtual-number-N]{/red}\r\n"
			    "  |--> deletes the map-assign'd mob to meq vnum mapping \r\n"
			    "  |--> A mob without a meq vnum will not spawn with equipment\r\n";
			get_signatures()["map-list"] =
			    "{grn}paybuild{/grn} {red}map-list{/red}\r\n"
			    "  |--> lists the specific mob to equipment profiles\r\n";

		}

		/** ======== */
		/** required */
		/** ======== */
		/** in order for the "new" command to work, we must implement this */
		status_response_t dispatch_new_command(const std::vector<std::string>& cmd_args,std::string argument) {
			m_debug("dispatch_new_command");
			/**
			 * cmd_args: [0] => "new" [1] => profile-name [2] => vnum
			 */
			if(cmd_args.size() < 3) {
				return {0,"{paybuild new} Error: not enough arguments"};
			}
			auto vnum = extract_int<int>("new",argument.c_str(),2).value_or(-1);
			if(vnum < 0) {
				return {0,"{paybuild new} vnum must be a positive number"};
			}
			auto meq = std::make_shared<mods::orm::mob_equipment>();
			auto id = meq->initialize_row(cmd_args[1],vnum);
			if(id <= 0) {
				return {0,"{paybuild new} Couldn't initialize row"};
			}
			mods::orm::mob_equipment_list().emplace_back(std::move(meq));
			return {1,"{paybuild new} Created row."};
		}
		/** ======== */
		/** required */
		/** ======== */
		void clear() {
			m_debug("clear");
			mods::orm::mob_equipment_list().clear();
			mods::orm::mob_equipment_map_list().clear();
			mods::mob_equipment::refresh_mappings();
		}
		/** ======== */
		/** required */
		/** ======== */
		void load_all() {
			m_debug("load-all");
			mods::orm::mob_equipment_list() = std::move(mods::orm::load_all_by_table<mods::orm::mob_equipment>());
			mods::orm::mob_equipment_map_list() = std::move(mods::orm::load_all_by_table<mods::orm::mob_equipment_map>());
			mods::mob_equipment::refresh_mappings();
		}
		bool dispatch_multi_vnum_action(std::string argument) override {
			m_debug("paybuild class dispatch_multi_vnum_action");

			m_debug("dispatch_multi_vnum_action: '" << argument << "'");
			if(argshave()->first_is("save")->passed()) {
				std::size_t ok = 0, error = 0;
				for(const auto& vnum : args()->gather_uuids_starting_at(1)) {
					m_debug("dispatch_multi_vnum_action vnum:" << vnum);
					auto opt = by_vnum(vnum);
					if(opt.has_value()) {
						auto m = opt.value();
						auto s = m->save();
						if(s <= 0) {
							push_encoded_ok(CAT("Success saved ",vnum));
							++ok;
						} else {
							push_encoded_error(CAT("FAILED to save ",vnum));
							++error;
						}
					}
				}
				player->sendln(CAT("successfully saved (",ok,") items. Failed to save (",error,") items"));

				return true;
			}
			return false;
		}

	};

	namespace map {

		bool delete_by_vnum(const uint64_t& vnum) {
			m_debug("delete_by_vnum");
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
			m_debug("by_mob_vnum");
			std::vector<std::shared_ptr<mods::orm::mob_equipment_map>> list;
			for(const auto& m : mods::orm::mob_equipment_map_list()) {
				if(m->mmap_mob_vnum == vnum) {
					list.emplace_back(m);
				}
			}
			return list;
		}
		std::vector<std::shared_ptr<mods::orm::mob_equipment_map>> by_profile_vnum(const uint64_t& vnum) {
			m_debug("by_profile_vnum");
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

	paybuild_interface& paybuilder(player_ptr_t player) {
		static paybuild_interface interface;
		interface.set_builder(player);
		return interface;
	}

	SUPERCMD(do_paybuild) {
		static int txn_id = 0;

		m_debug("txn_id: " << txn_id);
		mods::builder::initialize_builder(player);
		ADMIN_REJECT();
		m_debug("paybuild passing to handle_input ");
		m_debug("handle_input args:" << argument);
		auto handled = paybuilder(player).handle_input(argument);
		paybuilder(player).flush();
		if(handled) {
			m_debug("handled by paybuilder");
			//return;
		}

		auto vec_args = mods::util::arglist<std::vector<std::string>>(std::string(argument));
		/** map-list */
		{
			auto args = mods::util::subcmd_args<11,args_t>(argument,"map-list");

			if(args.has_value()) {
				m_debug("processing map-list");
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
				m_debug("processing map-assign");
				ENCODE_INIT();
				auto cmd_args = args.value();
				if(cmd_args.size() < 3) {
					m_debug("not enough args to map-assign");
					r_error(player,"{paybuild map-assign} Not enough arguments");
					return;
				}
				/**
				 * cmd_args will be: [0] => map-assign, [1] => <mob-vnum> [2] => <eq-vnum>
				 */
				auto mvn = mods::util::stoi(cmd_args[1]).value_or(-1);
				auto eq_vnum = mods::util::stoi(cmd_args[2]).value_or(-1);
				if(mvn <= 0) {
					m_debug("map-assign mob vnum invalid");
					r_error(player,"{paybuild map-assign} mob vnum must be a positive number");
					return;
				}
				if(eq_vnum <= 0) {
					m_debug("map-assign eq vnum invalid");
					r_error(player,"{paybuild map-assign} eq vnum must be a positive number");
					return;
				}
				for(auto& m : mods::orm::mob_equipment_map_list()) {
					if(m->mmap_mob_vnum == mvn) {
						auto backup = m->mmap_mob_equipment_vnum;
						m->mmap_mob_equipment_vnum = eq_vnum;
						if(m->save() < 0) {
							r_error(player,"{paybuild map-assign} Unable to save existing.");
							m->mmap_mob_equipment_vnum = backup;
							m_debug("map-assign cant save existing");
							return;
						}
						m_debug("map-assign saved existing");
						r_success(player,"{paybuild map-assign} Saved existing.");
						ENCODE_R("ok");
						return;
					}
				}
				/** we've reached here which means no existing map exists. create one */
				m_debug("map-assign creating");
				auto ref = std::make_shared<mods::orm::mob_equipment_map>();
				ref->initialize_row(mvn,eq_vnum);
				ref->mmap_mob_vnum = mvn;
				ref->mmap_mob_equipment_vnum = eq_vnum;
				if(ref->save() < 0) {
					m_debug("map-assign unable to create");
					r_error(player,"{paybuild map-assign} Unable to save.");
					return;
				}
				mods::orm::mob_equipment_map_list().emplace_back(std::move(ref));
				r_success(player,"{paybuild map-assign} Created new mapping and saved");
				ENCODE_R("ok");
				m_debug("ok processing map-assign");
				return;
			}//end pave on
		}
		/** map-delete */
		{
			m_debug("checking map-delete");
			auto args = mods::util::subcmd_args<11,args_t>(argument,"map-delete");

			if(args.has_value()) {
				m_debug("map-delete processing");
				ENCODE_INIT();
				auto cmd_args = args.value();
				if(cmd_args.size() < 2) {
					r_error(player,"{paybuild map-delete} Not enough arguments");
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
						m_debug("map-delete mob vnum invalid");
						r_error(player,"{paybuild map-delete} mob vnum must be a positive number");
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
				r_success(player,CAT("{paybuild map-delete} Deleted: ",list));
				m_debug("map-delete okay");
				ENCODE_R(list);
				return;
			}//end pave on
		}
		m_debug("::END:: txn_id: " << txn_id);
		++txn_id;
	}	//end paybuild
	void init() {
		ADD_BUILDER_COMMAND("admin:loot:payload:create",do_paybuild);
		ADD_BUILDER_COMMAND("admin:loot:payload:delete",do_paybuild);
		paybuilder(nullptr);
	}
};
#undef m_debug
#undef m_error
