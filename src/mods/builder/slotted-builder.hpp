#ifndef __MENTOC_MODS_BUILDER_SLOTTED_BUILDER_HEADER__
#define __MENTOC_MODS_BUILDER_SLOTTED_BUILDER_HEADER__
#include <functional>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "../../globals.hpp"
#include "../orm/scripted-step.hpp"

#undef m_debug
#undef m_error
#define m_debug(MSG) mentoc_prefix_debug("[mods::slotted_builder::debug]")  << MSG << "\n";
#define m_error(MSG) mentoc_prefix_debug(red_str("[mods::slotted_builder::ERROR]"))  << MSG << "\n";

namespace mods::builder {
	static constexpr uint8_t MSG_SUCCESS = 0;
	static constexpr uint8_t MSG_ERROR = 1;

	template <typename TVnumType, typename TOrmType>
	struct slotted_builder {
			using status_response_t = std::tuple<bool,std::string>;
			using optional_orm_t = std::optional<std::shared_ptr<TOrmType>>;
			using custom_command_t = std::function<std::tuple<bool,std::string>(const std::vector<std::string>&,std::string,std::shared_ptr<TOrmType>)>;
			using custom_non_profile_command_t = std::function<std::tuple<bool,std::string>(const std::vector<std::string>&,std::string)>;
			using custom_accumulator_command_t = std::function<std::tuple<bool,std::string>(std::string&&,std::shared_ptr<TOrmType>)>;
			using custom_indexed_accumulator_command_t = std::function<std::tuple<bool,std::string>(std::string&&,std::shared_ptr<TOrmType>,int)>;
			using custom_integral_accumulator_command_t = std::function<std::tuple<bool,std::string>(const std::vector<int>&&,std::shared_ptr<TOrmType>)>;
			using custom_manual_command_t = std::function<std::tuple<bool,std::string>(const std::string&)>;
			using orm_container_t = std::deque<std::shared_ptr<TOrmType>>;
		protected:
			std::vector<std::string> m_slot_list;
			player_ptr_t m_builder_ptr;
			player_ptr_t player;
			std::vector<std::string> m_encoded_response;
			std::map<std::string,custom_command_t> m_custom_command_map;
			std::map<std::string,custom_manual_command_t> m_custom_manual_command_map;
			std::map<std::string,custom_non_profile_command_t> m_custom_non_profile_command_map;
			std::map<std::string,custom_accumulator_command_t> m_custom_accumulator_command_map;
			std::map<std::string,custom_indexed_accumulator_command_t> m_custom_indexed_accumulator_command_map;
			std::map<std::string,custom_integral_accumulator_command_t> m_custom_integral_accumulator_command_map;
			orm_container_t* m_orm_list;
			bool m_automatically_clear;
			std::map<std::string,std::string> m_signatures;
			std::string m_base_command;
		public:
			/** ================================================================================= */
			/** ### [ SECTION: Virtual functions that must be implemented                     ### */
			/** ### [ description ]:                                                          ### */
			/** ### These are functions that the child class needs to pay attention to        ### */
			/** ###---------------------------------------------------------------------------### */
			/** ###                           [ START ]                                       ### */
			/** ###---------------------------------------------------------------------------### */
			virtual bool delete_by_vnum(TVnumType vnum) = 0;
			virtual std::optional<std::shared_ptr<TOrmType>> by_vnum(TVnumType vnum) = 0;
			virtual status_response_t dispatch_new_command(const std::vector<std::string>&,std::string) = 0;
			virtual void clear() = 0;
			virtual void load_all() = 0;
			/** ###---------------------------------------------------------------------------### */
			/** ###                            [ END ]                                        ### */
			/** ================================================================================= */

			/** ================================================================================= */
			/** ### [ SECTION: bootstrap functions                                            ### */
			/** ### [ description ]:                                                          ### */
			/** ###  these functions must be called upon construction!                        ### */
			/** ###---------------------------------------------------------------------------### */
			/** ###                           [ START ]                                       ### */
			/** ###---------------------------------------------------------------------------### */

