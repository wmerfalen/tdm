#include "projectile.hpp"
#include "affects.hpp"
#include "../handler.h"
//#include "calc_visibility.hpp"
#include "sensor-grenade.hpp"
#include "pfind.hpp"

#include <cctype>	/* for std::tolower */
/* using directives */
using mw_explosive = mods::weapon::type::explosive;
using mw_rifle = mods::weapon::type::rifle;
using mw_drone = mods::weapon::type::drone;
using affect_t = mods::affects::affect_t;
namespace mods {
	namespace projectile {
		using affect_vector_t = mods::affects::affect_vector_t;
		using affect_t = mods::affects::affect_t;
		using texture_type_t = room_data::texture_type_t;
		/** we simply give the user the affect, and in the point update we remove that when it needs to be removed */
		template <typename T>
		void queue_affect_on_room(T affects,room_rnum room_id){
				auto people = mods::globals::get_room_list(room_id);
				if(people.size() == 0){
					return;
				}
				for(auto & player : people){
					mods::affects::affect_player({affect_t::BLIND,affect_t::DISORIENT},player);
				}
		}
#define QUEUE_TEXTURE_REMOVAL(t_texture,r_room_id) queue_remove_texture(40,r_room_id,room_data::texture_type_t::t_texture);
		void queue_remove_texture(uint64_t ticks_in_future,room_rnum& room_id,room_data::texture_type_t texture){
			/** TODO: calculate blast radius to remove smoke textures from those rooms */
				mods::globals::defer_queue->detexturize_room(
						ticks_in_future,
						room_id,
						texture
				);
		}
		void propagate_chemical_blast(room_rnum& room_id,obj_ptr_t device){
			QUEUE_TEXTURE_REMOVAL(HAZARDOUS_SMOKE,room_id);
		}
		void propagate_chemical_blast(room_rnum& room_id,obj_ptr_t device,std::size_t depth){
			QUEUE_TEXTURE_REMOVAL(HAZARDOUS_SMOKE,room_id);
			for(auto &player : mods::globals::get_room_list(room_id)){
				player->sendln("Propagating chemblast " + std::to_string((1.0) * (depth / 3.0)));
			}
		}
		void emp_damage(room_rnum& room_id,obj_ptr_t object){
			send_to_room(room_id,"Your electronics sizzle and malfunction into uselessness.");
			QUEUE_TEXTURE_REMOVAL(EMP,room_id);
		}
		/*
		void damage_room(room_rnum& room_id,obj_ptr_t object){
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
			for(auto & person : mods::globals::get_room_list(room_id)){
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
		void perform_blast_radius(
				room_rnum room_id,
				std::size_t blast_radius,
				obj_ptr_t device,
				uuid_t player_uuid) {
			log("perform blast radius entry");
			auto current_room = room_id;
			auto type = device->explosive()->type;
			for(auto & dir : world[room_id].directions()){
				current_room = room_id;
				log("direction: (%d) roomid(%d)",dir,room_id);
				for(std::size_t blast_count = 0; blast_count < blast_radius;blast_count++){
					auto dir_option = world[current_room].dir_option[dir];
					current_room = dir_option->to_room;
					float damage_multiplier = 0;
					auto opposite = OPPOSITE_DIR(dir);

					/**
					 * It's okay to not have a defaulted case since the
					 * following switch deals only with room textures
					 */
					switch(type){
						default: break;
						case mw_explosive::EXPLOSIVE_NONE:
							log("SYSERR: EXPLOSIVE_NONE specified in perform_blast_radius");
							return;
						case mw_explosive::REMOTE_CHEMICAL:
							QUEUE_TEXTURE_REMOVAL(HAZARDOUS_SMOKE,current_room);
							break;
						case mw_explosive::INCENDIARY_GRENADE:
							QUEUE_TEXTURE_REMOVAL(ON_FIRE,current_room);
							break;
						case mw_explosive::EMP_GRENADE:
							QUEUE_TEXTURE_REMOVAL(EMP,current_room);
							break;
						case mw_explosive::SMOKE_GRENADE:
							QUEUE_TEXTURE_REMOVAL(NON_HAZARDOUS_SMOKE,current_room);
							break;
					}
					for(auto & person : mods::globals::get_room_list(current_room)){
						switch(type){
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
			obj_from_room(device);
		}
		int grenade_damage(player_ptr_t victim,obj_ptr_t projectile){
			mods::projectile::explosive_damage(victim,projectile);
			return 0;
		}
		int fire_damage(player_ptr_t victim,obj_ptr_t projectile){
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

		int explosive_damage(player_ptr_t victim, obj_ptr_t item){
			victim->sendln("An explosion causes you to take damage!");
			return 0;
		}
		int chemical_damage(player_ptr_t victim, obj_ptr_t item){

			return 0;
		}
		void disorient_target(player_ptr_t player){
			player->sendln("You become extremely disoriented!");
		}
		void explode(room_rnum room_id,uuid_t object_uuid,uuid_t player_uuid){
			if(room_id >= world.size()){
				log("[error]: mods::projectile::explode received room_id greater than world.size()");
				return;
			}
			log("explode, calling optr");
			auto opt_object = optr_opt(object_uuid);
			if(!opt_object.has_value()){
				log("[error]: mods::projectile::explode received invalid object to blow up");
				return;
			}
			log("opt_object has a value");
			auto object = std::move(opt_object.value());
			if(object->has_explosive() == false || object->explosive()->attributes == nullptr ||
					object->explosive()->type == mw_explosive::EXPLOSIVE_NONE){
				log("[error]: mods::projectile::explode. explosive integrity check failed");
				return;
			}
			auto type = object->explosive()->type;
			std::size_t blast_radius = object->explosive()->attributes->blast_radius;	/** TODO: grab from explosive()->blast_radius */
			log("grabbed blast radius... (%d)",blast_radius);
			switch(type){
				default: 
					log("SYSERR: Invalid explosive type(%d) in %s:%d",type,__FILE__,__LINE__);
					return;
				case mw_explosive::REMOTE_EXPLOSIVE:
					send_to_room(room_id,"A %s explodes!\r\n",object->name.c_str());
					break;
				case mw_explosive::REMOTE_CHEMICAL:
					send_to_room(room_id,"A %s explodes! A noxious chemical is released!\r\n",object->name.c_str());
					QUEUE_TEXTURE_REMOVAL(HAZARDOUS_SMOKE,room_id);
					break;
				case mw_explosive::CLAYMORE_MINE:
					send_to_room(room_id,"You trip over a %s! An explosion catches you off guard!\r\n",object->name.c_str());
					break;
				case mw_explosive::FRAG_GRENADE:
					send_to_room(room_id,"A %s explodes!\r\n",object->name.c_str());
					break;
				case mw_explosive::INCENDIARY_GRENADE:
					send_to_room(room_id,"A %s explodes! The room turns into a fiery blaze!\r\n",object->name.c_str());
					QUEUE_TEXTURE_REMOVAL(ON_FIRE,room_id);
					break;
				case mw_explosive::EMP_GRENADE:
					send_to_room(room_id,"A %s explodes!\r\n",object->name.c_str());
					QUEUE_TEXTURE_REMOVAL(EMP,room_id);
					break;
				case mw_explosive::SMOKE_GRENADE:
					send_to_room(room_id,"A cloud of sight limiting gas fills the room.\r\n");
					QUEUE_TEXTURE_REMOVAL(NON_HAZARDOUS_SMOKE,room_id);
					break;
				case mw_explosive::FLASHBANG_GRENADE:
					send_to_room(room_id,"Your senses become scattered as a bright flash of light fills the room!\r\n");
					break;
				case mw_explosive::SENSOR_GRENADE:
					send_to_room(room_id,"A %s scans the room.\r\n", object->name.c_str());
					break;
			}

			if(type == mw_explosive::SENSOR_GRENADE){
				mods::sensor_grenade::handle_explosion(object_uuid,player_uuid,room_id,object->from_direction);
				return;
			}

			for(auto & person : mods::globals::get_room_list(room_id)) {
				switch(type){
					default: 
						log("SYSERR: Invalid explosive type(%d) in %s:%d",type,__FILE__,__LINE__);
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
						break;
				}
				log("gt_room_list(%d) for looping through (%d) tenants", room_id, mods::globals::get_room_list(room_id).size());
			}
			if(type == mw_explosive::FLASHBANG_GRENADE){
				queue_on_room( {AFF(BLIND),AFF(DISORIENT)}, room_id);
			}
			if(type == mw_explosive::REMOTE_CHEMICAL){
				queue_on_room( {AFF(DISORIENT),AFF(POISON)}, room_id);
			}
			log("calling perform blast radius");
			mods::projectile::perform_blast_radius(room_id,blast_radius,object,player_uuid);
		}
		/**
		 * TODO: place the grenade on the floor so that some crazy bastards can potentially throw it back, 
		 * but moreso that the user can atleast see it and do something about it
		 */
		int to_direction(const std::string & str){
			return mods::util::to_direction<const std::string&>(str);
		}
		rooms_away_t calculate_shrapnel_rooms(room_rnum room, obj_ptr_t held_object,std::size_t blast_radius){
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

		void projectile_lands(room_rnum room_id,obj_ptr_t object,int from_direction) {
			
			//for(auto & player : mods::globals::get_room_list(room_id)) {
			//	player->stc(std::string("A ") + object->name + " tumbles to the ground" + fromdirstr(from_direction,1,0) + "! Take cover!");
			//}
		}

		int travel_to(room_rnum from, int direction, std::size_t depth, std::shared_ptr<obj_data> object){
			room_rnum room_id = resolve_room(from,direction,depth);
			obj_to_room(object,room_id);
			object->from_direction = OPPOSITE_DIR(direction);
			return room_id;
		}

		room_rnum resolve_room(room_rnum source_room,int direction,std::size_t depth) {
			room_rnum room_id = source_room;

			auto room_dir = world[source_room].dir_option[direction];
			for(std::size_t recursive_depth =0; recursive_depth <= depth; recursive_depth++) {
				room_dir = world[room_id].dir_option[direction];
				if(!room_dir) {
					return room_id;
				}
				if((EXIT_FLAGGED(room_dir, EX_CLOSED) || EXIT_FLAGGED(room_dir,EX_REINFORCED))
						&& !(EXIT_FLAGGED(room_dir,EX_BREACHED))) {
					break;
				}

				auto previous_room_id = room_id;
				room_id = room_dir->to_room;

				if(room_id == NOWHERE){
					return previous_room_id;
				}
				room_dir = world[room_id].dir_option[direction];
			}

			return room_id;
		}

		/*
		 * Alert: As of bpl14, this function returns the following codes:
		 *  < 0 Victim died.
		 *  = 0 No damage.
		 *  > 0 How much damage done.
		 */
		//int grenade_damage(player_ptr_t victim, obj_ptr_t projectile) {
			//	victim->stc(std::string("You are hit by a ") + projectile->explosive()->name);
			//	return 0;
		//}

		void explode_in_future(int room_id, int ticks, uuid_t object_uuid,uuid_t player_uuid) {
			d("explode_in_future uuid: " << object_uuid);
			mods::globals::defer_queue->push(ticks, [room_id,object_uuid,player_uuid]() {
				d("[defer_queue] callback:: explode_in_future uuid: " << object_uuid << " room_id:" << room_id);
				explode(room_id,object_uuid,player_uuid);
			});
		}

		void throw_object(player_ptr_t player, int direction, std::size_t depth,
				std::shared_ptr<obj_data> object, std::string_view verb) {
			std::array<char,MAX_INPUT_LENGTH> str_dir_buffer;
			std::string str_dir = mods::projectile::todirstr(static_cast<const char*>(&str_dir_buffer[0]),1,0);
			std::string object_name = "";
			int ticks = 0;
			assert(object->has_explosive());
			mw_explosive type = object->explosive()->type;
			switch((mw_explosive)type) {
				default:
				case mw_explosive::REMOTE_EXPLOSIVE:
				case mw_explosive::REMOTE_CHEMICAL:
				case mw_explosive::CLAYMORE_MINE:
				case mw_explosive::EXPLOSIVE_NONE:
					player->sendln("This type of explosive is not throwable!");
					return;
					break;
				case mw_explosive::FRAG_GRENADE:
					ticks = 2;
					object_name = "frag grenade";
					break;
				case mw_explosive::INCENDIARY_GRENADE:
					object_name = "incendiary grenade";
					ticks = 2;
					break;
				case mw_explosive::EMP_GRENADE:
					object_name = "emp grenade";
					ticks = 2;
					break;
				case mw_explosive::SMOKE_GRENADE:
					object_name = "smoke grenade";
					ticks = 3;
					break;
				case mw_explosive::FLASHBANG_GRENADE:
					object_name = "flashbang grenade";
					ticks = 6;
					break;
				case mw_explosive::SENSOR_GRENADE:
					object_name = "sensor grenade";
					ticks = 6;
					break;
			}
			send_to_room_except(player->room(), player, "%s %ss a %s%s!\r\n",
					player->ucname().c_str(), 
					verb.data(),
					object_name.c_str(),
					str_dir.c_str());
			player->sendln("You " + std::string(verb.data()) + " a " + object->explosive()->name + str_dir);
			obj_from_char(object);
			auto room_id = travel_to(player->room(), direction, depth, object);
			explode_in_future(room_id, ticks, object->uuid,player->uuid());
		}
	};
};
