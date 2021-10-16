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
(trap) \
(container) \
(melee) \
(vehicle)

#define MENTOC_ITEM_TYPES_CAPS_SEQ \
(RIFLE) \
(EXPLOSIVE) \
(DRONE) \
(GADGET) \
(ATTACHMENT) \
(ARMOR) \
(CONSUMABLE) \
(TRAP) \
(CONTAINER) \
(MELEE) \
(VEHICLE)

#define MENTOC_ITEM_TYPES_BY_CAPS_SEQ \
(rifle,RIFLE) \
(explosive,EXPLOSIVE) \
(drone,DRONE) \
(gadget,GADGET) \
(attachment,ATTACHMENT) \
(armor,ARMOR) \
(consumable,CONSUMABLE) \
(trap,TRAP) \
(container,CONTAINER) \
(melee,MELEE) \
(vehicle,VEHICLE)

#define MENTOC_WEAR_PLACES_SEQ \
(LIGHT) \
(FINGER_R) \
(FINGER_L) \
(NECK_1) \
(NECK_2) \
(BODY) \
(HEAD) \
(LEGS) \
(FEET) \
(HANDS) \
(ARMS) \
(SHIELD) \
(ABOUT) \
(WAIST) \
(WRIST_R) \
(WRIST_L) \
(WIELD) \
(PRIMARY) \
(HOLD) \
(SECONDARY_WEAPON) \
(SECONDARY) \
(WEAPON_ATTACHMENT) \
(SHOULDERS_L) \
(SHOULDERS_R) \
(BACKPACK) \
(GOGGLES) \
(VEST_PACK) \
(ELBOW_L) \
(ELBOW_R)


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
(ITEM_TRAP) \
(ITEM_CONTAINER) \
(ITEM_MELEE) \
(ITEM_VEHICLE)



/**************************************************************************/
/** ENUM MACROS AND SEQUENCES                                             */
/**************************************************************************/
/*********************************************************/
/** HOWTO: Add new item and subcategories                */
/* Step 3: Add a new *_TYPES_SEQUENCE for sub categories */
/*********************************************************/
#define MENTOC_CONTAINER_TYPES_SEQUENCE (ARMOR_LOCKER) \
	(WEAPON_LOCKER) \
	(LOCKER) \
	(EXPLOSIVE_BARREL) \
	(BILE_BARREL) \
	(WATER_BARREL) \
	(GASOLINE_BARREL) \
	(GAS_STATION_PUMP)

#define MENTOC_CONTAINER_MEMBERS_TUPLE ( \
(std::string,detailed_description,"#string, description","'Ammo locker.'") \
)

#define MENTOC_VEHICLE_TYPES_SEQUENCE (SUV) \
(MOTORCYCLE) \
(TRUCK) \
(SEMI) \
(VAN) \
(SWAT_VAN) \
(TANK) \
(SPORTS_CAR) \
(CONVERTIBLE) \
(FOUR_DOOR_SEDAN) \
(JEEP)

#define MENTOC_VEHICLE_MEMBERS_TUPLE ( \
(int32_t,hp,"#int32_t, hitpoints",250), \
(int32_t,armor_hp,"#int32_t, armor hitpoints",0), \
(std::string,vehicle_type,"#string, SUV, MOTORCYCLE,TWO_DOOR,FOUR_DOOR,TRUCK,SEMI,VAN,SWAT_VAN,TANK,JEEP","'SUV'"), \
(int32_t,horsepower,"#int32_t, horsepower", 1000), \
(int16_t,zero_to_sixty,"#int16_t, amount in seconds to go from zero to 60mph",10), \
(int32_t,tire_hp,"#int32_t, tire hp",150), \
(std::string,tire_types,"#string, tire types AIR,AIRLESS,TRACKS (tank)","'AIR'"), \
(std::string,front_windshield_glass_type,"#string, glass types, BULLET_PROOF,STOCK_GLASS,BULLET_PROOF_TINTED,STOCK_TINTED", "'STOCK_GLASS'"), \
(std::string,rear_windshield_glass_type,"#string, glass types, BULLET_PROOF,STOCK_GLASS,BULLET_PROOF_TINTED,STOCK_TINTED", "'STOCK_GLASS'"), \
(int16_t,passenger_count,"#int16_t, passenger count", 2) \
)


#define MENTOC_RIFLE_TYPES_SEQUENCE (SHOTGUN) \
(ASSAULT_RIFLE) \
(SUB_MACHINE_GUN) \
(SNIPER) \
(HANDGUN) \
(PISTOL) \
(MACHINE_PISTOL) \
(LIGHT_MACHINE_GUN)

