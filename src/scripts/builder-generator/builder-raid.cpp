#include "___[[[TABLE]]]___.hpp"
#include "../interpreter.hpp"
#include "../util.hpp"
#include "../builder_util.hpp"
#include <map>

#include "../orm/___[[[TABLE]]]___.hpp"
#include "slotted-builder.hpp"
#include "../../db.h"

namespace mods::builder::___[[[TABLE]]]___ {


	/** ===================================================================================== */
	/** ===================================================================================== */
	/** ===================================================================================== */
	using ___[[[TABLE]]]____vnum_t = uint64_t;
	using ___[[[TABLE]]]____orm_type = mods::orm::___[[[TABLE]]]___;
	struct ___[[[TABLE]]]____interface : public slotted_builder<___[[[TABLE]]]____vnum_t,___[[[TABLE]]]____orm_type> {
		void reload_orm() {
			mods::orm::___[[[TABLE]]]____list() = std::move(mods::orm::load_all____[[[TABLE]]]____list());
			set_orm_list(&mods::orm::___[[[TABLE]]]____list());
		}

		bool dispatch_multi_vnum_action(std::string argument) override {
			std::string msg;
			if(argshave()->first_is("list")->passed()) {
				for(auto& r : mods::orm::___[[[TABLE]]]____list()) {
					msg += r->dump();
				}
				push_encoded_ok(msg);
				return true;
			}
			if(argshave()->first_is("save")->passed()) {

			}
			if(argshave()->first_is("delete")->size_gt(1)->passed()) {
				for(auto i=1; i < args()->size; i++) {
					auto vnum = intat(i);
					if(vnum == -1) {
						player->sendln(CAT("Invalid integer value: ",argat(i), ". Ignoring..."));
						continue;
					}
					player->sendln(CAT("Deleting by vnum/id:",vnum));
					auto s = this->by_vnum(vnum);
					if(s.has_value() == false) {
						msg += CAT("no profile for vnum/id:",vnum,"\r\n");
					}
					auto rs = s.value()->destroy();
					if(ORM_FAILURE(rs)) {
						msg += CAT("failed to delete ",vnum,":",std::get<1>(rs));
					} else {
						msg += CAT("deleted ",vnum,"\r\n");
					}
				}
				this->reload_orm();
				push_encoded_ok(msg);
				return true;
			}
			/**
			 * MUST ALWAYS RETURN FALSE. Otherwise all other commands won't be processed.
			 * Returning true means we handled the command.
			 * Returning false means we pass it on to the custom/manual commands.
			 */
			return false;
		}

		/** ======== */
		/** required */
		/** ======== */
		bool delete_by_vnum(___[[[TABLE]]]____vnum_t vnum) {
			mods::orm::___[[[TABLE]]]____list_t list;
			bool deleted = false;
			for(auto& r : mods::orm::___[[[TABLE]]]____list()) {
				if(r->id == vnum) {
					r->destroy();
					deleted = true;
					continue;
				}
				list.emplace_back(std::move(r));
			}
			mods::orm::___[[[TABLE]]]____list() = std::move(list);
			return deleted;
		}

		/** ======== */
		/** required */
		/** ======== */
		/** will optionally return the orm entity that has the given vnum */
		std::optional<std::shared_ptr<___[[[TABLE]]]____orm_type>> by_vnum(___[[[TABLE]]]____vnum_t vnum) {
			for(const auto& m : mods::orm::___[[[TABLE]]]____list()) {
				if(m->vnum() == vnum) {
					return m;
				}
			}
			return std::nullopt;
		}
		static mods::orm::___[[[TABLE]]]___& temp_step() {
			static mods::orm::___[[[TABLE]]]___ orm;
			return orm;
		}
		std::vector<std::string> step_column_list() const {
			return mods::orm::___[[[TABLE]]]___::get_slot_list();
		}
		const std::vector<std::string>& integral_list() const {
			return temp_step().integral_slots();
		}
		const std::vector<std::string>& string_list() const {
			return temp_step().string_slots();
		}
		const std::vector<std::string>& vector_string_list() const {
			return temp_step().vector_string_slots();
		}


		const std::vector<std::string>& accumulator_list() const {
			return temp_step().accumulator_slot_list();
		}

		/** ___[[[TABLE]]]___s */
		/** will optionally return the orm entity that has the given vnum */
		std::optional<std::shared_ptr<mods::orm::___[[[TABLE]]]___>> step_by_vnum(___[[[TABLE]]]____vnum_t vnum) {
			for(const auto& m : mods::orm::___[[[TABLE]]]____list()) {
				if(m->vnum() == vnum) {
					return m;
				}
			}
			return std::nullopt;
		}

		mods::orm::___[[[TABLE]]]____list_t step_list;
		std::optional<std::shared_ptr<mods::orm::___[[[TABLE]]]___>> find_local_step_by_id(const uint64_t& step_id) {
			for(const auto& s : step_list) {
				if(s->id == step_id) {
					return s;
				}
			}
			return std::nullopt;
		}
		void dump_step(std::shared_ptr<mods::orm::___[[[TABLE]]]___> s) {
			auto str = s->dump();
			//str += CAT("task_type_t: '", IMPLODE(mods::contracts::get_string_list_from_task_flags((mods::contracts::task_t)s->s_task_type),","),"'\r\n");
			//str += CAT("task_target_t: '", mods::contracts::get_string_from_target((mods::contracts::target_t)s->s_task_target),"'\r\n");
			push_encoded_ok(str);
		}

