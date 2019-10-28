#include "projectile.hpp"
#include "../handler.h"

#include <cctype>	/* for std::tolower */
/* using directives */
using mw_explosive = mods::weapon::type::explosive;
using mw_rifle = mods::weapon::type::rifle;
using mw_drone = mods::weapon::type::drone;
namespace mods {
	namespace projectile {
		using texture_type_t = room_data::texture_type_t;
		void queue_affect_remove_flashbang(uint64_t cooldown_ticks,room_rnum room_id){
				auto people = mods::globals::room_list[room_id];
				mods::globals::defer_queue->affect_remove_via_callback<
					std::vector<player_ptr_t>,std::array<std::function<void(player_ptr_t)>,2>>(
						cooldown_ticks,
						people,
						{mods::projectile::blindness_clears_up,
						mods::projectile::disorient_clears_up}
				);
		}
#define REMOVE_TEXTURE_QUEUE(t_texture,r_room_id) queue_remove_texture(40,r_room_id,room_data::texture_type_t::t_texture);
		void queue_remove_texture(uint64_t ticks_in_future,room_rnum& room_id,room_data::texture_type_t texture){
			/** TODO: calculate blast radius to remove smoke textures from those rooms */
				mods::globals::defer_queue->detexturize_room(
						ticks_in_future,
						room_id,
						texture
				);
		}
		void propagate_chemical_blast(room_rnum& room_id,obj_data* device){
			REMOVE_TEXTURE_QUEUE(HAZARDOUS_SMOKE,room_id);
		}
		void propagate_chemical_blast(room_rnum& room_id,obj_data* device,std::size_t depth){
			REMOVE_TEXTURE_QUEUE(HAZARDOUS_SMOKE,room_id);
			for(auto &player : mods::globals::room_list[room_id]){
				player->sendln("Propagating chemblast " + std::to_string((1.0) * (depth / 3.0)));
			}
		}
		void emp_damage(room_rnum& room_id,obj_data* object){
			send_to_room(room_id,"Your electronics sizzle and malfunction into uselessness.");
			REMOVE_TEXTURE_QUEUE(EMP,room_id);
		}
		/*
		void damage_room(room_rnum& room_id,obj_data* object){
			auto exp_type = object->explosive()->type;
			if(exp_type == mw_explosive::EMP_GRENADE){
				emp_damage(room_id,object);
				mods::util::texturize_room(room_id,texture_type_t::EMP);
				return;
			}
			if(exp_type == mw_explosive::SMOKE_GRENADE){
				mods::util::texturize_room(room_id,texture_type_t::NON_HAZARDOUS_SMOKE);
				return;
			}
		 	if(exp_type == mw_explosive::REMOTE_CHEMICAL){
				mods::projectile::propagate_chemical_blast(room_id,object);
				return;
			}
			for(auto & person : mods::globals::room_list[room_id]){
				switch(object->explosive()->type){
					case mw_explosive::FRAG_GRENADE:
						explosive_damage(person,object);
						break;
					case mw_explosive::REMOTE_EXPLOSIVE:
						explosive_damage(person,object);
						break;
					case mw_explosive::CLAYMORE_MINE:
						explosive_damage(person,object);
						break;
					case mw_explosive::INCENDIARY_GRENADE:
						explosive_damage(person,object);
						fire_damage(person,object);
						break;
					case mw_explosive::FLASHBANG_GRENADE:
						blind_target(person);
						break;
				}
			}
		}
		*/
		void perform_blast_radius(room_rnum room_id,std::size_t blast_radius,obj_data* device) {
			auto current_room = room_id;
			for(auto & dir : {NORTH, SOUTH, EAST, WEST, UP, DOWN}){
				current_room = room_id;
				for(std::size_t blast_count = blast_radius; blast_count > 0;blast_count--){
					auto dir_option = world[current_room].dir_option[dir];
					if(!dir_option){
						break;
					}
					if(dir_option->to_room == NOWHERE){
						break;
					}
					current_room = dir_option->to_room;
					float damage_multiplier = 0;
					auto opposite = OPPOSITE_DIR(dir);

					/**
					 * It's okay to not have a defaulted case since the
					 * following switch deals only with room textures
					 */
					switch(device->explosive()->type){
						default: break;
						case mw_explosive::EXPLOSIVE_NONE:
							log("SYSERR: EXPLOSIVE_NONE specified in perform_blast_radius");
							return;
						case mw_explosive::REMOTE_CHEMICAL:
							REMOVE_TEXTURE_QUEUE(HAZARDOUS_SMOKE,current_room);
							break;
						case mw_explosive::INCENDIARY_GRENADE:
							REMOVE_TEXTURE_QUEUE(ON_FIRE,current_room);
							break;
						case mw_explosive::EMP_GRENADE:
							REMOVE_TEXTURE_QUEUE(EMP,current_room);
							break;
						case mw_explosive::SMOKE_GRENADE:
							REMOVE_TEXTURE_QUEUE(NON_HAZARDOUS_SMOKE,current_room);
							break;
					}
					for(auto & person : mods::globals::room_list[current_room]){
						switch(device->explosive()->type){
							case mw_explosive::REMOTE_CHEMICAL:
								mods::projectile::propagate_chemical_blast(current_room,device,blast_count);
								break;
							default: return;	/** THis should _NEVER_ happen */
							case mw_explosive::REMOTE_EXPLOSIVE:
							case mw_explosive::CLAYMORE_MINE:
							case mw_explosive::FRAG_GRENADE:
								person->sendln("Shrapnel tears through you" + mods::projectile::fromdirstr(opposite,1,0) + "!");
								damage_multiplier = (1.0 * blast_count) / 3;
								person->sendln("[damage: " + std::to_string(damage_multiplier) + "]");
								break;
							case mw_explosive::INCENDIARY_GRENADE:
								person->sendln("A heated explosion sets the room on fire" + mods::projectile::fromdirstr(opposite,1,0) + "!");
								damage_multiplier = (1.0 * blast_count) / 3;
								person->sendln("[damage: " + std::to_string(damage_multiplier) + "]");
								break;
							case mw_explosive::EMP_GRENADE:
								person->sendln("The effectiveness of your electronics is hindered" + mods::projectile::fromdirstr(opposite,1,0) + "!");
								damage_multiplier = (1.0 * blast_count) / 3;
								person->sendln("[electronics " + std::to_string(damage_multiplier) + "]");
								break;
							case mw_explosive::SMOKE_GRENADE:
								person->sendln("A cloud of smoke billows in" + mods::projectile::fromdirstr(opposite,1,0) + "!");
								break;
							case mw_explosive::FLASHBANG_GRENADE:
								person->sendln("You are partially blinded by flash of light" + mods::projectile::fromdirstr(opposite,1,0) + "!");
								break;
						}
					}
				}
			}
		}
		int grenade_damage(player_ptr_t victim,obj_data* projectile){
			mods::projectile::explosive_damage(victim,projectile);
			return 0;
		}
		int fire_damage(player_ptr_t victim,obj_data* projectile){
			victim->sendln("The room and part of your equipment catch on fire!");
			return 0;
		}
		void disable_electronics(room_rnum room){


		}
		void smoke_room(room_rnum room){

		}
		void blindness_clears_up(player_ptr_t victim){
			victim->remove_affect(AFF_BLIND);
			victim->sendln("You take the world in as the effects of your blindness wear off.");
		}
		void disorient_clears_up(player_ptr_t victim){
			victim->remove_affect(AFF_DISORIENT);
			victim->sendln("The world around you starts to focus. You are no longer disoriented.");
		}
		void blind_target(player_ptr_t victim){
			victim->affect(AFF_BLIND);
			victim->sendln("You are blinded.");
		}
		void disorient_person(player_ptr_t victim){
			victim->affect(AFF_DISORIENT);
			victim->sendln("You become disoriented.");
		}

