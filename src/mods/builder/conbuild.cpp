#include "conbuild.hpp"
#include "../interpreter.hpp"
#include "../util.hpp"
#include "../builder_util.hpp"
#include <map>
#include "../orm/contracts.hpp"
#include "../orm/contract-steps.hpp"
#include "../orm/player-contract-state.hpp"
#include "../contracts.hpp"
#include "../contract-types.hpp"

#include "slotted-builder.hpp"
namespace mods::orm {
	std::deque<std::shared_ptr<mods::orm::contracts>>& contract_list();
};

namespace mods::builder::conbuild {


	/** ===================================================================================== */
	/** ===================================================================================== */
	/** ===================================================================================== */
	using conbuild_vnum_t = uint64_t;
	using conbuild_orm_type = mods::orm::contracts;
	struct conbuild_interface : public slotted_builder<conbuild_vnum_t,conbuild_orm_type> {
		/** ======== */
		/** required */
		/** ======== */
		bool delete_by_vnum(conbuild_vnum_t vnum) {
			std::deque<std::shared_ptr<mods::orm::contracts>> list;
			bool deleted = false;
			for(auto& m : mods::orm::contract_list()) {
				if(m->vnum() == vnum) {
					m->destroy();
					deleted = true;
					continue;
				}
				list.emplace_back(std::move(m));
			}
			mods::orm::contract_list() = std::move(list);
			return deleted;
		}

		/** ======== */
		/** required */
		/** ======== */
		/** will optionally return the orm entity that has the given vnum */
		std::optional<std::shared_ptr<conbuild_orm_type>> by_vnum(conbuild_vnum_t vnum) {
			for(const auto& m : mods::orm::contract_list()) {
				if(m->c_vnum == vnum) {
					return m;
				}
			}
			return std::nullopt;
		}

		/** contract_steps */
		/** will optionally return the orm entity that has the given vnum */
		std::optional<std::shared_ptr<mods::orm::contract_steps>> step_by_vnum(conbuild_vnum_t vnum) {
			for(const auto& m : mods::orm::contract_steps_list()) {
				if(m->s_contract_vnum == vnum) {
					return m;
				}
			}
			return std::nullopt;
		}

		std::shared_ptr<mods::orm::contract_steps> create_step(conbuild_vnum_t vnum) {
			auto s = std::make_shared<mods::orm::contract_steps>();
			s->initialize_row(vnum);
			return std::move(s);
		}
		std::deque<std::shared_ptr<mods::orm::contract_steps>> step_list;
		std::optional<std::shared_ptr<mods::orm::contract_steps>> find_local_step_by_id(const uint64_t& step_id) {
			for(const auto& s : step_list) {
				if(s->id == step_id) {
					return s;
				}
			}
			return std::nullopt;
		}
		std::pair<unsigned,unsigned> foreach_step_do(std::vector<int> list,std::string mode) {
			/** signature: [0] => save-step [1] => vnum [2] => step-id ... [N] => [step-id-N] */
			unsigned ok =0, error = 0;
			uint16_t ctr = 0;
			for(const auto& step_id : list) {
				++ctr;
				auto s = find_local_step_by_id(step_id);
				if(!s.has_value()) {
					push_encoded_error(CAT("Skipping due to: Cannot find step by that step-id for argument number ",ctr + 2));
					continue;
				}
				auto& step = s.value();
				if(mode.compare("save") == 0) {
					auto status = step->update_row();
					if(ORM_SUCCESS(status)) {
						push_encoded_ok(CAT("Success for step-id:'",step->id,"'"));
						++ok;
					} else {
						push_encoded_error(CAT("FAILED for step-id:'",step->id,"'"));
						++error;
					}
				}
				if(mode.compare("delete") == 0) {
					step->destroy();
					if(ORM_SUCCESS(step->destroy_status)) {
						push_encoded_ok(CAT("Success for step-id:'",step->id,"'"));
						++ok;
					} else {
						push_encoded_error(CAT("FAILED for step-id:'",step->id,"'"));
						++error;
					}
				}
			}
			return {ok,error};
		}

