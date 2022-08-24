#ifndef __MENTOC_MODS_DIALOG_TREE_HEADER__
#define  __MENTOC_MODS_DIALOG_TREE_HEADER__

#include <forward_list>
#include <functional>
#include <memory>
#include <vector>

namespace mods::dialog_tree {
	using screen_id_t = uint16_t;
	using id_type_t = uint32_t;

	/** =============================================================== */
	/** = STRUCT precondition_t                                         */
	/** =============================================================== */
	struct precondition_t {
			enum type_t : uint8_t {
				NONE = 0,
				ATTACK,
				DEFEND,
				DEFEAT,
				DEFEAT_WITH,
				GIVE_TO,
				SAVE,
				STEAL_FROM,
				TALK_TO,
				USE_ABILITY,
				WALK_BY,
			};

			/** ============================================================= */
			/** = Constructors                                                */
			/** ============================================================= */
			precondition_t(type_t t, std::string_view d);
			/** Talk to an NPC */
			precondition_t(std::string_view npc);
			precondition_t()  = default;


			/** ============================================================= */
			/** = Member variables                                            */
			/** ============================================================= */
			void assign(auto& s);
			void attack(std::string_view npc);
			void defeat(std::string_view npc);
			void defeat_with(std::string_view target,std::string_view npc);
			void defend(std::string_view npc);
			void give_to(std::string_view item,std::string_view npc);
			void save(std::string_view npc);
			void set_none();
			void steal_from(std::string_view npc);
			void steal_specific_item_from(std::string_view item,std::string_view npc);
			void talk_to(std::string_view npc);
			void use_ability(std::string_view ability,std::string_view npc);
			void walk_by(std::string_view npc);

			/** ============================================================= */
			/** = Member variables                                            */
			/** ============================================================= */
			std::vector<char> data;
			type_t            type;

		public:
			/** ============================================================= */
			/** = Destructor                                                  */
			/** ============================================================= */
			~precondition_t() = default;
	};

	/** =============================================================== */
	/** = END struct precondition_t                                     */
	/** =============================================================== */

	using precondition_ptr = precondition_t*;
	using state_logic_t    = std::function<void(void*,void*)>;
	using variable_data_t  = std::vector<char>;

	/** =============================================================== */
	/** = STRUCT state_t                                                */
	/** =============================================================== */
	struct state_t {
			/** ============================================================= */
			/** = Constructors                                                */
			/** ============================================================= */
			state_t(state_logic_t l,variable_data_t&& d) :
				data(d), logic(l) {
			}
			state_t() = default;

			/** ============================================================= */
			/** = Member functions                                            */
			/** ============================================================= */
			void run(void* c);

			/** ============================================================= */
			/** = Member variables                                            */
			/** ============================================================= */
			std::vector<std::string> code;
			variable_data_t          data;
			id_type_t                id;
			state_logic_t            logic;

			/** ============================================================= */
			/** = Destructor                                                  */
			/** ============================================================= */
		public:
			~state_t() = default;
	};

	struct conversation_t;

	/** =============================================================== */
	/** = STRUCT option_t                                               */
	/** =============================================================== */
	struct option_t {
			enum operation_t : uint16_t {
				OP_NONE,
				OP_GOTO,
				OP_EFFECT,
				OP_IF,
				OP_ADD_OPTION,
				OP_CUSTOM,
			};
			enum direction_t : uint8_t {
				D_LOGIC = 0,
				D_ASK_LOGIC,
				D_ANSWER_LOGIC,
				D_ASK,
				D_ANSWER,
				D_SAY_GOODBYE,
				D_ASK_WITH_IMMEDIATE_ANSWER_WITH_LOGIC,
			};

			/** ============================================================= */
			/** = Constructors                                                */
			/** ============================================================= */
			option_t(
			    std::string_view question,
			    conversation_t* load_this_conversation
			);
			option_t(
			    std::string_view msg,
			    conversation_t* load_this_conversation,
			    direction_t d
			);
			option_t(
			    std::string_view question,
			    std::vector<std::string> code
			);

			/** ============================================================= */
			/** = Member functions                                            */
			/** ============================================================= */
			void assign_context_data(auto& c);
			bool is_simple_goto() const;
			void set_as_answer_goto(std::string_view answer,conversation_t* c);
			void set_as_effect(std::string_view line);
			void set_as_goto(std::string_view question,conversation_t* c);
			void set_as_question_with_immediate_answer_with_logic(std::string_view question,std::string_view answer,std::string_view logic);
			void set_as_question_with_immediate_answer_with_logic(std::string_view question,std::string_view answer,const std::vector<std::string>& logic);
			void set_question_with_logic(std::string_view question, std::vector<std::string> code);

