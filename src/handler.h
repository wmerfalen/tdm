/* ************************************************************************
*   File: handler.h                                     Part of CircleMUD *
*  Usage: header file: prototypes of handling and utility functions       *
*                                                                         *
*  All rights reserved.  See license.doc for complete information.        *
*                                                                         *
*  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
*  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
************************************************************************ */

namespace mods {
	struct player;
};
#include "mods/player.hpp"
/* handling the affected-structures */
void	affect_total(char_data *ch);
void	affect_modify(char_data *ch, byte loc, sbyte mod, bitvector_t bitv, bool add);
void	affect_to_char(char_data *ch, struct affected_type *af);
void	affect_remove(char_data *ch, struct affected_type *af);
void	affect_from_char(char_data *ch, int type);
bool	affected_by_spell(char_data *ch, int type);
void	affect_join(char_data *ch, struct affected_type *af,
                    bool add_dur, bool avg_dur, bool add_mod, bool avg_mod);


/* utility */
const char *money_desc(int amount);
obj_ptr_t create_money(int amount);
int	isname(const char *str, const char *namelist);
char	*fname(const char *namelist);
int	get_number(char **name);

/* ******** objects *********** */

void obj_to_char(std::shared_ptr<obj_data> object, std::shared_ptr<mods::player> player);
void	obj_to_char(struct obj_data *object, char_data *ch);
void	obj_from_char(struct obj_data *object);
void	obj_from_char(obj_ptr_t object);

void	legacy_equip_char(char_data *ch, struct obj_data *obj, int pos);
void	equip_char(player_ptr_t player, obj_ptr_t obj, int pos);
obj_ptr_t unequip_char(player_ptr_t player, int pos);
int	invalid_align(char_data *ch, struct obj_data *obj);

void	obj_to_room(struct obj_data *object, room_rnum room);
void	obj_to_room(obj_ptr_t object, room_rnum room);
void	obj_from_room(struct obj_data *object);
void	obj_from_room(obj_ptr_t object);
void	obj_to_obj(obj_ptr_t from_object, obj_ptr_t to_object);
void	obj_from_obj(struct obj_data *obj);
void	object_list_new_owner(struct obj_data *list, char_data *ch);

void	extract_obj(struct obj_data *obj);

/* ******* characters ********* */

char_data *get_char_room(char *name, int *num, room_rnum room);
char_data *get_char_num(mob_rnum nr);

void	char_from_room(char_data *ch);
void	char_to_room(char_data *ch, room_rnum room);
static inline void	char_from_room(player_ptr_t player){
	char_from_room(player->cd());
}
static inline void	char_to_room(player_ptr_t player, room_rnum room) {
	char_to_room(player->cd(),room);
}
void	extract_char(char_data *ch);
void	extract_char_final(char_data *ch);
void	extract_pending_chars(void);

/* find if character can see */
char_data *get_player_vis(char_data *ch, char *name, int *number, int inroom);
char_data *get_char_vis(char_data *ch, char *name, int *number, int where);
player_ptr_t get_char_vis(player_ptr_t ch, char *name, int *number, int where);
char_data *get_char_room_vis(char_data *ch, char *name, int *number);
char_data *get_char_world_vis(char_data *ch, char *name, int *number);

struct obj_data *get_obj_in_list_num(int num, struct obj_data *list);
struct obj_data *get_obj_num(obj_rnum nr);
struct obj_data *get_obj_in_list_vis(char_data *ch, char *name, int *number, struct obj_data *list);
struct obj_data *get_obj_vis(char_data *ch, char *name, int *num);
struct obj_data *get_obj_in_equip_vis(char_data *ch, char *arg, int *number, struct obj_data *equipment[]);
int              get_obj_pos_in_equip_vis(char_data *ch, char *arg, int *num, struct obj_data *equipment[]);


/* find all dots */

int	find_all_dots(char *arg);

#define FIND_INDIV	0
#define FIND_ALL	1
#define FIND_ALLDOT	2


/* Generic Find */

int	generic_find(char *arg, bitvector_t bitvector, char_data *ch,
                 char_data **tar_ch, struct obj_data **tar_obj);

#define FIND_CHAR_ROOM     (1 << 0)
#define FIND_CHAR_WORLD    (1 << 1)
#define FIND_OBJ_INV       (1 << 2)
#define FIND_OBJ_ROOM      (1 << 3)
#define FIND_OBJ_WORLD     (1 << 4)
#define FIND_OBJ_EQUIP     (1 << 5)


/* prototypes from crash save system */

int	Crash_delete_file(char *name);
int	Crash_delete_crashfile(char_data *ch);
int	Crash_clean_file(char *name);
void	Crash_listrent(char_data *ch, char *name);
int	Crash_load(char_data *ch);
void	Crash_crashsave(char_data *ch);
void	Crash_idlesave(char_data *ch);
void	Crash_save_all(void);

/* prototypes from fight.c */
void	set_fighting(char_data *ch, char_data *victim);
void	stop_fighting(char_data *ch);
void	hit(char_data *ch, char_data *victim, int type);
void	forget(char_data *ch, char_data *victim);
void	remember(char_data *ch, char_data *victim);
int	damage(char_data *ch, char_data *victim, int dam, int attacktype);
int	skill_message(int dam, char_data *ch, char_data *vict,
                  int attacktype);
