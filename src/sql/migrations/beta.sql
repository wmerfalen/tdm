--
-- PostgreSQL database dump
--

-- Dumped from database version 10.11 (Debian 10.11-1.pgdg90+1)
-- Dumped by pg_dump version 10.11 (Debian 10.11-1.pgdg90+1)



-- \c postgres
-- DROP DATABASE mud;
\c postgres
DROP DATABASE mud;
CREATE DATABASE mud WITH OWNER = postgres;
\c mud

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

--
-- Name: plpgsql; Type: EXTENSION; Schema: -; Owner: 
--

CREATE EXTENSION IF NOT EXISTS plpgsql WITH SCHEMA pg_catalog;


--
-- Name: EXTENSION plpgsql; Type: COMMENT; Schema: -; Owner: 
--

COMMENT ON EXTENSION plpgsql IS 'PL/pgSQL procedural language';


--
-- Name: pgcrypto; Type: EXTENSION; Schema: -; Owner: 
--

CREATE EXTENSION IF NOT EXISTS pgcrypto WITH SCHEMA public;


--
-- Name: EXTENSION pgcrypto; Type: COMMENT; Schema: -; Owner: 
--

COMMENT ON EXTENSION pgcrypto IS 'cryptographic functions';

--
-- Name: player; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.player (
    id SERIAL UNIQUE,
    player_password character varying(1024) NOT NULL,
    player_affection_plr_bitvector character varying(2048),
    player_affection_bitvector character varying(2048),
    player_name character varying(24) DEFAULT 'recruit'::character varying NOT NULL,
    player_short_description character varying(512) DEFAULT 'A lonely recruit'::character varying NOT NULL,
    player_long_description text,
    player_action_bitvector integer DEFAULT 0 NOT NULL,
    player_ability_strength integer DEFAULT 1 NOT NULL,
    player_ability_strength_add integer DEFAULT 1 NOT NULL,
    player_ability_intelligence integer DEFAULT 1 NOT NULL,
    player_ability_wisdom integer DEFAULT 1 NOT NULL,
    player_ability_dexterity integer DEFAULT 1 NOT NULL,
    player_ability_constitution integer DEFAULT 1 NOT NULL,
    player_ability_charisma integer DEFAULT 1 NOT NULL,
    player_ability_alignment integer DEFAULT 0 NOT NULL,
    player_attack_type integer DEFAULT 0 NOT NULL,
    player_max_hitpoints integer DEFAULT 100 NOT NULL,
    player_max_mana integer DEFAULT 100 NOT NULL,
    player_max_move integer DEFAULT 100 NOT NULL,
    player_gold integer DEFAULT 0 NOT NULL,
    player_exp integer DEFAULT 0 NOT NULL,
    player_sex character varying(1) DEFAULT 'M'::character varying NOT NULL,
    player_hitpoints integer DEFAULT 100 NOT NULL,
    player_mana integer DEFAULT 100 NOT NULL,
    player_move integer DEFAULT 100 NOT NULL,
    player_damroll integer DEFAULT 6 NOT NULL,
    player_weight integer DEFAULT 10 NOT NULL,
    player_height integer DEFAULT 60 NOT NULL,
    player_class character varying(16) DEFAULT 'x'::character varying NOT NULL,
    player_title character varying(24) DEFAULT 'recruit'::character varying,
    player_hometown character varying(24) DEFAULT 'SLC'::character varying,
    player_damnodice integer DEFAULT 3 NOT NULL,
    player_damsizedice integer DEFAULT 6 NOT NULL,
    player_type character varying(6) DEFAULT 'x'::character varying NOT NULL,
    player_alignment integer DEFAULT 0 NOT NULL,
    player_level integer DEFAULT 1 NOT NULL,
    player_hitroll integer DEFAULT 3 NOT NULL,
    player_armor integer DEFAULT 3 NOT NULL,
    player_birth timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
    player_time_played integer DEFAULT 0 NOT NULL,
    player_logon timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
    player_preferences character varying(2048),
		PRIMARY KEY(id)
);
ALTER TABLE public.player OWNER TO postgres;

--
-- Name: zone; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.zone (
    id SERIAL UNIQUE,
    zone_virtual_number integer NOT NULL UNIQUE,
    zone_start integer NOT NULL,
    zone_end integer NOT NULL,
    zone_name character varying(64) NOT NULL,
    lifespan integer NOT NULL,
    reset_mode integer NOT NULL
);


ALTER TABLE public.zone OWNER TO postgres;


CREATE TABLE public.room (
    id SERIAL,
    room_number integer NOT NULL UNIQUE,
    zone integer NOT NULL,
    sector_type integer NOT NULL,
    name character varying(256) NOT NULL,
    description text NOT NULL,
    ex_keyword character varying(256),
    ex_description text,
    light integer,
    room_flag integer NOT NULL,
		PRIMARY KEY(id),
		CONSTRAINT fk_zone
			FOREIGN KEY (zone)
			REFERENCES public.zone(id)
			ON DELETE CASCADE
			ON UPDATE CASCADE
);


ALTER TABLE public.room OWNER TO postgres;

CREATE TABLE public.room_direction_data (
    id SERIAL,
    room_number integer NOT NULL,
    exit_direction integer NOT NULL,
    general_description character varying(256) NOT NULL,
    keyword character varying(16),
    exit_info integer,
    exit_key integer,
    to_room integer NOT NULL,
		PRIMARY KEY(id),
			CONSTRAINT fk_room_vnum
				FOREIGN KEY(room_number)
					REFERENCES public.room(room_number)
					ON DELETE CASCADE
					ON UPDATE CASCADE
);

--
-- Name: mobile; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.mobile (
    mob_id SERIAL UNIQUE,
    mob_virtual_number integer NOT NULL UNIQUE,
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
    mob_class integer NOT NULL,
    mob_special_extended_type integer DEFAULT 0 NOT NULL
);


ALTER TABLE public.mobile OWNER TO postgres;

--
-- Name: armor_type_t; Type: TYPE; Schema: public; Owner: postgres
--

CREATE TYPE public.armor_type_t AS ENUM (
    'VEST',
    'LEGS',
    'GAUNTLETS',
    'SHOULDER_PADS',
    'HELMET',
    'HAT',
    'EYEWEAR',
    'GOGGLES',
    'BACKPACK',
    'WAIST_PACK',
    'VEST_PACK',
    'ELBOW_GUARDS',
    'GLOVES',
    'SHIELD'
);


ALTER TYPE public.armor_type_t OWNER TO postgres;

--
-- Name: base_attribute; Type: TYPE; Schema: public; Owner: postgres
--

CREATE TYPE public.base_attribute AS ENUM (
    'STR',
    'INT',
    'DEX',
    'WIS',
    'CHA',
    'CON',
    'LCK'
);


ALTER TYPE public.base_attribute OWNER TO postgres;

--
-- Name: durability_profile_type_t; Type: TYPE; Schema: public; Owner: postgres
--

CREATE TYPE public.durability_profile_type_t AS ENUM (
    'FLIMSY',
    'DECENT',
    'DURABLE',
    'HARDENED',
    'INDUSTRIAL_STRENGTH',
    'GODLIKE',
    'INDESTRUCTIBLE'
);


ALTER TYPE public.durability_profile_type_t OWNER TO postgres;

--
-- Name: explosive_alternate_explosion_type_t; Type: TYPE; Schema: public; Owner: postgres
--

CREATE TYPE public.explosive_alternate_explosion_type_t AS ENUM (
    'NONE',
    'SCAN'
);


ALTER TYPE public.explosive_alternate_explosion_type_t OWNER TO postgres;

--
-- Name: explosive_loudness_type_t; Type: TYPE; Schema: public; Owner: postgres
--

CREATE TYPE public.explosive_loudness_type_t AS ENUM (
    'SILENT',
    'SUSPICIOUS',
    'LOUD',
    'DEAFENING',
    'WORLD_ENDING'
);


ALTER TYPE public.explosive_loudness_type_t OWNER TO postgres;

--
-- Name: explosive_type_t; Type: TYPE; Schema: public; Owner: postgres
--

CREATE TYPE public.explosive_type_t AS ENUM (
    'FRAG_GRENADE',
    'INCENDIARY_GRENADE',
    'REMOTE_EXPLOSIVE',
    'REMOTE_CHEMICAL',
    'EMP_GRENADE',
    'CLAYMORE_MINE',
    'SMOKE_GRENADE',
    'C4',
    'BREACH_CHARGE',
    'THERMITE_CHARGE',
    'FLASHBANG_GRENADE',
    'SENSOR_GRENADE'
);


ALTER TYPE public.explosive_type_t OWNER TO postgres;

--
-- Name: gadget_type_t; Type: TYPE; Schema: public; Owner: postgres
--

CREATE TYPE public.gadget_type_t AS ENUM (
    'GRAPPLING_HOOK',
    'BARBED_WIRE',
    'CAMERA',
    'THERMAL_CAMERA',
    'NIGHT_VISION_CAMERA',
    'MOTION_SENSOR',
    'DEPLOYABLE_SHIELD',
    'TRIPWIRE'
);


ALTER TYPE public.gadget_type_t OWNER TO postgres;

--
-- Name: player_object_type_t; Type: TYPE; Schema: public; Owner: postgres
--

CREATE TYPE public.player_object_type_t AS ENUM (
    'rifle',
    'explosive',
    'attachment',
    'gadget',
    'drone',
    'armor',
    'consumable',
    'object'
);


ALTER TYPE public.player_object_type_t OWNER TO postgres;

--
-- Name: rarity_t; Type: TYPE; Schema: public; Owner: postgres
--

CREATE TYPE public.rarity_t AS ENUM (
    'COMMON',
    'UNCOMMON',
    'RARE',
    'LEGENDARY',
    'GOD_TIER'
);


ALTER TYPE public.rarity_t OWNER TO postgres;

--
-- Name: rifle_type_t; Type: TYPE; Schema: public; Owner: postgres
--

CREATE TYPE public.rifle_type_t AS ENUM (
    'SHOTGUN',
    'ASSAULT_RIFLE',
    'SUB_MACHINE_GUN',
    'SNIPER',
    'HANDGUN',
    'MACHINE_PISTOL',
    'LIGHT_MACHINE_GUN',
    'PISTOL'
);


ALTER TYPE public.rifle_type_t OWNER TO postgres;

--
-- Name: speed_profile_type_t; Type: TYPE; Schema: public; Owner: postgres
--

CREATE TYPE public.speed_profile_type_t AS ENUM (
    'UNHINDERED',
    'FAST',
    'HINDERED',
    'SLOW',
    'SLUGGISH'
);


ALTER TYPE public.speed_profile_type_t OWNER TO postgres;

SET default_tablespace = '';

SET default_with_oids = false;

--
-- Name: affected_type; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.affected_type (
    id SERIAL,
    aff_fk_id integer NOT NULL,
    aff_location integer NOT NULL,
    aff_modifier integer NOT NULL
);


ALTER TABLE public.affected_type OWNER TO postgres;

--
-- Name: camera_feed; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.camera_feed (
    feed_id SERIAL,
    feed_type character varying(16) NOT NULL,
    feed_vnum integer NOT NULL,
    feed_room_vnum integer NOT NULL,
    feed_order integer DEFAULT 0 NOT NULL,
    created_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
    updated_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
		PRIMARY KEY(feed_id),
		CONSTRAINT fk_room_vnum
			FOREIGN KEY (feed_room_vnum)
				REFERENCES public.room(room_number)
					ON DELETE CASCADE
					ON UPDATE CASCADE
);


ALTER TABLE public.camera_feed OWNER TO postgres;

CREATE TABLE public.computer_terminal (
    id SERIAL,
    terminal_room_vnum integer NOT NULL,
    terminal_type character varying DEFAULT 'chooser'::character varying NOT NULL,
    terminal_attaches_to character varying,
    terminal_name character varying NOT NULL,
    terminal_description text,
    created_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
    updated_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
		PRIMARY KEY(id),
		CONSTRAINT fk_room_vnum
			FOREIGN KEY (terminal_room_vnum)
				REFERENCES public.room(room_number)
					ON DELETE CASCADE
					ON UPDATE CASCADE
);


ALTER TABLE public.computer_terminal OWNER TO postgres;

--
-- Name: computer_terminal_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--


ALTER TABLE public.computer_terminal_id_seq OWNER TO postgres;

--
-- Name: computer_terminal_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--



--
-- Name: extra_description; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.extra_description (
    id SERIAL,
    obj_fk_id integer NOT NULL,
    extra_keyword character varying(64),
    extra_description character varying(1024),
		PRIMARY KEY(id)
);


ALTER TABLE public.extra_description OWNER TO postgres;


--
-- Name: extra_description_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

-- ALTER SEQUENCE public.extra_description_id_seq OWNED BY public.extra_description.id;


--
-- Name: integral_object; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.integral_object (
    object_id SERIAL,
    object_room_vnum integer NOT NULL,
    object_type character varying(16) NOT NULL,
    object_vnum integer NOT NULL,
    created_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
    updated_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
		PRIMARY KEY(object_id),
		CONSTRAINT fk_room_vnum
			FOREIGN KEY (object_room_vnum)
				REFERENCES public.room(room_number)
					ON DELETE CASCADE
					ON UPDATE CASCADE
);


