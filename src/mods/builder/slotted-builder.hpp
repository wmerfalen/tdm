#ifndef __MENTOC_MODS_BUILDER_SLOTTED_BUILDER_HEADER__
#define __MENTOC_MODS_BUILDER_SLOTTED_BUILDER_HEADER__
#include <functional>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "../../globals.hpp"

namespace mods::builder {
	static constexpr uint8_t MSG_SUCCESS = 0;
	static constexpr uint8_t MSG_ERROR = 1;

	template <typename TVnumType, typename TOrmType>
	struct slotted_builder {
			using status_response_t = std::tuple<bool,std::string>;
			using optional_orm_t = std::optional<std::shared_ptr<TOrmType>>;
			using custom_command_t = std::function<std::tuple<bool,std::string>(const std::vector<std::string>&,std::string,std::shared_ptr<TOrmType>)>;
			using orm_container_t = std::deque<std::shared_ptr<TOrmType>>;
		protected:
			std::string m_new_format;
			std::vector<std::string> m_slot_list;
			player_ptr_t m_builder_ptr;
			std::vector<std::string> m_encoded_response;
			std::map<std::string,custom_command_t> m_custom_command_map;
			orm_container_t* m_orm_list;
		public:
			/** must be implemented by child class */
			virtual bool delete_by_vnum(TVnumType vnum) = 0;
			virtual std::optional<std::shared_ptr<TOrmType>> by_vnum(TVnumType vnum) = 0;
			virtual status_response_t dispatch_new_command(const std::vector<std::string>&,std::string) = 0;

			/** must be set at constructor, otherwise player will not get messages */
			void set_builder(player_ptr_t& player) {
				m_builder_ptr = player;
			}

			void set_orm_list(orm_container_t* container) {
				m_orm_list = container;
			}

			/** utility function which will extract an integer from the given index
			 * usage: extract_int<int32_t>("set",argument,1);
			 * */
			template <typename TExtractType>
			std::optional<TExtractType> extract_int(std::string verb,std::string argument,std::size_t index) {
				auto args = mods::util::subcmd_args<64,args_t>(argument,verb.c_str());
				if(!args.has_value()) {
					return std::nullopt;
				}
				auto cmd_args = args.value();
				if(cmd_args.size() < index) {
					return std::nullopt;
				}
				return mods::util::stoi(cmd_args[index]);
			}
			/** utilitiy function which will extract the exact profile given the index when
			 * the grammar is: mbuild <verb> <vnum> [arg]...[argN]
			 * usage: extract_profile("set",argument);
			 */
			optional_orm_t extract_profile(std::string verb,std::string argument) {
				auto opt_vnum = extract_int<TVnumType>(verb,argument,1);
				if(!opt_vnum.has_value()) {
					return std::nullopt;
				}
				return by_vnum(opt_vnum.value());
			}
			/**
			 * utility function to extract the given profile but by using the exact index in the cmd_args array
			 * this is sometimes needed when the vnum isn't always at index [1]
			 */
			optional_orm_t extract_profile(std::string verb,std::string argument,std::size_t index) {
				auto opt_vnum = extract_int<TVnumType>(verb,argument,index);
				if(!opt_vnum.has_value()) {
					return std::nullopt;
				}
				return by_vnum(opt_vnum.value());
			}

			/** use this to register custom commands with custom verbs.
			 * register_custom_command("reload-all",[](const std::vector<std::string>& cmd_args,std::string argument, TOrmType) { std::cout << "inside lambda" });
			 */
			status_response_t register_custom_command(std::string verb, auto lambda) {
				m_custom_command_map[verb] = lambda;
				return {1,"done"};
			}

			/** clear specific verb */
			void clear_custom_command(std::string verb) {
				m_custom_command_map.erase(verb);
			}
			/** clear all custom commands */
			void clear_all_custom_commands() {
				m_custom_command_map.clear();
			}

			/** helper */
			bool has_custom_command_for(std::string command) {
				return m_custom_command_map.find(command) != m_custom_command_map.end();
			}

