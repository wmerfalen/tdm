#include "loot-payload.hpp"
#include "../interpreter.hpp"
#include "../util.hpp"
#include "../builder_util.hpp"
#include <map>
#include "../orm/loot-payload.hpp"
#include "../orm/base.hpp"

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
	using paybuild_vnum_t = room_vnum;
	using paybuild_orm_type = mods::orm::loot_payload;
	using orm_list_t = mods::orm::loot_payload_list_t;
	using loot_payload_list_iterator_t = std::deque<std::shared_ptr<mods::orm::loot_payload>>::iterator;
	struct paybuild_interface : public slotted_builder<paybuild_vnum_t,paybuild_orm_type> {
		/** ======== */
		/** required */
		/** ======== */
		bool delete_by_vnum(paybuild_vnum_t vnum) {
			m_debug("delete by vnum");
			bool deleted = false;
			std::vector<loot_payload_list_iterator_t> it_list;
			auto end = mods::orm::loot_payload_list().end();
			for(auto it = mods::orm::loot_payload_list().begin(); it != end; ++it) {
				if((*it)->lp_room == vnum) {
					m_debug("deleting payload vnum: " << vnum);
					(*it)->destroy();
					deleted = true;
					it_list.emplace_back(it);
					continue;
				}
			}
			for(auto it : it_list) {
				mods::orm::loot_payload_list().erase(it);
			}
			return deleted;
		}

		/** ======== */
		/** required */
		/** ======== */
		/** will optionally return the orm entity that has the given vnum */
		std::optional<std::shared_ptr<paybuild_orm_type>> by_vnum(paybuild_vnum_t vnum) {
			m_debug("by_vnum");
			for(const auto& m : mods::orm::loot_payload_list()) {
				if(m->lp_room == vnum) {
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
			set_orm_list(&mods::orm::loot_payload_list());
			/** set the base command */
			set_base_command("paybuild");
			this->clear();
			this->load_all();
			get_signatures().clear();
			get_signatures()["new"] =
			    "{grn}admin:loot:payload{/grn} {red}new <rarity> <levels> <count>{/red}\r\n";
			get_signatures()["clear"] =
			    "{grn}admin:loot:payload{/grn} {red}clear{/red}\r\n"
			    "  |--> deletes all payloads in current room\r\n";


		}

		/** ======== */
		/** required */
		/** ======== */
		/** in order for the "new" command to work, we must implement this */
		status_response_t dispatch_new_command(const std::vector<std::string>& cmd_args,std::string argument) {
			m_debug("dispatch_new_command");
			/**
			 * cmd_args: [0] => "new" [1] => rarity [2] => levels [3] => count
			 */
			if(cmd_args.size() < 4) {
				return {0,"{paybuild new} Error: not enough arguments"};
			}
			auto count = extract_int<int>("new",argument.c_str(),3).value_or(-1);
			if(count < 0) {
				return {0,"{paybuild new} count must be a positive number"};
			}
			auto payload = std::make_shared<mods::orm::loot_payload>();
			auto id = payload->initialize_row(player->vnum(),cmd_args[1],cmd_args[1],count);
			if(id <= 0) {
				return {0,"{paybuild new} Couldn't initialize row"};
			}
			mods::orm::loot_payload_list().emplace_back(std::move(payload));
			for(auto& m : mods::orm::loot_payload_list()) {
				player->sendln(CAT(m->id, ": ",m->lp_rarity,"|",m->lp_room));
			}
			return {1,"{paybuild new} Created row."};
		}
		/** ======== */
		/** required */
		/** ======== */
		void clear() {
			m_debug("clear");
			mods::orm::loot_payload_list().clear();
		}
		/** ======== */
		/** required */
		/** ======== */
		void load_all() {
			m_debug("load-all");
			mods::orm::loot_payload_list() = mods::orm::load_all_loot_payload_list();
		}
		bool dispatch_multi_vnum_action(std::string argument) override {
			m_debug("paybuild class dispatch_multi_vnum_action");

			m_debug("dispatch_multi_vnum_action: '" << argument << "'");
			if(argshave()->first_is("list")->passed()) {
				player->sendln("Listing...");
				for(const auto& m : mods::orm::loot_payload_list()) {
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

	static inline paybuild_interface& paybuilder(player_ptr_t player) {
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

		++txn_id;
	}	//end paybuild
	void init() {
		ADD_BUILDER_COMMAND("admin:loot:payload",do_paybuild);
		paybuilder(nullptr);
	}
};
#undef m_debug
#undef m_error
