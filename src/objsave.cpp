/* ************************************************************************
*   File: objsave.c                                     Part of CircleMUD *
*  Usage: loading/saving player objects for rent and crash-save           *
*                                                                         *
*  All rights reserved.  See license.doc for complete information.        *
*                                                                         *
*  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
*  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
************************************************************************ */

#include "conf.h"
#include "sysdep.h"
#include "mods/util.hpp" //for operator"" _s


#include "structs.h"
#include "comm.h"
#include "handler.h"
#include "db.h"
#include "interpreter.h"
#include "utils.h"
#include "spells.h"

/* these factors should be unique integers */
#define RENT_FACTOR 	1
#define CRYO_FACTOR 	4

#define LOC_INVENTORY	0
#define MAX_BAG_ROWS	5

/* external variables */
extern struct player_index_element *player_table;
extern int top_of_p_table;
extern int rent_file_timeout, crash_file_timeout;
extern int free_rent;
extern int min_rent_cost;
extern int max_obj_save;	/* change in config.c */

/* Extern functions */
ACMD(do_action);
ACMD(do_tell);
SPECIAL(receptionist);
SPECIAL(cryogenicist);
int invalid_class(struct char_data *ch, struct obj_data *obj);

/* local functions */
void Crash_extract_norent_eq(struct char_data *ch);
void auto_equip(struct char_data *ch, struct obj_data *obj, int location);
int Crash_offer_rent(struct char_data *ch, struct char_data *recep, int display, int factor);
int Crash_report_unrentables(struct char_data *ch, struct char_data *recep, struct obj_data *obj);
void Crash_report_rent(struct char_data *ch, struct char_data *recep, struct obj_data *obj, long *cost, long *nitems, int display, int factor);
struct obj_data *Obj_from_store(struct obj_file_elem object, int *location);
int Obj_to_store(struct obj_data *obj, FILE *fl, int location);
void update_obj_file(void);
int Crash_write_rentcode(struct char_data *ch, FILE *fl, struct rent_info *rent);
int gen_receptionist(struct char_data *ch, struct char_data *recep, int cmd, char *arg, int mode);
int Crash_save(struct obj_data *obj, FILE *fp, int location);
void Crash_rent_deadline(struct char_data *ch, struct char_data *recep, long cost);
void Crash_restore_weight(struct obj_data *obj);
void Crash_extract_objs(struct obj_data *obj);
int Crash_is_unrentable(struct obj_data *obj);
void Crash_extract_norents(struct obj_data *obj);
void Crash_extract_expensive(struct obj_data *obj);
void Crash_calculate_rent(struct obj_data *obj, int *cost);
void Crash_rentsave(struct char_data *ch, int cost);
void Crash_cryosave(struct char_data *ch, int cost);


struct obj_data *Obj_from_store(struct obj_file_elem object, int *location) {
	struct obj_data *obj;
	obj_rnum itemnum;
	int j;

	*location = 0;

	if((itemnum = real_object(object.item_number)) == NOTHING) {
		return (NULL);
	}

	obj = read_object(itemnum, REAL);
#if USE_AUTOEQ
	*location = object.location;
#endif
	GET_OBJ_VAL(obj, 0) = object.value[0];
	GET_OBJ_VAL(obj, 1) = object.value[1];
	GET_OBJ_VAL(obj, 2) = object.value[2];
	GET_OBJ_VAL(obj, 3) = object.value[3];
	GET_OBJ_EXTRA(obj) = object.extra_flags;
	GET_OBJ_WEIGHT(obj) = object.weight;
	GET_OBJ_TIMER(obj) = object.timer;
	GET_OBJ_AFFECT(obj) = object.bitvector;

	for(j = 0; j < MAX_OBJ_AFFECT; j++) {
		obj->affected[j] = object.affected[j];
	}

	return (obj);
}