ALTER TABLE public.integral_object OWNER TO postgres;


--
-- Name: karma; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.karma (
    karma_id SERIAL,
    karma_player_id integer NOT NULL,
    karma_alignment integer DEFAULT 0 NOT NULL,
    karma_cold_blooded_index integer DEFAULT 0 NOT NULL,
    created_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
    updated_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
		PRIMARY KEY(karma_id),
		CONSTRAINT fk_player_id
			FOREIGN KEY (karma_player_id)
				REFERENCES public.player(id)
				ON DELETE CASCADE
				ON UPDATE CASCADE
);


ALTER TABLE public.karma OWNER TO postgres;

--
-- Name: karma_karma_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--


--
-- Name: mini_game; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.mini_game (
    game_id SERIAL,
    game_type character varying(16) NOT NULL,
    game_vnum integer NOT NULL,
    game_name character varying(32),
    game_action text DEFAULT 'unlock'::text NOT NULL,
    game_room_vnum integer NOT NULL,
    game_order integer DEFAULT 0 NOT NULL,
    created_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
    updated_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
		PRIMARY KEY(game_id),
		CONSTRAINT fk_room_vnum
			FOREIGN KEY (game_room_vnum)
				REFERENCES public.room(room_number)
					ON DELETE CASCADE
					ON UPDATE CASCADE
);


ALTER TABLE public.mini_game OWNER TO postgres;

--
-- Name: mini_gunner_sentinel; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.mini_gunner_sentinel (
    id SERIAL,
    mgs_mob_vnum integer DEFAULT '-1'::integer NOT NULL,
    mgs_face_direction character varying(1) DEFAULT 'N'::character varying NOT NULL,
    mgs_room_vnum integer NOT NULL,
		PRIMARY KEY(id),
		CONSTRAINT fk_room_vnum
			FOREIGN KEY (mgs_room_vnum)
				REFERENCES public.room(room_number)
					ON DELETE CASCADE
					ON UPDATE CASCADE,
    CONSTRAINT fk_mob_vnum
			FOREIGN KEY (mgs_mob_vnum)
				REFERENCES public.mobile(mob_id)
					ON DELETE CASCADE
					ON UPDATE CASCADE
);


ALTER TABLE public.mini_gunner_sentinel OWNER TO postgres;

--
-- Name: mini_gunner_sentinel_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

-- ALTER SEQUENCE public.mini_gunner_sentinel_id_seq OWNED BY public.mini_gunner_sentinel.id;


--
-- Name: mob_zone; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.mob_zone (
    id SERIAL,
    zone_virtual_number integer NOT NULL,
    mob_virtual_number integer NOT NULL,
    room_virtual_number integer NOT NULL,
    max_existing integer,
		PRIMARY KEY (id),
		CONSTRAINT fk_zone_virtual_number
			FOREIGN KEY (zone_virtual_number)
				REFERENCES public.zone(zone_virtual_number)
				ON DELETE CASCADE
				ON UPDATE CASCADE,
		CONSTRAINT fk_mob_virtual_number
			FOREIGN KEY (mob_virtual_number)
				REFERENCES public.mobile(mob_virtual_number)
				ON DELETE CASCADE
				ON UPDATE CASCADE,
		CONSTRAINT fk_room_virtual_number
			FOREIGN KEY (room_virtual_number)
				REFERENCES public.room(room_number)
				ON DELETE CASCADE
				ON UPDATE CASCADE
);


CREATE TABLE public.mob_roam (
    id SERIAL,
    mob_virtual_number integer NOT NULL,
    room_virtual_number integer NOT NULL,
		PRIMARY KEY (id),
		CONSTRAINT fk_mob_virtual_number
			FOREIGN KEY (mob_virtual_number)
				REFERENCES public.mobile(mob_virtual_number)
				ON DELETE CASCADE
				ON UPDATE CASCADE,
		CONSTRAINT fk_room_virtual_number
			FOREIGN KEY (room_virtual_number)
				REFERENCES public.room(room_number)
				ON DELETE CASCADE
				ON UPDATE CASCADE
);

ALTER TABLE public.mob_zone OWNER TO postgres;

--
-- Name: npc_dialogue; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.npc_dialogue (
    id integer NOT NULL,
    dialogue_mob_vnum integer NOT NULL,
    dialogue_text text NOT NULL,
    dialogue_part_of character varying,
    dialogue_comment text,
    dialogue_vnum integer NOT NULL,
    dialogue_order integer,
    created_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
    updated_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
		PRIMARY KEY(id),
		CONSTRAINT fk_room_vnum
			FOREIGN KEY (dialogue_mob_vnum)
				REFERENCES public.mobile(mob_virtual_number)
					ON DELETE CASCADE
					ON UPDATE CASCADE
);


ALTER TABLE public.npc_dialogue OWNER TO postgres;

--
-- Name: npc_dialogue_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.npc_dialogue_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.npc_dialogue_id_seq OWNER TO postgres;

--
-- Name: npc_dialogue_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.npc_dialogue_id_seq OWNED BY public.npc_dialogue.id;


--
-- Name: object; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.object (
    id integer NOT NULL,
    obj_item_number integer NOT NULL,
    obj_flags integer,
    obj_name character varying(128),
    obj_description character varying(1024),
    obj_short_description character varying(64),
    obj_action_description character varying(32),
    obj_type integer NOT NULL,
    obj_worn_on integer,
    obj_type_data character varying(16),
    obj_extra_keyword character varying(64),
    obj_extra_description character varying(1024),
    obj_file character varying(255) NOT NULL,
    obj_belongs_to_player integer DEFAULT 0
);


ALTER TABLE public.object OWNER TO postgres;

--
-- Name: object_armor; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.object_armor (
    armor_id integer NOT NULL,
    armor_csv_capabilities character varying(2048),
    armor_csv_attach_to character varying(2048),
    armor_thac0 integer NOT NULL,
    armor_weight_in_lbs double precision NOT NULL,
    armor_fire_resistance_percent double precision DEFAULT 0.0 NOT NULL,
    armor_balistic_resistance_percent double precision DEFAULT 0.0 NOT NULL,
    armor_speed_profile public.speed_profile_type_t DEFAULT 'HINDERED'::public.speed_profile_type_t NOT NULL,
    armor_offensive_damage_amount integer DEFAULT 0 NOT NULL,
    armor_durability_profile public.durability_profile_type_t DEFAULT 'DURABLE'::public.durability_profile_type_t NOT NULL,
    armor_file character varying(2048),
    armor_str_type public.armor_type_t NOT NULL,
    armor_manufacturer character varying(256) NOT NULL,
    armor_name character varying(256) NOT NULL,
    armor_vnum integer NOT NULL,
    armor_rarity public.rarity_t DEFAULT 'COMMON'::public.rarity_t NOT NULL,
    armor_type integer NOT NULL,
    armor_hp integer DEFAULT 0 NOT NULL,
    armor_classification character varying(8) DEFAULT 'BASIC'::character varying NOT NULL,
    armor_worth integer DEFAULT 0 NOT NULL,
		armor_incendiary_resistance_percent integer NOT NULL DEFAULT 0,
		armor_explosive_resistance_percent integer NOT NULL DEFAULT 0,
		armor_shrapnel_resistance_percent integer NOT NULL DEFAULT 0,
		armor_corrosive_resistance_percent integer NOT NULL DEFAULT 0,
		armor_cryogenic_resistance_percent integer NOT NULL DEFAULT 0,
		armor_radiation_resistance_percent integer NOT NULL DEFAULT 0,
		armor_emp_resistance_percent integer NOT NULL DEFAULT 0,
		armor_shock_resistance_percent integer NOT NULL DEFAULT 0,
		armor_anti_matter_resistance_percent integer NOT NULL DEFAULT 0,
   	created_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
   	updated_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL
);


ALTER TABLE public.object_armor OWNER TO postgres;

--
-- Name: object_armor_armor_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.object_armor_armor_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.object_armor_armor_id_seq OWNER TO postgres;

--
-- Name: object_armor_armor_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.object_armor_armor_id_seq OWNED BY public.object_armor.armor_id;


--
-- Name: object_explosive; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.object_explosive (
    explosive_id integer NOT NULL,
    explosive_alternate_explosion_type public.explosive_alternate_explosion_type_t DEFAULT 'NONE'::public.explosive_alternate_explosion_type_t NOT NULL,
    explosive_chance_to_injure double precision DEFAULT 0.0 NOT NULL,
    explosive_critical_chance double precision DEFAULT 0.0 NOT NULL,
    explosive_critical_range integer DEFAULT 1 NOT NULL,
    explosive_blast_radius integer DEFAULT 1 NOT NULL,
    explosive_damage_per_second double precision DEFAULT 1.0 NOT NULL,
    explosive_disorient_amount double precision DEFAULT 0.0 NOT NULL,
    explosive_loudness_type public.explosive_loudness_type_t DEFAULT 'LOUD'::public.explosive_loudness_type_t NOT NULL,
    explosive_str_type public.explosive_type_t NOT NULL,
    explosive_type integer NOT NULL,
    explosive_manufacturer character varying(256) NOT NULL,
    explosive_name character varying(256) NOT NULL,
    explosive_vnum integer NOT NULL,
    explosive_rarity public.rarity_t DEFAULT 'COMMON'::public.rarity_t NOT NULL,
    explosive_file character varying(2048),
		explosive_incendiary_damage_dice_count integer NOT NULL DEFAULT 0,
		explosive_incendiary_damage_dice_sides integer NOT NULL DEFAULT 0,
		explosive_radiation_damage_dice_count integer NOT NULL DEFAULT 0,
		explosive_radiation_damage_dice_sides integer NOT NULL DEFAULT 0,
		explosive_chemical_damage_dice_count integer NOT NULL DEFAULT 0,
		explosive_chemical_damage_dice_sides integer NOT NULL DEFAULT 0,
		explosive_electric_damage_dice_count integer NOT NULL DEFAULT 0,
		explosive_electric_damage_dice_sides integer NOT NULL DEFAULT 0,
		explosive_armor_penetration_damage_dice_count integer NOT NULL DEFAULT 0,
		explosive_armor_penetration_damage_dice_sides integer NOT NULL DEFAULT 0,
		explosive_damage_dice_count integer NOT NULL DEFAULT 0,
		explosive_damage_dice_sides integer NOT NULL DEFAULT 0,
		explosive_incendiary_damage_percent integer NOT NULL DEFAULT 0,
		explosive_explosive_damage_percent integer NOT NULL DEFAULT 0,
		explosive_shrapnel_damage_percent integer NOT NULL DEFAULT 0,
		explosive_corrosive_damage_percent integer NOT NULL DEFAULT 0,
		explosive_cryogenic_damage_percent integer NOT NULL DEFAULT 0,
		explosive_radiation_damage_percent integer NOT NULL DEFAULT 0,
		explosive_emp_damage_percent integer NOT NULL DEFAULT 0,
		explosive_shock_damage_percent integer NOT NULL DEFAULT 0,
		explosive_anti_matter_damage_percent integer NOT NULL DEFAULT 0,
    created_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
    updated_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL
);


ALTER TABLE public.object_explosive OWNER TO postgres;

--
-- Name: object_explosive_explosive_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.object_explosive_explosive_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.object_explosive_explosive_id_seq OWNER TO postgres;

--
-- Name: object_explosive_explosive_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.object_explosive_explosive_id_seq OWNED BY public.object_explosive.explosive_id;


--
-- Name: object_flags; Type: TABLE; Schema: public; Owner: postgres
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
    bitvector integer NOT NULL,
    weapon_flags integer DEFAULT 0 NOT NULL,
    ammo_max integer DEFAULT 250 NOT NULL,
    clip_size integer DEFAULT 21 NOT NULL
);


ALTER TABLE public.object_flags OWNER TO postgres;

--
-- Name: object_flags_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.object_flags_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.object_flags_id_seq OWNER TO postgres;

--
-- Name: object_flags_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.object_flags_id_seq OWNED BY public.object_flags.id;


--
-- Name: object_gadget; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.object_gadget (
    gadget_id integer NOT NULL,
    gadget_csv_capabilities character varying(2048),
    gadget_csv_attach_to character varying(2048),
    gadget_electronic integer DEFAULT 0 NOT NULL,
    gadget_health_points integer DEFAULT 0 NOT NULL,
    gadget_shield_points integer DEFAULT 0 NOT NULL,
    gadget_durability_profile public.durability_profile_type_t DEFAULT 'DURABLE'::public.durability_profile_type_t NOT NULL,
    gadget_move_points integer DEFAULT 0 NOT NULL,
    gadget_damage_points integer DEFAULT 0 NOT NULL,
    gadget_str_type public.gadget_type_t NOT NULL,
    gadget_type integer NOT NULL,
    gadget_manufacturer character varying(256) NOT NULL,
    gadget_name character varying(256) NOT NULL,
    gadget_vnum integer NOT NULL,
    gadget_rarity public.rarity_t DEFAULT 'COMMON'::public.rarity_t NOT NULL,
    gadget_file character varying(2048),
    created_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
    updated_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
    gadget_exit_key integer DEFAULT 0 NOT NULL,
    gadget_min_level integer DEFAULT 0 NOT NULL,
    gadget_worth integer DEFAULT 0 NOT NULL
);