			/** [ 1 ] -> set this to the player calling your ACMD function */
			void set_builder(player_ptr_t p) {
				m_debug("set_builder");
				m_builder_ptr = p;
				player = p;
			}

			/** [ 2 ] -> point this to your singleton instance (i.e.: mods::orm::mob_equipment_list()) */
			void set_orm_list(orm_container_t* container) {
				m_orm_list = container;
			}

			std::map<std::string,std::string>& get_signatures() {
				return m_signatures;
			}

			std::string& footer() {
				return m_footer;
			}


			void register_signature(std::string_view verb,std::string_view sig) {
				get_signatures()[verb.data()] = CAT("{grn}",m_base_command,"{/grn} {red}",verb.data()," ",sig.data(),"{/red}\r\n");
			}

			void set_base_command(std::string base) {
				m_base_command = base;
			}

			/**
			 * [ 3 ] -> function used to set the available slot keys
			 * if your command plans on using the "set" <vnum> <key> <value> subcommand, then
			 * you will need to set this to whatever your orm object's return value is. that's almost
			 * always mods::orm::get_slot_list();
			 */
			void set_slot_list(std::vector<std::string> s) {
				m_slot_list = s;
			}

			/** [ 4 ] -> use this to register custom commands with custom verbs. */
			status_response_t register_custom_command(std::string verb, auto lambda) {
				m_custom_command_map[verb] = lambda;
				return {1,"done"};
			}
			status_response_t register_manual_command(std::string verb, std::string_view signature, auto lambda) {
				m_custom_manual_command_map[verb] = lambda;
				register_signature(verb,signature);
				return {1,"done"};
			}
			status_response_t register_custom_command(std::string verb, std::string_view signature,auto lambda) {
				m_custom_command_map[verb] = lambda;
				register_signature(verb,signature);
				return {1,"done"};
			}
			status_response_t register_custom_non_profile_command(std::string verb, auto lambda) {
				m_custom_non_profile_command_map[verb] = lambda;
				return {1,"done"};
			}
			/** [ 4 ] -> use this to register custom commands with custom verbs. */
			status_response_t register_accumulator_command(std::string verb, std::string_view signature,auto lambda) {
				m_custom_accumulator_command_map[verb] = lambda;
				register_signature(verb,signature);
				return {1,"done"};
			}
			status_response_t register_indexed_accumulator_command(std::string verb, std::string_view signature,auto lambda) {
				m_custom_indexed_accumulator_command_map[verb] = lambda;
				register_signature(verb,signature);
				return {1,"done"};
			}
			/** [ 4 ] -> use this to register custom commands with custom verbs. */
			status_response_t register_integral_accumulator_command(std::string verb, std::string_view signature,auto lambda) {
				m_custom_integral_accumulator_command_map[verb] = lambda;
				register_signature(verb,signature);
				return {1,"done"};
			}
			/** ###---------------------------------------------------------------------------### */
			/** ###                            [ END ]                                        ### */
			/** ================================================================================= */