			/** ============================================================= */
			/** = Member variables                                            */
			/** ============================================================= */
			std::vector<char>           context_data;
			direction_t                 direction;
			conversation_t*             goto_conversation;
			id_type_t                   id;
			std::forward_list<state_t*> logic;
			std::string                 message;
			operation_t                 operation;

		public:
	};




	/** =============================================================== */
	/** = STRUCT conversation_t                                         */
	/** =============================================================== */
	struct conversation_t {
			/** ============================================================= */
			/** = Constructors                                                */
			/** ============================================================= */
			conversation_t(std::string_view title,std::string_view npc);

			/** ============================================================= */
			/** = Member functions                                            */
			/** ============================================================= */
			void add_answer_goto_conversation(
			    std::string_view answer,
			    conversation_t* load_this_conversation
			);
			void add_option(option_t* opt);
			void add_question_goto_conversation(
			    std::string_view question,
			    conversation_t* load_this_conversation
			);
			void add_question_with_immediate_answer(
			    std::string_view question,
			    std::string_view answer
			);
			void add_question_with_immediate_answer_and_logic(
			    std::string_view question,
			    std::string_view answer,
			    const std::vector<std::string>& logic
			);
			void add_question_with_immediate_answer_and_logic(
			    std::string_view question,
			    std::string_view answer,
			    std::string_view logic
			);
			option_t* add_question_with_logic(
			    std::string_view question,
			    const std::vector<std::string>& logic
			);
			void remove_option(option_t* opt);
			void set_mode(const std::vector<std::string>& logic);
			void set_mode(std::string_view logic);


			/** ============================================================= */
			/** = Member variables                                            */
			/** ============================================================= */
			id_type_t                    id;
			std::forward_list<option_t*> options;
			precondition_t               precondition;
			/** [Blacksmith.intro] */
			std::string                  title;
			/** talk_to("A Red Cloak Blacksmith") */

			/** ============================================================= */
			/** = Destructor                                                  */
			/** ============================================================= */
		public:
			~conversation_t() = default;
	};

	/**
	 [Blacksmith.intro] :: [talk_to("A Red Cloak Blacksmith")]
	  -> "Hello, are you the blacksmith here?"
			-[goto Marduk.servant]
		-> "Can you tell me about the Elven Guard?"
			-[goto Elven.questions]
		-> "Goodbye."
			-[close]

		[Marduk.questions]
		 -> "Can you tell me more about Lord Marduk?"
		 	 <- "What is it you would like to know?"
		     |-> -[add_option(Marduk.about)]
		 	-[goto Marduk.about]
		 -> "[Hard-ass]: He has some nerve invading that village in Mount Orthos..."
		  |->[script]
			  effect('+5 rage, -10 reputation(Marduk)');
			  if(roll(player,intimidation) > roll("2d6")){
			  	add_option(Marduk.motivation);
			  	add_option(Marduk.revenge_rumors);
				}
			  if(roll(player,unlucky) < roll("2d6")){
			  	alert_guards();
				}
			|->[/script]
		 -> "I'd like to talk about something else"
		  -[return]

		[Marduk.about]
		 -> -[mode(redisplay)]
		 -> "How long has he been in power?"
		   <- "Oh, quite a long time actually... way before my time even.. heh.. "
			  |-> -[emote("The Blacksmith laughs at himself... embarrassed...")]
		 -> "Why is he called the Lord of The Seven Seas?"
		   <- "... long dialog part 1 ... "
		 -> "Who is his second in command?"
		   -> "... long dialog part 2 ... "
			   |-> -[emote("The Blacksmith looks to have shed a tear.. he wipes it away in haste.. ")]
		 -> "I'd like to talk about something else..."
		   -[return(Marduk.questions)]


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
		 <- [segway] t"Sure. What would you like to know?"
		 -> "Is it true he was born to the Elven Guard?"
		   <- "Yes, his mother and father were of nobility ...
			    " ... - dialog - ... "
				  -[up]-
		 -> "What is the knife he carries with him?"
		   <- "The knife of Aegis... "
			    " ... - dialog - ... "
					-[up]-
		 -> "I'm done talking about Lord Marduk..."
		    -[return]-

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

	void entry();

};


#endif
