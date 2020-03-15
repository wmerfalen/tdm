#ifndef __MENTOC_MODS_ITEM_TYPES__
#define __MENTOC_MODS_ITEM_TYPES__
#include <boost/preprocessor.hpp>

/********************************************/
/** HOWTO: Add new item and subcategories   */
/* Step 1: Add a constant to ITEM_TYPES_SEQ */
/********************************************/
/**************************************************************************/
/** Each individual item type                                             */
/**************************************************************************/
#define MENTOC_ITEM_TYPES_SEQ \
(rifle) \
(explosive) \
(drone) \
(gadget) \
(attachment) \
(armor) \
(consumable) \
(trap) 

/*************************************************/
/** HOWTO: Add new item and subcategories        */
/* Step 2: Add a constant to ITEM_TYPES_SEQ_CAPS */
/*************************************************/
#define MENTOC_ITEM_TYPES_SEQ_CAPS \
(ITEM_RIFLE) \
(ITEM_EXPLOSIVE) \
(ITEM_DRONE) \
(ITEM_GADGET) \
(ITEM_ATTACHMENT) \
(ITEM_ARMOR) \
(ITEM_CONSUMABLE) \
(ITEM_TRAP)



/**************************************************************************/
/** ENUM MACROS AND SEQUENCES                                             */
/**************************************************************************/
/*********************************************************/
/** HOWTO: Add new item and subcategories                */
/* Step 3: Add a new *_TYPES_SEQUENCE for sub categories */
/*********************************************************/
#define MENTOC_RIFLE_TYPES_SEQUENCE (SHOTGUN) \
(ASSAULT_RIFLE) \
(SUB_MACHINE_GUN) \
(SNIPER) \
(HANDGUN) \
(PISTOL) \
(MACHINE_PISTOL) \
(LIGHT_MACHINE_GUN) 

#define MENTOC_RIFLE_MEMBERS_TUPLE ( \
(int,ammo_max,"#int, bullets",30), \
(std::string,ammo_type,"#string, enum","'SNIPER'"), \
(float,chance_to_injure,"#float, out of 100",10.0), \
(int,clip_size,"#int, bullets",7), \
(float,cooldown_between_shots,"#float, seconds",1.3), \
(float,critical_chance,"#float, out of 100",33.3), \
(int,critical_range,"#int, rooms",3), \
(float,damage_per_second,"#float, out of 100",44.0), \
(float,disorient_amount,"#float, out of 100",3.0), \
(float,headshot_bonus,"#float, static addition to hitroll",48.00), \
(int,max_range,"#int, rooms",8), \
(float,range_multiplier,"#float, TBA",1.0), \
(float,reload_time,"#float, seconds",2.3), \
(int,rounds_per_minute,"#int, bullets",8), \
(int,muzzle_velocity,"#int, TBA",1.0), \
(int,effective_firing_range,"#int, rooms",2) \
)


#define MENTOC_EXPLOSIVE_TYPES_SEQUENCE (FRAG_GRENADE) \
(INCENDIARY_GRENADE) \
(REMOTE_EXPLOSIVE) \
(REMOTE_CHEMICAL) \
(EMP_GRENADE) \
(CLAYMORE_MINE) \
(SMOKE_GRENADE) \
(C4) \
(BREACH_CHARGE) \
(FLASHBANG_GRENADE) \
(SENSOR_GRENADE) 


#define MENTOC_EXPLOSIVE_MEMBERS_TUPLE ( \
(int,alternate_explosion_type,"#int, enum 0 = NONE, 1 = SCAN",0), \
(percent_t,chance_to_injure,"#float, percent",3.0), \
(percent_t,critical_chance,"#float, percent",14.3), \
(rooms_t,critical_range,"#int, rooms",2), \
(rooms_t,blast_radius,"#int, rooms",2), \
(static_amount_t,damage_per_second,"#int, static base damage done",44.0), \
(static_amount_t,disorient_amount,"#int, percent",13.0), \
(int,loudness_type,"#int, 0=SILENT,1=SUSPICIOUS,2=LOUD,3=DEAFENING,4=WORLD_ENDING",2) \
)

#define MENTOC_DRONE_TYPES_SEQUENCE (GROUND_DRONE) \
(AERIAL_DRONE) \
(AQUATIC_DRONE) 

