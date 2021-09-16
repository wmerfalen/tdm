#include "weapons/damage-calculator.hpp"
#include "projectile.hpp"
#include "affects.hpp"
#include "../handler.h"
//#include "calc_visibility.hpp"
#include "sensor-grenade.hpp"
#include "pfind.hpp"
#include "skills.hpp"

#include <cctype>	/* for std::tolower */
#include "flashbang.hpp"
#include "rooms.hpp"
#include "injure.hpp"
#include "explosive.hpp"
#include "doors.hpp"
#include "super-users.hpp"

#include "interpreter.hpp"
#include "classes/ghost.hpp"

#define __MENTOC_SHOW_MODS_EXPLODE_DEBUG_OUTPUT__
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


#define __MENTOC_SHOW_MODS_EXPLODE_IN_FUTURE_DEBUG_OUTPUT__
#ifdef __MENTOC_SHOW_MODS_EXPLODE_IN_FUTURE_DEBUG_OUTPUT__
#define eif_debug(MSG) mentoc_prefix_debug("[mods::projectile::explode_in_future]")  << MSG << "\n";
#else
#define eif_debug(MSG) ;;
#endif

#define m_error(MSG) mentoc_prefix_debug(red_str("[mods::projectile::ERROR]"))  << MSG << "\n";

namespace mods::weapons::damage_types {
	extern void deal_hp_damage(player_ptr_t& player, uint16_t damage);
};