ALTER TABLE public.object_gadget OWNER TO postgres;

--
-- Name: object_gadget_gadget_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.object_gadget_gadget_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.object_gadget_gadget_id_seq OWNER TO postgres;

--
-- Name: object_gadget_gadget_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.object_gadget_gadget_id_seq OWNED BY public.object_gadget.gadget_id;


--
-- Name: object_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.object_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.object_id_seq OWNER TO postgres;

--
-- Name: object_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.object_id_seq OWNED BY public.object.id;


--
-- Name: object_rifle; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.object_rifle (
    rifle_id integer NOT NULL,
    rifle_accuracy_map_0 double precision DEFAULT 10.0,
    rifle_accuracy_map_1 double precision DEFAULT 10.0,
    rifle_accuracy_map_2 double precision DEFAULT 10.0,
    rifle_accuracy_map_3 double precision DEFAULT 10.0,
    rifle_damage_map_0 double precision DEFAULT 10.0,
    rifle_damage_map_1 double precision DEFAULT 10.0,
    rifle_damage_map_2 double precision DEFAULT 10.0,
    rifle_damage_map_3 double precision DEFAULT 10.0,
    rifle_ammo_max integer,
    rifle_ammo_type character varying(32) NOT NULL,
    rifle_chance_to_injure double precision,
    rifle_clip_size integer,
    rifle_cooldown_between_shots double precision,
    rifle_critical_chance double precision,
    rifle_critical_range integer,
    rifle_damage_per_second double precision,
    rifle_disorient_amount double precision,
    rifle_headshot_bonus double precision,
    rifle_max_range integer,
    rifle_range_multiplier double precision,
    rifle_reload_time double precision,
    rifle_rounds_per_minute integer,
    rifle_muzzle_velocity integer,
    rifle_effective_firing_range integer,
    rifle_str_type public.rifle_type_t NOT NULL,
    rifle_type integer NOT NULL,
    rifle_manufacturer character varying(256) NOT NULL,
    rifle_name character varying(256) NOT NULL,
    rifle_vnum integer NOT NULL,
    rifle_rarity public.rarity_t DEFAULT 'COMMON'::public.rarity_t NOT NULL,
    rifle_file character varying(2048),
		rifle_incendiary_damage integer NOT NULL DEFAULT 0,
		rifle_explosive_damage integer NOT NULL DEFAULT 0,
		rifle_shrapnel_damage integer NOT NULL DEFAULT 0,
		rifle_corrosive_damage integer NOT NULL DEFAULT 0,
		rifle_cryogenic_damage integer NOT NULL DEFAULT 0,
		rifle_radioactive_damage integer NOT NULL DEFAULT 0,
		rifle_emp_damage integer NOT NULL DEFAULT 0,
		rifle_shock_damage integer NOT NULL DEFAULT 0,
		rifle_anti_matter_damage integer NOT NULL DEFAULT 0,
		rifle_stat_strength integer NOT NULL DEFAULT 0,
		rifle_stat_intelligence integer NOT NULL DEFAULT 0,
		rifle_stat_wisdom integer NOT NULL DEFAULT 0,
		rifle_stat_dexterity integer NOT NULL DEFAULT 0,
		rifle_stat_constitution integer NOT NULL DEFAULT 0,
		rifle_stat_electronics integer NOT NULL DEFAULT 0,
		rifle_stat_armor integer NOT NULL DEFAULT 0,
		rifle_stat_marksmanship integer NOT NULL DEFAULT 0,
		rifle_stat_sniping integer NOT NULL DEFAULT 0,
		rifle_stat_demolitions integer NOT NULL DEFAULT 0,
		rifle_stat_chemistry integer NOT NULL DEFAULT 0,
		rifle_stat_weapon_handling integer NOT NULL DEFAULT 0,
		rifle_stat_strategy integer NOT NULL DEFAULT 0,
		rifle_stat_medical integer NOT NULL DEFAULT 0,
    created_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
    updated_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL
);


ALTER TABLE public.object_rifle OWNER TO postgres;

--
-- Name: object_rifle_rifle_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.object_rifle_rifle_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.object_rifle_rifle_id_seq OWNER TO postgres;

--
-- Name: object_rifle_rifle_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.object_rifle_rifle_id_seq OWNED BY public.object_rifle.rifle_id;


--
-- Name: object_weapon; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.object_weapon (
    id integer NOT NULL,
    obj_fk_id integer NOT NULL,
    obj_ammo_max integer NOT NULL,
    obj_ammo_type integer NOT NULL,
    obj_cooldown integer NOT NULL,
    obj_can_snipe boolean NOT NULL
);


ALTER TABLE public.object_weapon OWNER TO postgres;

--
-- Name: object_weapon_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.object_weapon_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.object_weapon_id_seq OWNER TO postgres;

--
-- Name: object_weapon_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.object_weapon_id_seq OWNED BY public.object_weapon.id;



--
-- Name: player_classes; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.player_classes (
    id integer NOT NULL,
    pclass_name character varying(24) NOT NULL,
    pclass_description text
);


ALTER TABLE public.player_classes OWNER TO postgres;

--
-- Name: player_classes_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.player_classes_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.player_classes_id_seq OWNER TO postgres;

--
-- Name: player_classes_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.player_classes_id_seq OWNED BY public.player_classes.id;


--
-- Name: player_flags; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.player_flags (
    id integer NOT NULL,
    player_id integer NOT NULL,
    chunk_index integer DEFAULT 0 NOT NULL,
    flag_value integer NOT NULL
);


ALTER TABLE public.player_flags OWNER TO postgres;

--
-- Name: player_flags_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.player_flags_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.player_flags_id_seq OWNER TO postgres;

--
-- Name: player_flags_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.player_flags_id_seq OWNED BY public.player_flags.id;


--
-- Name: player_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

ALTER TABLE public.player_id_seq OWNER TO postgres;

--
-- Name: player_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.player_id_seq OWNED BY public.player.id;


--
-- Name: player_object; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.player_object (
    po_id integer NOT NULL,
    po_player_id integer NOT NULL,
    po_type public.player_object_type_t DEFAULT 'object'::public.player_object_type_t NOT NULL,
    po_type_vnum integer,
    po_type_id integer,
    po_type_load character varying(4) DEFAULT 'id'::character varying NOT NULL,
    po_wear_position integer NOT NULL,
    po_in_inventory integer DEFAULT 0 NOT NULL,
    created_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
    updated_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL
);


ALTER TABLE public.player_object OWNER TO postgres;

--
-- Name: player_object_po_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.player_object_po_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.player_object_po_id_seq OWNER TO postgres;

--
-- Name: player_object_po_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.player_object_po_id_seq OWNED BY public.player_object.po_id;


--
-- Name: player_player_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.player_player_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.player_player_id_seq OWNER TO postgres;

--
-- Name: player_race_perks; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.player_race_perks (
    id integer NOT NULL,
    prperk_id integer NOT NULL,
    prperk_name character varying(64) NOT NULL,
    prperk_description text,
    prperk_base_attribute_mutate public.base_attribute NOT NULL,
    prperk_base_attribute_amount_add integer DEFAULT 0 NOT NULL
);


ALTER TABLE public.player_race_perks OWNER TO postgres;

--
-- Name: player_race_perks_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.player_race_perks_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.player_race_perks_id_seq OWNER TO postgres;

--
-- Name: player_race_perks_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.player_race_perks_id_seq OWNED BY public.player_race_perks.id;


--
-- Name: player_races; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.player_races (
    id integer NOT NULL,
    prace_name character varying(24) NOT NULL,
    prace_description text
);


ALTER TABLE public.player_races OWNER TO postgres;

--
-- Name: player_races_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.player_races_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.player_races_id_seq OWNER TO postgres;

--
-- Name: player_races_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.player_races_id_seq OWNED BY public.player_races.id;


--
-- Name: room; Type: TABLE; Schema: public; Owner: postgres
--

--
-- Name: room_direction_data; Type: TABLE; Schema: public; Owner: postgres
--


ALTER TABLE public.room_direction_data OWNER TO postgres;

ALTER TABLE public.room_direction_data_id_seq OWNER TO postgres;

--
-- Name: room_direction_data_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.room_direction_data_id_seq OWNED BY public.room_direction_data.id;



--
-- Name: room_virtual_number_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.room_virtual_number_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.room_virtual_number_seq OWNER TO postgres;

--
-- Name: shop_objects; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.shop_objects (
    shop_objects_id integer NOT NULL,
    shop_vnum integer NOT NULL,
    shop_object_vnum integer NOT NULL
);


ALTER TABLE public.shop_objects OWNER TO postgres;

--
-- Name: shop_objects_shop_objects_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.shop_objects_shop_objects_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.shop_objects_shop_objects_id_seq OWNER TO postgres;

--
-- Name: shop_objects_shop_objects_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.shop_objects_shop_objects_id_seq OWNED BY public.shop_objects.shop_objects_id;


--
-- Name: shop_rooms; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.shop_rooms (
    shop_rooms_id integer NOT NULL,
    shop_vnum integer NOT NULL,
    shop_room_vnum integer NOT NULL,
		PRIMARY KEY(shop_rooms_id),
		CONSTRAINT fk_room_vnum
			FOREIGN KEY(shop_room_vnum)
				REFERENCES public.room(room_number)
					ON DELETE CASCADE
					ON UPDATE CASCADE
);


ALTER TABLE public.shop_rooms OWNER TO postgres;

--
-- Name: shop_rooms_shop_rooms_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.shop_rooms_shop_rooms_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.shop_rooms_shop_rooms_id_seq OWNER TO postgres;

--
-- Name: shop_rooms_shop_rooms_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.shop_rooms_shop_rooms_id_seq OWNED BY public.shop_rooms.shop_rooms_id;


--
-- Name: shops; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.shops (
    shop_id integer NOT NULL,
    shop_vnum integer NOT NULL,
    shop_title character varying(256) NOT NULL,
    shop_description character varying(2048) NOT NULL,
    shop_profit_buy double precision NOT NULL,
    shop_profit_sell double precision NOT NULL,
    shop_type integer NOT NULL,
    shop_no_such_item1 character varying(256) NOT NULL,
    shop_no_such_item2 character varying(256) NOT NULL,
    shop_missing_cash1 character varying(256) NOT NULL,
    shop_missing_cash2 character varying(256) NOT NULL,
    shop_do_not_buy character varying(256) NOT NULL,
    shop_message_buy character varying(256) NOT NULL,
    shop_message_sell character varying(256) NOT NULL,
    shop_temper1 integer NOT NULL,
    shop_bitvector integer NOT NULL,
    shop_keeper integer NOT NULL,
    shop_with_who integer NOT NULL,
    shop_open1 integer NOT NULL,
    shop_open2 integer NOT NULL,
    shop_close1 integer NOT NULL,
    shop_close2 integer NOT NULL,
    shop_bankaccount integer NOT NULL,
    shop_lastsort integer NOT NULL,
    shop_flags integer
);


ALTER TABLE public.shops OWNER TO postgres;

--
-- Name: shops_shop_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.shops_shop_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.shops_shop_id_seq OWNER TO postgres;

--
-- Name: shops_shop_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.shops_shop_id_seq OWNED BY public.shops.shop_id;



--
-- Name: tasks; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.tasks (
    id integer,
    t_name character varying(256)
);


ALTER TABLE public.tasks OWNER TO postgres;

--
-- Name: tasks_notes; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.tasks_notes (
    id integer,
    task_id integer,
    t_notes character varying(256)
);


ALTER TABLE public.tasks_notes OWNER TO postgres;

--
-- Name: terminal_choices; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.terminal_choices (
    id integer NOT NULL,
    choice_terminal_id integer NOT NULL,
    choice_id integer NOT NULL,
    choice_title text NOT NULL,
    choice_description text NOT NULL,
    choice_action text NOT NULL
);


ALTER TABLE public.terminal_choices OWNER TO postgres;

--
-- Name: terminal_choices_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.terminal_choices_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.terminal_choices_id_seq OWNER TO postgres;

--
-- Name: terminal_choices_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.terminal_choices_id_seq OWNED BY public.terminal_choices.id;


--
-- Name: world_configuration_start_rooms; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.world_configuration_start_rooms (
    id integer NOT NULL,
    mortal_start_room integer NOT NULL,
    immortal_start_room integer NOT NULL,
    created_at timestamp without time zone DEFAULT now() NOT NULL,
    is_active boolean,
    idle_room integer DEFAULT 0 NOT NULL,
    frozen_room integer DEFAULT 0 NOT NULL
);


ALTER TABLE public.world_configuration_start_rooms OWNER TO postgres;