		conbuild_interface() {
			/** ======== */
			/** required */
			/** ======== */
			/** set available slots */
			set_slot_list(conbuild_orm_type::get_slot_list());
			/** point the class to our base orm structure */
			set_orm_list(&mods::orm::contract_list());
			/** set the base command */
			set_base_command("conbuild");
			clear();
			load_all();
			/**
			 * gathers text from sub command arguments
			 */
			register_accumulator_command("title","<virtual_number> <text>",[](std::string&& value, std::shared_ptr<conbuild_orm_type> profile) -> std::tuple<bool,std::string> {
				profile->c_title = value;
				return {1,CAT("Title set to: '",profile->c_title,"'")};
			});
			/**
			 * gathers text from sub command arguments
			 */
			register_accumulator_command("description","<virtual_number> <text>",[](std::string&& value, std::shared_ptr<conbuild_orm_type> profile) -> std::tuple<bool,std::string> {
				profile->c_description = value;
				return {1,CAT("Description set to: '",profile->c_description,"'")};
			});

			/**
			 * ----------------------------------------------------
			 * step commands. used to manage on a per profile basis
			 * ----------------------------------------------------
			 */
			register_custom_command("new-step","<virtual_number>",[&,this](const std::vector<std::string>& args,std::string argument,std::shared_ptr<conbuild_orm_type> profile) -> std::tuple<bool,std::string> {
				this->step_list.emplace_back(create_step(profile->vnum()));
				return {1,"Created"};
			});
			register_custom_command("columns","",[&,this](const std::vector<std::string>& args,std::string argument,std::shared_ptr<conbuild_orm_type> profile) -> std::tuple<bool,std::string> {
				for(const auto& str : mods::orm::contract_steps::column_list()) {
					push_encoded_ok(str);
				}
				return {1,""};
			});

			register_custom_command("show-steps","<virtual_number>",[&,this](const std::vector<std::string>& args,std::string argument,std::shared_ptr<conbuild_orm_type> profile) -> std::tuple<bool,std::string> {
				for(const auto& s : this->step_list) {
					if(s->s_contract_vnum == profile->vnum()) {
						auto str = s->dump();
						str += CAT("task_type_t: '", IMPLODE(mods::contracts::get_string_list_from_task_flags((mods::contracts::task_t)s->s_task_type),","),"'\r\n");
						str += CAT("task_target_t: '", mods::contracts::get_string_from_target((mods::contracts::target_t)s->s_task_target),"'\r\n");
						push_encoded_ok(str);
					}
				}
				return {1,"Done listing."};
			});

			register_integral_accumulator_command("save-step","<virtual_number> <step-id>...[step-id-N]",[&,this](const std::vector<int>&& step_ids,std::shared_ptr<conbuild_orm_type> profile) -> std::tuple<bool,std::string> {
				auto p = foreach_step_do(step_ids,"save");
				this->step_list.clear();
				mods::orm::gather_contract_steps_by_contract_vnum(profile->vnum(),&this->step_list);
				return {1,CAT("saved ",std::get<0>(p)," successfully. failed saving: ",std::get<1>(p), " items")};
			});

			register_integral_accumulator_command("delete-step","<virtual_number> <step-id>...[step-id-N]",[&,this](const std::vector<int>&& step_ids,std::shared_ptr<conbuild_orm_type> profile) -> std::tuple<bool,std::string> {
				auto p = foreach_step_do(step_ids,"delete");
				this->step_list.clear();
				mods::orm::gather_contract_steps_by_contract_vnum(profile->vnum(),&this->step_list);
				return {1,CAT("deleted ",std::get<0>(p)," successfully. failed deleting: ",std::get<1>(p), " items")};
			});

			register_custom_command("load-steps","<virtual_number>",[&,this](const std::vector<std::string>& args,std::string argument,std::shared_ptr<conbuild_orm_type> profile) -> std::tuple<bool,std::string> {
				this->step_list.clear();
				auto s = mods::orm::gather_contract_steps_by_contract_vnum(profile->vnum(),&this->step_list);
				return {1,CAT("loaded ",std::get<0>(s)," entries.")};
			});

			register_custom_command("set-step-data","<virtual_number> <step-id> <field> <value>",[&,this](const std::vector<std::string>& args,std::string argument,std::shared_ptr<conbuild_orm_type> profile) -> std::tuple<bool,std::string> {
				/** signature: [0] => set-step-data [1] => vnum [2] => step-id [3] => field [4] => value */
				if(args.size() < 5) {
					return {0,"Invalid number of arguments"};
				}
				auto osid = mods::util::stoi(args[2]);
				if(!osid.has_value()) {
					return {0,"step-id must be a valid integer"};
				}
				auto step_id = osid.value();
				auto s = find_local_step_by_id(step_id);
				if(!s.has_value()) {
					return {0,"Cannot find step by that step-id"};
				}
				auto& step = s.value();
				auto& f = args[3];
				auto& v = args[4];
#define LAZY_CHECK() if(!mods::util::stoi(v).has_value()){ return {0,"Value must be a valid integer"}; }
				if(f.compare("s_contract_vnum") == 0) {
					LAZY_CHECK();
					step->s_contract_vnum = mods::util::stoi(v).value();
					return {1,"set"};
				}
				if(f.compare("s_task_type") == 0) {
					if(v.length() == 0) {
						step->s_task_type = 0;
						return {1,"cleared task type (set to zero)"};
					}
					auto task_flag = mods::contracts::parse_csv_to_task_flag(v);
					step->s_task_type = task_flag;
					return {1,CAT("set task_flag to: ",step->s_task_type,", or: '",v,"'")};
				}
				if(f.compare("s_task_target") == 0) {
					if(v.length() == 0) {
						step->s_task_target = 0;
						return {1,"cleared target type (set to zero)"};
					}
					auto target = mods::contracts::parse_target_string(v);
					step->s_task_target = target;
					return {1,CAT("set target to: ",step->s_task_target,", or: '",v,"'")};
				}
				if(f.compare("s_description") == 0) {
					step->s_description = this->accumulate_from(args,4);
					return {1,"set"};
				}
				if(f.compare("s_mob_vnum") == 0) {
					LAZY_CHECK();
					step->s_mob_vnum = mods::util::stoi(v).value();
					return {1,"set"};
				}
				if(f.compare("s_room_vnum") == 0) {
					LAZY_CHECK();
					step->s_room_vnum = mods::util::stoi(v).value();
					return {1,"set"};
				}
				if(f.compare("s_quota") == 0) {
					LAZY_CHECK();
					step->s_quota = mods::util::stoi(v).value();
					return {1,"set"};
				}
				if(f.compare("s_is_optional") == 0) {
					LAZY_CHECK();
					step->s_is_optional = mods::util::stoi(v).value();
					return {1,"set"};
				}
				if(f.compare("s_order") == 0) {
					LAZY_CHECK();
					step->s_order = mods::util::stoi(v).value();
					return {1,"set"};
				}
				if(f.compare("s_object_ymal") == 0) {
					step->s_object_yaml = v;
					return {1,"set"};
				}
				return {0,"nothing set"};
			});
		}

