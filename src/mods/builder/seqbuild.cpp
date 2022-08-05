#include "seqbuild.hpp"
#include "../interpreter.hpp"
#include "../util.hpp"
#include "../builder_util.hpp"
#include <map>
#include "../orm/scripted-step.hpp"
#include "../orm/contract-steps.hpp"
#include "../orm/contracts.hpp"
#include "../contracts.hpp"
#include "../contract-steps.hpp"
#include "../contract-types.hpp"

#include "slotted-builder.hpp"

namespace mods::builder::seqbuild {

	bool contract_exists(int vnum) {
		for(const auto& c : mods::orm::orm_contract_list()) {
			if(c->vnum() == vnum) {
				return true;
			}
		}
		return false;
	}
	bool sequence_exists(int vnum) {
		for(const auto& c : mods::orm::scripted_sequences_list()) {
			if(c->vnum() == vnum) {
				return true;
			}
		}

		return false;
	}

	std::deque<std::shared_ptr<mods::orm::scripted_step>> steps_by_sequence_vnum(int vnum) {
		std::deque<std::shared_ptr<mods::orm::scripted_step>> list;
		for(const auto& c : mods::orm::scripted_step_list()) {
			if(c->s_sequence_vnum == vnum) {
				list.emplace_back(c);
			}
		}

		return list;
	}

	/** ===================================================================================== */
	/** ===================================================================================== */
	/** ===================================================================================== */
	using seqbuild_vnum_t = uint64_t;
	using seqbuild_orm_type = mods::orm::scripted_sequences;
	struct seqbuild_interface : public slotted_builder<seqbuild_vnum_t,seqbuild_orm_type> {
		/** ======== */
		/** required */
		/** ======== */
		bool delete_by_vnum(seqbuild_vnum_t vnum) {
			std::deque<std::shared_ptr<mods::orm::scripted_sequences>> list;
			bool deleted = false;
			for(auto& m : mods::orm::scripted_sequences_list()) {
				if(m->vnum() == vnum) {
					m->destroy();
					deleted = true;
					continue;
				}
				list.emplace_back(std::move(m));
			}
			mods::orm::scripted_sequences_list() = std::move(list);
			return deleted;
		}

