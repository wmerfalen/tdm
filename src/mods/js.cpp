#include "js.hpp"
#include "quests.hpp"
#include "util.hpp"
#include "extern.hpp"
#include "loops.hpp"
#include "../spells.h"
#include "db.hpp"
#include "date-time.hpp"
#include "../config.hpp"

#include <unistd.h>	//for getcwd()
#define DT_FORMAT "{player_name}:mob_death_trigger"
extern void command_interpreter(struct char_data* ch,char* argument);
extern void hit(struct char_data* ch,struct char_data* vict,int type);
extern void affect_from_char(struct char_data *ch, int type);
extern void mobile_activity();
namespace mods {
	namespace js {
			std::string current_working_dir(){
				char* cwd = ::get_current_dir_name();
				std::string path = cwd == nullptr ? "" : cwd;
				if(cwd){ free(cwd); }
				return path;
			}
		namespace utils {
			struct find_player_payload_t {
				find_player_payload_t(std::string_view name) : player_name(name.data()), found(false){
					player = nullptr;
				}
				std::string player_name;
				bool found;
				std::shared_ptr<mods::player> player;
			};
			static inline bool __find_player_by_name(std::shared_ptr<mods::player> player_ptr,find_player_payload_t* param){
				param->found = false;
				std::cerr << "current-player(find):'" << player_ptr->name().c_str() << "'\nLooking for: '" << param->player_name.c_str() << "'\n";
					if(std::string(player_ptr->name().c_str()).compare(param->player_name.c_str()) == 0){
						param->found = true;
						param->player = player_ptr;
						return false;//False means stop looping, we're done. 
					}else{
						return true;//True means keep looping, we're not done
					}
			}
			std::shared_ptr<mods::player> find_player_by_name(std::string_view name,bool& found){
				find_player_payload_t payload(name);
				mods::loops::foreach_player<mods::js::utils::find_player_payload_t>(
						__find_player_by_name, &payload);
				found = payload.found;
				return payload.player;
			}

		};//end utils namespace
		int load_library(duk_context*,std::string_view);
		namespace test {
			static duk_ret_t require_test(duk_context *ctx) {
				/* First parameter is character name */
				auto fname = duk_to_string(ctx,0);
				std::string path = mods::js::current_working_dir() + std::string("/js/tests/") + fname;
				duk_push_number(ctx,mods::js::load_library(ctx,path));
				return 1;	/* number of return values */
			}
		};
		/*! \brief returns the current month as a string within the game
		 * \return integer
		 */
		static duk_ret_t get_month(duk_context *ctx) {
			/* First parameter is character name */
			duk_push_string(ctx,mods::date_time::get_month().c_str());
			return 1;	/* number of return values */
		}
		/*! \brief returns the current day as a string within the game
		 * \return integer
		 */
		static duk_ret_t get_day(duk_context *ctx) {
			/* First parameter is character name */
			duk_push_string(ctx,mods::date_time::get_day().c_str());
			return 1;	/* number of return values */
		}
		/*! \brief returns the current moon phase as a string
		 * \return integer
		 */
		static duk_ret_t get_moon_phase(duk_context *ctx) {
			/* First parameter is character name */
			duk_push_string(ctx,mods::date_time::get_moon_phase().c_str());
			return 1;	/* number of return values */
		}
		/*! \brief returns the current hour within the game
		 * \return integer
		 */
		static duk_ret_t get_ihour(duk_context *ctx) {
			/* First parameter is character name */
			duk_push_number(ctx,mods::date_time::get_ihour());
			return 1;	/* number of return values */
		}
		/*! \brief returns the current month as integer within the game
		 * \return integer
		 */
		static duk_ret_t get_imonth(duk_context *ctx) {
			/* First parameter is character name */
			duk_push_number(ctx,mods::date_time::get_imonth());
			return 1;	/* number of return values */
		}
		/*! \brief returns the current day as an integer within the game
		 * \return integer
		 */
		static duk_ret_t get_iday(duk_context *ctx) {
			/* First parameter is character name */
			duk_push_number(ctx,mods::date_time::get_iday());
			return 1;	/* number of return values */
		}
		/*! \brief returns the current year as an integer within the game
		 * \return integer
		 */
		static duk_ret_t get_iyear(duk_context *ctx) {
			/* First parameter is character name */
			duk_push_number(ctx,mods::date_time::get_iyear());
			return 1;	/* number of return values */
		}