			/** ###---------------------------------------------------------------------------### */
			/** ### The only other function you'll need to call 99% of the time is here       ### */
			/** ###---------------------------------------------------------------------------### */
			bool handle_input(std::string argument) {
				m_debug("handle_input");
				auto& player = this->m_builder_ptr;
				if(m_automatically_clear) {
					clear_response();
				}
				if(argument.length() == 0) {
					m_debug("arglength zero, displaying help");
					dispatch_help("help");
					return true;
				}
				if(dispatch_help(argument)) {
					m_debug("matched displaying help, returning");
					return true;
				}
				/**
				 * new <vnum> ... (handled exclusively by child class
				 */
				if(base_dispatch_new_command(argument)) {
					m_debug("base dispatched new command");
					return true;
				}
				/**
				 * remove <vnum> <slot>...<slot-N>
				 */
				if(dispatch_remove_slot(argument)) {
					m_debug("dispatched remove slot");
					return true;
				}
				/**
				 * set <vnum> <slot> <value>
				 */
				if(dispatch_slot(argument)) {
					m_debug("dispatched slot");
					return true;
				}
				/**
				 * list|delete|save <vnum>...<vnum-N>
				 */
				if(dispatch_multi_vnum_action(argument)) {
					m_debug("dispatched multi vnum action");
					return true;
				}
				/**
				 * paginate [page] [pageSize]
				 */
				if(dispatch_paginate_action(argument)) {
					m_debug("dispatched paginate action");
					return true;
				}
				/**
				 * list-extract <vnum> <field>...<field-N>
				 */
				if(dispatch_list_extract_action(argument)) {
					m_debug("dispatched list extract action");
					return true;
				}
				/**
				 * show <vnum> [field]...[field-N]
				 */
				if(dispatch_show_action(argument)) {
					m_debug("dispatched show action");
					return true;
				}
				/**
				 * reload-all
				 */
				if(dispatch_reload_all_action(argument)) {
					m_debug("dispatched reload all ction");
					return true;
				}
				/**
				 * custom non-profile commands the child class has provided us
				 */
				for(auto& pair : m_custom_non_profile_command_map) {
					auto args = mods::util::subcmd_args<50,args_t>(argument,pair.first.c_str());
					if(args.has_value()) {
						auto args = PARSE_ARGS();
						return tuple_wrap(m_custom_non_profile_command_map[pair.first](std::move(args),argument));
					}
				}
				/**
				 * custom _INTEGRAL_ accumulator commands the child class has provided us
				 */
				for(auto& pair : m_custom_integral_accumulator_command_map) {
					auto args = mods::util::subcmd_args<50,args_t>(argument,pair.first.c_str());
					if(args.has_value()) {
						auto cmd_args = args.value();
						auto opt = extract_profile(pair.first,argument);
						if(!opt.has_value()) {
							push_encoded_error("Unable to find profile");
							return false;
						}
						auto& profile = opt.value();
						auto args = PARSE_ARGS();
						if(args.size() < 3) {
							push_encoded_error("not enough arguments");
							return false;
						}
						std::vector<int> values;
						for(int i=2; i < args.size(); i++) {
							values.emplace_back(mods::util::stoi(args[i]).value_or(-1));
						}
						return tuple_wrap(m_custom_integral_accumulator_command_map[pair.first](std::move(values),profile));
					}
				}
				/**
				 * custom indexed accumulator commands the child class has provided us
				 */
				for(auto& pair : m_custom_indexed_accumulator_command_map) {
					/**
					 * syntax: <command> <profile-vnum> <index> <text...>
					 */
					auto args = mods::util::subcmd_args<50,args_t>(argument,pair.first.c_str());
					if(args.has_value()) {
						auto cmd_args = args.value();
						auto opt = extract_profile(pair.first,argument);
						if(!opt.has_value()) {
							push_encoded_error("Unable to find profile");
							return false;
						}
						auto& profile = opt.value();
						auto args = PARSE_ARGS();
						if(args.size() < 4) {
							push_encoded_error("not enough arguments");
							return false;
						}
						args()->save_integer(2);
						std::string value;
						for(int i=3; i < args.size(); i++) {
							value += args[i];
							if(i + 1 < args.size()) {
								value += " ";
							}
						}
						return tuple_wrap(m_custom_indexed_accumulator_command_map[pair.first](std::move(value),profile,args()->fetch_integer(2)));
					}
				}
				/**
				 * custom accumulator commands the child class has provided us
				 */
				for(auto& pair : m_custom_accumulator_command_map) {
					auto args = mods::util::subcmd_args<50,args_t>(argument,pair.first.c_str());
					if(args.has_value()) {
						auto cmd_args = args.value();
						auto opt = extract_profile(pair.first,argument);
						if(!opt.has_value()) {
							push_encoded_error("Unable to find profile");
							return false;
						}
						auto& profile = opt.value();
						auto args = PARSE_ARGS();
						if(args.size() < 3) {
							push_encoded_error("not enough arguments");
							return false;
						}
						std::string value;
						for(int i=2; i < args.size(); i++) {
							value += args[i];
							if(i + 1 < args.size()) {
								value += " ";
							}
						}
						return tuple_wrap(m_custom_accumulator_command_map[pair.first](std::move(value),profile));
					}
				}
				/**
				 * custom commands the child class has provided us
				 */
				for(auto& pair : m_custom_command_map) {
					auto args = mods::util::subcmd_args<50,args_t>(argument,pair.first.c_str());
					if(args.has_value()) {
						auto cmd_args = args.value();
						auto opt = extract_profile(pair.first,argument);
						return tuple_wrap(m_custom_command_map[pair.first](cmd_args,argument,opt.value_or(nullptr)));
					}
				}
				for(auto& pair : m_custom_manual_command_map) {
					auto args = mods::util::subcmd_args<50,args_t>(argument,pair.first.c_str());
					if(args.has_value()) {
						return tuple_wrap(m_custom_manual_command_map[pair.first](argument));
					}
				}
				return false;
			}