			/** proxy function to set slot on specific orm class */
			void set_slot(const TVnumType& vnum,std::string key,std::string value) {
				if(vnum < 0) {
					push_encoded_message(CAT("vnum is negative"),CAT("negargvnum"),MSG_ERROR);
					return;
				}
				auto opt_profile = by_vnum(vnum);
				if(!opt_profile.has_value()) {
					push_encoded_message(
					    CAT("couldn't find profile for argument"),
					    CAT("noprof"),MSG_ERROR
					);
					return;
				}
				auto status = opt_profile.value()->set_slot(key,value);
				if(!std::get<0>(status)) {
					push_encoded_message(
					    CAT("couldn't set slot: '",std::get<1>(status)),
					    std::get<1>(status),MSG_ERROR
					);
					return;
				}
				push_encoded_message(
				    CAT("set slot '",std::get<1>(status)),
				    "ok",MSG_SUCCESS
				);
			}

			/** function used to set the available slot keys
			 */
			void set_slot_list(std::vector<std::string> s) {
				m_slot_list = s;
			}

			/** show the player the available slots setup for this class */
			void display_slot_list() {
				std::string list;
				for(const auto& item : m_slot_list) {
					m_builder_ptr->send("  {gld}|:: %s{/gld}\r\n",item.data());
				}
			}
			/** helper function to handle communication of messages. supports encoding if user is executing js */
			void push_encoded_message(std::string msg, std::string encoded,uint8_t msg_type) {
				if(m_builder_ptr->is_executing_js()) {
					if(msg_type == MSG_SUCCESS) {
						m_encoded_response.emplace_back(CAT("{type:\"success\",msg_len:",encoded.length(),",msg:\"",encoded.data(),"\"}"));
					} else {
						m_encoded_response.emplace_back(CAT("{type:\"error\",msg_len:",encoded.length(),",msg:\"",encoded.data(),"\"}"));
					}
					return;
				}
				if(msg_type == MSG_SUCCESS) {
					r_success(m_builder_ptr,msg.data());
					return;
				}
				r_error(m_builder_ptr,msg.data());
			}
			/** encodes the specific orm profile (if exec js), or dumps using r_success */
			void push_profile(const std::shared_ptr<TOrmType>& profile) {
				if(m_builder_ptr->is_executing_js()) {
					m_encoded_response.emplace_back(profile->encode());
					return;
				}
				r_success(m_builder_ptr,profile->dump());
			}
			/** encodes the specific orm profile (if exec js), or dumps using r_success  but only encodes specific fields */
			void push_profile_with_fields(const std::shared_ptr<TOrmType>& profile,const std::vector<std::string>& fields) {
				if(m_builder_ptr->is_executing_js()) {
					m_encoded_response.emplace_back(profile->encode_fields(fields));
					return;
				}
				r_success(m_builder_ptr,profile->dump_fields(fields));
			}
			/** saves profile then lets the user know if it succeeded */
			void report_profile_save(std::shared_ptr<TOrmType> profile,TVnumType vnum) {
				if(profile->save()) {
					push_encoded_ok("saved");
					return;
				}
				push_encoded_error("failed");
			}
			/**
			 * dispatches the set command.
			 * Nbuild set <vnum> <slot_key> <value>
			 */
			bool dispatch_slot(std::string argument) {

				auto args = mods::util::subcmd_args<64,args_t>(argument.data(),"set");
				if(!args.has_value()) {
					return false;
				}
				auto cmd_args = args.value();
				/**
				 * cmd_args will be: [0] => set, [1] => vnum ... [2] => key [3] => value
				 */
				if(cmd_args.size() < 4) {
					push_encoded_message("not enough arguments","argcount",MSG_ERROR);
					return true;
				}
				auto opt_profile = extract_profile("set",argument);
				if(!opt_profile.has_value()) {
					push_encoded_message(
					    CAT("couldn't find profile for argument"),
					    CAT("noprof"),MSG_ERROR
					);
					return true;
				}
				/** custom command override */
				if(has_custom_command_for("set")) {
					return tuple_wrap(m_custom_command_map["set"](cmd_args,argument,opt_profile.value()));
				}
				auto& profile = opt_profile.value();
				auto status = profile->set_slot(cmd_args[2],cmd_args[3]);
				if(!std::get<0>(status)) {
					push_encoded_message(std::get<1>(status),
					                     std::get<1>(status),MSG_ERROR
					                    );
					return true;
				}

				return true;
			}
			void push_encoded_error(std::string msg) {
				push_encoded_message(msg,msg,MSG_ERROR);
			}
			void push_encoded_ok(std::string msg) {
				push_encoded_message(msg,msg,MSG_SUCCESS);
			}
			bool tuple_wrap(status_response_t s) {
				if(!std::get<0>(s)) {
					push_encoded_error(std::get<1>(s));
				} else {
					push_encoded_ok(std::get<1>(s));
				}
				return std::get<0>(s);
			}