		/*! \brief requires the javascript library by using the internal mods::js::load_library method
		 * \param string to be appended to mods::js::JS_PATH
		 * \return integer
		 */
		static duk_ret_t require_js(duk_context *ctx) {
			/* First parameter is character name */
			auto fname = duk_to_string(ctx,0);
			std::string path = mods::js::current_working_dir() + std::string("/js/") + fname;
			duk_push_number(ctx,mods::js::load_library(ctx,path));
			return 1;	/* number of return values */
		}
		/*
		static duk_ret_t list_mobiles(duk_context *ctx){
			return 0;
		}*/
		/*! \brief internally calls read_mobile. parameter 1 is the mob's id as REAL (not VIRTUAL). Places the mob into the second parameter (REAL room id). 
		 * \param 1 integer mob's ID (REAL)
		 * \param 2 integer room ID (REAL)
		 * \return integer returns -1 if there were incorrect number of parameters, returns -2 if we could not find the mobile, returns -3 if we couldn't find the room, returns 1 if we successfully found the mob, the room, and placed the mob inside the room.
		 */
		static duk_ret_t read_mobile(duk_context *ctx){
			if(duk_get_top(ctx) < 2){
				duk_push_number(ctx,1);
				return -1;
			}
			uint32_t mob_id = duk_to_uint32(ctx,0);
			uint32_t room_id = duk_to_uint32(ctx,1);
			auto mob = read_mobile(mob_id,REAL);
			if(!mob){
				duk_push_number(ctx,1);
				return -2;
			}
			auto real_room_id = real_room(room_id);
			if(real_room_id == NOWHERE){
				duk_push_number(ctx,1);
				return -3;
			}
			char_to_room(mob,real_room_id);
			duk_push_number(ctx,1);
			return 1;
		}
		/*! \brief deals damage from the player identified (by uuid) to the player identified (by uuid).
		 * \param unsigned integer uuid of the player dealing damage
		 * \param unsigned integer uuid of the player to receive damage
		 * \return integer always return 1
		 */
		static duk_ret_t hit(duk_context *ctx) {
			/* First parameter is character name */
			auto char_uuid = duk_to_number(ctx,0);
			auto vict_uuid = duk_to_number(ctx,1);
			::hit(mods::globals::player_list[char_uuid]->cd(),mods::globals::player_list[vict_uuid]->cd(),0);
			duk_push_number(ctx,1);
			return 1;	/* number of return values */
		}

		/*! \brief gets the player's uuid and player.name data and returns it as a duktape.js object with keys 'uuid' and 'name'
		 * \return object the duktape.js javascript object with keys: 'uuid' and 'name'
		 */
		static duk_ret_t get_current_player(duk_context *ctx) {
			/* First parameter is character name */
			duk_idx_t obj_idx;

			obj_idx = duk_push_object(ctx);
			duk_push_int(ctx, mods::globals::current_player->cd()->uuid);
			duk_put_prop_string(ctx, obj_idx, "uuid");
			duk_push_string(ctx, mods::globals::current_player->cd()->player.name);
			duk_put_prop_string(ctx, obj_idx, "name");
			return 1;
		}
		/*! \brief sends a string to the current player. The method for which this function finds the current player is by utilizing mods::globals::current_player
		 * \param string the string to send
		 * \return nothing
		 */
		static duk_ret_t send(duk_context *ctx) {
			/* First parameter is character name */
			std::string message = duk_to_string(ctx,0);
			if(mods::globals::current_player){
				*mods::globals::current_player << message << "\r\n";
			}
			duk_push_number(ctx,0);
			return 0;
		}
		/*! \brief clears the player's affected flags. Internally, this function calls mods::player::clear_all_affected_plr.
		 * The method for which it finds the player is utils::find_player_by_name(name,&found)
		 * \param player's name
		 * \return integer -1 if player couldn't be found by the method above, 0 if found and cleared.
		 */
		static duk_ret_t clear_all_plr_flags(duk_context *ctx) {
			/** TODO: get array from duktape */
			std::string name = duk_to_string(ctx,0);
				bool found = false;
				auto player_ptr = utils::find_player_by_name(name,found);
				if(!found){
					duk_push_number(ctx,-1);
					return 1;
				}
				player_ptr->clear_all_affected_plr();
				duk_push_number(ctx,0);
				return 1;
		}
		/*! \brief clears the player's affected flags. Internally calls
		 * player::clear_all_affected on the found player. 
		 * \param string player's name as found by utils::find_player_by_name
		 * \return integer returns -1 if player couldn't be found, 0 if player found and clear_all_affected called on player's object
		 */
		static duk_ret_t clear_all_affected_flags(duk_context *ctx) {
			/** TODO: get array from duktape */
			std::string name = duk_to_string(ctx,0);
				bool found = false;
				auto player_ptr = utils::find_player_by_name(name,found);
				if(!found){
					duk_push_number(ctx,-1);
					return 1;
				}
				player_ptr->clear_all_affected();
				duk_push_number(ctx,0);
				return 1;
		}

