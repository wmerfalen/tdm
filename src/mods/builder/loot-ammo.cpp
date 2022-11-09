#include "loot-ammo.hpp"
#include "../interpreter.hpp"
#include "../util.hpp"
#include "../builder_util.hpp"
#include "../orm/loot-ammo.hpp"
#include <map>

#include "slotted-builder.hpp"

#undef m_debug
#undef m_error
#ifdef __MENTOC_SHOW_MODS_AMMOBUILD_DEBUG_OUTPUT__
	#define m_debug(MSG) mentoc_prefix_debug("[mods::ammobuild::debug]")  << MSG << "\n";
	#define m_error(MSG) mentoc_prefix_debug(red_str("[mods::ammobuild::ERROR]"))  << MSG << "\n";
#else
	#define m_debug(MSG) ;;
	#define m_error(MSG) ;;
#endif


namespace mods::builder::ammobuild {


	/** ===================================================================================== */
	/** ===================================================================================== */
	/** ===================================================================================== */
	using ammobuild_vnum_t = uint64_t;
	using ammobuild_orm_type = mods::orm::loot_ammo;
	using orm_list_t = mods::orm::loot_ammo_list_t;
	using loot_ammo_list_iterator_t = std::deque<std::shared_ptr<mods::orm::loot_ammo>>::iterator;
	using loot_it_list_t = std::vector<loot_ammo_list_iterator_t>;
	struct ammobuild_interface : public slotted_builder<ammobuild_vnum_t,ammobuild_orm_type> {
		/** ======== */
		/** required */
		/** ======== */
		bool delete_by_vnum(ammobuild_vnum_t vnum) {
			m_debug("delete by vnum");
			bool deleted = false;

			loot_it_list_t it_list;
			auto begin = mods::orm::loot_ammo_list().begin();
			auto end = mods::orm::loot_ammo_list().end();
			for(auto it = begin; it != end; ++it) {
				auto& m = *it;
				if(m->la_room == vnum) {
					m_debug("deleting loot_ammo in room: " << vnum);
					m->destroy();
					deleted = true;
					it_list.emplace_back(it);
					continue;
				}
			}
			for(const auto it : it_list) {
				mods::orm::loot_ammo_list().erase(it);
			}
			return deleted;
		}

		/** ======== */
		/** required */
		/** ======== */
		/** will optionally return the orm entity that has the given vnum */
		std::optional<std::shared_ptr<ammobuild_orm_type>> by_vnum(ammobuild_vnum_t vnum) {
			m_debug("by_vnum");
			for(const auto& m : mods::orm::loot_ammo_list()) {
				if(m->la_room == vnum) {
					return m;
				}
			}
			return std::nullopt;
		}

		ammobuild_interface() {
			m_debug("ammobuild_interface");
			/** ======== */
			/** required */
			/** ======== */
			/** set available slots */
			set_slot_list(ammobuild_orm_type::get_slot_list());
			/** point the class to our base orm structure */
			set_orm_list(&mods::orm::loot_ammo_list());
			/** set the base command */
			set_base_command("ammobuild");
			this->clear();
			this->load_all();
			get_signatures()["new"] =
			    "{grn}admin:loot:ammo{/grn} {red}new <rarity> <levels> <count>{/red}\r\n";
			get_signatures()["clear"] =
			    "{grn}admin:loot:ammo{/grn} {red}clear{/red}\r\n"
			    "  |--> deletes all ammo crates in current room\r\n";

		}

		/** ======== */
		/** required */
		/** ======== */
		/** in order for the "new" command to work, we must implement this */
		status_response_t dispatch_new_command(const std::vector<std::string>& cmd_args,std::string argument) {
			m_debug("dispatch_new_command");
			/**
			 * cmd_args: [0] => "new" [1] =>types  [2] => levels [3] => count
			 */
			if(cmd_args.size() < 4) {
				return {0,"{ammobuild new} Error: not enough arguments"};
			}
			auto count = extract_int<int>("new",argument.c_str(),3).value_or(-1);
			if(count< 0) {
				return {0,"{ammobuild new} count must be a positive number"};
			}
			auto ammo = std::make_shared<mods::orm::loot_ammo>();
			auto id = ammo->initialize_row(player->vnum(),cmd_args[1],cmd_args[2],count);
			if(id <= 0) {
				return {0,"{ammobuild new} Couldn't initialize row"};
			}
			this->clear();
			this->load_all();
			return {1,"{ammobuild new} Created row."};
		}
		/** ======== */
		/** required */
		/** ======== */
		void clear() {
			m_debug("clear");
			mods::orm::loot_ammo_list().clear();
		}
		/** ======== */
		/** required */
		/** ======== */
		void load_all() {
			m_debug("load-all");
			mods::orm::loot_ammo_list() = mods::orm::load_all_loot_ammo_list();
		}
		bool dispatch_multi_vnum_action(std::string argument) override {
			m_debug("ammobuild class dispatch_multi_vnum_action");

			m_debug("dispatch_multi_vnum_action: '" << argument << "'");
			if(argshave()->first_is("list")->passed()) {
				player->sendln("Listing...");
				for(const auto& a : mods::orm::loot_ammo_list()) {
					player->sendln(CAT(a->dump()));
				}
				player->sendln("[+] Done listing.");
				return true;
			}
			if(argshave()->first_is("clear")->passed()) {
				if(delete_by_vnum(player->vnum())) {
					push_encoded_ok(CAT("Success deleted ",player->vnum()));
				} else {
					push_encoded_error(CAT("Did not delete ",player->vnum()));
				}
				return true;
			}
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

	using args_t = std::vector<std::string>;

	ammobuild_interface& ammobuilder(player_ptr_t player) {
		static ammobuild_interface interface;
		interface.set_builder(player);
		return interface;
	}

	SUPERCMD(do_ammobuild) {
		static int txn_id = 0;

		m_debug("txn_id: " << txn_id);
		mods::builder::initialize_builder(player);
		ADMIN_REJECT();
		m_debug("ammobuild passing to handle_input ");
		m_debug("handle_input args:" << argument);
		auto handled = ammobuilder(player).handle_input(argument);
		ammobuilder(player).flush();
		if(handled) {
			m_debug("handled by ammobuilder");
			//return;
		}
		++txn_id;
	}	//end ammobuild
	void init() {
		ADD_BUILDER_COMMAND("admin:loot:ammo",do_ammobuild);
		ammobuilder(nullptr);
	}
};
#undef m_debug
#undef m_error