#define MENTOC_RIFLE_MEMBERS_TUPLE ( \
(int16_t,ammo_max,"#int16_t, bullets",30), \
(std::string,ammo_type,"#string, enum","'SNIPER'"), \
(float,chance_to_injure,"#float, out of 100",10.0), \
(int16_t,clip_size,"#int16_t, bullets",7), \
(int16_t,cooldown_between_shots,"#int16_t, ticks",2), \
(int16_t,critical_chance,"#int16_t, out of 100",33), \
(int16_t,critical_range,"#int16_t, rooms",3), \
(int,base_damage,"#int, out of 100",44), \
(float,disorient_amount,"#float, out of 100",3.0), \
(float,headshot_bonus,"#float, static addition to hitroll",48.00), \
(int16_t,max_range,"#int16_t, rooms",8), \
(float,range_multiplier,"#float, TBA",1.0), \
(int16_t,reload_time,"#int16_t, ticks",3), \
(int16_t,rounds_per_minute,"#int16_t, bullets",8), \
(int16_t,muzzle_velocity,"#int16_t, TBA",1), \
(int16_t,effective_firing_range,"#int16_t, rooms",2), \
(int16_t,damage_dice_count,"#int16_t, number of dice",2), \
(int16_t,damage_dice_sides,"#int16_t, dice sides",6), \
(int16_t,incendiary_damage,"#int16_t, damage",0), \
(int16_t,explosive_damage,"#int16_t, damage",0), \
(int16_t,shrapnel_damage,"#int16_t, damage",0), \
(int16_t,corrosive_damage,"#int16_t, damage",0), \
(int16_t,cryogenic_damage,"#int16_t, damage",0), \
(int16_t,radioactive_damage,"#int16_t, damage",0), \
(int16_t,emp_damage,"#int16_t, damage",0), \
(int16_t,shock_damage,"#int16_t, damage",0), \
(int16_t,anti_matter_damage,"#int16_t, damage",0), \
(int16_t,stat_strength,"#int16_t, positive or negative amount to affect strength stat",0), \
(int16_t,stat_intelligence,"#int16_t, positive or negative amount to affect intelligence stat",0), \
(int16_t,stat_wisdom,"#int16_t, positive or negative amount to affect wisdom stat",0), \
(int16_t,stat_dexterity,"#int16_t, positive or negative amount to affect dexterity stat",0), \
(int16_t,stat_constitution,"#int16_t, positive or negative amount to affect constitution stat",0), \
(int16_t,stat_electronics,"#int16_t, positive or negative amount to affect electronics stat",0), \
(int16_t,stat_armor,"#int16_t, positive or negative amount to affect armor stat",0), \
(int16_t,stat_marksmanship,"#int16_t, positive or negative amount to affect marksmanship stat",0), \
(int16_t,stat_sniping,"#int16_t, positive or negative amount to affect sniping stat",0), \
(int16_t,stat_demolitions,"#int16_t, positive or negative amount to affect demolitions stat",0), \
(int16_t,stat_chemistry,"#int16_t, positive or negative amount to affect chemistry stat",0), \
(int16_t,stat_weapon_handling,"#int16_t, positive or negative amount to affect weapon_handling stat",0), \
(int16_t,stat_strategy,"#int16_t, positive or negative amount to affect strategy stat",0), \
(int16_t,stat_medical,"#int16_t, positive or negative amount to affect medical stat",0) \
)

#define MENTOC_MELEE_TYPES_SEQUENCE (MACHETE) \
(KNIFE) \
(CROWBAR) \
(CHAIN) \
(WOODEN_BASEBALL_BAT) \
(ALUMINUM_BASEBALL_BAT) \
(HAMMER) \
(SLEDGEHAMMER) \
(BATON) \
(HOCKEY_STICK) \
(AXE) \
(SHOVEL) \
(PICKAXE) \
(KATANA) \
(MACE) \
(LONGSWORD) \
(NUNCHAKU) \
(NAGINATA) \
(DAGGER) \
(SCYTHE) \
(SHURIKEN) \
(SPEAR) \
(PIKE) \
(HALBERD) \
(LANCE) \
(TRIDENT) \
(QUARTERSTAFF) \
(REBAR) \
(SICKLE) \
(ICE_PICK) \
(CHAINSAW) \
(BRASS_KNUCKLES)

#define MENTOC_MELEE_MEMBERS_TUPLE ( \
(float,chance_to_injure,"#float, out of 100",10.0), \
(int16_t,critical_chance,"#int16_t, out of 100",33), \
(int,base_damage,"#int, base damage done",44), \
(float,disorient_amount,"#float, out of 100",3.0), \
(int16_t,damage_dice_count,"#int16_t, number of dice",2), \
(int16_t,damage_dice_sides,"#int16_t, dice sides",6), \
(int16_t,incendiary_damage,"#int16_t, damage",0), \
(int16_t,explosive_damage,"#int16_t, damage",0), \
(int16_t,shrapnel_damage,"#int16_t, damage",0), \
(int16_t,corrosive_damage,"#int16_t, damage",0), \
(int16_t,cryogenic_damage,"#int16_t, damage",0), \
(int16_t,radioactive_damage,"#int16_t, damage",0), \
(int16_t,emp_damage,"#int16_t, damage",0), \
(int16_t,shock_damage,"#int16_t, damage",0), \
(int16_t,anti_matter_damage,"#int16_t, damage",0), \
(int16_t,stat_strength,"#int16_t, positive or negative amount to affect strength stat",0), \
(int16_t,stat_intelligence,"#int16_t, positive or negative amount to affect intelligence stat",0), \
(int16_t,stat_wisdom,"#int16_t, positive or negative amount to affect wisdom stat",0), \
(int16_t,stat_dexterity,"#int16_t, positive or negative amount to affect dexterity stat",0), \
(int16_t,stat_constitution,"#int16_t, positive or negative amount to affect constitution stat",0), \
(int16_t,stat_electronics,"#int16_t, positive or negative amount to affect electronics stat",0), \
(int16_t,stat_armor,"#int16_t, positive or negative amount to affect armor stat",0), \
(int16_t,stat_marksmanship,"#int16_t, positive or negative amount to affect marksmanship stat",0), \
(int16_t,stat_sniping,"#int16_t, positive or negative amount to affect sniping stat",0), \
(int16_t,stat_demolitions,"#int16_t, positive or negative amount to affect demolitions stat",0), \
(int16_t,stat_chemistry,"#int16_t, positive or negative amount to affect chemistry stat",0), \
(int16_t,stat_weapon_handling,"#int16_t, positive or negative amount to affect weapon_handling stat",0), \
(int16_t,stat_strategy,"#int16_t, positive or negative amount to affect strategy stat",0), \
(int16_t,stat_medical,"#int16_t, positive or negative amount to affect medical stat",0) \
)