#define MENTOC_DRONE_MEMBERS_TUPLE ( \
(float,hover_airtime,"#float, seconds",4.0), \
(int,radio_range,"#int, rooms",4), \
(int,ammo_max,"#int, bullets",33), \
(float,recharge_rate,"#float, percentage per second",2.5), \
(float,discharge_rate,"#float, percentage battery is reduced per shot",38.0), \
(int,self_destruct,"#int, 0 = NEVER, 1 = CAN DESTRUCT, 2 = KAMIKAZE",1), \
(float,health_points,"#float, no limit",60.00), \
(float,move_points,"#float, no limit",50.00), \
(int,emp_range,"#int, rooms. 0 = disabled",0), \
(int,spray_range,"#int, rooms. 0 = disabled",0), \
(std::string,spray_type,"#string, enum. TBE. leave empty to disable","'DISABLED'") \
)


#define MENTOC_GADGET_TYPES_SEQUENCE (GRAPPLING_HOOK) \
(BARBED_WIRE) \
(CAMERA) \
(MOTION_SENSOR) \
(DEPLOYABLE_SHIELD) \
(TRIPWIRE) 

#define MENTOC_GADGET_MEMBERS_TUPLE ( \
(std::string,csv_capabilities,"#csv, strings free-form TBA","'TBA'"), \
(std::string,csv_attach_to,"#csv, strings attach to various kinds of items TBA","'TBA'") \
)

#define MENTOC_ATTACHMENT_TYPES_SEQUENCE (SIGHT) \
(MUZZLE) \
(MAGAZINE) 

#define MENTOC_ATTACHMENT_MEMBERS_TUPLE ( \
(float,zoom_multiplier,"#float","2.0"), \
(static_amount_t,ammunition_amount,"#int","32"), \
(percent_t,silence_percent,"#float, percentage","10.0"), \
(static_amount_t,durability_points,"#int, static amount of durability points, no limit","100"), \
(static_amount_t,incendiary_damage,"#int, static amount of incendiary damage, no limit","10"), \
(static_amount_t,radiation_damage,"#int, static amount of radiation damage, no limit","10"), \
(static_amount_t,poison_damage,"#int, static amount of poison damage, no limit","10"), \
(static_amount_t,electric_damage,"#int, static amount of electric damage, no limit","10"), \
(static_amount_t,armor_penetration_amount,"#int, static amount of armor penetration done, no limit","10"), \
(static_amount_t,disorient_amount,"#int, static amount of disorientation done, no limit","10"), \
(static_amount_t,emp_amount,"#int, static amount of EMP done, no limit","10"), \
(rooms_t,emp_radius,"#int, radius size in rooms","1"), \
(std::string,attach_to,"#csv, strings attach to various kinds of items TBA","'TBA'") \
)

#define MENTOC_ARMOR_TYPES_SEQUENCE (VEST) \
(LEGS) \
(GAUNTLETS) \
(SHOULDER_PADS) \
(HELMET) \
(HAT) \
(EYEWEAR) \
(GOGGLES) \
(BACKPACK) \
(WAIST_PACK) \
(VEST_PACK) \
(ELBOW_GUARDS) \
(GLOVES) \
(SHIELD)

#define MENTOC_ARMOR_MEMBERS_TUPLE ( \
(std::string,csv_capabilities,"#csv, strings free-form TBA","'TBA'"), \
(std::string,csv_attach_to,"#csv, strings attach to various kinds of items TBA","'TBA'"), \
(int,thac0,"#int",50), \
(float,weight_in_lbs,"#float, weight in lbs",5.0), \
(percent_t,fire_resistance_percent,"#percent",15.0), \
(percent_t,balistic_resistance_percent,"#percent",15.0), \
(int,speed_profile,"#int, ENUM 0=UNHINDERED,1=FAST,2=HINDERED,3=SLOW,4=SLUGGISH",0), \
(static_amount_t,offensive_damage_amount,"#int, static amount of offensive damage",0), \
(int,durability_profile,"#int, ENUM 0=FLIMSY,1=DECENT,2=DURABLE,3=HARDENED,4=INDUSTRIAL_STRENGTH,5=GODLIKE,6=INDESTRUCTIBLE",1), \
(int,repair_cost_profile,"#int, ENUM 0=CHEAP,1=DECENT,2=COSTLY,3=VERY_EXPENSIVE",1) \
)

#define MENTOC_CONSUMABLE_TYPES_SEQUENCE (AMMUNITION) \
(FOOD) \
(PILLS) \
(HERBS) \
(HALLUCINOGENS) \
(PEDS) \
(GASOLINE) 

