DROP TABLE IF EXISTS terminal_choices ;
DROP TABLE IF EXISTS computer_terminal ;
DROP TABLE IF EXISTS npc_dialogue;

DELETE FROM room WHERE room_number=410;

CREATE TABLE npc_dialogue (
	id SERIAL PRIMARY KEY,
	dialogue_mob_vnum INTEGER NOT NULL,
	dialogue_text TEXT NOT NULL,
	dialogue_part_of VARCHAR,
	dialogue_comment TEXT,
	dialogue_vnum INTEGER NOT NULL,
	dialogue_order INTEGER,
	created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE computer_terminal (
	id SERIAL PRIMARY KEY,
	terminal_room_vnum INTEGER NOT NULL,
	terminal_type VARCHAR NOT NULL DEFAULT 'chooser',
	terminal_attaches_to VARCHAR,
	terminal_name VARCHAR NOT NULL,
	terminal_description TEXT,
	created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE terminal_choices (
	id SERIAL PRIMARY KEY,
	choice_terminal_id INTEGER NOT NULL references computer_terminal(id),
	choice_id INTEGER NOT NULL,
	choice_title TEXT NOT NULL,
	choice_description TEXT NOT NULL,
	choice_action TEXT NOT NULL
);

INSERT INTO room(room_number,zone,sector_type,room_flag,name,description)
VALUES(
	410,1,0,0, 'Outer training courtyard - D.M.Z 0x4155',
					'You stand in a square shaped courtyard amongs other new recruits. You can smell the reek of sweat and dirt everywhere you go. {yel}Corporal James Taggart{/yel} stands before you: a menacing authoritative presence. There is a heavy steel door to the {grn}North{/grn}.');

INSERT INTO computer_terminal (terminal_room_vnum,terminal_attaches_to,terminal_name) VALUES(410, 'north','DMZ Proving Grounds - Course Selection Interface [Baphomet OS - version 1.3.0]');

INSERT INTO terminal_choices (choice_terminal_id, choice_id, choice_action, choice_title, choice_description) VALUES(
	(SELECT id FROM computer_terminal WHERE terminal_room_vnum=410 LIMIT 1)
, 1, 'start_course ${choice_id}', 
'Drone assisted Sniping Course',
			'Train your ability to snipe at incredibly long distances while utilizing your drone.|(+10% accuracy when drone scans the current room)|(+8% chance to go undetected)|(+10% bonus to critical chance)');

INSERT INTO terminal_choices (choice_terminal_id, choice_id, choice_action, choice_title, choice_description) VALUES(
	(SELECT id FROM computer_terminal WHERE terminal_room_vnum=410 LIMIT 1)
, 2, 'start_course ${choice_id}', 
 'Demolitions Course',
			'This demolitions course will get you up to speed with grenades, C4 plastic explosive, and claymore mines.|(+10% bonus when dealing explosive damage)|(+2 room grenade throwing distance)|(+4% chance to injure with C4 explosives)|(10% chance of enemy becoming maimed/unconscious from claymore mine)');
INSERT INTO terminal_choices (choice_terminal_id, choice_id, choice_action, choice_title, choice_description) VALUES(
	(SELECT id FROM computer_terminal WHERE terminal_room_vnum=410 LIMIT 1)
, 3, 'start_course ${choice_id}', 
		'Tactical Gadgetry Course',
			'Utilize your PDA to hack devices and unlock areas previously locked down by the enemy.|Discover how to breach a door using a breach charge.|Learn how to utilize flashbang and smoke grenades to disorient and mask your movements.|Learn how E.M.P. grenades affect the enemy communications network.|Attack your enemies with powerful incendiary grenades.|(+10% incendiary grenade bonus)|(+2 room range for EMP status affect dispersal)|(+40 ticks to smoke affect duration)|(+40 ticks to flashbang disorient affect duration)|(4% chance of not consuming flash bang grenade when thrown)');
INSERT INTO terminal_choices (choice_terminal_id, choice_id, choice_action, choice_title, choice_description) VALUES(
	(SELECT id FROM computer_terminal WHERE terminal_room_vnum=410 LIMIT 1)
, 4, 'start_course ${choice_id}', 
	'Marine Corps Martial Arts Program - {grn}Striking Clinic{/grn}',
			'Learn to save and recall strike combos.|Learn how to transition from striking distance to grappling and submissions techniques.|Learn the powerful knee stomp.|Prevent enemy from taking you to the ground.|(+10% chance of stuffing take down)|(4% chance of knee stomp causing depletion of all movement points)|(+10% ticks to status duration from stunning jab disorient affect)|(10% chance to double up jab at no extra stamina cost)');

INSERT INTO terminal_choices (choice_terminal_id, choice_id, choice_action, choice_title, choice_description) VALUES(
	(SELECT id FROM computer_terminal WHERE terminal_room_vnum=410 LIMIT 1)
, 5, 'start_course ${choice_id}', 
	'Marine Corps Martial Arts Program - {grn}Judo Trips and Throws{/grn}',
			'Learn the inner leg reap to disorient your target while throwing them to the ground.|(10% chance of inner leg reap causing opponent to break knee)|Learn to perform the sacrifice throw to launch your enemy to the ground.|(10% chance of enemy being thrown to ground in another room)|Learn the drop shoulder throw to force your enemy to land on their head.|(15% chance of breaking neck and status being MAIMED)|(10% chance of breaking arm)');

INSERT INTO terminal_choices (choice_terminal_id, choice_id, choice_action, choice_title, choice_description) VALUES(
	(SELECT id FROM computer_terminal WHERE terminal_room_vnum=410 LIMIT 1)
, 6, 'start_course ${choice_id}', 
	'Marine Corps Martial Arts Program - {grn}Submission Grappling{/grn}',
			'Armbar - Level 1 -> Dislocate arm of target enemy using the armbar technique.|(10% chance of ranged attack completely failing if one arm dislocated by armbar [Level 1 Armbar])|(5% of damage done by enemy with dislocated arm is inflicted on enemy when melee attacks are dealt [Level 1 Armbar])|Armbar - Level 2 -> Completely break arm with no chance of repair.|(50% damage is dealt by enemies with one broken arm [Level 2 Armbar])|(25% chance for ranged attacks to fail from enemy with broken arm [Level 2 Armbar])|Armbar - Level 3 -> Break both arms.|(4% chance of second arm being broken once first arm is broken [Level 3 Armbar])|(100% of melee attacks are impossible if attack requires arms or ranged weaponry [Level 3 Armbar])|(85% chance of being able to take primary weapon from enemy with 1 broken arm [Level 3 Armbar])|Knee Bar -> Break the targets knee using your waist as a fulcrum.|(25% chance of target to fall when attempting to flee or move)|(20% of movement is permanently subtracted from target enemy with one broken knee)|Rear Naked Choke -> Level 1 -> Seize control of the target and apply a chokehold from behind that will knock the enemy unconscious if not stopped.|(4% chance of target losing consciousness and not recovering at all)|(+15% damage bonus if allies or gadgets deal damage to target while in RNC hold)|(2.5% chance of breaking neck and status becoming UNCONSCIOUS)');


DELETE FROM mobile where mob_virtual_number=410;

INSERT INTO mobile(
mob_virtual_number,
mob_name,
mob_short_description,
mob_long_description,
mob_description,
mob_action_bitvector,
mob_affection_bitvector,
mob_ability_strength,
mob_ability_strength_add,
mob_ability_intelligence,
mob_ability_wisdom,
mob_ability_dexterity,
mob_ability_constitution,
mob_ability_charisma,
mob_alignment,
mob_attack_type,
mob_level,
mob_hitroll,
mob_armor,
mob_max_hitpoints,
mob_max_mana,
mob_max_move,
mob_gold,
mob_exp,
mob_load_position,
mob_default_position,
mob_sex,
mob_hitpoints,
mob_mana,
mob_move,
mob_damnodice,
mob_damsizedice,
mob_damroll,
mob_weight,
mob_height,
mob_class,
mob_special_extended_type)

VALUES(410,
	'Corporal James Taggart',
	'Corporal James Tagger short description',
	'Corporal James Tagger long description',
	'Corporal James Tagger description',
	'0', /** action bv */
	'0', /* aff bv */
	10, /* ab str */
	10, /** ab sa*/
	10, /** ab int */
	10, /** ab wis */
	10, /* ab dex */
	10, /* ab con */
	10, /* ab cha */
	0, /* align */
	0, /* atk */
	150, /* level */
	150, /*hitroll */
	150, /* armor */
	-1, /* max hp */
	-1, /* max mana */
	-1, /* max move */
	0, /* gold */
	9800, /* exp */
	0, /* load pos */
	0, /* def pos */
	0, /* sex */
	-1, /* hp */
	-1, /* mana */
	-1, /* move */
	50, /* damnodice */
	50, /* dam size dice */
	50, /* damroll */
	80, /* weight */
	9, /* height */
	0, /* class */
	0); /* spec ext type */

INSERT INTO npc_dialogue(dialogue_mob_vnum, dialogue_text, dialogue_part_of, dialogue_comment, dialogue_vnum, dialogue_order)
VALUES(410,
	'Welcome to the D.M.Z 0x4155 training courtyard, soldier.',
	'orientation-1',
	'what corp should say at orientation',
	1,
	0);

INSERT INTO npc_dialogue(dialogue_mob_vnum, dialogue_text, dialogue_part_of, dialogue_comment, dialogue_vnum, dialogue_order)
VALUES(410,
	'Here you will master the basics skills to make you an effective asset out in the field of combat.',
	'orientation-1',
	'what corp should say at orientation', 1, 1);

INSERT INTO npc_dialogue(dialogue_mob_vnum, dialogue_text, dialogue_part_of, dialogue_comment, dialogue_vnum, dialogue_order)
VALUES(410,
	'To see what training courses are available {yel}look{/yel} at the {blu}Course Selection Interface{/blu}.',
	'orientation-1',
	'what corp should say at orientation', 1, 2);

INSERT INTO npc_dialogue(dialogue_mob_vnum, dialogue_text, dialogue_part_of, dialogue_comment, dialogue_vnum, dialogue_order)
VALUES(410,
	'To start a course, type {yel}start ID{/yel} where ID is the id of the course on the far left of the screen.',
	'orientation-1',
	'what corp should say at orientation', 1, 3);

INSERT INTO npc_dialogue(dialogue_mob_vnum, dialogue_text, dialogue_part_of, dialogue_comment, dialogue_vnum, dialogue_order)
VALUES(410,
	'If you are already accustomed to this system, disregard my instructions and proceed when you ready.',
	'orientation-1',
	'what corp should say at orientation', 1,
	4);

INSERT INTO npc_dialogue(dialogue_mob_vnum, dialogue_text, dialogue_part_of, dialogue_comment, dialogue_vnum, dialogue_order)
VALUES(410,
			'Demolitions HOW TO: Section: GRENADES|'
			'=====================================|'
			'Step 1) Hold your grenade|'
			'run the command: {yel}hold grenade{/yel}|'
			'|'
			'Step 2) Throw grenade.|'
			'run the command: {yel}throw grenade DISTANCE{/yel}|'
			'{grn}replace the phrase DISTANCE with the number of rooms you would like to throw.{/grn}|'
			'|'
			'|'
			'Demolitions HOW TO: Section: C4|'
			'===========================================|'
			'Step 1) Attach the C4 to an object, door, person, or vehicle.|'
			'run the command: {yel}attach c4 ITEM{/yel}|'
			'{grn}replace the phrase ITEM with the target object, person, vehicle, or direction.{/grn}|'
			'|'
			'Step 2) Detonate the C4.|'
			'run the command: {yel}detonate{/yel}|'
			'|'
			'|'
			'Demolitions HOW TO: Section: Claymore Mines|'
			'===========================================|'
			'Step 1) Install the claymore mine at the specified door.|'
			'run the command: {yel}install claymore DIRECTION{/yel}|'
			'{grn}replace the phrase DISTANCE with the direction of the door you would like to install the claymore mine on.{/grn}|'
			'|',
	'demolitions-howto',
	'what corp should say at orientation regarding demolitions', 2, 0);

INSERT INTO npc_dialogue(dialogue_mob_vnum, dialogue_text, dialogue_part_of, dialogue_comment, dialogue_vnum, dialogue_order)
VALUES(410,
			'Demolitions HOW TO: Section: GRENADES|'
			'=====================================|'
			'Step 1) Hold your grenade|'
			'run the command: {yel}hold grenade{/yel}|'
			'|'
			'Step 2) Throw grenade.|'
			'run the command: {yel}throw grenade DISTANCE{/yel}|'
			'{grn}replace the phrase DISTANCE with the number of rooms you would like to throw.{/grn}|'
			'|'
			'|'
			'Demolitions HOW TO: Section: C4|'
			'===========================================|'
			'Step 1) Attach the C4 to an object, door, person, or vehicle.|'
			'run the command: {yel}attach c4 ITEM{/yel}|'
			'{grn}replace the phrase ITEM with the target object, person, vehicle, or direction.{/grn}|'
			'|'
			'Step 2) Detonate the C4.|'
			'run the command: {yel}detonate{/yel}|'
			'|'
			'|'
			'Demolitions HOW TO: Section: Claymore Mines|'
			'===========================================|'
			'Step 1) Install the claymore mine at the specified door.|'
			'run the command: {yel}install claymore DIRECTION{/yel}|'
			'{grn}replace the phrase DISTANCE with the direction of the door you would like to install the claymore mine on.{/grn}|'
			'|',
	'demolitions-howto',
	'what corp should say at orientation regarding demolitions', 2, 0);

INSERT INTO npc_dialogue(dialogue_mob_vnum, dialogue_text, dialogue_part_of, dialogue_comment, dialogue_vnum, dialogue_order)
VALUES(410,
			'Drone assisted sniping HOW TO:|'
			'==============================|'
			'Step 1) Deploy your drone.|'
			'run the command: {yel}deploy drone{/yel}|'
			'You will now see the world through the eyes of your drone.|'
			'|'
			'Step 2) Fly or ground travel to the target room.|'
			'run any direction commands to get to the room where your target enemy is. (north, east, south, etc)...|'
			'|'
			'Step 3) Target your enemy.|'
			'run the command: {yel}target ENEMY{/yel} where ENEMY is the name of the mob.|'
			'|'
			'Step 4) Snipe your target.|'
			'type this exactly to snipe your enemy: {yel}snipe target{/yel}. If you try to use |'
			'any other phrase other than target, the command will not work.|'
			'|',
	'drone-assisted-sniping-howto',
	'course page for drone assisted sniping', 2, 0);