		/*! \brief modifies the player's affected flags. 
		 * \param user's name
		 * \param the flag to toggle
		 * \param boolean toggle state. (1 or 0)
		 * \return integer
		 */
		static duk_ret_t modify_plr_flags(duk_context *ctx) {
			/** TODO: get array from duktape */
			std::string name = duk_to_string(ctx,0);
			std::string key = duk_to_string(ctx,1);
			bool on = (bool)duk_to_number(ctx,2);
				bool found = false;
				auto player_ptr = utils::find_player_by_name(name,found);
				if(!found){
					duk_push_number(ctx,-1);
					return 1;
				}
if(key.compare("KILLER")==0){ if(on){ player_ptr->affect_plr(PLR_KILLER); }else{ player_ptr->remove_affect_plr(PLR_KILLER); }}
else if(key.compare("THIEF")==0){ if(on){ player_ptr->affect_plr(PLR_THIEF); }else{ player_ptr->remove_affect_plr(PLR_THIEF); }}
else if(key.compare("FROZEN")==0){ if(on){ player_ptr->affect_plr(PLR_FROZEN); }else{ player_ptr->remove_affect_plr(PLR_FROZEN); }}
else if(key.compare("DONTSET")==0){ if(on){ player_ptr->affect_plr(PLR_DONTSET); }else{ player_ptr->remove_affect_plr(PLR_DONTSET); }}
else if(key.compare("WRITING")==0){ if(on){ player_ptr->affect_plr(PLR_WRITING); }else{ player_ptr->remove_affect_plr(PLR_WRITING); }}
else if(key.compare("MAILING")==0){ if(on){ player_ptr->affect_plr(PLR_MAILING); }else{ player_ptr->remove_affect_plr(PLR_MAILING); }}
else if(key.compare("CRASH")==0){ if(on){ player_ptr->affect_plr(PLR_CRASH); }else{ player_ptr->remove_affect_plr(PLR_CRASH); }}
else if(key.compare("SITEOK")==0){ if(on){ player_ptr->affect_plr(PLR_SITEOK); }else{ player_ptr->remove_affect_plr(PLR_SITEOK); }}
else if(key.compare("NOSHOUT")==0){ if(on){ player_ptr->affect_plr(PLR_NOSHOUT); }else{ player_ptr->remove_affect_plr(PLR_NOSHOUT); }}
else if(key.compare("NOTITLE")==0){ if(on){ player_ptr->affect_plr(PLR_NOTITLE); }else{ player_ptr->remove_affect_plr(PLR_NOTITLE); }}
else if(key.compare("DELETED")==0){ if(on){ player_ptr->affect_plr(PLR_DELETED); }else{ player_ptr->remove_affect_plr(PLR_DELETED); }}
else if(key.compare("LOADROOM")==0){ if(on){ player_ptr->affect_plr(PLR_LOADROOM); }else{ player_ptr->remove_affect_plr(PLR_LOADROOM); }}
else if(key.compare("NOWIZLIST")==0){ if(on){ player_ptr->affect_plr(PLR_NOWIZLIST); }else{ player_ptr->remove_affect_plr(PLR_NOWIZLIST); }}
else if(key.compare("NODELETE")==0){ if(on){ player_ptr->affect_plr(PLR_NODELETE); }else{ player_ptr->remove_affect_plr(PLR_NODELETE); }}
else if(key.compare("INVSTART")==0){ if(on){ player_ptr->affect_plr(PLR_INVSTART); }else{ player_ptr->remove_affect_plr(PLR_INVSTART); }}
else if(key.compare("CRYO")==0){ if(on){ player_ptr->affect_plr(PLR_CRYO); }else{ player_ptr->remove_affect_plr(PLR_CRYO); }}
else if(key.compare("NOTDEADYET")==0){ if(on){ player_ptr->affect_plr(PLR_NOTDEADYET); }else{ player_ptr->remove_affect_plr(PLR_NOTDEADYET); }}
				duk_push_number(ctx,0);
				return 1;
		}
		/*! \brief toggles player affected flags. Internally calls player::affect
		 * and player::remove_affect for toggling on or off, respectively.
		 * \example js modify_affected_flags('far','INVISIBLE',1);
		 * \param player name
		 * \param string constant of the affected player flag
		 * \param boolean on or off (1|0)
		 * \return int
		 */
		static duk_ret_t modify_affected_flags(duk_context *ctx) {
			/** TODO: get array from duktape */
			std::string name = duk_to_string(ctx,0);
			std::string key = duk_to_string(ctx,1);
			bool on = (bool)duk_to_number(ctx,2);
				bool found = false;
				auto player_ptr = utils::find_player_by_name(name,found);
				if(!found){
					duk_push_number(ctx,-1);
					return 1;
				}

if(key.compare("BLIND") == 0){ if(on){ player_ptr->affect(AFF_BLIND); }else{ player_ptr->remove_affect(AFF_BLIND); }}
else if(key.compare("INVISIBLE") == 0){ if(on){ player_ptr->affect(AFF_INVISIBLE); }else{ player_ptr->remove_affect(AFF_INVISIBLE); }}
else if(key.compare("DETECT_ALIGN") == 0){ if(on){ player_ptr->affect(AFF_DETECT_ALIGN); }else{ player_ptr->remove_affect(AFF_DETECT_ALIGN); }}
else if(key.compare("DETECT_INVIS") == 0){ if(on){ player_ptr->affect(AFF_DETECT_INVIS); }else{ player_ptr->remove_affect(AFF_DETECT_INVIS); }}
else if(key.compare("DETECT_MAGIC") == 0){ if(on){ player_ptr->affect(AFF_DETECT_MAGIC); }else{ player_ptr->remove_affect(AFF_DETECT_MAGIC); }}
else if(key.compare("SENSE_LIFE") == 0){ if(on){ player_ptr->affect(AFF_SENSE_LIFE); }else{ player_ptr->remove_affect(AFF_SENSE_LIFE); }}
else if(key.compare("WATERWALK") == 0){ if(on){ player_ptr->affect(AFF_WATERWALK); }else{ player_ptr->remove_affect(AFF_WATERWALK); }}
else if(key.compare("SANCTUARY") == 0){ if(on){ player_ptr->affect(AFF_SANCTUARY); }else{ player_ptr->remove_affect(AFF_SANCTUARY); }}
else if(key.compare("GROUP") == 0){ if(on){ player_ptr->affect(AFF_GROUP); }else{ player_ptr->remove_affect(AFF_GROUP); }}
else if(key.compare("CURSE") == 0){ if(on){ player_ptr->affect(AFF_CURSE); }else{ player_ptr->remove_affect(AFF_CURSE); }}
else if(key.compare("INFRAVISION") == 0){ if(on){ player_ptr->affect(AFF_INFRAVISION); }else{ player_ptr->remove_affect(AFF_INFRAVISION); }}
else if(key.compare("POISON") == 0){ if(on){ player_ptr->affect(AFF_POISON); }else{ player_ptr->remove_affect(AFF_POISON); }}
else if(key.compare("PROTECT_EVIL") == 0){ if(on){ player_ptr->affect(AFF_PROTECT_EVIL); }else{ player_ptr->remove_affect(AFF_PROTECT_EVIL); }}
else if(key.compare("PROTECT_GOOD") == 0){ if(on){ player_ptr->affect(AFF_PROTECT_GOOD); }else{ player_ptr->remove_affect(AFF_PROTECT_GOOD); }}
else if(key.compare("SLEEP") == 0){ if(on){ player_ptr->affect(AFF_SLEEP); }else{ player_ptr->remove_affect(AFF_SLEEP); }}
else if(key.compare("NOTRACK") == 0){ if(on){ player_ptr->affect(AFF_NOTRACK); }else{ player_ptr->remove_affect(AFF_NOTRACK); }}
else if(key.compare("UNUSED16") == 0){ if(on){ player_ptr->affect(AFF_UNUSED16); }else{ player_ptr->remove_affect(AFF_UNUSED16); }}
else if(key.compare("UNUSED17") == 0){ if(on){ player_ptr->affect(AFF_UNUSED17); }else{ player_ptr->remove_affect(AFF_UNUSED17); }}
else if(key.compare("SNEAK") == 0){ if(on){ player_ptr->affect(AFF_SNEAK); }else{ player_ptr->remove_affect(AFF_SNEAK); }}
else if(key.compare("HIDE") == 0){ if(on){ player_ptr->affect(AFF_HIDE); }else{ player_ptr->remove_affect(AFF_HIDE); }}
else if(key.compare("UNUSED20") == 0){ if(on){ player_ptr->affect(AFF_UNUSED20); }else{ player_ptr->remove_affect(AFF_UNUSED20); }}
else if(key.compare("CHARM") == 0){ if(on){ player_ptr->affect(AFF_CHARM); }else{ player_ptr->remove_affect(AFF_CHARM); }}
duk_push_number(ctx,0);
return 1;
		}
		static duk_ret_t affect_from_char(duk_context *ctx) {
			/** TODO: get array from duktape */
			std::string name = duk_to_string(ctx,0);
			std::string key = duk_to_string(ctx,1);
				bool found = false;
				auto player_ptr = utils::find_player_by_name(name,found);
				if(!found){
					duk_push_number(ctx,-1);
					return 1;
				}
if(key.compare("ARMOR") == 0){ ::affect_from_char(*player_ptr,SPELL_ARMOR); }
if(key.compare("TELEPORT") == 0){ ::affect_from_char(*player_ptr,SPELL_TELEPORT); }
if(key.compare("BLESS") == 0){ ::affect_from_char(*player_ptr,SPELL_BLESS); }
if(key.compare("BLINDNESS") == 0){ ::affect_from_char(*player_ptr,SPELL_BLINDNESS); }
if(key.compare("BURNING_HANDS") == 0){ ::affect_from_char(*player_ptr,SPELL_BURNING_HANDS); }
if(key.compare("CALL_LIGHTNING") == 0){ ::affect_from_char(*player_ptr,SPELL_CALL_LIGHTNING); }
if(key.compare("CHARM") == 0){ ::affect_from_char(*player_ptr,SPELL_CHARM); }
if(key.compare("CHILL_TOUCH") == 0){ ::affect_from_char(*player_ptr,SPELL_CHILL_TOUCH); }
if(key.compare("CLONE") == 0){ ::affect_from_char(*player_ptr,SPELL_CLONE); }
if(key.compare("COLOR_SPRAY") == 0){ ::affect_from_char(*player_ptr,SPELL_COLOR_SPRAY); }
if(key.compare("CONTROL_WEATHER") == 0){ ::affect_from_char(*player_ptr,SPELL_CONTROL_WEATHER); }
if(key.compare("CREATE_FOOD") == 0){ ::affect_from_char(*player_ptr,SPELL_CREATE_FOOD); }
if(key.compare("CREATE_WATER") == 0){ ::affect_from_char(*player_ptr,SPELL_CREATE_WATER); }
if(key.compare("CURE_BLIND") == 0){ ::affect_from_char(*player_ptr,SPELL_CURE_BLIND); }
if(key.compare("CURE_CRITIC") == 0){ ::affect_from_char(*player_ptr,SPELL_CURE_CRITIC); }
if(key.compare("CURE_LIGHT") == 0){ ::affect_from_char(*player_ptr,SPELL_CURE_LIGHT); }
if(key.compare("CURSE") == 0){ ::affect_from_char(*player_ptr,SPELL_CURSE); }
if(key.compare("DETECT_ALIGN") == 0){ ::affect_from_char(*player_ptr,SPELL_DETECT_ALIGN); }
if(key.compare("DETECT_INVIS") == 0){ ::affect_from_char(*player_ptr,SPELL_DETECT_INVIS); }
if(key.compare("DETECT_MAGIC") == 0){ ::affect_from_char(*player_ptr,SPELL_DETECT_MAGIC); }
if(key.compare("DETECT_POISON") == 0){ ::affect_from_char(*player_ptr,SPELL_DETECT_POISON); }
if(key.compare("DISPEL_EVIL") == 0){ ::affect_from_char(*player_ptr,SPELL_DISPEL_EVIL); }
if(key.compare("EARTHQUAKE") == 0){ ::affect_from_char(*player_ptr,SPELL_EARTHQUAKE); }
if(key.compare("ENCHANT_WEAPON") == 0){ ::affect_from_char(*player_ptr,SPELL_ENCHANT_WEAPON); }
if(key.compare("ENERGY_DRAIN") == 0){ ::affect_from_char(*player_ptr,SPELL_ENERGY_DRAIN); }
if(key.compare("FIREBALL") == 0){ ::affect_from_char(*player_ptr,SPELL_FIREBALL); }
if(key.compare("HARM") == 0){ ::affect_from_char(*player_ptr,SPELL_HARM); }
if(key.compare("HEAL") == 0){ ::affect_from_char(*player_ptr,SPELL_HEAL); }
if(key.compare("INVISIBLE") == 0){ ::affect_from_char(*player_ptr,SPELL_INVISIBLE); }
if(key.compare("LIGHTNING_BOLT") == 0){ ::affect_from_char(*player_ptr,SPELL_LIGHTNING_BOLT); }
if(key.compare("LOCATE_OBJECT") == 0){ ::affect_from_char(*player_ptr,SPELL_LOCATE_OBJECT); }
if(key.compare("MAGIC_MISSILE") == 0){ ::affect_from_char(*player_ptr,SPELL_MAGIC_MISSILE); }
if(key.compare("POISON") == 0){ ::affect_from_char(*player_ptr,SPELL_POISON); }
if(key.compare("PROT_FROM_EVIL") == 0){ ::affect_from_char(*player_ptr,SPELL_PROT_FROM_EVIL); }
if(key.compare("REMOVE_CURSE") == 0){ ::affect_from_char(*player_ptr,SPELL_REMOVE_CURSE); }
if(key.compare("SANCTUARY") == 0){ ::affect_from_char(*player_ptr,SPELL_SANCTUARY); }
if(key.compare("SHOCKING_GRASP") == 0){ ::affect_from_char(*player_ptr,SPELL_SHOCKING_GRASP); }
if(key.compare("SLEEP") == 0){ ::affect_from_char(*player_ptr,SPELL_SLEEP); }
if(key.compare("STRENGTH") == 0){ ::affect_from_char(*player_ptr,SPELL_STRENGTH); }
if(key.compare("SUMMON") == 0){ ::affect_from_char(*player_ptr,SPELL_SUMMON); }
if(key.compare("VENTRILOQUATE") == 0){ ::affect_from_char(*player_ptr,SPELL_VENTRILOQUATE); }
if(key.compare("WORD_OF_RECALL") == 0){ ::affect_from_char(*player_ptr,SPELL_WORD_OF_RECALL); }
if(key.compare("REMOVE_POISON") == 0){ ::affect_from_char(*player_ptr,SPELL_REMOVE_POISON); }
if(key.compare("SENSE_LIFE") == 0){ ::affect_from_char(*player_ptr,SPELL_SENSE_LIFE); }
if(key.compare("ANIMATE_DEAD") == 0){ ::affect_from_char(*player_ptr,SPELL_ANIMATE_DEAD); }
if(key.compare("DISPEL_GOOD") == 0){ ::affect_from_char(*player_ptr,SPELL_DISPEL_GOOD); }
if(key.compare("GROUP_ARMOR") == 0){ ::affect_from_char(*player_ptr,SPELL_GROUP_ARMOR); }
if(key.compare("GROUP_HEAL") == 0){ ::affect_from_char(*player_ptr,SPELL_GROUP_HEAL); }
if(key.compare("GROUP_RECALL") == 0){ ::affect_from_char(*player_ptr,SPELL_GROUP_RECALL); }
if(key.compare("INFRAVISION") == 0){ ::affect_from_char(*player_ptr,SPELL_INFRAVISION); }
if(key.compare("WATERWALK") == 0){ ::affect_from_char(*player_ptr,SPELL_WATERWALK); }
			duk_push_number(ctx,0);
			return 1;
		}

