--
-- PostgreSQL database dump
--

-- Dumped from database version 11.9 (Debian 11.9-0+deb10u1)
-- Dumped by pg_dump version 11.9 (Debian 11.9-0+deb10u1)

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

SET default_tablespace = '';

SET default_with_oids = false;

--
-- Name: help_pages; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.help_pages (
    id integer NOT NULL,
    hp_section character varying NOT NULL,
    hp_cmd character varying NOT NULL,
    hp_content text NOT NULL,
    created_at timestamp without time zone DEFAULT now() NOT NULL,
    updated_at timestamp without time zone DEFAULT now() NOT NULL
);


ALTER TABLE public.help_pages OWNER TO postgres;

--
-- Name: help_pages_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.help_pages_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.help_pages_id_seq OWNER TO postgres;

--
-- Name: help_pages_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.help_pages_id_seq OWNED BY public.help_pages.id;


--
-- Name: help_topics; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.help_topics (
    id integer NOT NULL,
    ht_section character varying NOT NULL,
    ht_cmd character varying NOT NULL,
    created_at timestamp without time zone DEFAULT now() NOT NULL,
    updated_at timestamp without time zone DEFAULT now() NOT NULL
);


ALTER TABLE public.help_topics OWNER TO postgres;

--
-- Name: help_topics_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.help_topics_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.help_topics_id_seq OWNER TO postgres;

--
-- Name: help_topics_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.help_topics_id_seq OWNED BY public.help_topics.id;


--
-- Name: help_pages id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.help_pages ALTER COLUMN id SET DEFAULT nextval('public.help_pages_id_seq'::regclass);


--
-- Name: help_topics id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.help_topics ALTER COLUMN id SET DEFAULT nextval('public.help_topics_id_seq'::regclass);


