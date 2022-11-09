#include "static-loot.hpp"
#include "../interpreter.hpp"
#include "../util.hpp"
#include "../builder_util.hpp"
#include <map>
#include "../orm/static-loot.hpp"
#include "../orm/base.hpp"

#include "slotted-builder.hpp"

#undef m_debug
#undef m_error
//#ifdef __MENTOC_SHOW_MODS_BLEED_DEBUG_OUTPUT__
#define m_debug(MSG) mentoc_prefix_debug("[mods::staticlootbuild::debug]")  << MSG << "\n";
#define m_error(MSG) mentoc_prefix_debug(red_str("[mods::staticlootbuild::ERROR]"))  << MSG << "\n";
//#else
//#define m_debug(MSG) ;;
//#define m_error(MSG) ;;
//#endif


namespace mods::builder::staticlootbuild {


	/** ===================================================================================== */
	/** ===================================================================================== */
	/** ===================================================================================== */
	using staticlootbuild_vnum_t = room_vnum;
	using staticlootbuild_orm_type = mods::orm::static_loot;
	using orm_list_t = mods::orm::static_loot_list_t;
	using static_loot_list_iterator_t = std::deque<std::shared_ptr<mods::orm::static_loot>>::iterator;
	struct staticlootbuild_interface : public slotted_builder<staticlootbuild_vnum_t,staticlootbuild_orm_type> {
		/** ======== */
		/** required */
		/** ======== */
		bool delete_by_vnum(staticlootbuild_vnum_t vnum) {
			m_debug("delete by vnum");
			bool deleted = false;
			auto& r = mods::orm::static_loot_list();
			r.erase(
			std::remove_if(r.begin(),r.end(),[&vnum,&deleted](auto& r) -> bool {
				if(r->sl_room == vnum) {
					r->destroy();
					deleted = true;
					return true;
				}
				return false;
			}),r.end());
			return deleted;
		}

		/** ======== */
		/** required */
		/** ======== */
		/** will optionally return the orm entity that has the given vnum */
		std::optional<std::shared_ptr<staticlootbuild_orm_type>> by_vnum(staticlootbuild_vnum_t vnum) {
			m_debug("by_vnum");
			for(const auto& m : mods::orm::static_loot_list()) {
				if(m->sl_room == vnum) {
					return m;
				}
			}
			return std::nullopt;
		}

		staticlootbuild_interface() {
			m_debug("staticlootbuild_interface");
			/** ======== */
			/** required */
			/** ======== */
			/** set available slots */
			set_slot_list(staticlootbuild_orm_type::get_slot_list());
			/** point the class to our base orm structure */
			set_orm_list(&mods::orm::static_loot_list());
			/** set the base command */
			set_base_command("staticlootbuild");
			this->clear();
			this->load_all();
			get_signatures().clear();
			get_signatures()["new"] =
			    "{grn}admin:static:loot{/grn} {red}new <yaml> <count>{/red}\r\n";
			get_signatures()["clear"] =
			    "{grn}admin:static:loot{/grn} {red}clear{/red}\r\n"
			    "  |--> deletes all payloads in current room\r\n";


		}

		/** ======== */
		/** required */
		/** ======== */
		/** in order for the "new" command to work, we must implement this */
		status_response_t dispatch_new_command(const std::vector<std::string>& cmd_args,std::string argument) {
			m_debug("dispatch_new_command");
			/**
			 * cmd_args: [0] => "new" [1] => yaml [2] => count
			 */
			if(cmd_args.size() < 3) {
				return {0,"{staticlootbuild new} Error: not enough arguments"};
			}
			auto count = extract_int<int>("new",argument.c_str(),2).value_or(-1);
			if(count < 0) {
				return {0,"{staticlootbuild new} count must be a positive number"};
			}
			auto payload = std::make_shared<mods::orm::static_loot>();
			auto id = payload->initialize_row(player->vnum(),cmd_args[1],count);
			if(id <= 0) {
				return {0,"{staticlootbuild new} Couldn't initialize row"};
			}
			mods::orm::static_loot_list().emplace_back(std::move(payload));
			for(auto& m : mods::orm::static_loot_list()) {
				player->sendln(CAT(m->id, ": yaml:",m->sl_yaml,"|room:",m->sl_room,"|count:",m->sl_count));
			}
			return {1,"{staticlootbuild new} Created row."};
		}
		/** ======== */
		/** required */
		/** ======== */
		void clear() {
			m_debug("clear");
			mods::orm::static_loot_list().clear();
		}
		/** ======== */
		/** required */
		/** ======== */
		void load_all() {
			m_debug("load-all");
			mods::orm::static_loot_list() = mods::orm::load_all_static_loot_list();
			m_debug("done load-all");
		}
		bool dispatch_multi_vnum_action(std::string argument) override {
			m_debug("staticlootbuild class dispatch_multi_vnum_action");

			m_debug("dispatch_multi_vnum_action: '" << argument << "'");
			if(argshave()->first_is("list")->passed()) {
				player->sendln("Listing...");
				for(const auto& m : mods::orm::static_loot_list()) {
					player->sendln(CAT(m->dump()));
				}
				return true;
			}
			if(argshave()->first_is("clear")->passed()) {
				if(delete_by_vnum(player->vnum())) {
					push_encoded_ok(CAT("Success deleted ",player->vnum()));
				} else {
					push_encoded_error(CAT("FAILED to delete ",player->vnum()));
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

	static inline staticlootbuild_interface& staticlootbuilder(player_ptr_t player) {
		static staticlootbuild_interface interface;
		interface.set_builder(player);
		return interface;
	}

	SUPERCMD(do_staticlootbuild) {
		static int txn_id = 0;

		m_debug("txn_id: " << txn_id);
		mods::builder::initialize_builder(player);
		ADMIN_REJECT();
		m_debug("staticlootbuild passing to handle_input ");
		m_debug("handle_input args:" << argument);
		auto handled = staticlootbuilder(player).handle_input(argument);
		staticlootbuilder(player).flush();
		if(handled) {
			m_debug("handled by staticlootbuilder");
			//return;
		}

		++txn_id;
	}	//end staticlootbuild
	void init() {
		ADD_BUILDER_COMMAND("admin:static:loot",do_staticlootbuild);
		staticlootbuilder(nullptr);
	}
};
#undef m_debug
#undef m_error
