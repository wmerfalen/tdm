#include "dialog-tree.hpp"
//#include <boost/pool/simple_segregated_storage.hpp>
#include <boost/pool/object_pool.hpp>
#include <iostream>

#define __MENTOC_SHOW_MODS_DIALOG_TREE_DEBUG_OUTPUT__
#ifdef __MENTOC_SHOW_MODS_DIALOG_TREE_DEBUG_OUTPUT__
#define m_debug(MSG) mentoc_prefix_debug("[mods::dialog_tree::debug]")  << MSG << "\n";
#define m_error(MSG) mentoc_prefix_debug(red_str("[mods::dialog_tree::ERROR]"))  << MSG << "\n";
#else
#define m_debug(MSG) ;;
#define m_error(MSG) ;;
#endif

namespace mods::dialog_tree {
	namespace memory_pool {
		boost::object_pool<precondition_t> preconditions{64,0};
		boost::object_pool<state_t> states{64,0};
		boost::object_pool<option_t> options{64,0};
		boost::object_pool<conversation_t> conversations{64,0};
	};
	namespace memory {
		using id_type_t = uint32_t;
		id_type_t option_id() {
			static id_type_t id = 0;
			return ++id;
		}
		id_type_t state_id() {
			static id_type_t id = 0;
			return ++id;
		}
		id_type_t conversation_id() {
			static id_type_t id = 0;
			return ++id;
		}
		id_type_t precondition_id() {
			static id_type_t id = 0;
			return ++id;
		}
		precondition_t* create_precondition(auto&& _p) {
			auto p = memory_pool::preconditions.construct(_p);
			p->id = precondition_id();
			return p;
		}
		state_t* create_state(auto&& _s) {
			auto s = memory_pool::states.construct(_s);
			s->id = state_id();
			return s;
		}
		option_t* create_option(auto&& _o) {
			auto o = memory_pool::options.construct(_o);
			o->id = option_id();
			return o;
		}
		conversation_t* create_conversation(auto&& _c) {
			auto c = memory_pool::conversations.construct(_c);
			c->id = conversation_id();
			return c;
		}
		void release_precondition(precondition_t* ptr) {
			memory_pool::preconditions.free(ptr);
		}
		void release_state(state_t* ptr) {
			memory_pool::states.free(ptr);
		}
		void release_option(option_t* ptr) {
			memory_pool::options.free(ptr);
		}
		void release_conversation(conversation_t* ptr) {
			memory_pool::conversations.free(ptr);
		}
	};
	void state_t::run(void* c) {
		logic(c,(void*)this);
	}
	using namespace mods::dialog_tree::memory;
	using dir = option_t::direction_t;
	using precon_type_t = precondition_t::type_t;
	precondition_t::precondition_t(precon_type_t t, std::string_view d) {
		type = t;
		data.resize(d.size());
		std::copy(d.begin(),d.end(),data.begin());
	}
	void precondition_t::attack(std::string_view npc) {
		assign(npc);
	}
	void precondition_t::defend(std::string_view npc) {
		assign(npc);
	}
	void precondition_t::defeat(std::string_view npc) {
		assign(npc);
	}
	void precondition_t::defeat_with(std::string_view target,std::string_view npc) {
		/** TODO serialize two arguments into data */
		assign(npc);
	}
	void precondition_t::give_to(std::string_view item,std::string_view npc) {
		/** TODO serialize two arguments into data */
		assign(npc);
	}
	void precondition_t::save(std::string_view npc) {
		assign(npc);
	}
	void precondition_t::steal_from(std::string_view npc) {
		assign(npc);
	}
	void precondition_t::steal_specific_item_from(std::string_view item,std::string_view npc) {
		/** TODO serialize two arguments into data */
		assign(npc);
	}
	void precondition_t::talk_to(std::string_view npc) {
		assign(npc);
	}
	void precondition_t::use_ability(std::string_view ability,std::string_view npc) {
		/** TODO serialize two arguments into data */
		assign(npc);
	}
	void precondition_t::walk_by(std::string_view npc) {
		assign(npc);
	}
	void precondition_t::assign(auto& s) {
		data.resize(s.size());
		std::copy(s.begin(),s.end(),data.begin());
	}