			/** ================================================================================= */
			/** ### [ SECTION: configuration functions ]                                      ### */
			/** ### [ description ]:                                                          ### */
			/** ### these functions are merely for configuring the class and are optional     ### */
			/** ###---------------------------------------------------------------------------### */
			/** ###                           [ START ]                                       ### */
			/** ###---------------------------------------------------------------------------### */

			/** [  ] -> clear specific custom command */
			void clear_custom_command(std::string verb) {
				m_custom_command_map.erase(verb);
			}
			/** [  ] -> clear all custom commands */
			void clear_all_custom_commands() {
				m_custom_command_map.clear();
			}
			/** [  ] -> will clear the encoded_response before handling input if set to true */
			void automatically_clear_response(bool s) {
				m_automatically_clear = s;
			}
			/** [  ] -> fetch the auto clear flag */
			bool get_auto_clear_response_flag() {
				return m_automatically_clear;
			}
			/** [  ] -> check for existence of custom command */
			bool has_custom_command_for(std::string command) {
				return m_custom_command_map.find(command) != m_custom_command_map.end();
			}
			void remove_command_signatures(std::vector<std::string> list) {
				for(const auto& cmd : list) {
					m_signatures.erase(cmd);
				}
			}

			slotted_builder() {
#ifdef __MENTOC_ENCODED_MESSAGE_DEBUG_OUTPUT__
				std::cerr << green_str("slotted_builder") << "\n";
#endif
				m_automatically_clear = true;
				m_builder_ptr = nullptr;
				m_encoded_response.clear();
				m_custom_command_map.clear();
				m_orm_list = nullptr;
				m_signatures["new"] = "{grn}%s{/grn} {red}new{/red}\r\n";
				m_signatures["save"] = "{grn}%s{/grn} {red}save <virtual_number>...<virtual_number-N>{/red}\r\n";
				m_signatures["list"] = "{grn}%s{/grn} {red}list <virtual_number>...<virtual_number-N>{/red}\r\n";
				m_signatures["list-extract"] = "{grn}%s{/grn} {red}list-extract <virtual_number> <field>...<field-N>{/red}\r\n";
				m_signatures["set"] = "{grn}%s{/grn} {red}set <virtual_number> <slot> <value>{/red}\r\n";
				m_signatures["remove"] = "{grn}%s{/grn} {red}remove <virtual_number> <slot>...<slot-N>{/red}\r\n";
				m_signatures["delete"] = "{grn}%s{/grn} {red}delete <virtual_number>...<virtual_number-N>{/red}\r\n";
				m_signatures["show"] = "{grn}%s{/grn} {red}show <virtual_number> [field]...[fieldN]{/red}\r\n";
				m_signatures["paginate"] = "{grn}%s{/grn} {red}paginate [page] [pageSize]{/red}\r\n";
				m_signatures["reload-all"] = "{grn}%s{/grn} {red}reload-all{/red}\r\n";
			}
			virtual ~slotted_builder() {
#ifdef __MENTOC_ENCODED_MESSAGE_DEBUG_OUTPUT__
				std::cerr << red_str("~slotted_builder") << "\n";
#endif
				m_orm_list = nullptr;
				m_builder_ptr = nullptr;
				m_encoded_response.clear();
				m_custom_command_map.clear();
			}