		static duk_ret_t uuid(duk_context *ctx) {
			/* First parameter is character name */

			std::string char_name = duk_to_string(ctx,0);

			for(auto ch = character_list; ch; ch = ch->next) {
				if(char_name.compare(ch->player.name) == 0 && !IS_NPC(ch)) {
					duk_push_number(ctx,ch->uuid);
					return 1;
				}
			}

			duk_push_number(ctx,-1);
			return 1;
		}
		static duk_ret_t char_from_room(duk_context *ctx){
			std::string c_name = duk_to_string(ctx,0);

			if(c_name.length()){
				bool found = false;
				auto player_ptr = utils::find_player_by_name(c_name,found);
				if(found){
					std::cerr << "found player bypayload\n";
					mods::globals::rooms::char_from_room(*(player_ptr));
					duk_push_number(ctx,0);
					return 1;
				}
			}
			duk_push_number(ctx,-1);
			return 1; //mods::globals::current_player->cd()->in_room;
		}
		static duk_ret_t set_char_pk_id(duk_context *ctx){
			std::string c_name = duk_to_string(ctx,0);
			aligned_int_t pk = duk_to_number(ctx,1);
				bool found = false;
				auto player_ptr = utils::find_player_by_name(c_name,found);
				if(found){
					player_ptr->set_db_id(pk);
					mods::db::save_char(player_ptr);
					duk_push_number(ctx,0);
					return 1;
				}
					duk_push_number(ctx,-1);
					return 1;
		}
		static duk_ret_t room(duk_context *ctx){
			auto room = mods::globals::current_player->room();
			duk_push_number(ctx,room);
			return 1;
		}