		int explosive_damage(player_ptr_t victim, obj_data* item){
			victim->sendln("An explosion causes you to take damage!");
			return 0;
		}
		int chemical_damage(player_ptr_t victim, obj_data* item){

			return 0;
		}
		void disorient_target(player_ptr_t player){
			player->sendln("You become extremely disoriented!");
		}
		void explode(room_rnum room_id,obj_data* object) {
			if(object->explosive()->type == mw_explosive::EXPLOSIVE_NONE){
				log("[error]: mods::projectile::explode received EXPLOSIVE_NONE");
				return;
			}
			uint16_t cooldown_ticks = 5;
			std::size_t blast_radius = 2;	/** TODO: grab from explosive()->blast_radius */
			switch(object->explosive()->type){
				default: 
				case mw_explosive::EXPLOSIVE_NONE:
					log("SYSERR: Was given an EXPLOSIVE_NONE in explode()");
					return;
				case mw_explosive::REMOTE_EXPLOSIVE:
					send_to_room(room_id,"A %s explodes!",object->name);
					break;
				case mw_explosive::REMOTE_CHEMICAL:
					send_to_room(room_id,"A %s explodes! A noxious chemical is released!",object->name);
					REMOVE_TEXTURE_QUEUE(HAZARDOUS_SMOKE,room_id);
					break;
				case mw_explosive::CLAYMORE_MINE:
					send_to_room(room_id,"You trip over a %s! An explosion catches you off guard!",object->name);
					break;
				case mw_explosive::FRAG_GRENADE:
					send_to_room(room_id,"A %s explodes!",object->name);
					break;
				case mw_explosive::INCENDIARY_GRENADE:
					send_to_room(room_id,"A %s explodes! The room turns into a fiery blaze!",object->name);
					REMOVE_TEXTURE_QUEUE(ON_FIRE,room_id);
					break;
				case mw_explosive::EMP_GRENADE:
					send_to_room(room_id,"A %s explodes!",object->name);
					REMOVE_TEXTURE_QUEUE(EMP,room_id);
					break;
				case mw_explosive::SMOKE_GRENADE:
					send_to_room(room_id,"A cloud of sight limiting gas fills the room.");
					REMOVE_TEXTURE_QUEUE(NON_HAZARDOUS_SMOKE,room_id);
					break;
				case mw_explosive::FLASHBANG_GRENADE:
					/** TODO: grab cooldown ticks from explosive() */
					send_to_room(room_id,"A bright flash of light has filled the room!");
					break;
			}
			for(auto & person : mods::globals::room_list[room_id]) {
				switch(object->explosive()->type){
					default: 
					case mw_explosive::EXPLOSIVE_NONE:
						log("SYSERR: Was given an EXPLOSIVE_NONE in explode()");
						return;
					case mw_explosive::REMOTE_EXPLOSIVE:
						mods::projectile::explosive_damage(person,object);
						break;
					case mw_explosive::REMOTE_CHEMICAL:
						mods::projectile::explosive_damage(person,object);
						mods::projectile::chemical_damage(person,object);
						break;
					case mw_explosive::CLAYMORE_MINE:
						mods::projectile::explosive_damage(person,object);
						break;
					case mw_explosive::FRAG_GRENADE:
					 	mods::projectile::grenade_damage(person, object);
						break;
					case mw_explosive::INCENDIARY_GRENADE:
						mods::projectile::grenade_damage(person, object);
						mods::projectile::fire_damage(person, object);
						break;
					case mw_explosive::EMP_GRENADE:
						mods::projectile::disable_electronics(room_id);
						break;
					case mw_explosive::SMOKE_GRENADE:
						mods::projectile::smoke_room(room_id);
						break;
					case mw_explosive::FLASHBANG_GRENADE:
						/** TODO: grab cooldown ticks from explosive() */
						mods::projectile::blind_target(person);
						mods::projectile::disorient_target(person);
						break;
				}
			}
			mods::projectile::perform_blast_radius(room_id,blast_radius,object);
			obj_from_room(object);
		}
		/**
		 * TODO: place the grenade on the floor so that some crazy bastards can potentially throw it back, 
		 * but moreso that the user can atleast see it and do something about it
		 */
		int8_t to_direction(const std::string & str){
			std::string direction = "";
			for(unsigned i = 0; i < 5 && i < str.length();i++){
				if(!isalpha(str[i])){
					return -1;
				}
				direction += std::tolower(str[i]);
			}
			if(direction.length() == 0){
				return -1;
			}
			if(direction.compare("north") == 0 || direction[0] == 'n'){
				return NORTH;
			}
			if(direction.compare("south") == 0 || direction[0] == 's'){
				return SOUTH;
			}
			if(direction.compare("east") == 0 || direction[0] == 'e'){
				return EAST;
			}
			if(direction.compare("west") == 0 || direction[0] == 'w'){
				return WEST;
			}
			if(direction.compare("up") == 0 || direction[0] == 'u'){
				return UP;
			}
			if(direction.compare("down") == 0 || direction[0] == 'd'){
				return DOWN;
			}
			return -1;
		}
		rooms_away_t calculate_shrapnel_rooms(room_rnum room, obj_data* held_object,std::size_t blast_radius){
			rooms_away_t rooms;
			for(auto & direction : {NORTH, SOUTH, EAST, WEST, UP, DOWN}){
				room_rnum r = resolve_room(room, direction,blast_radius); /** TODO: use blast_radius as dynamic var */
				if(r == room){
					rooms[direction] = std::nullopt;
					continue;
				}
				rooms[direction] = r;
			}
			return rooms;
		}
		/** FIXME: needs blast radius stuff to happen */
		/** FIXME: needs shrapnel calculation */
		std::string todirstr(const char* direction,bool prefix,bool suffix) {
			std::string pre = prefix ? " " : "";
			std::string suf = suffix ? " " : "";
			if(strncmp(direction,"north",5) == 0 || direction[0] == 'n' || direction[0] == 'N') {
				return pre + "to the north" + suf;
			}

			if(strncmp(direction,"south",5) == 0 || direction[0] == 's' || direction[0] == 'S') {
				return pre + "to the south" + suf;
			}

			if(strncmp(direction,"east",4) == 0 || direction[0] == 'e' || direction[0] == 'E') {
				return pre + "to the east" + suf;
			}

			if(strncmp(direction,"west",4) == 0 || direction[0] == 'w' || direction[0] == 'W') {
				return pre + "to the west" + suf;
			}

			if(strncmp(direction,"up",2) == 0 || direction[0] == 'u' || direction[0] == 'U') {
				return pre + "above you" + suf;
			}

			if(strncmp(direction,"down",4) == 0 || direction[0] == 'd' || direction[0] == 'D') {
				return pre + "below you" + suf;
			}
			return pre + "??" + suf;
		}