			/** ================================================================================= */
			/** ### [ SECTION: communication responses to the user                            ### */
			/** ### [ description ]:                                                          ### */
			/** ###  for fetching and management of command responses                         ### */
			/** ###---------------------------------------------------------------------------### */
			/** ###                           [ START ]                                       ### */
			/** ###---------------------------------------------------------------------------### */

			/** will clear the encoded_response before handling input if set to true */
			std::vector<std::string>& encoded_response() {
				return m_encoded_response;
			}
			void clear_response() {
#ifdef __MENTOC_ENCODED_MESSAGE_DEBUG_OUTPUT__
				std::cerr << green_str("clearing response") << "\n";
				for(auto c : m_encoded_response) {
					std::cerr << "response: '" << c << "'\n";
				}
#endif
				m_encoded_response.clear();
			}
			void flush() {
				m_debug("flushing..");
				std::string m;
				for(const auto& s : m_encoded_response) {
					m_debug("encoded response being flushed: '" << s << "'");
					m += s;
				}
				m_builder_ptr->set_scripted_response(m);
				m_encoded_response.clear();
			}
			/** show the player the available slots setup for this class */
			void display_slot_list() {
				std::string list;
				for(const auto& item : m_slot_list) {
					m_builder_ptr->sendln(
					    CAT(
					        "  {gld}|:: ",item.data(),
					        "{/gld}"
					    )
					);
				}
			}
			void display_signatures() {
				if(!m_builder_ptr->is_executing_js()) {
					m_builder_ptr->pager_start();
				}
				for(const auto& pair : m_signatures) {
					std::vector<char> buffer;
					buffer.resize(pair.second.length() + m_base_command.length() + 16);
					std::fill(buffer.begin(),buffer.end(),0);
					snprintf(&buffer[0],buffer.size()-1,pair.second.c_str(),m_base_command.c_str());
					push_encoded_ok(&buffer[0]);
				}
				if(!m_builder_ptr->is_executing_js()) {
					m_builder_ptr->pager_end();
					m_builder_ptr->page(0);
				}
			}

		protected:
			/** ================================================================================= */
			/** ### [ SECTION: helpers ]                                                      ### */
			/** ### [ description ]:                                                          ### */
			/** ###   most of these functions are used internally by various other member     ### */
			/** ###   functions, but can also be useful to a child class if parsing args.     ### */
			/** ###---------------------------------------------------------------------------### */
			/** ###                           [ START ]                                       ### */
			/** ###---------------------------------------------------------------------------### */

			std::string accumulate_from(const std::vector<std::string>& args,std::size_t index) {
				if(args.size() <= index) {
					return "";
				}
				std::string value;
				for(auto i = index; i < args.size(); i++) {
					value += args[i];
					if(i + 1 < args.size()) {
						value += " ";
					}
				}
				return value;
			}
			/**
			 * utility function which will extract an integer from the given index
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
			/**
			 * utilitiy function which will extract the exact profile given the index when
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
			void push_encoded_error(std::string msg) {
				push_encoded_message(msg,msg,MSG_ERROR);
			}
			void push_encoded_ok(std::string msg) {
				push_encoded_message(msg,msg,MSG_SUCCESS);
			}



			/** proxy function to set slot on specific orm class */

			/** ###----------------------------------------------------------------------------### */
			/** ### it's safe to ignore everything below this line unless you need to refactor ### */
			/** ###----------------------------------------------------------------------------### */
		private:
			std::string m_footer;
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


