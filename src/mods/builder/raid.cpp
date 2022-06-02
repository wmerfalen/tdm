#include "raid.hpp"
#include "../interpreter.hpp"
#include "../util.hpp"
#include "../builder_util.hpp"
#include <map>

#include "../orm/raid.hpp"
#include "slotted-builder.hpp"
#include "../../db.h"

namespace mods::builder::raid {
	std::vector<raid_t> raid_list;


	/** ===================================================================================== */
	/** ===================================================================================== */
	/** ===================================================================================== */
	using raid_vnum_t = uint64_t;
	using raid_orm_type = mods::orm::raid;
	struct raid_interface : public slotted_builder<raid_vnum_t,raid_orm_type> {
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
			remove_command_signatures({"name","level","reload-all","destroy","set"});
			get_signatures()["new"] = "{grn}raid{/grn} {red}new <virtual-number>{/red}\r\n";

			register_manual_command("reload-orm","",[&](const std::string& argument) -> std::tuple<bool,std::string> {
				mods::orm::load_all_raid_list();
				return {1,"Loaded"};
			});

			register_custom_command("help","",[this](const std::vector<std::string>& args,std::string argument,std::shared_ptr<raid_orm_type> profile) -> std::tuple<bool,std::string> {
				display_signatures();
				auto msg = CAT("\r\n",
				               "Example: How to set multiple types on a step's task_type_t column:\r\n",
				               "{grn}raid set-step-data 400 0 s_task_type GOAL_QUOTA,GOAL_KILL{/grn}\r\n",
				               "Example: How to set step target:\r\n",
				               "{grn}raid set-step-data 400 0 s_task_target TARGET_MOB{/grn}\r\n",
				               "\r\n"
				              );

				push_encoded_ok(msg);
				return {1,""};
			});

			/**
			 * ==========================================
			 * level <vnum> <index> <text>...
			 * ==========================================
			 * [level]
			 *  -----------------------------------------
			 * 	sets the level on the given step
			 *  -----------------------------------------
			 */
			register_indexed_accumulator_command("level","<virtual_number> <index> <text>",[&](
			                                         std::string&& value,
			                                         std::shared_ptr<raid_orm_type> profile,
			int index) -> std::tuple<bool,std::string> {
				if(!profile) {
					return {0,"Invalid or missing profile vnum"};
				}
				if(index < 0) {
					return {0,"Invalid index"};
				}
				if(index >= this->step_list.size()) {
					return {0,"Index out of bounds"};
				}
				this->step_list[index]->r_level.assign(value);
				return {1,"Level successfully set"};
			});

			/**
			 * ==========================================
			 * name <vnum> <index> <text>...
			 * ==========================================
			 * [name]
			 *  -----------------------------------------
			 * 	sets the name on the given step
			 *  -----------------------------------------
			 */
			register_indexed_accumulator_command("name","<virtual_number> <index> <text>",[&](
			                                         std::string&& value,
			                                         std::shared_ptr<raid_orm_type> profile,
			int index) -> std::tuple<bool,std::string> {
				if(!profile) {
					return {0,"Invalid or missing profile vnum"};
				}
				if(index < 0) {
					return {0,"Invalid index"};
				}
				if(index >= this->step_list.size()) {
					return {0,"Index out of bounds"};
				}
				this->step_list[index]->r_name.assign(value);
				return {1,"Name successfully set"};
			});

			/**
			 * ==========================================
			 * name <vnum> <index> <text>...
			 * ==========================================
			 * [name]
			 *  -----------------------------------------
			 * 	sets the name on the given step
			 *  -----------------------------------------
			 */
			register_indexed_accumulator_command("name","<virtual_number> <index> <text>",[&](std::string&& value, std::shared_ptr<raid_orm_type> profile, int index) -> std::tuple<bool,std::string> {
				if(!profile) {
					return {0,"Invalid or missing profile vnum"};
				}
				if(index < 0) {
					return {0,"Invalid index"};
				}
				if(index >= this->step_list.size()) {
					return {0,"Index out of bounds"};
				}
				this->step_list[index]->r_name.assign(value);
				return {1,"Name successfully set"};
			});

			/**
			 * ==========================================
			 * create
			 * ==========================================
			 * [description]
			 *  -----------------------------------------
			 * 	creates a new step for the given contract vnum
			 *  -----------------------------------------
			 */
			register_custom_command("create","",[&,this](
			                            const std::vector<std::string>& args,
			                            std::string argument,
			                            std::shared_ptr<raid_orm_type> profile
			) -> std::tuple<bool,std::string> {
				step_list.emplace_back(std::make_shared<mods::orm::raid>());
				auto& step = step_list.back();
				return {1,CAT("Created step (pkid:",step->id,") for sequence vnum: ",profile->vnum(),". Step count is now: ",step_list.size())};
			});


			/**
			 * =============
			 * delete <vnum>
			 * =============
			 * [description]
			 *  --------------------------------------
			 * 	deletes the raid specified by the vnum
			 *  --------------------------------------
			 */
			register_custom_command("delete","<virtual_number>",[&,this](
			                            const std::vector<std::string>& args,
			                            std::string argument,
			std::shared_ptr<raid_orm_type> profile) -> std::tuple<bool,std::string> {
				if(!profile) {
					return {0,"Invalid or missing profile vnum"};
				}
				return profile->destroy();
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
				return {0,"Not enough arguments. Need 4"};
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