--
-- Name: world_configuration_start_rooms_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.world_configuration_start_rooms_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.world_configuration_start_rooms_id_seq OWNER TO postgres;

--
-- Name: world_configuration_start_rooms_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.world_configuration_start_rooms_id_seq OWNED BY public.world_configuration_start_rooms.id;


--
-- Name: zone_data; Type: TABLE; Schema: public; Owner: postgres
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


ALTER TABLE public.zone_data OWNER TO postgres;

--
-- Name: zone_data_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.zone_data_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.zone_data_id_seq OWNER TO postgres;

--
-- Name: zone_data_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.zone_data_id_seq OWNED BY public.zone_data.id;


--
-- Name: affected_type id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.affected_type ALTER COLUMN id SET DEFAULT nextval('public.affected_type_id_seq'::regclass);


--
-- Name: camera_feed feed_id; Type: DEFAULT; Schema: public; Owner: postgres
--

-- ALTER TABLE ONLY public.camera_feed ALTER COLUMN feed_id SET DEFAULT nextval('public.camera_feed_feed_id_seq'::regclass);


--
-- Name: extra_description id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.extra_description ALTER COLUMN id SET DEFAULT nextval('public.extra_description_id_seq'::regclass);


--
-- Name: integral_object object_id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.integral_object ALTER COLUMN object_id SET DEFAULT nextval('public.integral_object_object_id_seq'::regclass);


--
-- Name: karma karma_id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.karma ALTER COLUMN karma_id SET DEFAULT nextval('public.karma_karma_id_seq'::regclass);


--
-- Name: mini_game game_id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.mini_game ALTER COLUMN game_id SET DEFAULT nextval('public.mini_game_game_id_seq'::regclass);


--
-- Name: mini_gunner_sentinel id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.mini_gunner_sentinel ALTER COLUMN id SET DEFAULT nextval('public.mini_gunner_sentinel_id_seq'::regclass);


--
-- Name: mob_zone id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.mob_zone ALTER COLUMN id SET DEFAULT nextval('public.mob_zone_id_seq'::regclass);


--
-- Name: mobile mob_id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.mobile ALTER COLUMN mob_id SET DEFAULT nextval('public.mobile_mob_id_seq'::regclass);


--
-- Name: npc_dialogue id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.npc_dialogue ALTER COLUMN id SET DEFAULT nextval('public.npc_dialogue_id_seq'::regclass);


--
-- Name: object id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.object ALTER COLUMN id SET DEFAULT nextval('public.object_id_seq'::regclass);


--
-- Name: object_armor armor_id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.object_armor ALTER COLUMN armor_id SET DEFAULT nextval('public.object_armor_armor_id_seq'::regclass);


--
-- Name: object_explosive explosive_id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.object_explosive ALTER COLUMN explosive_id SET DEFAULT nextval('public.object_explosive_explosive_id_seq'::regclass);


--
-- Name: object_flags id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.object_flags ALTER COLUMN id SET DEFAULT nextval('public.object_flags_id_seq'::regclass);


--
-- Name: object_gadget gadget_id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.object_gadget ALTER COLUMN gadget_id SET DEFAULT nextval('public.object_gadget_gadget_id_seq'::regclass);


--
-- Name: object_rifle rifle_id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.object_rifle ALTER COLUMN rifle_id SET DEFAULT nextval('public.object_rifle_rifle_id_seq'::regclass);


--
-- Name: object_weapon id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.object_weapon ALTER COLUMN id SET DEFAULT nextval('public.object_weapon_id_seq'::regclass);


--
-- Name: player id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.player ALTER COLUMN id SET DEFAULT nextval('public.player_id_seq'::regclass);


--
-- Name: player_classes id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.player_classes ALTER COLUMN id SET DEFAULT nextval('public.player_classes_id_seq'::regclass);


--
-- Name: player_flags id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.player_flags ALTER COLUMN id SET DEFAULT nextval('public.player_flags_id_seq'::regclass);


--
-- Name: player_object po_id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.player_object ALTER COLUMN po_id SET DEFAULT nextval('public.player_object_po_id_seq'::regclass);


--
-- Name: player_race_perks id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.player_race_perks ALTER COLUMN id SET DEFAULT nextval('public.player_race_perks_id_seq'::regclass);


--
-- Name: player_races id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.player_races ALTER COLUMN id SET DEFAULT nextval('public.player_races_id_seq'::regclass);


--
-- Name: room id; Type: DEFAULT; Schema: public; Owner: postgres
--

-- ALTER TABLE ONLY public.room ALTER COLUMN id SET DEFAULT nextval('public.room_room_id_seq'::regclass);


--
-- Name: room_direction_data id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.room_direction_data ALTER COLUMN id SET DEFAULT nextval('public.room_direction_data_id_seq'::regclass);


--
-- Name: shop_objects shop_objects_id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.shop_objects ALTER COLUMN shop_objects_id SET DEFAULT nextval('public.shop_objects_shop_objects_id_seq'::regclass);


--
-- Name: shop_rooms shop_rooms_id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.shop_rooms ALTER COLUMN shop_rooms_id SET DEFAULT nextval('public.shop_rooms_shop_rooms_id_seq'::regclass);


--
-- Name: shops shop_id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.shops ALTER COLUMN shop_id SET DEFAULT nextval('public.shops_shop_id_seq'::regclass);


--
-- Name: terminal_choices id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.terminal_choices ALTER COLUMN id SET DEFAULT nextval('public.terminal_choices_id_seq'::regclass);


--
-- Name: world_configuration_start_rooms id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.world_configuration_start_rooms ALTER COLUMN id SET DEFAULT nextval('public.world_configuration_start_rooms_id_seq'::regclass);


--
-- Name: zone id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.zone ALTER COLUMN id SET DEFAULT nextval('public.zone_id_seq'::regclass);


--
-- Name: zone_data id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.zone_data ALTER COLUMN id SET DEFAULT nextval('public.zone_data_id_seq'::regclass);


--
-- Data for Name: affected_type; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.affected_type (id, aff_fk_id, aff_location, aff_modifier) FROM stdin;
\.


--
-- Data for Name: camera_feed; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.camera_feed (feed_id, feed_type, feed_vnum, feed_room_vnum, feed_order, created_at, updated_at) FROM stdin;
\.



--
-- Data for Name: mobile; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.mobile (mob_id, mob_virtual_number, mob_name, mob_short_description, mob_long_description, mob_description, mob_action_bitvector, mob_affection_bitvector, mob_ability_strength, mob_ability_strength_add, mob_ability_intelligence, mob_ability_wisdom, mob_ability_dexterity, mob_ability_constitution, mob_ability_charisma, mob_alignment, mob_attack_type, mob_level, mob_hitroll, mob_armor, mob_max_hitpoints, mob_max_mana, mob_max_move, mob_gold, mob_exp, mob_load_position, mob_default_position, mob_sex, mob_hitpoints, mob_mana, mob_move, mob_damnodice, mob_damsizedice, mob_damroll, mob_weight, mob_height, mob_class, mob_special_extended_type) FROM stdin;
1	1	chef  employee	A pissed looking Los  employee	A pissed looking Los  employee	This particular employee looks like he just got out of a federal penitentiary. He's most likely hiding some weapon in one of the many compartments that should be used for storing utensils and food paraphernalia.	8	0	25	0	25	0	25	0	25	0	0	0	0	0	250	250	250	5000	0	8	8	0	250	250	250	25	0	0	50	15	0	1
2	100	dallas gibson gib g grim gibbs	{gld}Dallas {red}'Grim'{/red} {gld}Gibson{/gld}	{gld}Dallas {red}'Grim'{/red} {gld}Gibson the shopkeeper{/gld}	Short of stature and sharp as an eagle, this man may look like an under-powered elderly old man, but underneath that meek exterior lies a fierce beast.	8	0	2	0	2	0	2	0	2	0	0	0	20	0	70	15	30	25000	0	8	8	0	70	15	30	2	0	0	10	4	0	1
3	101	dallas gibson gib g grim gibbs	{gld}Dallas {red}'Grim'{/red} {gld}Gibson{/gld}	{gld}Dallas {red}'Grim'{/red} {gld}Gibson the shopkeeper{/gld}	Short of stature and sharp as an eagle, this man may look like an under-powered elderly old man, but underneath that meek exterior lies a fierce beast.	8	0	2	0	2	0	2	0	2	0	0	0	0	0	70	15	30	25000	0	0	0	0	70	15	30	0	0	0	10	4	0	1
11	410	Corporal James Taggart	Corporal James Tagger short description	Corporal James Tagger long description	Corporal James Tagger description	0	0	10	10	10	10	10	10	10	0	0	150	150	150	-1	-1	-1	0	9800	0	0	0	-1	-1	-1	50	50	50	80	9	0	0
4	103	chef  employee	A pissed looking Los  employee	A pissed looking Los  employee	This particular employee looks like he just got out of a federal penitentiary. He's most likely hiding some weapon in one of the many compartments that should be used for storing utensils and food paraphernalia.	8	0	25	0	25	0	25	0	25	0	0	0	20	0	250	250	250	5000	0	0	0	0	250	250	250	0	0	0	50	15	0	2
\.


--
-- Data for Name: npc_dialogue; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.npc_dialogue (id, dialogue_mob_vnum, dialogue_text, dialogue_part_of, dialogue_comment, dialogue_vnum, dialogue_order, created_at, updated_at) FROM stdin;
1	410	Welcome to the D.M.Z 0x4155 training courtyard, soldier.	orientation-1	what corp should say at orientation	1	0	2020-10-04 04:06:45.269353	2020-10-04 04:06:45.269353
2	410	Here you will master the basics skills to make you an effective asset out in the field of combat.	orientation-1	what corp should say at orientation	1	1	2020-10-04 04:06:45.277572	2020-10-04 04:06:45.277572
3	410	To see what training courses are available {yel}look{/yel} at the {blu}Course Selection Interface{/blu}.	orientation-1	what corp should say at orientation	1	2	2020-10-04 04:06:45.285879	2020-10-04 04:06:45.285879
4	410	To start a course, type {yel}start ID{/yel} where ID is the id of the course on the far left of the screen.	orientation-1	what corp should say at orientation	1	3	2020-10-04 04:06:45.29423	2020-10-04 04:06:45.29423
5	410	If you are already accustomed to this system, disregard my instructions and proceed when you ready.	orientation-1	what corp should say at orientation	1	4	2020-10-04 04:06:45.302534	2020-10-04 04:06:45.302534
6	410	Demolitions HOW TO: Section: GRENADES|=====================================|Step 1) Hold your grenade|run the command: {yel}hold grenade{/yel}||Step 2) Throw grenade.|run the command: {yel}throw grenade DISTANCE{/yel}|{grn}replace the phrase DISTANCE with the number of rooms you would like to throw.{/grn}|||Demolitions HOW TO: Section: C4|===========================================|Step 1) Attach the C4 to an object, door, person, or vehicle.|run the command: {yel}attach c4 ITEM{/yel}|{grn}replace the phrase ITEM with the target object, person, vehicle, or direction.{/grn}||Step 2) Detonate the C4.|run the command: {yel}detonate{/yel}|||Demolitions HOW TO: Section: Claymore Mines|===========================================|Step 1) Install the claymore mine at the specified door.|run the command: {yel}install claymore DIRECTION{/yel}|{grn}replace the phrase DISTANCE with the direction of the door you would like to install the claymore mine on.{/grn}||	demolitions-howto	what corp should say at orientation regarding demolitions	2	0	2020-10-04 04:06:45.311034	2020-10-04 04:06:45.311034
7	410	Demolitions HOW TO: Section: GRENADES|=====================================|Step 1) Hold your grenade|run the command: {yel}hold grenade{/yel}||Step 2) Throw grenade.|run the command: {yel}throw grenade DISTANCE{/yel}|{grn}replace the phrase DISTANCE with the number of rooms you would like to throw.{/grn}|||Demolitions HOW TO: Section: C4|===========================================|Step 1) Attach the C4 to an object, door, person, or vehicle.|run the command: {yel}attach c4 ITEM{/yel}|{grn}replace the phrase ITEM with the target object, person, vehicle, or direction.{/grn}||Step 2) Detonate the C4.|run the command: {yel}detonate{/yel}|||Demolitions HOW TO: Section: Claymore Mines|===========================================|Step 1) Install the claymore mine at the specified door.|run the command: {yel}install claymore DIRECTION{/yel}|{grn}replace the phrase DISTANCE with the direction of the door you would like to install the claymore mine on.{/grn}||	demolitions-howto	what corp should say at orientation regarding demolitions	2	0	2020-10-04 04:06:45.361267	2020-10-04 04:06:45.361267
8	410	Drone assisted sniping HOW TO:|==============================|Step 1) Deploy your drone.|run the command: {yel}deploy drone{/yel}|You will now see the world through the eyes of your drone.||Step 2) Fly or ground travel to the target room.|run any direction commands to get to the room where your target enemy is. (north, east, south, etc)...||Step 3) Target your enemy.|run the command: {yel}target ENEMY{/yel} where ENEMY is the name of the mob.||Step 4) Snipe your target.|type this exactly to snipe your enemy: {yel}snipe target{/yel}. If you try to use |any other phrase other than target, the command will not work.||	drone-assisted-sniping-howto	course page for drone assisted sniping	2	0	2020-10-04 04:06:45.369464	2020-10-04 04:06:45.369464
\.