#define MENTOC_CONSUMABLE_MEMBERS_TUPLE ( \
(std::string,csv_capabilities,"#csv, strings free-form TBA","'TBA'"), \
(std::string,csv_attach_to,"#csv, strings attach to various kinds of items TBA","'TBA'") \
)


#define MENTOC_TRAP_TYPES_SEQUENCE (BEAR_TRAP) \
	(POSITION_AMPLIFIER)

#define MENTOC_TRAP_MEMBERS_TUPLE ( \
(int,explosion_type,"#int, enum 0 = NONE, 1 = SCAN",0), \
(percent_t,chance_to_injure,"#float, percent",3.0), \
(percent_t,critical_chance,"#float, percent",14.3), \
(rooms_t,blast_radius,"#int, rooms",2), \
(static_amount_t,damage_per_second,"#int, static base damage done",44.0), \
(static_amount_t,disorient_amount,"#int, percent",13.0), \
(int,loudness_type,"#int, 0=SILENT,1=SUSPICIOUS,2=LOUD,3=DEAFENING,4=WORLD_ENDING",2) \
)


#define MENTOC_EXPLOSIVE_MEMBERS_TUPLE ( \
(int,alternate_explosion_type,"#int, enum 0 = NONE, 1 = SCAN",0), \
(percent_t,chance_to_injure,"#float, percent",3.0), \
(percent_t,critical_chance,"#float, percent",14.3), \
(rooms_t,critical_range,"#int, rooms",2), \
(rooms_t,blast_radius,"#int, rooms",2), \
(static_amount_t,damage_per_second,"#int, static base damage done",44.0), \
(static_amount_t,disorient_amount,"#int, percent",13.0), \
(int,loudness_type,"#int, 0=SILENT,1=SUSPICIOUS,2=LOUD,3=DEAFENING,4=WORLD_ENDING",2) \
)


#define MENTOC_ITEM_TYPE_LIST_SEQUENCE (RIFLE) \
(EXPLOSIVE) \
(DRONE) \
(GADGET) \
(ATTACHMENT) \
(ARMOR)  \
(CONSUMABLE) \
(TRAP)



/**************************************************************************/
/** aliases for each class type                                           */
/**************************************************************************/
#define MENTOC_USING_CT_IMPL(r,data,CLASS_TYPE)\
using BOOST_PP_CAT(mw_,CLASS_TYPE) = mods::weapon::type::CLASS_TYPE;

#define MENTOC_USING_CT \
BOOST_PP_SEQ_FOR_EACH(MENTOC_USING_CT_IMPL, ~, MENTOC_ITEM_TYPES_SEQ)



/**************************************************************************/
/** forward declarations of each *_description_t class                    */
/**************************************************************************/
#define MENTOC_YAML_DESC_IMPL(r,data,T) struct BOOST_PP_CAT(T,_description_t);
#define MENTOC_YAML_DESC \
BOOST_PP_SEQ_FOR_EACH(MENTOC_YAML_DESC_IMPL, ~, MENTOC_ITEM_TYPES_SEQ)

#define MENTOC_DECLARE_DATA_STRUCTS_IMPL(r,data,CLASS_TYPE)\
struct BOOST_PP_CAT(CLASS_TYPE,_data_t);



/**************************************************************************/
/** declarations for each *_data_t class                                  */
/**************************************************************************/
#define MENTOC_DECLARE_DATA_STRUCTS \
BOOST_PP_SEQ_FOR_EACH(MENTOC_DECLARE_DATA_STRUCTS_IMPL,~,MENTOC_ITEM_TYPES_SEQ)



/**************************************************************************/
/** obj_data::feed(pqxx::row&) switch+case expansion                      */
/**************************************************************************/
#define MENTOC_OBJ_DATA_FEED_SWITCH_IMPL(IN_R,data,CLASS_TYPE)\
	case CLASS_TYPE: if(obj_file.length() == 0){\
										 obj_file = default_yaml_file( BOOST_PP_STRINGIZE(BOOST_PP_ELEM(IN_R,MENTOC_ITEM_TYPES_SEQ)) );\
			 }; this->feed(BOOST_PP_STRINGIZE(CLASS_TYPE), row["obj_file"].as<std::string>());\
			 break;

#define MENTOC_OBJ_DATA_FEED_SWITCH \
BOOST_PP_SEQ_FOR_EACH(MENTOC_OBJ_DATA_FEED_SWITCH_IMPL, ~, MENTOC_ITEM_TYPES_SEQ_CAPS)




#define MENTOC_ENUM_TYPES_IMPL(r,data,ENUM_TYPE) ENUM_TYPE = BOOST_PP_ADD(r,1) BOOST_PP_COMMA()