int Obj_to_store(struct obj_data *obj, FILE *fl, int location) {
	int j;
	struct obj_file_elem object;

	object.item_number = GET_OBJ_VNUM(obj);
#if USE_AUTOEQ
	object.location = location;
#endif
	object.value[0] = GET_OBJ_VAL(obj, 0);
	object.value[1] = GET_OBJ_VAL(obj, 1);
	object.value[2] = GET_OBJ_VAL(obj, 2);
	object.value[3] = GET_OBJ_VAL(obj, 3);
	object.extra_flags = GET_OBJ_EXTRA(obj);
	object.weight = GET_OBJ_WEIGHT(obj);
	object.timer = GET_OBJ_TIMER(obj);
	object.bitvector = GET_OBJ_AFFECT(obj);

	for(j = 0; j < MAX_OBJ_AFFECT; j++) {
		object.affected[j] = obj->affected[j];
	}

	if(fwrite(&object, sizeof(struct obj_file_elem), 1, fl) < 1) {
		perror("SYSERR: error writing object in Obj_to_store");
		return (0);
	}

	return (1);
}

/*
 * AutoEQ by Burkhard Knopf <burkhard.knopf@informatik.tu-clausthal.de>
 */
void auto_equip(struct char_data *ch, struct obj_data *obj, int location) {
	int j;

	/* Lots of checks... */
	if(location > 0) {	/* Was wearing it. */
		switch(j = (location - 1)) {
			case WEAR_LIGHT:
				break;

			case WEAR_FINGER_R:
			case WEAR_FINGER_L:
				if(!CAN_WEAR(obj, ITEM_WEAR_FINGER)) { /* not fitting :( */
					location = LOC_INVENTORY;
				}

				break;

			case WEAR_NECK_1:
			case WEAR_NECK_2:
				if(!CAN_WEAR(obj, ITEM_WEAR_NECK)) {
					location = LOC_INVENTORY;
				}

				break;

			case WEAR_BODY:
				if(!CAN_WEAR(obj, ITEM_WEAR_BODY)) {
					location = LOC_INVENTORY;
				}

				break;

			case WEAR_HEAD:
				if(!CAN_WEAR(obj, ITEM_WEAR_HEAD)) {
					location = LOC_INVENTORY;
				}

				break;

			case WEAR_LEGS:
				if(!CAN_WEAR(obj, ITEM_WEAR_LEGS)) {
					location = LOC_INVENTORY;
				}

				break;

			case WEAR_FEET:
				if(!CAN_WEAR(obj, ITEM_WEAR_FEET)) {
					location = LOC_INVENTORY;
				}

				break;

			case WEAR_HANDS:
				if(!CAN_WEAR(obj, ITEM_WEAR_HANDS)) {
					location = LOC_INVENTORY;
				}

				break;

			case WEAR_ARMS:
				if(!CAN_WEAR(obj, ITEM_WEAR_ARMS)) {
					location = LOC_INVENTORY;
				}

				break;

			case WEAR_SHIELD:
				if(!CAN_WEAR(obj, ITEM_WEAR_SHIELD)) {
					location = LOC_INVENTORY;
				}

				break;

			case WEAR_ABOUT:
				if(!CAN_WEAR(obj, ITEM_WEAR_ABOUT)) {
					location = LOC_INVENTORY;
				}

				break;

			case WEAR_WAIST:
				if(!CAN_WEAR(obj, ITEM_WEAR_WAIST)) {
					location = LOC_INVENTORY;
				}

				break;

			case WEAR_WRIST_R:
			case WEAR_WRIST_L:
				if(!CAN_WEAR(obj, ITEM_WEAR_WRIST)) {
					location = LOC_INVENTORY;
				}

				break;

			case WEAR_WIELD:
				if(!CAN_WEAR(obj, ITEM_WEAR_WIELD)) {
					location = LOC_INVENTORY;
				}

				break;

			case WEAR_HOLD:
				if(CAN_WEAR(obj, ITEM_WEAR_HOLD)) {
					break;
				}

				if(IS_WARRIOR(ch) && CAN_WEAR(obj, ITEM_WEAR_WIELD) && GET_OBJ_TYPE(obj) == ITEM_WEAPON) {
					break;
				}

				location = LOC_INVENTORY;
				break;

			default:
				location = LOC_INVENTORY;
		}

		if(location > 0) {	     /* Wearable. */
			if(!GET_EQ(ch,j)) {
				/*
				 * Check the characters's alignment to prevent them from being
				 * zapped through the auto-equipping.
				     */
				if(invalid_align(ch, obj) || invalid_class(ch, obj)) {
					location = LOC_INVENTORY;
				} else {
					equip_char(ch, obj, j);
				}
			} else {	/* Oops, saved a player with double equipment? */
				mudlog(BRF, LVL_IMMORT, TRUE, "SYSERR: autoeq: '%s' already equipped in position %d.", GET_NAME(ch).c_str(), location);
				location = LOC_INVENTORY;
			}
		}
	}

	if(location <= 0) {	/* Inventory */
		obj_to_char(obj, ch);
	}
}