--
-- Data for Name: object; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.object (id, obj_item_number, obj_flags, obj_name, obj_description, obj_short_description, obj_action_description, obj_type, obj_worn_on, obj_type_data, obj_extra_keyword, obj_extra_description, obj_file, obj_belongs_to_player) FROM stdin;
2	137	0	mp5	A MP5 lies here.	a MP%	An MP5	1	0	0	<default>	<default>	mp5.yml	0
6	94	0	xm109 xm snipe sniper rifle	A devastating XM109 lies here.	a devastating XM109 sniper rifle	Action Description	1	0	0	<obj.ex_description->keyword>	<obj.ex_description->description>	xm109.yml	0
4	90	0	p90 smg	A P90 lies here.	A P90	A P90	1	0	0	p90	The FN P90, also known as the FN Project 1990, is a personal defense weapon (PDW) designed and manufactured by FN Herstal in Belgium. Created in response to NATO requests for a replacement for 919mm Parabellum firearms, the P90 was designed as a compact but powerful firearm for vehicle crews, operators of crew-served weapons, support personnel, special forces, and counter-terrorist groups.	p90.yml	0
5	91	0	p90 smg	A P90 lies here.	A P90	A P90	1	0	0	p90	A P90	p90.yml	0
\.


--
-- Data for Name: object_armor; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.object_armor (armor_id, armor_csv_capabilities, armor_csv_attach_to, armor_thac0, armor_weight_in_lbs, armor_fire_resistance_percent, armor_balistic_resistance_percent, armor_speed_profile, armor_offensive_damage_amount, armor_durability_profile, created_at, updated_at, armor_file, armor_str_type, armor_manufacturer, armor_name, armor_vnum, armor_rarity, armor_type, armor_hp, armor_classification, armor_worth) FROM stdin;
\.


--
-- Data for Name: object_explosive; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.object_explosive (explosive_id, explosive_alternate_explosion_type, explosive_chance_to_injure, explosive_critical_chance, explosive_critical_range, explosive_blast_radius, explosive_damage_per_second, explosive_disorient_amount, explosive_loudness_type, explosive_str_type, explosive_type, explosive_manufacturer, explosive_name, explosive_vnum, explosive_rarity, explosive_file, created_at, updated_at) FROM stdin;
\.


--
-- Data for Name: object_flags; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.object_flags (id, obj_fk_id, value_0, value_1, value_2, value_3, type_flag, wear_flags, extra_flags, weight, cost, cost_per_day, timer, bitvector, weapon_flags, ammo_max, clip_size) FROM stdin;
4	2	0	0	0	0	5	8193	0	0	0	0	0	0	0	250	21
9	3	0	0	0	0	15	0	0	0	0	0	0	0	0	250	21
14	4	0	0	0	0	5	8193	0	0	0	0	0	0	0	250	21
15	5	0	0	0	0	5	8193	0	0	0	0	0	0	0	250	21
25	6	0	0	0	0	5	8193	0	0	0	0	0	0	4	250	21
26	1	0	0	0	0	5	8193	0	0	0	0	0	0	0	250	21
\.


--
-- Data for Name: object_gadget; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.object_gadget (gadget_id, gadget_csv_capabilities, gadget_csv_attach_to, gadget_electronic, gadget_health_points, gadget_shield_points, gadget_durability_profile, gadget_move_points, gadget_damage_points, gadget_str_type, gadget_type, gadget_manufacturer, gadget_name, gadget_vnum, gadget_rarity, gadget_file, created_at, updated_at, gadget_exit_key, gadget_min_level, gadget_worth) FROM stdin;
1	watch,listen,hear,remotely-view,access,hack	wall,door,room	1	10	0	FLIMSY	0	0	CAMERA	5	M4x	Pi-Mx Deployable Camera	9	COMMON	camera.yml	2020-03-20 00:32:07.340178	2020-03-20 00:32:07.340178	0	0	0
\.


--
-- Data for Name: object_weapon; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.object_weapon (id, obj_fk_id, obj_ammo_max, obj_ammo_type, obj_cooldown, obj_can_snipe) FROM stdin;
1	4	0	0	0	f
2	5	0	0	0	f
3	6	0	4	0	f
4	1	0	0	0	f
5	7	0	0	0	f
\.


--
-- Data for Name: player; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.player (id, player_password, player_affection_plr_bitvector, player_affection_bitvector, player_name, player_short_description, player_long_description, player_action_bitvector, player_ability_strength, player_ability_strength_add, player_ability_intelligence, player_ability_wisdom, player_ability_dexterity, player_ability_constitution, player_ability_charisma, player_ability_alignment, player_attack_type, player_max_hitpoints, player_max_mana, player_max_move, player_gold, player_exp, player_sex, player_hitpoints, player_mana, player_move, player_damroll, player_weight, player_height, player_class, player_title, player_hometown, player_damnodice, player_damsizedice, player_type, player_alignment, player_level, player_hitroll, player_armor, player_birth, player_time_played, player_logon, player_preferences) FROM stdin;
1	foKntnEF3KSXA	0	0	far	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	M	0	0	0	0	0	0	0	0	0	0	0	PC	0	0	0	0	2019-03-20 22:38:47.454111	0	2019-03-20 22:38:47.454111	14680304
\.
update public.player set player_max_hitpoints=65000,player_max_mana=65000,player_max_move=65000,player_hitpoints=65000,player_mana=65000,player_move=65000,player_damroll=90 where player_name='far';

update public.player set player_ability_strength=6500, player_ability_strength_add=6500, player_ability_intelligence=6500, player_ability_wisdom=6500, player_ability_dexterity=6500, player_ability_constitution=6500, player_ability_charisma=6500, player_ability_alignment=6500, player_attack_type=6500, player_max_hitpoints=6500, player_max_mana=6500, player_max_move=6500, player_gold=6500, player_exp=6500, player_hitpoints=6500, player_mana=6500, player_move=6500, player_damroll=6500, player_class=1, player_damnodice=6500, player_damsizedice=6500, player_level=1, player_hitroll=6500, player_armor=6500 where player_name='far';

--
-- Data for Name: player_classes; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.player_classes (id, pclass_name, pclass_description) FROM stdin;
\.


--
-- Data for Name: player_flags; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.player_flags (id, player_id, chunk_index, flag_value) FROM stdin;
\.



--
-- Data for Name: player_race_perks; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.player_race_perks (id, prperk_id, prperk_name, prperk_description, prperk_base_attribute_mutate, prperk_base_attribute_amount_add) FROM stdin;
\.


--
-- Data for Name: player_races; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.player_races (id, prace_name, prace_description) FROM stdin;
\.


INSERT INTO public.zone (
		id,
    zone_virtual_number,
    zone_start,
    zone_end,
    zone_name,
    lifespan,
    reset_mode
) VALUES (1,1, 128,228, 'Hereford Base', 10, 0);


--
-- Data for Name: room; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.room (room_number, zone, sector_type, name, description, ex_keyword, ex_description, light, room_flag, id) FROM stdin;
128	1	18	MP5 repo	The ebb and flow of traffic has worn down the tile floor here. You can't help but notice the pungent chemical smell that permeates every corner of this dingy hideout. The owner obviously doesn't care for appearances, but you can't really blame him. The idea of a clean weapons shop is a foreign concept. The very mention of the subject of cleanliness is instantly dismissed. A heavily armored door to the east is likely where all official business is conducted behind the scenes. You notice the stock loss prevention camera jutting from the corner of the room which has the best angle on every corner of the room.\r\n	\N	\N	0	16	20
\.


--
-- Data for Name: shop_objects; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.shop_objects (shop_objects_id, shop_vnum, shop_object_vnum) FROM stdin;
1	2	90
2	2	91
3	3	90
4	3	91
5	2	137
6	3	137
7	3	94
8	2	94
\.


--
-- Data for Name: shop_rooms; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.shop_rooms (shop_rooms_id, shop_vnum, shop_room_vnum) FROM stdin;
1	2	128
\.


--
-- Data for Name: shops; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.shops (shop_id, shop_vnum, shop_title, shop_description, shop_profit_buy, shop_profit_sell, shop_type, shop_no_such_item1, shop_no_such_item2, shop_missing_cash1, shop_missing_cash2, shop_do_not_buy, shop_message_buy, shop_message_sell, shop_temper1, shop_bitvector, shop_keeper, shop_with_who, shop_open1, shop_open2, shop_close1, shop_close2, shop_bankaccount, shop_lastsort, shop_flags) FROM stdin;
2	2	MP5 repo	Feeling over burdened by money?	0	0	0	We don't carry that sort of thing here.	Look, we don't carry that...	No money, no product. Simple as that.	The precursor to buying guns is that you have to have money upfront...	I don't work with those types of items.	You got it. Here you go!	Nice. Maybe I can find some poor schmuck to sell this to...	0	0	0	0	0	0	0	0	0	0	0
3	3	MP5 repo	Feeling over burdened by money?	0	0	0	We don't carry that sort of thing here.	Look, we don't carry that...	No money, no product. Simple as that.	The precursor to buying guns is that you have to have money upfront...	I don't work with those types of items.	You got it. Here you go!	Nice. Maybe I can find some poor schmuck to sell this to...	0	0	0	0	0	0	0	0	0	0	0
\.


--
-- Data for Name: tasks; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.tasks (id, t_name) FROM stdin;
0	task 0
1	task 1
2	task 2
\.


--
-- Data for Name: tasks_notes; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.tasks_notes (id, task_id, t_notes) FROM stdin;
0	1	foobar
1	1	funbar
2	1	bazbar
\.


--
-- Data for Name: terminal_choices; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.terminal_choices (id, choice_terminal_id, choice_id, choice_title, choice_description, choice_action) FROM stdin;
1	1	1	Drone assisted Sniping Course	Train your ability to snipe at incredibly long distances while utilizing your drone.|(+10% accuracy when drone scans the current room)|(+8% chance to go undetected)|(+10% bonus to critical chance)	start_course ${choice_id}
2	1	2	Demolitions Course	This demolitions course will get you up to speed with grenades, C4 plastic explosive, and claymore mines.|(+10% bonus when dealing explosive damage)|(+2 room grenade throwing distance)|(+4% chance to injure with C4 explosives)|(10% chance of enemy becoming maimed/unconscious from claymore mine)	start_course ${choice_id}
3	1	3	Tactical Gadgetry Course	Utilize your PDA to hack devices and unlock areas previously locked down by the enemy.|Discover how to breach a door using a breach charge.|Learn how to utilize flashbang and smoke grenades to disorient and mask your movements.|Learn how E.M.P. grenades affect the enemy communications network.|Attack your enemies with powerful incendiary grenades.|(+10% incendiary grenade bonus)|(+2 room range for EMP status affect dispersal)|(+40 ticks to smoke affect duration)|(+40 ticks to flashbang disorient affect duration)|(4% chance of not consuming flash bang grenade when thrown)	start_course ${choice_id}
4	1	4	Marine Corps Martial Arts Program - {grn}Striking Clinic{/grn}	Learn to save and recall strike combos.|Learn how to transition from striking distance to grappling and submissions techniques.|Learn the powerful knee stomp.|Prevent enemy from taking you to the ground.|(+10% chance of stuffing take down)|(4% chance of knee stomp causing depletion of all movement points)|(+10% ticks to status duration from stunning jab disorient affect)|(10% chance to double up jab at no extra stamina cost)	start_course ${choice_id}
5	1	5	Marine Corps Martial Arts Program - {grn}Judo Trips and Throws{/grn}	Learn the inner leg reap to disorient your target while throwing them to the ground.|(10% chance of inner leg reap causing opponent to break knee)|Learn to perform the sacrifice throw to launch your enemy to the ground.|(10% chance of enemy being thrown to ground in another room)|Learn the drop shoulder throw to force your enemy to land on their head.|(15% chance of breaking neck and status being MAIMED)|(10% chance of breaking arm)	start_course ${choice_id}
6	1	6	Marine Corps Martial Arts Program - {grn}Submission Grappling{/grn}	Armbar - Level 1 -> Dislocate arm of target enemy using the armbar technique.|(10% chance of ranged attack completely failing if one arm dislocated by armbar [Level 1 Armbar])|(5% of damage done by enemy with dislocated arm is inflicted on enemy when melee attacks are dealt [Level 1 Armbar])|Armbar - Level 2 -> Completely break arm with no chance of repair.|(50% damage is dealt by enemies with one broken arm [Level 2 Armbar])|(25% chance for ranged attacks to fail from enemy with broken arm [Level 2 Armbar])|Armbar - Level 3 -> Break both arms.|(4% chance of second arm being broken once first arm is broken [Level 3 Armbar])|(100% of melee attacks are impossible if attack requires arms or ranged weaponry [Level 3 Armbar])|(85% chance of being able to take primary weapon from enemy with 1 broken arm [Level 3 Armbar])|Knee Bar -> Break the targets knee using your waist as a fulcrum.|(25% chance of target to fall when attempting to flee or move)|(20% of movement is permanently subtracted from target enemy with one broken knee)|Rear Naked Choke -> Level 1 -> Seize control of the target and apply a chokehold from behind that will knock the enemy unconscious if not stopped.|(4% chance of target losing consciousness and not recovering at all)|(+15% damage bonus if allies or gadgets deal damage to target while in RNC hold)|(2.5% chance of breaking neck and status becoming UNCONSCIOUS)	start_course ${choice_id}
\.