extern void	send_to_room(room_rnum room, const char *messg, ...) __attribute__((format(printf, 2, 3)));
extern void send_to_room_except(room_rnum room, std::vector<uuid_t> except, const char *messg, ...);
extern void send_to_room_except(room_rnum room, const std::vector<char_data*>& except, const char *messg, ...);
extern void send_to_room_except(room_rnum room, player_ptr_t except_me, const char *messg, ...);
extern void send_to_room(room_rnum room, const char *messg, ...);
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
namespace mods::weapons {
	int corrosive_claymore_explode(player_ptr_t&,obj_ptr_t&);
	void corrosive_claymore_perform_blast_radius(room_rnum,obj_ptr_t);
	void shrapnel_claymore_perform_blast_radius(room_rnum,obj_ptr_t);
	int shrapnel_claymore_explode(player_ptr_t&,obj_ptr_t&);
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
		void queue_affect_on_room(T affects,room_rnum room_id) {
			auto people = mods::globals::get_room_list(room_id);
			if(people.size() == 0) {
				return;
			}
			for(auto& player : people) {
				mods::affects::affect_player({affect_t::BLIND,affect_t::DISORIENT},player);
			}
		}
#define QUEUE_TEXTURE_REMOVAL(t_texture,r_room_id) queue_remove_texture(40,r_room_id,room_data::texture_type_t::t_texture);
		void queue_remove_texture(uint64_t ticks_in_future,room_rnum& room_id,room_data::texture_type_t texture) {
			/** TODO: calculate blast radius to remove smoke textures from those rooms */
			mods::globals::defer_queue->detexturize_room(
			    ticks_in_future,
			    room_id,
			    texture
			);
		}
		void propagate_chemical_blast(room_rnum& room_id,obj_ptr_t device) {
			/** TODO: fill this function */
			QUEUE_TEXTURE_REMOVAL(HAZARDOUS_SMOKE,room_id);
		}
		void propagate_chemical_blast(room_rnum& room_id,obj_ptr_t device,std::size_t depth) {
			/** TODO: fill this function */
			QUEUE_TEXTURE_REMOVAL(HAZARDOUS_SMOKE,room_id);
			for(auto& player : mods::globals::get_room_list(room_id)) {
				player->sendln("Propagating chemblast " + std::to_string((1.0) * (depth / 3.0)));
			}
		}
		void emp_damage(room_rnum& room_id,obj_ptr_t object) {
			if(mods::rooms::is_peaceful(room_id)) {
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
		int grenade_damage(player_ptr_t victim,obj_ptr_t projectile) {
			if(mods::super_users::player_is(victim)) {
				return 0;
			}
			return explosive_damage(victim,projectile);
		}
		int fire_damage(player_ptr_t victim,obj_ptr_t projectile) {
			if(mods::super_users::player_is(victim)) {
				return 0;
			}
			victim->sendln("The room catches fire!");
			return explosive_damage(victim,projectile);
		}
		void disable_electronics(room_rnum room) {
			/** TODO: disable electronics */
		}
		void smoke_room(room_rnum room) {
			/** TODO: cause limited visibility and smoke texture */
		}
		void blindness_clears_up(player_ptr_t victim) {
			if(mods::super_users::player_is(victim)) {
				return;
			}
			victim->remove_affect(AFF_BLIND);
			victim->sendln("You take the world in as the effects of your blindness wear off.");
		}
		void disorient_clears_up(player_ptr_t victim) {
			if(mods::super_users::player_is(victim)) {
				return;
			}
			victim->remove_affect(AFF_DISORIENT);
			victim->sendln("The world around you starts to focus. You are no longer disoriented.");
		}
		void blind_target(player_ptr_t victim) {
			if(mods::super_users::player_is(victim)) {
				return;
			}
			victim->affect(AFF_BLIND);
			victim->sendln("You are blinded.");
		}
		void disorient_person(player_ptr_t victim) {
			if(mods::super_users::player_is(victim)) {
				return;
			}
			victim->affect(AFF_DISORIENT);
			victim->sendln("You become disoriented.");
		}

		void perform_blast_radius(
		    room_rnum room_id,
		    std::size_t blast_radius,
		    obj_ptr_t device,
		    uuid_t player_uuid) {
			if(mods::rooms::is_peaceful(room_id)) {
				return;
			}
			if(mods::object_utils::is_corrosive_claymore(device)) {
				mods::weapons::corrosive_claymore_perform_blast_radius(room_id,device);
				return;
			}
			if(mods::object_utils::is_shrapnel_claymore(device)) {
				mods::weapons::shrapnel_claymore_perform_blast_radius(room_id,device);
				return;
			}
			pbr_debug("perform blast radius entry");
			auto current_room = room_id;
			auto type = device->explosive()->type;
			for(auto& dir : world[room_id].directions()) {
				current_room = room_id;
				pbr_debug("direction: (" << dirstr(dir) << ") roomid(" << room_id << ")");
				for(std::size_t blast_count = 0; blast_count < blast_radius; blast_count++) {
					auto dir_option = world[current_room].dir_option[dir];
					if(!dir_option) {
						break;
					}
					current_room = dir_option->to_room;
					float damage_multiplier = 0;
					auto opposite = OPPOSITE_DIR(dir);

					/**
					 * It's okay to not have a defaulted case since the
					 * following switch deals only with room textures
					 */
					switch(type) {
						default:
							break;
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
							disable_electronics(current_room);
							QUEUE_TEXTURE_REMOVAL(EMP,current_room);
							break;
						case mw_explosive::SMOKE_GRENADE:
							QUEUE_TEXTURE_REMOVAL(NON_HAZARDOUS_SMOKE,current_room);
							break;
					}
					for(auto& person : mods::globals::get_room_list(current_room)) {
						if(mods::super_users::player_is(person)) {
							continue;
						}
						switch(type) {
							case mw_explosive::REMOTE_CHEMICAL:
								mods::projectile::propagate_chemical_blast(current_room,device,blast_count);
								break;
							default:
								m_error("type of explosive is invalid value!");
								return;	/** THis should _NEVER_ happen */
							case mw_explosive::CLAYMORE_MINE:
							case mw_explosive::REMOTE_EXPLOSIVE:
							case mw_explosive::FRAG_GRENADE:
								//if(mods::object_utils::is_corrosive_claymore(device)) {
								//	person->sendln("{grn}Corrosive shrapnel{/grn} tears through you" + mods::projectile::fromdirstr(opposite,1,0) + "!");
								//	damage_multiplier = (1.0 * blast_count) / CORROSIVE_DAMAGE_DIVISOR();
								//	person->sendln("[damage: " + std::to_string(damage_multiplier) + "]");
								//	mods::weapons::corrosive_claymore_explode(person,device);
								//} else if(mods::object_utils::is_shrapnel_claymore(device)) {
								//	person->sendln("{yel}Enhanced shrapnel{/yel} tears through you" + mods::projectile::fromdirstr(opposite,1,0) + "!");
								//	damage_multiplier = (1.0 * blast_count) / SHRAPNEL_DAMAGE_DIVISOR();
								//	person->sendln("[damage: " + std::to_string(damage_multiplier) + "]");
								//	mods::weapons::shrapnel_claymore_explode(person,device);
								//} else {
								person->sendln("Shrapnel tears through you" + mods::projectile::fromdirstr(opposite,1,0) + "!");
								damage_multiplier = (1.0 * blast_count) / DAMAGE_DIVISOR();
								person->sendln("[damage: " + std::to_string(damage_multiplier) + "]");
								explosive_damage(person, device);
								//}
								break;
							case mw_explosive::INCENDIARY_GRENADE:
								person->sendln("A heated explosion sets the room on fire" + mods::projectile::fromdirstr(opposite,1,0) + "!");
								damage_multiplier = (1.0 * blast_count) / DAMAGE_DIVISOR();
								person->sendln("[damage: " + std::to_string(damage_multiplier) + "]");
								fire_damage(person, device);
								break;
							case mw_explosive::EMP_GRENADE:
								person->sendln("The effectiveness of your electronics is hindered" + mods::projectile::fromdirstr(opposite,1,0) + "!");
								damage_multiplier = (1.0 * blast_count) / DAMAGE_DIVISOR();
								person->sendln("[electronics " + std::to_string(damage_multiplier) + "]");
								/** TODO */
								break;
							case mw_explosive::SMOKE_GRENADE:
								person->sendln("A cloud of smoke billows in" + mods::projectile::fromdirstr(opposite,1,0) + "!");
								/** TODO */
								break;
							case mw_explosive::FLASHBANG_GRENADE:
								blind_target(person);
								disorient_person(person);
								break;
						}
					}
				}
			}
			//obj_from_room(device);
		}
		int explosive_damage(player_ptr_t victim, obj_ptr_t item) {
			if(mods::super_users::player_is(victim)) {
				return 0;
			}
			if(mods::rooms::is_peaceful(victim->room())) {
				return 0;
			}
			auto& attr = item->explosive()->attributes;
			if(attr->alternate_explosion_type.compare("SCAN") == 0) {
				return 0;
			}
			auto e = mods::weapons::damage_calculator::calculate_explosive_damage(victim,item);
			if(e.injured) {
				mods::injure::injure_player(victim);
				msg::youre_injured(victim);
			}
			/** TODO handle critical range attribute */
			/** TODO handle blast radius attribute */
			/** TODO handle loudness type */
			int damage = e.damage;

			if(e.critical) {
				damage += e.critical;
				victim->sendln("{red}*** [CRITICAL] ***{/red} -- ");
			}

			if(e.chemical) {
				damage += e.chemical;
				victim->sendln("A chemical weapons explosion causes you to take damage!");
			}
			if(e.fire) {
				damage += e.fire;
				victim->sendln("An incendiary explosion causes you to take damage!");
				mods::explosive::append_incendiary_damage_to_room(victim->room(),e.fire);
			}
			if(e.radiation) {
				damage += e.radiation;
				victim->sendln("A radiation explosion causes you to take damage!");
			}
			if(e.electric) {
				damage += e.electric;
				victim->sendln("An electric explosion causes you to take damage!");
			}
			if(e.armor_pen) {
				damage += e.armor_pen;
				victim->sendln("The explosion shreds through your armor!");
			}

			mods::weapons::damage_types::deal_hp_damage(victim,damage);
			mods::explosive::append_explosive_damage_to_room(victim->room(),damage);
			return damage;
		}
		int chemical_damage(player_ptr_t victim, obj_ptr_t item) {
			if(mods::super_users::player_is(victim)) {
				return 0;
			}
			return explosive_damage(victim,item);
		}

		void disorient_target(player_ptr_t player) {
			if(mods::super_users::player_is(player)) {
				return;
			}
			disorient_person(player);
			player->sendln("You become extremely disoriented!");
		}
		int8_t explode(room_rnum room_id,uuid_t object_uuid,uuid_t player_uuid, bool dispose) {
			if(room_id >= world.size()) {
				log("[error]: mods::projectile::explode received room_id greater than world.size()");
				return -1;
			}
			if(mods::rooms::is_peaceful(room_id)) {
				auto device = optr_by_uuid(object_uuid);
				return -2;
			}
			explode_debug("explode, calling optr");
			auto opt_object = optr_opt(object_uuid);
			if(!opt_object.has_value()) {
				log("[error]: mods::projectile::explode received invalid object to blow up");
				return -3;
			}
			explode_debug("opt_object has a value");
			auto object = std::move(opt_object.value());
			if(object->has_explosive() == false || object->explosive()->attributes == nullptr ||
			        object->explosive()->type == mw_explosive::EXPLOSIVE_NONE) {
				log("[error]: mods::projectile::explode. explosive integrity check failed");
				return -4;
			}
			auto type = object->explosive()->type;
			std::size_t blast_radius = object->explosive()->attributes->blast_radius;	/** TODO: grab from explosive()->blast_radius */
			explode_debug("grabbed blast radius... (" << blast_radius << ")");
			bool does_damage = false;
			auto victim = ptr_by_uuid(player_uuid);
			if(mods::object_utils::is_corrosive_claymore(object)) {
				return mods::weapons::corrosive_claymore_explode(victim,object);
			}
			if(mods::object_utils::is_shrapnel_claymore(object)) {
				return mods::weapons::shrapnel_claymore_explode(victim,object);
			}

			switch(type) {
				default:
					log("SYSERR: Invalid explosive type(%d) in %s:%d",type,__FILE__,__LINE__);
					return -5;
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
					send_to_room(room_id,"An explosion catches you off guard as a {red}%s{/red} {yel}DETONATES!!!{/yel}\r\n",object->name.c_str());
					explosive_damage(victim, object);
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

			if(type == mw_explosive::SENSOR_GRENADE) {
				mods::sensor_grenade::handle_explosion(object_uuid,player_uuid,room_id,object->from_direction);
				return 1;
			}

			for(auto& person : mods::globals::get_room_list(room_id)) {
				if(mods::super_users::player_is(person)) {
					continue;
				}
				switch(type) {
					default:
						log("SYSERR: Invalid explosive type(%d) in %s:%d",type,__FILE__,__LINE__);
						return -5;
					case mw_explosive::REMOTE_EXPLOSIVE:
						mods::projectile::explosive_damage(person,object);
						break;
					case mw_explosive::REMOTE_CHEMICAL:
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
				if(does_damage) {
					mods::injure::explosive::handle_crit_injure(object,person);
				}
				explode_debug("gt_room_list(" << room_id << ") for looping through (" <<
				              mods::globals::get_room_list(room_id).size() << ") tenants"
				             );
			}
			if(type == mw_explosive::FLASHBANG_GRENADE) {
				queue_on_room({AFF(BLIND),AFF(DISORIENT)}, room_id);
			}
			if(type == mw_explosive::REMOTE_CHEMICAL) {
				queue_on_room({AFF(DISORIENT),AFF(POISON)}, room_id);
			}
			explode_debug("calling perform blast radius");
			mods::projectile::perform_blast_radius(room_id,blast_radius,object,player_uuid);
			if(dispose) {
				mods::globals::dispose_object(object_uuid);
			}
			return 2;
		}
		int8_t explode(room_rnum room_id,uuid_t object_uuid,uuid_t player_uuid) {
			return explode(room_id,object_uuid,player_uuid, true);
		}
		/**
		 * TODO: place the grenade on the floor so that some crazy bastards can potentially throw it back,
		 * but moreso that the user can atleast see it and do something about it
		 */
		int to_direction(const std::string& str) {
			return mods::util::to_direction<const std::string&>(str);
		}
		rooms_away_t calculate_shrapnel_rooms(room_rnum room, obj_ptr_t held_object,std::size_t blast_radius) {
			rooms_away_t rooms;
			for(auto& direction : {
			            NORTH, SOUTH, EAST, WEST, UP, DOWN
			        }) {
				room_rnum r = resolve_room(room, direction,blast_radius); /** TODO: use blast_radius as dynamic var */
				if(r == room) {
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
			switch(direction) {
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
				default:
					return "<?>";
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
			switch(direction) {
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
		int travel_to(room_rnum from, int direction, std::size_t depth, std::shared_ptr<obj_data> object) {
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

				if(room_id == NOWHERE) {
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
				explode(room_id,object_uuid,player_uuid,true);
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
				if(mods::rooms::is_peaceful(room_id)) {
					player->sendln("Target room is in the D.M.Z.. Your item has been destroyed.");
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
					ticks = FRAG_GRENADE_TICKS();
					break;
				case mw_explosive::INCENDIARY_GRENADE:
					ticks = INCENDIARY_GRENADE_TICKS();
					break;
				case mw_explosive::EMP_GRENADE:
					ticks = EMP_GRENADE_TICKS();
					break;
				case mw_explosive::SMOKE_GRENADE:
					ticks = SMOKE_GRENADE_TICKS();
					break;
				case mw_explosive::FLASHBANG_GRENADE:
					ticks = FLASHBANG_GRENADE_TICKS();
					break;
				case mw_explosive::SENSOR_GRENADE:
					ticks = SENSOR_GRENADE_TICKS();
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
		int deploy_shrapnel_at(player_ptr_t& victim, int dice_count,int dice_sides,int from_direction) {
			if(mods::super_users::player_is(victim)) {
				return 0;
			}
			int damage = dice(dice_count,dice_sides);
			victim->hp() -= damage;
			return damage;
		}
		std::tuple<bool,std::string> explode_frag_here(player_ptr_t& attacker, obj_ptr_t& frag, const room_rnum room_id) {
			explode(room_id,frag->uuid,attacker->uuid());
			return {1,"A fragmentation grenade explodes!"};
		}
		std::tuple<bool,std::string> frag_underbarrel_explodes_here(
		    player_ptr_t& attacker,
		    obj_ptr_t& frag,
		    const direction_t& direction,
		    const room_rnum& room_id,
		    const uint8_t& travel_distance
		) {
			/** TODO honor travel_distance and explode at the maximum distance */
			static constexpr const char* neither = "";
			if(world[room_id].dir_option[direction] == nullptr) {
				return explode_frag_here(attacker,frag,room_id);
			}
			auto& player_in_room = mods::globals::get_room_list(room_id);
			if(player_in_room.size() > FRAG_GRENADE_PREMATURE_ROOM_SIZE()) {
				explode(room_id,frag->uuid,attacker->uuid());
				return {true,"Your fragmentation grenade hits a group of people!"};
			}
			/** TODO: if anyone is holding a shield inside this room, blow it up here */
			return {false,neither};
		}

		/**
		 * The idea here is to accept different types of
		 * underbarrel attachments.
		 * 	- fragmentation grenades
		 * 	- sensor grenades (todo)
		 * 	- flash bang (todo)
		 * 	- incendiary (todo)
		 * 	- emp (todo)
		 * 	- corossive (todo)
		 *  -
		 */
		std::tuple<bool,std::string> launch_underbarrel_grenade(
		    player_ptr_t& attacker,
		    obj_ptr_t& launcher,
		    const direction_t& direction,
		    const std::size_t& distance
		) {
			if(launcher->has_attachment() == false) {
				return {0,"{red}You must have a valid attachment connected!{/red}"};
			}

			std::string type = launcher->attachment()->attributes->underbarrel_launcher_type;
			if(type.compare("SHOTGUN") == 0 || type.compare("TASER") == 0 || type.compare("CORROSIVE_SPRAY") == 0 ||
			        type.compare("FLAME_THROWER") == 0 || type.compare("NONE") == 0) {
				return {0,"{red}You are using the wrong type of underbarrel attachment!{/red}"};
			}
			obj_ptr_t frag = nullptr;
			if(type.compare("FRAG_LAUNCHER") == 0) {
				frag = create_object(ITEM_EXPLOSIVE,"frag-grenade.yml");
			} else if(type.compare("SENSOR_LAUNCHER") == 0) {
				frag = create_object(ITEM_EXPLOSIVE,"sensor-grenade.yml");
			} else if(type.compare("SMOKE_LAUNCHER") == 0) {
				frag = create_object(ITEM_EXPLOSIVE,"smoke-grenade.yml");
			} else if(type.compare("EMP_LAUNCHER") == 0) {
				frag = create_object(ITEM_EXPLOSIVE,"emp-grenade.yml");
			} else if(type.compare("INCENDIARY_LAUNCHER") == 0) {
				frag = create_object(ITEM_EXPLOSIVE,"incendiary-grenade.yml");
			} else if(type.compare("BREACH_LAUNCHER") == 0) {
				frag = create_object(ITEM_EXPLOSIVE,"breach-charge.yml");
			} else if(type.compare("THERMITE_BREACH_LAUNCHER") == 0) {
				frag = create_object(ITEM_EXPLOSIVE,"thermite-charge.yml");
			}

			if(!frag) {
				return {0,"{red}You are using the wrong type of underbarrel attachment!{/red}"};
			}
			auto room_id = attacker->room();
			if(!world[room_id].dir_option[direction]) {
				explode_debug("frag travel to hit a wall!");
				return explode_frag_here(attacker, frag, room_id);
			} else {
				int travel_distance = 0;
				do {
					explode_debug("looping");
					auto s = frag_underbarrel_explodes_here(attacker, frag, direction, room_id,travel_distance);
					if(std::get<0>(s)) {
						explode_debug("stopped short");
						return s;
					}
					if(!world[room_id].dir_option[direction]) {
						explode_debug("stopped short due to invalid dir_option");
						return explode_frag_here(attacker, frag, room_id);
					}
					explode_debug("traversing...");
					room_id = world[room_id].dir_option[direction]->to_room;
					if(room_id >= world.size()) {
						return explode_frag_here(attacker, frag, room_id);
					}
				} while(++travel_distance < distance);
			}
			explode_debug("Returning: " << room_id << " explode here");
			return explode_frag_here(attacker, frag, room_id);
		}

		ACMD(do_fragtest_east) {
			ADMIN_REJECT();
			DO_HELP("fragtest_east");
			static constexpr const char* usage = "usage: fragtest_east <direction> <distance>";
			if(argshave()->int_at(1)->direction_at(0)->passed() == false) {
				player->sendln(usage);
				return;
			}
			auto dir = dirat(0);
			auto distance = intat(1);
			if(dir.has_value() == false) {
				player->sendln("{red}Invalid direction{/red}");
				return;
			}
			auto launcher = player->ghost()->underbarrel();
			if(!launcher) {
				player->sendln("{red}You don't have a launcher attached!{/red}");
				return;
			}
			launch_underbarrel_grenade(
			    player,
			    launcher,
			    dir.value(),
			    distance
			);

			ADMIN_DONE();
		}
		void init() {
			mods::interpreter::add_command("fe", POS_RESTING, do_fragtest_east, LVL_BUILDER,0);
		}

	};
};
#undef m_error