#define MENTOC_ELEMENTAL_TYPES_SEQUENCE (ELEM_INCENDIARY) \
(ELEM_EXPLOSIVE) \
(ELEM_SHRAPNEL) \
(ELEM_CORROSIVE) \
(ELEM_CRYOGENIC) \
(ELEM_RADIOACTIVE) \
(ELEM_EMP) \
(ELEM_SHOCK) \
(ELEM_ANTI_MATTER) \

#define MENTOC_EXPLOSIVE_TYPES_SEQUENCE (FRAG_GRENADE) \
(INCENDIARY_GRENADE) \
(REMOTE_EXPLOSIVE) \
(REMOTE_CHEMICAL) \
(EMP_GRENADE) \
(CLAYMORE_MINE) \
(SMOKE_GRENADE) \
(C4) \
(BREACH_CHARGE) \
(THERMITE_CHARGE) \
(FLASHBANG_GRENADE) \
(SENSOR_GRENADE)


#define MENTOC_EXPLOSIVE_MEMBERS_TUPLE ( \
(std::string,alternate_explosion_type,"#int, ENUM  NONE,SCAN,CORROSIVE,SHRAPNEL","'NONE'"), \
(float,chance_to_injure,"#float, percent",3.0), \
(float,critical_chance,"#float, percent",14.3), \
(int,critical_range,"#int, rooms",2), \
(int,blast_radius,"#int, rooms",2), \
(int,base_damage,"#int, static base damage done",44), \
(float,disorient_amount,"#float, percent",13.0), \
(int,range_modifier,"#int, positive or negative, range modifier in rooms",0), \
(std::string,loudness_type,"#std::string, ENUM SILENT,SUSPICIOUS,LOUD,DEAFENING,WORLD_ENDING","'LOUD'"), \
(int,incendiary_damage_dice_count,"#int, dice count of incendiary damage, no limit",6), \
(int,incendiary_damage_dice_sides,"#int, dice sides of incendiary damage, no limit",6), \
(int,radiation_damage_dice_count,"#int, dice count of radiation damage, no limit",6), \
(int,radiation_damage_dice_sides,"#int, dice sides of radiation damage, no limit",6), \
(int,chemical_damage_dice_count,"#int, dice count of chemical damage, no limit",6), \
(int,chemical_damage_dice_sides,"#int, dice sides of chemical damage, no limit",6), \
(int,electric_damage_dice_count,"#int, dice count of electric damage, no limit",6), \
(int,electric_damage_dice_sides,"#int, dice sides of electric damage, no limit",6), \
(int,armor_penetration_damage_dice_count,"#int, dice count of armor penetration done, no limit",6), \
(int,armor_penetration_damage_dice_sides,"#int, dice sides of armor penetration done, no limit",6), \
(int,damage_dice_count,"#int, number of dice",2), \
(int,damage_dice_sides,"#int, dice sides",6), \
(static_amount_t,incendiary_damage_percent,"#int, static amount of incendiary damage_percent, no limit",0), \
(static_amount_t,explosive_damage_percent,"#int, static amount of explosive damage_percent, no limit",0), \
(static_amount_t,shrapnel_damage_percent,"#int, static amount of shrapnel damage_percent, no limit",0), \
(static_amount_t,corrosive_damage_percent,"#int, static amount of corrosive damage_percent, no limit",0), \
(static_amount_t,cryogenic_damage_percent,"#int, static amount of cryogenic damage_percent, no limit",0), \
(static_amount_t,radiation_damage_percent,"#int, static amount of radiation damage_percent, no limit",0), \
(static_amount_t,emp_damage_percent,"#int, static amount of emp damage_percent, no limit",0), \
(static_amount_t,shock_damage_percent,"#int, static amount of shock damage_percent, no limit",0), \
(static_amount_t,anti_matter_damage_percent,"#int, static amount of anti_matter damage_percent, no limit",0) \
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
(THERMAL_CAMERA) \
(NIGHT_VISION_CAMERA) \
(MOTION_SENSOR) \
(DEPLOYABLE_SHIELD) \
(TRIPWIRE) \
(DOOR_CAMERA) \
(CB_RADIO) \
(KEY_FOB)

