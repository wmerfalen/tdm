#ifndef __MENTOC_MODS_DIALOG_TREE_HEADER__
#define  __MENTOC_MODS_DIALOG_TREE_HEADER__

namespace mods::dialog_tree {
	using screen_id_t = uint16_t;

	struct node_t {
			node_t(std::string_view msg,screen_id_t id) :
				m_message(msg.data()), m_screen(id) {}
			node_t() = delete;
			node_t() = default;
		private:
			std::string m_message;
			screen_id_t m_screen;
	};

	struct conversation_t {
		std::forward_list<node_t> options;
	};
	static inline void entry() {
		conversation_t convo;
		convo.options.push_front("Hello, are you the blacksmith here?",0);
		convo.options.push_front("Can you tell me about the Elven Guard?",0);
		convo.options.push_front("Goodbye",0);
	}

	/**
	 [Blacksmith.intro] :: [initiate_with("A Red Cloak Blacksmith")]
	  -> "Hello, are you the blacksmith here?"
			-[goto Marduk.servant]
		-> "Can you tell me about the Elven Guard?"
			-[goto Elven.questions]
		-> "Goodbye."
			-[close]

		[Marduk.questions]
		 -> "Can you tell me more about Lord Marduk?"
		 	-[goto Marduk.servant]
		 -> "[Hard-ass]: He has some nerve invading that village in Mount Orthos..."
		  -[effect(+5 rage,-10 reputation(Marduk)]
			-[if(roll(player,intimidation) > 2d6)]
				[add_option(Marduk.motivation)]
				[add_option(Marduk.revenge_rumors)]
			-[if(roll(player,unlucky) < 2d6)]
				-[alert_guards()]
		 -> "I'd like to talk about something else"
		  -[return]

		[Marduk.motivation]
		 <- "... wha?.. I.. well.. okay.. look ... "
		    *the blacksmith looks around and lowers his voice*
				" ... - dialog - ... "
		    -[return]

		[Marduk.revenge_rumors]
		 -> "That still doesn't answer ... dialog ... "
		   -[always_reply(Blacksmith.scared)]

		[Blacksmith.scared]
		 <- "I've already spoken too much! I cannot and WILL NOT "
		    "speak any more treachery!"
				*the blacksmith goes back to hammering away...*

		[Marduk.servant]
		 <- "Why yes! I am a loyal servant of Lord Marduk!"
			-[add_option(Marduk.questions)]

		[Elven.questions]
		 -> [if(roll(player,reputation(Marduk)) > 0)]
		 	   -[goto Elven.secrets]
			  -[else goto Elven.no_trust]

		[Elven.secrets]
		 <- "It has been said that the Elven Guard is preparing ...
		     ...
				 -- long dialog that spans multiple pages --
				 ...
				"
			-[add_option(Elven.conquest)]
		  -[edge]

		[Elven.why_not]
		 -> "Why can't you tell me about the Elven Guard?"
		  -[effect(+5 irritation,+5 rage,-10 reputation(Marduk))]
			-[evaluate]

		[Elven.conquest]
		 -> [autoplay] "So that means Marduk's empire .. "
		   <- [autoplay] "... crumbles if the Sword of Elms is nothing more than a fairy tale... "
			   -> [autoplay] "That can't be.. How could something so crucial be overlooked.. ?"
			     <- [autoplay] "The queen's guard wants nothing more than power that reaches the nine realms..
					    ...
							-- dialog about the queen --
						  ...
							"
		   -> "A conspiracy of the highest level."
			   <- "Precisely."
	         -[unlock("Quest.conspirator")]
					 -[effect(+10 reputation(Blacksmith),-10% cost(at(Blacksmith)))]
		 <- "It's even been said that the Elven Conquest of ...
		    ...
				-- long dialog about elven conquest --
				...
				"


		[Elven.no_trust]
		 <- "That is not something I can talk to just *anybody* about."
		  -[goto Blacksmith.intro]
			-[add_option(Elven.why_not)]

	*/
};

#include "util.hpp"


#endif
