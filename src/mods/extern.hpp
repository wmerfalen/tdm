extern void do_auto_exits(struct char_data *ch);
extern void char_from_room(struct char_data*);
extern struct player_special_data dummy_mob;
extern void char_to_room(struct char_data*,room_rnum);
extern void perform_tell(struct char_data *ch, struct char_data *vict, char *arg);
extern size_t send_to_char(struct char_data *ch, const char *messg, ...);
extern void hit(struct char_data *ch, struct char_data *victim, int type);
extern struct char_data* character_list;
extern void command_interpreter(struct char_data* ch,char* argument);
extern void hit(struct char_data* ch,struct char_data* vict,int type);
extern std::vector<obj_data> object_list;
extern void do_auto_exits(struct char_data *ch);
extern std::vector<room_data> world;
extern int grenade_damage(struct char_data *ch, struct char_data *victim, int dam, int attacktype);
extern struct char_data* character_list;
