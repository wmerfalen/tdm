#include "utils.h"
#include "structs.h"
#include "comm.h"
#include <array>

char_player_data::char_player_data(){
  name.clear();         /* PC / NPC s name (kill ...  )         */
  short_descr.clear();  /* for NPC 'actions'                    */
  long_descr.clear();   /* for 'look'             */
  description.clear();  /* Extra descriptions                   */
  title.clear();        /* PC / NPC's title                     */
  sex =  chclass = level = 0;
  hometown = 0;
  memset(&time,0,sizeof(time));
  weight = height = 0;
  /** TODO: phase this out */
  passwd.clear();
	std::fill(m_passwd.begin(),m_passwd.end(),0);
}
char_data::char_data(char_data* o){
		has_desc = false;
		desc = nullptr;
		pfilepos = o->pfilepos;
		uuid = o->uuid;
		last_fight_timestamp = o->last_fight_timestamp;
		nr = o->nr;
		in_room = o->in_room;
		was_in_room = o->was_in_room;
		wait = o->wait;
		drone = o->drone;
		drone_owner = o->drone_owner;
		drone_simulate = o->drone_simulate;
		drone_uuid = o->drone_uuid;
		player = o->player;       /* Normal data                   */
		real_abils = o->real_abils;  /* Abilities without modifiers   */
		aff_abils = o->aff_abils;   /* Abils with spells/stones/etc  */
		points = o->points;        /* Points                        */
		char_specials = o->char_specials;  /* PC/NPC specials    */
		player_specials = o->player_specials; /* PC specials      */
		mob_specials = o->mob_specials;  /* NPC specials     */
		affected = o->affected;       /* affected by what spells       */
		for(unsigned i = 0; i < NUM_WEARS; i++){
			equipment[i] = o->equipment[i];/* Equipment array               */
		}
		carrying = o->carrying;            /* Head of list                  */
		desc = o->desc;         /* NULL for mobiles              */
		next_in_room = o->next_in_room;     /* For room->people - list         */
		next = o->next;             /* For either monster or ppl-list  */
		next_fighting = o->next_fighting;    /* For fighting list               */
		followers = o->followers;        /* List of chars followers       */
		master = o->master;             /* Who is char following?        */
		player_ptr = o->player_ptr;
		goal = o->goal;
		disorient = o->disorient;
		state = o->state;
		builder_data = o->builder_data;
	}
	void char_data::init(){
		has_desc = false;
		desc = nullptr;
		pfilepos = 0;
		uuid = 0;
		last_fight_timestamp = 0;
		nr = 0;
		in_room = 0;
		was_in_room = 0;
		wait = 0;
		drone = 0;
		drone_owner = 0;
		drone_simulate = 0;
		drone_uuid = 0;
		/** TODO: do something smart with this.. maybe call .init() on them 
		memset(&real_abils,0,sizeof(real_abils));
		memset(&aff_abils,0,sizeof(aff_abils));
		memset(&char_specials,0,sizeof(char_specials));
		memset(&mob_specials,0,sizeof(mob_specials));
		*/
		affected = nullptr;
		for(unsigned i = 0; i < NUM_WEARS; i++){
			equipment[i] = nullptr;
		}
		carrying = nullptr;
		desc.reset();
		next_in_room = 0;
		next = nullptr;
		next_fighting = nullptr;
		followers = nullptr;
		master = nullptr;
		player_ptr = nullptr;
		goal = 0;
		disorient = 0;
		state = 0;
		builder_data.reset();
		has_desc = false;
		desc.reset();
		player_specials = std::make_shared<player_special_data>();
	}
namespace mods{
		size_t descriptor_data::queue_output(const std::string &s){
			switch(m_queue_behaviour){
				case queue_behaviour_enum_t::NORMAL:
					output += s;
					has_output = true;
					return s.length();
					break;
				case queue_behaviour_enum_t::IGNORE_ALL:
					has_output = false;
					return 0;
				case queue_behaviour_enum_t::REDIRECT_TO_PLAYER:
				case queue_behaviour_enum_t::REDIRECT_TO_FILESYSTEM:
				case queue_behaviour_enum_t::REDIRECT_TO_DB:
				default:
					return 0;
			}
		}
		size_t mods::descriptor_data::flush_output(){
		std::size_t result; 
			switch(m_queue_behaviour){
				case queue_behaviour_enum_t::NORMAL:
					if(output.size() == 0){ 
						has_output = false; 
						return 0; 
					}

					result = write_to_descriptor(descriptor,output.c_str());

					/* Handle snooping: prepend "% " and send to snooper. */
					if(snoop_by) {
						write_to_output(*snoop_by, "%% %*s%%%%", static_cast<int>(result), output.c_str());
					}
					output.clear();
					has_output = false;
					return (result);
					break;
				case queue_behaviour_enum_t::IGNORE_ALL:
					has_output = false;
					return 0;
				case queue_behaviour_enum_t::REDIRECT_TO_PLAYER:
				case queue_behaviour_enum_t::REDIRECT_TO_FILESYSTEM:
				case queue_behaviour_enum_t::REDIRECT_TO_DB:
				default:
					return 0;
			}
		}
};