		static duk_ret_t cmd(duk_context *ctx) {
			/* First parameter is character name */
			std::string cmd =  duk_to_string(ctx,0);
			mods::globals::current_player->executing_js(true);
			command_interpreter(mods::globals::current_player->cd(),cmd.c_str());
			mods::globals::current_player->executing_js(false);
			return 0;	/* number of return values */
		}
		static duk_ret_t cmd_exec(duk_context *ctx) {
			/* First parameter is character name */
			std::string cmd =  duk_to_string(ctx,0);
			auto player = mods::globals::current_player;
			mods::globals::current_player->executing_js(true);
			player->capture_output(true);
			command_interpreter(player->cd(),cmd.c_str());
			player->capture_output(false);
			mods::globals::current_player->executing_js(false);
			duk_push_string(ctx,player->get_captured_output().data());
			player->clear_captured_output();
			return 1;
		}
		static duk_ret_t mob_death_trigger(duk_context *ctx) {
			/* First parameter is character name */
			std::string char_name = duk_to_string(ctx,0);
			std::string functor = duk_to_string(ctx,1);
			std::string dt_key = mods::globals::replace_all(DT_FORMAT,"{player_name}",char_name);
			DBSET(dt_key,functor);
			return 0;	/* number of return values */
		}
		static duk_ret_t in_room(duk_context *ctx) {
			/* First parameter is character name */
			std::string c_name = duk_to_string(ctx,0);

			for(auto ch = character_list; ch->next; ch = ch->next) {
				if(c_name.compare(ch->player.name) == 0) {
					duk_push_number(ctx,IN_ROOM(ch));
					return 1;
				}
			}

			duk_push_number(ctx,-1);
			return 1;	/* number of return values */
		}
		static duk_ret_t send_to_uuid(duk_context *ctx) {
			/* First parameter is character name */
			auto cuuid = duk_to_number(ctx,0);
			send_to_char(mods::globals::player_list.at(cuuid)->cd(),"%s",duk_to_string(ctx,1));
			return 0;	/* number of return values */
		}
		static duk_ret_t send_to_char(duk_context *ctx) {
			/* First parameter is character name */
			std::string c_name = duk_to_string(ctx,0);
				bool found = false;
				auto player_ptr = utils::find_player_by_name(c_name,found);
				if(found){
					::send_to_char(*player_ptr,"%s",duk_to_string(ctx,1));
					return 0;
				}
			return 0;	/* number of return values */
		}
		static duk_ret_t mobile_activity(duk_context *ctx){
			::mobile_activity();
			duk_push_number(ctx,0);
			return 1;
		}
		static duk_ret_t db_seti(duk_context *ctx) {
			std::string key = duk_to_string(ctx,0);
			auto value = duk_to_number(ctx,1);
			mods::globals::db->put(key,std::to_string(value));
			return 0;
		}
		static duk_ret_t db_geti(duk_context *ctx) {
			std::string key = duk_to_string(ctx,0);
			std::string value = "";
			mods::globals::db->get(key,value);
			auto i_value = mods::util::stoi(value);

			if(i_value.has_value()) {
				duk_push_number(ctx,i_value.value());
			}
			return 1;
		}
		static duk_ret_t db_set(duk_context *ctx) {
			std::string key = duk_to_string(ctx,0);
			std::string value = duk_to_string(ctx,1);
			mods::globals::db->put(key,value);
			return 0;
		}
		static duk_ret_t db_get(duk_context *ctx) {
			std::string key = duk_to_string(ctx,0);
			std::string value = "";
			mods::globals::db->get(key,value);
			duk_push_string(ctx,value.c_str());
			return 1;
		}
		static duk_ret_t unsafe_code(duk_context *ctx,void* udata) {
			duk_push_string(ctx,static_cast<char*>(udata));
			duk_eval(ctx);
			return 0;
		}
		void log_js_error(std::string_view error) {
			std::cerr << "js error: " << error.data() << "\n";
		}

