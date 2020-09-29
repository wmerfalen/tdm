/* ************************************************************************
*   File: shop.c                                        Part of CircleMUD *
*  Usage: shopkeepers: loading config files, spec procs.                  *
*                                                                         *
*  All rights reserved.  See license.doc for complete information.        *
*                                                                         *
*  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
*  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
************************************************************************ */

/***
 * The entire shop rewrite for Circle 3.0 was done by Jeff Fink.  Thanks Jeff!
 ***/

#include "conf.h"
#include "sysdep.h"

#include "structs.h"
#include "comm.h"
#include "handler.h"
#include "db.h"
#include "interpreter.h"
#include "utils.h"
#include "shop.h"
#include "constants.h"
#include "mods/orm/shop.hpp"
#include "mods/shop.hpp"

/* External variables */
extern struct time_info_data time_info;

/* Forward/External function declarations */
ACMD(do_tell);
ACMD(do_action);
ACMD(do_echo);
ACMD(do_say);
void sort_keeper_objs(char_data *keeper, int shop_nr);

using shop_data_t = shop_data<mods::orm::shop,mods::orm::shop_rooms,mods::orm::shop_objects>;
/* Local variables */
std::deque<shop_data_t> shop_index;
int top_shop = -1;
int cmd_say, cmd_tell, cmd_emote, cmd_slap, cmd_puke;

namespace mods::globals {
	std::map<room_rnum,std::shared_ptr<shop_data_t>> room_shopmap;
};

/* local functions */
char *read_shop_message(int mnum, room_vnum shr, FILE *shop_f, const char *why);
int read_type_list(FILE *shop_f, struct shop_buy_data *list, int new_format, int max);
int read_list(FILE *shop_f, struct shop_buy_data *list, int new_format, int max, int type);
void shopping_list(char *arg, char_data *ch, char_data *keeper, int shop_nr);
void shopping_value(char *arg, char_data *ch, char_data *keeper, int shop_nr);
void shopping_sell(char *arg, char_data *ch, char_data *keeper, int shop_nr);
struct obj_data *get_selling_obj(char_data *ch, char *name, char_data *keeper, int shop_nr, int msg);
struct obj_data *slide_obj(struct obj_data *obj, char_data *keeper, int shop_nr);
void shopping_buy(char *arg, char_data *ch, char_data *keeper, int shop_nr);
struct obj_data *get_purchase_obj(char_data *ch, char *arg, char_data *keeper, int shop_nr, int msg);
struct obj_data *get_hash_obj_vis(char_data *ch, char *name, struct obj_data *list);
struct obj_data *get_slide_obj_vis(char_data *ch, char *name, struct obj_data *list);
void boot_the_shops(FILE *shop_f, char *filename, int rec_count);
void assign_the_shopkeepers(void);
char *customer_string(int shop_nr, int detailed);
void list_all_shops(char_data *ch);
void list_detailed_shop(char_data *ch, int shop_nr);
void show_shops(char_data *ch, char *arg);
int is_ok_char(char_data *keeper, char_data *ch, int shop_nr);
int is_open(char_data *keeper, int shop_nr, int msg);
int is_ok(char_data *keeper, char_data *ch, int shop_nr);
void push(struct stack_data *stack, int pushval);
int top(struct stack_data *stack);
int pop(struct stack_data *stack);
void evaluate_operation(struct stack_data *ops, struct stack_data *vals);
int find_oper_num(char token);
int evaluate_expression(struct obj_data *obj, char *expr);
int trade_with(struct obj_data *item, int shop_nr);
int same_obj(struct obj_data *obj1, struct obj_data *obj2);
int shop_producing(struct obj_data *item, int shop_nr);
int transaction_amt(char *arg);
char *times_message(struct obj_data *obj, char *name, int num);
int buy_price(struct obj_data *obj, int shop_nr, char_data *keeper, char_data *buyer);
int sell_price(struct obj_data *obj, int shop_nr, char_data *keeper, char_data *seller);
char *list_object(struct obj_data *obj, int cnt, int oindex, int shop_nr, char_data *keeper, char_data *seller);
int ok_shop_room(int shop_nr, room_vnum room);
SPECIAL(shop_keeper);
int ok_damage_shopkeeper(char_data *ch, char_data *victim);
int add_to_list(struct shop_buy_data *list, int type, int *len, int *val);
int end_read_list(struct shop_buy_data *list, int len, int error);
void read_line(FILE *shop_f, const char *string, void *data);
void destroy_shops(void);


/* config arrays */
const char *operator_str[] = {
	"[({",
	"])}",
	"|+",
	"&*",
	"^'"
} ;

/* Constant list for printing out who we sell to */
const char *trade_letters[] = {
	"Good",                 /* First, the alignment based ones */
	"Evil",
	"Neutral",
	"Magic User",           /* Then the class based ones */
	"Cleric",
	"Thief",
	"Warrior",
	"\n"
};


const char *shop_bits[] = {
	"WILL_FIGHT",
	"USES_BANK",
	"\n"
};

int is_ok_char(char_data *keeper, char_data *ch, int shop_nr) {
	MENTOC_PREAMBLE();
	char buf[MAX_INPUT_LENGTH];

	if(!CAN_SEE(keeper, ch)) {
		char actbuf[MAX_INPUT_LENGTH] = MSG_NO_SEE_CHAR;
		do_say(keeper, actbuf, cmd_say, 0,player);
		return (FALSE);
	}

	if(IS_GOD(ch)) {
		return (TRUE);
	}

	if((IS_GOOD(ch) && NOTRADE_GOOD(shop_nr)) ||
	        (IS_EVIL(ch) && NOTRADE_EVIL(shop_nr)) ||
	        (IS_NEUTRAL(ch) && NOTRADE_NEUTRAL(shop_nr))) {
		snprintf(buf, sizeof(buf), "%s %s", GET_NAME(ch).c_str(), MSG_NO_SELL_ALIGN);
		do_tell(keeper, buf, cmd_tell, 0,player);
		return (FALSE);
	}

	if(IS_NPC(ch)) {
		return (TRUE);
	}

	if((IS_MAGIC_USER(ch) && NOTRADE_MAGIC_USER(shop_nr)) ||
	        (IS_CLERIC(ch) && NOTRADE_CLERIC(shop_nr)) ||
	        (IS_THIEF(ch) && NOTRADE_THIEF(shop_nr)) ||
	        (IS_WARRIOR(ch) && NOTRADE_WARRIOR(shop_nr))) {
		snprintf(buf, sizeof(buf), "%s %s", GET_NAME(ch).c_str(), MSG_NO_SELL_CLASS);
		do_tell(keeper, buf, cmd_tell, 0,player);
		return (FALSE);
	}

	return (TRUE);
}


int is_open(char_data *keeper, int shop_nr, int msg) {
	char buf[MAX_INPUT_LENGTH];

	*buf = '\0';

	if(SHOP_OPEN1(shop_nr) > time_info.hours) {
		strlcpy(buf, MSG_NOT_OPEN_YET, sizeof(buf));
	} else if(SHOP_CLOSE1(shop_nr) < time_info.hours) {
		if(SHOP_OPEN2(shop_nr) > time_info.hours) {
			strlcpy(buf, MSG_NOT_REOPEN_YET, sizeof(buf));
		} else if(SHOP_CLOSE2(shop_nr) < time_info.hours) {
			strlcpy(buf, MSG_CLOSED_FOR_DAY, sizeof(buf));
		}
	}

	if(!*buf) {
		return (TRUE);
	}

	if(msg) {
		auto player = ptr(keeper);
		do_say(keeper, buf, cmd_tell, 0,player);
	}

	return (FALSE);
}


int is_ok(char_data *keeper, char_data *ch, int shop_nr) {
	if(is_open(keeper, shop_nr, TRUE)) {
		return (is_ok_char(keeper, ch, shop_nr));
	} else {
		return (FALSE);
	}
}


void push(struct stack_data *stack, int pushval) {
	S_DATA(stack, S_LEN(stack)++) = pushval;
}


int top(struct stack_data *stack) {
	if(S_LEN(stack) > 0) {
		return (S_DATA(stack, S_LEN(stack) - 1));
	} else {
		return (NOTHING);
	}
}


int pop(struct stack_data *stack) {
	if(S_LEN(stack) > 0) {
		return (S_DATA(stack, --S_LEN(stack)));
	} else {
		log("SYSERR: Illegal expression %d in shop keyword list.", S_LEN(stack));
		return (0);
	}
}