#define MENTOC_GADGET_MEMBERS_TUPLE ( \
(std::string,csv_capabilities,"#csv, strings free-form TBA","'TBA'"), \
(std::string,csv_attach_to,"#csv, strings attach to various kinds of items TBA","'TBA'"), \
(int,electronic,"#int, 1 or zero. If item is electronic",0), \
(int,health_points,"#int, health points before it dies. -1 is invincible",100), \
(int,shield_points,"#int, additional points to decrement before decrementing health points. -1 is invincible",1), \
(std::string,durability_profile,"#string, ENUM FLIMSY,DECENT,DURABLE,HARDENED,INDUSTRIAL_STRENGTH,GODLIKE,INDESTRUCTIBLE","'DECENT'"), \
(int,move_points,"#int, number of move points required to circumvent gadget",0), \
(int,damage_points,"#int, number of damage points acrued when tripped/touched",0), \
(int,exit_key,"#int, numeric identifier of door that this object opens.",0), \
(int,color,"#int, enum. 0=black, 1=red, 2=yellow, 3=green, 4=blue, 5=white",0), \
(int,min_level,"#int, the minimum level you need to be in order to use this gadget.",0), \
(int,worth,"#int, how much the item is worth.",0) \
)

#define MENTOC_ATTACHMENT_TYPES_SEQUENCE (SIGHT) \
(UNDER_BARREL) \
(GRIP) \
(BARREL) \
(MUZZLE) \
(MAGAZINE) \
(STOCK) \
(STRAP)

#define MENTOC_ATTACHMENT_MEMBERS_TUPLE ( \
(float,zoom_multiplier,"#float, magnification amount. for example: 6.0 means 6 times magnification",0.0), \
(int16_t,ammunition_amount,"#int16_t, number of bullets to add per magazine",0), \
(int16_t,loudness_reduction,"#int16_t, the higher the value the less noise it will make",0), \
(int16_t,durability_profile,"#int16_t, ENUM 0=FLIMSY,1=DECENT,2=DURABLE,3=HARDENED,4=INDUSTRIAL_STRENGTH,5=GODLIKE,6=INDESTRUCTIBLE",1), \
(int16_t,accuracy_points,"#int16_t,the higher the value, the more control",0), \
(int16_t,incendiary_damage,"#int16_t, amount of incendiary damage",0), \
(int16_t,radiation_damage,"#int16_t, amount of radiation damage",0), \
(int16_t,armor_penetration_amount,"#int16_t, amount of armor penetration done",0), \
(int16_t,disorient_amount,"#int16_t, amount of disorientation done",0), \
(int16_t,incendiary_damage_percent,"#int16_t,incendiary damage percent",0), \
(int16_t,explosive_damage_percent,"#int16_t,explosive damage percent",0), \
(int16_t,shrapnel_damage_percent,"#int16_t,shrapnel damage percent",0), \
(int16_t,corrosive_damage_percent,"#int16_t,corrosive damage percent",0), \
(int16_t,cryogenic_damage_percent,"#int16_t,cryogenic damage percent",0), \
(int16_t,radiation_damage_percent,"#int16_t,radiation damage percent",0), \
(int16_t,emp_damage_percent,"#int16_t,emp damage percent",0), \
(int16_t,shock_damage_percent,"#int16_t,shock damage percent",0), \
(int16_t,anti_matter_damage_percent,"#int16_t,anti_matter damage percent",0), \
(int16_t,recoil_reduction,"#int16_t,the higher the value, the less recoil",0), \
(int16_t,free_ammo_chance,"#int16_t,the higher the value, the less likely the weapon will use ammunition",0), \
(int16_t,regenerate_ammo_chance,"#int16_t,the higher the value, the more likely the weapon will use generate ammunition",0), \
(int16_t,bleed_chance,"#int16_t,the higher the value, the more likely the weapon will cause the target to bleed",0), \
(int16_t,aimed_limb_accuracy_percent,"#int16_t,the higher the value, the more likely aimed limb shots will land successfully",0), \
(std::string,underbarrel_launcher_type,"#string, adds an underbarrel launcher of the specified type NONE,EMP_LAUNCHER,BREACH_LAUNCHER,THERMITE_BREACH_LAUNCHER,FRAG_LAUNCHER,SENSOR_LAUNCHER,SMOKE_LAUNCHER,INCENDIARY_LAUNCHER,SHOTGUN,TASER,CORROSIVE_SPRAY,FLAME_THROWER","'NONE'"), \
(int16_t,damage_percent_bonus,"#int16_t, percent value multiplied by base weapon damage and added to every attack",0.0) \
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
(SHIELD) \
(WRIST) \
(MASK) \
(FEET) \
(WAIST) \
(NECK)