	void conversation_t::add_question_goto_conversation(
	    std::string_view question,
	    conversation_t* load_this_conversation
	) {
		auto opt_who = create_option(option_t(question,load_this_conversation));
		options.push_front(opt_who);
	}
	void conversation_t::add_answer_goto_conversation(
	    std::string_view answer,
	    conversation_t* load_this_conversation
	) {
		auto opt = create_option(option_t(answer,load_this_conversation,dir::D_ANSWER));
		options.push_front(opt);
	}
	option_t::option_t(
	    std::string_view msg,
	    conversation_t* load_this_conversation,
	    direction_t d) {
		message.assign(msg.data());
		set_as_goto(msg,load_this_conversation);
		direction = d;
	}
	/**
	 * Talk to NPC with title of conversation
	 */
	conversation_t::conversation_t(std::string_view t,std::string_view npc) {
		title = t;
		precondition.talk_to(npc);
	}
	/**
	 * Talk to constructor.
	 * Talk to NPC
	 */
	precondition_t::precondition_t(std::string_view npc) {
		type = precon_type_t::TALK_TO;
		assign(npc);
	}
	option_t::option_t(
	    std::string_view question,
	    conversation_t* load_this_conversation
	) {
		set_as_goto(question,load_this_conversation);
	}
	void conversation_t::add_option(option_t* opt) {
		options.push_front(opt);
	}
	void conversation_t::remove_option(option_t* opt) {
		std::cerr << "[remove_option] stub\n";
		options.remove_if([&opt](const auto* option) -> bool {
			return option->id == opt->id;
		});
	}
	void option_t::set_question_with_logic(
	    std::string_view question,
	    std::vector<std::string> code
	) {
		message = question;
		direction = dir::D_ASK_LOGIC;
		goto_conversation = nullptr;
		auto s = create_state(state_t());
		s->code = code;
		logic.push_front(s);
	}
	option_t::option_t(
	    std::string_view question,
	    std::vector<std::string> logic) {
		set_question_with_logic(question,logic);
	}
	option_t* conversation_t::add_question_with_logic(
	    std::string_view question,
	    std::vector<std::string> logic
	) {
		auto opt = create_option(option_t(question,logic));
		options.push_front(opt);
		return opt;
	}

	/** [Blacksmith.intro] */
	void entry() {
		std::string mob = "A Red Cloak Blacksmith";

		auto servant = create_conversation(conversation_t("Marduk.servant",mob));
		auto marduk_q = create_conversation(conversation_t("Marduk.questions",mob));
		marduk_q->add_question_with_logic(
		    "[Hard-ass]: He has some nerve invading that village in Mount Orthos...",
		std::vector<std::string> {
			"-[",
			" effect(+5 rage,-10 reputation(Marduk)",
			" if(roll(player,intimidation) > 2d6)",
			"   add_option(Marduk.motivation)",
			"   add_option(Marduk.revenge_rumors)",
			" endif",
			" if(roll(player,unlucky) < 2d6)",
			"   alert_guards()",
			" endif"
			"]-",
		}

		);

		servant->add_answer_goto_conversation("Why yes! I am a loyal servant of Lord Marduk!",marduk_q);

		auto b = create_conversation(conversation_t("Blacksmith.intro",mob));

		b->add_question_goto_conversation("Hello, are you the blacksmith here?",servant);

		//opt_who->add_question_goto_conversation("Can you tell me more about Lord Marduk?",servant);
		//-> "Can you tell me more about Lord Marduk?"
		//	-[goto Marduk.servant]
		//-> "[Hard-ass]: He has some nerve invading that village in Mount Orthos..."
		// -[effect(+5 rage,-10 reputation(Marduk)]
		// -[if(roll(player,intimidation) > 2d6)]
		// 	[add_option(Marduk.motivation)]
		// 	[add_option(Marduk.revenge_rumors)]
		// -[if(roll(player,unlucky) < 2d6)]
		// 	-[alert_guards()]
		//-> "I'd like to talk about something else"
		// -[return]


		//
		// [Marduk.servant]
		//  <- "Why yes! I am a loyal servant of Lord Marduk!"
		//   -[add_option(Marduk.questions)]
		//
		//auto mservant = create_conversation(conversation_t("Marduk.servant",mob));
		//mservant->options

		//-> "Can you tell me about the Elven Guard?"
		//    -[goto Elven.questions]
		//    -> "Goodbye."
		//    -[close]
		//    b->options
		//  }

	}
	void init() {
		entry();
	}

};