/**************************************************************************/
/** FEED/EXAMPLE FILE MACROS                                              */
/**************************************************************************/
#define MENTOC_FEED_TYPE_IMPL(r,data,TYPE) \
		if(type_string.compare(BOOST_PP_STRINGIZE(TYPE)) == 0){\
			type = data::TYPE;\
		}

#define MENTOC_EXAMPLE_IMPL(r,data,TYPE) \
	out_file << "# type: " << BOOST_PP_STRINGIZE(TYPE) << "\n";

#define MENTOC_MEMBER_VARS_EXAMPLE_IMPL(r,data,MEMBER_TUPLE) \
			out_file << BOOST_PP_STRINGIZE(                        \
					BOOST_PP_TUPLE_ELEM(4,1,MEMBER_TUPLE)        \
					) << ": " <<                                       \
						BOOST_PP_TUPLE_ELEM(4,3,MEMBER_TUPLE)      \
					  <<                                               \
						BOOST_PP_TUPLE_ELEM(4,2,MEMBER_TUPLE)      \
						<< "\n";

#define MENTOC_MEMBER_VARS_EXAMPLE_FOR(SEQUENCE) \
			BOOST_PP_SEQ_FOR_EACH(MENTOC_MEMBER_VARS_EXAMPLE_IMPL,~,BOOST_PP_TUPLE_TO_SEQ(BOOST_PP_TUPLE_SIZE(SEQUENCE),SEQUENCE))

#define MENTOC_MEMBER_VARS_FOR(SEQUENCE) \
			BOOST_PP_SEQ_FOR_EACH(MENTOC_MEMBER_VARS_IMPL,~,BOOST_PP_TUPLE_TO_SEQ(BOOST_PP_TUPLE_SIZE(SEQUENCE),SEQUENCE))

#define MENTOC_MEMBER_VARS_IMPL(r,data,MEMBER_TUPLE) \
	BOOST_PP_TUPLE_ELEM(4,0,MEMBER_TUPLE) BOOST_PP_TUPLE_ELEM(4,1,MEMBER_TUPLE);

#define MENTOC_FEED_PARSE_ACTUAL_IMPL(r,data,MEMBER_TUPLE) \
	std::cerr << "[feed parse actual]: " << BOOST_PP_STRINGIZE(BOOST_PP_TUPLE_ELEM(4,1,MEMBER_TUPLE)) << "\n";\
	BOOST_PP_TUPLE_ELEM(4,1,MEMBER_TUPLE) = yaml_file[BOOST_PP_STRINGIZE(BOOST_PP_TUPLE_ELEM(4,1,MEMBER_TUPLE))].as<BOOST_PP_TUPLE_ELEM(4,0,MEMBER_TUPLE)>();

#define MENTOC_FEED_PARSE_ALL_IMPL(SEQUENCE) \
			BOOST_PP_SEQ_FOR_EACH(MENTOC_FEED_PARSE_ACTUAL_IMPL,~,BOOST_PP_TUPLE_TO_SEQ(BOOST_PP_TUPLE_SIZE(SEQUENCE),SEQUENCE))



/**************************************************************************/
/** GADGETS                                                               */
/**************************************************************************/
#define MENTOC_FEED_GADGET \
	BOOST_PP_SEQ_FOR_EACH(MENTOC_FEED_TYPE_IMPL,mw_gadget,MENTOC_GADGET_TYPES_SEQUENCE) \
	MENTOC_FEED_PARSE_ALL_IMPL(MENTOC_GADGET_MEMBERS_TUPLE)

#define MENTOC_EXAMPLE_GADGETS \
	BOOST_PP_SEQ_FOR_EACH(MENTOC_EXAMPLE_IMPL,mw_gadget,MENTOC_GADGET_TYPES_SEQUENCE)


/**************************************************************************/
/** DRONES                                                                */
/**************************************************************************/
#define MENTOC_FEED_DRONE \
	BOOST_PP_SEQ_FOR_EACH(MENTOC_FEED_TYPE_IMPL,mw_drone,MENTOC_DRONE_TYPES_SEQUENCE) \
	MENTOC_FEED_PARSE_ALL_IMPL(MENTOC_DRONE_MEMBERS_TUPLE)

#define MENTOC_EXAMPLE_DRONES \
	BOOST_PP_SEQ_FOR_EACH(MENTOC_EXAMPLE_IMPL,mw_drone,MENTOC_DRONE_TYPES_SEQUENCE)