#define MENTOC_ARMOR_MEMBERS_TUPLE ( \
(std::string,csv_capabilities,"#csv, strings free-form TBA","'TBA'"), \
(std::string,csv_attach_to,"#csv, strings attach to various kinds of items TBA","'TBA'"), \
(int,thac0,"#int",50), \
(float,weight_in_lbs,"#float, weight in lbs",5.0), \
(percent_t,fire_resistance_percent,"#percent",15.0), \
(percent_t,balistic_resistance_percent,"#percent",15.0), \
(int,speed_profile,"#int, ENUM 0=UNHINDERED,1=FAST,2=HINDERED,3=SLOW,4=SLUGGISH",0), \
(static_amount_t,offensive_damage_amount,"#int, static amount of offensive damage",0), \
(std::string,durability_profile,"#string, ENUM FLIMSY,DECENT,DURABLE,HARDENED,INDUSTRIAL_STRENGTH,GODLIKE,INDESTRUCTIBLE","'DECENT'"), \
(int,repair_cost_profile,"#int, ENUM 0=CHEAP,1=DECENT,2=COSTLY,3=VERY_EXPENSIVE",1), \
(int,hp,"#int, hit points",1000), \
(std::string,classification,"#string, 'BASIC', 'ADVANCED', 'ELITE'","'BASIC'"), \
(int,worth,"#int, how much the item is worth.",0), \
(static_amount_t,incendiary_resistance_percent,"#int, static amount of incendiary resistance_percent, no limit",0), \
(static_amount_t,explosive_resistance_percent,"#int, static amount of explosive resistance_percent, no limit",0), \
(static_amount_t,shrapnel_resistance_percent,"#int, static amount of shrapnel resistance_percent, no limit",0), \
(static_amount_t,corrosive_resistance_percent,"#int, static amount of corrosive resistance_percent, no limit",0), \
(static_amount_t,cryogenic_resistance_percent,"#int, static amount of cryogenic resistance_percent, no limit",0), \
(static_amount_t,radiation_resistance_percent,"#int, static amount of radiation resistance_percent, no limit",0), \
(static_amount_t,emp_resistance_percent,"#int, static amount of emp resistance_percent, no limit",0), \
(static_amount_t,shock_resistance_percent,"#int, static amount of shock resistance_percent, no limit",0), \
(static_amount_t,anti_matter_resistance_percent,"#int, static amount of anti_matter resistance_percent, no limit",0), \
(int16_t,stat_strength,"#int16_t, positive or negative amount to affect strength stat",0), \
(int16_t,stat_intelligence,"#int16_t, positive or negative amount to affect intelligence stat",0), \
(int16_t,stat_wisdom,"#int16_t, positive or negative amount to affect wisdom stat",0), \
(int16_t,stat_dexterity,"#int16_t, positive or negative amount to affect dexterity stat",0), \
(int16_t,stat_constitution,"#int16_t, positive or negative amount to affect constitution stat",0), \
(int16_t,stat_electronics,"#int16_t, positive or negative amount to affect electronics stat",0), \
(int16_t,stat_armor,"#int16_t, positive or negative amount to affect armor stat",0), \
(int16_t,stat_marksmanship,"#int16_t, positive or negative amount to affect marksmanship stat",0), \
(int16_t,stat_sniping,"#int16_t, positive or negative amount to affect sniping stat",0), \
(int16_t,stat_demolitions,"#int16_t, positive or negative amount to affect demolitions stat",0), \
(int16_t,stat_chemistry,"#int16_t, positive or negative amount to affect chemistry stat",0), \
(int16_t,stat_weapon_handling,"#int16_t, positive or negative amount to affect weapon_handling stat",0), \
(int16_t,stat_strategy,"#int16_t, positive or negative amount to affect strategy stat",0), \
(int16_t,stat_medical,"#int16_t, positive or negative amount to affect medical stat",0) \
)

#define MENTOC_CONSUMABLE_TYPES_SEQUENCE (AMMUNITION) \
(FOOD) \
(PILLS) \
(HERBS) \
(HALLUCINOGENS) \
(PEDS) \
(GASOLINE) \
(TRANQUILIZER_DARTS)

#define MENTOC_CONSUMABLE_MEMBERS_TUPLE ( \
(std::string,csv_capabilities,"#csv, strings free-form TBA","'TBA'"), \
(std::string,csv_attach_to,"#csv, strings attach to various kinds of items TBA","'TBA'"), \
(int,weight_in_lbs,"#int, weight in lbs",2), \
(std::string,consumed_by,"#string, ENUM HUMAN,VEHICLE,ATTACHMENT,DRONE,GADGET,RIFLE,TRAP ",2), \
(int,adds_hp,"#int,  amount of hp it adds when consumed (can be negative)",0), \
(int,adds_movement,"#int, amount of movement it adds when consumed (can be negative)",0), \
(int,adds_room_range,"#int, amount of rooms it adds when consumed (can be negative)",0), \
(int,adds_strength,"#int, amount of STR it adds when consumed (can be negative)",0), \
(int,adds_intelligence,"#int, amount of INT it adds when consumed (can be negative)",0), \
(int,adds_charisma,"#int, amount of CHA it adds when consumed (can be negative)",0), \
(int,adds_dexterity,"#int, amount of DEX it adds when consumed (can be negative)",0), \
(int,adds_constitution,"#int, amount of CON it adds when consumed (can be negative)",0), \
(int,adds_wisdom,"#int, amount of WIS it adds when consumed (can be negative)",0), \
(int,adds_weight,"#int, amount of weight it adds when consumed (can be negative)",0), \
(int,adds_experience,"#int, amount of EXP it adds when consumed (can be negative)",0), \
(int,adds_armor_class,"#int, amount of AC it adds when consumed (can be negative)",0), \
(int,adds_hitroll,"#int, amount of HITROLL it adds when consumed (can be negative)",0), \
(int,adds_damage_roll,"#int, amount of DAMROLL it adds when consumed (can be negative)",0), \
(int,ticks_until_zero,"#int, amount of game ticks until consumable wears off (can be negative)",0), \
(std::string,ammo_type,"#string, ENUM any type of ITEM_RIFLE if this is ammunition","'NONE'"), \
(int,capacity,"#int, usually used for ammunition. how much of this consumable is present (i.e.: bullets)",0), \
(int,adds_fire_damage,"#int, amount of fire damage it adds to the consumer",0), \
(int,adds_critical_chance,"#int, amount of points to add to critical chance of consumer",0), \
(int,adds_ammo_max,"#int, adds to ammo maximum of consumer",30), \
(float,adds_chance_to_injure,"#float, out of 100",10.0), \
(int,adds_clip_size,"#int, bullets",7), \
(int,adds_cooldown_between_shots,"#int, ticks (can be negative)",0), \
(int,adds_critical_range,"#int, rooms (can be negative)",3), \
(float,adds_disorient_amount,"#float, out of 100",3.0), \
(float,adds_headshot_bonus,"#float, static addition to hitroll",48.00), \
(int,adds_max_range,"#int, rooms",8), \
(int,adds_reload_time,"#int, ticks",3), \
(int,adds_muzzle_velocity,"#int, TBA",1.0), \
(int,adds_damage_dice_count,"#int, number of dice",2), \
(int,adds_damage_dice_sides,"#int, dice sides",6) \
)