		void contextual_eval_string(char_data* player,duk_context* ctx,const std::string& str) {
			auto shrd_ptr_player = mods::globals::player_list.at(player->uuid);
			std::string code = std::string("player_object = ") + shrd_ptr_player->js_object() + std::string(";\n");
			code += str;

			if(duk_safe_call(ctx,mods::js::unsafe_code,static_cast<void*>(const_cast<char*>(code.c_str())),0,1) != 0) {
				log_js_error(duk_safe_to_string(ctx,-1));
			}

			duk_pop(ctx);
		}
		void contextual_eval_string(mods::player* player,duk_context* ctx,const std::string& str) {
			contextual_eval_string(player->cd(),ctx,str);
		}
		void eval_string(duk_context* ctx,const std::string& str) {
			if(duk_safe_call(ctx,mods::js::unsafe_code,static_cast<void*>(const_cast<char*>(str.c_str())),0,1) != 0) {
				log_js_error(duk_safe_to_string(ctx,-1));
			}

			duk_pop(ctx);
		}
		void eval_string(std::string_view str) {
			eval_string(mods::globals::duktape_context,str.data());
		}
		void load_c_functions() {
			load_c_functions(mods::globals::duktape_context);
		}
		void load_c_require_functions(duk_context *ctx) {
			duk_push_c_function(ctx,mods::js::require_js,1);
			duk_put_global_string(ctx,"require_js");
		}
		void load_c_test_functions(duk_context *ctx) {
			duk_push_c_function(ctx,mods::js::test::require_test,1);
			duk_put_global_string(ctx,"require_test");
			duk_push_c_function(ctx,mods::js::mobile_activity,0);
			duk_put_global_string(ctx,"mobile_activity");
		}
		void load_base_functions(duk_context *ctx) {
			duk_push_c_function(ctx,mods::js::get_month,0);
			duk_put_global_string(ctx,"get_month");
			duk_push_c_function(ctx,mods::js::get_day,0);
			duk_put_global_string(ctx,"get_day");
			duk_push_c_function(ctx,mods::js::get_moon_phase,0);
			duk_put_global_string(ctx,"get_moon_phase");
			duk_push_c_function(ctx,mods::js::get_ihour,0);
			duk_put_global_string(ctx,"get_ihour");
			duk_push_c_function(ctx,mods::js::get_imonth,0);
			duk_put_global_string(ctx,"get_imonth");
			duk_push_c_function(ctx,mods::js::get_iday,0);
			duk_put_global_string(ctx,"get_iday");
			duk_push_c_function(ctx,mods::js::get_iyear,0);
			duk_put_global_string(ctx,"get_iyear");

			duk_push_c_function(ctx,mods::js::room,0);
			duk_put_global_string(ctx,"room");
			duk_push_c_function(ctx,mods::js::read_mobile,2);
			duk_put_global_string(ctx,"read_mobile");
			duk_push_c_function(ctx,mods::js::get_current_player,0);
			duk_put_global_string(ctx,"get_current_player");
			duk_push_c_function(ctx,mods::js::send,1);
			duk_put_global_string(ctx,"send");
			duk_push_c_function(ctx,mods::js::uuid,1);
			duk_put_global_string(ctx,"uuid");
			duk_push_c_function(ctx,mods::js::hit,2);
			duk_put_global_string(ctx,"hit");
			duk_push_c_function(ctx,mods::js::cmd_exec,2);
			duk_put_global_string(ctx,"cmd_exec");
			duk_push_c_function(ctx,mods::js::cmd,2);
			duk_put_global_string(ctx,"cmd");
			duk_push_c_function(ctx,mods::js::send_to_uuid,2);
			duk_put_global_string(ctx,"send_to_uuid");
			duk_push_c_function(ctx,mods::js::send_to_char,2);
			duk_put_global_string(ctx,"send_to_char");
			duk_push_c_function(ctx,mods::js::db_seti,2);
			duk_put_global_string(ctx,"db_seti");
			duk_push_c_function(ctx,mods::js::db_geti,1);
			duk_put_global_string(ctx,"db_geti");
			duk_push_c_function(ctx,mods::js::db_set,2);
			duk_put_global_string(ctx,"db_set");
			duk_push_c_function(ctx,mods::js::db_get,1);
			duk_put_global_string(ctx,"db_get");
			duk_push_c_function(ctx,mods::js::in_room,1);
			duk_put_global_string(ctx,"in_room");
			duk_push_c_function(ctx,mods::js::mob_death_trigger,2);
			duk_put_global_string(ctx,"mob_death_trigger");
			duk_push_c_function(ctx,mods::js::char_from_room,1);
			duk_put_global_string(ctx,"char_from_room");
		}

