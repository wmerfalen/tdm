#include "../shop.h"
#include "../room-data.hpp"
extern void do_auto_exits(char_data *ch);
extern void char_from_room(char_data*);
extern struct player_special_data dummy_mob;
extern void char_to_room(char_data*,room_rnum);
extern void perform_tell(char_data *ch, char_data *vict, char *arg);
extern size_t send_to_char(char_data *ch, const char *messg, ...);
extern void hit(char_data *ch, char_data *victim, int type);
extern char_data* character_list;
extern void command_interpreter(player_ptr_t&, std::string argument);
extern void hit(char_data* ch,char_data* vict,int type);
extern std::deque<obj_data> object_list;
extern std::deque<std::shared_ptr<obj_data>> obj_list;
extern void do_auto_exits(char_data *ch);
extern std::deque<room_data> world;
extern int grenade_damage(char_data *ch, char_data *victim, int dam, int attacktype);
extern char_data* character_list;
extern std::deque<char_data> mob_proto;
extern std::deque<obj_data> obj_proto;
extern void obj_to_room(struct obj_data *object, room_rnum room);
extern room_vnum mortal_start_room;
extern room_vnum immort_start_room;
extern room_vnum frozen_start_room;
extern std::deque<shop_data<mods::orm::shop,mods::orm::shop_rooms,mods::orm::shop_objects>> shop_index;