#define MENTOC_TRAP_TYPES_SEQUENCE (BEAR_TRAP) \
	(POSITION_AMPLIFIER)

#define MENTOC_TRAP_MEMBERS_TUPLE ( \
(int,explosion_type,"#int, enum 0 = NONE, 1 = SCAN",0), \
(percent_t,chance_to_injure,"#float, percent",3.0), \
(percent_t,critical_chance,"#float, percent",14.3), \
(rooms_t,blast_radius,"#int, rooms",2), \
(static_amount_t,base_damage,"#int, static base damage done",44), \
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
(TRAP) \
(VEHICLE)



/**************************************************************************/
/** aliases for each class type                                           */
/**************************************************************************/
#define MENTOC_USING_CT_IMPL(r,data,CLASS_TYPE)\
using BOOST_PP_CAT(mw_,CLASS_TYPE) = mods::weapon::type::CLASS_TYPE;

#define MENTOC_USING_CT \
BOOST_PP_SEQ_FOR_EACH(MENTOC_USING_CT_IMPL, ~, MENTOC_ITEM_TYPES_SEQ)


#define MENTOC_GENERIC_WEARS_IMPL(r,data,WEAR_CONSTANT)\
		this->wear_list[BOOST_PP_CAT(WEAR_,WEAR_CONSTANT)] = BOOST_PP_CAT(data,BOOST_PP_CAT(_WEAR_,WEAR_CONSTANT))();
#define MENTOC_MOB_WEARS(TYPE) \
	BOOST_PP_SEQ_FOR_EACH(MENTOC_GENERIC_WEARS_IMPL, TYPE, MENTOC_WEAR_PLACES_SEQ) \
	this->wear_all();

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
/* IMPORTANT:
 * Right here, CLASS_TYPE will be the numeric value of ITEM_*
 */
#define MENTOC_OBJ_DATA_FEED_SWITCH_IMPL(IN_R,data,CLASS_TYPE)\
	case CLASS_TYPE: if(obj_file.length() == 0){\
										 obj_file = default_yaml_file( BOOST_PP_STRINGIZE(BOOST_PP_ELEM(IN_R,MENTOC_ITEM_TYPES_SEQ)) );\
			 }; this->feed(CLASS_TYPE, row["obj_file"].as<std::string>());\
			 break;

#define MENTOC_OBJ_DATA_FEED_SWITCH \
BOOST_PP_SEQ_FOR_EACH(MENTOC_OBJ_DATA_FEED_SWITCH_IMPL, ~, MENTOC_ITEM_TYPES_SEQ_CAPS)




#define MENTOC_ENUM_TYPES_IMPL(r,data,ENUM_TYPE) ENUM_TYPE = BOOST_PP_ADD(r,1) BOOST_PP_COMMA()


/**************************************************************************/
/** FEED/EXAMPLE FILE MACROS                                              */
/**************************************************************************/
#define MENTOC_FEED_TYPE_IMPL(r,data,TYPE) \
		if(this->str_type.compare(BOOST_PP_STRINGIZE(TYPE)) == 0){\
			this->type = mw_type::TYPE;\
		}

#define MENTOC_EXAMPLE_IMPL(r,data,TYPE) \
	out_file << "# type: " << BOOST_PP_STRINGIZE(TYPE) << "\n";\

#define MENTOC_MEMBER_VARS_EXAMPLE_IMPL(r,data,MEMBER_TUPLE) \
			out_file << BOOST_PP_STRINGIZE(                        \
					BOOST_PP_TUPLE_ELEM(4,1,MEMBER_TUPLE)        \
					) << ": " <<                                       \
						BOOST_PP_TUPLE_ELEM(4,3,MEMBER_TUPLE)      \
					  << " "  <<                                       \
						BOOST_PP_TUPLE_ELEM(4,2,MEMBER_TUPLE)      \
						<< "\n";

#ifdef __MENTOC_GENERATING_MAP_OUTPUT__
#define mgen_debug(A) std::cerr << "generating map: '" << A << "\n";
#else
#define mgen_debug(A) /**-*/
#endif

#define MENTOC_GENERATE_MAP_IMPL(r,PREFIX,MEMBER_TUPLE) \
{\
	std::string item_name = PREFIX; item_name += "_";\
	item_name += BOOST_PP_STRINGIZE(BOOST_PP_TUPLE_ELEM(4,1,MEMBER_TUPLE));\
	mgen_debug(item_name);\
		this->exported[item_name.c_str()] = mods::yaml::to_string<BOOST_PP_TUPLE_ELEM(4,0,MEMBER_TUPLE)>(BOOST_PP_TUPLE_ELEM(4,1,MEMBER_TUPLE));\
}

