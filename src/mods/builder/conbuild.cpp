#include "conbuild.hpp"
#include "../interpreter.hpp"
#include "../util.hpp"
#include "../builder_util.hpp"
#include <map>
#include "../orm/contracts.hpp"
#include "../orm/contract-steps.hpp"
#include "../orm/player-contract-state.hpp"
#include "../contracts.hpp"
#include "../contract-steps.hpp"
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
			for(auto& m : mods::orm::orm_contract_list()) {
				if(m->vnum() == vnum) {
					m->destroy();
					deleted = true;
					continue;
				}
				list.emplace_back(std::move(m));
			}
			mods::orm::orm_contract_list() = std::move(list);
			return deleted;
		}

		/** ======== */
		/** required */
		/** ======== */
		/** will optionally return the orm entity that has the given vnum */
		std::optional<std::shared_ptr<conbuild_orm_type>> by_vnum(conbuild_vnum_t vnum) {
			for(const auto& m : mods::orm::orm_contract_list()) {
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
			/** signature: [0] => save-step [1] => vnum [2] => Nth-step-id ... [N] => [step-id-N] */
			unsigned ok =0, error = 0;
			uint16_t ctr = 0;
			for(const auto& step_id : list) {
				++ctr;
				if(step_id >= step_list.size()) {
					push_encoded_error(CAT("error. step_id out of bounds: ",step_id, ". Skipping..."));
					continue;
				}

				auto& step = step_list[step_id];
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
		void dump_step(std::shared_ptr<mods::orm::contract_steps> s) {
			auto str = s->dump();
			str += CAT("task_type_t: '", IMPLODE(mods::contracts::get_string_list_from_task_flags((mods::contracts::task_t)s->s_task_type),","),"'\r\n");
			str += CAT("task_target_t: '", mods::contracts::get_string_from_target((mods::contracts::target_t)s->s_task_target),"'\r\n");
			push_encoded_ok(str);
		}

		conbuild_interface() {
			/** ======== */
			/** required */
			/** ======== */
			/** set available slots */
			set_slot_list(conbuild_orm_type::get_slot_list());
			/** point the class to our base orm structure */
			set_orm_list(&mods::orm::orm_contract_list());
			/** set the base command */
			set_base_command("conbuild");
			clear();
			load_all();
			remove_command_signatures({"list-extract","reload-all","remove","set"});
			get_signatures()["new"] = "{grn}conbuild{/grn} {red}new <virtual-number>{/red}\r\n";
			register_custom_non_profile_command("legend",[&,this](const std::vector<std::string>& args,std::string argument) -> std::tuple<bool,std::string> {
				std::string legend = CAT("{yel}LEGEND:{/yel}\r\n",
				                         "{yel}virtual-number{/yel}: {grn}The contract virtual number.{/grn}\r\n",
				                         "{yel}Nth-step{/yel}: {grn}The zero-indexed position of the step.{/grn}\r\n",
				                         "{yel}field{/yel}: {grn}A specific column on the contract_steps table. Use this to see a list of valid columns: 'conbuild columns'.{/grn}\r\n",
				                         "{yel}text{/yel}: {grn}One or more words separated by spaces.{/grn}\r\n",
				                         "{yel}---------------------------------------------------------------------------{/yel}\r\n",
				                         "Important concepts:\r\n",
				                         "When using the {yel}load-steps{/yel} command, a structure is loaded and is exclusive to you.\r\n",
				                         "This exclusive structure has all the steps loaded for the given contract virtual number that you specify.\r\n",
				                         "This does not mean that all other steps don't exist. They merely aren't included when you call various \r\n",
				                         "commands that depend on that step structure.\r\n",
				                         "Example workflow:\r\n",
				                         "{grn}conbuild new 400{/grn} {yel}# this creates a new contract with the virtual number of 400{/yel}\r\n",
				                         "{grn}conbuild new-step 400{/grn} {yel}# this creates a new step for the contract we just created.{/yel}\r\n",
				                         "{grn}conbuild load-steps 400{/grn} {yel}# this loads the step we just created into our exclusive structure.{/yel}\r\n",
				                         "{grn}conbuild show-steps 400{/grn} {yel}# this will now display the step we just created and loaded.{/yel}\r\n",
				                         "\r\n",
				                         "Below is an example output:\r\n",
				                         "-----------------------------------------------\r\n",
				                         " START EXAMPLE\r\n",
				                         "-----------------------------------------------\r\n",
				                         "[success]: [id]:->'218'\r\n",
				                         "[s_contract_vnum]->'400'\r\n",
				                         "[s_description]->'description'\r\n",
				                         "[s_is_optional]->'0'\r\n",
				                         "[s_mob_vnum]->'0'\r\n",
				                         "[s_object_yaml]->''\r\n",
				                         "[s_order]->'0'\r\n",
				                         "[s_quota]->'0'\r\n",
				                         "[s_room_vnum]->'0'\r\n",
				                         "[s_task_target]->'0'\r\n",
				                         "[s_task_type]->'0'\r\n",
				                         "task_type_t: ''\r\n",
				                         "task_target_t: 'TARGET_MOB'\r\n",
				                         "\r\n",
				                         "[success]: Done listing.\r\n",
				                         "-----------------------------------------------\r\n",
				                         " END EXAMPLE\r\n",
				                         "-----------------------------------------------\r\n",
				                         "\r\n",
				                         "We know that there is only one step in our contract, so in order to modify the data on that step we address it as zero.\r\n"
				                         "{grn}conbuild set-step-data 400 0 s_description this is my test description{/grn} {yel}# set the description {/yel}\r\n",
				                         "{grn}conbuild set-step-data 400 0 s_task_type GOAL_FIND{/grn} {yel}# set the task type {/yel}\r\n",
				                         "{grn}conbuild save-step 400 0{/grn} {yel}# save our step {/yel}\r\n",
				                         "If we were to add another step we would use the index 1 instead of zero because now there will be two steps in the contract.\r\n",
				                         "\r\n"
				                        );
				push_encoded_ok(legend);
				return {1,""};
			});
			register_custom_command("help","",[this](const std::vector<std::string>& args,std::string argument,std::shared_ptr<conbuild_orm_type> profile) -> std::tuple<bool,std::string> {
				display_signatures();
				std::string msg = CAT("\r\n",
				                      "{grn}Step Goals:{/grn}\r\n"
				                     );
				for(const auto& pair : mods::contracts::task_string_map()) {
					msg += CAT("{yel}",pair.first,"{/yel}\r\n");
				}
				msg += CAT("{grn}Step Targets:{/gnr}\r\n");
				for(const auto& pair : mods::contracts::target_string_map()) {
					msg += CAT("{yel}",pair.first,"{/yel}\r\n");
				}
				msg += CAT("\r\n",
				           "Example: How to set multiple types on a step's task_type_t column:\r\n",
				           "{grn}conbuild set-step-data 400 0 s_task_type GOAL_QUOTA,GOAL_KILL{/grn}\r\n",
				           "Example: How to set step target:\r\n",
				           "{grn}conbuild set-step-data 400 0 s_task_target TARGET_MOB{/grn}\r\n",
				           "\r\n"
				          );

				push_encoded_ok(msg);
				return {1,""};
			});
			/**
			 * ==========================================
			 * title <vnum> <text>...
			 * ==========================================
			 * [description]
			 *  -----------------------------------------
			 * 	sets the title on the given contract
			 *  -----------------------------------------
			 */
			register_accumulator_command("title","<virtual_number> <text>",[](std::string&& value, std::shared_ptr<conbuild_orm_type> profile) -> std::tuple<bool,std::string> {
				profile->c_title = value;
				return profile->update();
			});
			/**
			 * ==========================================
			 * description <vnum> <text>...
			 * ==========================================
			 * [description]
			 *  -----------------------------------------
			 * 	sets the description on the given contract
			 *  -----------------------------------------
			 */
			register_accumulator_command("description","<virtual_number> <text>",[](std::string&& value, std::shared_ptr<conbuild_orm_type> profile) -> std::tuple<bool,std::string> {
				profile->c_description = value;
				return profile->update();
			});

			/**
			 * ==========================================
			 * new-step <vnum>
			 * ==========================================
			 * [description]
			 *  -----------------------------------------
			 * 	creates a new step for the given contract vnum
			 *  -----------------------------------------
			 */
			register_custom_command("new-step","<virtual_number>",[&,this](const std::vector<std::string>& args,std::string argument,std::shared_ptr<conbuild_orm_type> profile) -> std::tuple<bool,std::string> {
				this->step_list.emplace_back(create_step(profile->vnum()));
				return {1,"Created"};
			});
			/**
			 * ==========================================
			 * columns
			 * ==========================================
			 * [description]
			 *  -----------------------------------------
			 * 	lists the columns on the steps orm object
			 *  -----------------------------------------
			 */
			register_custom_command("columns","",[&,this](const std::vector<std::string>& args,std::string argument,std::shared_ptr<conbuild_orm_type> profile) -> std::tuple<bool,std::string> {
				for(const auto& str : mods::orm::contract_steps::column_list()) {
					push_encoded_ok(str);
				}
				return {1,""};
			});
			/**
			 * ==========================================
			 * extract-step <vnum> <field>...[field-N]
			 * ==========================================
			 * [description]
			 *  -----------------------------------------
			 * 	grabs the specified field(s) on the steps for contract vnum
			 *  -----------------------------------------
			 */
			register_custom_command("extract-step","<virtual_number> <field>...[field-N]",[&,this](const std::vector<std::string>& args,std::string argument,std::shared_ptr<conbuild_orm_type> profile) -> std::tuple<bool,std::string> {
				/** signature: [0] => extract-step [1] => vnum [2] => field ... [N] => [field-N] */
				// grab step
				if(args.size() < 3) {
					return {0,"Invalid number of arguments"};
				}

				for(const auto& s : this->step_list) {
					if(s->s_contract_vnum != profile->vnum()) {
						continue;
					}
					auto smap = s->export_class();
					for(int i = 2; i < args.size(); i++) {
						if(args[i].compare("s_task_type") == 0) {
							push_encoded_ok(IMPLODE(mods::contracts::get_string_list_from_task_flags((mods::contracts::task_t)s->s_task_type),","));
							continue;
						}
						if(args[i].compare("s_task_target") == 0) {
							push_encoded_ok(mods::contracts::get_string_from_target((mods::contracts::target_t)s->s_task_target));
							continue;
						}
						push_encoded_ok(smap[args[i]]);
					}
				}
				return {1,"Done listing."};
			});

			/**
			 * ==========================================
			 * paginate-steps <vnum> <page> <page-size>
			 * ==========================================
			 * [description]
			 *  -----------------------------------------
			 * 	paginates the steps
			 *  -----------------------------------------
			 */
			register_custom_command("paginate-steps","<virtual_number> <page> <page-size>",[&,this](const std::vector<std::string>& args,std::string argument,std::shared_ptr<conbuild_orm_type> profile) -> std::tuple<bool,std::string> {
				/** signature: [0] => paginate-steps [1] => vnum [2] => page [3] => page-size */
				// grab step
				if(args.size() < 4) {
					return {0,"Invalid number of arguments"};
				}
				auto opage = mods::util::stoi(args[2]);
				if(!opage.has_value()) {
					return {0,"page must be a valid integer"};
				}
				auto page = opage.value();

				auto opage_size = mods::util::stoi(args[3]);
				if(!opage_size.has_value()) {
					return {0,"page size must be a valid integer"};
				}
				auto page_size = opage_size.value();

				int offset = page * page_size;

				std::vector<std::shared_ptr<mods::orm::contract_steps>> full_list;
				for(const auto& s : this->step_list) {
					if(s->s_contract_vnum == profile->vnum()) {
						full_list.emplace_back(s);
					}
				}
				if(full_list.size() <= offset) {
					return {1,CAT("Out of bounds. Nothing to list. list size is: ",full_list.size())};
				}
				auto start = full_list.cbegin() + offset;
				for(int ctr =0; start != full_list.end() && ctr < page_size; ++start,++ctr) {
					dump_step(*start);
				}
				return {1,"Done listing."};
			});
			/**
			 * ==========================================
			 * show-steps <vnum>
			 * ==========================================
			 * [description]
			 *  -----------------------------------------
			 * 	shows the steps for the given contract vnum
			 *  -----------------------------------------
			 */
			register_custom_command("show-steps","<virtual_number>",[&,this](const std::vector<std::string>& args,std::string argument,std::shared_ptr<conbuild_orm_type> profile) -> std::tuple<bool,std::string> {
				for(const auto& s : this->step_list) {
					if(s->s_contract_vnum == profile->vnum()) {
						dump_step(s);
					}
				}
				return {1,"Done listing."};
			});

			/**
			 * ==========================================
			 * save-step <vnum> <Nth-step-id>...[step-id-N]
			 * ==========================================
			 * [description]
			 *  -----------------------------------------
			 * 	saves the step by step id (step->id)
			 *  -----------------------------------------
			 */
			register_integral_accumulator_command("save-step","<virtual_number> <Nth-step-id>...[Nth-step-id-N]",[&,this](const std::vector<int>&& step_ids,std::shared_ptr<conbuild_orm_type> profile) -> std::tuple<bool,std::string> {
				auto p = foreach_step_do(step_ids,"save");
				this->step_list.clear();
				mods::orm::gather_contract_steps_by_contract_vnum(profile->vnum(),&this->step_list);
				return {1,CAT("saved ",std::get<0>(p)," successfully. failed saving: ",std::get<1>(p), " items")};
			});

			/**
			 * ==========================================
			 * delete-step <vnum> <step-id>...[step-id-N]
			 * ==========================================
			 * [description]
			 *  -----------------------------------------
			 * 	deletes the step by step id (step->id)
			 *  -----------------------------------------
			 */
			register_integral_accumulator_command("delete-step","<virtual_number> <step-id>...[step-id-N]",[&,this](const std::vector<int>&& step_ids,std::shared_ptr<conbuild_orm_type> profile) -> std::tuple<bool,std::string> {
				auto p = foreach_step_do(step_ids,"delete");
				this->step_list.clear();
				mods::orm::gather_contract_steps_by_contract_vnum(profile->vnum(),&this->step_list);
				return {1,CAT("deleted ",std::get<0>(p)," successfully. failed deleting: ",std::get<1>(p), " items")};
			});

			/**
			 * ==========================================
			 * load-steps <vnum>
			 * ==========================================
			 * [description]
			 *  -----------------------------------------
			 * 	loads the contract vnum's steps into local step storage.
			 * 	ANY steps that do not have a contract of vnum will be
			 * 	removed from said storage. those items will still be in
			 * 	postgres if saved prior to this command being invoked.
			 *  -----------------------------------------
			 */
			register_custom_command("load-steps","<virtual_number>",[&,this](const std::vector<std::string>& args,std::string argument,std::shared_ptr<conbuild_orm_type> profile) -> std::tuple<bool,std::string> {
				this->step_list.clear();
				auto s = mods::orm::gather_contract_steps_by_contract_vnum(profile->vnum(),&this->step_list);
				return {1,CAT("loaded ",std::get<0>(s)," entries.")};
			});

			/**
			 * ==============================================
			 * set-step-data <vnum> <Nth-step> <field> <value>
			 * ==============================================
			 * [description]
			 *  -----------------------------------------
			 * 	sets the column (field) to the value. to see a list
			 * 	of columns call the columns sub-command
			 * 	-----------------------------------------
			 */
			register_custom_command("set-step-data","<virtual_number> <Nth-step> <field> <value>",[&,this](const std::vector<std::string>& args,std::string argument,std::shared_ptr<conbuild_orm_type> profile) -> std::tuple<bool,std::string> {
				/** signature: [0] => set-step-data [1] => vnum [2] => Nth-step [3] => field [4] => value */
				if(args.size() < 5) {
					return {0,"Invalid number of arguments"};
				}
				if(step_list.size() == 0) {
					return {0,"no steps present on this contract"};
				}
				auto osid = mods::util::stoi(args[2]);
				if(!osid.has_value()) {
					return {0,"step-id must be a valid integer"};
				}
				auto step_id = osid.value();
				if(step_id >= step_list.size()) {
					return {0,CAT("Nth-step must be between 0 and ",step_list.size() -1)};
				}
				auto& step = step_list[step_id];
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
				if(f.compare("s_object_yaml") == 0) {
					step->s_object_yaml = v;
					return {1,"set"};
				}
				if(f.compare("s_reward_1") == 0) {
					step->s_reward_1 = v;
					return {1,"set"};
				}
				if(f.compare("s_reward_2") == 0) {
					step->s_reward_2 = v;
					return {1,"set"};
				}
				if(f.compare("s_reward_3") == 0) {
					step->s_reward_3 = v;
					return {1,"set"};
				}
				if(f.compare("s_reward_4") == 0) {
					step->s_reward_4 = v;
					return {1,"set"};
				}
				if(f.compare("s_reward_5") == 0) {
					step->s_reward_5 = v;
					return {1,"set"};
				}
				if(f.compare("s_reward_6") == 0) {
					step->s_reward_6 = v;
					return {1,"set"};
				}
				if(f.compare("s_reward_7") == 0) {
					step->s_reward_7 = v;
					return {1,"set"};
				}
				if(f.compare("s_reward_8") == 0) {
					step->s_reward_8 = v;
					return {1,"set"};
				}
				if(f.compare("s_reward_9") == 0) {
					step->s_reward_9 = v;
					return {1,"set"};
				}
				if(f.compare("s_reward_10") == 0) {
					step->s_reward_10 = v;
					return {1,"set"};
				}
				if(f.compare("s_reward_money") == 0) {
					step->s_reward_money = mods::util::stoi(v).value_or(0);
					return {1,"set"};
				}
				if(f.compare("s_reward_xp") == 0) {
					step->s_reward_xp = mods::util::stoi(v).value_or(0);
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
			if(cmd_args.size() < 2) {
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
			mods::orm::orm_contract_list().emplace_back(std::move(c));
			return {1,"Created row."};
		}
		/** ======== */
		/** required */
		/** ======== */
		void clear() {
			mods::orm::orm_contract_list().clear();
			mods::orm::contract_steps_list().clear();
		}
		/** ======== */
		/** required */
		/** ======== */
		void load_all() {
			mods::orm::orm_contract_list() = std::move(mods::orm::load_all_by_table<mods::orm::contracts>());
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

	static conbuild_interface& conbuilder(player_ptr_t player) {
		static conbuild_interface interface;
		interface.set_builder(player);
		return interface;
	}

	ACMD(do_conbuild) {
		mods::builder::initialize_builder(player);

		auto handled = conbuilder(player).handle_input(argument);
		conbuilder(player).flush();
		if(handled) {
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