		//enum mask_type { SMG, SNIPE, SHOTGUN, GRENADE };
			//typedef short weapon_set;
			//using		time_type_t = unsigned long;//std::chrono::system_clock::time_point;
			//enum player_type_enum_t { 
			//	PLAYER, MOB, DRONE,
		//		PLAYER_MUTED_DESCRIPTOR,
		//		MOB_MUTED_DESCRIPTOR,
		//		DRONE_MUTED_DESCRIPTOR
		//	};


		//	void init();

		//	std::string js_object();

		//	bool has_ammo();
		//	bool has_class_capability(class_type);
		//	bool has_equipment_tag(const std::string&);
		//	bool has_inventory_capability(int);
		//	bool has_thermite();
		//	bool has_weapon_capability(int);


		//	obj_data* get_first_ammo_of_type(const weapon_type_t&) const;

		//	bool can_snipe(char_data *target);
		//	bool is_weapon_loaded();
		//	bool carrying_ammo_of_type(const weapon_type_t&);
		//	void ammo_adjustment(int);
		//	int  ammo_type_adjustment(int,const weapon_type_t&);

		//	void set_class_capability(const class_capability_t& caps);
		//	time_type_t time() const;
		static duk_ret_t set_points(duk_context *ctx) {
			/** TODO: get array from duktape */
			std::string name = duk_to_string(ctx,0);
			std::string key = duk_to_string(ctx,1);
				bool found = false;
				auto player_ptr = utils::find_player_by_name(name,found);
				if(!found){
					duk_push_number(ctx,-1);
					return 1;
				}
				auto value = duk_to_number(ctx,2);
				if(key.compare("name") == 0){ 
					std::string str_value = duk_to_string(ctx,2);
					player_ptr->name() = str_value;
					goto __set_points_cleanup;
				}
			if(key.compare("mana") == 0){ player_ptr->mana() = static_cast<sh_int>(value); }
			else if(key.compare("max_mana") == 0){ player_ptr->max_mana() = static_cast<sh_int>(value); }
			else if(key.compare("hp") == 0){ player_ptr->hp() = static_cast<sh_int>(value); }
			else if(key.compare("max_hp") == 0){ player_ptr->max_hp() = static_cast<sh_int>(value); }
			else if(key.compare("move") == 0){ player_ptr->move() = static_cast<sh_int>(value); }
			else if(key.compare("max_move") == 0){ player_ptr->max_move() = static_cast<sh_int>(value); }
			else if(key.compare("armor") == 0){ player_ptr->armor() = static_cast<sh_int>(value); }
			else if(key.compare("gold") == 0){ player_ptr->gold() = static_cast<int>(value); }
			else if(key.compare("bank_gold") == 0){ player_ptr->bank_gold() = static_cast<int>(value); }
			else if(key.compare("exp") == 0){ player_ptr->exp() = static_cast<int>(value); }
			else if(key.compare("hitroll") == 0){ player_ptr->hitroll() = static_cast<sbyte>(value); }
			else if(key.compare("damroll") == 0){ player_ptr->damroll() = static_cast<sbyte>(value); }
			else if(key.compare("level") == 0){ player_ptr->level() = static_cast<byte>(value); }
			else if(key.compare("sex") == 0){ player_ptr->sex() = static_cast<byte>(value); }
			else if(key.compare("room") == 0){ player_ptr->room() = static_cast<room_rnum>(value); }
			else if(key.compare("uuid") == 0){ player_ptr->uuid() = static_cast<uuid_t>(value); }
__set_points_cleanup:
					duk_push_number(ctx,0);
					return 1;
		}