int Crash_delete_file(char *name) {
	char filename[50];
	FILE *fl;

	if(!get_filename(filename, sizeof(filename), CRASH_FILE, name)) {
		return (0);
	}

	if(!(fl = fopen(filename, "rb"))) {
		if(errno != ENOENT) {	/* if it fails but NOT because of no file */
			log("SYSERR: deleting crash file %s (1): %s", filename, strerror(errno));
		}

		return (0);
	}

	fclose(fl);

	/* if it fails, NOT because of no file */
	if(remove(filename) < 0 && errno != ENOENT) {
		log("SYSERR: deleting crash file %s (2): %s", filename, strerror(errno));
	}

	return (1);
}


int Crash_delete_crashfile(struct char_data *ch) {
	char filename[MAX_INPUT_LENGTH];
	struct rent_info rent;
	int numread;
	FILE *fl;

	if(!get_filename(filename, sizeof(filename), CRASH_FILE, GET_NAME(ch))) {
		return (0);
	}

	if(!(fl = fopen(filename, "rb"))) {
		if(errno != ENOENT) {	/* if it fails, NOT because of no file */
			log("SYSERR: checking for crash file %s (3): %s", filename, strerror(errno));
		}

		return (0);
	}

	numread = fread(&rent, sizeof(struct rent_info), 1, fl);
	fclose(fl);

	if(numread == 0) {
		return (0);
	}

	if(rent.rentcode == RENT_CRASH) {
		Crash_delete_file(GET_NAME(ch).ptr());
	}

	return (1);
}


int Crash_clean_file(char *name) {
	return (0);
}


void update_obj_file(void) {
}


void Crash_listrent(struct char_data *ch, char *name) {
}


int Crash_write_rentcode(struct char_data *ch, FILE *fl, struct rent_info *rent) {
	return 1;
}


/*
 * Return values:
 *  0 - successful load, keep char in rent room.
 *  1 - load failure or load of crash items -- put char in temple.
 *  2 - rented equipment lost (no $)
 */
int Crash_load(struct char_data *ch) {
	return 2;
}



int Crash_save(struct obj_data *obj, FILE *fp, int location) {
	return 0;
}


void Crash_restore_weight(struct obj_data *obj) {
}

/*
 * Get !RENT items from equipment to inventory and
 * extract !RENT out of worn containers.
 */
void Crash_extract_norent_eq(struct char_data *ch) {
}

void Crash_extract_objs(struct obj_data *obj) {
}


int Crash_is_unrentable(struct obj_data *obj) {
	return 0;
}


void Crash_extract_norents(struct obj_data *obj) {
}


void Crash_extract_expensive(struct obj_data *obj) {
}



void Crash_calculate_rent(struct obj_data *obj, int *cost) {
}


void Crash_crashsave(struct char_data *ch) {
}


void Crash_idlesave(struct char_data *ch) {
}


void Crash_rentsave(struct char_data *ch, int cost) {
}


void Crash_cryosave(struct char_data *ch, int cost) {
}


/* ************************************************************************
* Routines used for the receptionist					  *
************************************************************************* */

void Crash_rent_deadline(struct char_data *ch, struct char_data *recep,
                         long cost) {
}

int Crash_report_unrentables(struct char_data *ch, struct char_data *recep,
                             struct obj_data *obj) {
	return 0;
}



void Crash_report_rent(struct char_data *ch, struct char_data *recep,
                       struct obj_data *obj, long *cost, long *nitems, int display, int factor) {
}



int Crash_offer_rent(struct char_data *ch, struct char_data *recep,
                     int display, int factor) {
	return 0;
}



int gen_receptionist(struct char_data *ch, struct char_data *recep,
                     int cmd, char *arg, int mode) {
	return 0;
}


SPECIAL(receptionist) {
	return (gen_receptionist(ch, (struct char_data *)me, cmd, argument, RENT_FACTOR));
}


SPECIAL(cryogenicist) {
	return (gen_receptionist(ch, (struct char_data *)me, cmd, argument, CRYO_FACTOR));
}


void Crash_save_all(void) {
}