void evaluate_operation(struct stack_data *ops, struct stack_data *vals) {
	int oper;

	if((oper = pop(ops)) == OPER_NOT) {
		push(vals, !pop(vals));
	} else {
		int val1 = pop(vals),
		    val2 = pop(vals);

		/* Compiler would previously short-circuit these. */
		if(oper == OPER_AND) {
			push(vals, val1 && val2);
		} else if(oper == OPER_OR) {
			push(vals, val1 || val2);
		}
	}
}


int find_oper_num(char token) {
	int oindex;

	for(oindex = 0; oindex <= MAX_OPER; oindex++)
		if(strchr(operator_str[oindex], token)) {
			return (oindex);
		}

	return (NOTHING);
}


int evaluate_expression(struct obj_data *obj, char *expr) {
	struct stack_data ops, vals;
	char *ptr, *end, name[MAX_STRING_LENGTH];
	int temp, eindex;

	if(!expr || !*expr) {	/* Allows opening ( first. */
		return (TRUE);
	}

	ops.len = vals.len = 0;
	ptr = expr;

	while(*ptr) {
		if(isspace(*ptr)) {
			ptr++;
		} else {
			if((temp = find_oper_num(*ptr)) == NOTHING) {
				end = ptr;

				while(*ptr && !isspace(*ptr) && find_oper_num(*ptr) == NOTHING) {
					ptr++;
				}

				strncpy(name, end, ptr - end);	/* strncpy: OK (name/end:MAX_STRING_LENGTH) */
				name[ptr - end] = '\0';

				for(eindex = 0; *extra_bits[eindex] != '\n'; eindex++)
					if(!str_cmp(name, extra_bits[eindex])) {
						push(&vals, OBJ_FLAGGED(obj, 1 << eindex));
						break;
					}

				if(*extra_bits[eindex] == '\n') {
					push(&vals, isname(name, obj->name));
				}
			} else {
				if(temp != OPER_OPEN_PAREN)
					while(top(&ops) > temp) {
						evaluate_operation(&ops, &vals);
					}

				if(temp == OPER_CLOSE_PAREN) {
					if((temp = pop(&ops)) != OPER_OPEN_PAREN) {
						log("SYSERR: Illegal parenthesis in shop keyword expression.");
						return (FALSE);
					}
				} else {
					push(&ops, temp);
				}

				ptr++;
			}
		}
	}

	while(top(&ops) != NOTHING) {
		evaluate_operation(&ops, &vals);
	}

	temp = pop(&vals);

	if(top(&vals) != NOTHING) {
		log("SYSERR: Extra operands left on shop keyword expression stack.");
		return (FALSE);
	}

	return (temp);
}


int trade_with(struct obj_data *item, int shop_nr) {
	int counter;

	if(GET_OBJ_COST(item) < 1) {
		return (OBJECT_NOVAL);
	}

	if(OBJ_FLAGGED(item, ITEM_NOSELL)) {
		return (OBJECT_NOTOK);
	}

	for(counter = 0; SHOP_BUYTYPE(shop_nr, counter) != NOTHING; counter++)
		if(SHOP_BUYTYPE(shop_nr, counter) == GET_OBJ_TYPE(item)) {
			if(GET_OBJ_VAL(item, 2) == 0 &&
			        (GET_OBJ_TYPE(item) == ITEM_WAND ||
			         GET_OBJ_TYPE(item) == ITEM_STAFF)) {
				return (OBJECT_DEAD);
			} else if(evaluate_expression(item, SHOP_BUYWORD(shop_nr, counter))) {
				return (OBJECT_OK);
			}
		}

	return (OBJECT_NOTOK);
}


int same_obj(struct obj_data *obj1, struct obj_data *obj2) {
	int aindex;

	if(!obj1 || !obj2) {
		return (obj1 == obj2);
	}

	if(GET_OBJ_RNUM(obj1) != GET_OBJ_RNUM(obj2)) {
		return (FALSE);
	}

	if(GET_OBJ_COST(obj1) != GET_OBJ_COST(obj2)) {
		return (FALSE);
	}

	if(GET_OBJ_EXTRA(obj1) != GET_OBJ_EXTRA(obj2)) {
		return (FALSE);
	}

	for(aindex = 0; aindex < MAX_OBJ_AFFECT; aindex++)
		if((obj1->affected[aindex].location != obj2->affected[aindex].location) ||
		        (obj1->affected[aindex].modifier != obj2->affected[aindex].modifier)) {
			return (FALSE);
		}

	return (TRUE);
}


int shop_producing(struct obj_data *item, int shop_nr) {
	int counter;

	if(GET_OBJ_RNUM(item) == NOTHING) {
		return (FALSE);
	}

	for(counter = 0; SHOP_PRODUCT(shop_nr, counter) != NOTHING; counter++)
		if(same_obj(item, &obj_proto[SHOP_PRODUCT(shop_nr, counter)])) {
			return (TRUE);
		}

	return (FALSE);
}


int transaction_amt(char *arg) {
	char buf[MAX_INPUT_LENGTH];

	char *buywhat;

	/*
	 * If we have two arguments, it means 'buy 5 3', or buy 5 of #3.
	 * We don't do that if we only have one argument, like 'buy 5', buy #5.
	 * Code from Andrey Fidrya <andrey@ALEX-UA.COM>
	 */
	buywhat = one_argument(arg, buf);

	if(*buywhat && *buf && is_number(buf)) {
		strcpy(arg, arg + strlen(buf) + 1);	/* strcpy: OK (always smaller) */
		return (atoi(buf));
	}

	return (1);
}


char *times_message(struct obj_data *obj, char *name, int num) {
	static char buf[256];
	size_t len;
	char *ptr;

	if(obj) {
		len = strlcpy(buf, obj->short_description, sizeof(buf));
	} else {
		if((ptr = strchr(name, '.')) == NULL) {
			ptr = name;
		} else {
			ptr++;
		}

		len = snprintf(buf, sizeof(buf), "%s %s", AN(ptr), ptr);
	}

	if(num > 1 && len < sizeof(buf)) {
		snprintf(buf + len, sizeof(buf) - len, " (x %d)", num);
	}

	return (buf);
}


struct obj_data *get_slide_obj_vis(char_data *ch, char *name,
                                   struct obj_data *list) {
	struct obj_data *i, *last_match = NULL;
	int j, number;
	char tmpname[MAX_INPUT_LENGTH];
	char *tmp;

	strlcpy(tmpname, name, sizeof(tmpname));
	tmp = tmpname;

	if(!(number = get_number(&tmp))) {
		return (NULL);
	}

	for(i = list, j = 1; i && (j <= number); i = i->next_content)
		if(isname(tmp, i->name))
			if(CAN_SEE_OBJ(ch, i) && !same_obj(last_match, i)) {
				if(j == number) {
					return (i);
				}

				last_match = i;
				j++;
			}

	return (NULL);
}


struct obj_data *get_hash_obj_vis(char_data *ch, char *name,
                                  struct obj_data *list) {
	struct obj_data *loop, *last_obj = NULL;
	int qindex;

	if(is_number(name)) {
		qindex = atoi(name);
	} else if(is_number(name + 1)) {
		qindex = atoi(name + 1);
	} else {
		return (NULL);
	}

	for(loop = list; loop; loop = loop->next_content){
		if(CAN_SEE_OBJ(ch, loop) && GET_OBJ_COST(loop) > 0){
			if(!same_obj(last_obj, loop)) {
				if(--qindex == 0) {
					return (loop);
				}

				last_obj = loop;
			}
		}
	}

	return (NULL);
}


struct obj_data *get_purchase_obj(char_data *ch, char *arg,
                                  char_data *keeper, int shop_nr, int msg) {
	MENTOC_PREAMBLE();
	char name[MAX_INPUT_LENGTH];
	struct obj_data *obj;

	one_argument(arg, name);

	do {
		if(*name == '#' || is_number(name)) {
			obj = get_hash_obj_vis(ch, name, keeper->carrying);
		} else {
			obj = get_slide_obj_vis(ch, name, keeper->carrying);
		}

		if(!obj) {
			if(msg) {
				char buf[MAX_INPUT_LENGTH];

				snprintf(buf, sizeof(buf), shop_index[shop_nr].no_such_item1, GET_NAME(ch).c_str());
				do_tell(keeper, buf, cmd_tell, 0,player);
			}

			return (NULL);
		}

		if(GET_OBJ_COST(obj) <= 0) {
			extract_obj(obj);
			obj = NULL;
		}
	} while(!obj);

	return (obj);
}