		/** ======== */
		/** required */
		/** ======== */
		/** in order for the "new" command to work, we must implement this */
		status_response_t dispatch_new_command(const std::vector<std::string>& cmd_args,std::string argument) {
			/**
			 * cmd_args: [0] => "new" [1] => vnum
			 */
			if(cmd_args.size() < 4) {
				return {0,"Error: not enough arguments"};
			}
			auto vnum = extract_int<int>("new",argument.c_str(),1).value_or(-1);
			if(vnum < 0) {
				return {0,"vnum must be a positive number"};
			}
			auto c = std::make_shared<mods::orm::contracts>();
			auto id = c->initialize_row(vnum,"title","description");
			if(id <= 0) {
				return {0,"Couldn't initialize row"};
			}
			mods::orm::contract_list().emplace_back(std::move(c));
			return {1,"Created row."};
		}
		/** ======== */
		/** required */
		/** ======== */
		void clear() {
			mods::orm::contract_list().clear();
			mods::orm::contract_steps_list().clear();
		}
		/** ======== */
		/** required */
		/** ======== */
		void load_all() {
			mods::orm::contract_list() = std::move(mods::orm::load_all_by_table<mods::orm::contracts>());
			mods::orm::contract_steps_list() = std::move(mods::orm::load_all_by_table<mods::orm::contract_steps>());
		}
	};