--
-- Data for Name: help_pages; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.help_pages (id, hp_section, hp_cmd, hp_content, created_at, updated_at) FROM stdin;
1372	allow_skill	allow_skill	usage: allow_skill <player_name> <skill|all> \r\ndescription: this command will take a player name and a skill as the second\r\nargument. You can also pass in 'all' as the second argument and it will\r\nallow all skills available.\r\nExample: allow_skill player1 basic-armor\r\nExample: allow_skill player1 all\r\n\r\nthis documentation was written on 2020-09-06.	2022-01-15 00:16:35.010127	2022-01-15 00:16:35.010127
1373	attract	attract	usage: invoke attract <direction>\r\n- Attract\r\n\t- Get the attention of target, causing them to attack something else\r\n\r\nthis documentation was written on 2021-09-25.	2022-01-15 00:16:35.011136	2022-01-15 00:16:35.011136
1374	bladed_array	bladed_array	usage: invoke bladed_array  \r\n- Bladed Array\r\n\t- Each piece of worn armor causes bladed knife damage to melee attackers\r\n\t- CQC attacks:\r\n\t\t- throw, wrestle, grab, clinch\r\n\t\t- cause HP damage to attacker\r\n\r\nthis documentation was written on 2021-09-25.	2022-01-15 00:16:35.011971	2022-01-15 00:16:35.011971
1417	roots_of_mayhem	roots_of_mayhem	usage: invoke roots_of_mayhem <target> <direction>\r\n- Roots of mayhem\r\n\t- Cause the dead to reach up from beneath a target\r\n\t- Target is stuck in place and can only use ranged attacks\r\n\t- Target cannot move or flee until spell is over\r\n\r\nthis documentation was written on 2021-09-25.	2022-01-15 00:16:35.038987	2022-01-15 00:16:35.038987
1418	set_ammo	set_ammo	usage: set_ammo <weapon> <number>\r\ndescription: \r\nexample: \r\n\r\nthis documentation was written on 2020-11-15.	2022-01-15 00:16:35.039619	2022-01-15 00:16:35.039619
1375	camera,claymore,install,uninstall	camera,claymore,install,uninstall	usage: install <object> <direction>\r\ndescription: the 'install' command is used to install \r\ndevices like cameras or claymore mines. To install a claymore\r\nmine, you would simply type 'install claymore north'. This would\r\ninstall the claymore to the north exit of the room. Any NPC that\r\nleaves or enters the current room through the northern exit will\r\nbe met with an explosion.\r\n\r\nThe other use case of the 'install' command is to install cameras\r\nonto a wall inside a room. To install a camera on the north side of\r\nthe room, you would type 'install camera north'.\r\n\r\nTo remove the device, use the 'uninstall' command\r\nType 'help uninstall' for more information.\r\n\r\nTo cancel the installation of a device while you are currently\r\ninstalling it, you must type 'cancel'\r\n\r\nFor more information: see the help manual for the following keywords:\r\n'cancel','camera','claymore','install','uninstall'\r\n\r\nThis documentation was written on 2020-03-29.	2022-01-15 00:16:35.01271	2022-01-15 00:16:35.01271
1376	cancel	cancel	usage: cancel\r\ndescription: the 'cancel' command is used to stop the install\r\ncommand. If you are installing a camera on the wall, it takes a\r\ncertain amount of time before that process is done. In that time,\r\nyou are vulnerable to attacks.\r\n\r\nFor more information: see the help manual for the following keywords:\r\n'cancel','camera','claymore','install','uninstall'\r\n\r\nThis documentation was written on 2020-03-29.	2022-01-15 00:16:35.013444	2022-01-15 00:16:35.013444
1377	combat,roe,rules_of_engagement,cqc,sniping,kill,engage	combat,roe,rules_of_engagement,cqc,sniping,kill,engage	 Rules of engagement\r\n{hr} There are different ranges of combat in TD.\r\n {blu}1) Ranged combat{/blu}\r\n {blu}2) Close quarters combat{/blu}\r\n\r\n When you use the 'snipe' command and hit a target that is a few rooms away   \r\n from you, you are engaging in ranged combat. You are free to move around and \r\n do not need to flee from combat. As long as your target doesn't walk into    \r\n the room you reside in and subsequently attack you, you are safe to move     \r\n around freely.                                                               \r\n                                                                              \r\n However, if you choose to go into the same room as your target, or if your   \r\n target moves to your room, you are now in Close Quarters Combat range.       \r\n                                                                              \r\n When you are engaged in CQC, the dynamics of battle change. If you are       \r\n wielding a Sniper rifle, you will not be able to hit your target as sniper   \r\n rifles only work on targets that are not in the same room as you.            \r\n                                                                              \r\n Now if you were to use any other type of ranged weapon like an A.R., or a    \r\n sub-machine gun, you could continue doing battle with your target even if    \r\n they are in the same room as you.                                            \r\n                                                                              \r\n There some exceptions to this rule. Mainly, if you have an underbarrel       \r\n attachment like a shotgun underbarrel attached to your sniper rifle.         \r\n                                                                              \r\n If you are wielding a weapoon in your SECONDARY position, you would be able  \r\n use that weapon in CQC range. All weapons that can be wielded in the         \r\n SECONDARY position can seemlessly deal damage to targets in the CQC range.   \r\n                                                                              \r\n But sometimes you want more control over how you react to a target that goes \r\n from RANGED comat to CQC combat. This is where Rules of Engagement come into \r\n play.                                                                        \r\n                                                                              \r\n Rules of engagement allow you to dictate how you respond to an enemy closing \r\n the distance on you. There are different types of rules of engagement        \r\n available to you:                                                            \r\n\r\n {blu}1) Ballistic{/blu}\r\n {blu}2) CQC{/blu} \r\n {blu}3) Auxiliary{/blu}\r\n {blu}4) Secondary{/blu}\r\n\r\n {blu}Ballistic{/blu}\r\n When your rules of engagement are set to Ballistic, when a target closes the \r\n distance and appears in your room, you will continune to use your primary    \r\n weapon as long as your primary is not a sniper rifle                         \r\n\r\n {blu}CQC{/blu}\r\n When your rules of engagement are set to CQC, when a target closes the       \r\n distance and appears in your room, you will use hand to hand combat.         \r\n If you have set a combat order (see the helpfile for combat_ordere), you will\r\n proceed to throw the maneuvers you specified.                                \r\n\r\n\r\n {blu}Auxiliary{/blu}\r\n When your rules of engagement are set to Auxiliary, when a target closes the \r\n distance and appears in your room, you will use the underbarrel attachment   \r\n on your primary weapon to engage your target. This will work as long as your \r\n attachment has ammunition.\r\n\r\n {blu}Secondary{/blu}\r\n When your rules of engagement are set to Secondary, when a target closes the \r\n distance and appears in your room, you will use the weapon in your secondary \r\n position.\r\n\r\n	2022-01-15 00:16:35.014126	2022-01-15 00:16:35.014126
1378	conceal	conceal	usage: conceal <item>\r\ndescription: \r\nexample: \r\n\r\nthis documentation was written on 2020-11-16.	2022-01-15 00:16:35.014932	2022-01-15 00:16:35.014932
1379	confuse	confuse	usage: invoke confuse <target>\r\n- Confuse\r\n\t- Target loses focus and forgets who he is fighting.\r\n\r\nthis documentation was written on 2021-09-25.	2022-01-15 00:16:35.015689	2022-01-15 00:16:35.015689
1380	contract,contracts,quest,quests	contract,contracts,quest,quests	usage: contract <list>\r\nusage: contract <join> <N>\r\nusage: contract <leave> <N>\r\nusage: contract <step>\r\nusage: contract <current>\r\nusage: contract <show|describe> <N>\r\nThis documentation was written on 2021-06-06.	2022-01-15 00:16:35.016358	2022-01-15 00:16:35.016358
1381	corpse_explosion	corpse_explosion	usage: invoke corpse_explosion <target>\r\n- Corpse explosion\r\n\t- Cause target corpse to detonate\r\n\r\nthis documentation was written on 2021-09-25.	2022-01-15 00:16:35.01702	2022-01-15 00:16:35.01702
1382	cursed_ballistics	cursed_ballistics	usage: invoke cursed_ballistics    \r\n- Cursed Ballistics\r\n\t- Worn armor becomes more effective at the cost of movement points\r\n\r\nthis documentation was written on 2021-09-25.	2022-01-15 00:16:35.017679	2022-01-15 00:16:35.017679
1383	demonic_incantation	demonic_incantation	usage: invoke demonic_incantation <target>\r\n- Demonic incantation\r\n\t- Must have 1 demonic incantation charge from 'Recruit' skill\r\n\t- Raise target corpse.\r\n\t\t- Corpse can be ordered to attack target\r\n\r\nthis documentation was written on 2021-09-25.	2022-01-15 00:16:35.018356	2022-01-15 00:16:35.018356
1384	detonate_limb	detonate_limb	usage: invoke detonate_limb <target> <direction>\r\n- Detonate Limb\r\n\t- Cause target arm/leg to explode causing damage to room inhabitants\r\n\r\nthis documentation was written on 2021-09-25.	2022-01-15 00:16:35.019017	2022-01-15 00:16:35.019017
1385	extract_organs	extract_organs	usage: invoke extract_organs <target>\r\n- Extract Organs\r\n\t- Player dissects a corpse and consumes it's organs\r\n\t\t- Gains HP, Mana, Movement equal to 25 percent of the \r\n\t\t\tHP, Mana, and Movement of the NPC or PC that died\r\n\r\nthis documentation was written on 2021-09-25.	2022-01-15 00:16:35.019652	2022-01-15 00:16:35.019652
1386	feign_death	feign_death	usage: feign_death\r\ndescription: \r\nexample: \r\n\r\nthis documentation was written on 2020-11-16.	2022-01-15 00:16:35.020308	2022-01-15 00:16:35.020308
1387	force_out	force_out	usage: invoke force_out <target> <direction>\r\n- Force out\r\n\t- Force target to move in a specific direction\r\n\r\nthis documentation was written on 2021-09-25.	2022-01-15 00:16:35.020918	2022-01-15 00:16:35.020918
1388	ghastly_double	ghastly_double	usage:     \r\n- Ghastly double\r\n\t- Create an illusion that there are two of you\r\n\t- Chance of taking damage is reduced by 30 percent\r\n\r\nthis documentation was written on 2021-09-25.	2022-01-15 00:16:35.021522	2022-01-15 00:16:35.021522
1389	go_dark	go_dark	usage: go_dark\r\ndescription: \r\nexample: \r\n\r\nthis documentation was written on 2020-11-16.	2022-01-15 00:16:35.022103	2022-01-15 00:16:35.022103
1390	grim_aura	grim_aura	usage: invoke grim_aura   \r\ndescription: \r\n- +15 percent damage done by melee/ranged attacks\r\n- damage taken is reduced by 15 percent\r\n- Player gets 15 percent of their max hp added to their current hp\r\n\r\nthis documentation was written on 2021-09-25.	2022-01-15 00:16:35.022623	2022-01-15 00:16:35.022623
1391	hellfire_circle	hellfire_circle	usage: invoke hellfire_circle   \r\n- Hellfire Circle\r\n\t- Reduces incendiary, explosive damage\r\n\t- Reduces CQC, melee damage by 25 percent\r\n\r\nthis documentation was written on 2021-09-25.	2022-01-15 00:16:35.023153	2022-01-15 00:16:35.023153
1392	hellfire_incantation	hellfire_incantation	usage: invoke hellfire_incantation\r\n- Hellfire Incantation\r\n\t- Adds incendiary and radioactive damage to all damage dealt\r\n\t- Lasts 99 ticks\r\n\r\nthis documentation was written on 2021-09-25.	2022-01-15 00:16:35.023763	2022-01-15 00:16:35.023763
1393	install_camera_feed	install_camera_feed	usage: install_camera_feed <name> <id>...[id_N]\r\ndescription: installs a camera feed identified by 'name'.\r\nThe room virtual numbers you pass after the name of the camera feed will be the rooms\r\nthat you would like to be shown on the camera feed.\r\n{blu}Example: {yel}install_camera_feed "Camera Feed A" 20 21 22 23 24{/yel}\r\n\r\n{blu}this documentation was written on 2020-09-26.{/blu}	2022-01-15 00:16:35.024306	2022-01-15 00:16:35.024306
1394	install_minigame	install_minigame	usage: install_minigame <name> <type> <difficulty> <unlock-event>\r\ndescription: installs a mini game identified by 'name'.\r\nValid types:\r\n{blu}line-up: Will show a series of rows that the user has to line up accordingly\r\n{blu}wires: will show a series of wires and allow the user to attach them\r\nValid difficulties:\r\neasy\r\nmedium\r\nhard\r\nimpossible\r\n\r\nThe unlock-event will be one of the following:\r\nunlock <direction>\r\nlock <direction>\r\ntoggle <direction>\r\n\r\nThe unlock-event can also work with several directions separated by commas\r\nThe unlock-event can work with room virtual numbers and directions\r\nunlock vnum:40 <direction>\r\nlock vnum:40 <direction>\r\ntoggle vnum:40 <direction>\r\nIf the unlock-event is neither of the above strings, then it will be fed to the system\r\nand handled accordingly.\r\nExample of a custom event:\r\n{blu}Example: {yel}install_minigame "North Door Lock" medium custom "custom event. handled by system"{/yel}\r\nThe 'custom' keyword takes whatever you pass in and the system interprets it. This is for adding custom events\r\nthat might work in the future or if the developers have a custom even that allow you to do special things.\r\nAs of this writing, there are no custom events. 2020-09-28\r\n{blu}Example: {yel}install_minigame "North Door Lock" line-up medium unlock north{/yel}\r\n{blu}Example: {yel}install_minigame "North Door Lock" line-up medium unlock vnum:40 north,south,east,west{/yel}\r\n{blu}Example: {yel}install_minigame "North Door Lock" line-up medium toggle vnum:40 south,west,up{/yel}\r\n\r\n{blu}this documentation was written on 2020-09-28.{/blu}	2022-01-15 00:16:35.02483	2022-01-15 00:16:35.02483
1395	intimidate	intimidate	usage: intimidate <target>\r\ndescription: \r\nexample: \r\n\r\nthis documentation was written on 2020-11-16.	2022-01-15 00:16:35.025395	2022-01-15 00:16:35.025395
1396	leech	leech	usage: invoke leech <target> <direction>\r\n- Leech\r\n\t- Fire several devices at target.\r\n\t- Target gets BLEED for 30 ticks\r\n\t\t- Player gets hp equal to the bleed damage\r\n\r\nthis documentation was written on 2021-09-25.	2022-01-15 00:16:35.025927	2022-01-15 00:16:35.025927
1397	life_tap	life_tap	usage: invoke life_tap <target>\r\n- Life Tap\r\n\t- Damage done to a target that's been the target of life tap will increase your HP/Mana/Movement points\r\n\r\nthis documentation was written on 2021-09-25.	2022-01-15 00:16:35.02646	2022-01-15 00:16:35.02646
1398	list_minigame	list_minigame	usage: list_minigame\r\ndescription: lists all the mini games in the current room. This command is needed to pass the id of the minigame\r\nto the uninstall_minigame command.\r\n\r\n{blu}this documentation was written on 2020-09-28.{/blu}	2022-01-15 00:16:35.026984	2022-01-15 00:16:35.026984
1399	melt	melt	usage: invoke melt <target>  \r\n- Player places hands on target\r\n- Target is set on fire and blinded for 50 ticks\r\n\r\nthis documentation was written on 2021-09-25.	2022-01-15 00:16:35.02754	2022-01-15 00:16:35.02754
1400	minor_shielding	minor_shielding	usage: invoke minor_shielding   \r\n- Minor shielding\r\n\t- Create a ballistic resistant shielding around self\r\n - Lasts for 33 ticks per level\r\n\r\nthis documentation was written on 2021-09-25.	2022-01-15 00:16:35.028138	2022-01-15 00:16:35.028138
1401	morbid_doubt	morbid_doubt	usage: invoke morbid_doubt <target>\r\n- Morbid doubt\r\n\t- Forces target to turn their weapon against themselves\r\n\r\nthis documentation was written on 2021-09-25.	2022-01-15 00:16:35.02873	2022-01-15 00:16:35.02873
1402	morbid_insight	morbid_insight	usage: invoke morbid_insight <target> <direction>\r\n- Morbid Insight\r\n\t- Player can detect nearby enemies if corpses are nearby\r\n\t- Player can detect HP/Mana/Move points of target if corpse is nearby\r\n\r\nthis documentation was written on 2021-09-25.	2022-01-15 00:16:35.029332	2022-01-15 00:16:35.029332
1403	muscle_memory	muscle_memory	usage: invoke muscle_memory  \r\n- Muscle Memory\r\n- Once you die\r\n You can order your corpse to explode\r\n\r\nthis documentation was written on 2021-09-25.	2022-01-15 00:16:35.029925	2022-01-15 00:16:35.029925
1404	neutron_shield	neutron_shield	usage: invoke neutron_shield\r\n- Neutron Shield\r\n\t- Creates a shield that\r\n\t\t- dampens radioactive, cryogenic, and anti-matter damage\r\n\t- Each bullet absorbed while Neutron Shield is active becomes a radioactive charge\r\n\t- Radioactive charges can be released as a ranged attack but only while NS is active\r\n\r\nthis documentation was written on 2021-09-25.	2022-01-15 00:16:35.030524	2022-01-15 00:16:35.030524
1405	parasitic_corpse_tap	parasitic_corpse_tap	usage: invoke parasitic_corpse_tap <target>\r\n- Parasitic Corpse Tap\r\n\t- Walk up to any corpse and absorb hp,mana,movement points\r\n\t- Corpse dissipates once done\r\n\r\nthis documentation was written on 2021-09-25.	2022-01-15 00:16:35.031129	2022-01-15 00:16:35.031129
1406	particle_deceleration	particle_deceleration	usage: invoke particle_deceleration\r\n- Particle Deceleration\r\n\t- Create an aura around player that slows the velocity of attacks\r\n\t\t- Reduces damage done by ranged weapons\r\n\r\nthis documentation was written on 2021-09-25.	2022-01-15 00:16:35.031858	2022-01-15 00:16:35.031858
1407	pathogen_ammunition	pathogen_ammunition	usage: invoke pathogen_ammunition [primary|secondary]\r\ndescription: Loads a special magazine into your primary or secondary weapon.\r\nPathogen ammunition infects your ammunition with a genetically engineered\r\nvirus that continues to deal poison damage to a target for a period of time.\r\nexample: invoke pathogen_ammunition primary\r\nexample: invoke pathogen_ammunition # this is equivalent to {grn}invoke pathogen_ammunition primary{/grn}\r\n\r\nthis documentation was written on 2021-09-25.	2022-01-15 00:16:35.032508	2022-01-15 00:16:35.032508
1408	penetrating_shot	penetrating_shot	usage: penetrating_shot <target>\r\ndescription: \r\nexample: \r\n\r\nthis documentation was written on 2020-11-16.	2022-01-15 00:16:35.033174	2022-01-15 00:16:35.033174
1409	plant_claymore	plant_claymore	usage: plant_claymore <direction>\r\ndescription: \r\nexample: \r\n\r\nthis documentation was written on 2020-11-16.	2022-01-15 00:16:35.033834	2022-01-15 00:16:35.033834
1410	plug_cable	plug_cable	usage: plug_cable <id>\r\ndescription: plugs into the ethernet port identified by 'id'.\r\n{blu}Example: {yel}plug_cable A{/yel}\r\n\r\n{blu}this documentation was written on 2020-10-02.{/blu}	2022-01-15 00:16:35.034487	2022-01-15 00:16:35.034487
1411	practice	practice	usage: practice <help>\r\nusage: practice <skill-shorthand>\r\nsee also:\r\n skills train\r\n	2022-01-15 00:16:35.035149	2022-01-15 00:16:35.035149
1412	recruit	recruit	usage: invoke recruit <target> \r\n- Recruit\r\n\t- Walk up to any corpse\r\n\t- Place hex on corpse\r\n\t- Add 1 demonic incantation charge for every 2 corpse this is done to\r\n\t- Corpse dissipates once done\r\n\r\nthis documentation was written on 2021-09-25.	2022-01-15 00:16:35.035811	2022-01-15 00:16:35.035811
1413	registration_status,enable_registration,disable_registration	registration_status,enable_registration,disable_registration	usage: enable_registration\r\nusage: disable_registration\r\nusage: registration_status\r\ndescription: depending on which command you call, it will enable/disable player\r\nregistration until you specify otherwise.\r\n{blu}Example: {yel}enable_registration{/yel}\r\nregistration_status will tell you if player registration is enabled or not.\r\n\r\n{blu}this documentation was written on 2020-09-11.{/blu}	2022-01-15 00:16:35.036438	2022-01-15 00:16:35.036438
1414	reload	reload	usage: reload [primary|secondary]\r\ndescription: reloads either your primary or secondary. If neither is supplied will automatically reload your primary.\r\nexample: reload primary\r\nexample: reload # this is equivalent to {grn}reload primary{/grn}\r\nexample: reload secondary\r\n\r\nthis documentation was written on 2020-11-16.	2022-01-15 00:16:35.037096	2022-01-15 00:16:35.037096
1415	room_dark	room_dark	usage: room_dark <on|off>\r\ndescription: \r\n\r\nexample: \r\n\r\nthis documentation was written on 2020-11-15.	2022-01-15 00:16:35.037728	2022-01-15 00:16:35.037728
1416	room_fire	room_fire	usage: room_fire <on|off> [level]\r\ndescription: \r\nexample: \r\n\r\nthis documentation was written on 2020-11-15.	2022-01-15 00:16:35.03835	2022-01-15 00:16:35.03835
1419	set_npc_position	set_npc_position	usage: set_npc_position <UUID> <POSITION>\r\ndescription: valid positions include: \r\nDEAD        -> dead\r\nMORTALLYW   -> mortally wounded\r\nINCAP       -> incapacitated\r\nSTUNNED     -> stunned\r\nSLEEPING    -> sleeping\r\nRESTING     -> resting\r\nSITTING     -> sitting\r\nFIGHTING    -> fighting\r\nSTANDING    -> standing\r\n\r\nThis command is not case-sensitive.\r\n\r\nexample: set_npc_position 45 INCAP\r\n\r\nNote: to grab an npc's uuid, go to the same room as it and type room_list_uuid\r\n\r\nFor more information: see the help manual for the following keywords:\r\n'room_list_uuid', 'set_npc_position', 'set_position'\r\n\r\nthis documentation was written on 2020-06-26.	2022-01-15 00:16:35.040245	2022-01-15 00:16:35.040245
1420	set_position	set_position	usage: set_position <POSITION>\r\ndescription: valid positions include: \r\nDEAD        -> dead\r\nMORTALLYW   -> mortally wounded\r\nINCAP       -> incapacitated\r\nSTUNNED     -> stunned\r\nSLEEPING    -> sleeping\r\nRESTING     -> resting\r\nSITTING     -> sitting\r\nFIGHTING    -> fighting\r\nSTANDING    -> standing\r\n\r\nThis command is not case-sensitive.\r\n\r\nexample: set_position INCAP\r\n\r\nthis documentation was written on 2020-06-26.	2022-01-15 00:16:35.040913	2022-01-15 00:16:35.040913
1421	shadow_sight	shadow_sight	usage: invoke shadow_sight\r\n- Shadow sight\r\n\t- Player has night vision\r\n\r\nthis documentation was written on 2021-09-25.	2022-01-15 00:16:35.041547	2022-01-15 00:16:35.041547
1422	shredded_cantrip	shredded_cantrip	usage:     \r\n- Shredded cantrip\r\n- Place a trap in the room that causes targets to bleed\r\n\r\nthis documentation was written on 2021-09-25.	2022-01-15 00:16:35.042172	2022-01-15 00:16:35.042172
1423	skills,skill	skills,skill	usage: skills [show] [skill_name]...[skill_N]\r\ndescription: This command will show you your proficiencies with each skill.\r\nTo see a detailed description of a skill, type: {grn}skills show <skill>{/grn}\r\n{blu}Example: {yel}skills show spray-chance{/yel}\r\n{blu}NOTE: you can specify multiple skills\r\nExample: {yel}skills show spray-chance basic-armor mold{/yel}\r\n\r\n{blu}this documentation was written on 2020-09-10.{/blu}	2022-01-15 00:16:35.042785	2022-01-15 00:16:35.042785
1424	sniper	sniper	____________________ \r\n   S N I P I N G    |\r\n____________________|\r\n Ability:     X-ray shot\r\n Command:     xray_shot <no-args>\r\n{hr} Description: Hit a target through walls or doors. X-ray shot does not requi-\r\n              you to have your target within your line of sight. You can be  \r\n              several rooms away and still do damage to your target.         \r\n\r\n Usage: xray_shot\r\n\r\nE X A M P L E\r\n{hr}{grn}$>{/grn} mark Enforcer \r\n{grn}::{/grn} CONFIRM :: Marked target\r\n{grn}$>{/grn} engage \r\n{grn}::{/grn} CALIBRATE :: Okay -: Target locked :-\r\n{grn}$>{/grn} xray_shot \r\nYou OBLITERATE A Military Police enforcer with your deadly snipe!!\r\n{grn}$>{/grn} disengage \r\n{grn}::{/grn} DISENGAGED\r\n{grn}$>{/grn} xray_shot \r\n{grn}::{/grn} NO TARGET CALIBRATED\r\n                                  \r\n{hr} Ability: Tracking Shot \r\n Command: tracking_shot <target> <direction>\r\n{hr} Description: targets that are marked take extra damage.\r\n{hr} Ability: Mark Target\r\n Command: mark <target>\r\n{hr} Description: For use with x-ray shot.\r\n{hr} Ability: Mark Target\r\n Command: mark <target>\r\n{hr} Description: For use with x-ray shot.\r\n{hr} Ability: Attach frag grenade underbarrel\r\n Command: attach_frag\r\n{hr} Description: Attaches a fragmentation grenade launcher to your primary.\r\n Once you've attached the launcher, use {grn}fire <direction> <count>{/grn}\r\n{hr}E X A M P L E \r\n{hr}{grn}$>{/grn} attach_frag \r\n{grn}You attach an PWM Grenade Launcher to your primary weapon{/grn}\r\n{grn}$>{/grn} fire north 3 \r\n{grn}You launch a fragmentation grenade way off to the north!{/grn}\r\n{hr} Description: For use with x-ray shot.\r\n{grn}target_limb{/grn}: specify the limb to target.\r\n{grn}tracking_shot{/grn}: tracking your target.\r\n{yel}Usage:{/yel}{grn}tracking_shot <target> <direction>{/grn}\r\n{yel}Example:{/yel}\r\n{grn}$> tracking_shot enforcer east{/grn}\r\n{grn}You tag your target!{/grn}\r\nTagged enemies take extra damage.\r\n ____________________ \r\n|      Healing       |\r\n|____________________|\r\n{grn}light_bandage{/grn}: use to regain some hp\r\n{grn}suture{/grn}: regain more hp than light_bandage but at the cost of movement points\r\n{grn}adrenaline_shot{/grn}: inject yourself with adrenaline and get +5 armor, +5 movement, and +3 strength\r\n ____________________ \r\n|     Demolitions    |\r\n|____________________|\r\n{grn}build_claymore{/grn}: creates a claymore in your inventory (if you have a charge left).\r\n{grn}build_shrapnel_claymore{/grn}: creates a {grn}shrapnel{/grn} claymore in your inventory (if you have a charge left).\r\n{grn}build_corrosive_claymore{/grn}: creates a {grn}corrosive{/grn} claymore in your inventory (if you have a charge left).\r\n{grn}guided_missile{/grn}: pre-program a guided missile to travel and detonate at the end of the path.\r\n{yel}Usage:{/yel}{grn}guided_missile <direction>...[direction]\r\n{yel}Example:{/yel}{grn}guided_missile n e e n n w{/grn}\r\nThe above example will send a guided missile along the path until it reaches the last direction (west)\r\nupon which it will detonate in that room.\r\n ____________________ \r\n|     I N T E L      |\r\n|____________________|\r\n{grn}build_emp{/grn}: creates an EMP grenade in your inventory (if you have a charge left).\r\n{grn}build_chaff{/grn}: creates a Chaff grenade in your inventory (if you have a charge left).\r\n{grn}build_sensor{/grn}: creates a Sensor grenade in your inventory (if you have a charge left).\r\n{grn}request_recon{/grn}: creates a Sensor grenade in your inventory (if you have a charge left).\r\n\r\n{grn}attach_shotgun{/grn}: activate an underbarrel shotgun with 5 shells pre-loaded.\r\n  {yel}Usage:{/yel}{grn}attach_shotgun{/grn}\r\n  Once you've attached the shotgun, the next same-room fire fight you're in will use the underbarrel\r\n  shotgun until it's ammo is depleted. Once the shotgun's ammo is depleted, you will switch back to your\r\n  primary weapon's main attack.\r\n{grn}attach_frag{/grn}: activate an underbarrel grenade launcher with 3 grenades pre-loaded.\r\n{yel}Usage:{/yel}{grn}attach_frag{/grn}\r\nOnce you've attached the launcher, use {grn}fire <direction> <count>{/grn}\r\n{yel}Example:{/yel}\r\n{grn}$> attach_frag{/grn}\r\n{grn}You attach an PWM Grenade Launcher to your primary weapon{/grn}\r\n{grn}$> fire north 3{/grn}\r\n{grn}You launch a fragmentation grenade way off to the north!{/grn}\r\n{grn}target_limb{/grn}: specify the limb to target.\r\n{yel}Usage:{/yel}{grn}target_limb <right-arm|left-arm|none>\r\n{yel}Example:{/yel}\r\n{grn}$> target_limb right-arm{/grn}\r\n{grn}You start targeting the right arm of your opponents.{/grn}\r\n{grn}$> snipe Enforcer west{/grn}\r\n{grn}You OBLITERATE A Military Police enforcer with your deadly snipe!!{/grn}\r\n{grn}*** LIMB DAMAGE (right-arm) [ INTEGRITY: POOR ] ***\r\n{grn}$> snipe Enforcer west{/grn}\r\n{grn}You OBLITERATE A Military Police enforcer with your deadly snipe!!{/grn}\r\n{grn}*** LIMB DAMAGE (right-arm) [ INTEGRITY: USELESS ] ***\r\n\r\nOnce a limb reaches {yel}USELESS{/yel}, the limb is effectively destroyed.\r\n\r\n|-----------------------|\r\n| A note on handed-ness |\r\n|-----------------------|\r\n Every NPC is right-handed by default. An NPC uses their left hand to utilize a secondary weapon such \r\n as a pistol. NPC's will use their right hand to utilize their primary weapon. This goes for rifles and melee weapons.\r\n{hr}\r\n{hr} Limb damage and effects on {grn}MELEE{/grn} primary weapon\r\n{hr} LIMB EFFECTS \r\n{hr} Right arm: 1 or more of the below will occur...\r\n            * 30 percent reduced chance to hit target \r\n            * 1l percent damage reduction for 33 ticks\r\n            * reduce dice sides by 10-25 percent for 45 ticks \r\n{hr} Left arm:  1 or more of the below will occur...\r\n            * next 3 attacks do half damage     \r\n{hr} Limb damage and effects on {grn}RIFLE{/grn} primary weapon\r\n{hr} LIMB EFFECTS \r\n{hr} Right arm: 1 or more of the below will occur...\r\n            * 25 percent reduced damage with primary weapon\r\n            * 33 percent chance of jamming primary weapon\r\n            * 40 percent penalty to accuracy\r\n            * 20 percent chance of dropping primary weapon\r\n            * 14 percent chance of primary magazine emptying\r\n{hr} Left arm:  1 or more of the below will occur...\r\n            * 35 percent less likely to deal critical shots\r\n            * add 15 ticks to cooldown between shots\r\n            * 25 percent damage reduction for 60 ticks\r\n{hr}{grn}xray_shot{/grn}: specify the limb to target.\r\n\r\n	2022-01-15 00:16:35.043456	2022-01-15 00:16:35.043456
1425	suffocate	suffocate	usage: invoke suffocate <target>  \r\n- Player summons a rope around target's neck\r\n- Target continues to suffocate, losing HP\r\n\r\nthis documentation was written on 2021-09-25.	2022-01-15 00:16:35.044299	2022-01-15 00:16:35.044299
1426	summon_extraction	summon_extraction	usage: summon_extraction\r\ndescription: \r\nexample: \r\n\r\nthis documentation was written on 2020-11-16.	2022-01-15 00:16:35.044955	2022-01-15 00:16:35.044955
1427	throw,grenade	throw,grenade	usage: throw <direction> [room_count=4]\r\nexample: \r\n $ get frag backpack\r\n $ hold frag\r\n $ throw north 2\r\n This will throw a frag 2 rooms away\r\n NOTE:\r\nAll grenades are thrown as far as they can up to a maximum amount of 4 rooms away\r\nor however many rooms before it reaches a dead-end\r\nsee: help grenade	2022-01-15 00:16:35.045588	2022-01-15 00:16:35.045588
1428	toss_cryogenic_grenade	toss_cryogenic_grenade	usage: toss_cryogenic_grenade <direction> <rooms>\r\ndescription: \r\nexample: \r\n\r\nthis documentation was written on 2020-11-16.	2022-01-15 00:16:35.046216	2022-01-15 00:16:35.046216
1429	uninstall_camera_feed	uninstall_camera_feed	usage: uninstall_camera_feed <name>\r\ndescription: uninstalls the camera feed identified by 'name'.\r\n{blu}Example: {yel}uninstall_camera_feed "Camera Feed A"{/yel}\r\n\r\n{blu}this documentation was written on 2020-09-26.{/blu}	2022-01-15 00:16:35.046871	2022-01-15 00:16:35.046871
1430	uninstall_minigame	uninstall_minigame	usage: uninstall_minigame <id>\r\ndescription: uninstalls the currently installed mini game identified by the id you pass in.\r\nTo see the ID's of the mini games currently installed in this room, see the list_minigame command.\r\n{blu}Example: {yel}uninstall_minigame "North Door Lock"{/yel}\r\n\r\n{blu}this documentation was written on 2020-09-28.{/blu}	2022-01-15 00:16:35.047496	2022-01-15 00:16:35.047496
1431	use_flash_underbarrel	use_flash_underbarrel	usage: use_flash_underbarrel\r\ndescription: \r\nexample: \r\n\r\nthis documentation was written on 2020-11-16.	2022-01-15 00:16:35.048139	2022-01-15 00:16:35.048139
1432	xray_shot	xray_shot	usage: xray_shot\r\ndescription: \r\nexample: \r\n\r\nthis documentation was written on 2020-11-16.	2022-01-15 00:16:35.048779	2022-01-15 00:16:35.048779
1433	yaml_example	yaml_example	usage: yaml_example <list> <object_type>\r\ndescription: this command will take an object type and write an example yaml file.\r\nyou can optionally send the string list as the only argument to this function\r\nand it will spit out all the possible object types.\r\n\r\nThe main function of yaml_example is to write an example file for the object type you specify.\r\n\r\nExample: yaml_example drone\r\nThe above example will write to /lib/objects/drone.yml\r\n\r\nthis documentation was written on 2020-09-08.	2022-01-15 00:16:35.049411	2022-01-15 00:16:35.049411
1434	yaml_log	yaml_log	usage: yaml_log \r\ndescription: the yaml_log command has two types of usages.\r\n1) calling yaml_log with no arguments will send you the current yaml log\r\n2) calling yaml_log the same way you would call yaml_import\r\nExample: yaml_log RIFLE g36c.yml\r\nThe above example will attempt to import and give you the g36c.yml file.\r\nShould any exceptions with the yaml import occur, you can see the log\r\nof those errors by calling yaml_log with no arguments.\r\n\r\nthis documentation was written on 2020-09-02.	2022-01-15 00:16:35.05005	2022-01-15 00:16:35.05005
\.