/*
 * Shop purchase adjustment, based on charisma-difference from buyer to keeper.
 *
 * for i in `seq 15 -15`; do printf " * %3d: %6.4f\n" $i \
 * `echo "scale=4; 1+$i/70" | bc`; done
 *
 *  Shopkeeper higher charisma (markup)
 *  ^  15: 1.2142  14: 1.2000  13: 1.1857  12: 1.1714  11: 1.1571
 *  |  10: 1.1428   9: 1.1285   8: 1.1142   7: 1.1000   6: 1.0857
 *  |   5: 1.0714   4: 1.0571   3: 1.0428   2: 1.0285   1: 1.0142
 *  +   0: 1.0000
 *  |  -1: 0.9858  -2: 0.9715  -3: 0.9572  -4: 0.9429  -5: 0.9286
 *  |  -6: 0.9143  -7: 0.9000  -8: 0.8858  -9: 0.8715 -10: 0.8572
 *  v -11: 0.8429 -12: 0.8286 -13: 0.8143 -14: 0.8000 -15: 0.7858
 *  Player higher charisma (discount)
 *
 * Most mobiles have 11 charisma so an 18 charisma player would get a 10%
 * discount beyond the basic price.  That assumes they put a lot of points
 * into charisma, because on the flip side they'd get 11% inflation by
 * having a 3.
 */
int buy_price(struct obj_data *obj, int shop_nr, char_data *keeper, char_data *buyer) {
	return (int)(GET_OBJ_COST(obj) * SHOP_BUYPROFIT(shop_nr)
	             * (1 + (GET_CHA(keeper) - GET_CHA(buyer)) / (float)70));
}

/*
 * When the shopkeeper is buying, we reverse the discount. Also make sure
 * we don't buy for more than we sell for, to prevent infinite money-making.
 */
int sell_price(struct obj_data *obj, int shop_nr, char_data *keeper, char_data *seller) {
	float sell_cost_modifier = SHOP_SELLPROFIT(shop_nr) * (1 - (GET_CHA(keeper) - GET_CHA(seller)) / (float)70);
	float buy_cost_modifier = SHOP_BUYPROFIT(shop_nr) * (1 + (GET_CHA(keeper) - GET_CHA(seller)) / (float)70);

	if(sell_cost_modifier > buy_cost_modifier) {
		sell_cost_modifier = buy_cost_modifier;
	}

	return (int)(GET_OBJ_COST(obj) * sell_cost_modifier);
}


void shopping_buy(char *arg, char_data *ch, char_data *keeper, int shop_nr) {
	log("DEPRECATED: shopping_buy. Reason: vuln+cba");
	return;
#if 0
	char tempstr[MAX_INPUT_LENGTH], tempbuf[MAX_INPUT_LENGTH];
	struct obj_data *obj, *last_obj = NULL;
	int goldamt = 0, buynum, bought = 0;

	if(!is_ok(keeper, ch, shop_nr)) {
		return;
	}

	if(SHOP_SORT(shop_nr) < IS_CARRYING_N(keeper)) {
		sort_keeper_objs(keeper, shop_nr);
	}

	if((buynum = transaction_amt(arg)) < 0) {
		char buf[MAX_INPUT_LENGTH];

		snprintf(buf, sizeof(buf), "%s A negative amount?  Try selling me something.",
		         GET_NAME(ch).c_str());
		do_tell(keeper, buf, cmd_tell, 0);
		return;
	}

	if(!*arg || !buynum) {
		char buf[MAX_INPUT_LENGTH];

		snprintf(buf, sizeof(buf), "%s What do you want to buy??", GET_NAME(ch).c_str());
		do_tell(keeper, buf, cmd_tell, 0);
		return;
	}

	if(!(obj = get_purchase_obj(ch, arg, keeper, shop_nr, TRUE))) {
		return;
	}

	if(buy_price(obj, shop_nr, keeper, ch) > GET_GOLD(ch) && !IS_GOD(ch)) {
		char actbuf[MAX_INPUT_LENGTH];

		snprintf(actbuf, sizeof(actbuf), shop_index[shop_nr].missing_cash2, GET_NAME(ch).c_str());
		do_tell(keeper, actbuf, cmd_tell, 0);

		switch(SHOP_BROKE_TEMPER(shop_nr)) {
			case 0:
				do_action(keeper, strcpy(actbuf, GET_NAME(ch).c_str()), cmd_puke, 0);	/* strcpy: OK (MAX_NAME_LENGTH < MAX_INPUT_LENGTH) */
				return;

			case 1:
				do_echo(keeper, strcpy(actbuf, "smokes on his joint."), cmd_emote, SCMD_EMOTE);	/* strcpy: OK */
				return;

			default:
				return;
		}
	}

	if(IS_CARRYING_N(ch) + 1 > CAN_CARRY_N(ch)) {
		send_to_char(ch, "%s: You can't carry any more items.\r\n", fname(obj->name));
		return;
	}

	if(IS_CARRYING_W(ch) + GET_OBJ_WEIGHT(obj) > CAN_CARRY_W(ch)) {
		send_to_char(ch, "%s: You can't carry that much weight.\r\n", fname(obj->name));
		return;
	}

	while(obj && (GET_GOLD(ch) >= buy_price(obj, shop_nr, keeper, ch) || IS_GOD(ch))
	        && IS_CARRYING_N(ch) < CAN_CARRY_N(ch) && bought < buynum
	        && IS_CARRYING_W(ch) + GET_OBJ_WEIGHT(obj) <= CAN_CARRY_W(ch)) {
		int charged;

		bought++;

		/* Test if producing shop ! */
		if(shop_producing(obj, shop_nr)) {
			obj = read_object(GET_OBJ_RNUM(obj), REAL);
		} else {
			obj_from_char(obj);
			SHOP_SORT(shop_nr)--;
		}

		obj_to_char(obj, ch);

		charged = buy_price(obj, shop_nr, keeper, ch);
		goldamt += charged;

		if(!IS_GOD(ch)) {
			GET_GOLD(ch) -= charged;
		}

		last_obj = obj;
		obj = get_purchase_obj(ch, arg, keeper, shop_nr, FALSE);

		if(!same_obj(obj, last_obj)) {
			break;
		}
	}

	if(bought < buynum) {
		char buf[MAX_INPUT_LENGTH];

		if(!obj || !same_obj(last_obj, obj)) {
			snprintf(buf, sizeof(buf), "%s I only have %d to sell you.", GET_NAME(ch).c_str(), bought);
		} else if(GET_GOLD(ch) < buy_price(obj, shop_nr, keeper, ch)) {
			snprintf(buf, sizeof(buf), "%s You can only afford %d.", GET_NAME(ch).c_str(), bought);
		} else if(IS_CARRYING_N(ch) >= CAN_CARRY_N(ch)) {
			snprintf(buf, sizeof(buf), "%s You can only hold %d.", GET_NAME(ch).c_str(), bought);
		} else if(IS_CARRYING_W(ch) + GET_OBJ_WEIGHT(obj) > CAN_CARRY_W(ch)) {
			snprintf(buf, sizeof(buf), "%s You can only carry %d.", GET_NAME(ch).c_str(), bought);
		} else {
			snprintf(buf, sizeof(buf), "%s Something screwy only gave you %d.", GET_NAME(ch).c_str(), bought);
		}

		do_tell(keeper, buf, cmd_tell, 0);
	}

	if(!IS_GOD(ch)) {
		GET_GOLD(keeper) += goldamt;
	}

	strlcpy(tempstr, times_message(ch->carrying, 0, bought), sizeof(tempstr));

	snprintf(tempbuf, sizeof(tempbuf), "$n buys %s.", tempstr);
	act(tempbuf, FALSE, ch, obj, 0, TO_ROOM);

	snprintf(tempbuf, sizeof(tempbuf), shop_index[shop_nr].message_buy, GET_NAME(ch).c_str(), goldamt);
	do_tell(keeper, tempbuf, cmd_tell, 0);

	send_to_char(ch, "You now have %s.\r\n", tempstr);

	if(SHOP_USES_BANK(shop_nr))
		if(GET_GOLD(keeper) > MAX_OUTSIDE_BANK) {
			SHOP_BANK(shop_nr) += (GET_GOLD(keeper) - MAX_OUTSIDE_BANK);
			GET_GOLD(keeper) = MAX_OUTSIDE_BANK;
		}
#endif
}


struct obj_data *get_selling_obj(char_data *ch, char *name, char_data *keeper, int shop_nr, int msg) {
			auto player = ptr(ch);
	char buf[MAX_INPUT_LENGTH];
	struct obj_data *obj;
	int result;

	if(!(obj = get_obj_in_list_vis(ch, name, NULL, ch->carrying))) {
		if(msg) {
			char tbuf[MAX_INPUT_LENGTH];

			snprintf(tbuf, sizeof(tbuf), shop_index[shop_nr].no_such_item2, GET_NAME(ch).c_str());
			do_tell(keeper, tbuf, cmd_tell, 0,player);
		}

		return (NULL);
	}

