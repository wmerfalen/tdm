#include "utils.h"
#include "structs.h"
#include "comm.h"
#include <array>

namespace mods {
	struct extra_desc_data;
};

static inline std::string default_yaml_file(std::string type){
	return type + ".yml";
}

//void room_data::ex_descriptions_append(mods::extra_desc_data&& other){
//	m_ex_descriptions.emplace_back(std::move(other));
//}
//void room_data::ex_descriptions(mods::extra_desc_data&& other){
//	m_ex_descriptions = std::move(other);
//}
//void room_data::ex_descriptions(const std::vector<mods::extra_desc_data>& other){
//	m_ex_descriptions = other;
//}
//std::vector<mods::extra_desc_data> room_data::ex_descriptions() const {
//	return m_ex_descriptions;
//}

#include "mods/util.hpp"
void obj_data::init(){
	std::fill(m_capabilities.begin(),m_capabilities.end(),0);
	item_number = 0;
	in_room = 0;
	name = strdup("<default-1>");
	description = name;
	short_description = name;
	action_description = name;
	ex_description.clear();
	carried_by = nullptr;
	worn_by = nullptr;
	worn_on = -1;
	in_obj = nullptr;
	contains = nullptr;
	CREATE(next_content,struct obj_data,1);
	next = nullptr;
	ai_state = 0;
	obj_flags.init();
	type = 0;
	this->uuid = mods::globals::obj_uuid();

#define MENTOC_OBJ_INITIALIZE_CONSTRUCTOR(r,data,CLASS_TYPE) \
			this->BOOST_PP_CAT(m_,CLASS_TYPE) = nullptr;
BOOST_PP_SEQ_FOR_EACH(MENTOC_OBJ_INITIALIZE_CONSTRUCTOR, ~, MENTOC_ITEM_TYPES_SEQ)

	std::cerr << "obj_data() assigned uuid: " << this->uuid << "\n";
}
		void obj_data::feed(std::string in_type,std::string_view feed_file){
			std::string type = in_type;
			std::transform(type.begin(),type.end(),type.begin(),
					[](unsigned char c){ return std::tolower(c); });
			type = type.substr(strlen("ITEM_"));
			std::cerr << "[parsed type]:'" << type << "'\n";
#define MENTOC_OBJ_DATA_FEED_DUAL(r,data,CLASS_TYPE) \
			if(type.compare( BOOST_PP_STRINGIZE(CLASS_TYPE) ) == 0){\
				std::cerr << "[obj_data::feed(str,str) feeding type: " << BOOST_PP_STRINGIZE(CLASS_TYPE) << "\n";\
				this->CLASS_TYPE(feed_file);\
			}

			BOOST_PP_SEQ_FOR_EACH(MENTOC_OBJ_DATA_FEED_DUAL, ~, MENTOC_ITEM_TYPES_SEQ)

		}