--
-- Data for Name: world_configuration_start_rooms; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.world_configuration_start_rooms (id, mortal_start_room, immortal_start_room, created_at, is_active, idle_room, frozen_room) FROM stdin;
4	128	128	2019-06-01 19:30:07.630823	t	0	0
\.


--
-- Name: affected_type_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.affected_type_id_seq', 1, false);


--
-- Name: camera_feed_feed_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.camera_feed_feed_id_seq', 1, false);


--
-- Name: computer_terminal_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.computer_terminal_id_seq', 1, true);


--
-- Name: extra_description_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.extra_description_id_seq', 10, true);


--
-- Name: integral_object_object_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.integral_object_object_id_seq', 1, false);


--
-- Name: karma_karma_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.karma_karma_id_seq', 1, false);


--
-- Name: mini_game_game_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.mini_game_game_id_seq', 4, true);


--
-- Name: mini_gunner_sentinel_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.mini_gunner_sentinel_id_seq', 2, true);


--
-- Name: mob_zone_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.mob_zone_id_seq', 1, false);


--
-- Name: mobile_mob_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.mobile_mob_id_seq', 11, true);


--
-- Name: npc_dialogue_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.npc_dialogue_id_seq', 8, true);


--
-- Name: object_armor_armor_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.object_armor_armor_id_seq', 1, false);


--
-- Name: object_explosive_explosive_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.object_explosive_explosive_id_seq', 1, false);


--
-- Name: object_flags_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.object_flags_id_seq', 26, true);


--
-- Name: object_gadget_gadget_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.object_gadget_gadget_id_seq', 1, true);


--
-- Name: object_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.object_id_seq', 7, true);


--
-- Name: object_rifle_rifle_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.object_rifle_rifle_id_seq', 3120, true);


--
-- Name: object_weapon_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.object_weapon_id_seq', 5, true);


--
-- Name: player_classes_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.player_classes_id_seq', 1, false);


--
-- Name: player_flags_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.player_flags_id_seq', 1, false);


--
-- Name: player_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.player_id_seq', 95, true);


--
-- Name: player_object_po_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.player_object_po_id_seq', 797, true);


--
-- Name: player_player_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.player_player_id_seq', 1, false);


--
-- Name: player_race_perks_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.player_race_perks_id_seq', 1, false);


--
-- Name: player_races_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.player_races_id_seq', 1, false);


--
-- Name: room_direction_data_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.room_direction_data_id_seq', 575, true);


--
-- Name: room_room_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

-- SELECT pg_catalog.setval('public.room_room_id_seq', 81, true);


--
-- Name: room_virtual_number_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.room_virtual_number_seq', 1, false);


--
-- Name: shop_objects_shop_objects_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.shop_objects_shop_objects_id_seq', 8, true);


--
-- Name: shop_rooms_shop_rooms_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.shop_rooms_shop_rooms_id_seq', 2, true);


--
-- Name: shops_shop_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.shops_shop_id_seq', 3, true);


--
-- Name: terminal_choices_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.terminal_choices_id_seq', 6, true);


--
-- Name: world_configuration_start_rooms_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.world_configuration_start_rooms_id_seq', 4, true);


--
-- Name: zone_data_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.zone_data_id_seq', 16, true);


--
-- Name: zone_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.zone_id_seq', 151, true);


--
-- Name: computer_terminal computer_terminal_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

-- ALTER TABLE ONLY public.computer_terminal
    --ADD CONSTRAINT computer_terminal_pkey PRIMARY KEY (id);


--
-- Name: mini_gunner_sentinel mini_gunner_sentinel_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

-- ALTER TABLE ONLY public.mini_gunner_sentinel
    --ADD CONSTRAINT mini_gunner_sentinel_pkey PRIMARY KEY (id);


--
-- Name: npc_dialogue npc_dialogue_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

-- ALTER TABLE ONLY public.npc_dialogue
    --ADD CONSTRAINT npc_dialogue_pkey PRIMARY KEY (id);


--
-- Name: player_classes player_classes_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.player_classes
    ADD CONSTRAINT player_classes_pkey PRIMARY KEY (id);


--
-- Name: player_flags player_flags_player_id_chunk_index_key; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.player_flags
    ADD CONSTRAINT player_flags_player_id_chunk_index_key UNIQUE (player_id, chunk_index);


--
-- Name: player player_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--



--
-- Name: player_race_perks player_race_perks_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.player_race_perks
    ADD CONSTRAINT player_race_perks_pkey PRIMARY KEY (id);


--
-- Name: player_races player_races_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.player_races
    ADD CONSTRAINT player_races_pkey PRIMARY KEY (id);

--
-- Name: terminal_choices terminal_choices_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.terminal_choices
    ADD CONSTRAINT terminal_choices_pkey PRIMARY KEY (id);


--
-- Name: mini_gunner_sentinel mini_gunner_sentinel_mgs_room_vnum_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.mini_gunner_sentinel
    ADD CONSTRAINT mini_gunner_sentinel_mgs_room_vnum_fkey FOREIGN KEY (mgs_room_vnum) REFERENCES public.room(room_number);


--
-- Name: player_race_perks player_race_perks_prperk_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.player_race_perks
    ADD CONSTRAINT player_race_perks_prperk_id_fkey FOREIGN KEY (prperk_id) REFERENCES public.player_races(id);



--
-- PostgreSQL database dump complete
--