	if((result = trade_with(obj, shop_nr)) == OBJECT_OK) {
		return (obj);
	}

	if(!msg) {
		return (0);
	}

	switch(result) {
		case OBJECT_NOVAL:
			snprintf(buf, sizeof(buf), "%s You've got to be kidding, that thing is worthless!", GET_NAME(ch).c_str());
			break;

		case OBJECT_NOTOK:
			snprintf(buf, sizeof(buf), shop_index[shop_nr].do_not_buy, GET_NAME(ch).c_str());
			break;

		case OBJECT_DEAD:
			snprintf(buf, sizeof(buf), "%s %s", GET_NAME(ch).c_str(), MSG_NO_USED_WANDSTAFF);
			break;

		default:
			log("SYSERR: Illegal return value of %d from trade_with() (%s)", result, __FILE__);	/* Someone might rename it... */
			snprintf(buf, sizeof(buf), "%s An error has occurred.", GET_NAME(ch).c_str());
			break;
	}

	do_tell(keeper, buf, cmd_tell, 0,player);
	return (NULL);
}


struct obj_data *slide_obj(struct obj_data *obj, char_data *keeper,
                           int shop_nr)
/*
   This function is a slight hack!  To make sure that duplicate items are
   only listed once on the "list", this function groups "identical"
   objects together on the shopkeeper's inventory list.  The hack involves
   knowing how the list is put together, and manipulating the order of
   the objects on the list.  (But since most of DIKU is not encapsulated,
   and information hiding is almost never used, it isn't that big a deal) -JF
*/
{
	struct obj_data *loop;
	int temp;

	if(SHOP_SORT(shop_nr) < IS_CARRYING_N(keeper)) {
		sort_keeper_objs(keeper, shop_nr);
	}

	/* Extract the object if it is identical to one produced */
	if(shop_producing(obj, shop_nr)) {
		temp = GET_OBJ_RNUM(obj);
		extract_obj(obj);
		return (&obj_proto[temp]);
	}

	SHOP_SORT(shop_nr)++;
	loop = keeper->carrying;
	obj_to_char(obj, keeper);
	keeper->carrying = loop;

	while(loop) {
		if(same_obj(obj, loop)) {
			obj->next_content = loop->next_content;
			loop->next_content = obj;
			return (obj);
		}

		loop = loop->next_content;
	}

	keeper->carrying = obj;
	return (obj);
}


void sort_keeper_objs(char_data *keeper, int shop_nr) {
	struct obj_data *list = NULL, *temp;

	while(SHOP_SORT(shop_nr) < IS_CARRYING_N(keeper)) {
		temp = keeper->carrying;
		obj_from_char(temp);
		temp->next_content = list;
		list = temp;
	}

	while(list) {
		temp = list;
		list = list->next_content;

		if(shop_producing(temp, shop_nr) &&
		        !get_obj_in_list_num(GET_OBJ_RNUM(temp), keeper->carrying)) {
			obj_to_char(temp, keeper);
			SHOP_SORT(shop_nr)++;
		} else {
			slide_obj(temp, keeper, shop_nr);
		}
	}
}


void shopping_sell(char *arg, char_data *ch, char_data *keeper, int shop_nr) {
	MENTOC_PREAMBLE();
	char tempstr[MAX_INPUT_LENGTH], name[MAX_INPUT_LENGTH];//FIXME related to vulnerable code below, tempbuf[MAX_INPUT_LENGTH];
	struct obj_data *obj;
	int sellnum, sold = 0, goldamt = 0;

	if(!(is_ok(keeper, ch, shop_nr))) {
		return;
	}

	if((sellnum = transaction_amt(arg)) < 0) {
		char buf[MAX_INPUT_LENGTH];

		snprintf(buf, sizeof(buf), "%s A negative amount?  Try buying something.", GET_NAME(ch).c_str());
		do_tell(keeper, buf, cmd_tell, 0,player);
		return;
	}

	if(!*arg || !sellnum) {
		char buf[MAX_INPUT_LENGTH];

		snprintf(buf, sizeof(buf), "%s What do you want to sell??", GET_NAME(ch).c_str());
		do_tell(keeper, buf, cmd_tell, 0,player);
		return;
	}

	one_argument(arg, name);

	if(!(obj = get_selling_obj(ch, name, keeper, shop_nr, TRUE))) {
		return;
	}

	if(GET_GOLD(keeper) + SHOP_BANK(shop_nr) < sell_price(obj, shop_nr, keeper, ch)) {
		char buf[MAX_INPUT_LENGTH];

		snprintf(buf, sizeof(buf), shop_index[shop_nr].missing_cash1, GET_NAME(ch).c_str());
		do_tell(keeper, buf, cmd_tell, 0,player);
		return;
	}

	while(obj && GET_GOLD(keeper) + SHOP_BANK(shop_nr) >= sell_price(obj, shop_nr, keeper, ch) && sold < sellnum) {
		int charged = sell_price(obj, shop_nr, keeper, ch);

		goldamt += charged;
		GET_GOLD(keeper) -= charged;

		sold++;
		obj_from_char(obj);
		slide_obj(obj, keeper, shop_nr);	/* Seems we don't use return value. */
		obj = get_selling_obj(ch, name, keeper, shop_nr, FALSE);
	}

	if(sold < sellnum) {
		char buf[MAX_INPUT_LENGTH];

		if(!obj) {
			snprintf(buf, sizeof(buf), "%s You only have %d of those.", GET_NAME(ch).c_str(), sold);
		} else if(GET_GOLD(keeper) + SHOP_BANK(shop_nr) < sell_price(obj, shop_nr, keeper, ch)) {
			snprintf(buf, sizeof(buf), "%s I can only afford to buy %d of those.", GET_NAME(ch).c_str(), sold);
		} else {
			snprintf(buf, sizeof(buf), "%s Something really screwy made me buy %d.", GET_NAME(ch).c_str(), sold);
		}

		do_tell(keeper, buf, cmd_tell, 0,player);
	}

	GET_GOLD(ch) += goldamt;

	strlcpy(tempstr, times_message(0, name, sold), sizeof(tempstr));
	/** FIXME crappy vulnerable code
	 */
	//snprintf(tempbuf, sizeof(tempbuf) -10 , "$n sells %s.", tempstr);
	//act(tempbuf, FALSE, ch, obj, 0, TO_ROOM);

	//snprintf(tempbuf, sizeof(tempbuf) - 10, shop_index[shop_nr].message_sell, GET_NAME(ch).c_str(), goldamt);
	//do_tell(keeper, tempbuf, cmd_tell, 0);

	//send_to_char(ch, "The shopkeeper now has %s.\r\n", tempstr);

	if(GET_GOLD(keeper) < MIN_OUTSIDE_BANK) {
		goldamt = MIN(MAX_OUTSIDE_BANK - GET_GOLD(keeper), SHOP_BANK(shop_nr));
		SHOP_BANK(shop_nr) -= goldamt;
		GET_GOLD(keeper) += goldamt;
	}
}


void shopping_value(char *arg, char_data *ch, char_data *keeper, int shop_nr) {
	MENTOC_PREAMBLE();
	char buf[MAX_STRING_LENGTH], name[MAX_INPUT_LENGTH];
	struct obj_data *obj;

	if(!is_ok(keeper, ch, shop_nr)) {
		return;
	}

	if(!*arg) {
		snprintf(buf, sizeof(buf), "%s What do you want me to evaluate??", GET_NAME(ch).c_str());
		do_tell(keeper, buf, cmd_tell, 0,player);
		return;
	}

	one_argument(arg, name);

	if(!(obj = get_selling_obj(ch, name, keeper, shop_nr, TRUE))) {
		return;
	}

	snprintf(buf, sizeof(buf), "%s I'll give you %d gold coins for that!", GET_NAME(ch).c_str(), sell_price(obj, shop_nr, keeper, ch));
	do_tell(keeper, buf, cmd_tell, 0,player);
}