#define MENTOC_INITIALIZE_IMPL(r,data,MEMBER_TUPLE) \
	this->BOOST_PP_TUPLE_ELEM(4,1,MEMBER_TUPLE) = BOOST_PP_TUPLE_ELEM(4,3,MEMBER_TUPLE);


#define MENTOC_INITIALIZE(SEQUENCE) \
			BOOST_PP_SEQ_FOR_EACH(MENTOC_INITIALIZE_IMPL,~,BOOST_PP_TUPLE_TO_SEQ(BOOST_PP_TUPLE_SIZE(SEQUENCE),SEQUENCE))

#define MENTOC_GENERATE_MAP(SEQUENCE,PREFIX) \
			BOOST_PP_SEQ_FOR_EACH(MENTOC_GENERATE_MAP_IMPL,PREFIX,BOOST_PP_TUPLE_TO_SEQ(BOOST_PP_TUPLE_SIZE(SEQUENCE),SEQUENCE))

#define MENTOC_MEMBER_VARS_EXAMPLE_FOR(SEQUENCE) \
			BOOST_PP_SEQ_FOR_EACH(MENTOC_MEMBER_VARS_EXAMPLE_IMPL,~,BOOST_PP_TUPLE_TO_SEQ(BOOST_PP_TUPLE_SIZE(SEQUENCE),SEQUENCE))

#define MENTOC_MEMBER_VARS_FOR(SEQUENCE) \
			BOOST_PP_SEQ_FOR_EACH(MENTOC_MEMBER_VARS_IMPL,~,BOOST_PP_TUPLE_TO_SEQ(BOOST_PP_TUPLE_SIZE(SEQUENCE),SEQUENCE))

#define MENTOC_MEMBER_VARS_IMPL(r,data,MEMBER_TUPLE) \
	BOOST_PP_TUPLE_ELEM(4,0,MEMBER_TUPLE) BOOST_PP_TUPLE_ELEM(4,1,MEMBER_TUPLE);

#ifdef MENTOC_SHOW_YAML_DEFAULTED_FIELDS
#define MENTOC_FEED_DEFAULTED_VALUE(a) std::cerr << "[USING DEFAULT FOR FIELD:]'" << a << "'\n";
#else
#define MENTOC_FEED_DEFAULTED_VALUE(a) /*-*/
#endif
#ifdef __MENTOC_SHOW_YAML_FIELD_BEING_FED__
#define MENTOC_FEED_REPORT_FIELD(a) std::cerr << "[FEEDING field:]'" << a << "'\n";
#else
#define MENTOC_FEED_REPORT_FIELD(a)
#endif

#define MENTOC_FEED_PARSE_ACTUAL_IMPL(r,data,MEMBER_TUPLE) \
	MENTOC_FEED_REPORT_FIELD(BOOST_PP_STRINGIZE(BOOST_PP_TUPLE_ELEM(4,1,MEMBER_TUPLE)));\
	fed_items.push_back(BOOST_PP_STRINGIZE(BOOST_PP_TUPLE_ELEM(4,1,MEMBER_TUPLE)));\
	try {\
	BOOST_PP_TUPLE_ELEM(4,1,MEMBER_TUPLE) = yaml_file[BOOST_PP_STRINGIZE(BOOST_PP_TUPLE_ELEM(4,1,MEMBER_TUPLE))].as<BOOST_PP_TUPLE_ELEM(4,0,MEMBER_TUPLE)>(); \
	} catch (std::exception& e){\
		MENTOC_FEED_DEFAULTED_VALUE(BOOST_PP_STRINGIZE(BOOST_PP_TUPLE_ELEM(4,1,MEMBER_TUPLE)));\
		BOOST_PP_TUPLE_ELEM(4,1,MEMBER_TUPLE) = BOOST_PP_TUPLE_ELEM(4,1,MEMBER_TUPLE);\
	}


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
/** CONTAINER */
/**************************************************************************/
#define MENTOC_FEED_CONTAINER \
	BOOST_PP_SEQ_FOR_EACH(MENTOC_FEED_TYPE_IMPL,mw_container,MENTOC_CONTAINER_TYPES_SEQUENCE) \
	MENTOC_FEED_PARSE_ALL_IMPL(MENTOC_CONTAINER_MEMBERS_TUPLE)

#define MENTOC_EXAMPLE_CONTAINERS \
	BOOST_PP_SEQ_FOR_EACH(MENTOC_EXAMPLE_IMPL,mw_container,MENTOC_CONTAINER_TYPES_SEQUENCE)


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
	BOOST_PP_SEQ_FOR_EACH(MENTOC_EXAMPLE_IMPL,mw_explosive,MENTOC_EXPLOSIVE_TYPES_SEQUENCE) \
	MENTOC_MEMBER_VARS_EXAMPLE_FOR(MENTOC_EXPLOSIVE_MEMBERS_TUPLE)

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


/**************************************************************************/
/** MELEE                                                                 */
/**************************************************************************/
#define MENTOC_FEED_MELEE \
	BOOST_PP_SEQ_FOR_EACH(MENTOC_FEED_TYPE_IMPL,mw_melee,MENTOC_MELEE_TYPES_SEQUENCE) \
	MENTOC_FEED_PARSE_ALL_IMPL(MENTOC_MELEE_MEMBERS_TUPLE)