/**************************************************************************/
/** ATTACHMENTS                                                           */
/**************************************************************************/
#define MENTOC_FEED_ATTACHMENT \
	BOOST_PP_SEQ_FOR_EACH(MENTOC_FEED_TYPE_IMPL,mw_attachment,MENTOC_ATTACHMENT_TYPES_SEQUENCE) \
	MENTOC_FEED_PARSE_ALL_IMPL(MENTOC_ATTACHMENT_MEMBERS_TUPLE)

#define MENTOC_EXAMPLE_ATTACHMENTS \
	BOOST_PP_SEQ_FOR_EACH(MENTOC_EXAMPLE_IMPL,mw_attachment,MENTOC_ATTACHMENT_TYPES_SEQUENCE)


/**************************************************************************/
/** RIFLES                                                                */
/**************************************************************************/
#define MENTOC_FEED_RIFLE \
	BOOST_PP_SEQ_FOR_EACH(MENTOC_FEED_TYPE_IMPL,mw_rifle,MENTOC_RIFLE_TYPES_SEQUENCE) \
	MENTOC_FEED_PARSE_ALL_IMPL(MENTOC_RIFLE_MEMBERS_TUPLE)

#define MENTOC_EXAMPLE_RIFLES \
	BOOST_PP_SEQ_FOR_EACH(MENTOC_EXAMPLE_IMPL,mw_rifle,MENTOC_RIFLE_TYPES_SEQUENCE)


/**************************************************************************/
/** EXPLOSIVES                                                            */
/**************************************************************************/
#define MENTOC_FEED_EXPLOSIVE \
	BOOST_PP_SEQ_FOR_EACH(MENTOC_FEED_TYPE_IMPL,mw_explosive,MENTOC_EXPLOSIVE_TYPES_SEQUENCE) \
	MENTOC_FEED_PARSE_ALL_IMPL(MENTOC_EXPLOSIVE_MEMBERS_TUPLE)

#define MENTOC_EXAMPLE_EXPLOSIVES \
	BOOST_PP_SEQ_FOR_EACH(MENTOC_EXAMPLE_IMPL,mw_explosive,MENTOC_EXPLOSIVE_TYPES_SEQUENCE)

/**************************************************************************/
/** ARMORS                                                                */
/**************************************************************************/
#define MENTOC_FEED_ARMOR \
	BOOST_PP_SEQ_FOR_EACH(MENTOC_FEED_TYPE_IMPL,mw_armor,MENTOC_ARMOR_TYPES_SEQUENCE) \
	MENTOC_FEED_PARSE_ALL_IMPL(MENTOC_ARMOR_MEMBERS_TUPLE)

#define MENTOC_EXAMPLE_ARMORS \
	BOOST_PP_SEQ_FOR_EACH(MENTOC_EXAMPLE_IMPL,mw_armor,MENTOC_ARMOR_TYPES_SEQUENCE)

/**************************************************************************/
/** CONSUMABLES                                                               */
/**************************************************************************/
#define MENTOC_FEED_CONSUMABLE \
	BOOST_PP_SEQ_FOR_EACH(MENTOC_FEED_TYPE_IMPL,mw_consumable,MENTOC_CONSUMABLE_TYPES_SEQUENCE) \
	MENTOC_FEED_PARSE_ALL_IMPL(MENTOC_CONSUMABLE_MEMBERS_TUPLE)

#define MENTOC_EXAMPLE_CONSUMABLE \
	BOOST_PP_SEQ_FOR_EACH(MENTOC_EXAMPLE_IMPL,mw_consumable,MENTOC_CONSUMABLE_TYPES_SEQUENCE)

/**************************************************************************/
/** TRAPS                                                                 */
/**************************************************************************/
#define MENTOC_FEED_TRAP \
	BOOST_PP_SEQ_FOR_EACH(MENTOC_FEED_TYPE_IMPL,mw_trap,MENTOC_TRAP_TYPES_SEQUENCE) \
	MENTOC_FEED_PARSE_ALL_IMPL(MENTOC_TRAP_MEMBERS_TUPLE)

#define MENTOC_EXAMPLE_TRAP \
	BOOST_PP_SEQ_FOR_EACH(MENTOC_EXAMPLE_IMPL,mw_trap,MENTOC_TRAP_TYPES_SEQUENCE)

/*********************************************************/
/** HOWTO: Add new item and subcategories                */
/* Step 4: Add a new FEED_* and EXAMPLE_* constants      */
/*********************************************************/



#endif