		bool run_test_suite(mods::player& player,std::string_view suite) {
			auto ctx = mods::js::new_context();
			std::string path = mods::js::current_working_dir() + "/js/tests/" + suite.data();
			mods::js::load_c_functions(ctx);

			if(mods::js::load_library(ctx,path) == -1) {
				player << "{red}[js]{/red} Unable to load library: '" << path.c_str() << "'\r\n";
				return false;
			}

			eval_string(ctx,std::string("test_main(") +
					std::string(mods::util::itoa(player.cd()->uuid)) + ");"
					);
				player << "{grn}[js]{/grn} Loaded and evaluated library: '" << path.c_str() << "'\r\n";
			return true;
		}

		void load_mods_player_functions(duk_context *ctx) {
			duk_push_c_function(ctx,mods::js::set_points,3);
			duk_put_global_string(ctx,"set_points");
			duk_push_c_function(ctx,mods::js::modify_affected_flags,3);
			duk_put_global_string(ctx,"modify_affected_flags");
			duk_push_c_function(ctx,mods::js::affect_from_char,3);
			duk_put_global_string(ctx,"affect_from_char");
			duk_push_c_function(ctx,mods::js::modify_plr_flags,3);
			duk_put_global_string(ctx,"modify_plr_flags");
			duk_push_c_function(ctx,mods::js::clear_all_plr_flags,1);
			duk_put_global_string(ctx,"clear_all_plr_flags");
			duk_push_c_function(ctx,mods::js::clear_all_affected_flags,1);
			duk_put_global_string(ctx,"clear_all_affected_flags");
			duk_push_c_function(ctx,mods::js::set_char_pk_id,2);
			duk_put_global_string(ctx,"set_char_pk_id");
		}

		void run_profile_scripts(const std::string& player_name){
			if(config::run_profile_scripts){
			 load_library(mods::globals::duktape_context,
					 std::string(mods::js::current_working_dir() + "/js/profiles/" + player_name + ".js").c_str());
			}
		}

		duk_context* new_context() {
			return duk_create_heap_default();
		}

		bool include::include_file() {
			std::string path = m_file;

			if(m_dir.length()) {
				path = m_dir + "/" + m_file;
			}
			if(access(path.c_str(),F_OK) == -1){
				/** File doesn't exist */
				return false;
			}

			std::ifstream include_file(path,std::ios::in);

			if(!include_file.is_open()) {
				return false;
			}
			if(!include_file.good()){
				return false;
			}
			std::vector<char> buffer;
			struct stat statbuf;

			if(stat(path.c_str(), &statbuf) == -1) {
				return false;
			}

			buffer.reserve(statbuf.st_size);
			std::fill(buffer.begin(),buffer.end(),0);

			unsigned buffer_length = statbuf.st_size;
			while(!include_file.eof() && buffer_length != 0) {
				include_file.read((char*)&buffer[0],statbuf.st_size);
				--buffer_length;
			}

			eval_string(m_context,std::string((char*)&buffer[0]));

			return true;
		}
		int load_library(duk_context *ctx,std::string_view file) {
			auto m = std::make_unique<include>(ctx,file.data());

			if(m->good()) {
				std::cerr << "[DEBUG]: included js file: '" << file.data() << "'\n";
				return 0;
			}

			std::cerr << "SYSERR: couldn't include js file! '" << file.data() << "'\n";
			return -1;
		}
		void load_c_functions(duk_context *ctx) {
			mods::js::load_base_functions(ctx);
			mods::quests::load_c_functions(ctx);
			mods::js::load_c_test_functions(ctx);
			mods::js::load_c_require_functions(ctx);
			mods::js::load_mods_player_functions(ctx);
		}
	};
};