char *list_object(struct obj_data *obj, int cnt, int aindex, int shop_nr, char_data *keeper, char_data *ch) {
	MENTOC_PREAMBLE();
	static char result[256];
	char	itemname[128],
	        quantity[16];	/* "Unlimited" or "%d" */

	if(shop_producing(obj, shop_nr)) {
		strcpy(quantity, "Unlimited");    /* strcpy: OK (for 'quantity >= 10') */
	} else {
		sprintf(quantity, "%d", cnt);    /* sprintf: OK (for 'quantity >= 11', 32-bit int) */
	}

	switch(GET_OBJ_TYPE(obj)) {
		case ITEM_DRINKCON:
			if(GET_OBJ_VAL(obj, 1)) {
				snprintf(itemname, sizeof(itemname), "%s of %s", obj->short_description.c_str(), drinks[GET_OBJ_VAL(obj, 2)]);
			} else {
				strlcpy(itemname, obj->short_description.c_str(), sizeof(itemname));
			}

			break;

		case ITEM_WAND:
		case ITEM_STAFF:
			snprintf(itemname, sizeof(itemname), "%s%s", obj->short_description.c_str(),
			         GET_OBJ_VAL(obj, 2) < GET_OBJ_VAL(obj, 1) ? " (partially used)" : "");
			break;

		default:
			strlcpy(itemname, obj->short_description.c_str(), sizeof(itemname));
			break;
	}

	CAP(itemname);

	snprintf(result, sizeof(result), " %2d)  %9s   %-48s %6d\r\n",
	         aindex, quantity, itemname, buy_price(obj, shop_nr, keeper, ch));
	return (result);
}


void shopping_list(char *arg, char_data *ch, char_data *keeper, int shop_nr) {
	char buf[MAX_STRING_LENGTH], name[MAX_INPUT_LENGTH];
	struct obj_data *obj, *last_obj = NULL;
	int cnt = 0, lindex = 0, found = FALSE;
	size_t len;
	/* cnt is the number of that particular object available */

	if(!is_ok(keeper, ch, shop_nr)) {
		return;
	}

	if(SHOP_SORT(shop_nr) < IS_CARRYING_N(keeper)) {
		sort_keeper_objs(keeper, shop_nr);
	}

	one_argument(arg, name);

	len = strlcpy(buf,   " ##   Available   Item                                               Cost\r\n"
	              "-------------------------------------------------------------------------\r\n", sizeof(buf));

	if(keeper->carrying)
		for(obj = keeper->carrying; obj; obj = obj->next_content)
			if(CAN_SEE_OBJ(ch, obj) && GET_OBJ_COST(obj) > 0) {
				if(!last_obj) {
					last_obj = obj;
					cnt = 1;
				} else if(same_obj(last_obj, obj)) {
					cnt++;
				} else {
					lindex++;

					if(!*name || isname(name, last_obj->name)) {
						strncat(buf, list_object(last_obj, cnt, lindex, shop_nr, keeper, ch), sizeof(buf) - len - 1);	/* strncat: OK */
						len = strlen(buf);

						if(len + 1 >= sizeof(buf)) {
							break;
						}

						found = TRUE;
					}

					cnt = 1;
					last_obj = obj;
				}
			}

	lindex++;

	if(!last_obj) {	/* we actually have nothing in our list for sale, period */
		send_to_char(ch, "Currently, there is nothing for sale.\r\n");
	} else if(*name && !found) {	/* nothing the char was looking for was found */
		send_to_char(ch, "Presently, none of those are for sale.\r\n");
	} else {
		if(!*name || isname(name, last_obj->name))	/* show last obj */
			if(len < sizeof(buf)) {
				strncat(buf, list_object(last_obj, cnt, lindex, shop_nr, keeper, ch), sizeof(buf) - len - 1);    /* strncat: OK */
			}

		page_string(*ch->desc, buf, TRUE);
	}
}

ACMD(do_list){
	mods::shop::list_shop_items<decltype(mods::globals::room_shopmap)>(
			player,
			mods::globals::room_shopmap
	);
}
ACMD(do_buy){
	mods::shop::buy_item<decltype(mods::globals::room_shopmap)>(
			player,
			mods::globals::room_shopmap,
			argument
	);
}
void shop_view_item(player_ptr_t& player, int16_t item_number){
	mods::shop::show_item<decltype(mods::globals::room_shopmap)>(
			player,
			mods::globals::room_shopmap,
			item_number
	);
}

int ok_shop_room(int shop_nr, room_vnum room) {
	int mindex;

	for(mindex = 0; SHOP_ROOM(shop_nr, mindex) != NOWHERE; mindex++)
		if(SHOP_ROOM(shop_nr, mindex) == room) {
			return (TRUE);
		}

	return (FALSE);
}


SPECIAL(shop_keeper) {
	char_data *keeper = (char_data *)me;
	int shop_nr;

	for(shop_nr = 0; shop_nr <= top_shop; shop_nr++)
		if(SHOP_KEEPER(shop_nr) == keeper->nr) {
			break;
		}

	if(shop_nr > top_shop) {
		return (FALSE);
	}

	if(SHOP_FUNC(shop_nr))	/* Check secondary function */
		if((SHOP_FUNC(shop_nr))(ch, me, cmd, argument,player)) {
			return (TRUE);
		}

	if(keeper == ch) {
		if(cmd) {
			SHOP_SORT(shop_nr) = 0;    /* Safety in case "drop all" */
		}

		return (FALSE);
	}

	if(!ok_shop_room(shop_nr, GET_ROOM_VNUM(IN_ROOM(ch)))) {
		return (0);
	}

	if(!AWAKE(keeper)) {
		return (FALSE);
	}

	if(CMD_IS("steal")) {
		char argm[MAX_INPUT_LENGTH];

		snprintf(argm, sizeof(argm), "$N shouts '%s'", MSG_NO_STEAL_HERE);
		act(argm, FALSE, ch, 0, keeper, TO_CHAR);

		do_action(keeper, GET_NAME(ch).ptr(), cmd_slap, 0,player);
		return (TRUE);
	}

	if(CMD_IS("buy")) {
		shopping_buy(argument, ch, keeper, shop_nr);
		return (TRUE);
	} else if(CMD_IS("sell")) {
		shopping_sell(argument, ch, keeper, shop_nr);
		return (TRUE);
	} else if(CMD_IS("value")) {
		shopping_value(argument, ch, keeper, shop_nr);
		return (TRUE);
	} else if(CMD_IS("list")) {
		shopping_list(argument, ch, keeper, shop_nr);
		return (TRUE);
	}

	return (FALSE);
}


int ok_damage_shopkeeper(char_data *ch, char_data *victim) {
	MENTOC_PREAMBLE();
	int sindex;

	if(static_cast<std::size_t>(GET_MOB_RNUM(victim)) >= mob_index.size()){
		return 0;
	}
	if(!IS_MOB(victim) || mob_index[GET_MOB_RNUM(victim)].func != shop_keeper) {
		return (TRUE);
	}

	/* Prevent "invincible" shopkeepers if they're charmed. */
	if(AFF_FLAGGED(victim, AFF_CHARM)) {
		return (TRUE);
	}

	for(sindex = 0; sindex <= top_shop; sindex++)
		if(GET_MOB_RNUM(victim) == SHOP_KEEPER(sindex) && !SHOP_KILL_CHARS(sindex)) {
			char buf[MAX_INPUT_LENGTH];

			snprintf(buf, sizeof(buf), "%s %s", GET_NAME(ch).c_str(), MSG_CANT_KILL_KEEPER);
			do_tell(victim, buf, cmd_tell, 0,player);

			do_action(victim, GET_NAME(ch).ptr(), cmd_slap, 0,player);
			return (FALSE);
		}

	return (TRUE);
}


/* val == obj_vnum and obj_rnum (?) */
int add_to_list(struct shop_buy_data *list, int type, int *len, int *val) {
	if(*val != NOTHING) {
		if(*len < MAX_SHOP_OBJ) {
			if(type == LIST_PRODUCE) {
				*val = real_object(*val);
			}

			if(*val != NOTHING) {
				BUY_TYPE(list[*len]) = *val;
				BUY_WORD(list[(*len)++]) = NULL;
			} else {
				*val = NOTHING;
			}

			return (FALSE);
		} else {
			return (TRUE);
		}
	}

	return (FALSE);
}


int end_read_list(struct shop_buy_data *list, int len, int error) {
	if(error) {
		log("SYSERR: Raise MAX_SHOP_OBJ constant in shop.h to %d", len + error);
	}

	BUY_WORD(list[len]) = NULL;
	BUY_TYPE(list[len++]) = NOTHING;
	return (len);
}


void read_line(FILE *shop_f, const char *string, void *data) {
	char buf[READ_SIZE];

	if(!get_line(shop_f, buf) || !sscanf(buf, string, data)) {
		log("SYSERR: Error in shop #%d, near '%s' with '%s'", SHOP_NUM(top_shop), buf, string);
		exit(1);
	}
}


int read_list(FILE *shop_f, struct shop_buy_data *list, int new_format,
              int max, int type) {
	int count, temp, len = 0, error = 0;

	if(new_format) {
		for(;;) {
			read_line(shop_f, "%d", &temp);

			if(temp < 0) {	/* Always "-1" the string. */
				break;
			}

			error += add_to_list(list, type, &len, &temp);
		}
	} else
		for(count = 0; count < max; count++) {
			read_line(shop_f, "%d", &temp);
			error += add_to_list(list, type, &len, &temp);
		}

	return (end_read_list(list, len, error));
}


