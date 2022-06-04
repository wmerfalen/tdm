#include "raid.hpp"
#include "../interpreter.hpp"
#include "../util.hpp"
#include "../builder_util.hpp"
#include <map>

#include "../orm/raid.hpp"
#include "slotted-builder.hpp"
#include "../../db.h"

namespace mods::builder::raid {


	/** ===================================================================================== */
	/** ===================================================================================== */
	/**
	 * Checklist
	 * ---------
	 * [x] List
	 * 	[ ] Must support encoded responses
	 * [x] Reload ORM
	 * [x] save <vnum> ... [N-vnum]
	 * 	[ ] Must support encoded responses
	 * [x] create <name> <level> <type>
	 * 	[ ] Must support encoded responses
	 * [ ] set <vnum> <field> <value>
	 * [x] Delete
	 *
	 * stretch-goals:
	 * [ ] filter <field> <regex>
	 */
	/** ===================================================================================== */
	using raid_vnum_t = uint64_t;
	using raid_orm_type = mods::orm::raid;
	struct raid_interface : public slotted_builder<raid_vnum_t,raid_orm_type> {
		void reload_orm() {
			mods::orm::raid_list() = std::move(mods::orm::load_all_raid_list());
			set_orm_list(&mods::orm::raid_list());
		}

		bool dispatch_multi_vnum_action(std::string argument) override {
			std::string msg;
			if(argshave()->first_is("list")->passed()) {
				for(auto& r : mods::orm::raid_list()) {
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
		bool delete_by_vnum(raid_vnum_t vnum) {
			mods::orm::raid_list_t list;
			bool deleted = false;
			for(auto& r : mods::orm::raid_list()) {
				if(r->id == vnum) {
					r->destroy();
					deleted = true;
					continue;
				}
				list.emplace_back(std::move(r));
			}
			mods::orm::raid_list() = std::move(list);
			return deleted;
		}

		/** ======== */
		/** required */
		/** ======== */
		/** will optionally return the orm entity that has the given vnum */
		std::optional<std::shared_ptr<raid_orm_type>> by_vnum(raid_vnum_t vnum) {
			for(const auto& m : mods::orm::raid_list()) {
				if(m->vnum() == vnum) {
					return m;
				}
			}
			return std::nullopt;
		}
		static mods::orm::raid& temp_step() {
			static mods::orm::raid orm;
			return orm;
		}
		std::vector<std::string> step_column_list() const {
			return mods::orm::raid::get_slot_list();
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

		/** raids */
		/** will optionally return the orm entity that has the given vnum */
		std::optional<std::shared_ptr<mods::orm::raid>> step_by_vnum(raid_vnum_t vnum) {
			for(const auto& m : mods::orm::raid_list()) {
				if(m->vnum() == vnum) {
					return m;
				}
			}
			return std::nullopt;
		}

		mods::orm::raid_list_t step_list;
		std::optional<std::shared_ptr<mods::orm::raid>> find_local_step_by_id(const uint64_t& step_id) {
			for(const auto& s : step_list) {
				if(s->id == step_id) {
					return s;
				}
			}
			return std::nullopt;
		}
		void dump_step(std::shared_ptr<mods::orm::raid> s) {
			auto str = s->dump();
			//str += CAT("task_type_t: '", IMPLODE(mods::contracts::get_string_list_from_task_flags((mods::contracts::task_t)s->s_task_type),","),"'\r\n");
			//str += CAT("task_target_t: '", mods::contracts::get_string_from_target((mods::contracts::target_t)s->s_task_target),"'\r\n");
			push_encoded_ok(str);
		}

		raid_interface() {
			/** ======== */
			/** required */
			/** ======== */
			/** set available slots */
			set_slot_list(raid_orm_type::get_slot_list());
			/** point the class to our base orm structure */
			set_orm_list(&mods::orm::raid_list());
			/** set the base command */
			set_base_command("admin:raid");
			clear();
			load_all();
			remove_command_signatures({"name","list-extract","destroy","set"});
			get_signatures()["new"] = "{grn}admin:raid{/grn} {red}new <virtual-number>{/red}\r\n";

			register_manual_command("reload-orm","",[this](std::string argument) -> std::tuple<bool,std::string> {
				player->sendx("Reloading..");
				this->reload_orm();
				player->sendln("done");
				return {1,"Loaded"};
			});

			register_custom_command("help","",[this](const std::vector<std::string>& args,std::string argument,std::shared_ptr<raid_orm_type> profile) -> std::tuple<bool,std::string> {
				display_signatures();
				return {1,""};
			});

			register_custom_command("set","<vnum> <name|level|type> <value>",
			                        [this](
			                            const std::vector<std::string>& args,
			                            std::string argument,
			std::shared_ptr<raid_orm_type> profile) -> std::tuple<bool,std::string> {
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
				if(field.compare("name") == 0) {
					msg += ("Setting name..");
					profile->set("r_name",argat(3));
					msg += "Done";
				} else if(field.compare("level") == 0) {
					msg += "Setting level...";
					profile->set("r_level",argat(3));
					msg += "Done";
				} else if(field.compare("type") == 0) {
					msg += "Setting level...";
					profile->set("r_type",argat(3));
					msg += "Done";
				} else {
					push_encoded_error("Did not recognize field name. Expected one of: name,level,type");
					return {0,"Did not recognize field name. Expected one of: name,level,type"};
				}

				push_encoded_ok(msg);
				return {1,""};
			});

		}//end raid_interface

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
			auto c = std::make_shared<mods::orm::raid>();
			auto id = c->initialize_row(argat(1),argat(2),argat(3));
			if(id <= 0) {
				return {0,"Couldn't initialize row"};
			}
			mods::orm::raid_list().emplace_back(std::move(c));
			this->step_list.clear();
			return {1,"Created row."};
		}
		/** ======== */
		/** required */
		/** ======== */
		void clear() {
			mods::orm::raid_list().clear();
		}
		/** ======== */
		/** required */
		/** ======== */
		void load_all() {
			mods::orm::raid_list() = std::move(mods::orm::load_all_raid_list());
		}
	};

	namespace map {

		bool delete_by_vnum(const uint64_t& vnum) {
			std::deque<std::shared_ptr<mods::orm::raid>> list;
			bool deleted = false;
			for(auto& m : mods::orm::raid_list()) {
				if(m->vnum() == vnum) {
					m->destroy();
					deleted = true;
					continue;
				}
				list.emplace_back(std::move(m));
			}
			mods::orm::raid_list() = std::move(list);
			return deleted;
		}
		std::deque<std::shared_ptr<mods::orm::raid>> by_profile_vnum(const uint64_t& vnum) {
			std::deque<std::shared_ptr<mods::orm::raid>> list;
			for(const auto& m : mods::orm::raid_list()) {
				if(m->vnum() == vnum) {
					list.emplace_back(m);
				}
			}
			return list;
		}
	};
	using args_t = std::vector<std::string>;

	static raid_interface& raidbuild(player_ptr_t player) {
		static raid_interface interface;
		interface.set_builder(player);
		return interface;
	}

	SUPERCMD(do_raid) {
		mods::builder::initialize_builder(player);

		auto handled = raidbuild(player).handle_input(argument);
		raidbuild(player).flush();
		if(handled) {
			return;
		}

	}	//end raid

	void init() {
		ADD_BUILDER_COMMAND("admin:raid",do_raid);
		raidbuild(nullptr);
	}
};