#define MENTOC_EXAMPLE_MELEE \
	BOOST_PP_SEQ_FOR_EACH(MENTOC_EXAMPLE_IMPL,mw_melee,MENTOC_MELEE_TYPES_SEQUENCE)

/**************************************************************************/
/** VEHICLE                                                               */
/**************************************************************************/
#define MENTOC_FEED_VEHICLE \
	BOOST_PP_SEQ_FOR_EACH(MENTOC_FEED_TYPE_IMPL,mw_vehicle,MENTOC_VEHICLE_TYPES_SEQUENCE) \
	MENTOC_FEED_PARSE_ALL_IMPL(MENTOC_VEHICLE_MEMBERS_TUPLE)

#define MENTOC_EXAMPLE_VEHICLE \
	BOOST_PP_SEQ_FOR_EACH(MENTOC_EXAMPLE_IMPL,mw_vehicle,MENTOC_VEHICLE_TYPES_SEQUENCE)


/*********************************************************/
/** HOWTO: Add new item and subcategories                */
/* Step 4: Add a new FEED_* and EXAMPLE_* constants      */
/*********************************************************/


#ifndef __MENTOC_ITEM_CONSTANTS_DEFINED__
#define ITEM_RIFLE             1
#define ITEM_EXPLOSIVE         2
#define ITEM_GADGET            3
#define ITEM_DRONE             4
#define ITEM_WEAPON            5		/* Item is a weapon		*/
#define ITEM_WEAPON_ATTACHMENT 6 /* item is a weapon attachment */
#define ITEM_ATTACHMENT        6
#define ITEM_ARMOR             7
#define ITEM_CONSUMABLE        8
#define ITEM_TRAP              9
#define ITEM_CONTAINER         10
#define ITEM_MELEE             11
#define ITEM_VEHICLE           12
#endif
namespace mods {
	enum elemental_types_t {
		ELEM_NONE = 0,
		BOOST_PP_SEQ_FOR_EACH(MENTOC_ENUM_TYPES_IMPL,~,MENTOC_ELEMENTAL_TYPES_SEQUENCE)
		__ELEM_LAST,
	};
};

namespace mods::weapon::type {
	enum rifle {
		RIFLE_NONE = 0,
		BOOST_PP_SEQ_FOR_EACH(MENTOC_ENUM_TYPES_IMPL,~,MENTOC_RIFLE_TYPES_SEQUENCE)
	};
	enum explosive {
		EXPLOSIVE_NONE = 0,
		BOOST_PP_SEQ_FOR_EACH(MENTOC_ENUM_TYPES_IMPL,~,MENTOC_EXPLOSIVE_TYPES_SEQUENCE)
	};
	enum drone {
		DRONE_NONE = 0,
		BOOST_PP_SEQ_FOR_EACH(MENTOC_ENUM_TYPES_IMPL,~,MENTOC_DRONE_TYPES_SEQUENCE)
	};
	enum gadget {
		GADGET_NONE = 0,
		BOOST_PP_SEQ_FOR_EACH(MENTOC_ENUM_TYPES_IMPL,~,MENTOC_GADGET_TYPES_SEQUENCE)
	};

	enum attachment {
		ATTACHMENT_NONE = 0,
		BOOST_PP_SEQ_FOR_EACH(MENTOC_ENUM_TYPES_IMPL,~,MENTOC_ATTACHMENT_TYPES_SEQUENCE)
	};

	enum armor {
		ARMOR_NONE = 0,
		BOOST_PP_SEQ_FOR_EACH(MENTOC_ENUM_TYPES_IMPL,~,MENTOC_ARMOR_TYPES_SEQUENCE)
	};

	enum consumable {
		CONSUMABLE_NONE = 0,
		BOOST_PP_SEQ_FOR_EACH(MENTOC_ENUM_TYPES_IMPL,~,MENTOC_CONSUMABLE_TYPES_SEQUENCE)
	};

	enum trap {
		TRAP_NONE = 0,
		BOOST_PP_SEQ_FOR_EACH(MENTOC_ENUM_TYPES_IMPL,~,MENTOC_TRAP_TYPES_SEQUENCE)
	};

	enum container {
		CONTAINER_NONE = 0,
		BOOST_PP_SEQ_FOR_EACH(MENTOC_ENUM_TYPES_IMPL,~,MENTOC_CONTAINER_TYPES_SEQUENCE)
	};
	enum melee {
		MELEE_NONE = 0,
		BOOST_PP_SEQ_FOR_EACH(MENTOC_ENUM_TYPES_IMPL,~,MENTOC_MELEE_TYPES_SEQUENCE)
	};
	enum vehicle {
		VEHICLE_NONE = 0,
		BOOST_PP_SEQ_FOR_EACH(MENTOC_ENUM_TYPES_IMPL,~,MENTOC_VEHICLE_TYPES_SEQUENCE)
	};


#define MENTOC_TYPE_LIST_IMPL(r,data,ITEM_TYPE) \
	ITEM_TYPE = BOOST_PP_CAT(ITEM_,ITEM_TYPE) BOOST_PP_COMMA()

	enum type_list {
		BOOST_PP_SEQ_FOR_EACH(MENTOC_TYPE_LIST_IMPL,~,MENTOC_ITEM_TYPE_LIST_SEQUENCE)
	};

};


#endif
