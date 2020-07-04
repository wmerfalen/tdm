#include "projectile.hpp"
#include "affects.hpp"
#include "../handler.h"
//#include "calc_visibility.hpp"
#include "sensor-grenade.hpp"
#include "pfind.hpp"

#include <cctype>	/* for std::tolower */
#include "flashbang.hpp"
#include "rooms.hpp"
#ifdef __MENTOC_SHOW_MODS_EXPLODE_DEBUG_OUTPUT__
#define explode_debug(MSG) mentoc_prefix_debug("[mods::projectile::explode]")  << MSG << "\n";
#else
#define explode_debug(MSG) ;;
#endif

#ifdef __MENTOC_SHOW_MODS_PERFORM_BLAST_RADIUS_DEBUG_OUTPUT__
#define pbr_debug(MSG) mentoc_prefix_debug("[mods::projectile::perform_blast_radius]")  << MSG << "\n";
#else
#define pbr_debug(MSG) ;;
#endif


#ifdef __MENTOC_SHOW_MODS_EXPLODE_IN_FUTURE_DEBUG_OUTPUT__
#define eif_debug(MSG) mentoc_prefix_debug("[mods::projectile::explode_in_future]")  << MSG << "\n";
#else
#define eif_debug(MSG) ;;
#endif


/* using directives */
using mw_explosive = mods::weapon::type::explosive;
using mw_rifle = mods::weapon::type::rifle;
using mw_drone = mods::weapon::type::drone;
using affect_t = mods::affects::affect_t;
namespace mods::injure {
	extern bool do_injure_roll(uint8_t chance);
	extern void injure_player(player_ptr_t& person);
	namespace explosive {
		extern void handle_crit_injure(obj_ptr_t&,player_ptr_t&);
	};
};
namespace mods {
	namespace projectile {
		using affect_vector_t = mods::affects::affect_vector_t;
		using affect_t = mods::affects::affect_t;
		using texture_type_t = room_data::texture_type_t;
		constexpr static uint8_t CI_CRIT = 0;
		constexpr static uint8_t CI_INJ = 0;
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
			/** TODO: fill this function */
			QUEUE_TEXTURE_REMOVAL(HAZARDOUS_SMOKE,room_id);
		}
		void propagate_chemical_blast(room_rnum& room_id,obj_ptr_t device,std::size_t depth){
			/** TODO: fill this function */
			QUEUE_TEXTURE_REMOVAL(HAZARDOUS_SMOKE,room_id);
			for(auto &player : mods::globals::get_room_list(room_id)){
				player->sendln("Propagating chemblast " + std::to_string((1.0) * (depth / 3.0)));
			}
		}
		void emp_damage(room_rnum& room_id,obj_ptr_t object){
			if(mods::rooms::is_peaceful(room_id)){
				return;
			}
			/** TODO: fill this function */
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
			if(mods::rooms::is_peaceful(room_id)){
				return;
			}
			pbr_debug("perform blast radius entry");
			auto current_room = room_id;
			auto type = device->explosive()->type;
			for(auto & dir : world[room_id].directions()){
				current_room = room_id;
				pbr_debug("direction: (" << dirstr(dir) << ") roomid(" << room_id << ")");
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
										 mods::rooms::start_fire_dissolver(current_room);
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
											 damage_multiplier = (1.0 * blast_count) / DAMAGE_DIVISOR;
											 person->sendln("[damage: " + std::to_string(damage_multiplier) + "]");
											 /** TODO: deal explosive damage here */
											 break;
							case mw_explosive::INCENDIARY_GRENADE:
											 person->sendln("A heated explosion sets the room on fire" + mods::projectile::fromdirstr(opposite,1,0) + "!");
											 damage_multiplier = (1.0 * blast_count) / DAMAGE_DIVISOR;
											 person->sendln("[damage: " + std::to_string(damage_multiplier) + "]");
											 /** TODO: deal fire damage here */
											 break;
							case mw_explosive::EMP_GRENADE:
											 person->sendln("The effectiveness of your electronics is hindered" + mods::projectile::fromdirstr(opposite,1,0) + "!");
											 damage_multiplier = (1.0 * blast_count) / DAMAGE_DIVISOR;
											 person->sendln("[electronics " + std::to_string(damage_multiplier) + "]");
											 /** TODO: affect room with emp here */
											 break;
							case mw_explosive::SMOKE_GRENADE:
											 person->sendln("A cloud of smoke billows in" + mods::projectile::fromdirstr(opposite,1,0) + "!");
											 /** TODO: add smoke texture to room */
											 break;
							case mw_explosive::FLASHBANG_GRENADE:
											 person->sendln("You are partially blinded by flash of light" + mods::projectile::fromdirstr(opposite,1,0) + "!");
											 /** TODO: add disorient to player here */
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
			/** TODO: add fire texture to room */
			mods::injure::explosive::handle_crit_injure(projectile,victim);
			victim->sendln("The room and part of your equipment catch on fire!");
			return 0;
		}
		void disable_electronics(room_rnum room){
			/** TODO: disable electronics */

		}
		void smoke_room(room_rnum room){
			/** TODO: cause limited visibility and smoke texture */
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
			/** TODO: cause explosive damage */
			/** TODO: check if critical, if so, cause critical damage */
			/** TODO: check if injured, if so, injure player */
			victim->sendln("An explosion causes you to take damage!");
			return 0;
		}
		int chemical_damage(player_ptr_t victim, obj_ptr_t item){
			/** TODO: cause chemical damage */
			/** TODO: check if critical, if so, cause critical damage */
			/** TODO: check if injured, if so, injure player */
			return 0;
		}
		void disorient_target(player_ptr_t player){
			/** TODO: cause disorientation affect */
			player->sendln("You become extremely disoriented!");
		}
		void explode(room_rnum room_id,uuid_t object_uuid,uuid_t player_uuid){
			if(room_id >= world.size()){
				log("[error]: mods::projectile::explode received room_id greater than world.size()");
				return;
			}
			if(mods::rooms::is_peaceful(room_id)){
				return;
			}
			explode_debug("explode, calling optr");
			auto opt_object = optr_opt(object_uuid);
			if(!opt_object.has_value()){
				log("[error]: mods::projectile::explode received invalid object to blow up");
				return;
			}
			explode_debug("opt_object has a value");
			auto object = std::move(opt_object.value());
			if(object->has_explosive() == false || object->explosive()->attributes == nullptr ||
					object->explosive()->type == mw_explosive::EXPLOSIVE_NONE){
				log("[error]: mods::projectile::explode. explosive integrity check failed");
				return;
			}
			auto type = object->explosive()->type;
			std::size_t blast_radius = object->explosive()->attributes->blast_radius;	/** TODO: grab from explosive()->blast_radius */
			explode_debug("grabbed blast radius... (" << blast_radius << ")");
			bool does_damage = false;

			switch(type){
				default: 
					log("SYSERR: Invalid explosive type(%d) in %s:%d",type,__FILE__,__LINE__);
					return;
				case mw_explosive::REMOTE_EXPLOSIVE:
					does_damage = true;
					send_to_room(room_id,"A %s explodes!\r\n",object->name.c_str());
					break;
				case mw_explosive::REMOTE_CHEMICAL:
					does_damage = true;
					send_to_room(room_id,"A %s explodes! A noxious chemical is released!\r\n",object->name.c_str());
					QUEUE_TEXTURE_REMOVAL(HAZARDOUS_SMOKE,room_id);
					break;
				case mw_explosive::CLAYMORE_MINE:
					does_damage = true;
					send_to_room(room_id,"You trip over a %s! An explosion catches you off guard!\r\n",object->name.c_str());
					break;
				case mw_explosive::FRAG_GRENADE:
					does_damage = true;
					send_to_room(room_id,"A %s explodes!\r\n",object->name.c_str());
					break;
				case mw_explosive::INCENDIARY_GRENADE:
					does_damage = true;
					send_to_room(room_id,"A %s explodes! The room turns into a fiery blaze!\r\n",object->name.c_str());
					mods::rooms::start_fire_dissolver(room_id);
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
				mods::globals::dispose_object(object_uuid);
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
						mods::flashbang::affect_room(room_id,object);
						break;
				}
				if(does_damage){
					mods::injure::explosive::handle_crit_injure(object,person);
				}
				explode_debug("gt_room_list(" << room_id << ") for looping through (" <<
						mods::globals::get_room_list(room_id).size() << ") tenants"
						);
			}
			if(type == mw_explosive::FLASHBANG_GRENADE){
				queue_on_room( {AFF(BLIND),AFF(DISORIENT)}, room_id);
			}
			if(type == mw_explosive::REMOTE_CHEMICAL){
				queue_on_room( {AFF(DISORIENT),AFF(POISON)}, room_id);
			}
			explode_debug("calling perform blast radius");
			mods::projectile::perform_blast_radius(room_id,blast_radius,object,player_uuid);
			mods::globals::dispose_object(object_uuid);
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
		/**
		 * @brief will give you " to the DIRECTION " (i.e.: " to the north ", " above you ")
		 *
		 * @param direction
		 * @param prefix
		 * @param suffix
		 *
		 * @return 
		 */
		std::string todirstr(int direction,bool prefix,bool suffix) {
			std::string pre = prefix ? " " : "";
			std::string suf = suffix ? " " : "";
			switch(direction){
				case NORTH:
					return pre + "to the north" + suf;
				case SOUTH:
					return pre + "to the south" + suf;
				case EAST:
					return pre + "to the east" + suf;
				case WEST:
					return pre + "to the west" + suf;
				case UP:
					return pre + "above you" + suf;
				case DOWN:
					return pre + "below you" + suf;
				default: return "<?>";
			}
		}

		/**
		 * @brief get a string that results in " from the DIRECTION " (i.e.: " from the north ", " from above ")
		 *
		 * @param direction
		 * @param prefix
		 * @param suffix
		 *
		 * @return 
		 */
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

		/**
		 * @brief crawls depth rooms from 'from' into direction and gives you the room id
		 *
		 * @param from
		 * @param direction
		 * @param depth
		 * @param object
		 *
		 * @return 
		 */
		int travel_to(room_rnum from, int direction, std::size_t depth, std::shared_ptr<obj_data> object){
			room_rnum room_id = resolve_room(from,direction,depth);
			obj_to_room(object,room_id);
			object->from_direction = OPPOSITE_DIR(direction);
			return room_id;
		}

		/**
		 * @brief crawls up to depth rooms in direction from source_room
		 *
		 * @param source_room
		 * @param direction
		 * @param depth
		 *
		 * @return 
		 */
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

		/**
		 * @brief explodes the obj uuid in room_id ticks in the future. wrapper to explode(rnum,uuid obj,uuid plr)
		 *
		 * @param room_id
		 * @param ticks
		 * @param object_uuid
		 * @param player_uuid
		 */
		void explode_in_future(int room_id, int ticks, uuid_t object_uuid,uuid_t player_uuid) {
			eif_debug("explode_in_future uuid: " << object_uuid);
			mods::globals::defer_queue->push(ticks, [room_id,object_uuid,player_uuid]() {
					eif_debug("[defer_queue] callback:: explode_in_future uuid: " << object_uuid << " room_id:" << room_id);
					explode(room_id,object_uuid,player_uuid);
					});
		}

		/**
		 * @brief Throw an object (grenade)
		 *
		 * @param player
		 * @param direction
		 * @param depth
		 * @param object
		 * @param verb
		 */
		void throw_object(player_ptr_t& player, int direction, std::size_t depth,
				std::shared_ptr<obj_data>& object, std::string_view verb) {

			{
				room_rnum room_id = resolve_room(player->room(),direction,depth);
				if(mods::rooms::is_peaceful(room_id)){
					player->sendln("Target room is in the D.M.Z.. Your item has been destroyed.");
					mods::globals::dispose_object(object->uuid);
					return;
				}
			}

			std::array<char,32> str_dir_buffer;
			std::fill(str_dir_buffer.begin(),str_dir_buffer.end(),0);
			std::string str_dir = mods::projectile::todirstr(direction,0,0);
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
					ticks = FRAG_GRENADE_TICKS;
					break;
				case mw_explosive::INCENDIARY_GRENADE:
					ticks = INCENDIARY_GRENADE_TICKS;
					break;
				case mw_explosive::EMP_GRENADE:
					ticks = EMP_GRENADE_TICKS;
					break;
				case mw_explosive::SMOKE_GRENADE:
					ticks = SMOKE_GRENADE_TICKS;
					break;
				case mw_explosive::FLASHBANG_GRENADE:
					ticks = FLASHBANG_GRENADE_TICKS;
					break;
				case mw_explosive::SENSOR_GRENADE:
					ticks = SENSOR_GRENADE_TICKS;
					break;
			}
			send_to_room_except(player->room(), player, "%s %ss a %s%s!\r\n",
					player->ucname().c_str(), 
					verb.data(),
					object->short_description.c_str(),
					str_dir.c_str());
			player->send("You %s a %s %s!\r\n", verb.data(), object->name.c_str(),str_dir.c_str());
			player->unequip(WEAR_HOLD);
			auto room_id = travel_to(player->room(), direction, depth, object);
			explode_in_future(room_id, ticks, object->uuid,player->uuid());
		}
	};
};