void obj_flag_data::init(){
	memset(value,0,sizeof(value));
	holds_ammo = 0;
	is_ammo = 0;
	type = 0;
	type_flag = 0;
	weapon_flags = 0;
	ammo_max = 0;
	ammo = 0;
	clip_size = 0;
	wear_flags = 0;
	extra_flags = 0;
	weight = 0;
	cost = 0;
	cost_per_day = 0;
	timer = 0;
	bitvector = 0;
}
bool obj_data::flagged(int value){
	return ((obj_flags.extra_flags) & (value));
}
#ifdef __MENTOC_USE_PQXX_RESULT__
void obj_data::feed(const pqxx::result::reference & row){
#else
	void obj_data::feed(pqxx::row row){
#endif
		/**
		 * Weapon flow
		 * --------------
		 - Prototype Phase
		 - Item has basic features
		 - Item describes the object and can be used in game

		 - Creation phase
		 - [CONDITIONAL]
		 - CONDITION: Item placed in container via zone command
		 - OR CONDITION: Item awarded by quest
		 - Unique item number placed on item
		 - Save in db

		 - [CONDITIONAL] Decoration phase
		 - CONDITION: Item belongs to user
		 - Decorate item and load sub-structures (i.e.: weapon())

		 - Item is used
		 - FOREACH item below, save()
		 - Item is attached
		 - Item gets attachments
		 - Item has properties modified
		 - Item gets put in user's inventory

		 - [CONDITIONAL] User drops item
		 - Save room and/or container

*/
		type = mods::util::stoi<int>(row["obj_type"]);
		std::string obj_file = row["obj_file"].c_str();

		switch(type){
			MENTOC_OBJ_DATA_FEED_SWITCH
			default:
				std::cerr << "[obj_data::feed] WARNING! Unknown 'type' value: " << type << "\n";
				break;
		}
	}

#ifdef __MENTOC_USE_PQXX_RESULT__
	void obj_flag_data::feed(const pqxx::result::reference & row){
#else
		void obj_flag_data::feed(pqxx::row row){
#endif
			if(mods::string(row["value_0"]).length()) {
				value[0] = mods::util::stoi<int>(row["value_0"]);
			}

			if(mods::string(row["value_1"]).length()) {
				value[1] = mods::util::stoi<int>(row["value_1"]);
			}

			if(mods::string(row["value_2"]).length()) {
				value[2] = mods::util::stoi<int>(row["value_2"]);
			}

			if(mods::string(row["value_3"]).length()) {
				value[3] = mods::util::stoi<int>(row["value_3"]);
			}
			ammo_max = mods::util::stoi<int>(row["ammo_max"]);
			clip_size = mods::util::stoi<int>(row["clip_size"]);
			weapon_flags =mods::util::stoi<weapon_type_t>(row["weapon_flags"]);
			bitvector = mods::util::stoi<int>(row["bitvector"]);
			type_flag =mods::util::stoi<int>(row["type_flag"]);
			wear_flags = mods::util::stoi<int>(row["wear_flags"]);
			extra_flags =mods::util::stoi<int>(row["extra_flags"]);
			weight = mods::util::stoi<int>(row["weight"]);
			cost = mods::util::stoi<int>(row["cost"]);
			cost_per_day =mods::util::stoi<int>(row["cost_per_day"]);
			timer = mods::util::stoi<int>(row["timer"]);
		}
		std::vector<mods::extra_desc_data>& room_data::ex_descriptions(){
			log("DEPRECATED: implement room_data::ex_descriptions or stop calling it");
			return m_ex_descriptions;
		}
		char_player_data::char_player_data(){
			name.assign("");         /* PC / NPC s name (kill ...  )         */
			short_descr.assign("");  /* for NPC 'actions'                    */
			long_descr.assign("");   /* for 'look'             */
			description.assign("");  /* Extra descriptions                   */
			title.assign("");        /* PC / NPC's title                     */
			sex =  chclass = level = 0;
			hometown = 0;
			memset(&time,0,sizeof(time));
			weight = height = 0;
			/** TODO: phase this out */
			passwd.assign("");
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
			desc.reset();
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
			/** TODO: m_carrying */
			next = nullptr;
			next_fighting = nullptr;
			followers = nullptr;
			master = nullptr;
			player_ptr = nullptr;
			goal = 0;
			disorient = 0;
			state = 0;
			builder_data.reset();
			//has_desc = false;
			//desc.reset();
			player_specials = std::make_shared<player_special_data>();
		}

		std::string_view room_data::overhead(const lense_type_t& lense){
			switch(lense){
				case NORMAL_SIGHT:
					if(std::find(m_textures.begin(),m_textures.end(),texture_type_t::GRASS) != m_textures.end()){
						return "{grn}==={/grn}";
					}
					if(std::find(m_textures.begin(),m_textures.end(),texture_type_t::CEMENT) != m_textures.end()){
						return "{grey}[ ]{/grey}";
					}
					if(std::find(m_textures.begin(),m_textures.end(),texture_type_t::OUTSIDE) != m_textures.end()){
						return "{blu}[ ]{/blu}";
					}
					if(std::find(m_textures.begin(),m_textures.end(),texture_type_t::INSIDE) != m_textures.end()){
						return "{wht}[ ]{/wht}";
					}
					return "[ ]";
					break;

				case THERMAL_GOGGLES:
					if(std::find(m_textures.begin(),m_textures.end(),texture_type_t::GRASS) != m_textures.end()){
						return "{blu}...{/blu}";
					}
					if(std::find(m_textures.begin(),m_textures.end(),texture_type_t::CEMENT) != m_textures.end()){
						return "{blu}...{/blu}";
					}
					if(std::find(m_textures.begin(),m_textures.end(),texture_type_t::OUTSIDE) != m_textures.end()){
						return "{red}...{/red}";
					}
					/** TODO: if we have mobs in this room, mark it with a red indicator to simulate heat signatures */
					break;

				case NIGHT_VISION_GOGGLES:
					break;


				case AERIAL_DRONE:
					break;

				case AERIAL_DRONE_THERMAL:
					break;

				case AERIAL_DRONE_NIGHT_VISION:
					break;


				case RC_DRONE:
					break;

				case RC_DRONE_THERMAL:
					break;

				case RC_DRONE_NIGHT_VISION:
					break;

				default:
					return "[ ]";
			}
			return "[ ]";
		}
		const std::vector<room_data::texture_type_t>& room_data::textures() const { 
			return m_textures;
		}
		void room_data::init(){
			number = 0;
			zone = 0;
			sector_type = 0;
			contents = nullptr;
			people = nullptr;
			for(unsigned i = 0; i < NUM_OF_DIRS;i++){ 
				dir_option[i] = nullptr;
			}
		}

		room_data::room_data() {
			init();
		}
		room_data::room_data(const room_data& r){
			init();
			number = r.number;
			zone = r.zone;
			sector_type = r.sector_type;
			contents = r.contents;
			people = r.people;
		}
		room_data::~room_data() {
			for(unsigned i = 0; i < NUM_OF_DIRS; i++){
				if(dir_option[i] != nullptr){
					free(dir_option[i]);
				}
			}
		}

		void room_data::set_dir_option(byte i,
				const std::string& gen_desc,
				const std::string& keyword,
				const int & ex_info,
				const int & key,
				const room_rnum to_room){
			if(i >= NUM_OF_DIRS){
				return;
			}
			if(dir_option[i] == nullptr){
				dir_option[i] = reinterpret_cast<room_direction_data*>(calloc(sizeof(room_direction_data),1));
			}else{
				free(dir_option[i]);
				dir_option[i] = reinterpret_cast<room_direction_data*>(calloc(sizeof(room_direction_data),1));
			}
			/** FIXME: replace strdup'd members with mods::string */
			dir_option[i]->general_description = strdup(gen_desc.c_str());
			dir_option[i]->keyword = strdup(keyword.c_str());
			dir_option[i]->exit_info = ex_info;
			/** FIXME: make this dynamic when bugs with builder.cpp are cleaned up */
			dir_option[i]->key = 0;
			dir_option[i]->to_room = to_room;
		}
		namespace mods{
			void descriptor_data::queue_output(std::string_view msg, bool newline, bool plain) {
				switch(m_queue_behaviour){
					case queue_behaviour_enum_t::NORMAL:
						if(plain){
							output += msg;
						} else {
							output += mods::globals::color_eval(msg);
						}
						if(newline){ output += "\r\n"; }
						has_output = true;
						return;
					case queue_behaviour_enum_t::IGNORE_ALL:
						has_output = false;
						return;
					case queue_behaviour_enum_t::REDIRECT_TO_PLAYER:
					case queue_behaviour_enum_t::REDIRECT_TO_FILESYSTEM:
					case queue_behaviour_enum_t::REDIRECT_TO_DB:
					default:
						return;
				}
			}
			size_t mods::descriptor_data::flush_output(){
				std::size_t result; 
				switch(m_queue_behaviour){
					default:
					case queue_behaviour_enum_t::NORMAL:
						std::cerr << "mods::descriptor_data::flush_output:: NORMAL\n";
						if(output.size() == 0){ 
							std::cerr << "mods::descriptor_data::flush_output:: no output NORMAL\n";
							has_output = false; 
							return 0; 
						}

						std::cerr << "mods::descriptor_data::flush_output:: write to desc: '" <<
							output << "' NORMAL\n";
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
						return 0;
				}
			}

		};
		builder_data_t::builder_data_t(int type,int start_room,int start_zone) {
			//switch(type){
			//	default:
			//	case ROOM_PAVEMENT:
			//		/** TODO: I want this functionality to be done next
			//		room_pavement_list.emplace_back(start_room,start_zone);
			//		room_pavement_list.back().transact_id = next_room_pavement_transaction_id();
			//		*/
			room_pavements.start_room = start_room;
			room_pavements.transact_id = next_room_pavement_transaction_id();
			room_pavements.zone_id = start_zone;
			//			break;
			//		case ZONE_PAVEMENT:
			//			/** FIXME: this is incomplete code. fix this */
			//			std::cerr << "[builder_data_t::create_pavement] WARNING: zone pavements are experimental!\n";
			//			break;
			//	}
		}