	namespace map {

		bool delete_by_vnum(const uint64_t& vnum) {
			std::deque<std::shared_ptr<mods::orm::contract_steps>> list;
			bool deleted = false;
			for(auto& m : mods::orm::contract_steps_list()) {
				if(m->s_contract_vnum == vnum) {
					m->destroy();
					deleted = true;
					continue;
				}
				list.emplace_back(std::move(m));
			}
			mods::orm::contract_steps_list() = std::move(list);
			return deleted;
		}
		std::deque<std::shared_ptr<mods::orm::contract_steps>> by_profile_vnum(const uint64_t& vnum) {
			std::deque<std::shared_ptr<mods::orm::contract_steps>> list;
			for(const auto& m : mods::orm::contract_steps_list()) {
				if(m->s_contract_vnum == vnum) {
					list.emplace_back(m);
				}
			}
			return list;
		}
	};
	using args_t = std::vector<std::string>;

	conbuild_interface& conbuilder(player_ptr_t player) {
		static conbuild_interface interface;
		interface.set_builder(player);
		return interface;
	}

	ACMD(do_conbuild) {
		mods::builder::initialize_builder(player);

		if(conbuilder(player).handle_input(argument)) {
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
				for(const auto& m : mods::orm::contract_steps_list()) {
					list += CAT("{contract_vnum:",m->s_contract_vnum,",todo:1}");
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
#if 0
				for(auto& m : mods::orm::contract_steps_list()) {
					/*
					if(m->mmap_mob_vnum == mvn) {
						auto backup = m->mmap_contracts_vnum;
						m->mmap_contracts_vnum = eq_vnum;
						if(m->save() < 0) {
							r_error(player,"Unable to save existing.");
							m->mmap_contracts_vnum = backup;
							return;
						}
						*/
					r_success(player,"Saved existing.");
					ENCODE_R("ok");
					return;
					//}
				}
				/** we've reached here which means no existing map exists. create one */
				auto ref = std::make_shared<mods::orm::contract_steps>();
				ref->initialize_row(mvn,eq_vnum);
				ref->mmap_mob_vnum = mvn;
				ref->mmap_contracts_vnum = eq_vnum;
				if(ref->save() < 0) {
					r_error(player,"Unable to save.");
					return;
				}
				mods::orm::contracts_map_list().emplace_back(std::move(ref));
#endif
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
#if 0
				std::string list = "";
				std::deque<std::shared_ptr<mods::orm::contract_steps>> after;
				for(unsigned i=1; i < cmd_args.size(); ++i) {
					auto mvn = mods::util::stoi(cmd_args[i]).value_or(-1);
					if(mvn < 0) {
						r_error(player,"mob vnum must be a positive number");
						continue;
					}
					for(auto& m : mods::orm::contracts_map_list()) {
						if(m->mmap_mob_vnum == mvn) {
							m->destroy();
							m->destroyed = true;
							list += CAT("{",mvn,"}");
							continue;
						}
					}
				}
				for(auto& m : mods::orm::contracts_map_list()) {
					if(m->destroyed) {
						continue;
					}
					after.emplace_back(std::move(m));
				}
				mods::orm::contracts_map_list() = std::move(after);
				r_success(player,CAT("Deleted: ",list));
				ENCODE_R(list);
#endif
				return;
			}//end pave on
		}
	}	//end conbuild
	void init() {
		mods::interpreter::add_command("conbuild", POS_RESTING, do_conbuild, LVL_BUILDER,0);
		conbuilder(nullptr);
	}
};
