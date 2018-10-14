--
-- PostgreSQL database dump
--
-- Dumped from database version 10.3 (Ubuntu 10.3-1.pgdg16.04+1)
-- Dumped by pg_dump version 10.3 (Ubuntu 10.3-1.pgdg16.04+1)
SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET client_min_messages = warning;
SET row_security = off;
--
-- Name: plpgsql; Type: EXTENSION; Schema: -; Owner: 
--
CREATE EXTENSION IF NOT EXISTS plpgsql WITH SCHEMA pg_catalog;
--
-- Name: EXTENSION plpgsql; Type: COMMENT; Schema: -; Owner: 
--
COMMENT ON EXTENSION plpgsql IS 'PL/pgSQL procedural language';
SET default_tablespace = '';
SET default_with_oids = false;
--
-- Name: affected_type; Type: TABLE; Schema: public; Owner: llvm
--
CREATE TABLE public.affected_type (
    id integer NOT NULL,
    aff_fk_id integer NOT NULL,
    aff_location integer NOT NULL,
    aff_modifier integer NOT NULL
);
ALTER TABLE public.affected_type OWNER TO llvm;
--
-- Name: affected_type_id_seq; Type: SEQUENCE; Schema: public; Owner: llvm
--
CREATE SEQUENCE public.affected_type_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
ALTER TABLE public.affected_type_id_seq OWNER TO llvm;
--
-- Name: affected_type_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: llvm
--
ALTER SEQUENCE public.affected_type_id_seq OWNED BY public.affected_type.id;
--
-- Name: extra_description; Type: TABLE; Schema: public; Owner: llvm
--
CREATE TABLE public.extra_description (
    id integer NOT NULL,
    obj_fk_id integer NOT NULL,
    extra_keyword character varying(64),
    extra_description character varying(1024)
);
ALTER TABLE public.extra_description OWNER TO llvm;
--
-- Name: extra_description_id_seq; Type: SEQUENCE; Schema: public; Owner: llvm
--
CREATE SEQUENCE public.extra_description_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
ALTER TABLE public.extra_description_id_seq OWNER TO llvm;
--
-- Name: extra_description_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: llvm
--
ALTER SEQUENCE public.extra_description_id_seq OWNED BY public.extra_description.id;
--
-- Name: mob_zone; Type: TABLE; Schema: public; Owner: llvm
--
CREATE TABLE public.mob_zone (
    id integer NOT NULL,
    zone_id integer NOT NULL,
    mob_id integer NOT NULL,
    room_id integer NOT NULL,
    max_existing integer
);
ALTER TABLE public.mob_zone OWNER TO llvm;
--
-- Name: mob_zone_id_seq; Type: SEQUENCE; Schema: public; Owner: llvm
--
CREATE SEQUENCE public.mob_zone_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
ALTER TABLE public.mob_zone_id_seq OWNER TO llvm;
--
-- Name: mob_zone_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: llvm
--
ALTER SEQUENCE public.mob_zone_id_seq OWNED BY public.mob_zone.id;
--
-- Name: mobile; Type: TABLE; Schema: public; Owner: llvm
--
CREATE TABLE public.mobile (
    mob_id integer NOT NULL,
    mob_virtual_number integer NOT NULL,
    mob_name character varying(256) NOT NULL,
    mob_short_description character varying(64) NOT NULL,
    mob_long_description character varying(512) NOT NULL,
    mob_description text,
    mob_action_bitvector character varying(2) NOT NULL,
    mob_affection_bitvector character varying(2) NOT NULL,
    mob_ability_strength integer NOT NULL,
    mob_ability_strength_add integer NOT NULL,
    mob_ability_intelligence integer NOT NULL,
    mob_ability_wisdom integer NOT NULL,
    mob_ability_dexterity integer NOT NULL,
    mob_ability_constitution integer NOT NULL,
    mob_ability_charisma integer NOT NULL,
    mob_alignment integer NOT NULL,
    mob_attack_type integer NOT NULL,
    mob_level integer NOT NULL,
    mob_hitroll integer NOT NULL,
    mob_armor integer NOT NULL,
    mob_max_hitpoints integer NOT NULL,
    mob_max_mana integer NOT NULL,
    mob_max_move integer NOT NULL,
    mob_gold integer NOT NULL,
    mob_exp integer NOT NULL,
    mob_load_position integer NOT NULL,
    mob_default_position integer NOT NULL,
    mob_sex integer NOT NULL,
    mob_hitpoints integer,
    mob_mana integer NOT NULL,
    mob_move integer NOT NULL,
    mob_damnodice integer NOT NULL,
    mob_damsizedice integer NOT NULL,
    mob_damroll integer NOT NULL,
    mob_weight integer NOT NULL,
    mob_height integer NOT NULL,
    mob_class integer NOT NULL
);
ALTER TABLE public.mobile OWNER TO llvm;
--
-- Name: mobile_mob_id_seq; Type: SEQUENCE; Schema: public; Owner: llvm
--
CREATE SEQUENCE public.mobile_mob_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
ALTER TABLE public.mobile_mob_id_seq OWNER TO llvm;
--
-- Name: mobile_mob_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: llvm
--
ALTER SEQUENCE public.mobile_mob_id_seq OWNED BY public.mobile.mob_id;
--
-- Name: object; Type: TABLE; Schema: public; Owner: llvm
--
CREATE TABLE public.object (
    id integer NOT NULL,
    obj_item_number integer NOT NULL,
    obj_flags integer NOT NULL,
    obj_name character varying(128) NOT NULL,
    obj_description character varying(1024),
    obj_short_description character varying(64) NOT NULL,
    obj_action_description character varying(32),
    obj_type integer,
    obj_worn_on integer,
    obj_type_data character varying(16),
    obj_extra_keyword character varying(64),
    obj_extra_description character varying(1024)
);
ALTER TABLE public.object OWNER TO llvm;
--
-- Name: object_flags; Type: TABLE; Schema: public; Owner: llvm
--
CREATE TABLE public.object_flags (
    id integer NOT NULL,
    obj_fk_id integer NOT NULL,
    value_0 integer NOT NULL,
    value_1 integer NOT NULL,
    value_2 integer NOT NULL,
    value_3 integer NOT NULL,
    type_flag integer NOT NULL,
    wear_flags integer NOT NULL,
    extra_flags integer NOT NULL,
    weight integer NOT NULL,
    cost integer NOT NULL,
    cost_per_day integer NOT NULL,
    timer integer NOT NULL,
    bitvector integer NOT NULL
);
ALTER TABLE public.object_flags OWNER TO llvm;
--
-- Name: object_flags_id_seq; Type: SEQUENCE; Schema: public; Owner: llvm
--
CREATE SEQUENCE public.object_flags_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
ALTER TABLE public.object_flags_id_seq OWNER TO llvm;
--
-- Name: object_flags_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: llvm
--
ALTER SEQUENCE public.object_flags_id_seq OWNED BY public.object_flags.id;
--
-- Name: object_id_seq; Type: SEQUENCE; Schema: public; Owner: llvm
--
CREATE SEQUENCE public.object_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
ALTER TABLE public.object_id_seq OWNER TO llvm;
--
-- Name: object_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: llvm
--
ALTER SEQUENCE public.object_id_seq OWNED BY public.object.id;
--
-- Name: object_weapon; Type: TABLE; Schema: public; Owner: llvm
--
CREATE TABLE public.object_weapon (
    id integer NOT NULL,
    obj_fk_id integer NOT NULL,
    obj_ammo_max integer NOT NULL,
    obj_ammo_type integer NOT NULL,
    obj_cooldown integer NOT NULL,
    obj_can_snipe boolean NOT NULL
);
ALTER TABLE public.object_weapon OWNER TO llvm;
--
-- Name: object_weapon_id_seq; Type: SEQUENCE; Schema: public; Owner: llvm
--
CREATE SEQUENCE public.object_weapon_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
ALTER TABLE public.object_weapon_id_seq OWNER TO llvm;
--
-- Name: object_weapon_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: llvm
--
ALTER SEQUENCE public.object_weapon_id_seq OWNED BY public.object_weapon.id;
--
-- Name: player; Type: TABLE; Schema: public; Owner: llvm
--
CREATE TABLE public.player (
    player_id integer NOT NULL,
    player_virtual_number integer NOT NULL,
    player_name character varying(256) NOT NULL,
    player_short_description character varying(64) NOT NULL,
    player_long_description character varying(512) NOT NULL,
    player_description text,
    player_action_bitvector character varying(2) NOT NULL,
    player_affection_bitvector character varying(2) NOT NULL,
    player_ability_strength integer NOT NULL,
    player_ability_strength_add integer NOT NULL,
    player_ability_intelligence integer NOT NULL,
    player_ability_wisdom integer NOT NULL,
    player_ability_dexterity integer NOT NULL,
    player_ability_constitution integer NOT NULL,
    player_ability_charisma integer NOT NULL,
    player_alignment integer NOT NULL,
    player_attack_type integer NOT NULL,
    player_level integer NOT NULL,
    player_hitroll integer NOT NULL,
    player_armor integer NOT NULL,
    player_max_hitpoints integer NOT NULL,
    player_max_mana integer NOT NULL,
    player_max_move integer NOT NULL,
    player_gold integer NOT NULL,
    player_exp integer NOT NULL,
    player_sex integer NOT NULL,
    player_hitpoints integer,
    player_mana integer NOT NULL,
    player_move integer NOT NULL,
    player_damnodice integer NOT NULL,
    player_damsizedice integer NOT NULL,
    player_damroll integer NOT NULL,
    player_weight integer NOT NULL,
    player_height integer NOT NULL,
    player_class integer NOT NULL,
    player_title character varying(256) NOT NULL,
    player_hometown character varying(16) NOT NULL,
    player_password character varying(512),
    player_type character varying(3) NOT NULL
);
ALTER TABLE public.player OWNER TO llvm;
--
-- Name: player_player_id_seq; Type: SEQUENCE; Schema: public; Owner: llvm
--
CREATE SEQUENCE public.player_player_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
ALTER TABLE public.player_player_id_seq OWNER TO llvm;
--
-- Name: player_player_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: llvm
--
ALTER SEQUENCE public.player_player_id_seq OWNED BY public.player.player_id;
--
-- Name: room; Type: TABLE; Schema: public; Owner: llvm
--
CREATE TABLE public.room (
    id integer NOT NULL,
    room_number integer NOT NULL,
    zone integer NOT NULL,
    sector_type integer NOT NULL,
    name character varying(256) NOT NULL,
    description text NOT NULL,
    ex_keyword character varying(256),
    ex_description text,
    light integer,
    room_flag integer NOT NULL
);
ALTER TABLE public.room OWNER TO llvm;
--
-- Name: room_direction_data; Type: TABLE; Schema: public; Owner: llvm
--
CREATE TABLE public.room_direction_data (
    id integer NOT NULL,
    room_number integer NOT NULL,
    exit_direction integer NOT NULL,
    general_description character varying(256) NOT NULL,
    keyword character varying(16),
    exit_info integer,
    exit_key integer,
    to_room integer NOT NULL
);
ALTER TABLE public.room_direction_data OWNER TO llvm;
--
-- Name: room_direction_data_id_seq; Type: SEQUENCE; Schema: public; Owner: llvm
--
CREATE SEQUENCE public.room_direction_data_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
ALTER TABLE public.room_direction_data_id_seq OWNER TO llvm;
--
-- Name: room_direction_data_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: llvm
--
ALTER SEQUENCE public.room_direction_data_id_seq OWNED BY public.room_direction_data.id;
--
-- Name: room_id_seq; Type: SEQUENCE; Schema: public; Owner: llvm
--
CREATE SEQUENCE public.room_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
ALTER TABLE public.room_id_seq OWNER TO llvm;
--
-- Name: room_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: llvm
--
ALTER SEQUENCE public.room_id_seq OWNED BY public.room.id;
--
-- Name: tasks; Type: TABLE; Schema: public; Owner: llvm
--
CREATE TABLE public.tasks (
    id integer,
    t_name character varying(256)
);
ALTER TABLE public.tasks OWNER TO llvm;
--
-- Name: tasks_notes; Type: TABLE; Schema: public; Owner: llvm
--
CREATE TABLE public.tasks_notes (
    id integer,
    task_id integer,
    t_notes character varying(256)
);
ALTER TABLE public.tasks_notes OWNER TO llvm;
--
-- Name: zone; Type: TABLE; Schema: public; Owner: llvm
--
CREATE TABLE public.zone (
    id integer NOT NULL,
    zone_start integer NOT NULL,
    zone_end integer NOT NULL,
    zone_name character varying(64) NOT NULL,
    lifespan integer NOT NULL,
    reset_mode integer NOT NULL
);
ALTER TABLE public.zone OWNER TO llvm;
--
-- Name: zone_data; Type: TABLE; Schema: public; Owner: llvm
--
CREATE TABLE public.zone_data (
    id integer NOT NULL,
    zone_id integer NOT NULL,
    zone_command character varying(1) NOT NULL,
    zone_if_flag character varying(1) NOT NULL,
    zone_arg1 integer NOT NULL,
    zone_arg2 integer NOT NULL,
    zone_arg3 integer NOT NULL
);
ALTER TABLE public.zone_data OWNER TO llvm;
--
-- Name: zone_data_id_seq; Type: SEQUENCE; Schema: public; Owner: llvm
--
CREATE SEQUENCE public.zone_data_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
ALTER TABLE public.zone_data_id_seq OWNER TO llvm;
--
-- Name: zone_data_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: llvm
--
ALTER SEQUENCE public.zone_data_id_seq OWNED BY public.zone_data.id;
--
-- Name: zone_id_seq; Type: SEQUENCE; Schema: public; Owner: llvm
--
CREATE SEQUENCE public.zone_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
ALTER TABLE public.zone_id_seq OWNER TO llvm;
--
-- Name: zone_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: llvm
--
ALTER SEQUENCE public.zone_id_seq OWNED BY public.zone.id;
--
-- Name: affected_type id; Type: DEFAULT; Schema: public; Owner: llvm
--
ALTER TABLE ONLY public.affected_type ALTER COLUMN id SET DEFAULT nextval('public.affected_type_id_seq'::regclass);
--
-- Name: extra_description id; Type: DEFAULT; Schema: public; Owner: llvm
--
ALTER TABLE ONLY public.extra_description ALTER COLUMN id SET DEFAULT nextval('public.extra_description_id_seq'::regclass);
--
-- Name: mob_zone id; Type: DEFAULT; Schema: public; Owner: llvm
--
ALTER TABLE ONLY public.mob_zone ALTER COLUMN id SET DEFAULT nextval('public.mob_zone_id_seq'::regclass);
--
-- Name: mobile mob_id; Type: DEFAULT; Schema: public; Owner: llvm
--
ALTER TABLE ONLY public.mobile ALTER COLUMN mob_id SET DEFAULT nextval('public.mobile_mob_id_seq'::regclass);
--
-- Name: object id; Type: DEFAULT; Schema: public; Owner: llvm
--
ALTER TABLE ONLY public.object ALTER COLUMN id SET DEFAULT nextval('public.object_id_seq'::regclass);
--
-- Name: object_flags id; Type: DEFAULT; Schema: public; Owner: llvm
--
ALTER TABLE ONLY public.object_flags ALTER COLUMN id SET DEFAULT nextval('public.object_flags_id_seq'::regclass);
--
-- Name: object_weapon id; Type: DEFAULT; Schema: public; Owner: llvm
--
ALTER TABLE ONLY public.object_weapon ALTER COLUMN id SET DEFAULT nextval('public.object_weapon_id_seq'::regclass);
--
-- Name: player player_id; Type: DEFAULT; Schema: public; Owner: llvm
--
ALTER TABLE ONLY public.player ALTER COLUMN player_id SET DEFAULT nextval('public.player_player_id_seq'::regclass);
--
-- Name: room id; Type: DEFAULT; Schema: public; Owner: llvm
--
ALTER TABLE ONLY public.room ALTER COLUMN id SET DEFAULT nextval('public.room_id_seq'::regclass);
--
-- Name: room_direction_data id; Type: DEFAULT; Schema: public; Owner: llvm
--
ALTER TABLE ONLY public.room_direction_data ALTER COLUMN id SET DEFAULT nextval('public.room_direction_data_id_seq'::regclass);
--
-- Name: zone id; Type: DEFAULT; Schema: public; Owner: llvm
--
ALTER TABLE ONLY public.zone ALTER COLUMN id SET DEFAULT nextval('public.zone_id_seq'::regclass);
--
-- Name: zone_data id; Type: DEFAULT; Schema: public; Owner: llvm
--
ALTER TABLE ONLY public.zone_data ALTER COLUMN id SET DEFAULT nextval('public.zone_data_id_seq'::regclass);
--
-- Data for Name: affected_type; Type: TABLE DATA; Schema: public; Owner: llvm
--
--
-- Data for Name: extra_description; Type: TABLE DATA; Schema: public; Owner: llvm
--
--
-- Data for Name: mob_zone; Type: TABLE DATA; Schema: public; Owner: llvm
--
--
-- Data for Name: mobile; Type: TABLE DATA; Schema: public; Owner: llvm
--
INSERT INTO public.mobile VALUES (1, 0, 'chef putos employee', 'A pissed looking Los Putos employee', 'A pissed looking Los Putos employee', 'This particular employee looks like he just got out of a federal penitentiary. He''s most likely hiding some weapon in one of the many compartments that should be used for storing utensils and food paraphernalia.', '8', '0', 25, 0, 25, 0, 25, 0, 25, 0, 0, 0, 20, 0, 250, 250, 250, 5000, 0, 8, 8, 0, 250, 250, 250, 25, 0, 0, 50, 15, 0);
--
-- Data for Name: object; Type: TABLE DATA; Schema: public; Owner: llvm
--
INSERT INTO public.object VALUES (4, 90, 0, 'p90 smg', 'A P90 lies here.', 'A P90', 'A P90', 0, 0, '0', 'p90', 'The FN P90, also known as the FN Project 1990, is a personal defense weapon (PDW) designed and manufactured by FN Herstal in Belgium. Created in response to NATO requests for a replacement for 919mm Parabellum firearms, the P90 was designed as a compact but powerful firearm for vehicle crews, operators of crew-served weapons, support personnel, special forces, and counter-terrorist groups.');
INSERT INTO public.object VALUES (5, 91, 0, 'p90 smg', 'A P90 lies here.', 'A P90', 'A P90', 0, 0, '0', 'p90', 'A P90');
INSERT INTO public.object VALUES (6, 94, 0, 'xm109 xm snipe sniper rifle', 'A devastating XM109 lies here.', 'A devastating XM109', 'A devastating XM109', 0, 0, '0', 'xm109', 'A devastating XM109');
INSERT INTO public.object VALUES (1, 139, 0, '[ammo] ammo xm109 [xm109-ammo] {snipe}', 'A bunch of XM109 ammo has been left here.', 'XM109 Ammo', NULL, 0, -1, '0', 'ammo [ammo] xm109 [xm109-ammo]', 'It''s a bunch of XM109 ammo.');
INSERT INTO public.object VALUES (2, 137, 0, 'ump45 ump sub smg [ump45]', 'A Heckler & Koch UMP45 lies here.', 'a Heckler & Koch UMP45', 'A UMP45', 0, -1, '0', '<default>', '<default>');
INSERT INTO public.object VALUES (3, 1, 0, 'rack weapon weapons', 'A military grade weapons rack is attached to the wall.', 'A weapons rack', 'rack', 0, 0, '0', 'rack', 'Assault rifles, SMG''s, and high powered sniper rifles fill this weapons rack. The only question is: how will you use it?');
--
-- Data for Name: object_flags; Type: TABLE DATA; Schema: public; Owner: llvm
--
INSERT INTO public.object_flags VALUES (4, 2, 0, 0, 0, 0, 5, 8193, 0, 0, 0, 0, 0, 0);
INSERT INTO public.object_flags VALUES (9, 3, 0, 0, 0, 0, 15, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO public.object_flags VALUES (14, 4, 0, 0, 0, 0, 5, 8193, 0, 0, 0, 0, 0, 0);
INSERT INTO public.object_flags VALUES (15, 5, 0, 0, 0, 0, 5, 8193, 0, 0, 0, 0, 0, 0);
INSERT INTO public.object_flags VALUES (16, 6, 0, 5, 250, 7, 5, 8193, 0, 0, 0, 0, 0, 0);
--
-- Data for Name: object_weapon; Type: TABLE DATA; Schema: public; Owner: llvm
--
INSERT INTO public.object_weapon VALUES (1, 4, 0, 0, 0, false);
INSERT INTO public.object_weapon VALUES (2, 5, 0, 0, 0, false);
INSERT INTO public.object_weapon VALUES (3, 6, 0, 1, 0, false);
--
-- Data for Name: player; Type: TABLE DATA; Schema: public; Owner: llvm
--
--
-- Data for Name: room; Type: TABLE DATA; Schema: public; Owner: llvm
--
INSERT INTO public.room VALUES (5, 27089, 2, 0, 'Weapon Supply', 'Once a haven for the oft-aggressive civilian control brigade, this armory lays waste to any idea of neutrality and the higher perceptions of peace for the human race. Arms deals and aggressive legislation have made this room possible. Before you stands racks of weapons and sentry artillery that once was used to lay waste to factions of dissident followers and extremist groups that don''t fit {red}the plan{/red}. A {gld}sign{/gld} is on the wall describing the various actions that can be done in this room. Gear up and head out soldier.', '', '', 0, 0);
INSERT INTO public.room VALUES (6, 27087, 2, 0, '{grn}Sector Defense & Communications{/grn}', 'The brightest minds in the INFOSEC industry lend their technological expertise to some of the most advanced cyber intelligence weaponry currently displayed before you in the form of sever racks and terminals. Artificial intelligence modules running in a background process now replace what once was a living breathing human being. The changes of technological growth and their impact on human reliance are evident in the absence of human attendees at these once busy terminals. What once was a busy room that needed human interaction is now a room full of machines that can do the job infinitely faster and more efficiently. ', '', '', 0, 0);
INSERT INTO public.room VALUES (79, 122, 10, 0, 'Downtown Lawton City', 'Make an entry into the ''Guinness Book of World Records'' for the absolute most disgusting street you''ve ever encountered. Trash is strewn apathetically across every conceivable walkway area. The crew responsible for cleaning this place hasn''t visited in quite a long time from the looks of it. Homeless drug addicts are lying in their own self-contained cardboard hells.', NULL, NULL, 0, 0);
INSERT INTO public.room VALUES (80, 123, 10, 0, 'Francesco''s Italian Food', 'A refreshing change from the trashy cesspool that is Downtown Lawton. The waiting area immediately greets you, but there is nobody there. Air conditioning keeps the place either too hot or too cold-- there is no middle ground. A waitress can be seen putting too much salt in a dish to keep a certain customer from coming back. ', NULL, NULL, 0, 0);
INSERT INTO public.room VALUES (81, 124, 10, 0, 'Reverand Avenue', 'Of all the things that can be attributed to Downtown Lawton, faith surely is not one of them. From the abandoned streets and overcrowded walkways protrudes the infamous Sixth Tower Church. Worshippers gather and pay heed to the all famous deity. ', NULL, NULL, 0, 0);
INSERT INTO public.room VALUES (82, 125, 10, 0, 'Sixth Tower Church', 'Anyone would consider joining the Sixth Tower Church. That is, if the cleanliness of a patron''s soul matched the cleanliness of the inside of this chapel. This area is largely empty which contributes to the calming effect that the silence has on anything that steps into this chapel.', NULL, NULL, 0, 0);
INSERT INTO public.room VALUES (83, 126, 10, 0, 'Reverand Avenue', 'Of all the things that can be attributed to Downtown Lawton, faith surely is not one of them. From the abandoned streets and overcrowded walkways protrudes the infamous Sixth Tower Church. Worshippers gather and pay heed to the all famous deity. ', NULL, NULL, 0, 0);
INSERT INTO public.room VALUES (84, 127, 10, 0, 'Armor And Supplies', 'description', NULL, NULL, 0, 0);
INSERT INTO public.room VALUES (85, 128, 10, 0, 'Reverand Avenue', 'Of all the things that can be attributed to Downtown Lawton, faith surely is not one of them. From the abandoned streets and overcrowded walkways protrudes the infamous Sixth Tower Church. Worshippers gather and pay heed to the all famous deity. ', NULL, NULL, 0, 0);
INSERT INTO public.room VALUES (86, 129, 10, 0, 'Los Putos Mexican Food', 'description', NULL, NULL, 0, 0);
INSERT INTO public.room VALUES (87, 130, 10, 0, 'title', 'description', NULL, NULL, 0, 0);
INSERT INTO public.room VALUES (88, 131, 10, 0, 'Los Putos Mexican Food Back Entrance', 'description', NULL, NULL, 0, 0);
INSERT INTO public.room VALUES (89, 132, 10, 0, 'Entrance to an underground tunnel', 'description', NULL, NULL, 0, 0);
INSERT INTO public.room VALUES (90, 133, 10, 0, 'Inside a tunnel', 'This crudely carved out tunnel allows the average height male to comfortably walk through. The purpose of this tunnel isn''t immediately obvious, but knowing the residents of Downtown Lawton, it''s likely connected to drugs. Blue canvases are draped across the ground and grey canvases are laid over the padded walls. Why are the walls padded?', NULL, NULL, 0, 0);
INSERT INTO public.room VALUES (91, 134, 10, 0, 'Inside a tunnel', 'This crudely carved out tunnel allows the average height male to comfortably walk through. The purpose of this tunnel isn''t immediately obvious, but knowing the residents of Downtown Lawton, it''s likely connected to drugs. Blue canvases are draped across the ground and grey canvases are laid over the padded walls. Why are the walls padded?', NULL, NULL, 0, 0);
INSERT INTO public.room VALUES (77, 114, 1, 0, 'Outside The Operations Building', 'The crisp air assaults every bit of your body that is exposed to the elements. Huge industrial buildings obscure your view of the mountainous backdrop surrounding this vast metropolis. City buses will occasionally run by here. The street and sidewalk areas are filthy. {red}Welcome to Lawton City{/red}', NULL, NULL, 0, 0);
INSERT INTO public.room VALUES (63, 100, 1, 0, 'title', 'description', NULL, NULL, 0, 0);
INSERT INTO public.room VALUES (64, 101, 1, 0, 'title', 'description', NULL, NULL, 0, 0);
INSERT INTO public.room VALUES (65, 102, 1, 0, 'title', 'description', NULL, NULL, 0, 0);
INSERT INTO public.room VALUES (66, 103, 1, 0, 'title', 'description', NULL, NULL, 0, 0);
INSERT INTO public.room VALUES (67, 104, 1, 0, 'title', 'description', NULL, NULL, 0, 0);
INSERT INTO public.room VALUES (68, 105, 1, 0, 'title', 'description', NULL, NULL, 0, 0);
INSERT INTO public.room VALUES (69, 106, 1, 0, 'title', 'description', NULL, NULL, 0, 0);
INSERT INTO public.room VALUES (70, 107, 1, 0, 'title', 'description', NULL, NULL, 0, 0);
INSERT INTO public.room VALUES (71, 108, 1, 0, 'title', 'description', NULL, NULL, 0, 0);
INSERT INTO public.room VALUES (72, 109, 1, 0, 'title', 'description', NULL, NULL, 0, 0);
INSERT INTO public.room VALUES (73, 110, 1, 0, 'title', 'description', NULL, NULL, 0, 0);
INSERT INTO public.room VALUES (74, 111, 1, 0, 'title', 'description', NULL, NULL, 0, 0);
INSERT INTO public.room VALUES (75, 112, 1, 0, 'title', 'description', NULL, NULL, 0, 0);
INSERT INTO public.room VALUES (76, 113, 1, 0, 'title', 'description', NULL, NULL, 0, 0);
INSERT INTO public.room VALUES (4, 27088, 2, 0, '{grn}Directorate of Operations{/grn}', '	Three rows of terminals make up the center of this vast room, each with controlpanels and maintenance crew attending them. The northern end of the room is home to themany large monitors that keep this room constantly satiated with information. Humming machinesconvert and transmutate large quantities of data. Terrabytes of information are folded and manipulated-- parsed and dissected -- until the central hub and core AI modules spit outthe next crucial chunk of data that could potentially save or endanger the lives of millions.', '', '', 0, 8);
INSERT INTO public.room VALUES (78, 121, 10, 0, 'Downtown Lawton City', 'Make an entry into the ''Guinness Book of World Records'' for the absolute most disgusting street you''ve ever encountered. Trash is strewn apathetically across every conceivable walkway area. The crew responsible for cleaning this place hasn''t visited in quite a long time from the looks of it. Homeless drug addicts are lying in their own self-contained cardboard hells.', NULL, NULL, 0, 0);
INSERT INTO public.room VALUES (92, 135, 10, 0, 'Inside a tunnel', 'This crudely carved out tunnel allows the average height male to comfortably walk through. The purpose of this tunnel isn''t immediately obvious, but knowing the residents of Downtown Lawton, it''s likely connected to drugs. Blue canvases are draped across the ground and grey canvases are laid over the padded walls. Why are the walls padded?', NULL, NULL, 0, 0);
INSERT INTO public.room VALUES (93, 136, 10, 0, 'A dimly lit cave', 'The canvas floor and walls abruptly end here. A light is hung here providing just enough illumination to notice that there is a door to the west of here that likely leads to the real reason this underground system was built.', NULL, NULL, 0, 0);
INSERT INTO public.room VALUES (94, 137, 10, 0, 'A dimly lit cave', 'The canvas floor and walls abruptly end here. A light is hung here providing just enough illumination to notice that there is a door to the west of here that likely leads to the real reason this underground system was built.', NULL, NULL, 0, 0);
INSERT INTO public.room VALUES (95, 138, 10, 0, 'A dimly lit cave', 'The canvas floor and walls abruptly end here. A light is hung here providing just enough illumination to notice that there is a door to the west of here that likely leads to the real reason this underground system was built.', NULL, NULL, 0, 0);
INSERT INTO public.room VALUES (96, 139, 10, 0, 'Outside a fortified entrance', 'It''s not indestructible, but a normal breach charge wouldn''t cut through the thick armor that this door is fortified with. A {red}thermite{/red} charge would cut through that door, ableit very noisily.', NULL, NULL, 0, 0);
--
-- Data for Name: room_direction_data; Type: TABLE DATA; Schema: public; Owner: llvm
--
INSERT INTO public.room_direction_data VALUES (143, 27089, 0, 'general description', 'keyword', 1, -1, 113);
INSERT INTO public.room_direction_data VALUES (144, 27089, 3, '<default>', '<default>', 1, -1, 27088);
INSERT INTO public.room_direction_data VALUES (110, 100, 4, '{grn}Directorate of Operations{/grn}', 'door', 1, -1, 27088);
INSERT INTO public.room_direction_data VALUES (16, 27087, 1, '<default>', '<default>', 1, -1, 27088);
INSERT INTO public.room_direction_data VALUES (195, 125, 3, 'Reverand Avenue', 'door', 1, -1, 124);
INSERT INTO public.room_direction_data VALUES (186, 121, 0, 'general description', 'keyword', 1, -1, 124);
INSERT INTO public.room_direction_data VALUES (187, 121, 1, 'general description', 'keyword', 1, -1, 122);
INSERT INTO public.room_direction_data VALUES (188, 121, 2, 'Outside The Operations Building', 'door', 1, -1, 114);
INSERT INTO public.room_direction_data VALUES (189, 122, 1, 'general description', 'keyword', 1, -1, 123);
INSERT INTO public.room_direction_data VALUES (190, 122, 3, 'Downtown Lawton City', 'door', 1, -1, 121);
INSERT INTO public.room_direction_data VALUES (191, 123, 3, 'Downtown Lawton City', 'door', 1, -1, 122);
INSERT INTO public.room_direction_data VALUES (192, 124, 1, 'general description', 'keyword', 1, -1, 125);
INSERT INTO public.room_direction_data VALUES (193, 124, 2, 'Downtown Lawton City', 'door', 1, -1, 121);
INSERT INTO public.room_direction_data VALUES (194, 124, 3, 'general description', 'keyword', 1, -1, 126);
INSERT INTO public.room_direction_data VALUES (196, 126, 0, 'general description', 'keyword', 1, -1, 127);
INSERT INTO public.room_direction_data VALUES (197, 126, 1, 'Reverand Avenue', 'door', 1, -1, 124);
INSERT INTO public.room_direction_data VALUES (198, 126, 3, 'general description', 'keyword', 1, -1, 128);
INSERT INTO public.room_direction_data VALUES (199, 127, 2, 'Reverand Avenue', 'door', 1, -1, 126);
INSERT INTO public.room_direction_data VALUES (200, 128, 0, 'general description', 'keyword', 1, -1, 129);
INSERT INTO public.room_direction_data VALUES (201, 128, 1, 'Reverand Avenue', 'door', 1, -1, 126);
INSERT INTO public.room_direction_data VALUES (202, 129, 0, 'general description', 'keyword', 1, -1, 130);
INSERT INTO public.room_direction_data VALUES (203, 129, 2, 'Reverand Avenue', 'door', 1, -1, 128);
INSERT INTO public.room_direction_data VALUES (204, 130, 0, 'general description', 'keyword', 1, -1, 131);
INSERT INTO public.room_direction_data VALUES (205, 130, 2, 'Los Putos Mexican Food', 'door', 1, -1, 129);
INSERT INTO public.room_direction_data VALUES (206, 131, 2, 'title', 'door', 1, -1, 130);
INSERT INTO public.room_direction_data VALUES (207, 131, 5, 'general description', 'keyword', 1, -1, 132);
INSERT INTO public.room_direction_data VALUES (208, 132, 3, 'general description', 'keyword', 1, -1, 133);
INSERT INTO public.room_direction_data VALUES (209, 132, 4, 'Los Putos Mexican Food Back Entrance', 'door', 1, -1, 131);
INSERT INTO public.room_direction_data VALUES (210, 133, 1, 'Entrance to an underground tunnel', 'door', 1, -1, 132);
INSERT INTO public.room_direction_data VALUES (211, 133, 3, 'general description', 'keyword', 1, -1, 134);
INSERT INTO public.room_direction_data VALUES (212, 134, 1, 'Inside a tunnel', 'door', 1, -1, 133);
INSERT INTO public.room_direction_data VALUES (213, 134, 3, 'general description', 'keyword', 1, -1, 135);
INSERT INTO public.room_direction_data VALUES (214, 135, 0, 'general description', 'keyword', 1, -1, 136);
INSERT INTO public.room_direction_data VALUES (215, 135, 1, 'Inside a tunnel', 'door', 1, -1, 134);
INSERT INTO public.room_direction_data VALUES (216, 136, 2, 'Inside a tunnel', 'door', 1, -1, 135);
INSERT INTO public.room_direction_data VALUES (217, 136, 3, 'general description', 'keyword', 1, -1, 137);
INSERT INTO public.room_direction_data VALUES (218, 137, 1, 'A dimly lit cave', 'door', 1, -1, 136);
INSERT INTO public.room_direction_data VALUES (219, 137, 3, 'general description', 'keyword', 1, -1, 138);
INSERT INTO public.room_direction_data VALUES (220, 138, 1, 'A dimly lit cave', 'door', 1, -1, 137);
INSERT INTO public.room_direction_data VALUES (221, 138, 3, 'general description', 'keyword', 1, -1, 139);
INSERT INTO public.room_direction_data VALUES (111, 100, 5, 'general description', 'keyword', 1, -1, 101);
INSERT INTO public.room_direction_data VALUES (222, 139, 1, 'A dimly lit cave', 'door', 1, -1, 138);
INSERT INTO public.room_direction_data VALUES (223, 114, 0, 'general description', 'keyword', 1, -1, 122);
INSERT INTO public.room_direction_data VALUES (224, 114, 2, '{grn}Directorate of Operations{/grn}', 'door', 1, -1, 27088);
INSERT INTO public.room_direction_data VALUES (112, 101, 3, 'general description', 'keyword', 1, -1, 102);
INSERT INTO public.room_direction_data VALUES (113, 101, 4, 'title', 'door', 1, -1, 100);
INSERT INTO public.room_direction_data VALUES (114, 101, 5, 'general description', 'keyword', 1, -1, 103);
INSERT INTO public.room_direction_data VALUES (115, 102, 1, 'title', 'door', 1, -1, 101);
INSERT INTO public.room_direction_data VALUES (116, 103, 0, 'general description', 'keyword', 1, -1, 104);
INSERT INTO public.room_direction_data VALUES (117, 103, 4, 'title', 'door', 1, -1, 101);
INSERT INTO public.room_direction_data VALUES (118, 104, 0, 'general description', 'keyword', 1, -1, 105);
INSERT INTO public.room_direction_data VALUES (119, 104, 2, 'title', 'door', 1, -1, 103);
INSERT INTO public.room_direction_data VALUES (120, 105, 2, 'title', 'door', 1, -1, 104);
INSERT INTO public.room_direction_data VALUES (121, 105, 5, 'general description', 'keyword', 1, -1, 106);
INSERT INTO public.room_direction_data VALUES (122, 106, 0, 'general description', 'keyword', 1, -1, 107);
INSERT INTO public.room_direction_data VALUES (123, 106, 4, 'title', 'door', 1, -1, 105);
INSERT INTO public.room_direction_data VALUES (124, 107, 1, 'general description', 'keyword', 1, -1, 108);
INSERT INTO public.room_direction_data VALUES (125, 107, 2, 'title', 'door', 1, -1, 106);
INSERT INTO public.room_direction_data VALUES (126, 108, 1, 'general description', 'keyword', 1, -1, 109);
INSERT INTO public.room_direction_data VALUES (127, 108, 3, 'title', 'door', 1, -1, 107);
INSERT INTO public.room_direction_data VALUES (128, 109, 0, 'general description', 'keyword', 1, -1, 110);
INSERT INTO public.room_direction_data VALUES (129, 109, 3, 'title', 'door', 1, -1, 108);
INSERT INTO public.room_direction_data VALUES (130, 110, 0, 'general description', 'keyword', 1, -1, 111);
INSERT INTO public.room_direction_data VALUES (131, 110, 2, 'title', 'door', 1, -1, 109);
INSERT INTO public.room_direction_data VALUES (132, 111, 0, 'general description', 'keyword', 1, -1, 112);
INSERT INTO public.room_direction_data VALUES (133, 111, 2, 'title', 'door', 1, -1, 110);
INSERT INTO public.room_direction_data VALUES (134, 112, 2, 'title', 'door', 1, -1, 111);
INSERT INTO public.room_direction_data VALUES (135, 113, 2, '"Weapon supply"', 'door', 1, -1, 27089);
INSERT INTO public.room_direction_data VALUES (137, 27088, 0, 'general description', 'keyword', 1, -1, 114);
INSERT INTO public.room_direction_data VALUES (138, 27088, 1, '{grn}An Armory{/grn} lies to the east.', 'door', 1, -1, 27089);
INSERT INTO public.room_direction_data VALUES (139, 27088, 3, '{grn}Sector Defense & Communications{/grn} is to the west.', 'door', 1, -1, 27087);
INSERT INTO public.room_direction_data VALUES (140, 27088, 5, 'general description', 'keyword', 1, -1, 100);
--
-- Data for Name: tasks; Type: TABLE DATA; Schema: public; Owner: llvm
--
INSERT INTO public.tasks VALUES (0, 'task 0');
INSERT INTO public.tasks VALUES (1, 'task 1');
INSERT INTO public.tasks VALUES (2, 'task 2');
--
-- Data for Name: tasks_notes; Type: TABLE DATA; Schema: public; Owner: llvm
--
INSERT INTO public.tasks_notes VALUES (0, 1, 'foobar');
INSERT INTO public.tasks_notes VALUES (1, 1, 'funbar');
INSERT INTO public.tasks_notes VALUES (2, 1, 'bazbar');
--
-- Data for Name: zone; Type: TABLE DATA; Schema: public; Owner: llvm
--
INSERT INTO public.zone VALUES (7, 100, 120, 'D.O. Internals', 15, 2);
INSERT INTO public.zone VALUES (6, 27088, 30000, 'SLC', 1, 2);
INSERT INTO public.zone VALUES (10, 121, 200, 'Downtown Lawton', 10, 2);
--
-- Data for Name: zone_data; Type: TABLE DATA; Schema: public; Owner: llvm
--
INSERT INTO public.zone_data VALUES (6, 0, 'M', '1', 0, 10, 27088);
INSERT INTO public.zone_data VALUES (8, 0, 'M', '1', 0, 10, 27088);
INSERT INTO public.zone_data VALUES (13, 0, 'M', '0', 0, 10, 27088);
INSERT INTO public.zone_data VALUES (14, 0, 'M', '0', 0, 10, 27088);
INSERT INTO public.zone_data VALUES (15, 7, 'M', '0', 0, 10, 27088);
INSERT INTO public.zone_data VALUES (16, 7, 'O', '0', 94, 4, 27088);
--
-- Name: affected_type_id_seq; Type: SEQUENCE SET; Schema: public; Owner: llvm
--
SELECT pg_catalog.setval('public.affected_type_id_seq', 1, false);
--
-- Name: extra_description_id_seq; Type: SEQUENCE SET; Schema: public; Owner: llvm
--
SELECT pg_catalog.setval('public.extra_description_id_seq', 10, true);
--
-- Name: mob_zone_id_seq; Type: SEQUENCE SET; Schema: public; Owner: llvm
--
SELECT pg_catalog.setval('public.mob_zone_id_seq', 1, false);
--
-- Name: mobile_mob_id_seq; Type: SEQUENCE SET; Schema: public; Owner: llvm
--
SELECT pg_catalog.setval('public.mobile_mob_id_seq', 1, true);
--
-- Name: object_flags_id_seq; Type: SEQUENCE SET; Schema: public; Owner: llvm
--
SELECT pg_catalog.setval('public.object_flags_id_seq', 16, true);
--
-- Name: object_id_seq; Type: SEQUENCE SET; Schema: public; Owner: llvm
--
SELECT pg_catalog.setval('public.object_id_seq', 6, true);
--
-- Name: object_weapon_id_seq; Type: SEQUENCE SET; Schema: public; Owner: llvm
--
SELECT pg_catalog.setval('public.object_weapon_id_seq', 3, true);
--
-- Name: player_player_id_seq; Type: SEQUENCE SET; Schema: public; Owner: llvm
--
SELECT pg_catalog.setval('public.player_player_id_seq', 1, false);
--
-- Name: room_direction_data_id_seq; Type: SEQUENCE SET; Schema: public; Owner: llvm
--
SELECT pg_catalog.setval('public.room_direction_data_id_seq', 224, true);
--
-- Name: room_id_seq; Type: SEQUENCE SET; Schema: public; Owner: llvm
--
SELECT pg_catalog.setval('public.room_id_seq', 96, true);
--
-- Name: zone_data_id_seq; Type: SEQUENCE SET; Schema: public; Owner: llvm
--
SELECT pg_catalog.setval('public.zone_data_id_seq', 16, true);
--
-- Name: zone_id_seq; Type: SEQUENCE SET; Schema: public; Owner: llvm
--
SELECT pg_catalog.setval('public.zone_id_seq', 10, true);
--
-- Name: room_direction_data room_direction_data_pkey; Type: CONSTRAINT; Schema: public; Owner: llvm
--
ALTER TABLE ONLY public.room_direction_data
    ADD CONSTRAINT room_direction_data_pkey PRIMARY KEY (id);
--
-- Name: room room_room_number_key; Type: CONSTRAINT; Schema: public; Owner: llvm
--
ALTER TABLE ONLY public.room
    ADD CONSTRAINT room_room_number_key UNIQUE (room_number);
--
-- PostgreSQL database dump complete
--