/* END_OF inefficient. */
int read_type_list(FILE *shop_f, struct shop_buy_data *list,
                   int new_format, int max) {
	log("DEPRECATED: read_type_list. REASON: vulnerable+cantbarsed");
	return 0;
#if 0
	int tindex, num, len = 0, error = 0;
	char *ptr;
	char buf[MAX_STRING_LENGTH];

	if(!new_format) {
		return (read_list(shop_f, list, 0, max, LIST_TRADE));
	}

	do {
		fgets(buf, sizeof(buf), shop_f);

		if((ptr = strchr(buf, ';')) != NULL) {
			*ptr = '\0';
		} else {
			*(END_OF(buf) - 1) = '\0';
		}

		num = NOTHING;

		if(strncmp(buf, "-1", 4) != 0)
			for(tindex = 0; *item_types[tindex] != '\n'; tindex++)
				if(!strn_cmp(item_types[tindex], buf, strlen(item_types[tindex]))) {
					num = tindex;
					std::size_t len = strlen(item_types[tindex]);
					if(len > sizeof(buf) -1){
						len = sizeof(buf) -2;
					}
					strncpy(buf, buf + len,len);	/* strcpy: OK (always smaller) */
					break;
				}

		ptr = buf;

		if(num == NOTHING) {
			sscanf(buf, "%d", &num);

			while(!isdigit(*ptr)) {
				ptr++;
			}

			while(isdigit(*ptr)) {
				ptr++;
			}
		}

		while(isspace(*ptr)) {
			ptr++;
		}

		while(isspace(*(END_OF(ptr) - 1))) {
			*(END_OF(ptr) - 1) = '\0';
		}

		error += add_to_list(list, LIST_TRADE, &len, &num);

		if(*ptr) {
			BUY_WORD(list[len - 1]) = strdup(ptr);
		}
	} while(num >= 0);

	return (end_read_list(list, len, error));
#endif
}


char *read_shop_message(int mnum, room_vnum shr, FILE *shop_f, const char *why) {
	int cht, ss = 0, ds = 0, err = 0;
	char *tbuf;

	if(!(tbuf = fread_string(shop_f, why))) {
		return (NULL);
	}

	for(cht = 0; tbuf[cht]; cht++) {
		if(tbuf[cht] != '%') {
			continue;
		}

		if(tbuf[cht + 1] == 's') {
			ss++;
		} else if(tbuf[cht + 1] == 'd' && (mnum == 5 || mnum == 6)) {
			if(ss == 0) {
				log("SYSERR: Shop #%d has %%d before %%s, message #%d.", shr, mnum);
				err++;
			}

			ds++;
		} else if(tbuf[cht + 1] != '%') {
			log("SYSERR: Shop #%d has invalid format '%%%c' in message #%d.", shr, tbuf[cht + 1], mnum);
			err++;
		}
	}

	if(ss > 1 || ds > 1) {
		log("SYSERR: Shop #%d has too many specifiers for message #%d. %%s=%d %%d=%d", shr, mnum, ss, ds);
		err++;
	}

	if(err) {
		free(tbuf);
		return (NULL);
	}

	return (tbuf);
}

void boot_sql_shops() {
	log("Booting shops from sql");
	return;
}

void boot_the_shops() {
	boot_sql_shops();
	return;
}


void assign_the_shopkeepers(void) {
	int cindex;

	cmd_say = find_command("say");
	cmd_tell = find_command("tell");
	cmd_emote = find_command("emote");
	cmd_slap = find_command("slap");
	cmd_puke = find_command("puke");

	for(cindex = 0; cindex <= top_shop; cindex++) {
		if(SHOP_KEEPER(cindex) == NOBODY) {
			continue;
		}

		/* Having SHOP_FUNC() as 'shop_keeper' will cause infinite recursion. */
		if(mob_index[SHOP_KEEPER(cindex)].func && mob_index[SHOP_KEEPER(cindex)].func != shop_keeper) {
			SHOP_FUNC(cindex) = mob_index[SHOP_KEEPER(cindex)].func;
		}

		mob_index[SHOP_KEEPER(cindex)].func = shop_keeper;
	}
}


char *customer_string(int shop_nr, int detailed) {
	int sindex = 0, flag = 1, nlen;
	size_t len = 0;
	static char buf[256];

	while(*trade_letters[sindex] != '\n' && len + 1 < sizeof(buf)) {
		if(detailed) {
			if(!IS_SET(flag, SHOP_TRADE_WITH(shop_nr))) {
				nlen = snprintf(buf + len, sizeof(buf) - len, ", %s", trade_letters[sindex]);

				if(len + nlen >= sizeof(buf) || nlen < 0) {
					break;
				}

				len += nlen;
			}
		} else {
			buf[len++] = (IS_SET(flag, SHOP_TRADE_WITH(shop_nr)) ? '_' : *trade_letters[sindex]);
			buf[len] = '\0';

			if(len >= sizeof(buf)) {
				break;
			}
		}

		sindex++;
		flag <<= 1;
	}

	buf[sizeof(buf) - 1] = '\0';
	return (buf);
}


/* END_OF inefficient */
void list_all_shops(char_data *ch) {
	const char *list_all_shops_header =
	    " ##   Virtual   Where    Keeper    Buy   Sell   Customers\r\n"
	    "---------------------------------------------------------\r\n";
	int shop_nr, headerlen = strlen(list_all_shops_header);
	size_t len = 0;
	char buf[MAX_STRING_LENGTH], buf1[16];

	*buf = '\0';

	for(shop_nr = 0; shop_nr <= top_shop && len < sizeof(buf); shop_nr++) {
		/* New page in page_string() mechanism, print the header again. */
		if(!(shop_nr % (PAGE_LENGTH - 2))) {
			/*
			 * If we don't have enough room for the header, or all we have room left
			 * for is the header, then don't add it and just quit now.
			 */
			if(len + headerlen + 1 >= sizeof(buf)) {
				break;
			}

			strcpy(buf + len, list_all_shops_header);	/* strcpy: OK (length checked above) */
			len += headerlen;
		}

		if(SHOP_KEEPER(shop_nr) == NOBODY) {
			strcpy(buf1, "<NONE>");    /* strcpy: OK (for 'buf1 >= 7') */
		} else {
			sprintf(buf1, "%6d", mob_index[SHOP_KEEPER(shop_nr)].vnum);    /* sprintf: OK (for 'buf1 >= 11', 32-bit int) */
		}

		len += snprintf(buf + len, sizeof(buf) - len,
		                "%3d   %6d   %6d    %s   %3.2f   %3.2f    %s\r\n",
		                shop_nr + 1, SHOP_NUM(shop_nr), SHOP_ROOM(shop_nr, 0), buf1,
		                SHOP_SELLPROFIT(shop_nr), SHOP_BUYPROFIT(shop_nr),
		                customer_string(shop_nr, FALSE));
	}

	page_string(*ch->desc, buf, TRUE);
}