			/**
			 * dispatches "new" command
			 * example:
			 * 	Nbuild new <arg1>...[argN]
			 */
			bool base_dispatch_new_command(std::string argument) {
				auto args = mods::util::subcmd_args<10,args_t>(argument,"new");
				if(!args.has_value()) {
					return false;
				}
				auto cmd_args = args.value();
				/**
				 * cmd_args will be: [0] => new, [1] => arg ... [N] => argN
				 */
				if(has_custom_command_for("new")) {
					return tuple_wrap(m_custom_command_map[cmd_args[0]](cmd_args,argument,nullptr));
				}
				return tuple_wrap(this->dispatch_new_command(cmd_args,argument));
			}

			/**
			 * dispatches list, delete, or save multi commands
			 * example:
			 * 	Nbuild save <vnum>...<vnum_N>
			 */
			bool dispatch_multi_vnum_action(std::string argument) {
				m_encoded_response.clear();
				std::optional<std::vector<std::string>> args;
				for(auto& phrase : {
				            "list","delete","save"
				        }) {
					args = mods::util::subcmd_args<10,args_t>(argument.data(),phrase);
					if(args.has_value()) {
						break;
					}
				}
				if(!args.has_value()) {
					return false;
				}
				auto cmd_args = args.value();
				/**
				 * cmd_args will be: [0] => list, [1] => vnum ... [N] => vnumN
				 */
				for(unsigned i = 1; i < cmd_args.size(); i++) {
					auto opt_profile = extract_profile(cmd_args[0],argument.data(),i);
					if(!opt_profile.has_value()) {
						push_encoded_message(
						    CAT("couldn't find profile for argument ",i,". ignoring..."),
						    CAT("noprof:",i),MSG_ERROR
						);
						continue;
					}
					if(has_custom_command_for(cmd_args[0])) {
						m_custom_command_map[cmd_args[0]](cmd_args,argument,opt_profile.value());
						continue;
					}
					if(cmd_args[0].compare("delete") == 0) {
						if(this->delete_by_vnum(opt_profile.value()->vnum())) {
							push_encoded_message(
							    CAT("successfully deleted vnum:",opt_profile.value()->vnum(),"."),
							    CAT("deleted:",i),MSG_SUCCESS
							);
							continue;
						}
						push_encoded_message(
						    CAT("couldn't delete vnum:",opt_profile.value()->vnum(),"."),
						    CAT("failed:",i),MSG_ERROR
						);
						continue;
					}
					if(cmd_args[0].compare("list") == 0) {
						push_profile(opt_profile.value());
						continue;
					}
					if(cmd_args[0].compare("save") == 0) {
						report_profile_save(opt_profile.value(),opt_profile.value()->vnum());
						continue;
					}
				}//end for
				return true;
			}
			bool handle_input(std::string argument) {
				/**
				 * new <vnum> ... (handled exclusively by child class
				 */
				if(base_dispatch_new_command(argument)) {
					return true;
				}
				/**
				 * set <vnum> <slot> <value>
				 */
				if(dispatch_slot(argument)) {
					return true;
				}
				/**
				 * list|delete|save <vnum>...<vnum-N>
				 */
				if(dispatch_multi_vnum_action(argument)) {
					return true;
				}
				/**
				 * paginate [page] [pageSize]
				 */
				if(dispatch_paginate_action(argument)) {
					return true;
				}
				/**
				 * list-extract <vnum> <field>...<field-N>
				 */
				if(dispatch_list_extract_action(argument)) {
					return true;
				}
				/**
				 * show <vnum> [field]...[field-N]
				 */
				if(dispatch_show_action(argument)) {
					return true;
				}
				/**
				 * custom commands the child class has provided us
				 */
				for(auto& pair : m_custom_command_map) {
					auto args = mods::util::subcmd_args<50,args_t>(argument,pair.first.c_str());
					if(args.has_value()) {
						auto cmd_args = args.value();
						return tuple_wrap(m_custom_command_map[pair.first](cmd_args,argument,nullptr));
					}
				}
				return false;
			}
			/**
			 * dispatches paginate
			 * example:
			 * 	Nbuild paginate [page=1] [pageSize=50]
			 */
			bool dispatch_paginate_action(std::string argument) {
				auto args = mods::util::subcmd_args<30,args_t>(argument.data(),"paginate");
				if(!args.has_value()) {
					return false;
				}
				auto cmd_args = args.value();
				/**
				 * cmd_args will be: [0] => paginate, [1] => [page=1] ... [2] => [pageSize=50]
				 */
				if(has_custom_command_for(cmd_args[0])) {
					return tuple_wrap(m_custom_command_map[cmd_args[0]](cmd_args,argument,nullptr));
				}
				std::size_t page = 0,page_size = 50;

				if(cmd_args.size() > 1) {
					auto opt = extract_int<int>("paginate",argument,1).value_or(-1);
					if(opt < 0) {
						push_encoded_error("invalid page");
						return true;
					}
					page = opt;
				}
				if(cmd_args.size() > 2) {
					auto opt = extract_int<int>("paginate",argument,2).value_or(-1);
					if(opt <= 0) {
						push_encoded_error("invalid page size");
						return true;
					}
					page_size = opt;
				}
				if(!m_orm_list) {
					push_encoded_error("orm list not set!");
					return true;
				}
				page -= 1;

				if(m_orm_list->size() < page * page_size) {
					push_encoded_error("out of bounds");
					return true;
				}

				auto it = m_orm_list->cbegin() + (page * page_size);
				std::size_t i = 0;
				for(; i < page_size && it != m_orm_list->cend(); ++i,++it) {
					push_profile((*it));
				}
				return true;
			}
			/**
			 * dispatches list-extract
			 * example:
			 * 	Nbuild list-extract <vnum> <field>...<field-N>
			 */
			bool dispatch_list_extract_action(std::string argument) {
				auto args = mods::util::subcmd_args<30,args_t>(argument.data(),"list-extract");
				if(!args.has_value()) {
					return false;
				}
				auto cmd_args = args.value();
				if(cmd_args.size() < 3) {
					push_encoded_error("not enough arguments");
					return true;
				}
				/**
				 * cmd_args will be: [0] => list-extract, [1] => vnum [2] => field ... [N] => field-N
				 */
				if(has_custom_command_for(cmd_args[0])) {
					return tuple_wrap(m_custom_command_map[cmd_args[0]](cmd_args,argument,nullptr));
				}

				std::vector<std::string> fields;
				for(unsigned i =2; i < cmd_args.size(); i++) {
					fields.emplace_back(cmd_args[i]);
				}
				auto prof = extract_profile("list-extract",argument);
				if(!prof.has_value()) {
					push_encoded_error("no profile with that vnum");
					return true;
				}
				push_profile_with_fields(prof.value(),fields);
				return true;
			}
			/**
			 * dispatches show
			 * example:
			 * 	Nbuild show <vnum> [optional_field]...[optional_field-N]
			 */
			bool dispatch_show_action(std::string argument) {
				auto args = mods::util::subcmd_args<30,args_t>(argument.data(),"show");
				if(!args.has_value()) {
					return false;
				}
				auto cmd_args = args.value();
				/**
				 * cmd_args will be: [0] => show [1] => vnum [2] => [field] ... [N] => [field-N]
				 */
				if(has_custom_command_for(cmd_args[0])) {
					return tuple_wrap(m_custom_command_map[cmd_args[0]](cmd_args,argument,nullptr));
				}

				auto prof = extract_profile("show",argument);
				if(!prof.has_value()) {
					push_encoded_error("no profile with that vnum");
					return true;
				}

				if(cmd_args.size() > 2) {
					std::vector<std::string> fields;
					for(unsigned i =2; i < cmd_args.size(); i++) {
						fields.emplace_back(cmd_args[i]);
					}
					push_profile_with_fields(prof.value(),fields);
					return true;
				}
				push_profile(prof.value());
				return true;
			}
	};//end struct
};//end namespace
#endif