--
-- Data for Name: help_topics; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.help_topics (id, ht_section, ht_cmd, created_at, updated_at) FROM stdin;
1	chat	gossip	2022-01-14 20:39:15.449942	2022-01-14 20:39:15.449942
2	chat	ooc	2022-01-14 20:39:15.452513	2022-01-14 20:39:15.452513
3	chat	newbie	2022-01-14 20:39:15.454504	2022-01-14 20:39:15.454504
4	chat	grats	2022-01-14 20:39:15.456546	2022-01-14 20:39:15.456546
5	date	and	2022-01-14 20:39:15.458451	2022-01-14 20:39:15.458451
6	date	time	2022-01-14 20:39:15.460486	2022-01-14 20:39:15.460486
7	date	moon_phase	2022-01-14 20:39:15.462489	2022-01-14 20:39:15.462489
8	date	time	2022-01-14 20:39:15.464475	2022-01-14 20:39:15.464475
9	contracts	contract	2022-01-14 20:39:15.466022	2022-01-14 20:39:15.466022
10	loot	list_loot	2022-01-14 20:39:15.467492	2022-01-14 20:39:15.467492
11	loot	take_loot	2022-01-14 20:39:15.468922	2022-01-14 20:39:15.468922
12	loot	show_loot	2022-01-14 20:39:15.470247	2022-01-14 20:39:15.470247
13	objects	get	2022-01-14 20:39:15.471364	2022-01-14 20:39:15.471364
14	objects	put	2022-01-14 20:39:15.472435	2022-01-14 20:39:15.472435
15	objects	junk	2022-01-14 20:39:15.473556	2022-01-14 20:39:15.473556
16	objects	drop	2022-01-14 20:39:15.474645	2022-01-14 20:39:15.474645
17	objects	throw	2022-01-14 20:39:15.475607	2022-01-14 20:39:15.475607
18	preferences	prefs	2022-01-14 20:39:15.476558	2022-01-14 20:39:15.476558
19	shops	buy	2022-01-14 20:39:15.477514	2022-01-14 20:39:15.477514
20	shops	list	2022-01-14 20:39:15.478422	2022-01-14 20:39:15.478422
21	stats	score	2022-01-14 20:39:15.479308	2022-01-14 20:39:15.479308
22	stats	skills	2022-01-14 20:39:15.480251	2022-01-14 20:39:15.480251
23	stats	exp	2022-01-14 20:39:15.481087	2022-01-14 20:39:15.481087
24	ranged	combat	2022-01-14 20:39:15.481873	2022-01-14 20:39:15.481873
25	ranged	snipe	2022-01-14 20:39:15.48263	2022-01-14 20:39:15.48263
26	ranged	scan	2022-01-14 20:39:15.483423	2022-01-14 20:39:15.483423
27	ranged	spray	2022-01-14 20:39:15.484188	2022-01-14 20:39:15.484188
28	ranged	reload	2022-01-14 20:39:15.48495	2022-01-14 20:39:15.48495
29	melee	combat	2022-01-14 20:39:15.485638	2022-01-14 20:39:15.485638
30	melee	kill	2022-01-14 20:39:15.486319	2022-01-14 20:39:15.486319
31	melee	set_rules_of_gengagement	2022-01-14 20:39:15.486999	2022-01-14 20:39:15.486999
32	melee	set_combat_order	2022-01-14 20:39:15.487729	2022-01-14 20:39:15.487729
33	melee	list_combat_order	2022-01-14 20:39:15.488512	2022-01-14 20:39:15.488512
34	melee	clear_combat_order	2022-01-14 20:39:15.489229	2022-01-14 20:39:15.489229
35	melee	dispatch	2022-01-14 20:39:15.489893	2022-01-14 20:39:15.489893
36	melee	stance	2022-01-14 20:39:15.490525	2022-01-14 20:39:15.490525
37	hacking	hack	2022-01-14 20:39:15.491224	2022-01-14 20:39:15.491224
38	hacking	rotate_right	2022-01-14 20:39:15.491919	2022-01-14 20:39:15.491919
39	hacking	rotate_left	2022-01-14 20:39:15.492574	2022-01-14 20:39:15.492574
40	hacking	next_row	2022-01-14 20:39:15.493271	2022-01-14 20:39:15.493271
41	hacking	reset_hack	2022-01-14 20:39:15.493899	2022-01-14 20:39:15.493899
42	hacking	plug_cable	2022-01-14 20:39:15.494521	2022-01-14 20:39:15.494521
43	progression	score	2022-01-14 20:39:15.495195	2022-01-14 20:39:15.495195
44	progression	buy_practice	2022-01-14 20:39:15.495763	2022-01-14 20:39:15.495763
45	progression	skills	2022-01-14 20:39:15.496433	2022-01-14 20:39:15.496433
46	progression	practice	2022-01-14 20:39:15.49709	2022-01-14 20:39:15.49709
47	progression	mp	2022-01-14 20:39:15.497664	2022-01-14 20:39:15.497664
48	progression	levels	2022-01-14 20:39:15.498218	2022-01-14 20:39:15.498218
49	world	put	2022-01-14 20:39:15.498692	2022-01-14 20:39:15.498692
50	world	get	2022-01-14 20:39:15.499132	2022-01-14 20:39:15.499132
51	world	drop	2022-01-14 20:39:15.499606	2022-01-14 20:39:15.499606
52	world	give	2022-01-14 20:39:15.500064	2022-01-14 20:39:15.500064
53	world	drink	2022-01-14 20:39:15.500486	2022-01-14 20:39:15.500486
54	world	eat	2022-01-14 20:39:15.500933	2022-01-14 20:39:15.500933
55	world	pour	2022-01-14 20:39:15.501369	2022-01-14 20:39:15.501369
56	world	wear	2022-01-14 20:39:15.501792	2022-01-14 20:39:15.501792
57	world	wield	2022-01-14 20:39:15.502272	2022-01-14 20:39:15.502272
58	world	grab	2022-01-14 20:39:15.502713	2022-01-14 20:39:15.502713
59	world	remove	2022-01-14 20:39:15.503145	2022-01-14 20:39:15.503145
60	items	inventory	2022-01-14 20:39:15.503584	2022-01-14 20:39:15.503584
61	items	equipment	2022-01-14 20:39:15.50403	2022-01-14 20:39:15.50403
62	items	drone	2022-01-14 20:39:15.504458	2022-01-14 20:39:15.504458
63	session	quit	2022-01-14 20:39:15.504887	2022-01-14 20:39:15.504887
64	droning	drone	2022-01-14 20:39:15.50531	2022-01-14 20:39:15.50531
65	helpful	recall	2022-01-14 20:39:15.505734	2022-01-14 20:39:15.505734
66	chat	who	2022-01-14 20:39:15.50618	2022-01-14 20:39:15.50618
67	chat	users	2022-01-14 20:39:15.506618	2022-01-14 20:39:15.506618
68	informative	exits	2022-01-14 20:39:15.50705	2022-01-14 20:39:15.50705
69	informative	look	2022-01-14 20:39:15.507489	2022-01-14 20:39:15.507489
70	informative	examine	2022-01-14 20:39:15.507938	2022-01-14 20:39:15.507938
71	informative	weather	2022-01-14 20:39:15.508369	2022-01-14 20:39:15.508369
72	informative	gen_ps	2022-01-14 20:39:15.508799	2022-01-14 20:39:15.508799
73	informative	where	2022-01-14 20:39:15.50925	2022-01-14 20:39:15.50925
74	informative	consider	2022-01-14 20:39:15.509693	2022-01-14 20:39:15.509693
75	informative	diagnose	2022-01-14 20:39:15.510102	2022-01-14 20:39:15.510102
76	informative	color	2022-01-14 20:39:15.510501	2022-01-14 20:39:15.510501
77	informative	toggle	2022-01-14 20:39:15.510905	2022-01-14 20:39:15.510905
78	informative	commands	2022-01-14 20:39:15.511314	2022-01-14 20:39:15.511314
79	informative	view	2022-01-14 20:39:15.511713	2022-01-14 20:39:15.511713
80	other	quit	2022-01-14 20:39:15.512103	2022-01-14 20:39:15.512103
81	other	save	2022-01-14 20:39:15.512486	2022-01-14 20:39:15.512486
82	other	not_here	2022-01-14 20:39:15.512879	2022-01-14 20:39:15.512879
83	other	sneak	2022-01-14 20:39:15.513258	2022-01-14 20:39:15.513258
84	other	hide	2022-01-14 20:39:15.513646	2022-01-14 20:39:15.513646
85	other	steal	2022-01-14 20:39:15.51403	2022-01-14 20:39:15.51403
86	other	visible	2022-01-14 20:39:15.514423	2022-01-14 20:39:15.514423
87	other	title	2022-01-14 20:39:15.514807	2022-01-14 20:39:15.514807
88	other	group	2022-01-14 20:39:15.515189	2022-01-14 20:39:15.515189
89	other	ungroup	2022-01-14 20:39:15.51559	2022-01-14 20:39:15.51559
90	other	report	2022-01-14 20:39:15.515973	2022-01-14 20:39:15.515973
91	other	split	2022-01-14 20:39:15.516359	2022-01-14 20:39:15.516359
92	other	use	2022-01-14 20:39:15.516748	2022-01-14 20:39:15.516748
93	other	wimpy	2022-01-14 20:39:15.517132	2022-01-14 20:39:15.517132
94	other	display	2022-01-14 20:39:15.517526	2022-01-14 20:39:15.517526
95	other	gen_write	2022-01-14 20:39:15.517903	2022-01-14 20:39:15.517903
96	other	gen_tog	2022-01-14 20:39:15.518292	2022-01-14 20:39:15.518292
97	builder(admin)	flush_holding	2022-01-14 20:39:15.518687	2022-01-14 20:39:15.518687
98	builder(admin)	hold_anything	2022-01-14 20:39:15.519075	2022-01-14 20:39:15.519075
99	builder(admin)	uuid	2022-01-14 20:39:15.519478	2022-01-14 20:39:15.519478
100	offensive	throw	2022-01-14 20:39:15.519865	2022-01-14 20:39:15.519865
101	offensive	snipe_object	2022-01-14 20:39:15.520259	2022-01-14 20:39:15.520259
102	offensive	snipe	2022-01-14 20:39:15.520645	2022-01-14 20:39:15.520645
103	offensive	spray	2022-01-14 20:39:15.521034	2022-01-14 20:39:15.521034
104	offensive	silencers_on	2022-01-14 20:39:15.521433	2022-01-14 20:39:15.521433
105	offensive	silencers_off	2022-01-14 20:39:15.521817	2022-01-14 20:39:15.521817
106	offensive	go_loud	2022-01-14 20:39:15.522206	2022-01-14 20:39:15.522206
107	offensive	engagement_mode	2022-01-14 20:39:15.522611	2022-01-14 20:39:15.522611
108	offensive	regroup	2022-01-14 20:39:15.522997	2022-01-14 20:39:15.522997
109	offensive	command_sequence	2022-01-14 20:39:15.5234	2022-01-14 20:39:15.5234
110	offensive	scan	2022-01-14 20:39:15.523786	2022-01-14 20:39:15.523786
111	offensive	assist	2022-01-14 20:39:15.524176	2022-01-14 20:39:15.524176
112	offensive	hit	2022-01-14 20:39:15.52456	2022-01-14 20:39:15.52456
113	offensive	kill	2022-01-14 20:39:15.524943	2022-01-14 20:39:15.524943
114	offensive	backstab	2022-01-14 20:39:15.525337	2022-01-14 20:39:15.525337
115	offensive	order	2022-01-14 20:39:15.52572	2022-01-14 20:39:15.52572
116	offensive	flee	2022-01-14 20:39:15.526103	2022-01-14 20:39:15.526103
117	offensive	bash	2022-01-14 20:39:15.526497	2022-01-14 20:39:15.526497
118	offensive	rescue	2022-01-14 20:39:15.526899	2022-01-14 20:39:15.526899
119	offensive	kick	2022-01-14 20:39:15.527284	2022-01-14 20:39:15.527284
120	defensive	heal	2022-01-14 20:39:15.527702	2022-01-14 20:39:15.527702
121	defensive	revive	2022-01-14 20:39:15.528086	2022-01-14 20:39:15.528086
122	movement	move	2022-01-14 20:39:15.528472	2022-01-14 20:39:15.528472
123	movement	gen_door	2022-01-14 20:39:15.528858	2022-01-14 20:39:15.528858
124	movement	enter	2022-01-14 20:39:15.529286	2022-01-14 20:39:15.529286
125	movement	leave	2022-01-14 20:39:15.529658	2022-01-14 20:39:15.529658
126	movement	stand	2022-01-14 20:39:15.530029	2022-01-14 20:39:15.530029
127	movement	sit	2022-01-14 20:39:15.530402	2022-01-14 20:39:15.530402
128	movement	rest	2022-01-14 20:39:15.530767	2022-01-14 20:39:15.530767
129	movement	sleep	2022-01-14 20:39:15.531132	2022-01-14 20:39:15.531132
130	movement	wake	2022-01-14 20:39:15.531529	2022-01-14 20:39:15.531529
131	movement	follow	2022-01-14 20:39:15.531906	2022-01-14 20:39:15.531906
132	weapon-introspection	stats	2022-01-14 20:39:15.532297	2022-01-14 20:39:15.532297
133	demolitions	cancel	2022-01-14 20:39:15.532666	2022-01-14 20:39:15.532666
134	demolitions	install	2022-01-14 20:39:15.533021	2022-01-14 20:39:15.533021
135	demolitions	uninstall	2022-01-14 20:39:15.533391	2022-01-14 20:39:15.533391
136	demolitions	breach	2022-01-14 20:39:15.533748	2022-01-14 20:39:15.533748
137	demolitions	thermite	2022-01-14 20:39:15.53412	2022-01-14 20:39:15.53412
138	informative	room_vnum	2022-01-14 20:39:15.534471	2022-01-14 20:39:15.534471
139	helpful	alias	2022-01-14 20:39:15.534829	2022-01-14 20:39:15.534829
140	communication	say	2022-01-14 20:39:15.535192	2022-01-14 20:39:15.535192
141	communication	gsay	2022-01-14 20:39:15.535552	2022-01-14 20:39:15.535552
142	communication	tell	2022-01-14 20:39:15.535911	2022-01-14 20:39:15.535911
143	communication	reply	2022-01-14 20:39:15.536266	2022-01-14 20:39:15.536266
144	communication	spec_comm	2022-01-14 20:39:15.536629	2022-01-14 20:39:15.536629
145	communication	write	2022-01-14 20:39:15.536988	2022-01-14 20:39:15.536988
146	communication	gen_comm	2022-01-14 20:39:15.537349	2022-01-14 20:39:15.537349
147	communication	qcomm	2022-01-14 20:39:15.537701	2022-01-14 20:39:15.537701
148	social	action	2022-01-14 20:39:15.538053	2022-01-14 20:39:15.538053
149	social	insult	2022-01-14 20:39:15.538408	2022-01-14 20:39:15.538408
\.


--
-- Name: help_pages_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.help_pages_id_seq', 1434, true);


--
-- Name: help_topics_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.help_topics_id_seq', 149, true);


--
-- Name: help_pages help_pages_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.help_pages
    ADD CONSTRAINT help_pages_pkey PRIMARY KEY (id);


--
-- Name: help_topics help_topics_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.help_topics
    ADD CONSTRAINT help_topics_pkey PRIMARY KEY (id);


--
-- PostgreSQL database dump complete
--