void list_detailed_shop(char_data *ch, int shop_nr) {
	char_data *k;
	int sindex, column;
	char *ptrsave;

	send_to_char(ch, "Vnum:       [%5d], Rnum: [%5d]\r\n", SHOP_NUM(shop_nr), shop_nr + 1);


	send_to_char(ch, "Rooms:      ");
	column = 12;	/* ^^^ strlen ^^^ */

	for(sindex = 0; SHOP_ROOM(shop_nr, sindex) != NOWHERE; sindex++) {
		char buf1[128];
		int linelen, temp;

		if(sindex) {
			send_to_char(ch, ", ");
			column += 2;
		}

		if((temp = real_room(SHOP_ROOM(shop_nr, sindex))) != NOWHERE) {
			linelen = snprintf(buf1, sizeof(buf1), "%s (#%d)", world[temp].name.c_str(), GET_ROOM_VNUM(temp));
		} else {
			linelen = snprintf(buf1, sizeof(buf1), "<UNKNOWN> (#%d)", SHOP_ROOM(shop_nr, sindex));
		}

		/* Implementing word-wrapping: assumes screen-size == 80 */
		if(linelen + column >= 78 && column >= 20) {
			send_to_char(ch, "\r\n            ");
			/* 12 is to line up with "Rooms:" printed first, and spaces above. */
			column = 12;
		}

		if(!send_to_char(ch, "%s", buf1)) {
			return;
		}

		column += linelen;
	}

	if(!sindex) {
		send_to_char(ch, "Rooms:      None!");
	}

	send_to_char(ch, "\r\nShopkeeper: ");

	if(SHOP_KEEPER(shop_nr) != NOBODY) {
		send_to_char(ch, "%s (#%d), Special Function: %s\r\n",
		             GET_NAME(&mob_proto[SHOP_KEEPER(shop_nr)]).c_str(),
		             mob_index[SHOP_KEEPER(shop_nr)].vnum,
		             YESNO(SHOP_FUNC(shop_nr)));

		if((k = get_char_num(SHOP_KEEPER(shop_nr))))
			send_to_char(ch, "Coins:      [%9d], Bank: [%9d] (Total: %d)\r\n",
			             GET_GOLD(k), SHOP_BANK(shop_nr), GET_GOLD(k) + SHOP_BANK(shop_nr));
	} else {
		send_to_char(ch, "<NONE>\r\n");
	}


	send_to_char(ch, "Customers:  %s\r\n", (ptrsave = customer_string(shop_nr, TRUE)) ? ptrsave : "None");


	send_to_char(ch, "Produces:   ");
	column = 12;	/* ^^^ strlen ^^^ */

	for(sindex = 0; SHOP_PRODUCT(shop_nr, sindex) != NOTHING; sindex++) {
		char buf1[128];
		int linelen;

		if(sindex) {
			send_to_char(ch, ", ");
			column += 2;
		}

		linelen = snprintf(buf1, sizeof(buf1), "%s (#%d)",
		                   obj_proto[SHOP_PRODUCT(shop_nr, sindex)].short_description.c_str(),
		                   obj_index[SHOP_PRODUCT(shop_nr, sindex)].vnum);

		/* Implementing word-wrapping: assumes screen-size == 80 */
		if(linelen + column >= 78 && column >= 20) {
			send_to_char(ch, "\r\n            ");
			/* 12 is to line up with "Produces:" printed first, and spaces above. */
			column = 12;
		}

		if(!send_to_char(ch, "%s", buf1)) {
			return;
		}

		column += linelen;
	}

	if(!sindex) {
		send_to_char(ch, "Produces:   Nothing!");
	}

	send_to_char(ch, "\r\nBuys:       ");
	column = 12;	/* ^^^ strlen ^^^ */

	for(sindex = 0; SHOP_BUYTYPE(shop_nr, sindex) != NOTHING; sindex++) {
		char buf1[128];
		size_t linelen;

		if(sindex) {
			send_to_char(ch, ", ");
			column += 2;
		}

		linelen = snprintf(buf1, sizeof(buf1), "%s (#%d) [%s]",
		                   item_types[SHOP_BUYTYPE(shop_nr, sindex)],
		                   SHOP_BUYTYPE(shop_nr, sindex),
		                   SHOP_BUYWORD(shop_nr, sindex) ? SHOP_BUYWORD(shop_nr, sindex) : "all");

		/* Implementing word-wrapping: assumes screen-size == 80 */
		if(linelen + column >= 78 && column >= 20) {
			send_to_char(ch, "\r\n            ");
			/* 12 is to line up with "Buys:" printed first, and spaces above. */
			column = 12;
		}

		if(!send_to_char(ch, "%s", buf1)) {
			return;
		}

		column += linelen;
	}

	if(!sindex) {
		send_to_char(ch, "Buys:       Nothing!");
	}

	send_to_char(ch, "\r\nBuy at:     [%4.2f], Sell at: [%4.2f], Open: [%d-%d, %d-%d]\r\n",
	             SHOP_SELLPROFIT(shop_nr), SHOP_BUYPROFIT(shop_nr), SHOP_OPEN1(shop_nr),
	             SHOP_CLOSE1(shop_nr), SHOP_OPEN2(shop_nr), SHOP_CLOSE2(shop_nr));


	/* Need a local buffer. */
	{
		char buf1[128];
		sprintbit(SHOP_BITVECTOR(shop_nr), shop_bits, buf1, sizeof(buf1));
		send_to_char(ch, "Bits:       %s\r\n", buf1);
	}
}


void show_shops(char_data *ch, char *arg) {
	int shop_nr;

	if(!*arg) {
		list_all_shops(ch);
	} else {
		if(!str_cmp(arg, ".")) {
			for(shop_nr = 0; shop_nr <= top_shop; shop_nr++)
				if(ok_shop_room(shop_nr, GET_ROOM_VNUM(IN_ROOM(ch)))) {
					break;
				}

			if(shop_nr > top_shop) {
				send_to_char(ch, "This isn't a shop!\r\n");
				return;
			}
		} else if(is_number(arg)) {
			shop_nr = atoi(arg) - 1;
		} else {
			shop_nr = -1;
		}

		if(shop_nr < 0 || shop_nr > top_shop) {
			send_to_char(ch, "Illegal shop number.\r\n");
			return;
		}

		list_detailed_shop(ch, shop_nr);
	}
}


void destroy_shops(void) {
	ssize_t cnt, itr;

	if(!shop_index.size()) {
		return;
	}

	for(cnt = 0; cnt <= top_shop; cnt++) {
		if(shop_index[cnt].no_such_item1) {
			free(shop_index[cnt].no_such_item1);
		}

		if(shop_index[cnt].no_such_item2) {
			free(shop_index[cnt].no_such_item2);
		}

		if(shop_index[cnt].missing_cash1) {
			free(shop_index[cnt].missing_cash1);
		}

		if(shop_index[cnt].missing_cash2) {
			free(shop_index[cnt].missing_cash2);
		}

		if(shop_index[cnt].do_not_buy) {
			free(shop_index[cnt].do_not_buy);
		}

		if(shop_index[cnt].message_buy) {
			free(shop_index[cnt].message_buy);
		}

		if(shop_index[cnt].message_sell) {
			free(shop_index[cnt].message_sell);
		}

		if(shop_index[cnt].in_room) {
			free(shop_index[cnt].in_room);
		}

		if(shop_index[cnt].producing) {
			free(shop_index[cnt].producing);
		}

		if(shop_index[cnt].type) {
			for(itr = 0; BUY_TYPE(shop_index[cnt].type[itr]) != NOTHING; itr++)
				if(BUY_WORD(shop_index[cnt].type[itr])) {
					free(BUY_WORD(shop_index[cnt].type[itr]));
				}

			free(shop_index[cnt].type);
		}
	}

	top_shop = -1;
}

#ifdef __MENTOC_USE_PQXX_RESULT__
	template <>
	void shop_data<mods::orm::shop,mods::orm::shop_rooms,mods::orm::shop_objects>::feed(const pqxx::result::reference & in_row)
#else
	template <>
	void shop_data<mods::orm::shop,mods::orm::shop_rooms,mods::orm::shop_objects>::feed(pqxx::row in_row)
#endif
{
#ifdef __MENTOC_SHOW_SHOP_CPP_DEBUG_OUTPUT__
	std::cerr << "[shop_data::feed] called\n";
#endif
	this->db_id = in_row["shop_id"].as<db_id_t>();
	this->vnum = in_row["shop_vnum"].as<shop_vnum>();
	this->profit_buy = in_row["shop_profit_buy"].as<float>();
	this->profit_sell = in_row["shop_profit_sell"].as<float>();
	this->title.assign(in_row["shop_title"].c_str());
#ifdef __MENTOC_SHOW_SHOP_CPP_DEBUG_OUTPUT__
	std::cerr << "[shop_data::feed] title from row: '" << in_row["shop_title"].c_str() << "'\n";
#endif
	this->description.assign(in_row["shop_description"].c_str());
#ifdef __MENTOC_SHOW_SHOP_CPP_DEBUG_OUTPUT__
	std::cerr << "[shop_data::feed] description from row: '" << in_row["shop_description"].c_str() << "'\n";
#endif
	this->flags = in_row["shop_flags"].as<uint64_t>();
	this->no_such_item1.assign(in_row["shop_no_such_item1"].c_str());
	this->no_such_item2.assign(in_row["shop_no_such_item2"].c_str());
	this->missing_cash1.assign(in_row["shop_missing_cash1"].c_str());
	this->missing_cash2.assign(in_row["shop_missing_cash2"].c_str());
	this->do_not_buy.assign(in_row["shop_do_not_buy"].c_str());
	this->message_buy.assign(in_row["shop_message_buy"].c_str());
	this->message_sell.assign(in_row["shop_message_sell"].c_str());
	this->temper1 = in_row["shop_temper1"].as<int>();
	this->bitvector = in_row["shop_bitvector"].as<bitvector_t>();
	this->keeper = in_row["shop_keeper"].as<mob_vnum>();
	this->with_who = in_row["shop_with_who"].as<int>();
	this->open1 = in_row["shop_open1"].as<int>();
	this->open2 = in_row["shop_open2"].as<int>();
	this->close1 = in_row["shop_close1"].as<int>();
	this->close2 = in_row["shop_close2"].as<int>();
	this->bankAccount = in_row["shop_bankAccount"].as<int>();
	this->lastsort = in_row["shop_lastsort"].as<int>();
	this->shop_type = in_row["shop_type"].as<int>();
	
	for(auto && s_row : db_get_by_meta("shop_rooms","shop_vnum",std::to_string(this->vnum).c_str())){
#ifdef __MENTOC_SHOW_SHOP_CPP_DEBUG_OUTPUT__
		std::cerr << __FILE__ << "|" << __LINE__ << ": found shop_rooms record: " << s_row["shop_room_vnum"].c_str() << "\n";
#endif
		this->room_info.rooms.emplace_back(s_row["shop_room_vnum"].as<room_vnum>());
	}
	int16_t count_placed = this->room_info.place_keepers_in_rooms(this->keeper);
	log("%d keepers loaded in shop_vnum: %d",count_placed,this->vnum);
	for(auto &&  s_row : db_get_by_meta("shop_objects","shop_vnum",std::to_string(this->vnum).c_str())){
#ifdef __MENTOC_SHOW_SHOP_CPP_DEBUG_OUTPUT__
		std::cerr << __FILE__ << "|" << __LINE__ << ": found shop_objects record: " << s_row["shop_object_vnum"].c_str() << "\n";
#endif
		this->object_info.objects.emplace_back(s_row["shop_object_vnum"].as<obj_vnum>());
	}
	this->m_loaded = true;
}