		___[[[TABLE]]]____interface() {
			/** ======== */
			/** required */
			/** ======== */
			/** set available slots */
			set_slot_list(___[[[TABLE]]]____orm_type::get_slot_list());
			/** point the class to our base orm structure */
			set_orm_list(&mods::orm::___[[[TABLE]]]____list());
			/** set the base command */
			set_base_command("admin:___[[[TABLE]]]___");
			clear();
			load_all();
			remove_command_signatures({"name","list-extract","destroy","set"});
			get_signatures()["new"] = "{grn}admin:___[[[TABLE]]]___{/grn} {red}new <virtual-number>{/red}\r\n";

			register_manual_command("reload-orm","",[this](std::string argument) -> std::tuple<bool,std::string> {
				player->sendx("Reloading..");
				this->reload_orm();
				player->sendln("done");
				return {1,"Loaded"};
			});

			register_custom_command("help","",[this](const std::vector<std::string>& args,std::string argument,std::shared_ptr<___[[[TABLE]]]____orm_type> profile) -> std::tuple<bool,std::string> {
				display_signatures();
				return {1,""};
			});

			register_custom_command("set","<vnum> <field> <value>",
			                        [this](
			                            const std::vector<std::string>& args,
			                            std::string argument,
			std::shared_ptr<___[[[TABLE]]]____orm_type> profile) -> std::tuple<bool,std::string> {
				std::string msg;
				if(!profile) {
					push_encoded_error("no profile");
					return {0,"No profile"};
				}
				if(argshave()->size_gt(2)->failed()) {
					push_encoded_error("Expected 3 arguments");
					return {0,"Expected 3 arguments: <vnum> <name|level|type> <value>"};
				}
				msg = CAT("Okay, processing...");
				auto field = argat(2);
				return profile->set(argat(1),argat(2));
			});

		}//end ___[[[TABLE]]]____interface

		/** ======== */
		/** required */
		/** ======== */
		/** in order for the "new" command to work, we must implement this */
		status_response_t dispatch_new_command(const std::vector<std::string>& cmd_args,std::string argument) {
			/**
			 * cmd_args: [0] => "new" [1] => "name" [2] => level [3] => type
			 */
			if(argshave()->size_gt(3)->failed()) {
				return {0,"Not enough arguments. Signature: new <name> <level> <type>"};
			}
			auto c = std::make_shared<mods::orm::___[[[TABLE]]]___>();
			auto id = c->initialize_row(argat(1),argat(2),argat(3));
			if(id <= 0) {
				return {0,"Couldn't initialize row"};
			}
			mods::orm::___[[[TABLE]]]____list().emplace_back(std::move(c));
			this->step_list.clear();
			return {1,"Created row."};
		}
		/** ======== */
		/** required */
		/** ======== */
		void clear() {
			mods::orm::___[[[TABLE]]]____list().clear();
		}
		/** ======== */
		/** required */
		/** ======== */
		void load_all() {
			mods::orm::___[[[TABLE]]]____list() = std::move(mods::orm::load_all____[[[TABLE]]]____list());
		}
	};

	namespace map {

		bool delete_by_vnum(const uint64_t& vnum) {
			std::deque<std::shared_ptr<mods::orm::___[[[TABLE]]]___>> list;
			bool deleted = false;
			for(auto& m : mods::orm::___[[[TABLE]]]____list()) {
				if(m->vnum() == vnum) {
					m->destroy();
					deleted = true;
					continue;
				}
				list.emplace_back(std::move(m));
			}
			mods::orm::___[[[TABLE]]]____list() = std::move(list);
			return deleted;
		}
		std::deque<std::shared_ptr<mods::orm::___[[[TABLE]]]___>> by_profile_vnum(const uint64_t& vnum) {
			std::deque<std::shared_ptr<mods::orm::___[[[TABLE]]]___>> list;
			for(const auto& m : mods::orm::___[[[TABLE]]]____list()) {
				if(m->vnum() == vnum) {
					list.emplace_back(m);
				}
			}
			return list;
		}
	};
	using args_t = std::vector<std::string>;

	static ___[[[TABLE]]]____interface& ___[[[TABLE]]]___build(player_ptr_t player) {
		static ___[[[TABLE]]]____interface interface;
		interface.set_builder(player);
		return interface;
	}

	SUPERCMD(do____[[[TABLE]]]___) {
		mods::builder::initialize_builder(player);

		auto handled = ___[[[TABLE]]]___build(player).handle_input(argument);
		___[[[TABLE]]]___build(player).flush();
		if(handled) {
			return;
		}

	}	//end ___[[[TABLE]]]___

	void init() {
		ADD_BUILDER_COMMAND("admin:___[[[TABLE]]]___",do____[[[TABLE]]]___);
		___[[[TABLE]]]___build(nullptr);
	}
};