CREATE TABLE public.player_base_ability (
		pba_id SERIAL,
		pba_player_id INTEGER NOT NULL,
		pba_str INTEGER NOT NULL DEFAULT 0,
		pba_str_add INTEGER NOT NULL DEFAULT 0,
		pba_intel INTEGER NOT NULL DEFAULT 0,
		pba_wis INTEGER NOT NULL DEFAULT 0,
		pba_dex INTEGER NOT NULL DEFAULT 0,
		pba_con INTEGER NOT NULL DEFAULT 0,
		pba_cha INTEGER NOT NULL DEFAULT 0,
		pba_electronics INTEGER NOT NULL DEFAULT 0,
		pba_armor INTEGER NOT NULL DEFAULT 0,
		pba_marksmanship INTEGER NOT NULL DEFAULT 0,
		pba_sniping INTEGER NOT NULL DEFAULT 0,
		pba_demolitions INTEGER NOT NULL DEFAULT 0,
		pba_chemistry INTEGER NOT NULL DEFAULT 0,
		pba_weapon_handling INTEGER NOT NULL DEFAULT 0,
		pba_strategy INTEGER NOT NULL DEFAULT 0,
		pba_medical INTEGER NOT NULL DEFAULT 0,
		created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
		updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE public.class_marine (
	marine_id SERIAL,
	marine_player_id INTEGER NOT NULL,
	created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
	,PRIMARY KEY(marine_id),
	CONSTRAINT fk_player_id
		FOREIGN KEY (marine_player_id)
		REFERENCES public.player(id)
		ON DELETE CASCADE
		ON UPDATE CASCADE
);
CREATE TABLE public.class_breacher (
	breacher_id SERIAL,
	breacher_player_id INTEGER NOT NULL,
	created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
	,PRIMARY KEY(breacher_id),
	CONSTRAINT fk_player_id
		FOREIGN KEY (breacher_player_id)
		REFERENCES public.player(id)
		ON DELETE CASCADE
		ON UPDATE CASCADE
);

CREATE TABLE public.class_engineer (
	engineer_id SERIAL,
	engineer_player_id INTEGER NOT NULL,
	created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
	,PRIMARY KEY(engineer_id),
	CONSTRAINT fk_player_id
		FOREIGN KEY (engineer_player_id)
		REFERENCES public.player(id)
		ON DELETE CASCADE
		ON UPDATE CASCADE
);
CREATE TABLE public.class_medic (
	medic_id SERIAL,
	medic_player_id INTEGER NOT NULL,
	created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
	,PRIMARY KEY(medic_id),
	CONSTRAINT fk_player_id
		FOREIGN KEY (medic_player_id)
		REFERENCES public.player(id)
		ON DELETE CASCADE
		ON UPDATE CASCADE
);
CREATE TABLE public.class_support (
	support_id SERIAL,
	support_player_id INTEGER NOT NULL,
	created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
	,PRIMARY KEY(support_id),
	CONSTRAINT fk_player_id
		FOREIGN KEY (support_player_id)
		REFERENCES public.player(id)
		ON DELETE CASCADE
		ON UPDATE CASCADE
);
CREATE TABLE public.class_ghost (
	ghost_id SERIAL,
	ghost_player_id INTEGER NOT NULL,
	created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
	,PRIMARY KEY(ghost_id),
	CONSTRAINT fk_player_id
		FOREIGN KEY (ghost_player_id)
		REFERENCES public.player(id)
		ON DELETE CASCADE
		ON UPDATE CASCADE
);
CREATE TABLE public.class_sniper (
	sniper_id SERIAL,
	sniper_player_id INTEGER NOT NULL,
	created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
	,PRIMARY KEY(sniper_id),
	CONSTRAINT fk_player_id
		FOREIGN KEY (sniper_player_id)
		REFERENCES public.player(id)
		ON DELETE CASCADE
		ON UPDATE CASCADE
);
insert into public.class_sniper (sniper_player_id) VALUES((SELECT id from public.player where player_name='far'));
ALTER TABLE public.mob_roam ADD COLUMN profile_name varchar(255);
CREATE TABLE public.rifle_index (
  id SERIAL UNIQUE,
	rifle_filename VARCHAR NOT NULL,
	rifle_type VARCHAR NOT NULL,
	created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	PRIMARY KEY(id)
);
INSERT INTO public.rifle_index ( rifle_filename, rifle_type) VALUES ( 'augpara.yml', 'smg');
INSERT INTO public.rifle_index ( rifle_filename, rifle_type) VALUES ( 'fmg9.yml', 'smg');
INSERT INTO public.rifle_index ( rifle_filename, rifle_type) VALUES ( 'mp5.yml', 'smg');
INSERT INTO public.rifle_index ( rifle_filename, rifle_type) VALUES ( 'mp9.yml', 'smg');
INSERT INTO public.rifle_index ( rifle_filename, rifle_type) VALUES ( 'p90.yml', 'smg');
INSERT INTO public.rifle_index ( rifle_filename, rifle_type) VALUES ( 'tar21.yml', 'smg');
INSERT INTO public.rifle_index ( rifle_filename, rifle_type) VALUES ( 'ump45.yml', 'smg');

INSERT INTO public.rifle_index ( rifle_filename, rifle_type) VALUES ( 'l96aw.yml', 'sniper');
INSERT INTO public.rifle_index ( rifle_filename, rifle_type) VALUES ( 'psg1.yml', 'sniper');
INSERT INTO public.rifle_index ( rifle_filename, rifle_type) VALUES ( 'xm109.yml', 'sniper');

INSERT INTO public.rifle_index ( rifle_filename, rifle_type) VALUES ( 'belt-fed-minigun.yml', 'lmg');
INSERT INTO public.rifle_index ( rifle_filename, rifle_type) VALUES ( 'hk21.yml', 'lmg');
INSERT INTO public.rifle_index ( rifle_filename, rifle_type) VALUES ( 'mk46.yml', 'lmg');

INSERT INTO public.rifle_index ( rifle_filename, rifle_type) VALUES ( 'czp10.yml', 'pistol');
INSERT INTO public.rifle_index ( rifle_filename, rifle_type) VALUES ( 'desert-eagle.yml', 'pistol');
INSERT INTO public.rifle_index ( rifle_filename, rifle_type) VALUES ( 'glock.yml', 'pistol');
INSERT INTO public.rifle_index ( rifle_filename, rifle_type) VALUES ( 'magnum-revolver.yml', 'pistol');
INSERT INTO public.rifle_index ( rifle_filename, rifle_type) VALUES ( 'ppk.yml', 'pistol');

INSERT INTO public.rifle_index ( rifle_filename, rifle_type) VALUES ( 'famas.yml', 'ar');
INSERT INTO public.rifle_index ( rifle_filename, rifle_type) VALUES ( 'g36c.yml', 'ar');
INSERT INTO public.rifle_index ( rifle_filename, rifle_type) VALUES ( 'm16a4.yml', 'ar');
INSERT INTO public.rifle_index ( rifle_filename, rifle_type) VALUES ( 'm3.yml', 'ar');
INSERT INTO public.rifle_index ( rifle_filename, rifle_type) VALUES ( 'm4.yml', 'ar');
INSERT INTO public.rifle_index ( rifle_filename, rifle_type) VALUES ( 'scarh.yml', 'ar');
INSERT INTO public.rifle_index ( rifle_filename, rifle_type) VALUES ( '552-commando.yml', 'ar');
INSERT INTO public.rifle_index ( rifle_filename, rifle_type) VALUES ( 'aug-a3.yml', 'ar');

INSERT INTO public.rifle_index ( rifle_filename, rifle_type) VALUES ( 'saiga12.yml', 'shotgun');
INSERT INTO public.rifle_index ( rifle_filename, rifle_type) VALUES ( 'sasg12.yml', 'shotgun');

INSERT INTO public.rifle_index ( rifle_filename, rifle_type) VALUES ( 'uzi.yml', 'mp');

CREATE TABLE public.armor_index (
  id SERIAL UNIQUE,
	armor_filename VARCHAR NOT NULL,
	armor_type VARCHAR NOT NULL,
	created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	PRIMARY KEY(id)
);

INSERT INTO public.armor_index(armor_filename,armor_type) VALUES('baklava.yml','head');
INSERT INTO public.armor_index(armor_filename,armor_type) VALUES('tactical-gas-mask.yml','head');

INSERT INTO public.armor_index(armor_filename,armor_type) VALUES('thermal-goggles.yml','goggles');
INSERT INTO public.armor_index(armor_filename,armor_type) VALUES('night-vision-goggles.yml','goggles');

INSERT INTO public.armor_index(armor_filename,armor_type) VALUES('titan-shoulder-pads.yml','shoulders');
INSERT INTO public.armor_index(armor_filename,armor_type) VALUES('xm-scorpio-shoulder-pads.yml','shoulders');
INSERT INTO public.armor_index(armor_filename,armor_type) VALUES('viper-shoulder-pads.yml','shoulders');
INSERT INTO public.armor_index(armor_filename,armor_type) VALUES('falcon-shoulder-pads.yml','shoulders');

INSERT INTO public.armor_index(armor_filename,armor_type) VALUES('basic-ballistic-vest.yml','vest');
INSERT INTO public.armor_index(armor_filename,armor_type) VALUES('falcon-ballistic-vest.yml','vest');
INSERT INTO public.armor_index(armor_filename,armor_type) VALUES('raven-ballistic-vest.yml','vest');
INSERT INTO public.armor_index(armor_filename,armor_type) VALUES('vulture-ballistic-vest.yml','vest');

INSERT INTO public.armor_index(armor_filename,armor_type) VALUES('xm-scorpio-slotted-vest-pack.yml','vestpack');

INSERT INTO public.armor_index(armor_filename,armor_type) VALUES('raven-ultralight-backpack.yml','backpack');

INSERT INTO public.armor_index(armor_filename,armor_type) VALUES('titan-gauntlets.yml','arms');

INSERT INTO public.armor_index(armor_filename,armor_type) VALUES('titan-elbow-guards.yml','elbow');

INSERT INTO public.armor_index(armor_filename,armor_type) VALUES('razor-gps-wrist-watch.yml','wrist');

INSERT INTO public.armor_index(armor_filename,armor_type) VALUES('xm-scorpio-tactical-gloves.yml','hands');

INSERT INTO public.armor_index(armor_filename,armor_type) VALUES('xm-scorpio-belt.yml','waist');

INSERT INTO public.armor_index(armor_filename,armor_type) VALUES('titan-shin-guards.yml','legs');
INSERT INTO public.armor_index(armor_filename,armor_type) VALUES('viper-leg-guards.yml','legs');

INSERT INTO public.armor_index(armor_filename,armor_type) VALUES('basic-boots.yml','feet');
INSERT INTO public.armor_index(armor_filename,armor_type) VALUES('xm50-ultralight-boots.yml','feet');
INSERT INTO public.armor_index(armor_filename,armor_type) VALUES('xm607-vulture-boots.yml','feet');
INSERT INTO public.armor_index(armor_filename,armor_type) VALUES('xm8-panama-combat-boots.yml','feet');


CREATE TABLE public.skill_points (
  id SERIAL UNIQUE,
	sp_level integer NOT NULL,
	sp_points integer NOT NULL DEFAULT 0,
	created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	PRIMARY KEY(id)
);

INSERT INTO public.skill_points (sp_level,sp_points) VALUES(1, 1);
INSERT INTO public.skill_points (sp_level,sp_points) VALUES(2, 1);
INSERT INTO public.skill_points (sp_level,sp_points) VALUES(3, 1);
INSERT INTO public.skill_points (sp_level,sp_points) VALUES(4, 1);
INSERT INTO public.skill_points (sp_level,sp_points) VALUES(5, 1);
INSERT INTO public.skill_points (sp_level,sp_points) VALUES(6, 1);
INSERT INTO public.skill_points (sp_level,sp_points) VALUES(7, 1);
INSERT INTO public.skill_points (sp_level,sp_points) VALUES(8, 1);
INSERT INTO public.skill_points (sp_level,sp_points) VALUES(9, 1);
INSERT INTO public.skill_points (sp_level,sp_points) VALUES(10, 1);
INSERT INTO public.skill_points (sp_level,sp_points) VALUES(11, 1);
INSERT INTO public.skill_points (sp_level,sp_points) VALUES(12, 1);
INSERT INTO public.skill_points (sp_level,sp_points) VALUES(13, 1);
INSERT INTO public.skill_points (sp_level,sp_points) VALUES(14, 1);
INSERT INTO public.skill_points (sp_level,sp_points) VALUES(15, 1);
INSERT INTO public.skill_points (sp_level,sp_points) VALUES(16, 1);
INSERT INTO public.skill_points (sp_level,sp_points) VALUES(17, 1);
INSERT INTO public.skill_points (sp_level,sp_points) VALUES(18, 1);
INSERT INTO public.skill_points (sp_level,sp_points) VALUES(19, 1);
INSERT INTO public.skill_points (sp_level,sp_points) VALUES(20, 1);
INSERT INTO public.skill_points (sp_level,sp_points) VALUES(21, 1);
INSERT INTO public.skill_points (sp_level,sp_points) VALUES(22, 1);
INSERT INTO public.skill_points (sp_level,sp_points) VALUES(23, 1);
INSERT INTO public.skill_points (sp_level,sp_points) VALUES(24, 1);
INSERT INTO public.skill_points (sp_level,sp_points) VALUES(25, 1);
INSERT INTO public.skill_points (sp_level,sp_points) VALUES(26, 1);
INSERT INTO public.skill_points (sp_level,sp_points) VALUES(27, 1);
INSERT INTO public.skill_points (sp_level,sp_points) VALUES(28, 1);
INSERT INTO public.skill_points (sp_level,sp_points) VALUES(29, 1);
INSERT INTO public.skill_points (sp_level,sp_points) VALUES(30, 1);



CREATE TABLE public.skill_trees (
  id SERIAL UNIQUE,
	skill_name varchar(255) NOT NULL,
	skill_category varchar(255) NOT NULL,
	skill_player_class varchar(255) NOT NULL,
	skill_parent_id integer,	-- refers back to the primary key of this table
	skill_order integer NOT NULL DEFAULT 0,
	skill_description TEXT,
	created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	UNIQUE(skill_player_class,skill_category,skill_name),
	PRIMARY KEY(id),
		CONSTRAINT fk_parent_id
			FOREIGN KEY (skill_parent_id)
			REFERENCES public.skill_trees(id)
			ON DELETE CASCADE
			ON UPDATE CASCADE
);
INSERT INTO public.skill_trees (
	skill_name,
	skill_player_class,
	skill_category,
	skill_parent_id,
	skill_order
) VALUES(
				'intel:1-tracking-shot',
				'sniper',
				'Technology',
				NULL,
				0
			);

INSERT INTO public.skill_trees (
	skill_name,
	skill_player_class,
	skill_category,
	skill_parent_id,
	skill_order
) VALUES(
				'intel:2-sensor-grenade',
				'sniper',
				'Technology',
				(SELECT id from public.skill_trees WHERE skill_name='intel:1-tracking-shot'),
				0
			);
INSERT INTO public.skill_trees (
	skill_name,
	skill_player_class,
	skill_category,
	skill_parent_id,
	skill_order
) VALUES(
				'intel:3-xray-shot',
				'sniper',
				'Technology',
				(SELECT id from public.skill_trees WHERE skill_name='intel:2-sensor-grenade'),
				1
			);
INSERT INTO public.skill_trees (
	skill_name,
	skill_player_class,
	skill_category,
	skill_parent_id,
	skill_order
) VALUES(
				'intel:4-area-scan',
				'sniper',
				'Technology',
				(SELECT id from public.skill_trees WHERE skill_name='intel:3-sensor-grenade'),
				2
			);
INSERT INTO public.skill_trees (
	skill_name,
	skill_player_class,
	skill_category,
	skill_parent_id,
	skill_order
) VALUES(
				'disrupt:1-chaff-grenade',
				'sniper',
				'Technology',
				NULL,
				0
			);
INSERT INTO public.skill_trees (
	skill_name,
	skill_player_class,
	skill_category,
	skill_parent_id,
	skill_order
) VALUES(
				'disrupt:2-emp-grenade',
				'sniper',
				'Technology',
				(SELECT id from public.skill_trees WHERE skill_name='disrupt:1-chaff-grenade'),
				1
			);

INSERT INTO public.skill_trees (
	skill_name,
	skill_player_class,
	skill_category,
	skill_parent_id,
	skill_order
) VALUES(
				'claymore:1-plant',
				'sniper',
				'Demolitions',
				NULL,
				0
			);
INSERT INTO public.skill_trees (
	skill_name,
	skill_player_class,
	skill_category,
	skill_parent_id,
	skill_order
) VALUES(
				'claymore:2-shrapnel',
				'sniper',
				'Demolitions',
				(SELECT id from public.skill_trees WHERE skill_name='claymore:1-plant'),
				1
			);
INSERT INTO public.skill_trees (
	skill_name,
	skill_player_class,
	skill_category,
	skill_parent_id,
	skill_order
) VALUES(
				'claymore:3-corrosive',
				'sniper',
				'Demolitions',
				(SELECT id from public.skill_trees WHERE skill_name='claymore:2-shrapnel'),
				1
			);
INSERT INTO public.skill_trees (
	skill_name,
	skill_player_class,
	skill_category,
	skill_parent_id,
	skill_order
) VALUES(
				'guided-missile',
				'sniper',
				'Demolitions',
				NULL,
				1
			);
INSERT INTO public.skill_trees (
	skill_name,
	skill_player_class,
	skill_category,
	skill_parent_id,
	skill_order
) VALUES(
				'light-bandage',
				'sniper',
				'Medical',
				NULL,
				1
			);
INSERT INTO public.skill_trees (
	skill_name,
	skill_player_class,
	skill_category,
	skill_parent_id,
	skill_order
) VALUES(
				'suture',
				'sniper',
				'Medical',
				NULL,
				1
			);
INSERT INTO public.skill_trees (
	skill_name,
	skill_player_class,
	skill_category,
	skill_parent_id,
	skill_order
) VALUES(
				'adrenaline-shot',
				'sniper',
				'Medical',
				NULL,
				1
			);
INSERT INTO public.skill_trees (
	skill_name,
	skill_player_class,
	skill_category,
	skill_parent_id,
	skill_order
) VALUES(
				'underbarrel-grenade-launcher',
				'sniper',
				'Sniping',
				NULL,
				1
			);
INSERT INTO public.skill_trees (
	skill_name,
	skill_player_class,
	skill_category,
	skill_parent_id,
	skill_order
) VALUES(
				'underbarrel-shotgun',
				'sniper',
				'Sniping',
				NULL,
				1
			);
INSERT INTO public.skill_trees (
	skill_name,
	skill_player_class,
	skill_category,
	skill_parent_id,
	skill_order
) VALUES(
				'target-limb',
				'sniper',
				'Sniping',
				NULL,
				1
			);

CREATE TABLE public.player_skill_usage (
	id SERIAL UNIQUE,
	ps_player_id integer NOT NULL,
	ps_skill_id integer NOT NULL,
	ps_usage_count integer NOT NULL,
	created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	PRIMARY KEY(id),
		CONSTRAINT fk_player_id
		FOREIGN KEY (ps_player_id)
		REFERENCES public.player(id)
			ON DELETE CASCADE
			ON UPDATE CASCADE,
		CONSTRAINT fk_skill_id
		FOREIGN KEY (ps_skill_id)
		REFERENCES public.skill_trees(id)
			ON DELETE CASCADE
			ON UPDATE CASCADE
		);

CREATE TABLE public.player_skill_points (
  id SERIAL UNIQUE,
	ps_skill_id integer NOT NULL,
	ps_points integer NOT NULL DEFAULT 0,
	ps_player_id integer NOT NULL,
	created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	PRIMARY KEY(id),
		CONSTRAINT fk_skill_id
			FOREIGN KEY (ps_skill_id)
			REFERENCES public.skill_trees(id)
			ON DELETE CASCADE
			ON UPDATE CASCADE,
		CONSTRAINT fk_player_id
			FOREIGN KEY (ps_player_id)
			REFERENCES public.player(id)
			ON DELETE CASCADE
			ON UPDATE CASCADE
);

CREATE TABLE public.rifle_instance (
  rifle_id SERIAL UNIQUE,
  rifle_accuracy_map_0 double precision DEFAULT 10.0,
  rifle_accuracy_map_1 double precision DEFAULT 10.0,
  rifle_accuracy_map_2 double precision DEFAULT 10.0,
  rifle_accuracy_map_3 double precision DEFAULT 10.0,
  rifle_damage_map_0 double precision DEFAULT 10.0,
  rifle_damage_map_1 double precision DEFAULT 10.0,
  rifle_damage_map_2 double precision DEFAULT 10.0,
  rifle_damage_map_3 double precision DEFAULT 10.0,
	rifle_rarity public.rarity_t DEFAULT 'COMMON'::public.rarity_t NOT NULL,
	rifle_file VARCHAR(32),
	rifle_str_type VARCHAR(16),
	rifle_type VARCHAR(16),
	rifle_manufacturer VARCHAR(32),
	rifle_name VARCHAR(32),
	rifle_vnum integer,
	rifle_ammo_max integer,
	rifle_ammo_type VARCHAR(16) NOT NULL DEFAULT 'SNIPER',
	rifle_chance_to_injure FLOAT,
	rifle_clip_size integer,
	rifle_cooldown_between_shots integer,
	rifle_critical_chance integer,
	rifle_critical_range integer,
	rifle_damage_per_second double precision,
	rifle_disorient_amount double precision,
	rifle_headshot_bonus double precision,
	rifle_max_range integer,
	rifle_range_multiplier double precision,
	rifle_reload_time integer,
	rifle_rounds_per_minute integer,
	rifle_muzzle_velocity integer,
	rifle_effective_firing_range integer,
	rifle_damage_dice_count integer,
	rifle_damage_dice_sides integer,
	rifle_incendiary_damage integer,
	rifle_explosive_damage integer,
	rifle_shrapnel_damage integer,
	rifle_corrosive_damage integer,
	rifle_cryogenic_damage integer,
	rifle_radioactive_damage integer,
	rifle_emp_damage integer,
	rifle_shock_damage integer,
	rifle_anti_matter_damage integer,
	rifle_stat_strength integer,
	rifle_stat_intelligence integer,
	rifle_stat_wisdom integer,
	rifle_stat_dexterity integer,
	rifle_stat_constitution integer,
	rifle_stat_electronics integer,
	rifle_stat_armor integer,
	rifle_stat_marksmanship integer,
	rifle_stat_sniping integer,
	rifle_stat_demolitions integer,
	rifle_stat_chemistry integer,
	rifle_stat_weapon_handling integer,
	rifle_stat_strategy integer,
	rifle_stat_medical integer,
	created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	PRIMARY KEY(rifle_id)
);

CREATE TABLE public.rifle_placements (
  id SERIAL UNIQUE,
	ip_room_vnum integer NOT NULL,
	ip_container_selector TEXT,
	ip_rifle_id integer NOT NULL,
	created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	PRIMARY KEY(id),
		CONSTRAINT fk_rifle_id
			FOREIGN KEY (ip_rifle_id)
			REFERENCES public.rifle_instance(rifle_id)
			ON DELETE CASCADE
			ON UPDATE CASCADE
);

CREATE TABLE public.mob_equipment (
	id SERIAL UNIQUE,
	meq_profile_name varchar(1024),
	meq_vnum integer UNIQUE NOT NULL,
	meq_light varchar(1024),    
	meq_finger_r varchar(1024), 
	meq_finger_l varchar(1024), 
	meq_neck_1 varchar(1024),    
	meq_neck_2 varchar(1024),    
	meq_body varchar(1024),     
	meq_head varchar(1024),     
	meq_legs varchar(1024),     
	meq_feet varchar(1024),     
	meq_hands varchar(1024),    
	meq_arms varchar(1024),     
	meq_shield varchar(1024),   
	meq_about varchar(1024),    
	meq_waist varchar(1024),    
	meq_wrist_r varchar(1024),  
	meq_wrist_l varchar(1024),  
	meq_wield varchar(1024),    
	meq_hold varchar(1024),     
	meq_secondary varchar(1024),
	meq_shoulders_l varchar(1024),
	meq_shoulders_r varchar(1024),
	meq_backpack varchar(1024),
	meq_goggles varchar(1024),
	meq_vest_pack varchar(1024),
	meq_elbow_l varchar(1024),
	meq_elbow_r varchar(1024),
	created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	PRIMARY KEY(id)
);
CREATE TABLE public.mob_equipment_map (
  id SERIAL UNIQUE,
	mmap_mob_vnum integer NOT NULL UNIQUE,
	mmap_mob_equipment_vnum integer NOT NULL,
	created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	PRIMARY KEY(id),
		CONSTRAINT fk_mob_vnum
			FOREIGN KEY (mmap_mob_vnum)
			REFERENCES public.mobile(mob_virtual_number)
			ON DELETE CASCADE
			ON UPDATE CASCADE,
		CONSTRAINT fk_meq_vnum
			FOREIGN KEY (mmap_mob_equipment_vnum)
			REFERENCES public.mob_equipment(meq_vnum)
			ON DELETE CASCADE
			ON UPDATE CASCADE
);
CREATE TABLE public.hq_locations (
	id SERIAL UNIQUE,
	hq_affiliation varchar(32),
	hq_room_vnum integer UNIQUE NOT NULL,
	hq_level integer NOT NULL DEFAULT 1,
	hq_basic_mob_count integer NOT NULL DEFAULT 10,
	hq_advanced_mob_count integer NOT NULL DEFAULT 10,
	hq_elite_mob_count integer NOT NULL DEFAULT 10,
	hq_suv_count integer NOT NULL DEFAULT 10,
	hq_sedan_count integer NOT NULL DEFAULT 20,
	hq_armored_van_count integer NOT NULL DEFAULT 2,
	hq_replenish_ticks integer NOT NULL DEFAULT 150,
	hq_replenish_basic_count integer NOT NULL DEFAULT 15,
	hq_replenish_advanced_count integer NOT NULL DEFAULT 10,
	hq_replenish_elite_count integer NOT NULL DEFAULT 2,
	created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	PRIMARY KEY(id),
		CONSTRAINT fk_room_vnum
			FOREIGN KEY (hq_room_vnum)
			REFERENCES public.room(room_number)
			ON DELETE CASCADE
			ON UPDATE CASCADE
);
CREATE TABLE public.contracts (
	id SERIAL UNIQUE,
	c_vnum integer NOT NULL UNIQUE,
	c_description TEXT NOT NULL,
	c_title TEXT NOT NULL,
	created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	PRIMARY KEY(id)
);
CREATE TABLE public.contract_steps (
	id SERIAL UNIQUE,
	s_contract_vnum integer not null,
	s_task_type integer NOT NULL,
	s_task_target integer NOT NULL,
	s_description TEXT NOT NULL,
	s_object_yaml TEXT,
	s_mob_vnum integer,
	s_room_vnum integer,
	s_quota integer,
	s_is_optional boolean NOT NULL DEFAULT false,
	s_order integer not null,
	s_reward_xp integer,
	s_reward_money integer,
	s_reward_1 text,
	s_reward_2 text,
	s_reward_3 text,
	s_reward_4 text,
	s_reward_5 text,
	s_reward_6 text,
	s_reward_7 text,
	s_reward_8 text,
	s_reward_9 text,
	s_reward_10 text,
	created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	PRIMARY KEY(id),
		CONSTRAINT fk_contract_vnum
			FOREIGN KEY (s_contract_vnum)
			REFERENCES public.contracts(c_vnum)
			ON DELETE CASCADE
			ON UPDATE CASCADE
);
CREATE TABLE public.player_contract_state (
	id SERIAL UNIQUE,
	pc_player_id integer NOT NULL,
	pc_contract_vnum integer NOT NULL,
	pc_state_data TEXT,
	created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	PRIMARY KEY(id),
		CONSTRAINT fk_cvnum
			FOREIGN KEY (pc_contract_vnum)
			REFERENCES public.contracts(c_vnum)
			ON DELETE CASCADE
			ON UPDATE CASCADE,
		CONSTRAINT fk_player_id
			FOREIGN KEY (pc_player_id)
			REFERENCES public.player(id)
			ON DELETE CASCADE
			ON UPDATE CASCADE
);
CREATE TABLE public.rifle_attachment (
	id SERIAL UNIQUE,
	rifle_player_id integer NOT NULL,
	rifle_data TEXT NOT NULL,
	rifle_position TEXT NOT NULL DEFAULT 'inventory',
	created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	PRIMARY KEY(id),
		CONSTRAINT fk_player_id
			FOREIGN KEY (rifle_player_id)
			REFERENCES public.player(id)
			ON DELETE CASCADE
			ON UPDATE CASCADE
);

ALTER TABLE public.room ADD COLUMN nickname TEXT; 
CREATE TABLE public.scripted_sequences (
	id SERIAL UNIQUE,
	s_sequence_vnum INTEGER NOT NULL UNIQUE,
	PRIMARY KEY(id)
);
CREATE TABLE public.contract_step_callback (
	id SERIAL UNIQUE,
		s_contract_vnum INTEGER NOT NULL,
		s_task_type TEXT,
		s_task_target TEXT,
		s_task_vnum INTEGER,
		s_sequence_vnum INTEGER NOT NULL,
	PRIMARY KEY(id),
		CONSTRAINT fk_contract_vnum
			FOREIGN KEY (s_contract_vnum)
			REFERENCES public.contracts(c_vnum)
			ON DELETE CASCADE
			ON UPDATE CASCADE,
		CONSTRAINT fk_sequence_vnum
			FOREIGN KEY (s_sequence_vnum)
			REFERENCES public.scripted_sequences(s_sequence_vnum)
			ON DELETE CASCADE
			ON UPDATE CASCADE
);
CREATE TABLE public.scripted_steps (
	id SERIAL UNIQUE,
	s_sequence_vnum INTEGER NOT NULL,
	s_wait_ticks INTEGER NOT NULL DEFAULT 0,
	s_mob INTEGER,
	s_obj INTEGER,
	s_room INTEGER,
	s_quantity INTEGER,
	s_order INTEGER NOT NULL DEFAULT 0,
	s_interpret TEXT,
	s_yaml TEXT,
	s_type TEXT NOT NULL,
	s_dialogue TEXT,
	PRIMARY KEY(id),
		CONSTRAINT fk_sequence_vnum
			FOREIGN KEY (s_sequence_vnum)
			REFERENCES public.scripted_sequences(s_sequence_vnum)
			ON DELETE CASCADE
			ON UPDATE CASCADE
);

CREATE TABLE public.weapon_locker (
	id SERIAL UNIQUE,
	w_room_vnum INTEGER[] NOT NULL,
	w_yaml TEXT[] NOT NULL,
	PRIMARY KEY(id)
);

CREATE TABLE public.armor_locker (
	id SERIAL UNIQUE,
	a_room_vnum INTEGER[] NOT NULL,
	a_yaml TEXT[] NOT NULL,
	PRIMARY KEY(id)
);

CREATE TABLE public.room_signs (
	id SERIAL UNIQUE,
	r_room_vnum INTEGER NOT NULL,
	r_name TEXT NOT NULL,
	r_text TEXT NOT NULL,
	PRIMARY KEY(id),
		CONSTRAINT fk_room_vnum
			FOREIGN KEY (r_room_vnum)
			REFERENCES public.room(room_number)
			ON DELETE CASCADE
			ON UPDATE CASCADE
);

DROP TABLE public.player_object;
CREATE TABLE public.player_object (
    id SERIAL UNIQUE,
    po_player_id integer NOT NULL,
    po_type integer NOT NULL,
    po_type_id integer,
		po_yaml TEXT,
    po_load_type integer NOT NULL,
    po_wear_position integer,
    po_in_inventory integer,
		po_quantity integer NOT NULL DEFAULT 1,
	PRIMARY KEY(id),
		CONSTRAINT fk_player_id
			FOREIGN KEY (po_player_id)
			REFERENCES public.player(id)
			ON DELETE CASCADE
			ON UPDATE CASCADE
);

CREATE TABLE public.skill_usage (
    id SERIAL UNIQUE,
    player_id integer NOT NULL,
    skill_name VARCHAR(32) NOT NULL,
		skill_level float NOT NULL DEFAULT 0.0,
	PRIMARY KEY(id),
		CONSTRAINT fk_player_id
			FOREIGN KEY (player_id)
			REFERENCES public.player(id)
			ON DELETE CASCADE
			ON UPDATE CASCADE
);

ALTER TABLE public.player ADD COLUMN player_practice_sessions INTEGER NOT NULL DEFAULT 1;
/** sets mini gunner extended types to car thief extended types */
update public.mobile set mob_special_extended_type=14 where mob_special_extended_type=1;