template <>
shop_data<mods::orm::shop,mods::orm::shop_rooms,mods::orm::shop_objects>::shop_data(){
	init();
}

template <>
void shop_data<mods::orm::shop,mods::orm::shop_rooms,mods::orm::shop_objects>::list_to_char(player_ptr_t& player){
	d("stub list_to_char");
	/** TODO */
	static bool static_is_loaded = false;
	static std::string static_item_list = "";
	std::size_t item_number = 0;
	if(!static_is_loaded){
		const std::size_t name_field_len = 36;
		static_item_list =  "=-------------------------------------------------------=\r\n";
		static_item_list += "[  id  ]->[            item name                ][  mp  ]\r\n";
		static_item_list += "=-------------------------------------------------------=\r\n";
		for(const auto & obj_vnum_id : object_info.objects){
			auto obj_proto_item = real_object(obj_vnum_id);
			if(obj_proto_item == NOTHING){
				continue;
			}
			//id
			static_item_list += "[";
			auto id_number = std::to_string(++item_number);
			if(id_number.length() == 1){
				static_item_list += "   ";
				static_item_list += id_number;
				static_item_list += "  ";
			}
			if(id_number.length() == 2){
				static_item_list += "  ";
				static_item_list += id_number;
				static_item_list += "  ";
			}
			static_item_list += "]->[    ";
			static_item_list += obj_proto[obj_proto_item].name.str();
			for(unsigned i=obj_proto[obj_proto_item].name.str().length() + 4; i <= name_field_len;i++){
				static_item_list += " ";
			}
			static_item_list += "][";
			auto mp_str = std::to_string(mods::shop::object_cost(obj_proto[obj_proto_item]));
			if(mp_str.length() == 2){
				static_item_list += "  ";
				static_item_list += mp_str;
				static_item_list += "  ";
			}
			if(mp_str.length() == 1){
				static_item_list += "   ";
				static_item_list += mp_str;
				static_item_list += "  ";
			}
			if(mp_str.length() == 3){
				static_item_list += " ";
				static_item_list += mp_str;
				static_item_list += "  ";
			}
			if(mp_str.length() == 4){
				static_item_list += " ";
				static_item_list += mp_str;
				static_item_list += " ";
			}
			if(mp_str.length() == 5){
				static_item_list += mp_str;
				static_item_list += " ";
			}
			if(mp_str.length() == 6){
				static_item_list += mp_str;
			}
			static_item_list += "]\r\n";
		}
		static_is_loaded = true;
	}
	player->sendln(static_item_list);
	player->sendln("\r\nTo view an item, simply type {grn}view N{/grn} where N is the id");
	player->sendln("see: help list, help shop, help view");
}

template <typename TOrmType,typename T,typename R>
void shop_data<TOrmType,T,R>::init(){
#ifdef __MENTOC_SHOW_SHOP_CPP_DEBUG_OUTPUT__
	std::cerr << "[shop_data::init] called\n";
#endif
	this->func = nullptr;
	this->title.clear();
	this->description.clear();
	this->flags = 0;
	this->db_id = 0;
	this->vnum = 0;
	this->producing = nullptr;
	this->profit_buy = 0.0;
	this->profit_sell = 0.0;
	this->type = nullptr;
	this->no_such_item1.clear(); 
	this->no_such_item2.clear();
	this->missing_cash1.clear();
	this->missing_cash2.clear();
	this->do_not_buy.clear();
	this->message_buy.clear();
	this->message_sell.clear();
	this->temper1 = 0;
	this->bitvector = 0;
	this->keeper = 0; //rnum
	this->with_who = 0;
	this->in_room = nullptr;
	this->open1 = 0;
	this->open2 = 0;
	this->close1 = 0;
	this->close2 = 0;
	this->bankAccount = 0;
	this->lastsort = 0;
	this->shop_type = 0;
	this->m_loaded = false;
	this->m_objects.clear();
}
template <typename TOrmType,typename T,typename R>
std::map<std::string,std::string> shop_data<TOrmType,T,R>::export_shop(){
	std::map<std::string,std::string> s_map;
	s_map["shop_vnum"] = mods::util::itoa(vnum);
	s_map["shop_title"] = title.str();
	s_map["shop_description"] = description.str();
	s_map["shop_flags"] = std::to_string(flags);
	s_map["shop_profit_buy"] = mods::util::itoa(profit_buy);
	s_map["shop_profit_sell"] = mods::util::itoa(profit_sell);
	s_map["shop_no_such_item1"] = (no_such_item1.str());
	s_map["shop_no_such_item2"] = (no_such_item2.str());
	s_map["shop_missing_cash1"] = (missing_cash1.str());
	s_map["shop_missing_cash2"] = (missing_cash2.str());
	s_map["shop_do_not_buy"] =(do_not_buy.str());
	s_map["shop_message_buy"] = (message_buy.str());
	s_map["shop_message_sell"] =(message_sell.str());
	s_map["shop_temper1"] = mods::util::itoa(temper1);
	s_map["shop_bitvector"] = mods::util::itoa(bitvector);
	s_map["shop_keeper"] = mods::util::itoa(keeper);
	s_map["shop_with_who"] = mods::util::itoa(with_who);
	s_map["shop_open1"] = mods::util::itoa(open1);
	s_map["shop_open2"] = mods::util::itoa(open2);
	s_map["shop_close1"] = mods::util::itoa(close1);
	s_map["shop_close2"] = mods::util::itoa(close2);
	s_map["shop_bankAccount"] = mods::util::itoa(bankAccount);
	s_map["shop_lastsort"] = mods::util::itoa(lastsort);
	s_map["shop_type"] = std::to_string(shop_type);
	return std::move(s_map);
}

template <>
std::tuple<bool,uint64_t,std::string> shop_data<mods::orm::shop,mods::orm::shop_rooms,mods::orm::shop_objects>::save(){
	m_orm.shop_vnum_id = vnum;
	m_orm.shop_title = title;
	m_orm.shop_description = description;
	m_orm.shop_flags = flags;
	m_orm.shop_profit_buy = profit_buy;
	m_orm.shop_profit_sell = profit_sell;
	m_orm.shop_no_such_item1.assign(no_such_item1.str());
	m_orm.shop_no_such_item2.assign(no_such_item2.str());
	m_orm.shop_missing_cash1.assign(missing_cash1.str());
	m_orm.shop_missing_cash2.assign(missing_cash2.str());
	m_orm.shop_do_not_buy.assign(do_not_buy.str());
	m_orm.shop_message_buy.assign(message_buy.str());
	m_orm.shop_message_sell.assign(message_sell.str());
	m_orm.shop_temper1 = temper1;
	m_orm.shop_bitvector = bitvector;
	m_orm.shop_keeper = keeper;
	m_orm.shop_with_who = with_who;
	m_orm.shop_open1 = open1;
	m_orm.shop_open2 = open2;
	m_orm.shop_close1 = close1;
	m_orm.shop_close2 = close2;
	m_orm.shop_bankAccount = bankAccount;
	m_orm.shop_lastsort = lastsort;
	m_orm.shop_type = shop_type;
	m_orm.room_info = room_info;
	m_orm.object_info = object_info;
	auto result = m_orm.save();
	if(std::get<0>(result)){
		m_loaded = true;
		db_id = std::get<1>(result);
	}
	return result;
}