		std::string fromdirstr(int direction,bool prefix, bool suffix) {
			std::string f = std::string(prefix ? " " : "") + "from ";
			switch(direction){
				case NORTH:
					return f + "the north" + (suffix ? " " : "");
				case SOUTH:
					return f + "the south" + (suffix ? " " : "");
				case EAST:
					return f + "the east" + (suffix ? " " : "");
				case WEST:
					return f + "the west" + (suffix ? " " : "");
				case UP:
					return f + "above" + (suffix ? " " : "");
				case DOWN:
					return f + "below" + (suffix ? " " : "");
			}
			return f + "??" + (suffix ? " " : "");
		}

		void projectile_lands(room_rnum room_id,obj_data* object,int from_direction) {
			for(auto & player : mods::globals::room_list[room_id]) {
				player->stc(std::string("A ") + object->name + " tumbles to the ground" + fromdirstr(from_direction,1,0) + "! Take cover!");
			}
		}

		void projectile_flies_by(room_rnum room, obj_data* object, int from_direction, int to_direction) {
			for(auto & player : mods::globals::room_list[room]) {
				player->stc(std::string("A ") + object->name + " flies by" + fromdirstr(from_direction,1,0) + ", narrowly missing you!");
			}
		}

		void travel_to(room_rnum from, int direction, std::size_t depth, obj_data* object){
			room_rnum room_id = 0;
			room_rnum previous = 0;

			auto opposite = OPPOSITE_DIR(direction);
			for(unsigned room_depth = 1; room_depth < depth; ++room_depth) {
				room_id = resolve_room(from,direction,room_depth);

				if(previous == room_id) {
					return;
				}

				previous = room_id;
				if (room_depth + 1 == depth) {
					projectile_lands(room_id,object,opposite);
					return;
				}
				projectile_flies_by(room_id,object,opposite,direction);
			}

			return;
		}