		/** ======== */
		/** required */
		/** ======== */
		/** will optionally return the orm entity that has the given vnum */
		std::optional<std::shared_ptr<seqbuild_orm_type>> by_vnum(seqbuild_vnum_t vnum) {
			for(const auto& m : mods::orm::scripted_sequences_list()) {
				if(m->vnum() == vnum) {
					return m;
				}
			}
			return std::nullopt;
		}
		static mods::orm::scripted_step& temp_step() {
			static mods::orm::scripted_step orm;
			return orm;
		}
		std::vector<std::string> step_column_list() const {
			return mods::orm::scripted_step::get_slot_list();
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

		/** scripted_steps */
		/** will optionally return the orm entity that has the given vnum */
		std::optional<std::shared_ptr<mods::orm::scripted_step>> step_by_vnum(seqbuild_vnum_t vnum) {
			for(const auto& m : mods::orm::scripted_step_list()) {
				if(m->s_sequence_vnum == vnum) {
					return m;
				}
			}
			return std::nullopt;
		}

		std::deque<std::shared_ptr<mods::orm::scripted_step>> step_list;
		std::optional<std::shared_ptr<mods::orm::scripted_step>> find_local_step_by_id(const uint64_t& step_id) {
			for(const auto& s : step_list) {
				if(s->id == step_id) {
					return s;
				}
			}
			return std::nullopt;
		}
		std::pair<unsigned,unsigned> foreach_step_do(std::string_view mode) {
			/** signature: [0] => save-step [1] => vnum [2] => Nth-step-id ... [N] => [step-id-N] */
			unsigned ok =0, error = 0;
			for(auto& step : step_list) {
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
			}
			return {ok,error};
		}
		std::pair<unsigned,unsigned> foreach_step_do(const std::vector<int>& list,std::string_view mode) {
			/** signature: [0] => save-step [1] => vnum [2] => Nth-step-id ... [N] => [step-id-N] */
			unsigned ok =0, error = 0;
			uint16_t ctr = 0;
			for(const auto& step_id : list) {
				++ctr;
				if(step_id >= step_list.size()) {
					push_encoded_error(CAT("error. step_id out of bounds: ",step_id, ". Skipping..."));
					continue;
				}

				if(mode.compare("save") == 0) {
					auto status = this->step_list[step_id]->update_row();
					if(ORM_SUCCESS(status)) {
						push_encoded_ok(CAT("Success for step-id:'",this->step_list[step_id]->id,"'"));
						++ok;
					} else {
						push_encoded_error(CAT("FAILED for step-id:'",this->step_list[step_id]->id,"'"));
						++error;
					}
				}
				if(mode.compare("delete") == 0) {
					this->step_list[step_id]->destroy();
					if(ORM_SUCCESS(this->step_list[step_id]->destroy_status)) {
						push_encoded_ok(CAT("Success for step-id:'",this->step_list[step_id]->id,"'"));
						++ok;
					} else {
						push_encoded_error(CAT("FAILED for step-id:'",this->step_list[step_id]->id,"'"));
						++error;
					}
				}
			}
			return {ok,error};
		}
		void dump_step(std::shared_ptr<mods::orm::scripted_step> s) {
			auto str = s->dump();
			//str += CAT("task_type_t: '", IMPLODE(mods::contracts::get_string_list_from_task_flags((mods::contracts::task_t)s->s_task_type),","),"'\r\n");
			//str += CAT("task_target_t: '", mods::contracts::get_string_from_target((mods::contracts::target_t)s->s_task_target),"'\r\n");
			push_encoded_ok(str);
		}

		seqbuild_interface() {
			/** ======== */
			/** required */
			/** ======== */
			/** set available slots */
			set_slot_list(seqbuild_orm_type::get_slot_list());
			/** point the class to our base orm structure */
			set_orm_list(&mods::orm::scripted_sequences_list());
			/** set the base command */
			set_base_command("seqbuild");
			clear();
			load_all();
			remove_command_signatures({"list-extract","reload-all","remove","set"});
			get_signatures()["new"] = "{grn}seqbuild{/grn} {red}new <virtual-number>{/red}\r\n";
			register_custom_non_profile_command("legend",[&,this](const std::vector<std::string>& args,std::string argument) -> std::tuple<bool,std::string> {
				std::string legend = CAT("{yel}LEGEND:{/yel}\r\n",
				                         "{yel}virtual-number{/yel}: {grn}The contract virtual number.{/grn}\r\n",
				                         "{yel}Nth-step{/yel}: {grn}The zero-indexed position of the step.{/grn}\r\n",
				                         "{yel}field{/yel}: {grn}A specific column on the scripted_steps table. Use this to see a list of valid columns: 'seqbuild columns'.{/grn}\r\n",
				                         "{yel}text{/yel}: {grn}One or more words separated by spaces.{/grn}\r\n",
				                         "{yel}---------------------------------------------------------------------------{/yel}\r\n",
				                         "Important concepts:\r\n",
				                         "When using the {yel}load-steps{/yel} command, a structure is loaded and is exclusive to you.\r\n",
				                         "This exclusive structure has all the steps loaded for the given contract virtual number that you specify.\r\n",
				                         "This does not mean that all other steps don't exist. They merely aren't included when you call various \r\n",
				                         "commands that depend on that step structure.\r\n",
				                         "Example workflow:\r\n",
				                         "{grn}seqbuild new 400{/grn} {yel}# this creates a new contract with the virtual number of 400{/yel}\r\n",
				                         "{grn}seqbuild new-step 400{/grn} {yel}# this creates a new step for the contract we just created.{/yel}\r\n",
				                         "{grn}seqbuild load-steps 400{/grn} {yel}# this loads the step we just created into our exclusive structure.{/yel}\r\n",
				                         "{grn}seqbuild show-steps 400{/grn} {yel}# this will now display the step we just created and loaded.{/yel}\r\n",
				                         "\r\n",
				                         "Below is an example output:\r\n",
				                         "-----------------------------------------------\r\n",
				                         " START EXAMPLE\r\n",
				                         "-----------------------------------------------\r\n",
				                         "[success]: [id]:->'218'\r\n",
				                         "[s_sequence_vnum]->'400'\r\n",
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
				                         "{grn}seqbuild set-step-data 400 0 s_description this is my test description{/grn} {yel}# set the description {/yel}\r\n",
				                         "{grn}seqbuild set-step-data 400 0 s_task_type GOAL_FIND{/grn} {yel}# set the task type {/yel}\r\n",
				                         "{grn}seqbuild save-step 400 0{/grn} {yel}# save our step {/yel}\r\n",
				                         "{grn}seqbuild save-all-steps 400{/grn} {yel}# save ALL steps {/yel}\r\n",
				                         "If we were to add another step we would use the index 1 instead of zero because now there will be two steps in the contract.\r\n",
				                         "{grn}seqbuild callback <contract-vnum> <task-type> <task-target> <task-vnum> <sequence-vnum>{/grn} {yel}# create a callback on the contract step that triggers a sequence{/yel}\r\n",
				                         "{grn}seqbuild reload-orm{/grn} {yel}# reloads sequences, sequence steps, and callbacks into their respective orm objects{/yel}\r\n",
				                         "\r\n"
				                        );
				push_encoded_ok(legend);
				return {1,""};
			});
			register_manual_command("reload-orm","",[&](const std::string& argument) -> std::tuple<bool,std::string> {
				return mods::orm::load_all_scripted_data();
			});
			/**
			 * ==========================================
			 * callback <contract-vnum> <task-type> <task-target> <task-vnum> <sequence-vnum>
			 * ==========================================
			 * [description]
			 *  --------------------------------------------------------------------------------
			 * 	creates a callback for when the specified contract's step matches the task data.
			 * 	that callback will trigger the scripted_sequence identified by 'sequence-vnum'.
			 *  --------------------------------------------------------------------------------
			 */
			register_manual_command("callback","<contract-vnum> <task-type> <task-target> <task-vnum> <sequence-vnum>",[&,this](const std::string& argument) -> std::tuple<bool,std::string> {
				auto& player = m_builder_ptr;
				player->sendln(CAT("intat(1):",intat(1),",argat(1):",argat(1),",argat(2):",argat(2)));
				if(argshave()->int_at({1,4,5})->size_gt(5)->passed() == false) {
					return {0,"Not enough arguments"};
				}
				mods::orm::contract_step_callback orm;
				auto id = orm.initialize_row(intat(1),argat(2),argat(3),intat(4),intat(5));
				if(id > 0) {
					return {1,"Successfully created"};
				}
				return {0,"Something went wrong"};
			});
			register_custom_command("help","",[this](const std::vector<std::string>& args,std::string argument,std::shared_ptr<seqbuild_orm_type> profile) -> std::tuple<bool,std::string> {
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
				           "{grn}seqbuild set-step-data 400 0 s_task_type GOAL_QUOTA,GOAL_KILL{/grn}\r\n",
				           "Example: How to set step target:\r\n",
				           "{grn}seqbuild set-step-data 400 0 s_task_target TARGET_MOB{/grn}\r\n",
				           "\r\n"
				          );

				push_encoded_ok(msg);
				return {1,""};
			});
			/**
			 * ==========================================
			 * dialogue <vnum> <index> <text>...
			 * ==========================================
			 * [dialogue]
			 *  -----------------------------------------
			 * 	sets the dialogue on the given step
			 *  -----------------------------------------
			 */
			register_indexed_accumulator_command("dialogue","<virtual_number> <index> <text>",[&](std::string&& value, std::shared_ptr<seqbuild_orm_type> profile, int index) -> std::tuple<bool,std::string> {
				if(!profile) {
					return {0,"Invalid or missing profile vnum"};
				}
				if(index < 0) {
					return {0,"Invalid index"};
				}
				if(index >= this->step_list.size()) {
					return {0,"Index out of bounds"};
				}
				this->step_list[index]->s_dialogue.assign(value);
				return {1,"Dialogue successfully set"};
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
			register_custom_command("new-step","<virtual_number>",[&,this](const std::vector<std::string>& args,std::string argument,std::shared_ptr<seqbuild_orm_type> profile) -> std::tuple<bool,std::string> {
				if(!profile) {
					return {0,"Invalid or missing profile vnum"};
				}
				step_list.emplace_back(std::make_shared<mods::orm::scripted_step>());
				auto& step = step_list.back();
				step->initialize_row(profile->vnum(),"placeholder");
				return {1,CAT("Created step (pkid:",step->id,") for sequence vnum: ",profile->vnum(),". Step count is now: ",step_list.size())};
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
			register_custom_command("columns","",[&,this](const std::vector<std::string>& args,std::string argument,std::shared_ptr<seqbuild_orm_type> profile) -> std::tuple<bool,std::string> {
				for(const auto& str : mods::orm::scripted_sequences::column_list()) {
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
			register_custom_command("extract-step","<virtual_number> <field>...[field-N]",[&,this](const std::vector<std::string>& args,std::string argument,std::shared_ptr<seqbuild_orm_type> profile) -> std::tuple<bool,std::string> {
				if(!profile) {
					return {0,"Invalid or missing profile vnum"};
				}
				/** signature: [0] => extract-step [1] => vnum [2] => field ... [N] => [field-N] */
				// grab step
				if(args.size() < 3) {
					return {0,"Invalid number of arguments"};
				}

				for(const auto& s : this->step_list) {
					if(s->s_sequence_vnum != profile->vnum()) {
						continue;
					}
					auto smap = s->export_class();
					/** TODO DO THIS
					 */
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
			register_custom_command("paginate-steps","<virtual_number> <page> <page-size>",[&,this](const std::vector<std::string>& args,std::string argument,std::shared_ptr<seqbuild_orm_type> profile) -> std::tuple<bool,std::string> {
				if(!profile) {
					return {0,"Invalid or missing profile vnum"};
				}
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

				std::vector<std::shared_ptr<mods::orm::scripted_step>> full_list;
				for(const auto& s : this->step_list) {
					if(s->s_sequence_vnum == profile->vnum()) {
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
			register_custom_command("show-steps","<virtual_number>",[&,this](const std::vector<std::string>& args,std::string argument,std::shared_ptr<seqbuild_orm_type> profile) -> std::tuple<bool,std::string> {
				if(!profile) {
					return {0,"Invalid or missing profile vnum"};
				}
				std::size_t ctr = 0;
				for(const auto& step : step_list) {
					player->sendln(CAT("{grn}[step index]:{/grn}{yel}",ctr++,"{/yel}"));
					player->sendln(step->dump());
				}
				return {1,CAT("Listed ",step_list.size()," steps")};
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
			register_integral_accumulator_command("save-step","<virtual_number> <Nth-step-id>...[Nth-step-id-N]",[&,this](const std::vector<int>&& step_ids,std::shared_ptr<seqbuild_orm_type> profile) -> std::tuple<bool,std::string> {
				if(!profile) {
					return {0,"Invalid or missing profile vnum"};
				}
				auto p = foreach_step_do(step_ids,"save");
				return {1,CAT("saved ",std::get<0>(p)," successfully. failed saving: ",std::get<1>(p), " items")};
			});

			/**
			 * ==================================================
			 * save-all-steps <vnum>
			 * ==================================================
			 * [description]
			 *  -----------------------------------------
			 * 	saves all the step by sequence id
			 *  -----------------------------------------
			 */
			register_custom_command("save-all-steps","<virtual_number>",
			[&,this](const std::vector<std::string>& args,std::string argument,std::shared_ptr<seqbuild_orm_type> profile) -> std::tuple<bool,std::string> {
				if(!profile) {
					return {0,"Invalid or missing profile vnum"};
				}
				auto p = foreach_step_do("save");
				return {1, CAT("saved ",std::get<0>(p)," successfully. failed saving: ",std::get<1>(p), " items")};
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
			register_integral_accumulator_command("delete-step","<virtual_number> <step-id>...[step-id-N]",[&,this](const std::vector<int>&& step_ids,std::shared_ptr<seqbuild_orm_type> profile) -> std::tuple<bool,std::string> {
				if(!profile) {
					return {0,"Invalid or missing profile vnum"};
				}
				mods::orm::scripted_sequences orm;
				std::string errors;
				int i = 0;
				for(const auto& index : step_ids) {
					if(index >= this->step_list.size()) {
						errors += CAT("Invalid step index: ",index,"! Skipping...\r\n");
						continue;
					}
					this->step_list[index]->destroy();
					auto s = this->step_list[index]->result();
					if(ORM_FAILURE(s)) {
						errors += CAT("Sql failure deleting index {yel}",index,"{/yel}:{red}",std::get<1>(s),"{/red}\r\n");
					} else {
						++i;
					}
				}
				if(i) {
					decltype(step_list) cleaned_steps;
					for(unsigned k =0; k < step_list.size(); k++) {
						if(std::find(step_ids.begin(),step_ids.end(),k) == step_ids.end()) {
							cleaned_steps.emplace_back(step_list[k]);
						}
					}
					step_list = std::move(cleaned_steps);
				}
				if(errors.length() && i > 0) {
					return {1,CAT("Saved ",i," steps, but errors exist: ",errors)};
				}
				if(i == 0) {
					return {0,"No steps saved."};
				}
				return {1,CAT("Saved ",i," steps")};
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
			register_custom_command("load-steps","<virtual_number>",[&,this](const std::vector<std::string>& args,std::string argument,std::shared_ptr<seqbuild_orm_type> profile) -> std::tuple<bool,std::string> {
				if(!profile) {
					return {0,"Invalid or missing profile vnum"};
				}
				this->step_list.clear();
				mods::orm::gather_scripted_steps_by_sequence_vnum(profile->vnum(),&this->step_list);
				return {1,"Loaded"};
				//return {1,CAT("loaded ",std::get<0>(s)," entries.")};
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
			register_custom_command("set-step-data","<virtual_number> <Nth-step> <field> <value>",[&,this](const std::vector<std::string>& args,std::string argument,std::shared_ptr<seqbuild_orm_type> profile) -> std::tuple<bool,std::string> {
				if(!profile) {
					return {0,"Invalid or missing profile vnum"};
				}
				/** args:
				 * [0] set-step-data
				 * [1] vnum
				 * [2] step-index
				 * [3] field
				 * [4] value
				 */
				if(!argshave()->int_at({1,2})->nth_is_any(3,step_column_list())->size_gt(4)->passed()) {
					return {0,argshave()->errors()};
				}
				auto index = args()->int_at(2);
				if(index >= this->step_list.size() || index < 0) {
					return {0,CAT("step index out of bounds: ",index)};
				}
				if(argshave()->int_at({1,2,4})->nth_is_any(3,integral_list())->passed()) {
					return this->step_list[index]->set(argat(3),argat(4));
				}
				if(argshave()->int_at({1,2})->nth_is_any(3,accumulator_list())->passed()) {
					/** we need to accumulate. save everything after the field argument */
					return this->step_list[index]->set(argat(3),args()->gather_strings_starting_at(4));
				}
				if(argshave()->int_at({1,2})->nth_is_any(3,string_list())->passed()) {
					return this->step_list[index]->set(argat(3),argat(4));
				}
				return {0,"nothing set"};
			});
		}//end seqbuild_interface

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
			if(argshave()->int_at(1)->first_is("new")) {
				auto c = std::make_shared<mods::orm::scripted_sequences>();
				auto id = c->initialize_row(args()->int_at(1));
				if(id <= 0) {
					return {0,"Couldn't initialize row"};
				}
				mods::orm::scripted_sequences_list().emplace_back(std::move(c));
				this->step_list.clear();
				return {1,"Created row."};
			}
			return {0,"Invalid arguments"};
		}
		/** ======== */
		/** required */
		/** ======== */
		void clear() {
			mods::orm::scripted_sequences_list().clear();
			mods::orm::scripted_step_list().clear();
			mods::orm::contract_step_callback_list().clear();
		}
		/** ======== */
		/** required */
		/** ======== */
		void load_all() {
			mods::orm::scripted_sequences_list() = std::move(mods::orm::load_all_by_table<mods::orm::scripted_sequences>());
			mods::orm::scripted_step_list() = std::move(mods::orm::load_all_by_table<mods::orm::scripted_step>());
			mods::orm::contract_step_callback_list() = std::move(mods::orm::load_all_by_table<mods::orm::contract_step_callback>());
		}
	};

	namespace map {

		bool delete_by_vnum(const uint64_t& vnum) {
			std::deque<std::shared_ptr<mods::orm::scripted_step>> list;
			bool deleted = false;
			for(auto& m : mods::orm::scripted_step_list()) {
				if(m->s_sequence_vnum == vnum) {
					m->destroy();
					deleted = true;
					continue;
				}
				list.emplace_back(std::move(m));
			}
			mods::orm::scripted_step_list() = std::move(list);
			return deleted;
		}
		std::deque<std::shared_ptr<mods::orm::scripted_step>> by_profile_vnum(const uint64_t& vnum) {
			std::deque<std::shared_ptr<mods::orm::scripted_step>> list;
			for(const auto& m : mods::orm::scripted_step_list()) {
				if(m->s_sequence_vnum == vnum) {
					list.emplace_back(m);
				}
			}
			return list;
		}
	};
	using args_t = std::vector<std::string>;

	static seqbuild_interface& seqbuilder(player_ptr_t player) {
		static seqbuild_interface interface;
		interface.set_builder(player);
		return interface;
	}

	SUPERCMD(do_seqbuild) {
		mods::builder::initialize_builder(player);

		auto handled = seqbuilder(player).handle_input(argument);
		seqbuilder(player).flush();
		if(handled) {
			return;
		}

	}	//end seqbuild

	void init() {
		mods::interpreter::add_command("seqbuild", POS_RESTING, do_seqbuild, LVL_BUILDER,0);
		seqbuilder(nullptr);
	}
};