			/** helper function to handle communication of messages. supports encoding if user is executing js */
			void push_encoded_message(std::string msg, std::string encoded,uint8_t msg_type) {
#ifdef __MENTOC_ENCODED_MESSAGE_DEBUG_OUTPUT__
				std::cerr << green_str("push encoded message:'") << msg << "', encoded:'" << encoded << "'\n";
#endif
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

			/**
			 * remove <vnum> <slot>...<slot-N>
			 */
			bool dispatch_remove_slot(std::string argument) {
				auto args = mods::util::subcmd_args<64,args_t>(argument.data(),"remove");
				if(!args.has_value()) {
					return false;
				}
				auto cmd_args = args.value();
				/**
				 * cmd_args will be: [0] => remove, [1] => vnum ... [2] => slot ... [N] => slot-N
				 */
				if(cmd_args.size() < 3) {
					push_encoded_error("not enough arguments");
					return true;
				}
				auto opt_profile = extract_profile("remove",argument);
				if(!opt_profile.has_value()) {
					push_encoded_error("couldn't find profile for argument");
					return true;
				}
				auto& profile = opt_profile.value();

				/** custom command override */
				if(has_custom_command_for("remove")) {
					return tuple_wrap(m_custom_command_map["remove"](cmd_args,argument,profile));
				}
				for(unsigned i = 2; i < cmd_args.size(); i++) {
					auto status = profile->set_slot(cmd_args[i],"");
					if(!std::get<0>(status)) {
						push_encoded_message(std::get<1>(status), std::get<1>(status),MSG_ERROR);
					}
				}
				return true;
			}
			/**
			 * dispatches the set command.
			 * Nbuild set <vnum> <slot_key> <value>
			 */
			bool dispatch_slot(std::string argument) {

				m_debug("dispatch_slot argument: '" << argument << "'");
				auto args = mods::util::subcmd_args<64,args_t>(argument.data(),"set");
				if(!args.has_value()) {
					m_debug("not enough arguments to dispatch_slot");
					return false;
				}
				auto cmd_args = args.value();
				/**
				 * cmd_args will be: [0] => set, [1] => vnum ... [2] => key [3] => value
				 */
				if(cmd_args.size() < 4) {
					push_encoded_message("not enough arguments","argcount",MSG_ERROR);
					m_debug("[2] not enough arguments to dispatch_slot");
					return true;
				}
				auto opt_profile = extract_profile("set",argument);
				if(!opt_profile.has_value()) {
					m_debug("couldnt find profile for argument");
					push_encoded_message(
					    CAT("couldn't find profile for argument"),
					    CAT("noprof"),MSG_ERROR
					);
					return true;
				}
				/** custom command override */
				if(has_custom_command_for("set")) {
					m_debug("dispatching set command since has_custom_command_for is true");
					return tuple_wrap(m_custom_command_map["set"](cmd_args,argument,opt_profile.value()));
				}
				auto& profile = opt_profile.value();
				auto status = profile->set_slot(cmd_args[2],cmd_args[3]);
				if(!std::get<0>(status)) {
					m_debug("error with profile->set_slot(): " << std::get<1>(status));
					push_encoded_message(std::get<1>(status),
					                     std::get<1>(status),MSG_ERROR
					                    );
					return true;
				}

				push_encoded_message(std::get<1>(status),
				                     std::get<1>(status),MSG_SUCCESS
				                    );
				m_debug("dispatch_slot succeeded: '" << std::get<1>(status) << "'");
				return true;
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
			virtual bool dispatch_multi_vnum_action(std::string argument) {
				m_debug("base class dispatch_multi_vnum_action");
				return true;
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
			 * dispatches reload-all
			 * example:
			 * 	Nbuild reload-all
			 */
			bool dispatch_reload_all_action(std::string argument) {
				auto args = mods::util::subcmd_args<30,args_t>(argument.data(),"reload-all");
				if(!args.has_value()) {
					return false;
				}
				auto cmd_args = args.value();
				if(has_custom_command_for(cmd_args[0])) {
					return tuple_wrap(m_custom_command_map[cmd_args[0]](cmd_args,argument,nullptr));
				}
				clear();
				load_all();
				push_encoded_ok("done");
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
			bool dispatch_help(std::string argument) {
				auto args = mods::util::subcmd_args<64,args_t>(argument.data(),"help");
				if(!args.has_value()) {
					return false;
				}
				auto cmd_args = args.value();
				if(has_custom_command_for(cmd_args[0])) {
					return tuple_wrap(m_custom_command_map[cmd_args[0]](cmd_args,argument,nullptr));
				}
				display_signatures();
				push_encoded_ok(m_footer);
				return true;
			}
	};//end struct
};//end namespace
#undef m_debug
#undef m_error
#endif