		room_rnum cast_finite(struct char_data* source_char,room_rnum source_room,int direction,std::size_t depth,obj_data* object) {
			room_rnum room_id = 0;
			room_rnum previous = 0;

			auto opposite = OPPOSITE_DIR(direction);
			for(std::size_t room_depth = 1; room_depth <= depth +1; ++room_depth) {
				room_id = resolve_room(source_room,direction,room_depth);
				projectile_flies_by(room_id,object,opposite,direction);

				if(previous == room_id) {
					projectile_lands(room_id,object,direction);
					return room_id;
				}

				previous = room_id;
			}

			return room_id;
		}

		room_rnum resolve_room(room_rnum source_room,int direction,std::size_t depth) {
			room_rnum room_id = source_room;
			std::cerr << "resolve_room got source-rrom: " << source_room << "\n";

			auto room_dir = world[source_room].dir_option[direction];
			for(std::size_t recursive_depth =0; recursive_depth <= depth; recursive_depth++) {
				room_dir = world[room_id].dir_option[direction];
				std::cerr << "resolve-room recursive_depth: " << recursive_depth << "\n";
				if(!room_dir) {
					std::cerr << "invalid room_dir, returning: " << room_id << "\n";
					return room_id;
				}
				if((EXIT_FLAGGED(room_dir, EX_CLOSED) || EXIT_FLAGGED(room_dir,EX_REINFORCED))
						&& !(EXIT_FLAGGED(room_dir,EX_BREACHED))) {
					std::cerr << "breaking on recursive_depth: " << recursive_depth << "\n";
					break;
				}

				room_id = room_dir->to_room;

				if(room_id == NOWHERE || !room_dir) {
					std::cerr << "nowhere or !room_dir returning room_id: " << room_id << "\n";
					return room_id;
				}
				room_dir = world[room_id].dir_option[direction];
				std::cerr << "end for, looping\n";
			}

			std::cerr << "returning room_id [final]: " << room_id << "\n";
			return room_id;
		}

		/*
		 * Alert: As of bpl14, this function returns the following codes:
		 *  < 0 Victim died.
		 *  = 0 No damage.
		 *  > 0 How much damage done.
		 */
		//int grenade_damage(player_ptr_t victim, obj_data* projectile) {
		//	victim->stc(std::string("You are hit by a ") + projectile->explosive()->name);
		//	return 0;
		//}




	};
};
