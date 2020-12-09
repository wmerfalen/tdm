--
-- PostgreSQL database dump
--

-- Dumped from database version 10.11 (Debian 10.11-1.pgdg90+1)
-- Dumped by pg_dump version 10.11 (Debian 10.11-1.pgdg90+1)
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
    id integer NOT NULL,
    aff_fk_id integer NOT NULL,
    aff_location integer NOT NULL,
    aff_modifier integer NOT NULL
);


ALTER TABLE public.affected_type OWNER TO postgres;

--
-- Name: affected_type_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.affected_type_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.affected_type_id_seq OWNER TO postgres;

--
-- Name: affected_type_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.affected_type_id_seq OWNED BY public.affected_type.id;


--
-- Name: camera_feed; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.camera_feed (
    feed_id integer NOT NULL,
    feed_type character varying(16) NOT NULL,
    feed_vnum integer NOT NULL,
    feed_room_vnum integer NOT NULL,
    feed_order integer DEFAULT 0 NOT NULL,
    created_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
    updated_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL
);


ALTER TABLE public.camera_feed OWNER TO postgres;

--
-- Name: camera_feed_feed_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.camera_feed_feed_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.camera_feed_feed_id_seq OWNER TO postgres;

--
-- Name: camera_feed_feed_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.camera_feed_feed_id_seq OWNED BY public.camera_feed.feed_id;


--
-- Name: class_contagion; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.class_contagion (
    contagion_id integer NOT NULL,
    contagion_player_id integer NOT NULL,
    contagion_primary_type character varying(16) DEFAULT 'M3'::character varying NOT NULL,
    contagion_primary_weapon_id integer DEFAULT 0 NOT NULL,
    contagion_secondary_type character varying(16) DEFAULT 'CZP10'::character varying NOT NULL,
    contagion_secondary_weapon_id integer DEFAULT 0 NOT NULL,
    created_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
    updated_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL
);


ALTER TABLE public.class_contagion OWNER TO postgres;

--
-- Name: class_contagion_contagion_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.class_contagion_contagion_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.class_contagion_contagion_id_seq OWNER TO postgres;

--
-- Name: class_contagion_contagion_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.class_contagion_contagion_id_seq OWNED BY public.class_contagion.contagion_id;


--
-- Name: class_engineer; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.class_engineer (
    engineer_id integer NOT NULL,
    engineer_player_id integer NOT NULL,
    engineer_primary_type character varying(16) DEFAULT 'FMG9'::character varying NOT NULL,
    engineer_primary_weapon_id integer DEFAULT 0 NOT NULL,
    engineer_secondary_type character varying(16) DEFAULT 'CZP10'::character varying NOT NULL,
    engineer_secondary_weapon_id integer DEFAULT 0 NOT NULL,
    created_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
    updated_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL
);


ALTER TABLE public.class_engineer OWNER TO postgres;

--
-- Name: class_engineer_engineer_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.class_engineer_engineer_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.class_engineer_engineer_id_seq OWNER TO postgres;

--
-- Name: class_engineer_engineer_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.class_engineer_engineer_id_seq OWNED BY public.class_engineer.engineer_id;


--
-- Name: class_ghost; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.class_ghost (
    ghost_id integer NOT NULL,
    ghost_player_id integer NOT NULL,
    ghost_drone_scan_level integer DEFAULT 0 NOT NULL,
    ghost_stealth_level integer DEFAULT 0 NOT NULL,
    ghost_xray_shot_level integer DEFAULT 0 NOT NULL,
    ghost_feign_death_level integer DEFAULT 0 NOT NULL,
    ghost_summon_extraction_level integer DEFAULT 0 NOT NULL,
    ghost_plant_claymore_level integer DEFAULT 0 NOT NULL,
    ghost_penetrating_shot_level integer DEFAULT 0 NOT NULL,
    ghost_intimidation_level integer DEFAULT 0 NOT NULL,
    ghost_cryogenic_grenade_level integer DEFAULT 0 NOT NULL,
    ghost_flash_underbarrel_level integer DEFAULT 0 NOT NULL,
    created_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
    updated_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL
);


ALTER TABLE public.class_ghost OWNER TO postgres;

--
-- Name: class_ghost_ghost_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.class_ghost_ghost_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.class_ghost_ghost_id_seq OWNER TO postgres;

--
-- Name: class_ghost_ghost_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.class_ghost_ghost_id_seq OWNED BY public.class_ghost.ghost_id;


--
-- Name: class_marine; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.class_marine (
    marine_id integer NOT NULL,
    marine_player_id integer NOT NULL,
    marine_primary_type character varying(16) DEFAULT 'M16A4'::character varying NOT NULL,
    marine_primary_weapon_id integer DEFAULT 0 NOT NULL,
    marine_secondary_type character varying(16) DEFAULT 'CZP10'::character varying NOT NULL,
    marine_secondary_weapon_id integer DEFAULT 0 NOT NULL,
    created_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
    updated_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL
);


ALTER TABLE public.class_marine OWNER TO postgres;

--
-- Name: class_marine_marine_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.class_marine_marine_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.class_marine_marine_id_seq OWNER TO postgres;

--
-- Name: class_marine_marine_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.class_marine_marine_id_seq OWNED BY public.class_marine.marine_id;


--
-- Name: class_medic; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.class_medic (
    medic_id integer NOT NULL,
    medic_player_id integer NOT NULL,
    medic_primary_type character varying(16) DEFAULT 'AUGPARA'::character varying NOT NULL,
    medic_primary_weapon_id integer DEFAULT 0 NOT NULL,
    medic_secondary_type character varying(16) DEFAULT 'CZP10'::character varying NOT NULL,
    medic_secondary_weapon_id integer DEFAULT 0 NOT NULL,
    created_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
    updated_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL
);


ALTER TABLE public.class_medic OWNER TO postgres;

--
-- Name: class_medic_medic_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.class_medic_medic_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.class_medic_medic_id_seq OWNER TO postgres;

--
-- Name: class_medic_medic_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.class_medic_medic_id_seq OWNED BY public.class_medic.medic_id;


--
-- Name: class_psyop; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.class_psyop (
    psyop_id integer NOT NULL,
    psyop_player_id integer NOT NULL,
    psyop_primary_type character varying(16) DEFAULT 'SCARH'::character varying NOT NULL,
    psyop_primary_weapon_id integer DEFAULT 0 NOT NULL,
    psyop_secondary_type character varying(16) DEFAULT 'CZP10'::character varying NOT NULL,
    psyop_secondary_weapon_id integer DEFAULT 0 NOT NULL,
    created_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
    updated_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL
);


ALTER TABLE public.class_psyop OWNER TO postgres;

--
-- Name: class_psyop_psyop_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.class_psyop_psyop_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.class_psyop_psyop_id_seq OWNER TO postgres;

--
-- Name: class_psyop_psyop_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.class_psyop_psyop_id_seq OWNED BY public.class_psyop.psyop_id;


--
-- Name: class_sentinel; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.class_sentinel (
    sentinel_id integer NOT NULL,
    sentinel_player_id integer NOT NULL,
    sentinel_primary_type character varying(16) DEFAULT 'MP5'::character varying NOT NULL,
    sentinel_primary_weapon_id integer DEFAULT 0 NOT NULL,
    sentinel_secondary_type character varying(16) DEFAULT 'CZP10'::character varying NOT NULL,
    sentinel_secondary_weapon_id integer DEFAULT 0 NOT NULL,
    sentinel_heal_level integer DEFAULT 0 NOT NULL,
    sentinel_intimidate_level integer DEFAULT 0 NOT NULL,
    created_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
    updated_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
    sentinel_human_shield_level integer DEFAULT 0 NOT NULL,
    sentinel_deny_entry_level integer DEFAULT 0 NOT NULL,
    sentinel_gadget_shield_level integer DEFAULT 0 NOT NULL,
    sentinel_oblique_kick_level integer DEFAULT 0 NOT NULL
);


ALTER TABLE public.class_sentinel OWNER TO postgres;

--
-- Name: class_sentinel_sentinel_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.class_sentinel_sentinel_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.class_sentinel_sentinel_id_seq OWNER TO postgres;

--
-- Name: class_sentinel_sentinel_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.class_sentinel_sentinel_id_seq OWNED BY public.class_sentinel.sentinel_id;


--
-- Name: class_sniper; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.class_sniper (
    sniper_id integer NOT NULL,
    sniper_player_id integer NOT NULL,
    sniper_primary_type character varying(16) DEFAULT 'PSG1'::character varying NOT NULL,
    sniper_primary_weapon_id integer DEFAULT 0 NOT NULL,
    sniper_secondary_type character varying(16) DEFAULT 'CZP10'::character varying NOT NULL,
    sniper_secondary_weapon_id integer DEFAULT 0 NOT NULL,
    created_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
    updated_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL
);


ALTER TABLE public.class_sniper OWNER TO postgres;

--
-- Name: class_sniper_sniper_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.class_sniper_sniper_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.class_sniper_sniper_id_seq OWNER TO postgres;

--
-- Name: class_sniper_sniper_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.class_sniper_sniper_id_seq OWNED BY public.class_sniper.sniper_id;


--
-- Name: class_support; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.class_support (
    support_id integer NOT NULL,
    support_player_id integer NOT NULL,
    support_primary_type character varying(16) DEFAULT 'MK46'::character varying NOT NULL,
    support_primary_weapon_id integer DEFAULT 0 NOT NULL,
    support_secondary_type character varying(16) DEFAULT 'CZP10'::character varying NOT NULL,
    support_secondary_weapon_id integer DEFAULT 0 NOT NULL,
    created_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
    updated_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL
);


ALTER TABLE public.class_support OWNER TO postgres;

--
-- Name: class_support_support_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.class_support_support_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.class_support_support_id_seq OWNER TO postgres;

--
-- Name: class_support_support_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.class_support_support_id_seq OWNED BY public.class_support.support_id;


--
-- Name: computer_terminal; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.computer_terminal (
    id integer NOT NULL,
    terminal_room_vnum integer NOT NULL,
    terminal_type character varying DEFAULT 'chooser'::character varying NOT NULL,
    terminal_attaches_to character varying,
    terminal_name character varying NOT NULL,
    terminal_description text,
    created_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
    updated_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL
);


ALTER TABLE public.computer_terminal OWNER TO postgres;

--
-- Name: computer_terminal_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.computer_terminal_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.computer_terminal_id_seq OWNER TO postgres;

--
-- Name: computer_terminal_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.computer_terminal_id_seq OWNED BY public.computer_terminal.id;


--
-- Name: extra_description; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.extra_description (
    id integer NOT NULL,
    obj_fk_id integer NOT NULL,
    extra_keyword character varying(64),
    extra_description character varying(1024)
);


ALTER TABLE public.extra_description OWNER TO postgres;

--
-- Name: extra_description_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.extra_description_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.extra_description_id_seq OWNER TO postgres;

--
-- Name: extra_description_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.extra_description_id_seq OWNED BY public.extra_description.id;


--
-- Name: integral_object; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.integral_object (
    object_id integer NOT NULL,
    object_room_vnum integer NOT NULL,
    object_type character varying(16) NOT NULL,
    object_vnum integer NOT NULL,
    created_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
    updated_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL
);


ALTER TABLE public.integral_object OWNER TO postgres;

--
-- Name: integral_object_object_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.integral_object_object_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.integral_object_object_id_seq OWNER TO postgres;

--
-- Name: integral_object_object_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.integral_object_object_id_seq OWNED BY public.integral_object.object_id;


--
-- Name: karma; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.karma (
    karma_id integer NOT NULL,
    karma_player_id integer NOT NULL,
    karma_alignment integer DEFAULT 0 NOT NULL,
    karma_cold_blooded_index integer DEFAULT 0 NOT NULL,
    created_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
    updated_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL
);


ALTER TABLE public.karma OWNER TO postgres;

--
-- Name: karma_karma_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.karma_karma_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.karma_karma_id_seq OWNER TO postgres;

--
-- Name: karma_karma_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.karma_karma_id_seq OWNED BY public.karma.karma_id;


--
-- Name: mini_game; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.mini_game (
    game_id integer NOT NULL,
    game_type character varying(16) NOT NULL,
    game_vnum integer NOT NULL,
    game_name character varying(32),
    game_action text DEFAULT 'unlock'::text NOT NULL,
    game_room_vnum integer NOT NULL,
    game_order integer DEFAULT 0 NOT NULL,
    created_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
    updated_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL
);


ALTER TABLE public.mini_game OWNER TO postgres;

--
-- Name: mini_game_game_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.mini_game_game_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.mini_game_game_id_seq OWNER TO postgres;

--
-- Name: mini_game_game_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.mini_game_game_id_seq OWNED BY public.mini_game.game_id;


--
-- Name: mini_gunner_sentinel; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.mini_gunner_sentinel (
    id integer NOT NULL,
    mgs_mob_vnum integer DEFAULT '-1'::integer NOT NULL,
    mgs_face_direction character varying(1) DEFAULT 'N'::character varying NOT NULL,
    mgs_room_vnum integer NOT NULL
);


ALTER TABLE public.mini_gunner_sentinel OWNER TO postgres;

--
-- Name: mini_gunner_sentinel_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.mini_gunner_sentinel_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.mini_gunner_sentinel_id_seq OWNER TO postgres;

--
-- Name: mini_gunner_sentinel_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.mini_gunner_sentinel_id_seq OWNED BY public.mini_gunner_sentinel.id;


--
-- Name: mob_zone; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.mob_zone (
    id integer NOT NULL,
    zone_id integer NOT NULL,
    mob_id integer NOT NULL,
    room_id integer NOT NULL,
    max_existing integer
);


ALTER TABLE public.mob_zone OWNER TO postgres;

--
-- Name: mob_zone_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.mob_zone_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.mob_zone_id_seq OWNER TO postgres;

--
-- Name: mob_zone_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.mob_zone_id_seq OWNED BY public.mob_zone.id;


--
-- Name: mobile; Type: TABLE; Schema: public; Owner: postgres
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
    mob_class integer NOT NULL,
    mob_special_extended_type integer DEFAULT 0 NOT NULL
);


ALTER TABLE public.mobile OWNER TO postgres;

--
-- Name: mobile_mob_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.mobile_mob_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.mobile_mob_id_seq OWNER TO postgres;

--
-- Name: mobile_mob_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.mobile_mob_id_seq OWNED BY public.mobile.mob_id;


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
    updated_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL
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
    created_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
    updated_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
    armor_file character varying(2048),
    armor_str_type public.armor_type_t NOT NULL,
    armor_manufacturer character varying(256) NOT NULL,
    armor_name character varying(256) NOT NULL,
    armor_vnum integer NOT NULL,
    armor_rarity public.rarity_t DEFAULT 'COMMON'::public.rarity_t NOT NULL,
    armor_type integer NOT NULL,
    armor_hp integer DEFAULT 0 NOT NULL,
    armor_classification character varying(8) DEFAULT 'BASIC'::character varying NOT NULL,
    armor_worth integer DEFAULT 0 NOT NULL
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
-- Name: player; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.player (
    id integer NOT NULL,
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
    player_preferences character varying(2048)
);


ALTER TABLE public.player OWNER TO postgres;

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

CREATE SEQUENCE public.player_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


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

CREATE TABLE public.room (
    room_number integer NOT NULL,
    zone integer NOT NULL,
    sector_type integer NOT NULL,
    name character varying(256) NOT NULL,
    description text NOT NULL,
    ex_keyword character varying(256),
    ex_description text,
    light integer,
    room_flag integer NOT NULL,
    id integer NOT NULL
);


ALTER TABLE public.room OWNER TO postgres;

--
-- Name: room_direction_data; Type: TABLE; Schema: public; Owner: postgres
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


ALTER TABLE public.room_direction_data OWNER TO postgres;

--
-- Name: room_direction_data_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.room_direction_data_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.room_direction_data_id_seq OWNER TO postgres;

--
-- Name: room_direction_data_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.room_direction_data_id_seq OWNED BY public.room_direction_data.id;


--
-- Name: room_room_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.room_room_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.room_room_id_seq OWNER TO postgres;

--
-- Name: room_room_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.room_room_id_seq OWNED BY public.room.id;


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
    shop_room_vnum integer NOT NULL
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
-- Name: skill_trees; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.skill_trees (
    id integer NOT NULL,
    sktree_class integer NOT NULL,
    sktree_skill_name character varying(32) NOT NULL,
    sktree_skill_description text,
    sktree_skill_level_unlock integer NOT NULL,
    sktree_skill_parent_id integer,
    sktree_skill_order integer DEFAULT 0 NOT NULL
);


ALTER TABLE public.skill_trees OWNER TO postgres;

--
-- Name: skill_trees_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.skill_trees_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.skill_trees_id_seq OWNER TO postgres;

--
-- Name: skill_trees_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.skill_trees_id_seq OWNED BY public.skill_trees.id;


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
-- Name: unused_snipers; Type: VIEW; Schema: public; Owner: postgres
--

CREATE VIEW public.unused_snipers AS
 SELECT class_sniper.sniper_id,
    class_sniper.sniper_player_id
   FROM public.class_sniper
  WHERE (NOT (class_sniper.sniper_player_id IN ( SELECT player.id
           FROM (public.player
             JOIN public.class_sniper cs ON ((cs.sniper_player_id = player.id))))));


ALTER TABLE public.unused_snipers OWNER TO postgres;

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
-- Name: zone; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.zone (
    id integer NOT NULL,
    zone_virtual_number integer NOT NULL,
    zone_start integer NOT NULL,
    zone_end integer NOT NULL,
    zone_name character varying(64) NOT NULL,
    lifespan integer NOT NULL,
    reset_mode integer NOT NULL
);


ALTER TABLE public.zone OWNER TO postgres;

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
-- Name: zone_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.zone_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.zone_id_seq OWNER TO postgres;

--
-- Name: zone_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.zone_id_seq OWNED BY public.zone.id;


--
-- Name: affected_type id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.affected_type ALTER COLUMN id SET DEFAULT nextval('public.affected_type_id_seq'::regclass);


--
-- Name: camera_feed feed_id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.camera_feed ALTER COLUMN feed_id SET DEFAULT nextval('public.camera_feed_feed_id_seq'::regclass);


--
-- Name: class_contagion contagion_id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.class_contagion ALTER COLUMN contagion_id SET DEFAULT nextval('public.class_contagion_contagion_id_seq'::regclass);


--
-- Name: class_engineer engineer_id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.class_engineer ALTER COLUMN engineer_id SET DEFAULT nextval('public.class_engineer_engineer_id_seq'::regclass);


--
-- Name: class_ghost ghost_id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.class_ghost ALTER COLUMN ghost_id SET DEFAULT nextval('public.class_ghost_ghost_id_seq'::regclass);


--
-- Name: class_marine marine_id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.class_marine ALTER COLUMN marine_id SET DEFAULT nextval('public.class_marine_marine_id_seq'::regclass);


--
-- Name: class_medic medic_id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.class_medic ALTER COLUMN medic_id SET DEFAULT nextval('public.class_medic_medic_id_seq'::regclass);


--
-- Name: class_psyop psyop_id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.class_psyop ALTER COLUMN psyop_id SET DEFAULT nextval('public.class_psyop_psyop_id_seq'::regclass);


--
-- Name: class_sentinel sentinel_id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.class_sentinel ALTER COLUMN sentinel_id SET DEFAULT nextval('public.class_sentinel_sentinel_id_seq'::regclass);


--
-- Name: class_sniper sniper_id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.class_sniper ALTER COLUMN sniper_id SET DEFAULT nextval('public.class_sniper_sniper_id_seq'::regclass);


--
-- Name: class_support support_id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.class_support ALTER COLUMN support_id SET DEFAULT nextval('public.class_support_support_id_seq'::regclass);


--
-- Name: computer_terminal id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.computer_terminal ALTER COLUMN id SET DEFAULT nextval('public.computer_terminal_id_seq'::regclass);


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

ALTER TABLE ONLY public.room ALTER COLUMN id SET DEFAULT nextval('public.room_room_id_seq'::regclass);


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
-- Name: skill_trees id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.skill_trees ALTER COLUMN id SET DEFAULT nextval('public.skill_trees_id_seq'::regclass);


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
-- Data for Name: class_contagion; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.class_contagion (contagion_id, contagion_player_id, contagion_primary_type, contagion_primary_weapon_id, contagion_secondary_type, contagion_secondary_weapon_id, created_at, updated_at) FROM stdin;
\.


--
-- Data for Name: class_engineer; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.class_engineer (engineer_id, engineer_player_id, engineer_primary_type, engineer_primary_weapon_id, engineer_secondary_type, engineer_secondary_weapon_id, created_at, updated_at) FROM stdin;
\.


--
-- Data for Name: class_ghost; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.class_ghost (ghost_id, ghost_player_id, ghost_drone_scan_level, ghost_stealth_level, ghost_xray_shot_level, ghost_feign_death_level, ghost_summon_extraction_level, ghost_plant_claymore_level, ghost_penetrating_shot_level, ghost_intimidation_level, ghost_cryogenic_grenade_level, ghost_flash_underbarrel_level, created_at, updated_at) FROM stdin;
\.


--
-- Data for Name: class_marine; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.class_marine (marine_id, marine_player_id, marine_primary_type, marine_primary_weapon_id, marine_secondary_type, marine_secondary_weapon_id, created_at, updated_at) FROM stdin;
1	59	M4	0	czp10	0	2020-09-01 04:22:44.344128	2020-09-01 04:22:44.344128
2	60	M4	0	czp10	0	2020-09-01 04:35:13.694349	2020-09-01 04:35:13.694349
3	61	M4	47	czp10	48	2020-09-01 04:39:39.784779	2020-09-01 04:39:39.784779
4	63	M16A4	49	czp10	50	2020-09-01 04:54:05.135455	2020-09-01 04:54:05.135455
\.


--
-- Data for Name: class_medic; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.class_medic (medic_id, medic_player_id, medic_primary_type, medic_primary_weapon_id, medic_secondary_type, medic_secondary_weapon_id, created_at, updated_at) FROM stdin;
54	69	AUGPARA	287	czp10	288	2020-09-03 05:20:42.745382	2020-09-03 05:20:42.745382
55	69	AUGPARA	291	czp10	292	2020-09-03 05:22:48.352831	2020-09-03 05:22:48.352831
56	69	AUGPARA	295	czp10	296	2020-09-03 05:26:52.425409	2020-09-03 05:26:52.425409
57	69	AUGPARA	299	czp10	300	2020-09-03 05:29:51.202643	2020-09-03 05:29:51.202643
58	69	AUGPARA	303	czp10	304	2020-09-03 05:33:00.905435	2020-09-03 05:33:00.905435
59	69	AUGPARA	307	czp10	308	2020-09-03 05:34:36.86994	2020-09-03 05:34:36.86994
60	69	AUGPARA	311	czp10	312	2020-09-03 05:44:54.306257	2020-09-03 05:44:54.306257
61	69	AUGPARA	315	czp10	316	2020-09-03 07:06:46.417031	2020-09-03 07:06:46.417031
16	80	TAR21	141	czp10	142	2020-09-02 22:25:15.168229	2020-09-02 22:25:15.168229
17	69	AUGPARA	145	czp10	146	2020-09-02 22:26:14.955282	2020-09-02 22:26:14.955282
18	69	AUGPARA	149	czp10	150	2020-09-02 23:00:57.702112	2020-09-02 23:00:57.702112
19	69	AUGPARA	153	czp10	154	2020-09-02 23:39:44.213824	2020-09-02 23:39:44.213824
20	69	AUGPARA	157	czp10	158	2020-09-02 23:47:29.824861	2020-09-02 23:47:29.824861
21	69	AUGPARA	161	czp10	162	2020-09-02 23:49:23.948192	2020-09-02 23:49:23.948192
22	69	AUGPARA	165	czp10	166	2020-09-03 00:43:24.672217	2020-09-03 00:43:24.672217
23	69	AUGPARA	171	czp10	172	2020-09-03 00:48:00.130331	2020-09-03 00:48:00.130331
24	69	AUGPARA	175	czp10	176	2020-09-03 01:49:32.438779	2020-09-03 01:49:32.438779
25	69	AUGPARA	179	czp10	180	2020-09-03 02:16:33.784196	2020-09-03 02:16:33.784196
26	69	AUGPARA	183	czp10	184	2020-09-03 02:16:50.776806	2020-09-03 02:16:50.776806
27	69	AUGPARA	187	czp10	188	2020-09-03 02:18:42.433401	2020-09-03 02:18:42.433401
28	69	AUGPARA	191	czp10	192	2020-09-03 02:49:28.25856	2020-09-03 02:49:28.25856
29	69	AUGPARA	195	czp10	196	2020-09-03 02:49:51.659994	2020-09-03 02:49:51.659994
30	69	AUGPARA	199	czp10	200	2020-09-03 02:55:45.914113	2020-09-03 02:55:45.914113
31	69	AUGPARA	203	czp10	204	2020-09-03 03:00:28.230903	2020-09-03 03:00:28.230903
32	69	AUGPARA	207	czp10	208	2020-09-03 03:01:47.010484	2020-09-03 03:01:47.010484
33	69	AUGPARA	211	czp10	212	2020-09-03 03:13:13.600917	2020-09-03 03:13:13.600917
34	69	AUGPARA	215	czp10	216	2020-09-03 03:15:00.798975	2020-09-03 03:15:00.798975
35	69	AUGPARA	219	czp10	220	2020-09-03 03:19:29.323045	2020-09-03 03:19:29.323045
36	69	AUGPARA	223	czp10	224	2020-09-03 03:41:14.767797	2020-09-03 03:41:14.767797
37	69	AUGPARA	227	czp10	228	2020-09-03 04:04:21.849327	2020-09-03 04:04:21.849327
38	69	AUGPARA	231	czp10	232	2020-09-03 04:21:50.803284	2020-09-03 04:21:50.803284
39	69	AUGPARA	235	czp10	236	2020-09-03 04:27:19.355939	2020-09-03 04:27:19.355939
40	69	AUGPARA	239	czp10	240	2020-09-03 04:28:51.561427	2020-09-03 04:28:51.561427
41	69	AUGPARA	243	czp10	244	2020-09-03 04:35:12.142021	2020-09-03 04:35:12.142021
42	69	AUGPARA	247	czp10	248	2020-09-03 04:35:27.759656	2020-09-03 04:35:27.759656
43	69	AUGPARA	251	czp10	252	2020-09-03 04:37:11.107525	2020-09-03 04:37:11.107525
44	69	AUGPARA	255	czp10	256	2020-09-03 04:39:52.192024	2020-09-03 04:39:52.192024
45	69	AUGPARA	258	czp10	259	2020-09-03 04:42:32.676562	2020-09-03 04:42:32.676562
46	69	AUGPARA	261	czp10	262	2020-09-03 04:44:56.068329	2020-09-03 04:44:56.068329
47	69	AUGPARA	264	czp10	265	2020-09-03 04:46:48.041775	2020-09-03 04:46:48.041775
48	69	AUGPARA	267	czp10	268	2020-09-03 04:49:59.644708	2020-09-03 04:49:59.644708
49	69	AUGPARA	270	czp10	271	2020-09-03 04:52:07.810497	2020-09-03 04:52:07.810497
50	69	AUGPARA	273	czp10	274	2020-09-03 04:53:29.406961	2020-09-03 04:53:29.406961
51	69	AUGPARA	276	czp10	277	2020-09-03 04:58:30.007906	2020-09-03 04:58:30.007906
52	69	AUGPARA	279	czp10	280	2020-09-03 05:09:49.114654	2020-09-03 05:09:49.114654
53	69	AUGPARA	283	czp10	284	2020-09-03 05:14:56.707976	2020-09-03 05:14:56.707976
62	69	AUGPARA	319	czp10	320	2020-09-03 07:13:37.72439	2020-09-03 07:13:37.72439
63	69	AUGPARA	323	czp10	324	2020-09-03 07:47:43.444136	2020-09-03 07:47:43.444136
64	69	AUGPARA	327	czp10	328	2020-09-05 01:34:31.751342	2020-09-05 01:34:31.751342
65	69	AUGPARA	331	czp10	332	2020-09-05 01:48:40.684247	2020-09-05 01:48:40.684247
66	69	AUGPARA	335	czp10	336	2020-09-05 02:17:06.217859	2020-09-05 02:17:06.217859
67	69	AUGPARA	339	czp10	340	2020-09-05 02:48:10.694155	2020-09-05 02:48:10.694155
68	69	AUGPARA	343	czp10	344	2020-09-05 02:53:12.837072	2020-09-05 02:53:12.837072
69	69	AUGPARA	347	czp10	348	2020-09-05 03:15:09.105925	2020-09-05 03:15:09.105925
70	69	AUGPARA	351	czp10	352	2020-09-05 03:18:24.317588	2020-09-05 03:18:24.317588
71	69	AUGPARA	355	czp10	356	2020-09-05 03:21:08.89394	2020-09-05 03:21:08.89394
72	69	AUGPARA	359	czp10	360	2020-09-05 03:21:43.946157	2020-09-05 03:21:43.946157
73	69	AUGPARA	363	czp10	364	2020-09-05 03:22:21.298189	2020-09-05 03:22:21.298189
74	69	AUGPARA	367	czp10	368	2020-09-06 00:53:49.720264	2020-09-06 00:53:49.720264
75	69	AUGPARA	371	czp10	372	2020-09-06 00:59:59.433662	2020-09-06 00:59:59.433662
76	69	AUGPARA	375	czp10	376	2020-09-06 01:06:13.180592	2020-09-06 01:06:13.180592
77	69	AUGPARA	379	czp10	380	2020-09-06 01:13:48.949652	2020-09-06 01:13:48.949652
78	69	AUGPARA	383	czp10	384	2020-09-06 01:18:55.45322	2020-09-06 01:18:55.45322
79	69	AUGPARA	387	czp10	388	2020-09-06 01:22:43.633244	2020-09-06 01:22:43.633244
80	69	AUGPARA	391	czp10	392	2020-09-06 01:27:02.140417	2020-09-06 01:27:02.140417
81	69	AUGPARA	395	czp10	396	2020-09-06 01:29:03.672529	2020-09-06 01:29:03.672529
82	69	AUGPARA	399	czp10	400	2020-09-06 02:15:03.934472	2020-09-06 02:15:03.934472
83	69	AUGPARA	403	czp10	404	2020-09-06 02:25:01.069566	2020-09-06 02:25:01.069566
84	69	AUGPARA	407	czp10	408	2020-09-06 02:32:29.287641	2020-09-06 02:32:29.287641
85	69	AUGPARA	411	czp10	412	2020-09-06 02:42:52.482741	2020-09-06 02:42:52.482741
86	69	AUGPARA	415	czp10	416	2020-09-06 03:38:25.719603	2020-09-06 03:38:25.719603
87	69	AUGPARA	419	czp10	420	2020-09-06 03:43:58.030877	2020-09-06 03:43:58.030877
88	69	AUGPARA	423	czp10	424	2020-09-06 03:44:52.36752	2020-09-06 03:44:52.36752
89	69	AUGPARA	427	czp10	428	2020-09-07 02:20:18.239707	2020-09-07 02:20:18.239707
90	69	AUGPARA	431	czp10	432	2020-09-07 02:42:24.011386	2020-09-07 02:42:24.011386
91	69	AUGPARA	435	czp10	436	2020-09-07 03:10:17.673924	2020-09-07 03:10:17.673924
92	69	AUGPARA	439	czp10	440	2020-09-07 03:17:23.090456	2020-09-07 03:17:23.090456
93	69	AUGPARA	443	czp10	444	2020-09-07 04:16:38.26522	2020-09-07 04:16:38.26522
94	69	AUGPARA	447	czp10	448	2020-09-07 04:21:27.94914	2020-09-07 04:21:27.94914
95	69	AUGPARA	451	czp10	452	2020-09-07 04:43:58.753198	2020-09-07 04:43:58.753198
96	69	AUGPARA	455	czp10	456	2020-09-07 05:00:05.610044	2020-09-07 05:00:05.610044
97	69	AUGPARA	459	czp10	460	2020-09-07 05:00:29.328013	2020-09-07 05:00:29.328013
98	69	AUGPARA	463	czp10	464	2020-09-07 05:04:50.301887	2020-09-07 05:04:50.301887
99	69	AUGPARA	467	czp10	468	2020-09-07 05:12:51.405282	2020-09-07 05:12:51.405282
100	69	AUGPARA	471	czp10	472	2020-09-07 05:40:07.617977	2020-09-07 05:40:07.617977
101	69	AUGPARA	475	czp10	476	2020-09-07 06:18:01.076134	2020-09-07 06:18:01.076134
102	69	AUGPARA	479	czp10	480	2020-09-07 06:43:27.048576	2020-09-07 06:43:27.048576
103	69	AUGPARA	483	czp10	484	2020-09-07 07:30:53.464929	2020-09-07 07:30:53.464929
104	69	AUGPARA	487	czp10	488	2020-09-08 22:52:14.146172	2020-09-08 22:52:14.146172
105	69	AUGPARA	491	czp10	492	2020-09-08 23:14:31.135538	2020-09-08 23:14:31.135538
106	69	AUGPARA	495	czp10	496	2020-09-08 23:16:19.129943	2020-09-08 23:16:19.129943
107	69	AUGPARA	499	czp10	500	2020-09-09 00:39:35.628874	2020-09-09 00:39:35.628874
108	69	AUGPARA	503	czp10	504	2020-09-09 00:54:15.705123	2020-09-09 00:54:15.705123
109	69	AUGPARA	507	czp10	508	2020-09-09 00:58:27.936958	2020-09-09 00:58:27.936958
110	69	AUGPARA	511	czp10	512	2020-09-09 01:05:29.678238	2020-09-09 01:05:29.678238
111	69	AUGPARA	515	czp10	516	2020-09-09 22:41:51.014323	2020-09-09 22:41:51.014323
112	69	AUGPARA	519	czp10	520	2020-09-09 22:48:33.129544	2020-09-09 22:48:33.129544
113	69	AUGPARA	523	czp10	524	2020-09-09 22:58:44.371576	2020-09-09 22:58:44.371576
114	69	AUGPARA	527	czp10	528	2020-09-09 22:59:21.376011	2020-09-09 22:59:21.376011
115	69	AUGPARA	531	czp10	532	2020-09-09 23:00:31.755035	2020-09-09 23:00:31.755035
116	69	AUGPARA	535	czp10	536	2020-09-09 23:01:13.115866	2020-09-09 23:01:13.115866
117	69	AUGPARA	539	czp10	540	2020-09-09 23:03:20.990052	2020-09-09 23:03:20.990052
118	69	AUGPARA	543	czp10	544	2020-09-09 23:05:11.929853	2020-09-09 23:05:11.929853
119	69	AUGPARA	547	czp10	548	2020-09-09 23:10:00.888293	2020-09-09 23:10:00.888293
120	69	AUGPARA	551	czp10	552	2020-09-09 23:12:32.713789	2020-09-09 23:12:32.713789
121	69	AUGPARA	555	czp10	556	2020-09-10 19:53:49.265103	2020-09-10 19:53:49.265103
122	69	AUGPARA	559	czp10	560	2020-09-10 19:55:31.921193	2020-09-10 19:55:31.921193
123	69	AUGPARA	563	czp10	564	2020-09-10 19:56:45.867173	2020-09-10 19:56:45.867173
124	69	AUGPARA	567	czp10	568	2020-09-10 20:02:54.664199	2020-09-10 20:02:54.664199
125	69	AUGPARA	571	czp10	572	2020-09-10 20:04:04.843402	2020-09-10 20:04:04.843402
126	69	AUGPARA	575	czp10	576	2020-09-10 20:05:52.483049	2020-09-10 20:05:52.483049
127	69	AUGPARA	579	czp10	580	2020-09-10 20:19:56.140789	2020-09-10 20:19:56.140789
128	69	AUGPARA	583	czp10	584	2020-09-10 20:20:58.569447	2020-09-10 20:20:58.569447
129	69	AUGPARA	587	czp10	588	2020-09-10 20:24:06.13069	2020-09-10 20:24:06.13069
130	69	AUGPARA	591	czp10	592	2020-09-10 20:25:00.742338	2020-09-10 20:25:00.742338
131	69	AUGPARA	595	czp10	596	2020-09-10 20:25:46.995031	2020-09-10 20:25:46.995031
132	69	AUGPARA	599	czp10	600	2020-09-10 20:26:43.473399	2020-09-10 20:26:43.473399
133	69	AUGPARA	603	czp10	604	2020-09-10 20:27:14.383561	2020-09-10 20:27:14.383561
134	69	AUGPARA	607	czp10	608	2020-09-10 20:29:07.056829	2020-09-10 20:29:07.056829
135	69	AUGPARA	611	czp10	612	2020-09-10 20:30:13.585811	2020-09-10 20:30:13.585811
136	69	AUGPARA	615	czp10	616	2020-09-10 20:43:20.065442	2020-09-10 20:43:20.065442
137	69	AUGPARA	619	czp10	620	2020-09-10 20:51:30.144314	2020-09-10 20:51:30.144314
138	69	AUGPARA	623	czp10	624	2020-09-11 19:24:42.206947	2020-09-11 19:24:42.206947
139	69	AUGPARA	629	czp10	630	2020-09-11 19:53:15.964612	2020-09-11 19:53:15.964612
140	69	AUGPARA	633	czp10	634	2020-09-11 19:54:02.150534	2020-09-11 19:54:02.150534
141	69	AUGPARA	637	czp10	638	2020-09-11 19:56:14.458217	2020-09-11 19:56:14.458217
142	69	AUGPARA	641	czp10	642	2020-09-11 20:03:08.315601	2020-09-11 20:03:08.315601
143	69	AUGPARA	645	czp10	646	2020-09-12 01:25:33.974911	2020-09-12 01:25:33.974911
144	69	AUGPARA	649	czp10	650	2020-09-12 01:38:42.995981	2020-09-12 01:38:42.995981
145	69	AUGPARA	653	czp10	654	2020-09-12 01:41:43.189977	2020-09-12 01:41:43.189977
146	69	AUGPARA	657	czp10	658	2020-09-12 01:48:43.390941	2020-09-12 01:48:43.390941
147	69	AUGPARA	661	czp10	662	2020-09-12 01:53:16.715115	2020-09-12 01:53:16.715115
148	69	AUGPARA	667	czp10	668	2020-09-12 02:41:36.042528	2020-09-12 02:41:36.042528
149	69	AUGPARA	671	czp10	672	2020-09-12 02:44:05.026367	2020-09-12 02:44:05.026367
150	69	AUGPARA	675	czp10	676	2020-09-12 05:11:58.361918	2020-09-12 05:11:58.361918
151	69	AUGPARA	679	czp10	680	2020-09-12 06:48:20.025123	2020-09-12 06:48:20.025123
152	69	AUGPARA	683	czp10	684	2020-09-12 07:48:52.254048	2020-09-12 07:48:52.254048
153	69	AUGPARA	687	czp10	688	2020-09-12 07:53:16.828039	2020-09-12 07:53:16.828039
154	69	AUGPARA	691	czp10	692	2020-09-12 07:53:50.379966	2020-09-12 07:53:50.379966
155	69	AUGPARA	695	czp10	696	2020-09-12 08:02:36.077552	2020-09-12 08:02:36.077552
156	69	AUGPARA	699	czp10	700	2020-09-12 08:13:51.70043	2020-09-12 08:13:51.70043
157	69	AUGPARA	703	czp10	704	2020-09-12 08:19:55.321856	2020-09-12 08:19:55.321856
158	69	AUGPARA	707	czp10	708	2020-09-12 08:23:24.242554	2020-09-12 08:23:24.242554
159	69	AUGPARA	711	czp10	712	2020-09-12 08:29:07.379433	2020-09-12 08:29:07.379433
160	69	AUGPARA	715	czp10	716	2020-09-12 08:38:38.571783	2020-09-12 08:38:38.571783
161	69	AUGPARA	719	czp10	720	2020-09-12 08:39:23.449445	2020-09-12 08:39:23.449445
162	69	AUGPARA	723	czp10	724	2020-09-12 08:45:59.364384	2020-09-12 08:45:59.364384
163	69	AUGPARA	727	czp10	728	2020-09-12 08:53:57.109185	2020-09-12 08:53:57.109185
164	69	AUGPARA	731	czp10	732	2020-09-12 08:56:05.975203	2020-09-12 08:56:05.975203
165	69	AUGPARA	735	czp10	736	2020-09-12 08:56:53.194745	2020-09-12 08:56:53.194745
166	69	AUGPARA	739	czp10	740	2020-09-12 09:08:08.042814	2020-09-12 09:08:08.042814
167	69	AUGPARA	743	czp10	744	2020-09-12 09:08:56.970737	2020-09-12 09:08:56.970737
168	69	AUGPARA	747	czp10	748	2020-09-12 09:13:07.185368	2020-09-12 09:13:07.185368
169	69	AUGPARA	751	czp10	752	2020-09-12 09:33:08.247547	2020-09-12 09:33:08.247547
170	69	AUGPARA	755	czp10	756	2020-09-12 09:35:39.864802	2020-09-12 09:35:39.864802
171	69	AUGPARA	759	czp10	760	2020-09-12 10:13:30.681011	2020-09-12 10:13:30.681011
172	69	AUGPARA	763	czp10	764	2020-09-12 10:41:58.447737	2020-09-12 10:41:58.447737
173	69	AUGPARA	767	czp10	768	2020-09-12 10:45:42.119398	2020-09-12 10:45:42.119398
174	69	AUGPARA	771	czp10	772	2020-09-12 11:14:30.296099	2020-09-12 11:14:30.296099
175	69	AUGPARA	775	czp10	776	2020-09-12 11:35:55.080116	2020-09-12 11:35:55.080116
176	69	AUGPARA	779	czp10	780	2020-09-12 11:38:41.706097	2020-09-12 11:38:41.706097
177	69	AUGPARA	783	czp10	784	2020-09-12 11:47:24.478832	2020-09-12 11:47:24.478832
178	69	AUGPARA	787	czp10	788	2020-09-12 11:49:07.601494	2020-09-12 11:49:07.601494
179	69	AUGPARA	791	czp10	792	2020-09-12 11:52:27.030329	2020-09-12 11:52:27.030329
180	69	AUGPARA	795	czp10	796	2020-09-12 12:14:13.306733	2020-09-12 12:14:13.306733
181	69	AUGPARA	799	czp10	800	2020-09-12 14:04:12.434103	2020-09-12 14:04:12.434103
182	69	AUGPARA	803	czp10	804	2020-09-12 14:14:16.461195	2020-09-12 14:14:16.461195
183	69	AUGPARA	807	czp10	808	2020-09-12 14:15:34.815867	2020-09-12 14:15:34.815867
184	69	AUGPARA	811	czp10	812	2020-09-12 14:20:13.357445	2020-09-12 14:20:13.357445
185	69	AUGPARA	815	czp10	816	2020-09-12 14:22:26.865106	2020-09-12 14:22:26.865106
186	69	AUGPARA	819	czp10	820	2020-09-12 14:55:22.305526	2020-09-12 14:55:22.305526
187	69	AUGPARA	823	czp10	824	2020-09-12 16:44:38.918748	2020-09-12 16:44:38.918748
188	69	AUGPARA	827	czp10	828	2020-09-12 17:55:32.76167	2020-09-12 17:55:32.76167
189	69	AUGPARA	831	czp10	832	2020-09-12 19:20:46.515213	2020-09-12 19:20:46.515213
190	69	AUGPARA	835	czp10	836	2020-09-15 23:25:40.408754	2020-09-15 23:25:40.408754
191	69	AUGPARA	839	czp10	840	2020-09-15 23:43:10.835959	2020-09-15 23:43:10.835959
192	69	AUGPARA	843	czp10	844	2020-09-15 23:43:25.045138	2020-09-15 23:43:25.045138
193	69	AUGPARA	847	czp10	848	2020-09-15 23:50:46.429559	2020-09-15 23:50:46.429559
194	69	AUGPARA	851	czp10	852	2020-09-15 23:55:25.05416	2020-09-15 23:55:25.05416
195	69	AUGPARA	855	czp10	856	2020-09-15 23:55:46.080409	2020-09-15 23:55:46.080409
196	69	AUGPARA	859	czp10	860	2020-09-16 00:06:11.475296	2020-09-16 00:06:11.475296
197	69	AUGPARA	863	czp10	864	2020-09-16 00:07:19.095865	2020-09-16 00:07:19.095865
198	69	AUGPARA	867	czp10	868	2020-09-16 22:20:37.737896	2020-09-16 22:20:37.737896
199	69	AUGPARA	871	czp10	872	2020-09-16 22:54:23.071537	2020-09-16 22:54:23.071537
200	69	AUGPARA	875	czp10	876	2020-09-16 22:59:03.825326	2020-09-16 22:59:03.825326
201	69	AUGPARA	879	czp10	880	2020-09-16 23:46:19.382198	2020-09-16 23:46:19.382198
202	69	AUGPARA	883	czp10	884	2020-09-17 03:02:05.12369	2020-09-17 03:02:05.12369
203	69	AUGPARA	887	czp10	888	2020-09-17 04:34:58.539956	2020-09-17 04:34:58.539956
204	69	AUGPARA	891	czp10	892	2020-09-17 17:39:32.736269	2020-09-17 17:39:32.736269
205	69	AUGPARA	895	czp10	896	2020-09-17 17:52:41.680068	2020-09-17 17:52:41.680068
206	69	AUGPARA	899	czp10	900	2020-09-17 18:11:53.768807	2020-09-17 18:11:53.768807
207	69	AUGPARA	903	czp10	904	2020-09-17 18:14:04.851155	2020-09-17 18:14:04.851155
208	69	AUGPARA	907	czp10	908	2020-09-17 18:15:36.990675	2020-09-17 18:15:36.990675
209	69	AUGPARA	911	czp10	912	2020-09-17 18:17:49.656398	2020-09-17 18:17:49.656398
210	69	AUGPARA	915	czp10	916	2020-09-17 18:18:52.026511	2020-09-17 18:18:52.026511
211	69	AUGPARA	919	czp10	920	2020-09-17 18:20:51.366528	2020-09-17 18:20:51.366528
212	69	AUGPARA	923	czp10	924	2020-09-17 18:22:05.879068	2020-09-17 18:22:05.879068
213	69	AUGPARA	927	czp10	928	2020-09-17 18:22:49.381459	2020-09-17 18:22:49.381459
214	69	AUGPARA	931	czp10	932	2020-09-17 18:23:21.099903	2020-09-17 18:23:21.099903
215	69	AUGPARA	935	czp10	936	2020-09-17 18:30:25.890197	2020-09-17 18:30:25.890197
216	69	AUGPARA	939	czp10	940	2020-09-20 05:11:10.566677	2020-09-20 05:11:10.566677
217	69	AUGPARA	943	czp10	944	2020-09-20 05:17:09.111686	2020-09-20 05:17:09.111686
218	69	AUGPARA	947	czp10	948	2020-09-20 20:49:30.380295	2020-09-20 20:49:30.380295
219	69	AUGPARA	951	czp10	952	2020-09-24 18:11:48.379382	2020-09-24 18:11:48.379382
220	69	AUGPARA	955	czp10	956	2020-09-24 18:18:16.327291	2020-09-24 18:18:16.327291
221	69	AUGPARA	959	czp10	960	2020-09-24 18:22:15.958141	2020-09-24 18:22:15.958141
222	69	AUGPARA	963	czp10	964	2020-09-24 18:23:32.612726	2020-09-24 18:23:32.612726
223	69	AUGPARA	967	czp10	968	2020-09-24 18:30:04.985962	2020-09-24 18:30:04.985962
224	69	AUGPARA	971	czp10	972	2020-09-25 18:25:27.564385	2020-09-25 18:25:27.564385
225	69	AUGPARA	975	czp10	976	2020-09-25 18:27:37.447297	2020-09-25 18:27:37.447297
226	69	AUGPARA	979	czp10	980	2020-09-25 18:29:13.144664	2020-09-25 18:29:13.144664
227	69	AUGPARA	983	czp10	984	2020-09-25 18:30:59.384135	2020-09-25 18:30:59.384135
228	69	AUGPARA	987	czp10	988	2020-09-25 18:32:20.472259	2020-09-25 18:32:20.472259
229	69	AUGPARA	991	czp10	992	2020-09-25 18:37:40.207851	2020-09-25 18:37:40.207851
230	69	AUGPARA	995	czp10	996	2020-09-25 18:41:02.103062	2020-09-25 18:41:02.103062
231	69	AUGPARA	999	czp10	1000	2020-09-25 18:44:19.906644	2020-09-25 18:44:19.906644
232	69	AUGPARA	1003	czp10	1004	2020-09-25 18:51:36.274156	2020-09-25 18:51:36.274156
233	69	AUGPARA	1007	czp10	1008	2020-09-25 18:56:58.101655	2020-09-25 18:56:58.101655
234	69	AUGPARA	1011	czp10	1012	2020-09-25 19:03:37.425464	2020-09-25 19:03:37.425464
235	69	AUGPARA	1015	czp10	1016	2020-09-25 19:06:29.71885	2020-09-25 19:06:29.71885
236	69	AUGPARA	1019	czp10	1020	2020-09-25 19:07:37.122841	2020-09-25 19:07:37.122841
237	69	AUGPARA	1023	czp10	1024	2020-09-25 19:10:54.034435	2020-09-25 19:10:54.034435
238	69	AUGPARA	1027	czp10	1028	2020-09-25 19:12:12.355887	2020-09-25 19:12:12.355887
239	69	AUGPARA	1031	czp10	1032	2020-09-25 19:14:19.246858	2020-09-25 19:14:19.246858
240	69	AUGPARA	1035	czp10	1036	2020-09-25 19:16:51.122558	2020-09-25 19:16:51.122558
241	69	AUGPARA	1039	czp10	1040	2020-09-25 19:20:07.059428	2020-09-25 19:20:07.059428
242	69	AUGPARA	1043	czp10	1044	2020-09-25 21:51:11.904302	2020-09-25 21:51:11.904302
243	69	AUGPARA	1047	czp10	1048	2020-09-25 21:58:33.197155	2020-09-25 21:58:33.197155
244	69	AUGPARA	1051	czp10	1052	2020-09-25 21:59:50.293455	2020-09-25 21:59:50.293455
245	69	AUGPARA	1055	czp10	1056	2020-09-26 04:33:09.686721	2020-09-26 04:33:09.686721
246	69	AUGPARA	1059	czp10	1060	2020-09-26 04:34:59.393087	2020-09-26 04:34:59.393087
247	69	AUGPARA	1063	czp10	1064	2020-09-26 04:49:59.696073	2020-09-26 04:49:59.696073
248	69	AUGPARA	1067	czp10	1068	2020-09-26 04:51:20.225811	2020-09-26 04:51:20.225811
249	69	AUGPARA	1071	czp10	1072	2020-09-26 21:08:57.570141	2020-09-26 21:08:57.570141
250	69	AUGPARA	1075	czp10	1076	2020-09-27 03:30:17.709191	2020-09-27 03:30:17.709191
251	69	AUGPARA	1079	czp10	1080	2020-09-27 20:59:46.053107	2020-09-27 20:59:46.053107
252	69	AUGPARA	1083	czp10	1084	2020-09-27 21:00:35.414533	2020-09-27 21:00:35.414533
253	69	AUGPARA	1087	czp10	1088	2020-09-27 21:08:41.694628	2020-09-27 21:08:41.694628
254	69	AUGPARA	1091	czp10	1092	2020-09-27 21:27:50.747849	2020-09-27 21:27:50.747849
255	69	AUGPARA	1095	czp10	1096	2020-09-28 02:23:28.203798	2020-09-28 02:23:28.203798
256	69	AUGPARA	1099	czp10	1100	2020-09-28 02:29:08.749943	2020-09-28 02:29:08.749943
257	69	AUGPARA	1103	czp10	1104	2020-09-28 02:44:01.446044	2020-09-28 02:44:01.446044
258	69	AUGPARA	1107	czp10	1108	2020-09-28 02:56:27.733324	2020-09-28 02:56:27.733324
259	69	AUGPARA	1111	czp10	1112	2020-09-28 03:26:53.594457	2020-09-28 03:26:53.594457
260	69	AUGPARA	1115	czp10	1116	2020-09-28 03:31:01.118104	2020-09-28 03:31:01.118104
261	69	AUGPARA	1119	czp10	1120	2020-09-28 03:32:40.957584	2020-09-28 03:32:40.957584
262	69	AUGPARA	1123	czp10	1124	2020-09-28 18:20:38.434079	2020-09-28 18:20:38.434079
263	69	AUGPARA	1127	czp10	1128	2020-09-28 18:23:38.795301	2020-09-28 18:23:38.795301
264	69	AUGPARA	1131	czp10	1132	2020-09-28 18:28:39.938652	2020-09-28 18:28:39.938652
265	69	AUGPARA	1135	czp10	1136	2020-09-28 18:32:28.477683	2020-09-28 18:32:28.477683
266	69	AUGPARA	1139	czp10	1140	2020-09-28 18:44:33.553167	2020-09-28 18:44:33.553167
267	69	AUGPARA	1143	czp10	1144	2020-09-28 18:45:56.402208	2020-09-28 18:45:56.402208
268	69	AUGPARA	1147	czp10	1148	2020-09-28 18:48:57.363037	2020-09-28 18:48:57.363037
269	69	AUGPARA	1151	czp10	1152	2020-09-28 19:03:35.016505	2020-09-28 19:03:35.016505
270	69	AUGPARA	1155	czp10	1156	2020-09-28 19:09:57.314846	2020-09-28 19:09:57.314846
271	69	AUGPARA	1159	czp10	1160	2020-09-28 19:12:38.608091	2020-09-28 19:12:38.608091
272	69	AUGPARA	1163	czp10	1164	2020-09-28 19:13:43.286998	2020-09-28 19:13:43.286998
273	69	AUGPARA	1167	czp10	1168	2020-09-28 19:14:59.925107	2020-09-28 19:14:59.925107
274	69	AUGPARA	1171	czp10	1172	2020-09-28 19:15:20.817982	2020-09-28 19:15:20.817982
275	69	AUGPARA	1175	czp10	1176	2020-09-28 19:17:31.525917	2020-09-28 19:17:31.525917
276	69	AUGPARA	1179	czp10	1180	2020-09-28 19:18:36.755063	2020-09-28 19:18:36.755063
277	69	AUGPARA	1183	czp10	1184	2020-09-28 19:19:43.659524	2020-09-28 19:19:43.659524
278	69	AUGPARA	1187	czp10	1188	2020-09-28 19:20:57.246906	2020-09-28 19:20:57.246906
279	69	AUGPARA	1191	czp10	1192	2020-09-28 19:45:20.594747	2020-09-28 19:45:20.594747
280	69	AUGPARA	1195	czp10	1196	2020-09-28 19:46:48.358166	2020-09-28 19:46:48.358166
281	69	AUGPARA	1199	czp10	1200	2020-09-28 19:49:34.527159	2020-09-28 19:49:34.527159
282	69	AUGPARA	1203	czp10	1204	2020-09-28 19:50:28.355752	2020-09-28 19:50:28.355752
283	69	AUGPARA	1207	czp10	1208	2020-09-28 19:54:31.237316	2020-09-28 19:54:31.237316
284	69	AUGPARA	1211	czp10	1212	2020-09-28 19:55:41.358396	2020-09-28 19:55:41.358396
285	69	AUGPARA	1215	czp10	1216	2020-09-28 19:57:10.288749	2020-09-28 19:57:10.288749
286	69	AUGPARA	1219	czp10	1220	2020-09-28 20:00:01.741006	2020-09-28 20:00:01.741006
287	69	AUGPARA	1223	czp10	1224	2020-09-28 20:01:40.91373	2020-09-28 20:01:40.91373
288	69	AUGPARA	1227	czp10	1228	2020-09-28 20:02:41.325747	2020-09-28 20:02:41.325747
289	69	AUGPARA	1231	czp10	1232	2020-09-28 23:15:27.772943	2020-09-28 23:15:27.772943
290	69	AUGPARA	1235	czp10	1236	2020-09-28 23:21:32.403643	2020-09-28 23:21:32.403643
291	69	AUGPARA	1239	czp10	1240	2020-09-28 23:23:30.752644	2020-09-28 23:23:30.752644
292	69	AUGPARA	1243	czp10	1244	2020-09-28 23:45:35.258835	2020-09-28 23:45:35.258835
293	69	AUGPARA	1247	czp10	1248	2020-09-28 23:57:21.184959	2020-09-28 23:57:21.184959
294	69	AUGPARA	1251	czp10	1252	2020-09-29 00:09:27.270741	2020-09-29 00:09:27.270741
295	69	AUGPARA	1255	czp10	1256	2020-09-29 00:11:48.64605	2020-09-29 00:11:48.64605
296	69	AUGPARA	1259	czp10	1260	2020-09-29 00:13:54.420558	2020-09-29 00:13:54.420558
297	69	AUGPARA	1263	czp10	1264	2020-09-29 00:14:51.974246	2020-09-29 00:14:51.974246
298	69	AUGPARA	1267	czp10	1268	2020-09-29 00:18:00.802389	2020-09-29 00:18:00.802389
299	69	AUGPARA	1271	czp10	1272	2020-09-29 00:20:30.127997	2020-09-29 00:20:30.127997
300	69	AUGPARA	1275	czp10	1276	2020-09-29 00:22:57.367342	2020-09-29 00:22:57.367342
301	69	AUGPARA	1279	czp10	1280	2020-09-29 00:24:04.749565	2020-09-29 00:24:04.749565
302	69	AUGPARA	1283	czp10	1284	2020-09-29 00:26:52.28481	2020-09-29 00:26:52.28481
303	69	AUGPARA	1287	czp10	1288	2020-09-29 00:28:33.341399	2020-09-29 00:28:33.341399
304	69	AUGPARA	1291	czp10	1292	2020-09-29 00:30:07.213636	2020-09-29 00:30:07.213636
305	69	AUGPARA	1295	czp10	1296	2020-09-29 00:31:17.192803	2020-09-29 00:31:17.192803
306	69	AUGPARA	1299	czp10	1300	2020-09-29 00:32:43.531374	2020-09-29 00:32:43.531374
307	69	AUGPARA	1303	czp10	1304	2020-09-29 00:33:30.409261	2020-09-29 00:33:30.409261
308	69	AUGPARA	1307	czp10	1308	2020-09-29 00:34:07.503264	2020-09-29 00:34:07.503264
309	69	AUGPARA	1311	czp10	1312	2020-09-29 00:34:53.531029	2020-09-29 00:34:53.531029
310	69	AUGPARA	1315	czp10	1316	2020-09-29 00:35:27.550044	2020-09-29 00:35:27.550044
311	69	AUGPARA	1319	czp10	1320	2020-09-29 00:46:03.085591	2020-09-29 00:46:03.085591
312	69	AUGPARA	1323	czp10	1324	2020-09-29 00:52:25.167329	2020-09-29 00:52:25.167329
313	69	AUGPARA	1327	czp10	1328	2020-09-29 01:04:16.677425	2020-09-29 01:04:16.677425
314	69	AUGPARA	1331	czp10	1332	2020-09-29 01:08:23.79249	2020-09-29 01:08:23.79249
315	69	AUGPARA	1335	czp10	1336	2020-09-29 01:13:12.693463	2020-09-29 01:13:12.693463
316	69	AUGPARA	1339	czp10	1340	2020-09-29 01:24:35.409974	2020-09-29 01:24:35.409974
317	69	AUGPARA	1343	czp10	1344	2020-09-29 01:27:42.80464	2020-09-29 01:27:42.80464
318	69	AUGPARA	1347	czp10	1348	2020-09-29 01:34:33.779675	2020-09-29 01:34:33.779675
319	69	AUGPARA	1351	czp10	1352	2020-09-29 01:37:43.991376	2020-09-29 01:37:43.991376
320	69	AUGPARA	1355	czp10	1356	2020-09-29 01:39:58.891388	2020-09-29 01:39:58.891388
321	69	AUGPARA	1359	czp10	1360	2020-09-29 02:04:55.929973	2020-09-29 02:04:55.929973
322	69	AUGPARA	1363	czp10	1364	2020-09-29 02:12:03.497944	2020-09-29 02:12:03.497944
323	69	AUGPARA	1367	czp10	1368	2020-09-29 02:28:14.965595	2020-09-29 02:28:14.965595
324	69	AUGPARA	1371	czp10	1372	2020-09-29 03:50:17.398574	2020-09-29 03:50:17.398574
325	69	AUGPARA	1375	czp10	1376	2020-09-30 15:13:22.40464	2020-09-30 15:13:22.40464
326	69	AUGPARA	1379	czp10	1380	2020-09-30 19:29:58.068505	2020-09-30 19:29:58.068505
327	69	AUGPARA	1383	czp10	1384	2020-10-01 18:40:11.980929	2020-10-01 18:40:11.980929
328	69	AUGPARA	1387	czp10	1388	2020-10-01 18:46:41.500685	2020-10-01 18:46:41.500685
329	69	AUGPARA	1391	czp10	1392	2020-10-01 18:56:24.693244	2020-10-01 18:56:24.693244
330	69	AUGPARA	1395	czp10	1396	2020-10-02 18:50:50.580627	2020-10-02 18:50:50.580627
331	69	AUGPARA	1399	czp10	1400	2020-10-02 19:02:11.429195	2020-10-02 19:02:11.429195
332	69	AUGPARA	1403	czp10	1404	2020-10-02 23:40:57.800612	2020-10-02 23:40:57.800612
333	69	AUGPARA	1407	czp10	1408	2020-10-02 23:59:23.416066	2020-10-02 23:59:23.416066
334	69	AUGPARA	1411	czp10	1412	2020-10-03 00:04:48.518875	2020-10-03 00:04:48.518875
335	69	AUGPARA	1415	czp10	1416	2020-10-03 00:59:06.661934	2020-10-03 00:59:06.661934
336	69	AUGPARA	1419	czp10	1420	2020-10-03 02:34:57.68321	2020-10-03 02:34:57.68321
337	69	AUGPARA	1423	czp10	1424	2020-10-03 03:08:31.31085	2020-10-03 03:08:31.31085
338	69	AUGPARA	1427	czp10	1428	2020-10-03 03:19:45.528817	2020-10-03 03:19:45.528817
339	69	AUGPARA	1431	czp10	1432	2020-10-03 05:56:28.610599	2020-10-03 05:56:28.610599
340	69	AUGPARA	1437	czp10	1438	2020-10-03 07:49:18.002916	2020-10-03 07:49:18.002916
341	69	AUGPARA	1441	czp10	1442	2020-10-03 07:59:12.312984	2020-10-03 07:59:12.312984
342	69	AUGPARA	1445	czp10	1446	2020-10-03 08:00:20.70878	2020-10-03 08:00:20.70878
343	69	AUGPARA	1449	czp10	1450	2020-10-03 08:02:11.940595	2020-10-03 08:02:11.940595
344	69	AUGPARA	1453	czp10	1454	2020-10-03 08:03:07.393865	2020-10-03 08:03:07.393865
345	69	AUGPARA	1457	czp10	1458	2020-10-03 08:27:24.113339	2020-10-03 08:27:24.113339
346	69	AUGPARA	1461	czp10	1462	2020-10-03 08:30:13.181898	2020-10-03 08:30:13.181898
347	69	AUGPARA	1465	czp10	1466	2020-10-03 08:37:08.314817	2020-10-03 08:37:08.314817
348	69	AUGPARA	1469	czp10	1470	2020-10-03 08:39:39.54041	2020-10-03 08:39:39.54041
349	69	AUGPARA	1473	czp10	1474	2020-10-03 08:44:36.949823	2020-10-03 08:44:36.949823
350	69	AUGPARA	1477	czp10	1478	2020-10-03 08:46:06.030234	2020-10-03 08:46:06.030234
351	69	AUGPARA	1481	czp10	1482	2020-10-03 08:46:51.147198	2020-10-03 08:46:51.147198
352	69	AUGPARA	1485	czp10	1486	2020-10-03 08:49:25.100406	2020-10-03 08:49:25.100406
353	69	AUGPARA	1489	czp10	1490	2020-10-03 08:56:59.494296	2020-10-03 08:56:59.494296
354	69	AUGPARA	1493	czp10	1494	2020-10-03 08:57:46.122891	2020-10-03 08:57:46.122891
355	69	AUGPARA	1497	czp10	1498	2020-10-03 08:58:27.882791	2020-10-03 08:58:27.882791
356	69	AUGPARA	1501	czp10	1502	2020-10-03 08:59:18.06097	2020-10-03 08:59:18.06097
357	69	AUGPARA	1505	czp10	1506	2020-10-03 09:11:05.844662	2020-10-03 09:11:05.844662
358	69	AUGPARA	1509	czp10	1510	2020-10-03 09:21:07.880373	2020-10-03 09:21:07.880373
359	69	AUGPARA	1513	czp10	1514	2020-10-03 09:37:08.695694	2020-10-03 09:37:08.695694
360	69	AUGPARA	1517	czp10	1518	2020-10-03 09:39:55.389029	2020-10-03 09:39:55.389029
361	69	AUGPARA	1521	czp10	1522	2020-10-03 09:42:05.20518	2020-10-03 09:42:05.20518
362	69	AUGPARA	1525	czp10	1526	2020-10-03 09:42:59.516572	2020-10-03 09:42:59.516572
363	69	AUGPARA	1529	czp10	1530	2020-10-03 09:44:10.921376	2020-10-03 09:44:10.921376
364	69	AUGPARA	1533	czp10	1534	2020-10-03 09:45:24.859437	2020-10-03 09:45:24.859437
365	69	AUGPARA	1537	czp10	1538	2020-10-03 09:48:34.762133	2020-10-03 09:48:34.762133
366	69	AUGPARA	1541	czp10	1542	2020-10-03 09:50:23.668502	2020-10-03 09:50:23.668502
367	69	AUGPARA	1545	czp10	1546	2020-10-03 09:51:49.9903	2020-10-03 09:51:49.9903
368	69	AUGPARA	1549	czp10	1550	2020-10-03 09:53:14.112154	2020-10-03 09:53:14.112154
369	69	AUGPARA	1553	czp10	1554	2020-10-03 10:08:01.306614	2020-10-03 10:08:01.306614
370	69	AUGPARA	1557	czp10	1558	2020-10-03 10:15:30.249854	2020-10-03 10:15:30.249854
371	69	AUGPARA	1561	czp10	1562	2020-10-03 10:22:31.650089	2020-10-03 10:22:31.650089
372	69	AUGPARA	1565	czp10	1566	2020-10-03 10:24:15.122783	2020-10-03 10:24:15.122783
373	69	AUGPARA	1569	czp10	1570	2020-10-03 10:26:01.988651	2020-10-03 10:26:01.988651
374	69	AUGPARA	1573	czp10	1574	2020-10-03 10:29:26.492378	2020-10-03 10:29:26.492378
375	69	AUGPARA	1577	czp10	1578	2020-10-03 10:30:41.171858	2020-10-03 10:30:41.171858
376	69	AUGPARA	1581	czp10	1582	2020-10-03 10:36:22.817139	2020-10-03 10:36:22.817139
377	69	AUGPARA	1585	czp10	1586	2020-10-03 11:57:22.004161	2020-10-03 11:57:22.004161
378	69	AUGPARA	1589	czp10	1590	2020-10-03 17:17:16.977296	2020-10-03 17:17:16.977296
379	69	AUGPARA	1593	czp10	1594	2020-10-03 18:16:56.06089	2020-10-03 18:16:56.06089
380	69	AUGPARA	1597	czp10	1598	2020-10-03 18:37:26.000011	2020-10-03 18:37:26.000011
381	69	AUGPARA	1601	czp10	1602	2020-10-03 19:07:56.808075	2020-10-03 19:07:56.808075
382	69	AUGPARA	1605	czp10	1606	2020-10-03 19:27:46.503776	2020-10-03 19:27:46.503776
383	69	AUGPARA	1609	czp10	1610	2020-10-05 17:49:30.875272	2020-10-05 17:49:30.875272
384	69	AUGPARA	1613	czp10	1614	2020-10-05 17:50:44.379624	2020-10-05 17:50:44.379624
385	69	AUGPARA	1617	czp10	1618	2020-10-05 18:00:50.090613	2020-10-05 18:00:50.090613
386	69	AUGPARA	1621	czp10	1622	2020-10-05 18:06:10.434313	2020-10-05 18:06:10.434313
387	69	AUGPARA	1625	czp10	1626	2020-10-05 18:13:30.643799	2020-10-05 18:13:30.643799
388	69	AUGPARA	1629	czp10	1630	2020-10-05 18:17:13.757007	2020-10-05 18:17:13.757007
389	69	AUGPARA	1633	czp10	1634	2020-10-05 23:16:15.16416	2020-10-05 23:16:15.16416
390	69	AUGPARA	1637	czp10	1638	2020-10-05 23:33:15.591167	2020-10-05 23:33:15.591167
391	69	AUGPARA	1641	czp10	1642	2020-10-06 02:14:24.528908	2020-10-06 02:14:24.528908
392	69	AUGPARA	1645	czp10	1646	2020-10-06 18:11:30.158913	2020-10-06 18:11:30.158913
393	69	AUGPARA	1649	czp10	1650	2020-10-06 18:13:30.791269	2020-10-06 18:13:30.791269
394	69	AUGPARA	1653	czp10	1654	2020-10-06 18:13:44.65027	2020-10-06 18:13:44.65027
395	69	AUGPARA	1657	czp10	1658	2020-10-06 18:15:08.538553	2020-10-06 18:15:08.538553
396	69	AUGPARA	1661	czp10	1662	2020-10-06 18:15:25.472926	2020-10-06 18:15:25.472926
397	69	AUGPARA	1665	czp10	1666	2020-10-06 18:16:11.150771	2020-10-06 18:16:11.150771
398	69	AUGPARA	1669	czp10	1670	2020-10-06 18:16:48.086395	2020-10-06 18:16:48.086395
399	69	AUGPARA	1673	czp10	1674	2020-10-06 18:18:27.325699	2020-10-06 18:18:27.325699
400	69	AUGPARA	1677	czp10	1678	2020-10-06 18:19:48.038822	2020-10-06 18:19:48.038822
401	69	AUGPARA	1681	czp10	1682	2020-10-06 18:20:54.517754	2020-10-06 18:20:54.517754
402	69	AUGPARA	1685	czp10	1686	2020-10-06 18:42:05.184684	2020-10-06 18:42:05.184684
403	69	AUGPARA	1689	czp10	1690	2020-10-07 00:46:15.027552	2020-10-07 00:46:15.027552
404	69	AUGPARA	1693	czp10	1694	2020-10-07 01:22:27.571456	2020-10-07 01:22:27.571456
405	69	AUGPARA	1697	czp10	1698	2020-10-07 01:27:32.123485	2020-10-07 01:27:32.123485
406	69	AUGPARA	1701	czp10	1702	2020-10-07 01:32:13.108465	2020-10-07 01:32:13.108465
407	69	AUGPARA	1705	czp10	1706	2020-10-07 01:33:29.879496	2020-10-07 01:33:29.879496
408	69	AUGPARA	1709	czp10	1710	2020-10-08 17:22:19.630448	2020-10-08 17:22:19.630448
409	69	AUGPARA	1713	czp10	1714	2020-10-08 17:26:52.888808	2020-10-08 17:26:52.888808
410	69	AUGPARA	1717	czp10	1718	2020-10-08 18:01:04.617017	2020-10-08 18:01:04.617017
411	69	AUGPARA	1721	czp10	1722	2020-10-08 18:03:56.352848	2020-10-08 18:03:56.352848
412	69	AUGPARA	1725	czp10	1726	2020-10-09 17:04:20.120656	2020-10-09 17:04:20.120656
413	69	AUGPARA	1729	czp10	1730	2020-10-09 17:41:34.899721	2020-10-09 17:41:34.899721
414	69	AUGPARA	1733	czp10	1734	2020-10-09 17:47:58.990294	2020-10-09 17:47:58.990294
415	69	AUGPARA	1737	czp10	1738	2020-10-09 17:55:22.001318	2020-10-09 17:55:22.001318
416	69	AUGPARA	1741	czp10	1742	2020-10-09 18:01:12.181531	2020-10-09 18:01:12.181531
417	69	AUGPARA	1745	czp10	1746	2020-10-09 22:10:15.925477	2020-10-09 22:10:15.925477
418	69	AUGPARA	1749	czp10	1750	2020-10-09 22:12:54.660437	2020-10-09 22:12:54.660437
419	69	AUGPARA	1753	czp10	1754	2020-10-09 22:16:51.058475	2020-10-09 22:16:51.058475
420	69	AUGPARA	1757	czp10	1758	2020-10-09 23:32:25.363792	2020-10-09 23:32:25.363792
421	69	AUGPARA	1761	czp10	1762	2020-10-10 01:33:07.986462	2020-10-10 01:33:07.986462
422	69	AUGPARA	1765	czp10	1766	2020-10-10 02:19:01.498262	2020-10-10 02:19:01.498262
423	69	AUGPARA	1769	czp10	1770	2020-10-10 02:20:17.811449	2020-10-10 02:20:17.811449
424	69	AUGPARA	1773	czp10	1774	2020-10-10 02:21:07.347688	2020-10-10 02:21:07.347688
425	69	AUGPARA	1777	czp10	1778	2020-10-10 02:24:27.185095	2020-10-10 02:24:27.185095
426	69	AUGPARA	1781	czp10	1782	2020-10-10 02:27:54.51467	2020-10-10 02:27:54.51467
427	69	AUGPARA	1785	czp10	1786	2020-10-10 02:34:01.112626	2020-10-10 02:34:01.112626
428	69	AUGPARA	1789	czp10	1790	2020-10-10 02:35:48.927627	2020-10-10 02:35:48.927627
429	69	AUGPARA	1793	czp10	1794	2020-10-10 02:37:12.041091	2020-10-10 02:37:12.041091
430	69	AUGPARA	1797	czp10	1798	2020-10-10 02:45:11.573542	2020-10-10 02:45:11.573542
431	69	AUGPARA	1801	czp10	1802	2020-10-10 02:47:06.269621	2020-10-10 02:47:06.269621
432	69	AUGPARA	1805	czp10	1806	2020-10-10 02:47:46.57234	2020-10-10 02:47:46.57234
433	69	AUGPARA	1809	czp10	1810	2020-10-10 02:53:45.269532	2020-10-10 02:53:45.269532
434	69	AUGPARA	1813	czp10	1814	2020-10-10 02:54:03.479023	2020-10-10 02:54:03.479023
435	69	AUGPARA	1817	czp10	1818	2020-10-10 03:04:44.893794	2020-10-10 03:04:44.893794
436	69	AUGPARA	1821	czp10	1822	2020-10-10 03:32:20.746919	2020-10-10 03:32:20.746919
437	69	AUGPARA	1825	czp10	1826	2020-10-10 03:36:59.264506	2020-10-10 03:36:59.264506
438	69	AUGPARA	1829	czp10	1830	2020-10-10 03:45:36.913004	2020-10-10 03:45:36.913004
439	69	AUGPARA	1833	czp10	1834	2020-10-10 04:11:21.725721	2020-10-10 04:11:21.725721
440	69	AUGPARA	1837	czp10	1838	2020-10-13 00:11:28.242241	2020-10-13 00:11:28.242241
441	69	AUGPARA	1841	czp10	1842	2020-10-13 00:58:17.397733	2020-10-13 00:58:17.397733
442	69	AUGPARA	1845	czp10	1846	2020-10-13 01:00:20.746735	2020-10-13 01:00:20.746735
443	69	AUGPARA	1849	czp10	1850	2020-10-13 01:09:13.620006	2020-10-13 01:09:13.620006
444	69	AUGPARA	1853	czp10	1854	2020-10-13 01:11:15.544011	2020-10-13 01:11:15.544011
445	69	AUGPARA	1857	czp10	1858	2020-10-13 01:12:08.39704	2020-10-13 01:12:08.39704
446	69	AUGPARA	1861	czp10	1862	2020-10-13 01:13:07.525536	2020-10-13 01:13:07.525536
447	69	AUGPARA	1865	czp10	1866	2020-10-13 01:14:09.662541	2020-10-13 01:14:09.662541
448	69	AUGPARA	1869	czp10	1870	2020-10-13 01:14:39.364673	2020-10-13 01:14:39.364673
449	69	AUGPARA	1873	czp10	1874	2020-10-13 01:16:22.837253	2020-10-13 01:16:22.837253
450	69	AUGPARA	1877	czp10	1878	2020-10-13 01:19:22.665393	2020-10-13 01:19:22.665393
451	69	AUGPARA	1881	czp10	1882	2020-10-13 01:20:45.895289	2020-10-13 01:20:45.895289
452	69	AUGPARA	1885	czp10	1886	2020-10-13 01:24:46.059378	2020-10-13 01:24:46.059378
453	69	AUGPARA	1889	czp10	1890	2020-10-13 01:25:46.521404	2020-10-13 01:25:46.521404
454	69	AUGPARA	1893	czp10	1894	2020-10-13 02:05:41.071945	2020-10-13 02:05:41.071945
455	69	AUGPARA	1897	czp10	1898	2020-10-13 02:28:20.227664	2020-10-13 02:28:20.227664
456	69	AUGPARA	1901	czp10	1902	2020-10-13 17:35:45.430024	2020-10-13 17:35:45.430024
457	69	AUGPARA	1905	czp10	1906	2020-10-13 17:41:28.208236	2020-10-13 17:41:28.208236
458	69	AUGPARA	1907	czp10	1908	2020-10-13 20:02:05.837671	2020-10-13 20:02:05.837671
459	69	AUGPARA	1909	czp10	1910	2020-10-13 20:03:31.701241	2020-10-13 20:03:31.701241
460	69	AUGPARA	1911	czp10	1912	2020-10-13 20:04:38.471709	2020-10-13 20:04:38.471709
461	69	AUGPARA	1913	czp10	1914	2020-10-13 20:05:44.742517	2020-10-13 20:05:44.742517
462	69	AUGPARA	1915	czp10	1916	2020-10-13 20:06:08.56885	2020-10-13 20:06:08.56885
463	69	AUGPARA	1917	czp10	1918	2020-10-13 20:07:30.9153	2020-10-13 20:07:30.9153
464	69	AUGPARA	1919	czp10	1920	2020-10-13 20:08:00.750505	2020-10-13 20:08:00.750505
465	69	AUGPARA	1921	czp10	1922	2020-10-13 20:09:17.821768	2020-10-13 20:09:17.821768
466	69	AUGPARA	1923	czp10	1924	2020-10-13 20:13:42.203983	2020-10-13 20:13:42.203983
467	69	AUGPARA	1925	czp10	1926	2020-10-13 20:14:19.422917	2020-10-13 20:14:19.422917
468	69	AUGPARA	1927	czp10	1928	2020-10-15 21:54:02.219613	2020-10-15 21:54:02.219613
469	69	AUGPARA	1929	czp10	1930	2020-10-15 21:54:58.664501	2020-10-15 21:54:58.664501
470	69	AUGPARA	1931	czp10	1932	2020-10-15 22:00:46.868527	2020-10-15 22:00:46.868527
471	69	AUGPARA	1933	czp10	1934	2020-10-16 16:55:41.995149	2020-10-16 16:55:41.995149
472	69	AUGPARA	1935	czp10	1936	2020-10-16 17:12:00.385063	2020-10-16 17:12:00.385063
473	69	AUGPARA	1937	czp10	1938	2020-10-16 17:27:09.309149	2020-10-16 17:27:09.309149
474	69	AUGPARA	1939	czp10	1940	2020-10-16 17:45:48.621151	2020-10-16 17:45:48.621151
475	69	AUGPARA	1941	czp10	1942	2020-10-16 19:23:37.792673	2020-10-16 19:23:37.792673
476	69	AUGPARA	1943	czp10	1944	2020-10-16 19:27:19.114246	2020-10-16 19:27:19.114246
477	69	AUGPARA	1945	czp10	1946	2020-10-17 21:45:16.993811	2020-10-17 21:45:16.993811
478	69	AUGPARA	1947	czp10	1948	2020-10-17 21:51:55.025979	2020-10-17 21:51:55.025979
479	69	AUGPARA	1949	czp10	1950	2020-10-18 00:39:15.627292	2020-10-18 00:39:15.627292
480	69	AUGPARA	1951	czp10	1952	2020-10-19 19:37:18.062519	2020-10-19 19:37:18.062519
481	69	AUGPARA	1953	czp10	1954	2020-10-19 19:42:45.974017	2020-10-19 19:42:45.974017
482	69	AUGPARA	1955	czp10	1956	2020-10-19 19:43:25.543199	2020-10-19 19:43:25.543199
483	69	AUGPARA	1957	czp10	1958	2020-10-19 19:44:52.064992	2020-10-19 19:44:52.064992
484	69	AUGPARA	1959	czp10	1960	2020-10-19 19:55:18.969916	2020-10-19 19:55:18.969916
485	69	AUGPARA	1961	czp10	1962	2020-10-19 19:58:15.839075	2020-10-19 19:58:15.839075
486	69	AUGPARA	1963	czp10	1964	2020-10-19 20:06:24.939729	2020-10-19 20:06:24.939729
487	69	AUGPARA	1965	czp10	1966	2020-10-19 20:15:56.413818	2020-10-19 20:15:56.413818
488	69	AUGPARA	1967	czp10	1968	2020-10-19 20:20:04.929037	2020-10-19 20:20:04.929037
489	69	AUGPARA	1969	czp10	1970	2020-10-19 20:24:05.638835	2020-10-19 20:24:05.638835
490	69	AUGPARA	1971	czp10	1972	2020-10-19 20:24:37.44584	2020-10-19 20:24:37.44584
491	69	AUGPARA	1973	czp10	1974	2020-10-20 17:35:45.490351	2020-10-20 17:35:45.490351
492	69	AUGPARA	1975	czp10	1976	2020-10-20 17:53:44.121607	2020-10-20 17:53:44.121607
493	69	AUGPARA	1977	czp10	1978	2020-10-20 18:04:31.86115	2020-10-20 18:04:31.86115
494	69	AUGPARA	1979	czp10	1980	2020-10-20 18:06:12.925534	2020-10-20 18:06:12.925534
495	69	AUGPARA	1981	czp10	1982	2020-10-20 18:06:49.012052	2020-10-20 18:06:49.012052
496	69	AUGPARA	1983	czp10	1984	2020-10-20 18:08:32.043465	2020-10-20 18:08:32.043465
497	69	AUGPARA	1985	czp10	1986	2020-10-20 18:14:17.772926	2020-10-20 18:14:17.772926
498	69	AUGPARA	1987	czp10	1988	2020-10-20 18:15:06.117551	2020-10-20 18:15:06.117551
499	69	AUGPARA	1989	czp10	1990	2020-10-20 18:15:27.802291	2020-10-20 18:15:27.802291
500	69	AUGPARA	1991	czp10	1992	2020-10-20 18:16:20.097236	2020-10-20 18:16:20.097236
501	69	AUGPARA	1993	czp10	1994	2020-10-20 18:24:39.665101	2020-10-20 18:24:39.665101
502	69	AUGPARA	1995	czp10	1996	2020-10-20 18:24:55.578912	2020-10-20 18:24:55.578912
503	69	AUGPARA	1997	czp10	1998	2020-10-20 18:35:51.393959	2020-10-20 18:35:51.393959
504	69	AUGPARA	1999	czp10	2000	2020-10-20 18:41:06.346804	2020-10-20 18:41:06.346804
505	69	AUGPARA	2001	czp10	2002	2020-10-20 18:43:01.328866	2020-10-20 18:43:01.328866
506	69	AUGPARA	2003	czp10	2004	2020-10-20 18:46:01.381005	2020-10-20 18:46:01.381005
507	69	AUGPARA	2005	czp10	2006	2020-10-20 20:29:49.002728	2020-10-20 20:29:49.002728
508	69	AUGPARA	2007	czp10	2008	2020-10-20 20:34:48.529454	2020-10-20 20:34:48.529454
509	69	AUGPARA	2009	czp10	2010	2020-10-20 20:40:00.682095	2020-10-20 20:40:00.682095
510	69	AUGPARA	2011	czp10	2012	2020-10-20 20:41:09.261472	2020-10-20 20:41:09.261472
511	69	AUGPARA	2013	czp10	2014	2020-10-20 20:57:55.331469	2020-10-20 20:57:55.331469
512	69	AUGPARA	2015	czp10	2016	2020-10-20 21:10:32.236329	2020-10-20 21:10:32.236329
513	69	AUGPARA	2017	czp10	2018	2020-10-20 21:12:06.54215	2020-10-20 21:12:06.54215
514	69	AUGPARA	2019	czp10	2020	2020-10-20 21:14:21.167029	2020-10-20 21:14:21.167029
515	69	AUGPARA	2021	czp10	2022	2020-10-20 21:15:22.695841	2020-10-20 21:15:22.695841
516	69	AUGPARA	2023	czp10	2024	2020-10-20 21:15:31.679794	2020-10-20 21:15:31.679794
517	69	AUGPARA	2025	czp10	2026	2020-10-20 21:20:41.273816	2020-10-20 21:20:41.273816
518	69	AUGPARA	2027	czp10	2028	2020-10-20 21:27:53.900511	2020-10-20 21:27:53.900511
519	69	AUGPARA	2029	czp10	2030	2020-10-20 21:32:29.20089	2020-10-20 21:32:29.20089
520	69	AUGPARA	2031	czp10	2032	2020-10-21 17:24:41.641907	2020-10-21 17:24:41.641907
521	69	AUGPARA	2033	czp10	2034	2020-10-21 17:51:59.827914	2020-10-21 17:51:59.827914
522	69	AUGPARA	2035	czp10	2036	2020-10-21 17:52:25.354397	2020-10-21 17:52:25.354397
523	69	AUGPARA	2037	czp10	2038	2020-10-21 17:53:29.924702	2020-10-21 17:53:29.924702
524	69	AUGPARA	2039	czp10	2040	2020-10-21 17:56:20.051287	2020-10-21 17:56:20.051287
525	69	AUGPARA	2041	czp10	2042	2020-10-21 17:58:51.56839	2020-10-21 17:58:51.56839
526	69	AUGPARA	2043	czp10	2044	2020-10-21 17:59:23.403671	2020-10-21 17:59:23.403671
527	69	AUGPARA	2045	czp10	2046	2020-10-21 18:06:15.14427	2020-10-21 18:06:15.14427
528	69	AUGPARA	2047	czp10	2048	2020-10-21 18:10:51.318713	2020-10-21 18:10:51.318713
529	69	AUGPARA	2049	czp10	2050	2020-10-21 18:11:03.736124	2020-10-21 18:11:03.736124
530	69	AUGPARA	2051	czp10	2052	2020-10-21 18:34:05.950001	2020-10-21 18:34:05.950001
531	69	AUGPARA	2053	czp10	2054	2020-10-21 18:35:43.614087	2020-10-21 18:35:43.614087
532	69	AUGPARA	2055	czp10	2056	2020-10-21 18:51:24.394144	2020-10-21 18:51:24.394144
533	69	AUGPARA	2057	czp10	2058	2020-10-22 19:20:35.731087	2020-10-22 19:20:35.731087
534	69	AUGPARA	2059	czp10	2060	2020-10-22 19:20:48.090255	2020-10-22 19:20:48.090255
535	69	AUGPARA	2061	czp10	2062	2020-10-22 21:26:45.092592	2020-10-22 21:26:45.092592
536	69	AUGPARA	2063	czp10	2064	2020-10-22 21:32:31.529638	2020-10-22 21:32:31.529638
537	69	AUGPARA	2065	czp10	2066	2020-10-22 22:44:38.105106	2020-10-22 22:44:38.105106
538	69	AUGPARA	2067	czp10	2068	2020-10-22 22:56:19.267853	2020-10-22 22:56:19.267853
539	69	AUGPARA	2069	czp10	2070	2020-10-22 23:19:17.232745	2020-10-22 23:19:17.232745
540	69	AUGPARA	2071	czp10	2072	2020-10-22 23:19:38.975786	2020-10-22 23:19:38.975786
541	69	AUGPARA	2073	czp10	2074	2020-10-22 23:20:33.32896	2020-10-22 23:20:33.32896
542	69	AUGPARA	2075	czp10	2076	2020-10-23 18:35:16.270062	2020-10-23 18:35:16.270062
543	69	AUGPARA	2077	czp10	2078	2020-10-23 18:41:16.490912	2020-10-23 18:41:16.490912
544	69	AUGPARA	2079	czp10	2080	2020-10-23 18:42:39.295548	2020-10-23 18:42:39.295548
545	69	AUGPARA	2081	czp10	2082	2020-10-23 18:43:04.913788	2020-10-23 18:43:04.913788
546	69	AUGPARA	2083	czp10	2084	2020-10-23 18:51:46.769042	2020-10-23 18:51:46.769042
547	69	AUGPARA	2085	czp10	2086	2020-10-23 18:59:39.797406	2020-10-23 18:59:39.797406
548	69	AUGPARA	2087	czp10	2088	2020-10-23 19:04:00.244649	2020-10-23 19:04:00.244649
549	69	AUGPARA	2089	czp10	2090	2020-10-23 19:12:22.650732	2020-10-23 19:12:22.650732
550	69	AUGPARA	2091	czp10	2092	2020-10-23 19:13:15.787127	2020-10-23 19:13:15.787127
551	69	AUGPARA	2093	czp10	2094	2020-10-23 19:13:37.246749	2020-10-23 19:13:37.246749
552	69	AUGPARA	2095	czp10	2096	2020-10-26 19:20:30.406589	2020-10-26 19:20:30.406589
553	69	AUGPARA	2097	czp10	2098	2020-10-26 19:31:01.301256	2020-10-26 19:31:01.301256
554	69	AUGPARA	2099	czp10	2100	2020-10-26 19:40:48.785513	2020-10-26 19:40:48.785513
555	69	AUGPARA	2101	czp10	2102	2020-10-26 19:46:42.406271	2020-10-26 19:46:42.406271
556	69	AUGPARA	2103	czp10	2104	2020-10-26 20:03:48.91704	2020-10-26 20:03:48.91704
557	69	AUGPARA	2105	czp10	2106	2020-10-26 20:18:39.760641	2020-10-26 20:18:39.760641
558	69	AUGPARA	2107	czp10	2108	2020-10-26 20:25:59.261676	2020-10-26 20:25:59.261676
559	69	AUGPARA	2109	czp10	2110	2020-10-26 20:26:37.055605	2020-10-26 20:26:37.055605
560	69	AUGPARA	2111	czp10	2112	2020-10-26 23:01:13.969461	2020-10-26 23:01:13.969461
561	69	AUGPARA	2113	czp10	2114	2020-10-26 23:07:36.300217	2020-10-26 23:07:36.300217
562	69	AUGPARA	2115	czp10	2116	2020-10-26 23:13:10.420272	2020-10-26 23:13:10.420272
563	69	AUGPARA	2117	czp10	2118	2020-10-26 23:14:45.475983	2020-10-26 23:14:45.475983
564	69	AUGPARA	2119	czp10	2120	2020-10-26 23:16:57.884007	2020-10-26 23:16:57.884007
565	69	AUGPARA	2121	czp10	2122	2020-10-26 23:18:05.712994	2020-10-26 23:18:05.712994
566	69	AUGPARA	2123	czp10	2124	2020-10-26 23:23:29.574009	2020-10-26 23:23:29.574009
567	69	AUGPARA	2125	czp10	2126	2020-10-26 23:26:59.194893	2020-10-26 23:26:59.194893
568	69	AUGPARA	2127	czp10	2128	2020-10-26 23:29:49.888467	2020-10-26 23:29:49.888467
569	69	AUGPARA	2129	czp10	2130	2020-10-26 23:31:26.019266	2020-10-26 23:31:26.019266
570	69	AUGPARA	2131	czp10	2132	2020-10-26 23:31:33.894848	2020-10-26 23:31:33.894848
571	69	AUGPARA	2133	czp10	2134	2020-10-26 23:32:26.247875	2020-10-26 23:32:26.247875
572	69	AUGPARA	2135	czp10	2136	2020-10-26 23:34:16.596202	2020-10-26 23:34:16.596202
573	69	AUGPARA	2137	czp10	2138	2020-10-26 23:36:10.880569	2020-10-26 23:36:10.880569
574	69	AUGPARA	2139	czp10	2140	2020-10-26 23:38:24.402521	2020-10-26 23:38:24.402521
575	69	AUGPARA	2141	czp10	2142	2020-10-26 23:45:00.717779	2020-10-26 23:45:00.717779
576	69	AUGPARA	2143	czp10	2144	2020-10-26 23:47:04.050093	2020-10-26 23:47:04.050093
577	69	AUGPARA	2145	czp10	2146	2020-10-26 23:50:04.735864	2020-10-26 23:50:04.735864
578	69	AUGPARA	2147	czp10	2148	2020-10-26 23:51:37.541314	2020-10-26 23:51:37.541314
579	69	AUGPARA	2149	czp10	2150	2020-10-26 23:53:22.58108	2020-10-26 23:53:22.58108
580	69	AUGPARA	2151	czp10	2152	2020-10-26 23:56:58.035348	2020-10-26 23:56:58.035348
581	69	AUGPARA	2153	czp10	2154	2020-10-26 23:57:11.119489	2020-10-26 23:57:11.119489
582	69	AUGPARA	2155	czp10	2156	2020-10-26 23:59:06.834683	2020-10-26 23:59:06.834683
583	69	AUGPARA	2157	czp10	2158	2020-10-27 00:02:00.145094	2020-10-27 00:02:00.145094
584	69	AUGPARA	2159	czp10	2160	2020-10-27 00:03:12.307797	2020-10-27 00:03:12.307797
585	69	AUGPARA	2161	czp10	2162	2020-10-27 00:04:27.653855	2020-10-27 00:04:27.653855
586	69	AUGPARA	2163	czp10	2164	2020-10-27 00:07:44.64081	2020-10-27 00:07:44.64081
587	69	AUGPARA	2165	czp10	2166	2020-10-27 00:08:42.360759	2020-10-27 00:08:42.360759
588	69	AUGPARA	2167	czp10	2168	2020-10-27 00:11:56.39742	2020-10-27 00:11:56.39742
589	69	AUGPARA	2169	czp10	2170	2020-10-27 00:14:57.966605	2020-10-27 00:14:57.966605
590	69	AUGPARA	2171	czp10	2172	2020-10-27 00:17:49.268481	2020-10-27 00:17:49.268481
591	69	AUGPARA	2173	czp10	2174	2020-10-27 00:22:41.711012	2020-10-27 00:22:41.711012
592	69	AUGPARA	2175	czp10	2176	2020-10-27 00:25:37.804979	2020-10-27 00:25:37.804979
593	69	AUGPARA	2177	czp10	2178	2020-10-27 00:26:33.527752	2020-10-27 00:26:33.527752
594	69	AUGPARA	2179	czp10	2180	2020-10-27 00:32:30.904874	2020-10-27 00:32:30.904874
595	69	AUGPARA	2181	czp10	2182	2020-10-27 00:32:56.698049	2020-10-27 00:32:56.698049
596	69	AUGPARA	2183	czp10	2184	2020-10-27 00:35:47.841521	2020-10-27 00:35:47.841521
597	69	AUGPARA	2185	czp10	2186	2020-10-27 00:44:39.198069	2020-10-27 00:44:39.198069
598	69	AUGPARA	2187	czp10	2188	2020-10-27 00:46:04.980157	2020-10-27 00:46:04.980157
599	69	AUGPARA	2189	czp10	2190	2020-10-27 00:50:03.986125	2020-10-27 00:50:03.986125
600	69	AUGPARA	2191	czp10	2192	2020-10-27 00:51:43.325726	2020-10-27 00:51:43.325726
601	69	AUGPARA	2193	czp10	2194	2020-10-27 00:53:41.873528	2020-10-27 00:53:41.873528
602	69	AUGPARA	2195	czp10	2196	2020-10-27 00:56:28.451256	2020-10-27 00:56:28.451256
603	69	AUGPARA	2197	czp10	2198	2020-10-27 00:57:28.096395	2020-10-27 00:57:28.096395
604	69	AUGPARA	2199	czp10	2200	2020-10-27 01:00:15.156487	2020-10-27 01:00:15.156487
605	69	AUGPARA	2201	czp10	2202	2020-10-27 01:01:02.892522	2020-10-27 01:01:02.892522
606	69	AUGPARA	2203	czp10	2204	2020-10-27 01:06:10.744343	2020-10-27 01:06:10.744343
607	69	AUGPARA	2205	czp10	2206	2020-10-27 01:07:10.806339	2020-10-27 01:07:10.806339
608	69	AUGPARA	2207	czp10	2208	2020-10-27 01:07:52.766882	2020-10-27 01:07:52.766882
609	69	AUGPARA	2209	czp10	2210	2020-10-27 01:10:13.13417	2020-10-27 01:10:13.13417
610	69	AUGPARA	2211	czp10	2212	2020-10-27 01:11:54.615184	2020-10-27 01:11:54.615184
611	69	AUGPARA	2213	czp10	2214	2020-10-27 01:13:28.420733	2020-10-27 01:13:28.420733
612	69	AUGPARA	2215	czp10	2216	2020-10-27 01:15:27.718729	2020-10-27 01:15:27.718729
613	69	AUGPARA	2217	czp10	2218	2020-10-27 01:16:40.907263	2020-10-27 01:16:40.907263
614	69	AUGPARA	2219	czp10	2220	2020-10-27 01:17:08.583921	2020-10-27 01:17:08.583921
615	69	AUGPARA	2221	czp10	2222	2020-10-27 01:18:34.730746	2020-10-27 01:18:34.730746
616	69	AUGPARA	2223	czp10	2224	2020-10-27 01:21:01.925671	2020-10-27 01:21:01.925671
617	69	AUGPARA	2225	czp10	2226	2020-10-27 01:21:43.675761	2020-10-27 01:21:43.675761
618	69	AUGPARA	2227	czp10	2228	2020-10-27 01:22:36.470987	2020-10-27 01:22:36.470987
619	69	AUGPARA	2229	czp10	2230	2020-10-27 01:25:34.331543	2020-10-27 01:25:34.331543
620	69	AUGPARA	2231	czp10	2232	2020-10-27 01:26:16.634296	2020-10-27 01:26:16.634296
621	69	AUGPARA	2233	czp10	2234	2020-10-27 01:28:34.534155	2020-10-27 01:28:34.534155
622	69	AUGPARA	2235	czp10	2236	2020-10-27 01:29:36.856746	2020-10-27 01:29:36.856746
623	69	AUGPARA	2237	czp10	2238	2020-10-27 01:32:12.330694	2020-10-27 01:32:12.330694
624	69	AUGPARA	2239	czp10	2240	2020-10-27 01:35:06.941403	2020-10-27 01:35:06.941403
625	69	AUGPARA	2241	czp10	2242	2020-10-27 01:44:36.425049	2020-10-27 01:44:36.425049
626	69	AUGPARA	2243	czp10	2244	2020-10-27 01:44:41.783664	2020-10-27 01:44:41.783664
627	69	AUGPARA	2245	czp10	2246	2020-10-27 01:44:47.22575	2020-10-27 01:44:47.22575
628	69	AUGPARA	2247	czp10	2248	2020-10-27 01:44:52.667723	2020-10-27 01:44:52.667723
629	69	AUGPARA	2249	czp10	2250	2020-10-27 01:45:29.028229	2020-10-27 01:45:29.028229
630	69	AUGPARA	2251	czp10	2252	2020-10-27 01:45:34.570214	2020-10-27 01:45:34.570214
631	69	AUGPARA	2253	czp10	2254	2020-10-27 01:45:39.978855	2020-10-27 01:45:39.978855
632	69	AUGPARA	2255	czp10	2256	2020-10-27 01:45:45.529184	2020-10-27 01:45:45.529184
633	69	AUGPARA	2257	czp10	2258	2020-10-27 01:45:50.846233	2020-10-27 01:45:50.846233
634	69	AUGPARA	2259	czp10	2260	2020-10-27 01:45:56.279812	2020-10-27 01:45:56.279812
635	69	AUGPARA	2261	czp10	2262	2020-10-27 01:46:01.75514	2020-10-27 01:46:01.75514
636	69	AUGPARA	2263	czp10	2264	2020-10-27 01:46:07.088875	2020-10-27 01:46:07.088875
637	69	AUGPARA	2265	czp10	2266	2020-10-27 01:46:12.506024	2020-10-27 01:46:12.506024
638	69	AUGPARA	2267	czp10	2268	2020-10-27 01:46:17.914531	2020-10-27 01:46:17.914531
639	69	AUGPARA	2269	czp10	2270	2020-10-27 01:46:23.464847	2020-10-27 01:46:23.464847
640	69	AUGPARA	2271	czp10	2272	2020-10-27 01:57:48.780728	2020-10-27 01:57:48.780728
641	69	AUGPARA	2273	czp10	2274	2020-10-27 01:58:32.050039	2020-10-27 01:58:32.050039
642	69	AUGPARA	2275	czp10	2276	2020-10-27 02:15:16.80256	2020-10-27 02:15:16.80256
643	69	AUGPARA	2277	czp10	2278	2020-10-27 02:22:13.126091	2020-10-27 02:22:13.126091
644	69	AUGPARA	2279	czp10	2280	2020-10-27 03:08:40.95704	2020-10-27 03:08:40.95704
645	69	AUGPARA	2281	czp10	2282	2020-10-27 03:15:59.999752	2020-10-27 03:15:59.999752
646	69	AUGPARA	2283	czp10	2284	2020-10-27 03:18:35.184079	2020-10-27 03:18:35.184079
647	69	AUGPARA	2285	czp10	2286	2020-10-27 03:24:39.697333	2020-10-27 03:24:39.697333
648	69	AUGPARA	2287	czp10	2288	2020-10-27 03:27:56.767321	2020-10-27 03:27:56.767321
649	69	AUGPARA	2289	czp10	2290	2020-10-27 03:39:16.057629	2020-10-27 03:39:16.057629
650	69	AUGPARA	2291	czp10	2292	2020-10-27 03:41:18.006743	2020-10-27 03:41:18.006743
651	69	AUGPARA	2293	czp10	2294	2020-10-27 03:42:17.893485	2020-10-27 03:42:17.893485
652	69	AUGPARA	2295	czp10	2296	2020-10-27 03:46:54.376572	2020-10-27 03:46:54.376572
653	69	AUGPARA	2297	czp10	2298	2020-10-27 04:10:06.914539	2020-10-27 04:10:06.914539
654	69	AUGPARA	2299	czp10	2300	2020-10-27 04:11:11.755258	2020-10-27 04:11:11.755258
655	69	AUGPARA	2301	czp10	2302	2020-10-27 17:32:34.968553	2020-10-27 17:32:34.968553
656	69	AUGPARA	2303	czp10	2304	2020-10-27 17:32:49.610751	2020-10-27 17:32:49.610751
657	69	AUGPARA	2305	czp10	2306	2020-10-27 17:33:10.453604	2020-10-27 17:33:10.453604
658	69	AUGPARA	2307	czp10	2308	2020-10-27 17:51:40.55157	2020-10-27 17:51:40.55157
659	69	AUGPARA	2309	czp10	2310	2020-10-27 18:19:22.807443	2020-10-27 18:19:22.807443
660	69	AUGPARA	2311	czp10	2312	2020-10-27 19:03:11.371712	2020-10-27 19:03:11.371712
661	69	AUGPARA	2313	czp10	2314	2020-10-27 19:04:27.384405	2020-10-27 19:04:27.384405
662	69	AUGPARA	2315	czp10	2316	2020-10-27 19:05:14.34008	2020-10-27 19:05:14.34008
663	69	AUGPARA	2317	czp10	2318	2020-10-27 19:06:29.791511	2020-10-27 19:06:29.791511
664	69	AUGPARA	2319	czp10	2320	2020-10-27 19:12:45.338808	2020-10-27 19:12:45.338808
665	69	AUGPARA	2321	czp10	2322	2020-10-27 19:12:56.497782	2020-10-27 19:12:56.497782
666	69	AUGPARA	2323	czp10	2324	2020-10-27 19:13:28.82484	2020-10-27 19:13:28.82484
667	69	AUGPARA	2325	czp10	2326	2020-10-27 19:16:07.317581	2020-10-27 19:16:07.317581
668	69	AUGPARA	2327	czp10	2328	2020-10-27 19:23:13.159579	2020-10-27 19:23:13.159579
669	69	AUGPARA	2329	czp10	2330	2020-10-27 19:28:26.486875	2020-10-27 19:28:26.486875
670	69	AUGPARA	2331	czp10	2332	2020-10-27 19:37:28.869155	2020-10-27 19:37:28.869155
671	69	AUGPARA	2333	czp10	2334	2020-10-27 19:38:30.722745	2020-10-27 19:38:30.722745
672	69	AUGPARA	2335	czp10	2336	2020-10-27 19:46:11.558343	2020-10-27 19:46:11.558343
673	69	AUGPARA	2337	czp10	2338	2020-10-27 19:47:04.361409	2020-10-27 19:47:04.361409
674	69	AUGPARA	2339	czp10	2340	2020-10-27 19:54:07.878512	2020-10-27 19:54:07.878512
675	69	AUGPARA	2341	czp10	2342	2020-10-27 19:56:01.260183	2020-10-27 19:56:01.260183
676	69	AUGPARA	2343	czp10	2344	2020-10-27 23:59:04.973601	2020-10-27 23:59:04.973601
677	69	AUGPARA	2345	czp10	2346	2020-10-28 00:02:25.915943	2020-10-28 00:02:25.915943
678	69	AUGPARA	2347	czp10	2348	2020-10-28 00:04:21.90806	2020-10-28 00:04:21.90806
679	69	AUGPARA	2349	czp10	2350	2020-10-28 00:05:09.736876	2020-10-28 00:05:09.736876
680	69	AUGPARA	2351	czp10	2352	2020-10-28 00:06:17.690949	2020-10-28 00:06:17.690949
681	69	AUGPARA	2353	czp10	2354	2020-10-28 00:09:06.400976	2020-10-28 00:09:06.400976
682	69	AUGPARA	2355	czp10	2356	2020-10-28 00:21:46.121252	2020-10-28 00:21:46.121252
683	69	AUGPARA	2357	czp10	2358	2020-10-28 00:25:13.092024	2020-10-28 00:25:13.092024
684	69	AUGPARA	2359	czp10	2360	2020-10-28 00:25:38.64363	2020-10-28 00:25:38.64363
685	69	AUGPARA	2361	czp10	2362	2020-10-28 00:26:41.780701	2020-10-28 00:26:41.780701
686	69	AUGPARA	2363	czp10	2364	2020-10-28 01:54:43.453071	2020-10-28 01:54:43.453071
687	69	AUGPARA	2365	czp10	2366	2020-10-28 01:56:24.742444	2020-10-28 01:56:24.742444
688	69	AUGPARA	2367	czp10	2368	2020-10-28 02:04:03.936714	2020-10-28 02:04:03.936714
689	69	AUGPARA	2369	czp10	2370	2020-10-28 02:05:35.541946	2020-10-28 02:05:35.541946
690	69	AUGPARA	2371	czp10	2372	2020-10-28 02:08:26.777358	2020-10-28 02:08:26.777358
691	69	AUGPARA	2373	czp10	2374	2020-10-28 02:11:10.228613	2020-10-28 02:11:10.228613
692	69	AUGPARA	2375	czp10	2376	2020-10-28 02:12:52.859803	2020-10-28 02:12:52.859803
693	69	AUGPARA	2377	czp10	2378	2020-10-28 02:14:01.555631	2020-10-28 02:14:01.555631
694	69	AUGPARA	2379	czp10	2380	2020-10-28 02:20:41.271121	2020-10-28 02:20:41.271121
695	69	AUGPARA	2381	czp10	2382	2020-10-28 02:22:28.677537	2020-10-28 02:22:28.677537
696	69	AUGPARA	2383	czp10	2384	2020-10-30 17:00:18.134598	2020-10-30 17:00:18.134598
697	69	AUGPARA	2385	czp10	2386	2020-10-30 17:22:24.123283	2020-10-30 17:22:24.123283
698	69	AUGPARA	2387	czp10	2388	2020-10-30 17:22:33.265461	2020-10-30 17:22:33.265461
699	69	AUGPARA	2389	czp10	2390	2020-10-30 17:25:30.600335	2020-10-30 17:25:30.600335
700	69	AUGPARA	2391	czp10	2392	2020-10-30 17:29:13.237603	2020-10-30 17:29:13.237603
701	69	AUGPARA	2393	czp10	2394	2020-10-30 17:32:06.238853	2020-10-30 17:32:06.238853
702	69	AUGPARA	2395	czp10	2396	2020-10-30 23:43:56.685557	2020-10-30 23:43:56.685557
703	69	AUGPARA	2397	czp10	2398	2020-10-30 23:45:57.409021	2020-10-30 23:45:57.409021
704	69	AUGPARA	2399	czp10	2400	2020-10-30 23:53:40.976566	2020-10-30 23:53:40.976566
705	69	AUGPARA	2401	czp10	2402	2020-10-30 23:56:06.96812	2020-10-30 23:56:06.96812
706	69	AUGPARA	2403	czp10	2404	2020-10-30 23:58:17.950742	2020-10-30 23:58:17.950742
707	69	AUGPARA	2405	czp10	2406	2020-10-30 23:58:49.318928	2020-10-30 23:58:49.318928
708	69	AUGPARA	2407	czp10	2408	2020-10-31 00:00:13.848749	2020-10-31 00:00:13.848749
709	69	AUGPARA	2409	czp10	2410	2020-10-31 00:01:58.554459	2020-10-31 00:01:58.554459
710	69	AUGPARA	2411	czp10	2412	2020-10-31 00:03:44.068988	2020-10-31 00:03:44.068988
711	69	AUGPARA	2413	czp10	2414	2020-10-31 00:08:14.143053	2020-10-31 00:08:14.143053
712	69	AUGPARA	2415	czp10	2416	2020-10-31 00:09:18.13993	2020-10-31 00:09:18.13993
713	69	AUGPARA	2417	czp10	2418	2020-10-31 00:11:34.612655	2020-10-31 00:11:34.612655
714	69	AUGPARA	2419	czp10	2420	2020-10-31 00:12:54.29245	2020-10-31 00:12:54.29245
715	69	AUGPARA	2421	czp10	2422	2020-10-31 00:15:26.975888	2020-10-31 00:15:26.975888
716	69	AUGPARA	2423	czp10	2424	2020-10-31 00:16:21.595854	2020-10-31 00:16:21.595854
717	69	AUGPARA	2425	czp10	2426	2020-10-31 00:17:40.291602	2020-10-31 00:17:40.291602
718	69	AUGPARA	2427	czp10	2428	2020-10-31 00:20:45.368658	2020-10-31 00:20:45.368658
719	69	AUGPARA	2429	czp10	2430	2020-10-31 00:23:19.069077	2020-10-31 00:23:19.069077
720	69	AUGPARA	2431	czp10	2432	2020-10-31 00:24:04.305026	2020-10-31 00:24:04.305026
721	69	AUGPARA	2433	czp10	2434	2020-10-31 00:26:49.18086	2020-10-31 00:26:49.18086
722	69	AUGPARA	2435	czp10	2436	2020-10-31 00:28:43.670605	2020-10-31 00:28:43.670605
723	69	AUGPARA	2437	czp10	2438	2020-10-31 00:32:19.699443	2020-10-31 00:32:19.699443
724	69	AUGPARA	2439	czp10	2440	2020-10-31 00:35:08.033951	2020-10-31 00:35:08.033951
725	69	AUGPARA	2441	czp10	2442	2020-10-31 00:36:12.135658	2020-10-31 00:36:12.135658
726	69	AUGPARA	2443	czp10	2444	2020-10-31 00:37:24.392417	2020-10-31 00:37:24.392417
727	69	AUGPARA	2445	czp10	2446	2020-10-31 00:37:55.385142	2020-10-31 00:37:55.385142
728	69	AUGPARA	2447	czp10	2448	2020-10-31 00:40:24.035362	2020-10-31 00:40:24.035362
729	69	AUGPARA	2449	czp10	2450	2020-10-31 00:41:50.231576	2020-10-31 00:41:50.231576
730	69	AUGPARA	2451	czp10	2452	2020-10-31 00:44:05.613416	2020-10-31 00:44:05.613416
731	69	AUGPARA	2453	czp10	2454	2020-10-31 00:48:58.029014	2020-10-31 00:48:58.029014
732	69	AUGPARA	2455	czp10	2456	2020-10-31 00:52:24.918363	2020-10-31 00:52:24.918363
733	69	AUGPARA	2457	czp10	2458	2020-10-31 00:53:08.429621	2020-10-31 00:53:08.429621
734	69	AUGPARA	2459	czp10	2460	2020-10-31 00:55:46.338318	2020-10-31 00:55:46.338318
735	69	AUGPARA	2461	czp10	2462	2020-10-31 00:57:33.697413	2020-10-31 00:57:33.697413
736	69	AUGPARA	2463	czp10	2464	2020-10-31 00:58:47.798153	2020-10-31 00:58:47.798153
737	69	AUGPARA	2465	czp10	2466	2020-10-31 01:00:49.496764	2020-10-31 01:00:49.496764
738	69	AUGPARA	2467	czp10	2468	2020-10-31 01:02:53.606292	2020-10-31 01:02:53.606292
739	69	AUGPARA	2469	czp10	2470	2020-10-31 01:04:51.752206	2020-10-31 01:04:51.752206
740	69	AUGPARA	2471	czp10	2472	2020-10-31 01:07:46.170262	2020-10-31 01:07:46.170262
741	69	AUGPARA	2473	czp10	2474	2020-10-31 01:10:13.42839	2020-10-31 01:10:13.42839
742	69	AUGPARA	2475	czp10	2476	2020-10-31 01:11:11.981689	2020-10-31 01:11:11.981689
743	69	AUGPARA	2477	czp10	2478	2020-10-31 01:12:10.968447	2020-10-31 01:12:10.968447
744	69	AUGPARA	2479	czp10	2480	2020-10-31 01:22:04.535004	2020-10-31 01:22:04.535004
745	69	AUGPARA	2481	czp10	2482	2020-10-31 01:22:35.66173	2020-10-31 01:22:35.66173
746	69	AUGPARA	2483	czp10	2484	2020-10-31 01:28:33.023395	2020-10-31 01:28:33.023395
747	69	AUGPARA	2485	czp10	2486	2020-10-31 01:28:51.491132	2020-10-31 01:28:51.491132
748	69	AUGPARA	2487	czp10	2488	2020-10-31 01:33:03.039852	2020-10-31 01:33:03.039852
749	69	AUGPARA	2489	czp10	2490	2020-10-31 01:34:51.044643	2020-10-31 01:34:51.044643
750	69	AUGPARA	2491	czp10	2492	2020-10-31 01:39:13.909468	2020-10-31 01:39:13.909468
751	69	AUGPARA	2493	czp10	2494	2020-10-31 01:40:05.653987	2020-10-31 01:40:05.653987
752	69	AUGPARA	2495	czp10	2496	2020-10-31 01:41:02.207137	2020-10-31 01:41:02.207137
753	69	AUGPARA	2497	czp10	2498	2020-10-31 01:43:39.390979	2020-10-31 01:43:39.390979
754	69	AUGPARA	2499	czp10	2500	2020-10-31 01:44:31.161169	2020-10-31 01:44:31.161169
755	69	AUGPARA	2501	czp10	2502	2020-10-31 01:45:26.264289	2020-10-31 01:45:26.264289
756	69	AUGPARA	2503	czp10	2504	2020-10-31 01:48:09.756851	2020-10-31 01:48:09.756851
757	69	AUGPARA	2505	czp10	2506	2020-10-31 01:50:16.814148	2020-10-31 01:50:16.814148
758	69	AUGPARA	2507	czp10	2508	2020-10-31 01:52:27.688378	2020-10-31 01:52:27.688378
759	69	AUGPARA	2509	czp10	2510	2020-10-31 01:52:36.938585	2020-10-31 01:52:36.938585
760	69	AUGPARA	2511	czp10	2512	2020-10-31 01:53:04.490033	2020-10-31 01:53:04.490033
761	69	AUGPARA	2513	czp10	2514	2020-10-31 01:54:11.335797	2020-10-31 01:54:11.335797
762	69	AUGPARA	2515	czp10	2516	2020-10-31 01:56:04.301654	2020-10-31 01:56:04.301654
763	69	AUGPARA	2517	czp10	2518	2020-10-31 01:56:37.785442	2020-10-31 01:56:37.785442
764	69	AUGPARA	2519	czp10	2520	2020-10-31 01:58:14.982552	2020-10-31 01:58:14.982552
765	69	AUGPARA	2521	czp10	2522	2020-10-31 02:03:25.600047	2020-10-31 02:03:25.600047
766	69	AUGPARA	2523	czp10	2524	2020-10-31 02:16:38.452704	2020-10-31 02:16:38.452704
767	69	AUGPARA	2525	czp10	2526	2020-10-31 02:29:24.937461	2020-10-31 02:29:24.937461
768	69	AUGPARA	2527	czp10	2528	2020-10-31 02:29:52.255628	2020-10-31 02:29:52.255628
769	69	AUGPARA	2529	czp10	2530	2020-10-31 02:33:50.219023	2020-10-31 02:33:50.219023
770	69	AUGPARA	2531	czp10	2532	2020-10-31 02:45:08.190475	2020-10-31 02:45:08.190475
771	69	AUGPARA	2533	czp10	2534	2020-10-31 02:45:38.242119	2020-10-31 02:45:38.242119
772	69	AUGPARA	2535	czp10	2536	2020-10-31 02:51:17.541199	2020-10-31 02:51:17.541199
773	69	AUGPARA	2537	czp10	2538	2020-10-31 03:00:45.57682	2020-10-31 03:00:45.57682
774	69	AUGPARA	2539	czp10	2540	2020-10-31 03:03:45.978844	2020-10-31 03:03:45.978844
775	69	AUGPARA	2541	czp10	2542	2020-10-31 03:09:32.523339	2020-10-31 03:09:32.523339
776	69	AUGPARA	2543	czp10	2544	2020-10-31 03:12:11.090323	2020-10-31 03:12:11.090323
777	69	AUGPARA	2545	czp10	2546	2020-10-31 03:15:38.293637	2020-10-31 03:15:38.293637
778	69	AUGPARA	2547	czp10	2548	2020-10-31 03:20:32.543484	2020-10-31 03:20:32.543484
779	69	AUGPARA	2549	czp10	2550	2020-10-31 03:24:36.182075	2020-10-31 03:24:36.182075
780	69	AUGPARA	2551	czp10	2552	2020-10-31 03:28:06.110563	2020-10-31 03:28:06.110563
781	69	AUGPARA	2553	czp10	2554	2020-10-31 03:29:15.71448	2020-10-31 03:29:15.71448
782	69	AUGPARA	2555	czp10	2556	2020-10-31 03:32:22.604436	2020-10-31 03:32:22.604436
783	69	AUGPARA	2557	czp10	2558	2020-10-31 03:33:15.56957	2020-10-31 03:33:15.56957
784	69	AUGPARA	2559	czp10	2560	2020-10-31 03:36:15.737902	2020-10-31 03:36:15.737902
785	69	AUGPARA	2561	czp10	2562	2020-10-31 03:36:49.273298	2020-10-31 03:36:49.273298
786	69	AUGPARA	2563	czp10	2564	2020-10-31 03:38:35.495865	2020-10-31 03:38:35.495865
787	69	AUGPARA	2565	czp10	2566	2020-10-31 03:41:24.296926	2020-10-31 03:41:24.296926
788	69	AUGPARA	2567	czp10	2568	2020-10-31 03:45:30.352301	2020-10-31 03:45:30.352301
789	69	AUGPARA	2569	czp10	2570	2020-10-31 03:50:39.094547	2020-10-31 03:50:39.094547
790	69	AUGPARA	2571	czp10	2572	2020-10-31 03:50:45.603329	2020-10-31 03:50:45.603329
791	69	AUGPARA	2573	czp10	2574	2020-10-31 03:51:54.115615	2020-10-31 03:51:54.115615
792	69	AUGPARA	2575	czp10	2576	2020-10-31 03:53:26.137517	2020-10-31 03:53:26.137517
793	69	AUGPARA	2577	czp10	2578	2020-10-31 03:55:09.243337	2020-10-31 03:55:09.243337
794	69	AUGPARA	2579	czp10	2580	2020-10-31 03:56:40.914371	2020-10-31 03:56:40.914371
795	69	AUGPARA	2581	czp10	2582	2020-10-31 03:57:27.726019	2020-10-31 03:57:27.726019
796	69	AUGPARA	2583	czp10	2584	2020-10-31 03:57:54.260726	2020-10-31 03:57:54.260726
797	69	AUGPARA	2585	czp10	2586	2020-10-31 03:59:19.123845	2020-10-31 03:59:19.123845
798	69	AUGPARA	2587	czp10	2588	2020-10-31 04:05:06.610724	2020-10-31 04:05:06.610724
799	69	AUGPARA	2589	czp10	2590	2020-10-31 04:07:42.47703	2020-10-31 04:07:42.47703
800	69	AUGPARA	2591	czp10	2592	2020-10-31 04:14:38.750239	2020-10-31 04:14:38.750239
801	69	AUGPARA	2593	czp10	2594	2020-10-31 04:15:08.26863	2020-10-31 04:15:08.26863
802	69	AUGPARA	2595	czp10	2596	2020-10-31 04:16:06.36352	2020-10-31 04:16:06.36352
803	69	AUGPARA	2597	czp10	2598	2020-10-31 04:18:45.389084	2020-10-31 04:18:45.389084
804	69	AUGPARA	2599	czp10	2600	2020-10-31 04:20:35.845693	2020-10-31 04:20:35.845693
805	69	AUGPARA	2601	czp10	2602	2020-10-31 04:21:25.314899	2020-10-31 04:21:25.314899
806	69	AUGPARA	2603	czp10	2604	2020-10-31 04:21:41.86571	2020-10-31 04:21:41.86571
807	69	AUGPARA	2605	czp10	2606	2020-10-31 04:23:15.620852	2020-10-31 04:23:15.620852
808	69	AUGPARA	2607	czp10	2608	2020-10-31 04:27:49.594538	2020-10-31 04:27:49.594538
809	69	AUGPARA	2609	czp10	2610	2020-10-31 04:30:08.110801	2020-10-31 04:30:08.110801
810	69	AUGPARA	2611	czp10	2612	2020-10-31 04:33:00.970301	2020-10-31 04:33:00.970301
811	69	AUGPARA	2613	czp10	2614	2020-10-31 04:38:01.012287	2020-10-31 04:38:01.012287
812	69	AUGPARA	2615	czp10	2616	2020-10-31 04:39:49.259987	2020-10-31 04:39:49.259987
813	69	AUGPARA	2617	czp10	2618	2020-10-31 04:41:26.157035	2020-10-31 04:41:26.157035
814	69	AUGPARA	2619	czp10	2620	2020-10-31 04:42:53.136246	2020-10-31 04:42:53.136246
815	69	AUGPARA	2621	czp10	2622	2020-10-31 04:48:40.665002	2020-10-31 04:48:40.665002
816	69	AUGPARA	2623	czp10	2624	2020-10-31 04:50:22.430654	2020-10-31 04:50:22.430654
817	69	AUGPARA	2625	czp10	2626	2020-10-31 04:52:00.567611	2020-10-31 04:52:00.567611
818	69	AUGPARA	2627	czp10	2628	2020-10-31 04:54:25.625809	2020-10-31 04:54:25.625809
819	69	AUGPARA	2629	czp10	2630	2020-10-31 04:55:30.004427	2020-10-31 04:55:30.004427
820	69	AUGPARA	2631	czp10	2632	2020-10-31 04:59:47.15197	2020-10-31 04:59:47.15197
821	69	AUGPARA	2633	czp10	2634	2020-10-31 04:59:55.544212	2020-10-31 04:59:55.544212
822	69	AUGPARA	2635	czp10	2636	2020-10-31 05:03:03.87974	2020-10-31 05:03:03.87974
823	69	AUGPARA	2637	czp10	2638	2020-10-31 05:04:35.684846	2020-10-31 05:04:35.684846
824	69	AUGPARA	2639	czp10	2640	2020-10-31 05:06:08.448507	2020-10-31 05:06:08.448507
825	69	AUGPARA	2641	czp10	2642	2020-10-31 05:06:33.433245	2020-10-31 05:06:33.433245
826	69	AUGPARA	2643	czp10	2644	2020-10-31 05:07:31.028196	2020-10-31 05:07:31.028196
827	69	AUGPARA	2645	czp10	2646	2020-10-31 05:09:22.826115	2020-10-31 05:09:22.826115
828	69	AUGPARA	2647	czp10	2648	2020-10-31 05:11:58.526641	2020-10-31 05:11:58.526641
829	69	AUGPARA	2649	czp10	2650	2020-10-31 05:18:35.70716	2020-10-31 05:18:35.70716
830	69	AUGPARA	2651	czp10	2652	2020-10-31 05:20:04.461933	2020-10-31 05:20:04.461933
831	69	AUGPARA	2653	czp10	2654	2020-10-31 05:21:10.707422	2020-10-31 05:21:10.707422
832	69	AUGPARA	2655	czp10	2656	2020-10-31 05:22:24.053016	2020-10-31 05:22:24.053016
833	69	AUGPARA	2657	czp10	2658	2020-10-31 05:26:56.534984	2020-10-31 05:26:56.534984
834	69	AUGPARA	2659	czp10	2660	2020-10-31 05:29:49.486267	2020-10-31 05:29:49.486267
835	69	AUGPARA	2661	czp10	2662	2020-10-31 05:30:18.696282	2020-10-31 05:30:18.696282
836	69	AUGPARA	2663	czp10	2664	2020-10-31 05:30:47.865107	2020-10-31 05:30:47.865107
837	69	AUGPARA	2665	czp10	2666	2020-10-31 05:32:31.271109	2020-10-31 05:32:31.271109
838	69	AUGPARA	2667	czp10	2668	2020-10-31 05:33:08.031334	2020-10-31 05:33:08.031334
839	69	AUGPARA	2669	czp10	2670	2020-10-31 05:33:22.815418	2020-10-31 05:33:22.815418
840	69	AUGPARA	2671	czp10	2672	2020-10-31 05:34:18.960087	2020-10-31 05:34:18.960087
841	69	AUGPARA	2673	czp10	2674	2020-10-31 05:34:34.344342	2020-10-31 05:34:34.344342
842	69	AUGPARA	2675	czp10	2676	2020-10-31 05:36:00.754572	2020-10-31 05:36:00.754572
843	69	AUGPARA	2677	czp10	2678	2020-10-31 05:38:10.88149	2020-10-31 05:38:10.88149
844	69	AUGPARA	2679	czp10	2680	2020-10-31 05:40:09.446327	2020-10-31 05:40:09.446327
845	69	AUGPARA	2681	czp10	2682	2020-10-31 05:43:28.507546	2020-10-31 05:43:28.507546
846	69	AUGPARA	2683	czp10	2684	2020-10-31 05:45:45.065156	2020-10-31 05:45:45.065156
847	69	AUGPARA	2685	czp10	2686	2020-10-31 05:47:10.894924	2020-10-31 05:47:10.894924
848	69	AUGPARA	2687	czp10	2688	2020-10-31 05:49:12.018466	2020-10-31 05:49:12.018466
849	69	AUGPARA	2689	czp10	2690	2020-10-31 05:50:31.222801	2020-10-31 05:50:31.222801
850	69	AUGPARA	2691	czp10	2692	2020-10-31 05:51:11.341668	2020-10-31 05:51:11.341668
851	69	AUGPARA	2693	czp10	2694	2020-10-31 05:52:56.409556	2020-10-31 05:52:56.409556
852	69	AUGPARA	2695	czp10	2696	2020-10-31 05:55:16.563754	2020-10-31 05:55:16.563754
853	69	AUGPARA	2697	czp10	2698	2020-10-31 05:57:03.545497	2020-10-31 05:57:03.545497
854	69	AUGPARA	2699	czp10	2700	2020-10-31 06:00:04.092629	2020-10-31 06:00:04.092629
855	69	AUGPARA	2701	czp10	2702	2020-10-31 06:01:26.776078	2020-10-31 06:01:26.776078
856	69	AUGPARA	2703	czp10	2704	2020-10-31 06:03:19.049192	2020-10-31 06:03:19.049192
857	69	AUGPARA	2705	czp10	2706	2020-10-31 06:04:35.378709	2020-10-31 06:04:35.378709
858	69	AUGPARA	2707	czp10	2708	2020-10-31 06:07:34.969826	2020-10-31 06:07:34.969826
859	69	AUGPARA	2709	czp10	2710	2020-10-31 06:08:28.041263	2020-10-31 06:08:28.041263
860	69	AUGPARA	2711	czp10	2712	2020-10-31 06:09:48.187446	2020-10-31 06:09:48.187446
861	69	AUGPARA	2713	czp10	2714	2020-10-31 06:12:04.778799	2020-10-31 06:12:04.778799
862	69	AUGPARA	2715	czp10	2716	2020-10-31 06:16:59.503222	2020-10-31 06:16:59.503222
863	69	AUGPARA	2717	czp10	2718	2020-10-31 06:17:48.330997	2020-10-31 06:17:48.330997
864	69	AUGPARA	2719	czp10	2720	2020-10-31 06:19:37.534749	2020-10-31 06:19:37.534749
865	69	AUGPARA	2721	czp10	2722	2020-10-31 06:21:21.287699	2020-10-31 06:21:21.287699
866	69	AUGPARA	2723	czp10	2724	2020-10-31 06:36:55.645884	2020-10-31 06:36:55.645884
867	69	AUGPARA	2725	czp10	2726	2020-10-31 06:40:22.306515	2020-10-31 06:40:22.306515
868	69	AUGPARA	2727	czp10	2728	2020-10-31 06:42:07.204061	2020-10-31 06:42:07.204061
869	69	AUGPARA	2729	czp10	2730	2020-10-31 06:43:00.551017	2020-10-31 06:43:00.551017
870	69	AUGPARA	2731	czp10	2732	2020-10-31 06:47:18.929775	2020-10-31 06:47:18.929775
871	69	AUGPARA	2733	czp10	2734	2020-10-31 06:52:51.256673	2020-10-31 06:52:51.256673
872	69	AUGPARA	2735	czp10	2736	2020-10-31 06:56:01.050726	2020-10-31 06:56:01.050726
873	69	AUGPARA	2737	czp10	2738	2020-10-31 07:04:42.030144	2020-10-31 07:04:42.030144
874	69	AUGPARA	2739	czp10	2740	2020-10-31 07:05:58.76743	2020-10-31 07:05:58.76743
875	69	AUGPARA	2741	czp10	2742	2020-10-31 07:07:04.179731	2020-10-31 07:07:04.179731
876	69	AUGPARA	2743	czp10	2744	2020-10-31 07:13:46.485216	2020-10-31 07:13:46.485216
877	69	AUGPARA	2745	czp10	2746	2020-10-31 07:14:13.470127	2020-10-31 07:14:13.470127
878	69	AUGPARA	2747	czp10	2748	2020-10-31 07:15:07.373113	2020-10-31 07:15:07.373113
879	69	AUGPARA	2749	czp10	2750	2020-10-31 07:17:28.664308	2020-10-31 07:17:28.664308
880	69	AUGPARA	2751	czp10	2752	2020-10-31 07:18:44.460186	2020-10-31 07:18:44.460186
881	69	AUGPARA	2753	czp10	2754	2020-10-31 07:19:20.728981	2020-10-31 07:19:20.728981
882	69	AUGPARA	2755	czp10	2756	2020-10-31 07:21:27.494694	2020-10-31 07:21:27.494694
883	69	AUGPARA	2757	czp10	2758	2020-10-31 07:26:16.445078	2020-10-31 07:26:16.445078
884	69	AUGPARA	2759	czp10	2760	2020-10-31 07:26:36.069899	2020-10-31 07:26:36.069899
885	69	AUGPARA	2761	czp10	2762	2020-10-31 07:27:24.514327	2020-10-31 07:27:24.514327
886	69	AUGPARA	2763	czp10	2764	2020-10-31 07:27:41.006795	2020-10-31 07:27:41.006795
887	69	AUGPARA	2765	czp10	2766	2020-10-31 07:28:15.400505	2020-10-31 07:28:15.400505
888	69	AUGPARA	2767	czp10	2768	2020-10-31 07:29:20.679294	2020-10-31 07:29:20.679294
889	69	AUGPARA	2769	czp10	2770	2020-10-31 07:42:22.206289	2020-10-31 07:42:22.206289
890	69	AUGPARA	2771	czp10	2772	2020-10-31 07:45:29.751867	2020-10-31 07:45:29.751867
891	69	AUGPARA	2773	czp10	2774	2020-10-31 07:48:03.525501	2020-10-31 07:48:03.525501
892	69	AUGPARA	2775	czp10	2776	2020-10-31 07:49:10.705468	2020-10-31 07:49:10.705468
893	69	AUGPARA	2777	czp10	2778	2020-10-31 07:50:23.683464	2020-10-31 07:50:23.683464
894	69	AUGPARA	2779	czp10	2780	2020-10-31 07:51:12.57064	2020-10-31 07:51:12.57064
895	69	AUGPARA	2781	czp10	2782	2020-10-31 08:01:41.054375	2020-10-31 08:01:41.054375
896	69	AUGPARA	2783	czp10	2784	2020-10-31 08:03:43.864192	2020-10-31 08:03:43.864192
897	69	AUGPARA	2785	czp10	2786	2020-10-31 08:05:34.693905	2020-10-31 08:05:34.693905
898	69	AUGPARA	2787	czp10	2788	2020-10-31 08:44:50.85882	2020-10-31 08:44:50.85882
899	69	AUGPARA	2789	czp10	2790	2020-10-31 08:49:03.39007	2020-10-31 08:49:03.39007
900	69	AUGPARA	2791	czp10	2792	2020-10-31 08:50:10.610145	2020-10-31 08:50:10.610145
901	69	AUGPARA	2793	czp10	2794	2020-10-31 09:18:52.530994	2020-10-31 09:18:52.530994
902	69	AUGPARA	2795	czp10	2796	2020-11-03 19:11:22.789326	2020-11-03 19:11:22.789326
903	69	AUGPARA	2797	czp10	2798	2020-11-03 19:11:32.281525	2020-11-03 19:11:32.281525
904	69	AUGPARA	2799	czp10	2800	2020-11-03 19:28:47.041908	2020-11-03 19:28:47.041908
905	69	AUGPARA	2801	czp10	2802	2020-11-03 19:45:34.292445	2020-11-03 19:45:34.292445
906	69	AUGPARA	2803	czp10	2804	2020-11-04 19:17:01.193412	2020-11-04 19:17:01.193412
907	69	AUGPARA	2805	czp10	2806	2020-11-04 19:23:34.083351	2020-11-04 19:23:34.083351
908	69	AUGPARA	2807	czp10	2808	2020-11-04 19:25:32.540444	2020-11-04 19:25:32.540444
909	69	AUGPARA	2809	czp10	2810	2020-11-04 19:27:30.107694	2020-11-04 19:27:30.107694
910	69	AUGPARA	2811	czp10	2812	2020-11-04 19:28:16.884756	2020-11-04 19:28:16.884756
911	69	AUGPARA	2813	czp10	2814	2020-11-04 19:30:26.430352	2020-11-04 19:30:26.430352
912	69	AUGPARA	2815	czp10	2816	2020-11-04 19:31:08.268781	2020-11-04 19:31:08.268781
913	69	AUGPARA	2817	czp10	2818	2020-11-04 19:37:05.048361	2020-11-04 19:37:05.048361
914	69	AUGPARA	2819	czp10	2820	2020-11-04 19:38:41.556818	2020-11-04 19:38:41.556818
915	69	AUGPARA	2821	czp10	2822	2020-11-04 19:42:26.134012	2020-11-04 19:42:26.134012
916	69	AUGPARA	2823	czp10	2824	2020-11-04 19:51:43.692113	2020-11-04 19:51:43.692113
917	69	AUGPARA	2825	czp10	2826	2020-11-05 00:11:28.980849	2020-11-05 00:11:28.980849
918	69	AUGPARA	2827	czp10	2828	2020-11-05 05:00:19.383714	2020-11-05 05:00:19.383714
919	69	AUGPARA	2829	czp10	2830	2020-11-05 05:09:27.582858	2020-11-05 05:09:27.582858
920	69	AUGPARA	2831	czp10	2832	2020-11-05 05:15:18.295615	2020-11-05 05:15:18.295615
921	69	AUGPARA	2833	czp10	2834	2020-11-05 05:17:01.301918	2020-11-05 05:17:01.301918
922	69	AUGPARA	2835	czp10	2836	2020-11-05 05:17:55.163102	2020-11-05 05:17:55.163102
923	69	AUGPARA	2837	czp10	2838	2020-11-05 05:19:47.695183	2020-11-05 05:19:47.695183
924	69	AUGPARA	2839	czp10	2840	2020-11-05 05:22:00.452825	2020-11-05 05:22:00.452825
925	69	AUGPARA	2841	czp10	2842	2020-11-05 05:23:56.709669	2020-11-05 05:23:56.709669
926	69	AUGPARA	2843	czp10	2844	2020-11-05 05:30:19.290719	2020-11-05 05:30:19.290719
927	69	AUGPARA	2845	czp10	2846	2020-11-05 05:31:59.246654	2020-11-05 05:31:59.246654
928	69	AUGPARA	2847	czp10	2848	2020-11-05 05:33:43.494496	2020-11-05 05:33:43.494496
929	69	AUGPARA	2849	czp10	2850	2020-11-05 05:48:14.246245	2020-11-05 05:48:14.246245
930	69	AUGPARA	2851	czp10	2852	2020-11-05 05:53:30.256731	2020-11-05 05:53:30.256731
931	69	AUGPARA	2853	czp10	2854	2020-11-05 06:01:00.341827	2020-11-05 06:01:00.341827
932	69	AUGPARA	2855	czp10	2856	2020-11-05 18:44:14.217365	2020-11-05 18:44:14.217365
933	69	AUGPARA	2857	czp10	2858	2020-11-05 19:12:05.272414	2020-11-05 19:12:05.272414
934	69	AUGPARA	2859	czp10	2860	2020-11-05 19:14:32.197832	2020-11-05 19:14:32.197832
935	69	AUGPARA	2861	czp10	2862	2020-11-05 19:39:45.336613	2020-11-05 19:39:45.336613
936	69	AUGPARA	2863	czp10	2864	2020-11-05 21:48:49.41246	2020-11-05 21:48:49.41246
937	69	AUGPARA	2865	czp10	2866	2020-11-05 21:48:57.260106	2020-11-05 21:48:57.260106
938	69	AUGPARA	2867	czp10	2868	2020-11-05 21:49:49.928412	2020-11-05 21:49:49.928412
939	69	AUGPARA	2869	czp10	2870	2020-11-05 21:51:35.211236	2020-11-05 21:51:35.211236
940	69	AUGPARA	2871	czp10	2872	2020-11-06 00:53:13.954505	2020-11-06 00:53:13.954505
941	69	AUGPARA	2873	czp10	2874	2020-11-06 01:32:14.926674	2020-11-06 01:32:14.926674
942	69	AUGPARA	2875	czp10	2876	2020-11-06 03:21:45.982288	2020-11-06 03:21:45.982288
943	69	AUGPARA	2877	czp10	2878	2020-11-07 02:21:55.879197	2020-11-07 02:21:55.879197
944	69	AUGPARA	2879	czp10	2880	2020-11-07 04:23:38.609156	2020-11-07 04:23:38.609156
945	69	AUGPARA	2881	czp10	2882	2020-11-07 04:30:13.067412	2020-11-07 04:30:13.067412
946	69	AUGPARA	2883	czp10	2884	2020-11-07 06:39:59.470031	2020-11-07 06:39:59.470031
947	69	AUGPARA	2885	czp10	2886	2020-11-07 07:30:19.948456	2020-11-07 07:30:19.948456
948	69	AUGPARA	2887	czp10	2888	2020-11-07 07:57:51.387641	2020-11-07 07:57:51.387641
949	69	AUGPARA	2889	czp10	2890	2020-11-07 08:46:09.783848	2020-11-07 08:46:09.783848
950	69	AUGPARA	2891	czp10	2892	2020-11-07 08:46:28.818419	2020-11-07 08:46:28.818419
951	69	AUGPARA	2893	czp10	2894	2020-11-07 23:36:09.592588	2020-11-07 23:36:09.592588
952	69	AUGPARA	2895	czp10	2896	2020-11-07 23:43:52.993936	2020-11-07 23:43:52.993936
953	69	AUGPARA	2897	czp10	2898	2020-11-07 23:51:24.566529	2020-11-07 23:51:24.566529
954	69	AUGPARA	2899	czp10	2900	2020-11-08 00:22:26.199741	2020-11-08 00:22:26.199741
955	69	AUGPARA	2901	czp10	2902	2020-11-08 00:38:38.860238	2020-11-08 00:38:38.860238
956	69	AUGPARA	2903	czp10	2904	2020-11-08 00:54:31.16161	2020-11-08 00:54:31.16161
957	69	AUGPARA	2905	czp10	2906	2020-11-08 01:09:04.257772	2020-11-08 01:09:04.257772
958	69	AUGPARA	2907	czp10	2908	2020-11-08 05:44:07.497527	2020-11-08 05:44:07.497527
959	69	AUGPARA	2909	czp10	2910	2020-11-09 19:42:46.755635	2020-11-09 19:42:46.755635
960	69	AUGPARA	2911	czp10	2912	2020-11-09 20:06:44.048048	2020-11-09 20:06:44.048048
961	69	AUGPARA	2913	czp10	2914	2020-11-09 20:12:42.32405	2020-11-09 20:12:42.32405
962	69	AUGPARA	2915	czp10	2916	2020-11-09 20:13:21.44668	2020-11-09 20:13:21.44668
963	69	AUGPARA	2917	czp10	2918	2020-11-10 01:23:27.550189	2020-11-10 01:23:27.550189
964	69	AUGPARA	2919	czp10	2920	2020-11-10 19:09:53.629139	2020-11-10 19:09:53.629139
965	69	AUGPARA	2921	czp10	2922	2020-11-10 19:13:04.607051	2020-11-10 19:13:04.607051
966	69	AUGPARA	2923	czp10	2924	2020-11-10 19:17:11.713254	2020-11-10 19:17:11.713254
967	69	AUGPARA	2925	czp10	2926	2020-11-10 19:21:13.327348	2020-11-10 19:21:13.327348
968	69	AUGPARA	2927	czp10	2928	2020-11-10 19:35:30.552464	2020-11-10 19:35:30.552464
969	69	AUGPARA	2929	czp10	2930	2020-11-10 19:38:01.020043	2020-11-10 19:38:01.020043
970	69	AUGPARA	2931	czp10	2932	2020-11-10 19:38:25.221178	2020-11-10 19:38:25.221178
971	69	AUGPARA	2933	czp10	2934	2020-11-10 19:38:47.460837	2020-11-10 19:38:47.460837
972	69	AUGPARA	2935	czp10	2936	2020-11-10 19:44:04.907778	2020-11-10 19:44:04.907778
973	69	AUGPARA	2937	czp10	2938	2020-11-10 19:45:45.73001	2020-11-10 19:45:45.73001
974	69	AUGPARA	2939	czp10	2940	2020-11-11 00:12:37.735236	2020-11-11 00:12:37.735236
975	69	AUGPARA	2941	czp10	2942	2020-11-11 00:14:13.799365	2020-11-11 00:14:13.799365
976	69	AUGPARA	2943	czp10	2944	2020-11-11 00:15:17.253467	2020-11-11 00:15:17.253467
977	69	AUGPARA	2945	czp10	2946	2020-11-11 00:17:36.523375	2020-11-11 00:17:36.523375
978	69	AUGPARA	2947	czp10	2948	2020-11-11 00:32:43.106884	2020-11-11 00:32:43.106884
979	69	AUGPARA	2949	czp10	2950	2020-11-11 00:35:16.399223	2020-11-11 00:35:16.399223
980	69	AUGPARA	2951	czp10	2952	2020-11-11 00:36:25.278405	2020-11-11 00:36:25.278405
981	69	AUGPARA	2953	czp10	2954	2020-11-11 00:42:22.899506	2020-11-11 00:42:22.899506
982	69	AUGPARA	2955	czp10	2956	2020-11-11 00:43:25.769856	2020-11-11 00:43:25.769856
983	69	AUGPARA	2957	czp10	2958	2020-11-11 01:02:26.845636	2020-11-11 01:02:26.845636
984	69	AUGPARA	2959	czp10	2960	2020-11-11 01:03:27.449492	2020-11-11 01:03:27.449492
985	69	AUGPARA	2961	czp10	2962	2020-11-11 01:05:33.656906	2020-11-11 01:05:33.656906
986	69	AUGPARA	2963	czp10	2964	2020-11-11 02:15:34.637865	2020-11-11 02:15:34.637865
987	69	AUGPARA	2965	czp10	2966	2020-11-11 02:17:54.47947	2020-11-11 02:17:54.47947
988	69	AUGPARA	2967	czp10	2968	2020-11-11 02:19:46.561333	2020-11-11 02:19:46.561333
989	69	AUGPARA	2969	czp10	2970	2020-11-11 02:20:02.603797	2020-11-11 02:20:02.603797
990	69	AUGPARA	2971	czp10	2972	2020-11-11 02:22:03.352634	2020-11-11 02:22:03.352634
991	69	AUGPARA	2973	czp10	2974	2020-11-11 02:25:19.41433	2020-11-11 02:25:19.41433
992	69	AUGPARA	2975	czp10	2976	2020-11-11 02:27:02.278651	2020-11-11 02:27:02.278651
993	69	AUGPARA	2977	czp10	2978	2020-11-11 05:05:05.42123	2020-11-11 05:05:05.42123
994	69	AUGPARA	2979	czp10	2980	2020-11-12 19:36:32.32108	2020-11-12 19:36:32.32108
995	69	AUGPARA	2981	czp10	2982	2020-11-12 19:49:54.969618	2020-11-12 19:49:54.969618
996	69	AUGPARA	2983	czp10	2984	2020-11-12 19:58:38.53495	2020-11-12 19:58:38.53495
997	69	AUGPARA	2985	czp10	2986	2020-11-12 20:00:33.57935	2020-11-12 20:00:33.57935
998	69	AUGPARA	2987	czp10	2988	2020-11-12 20:02:33.307433	2020-11-12 20:02:33.307433
999	69	AUGPARA	2989	czp10	2990	2020-11-12 20:10:29.094431	2020-11-12 20:10:29.094431
1000	69	AUGPARA	2991	czp10	2992	2020-11-12 20:18:08.447152	2020-11-12 20:18:08.447152
1001	69	AUGPARA	2993	czp10	2994	2020-11-12 20:18:46.992375	2020-11-12 20:18:46.992375
1002	69	AUGPARA	2995	czp10	2996	2020-11-12 20:21:26.438897	2020-11-12 20:21:26.438897
1003	69	AUGPARA	2997	czp10	2998	2020-11-12 20:22:34.356317	2020-11-12 20:22:34.356317
1004	69	AUGPARA	2999	czp10	3000	2020-11-12 20:30:55.419739	2020-11-12 20:30:55.419739
1005	69	AUGPARA	3001	czp10	3002	2020-11-12 20:33:05.452909	2020-11-12 20:33:05.452909
1006	69	AUGPARA	3003	czp10	3004	2020-11-12 20:34:23.782448	2020-11-12 20:34:23.782448
1007	69	AUGPARA	3005	czp10	3006	2020-11-13 17:26:36.054137	2020-11-13 17:26:36.054137
1008	69	AUGPARA	3007	czp10	3008	2020-11-13 17:47:13.210922	2020-11-13 17:47:13.210922
1009	69	AUGPARA	3009	czp10	3010	2020-11-13 18:50:39.420784	2020-11-13 18:50:39.420784
1010	69	AUGPARA	3011	czp10	3012	2020-11-13 19:03:02.03969	2020-11-13 19:03:02.03969
1011	69	AUGPARA	3013	czp10	3014	2020-11-13 19:11:56.088095	2020-11-13 19:11:56.088095
1012	69	AUGPARA	3015	czp10	3016	2020-11-13 19:13:03.483831	2020-11-13 19:13:03.483831
1013	69	AUGPARA	3017	czp10	3018	2020-11-13 19:22:14.675095	2020-11-13 19:22:14.675095
1014	69	AUGPARA	3019	czp10	3020	2020-11-13 19:25:23.786468	2020-11-13 19:25:23.786468
1015	69	AUGPARA	3021	czp10	3022	2020-11-13 19:29:56.311335	2020-11-13 19:29:56.311335
1016	69	AUGPARA	3023	czp10	3024	2020-11-13 19:36:21.051084	2020-11-13 19:36:21.051084
1017	69	AUGPARA	3025	czp10	3026	2020-11-13 19:53:48.554607	2020-11-13 19:53:48.554607
1018	69	AUGPARA	3027	czp10	3028	2020-11-13 20:04:47.410741	2020-11-13 20:04:47.410741
1019	69	AUGPARA	3029	czp10	3030	2020-11-13 20:06:14.89078	2020-11-13 20:06:14.89078
1020	69	AUGPARA	3031	czp10	3032	2020-11-13 20:18:32.059746	2020-11-13 20:18:32.059746
1021	69	AUGPARA	3033	czp10	3034	2020-11-13 21:17:18.761197	2020-11-13 21:17:18.761197
1022	69	AUGPARA	3035	czp10	3036	2020-11-14 00:20:57.624109	2020-11-14 00:20:57.624109
1023	69	AUGPARA	3037	czp10	3038	2020-11-15 12:53:58.043628	2020-11-15 12:53:58.043628
1024	69	AUGPARA	3039	czp10	3040	2020-11-16 01:17:14.471842	2020-11-16 01:17:14.471842
1025	69	AUGPARA	3041	czp10	3042	2020-11-16 03:57:54.163188	2020-11-16 03:57:54.163188
1026	69	AUGPARA	3043	czp10	3044	2020-11-16 04:54:22.351986	2020-11-16 04:54:22.351986
1027	69	AUGPARA	3045	czp10	3046	2020-11-16 04:59:45.477421	2020-11-16 04:59:45.477421
1028	69	AUGPARA	3047	czp10	3048	2020-11-16 05:11:03.449572	2020-11-16 05:11:03.449572
1029	69	AUGPARA	3049	czp10	3050	2020-11-16 05:13:45.371142	2020-11-16 05:13:45.371142
1030	69	AUGPARA	3051	czp10	3052	2020-11-16 18:36:49.102318	2020-11-16 18:36:49.102318
1031	69	AUGPARA	3053	czp10	3054	2020-11-16 18:55:11.191063	2020-11-16 18:55:11.191063
1032	69	AUGPARA	3055	czp10	3056	2020-11-16 19:48:07.486181	2020-11-16 19:48:07.486181
1033	69	AUGPARA	3057	czp10	3058	2020-11-16 19:49:19.532252	2020-11-16 19:49:19.532252
1034	69	AUGPARA	3059	czp10	3060	2020-11-16 20:16:17.219241	2020-11-16 20:16:17.219241
1035	69	AUGPARA	3061	czp10	3062	2020-11-16 20:43:07.556248	2020-11-16 20:43:07.556248
1036	69	AUGPARA	3063	czp10	3064	2020-11-16 21:36:55.663374	2020-11-16 21:36:55.663374
1037	69	AUGPARA	3065	czp10	3066	2020-11-16 21:45:17.910025	2020-11-16 21:45:17.910025
1038	69	AUGPARA	3067	czp10	3068	2020-11-16 21:46:22.655225	2020-11-16 21:46:22.655225
1039	69	AUGPARA	3069	czp10	3070	2020-11-16 21:55:30.912776	2020-11-16 21:55:30.912776
1040	69	AUGPARA	3071	czp10	3072	2020-11-16 22:01:34.894589	2020-11-16 22:01:34.894589
1041	69	AUGPARA	3073	czp10	3074	2020-11-16 22:04:38.478493	2020-11-16 22:04:38.478493
1042	69	AUGPARA	3075	czp10	3076	2020-11-16 22:09:53.063823	2020-11-16 22:09:53.063823
1043	69	AUGPARA	3077	czp10	3078	2020-11-16 22:14:02.620346	2020-11-16 22:14:02.620346
1044	69	AUGPARA	3079	czp10	3080	2020-11-16 22:14:33.288973	2020-11-16 22:14:33.288973
1045	69	AUGPARA	3081	czp10	3082	2020-11-16 22:17:38.140263	2020-11-16 22:17:38.140263
1046	69	AUGPARA	3083	czp10	3084	2020-11-16 23:16:24.826461	2020-11-16 23:16:24.826461
1047	69	AUGPARA	3085	czp10	3086	2020-11-17 00:00:06.673229	2020-11-17 00:00:06.673229
1048	69	AUGPARA	3087	czp10	3088	2020-11-17 00:17:16.47599	2020-11-17 00:17:16.47599
1049	69	AUGPARA	3089	czp10	3090	2020-11-17 00:26:56.918561	2020-11-17 00:26:56.918561
1050	69	AUGPARA	3091	czp10	3092	2020-11-17 00:27:23.019898	2020-11-17 00:27:23.019898
1051	69	AUGPARA	3093	czp10	3094	2020-11-17 00:30:07.19684	2020-11-17 00:30:07.19684
1052	69	AUGPARA	3095	czp10	3096	2020-11-17 18:50:04.135928	2020-11-17 18:50:04.135928
1053	69	AUGPARA	3097	czp10	3098	2020-11-17 18:51:24.882352	2020-11-17 18:51:24.882352
1054	69	AUGPARA	3099	czp10	3100	2020-11-17 19:33:51.855036	2020-11-17 19:33:51.855036
1055	69	AUGPARA	3101	czp10	3102	2020-11-17 20:14:14.418422	2020-11-17 20:14:14.418422
1056	69	AUGPARA	3103	czp10	3104	2020-11-17 20:18:09.740832	2020-11-17 20:18:09.740832
1057	69	AUGPARA	3105	czp10	3106	2020-11-18 01:43:32.713684	2020-11-18 01:43:32.713684
1058	69	AUGPARA	3107	czp10	3108	2020-11-18 01:47:38.603433	2020-11-18 01:47:38.603433
1059	69	AUGPARA	3109	czp10	3110	2020-11-18 20:52:40.47891	2020-11-18 20:52:40.47891
1060	69	AUGPARA	3111	czp10	3112	2020-11-18 21:05:11.798308	2020-11-18 21:05:11.798308
1061	69	AUGPARA	3113	czp10	3114	2020-11-18 22:54:39.245471	2020-11-18 22:54:39.245471
1062	69	AUGPARA	3115	czp10	3116	2020-11-18 22:59:36.113781	2020-11-18 22:59:36.113781
1063	69	AUGPARA	3117	czp10	3118	2020-11-18 23:27:45.071863	2020-11-18 23:27:45.071863
1064	69	AUGPARA	3119	czp10	3120	2020-11-18 23:29:24.20253	2020-11-18 23:29:24.20253
\.


--
-- Data for Name: class_psyop; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.class_psyop (psyop_id, psyop_player_id, psyop_primary_type, psyop_primary_weapon_id, psyop_secondary_type, psyop_secondary_weapon_id, created_at, updated_at) FROM stdin;
\.


--
-- Data for Name: class_sentinel; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.class_sentinel (sentinel_id, sentinel_player_id, sentinel_primary_type, sentinel_primary_weapon_id, sentinel_secondary_type, sentinel_secondary_weapon_id, sentinel_heal_level, sentinel_intimidate_level, created_at, updated_at, sentinel_human_shield_level, sentinel_deny_entry_level, sentinel_gadget_shield_level, sentinel_oblique_kick_level) FROM stdin;
1	25	MP5	0	czp10	0	0	0	2020-02-22 09:27:31.743533	2020-02-22 09:27:31.743533	0	0	0	0
2	26	MP5	8	czp10	9	0	0	2020-02-22 09:33:13.413894	2020-02-22 09:33:13.413894	0	0	0	0
3	27	MP5	10	czp10	11	0	0	2020-02-22 09:55:51.718576	2020-02-22 09:55:51.718576	0	0	0	0
4	28	MP5	12	czp10	13	0	0	2020-02-22 10:00:14.951148	2020-02-22 10:00:14.951148	0	0	0	0
5	29	MP5	14	czp10	15	0	0	2020-02-22 10:02:09.757803	2020-02-22 10:02:09.757803	0	0	0	0
6	30	MP5	16	czp10	17	0	0	2020-02-22 10:20:33.314134	2020-02-22 10:20:33.314134	0	0	0	0
7	31	SASG12	0	czp10	18	0	0	2020-02-22 10:39:57.649349	2020-02-22 10:39:57.649349	0	0	0	0
8	32	MP5	19	czp10	20	0	0	2020-02-25 05:47:34.925122	2020-02-25 05:47:34.925122	0	0	0	0
9	33	SASG12	21	czp10	22	0	0	2020-02-28 23:38:10.386851	2020-02-28 23:38:10.386851	0	0	0	0
10	83	MP5	663	czp10	664	0	0	2020-09-12 02:10:31.817151	2020-09-12 02:10:31.817151	0	0	0	0
\.


--
-- Data for Name: class_sniper; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.class_sniper (sniper_id, sniper_player_id, sniper_primary_type, sniper_primary_weapon_id, sniper_secondary_type, sniper_secondary_weapon_id, created_at, updated_at) FROM stdin;
51	69	PSG1	229	czp10	230	2020-09-03 04:21:50.769938	2020-09-03 04:21:50.769938
52	69	PSG1	233	czp10	234	2020-09-03 04:27:19.322538	2020-09-03 04:27:19.322538
53	69	PSG1	237	czp10	238	2020-09-03 04:28:51.528049	2020-09-03 04:28:51.528049
54	69	PSG1	241	czp10	242	2020-09-03 04:35:12.108666	2020-09-03 04:35:12.108666
55	69	PSG1	245	czp10	246	2020-09-03 04:35:27.72628	2020-09-03 04:35:27.72628
56	69	PSG1	249	czp10	250	2020-09-03 04:37:11.074147	2020-09-03 04:37:11.074147
57	69	PSG1	253	czp10	254	2020-09-03 04:39:52.158641	2020-09-03 04:39:52.158641
58	69	PSG1	0	czp10	257	2020-09-03 04:42:32.643491	2020-09-03 04:42:32.643491
59	69	PSG1	0	czp10	260	2020-09-03 04:44:56.026692	2020-09-03 04:44:56.026692
60	69	PSG1	0	czp10	263	2020-09-03 04:46:48.008683	2020-09-03 04:46:48.008683
61	69	PSG1	0	czp10	266	2020-09-03 04:49:57.519562	2020-09-03 04:49:57.519562
62	69	PSG1	0	czp10	269	2020-09-03 04:52:07.777171	2020-09-03 04:52:07.777171
63	69	PSG1	0	czp10	272	2020-09-03 04:53:29.373657	2020-09-03 04:53:29.373657
64	69	PSG1	0	czp10	275	2020-09-03 04:58:29.97452	2020-09-03 04:58:29.97452
65	69	PSG1	0	czp10	278	2020-09-03 05:09:49.081539	2020-09-03 05:09:49.081539
29	69	PSG1	143	czp10	144	2020-09-02 22:26:14.921936	2020-09-02 22:26:14.921936
30	69	PSG1	147	czp10	148	2020-09-02 23:00:57.668779	2020-09-02 23:00:57.668779
31	69	PSG1	151	czp10	152	2020-09-02 23:39:44.180474	2020-09-02 23:39:44.180474
32	69	PSG1	155	czp10	156	2020-09-02 23:47:29.791494	2020-09-02 23:47:29.791494
33	69	PSG1	159	czp10	160	2020-09-02 23:49:23.898474	2020-09-02 23:49:23.898474
34	69	PSG1	163	czp10	164	2020-09-03 00:43:24.638864	2020-09-03 00:43:24.638864
35	81	PSG1	167	czp10	168	2020-09-03 00:44:02.224481	2020-09-03 00:44:02.224481
36	69	PSG1	169	czp10	170	2020-09-03 00:48:00.088574	2020-09-03 00:48:00.088574
37	69	PSG1	173	czp10	174	2020-09-03 01:49:32.405232	2020-09-03 01:49:32.405232
38	69	PSG1	177	czp10	178	2020-09-03 02:16:33.750815	2020-09-03 02:16:33.750815
39	69	PSG1	181	czp10	182	2020-09-03 02:16:50.743501	2020-09-03 02:16:50.743501
40	69	PSG1	185	czp10	186	2020-09-03 02:18:42.400007	2020-09-03 02:18:42.400007
41	69	PSG1	189	czp10	190	2020-09-03 02:49:28.225111	2020-09-03 02:49:28.225111
42	69	PSG1	193	czp10	194	2020-09-03 02:49:51.6266	2020-09-03 02:49:51.6266
43	69	PSG1	197	czp10	198	2020-09-03 02:55:45.880743	2020-09-03 02:55:45.880743
44	69	PSG1	201	czp10	202	2020-09-03 03:00:28.197535	2020-09-03 03:00:28.197535
45	69	PSG1	205	czp10	206	2020-09-03 03:01:46.977127	2020-09-03 03:01:46.977127
46	69	PSG1	209	czp10	210	2020-09-03 03:13:13.567581	2020-09-03 03:13:13.567581
47	69	PSG1	213	czp10	214	2020-09-03 03:15:00.765609	2020-09-03 03:15:00.765609
48	69	PSG1	217	czp10	218	2020-09-03 03:19:29.289702	2020-09-03 03:19:29.289702
49	69	PSG1	221	czp10	222	2020-09-03 03:41:14.734564	2020-09-03 03:41:14.734564
50	69	PSG1	225	czp10	226	2020-09-03 04:04:21.815965	2020-09-03 04:04:21.815965
66	69	PSG1	281	czp10	282	2020-09-03 05:14:56.666368	2020-09-03 05:14:56.666368
67	69	PSG1	285	czp10	286	2020-09-03 05:20:42.712004	2020-09-03 05:20:42.712004
68	69	PSG1	289	czp10	290	2020-09-03 05:22:48.230164	2020-09-03 05:22:48.230164
69	69	PSG1	293	czp10	294	2020-09-03 05:26:52.392006	2020-09-03 05:26:52.392006
70	69	PSG1	297	czp10	298	2020-09-03 05:29:51.169269	2020-09-03 05:29:51.169269
71	69	PSG1	301	czp10	302	2020-09-03 05:33:00.872111	2020-09-03 05:33:00.872111
72	69	PSG1	305	czp10	306	2020-09-03 05:34:36.836573	2020-09-03 05:34:36.836573
73	69	PSG1	309	czp10	310	2020-09-03 05:44:54.272867	2020-09-03 05:44:54.272867
74	69	PSG1	313	czp10	314	2020-09-03 07:06:46.383703	2020-09-03 07:06:46.383703
75	69	PSG1	317	czp10	318	2020-09-03 07:13:37.678699	2020-09-03 07:13:37.678699
76	69	PSG1	321	czp10	322	2020-09-03 07:47:43.410619	2020-09-03 07:47:43.410619
77	69	PSG1	325	czp10	326	2020-09-05 01:34:31.717825	2020-09-05 01:34:31.717825
78	69	PSG1	329	czp10	330	2020-09-05 01:48:40.635625	2020-09-05 01:48:40.635625
79	69	PSG1	333	czp10	334	2020-09-05 02:17:06.184466	2020-09-05 02:17:06.184466
80	69	PSG1	337	czp10	338	2020-09-05 02:48:10.660747	2020-09-05 02:48:10.660747
81	69	PSG1	341	czp10	342	2020-09-05 02:53:12.803738	2020-09-05 02:53:12.803738
82	69	PSG1	345	czp10	346	2020-09-05 03:15:09.072525	2020-09-05 03:15:09.072525
83	69	PSG1	349	czp10	350	2020-09-05 03:18:24.284177	2020-09-05 03:18:24.284177
84	69	PSG1	353	czp10	354	2020-09-05 03:21:08.86056	2020-09-05 03:21:08.86056
85	69	PSG1	357	czp10	358	2020-09-05 03:21:43.91282	2020-09-05 03:21:43.91282
86	69	PSG1	361	czp10	362	2020-09-05 03:22:21.264851	2020-09-05 03:22:21.264851
87	69	PSG1	365	czp10	366	2020-09-06 00:53:49.686993	2020-09-06 00:53:49.686993
88	69	PSG1	369	czp10	370	2020-09-06 00:59:59.400464	2020-09-06 00:59:59.400464
89	69	PSG1	373	czp10	374	2020-09-06 01:06:13.147404	2020-09-06 01:06:13.147404
90	69	PSG1	377	czp10	378	2020-09-06 01:13:48.907787	2020-09-06 01:13:48.907787
91	69	PSG1	381	czp10	382	2020-09-06 01:18:55.419821	2020-09-06 01:18:55.419821
92	69	PSG1	385	czp10	386	2020-09-06 01:22:43.600024	2020-09-06 01:22:43.600024
93	69	PSG1	389	czp10	390	2020-09-06 01:27:02.107046	2020-09-06 01:27:02.107046
94	69	PSG1	393	czp10	394	2020-09-06 01:29:03.639099	2020-09-06 01:29:03.639099
95	69	PSG1	397	czp10	398	2020-09-06 02:15:03.901108	2020-09-06 02:15:03.901108
96	69	PSG1	401	czp10	402	2020-09-06 02:25:01.036182	2020-09-06 02:25:01.036182
97	69	PSG1	405	czp10	406	2020-09-06 02:32:29.254228	2020-09-06 02:32:29.254228
98	69	PSG1	409	czp10	410	2020-09-06 02:42:52.449344	2020-09-06 02:42:52.449344
99	69	PSG1	413	czp10	414	2020-09-06 03:38:25.686257	2020-09-06 03:38:25.686257
100	69	PSG1	417	czp10	418	2020-09-06 03:43:57.997487	2020-09-06 03:43:57.997487
101	69	PSG1	421	czp10	422	2020-09-06 03:44:52.317571	2020-09-06 03:44:52.317571
102	69	PSG1	425	czp10	426	2020-09-07 02:20:18.206137	2020-09-07 02:20:18.206137
103	69	PSG1	429	czp10	430	2020-09-07 02:42:23.167187	2020-09-07 02:42:23.167187
104	69	PSG1	433	czp10	434	2020-09-07 03:10:17.640374	2020-09-07 03:10:17.640374
105	69	PSG1	437	czp10	438	2020-09-07 03:17:23.048724	2020-09-07 03:17:23.048724
106	69	PSG1	441	czp10	442	2020-09-07 04:16:38.231865	2020-09-07 04:16:38.231865
107	69	PSG1	445	czp10	446	2020-09-07 04:21:27.915795	2020-09-07 04:21:27.915795
108	69	PSG1	449	czp10	450	2020-09-07 04:43:58.71991	2020-09-07 04:43:58.71991
109	69	PSG1	453	czp10	454	2020-09-07 05:00:05.576753	2020-09-07 05:00:05.576753
110	69	PSG1	457	czp10	458	2020-09-07 05:00:29.294681	2020-09-07 05:00:29.294681
111	69	PSG1	461	czp10	462	2020-09-07 05:04:50.268762	2020-09-07 05:04:50.268762
112	69	PSG1	465	czp10	466	2020-09-07 05:12:51.371794	2020-09-07 05:12:51.371794
113	69	PSG1	469	czp10	470	2020-09-07 05:40:07.584516	2020-09-07 05:40:07.584516
114	69	PSG1	473	czp10	474	2020-09-07 06:18:01.042805	2020-09-07 06:18:01.042805
115	69	PSG1	477	czp10	478	2020-09-07 06:43:27.015204	2020-09-07 06:43:27.015204
116	69	PSG1	481	czp10	482	2020-09-07 07:30:53.431564	2020-09-07 07:30:53.431564
117	69	PSG1	485	czp10	486	2020-09-08 22:52:13.929422	2020-09-08 22:52:13.929422
118	69	PSG1	489	czp10	490	2020-09-08 23:14:31.081785	2020-09-08 23:14:31.081785
119	69	PSG1	493	czp10	494	2020-09-08 23:16:19.096649	2020-09-08 23:16:19.096649
120	69	PSG1	497	czp10	498	2020-09-09 00:39:35.536987	2020-09-09 00:39:35.536987
121	69	PSG1	501	czp10	502	2020-09-09 00:54:15.671719	2020-09-09 00:54:15.671719
122	69	PSG1	505	czp10	506	2020-09-09 00:58:27.903541	2020-09-09 00:58:27.903541
123	69	PSG1	509	czp10	510	2020-09-09 01:05:29.640914	2020-09-09 01:05:29.640914
124	69	PSG1	513	czp10	514	2020-09-09 22:41:50.772439	2020-09-09 22:41:50.772439
125	69	PSG1	517	czp10	518	2020-09-09 22:48:33.096111	2020-09-09 22:48:33.096111
126	69	PSG1	521	czp10	522	2020-09-09 22:58:44.323717	2020-09-09 22:58:44.323717
127	69	PSG1	525	czp10	526	2020-09-09 22:59:21.342707	2020-09-09 22:59:21.342707
128	69	PSG1	529	czp10	530	2020-09-09 23:00:31.705367	2020-09-09 23:00:31.705367
129	69	PSG1	533	czp10	534	2020-09-09 23:01:13.074125	2020-09-09 23:01:13.074125
130	69	PSG1	537	czp10	538	2020-09-09 23:03:20.948231	2020-09-09 23:03:20.948231
131	69	PSG1	541	czp10	542	2020-09-09 23:05:11.896458	2020-09-09 23:05:11.896458
132	69	PSG1	545	czp10	546	2020-09-09 23:10:00.854944	2020-09-09 23:10:00.854944
133	69	PSG1	549	czp10	550	2020-09-09 23:12:32.680466	2020-09-09 23:12:32.680466
134	69	PSG1	553	czp10	554	2020-09-10 19:53:49.231833	2020-09-10 19:53:49.231833
135	69	PSG1	557	czp10	558	2020-09-10 19:55:31.879484	2020-09-10 19:55:31.879484
136	69	PSG1	561	czp10	562	2020-09-10 19:56:45.833851	2020-09-10 19:56:45.833851
137	69	PSG1	565	czp10	566	2020-09-10 20:02:54.63082	2020-09-10 20:02:54.63082
138	69	PSG1	569	czp10	570	2020-09-10 20:04:04.810081	2020-09-10 20:04:04.810081
139	69	PSG1	573	czp10	574	2020-09-10 20:05:52.449684	2020-09-10 20:05:52.449684
140	69	PSG1	577	czp10	578	2020-09-10 20:19:56.107413	2020-09-10 20:19:56.107413
141	69	PSG1	581	czp10	582	2020-09-10 20:20:58.536127	2020-09-10 20:20:58.536127
142	69	PSG1	585	czp10	586	2020-09-10 20:24:06.09737	2020-09-10 20:24:06.09737
143	69	PSG1	589	czp10	590	2020-09-10 20:25:00.708936	2020-09-10 20:25:00.708936
144	69	PSG1	593	czp10	594	2020-09-10 20:25:46.961711	2020-09-10 20:25:46.961711
145	69	PSG1	597	czp10	598	2020-09-10 20:26:43.439988	2020-09-10 20:26:43.439988
146	69	PSG1	601	czp10	602	2020-09-10 20:27:14.350218	2020-09-10 20:27:14.350218
147	69	PSG1	605	czp10	606	2020-09-10 20:29:07.023737	2020-09-10 20:29:07.023737
148	69	PSG1	609	czp10	610	2020-09-10 20:30:13.55313	2020-09-10 20:30:13.55313
149	69	PSG1	613	czp10	614	2020-09-10 20:43:20.032117	2020-09-10 20:43:20.032117
150	69	PSG1	617	czp10	618	2020-09-10 20:51:30.088944	2020-09-10 20:51:30.088944
151	69	PSG1	621	czp10	622	2020-09-11 19:24:41.957001	2020-09-11 19:24:41.957001
152	82	PSG1	625	czp10	626	2020-09-11 19:25:57.586414	2020-09-11 19:25:57.586414
153	69	PSG1	627	czp10	628	2020-09-11 19:53:15.822958	2020-09-11 19:53:15.822958
154	69	PSG1	631	czp10	632	2020-09-11 19:54:02.117219	2020-09-11 19:54:02.117219
155	69	PSG1	635	czp10	636	2020-09-11 19:56:14.424846	2020-09-11 19:56:14.424846
156	69	PSG1	639	czp10	640	2020-09-11 20:03:08.282182	2020-09-11 20:03:08.282182
157	69	PSG1	643	czp10	644	2020-09-12 01:25:33.941502	2020-09-12 01:25:33.941502
158	69	PSG1	647	czp10	648	2020-09-12 01:38:42.920894	2020-09-12 01:38:42.920894
159	69	PSG1	651	czp10	652	2020-09-12 01:41:43.156686	2020-09-12 01:41:43.156686
160	69	PSG1	655	czp10	656	2020-09-12 01:48:43.356366	2020-09-12 01:48:43.356366
161	69	PSG1	659	czp10	660	2020-09-12 01:53:16.681757	2020-09-12 01:53:16.681757
162	69	PSG1	665	czp10	666	2020-09-12 02:41:36.009119	2020-09-12 02:41:36.009119
163	69	PSG1	669	czp10	670	2020-09-12 02:44:04.992991	2020-09-12 02:44:04.992991
164	69	PSG1	673	czp10	674	2020-09-12 05:11:58.328447	2020-09-12 05:11:58.328447
165	69	PSG1	677	czp10	678	2020-09-12 06:48:19.983385	2020-09-12 06:48:19.983385
166	69	PSG1	681	czp10	682	2020-09-12 07:48:52.22069	2020-09-12 07:48:52.22069
167	69	PSG1	685	czp10	686	2020-09-12 07:53:16.7459	2020-09-12 07:53:16.7459
168	69	PSG1	689	czp10	690	2020-09-12 07:53:50.346648	2020-09-12 07:53:50.346648
169	69	PSG1	693	czp10	694	2020-09-12 08:02:36.044212	2020-09-12 08:02:36.044212
170	69	PSG1	697	czp10	698	2020-09-12 08:13:51.66703	2020-09-12 08:13:51.66703
171	69	PSG1	701	czp10	702	2020-09-12 08:19:55.288494	2020-09-12 08:19:55.288494
172	69	PSG1	705	czp10	706	2020-09-12 08:23:24.209137	2020-09-12 08:23:24.209137
173	69	PSG1	709	czp10	710	2020-09-12 08:29:07.346057	2020-09-12 08:29:07.346057
174	69	PSG1	713	czp10	714	2020-09-12 08:38:38.538342	2020-09-12 08:38:38.538342
175	69	PSG1	717	czp10	718	2020-09-12 08:39:23.416127	2020-09-12 08:39:23.416127
176	69	PSG1	721	czp10	722	2020-09-12 08:45:59.331104	2020-09-12 08:45:59.331104
177	69	PSG1	725	czp10	726	2020-09-12 08:53:57.075795	2020-09-12 08:53:57.075795
178	69	PSG1	729	czp10	730	2020-09-12 08:56:05.892384	2020-09-12 08:56:05.892384
179	69	PSG1	733	czp10	734	2020-09-12 08:56:53.161339	2020-09-12 08:56:53.161339
180	69	PSG1	737	czp10	738	2020-09-12 09:08:08.001071	2020-09-12 09:08:08.001071
181	69	PSG1	741	czp10	742	2020-09-12 09:08:56.93741	2020-09-12 09:08:56.93741
182	69	PSG1	745	czp10	746	2020-09-12 09:13:07.127085	2020-09-12 09:13:07.127085
183	69	PSG1	749	czp10	750	2020-09-12 09:33:08.214158	2020-09-12 09:33:08.214158
184	69	PSG1	753	czp10	754	2020-09-12 09:35:39.831257	2020-09-12 09:35:39.831257
185	69	PSG1	757	czp10	758	2020-09-12 10:13:30.647623	2020-09-12 10:13:30.647623
186	69	PSG1	761	czp10	762	2020-09-12 10:41:58.414332	2020-09-12 10:41:58.414332
187	69	PSG1	765	czp10	766	2020-09-12 10:45:42.085963	2020-09-12 10:45:42.085963
188	69	PSG1	769	czp10	770	2020-09-12 11:14:30.262712	2020-09-12 11:14:30.262712
189	69	PSG1	773	czp10	774	2020-09-12 11:35:55.046278	2020-09-12 11:35:55.046278
190	69	PSG1	777	czp10	778	2020-09-12 11:38:41.672698	2020-09-12 11:38:41.672698
191	69	PSG1	781	czp10	782	2020-09-12 11:47:24.445506	2020-09-12 11:47:24.445506
192	69	PSG1	785	czp10	786	2020-09-12 11:49:07.568161	2020-09-12 11:49:07.568161
193	69	PSG1	789	czp10	790	2020-09-12 11:52:26.997066	2020-09-12 11:52:26.997066
194	69	PSG1	793	czp10	794	2020-09-12 12:14:13.273398	2020-09-12 12:14:13.273398
195	69	PSG1	797	czp10	798	2020-09-12 14:04:12.167418	2020-09-12 14:04:12.167418
196	69	PSG1	801	czp10	802	2020-09-12 14:14:16.361126	2020-09-12 14:14:16.361126
197	69	PSG1	805	czp10	806	2020-09-12 14:15:34.78252	2020-09-12 14:15:34.78252
198	69	PSG1	809	czp10	810	2020-09-12 14:20:13.123955	2020-09-12 14:20:13.123955
199	69	PSG1	813	czp10	814	2020-09-12 14:22:26.831769	2020-09-12 14:22:26.831769
200	69	PSG1	817	czp10	818	2020-09-12 14:55:22.263036	2020-09-12 14:55:22.263036
201	69	PSG1	821	czp10	822	2020-09-12 16:44:38.727176	2020-09-12 16:44:38.727176
202	69	PSG1	825	czp10	826	2020-09-12 17:55:32.553255	2020-09-12 17:55:32.553255
203	69	PSG1	829	czp10	830	2020-09-12 19:20:46.323528	2020-09-12 19:20:46.323528
204	69	PSG1	833	czp10	834	2020-09-15 23:25:40.200364	2020-09-15 23:25:40.200364
205	69	PSG1	837	czp10	838	2020-09-15 23:43:10.802578	2020-09-15 23:43:10.802578
206	69	PSG1	841	czp10	842	2020-09-15 23:43:25.01179	2020-09-15 23:43:25.01179
207	69	PSG1	845	czp10	846	2020-09-15 23:50:46.396178	2020-09-15 23:50:46.396178
208	69	PSG1	849	czp10	850	2020-09-15 23:55:25.020823	2020-09-15 23:55:25.020823
209	69	PSG1	853	czp10	854	2020-09-15 23:55:46.047073	2020-09-15 23:55:46.047073
210	69	PSG1	857	czp10	858	2020-09-16 00:06:11.383534	2020-09-16 00:06:11.383534
211	69	PSG1	861	czp10	862	2020-09-16 00:07:19.062645	2020-09-16 00:07:19.062645
212	69	PSG1	865	czp10	866	2020-09-16 22:20:37.546004	2020-09-16 22:20:37.546004
213	69	PSG1	869	czp10	870	2020-09-16 22:54:23.038181	2020-09-16 22:54:23.038181
214	69	PSG1	873	czp10	874	2020-09-16 22:59:03.641925	2020-09-16 22:59:03.641925
215	69	PSG1	877	czp10	878	2020-09-16 23:46:19.115436	2020-09-16 23:46:19.115436
216	69	PSG1	881	czp10	882	2020-09-17 03:02:04.857112	2020-09-17 03:02:04.857112
217	69	PSG1	885	czp10	886	2020-09-17 04:34:58.50663	2020-09-17 04:34:58.50663
218	69	PSG1	889	czp10	890	2020-09-17 17:39:32.702981	2020-09-17 17:39:32.702981
219	69	PSG1	893	czp10	894	2020-09-17 17:52:41.646697	2020-09-17 17:52:41.646697
220	69	PSG1	897	czp10	898	2020-09-17 18:11:53.244063	2020-09-17 18:11:53.244063
221	69	PSG1	901	czp10	902	2020-09-17 18:14:04.526118	2020-09-17 18:14:04.526118
222	69	PSG1	905	czp10	906	2020-09-17 18:15:36.827131	2020-09-17 18:15:36.827131
223	69	PSG1	909	czp10	910	2020-09-17 18:17:49.623082	2020-09-17 18:17:49.623082
224	69	PSG1	913	czp10	914	2020-09-17 18:18:51.993199	2020-09-17 18:18:51.993199
225	69	PSG1	917	czp10	918	2020-09-17 18:20:51.32484	2020-09-17 18:20:51.32484
226	69	PSG1	921	czp10	922	2020-09-17 18:22:05.84567	2020-09-17 18:22:05.84567
227	69	PSG1	925	czp10	926	2020-09-17 18:22:49.348144	2020-09-17 18:22:49.348144
228	69	PSG1	929	czp10	930	2020-09-17 18:23:21.066558	2020-09-17 18:23:21.066558
229	69	PSG1	933	czp10	934	2020-09-17 18:30:25.856806	2020-09-17 18:30:25.856806
230	69	PSG1	937	czp10	938	2020-09-20 05:11:10.533263	2020-09-20 05:11:10.533263
231	69	PSG1	941	czp10	942	2020-09-20 05:17:08.845333	2020-09-20 05:17:08.845333
232	69	PSG1	945	czp10	946	2020-09-20 20:49:30.34697	2020-09-20 20:49:30.34697
233	69	PSG1	949	czp10	950	2020-09-24 18:11:48.029224	2020-09-24 18:11:48.029224
234	69	PSG1	953	czp10	954	2020-09-24 18:18:16.260575	2020-09-24 18:18:16.260575
235	69	PSG1	957	czp10	958	2020-09-24 18:22:15.908165	2020-09-24 18:22:15.908165
236	69	PSG1	961	czp10	962	2020-09-24 18:23:32.554315	2020-09-24 18:23:32.554315
237	69	PSG1	965	czp10	966	2020-09-24 18:30:04.910943	2020-09-24 18:30:04.910943
238	69	PSG1	969	czp10	970	2020-09-25 18:25:27.505948	2020-09-25 18:25:27.505948
239	69	PSG1	973	czp10	974	2020-09-25 18:27:37.405648	2020-09-25 18:27:37.405648
240	69	PSG1	977	czp10	978	2020-09-25 18:29:13.061383	2020-09-25 18:29:13.061383
241	69	PSG1	981	czp10	982	2020-09-25 18:30:59.317791	2020-09-25 18:30:59.317791
242	69	PSG1	985	czp10	986	2020-09-25 18:32:20.438946	2020-09-25 18:32:20.438946
243	69	PSG1	989	czp10	990	2020-09-25 18:37:40.174683	2020-09-25 18:37:40.174683
244	69	PSG1	993	czp10	994	2020-09-25 18:41:02.069891	2020-09-25 18:41:02.069891
245	69	PSG1	997	czp10	998	2020-09-25 18:44:19.873295	2020-09-25 18:44:19.873295
246	69	PSG1	1001	czp10	1002	2020-09-25 18:51:36.240763	2020-09-25 18:51:36.240763
247	69	PSG1	1005	czp10	1006	2020-09-25 18:56:58.06823	2020-09-25 18:56:58.06823
248	69	PSG1	1009	czp10	1010	2020-09-25 19:03:37.392165	2020-09-25 19:03:37.392165
249	69	PSG1	1013	czp10	1014	2020-09-25 19:06:29.685489	2020-09-25 19:06:29.685489
250	69	PSG1	1017	czp10	1018	2020-09-25 19:07:37.089518	2020-09-25 19:07:37.089518
251	69	PSG1	1021	czp10	1022	2020-09-25 19:10:54.001099	2020-09-25 19:10:54.001099
252	69	PSG1	1025	czp10	1026	2020-09-25 19:12:12.322524	2020-09-25 19:12:12.322524
253	69	PSG1	1029	czp10	1030	2020-09-25 19:14:19.213538	2020-09-25 19:14:19.213538
254	69	PSG1	1033	czp10	1034	2020-09-25 19:16:51.089345	2020-09-25 19:16:51.089345
255	69	PSG1	1037	czp10	1038	2020-09-25 19:20:07.017541	2020-09-25 19:20:07.017541
256	69	PSG1	1041	czp10	1042	2020-09-25 21:51:11.854237	2020-09-25 21:51:11.854237
257	69	PSG1	1045	czp10	1046	2020-09-25 21:58:33.147096	2020-09-25 21:58:33.147096
258	69	PSG1	1049	czp10	1050	2020-09-25 21:59:50.260161	2020-09-25 21:59:50.260161
259	69	PSG1	1053	czp10	1054	2020-09-26 04:33:08.246128	2020-09-26 04:33:08.246128
260	69	PSG1	1057	czp10	1058	2020-09-26 04:34:59.359747	2020-09-26 04:34:59.359747
261	69	PSG1	1061	czp10	1062	2020-09-26 04:49:59.662759	2020-09-26 04:49:59.662759
262	69	PSG1	1065	czp10	1066	2020-09-26 04:51:20.192461	2020-09-26 04:51:20.192461
263	69	PSG1	1069	czp10	1070	2020-09-26 21:08:57.378403	2020-09-26 21:08:57.378403
264	69	PSG1	1073	czp10	1074	2020-09-27 03:30:17.046827	2020-09-27 03:30:17.046827
265	69	PSG1	1077	czp10	1078	2020-09-27 20:59:46.019763	2020-09-27 20:59:46.019763
266	69	PSG1	1081	czp10	1082	2020-09-27 21:00:35.381201	2020-09-27 21:00:35.381201
267	69	PSG1	1085	czp10	1086	2020-09-27 21:08:41.227962	2020-09-27 21:08:41.227962
268	69	PSG1	1089	czp10	1090	2020-09-27 21:27:50.714466	2020-09-27 21:27:50.714466
269	69	PSG1	1093	czp10	1094	2020-09-28 02:23:28.17041	2020-09-28 02:23:28.17041
270	69	PSG1	1097	czp10	1098	2020-09-28 02:29:08.708033	2020-09-28 02:29:08.708033
271	69	PSG1	1101	czp10	1102	2020-09-28 02:44:01.412822	2020-09-28 02:44:01.412822
272	69	PSG1	1105	czp10	1106	2020-09-28 02:56:27.699787	2020-09-28 02:56:27.699787
273	69	PSG1	1109	czp10	1110	2020-09-28 03:26:53.561254	2020-09-28 03:26:53.561254
274	69	PSG1	1113	czp10	1114	2020-09-28 03:31:01.084716	2020-09-28 03:31:01.084716
275	69	PSG1	1117	czp10	1118	2020-09-28 03:32:40.924231	2020-09-28 03:32:40.924231
276	69	PSG1	1121	czp10	1122	2020-09-28 18:20:38.400762	2020-09-28 18:20:38.400762
277	69	PSG1	1125	czp10	1126	2020-09-28 18:23:38.761872	2020-09-28 18:23:38.761872
278	69	PSG1	1129	czp10	1130	2020-09-28 18:28:39.63063	2020-09-28 18:28:39.63063
279	69	PSG1	1133	czp10	1134	2020-09-28 18:32:27.941779	2020-09-28 18:32:27.941779
280	69	PSG1	1137	czp10	1138	2020-09-28 18:44:33.022258	2020-09-28 18:44:33.022258
281	69	PSG1	1141	czp10	1142	2020-09-28 18:45:55.77031	2020-09-28 18:45:55.77031
282	69	PSG1	1145	czp10	1146	2020-09-28 18:48:57.329729	2020-09-28 18:48:57.329729
283	69	PSG1	1149	czp10	1150	2020-09-28 19:03:34.983095	2020-09-28 19:03:34.983095
284	69	PSG1	1153	czp10	1154	2020-09-28 19:09:57.281401	2020-09-28 19:09:57.281401
285	69	PSG1	1157	czp10	1158	2020-09-28 19:12:38.566461	2020-09-28 19:12:38.566461
286	69	PSG1	1161	czp10	1162	2020-09-28 19:13:42.783539	2020-09-28 19:13:42.783539
287	69	PSG1	1165	czp10	1166	2020-09-28 19:14:59.64696	2020-09-28 19:14:59.64696
288	69	PSG1	1169	czp10	1170	2020-09-28 19:15:20.78468	2020-09-28 19:15:20.78468
289	69	PSG1	1173	czp10	1174	2020-09-28 19:17:31.492581	2020-09-28 19:17:31.492581
290	69	PSG1	1177	czp10	1178	2020-09-28 19:18:36.721612	2020-09-28 19:18:36.721612
291	69	PSG1	1181	czp10	1182	2020-09-28 19:19:43.317678	2020-09-28 19:19:43.317678
292	69	PSG1	1185	czp10	1186	2020-09-28 19:20:57.213585	2020-09-28 19:20:57.213585
293	69	PSG1	1189	czp10	1190	2020-09-28 19:45:20.561248	2020-09-28 19:45:20.561248
294	69	PSG1	1193	czp10	1194	2020-09-28 19:46:48.324839	2020-09-28 19:46:48.324839
295	69	PSG1	1197	czp10	1198	2020-09-28 19:49:34.493731	2020-09-28 19:49:34.493731
296	69	PSG1	1201	czp10	1202	2020-09-28 19:50:27.893229	2020-09-28 19:50:27.893229
297	69	PSG1	1205	czp10	1206	2020-09-28 19:54:31.203922	2020-09-28 19:54:31.203922
298	69	PSG1	1209	czp10	1210	2020-09-28 19:55:40.637764	2020-09-28 19:55:40.637764
299	69	PSG1	1213	czp10	1214	2020-09-28 19:57:10.255439	2020-09-28 19:57:10.255439
300	69	PSG1	1217	czp10	1218	2020-09-28 20:00:01.374321	2020-09-28 20:00:01.374321
301	69	PSG1	1221	czp10	1222	2020-09-28 20:01:40.880387	2020-09-28 20:01:40.880387
302	69	PSG1	1225	czp10	1226	2020-09-28 20:02:41.292417	2020-09-28 20:02:41.292417
303	69	PSG1	1229	czp10	1230	2020-09-28 23:15:27.739553	2020-09-28 23:15:27.739553
304	69	PSG1	1233	czp10	1234	2020-09-28 23:21:32.370283	2020-09-28 23:21:32.370283
305	69	PSG1	1237	czp10	1238	2020-09-28 23:23:30.719327	2020-09-28 23:23:30.719327
306	69	PSG1	1241	czp10	1242	2020-09-28 23:45:34.783827	2020-09-28 23:45:34.783827
307	69	PSG1	1245	czp10	1246	2020-09-28 23:57:21.151538	2020-09-28 23:57:21.151538
308	69	PSG1	1249	czp10	1250	2020-09-29 00:09:27.237418	2020-09-29 00:09:27.237418
309	69	PSG1	1253	czp10	1254	2020-09-29 00:11:48.57006	2020-09-29 00:11:48.57006
310	69	PSG1	1257	czp10	1258	2020-09-29 00:13:54.387172	2020-09-29 00:13:54.387172
311	69	PSG1	1261	czp10	1262	2020-09-29 00:14:51.940865	2020-09-29 00:14:51.940865
312	69	PSG1	1265	czp10	1266	2020-09-29 00:18:00.157032	2020-09-29 00:18:00.157032
313	69	PSG1	1269	czp10	1270	2020-09-29 00:20:30.094609	2020-09-29 00:20:30.094609
314	69	PSG1	1273	czp10	1274	2020-09-29 00:22:56.904108	2020-09-29 00:22:56.904108
315	69	PSG1	1277	czp10	1278	2020-09-29 00:24:04.692072	2020-09-29 00:24:04.692072
316	69	PSG1	1281	czp10	1282	2020-09-29 00:26:52.251454	2020-09-29 00:26:52.251454
317	69	PSG1	1285	czp10	1286	2020-09-29 00:28:33.024597	2020-09-29 00:28:33.024597
318	69	PSG1	1289	czp10	1290	2020-09-29 00:30:06.733679	2020-09-29 00:30:06.733679
319	69	PSG1	1293	czp10	1294	2020-09-29 00:31:17.15946	2020-09-29 00:31:17.15946
320	69	PSG1	1297	czp10	1298	2020-09-29 00:32:43.497926	2020-09-29 00:32:43.497926
321	69	PSG1	1301	czp10	1302	2020-09-29 00:33:30.375908	2020-09-29 00:33:30.375908
322	69	PSG1	1305	czp10	1306	2020-09-29 00:34:07.461761	2020-09-29 00:34:07.461761
323	69	PSG1	1309	czp10	1310	2020-09-29 00:34:53.497704	2020-09-29 00:34:53.497704
324	69	PSG1	1313	czp10	1314	2020-09-29 00:35:26.932063	2020-09-29 00:35:26.932063
325	69	PSG1	1317	czp10	1318	2020-09-29 00:46:03.052003	2020-09-29 00:46:03.052003
326	69	PSG1	1321	czp10	1322	2020-09-29 00:52:25.13388	2020-09-29 00:52:25.13388
327	69	PSG1	1325	czp10	1326	2020-09-29 01:04:16.644057	2020-09-29 01:04:16.644057
328	69	PSG1	1329	czp10	1330	2020-09-29 01:08:23.759139	2020-09-29 01:08:23.759139
329	69	PSG1	1333	czp10	1334	2020-09-29 01:13:12.660176	2020-09-29 01:13:12.660176
330	69	PSG1	1337	czp10	1338	2020-09-29 01:24:35.368203	2020-09-29 01:24:35.368203
331	69	PSG1	1341	czp10	1342	2020-09-29 01:27:42.771338	2020-09-29 01:27:42.771338
332	69	PSG1	1345	czp10	1346	2020-09-29 01:34:33.679692	2020-09-29 01:34:33.679692
333	69	PSG1	1349	czp10	1350	2020-09-29 01:37:43.959419	2020-09-29 01:37:43.959419
334	69	PSG1	1353	czp10	1354	2020-09-29 01:39:58.858097	2020-09-29 01:39:58.858097
335	69	PSG1	1357	czp10	1358	2020-09-29 02:04:55.896547	2020-09-29 02:04:55.896547
336	69	PSG1	1361	czp10	1362	2020-09-29 02:12:03.464631	2020-09-29 02:12:03.464631
337	69	PSG1	1365	czp10	1366	2020-09-29 02:28:14.932067	2020-09-29 02:28:14.932067
338	69	PSG1	1369	czp10	1370	2020-09-29 03:50:17.365211	2020-09-29 03:50:17.365211
339	69	PSG1	1373	czp10	1374	2020-09-30 15:13:22.17984	2020-09-30 15:13:22.17984
340	69	PSG1	1377	czp10	1378	2020-09-30 19:29:58.035143	2020-09-30 19:29:58.035143
341	69	PSG1	1381	czp10	1382	2020-10-01 18:40:11.435854	2020-10-01 18:40:11.435854
342	69	PSG1	1385	czp10	1386	2020-10-01 18:46:41.175751	2020-10-01 18:46:41.175751
343	69	PSG1	1389	czp10	1390	2020-10-01 18:56:24.659952	2020-10-01 18:56:24.659952
344	69	PSG1	1393	czp10	1394	2020-10-02 18:50:50.547262	2020-10-02 18:50:50.547262
345	69	PSG1	1397	czp10	1398	2020-10-02 19:02:11.295739	2020-10-02 19:02:11.295739
346	69	PSG1	1401	czp10	1402	2020-10-02 23:40:57.749586	2020-10-02 23:40:57.749586
347	69	PSG1	1405	czp10	1406	2020-10-02 23:59:23.38279	2020-10-02 23:59:23.38279
348	69	PSG1	1409	czp10	1410	2020-10-03 00:04:48.485421	2020-10-03 00:04:48.485421
349	69	PSG1	1413	czp10	1414	2020-10-03 00:59:06.628572	2020-10-03 00:59:06.628572
350	69	PSG1	1417	czp10	1418	2020-10-03 02:34:57.649756	2020-10-03 02:34:57.649756
351	69	PSG1	1421	czp10	1422	2020-10-03 03:08:31.277573	2020-10-03 03:08:31.277573
352	69	PSG1	1425	czp10	1426	2020-10-03 03:19:45.495248	2020-10-03 03:19:45.495248
353	69	PSG1	1429	czp10	1430	2020-10-03 05:56:28.335601	2020-10-03 05:56:28.335601
354	84	L96AW	1433	czp10	1434	2020-10-03 05:56:45.445273	2020-10-03 05:56:45.445273
355	69	PSG1	1435	czp10	1436	2020-10-03 07:49:17.969737	2020-10-03 07:49:17.969737
356	69	PSG1	1439	czp10	1440	2020-10-03 07:59:12.27972	2020-10-03 07:59:12.27972
357	69	PSG1	1443	czp10	1444	2020-10-03 08:00:20.675381	2020-10-03 08:00:20.675381
358	69	PSG1	1447	czp10	1448	2020-10-03 08:02:11.907239	2020-10-03 08:02:11.907239
359	69	PSG1	1451	czp10	1452	2020-10-03 08:03:07.353544	2020-10-03 08:03:07.353544
360	69	PSG1	1455	czp10	1456	2020-10-03 08:27:24.080019	2020-10-03 08:27:24.080019
361	69	PSG1	1459	czp10	1460	2020-10-03 08:30:12.906922	2020-10-03 08:30:12.906922
362	69	PSG1	1463	czp10	1464	2020-10-03 08:37:08.281336	2020-10-03 08:37:08.281336
363	69	PSG1	1467	czp10	1468	2020-10-03 08:39:39.507089	2020-10-03 08:39:39.507089
364	69	PSG1	1471	czp10	1472	2020-10-03 08:44:36.916416	2020-10-03 08:44:36.916416
365	69	PSG1	1475	czp10	1476	2020-10-03 08:46:05.996883	2020-10-03 08:46:05.996883
366	69	PSG1	1479	czp10	1480	2020-10-03 08:46:50.685079	2020-10-03 08:46:50.685079
367	69	PSG1	1483	czp10	1484	2020-10-03 08:49:25.067129	2020-10-03 08:49:25.067129
368	69	PSG1	1487	czp10	1488	2020-10-03 08:56:58.98588	2020-10-03 08:56:58.98588
369	69	PSG1	1491	czp10	1492	2020-10-03 08:57:45.489099	2020-10-03 08:57:45.489099
370	69	PSG1	1495	czp10	1496	2020-10-03 08:58:27.849586	2020-10-03 08:58:27.849586
371	69	PSG1	1499	czp10	1500	2020-10-03 08:59:18.027554	2020-10-03 08:59:18.027554
372	69	PSG1	1503	czp10	1504	2020-10-03 09:11:05.811226	2020-10-03 09:11:05.811226
373	69	PSG1	1507	czp10	1508	2020-10-03 09:21:07.847079	2020-10-03 09:21:07.847079
374	69	PSG1	1511	czp10	1512	2020-10-03 09:37:08.662097	2020-10-03 09:37:08.662097
375	69	PSG1	1515	czp10	1516	2020-10-03 09:39:55.355759	2020-10-03 09:39:55.355759
376	69	PSG1	1519	czp10	1520	2020-10-03 09:42:05.171728	2020-10-03 09:42:05.171728
377	69	PSG1	1523	czp10	1524	2020-10-03 09:42:59.483188	2020-10-03 09:42:59.483188
378	69	PSG1	1527	czp10	1528	2020-10-03 09:44:10.646457	2020-10-03 09:44:10.646457
379	69	PSG1	1531	czp10	1532	2020-10-03 09:45:24.825955	2020-10-03 09:45:24.825955
380	69	PSG1	1535	czp10	1536	2020-10-03 09:48:34.339047	2020-10-03 09:48:34.339047
381	69	PSG1	1539	czp10	1540	2020-10-03 09:50:23.602013	2020-10-03 09:50:23.602013
382	69	PSG1	1543	czp10	1544	2020-10-03 09:51:49.957067	2020-10-03 09:51:49.957067
383	69	PSG1	1547	czp10	1548	2020-10-03 09:53:14.070261	2020-10-03 09:53:14.070261
384	69	PSG1	1551	czp10	1552	2020-10-03 10:08:01.2732	2020-10-03 10:08:01.2732
385	69	PSG1	1555	czp10	1556	2020-10-03 10:15:30.141508	2020-10-03 10:15:30.141508
386	69	PSG1	1559	czp10	1560	2020-10-03 10:22:31.341685	2020-10-03 10:22:31.341685
387	69	PSG1	1563	czp10	1564	2020-10-03 10:24:15.089374	2020-10-03 10:24:15.089374
388	69	PSG1	1567	czp10	1568	2020-10-03 10:26:01.955352	2020-10-03 10:26:01.955352
389	69	PSG1	1571	czp10	1572	2020-10-03 10:29:26.459135	2020-10-03 10:29:26.459135
390	69	PSG1	1575	czp10	1576	2020-10-03 10:30:41.13851	2020-10-03 10:30:41.13851
391	69	PSG1	1579	czp10	1580	2020-10-03 10:36:22.783739	2020-10-03 10:36:22.783739
392	69	PSG1	1583	czp10	1584	2020-10-03 11:57:21.970791	2020-10-03 11:57:21.970791
393	69	PSG1	1587	czp10	1588	2020-10-03 17:17:16.693849	2020-10-03 17:17:16.693849
394	69	PSG1	1591	czp10	1592	2020-10-03 18:16:56.027464	2020-10-03 18:16:56.027464
395	69	PSG1	1595	czp10	1596	2020-10-03 18:37:25.966651	2020-10-03 18:37:25.966651
396	69	PSG1	1599	czp10	1600	2020-10-03 19:07:56.716447	2020-10-03 19:07:56.716447
397	69	PSG1	1603	czp10	1604	2020-10-03 19:27:45.695351	2020-10-03 19:27:45.695351
398	69	PSG1	1607	czp10	1608	2020-10-05 17:49:30.691696	2020-10-05 17:49:30.691696
399	69	PSG1	1611	czp10	1612	2020-10-05 17:50:44.346063	2020-10-05 17:50:44.346063
400	69	PSG1	1615	czp10	1616	2020-10-05 18:00:49.650459	2020-10-05 18:00:49.650459
401	69	PSG1	1619	czp10	1620	2020-10-05 18:06:10.400989	2020-10-05 18:06:10.400989
402	69	PSG1	1623	czp10	1624	2020-10-05 18:13:30.610325	2020-10-05 18:13:30.610325
403	69	PSG1	1627	czp10	1628	2020-10-05 18:17:13.72355	2020-10-05 18:17:13.72355
404	69	PSG1	1631	czp10	1632	2020-10-05 23:16:15.130793	2020-10-05 23:16:15.130793
405	69	PSG1	1635	czp10	1636	2020-10-05 23:33:15.55775	2020-10-05 23:33:15.55775
406	69	PSG1	1639	czp10	1640	2020-10-06 02:14:24.495568	2020-10-06 02:14:24.495568
407	69	PSG1	1643	czp10	1644	2020-10-06 18:11:30.125544	2020-10-06 18:11:30.125544
408	69	PSG1	1647	czp10	1648	2020-10-06 18:13:30.100093	2020-10-06 18:13:30.100093
409	69	PSG1	1651	czp10	1652	2020-10-06 18:13:44.616908	2020-10-06 18:13:44.616908
410	69	PSG1	1655	czp10	1656	2020-10-06 18:15:08.505132	2020-10-06 18:15:08.505132
411	69	PSG1	1659	czp10	1660	2020-10-06 18:15:25.439614	2020-10-06 18:15:25.439614
412	69	PSG1	1663	czp10	1664	2020-10-06 18:16:11.117433	2020-10-06 18:16:11.117433
413	69	PSG1	1667	czp10	1668	2020-10-06 18:16:48.044622	2020-10-06 18:16:48.044622
414	69	PSG1	1671	czp10	1672	2020-10-06 18:18:27.242268	2020-10-06 18:18:27.242268
415	69	PSG1	1675	czp10	1676	2020-10-06 18:19:48.005488	2020-10-06 18:19:48.005488
416	69	PSG1	1679	czp10	1680	2020-10-06 18:20:54.484481	2020-10-06 18:20:54.484481
417	69	PSG1	1683	czp10	1684	2020-10-06 18:42:05.151332	2020-10-06 18:42:05.151332
418	69	PSG1	1687	czp10	1688	2020-10-07 00:46:14.810909	2020-10-07 00:46:14.810909
419	69	PSG1	1691	czp10	1692	2020-10-07 01:22:27.538073	2020-10-07 01:22:27.538073
420	69	PSG1	1695	czp10	1696	2020-10-07 01:27:32.090102	2020-10-07 01:27:32.090102
421	69	PSG1	1699	czp10	1700	2020-10-07 01:32:12.891829	2020-10-07 01:32:12.891829
422	69	PSG1	1703	czp10	1704	2020-10-07 01:33:29.84623	2020-10-07 01:33:29.84623
423	69	PSG1	1707	czp10	1708	2020-10-08 17:22:19.446959	2020-10-08 17:22:19.446959
424	69	PSG1	1711	czp10	1712	2020-10-08 17:26:52.855431	2020-10-08 17:26:52.855431
425	69	PSG1	1715	czp10	1716	2020-10-08 18:01:04.583711	2020-10-08 18:01:04.583711
426	69	PSG1	1719	czp10	1720	2020-10-08 18:03:56.319456	2020-10-08 18:03:56.319456
427	69	PSG1	1723	czp10	1724	2020-10-09 17:04:19.904022	2020-10-09 17:04:19.904022
428	69	PSG1	1727	czp10	1728	2020-10-09 17:41:34.807996	2020-10-09 17:41:34.807996
429	69	PSG1	1731	czp10	1732	2020-10-09 17:47:58.956881	2020-10-09 17:47:58.956881
430	69	PSG1	1735	czp10	1736	2020-10-09 17:55:21.967912	2020-10-09 17:55:21.967912
431	69	PSG1	1739	czp10	1740	2020-10-09 18:01:12.148297	2020-10-09 18:01:12.148297
432	69	PSG1	1743	czp10	1744	2020-10-09 22:10:15.892035	2020-10-09 22:10:15.892035
433	69	PSG1	1747	czp10	1748	2020-10-09 22:12:54.602097	2020-10-09 22:12:54.602097
434	69	PSG1	1751	czp10	1752	2020-10-09 22:16:51.02513	2020-10-09 22:16:51.02513
435	69	PSG1	1755	czp10	1756	2020-10-09 23:32:25.330485	2020-10-09 23:32:25.330485
436	69	PSG1	1759	czp10	1760	2020-10-10 01:33:07.953179	2020-10-10 01:33:07.953179
437	69	PSG1	1763	czp10	1764	2020-10-10 02:19:01.464852	2020-10-10 02:19:01.464852
438	69	PSG1	1767	czp10	1768	2020-10-10 02:20:17.778146	2020-10-10 02:20:17.778146
439	69	PSG1	1771	czp10	1772	2020-10-10 02:21:07.314357	2020-10-10 02:21:07.314357
440	69	PSG1	1775	czp10	1776	2020-10-10 02:24:27.151763	2020-10-10 02:24:27.151763
441	69	PSG1	1779	czp10	1780	2020-10-10 02:27:54.481366	2020-10-10 02:27:54.481366
442	69	PSG1	1783	czp10	1784	2020-10-10 02:34:01.079278	2020-10-10 02:34:01.079278
443	69	PSG1	1787	czp10	1788	2020-10-10 02:35:48.885947	2020-10-10 02:35:48.885947
444	69	PSG1	1791	czp10	1792	2020-10-10 02:37:12.007931	2020-10-10 02:37:12.007931
445	69	PSG1	1795	czp10	1796	2020-10-10 02:45:11.229276	2020-10-10 02:45:11.229276
446	69	PSG1	1799	czp10	1800	2020-10-10 02:47:06.236316	2020-10-10 02:47:06.236316
447	69	PSG1	1803	czp10	1804	2020-10-10 02:47:46.538989	2020-10-10 02:47:46.538989
448	69	PSG1	1807	czp10	1808	2020-10-10 02:53:45.23617	2020-10-10 02:53:45.23617
449	69	PSG1	1811	czp10	1812	2020-10-10 02:54:03.445661	2020-10-10 02:54:03.445661
450	69	PSG1	1815	czp10	1816	2020-10-10 03:04:44.860688	2020-10-10 03:04:44.860688
451	69	PSG1	1819	czp10	1820	2020-10-10 03:32:20.713627	2020-10-10 03:32:20.713627
452	69	PSG1	1823	czp10	1824	2020-10-10 03:36:58.739838	2020-10-10 03:36:58.739838
453	69	PSG1	1827	czp10	1828	2020-10-10 03:45:36.879618	2020-10-10 03:45:36.879618
454	69	PSG1	1831	czp10	1832	2020-10-10 04:11:21.692417	2020-10-10 04:11:21.692417
455	69	PSG1	1835	czp10	1836	2020-10-13 00:11:27.975663	2020-10-13 00:11:27.975663
456	69	PSG1	1839	czp10	1840	2020-10-13 00:58:17.36443	2020-10-13 00:58:17.36443
457	69	PSG1	1843	czp10	1844	2020-10-13 01:00:20.713419	2020-10-13 01:00:20.713419
458	69	PSG1	1847	czp10	1848	2020-10-13 01:09:13.528186	2020-10-13 01:09:13.528186
459	69	PSG1	1851	czp10	1852	2020-10-13 01:11:15.460627	2020-10-13 01:11:15.460627
460	69	PSG1	1855	czp10	1856	2020-10-13 01:12:08.36371	2020-10-13 01:12:08.36371
461	69	PSG1	1859	czp10	1860	2020-10-13 01:13:07.492231	2020-10-13 01:13:07.492231
462	69	PSG1	1863	czp10	1864	2020-10-13 01:14:09.629218	2020-10-13 01:14:09.629218
463	69	PSG1	1867	czp10	1868	2020-10-13 01:14:38.889542	2020-10-13 01:14:38.889542
464	69	PSG1	1871	czp10	1872	2020-10-13 01:16:22.803884	2020-10-13 01:16:22.803884
465	69	PSG1	1875	czp10	1876	2020-10-13 01:19:22.148432	2020-10-13 01:19:22.148432
466	69	PSG1	1879	czp10	1880	2020-10-13 01:20:45.861922	2020-10-13 01:20:45.861922
467	69	PSG1	1883	czp10	1884	2020-10-13 01:24:46.025983	2020-10-13 01:24:46.025983
468	69	PSG1	1887	czp10	1888	2020-10-13 01:25:46.460197	2020-10-13 01:25:46.460197
469	69	PSG1	1891	czp10	1892	2020-10-13 02:05:40.793125	2020-10-13 02:05:40.793125
470	69	PSG1	1895	czp10	1896	2020-10-13 02:28:20.194278	2020-10-13 02:28:20.194278
471	69	PSG1	1899	czp10	1900	2020-10-13 17:35:45.180392	2020-10-13 17:35:45.180392
472	69	PSG1	1903	czp10	1904	2020-10-13 17:41:28.174943	2020-10-13 17:41:28.174943
\.


--
-- Data for Name: class_support; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.class_support (support_id, support_player_id, support_primary_type, support_primary_weapon_id, support_secondary_type, support_secondary_weapon_id, created_at, updated_at) FROM stdin;
\.


--
-- Data for Name: computer_terminal; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.computer_terminal (id, terminal_room_vnum, terminal_type, terminal_attaches_to, terminal_name, terminal_description, created_at, updated_at) FROM stdin;
1	410	chooser	north	DMZ Proving Grounds - Course Selection Interface [Baphomet OS - version 1.3.0]	\N	2020-10-04 04:06:45.194267	2020-10-04 04:06:45.194267
\.


--
-- Data for Name: extra_description; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.extra_description (id, obj_fk_id, extra_keyword, extra_description) FROM stdin;
\.


--
-- Data for Name: integral_object; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.integral_object (object_id, object_room_vnum, object_type, object_vnum, created_at, updated_at) FROM stdin;
\.


--
-- Data for Name: karma; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.karma (karma_id, karma_player_id, karma_alignment, karma_cold_blooded_index, created_at, updated_at) FROM stdin;
\.


--
-- Data for Name: mini_game; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.mini_game (game_id, game_type, game_vnum, game_name, game_action, game_room_vnum, game_order, created_at, updated_at) FROM stdin;
1	line_up	1	Southside Lock #1	unlock vnum:128 north,east,south,west	128	0	2020-09-29 01:57:15.768037	2020-09-29 01:57:15.768037
2	line_up	1	Southside Lock #1	unlock vnum:129 north,east,south,west	129	0	2020-09-29 02:26:16.028043	2020-09-29 02:26:16.028043
3	line_up	1	Southside Lock #1	unlock vnum:128 north,east,south,west	128	0	2020-09-29 02:26:38.540912	2020-09-29 02:26:38.540912
4	line_up	1	Southside Lock #1	unlock vnum:128 north,east,south,west	129	0	2020-09-29 02:26:43.216918	2020-09-29 02:26:43.216918
\.


--
-- Data for Name: mini_gunner_sentinel; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.mini_gunner_sentinel (id, mgs_mob_vnum, mgs_face_direction, mgs_room_vnum) FROM stdin;
2	103	N	132
\.


--
-- Data for Name: mob_zone; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.mob_zone (id, zone_id, mob_id, room_id, max_existing) FROM stdin;
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
-- Data for Name: object_rifle; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.object_rifle (rifle_id, rifle_accuracy_map_0, rifle_accuracy_map_1, rifle_accuracy_map_2, rifle_accuracy_map_3, rifle_damage_map_0, rifle_damage_map_1, rifle_damage_map_2, rifle_damage_map_3, rifle_ammo_max, rifle_ammo_type, rifle_chance_to_injure, rifle_clip_size, rifle_cooldown_between_shots, rifle_critical_chance, rifle_critical_range, rifle_damage_per_second, rifle_disorient_amount, rifle_headshot_bonus, rifle_max_range, rifle_range_multiplier, rifle_reload_time, rifle_rounds_per_minute, rifle_muzzle_velocity, rifle_effective_firing_range, rifle_str_type, rifle_type, rifle_manufacturer, rifle_name, rifle_vnum, rifle_rarity, rifle_file, created_at, updated_at) FROM stdin;
3	80	40	0	0	50	0	0	0	75	PISTOL	20.5009990000000002	9	0.00130100000000000007	5.13999999999999968	1	20	0	33	1	0	2.00130000000000008	80	40	2	PISTOL	8	Prophet Industrial	A CZP10 pistol	600	COMMON	base_pistol.yml	2020-02-22 09:18:22.223558	2020-02-22 09:18:22.223558
5	80	40	0	0	50	0	0	0	75	PISTOL	20.5009990000000002	9	0.00130100000000000007	5.13999999999999968	1	20	0	33	1	0	2.00130000000000008	80	40	2	PISTOL	8	Prophet Industrial	A CZP10 pistol	600	COMMON	base_pistol.yml	2020-02-22 09:22:34.763742	2020-02-22 09:22:34.763742
7	80	40	0	0	50	0	0	0	75	PISTOL	20.5009990000000002	9	0.00130100000000000007	5.13999999999999968	1	20	0	33	1	0	2.00130000000000008	80	40	2	PISTOL	8	Prophet Industrial	A CZP10 pistol	600	COMMON	base_pistol.yml	2020-02-22 09:27:31.739197	2020-02-22 09:27:31.739197
9	80	40	0	0	50	0	0	0	75	PISTOL	20.5009990000000002	9	0.00130100000000000007	5.13999999999999968	1	20	0	33	1	0	2.00130000000000008	80	40	2	PISTOL	8	Prophet Industrial	A CZP10 pistol	600	COMMON	base_pistol.yml	2020-02-22 09:33:13.40928	2020-02-22 09:33:13.40928
11	80	40	0	0	50	0	0	0	75	PISTOL	20.5009990000000002	9	0.00130100000000000007	5.13999999999999968	1	20	0	33	1	0	2.00130000000000008	80	40	2	PISTOL	8	Prophet Industrial	A CZP10 pistol	600	COMMON	base_pistol.yml	2020-02-22 09:55:51.713635	2020-02-22 09:55:51.713635
13	80	40	0	0	50	0	0	0	75	PISTOL	20.5009990000000002	9	0.00130100000000000007	5.13999999999999968	1	20	0	33	1	0	2.00130000000000008	80	40	2	PISTOL	8	Prophet Industrial	A CZP10 pistol	600	COMMON	base_pistol.yml	2020-02-22 10:00:14.946582	2020-02-22 10:00:14.946582
15	80	40	0	0	50	0	0	0	75	PISTOL	20.5009990000000002	9	0.00130100000000000007	5.13999999999999968	1	20	0	33	1	0	2.00130000000000008	80	40	2	PISTOL	8	Prophet Industrial	A CZP10 pistol	600	COMMON	base_pistol.yml	2020-02-22 10:02:09.754172	2020-02-22 10:02:09.754172
17	80	40	0	0	50	0	0	0	75	PISTOL	20.5009990000000002	9	0.00130100000000000007	5.13999999999999968	1	20	0	33	1	0	2.00130000000000008	80	40	2	PISTOL	8	Prophet Industrial	A CZP10 pistol	600	COMMON	base_pistol.yml	2020-02-22 10:20:33.309622	2020-02-22 10:20:33.309622
18	80	40	0	0	50	0	0	0	75	PISTOL	20.5009990000000002	9	0.00130100000000000007	5.13999999999999968	1	20	0	33	1	0	2.00130000000000008	80	40	2	PISTOL	8	Prophet Industrial	A CZP10 pistol	600	COMMON	base_pistol.yml	2020-02-22 10:39:57.639911	2020-02-22 10:39:57.639911
1	90	10	0	0	80	40	0	0	400	SUB_MACHINE_GUN	30.8129999999999988	31	0.00100000000000000002	1.04000000000000004	1	30	0	13	3	4.29999999999999982	2.10000000000000009	180	818	20	SUB_MACHINE_GUN	5	Heckler and Koch	MP5	5	COMMON	mp5.yml	2020-02-22 08:58:09.493786	2020-02-22 08:58:09.493786
2	90	10	0	0	80	40	0	0	400	SUB_MACHINE_GUN	30.8129999999999988	31	0.00100000000000000002	1.04000000000000004	1	30	0	13	3	4.29999999999999982	2.10000000000000009	180	818	20	SUB_MACHINE_GUN	5	Heckler and Koch	MP5	5	COMMON	mp5.yml	2020-02-22 09:18:22.214858	2020-02-22 09:18:22.214858
4	90	10	0	0	80	40	0	0	400	SUB_MACHINE_GUN	30.8129999999999988	31	0.00100000000000000002	1.04000000000000004	1	30	0	13	3	4.29999999999999982	2.10000000000000009	180	818	20	SUB_MACHINE_GUN	5	Heckler and Koch	MP5	5	COMMON	mp5.yml	2020-02-22 09:22:34.745591	2020-02-22 09:22:34.745591
6	90	10	0	0	80	40	0	0	400	SUB_MACHINE_GUN	30.8129999999999988	31	0.00100000000000000002	1.04000000000000004	1	30	0	13	3	4.29999999999999982	2.10000000000000009	180	818	20	SUB_MACHINE_GUN	5	Heckler and Koch	MP5	5	COMMON	mp5.yml	2020-02-22 09:27:31.728295	2020-02-22 09:27:31.728295
8	90	10	0	0	80	40	0	0	400	SUB_MACHINE_GUN	30.8129999999999988	31	0.00100000000000000002	1.04000000000000004	1	30	0	13	3	4.29999999999999982	2.10000000000000009	180	818	20	SUB_MACHINE_GUN	5	Heckler and Koch	MP5	5	COMMON	mp5.yml	2020-02-22 09:33:13.396601	2020-02-22 09:33:13.396601
10	90	10	0	0	80	40	0	0	400	SUB_MACHINE_GUN	30.8129999999999988	31	0.00100000000000000002	1.04000000000000004	1	30	0	13	3	4.29999999999999982	2.10000000000000009	180	818	20	SUB_MACHINE_GUN	5	Heckler and Koch	MP5	5	COMMON	mp5.yml	2020-02-22 09:55:51.699457	2020-02-22 09:55:51.699457
12	90	10	0	0	80	40	0	0	400	SUB_MACHINE_GUN	30.8129999999999988	31	0.00100000000000000002	1.04000000000000004	1	30	0	13	3	4.29999999999999982	2.10000000000000009	180	818	20	SUB_MACHINE_GUN	5	Heckler and Koch	MP5	5	COMMON	mp5.yml	2020-02-22 10:00:14.938261	2020-02-22 10:00:14.938261
14	90	10	0	0	80	40	0	0	400	SUB_MACHINE_GUN	30.8129999999999988	31	0.00100000000000000002	1.04000000000000004	1	30	0	13	3	4.29999999999999982	2.10000000000000009	180	818	20	SUB_MACHINE_GUN	5	Heckler and Koch	MP5	5	COMMON	mp5.yml	2020-02-22 10:02:09.742178	2020-02-22 10:02:09.742178
16	90	10	0	0	80	40	0	0	400	SUB_MACHINE_GUN	30.8129999999999988	31	0.00100000000000000002	1.04000000000000004	1	30	0	13	3	4.29999999999999982	2.10000000000000009	180	818	20	SUB_MACHINE_GUN	5	Heckler and Koch	MP5	5	COMMON	mp5.yml	2020-02-22 10:20:33.294861	2020-02-22 10:20:33.294861
19	90	10	0	0	80	40	0	0	400	SUB_MACHINE_GUN	30.8129999999999988	31	0.00100000000000000002	1.04000000000000004	1	30	0	13	3	4.29999999999999982	2.10000000000000009	180	818	20	SUB_MACHINE_GUN	5	Heckler and Koch	MP5	5	COMMON	mp5.yml	2020-02-25 05:47:34.90094	2020-02-25 05:47:34.90094
20	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	0.00130100000000000007	5.13999999999999968	1	20	0	33	1	0	2.00130000000000008	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-02-25 05:47:34.920289	2020-02-25 05:47:34.920289
21	90	10	0	0	90	40	9	0	40	SHOTGUN	70	6	1.34099999999999997	4.04000000000000004	1	80	0	53	2	4.29999999999999982	6.09999999999999964	10	918	10	SHOTGUN	3	R.S.S.	SASG-12	6	COMMON	sasg12.yml	2020-02-28 23:38:10.371194	2020-02-28 23:38:10.371194
22	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	0.00130100000000000007	5.13999999999999968	1	20	0	33	1	0	2.00130000000000008	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-02-28 23:38:10.382443	2020-02-28 23:38:10.382443
45	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-08-28 23:33:01.379795	2020-08-28 23:33:01.379795
46	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-08-28 23:33:01.4328	2020-08-28 23:33:01.4328
47	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	8	10	1818	500	SNIPER	6	Heckler and Koch	L96 Arctic Warfare	24	COMMON	l96aw.yml	2020-09-01 04:39:39.652605	2020-09-01 04:39:39.652605
48	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-01 04:39:39.778276	2020-09-01 04:39:39.778276
49	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-01 04:54:05.116668	2020-09-01 04:54:05.116668
50	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-01 04:54:05.127594	2020-09-01 04:54:05.127594
51	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	8	10	1818	500	SNIPER	6	Heckler and Koch	L96 Arctic Warfare	24	COMMON	l96aw.yml	2020-09-02 02:57:17.634501	2020-09-02 02:57:17.634501
52	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-02 02:57:17.657793	2020-09-02 02:57:17.657793
53	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	8	10	1818	500	SNIPER	6	Heckler and Koch	L96 Arctic Warfare	24	COMMON	l96aw.yml	2020-09-02 03:04:39.459032	2020-09-02 03:04:39.459032
54	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-02 03:04:39.473668	2020-09-02 03:04:39.473668
55	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-02 03:17:15.657571	2020-09-02 03:17:15.657571
56	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-02 03:17:15.669185	2020-09-02 03:17:15.669185
57	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	8	10	1818	500	SNIPER	6	Heckler and Koch	L96 Arctic Warfare	24	COMMON	l96aw.yml	2020-09-02 03:19:44.28438	2020-09-02 03:19:44.28438
58	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-02 03:19:44.305147	2020-09-02 03:19:44.305147
59	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	8	10	1818	500	SNIPER	6	Heckler and Koch	L96 Arctic Warfare	24	COMMON	l96aw.yml	2020-09-02 04:05:04.746662	2020-09-02 04:05:04.746662
60	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-02 04:05:04.792464	2020-09-02 04:05:04.792464
61	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	A TAR-21 Assault Rifle	30	COMMON	tar21.yml	2020-09-02 05:48:42.619981	2020-09-02 05:48:42.619981
62	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-02 05:48:42.644091	2020-09-02 05:48:42.644091
63	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-02 05:52:03.222563	2020-09-02 05:52:03.222563
64	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-02 05:52:03.23085	2020-09-02 05:52:03.23085
65	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-02 06:48:23.143537	2020-09-02 06:48:23.143537
66	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-02 06:48:23.162057	2020-09-02 06:48:23.162057
67	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-02 06:49:08.116326	2020-09-02 06:49:08.116326
68	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-02 06:49:08.164671	2020-09-02 06:49:08.164671
69	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-02 06:49:53.251507	2020-09-02 06:49:53.251507
70	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-02 06:49:53.267398	2020-09-02 06:49:53.267398
71	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-02 06:50:38.338652	2020-09-02 06:50:38.338652
72	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-02 06:50:38.362762	2020-09-02 06:50:38.362762
73	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-02 06:51:23.422196	2020-09-02 06:51:23.422196
74	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-02 06:51:23.439241	2020-09-02 06:51:23.439241
75	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-02 06:52:08.445085	2020-09-02 06:52:08.445085
76	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-02 06:52:08.501406	2020-09-02 06:52:08.501406
77	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-02 06:52:53.704328	2020-09-02 06:52:53.704328
78	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-02 06:52:53.719665	2020-09-02 06:52:53.719665
79	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-02 06:53:38.794023	2020-09-02 06:53:38.794023
80	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-02 06:53:38.813933	2020-09-02 06:53:38.813933
81	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-02 06:54:23.7468	2020-09-02 06:54:23.7468
82	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-02 06:54:23.766473	2020-09-02 06:54:23.766473
83	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-02 06:55:09.210497	2020-09-02 06:55:09.210497
84	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-02 06:55:09.227503	2020-09-02 06:55:09.227503
85	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-02 06:55:55.498111	2020-09-02 06:55:55.498111
86	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-02 06:55:55.521909	2020-09-02 06:55:55.521909
87	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-02 06:56:07.177637	2020-09-02 06:56:07.177637
88	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-02 06:56:07.197545	2020-09-02 06:56:07.197545
89	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-02 07:14:26.26855	2020-09-02 07:14:26.26855
90	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-02 07:14:26.287019	2020-09-02 07:14:26.287019
91	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-02 07:15:23.302411	2020-09-02 07:15:23.302411
92	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-02 07:15:23.315264	2020-09-02 07:15:23.315264
93	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-02 07:15:36.201297	2020-09-02 07:15:36.201297
94	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-02 07:15:36.216062	2020-09-02 07:15:36.216062
95	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-02 07:17:11.533223	2020-09-02 07:17:11.533223
96	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-02 07:17:11.639025	2020-09-02 07:17:11.639025
97	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-02 07:17:26.785095	2020-09-02 07:17:26.785095
98	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-02 07:17:26.805701	2020-09-02 07:17:26.805701
99	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-02 07:20:26.638335	2020-09-02 07:20:26.638335
100	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-02 07:20:26.658445	2020-09-02 07:20:26.658445
101	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-02 07:22:56.351034	2020-09-02 07:22:56.351034
102	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-02 07:22:56.368117	2020-09-02 07:22:56.368117
103	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-02 07:24:39.079392	2020-09-02 07:24:39.079392
104	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-02 07:24:39.098069	2020-09-02 07:24:39.098069
105	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-02 07:24:54.584295	2020-09-02 07:24:54.584295
106	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-02 07:24:54.599148	2020-09-02 07:24:54.599148
107	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-02 07:30:22.924746	2020-09-02 07:30:22.924746
108	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-02 07:30:22.934963	2020-09-02 07:30:22.934963
109	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-02 07:36:36.544456	2020-09-02 07:36:36.544456
110	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-02 07:36:36.565121	2020-09-02 07:36:36.565121
111	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-02 07:37:01.498338	2020-09-02 07:37:01.498338
112	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-02 07:37:01.526352	2020-09-02 07:37:01.526352
113	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-02 07:47:18.093515	2020-09-02 07:47:18.093515
114	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-02 07:47:18.111121	2020-09-02 07:47:18.111121
115	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-02 07:53:51.149187	2020-09-02 07:53:51.149187
116	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-02 07:53:51.167306	2020-09-02 07:53:51.167306
117	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-02 07:53:51.184629	2020-09-02 07:53:51.184629
118	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-02 07:53:51.192198	2020-09-02 07:53:51.192198
119	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-02 07:59:25.582833	2020-09-02 07:59:25.582833
120	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-02 07:59:26.196342	2020-09-02 07:59:26.196342
121	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-02 07:59:26.229252	2020-09-02 07:59:26.229252
122	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-02 07:59:26.236868	2020-09-02 07:59:26.236868
123	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-02 08:03:52.011744	2020-09-02 08:03:52.011744
124	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-02 08:03:52.027687	2020-09-02 08:03:52.027687
125	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-02 08:03:52.044982	2020-09-02 08:03:52.044982
126	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-02 08:03:52.052505	2020-09-02 08:03:52.052505
127	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-02 08:08:07.245848	2020-09-02 08:08:07.245848
128	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-02 08:08:07.267633	2020-09-02 08:08:07.267633
129	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-02 08:10:26.713155	2020-09-02 08:10:26.713155
130	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-02 08:10:26.725758	2020-09-02 08:10:26.725758
131	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-02 08:10:26.742988	2020-09-02 08:10:26.742988
132	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-02 08:10:26.75077	2020-09-02 08:10:26.75077
133	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-02 08:12:03.745774	2020-09-02 08:12:03.745774
134	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-02 08:12:03.764908	2020-09-02 08:12:03.764908
135	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-02 08:13:03.802006	2020-09-02 08:13:03.802006
136	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-02 08:13:03.809931	2020-09-02 08:13:03.809931
137	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-02 21:36:14.818375	2020-09-02 21:36:14.818375
138	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-02 21:36:14.829701	2020-09-02 21:36:14.829701
139	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-02 21:36:14.855127	2020-09-02 21:36:14.855127
140	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-02 21:36:14.863	2020-09-02 21:36:14.863
141	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	A TAR-21 Assault Rifle	30	COMMON	tar21.yml	2020-09-02 22:25:15.135235	2020-09-02 22:25:15.135235
142	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-02 22:25:15.160338	2020-09-02 22:25:15.160338
143	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-02 22:26:14.906667	2020-09-02 22:26:14.906667
144	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-02 22:26:14.914921	2020-09-02 22:26:14.914921
145	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-02 22:26:14.940653	2020-09-02 22:26:14.940653
146	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-02 22:26:14.948892	2020-09-02 22:26:14.948892
147	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-02 23:00:57.644205	2020-09-02 23:00:57.644205
148	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-02 23:00:57.660819	2020-09-02 23:00:57.660819
149	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-02 23:00:57.686093	2020-09-02 23:00:57.686093
150	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-02 23:00:57.694136	2020-09-02 23:00:57.694136
151	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-02 23:39:44.154649	2020-09-02 23:39:44.154649
152	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-02 23:39:44.172594	2020-09-02 23:39:44.172594
153	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-02 23:39:44.197813	2020-09-02 23:39:44.197813
154	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-02 23:39:44.205834	2020-09-02 23:39:44.205834
155	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-02 23:47:29.760404	2020-09-02 23:47:29.760404
156	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-02 23:47:29.783454	2020-09-02 23:47:29.783454
157	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-02 23:47:29.810559	2020-09-02 23:47:29.810559
158	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-02 23:47:29.817811	2020-09-02 23:47:29.817811
159	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-02 23:49:22.77017	2020-09-02 23:49:22.77017
160	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-02 23:49:23.349872	2020-09-02 23:49:23.349872
161	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-02 23:49:23.933546	2020-09-02 23:49:23.933546
162	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-02 23:49:23.941356	2020-09-02 23:49:23.941356
163	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-03 00:43:24.613871	2020-09-03 00:43:24.613871
164	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 00:43:24.631014	2020-09-03 00:43:24.631014
165	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-03 00:43:24.65629	2020-09-03 00:43:24.65629
166	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 00:43:24.664229	2020-09-03 00:43:24.664229
167	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-03 00:44:02.194372	2020-09-03 00:44:02.194372
168	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 00:44:02.216678	2020-09-03 00:44:02.216678
169	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-03 00:48:00.06615	2020-09-03 00:48:00.06615
170	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 00:48:00.081555	2020-09-03 00:48:00.081555
171	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-03 00:48:00.107484	2020-09-03 00:48:00.107484
172	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 00:48:00.123221	2020-09-03 00:48:00.123221
173	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-03 01:49:32.387714	2020-09-03 01:49:32.387714
174	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 01:49:32.397408	2020-09-03 01:49:32.397408
175	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-03 01:49:32.422827	2020-09-03 01:49:32.422827
176	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 01:49:32.430689	2020-09-03 01:49:32.430689
177	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-03 02:16:33.716348	2020-09-03 02:16:33.716348
178	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 02:16:33.744284	2020-09-03 02:16:33.744284
179	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-03 02:16:33.769853	2020-09-03 02:16:33.769853
180	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 02:16:33.776951	2020-09-03 02:16:33.776951
181	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-03 02:16:50.724139	2020-09-03 02:16:50.724139
182	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 02:16:50.735421	2020-09-03 02:16:50.735421
183	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-03 02:16:50.762358	2020-09-03 02:16:50.762358
184	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 02:16:50.769843	2020-09-03 02:16:50.769843
185	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-03 02:18:42.374235	2020-09-03 02:18:42.374235
186	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 02:18:42.392077	2020-09-03 02:18:42.392077
187	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-03 02:18:42.417312	2020-09-03 02:18:42.417312
188	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 02:18:42.425399	2020-09-03 02:18:42.425399
189	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-03 02:49:28.005708	2020-09-03 02:49:28.005708
190	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 02:49:28.217268	2020-09-03 02:49:28.217268
191	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-03 02:49:28.243506	2020-09-03 02:49:28.243506
192	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 02:49:28.251139	2020-09-03 02:49:28.251139
193	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-03 02:49:51.600264	2020-09-03 02:49:51.600264
194	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 02:49:51.619139	2020-09-03 02:49:51.619139
195	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-03 02:49:51.644046	2020-09-03 02:49:51.644046
196	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 02:49:51.651982	2020-09-03 02:49:51.651982
197	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-03 02:55:45.851208	2020-09-03 02:55:45.851208
198	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 02:55:45.872825	2020-09-03 02:55:45.872825
199	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-03 02:55:45.89803	2020-09-03 02:55:45.89803
200	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 02:55:45.906133	2020-09-03 02:55:45.906133
201	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-03 03:00:25.643145	2020-09-03 03:00:25.643145
202	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 03:00:28.189711	2020-09-03 03:00:28.189711
203	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-03 03:00:28.216107	2020-09-03 03:00:28.216107
204	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 03:00:28.223606	2020-09-03 03:00:28.223606
205	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-03 03:01:46.947962	2020-09-03 03:01:46.947962
206	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 03:01:46.969184	2020-09-03 03:01:46.969184
207	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-03 03:01:46.994451	2020-09-03 03:01:46.994451
208	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 03:01:47.002493	2020-09-03 03:01:47.002493
209	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-03 03:13:13.542307	2020-09-03 03:13:13.542307
210	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 03:13:13.560609	2020-09-03 03:13:13.560609
211	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-03 03:13:13.586498	2020-09-03 03:13:13.586498
212	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 03:13:13.593823	2020-09-03 03:13:13.593823
213	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-03 03:15:00.737434	2020-09-03 03:15:00.737434
214	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 03:15:00.757639	2020-09-03 03:15:00.757639
215	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-03 03:15:00.784899	2020-09-03 03:15:00.784899
216	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 03:15:00.791853	2020-09-03 03:15:00.791853
217	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-03 03:19:28.282204	2020-09-03 03:19:28.282204
218	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 03:19:29.281852	2020-09-03 03:19:29.281852
219	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-03 03:19:29.307084	2020-09-03 03:19:29.307084
220	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 03:19:29.31518	2020-09-03 03:19:29.31518
221	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-03 03:41:13.929584	2020-09-03 03:41:13.929584
222	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 03:41:14.719985	2020-09-03 03:41:14.719985
223	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-03 03:41:14.753345	2020-09-03 03:41:14.753345
224	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 03:41:14.760667	2020-09-03 03:41:14.760667
225	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-03 04:04:21.789652	2020-09-03 04:04:21.789652
226	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 04:04:21.807984	2020-09-03 04:04:21.807984
227	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-03 04:04:21.833396	2020-09-03 04:04:21.833396
228	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 04:04:21.841443	2020-09-03 04:04:21.841443
229	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-03 04:21:50.705208	2020-09-03 04:21:50.705208
230	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 04:21:50.748847	2020-09-03 04:21:50.748847
231	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-03 04:21:50.787288	2020-09-03 04:21:50.787288
232	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 04:21:50.795301	2020-09-03 04:21:50.795301
233	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-03 04:27:19.297633	2020-09-03 04:27:19.297633
234	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 04:27:19.314603	2020-09-03 04:27:19.314603
235	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-03 04:27:19.339953	2020-09-03 04:27:19.339953
236	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 04:27:19.347964	2020-09-03 04:27:19.347964
237	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-03 04:28:51.500458	2020-09-03 04:28:51.500458
238	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 04:28:51.520052	2020-09-03 04:28:51.520052
239	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-03 04:28:51.545527	2020-09-03 04:28:51.545527
240	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 04:28:51.553592	2020-09-03 04:28:51.553592
241	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-03 04:35:12.078298	2020-09-03 04:35:12.078298
242	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 04:35:12.100858	2020-09-03 04:35:12.100858
243	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-03 04:35:12.126137	2020-09-03 04:35:12.126137
244	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 04:35:12.134084	2020-09-03 04:35:12.134084
245	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-03 04:35:27.703614	2020-09-03 04:35:27.703614
246	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 04:35:27.718424	2020-09-03 04:35:27.718424
247	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-03 04:35:27.743506	2020-09-03 04:35:27.743506
248	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 04:35:27.751829	2020-09-03 04:35:27.751829
249	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-03 04:37:11.050983	2020-09-03 04:37:11.050983
250	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 04:37:11.06615	2020-09-03 04:37:11.06615
251	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-03 04:37:11.091421	2020-09-03 04:37:11.091421
252	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 04:37:11.099544	2020-09-03 04:37:11.099544
253	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-03 04:39:52.131741	2020-09-03 04:39:52.131741
254	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 04:39:52.150644	2020-09-03 04:39:52.150644
255	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-03 04:39:52.175975	2020-09-03 04:39:52.175975
256	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 04:39:52.184016	2020-09-03 04:39:52.184016
257	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 04:42:32.620095	2020-09-03 04:42:32.620095
258	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-03 04:42:32.660572	2020-09-03 04:42:32.660572
259	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 04:42:32.668598	2020-09-03 04:42:32.668598
260	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 04:44:56.008581	2020-09-03 04:44:56.008581
261	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-03 04:44:56.044161	2020-09-03 04:44:56.044161
262	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 04:44:56.060368	2020-09-03 04:44:56.060368
263	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 04:46:47.952548	2020-09-03 04:46:47.952548
264	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-03 04:46:48.027547	2020-09-03 04:46:48.027547
265	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 04:46:48.034982	2020-09-03 04:46:48.034982
266	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 04:49:57.072744	2020-09-03 04:49:57.072744
267	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-03 04:49:59.571513	2020-09-03 04:49:59.571513
268	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 04:49:59.634986	2020-09-03 04:49:59.634986
269	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 04:52:07.761687	2020-09-03 04:52:07.761687
270	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-03 04:52:07.794703	2020-09-03 04:52:07.794703
271	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 04:52:07.8027	2020-09-03 04:52:07.8027
272	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 04:53:29.354327	2020-09-03 04:53:29.354327
273	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-03 04:53:29.391235	2020-09-03 04:53:29.391235
274	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 04:53:29.399168	2020-09-03 04:53:29.399168
275	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 04:58:29.960266	2020-09-03 04:58:29.960266
276	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-03 04:58:29.991954	2020-09-03 04:58:29.991954
277	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 04:58:30.000005	2020-09-03 04:58:30.000005
278	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 05:09:49.060534	2020-09-03 05:09:49.060534
279	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-03 05:09:49.099983	2020-09-03 05:09:49.099983
280	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 05:09:49.10717	2020-09-03 05:09:49.10717
281	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-03 05:14:55.060773	2020-09-03 05:14:55.060773
282	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 05:14:55.444061	2020-09-03 05:14:55.444061
283	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-03 05:14:56.692168	2020-09-03 05:14:56.692168
284	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 05:14:56.700194	2020-09-03 05:14:56.700194
285	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-03 05:20:42.683798	2020-09-03 05:20:42.683798
286	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 05:20:42.705339	2020-09-03 05:20:42.705339
287	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-03 05:20:42.730826	2020-09-03 05:20:42.730826
288	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 05:20:42.738345	2020-09-03 05:20:42.738345
289	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-03 05:22:47.442284	2020-09-03 05:22:47.442284
290	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 05:22:48.111491	2020-09-03 05:22:48.111491
291	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-03 05:22:48.338605	2020-09-03 05:22:48.338605
292	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 05:22:48.345915	2020-09-03 05:22:48.345915
293	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-03 05:26:52.374283	2020-09-03 05:26:52.374283
294	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 05:26:52.384047	2020-09-03 05:26:52.384047
295	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-03 05:26:52.411588	2020-09-03 05:26:52.411588
296	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 05:26:52.418445	2020-09-03 05:26:52.418445
297	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-03 05:29:51.139203	2020-09-03 05:29:51.139203
298	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 05:29:51.161253	2020-09-03 05:29:51.161253
299	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-03 05:29:51.188011	2020-09-03 05:29:51.188011
300	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 05:29:51.195489	2020-09-03 05:29:51.195489
301	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-03 05:33:00.841939	2020-09-03 05:33:00.841939
302	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 05:33:00.865356	2020-09-03 05:33:00.865356
303	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-03 05:33:00.891224	2020-09-03 05:33:00.891224
304	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 05:33:00.89862	2020-09-03 05:33:00.89862
305	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-03 05:34:36.805302	2020-09-03 05:34:36.805302
306	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 05:34:36.829585	2020-09-03 05:34:36.829585
307	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-03 05:34:36.855658	2020-09-03 05:34:36.855658
308	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 05:34:36.862886	2020-09-03 05:34:36.862886
309	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-03 05:44:54.236704	2020-09-03 05:44:54.236704
310	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 05:44:54.265921	2020-09-03 05:44:54.265921
311	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-03 05:44:54.29192	2020-09-03 05:44:54.29192
312	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 05:44:54.299018	2020-09-03 05:44:54.299018
313	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-03 07:06:46.349365	2020-09-03 07:06:46.349365
314	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 07:06:46.375989	2020-09-03 07:06:46.375989
315	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-03 07:06:46.402464	2020-09-03 07:06:46.402464
316	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 07:06:46.409935	2020-09-03 07:06:46.409935
317	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-03 07:13:36.356144	2020-09-03 07:13:36.356144
318	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 07:13:37.566496	2020-09-03 07:13:37.566496
319	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-03 07:13:37.700208	2020-09-03 07:13:37.700208
320	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 07:13:37.708028	2020-09-03 07:13:37.708028
321	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-03 07:47:43.385998	2020-09-03 07:47:43.385998
322	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 07:47:43.403769	2020-09-03 07:47:43.403769
323	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-03 07:47:43.429577	2020-09-03 07:47:43.429577
324	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-03 07:47:43.437017	2020-09-03 07:47:43.437017
325	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-05 01:34:31.687621	2020-09-05 01:34:31.687621
326	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-05 01:34:31.710172	2020-09-05 01:34:31.710172
327	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-05 01:34:31.735307	2020-09-05 01:34:31.735307
328	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-05 01:34:31.74422	2020-09-05 01:34:31.74422
329	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-05 01:48:37.652212	2020-09-05 01:48:37.652212
330	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-05 01:48:38.540619	2020-09-05 01:48:38.540619
331	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-05 01:48:40.668437	2020-09-05 01:48:40.668437
332	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-05 01:48:40.676387	2020-09-05 01:48:40.676387
333	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-05 02:17:06.160451	2020-09-05 02:17:06.160451
334	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-05 02:17:06.176578	2020-09-05 02:17:06.176578
335	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-05 02:17:06.201896	2020-09-05 02:17:06.201896
336	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-05 02:17:06.209875	2020-09-05 02:17:06.209875
337	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-05 02:48:10.633038	2020-09-05 02:48:10.633038
338	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-05 02:48:10.652779	2020-09-05 02:48:10.652779
339	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-05 02:48:10.678161	2020-09-05 02:48:10.678161
340	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-05 02:48:10.686148	2020-09-05 02:48:10.686148
341	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-05 02:53:11.117838	2020-09-05 02:53:11.117838
342	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-05 02:53:12.778319	2020-09-05 02:53:12.778319
343	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-05 02:53:12.820991	2020-09-05 02:53:12.820991
344	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-05 02:53:12.829053	2020-09-05 02:53:12.829053
345	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-05 03:15:09.044891	2020-09-05 03:15:09.044891
346	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-05 03:15:09.064751	2020-09-05 03:15:09.064751
347	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-05 03:15:09.090128	2020-09-05 03:15:09.090128
348	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-05 03:15:09.098095	2020-09-05 03:15:09.098095
349	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-05 03:18:24.245952	2020-09-05 03:18:24.245952
350	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-05 03:18:24.268102	2020-09-05 03:18:24.268102
351	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-05 03:18:24.301607	2020-09-05 03:18:24.301607
352	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-05 03:18:24.309564	2020-09-05 03:18:24.309564
353	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-05 03:21:08.835618	2020-09-05 03:21:08.835618
354	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-05 03:21:08.852574	2020-09-05 03:21:08.852574
355	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-05 03:21:08.877913	2020-09-05 03:21:08.877913
356	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-05 03:21:08.885964	2020-09-05 03:21:08.885964
357	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-05 03:21:43.888129	2020-09-05 03:21:43.888129
358	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-05 03:21:43.905829	2020-09-05 03:21:43.905829
359	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-05 03:21:43.931912	2020-09-05 03:21:43.931912
360	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-05 03:21:43.938939	2020-09-05 03:21:43.938939
361	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-05 03:22:21.242807	2020-09-05 03:22:21.242807
362	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-05 03:22:21.257024	2020-09-05 03:22:21.257024
363	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-05 03:22:21.282296	2020-09-05 03:22:21.282296
364	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-05 03:22:21.290303	2020-09-05 03:22:21.290303
365	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-06 00:53:49.65916	2020-09-06 00:53:49.65916
366	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-06 00:53:49.680162	2020-09-06 00:53:49.680162
367	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-06 00:53:49.70684	2020-09-06 00:53:49.70684
368	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-06 00:53:49.713156	2020-09-06 00:53:49.713156
369	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-06 00:59:59.371224	2020-09-06 00:59:59.371224
370	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-06 00:59:59.393468	2020-09-06 00:59:59.393468
371	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-06 00:59:59.418683	2020-09-06 00:59:59.418683
372	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-06 00:59:59.425758	2020-09-06 00:59:59.425758
373	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-06 01:06:12.484326	2020-09-06 01:06:12.484326
374	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-06 01:06:12.656619	2020-09-06 01:06:12.656619
375	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-06 01:06:13.164754	2020-09-06 01:06:13.164754
376	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-06 01:06:13.172707	2020-09-06 01:06:13.172707
377	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-06 01:13:48.881641	2020-09-06 01:13:48.881641
378	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-06 01:13:48.901553	2020-09-06 01:13:48.901553
379	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-06 01:13:48.926685	2020-09-06 01:13:48.926685
380	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-06 01:13:48.93458	2020-09-06 01:13:48.93458
381	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-06 01:18:55.395836	2020-09-06 01:18:55.395836
382	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-06 01:18:55.412047	2020-09-06 01:18:55.412047
383	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-06 01:18:55.43931	2020-09-06 01:18:55.43931
384	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-06 01:18:55.446671	2020-09-06 01:18:55.446671
385	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-06 01:22:43.569579	2020-09-06 01:22:43.569579
386	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-06 01:22:43.5937	2020-09-06 01:22:43.5937
387	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-06 01:22:43.618961	2020-09-06 01:22:43.618961
388	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-06 01:22:43.625723	2020-09-06 01:22:43.625723
389	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-06 01:27:02.083742	2020-09-06 01:27:02.083742
390	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-06 01:27:02.100908	2020-09-06 01:27:02.100908
391	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-06 01:27:02.126154	2020-09-06 01:27:02.126154
392	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-06 01:27:02.134286	2020-09-06 01:27:02.134286
393	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-06 01:29:03.612633	2020-09-06 01:29:03.612633
394	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-06 01:29:03.631307	2020-09-06 01:29:03.631307
395	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-06 01:29:03.656499	2020-09-06 01:29:03.656499
396	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-06 01:29:03.664563	2020-09-06 01:29:03.664563
397	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-06 02:15:03.874599	2020-09-06 02:15:03.874599
398	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-06 02:15:03.893307	2020-09-06 02:15:03.893307
399	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-06 02:15:03.91859	2020-09-06 02:15:03.91859
400	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-06 02:15:03.926603	2020-09-06 02:15:03.926603
401	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-06 02:25:01.010437	2020-09-06 02:25:01.010437
402	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-06 02:25:01.028526	2020-09-06 02:25:01.028526
403	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-06 02:25:01.053814	2020-09-06 02:25:01.053814
404	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-06 02:25:01.061691	2020-09-06 02:25:01.061691
405	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-06 02:32:29.22968	2020-09-06 02:32:29.22968
406	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-06 02:32:29.246423	2020-09-06 02:32:29.246423
407	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-06 02:32:29.271859	2020-09-06 02:32:29.271859
408	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-06 02:32:29.279644	2020-09-06 02:32:29.279644
409	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-06 02:42:52.413979	2020-09-06 02:42:52.413979
410	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-06 02:42:52.433079	2020-09-06 02:42:52.433079
411	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-06 02:42:52.466873	2020-09-06 02:42:52.466873
412	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-06 02:42:52.474911	2020-09-06 02:42:52.474911
413	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-06 03:38:25.661212	2020-09-06 03:38:25.661212
414	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-06 03:38:25.678485	2020-09-06 03:38:25.678485
415	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-06 03:38:25.70377	2020-09-06 03:38:25.70377
416	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-06 03:38:25.711801	2020-09-06 03:38:25.711801
417	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-06 03:43:57.970773	2020-09-06 03:43:57.970773
418	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-06 03:43:57.98954	2020-09-06 03:43:57.98954
419	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-06 03:43:58.014914	2020-09-06 03:43:58.014914
420	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-06 03:43:58.022813	2020-09-06 03:43:58.022813
421	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-06 03:44:50.629958	2020-09-06 03:44:50.629958
422	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-06 03:44:51.184682	2020-09-06 03:44:51.184682
423	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-06 03:44:52.351521	2020-09-06 03:44:52.351521
424	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-06 03:44:52.359472	2020-09-06 03:44:52.359472
425	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-07 02:20:18.186281	2020-09-07 02:20:18.186281
426	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-07 02:20:18.198278	2020-09-07 02:20:18.198278
427	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-07 02:20:18.223612	2020-09-07 02:20:18.223612
428	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-07 02:20:18.231691	2020-09-07 02:20:18.231691
429	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-07 02:42:23.135413	2020-09-07 02:42:23.135413
430	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-07 02:42:23.159226	2020-09-07 02:42:23.159226
431	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-07 02:42:23.273143	2020-09-07 02:42:23.273143
432	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-07 02:42:23.354668	2020-09-07 02:42:23.354668
433	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-07 03:10:17.570419	2020-09-07 03:10:17.570419
434	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-07 03:10:17.618289	2020-09-07 03:10:17.618289
435	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-07 03:10:17.657804	2020-09-07 03:10:17.657804
436	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-07 03:10:17.665753	2020-09-07 03:10:17.665753
437	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-07 03:17:23.024364	2020-09-07 03:17:23.024364
438	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-07 03:17:23.04074	2020-09-07 03:17:23.04074
439	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-07 03:17:23.066036	2020-09-07 03:17:23.066036
440	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-07 03:17:23.074154	2020-09-07 03:17:23.074154
441	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-07 04:16:36.533347	2020-09-07 04:16:36.533347
442	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-07 04:16:38.217279	2020-09-07 04:16:38.217279
443	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-07 04:16:38.249432	2020-09-07 04:16:38.249432
444	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-07 04:16:38.25737	2020-09-07 04:16:38.25737
445	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-07 04:21:27.885333	2020-09-07 04:21:27.885333
446	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-07 04:21:27.908198	2020-09-07 04:21:27.908198
447	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-07 04:21:27.933685	2020-09-07 04:21:27.933685
448	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-07 04:21:27.941557	2020-09-07 04:21:27.941557
449	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-07 04:43:57.519466	2020-09-07 04:43:57.519466
450	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-07 04:43:58.678768	2020-09-07 04:43:58.678768
451	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-07 04:43:58.73779	2020-09-07 04:43:58.73779
452	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-07 04:43:58.745546	2020-09-07 04:43:58.745546
453	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-07 05:00:05.550032	2020-09-07 05:00:05.550032
454	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-07 05:00:05.568768	2020-09-07 05:00:05.568768
455	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-07 05:00:05.595339	2020-09-07 05:00:05.595339
456	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-07 05:00:05.603298	2020-09-07 05:00:05.603298
457	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-07 05:00:29.263771	2020-09-07 05:00:29.263771
458	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-07 05:00:29.286726	2020-09-07 05:00:29.286726
459	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-07 05:00:29.311978	2020-09-07 05:00:29.311978
460	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-07 05:00:29.320006	2020-09-07 05:00:29.320006
461	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-07 05:04:47.902474	2020-09-07 05:04:47.902474
462	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-07 05:04:50.222863	2020-09-07 05:04:50.222863
463	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-07 05:04:50.28824	2020-09-07 05:04:50.28824
464	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-07 05:04:50.294746	2020-09-07 05:04:50.294746
465	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-07 05:12:51.349604	2020-09-07 05:12:51.349604
466	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-07 05:12:51.363956	2020-09-07 05:12:51.363956
467	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-07 05:12:51.389345	2020-09-07 05:12:51.389345
468	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-07 05:12:51.397437	2020-09-07 05:12:51.397437
469	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-07 05:40:07.552329	2020-09-07 05:40:07.552329
470	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-07 05:40:07.568381	2020-09-07 05:40:07.568381
471	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-07 05:40:07.602217	2020-09-07 05:40:07.602217
472	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-07 05:40:07.610136	2020-09-07 05:40:07.610136
473	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-07 06:18:01.018657	2020-09-07 06:18:01.018657
474	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-07 06:18:01.035704	2020-09-07 06:18:01.035704
475	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-07 06:18:01.061584	2020-09-07 06:18:01.061584
476	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-07 06:18:01.069125	2020-09-07 06:18:01.069125
477	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-07 06:43:26.92441	2020-09-07 06:43:26.92441
478	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-07 06:43:27.007602	2020-09-07 06:43:27.007602
479	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-07 06:43:27.032847	2020-09-07 06:43:27.032847
480	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-07 06:43:27.04063	2020-09-07 06:43:27.04063
481	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-07 07:30:53.402989	2020-09-07 07:30:53.402989
482	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-07 07:30:53.423875	2020-09-07 07:30:53.423875
483	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-07 07:30:53.44905	2020-09-07 07:30:53.44905
484	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-07 07:30:53.457015	2020-09-07 07:30:53.457015
485	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-08 22:52:13.653534	2020-09-08 22:52:13.653534
486	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-08 22:52:13.921871	2020-09-08 22:52:13.921871
487	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-08 22:52:14.094403	2020-09-08 22:52:14.094403
488	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-08 22:52:14.138415	2020-09-08 22:52:14.138415
489	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-08 23:14:31.058832	2020-09-08 23:14:31.058832
490	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-08 23:14:31.073968	2020-09-08 23:14:31.073968
491	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-08 23:14:31.099114	2020-09-08 23:14:31.099114
492	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-08 23:14:31.107201	2020-09-08 23:14:31.107201
493	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-08 23:16:19.070798	2020-09-08 23:16:19.070798
494	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-08 23:16:19.089193	2020-09-08 23:16:19.089193
495	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-08 23:16:19.114865	2020-09-08 23:16:19.114865
496	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-08 23:16:19.122763	2020-09-08 23:16:19.122763
497	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-09 00:39:35.506989	2020-09-09 00:39:35.506989
498	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-09 00:39:35.529197	2020-09-09 00:39:35.529197
499	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-09 00:39:35.555775	2020-09-09 00:39:35.555775
500	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-09 00:39:35.563685	2020-09-09 00:39:35.563685
501	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-09 00:54:15.644985	2020-09-09 00:54:15.644985
502	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-09 00:54:15.663764	2020-09-09 00:54:15.663764
503	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-09 00:54:15.689153	2020-09-09 00:54:15.689153
504	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-09 00:54:15.697163	2020-09-09 00:54:15.697163
505	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-09 00:58:27.879058	2020-09-09 00:58:27.879058
506	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-09 00:58:27.896778	2020-09-09 00:58:27.896778
507	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-09 00:58:27.922985	2020-09-09 00:58:27.922985
508	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-09 00:58:27.93026	2020-09-09 00:58:27.93026
509	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-09 01:05:28.510014	2020-09-09 01:05:28.510014
510	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-09 01:05:29.562008	2020-09-09 01:05:29.562008
511	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-09 01:05:29.662355	2020-09-09 01:05:29.662355
512	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-09 01:05:29.670249	2020-09-09 01:05:29.670249
513	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-09 22:41:50.528967	2020-09-09 22:41:50.528967
514	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-09 22:41:50.764688	2020-09-09 22:41:50.764688
515	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-09 22:41:50.969764	2020-09-09 22:41:50.969764
516	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-09 22:41:51.006433	2020-09-09 22:41:51.006433
517	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-09 22:48:33.067294	2020-09-09 22:48:33.067294
518	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-09 22:48:33.088292	2020-09-09 22:48:33.088292
519	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-09 22:48:33.11381	2020-09-09 22:48:33.11381
520	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-09 22:48:33.121716	2020-09-09 22:48:33.121716
521	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-09 22:58:44.297735	2020-09-09 22:58:44.297735
522	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-09 22:58:44.315816	2020-09-09 22:58:44.315816
523	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-09 22:58:44.341208	2020-09-09 22:58:44.341208
524	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-09 22:58:44.349255	2020-09-09 22:58:44.349255
525	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-09 22:59:21.317623	2020-09-09 22:59:21.317623
526	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-09 22:59:21.334624	2020-09-09 22:59:21.334624
527	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-09 22:59:21.361498	2020-09-09 22:59:21.361498
528	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-09 22:59:21.369157	2020-09-09 22:59:21.369157
529	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-09 23:00:31.615932	2020-09-09 23:00:31.615932
530	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-09 23:00:31.630776	2020-09-09 23:00:31.630776
531	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-09 23:00:31.739186	2020-09-09 23:00:31.739186
532	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-09 23:00:31.747163	2020-09-09 23:00:31.747163
533	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-09 23:01:13.051557	2020-09-09 23:01:13.051557
534	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-09 23:01:13.066301	2020-09-09 23:01:13.066301
535	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-09 23:01:13.099752	2020-09-09 23:01:13.099752
536	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-09 23:01:13.107806	2020-09-09 23:01:13.107806
537	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-09 23:03:20.924897	2020-09-09 23:03:20.924897
538	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-09 23:03:20.940312	2020-09-09 23:03:20.940312
539	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-09 23:03:20.975627	2020-09-09 23:03:20.975627
540	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-09 23:03:20.982916	2020-09-09 23:03:20.982916
541	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-09 23:05:11.865935	2020-09-09 23:05:11.865935
542	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-09 23:05:11.888645	2020-09-09 23:05:11.888645
543	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-09 23:05:11.913873	2020-09-09 23:05:11.913873
544	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-09 23:05:11.921853	2020-09-09 23:05:11.921853
545	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-09 23:10:00.836962	2020-09-09 23:10:00.836962
546	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-09 23:10:00.847159	2020-09-09 23:10:00.847159
547	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-09 23:10:00.872523	2020-09-09 23:10:00.872523
548	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-09 23:10:00.880491	2020-09-09 23:10:00.880491
549	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-09 23:12:32.498472	2020-09-09 23:12:32.498472
550	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-09 23:12:32.656188	2020-09-09 23:12:32.656188
551	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-09 23:12:32.69815	2020-09-09 23:12:32.69815
552	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-09 23:12:32.70589	2020-09-09 23:12:32.70589
553	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-10 19:53:48.50395	2020-09-10 19:53:48.50395
554	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-10 19:53:49.210063	2020-09-10 19:53:49.210063
555	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-10 19:53:49.24918	2020-09-10 19:53:49.24918
556	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-10 19:53:49.257153	2020-09-10 19:53:49.257153
557	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-10 19:55:31.830606	2020-09-10 19:55:31.830606
558	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-10 19:55:31.871698	2020-09-10 19:55:31.871698
559	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-10 19:55:31.896802	2020-09-10 19:55:31.896802
560	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-10 19:55:31.904868	2020-09-10 19:55:31.904868
561	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-10 19:56:45.806084	2020-09-10 19:56:45.806084
562	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-10 19:56:45.825858	2020-09-10 19:56:45.825858
563	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-10 19:56:45.851246	2020-09-10 19:56:45.851246
564	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-10 19:56:45.859191	2020-09-10 19:56:45.859191
565	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-10 20:02:54.601453	2020-09-10 20:02:54.601453
566	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-10 20:02:54.623031	2020-09-10 20:02:54.623031
567	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-10 20:02:54.648143	2020-09-10 20:02:54.648143
568	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-10 20:02:54.65623	2020-09-10 20:02:54.65623
569	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-10 20:04:04.649477	2020-09-10 20:04:04.649477
570	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-10 20:04:04.792153	2020-09-10 20:04:04.792153
571	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-10 20:04:04.827468	2020-09-10 20:04:04.827468
572	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-10 20:04:04.835408	2020-09-10 20:04:04.835408
573	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-10 20:05:52.338013	2020-09-10 20:05:52.338013
574	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-10 20:05:52.433644	2020-09-10 20:05:52.433644
575	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-10 20:05:52.466946	2020-09-10 20:05:52.466946
576	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-10 20:05:52.475038	2020-09-10 20:05:52.475038
577	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-10 20:19:56.080768	2020-09-10 20:19:56.080768
578	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-10 20:19:56.099427	2020-09-10 20:19:56.099427
579	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-10 20:19:56.124829	2020-09-10 20:19:56.124829
580	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-10 20:19:56.1328	2020-09-10 20:19:56.1328
581	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-10 20:20:58.50855	2020-09-10 20:20:58.50855
582	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-10 20:20:58.52818	2020-09-10 20:20:58.52818
583	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-10 20:20:58.553425	2020-09-10 20:20:58.553425
584	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-10 20:20:58.561615	2020-09-10 20:20:58.561615
585	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-10 20:24:06.071064	2020-09-10 20:24:06.071064
586	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-10 20:24:06.089415	2020-09-10 20:24:06.089415
587	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-10 20:24:06.11471	2020-09-10 20:24:06.11471
588	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-10 20:24:06.123085	2020-09-10 20:24:06.123085
589	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-10 20:25:00.57295	2020-09-10 20:25:00.57295
590	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-10 20:25:00.685719	2020-09-10 20:25:00.685719
591	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-10 20:25:00.726464	2020-09-10 20:25:00.726464
592	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-10 20:25:00.734366	2020-09-10 20:25:00.734366
593	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-10 20:25:46.934743	2020-09-10 20:25:46.934743
594	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-10 20:25:46.953753	2020-09-10 20:25:46.953753
595	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-10 20:25:46.979113	2020-09-10 20:25:46.979113
596	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-10 20:25:46.987075	2020-09-10 20:25:46.987075
597	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-10 20:26:43.409815	2020-09-10 20:26:43.409815
598	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-10 20:26:43.432061	2020-09-10 20:26:43.432061
599	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-10 20:26:43.457317	2020-09-10 20:26:43.457317
600	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-10 20:26:43.465403	2020-09-10 20:26:43.465403
601	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-10 20:27:14.32394	2020-09-10 20:27:14.32394
602	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-10 20:27:14.342321	2020-09-10 20:27:14.342321
603	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-10 20:27:14.367497	2020-09-10 20:27:14.367497
604	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-10 20:27:14.375591	2020-09-10 20:27:14.375591
605	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-10 20:29:05.909011	2020-09-10 20:29:05.909011
606	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-10 20:29:06.274344	2020-09-10 20:29:06.274344
607	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-10 20:29:07.040826	2020-09-10 20:29:07.040826
608	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-10 20:29:07.048892	2020-09-10 20:29:07.048892
609	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-10 20:30:12.956357	2020-09-10 20:30:12.956357
610	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-10 20:30:13.527976	2020-09-10 20:30:13.527976
611	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-10 20:30:13.569817	2020-09-10 20:30:13.569817
612	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-10 20:30:13.577829	2020-09-10 20:30:13.577829
613	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-10 20:43:19.631738	2020-09-10 20:43:19.631738
614	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-10 20:43:19.974332	2020-09-10 20:43:19.974332
615	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-10 20:43:20.049632	2020-09-10 20:43:20.049632
616	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-10 20:43:20.05744	2020-09-10 20:43:20.05744
617	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-10 20:51:28.790027	2020-09-10 20:51:28.790027
618	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-10 20:51:28.978244	2020-09-10 20:51:28.978244
619	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-10 20:51:30.119934	2020-09-10 20:51:30.119934
620	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-10 20:51:30.128162	2020-09-10 20:51:30.128162
621	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-11 19:24:41.677251	2020-09-11 19:24:41.677251
622	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-11 19:24:41.936936	2020-09-11 19:24:41.936936
623	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-11 19:24:42.16063	2020-09-11 19:24:42.16063
624	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-11 19:24:42.199039	2020-09-11 19:24:42.199039
625	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-11 19:25:57.558348	2020-09-11 19:25:57.558348
626	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-11 19:25:57.578611	2020-09-11 19:25:57.578611
627	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-11 19:53:15.794498	2020-09-11 19:53:15.794498
628	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-11 19:53:15.815753	2020-09-11 19:53:15.815753
629	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-11 19:53:15.883448	2020-09-11 19:53:15.883448
630	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-11 19:53:15.924783	2020-09-11 19:53:15.924783
631	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-11 19:54:02.09573	2020-09-11 19:54:02.09573
632	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-11 19:54:02.109344	2020-09-11 19:54:02.109344
633	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-11 19:54:02.134802	2020-09-11 19:54:02.134802
634	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-11 19:54:02.142835	2020-09-11 19:54:02.142835
635	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-11 19:56:14.397545	2020-09-11 19:56:14.397545
636	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-11 19:56:14.416879	2020-09-11 19:56:14.416879
637	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-11 19:56:14.442165	2020-09-11 19:56:14.442165
638	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-11 19:56:14.45019	2020-09-11 19:56:14.45019
639	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-11 20:03:08.203469	2020-09-11 20:03:08.203469
640	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-11 20:03:08.274417	2020-09-11 20:03:08.274417
641	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-11 20:03:08.299851	2020-09-11 20:03:08.299851
642	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-11 20:03:08.3078	2020-09-11 20:03:08.3078
643	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-12 01:25:33.915829	2020-09-12 01:25:33.915829
644	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 01:25:33.933665	2020-09-12 01:25:33.933665
645	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-12 01:25:33.958907	2020-09-12 01:25:33.958907
646	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 01:25:33.966918	2020-09-12 01:25:33.966918
647	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-12 01:38:42.892651	2020-09-12 01:38:42.892651
648	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 01:38:42.912935	2020-09-12 01:38:42.912935
649	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-12 01:38:42.938277	2020-09-12 01:38:42.938277
650	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 01:38:42.946303	2020-09-12 01:38:42.946303
651	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-12 01:41:41.752925	2020-09-12 01:41:41.752925
652	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 01:41:43.132547	2020-09-12 01:41:43.132547
653	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-12 01:41:43.173944	2020-09-12 01:41:43.173944
654	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 01:41:43.182216	2020-09-12 01:41:43.182216
655	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-12 01:48:43.328082	2020-09-12 01:48:43.328082
656	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 01:48:43.349489	2020-09-12 01:48:43.349489
657	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-12 01:48:43.376775	2020-09-12 01:48:43.376775
658	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 01:48:43.383823	2020-09-12 01:48:43.383823
659	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-12 01:53:16.656481	2020-09-12 01:53:16.656481
660	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 01:53:16.673854	2020-09-12 01:53:16.673854
661	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-12 01:53:16.699365	2020-09-12 01:53:16.699365
662	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 01:53:16.707269	2020-09-12 01:53:16.707269
663	90	10	0	0	80	40	0	0	400	SUB_MACHINE_GUN	30.8129999999999988	31	1	14	1	30	0	13	3	4.29999999999999982	5	180	818	20	SUB_MACHINE_GUN	5	Heckler and Koch	MP5	5	COMMON	mp5.yml	2020-09-12 02:10:31.797634	2020-09-12 02:10:31.797634
664	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 02:10:31.809171	2020-09-12 02:10:31.809171
665	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-12 02:41:35.982274	2020-09-12 02:41:35.982274
666	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 02:41:36.0013	2020-09-12 02:41:36.0013
667	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-12 02:41:36.026647	2020-09-12 02:41:36.026647
668	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 02:41:36.03471	2020-09-12 02:41:36.03471
669	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-12 02:44:04.915823	2020-09-12 02:44:04.915823
670	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 02:44:04.985294	2020-09-12 02:44:04.985294
671	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-12 02:44:05.010792	2020-09-12 02:44:05.010792
672	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 02:44:05.018613	2020-09-12 02:44:05.018613
673	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-12 05:11:58.298054	2020-09-12 05:11:58.298054
674	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 05:11:58.320687	2020-09-12 05:11:58.320687
675	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-12 05:11:58.346072	2020-09-12 05:11:58.346072
676	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 05:11:58.354018	2020-09-12 05:11:58.354018
677	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-12 06:48:19.954451	2020-09-12 06:48:19.954451
678	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 06:48:19.975301	2020-09-12 06:48:19.975301
679	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-12 06:48:20.005025	2020-09-12 06:48:20.005025
680	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 06:48:20.010141	2020-09-12 06:48:20.010141
681	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-12 07:48:52.19276	2020-09-12 07:48:52.19276
682	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 07:48:52.212752	2020-09-12 07:48:52.212752
683	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-12 07:48:52.238059	2020-09-12 07:48:52.238059
684	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 07:48:52.246043	2020-09-12 07:48:52.246043
685	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-12 07:53:15.757588	2020-09-12 07:53:15.757588
686	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 07:53:16.454568	2020-09-12 07:53:16.454568
687	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-12 07:53:16.812103	2020-09-12 07:53:16.812103
688	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 07:53:16.820155	2020-09-12 07:53:16.820155
689	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-12 07:53:50.32136	2020-09-12 07:53:50.32136
690	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 07:53:50.338936	2020-09-12 07:53:50.338936
691	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-12 07:53:50.364316	2020-09-12 07:53:50.364316
692	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 07:53:50.37213	2020-09-12 07:53:50.37213
693	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-12 08:02:36.021529	2020-09-12 08:02:36.021529
694	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 08:02:36.037192	2020-09-12 08:02:36.037192
695	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-12 08:02:36.063161	2020-09-12 08:02:36.063161
696	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 08:02:36.070764	2020-09-12 08:02:36.070764
697	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-12 08:13:51.639544	2020-09-12 08:13:51.639544
698	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 08:13:51.659094	2020-09-12 08:13:51.659094
699	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-12 08:13:51.68455	2020-09-12 08:13:51.68455
700	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 08:13:51.692603	2020-09-12 08:13:51.692603
701	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-12 08:19:55.258344	2020-09-12 08:19:55.258344
702	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 08:19:55.280637	2020-09-12 08:19:55.280637
703	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-12 08:19:55.30577	2020-09-12 08:19:55.30577
704	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 08:19:55.314003	2020-09-12 08:19:55.314003
705	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-12 08:23:24.183466	2020-09-12 08:23:24.183466
706	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 08:23:24.201354	2020-09-12 08:23:24.201354
707	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-12 08:23:24.226775	2020-09-12 08:23:24.226775
708	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 08:23:24.234758	2020-09-12 08:23:24.234758
709	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-12 08:29:07.310781	2020-09-12 08:29:07.310781
710	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 08:29:07.329791	2020-09-12 08:29:07.329791
711	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-12 08:29:07.36344	2020-09-12 08:29:07.36344
712	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 08:29:07.371454	2020-09-12 08:29:07.371454
713	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-12 08:38:38.513176	2020-09-12 08:38:38.513176
714	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 08:38:38.530614	2020-09-12 08:38:38.530614
715	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-12 08:38:38.556055	2020-09-12 08:38:38.556055
716	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 08:38:38.563965	2020-09-12 08:38:38.563965
717	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-12 08:39:23.388507	2020-09-12 08:39:23.388507
718	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 08:39:23.408322	2020-09-12 08:39:23.408322
719	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-12 08:39:23.433555	2020-09-12 08:39:23.433555
720	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 08:39:23.441591	2020-09-12 08:39:23.441591
721	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-12 08:45:56.958042	2020-09-12 08:45:56.958042
722	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 08:45:59.290013	2020-09-12 08:45:59.290013
723	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-12 08:45:59.348359	2020-09-12 08:45:59.348359
724	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 08:45:59.356409	2020-09-12 08:45:59.356409
725	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-12 08:53:57.05937	2020-09-12 08:53:57.05937
726	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 08:53:57.067833	2020-09-12 08:53:57.067833
727	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-12 08:53:57.093285	2020-09-12 08:53:57.093285
728	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 08:53:57.101418	2020-09-12 08:53:57.101418
729	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-12 08:56:04.340353	2020-09-12 08:56:04.340353
730	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 08:56:05.009129	2020-09-12 08:56:05.009129
731	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-12 08:56:05.95932	2020-09-12 08:56:05.95932
732	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 08:56:05.967345	2020-09-12 08:56:05.967345
733	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-12 08:56:53.13482	2020-09-12 08:56:53.13482
734	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 08:56:53.153371	2020-09-12 08:56:53.153371
735	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-12 08:56:53.178623	2020-09-12 08:56:53.178623
736	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 08:56:53.187278	2020-09-12 08:56:53.187278
737	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-12 09:08:07.978447	2020-09-12 09:08:07.978447
738	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 09:08:07.99331	2020-09-12 09:08:07.99331
739	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-12 09:08:08.018632	2020-09-12 09:08:08.018632
740	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 09:08:08.026642	2020-09-12 09:08:08.026642
741	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-12 09:08:56.907788	2020-09-12 09:08:56.907788
742	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 09:08:56.930331	2020-09-12 09:08:56.930331
743	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-12 09:08:56.956073	2020-09-12 09:08:56.956073
744	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 09:08:56.963922	2020-09-12 09:08:56.963922
745	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-12 09:13:05.689542	2020-09-12 09:13:05.689542
746	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 09:13:05.944137	2020-09-12 09:13:05.944137
747	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-12 09:13:07.169507	2020-09-12 09:13:07.169507
748	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 09:13:07.177491	2020-09-12 09:13:07.177491
749	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-12 09:33:08.197029	2020-09-12 09:33:08.197029
750	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 09:33:08.206064	2020-09-12 09:33:08.206064
751	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-12 09:33:08.233059	2020-09-12 09:33:08.233059
752	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 09:33:08.240442	2020-09-12 09:33:08.240442
753	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-12 09:35:39.801597	2020-09-12 09:35:39.801597
754	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 09:35:39.823375	2020-09-12 09:35:39.823375
755	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-12 09:35:39.848623	2020-09-12 09:35:39.848623
756	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 09:35:39.857663	2020-09-12 09:35:39.857663
757	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-12 10:13:30.622892	2020-09-12 10:13:30.622892
758	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 10:13:30.639912	2020-09-12 10:13:30.639912
759	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-12 10:13:30.666412	2020-09-12 10:13:30.666412
760	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 10:13:30.673834	2020-09-12 10:13:30.673834
761	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-12 10:41:58.390543	2020-09-12 10:41:58.390543
762	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 10:41:58.406502	2020-09-12 10:41:58.406502
763	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-12 10:41:58.432176	2020-09-12 10:41:58.432176
764	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 10:41:58.439927	2020-09-12 10:41:58.439927
765	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-12 10:45:42.062371	2020-09-12 10:45:42.062371
766	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 10:45:42.077969	2020-09-12 10:45:42.077969
767	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-12 10:45:42.103636	2020-09-12 10:45:42.103636
768	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 10:45:42.111577	2020-09-12 10:45:42.111577
769	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-12 11:14:30.232761	2020-09-12 11:14:30.232761
770	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 11:14:30.25572	2020-09-12 11:14:30.25572
771	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-12 11:14:30.28149	2020-09-12 11:14:30.28149
772	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 11:14:30.28892	2020-09-12 11:14:30.28892
773	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-12 11:35:55.02217	2020-09-12 11:35:55.02217
774	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 11:35:55.038883	2020-09-12 11:35:55.038883
775	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-12 11:35:55.064482	2020-09-12 11:35:55.064482
776	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 11:35:55.072254	2020-09-12 11:35:55.072254
777	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-12 11:38:41.643049	2020-09-12 11:38:41.643049
778	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 11:38:41.664895	2020-09-12 11:38:41.664895
779	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-12 11:38:41.690188	2020-09-12 11:38:41.690188
780	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 11:38:41.698149	2020-09-12 11:38:41.698149
781	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-12 11:47:24.299421	2020-09-12 11:47:24.299421
782	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 11:47:24.413929	2020-09-12 11:47:24.413929
783	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-12 11:47:24.464447	2020-09-12 11:47:24.464447
784	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 11:47:24.471697	2020-09-12 11:47:24.471697
785	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-12 11:49:07.544064	2020-09-12 11:49:07.544064
786	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 11:49:07.560144	2020-09-12 11:49:07.560144
787	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-12 11:49:07.585498	2020-09-12 11:49:07.585498
788	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 11:49:07.593492	2020-09-12 11:49:07.593492
789	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-12 11:52:26.08594	2020-09-12 11:52:26.08594
790	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 11:52:26.914351	2020-09-12 11:52:26.914351
791	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-12 11:52:27.014282	2020-09-12 11:52:27.014282
792	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 11:52:27.022351	2020-09-12 11:52:27.022351
793	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-12 12:14:13.257343	2020-09-12 12:14:13.257343
794	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 12:14:13.265522	2020-09-12 12:14:13.265522
795	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-12 12:14:13.290791	2020-09-12 12:14:13.290791
796	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 12:14:13.29879	2020-09-12 12:14:13.29879
797	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-12 14:04:11.947091	2020-09-12 14:04:11.947091
798	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 14:04:12.159722	2020-09-12 14:04:12.159722
799	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-12 14:04:12.388564	2020-09-12 14:04:12.388564
800	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 14:04:12.426347	2020-09-12 14:04:12.426347
801	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-12 14:14:16.333686	2020-09-12 14:14:16.333686
802	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 14:14:16.353286	2020-09-12 14:14:16.353286
803	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-12 14:14:16.378433	2020-09-12 14:14:16.378433
804	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 14:14:16.453477	2020-09-12 14:14:16.453477
805	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-12 14:15:34.756847	2020-09-12 14:15:34.756847
806	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 14:15:34.774717	2020-09-12 14:15:34.774717
807	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-12 14:15:34.799953	2020-09-12 14:15:34.799953
808	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 14:15:34.808011	2020-09-12 14:15:34.808011
809	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-12 14:20:12.25086	2020-09-12 14:20:12.25086
810	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 14:20:12.916329	2020-09-12 14:20:12.916329
811	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-12 14:20:13.225069	2020-09-12 14:20:13.225069
812	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 14:20:13.291146	2020-09-12 14:20:13.291146
813	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-12 14:22:26.803513	2020-09-12 14:22:26.803513
814	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 14:22:26.823823	2020-09-12 14:22:26.823823
815	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-12 14:22:26.849026	2020-09-12 14:22:26.849026
816	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 14:22:26.857141	2020-09-12 14:22:26.857141
817	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-12 14:55:20.025018	2020-09-12 14:55:20.025018
818	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 14:55:20.564497	2020-09-12 14:55:20.564497
819	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-12 14:55:22.289714	2020-09-12 14:55:22.289714
820	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 14:55:22.297759	2020-09-12 14:55:22.297759
821	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-12 16:44:38.44653	2020-09-12 16:44:38.44653
822	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 16:44:38.689713	2020-09-12 16:44:38.689713
823	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-12 16:44:38.889389	2020-09-12 16:44:38.889389
824	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 16:44:38.911084	2020-09-12 16:44:38.911084
825	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-12 17:55:32.314275	2020-09-12 17:55:32.314275
826	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 17:55:32.545539	2020-09-12 17:55:32.545539
827	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-12 17:55:32.73198	2020-09-12 17:55:32.73198
828	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 17:55:32.753785	2020-09-12 17:55:32.753785
829	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-12 19:20:46.034294	2020-09-12 19:20:46.034294
830	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 19:20:46.28571	2020-09-12 19:20:46.28571
831	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-12 19:20:46.485402	2020-09-12 19:20:46.485402
832	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-12 19:20:46.50734	2020-09-12 19:20:46.50734
833	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-15 23:25:39.919429	2020-09-15 23:25:39.919429
834	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-15 23:25:40.192673	2020-09-15 23:25:40.192673
835	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-15 23:25:40.378803	2020-09-15 23:25:40.378803
836	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-15 23:25:40.401005	2020-09-15 23:25:40.401005
837	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-15 23:43:10.774052	2020-09-15 23:43:10.774052
838	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-15 23:43:10.794595	2020-09-15 23:43:10.794595
839	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-15 23:43:10.820175	2020-09-15 23:43:10.820175
840	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-15 23:43:10.828174	2020-09-15 23:43:10.828174
841	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-15 23:43:24.988699	2020-09-15 23:43:24.988699
842	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-15 23:43:25.003988	2020-09-15 23:43:25.003988
843	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-15 23:43:25.029236	2020-09-15 23:43:25.029236
844	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-15 23:43:25.03714	2020-09-15 23:43:25.03714
845	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-15 23:50:46.369154	2020-09-15 23:50:46.369154
846	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-15 23:50:46.389252	2020-09-15 23:50:46.389252
847	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-15 23:50:46.415139	2020-09-15 23:50:46.415139
848	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-15 23:50:46.422733	2020-09-15 23:50:46.422733
849	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-15 23:55:24.994039	2020-09-15 23:55:24.994039
850	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-15 23:55:25.012933	2020-09-15 23:55:25.012933
851	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-15 23:55:25.03835	2020-09-15 23:55:25.03835
852	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-15 23:55:25.046283	2020-09-15 23:55:25.046283
853	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-15 23:55:46.017041	2020-09-15 23:55:46.017041
854	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-15 23:55:46.039268	2020-09-15 23:55:46.039268
855	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-15 23:55:46.064756	2020-09-15 23:55:46.064756
856	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-15 23:55:46.072541	2020-09-15 23:55:46.072541
857	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-16 00:06:11.312303	2020-09-16 00:06:11.312303
858	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-16 00:06:11.375813	2020-09-16 00:06:11.375813
859	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-16 00:06:11.401142	2020-09-16 00:06:11.401142
860	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-16 00:06:11.409084	2020-09-16 00:06:11.409084
861	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-16 00:07:18.088573	2020-09-16 00:07:18.088573
862	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-16 00:07:18.958335	2020-09-16 00:07:18.958335
863	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-16 00:07:19.079864	2020-09-16 00:07:19.079864
864	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-16 00:07:19.08791	2020-09-16 00:07:19.08791
865	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-16 22:20:37.247523	2020-09-16 22:20:37.247523
866	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-16 22:20:37.538218	2020-09-16 22:20:37.538218
867	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-16 22:20:37.70696	2020-09-16 22:20:37.70696
868	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-16 22:20:37.729934	2020-09-16 22:20:37.729934
869	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-16 22:54:22.985897	2020-09-16 22:54:22.985897
870	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-16 22:54:23.031759	2020-09-16 22:54:23.031759
871	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-16 22:54:23.057491	2020-09-16 22:54:23.057491
872	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-16 22:54:23.065011	2020-09-16 22:54:23.065011
873	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-16 22:59:03.451539	2020-09-16 22:59:03.451539
874	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-16 22:59:03.634191	2020-09-16 22:59:03.634191
875	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-16 22:59:03.794405	2020-09-16 22:59:03.794405
876	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-16 22:59:03.817503	2020-09-16 22:59:03.817503
877	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-16 23:46:18.841627	2020-09-16 23:46:18.841627
878	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-16 23:46:19.107755	2020-09-16 23:46:19.107755
879	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-16 23:46:19.32592	2020-09-16 23:46:19.32592
880	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-16 23:46:19.374258	2020-09-16 23:46:19.374258
881	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-17 03:02:04.516447	2020-09-17 03:02:04.516447
882	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-17 03:02:04.81007	2020-09-17 03:02:04.81007
883	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-17 03:02:05.068068	2020-09-17 03:02:05.068068
884	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-17 03:02:05.116944	2020-09-17 03:02:05.116944
885	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-17 04:34:58.483589	2020-09-17 04:34:58.483589
886	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-17 04:34:58.499708	2020-09-17 04:34:58.499708
887	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-17 04:34:58.52534	2020-09-17 04:34:58.52534
888	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-17 04:34:58.532833	2020-09-17 04:34:58.532833
889	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-17 17:39:32.637841	2020-09-17 17:39:32.637841
890	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-17 17:39:32.654142	2020-09-17 17:39:32.654142
891	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-17 17:39:32.721441	2020-09-17 17:39:32.721441
892	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-17 17:39:32.728968	2020-09-17 17:39:32.728968
893	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-17 17:52:41.629637	2020-09-17 17:52:41.629637
894	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-17 17:52:41.639273	2020-09-17 17:52:41.639273
895	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-17 17:52:41.665225	2020-09-17 17:52:41.665225
896	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-17 17:52:41.672763	2020-09-17 17:52:41.672763
897	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-17 18:11:51.464318	2020-09-17 18:11:51.464318
898	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-17 18:11:51.878386	2020-09-17 18:11:51.878386
899	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-17 18:11:53.446139	2020-09-17 18:11:53.446139
900	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-17 18:11:53.703411	2020-09-17 18:11:53.703411
901	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-17 18:14:04.006481	2020-09-17 18:14:04.006481
902	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-17 18:14:04.460763	2020-09-17 18:14:04.460763
903	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-17 18:14:04.651898	2020-09-17 18:14:04.651898
904	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-17 18:14:04.744179	2020-09-17 18:14:04.744179
905	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-17 18:15:35.588396	2020-09-17 18:15:35.588396
906	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-17 18:15:36.736156	2020-09-17 18:15:36.736156
907	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-17 18:15:36.97609	2020-09-17 18:15:36.97609
908	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-17 18:15:36.983489	2020-09-17 18:15:36.983489
909	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-17 18:17:49.595162	2020-09-17 18:17:49.595162
910	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-17 18:17:49.615969	2020-09-17 18:17:49.615969
911	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-17 18:17:49.641903	2020-09-17 18:17:49.641903
912	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-17 18:17:49.649153	2020-09-17 18:17:49.649153
913	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-17 18:18:51.967884	2020-09-17 18:18:51.967884
914	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-17 18:18:51.986042	2020-09-17 18:18:51.986042
915	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-17 18:18:52.01232	2020-09-17 18:18:52.01232
916	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-17 18:18:52.019329	2020-09-17 18:18:52.019329
917	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-17 18:20:51.301937	2020-09-17 18:20:51.301937
918	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-17 18:20:51.317619	2020-09-17 18:20:51.317619
919	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-17 18:20:51.343494	2020-09-17 18:20:51.343494
920	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-17 18:20:51.351053	2020-09-17 18:20:51.351053
921	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-17 18:22:05.823122	2020-09-17 18:22:05.823122
922	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-17 18:22:05.83857	2020-09-17 18:22:05.83857
923	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-17 18:22:05.864186	2020-09-17 18:22:05.864186
924	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-17 18:22:05.87193	2020-09-17 18:22:05.87193
925	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-17 18:22:49.323514	2020-09-17 18:22:49.323514
926	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-17 18:22:49.341329	2020-09-17 18:22:49.341329
927	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-17 18:22:49.367115	2020-09-17 18:22:49.367115
928	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-17 18:22:49.374448	2020-09-17 18:22:49.374448
929	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-17 18:23:21.037311	2020-09-17 18:23:21.037311
930	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-17 18:23:21.05941	2020-09-17 18:23:21.05941
931	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-17 18:23:21.085297	2020-09-17 18:23:21.085297
932	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-17 18:23:21.092632	2020-09-17 18:23:21.092632
933	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-17 18:30:25.826212	2020-09-17 18:30:25.826212
934	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-17 18:30:25.849787	2020-09-17 18:30:25.849787
935	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-17 18:30:25.875472	2020-09-17 18:30:25.875472
936	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-17 18:30:25.882892	2020-09-17 18:30:25.882892
937	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-20 05:11:10.507311	2020-09-20 05:11:10.507311
938	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-20 05:11:10.526365	2020-09-20 05:11:10.526365
939	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-20 05:11:10.552351	2020-09-20 05:11:10.552351
940	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-20 05:11:10.559522	2020-09-20 05:11:10.559522
941	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-20 05:17:08.025663	2020-09-20 05:17:08.025663
942	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-20 05:17:08.779694	2020-09-20 05:17:08.779694
943	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-20 05:17:09.097318	2020-09-20 05:17:09.097318
944	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-20 05:17:09.10452	2020-09-20 05:17:09.10452
945	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-20 20:49:30.320235	2020-09-20 20:49:30.320235
946	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-20 20:49:30.340015	2020-09-20 20:49:30.340015
947	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-20 20:49:30.366171	2020-09-20 20:49:30.366171
948	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-20 20:49:30.373217	2020-09-20 20:49:30.373217
949	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-24 18:11:47.435623	2020-09-24 18:11:47.435623
950	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-24 18:11:47.979238	2020-09-24 18:11:47.979238
951	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-24 18:11:48.303932	2020-09-24 18:11:48.303932
952	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-24 18:11:48.34737	2020-09-24 18:11:48.34737
953	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-24 18:18:16.240617	2020-09-24 18:18:16.240617
954	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-24 18:18:16.253905	2020-09-24 18:18:16.253905
955	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-24 18:18:16.312714	2020-09-24 18:18:16.312714
956	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-24 18:18:16.320124	2020-09-24 18:18:16.320124
957	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-24 18:22:15.887782	2020-09-24 18:22:15.887782
958	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-24 18:22:15.901403	2020-09-24 18:22:15.901403
959	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-24 18:22:15.943787	2020-09-24 18:22:15.943787
960	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-24 18:22:15.951096	2020-09-24 18:22:15.951096
961	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-24 18:23:32.528194	2020-09-24 18:23:32.528194
962	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-24 18:23:32.547739	2020-09-24 18:23:32.547739
963	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-24 18:23:32.598127	2020-09-24 18:23:32.598127
964	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-24 18:23:32.605562	2020-09-24 18:23:32.605562
965	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-24 18:30:04.872842	2020-09-24 18:30:04.872842
966	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-24 18:30:04.904099	2020-09-24 18:30:04.904099
967	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-24 18:30:04.946464	2020-09-24 18:30:04.946464
968	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-24 18:30:04.978801	2020-09-24 18:30:04.978801
969	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-25 18:25:27.431087	2020-09-25 18:25:27.431087
970	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-25 18:25:27.48362	2020-09-25 18:25:27.48362
971	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-25 18:25:27.548281	2020-09-25 18:25:27.548281
972	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-25 18:25:27.556987	2020-09-25 18:25:27.556987
973	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-25 18:27:37.342407	2020-09-25 18:27:37.342407
974	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-25 18:27:37.357038	2020-09-25 18:27:37.357038
975	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-25 18:27:37.424491	2020-09-25 18:27:37.424491
976	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-25 18:27:37.431805	2020-09-25 18:27:37.431805
977	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-25 18:29:12.953559	2020-09-25 18:29:12.953559
978	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-25 18:29:13.023944	2020-09-25 18:29:13.023944
979	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-25 18:29:13.080513	2020-09-25 18:29:13.080513
980	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-25 18:29:13.129611	2020-09-25 18:29:13.129611
981	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-25 18:30:59.285004	2020-09-25 18:30:59.285004
982	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-25 18:30:59.311273	2020-09-25 18:30:59.311273
983	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-25 18:30:59.334711	2020-09-25 18:30:59.334711
984	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-25 18:30:59.342882	2020-09-25 18:30:59.342882
985	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-25 18:32:20.38909	2020-09-25 18:32:20.38909
986	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-25 18:32:20.432425	2020-09-25 18:32:20.432425
987	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-25 18:32:20.458114	2020-09-25 18:32:20.458114
988	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-25 18:32:20.465252	2020-09-25 18:32:20.465252
989	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-25 18:37:40.122891	2020-09-25 18:37:40.122891
990	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-25 18:37:40.134501	2020-09-25 18:37:40.134501
991	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-25 18:37:40.193437	2020-09-25 18:37:40.193437
992	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-25 18:37:40.200776	2020-09-25 18:37:40.200776
993	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-25 18:41:02.028819	2020-09-25 18:41:02.028819
994	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-25 18:41:02.063409	2020-09-25 18:41:02.063409
995	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-25 18:41:02.087318	2020-09-25 18:41:02.087318
996	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-25 18:41:02.095178	2020-09-25 18:41:02.095178
997	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-25 18:44:19.851442	2020-09-25 18:44:19.851442
998	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-25 18:44:19.86653	2020-09-25 18:44:19.86653
999	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-25 18:44:19.892649	2020-09-25 18:44:19.892649
1000	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-25 18:44:19.899649	2020-09-25 18:44:19.899649
1001	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-25 18:51:36.223093	2020-09-25 18:51:36.223093
1002	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-25 18:51:36.234057	2020-09-25 18:51:36.234057
1003	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-25 18:51:36.260135	2020-09-25 18:51:36.260135
1004	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-25 18:51:36.267131	2020-09-25 18:51:36.267131
1005	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-25 18:56:58.013438	2020-09-25 18:56:58.013438
1006	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-25 18:56:58.061698	2020-09-25 18:56:58.061698
1007	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-25 18:56:58.087329	2020-09-25 18:56:58.087329
1008	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-25 18:56:58.09457	2020-09-25 18:56:58.09457
1009	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-25 19:03:37.331717	2020-09-25 19:03:37.331717
1010	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-25 19:03:37.377092	2020-09-25 19:03:37.377092
1011	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-25 19:03:37.411053	2020-09-25 19:03:37.411053
1012	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-25 19:03:37.418315	2020-09-25 19:03:37.418315
1013	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-25 19:06:29.659271	2020-09-25 19:06:29.659271
1014	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-25 19:06:29.677695	2020-09-25 19:06:29.677695
1015	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-25 19:06:29.70319	2020-09-25 19:06:29.70319
1016	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-25 19:06:29.710922	2020-09-25 19:06:29.710922
1017	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-25 19:07:37.067336	2020-09-25 19:07:37.067336
1018	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-25 19:07:37.082869	2020-09-25 19:07:37.082869
1019	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-25 19:07:37.109129	2020-09-25 19:07:37.109129
1020	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-25 19:07:37.11586	2020-09-25 19:07:37.11586
1021	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-25 19:10:53.948859	2020-09-25 19:10:53.948859
1022	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-25 19:10:53.993358	2020-09-25 19:10:53.993358
1023	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-25 19:10:54.018475	2020-09-25 19:10:54.018475
1024	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-25 19:10:54.026472	2020-09-25 19:10:54.026472
1025	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-25 19:12:12.303538	2020-09-25 19:12:12.303538
1026	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-25 19:12:12.314843	2020-09-25 19:12:12.314843
1027	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-25 19:12:12.340391	2020-09-25 19:12:12.340391
1028	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-25 19:12:12.348278	2020-09-25 19:12:12.348278
1029	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-25 19:14:19.176118	2020-09-25 19:14:19.176118
1030	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-25 19:14:19.207117	2020-09-25 19:14:19.207117
1031	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-25 19:14:19.232542	2020-09-25 19:14:19.232542
1032	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-25 19:14:19.240015	2020-09-25 19:14:19.240015
1033	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-25 19:16:51.056001	2020-09-25 19:16:51.056001
1034	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-25 19:16:51.082632	2020-09-25 19:16:51.082632
1035	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-25 19:16:51.108193	2020-09-25 19:16:51.108193
1036	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-25 19:16:51.116036	2020-09-25 19:16:51.116036
1037	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-25 19:20:06.968607	2020-09-25 19:20:06.968607
1038	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-25 19:20:07.011055	2020-09-25 19:20:07.011055
1039	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-25 19:20:07.036783	2020-09-25 19:20:07.036783
1040	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-25 19:20:07.043827	2020-09-25 19:20:07.043827
1041	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-25 21:51:11.814874	2020-09-25 21:51:11.814874
1042	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-25 21:51:11.847375	2020-09-25 21:51:11.847375
1043	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-25 21:51:11.890065	2020-09-25 21:51:11.890065
1044	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-25 21:51:11.897528	2020-09-25 21:51:11.897528
1045	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-25 21:58:33.12271	2020-09-25 21:58:33.12271
1046	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-25 21:58:33.140174	2020-09-25 21:58:33.140174
1047	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-25 21:58:33.182361	2020-09-25 21:58:33.182361
1048	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-25 21:58:33.189829	2020-09-25 21:58:33.189829
1049	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-25 21:59:50.234681	2020-09-25 21:59:50.234681
1050	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-25 21:59:50.253136	2020-09-25 21:59:50.253136
1051	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-25 21:59:50.279174	2020-09-25 21:59:50.279174
1052	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-25 21:59:50.286317	2020-09-25 21:59:50.286317
1053	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-26 04:33:08.193882	2020-09-26 04:33:08.193882
1054	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-26 04:33:08.229499	2020-09-26 04:33:08.229499
1055	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-26 04:33:09.264069	2020-09-26 04:33:09.264069
1056	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-26 04:33:09.454572	2020-09-26 04:33:09.454572
1057	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-26 04:34:59.300687	2020-09-26 04:34:59.300687
1058	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-26 04:34:59.352853	2020-09-26 04:34:59.352853
1059	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-26 04:34:59.378536	2020-09-26 04:34:59.378536
1060	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-26 04:34:59.386095	2020-09-26 04:34:59.386095
1061	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-26 04:49:59.628604	2020-09-26 04:49:59.628604
1062	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-26 04:49:59.638738	2020-09-26 04:49:59.638738
1063	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-26 04:49:59.68162	2020-09-26 04:49:59.68162
1064	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-26 04:49:59.688775	2020-09-26 04:49:59.688775
1065	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-26 04:51:20.164532	2020-09-26 04:51:20.164532
1066	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-26 04:51:20.185368	2020-09-26 04:51:20.185368
1067	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-26 04:51:20.211474	2020-09-26 04:51:20.211474
1068	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-26 04:51:20.218614	2020-09-26 04:51:20.218614
1069	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-26 21:08:57.138906	2020-09-26 21:08:57.138906
1070	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-26 21:08:57.371839	2020-09-26 21:08:57.371839
1071	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-26 21:08:57.534941	2020-09-26 21:08:57.534941
1072	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-26 21:08:57.563283	2020-09-26 21:08:57.563283
1073	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-27 03:30:16.530764	2020-09-27 03:30:16.530764
1074	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-27 03:30:16.981601	2020-09-27 03:30:16.981601
1075	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-27 03:30:17.54087	2020-09-27 03:30:17.54087
1076	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-27 03:30:17.654399	2020-09-27 03:30:17.654399
1077	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-27 20:59:45.989707	2020-09-27 20:59:45.989707
1078	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-27 20:59:46.012803	2020-09-27 20:59:46.012803
1079	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-27 20:59:46.038715	2020-09-27 20:59:46.038715
1080	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-27 20:59:46.04591	2020-09-27 20:59:46.04591
1081	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-27 21:00:35.357835	2020-09-27 21:00:35.357835
1082	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-27 21:00:35.374329	2020-09-27 21:00:35.374329
1083	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-27 21:00:35.400077	2020-09-27 21:00:35.400077
1084	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-27 21:00:35.407409	2020-09-27 21:00:35.407409
1085	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-27 21:08:40.554873	2020-09-27 21:08:40.554873
1086	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-27 21:08:41.16253	2020-09-27 21:08:41.16253
1087	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-27 21:08:41.38118	2020-09-27 21:08:41.38118
1088	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-27 21:08:41.658209	2020-09-27 21:08:41.658209
1089	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-27 21:27:50.687344	2020-09-27 21:27:50.687344
1090	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-27 21:27:50.707597	2020-09-27 21:27:50.707597
1091	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-27 21:27:50.733841	2020-09-27 21:27:50.733841
1092	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-27 21:27:50.740679	2020-09-27 21:27:50.740679
1093	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-28 02:23:28.131889	2020-09-28 02:23:28.131889
1094	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 02:23:28.163946	2020-09-28 02:23:28.163946
1095	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-28 02:23:28.189694	2020-09-28 02:23:28.189694
1096	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 02:23:28.196768	2020-09-28 02:23:28.196768
1097	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-28 02:29:08.689349	2020-09-28 02:29:08.689349
1098	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 02:29:08.701249	2020-09-28 02:29:08.701249
1099	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-28 02:29:08.72706	2020-09-28 02:29:08.72706
1100	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 02:29:08.734371	2020-09-28 02:29:08.734371
1101	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-28 02:44:01.383116	2020-09-28 02:44:01.383116
1102	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 02:44:01.406207	2020-09-28 02:44:01.406207
1103	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-28 02:44:01.432207	2020-09-28 02:44:01.432207
1104	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 02:44:01.439706	2020-09-28 02:44:01.439706
1105	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-28 02:56:27.683887	2020-09-28 02:56:27.683887
1106	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 02:56:27.69292	2020-09-28 02:56:27.69292
1107	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-28 02:56:27.717211	2020-09-28 02:56:27.717211
1108	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 02:56:27.72632	2020-09-28 02:56:27.72632
1109	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-28 03:26:53.530207	2020-09-28 03:26:53.530207
1110	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 03:26:53.554484	2020-09-28 03:26:53.554484
1111	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-28 03:26:53.580329	2020-09-28 03:26:53.580329
1112	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 03:26:53.586578	2020-09-28 03:26:53.586578
1113	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-28 03:31:01.067393	2020-09-28 03:31:01.067393
1114	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 03:31:01.077966	2020-09-28 03:31:01.077966
1115	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-28 03:31:01.102407	2020-09-28 03:31:01.102407
1116	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 03:31:01.111262	2020-09-28 03:31:01.111262
1117	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-28 03:32:40.907071	2020-09-28 03:32:40.907071
1118	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 03:32:40.917515	2020-09-28 03:32:40.917515
1119	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-28 03:32:40.941731	2020-09-28 03:32:40.941731
1120	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 03:32:40.949631	2020-09-28 03:32:40.949631
1121	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-28 18:20:38.365731	2020-09-28 18:20:38.365731
1122	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 18:20:38.394205	2020-09-28 18:20:38.394205
1123	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-28 18:20:38.420115	2020-09-28 18:20:38.420115
1124	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 18:20:38.42733	2020-09-28 18:20:38.42733
1125	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-28 18:23:38.726631	2020-09-28 18:23:38.726631
1126	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 18:23:38.755497	2020-09-28 18:23:38.755497
1127	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-28 18:23:38.781538	2020-09-28 18:23:38.781538
1128	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 18:23:38.788377	2020-09-28 18:23:38.788377
1129	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-28 18:28:38.314585	2020-09-28 18:28:38.314585
1130	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 18:28:39.561057	2020-09-28 18:28:39.561057
1131	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-28 18:28:39.758001	2020-09-28 18:28:39.758001
1132	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 18:28:39.93178	2020-09-28 18:28:39.93178
1133	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-28 18:32:26.442149	2020-09-28 18:32:26.442149
1134	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 18:32:27.846533	2020-09-28 18:32:27.846533
1135	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-28 18:32:28.409352	2020-09-28 18:32:28.409352
1136	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 18:32:28.470746	2020-09-28 18:32:28.470746
1137	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-28 18:44:32.835099	2020-09-28 18:44:32.835099
1138	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 18:44:32.948422	2020-09-28 18:44:32.948422
1139	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-28 18:44:33.216264	2020-09-28 18:44:33.216264
1140	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 18:44:33.348484	2020-09-28 18:44:33.348484
1141	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-28 18:45:54.525276	2020-09-28 18:45:54.525276
1142	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 18:45:55.654389	2020-09-28 18:45:55.654389
1143	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-28 18:45:56.254379	2020-09-28 18:45:56.254379
1144	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 18:45:56.328796	2020-09-28 18:45:56.328796
1145	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-28 18:48:57.298188	2020-09-28 18:48:57.298188
1146	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 18:48:57.322835	2020-09-28 18:48:57.322835
1147	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-28 18:48:57.348714	2020-09-28 18:48:57.348714
1148	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 18:48:57.355855	2020-09-28 18:48:57.355855
1149	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-28 19:03:34.958064	2020-09-28 19:03:34.958064
1150	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 19:03:34.975838	2020-09-28 19:03:34.975838
1151	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-28 19:03:35.00228	2020-09-28 19:03:35.00228
1152	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 19:03:35.009337	2020-09-28 19:03:35.009337
1153	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-28 19:09:57.258261	2020-09-28 19:09:57.258261
1154	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 19:09:57.274402	2020-09-28 19:09:57.274402
1155	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-28 19:09:57.300419	2020-09-28 19:09:57.300419
1156	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 19:09:57.30775	2020-09-28 19:09:57.30775
1157	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-28 19:12:38.542247	2020-09-28 19:12:38.542247
1158	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 19:12:38.559454	2020-09-28 19:12:38.559454
1159	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-28 19:12:38.584852	2020-09-28 19:12:38.584852
1160	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 19:12:38.59255	2020-09-28 19:12:38.59255
1161	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-28 19:13:41.962919	2020-09-28 19:13:41.962919
1162	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 19:13:42.608529	2020-09-28 19:13:42.608529
1163	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-28 19:13:43.247817	2020-09-28 19:13:43.247817
1164	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 19:13:43.280086	2020-09-28 19:13:43.280086
1165	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-28 19:14:58.166717	2020-09-28 19:14:58.166717
1166	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 19:14:59.556496	2020-09-28 19:14:59.556496
1167	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-28 19:14:59.911354	2020-09-28 19:14:59.911354
1168	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 19:14:59.918697	2020-09-28 19:14:59.918697
1169	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-28 19:15:20.751493	2020-09-28 19:15:20.751493
1170	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 19:15:20.777779	2020-09-28 19:15:20.777779
1171	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-28 19:15:20.804051	2020-09-28 19:15:20.804051
1172	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 19:15:20.811038	2020-09-28 19:15:20.811038
1173	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-28 19:17:31.476205	2020-09-28 19:17:31.476205
1174	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 19:17:31.485245	2020-09-28 19:17:31.485245
1175	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-28 19:17:31.511317	2020-09-28 19:17:31.511317
1176	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 19:17:31.518641	2020-09-28 19:17:31.518641
1177	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-28 19:18:36.693508	2020-09-28 19:18:36.693508
1178	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 19:18:36.714625	2020-09-28 19:18:36.714625
1179	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-28 19:18:36.740854	2020-09-28 19:18:36.740854
1180	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 19:18:36.747845	2020-09-28 19:18:36.747845
1181	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-28 19:19:42.975536	2020-09-28 19:19:42.975536
1182	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 19:19:43.244449	2020-09-28 19:19:43.244449
1183	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-28 19:19:43.453451	2020-09-28 19:19:43.453451
1184	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 19:19:43.560687	2020-09-28 19:19:43.560687
1185	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-28 19:20:57.184366	2020-09-28 19:20:57.184366
1186	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 19:20:57.207036	2020-09-28 19:20:57.207036
1187	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-28 19:20:57.232597	2020-09-28 19:20:57.232597
1188	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 19:20:57.240358	2020-09-28 19:20:57.240358
1189	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-28 19:45:20.520473	2020-09-28 19:45:20.520473
1190	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 19:45:20.545948	2020-09-28 19:45:20.545948
1191	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-28 19:45:20.57997	2020-09-28 19:45:20.57997
1192	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 19:45:20.587375	2020-09-28 19:45:20.587375
1193	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-28 19:46:48.299726	2020-09-28 19:46:48.299726
1194	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 19:46:48.309234	2020-09-28 19:46:48.309234
1195	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-28 19:46:48.343475	2020-09-28 19:46:48.343475
1196	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 19:46:48.35089	2020-09-28 19:46:48.35089
1197	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-28 19:49:34.469907	2020-09-28 19:49:34.469907
1198	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 19:49:34.486611	2020-09-28 19:49:34.486611
1199	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-28 19:49:34.512627	2020-09-28 19:49:34.512627
1200	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 19:49:34.519802	2020-09-28 19:49:34.519802
1201	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-28 19:50:27.304266	2020-09-28 19:50:27.304266
1202	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 19:50:27.79044	2020-09-28 19:50:27.79044
1203	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-28 19:50:28.066501	2020-09-28 19:50:28.066501
1204	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 19:50:28.198754	2020-09-28 19:50:28.198754
1205	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-28 19:54:31.181035	2020-09-28 19:54:31.181035
1206	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 19:54:31.196672	2020-09-28 19:54:31.196672
1207	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-28 19:54:31.222343	2020-09-28 19:54:31.222343
1208	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 19:54:31.230059	2020-09-28 19:54:31.230059
1209	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-28 19:55:39.557136	2020-09-28 19:55:39.557136
1210	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 19:55:40.526701	2020-09-28 19:55:40.526701
1211	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-28 19:55:41.250848	2020-09-28 19:55:41.250848
1212	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 19:55:41.328432	2020-09-28 19:55:41.328432
1213	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-28 19:57:10.226998	2020-09-28 19:57:10.226998
1214	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 19:57:10.24839	2020-09-28 19:57:10.24839
1215	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-28 19:57:10.27431	2020-09-28 19:57:10.27431
1216	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 19:57:10.281581	2020-09-28 19:57:10.281581
1217	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-28 20:00:00.693433	2020-09-28 20:00:00.693433
1218	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 20:00:01.275827	2020-09-28 20:00:01.275827
1219	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-28 20:00:01.709458	2020-09-28 20:00:01.709458
1220	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 20:00:01.725526	2020-09-28 20:00:01.725526
1221	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-28 20:01:40.854143	2020-09-28 20:01:40.854143
1222	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 20:01:40.873124	2020-09-28 20:01:40.873124
1223	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-28 20:01:40.898836	2020-09-28 20:01:40.898836
1224	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 20:01:40.906359	2020-09-28 20:01:40.906359
1225	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-28 20:02:41.265563	2020-09-28 20:02:41.265563
1226	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 20:02:41.28518	2020-09-28 20:02:41.28518
1227	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-28 20:02:41.310832	2020-09-28 20:02:41.310832
1228	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 20:02:41.31833	2020-09-28 20:02:41.31833
1229	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-28 23:15:27.711104	2020-09-28 23:15:27.711104
1230	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 23:15:27.732313	2020-09-28 23:15:27.732313
1231	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-28 23:15:27.757992	2020-09-28 23:15:27.757992
1232	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 23:15:27.765541	2020-09-28 23:15:27.765541
1233	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-28 23:21:32.345568	2020-09-28 23:21:32.345568
1234	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 23:21:32.36323	2020-09-28 23:21:32.36323
1235	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-28 23:21:32.388942	2020-09-28 23:21:32.388942
1236	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 23:21:32.396439	2020-09-28 23:21:32.396439
1237	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-28 23:23:30.696375	2020-09-28 23:23:30.696375
1238	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 23:23:30.712336	2020-09-28 23:23:30.712336
1239	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-28 23:23:30.738018	2020-09-28 23:23:30.738018
1240	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 23:23:30.745467	2020-09-28 23:23:30.745467
1241	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-28 23:45:34.17307	2020-09-28 23:45:34.17307
1242	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 23:45:34.709952	2020-09-28 23:45:34.709952
1243	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-28 23:45:34.961148	2020-09-28 23:45:34.961148
1244	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 23:45:35.101755	2020-09-28 23:45:35.101755
1245	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-28 23:57:21.124165	2020-09-28 23:57:21.124165
1246	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 23:57:21.144182	2020-09-28 23:57:21.144182
1247	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-28 23:57:21.170029	2020-09-28 23:57:21.170029
1248	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-28 23:57:21.177552	2020-09-28 23:57:21.177552
1249	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-29 00:09:27.20632	2020-09-29 00:09:27.20632
1250	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-29 00:09:27.222021	2020-09-29 00:09:27.222021
1251	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-29 00:09:27.256412	2020-09-29 00:09:27.256412
1252	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-29 00:09:27.263463	2020-09-29 00:09:27.263463
1253	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-29 00:11:48.526582	2020-09-29 00:11:48.526582
1254	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-29 00:11:48.547183	2020-09-29 00:11:48.547183
1255	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-29 00:11:48.631797	2020-09-29 00:11:48.631797
1256	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-29 00:11:48.638894	2020-09-29 00:11:48.638894
1257	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-29 00:13:54.359964	2020-09-29 00:13:54.359964
1258	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-29 00:13:54.37996	2020-09-29 00:13:54.37996
1259	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-29 00:13:54.405631	2020-09-29 00:13:54.405631
1260	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-29 00:13:54.413289	2020-09-29 00:13:54.413289
1261	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-29 00:14:51.916293	2020-09-29 00:14:51.916293
1262	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-29 00:14:51.933608	2020-09-29 00:14:51.933608
1263	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-29 00:14:51.959278	2020-09-29 00:14:51.959278
1264	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-29 00:14:51.966965	2020-09-29 00:14:51.966965
1265	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-29 00:17:58.80527	2020-09-29 00:17:58.80527
1266	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-29 00:17:59.929114	2020-09-29 00:17:59.929114
1267	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-29 00:18:00.663498	2020-09-29 00:18:00.663498
1268	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-29 00:18:00.745474	2020-09-29 00:18:00.745474
1269	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-29 00:20:30.068217	2020-09-29 00:20:30.068217
1270	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-29 00:20:30.087737	2020-09-29 00:20:30.087737
1271	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-29 00:20:30.113524	2020-09-29 00:20:30.113524
1272	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-29 00:20:30.120798	2020-09-29 00:20:30.120798
1273	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-29 00:22:56.044788	2020-09-29 00:22:56.044788
1274	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-29 00:22:56.822371	2020-09-29 00:22:56.822371
1275	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-29 00:22:57.091997	2020-09-29 00:22:57.091997
1276	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-29 00:22:57.180266	2020-09-29 00:22:57.180266
1277	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-29 00:24:04.435669	2020-09-29 00:24:04.435669
1278	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-29 00:24:04.667679	2020-09-29 00:24:04.667679
1279	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-29 00:24:04.727167	2020-09-29 00:24:04.727167
1280	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-29 00:24:04.734068	2020-09-29 00:24:04.734068
1281	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-29 00:26:52.239428	2020-09-29 00:26:52.239428
1282	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-29 00:26:52.244222	2020-09-29 00:26:52.244222
1283	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-29 00:26:52.269999	2020-09-29 00:26:52.269999
1284	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-29 00:26:52.277551	2020-09-29 00:26:52.277551
1285	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-29 00:28:32.769687	2020-09-29 00:28:32.769687
1286	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-29 00:28:32.926171	2020-09-29 00:28:32.926171
1287	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-29 00:28:33.248683	2020-09-29 00:28:33.248683
1288	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-29 00:28:33.315073	2020-09-29 00:28:33.315073
1289	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-29 00:30:06.399792	2020-09-29 00:30:06.399792
1290	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-29 00:30:06.631794	2020-09-29 00:30:06.631794
1291	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-29 00:30:06.907466	2020-09-29 00:30:06.907466
1292	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-29 00:30:07.04884	2020-09-29 00:30:07.04884
1293	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-29 00:31:17.124076	2020-09-29 00:31:17.124076
1294	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-29 00:31:17.152493	2020-09-29 00:31:17.152493
1295	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-29 00:31:17.17791	2020-09-29 00:31:17.17791
1296	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-29 00:31:17.185596	2020-09-29 00:31:17.185596
1297	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-29 00:32:43.473847	2020-09-29 00:32:43.473847
1298	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-29 00:32:43.49091	2020-09-29 00:32:43.49091
1299	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-29 00:32:43.516805	2020-09-29 00:32:43.516805
1300	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-29 00:32:43.523951	2020-09-29 00:32:43.523951
1301	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-29 00:33:30.350808	2020-09-29 00:33:30.350808
1302	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-29 00:33:30.368802	2020-09-29 00:33:30.368802
1303	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-29 00:33:30.394821	2020-09-29 00:33:30.394821
1304	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-29 00:33:30.401957	2020-09-29 00:33:30.401957
1305	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-29 00:34:07.444096	2020-09-29 00:34:07.444096
1306	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-29 00:34:07.454467	2020-09-29 00:34:07.454467
1307	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-29 00:34:07.480281	2020-09-29 00:34:07.480281
1308	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-29 00:34:07.496298	2020-09-29 00:34:07.496298
1309	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-29 00:34:53.472907	2020-09-29 00:34:53.472907
1310	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-29 00:34:53.482347	2020-09-29 00:34:53.482347
1311	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-29 00:34:53.516481	2020-09-29 00:34:53.516481
1312	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-29 00:34:53.523795	2020-09-29 00:34:53.523795
1313	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-29 00:35:25.418364	2020-09-29 00:35:25.418364
1314	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-29 00:35:26.784873	2020-09-29 00:35:26.784873
1315	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-29 00:35:27.385734	2020-09-29 00:35:27.385734
1316	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-29 00:35:27.459626	2020-09-29 00:35:27.459626
1317	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-29 00:46:03.03474	2020-09-29 00:46:03.03474
1318	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-29 00:46:03.044253	2020-09-29 00:46:03.044253
1319	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-29 00:46:03.071178	2020-09-29 00:46:03.071178
1320	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-29 00:46:03.078584	2020-09-29 00:46:03.078584
1321	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-29 00:52:25.115987	2020-09-29 00:52:25.115987
1322	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-29 00:52:25.127311	2020-09-29 00:52:25.127311
1323	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-29 00:52:25.153007	2020-09-29 00:52:25.153007
1324	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-29 00:52:25.160564	2020-09-29 00:52:25.160564
1325	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-29 01:04:16.616492	2020-09-29 01:04:16.616492
1326	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-29 01:04:16.63735	2020-09-29 01:04:16.63735
1327	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-29 01:04:16.663533	2020-09-29 01:04:16.663533
1328	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-29 01:04:16.67102	2020-09-29 01:04:16.67102
1329	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-29 01:08:23.722035	2020-09-29 01:08:23.722035
1330	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-29 01:08:23.752504	2020-09-29 01:08:23.752504
1331	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-29 01:08:23.778199	2020-09-29 01:08:23.778199
1332	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-29 01:08:23.785634	2020-09-29 01:08:23.785634
1333	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-29 01:13:12.631221	2020-09-29 01:13:12.631221
1334	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-29 01:13:12.653319	2020-09-29 01:13:12.653319
1335	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-29 01:13:12.679645	2020-09-29 01:13:12.679645
1336	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-29 01:13:12.686803	2020-09-29 01:13:12.686803
1337	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-29 01:24:35.353589	2020-09-29 01:24:35.353589
1338	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-29 01:24:35.362765	2020-09-29 01:24:35.362765
1339	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-29 01:24:35.387867	2020-09-29 01:24:35.387867
1340	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-29 01:24:35.395112	2020-09-29 01:24:35.395112
1341	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-29 01:27:42.74724	2020-09-29 01:27:42.74724
1342	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-29 01:27:42.763901	2020-09-29 01:27:42.763901
1343	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-29 01:27:42.788738	2020-09-29 01:27:42.788738
1344	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-29 01:27:42.796528	2020-09-29 01:27:42.796528
1345	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-29 01:34:33.65799	2020-09-29 01:34:33.65799
1346	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-29 01:34:33.672911	2020-09-29 01:34:33.672911
1347	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-29 01:34:33.765778	2020-09-29 01:34:33.765778
1348	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-29 01:34:33.772682	2020-09-29 01:34:33.772682
1349	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-29 01:37:43.931698	2020-09-29 01:37:43.931698
1350	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-29 01:37:43.95051	2020-09-29 01:37:43.95051
1351	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-29 01:37:43.975961	2020-09-29 01:37:43.975961
1352	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-29 01:37:43.983705	2020-09-29 01:37:43.983705
1353	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-29 01:39:58.840293	2020-09-29 01:39:58.840293
1354	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-29 01:39:58.851205	2020-09-29 01:39:58.851205
1355	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-29 01:39:58.877194	2020-09-29 01:39:58.877194
1356	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-29 01:39:58.884228	2020-09-29 01:39:58.884228
1357	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-29 02:04:55.867988	2020-09-29 02:04:55.867988
1358	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-29 02:04:55.889872	2020-09-29 02:04:55.889872
1359	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-29 02:04:55.915728	2020-09-29 02:04:55.915728
1360	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-29 02:04:55.923068	2020-09-29 02:04:55.923068
1361	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-29 02:12:03.437787	2020-09-29 02:12:03.437787
1362	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-29 02:12:03.456639	2020-09-29 02:12:03.456639
1363	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-29 02:12:03.483489	2020-09-29 02:12:03.483489
1364	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-29 02:12:03.491023	2020-09-29 02:12:03.491023
1365	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-29 02:28:14.904927	2020-09-29 02:28:14.904927
1366	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-29 02:28:14.925515	2020-09-29 02:28:14.925515
1367	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-29 02:28:14.951196	2020-09-29 02:28:14.951196
1368	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-29 02:28:14.958442	2020-09-29 02:28:14.958442
1369	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-29 03:50:17.332309	2020-09-29 03:50:17.332309
1370	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-29 03:50:17.349896	2020-09-29 03:50:17.349896
1371	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-29 03:50:17.384219	2020-09-29 03:50:17.384219
1372	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-29 03:50:17.391406	2020-09-29 03:50:17.391406
1373	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-30 15:13:21.902965	2020-09-30 15:13:21.902965
1374	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-30 15:13:22.143058	2020-09-30 15:13:22.143058
1375	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-30 15:13:22.375971	2020-09-30 15:13:22.375971
1376	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-30 15:13:22.39781	2020-09-30 15:13:22.39781
1377	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-09-30 19:29:58.019905	2020-09-30 19:29:58.019905
1378	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-30 19:29:58.027688	2020-09-30 19:29:58.027688
1379	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-09-30 19:29:58.053658	2020-09-30 19:29:58.053658
1380	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-09-30 19:29:58.061196	2020-09-30 19:29:58.061196
1381	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-01 18:40:10.883698	2020-10-01 18:40:10.883698
1382	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-01 18:40:11.062313	2020-10-01 18:40:11.062313
1383	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-01 18:40:11.654783	2020-10-01 18:40:11.654783
1384	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-01 18:40:11.803767	2020-10-01 18:40:11.803767
1385	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-01 18:46:39.662632	2020-10-01 18:46:39.662632
1386	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-01 18:46:39.993428	2020-10-01 18:46:39.993428
1387	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-01 18:46:41.294555	2020-10-01 18:46:41.294555
1388	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-01 18:46:41.360462	2020-10-01 18:46:41.360462
1389	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-01 18:56:24.631751	2020-10-01 18:56:24.631751
1390	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-01 18:56:24.653074	2020-10-01 18:56:24.653074
1391	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-01 18:56:24.67898	2020-10-01 18:56:24.67898
1392	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-01 18:56:24.686046	2020-10-01 18:56:24.686046
1393	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-02 18:50:50.524758	2020-10-02 18:50:50.524758
1394	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-02 18:50:50.540349	2020-10-02 18:50:50.540349
1395	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-02 18:50:50.566486	2020-10-02 18:50:50.566486
1396	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-02 18:50:50.573401	2020-10-02 18:50:50.573401
1397	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-02 19:02:11.267811	2020-10-02 19:02:11.267811
1398	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-02 19:02:11.288711	2020-10-02 19:02:11.288711
1399	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-02 19:02:11.314823	2020-10-02 19:02:11.314823
1400	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-02 19:02:11.321836	2020-10-02 19:02:11.321836
1401	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-02 23:40:57.693473	2020-10-02 23:40:57.693473
1402	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-02 23:40:57.718758	2020-10-02 23:40:57.718758
1403	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-02 23:40:57.786259	2020-10-02 23:40:57.786259
1404	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-02 23:40:57.793496	2020-10-02 23:40:57.793496
1405	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-02 23:59:23.349218	2020-10-02 23:59:23.349218
1406	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-02 23:59:23.37611	2020-10-02 23:59:23.37611
1407	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-02 23:59:23.402366	2020-10-02 23:59:23.402366
1408	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-02 23:59:23.409046	2020-10-02 23:59:23.409046
1409	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-03 00:04:48.454781	2020-10-03 00:04:48.454781
1410	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 00:04:48.478692	2020-10-03 00:04:48.478692
1411	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-03 00:04:48.504327	2020-10-03 00:04:48.504327
1412	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 00:04:48.511862	2020-10-03 00:04:48.511862
1413	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-03 00:59:06.599911	2020-10-03 00:59:06.599911
1414	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 00:59:06.620835	2020-10-03 00:59:06.620835
1415	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-03 00:59:06.646318	2020-10-03 00:59:06.646318
1416	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 00:59:06.654172	2020-10-03 00:59:06.654172
1417	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-03 02:34:57.618311	2020-10-03 02:34:57.618311
1418	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 02:34:57.64319	2020-10-03 02:34:57.64319
1419	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-03 02:34:57.668635	2020-10-03 02:34:57.668635
1420	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 02:34:57.676499	2020-10-03 02:34:57.676499
1421	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-03 03:08:31.241114	2020-10-03 03:08:31.241114
1422	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 03:08:31.2708	2020-10-03 03:08:31.2708
1423	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-03 03:08:31.29733	2020-10-03 03:08:31.29733
1424	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 03:08:31.303967	2020-10-03 03:08:31.303967
1425	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-03 03:19:45.472991	2020-10-03 03:19:45.472991
1426	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 03:19:45.487731	2020-10-03 03:19:45.487731
1427	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-03 03:19:45.513369	2020-10-03 03:19:45.513369
1428	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 03:19:45.521092	2020-10-03 03:19:45.521092
1429	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-03 05:56:28.163768	2020-10-03 05:56:28.163768
1430	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 05:56:28.329268	2020-10-03 05:56:28.329268
1431	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-03 05:56:28.578439	2020-10-03 05:56:28.578439
1432	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 05:56:28.603839	2020-10-03 05:56:28.603839
1433	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	8	10	1818	500	SNIPER	6	Heckler and Koch	L96 Arctic Warfare	24	COMMON	l96aw.yml	2020-10-03 05:56:45.411464	2020-10-03 05:56:45.411464
1434	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 05:56:45.438459	2020-10-03 05:56:45.438459
1435	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-03 07:49:17.945444	2020-10-03 07:49:17.945444
1436	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 07:49:17.962922	2020-10-03 07:49:17.962922
1437	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-03 07:49:17.98873	2020-10-03 07:49:17.98873
1438	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 07:49:17.995708	2020-10-03 07:49:17.995708
1439	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-03 07:59:12.252936	2020-10-03 07:59:12.252936
1440	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 07:59:12.272694	2020-10-03 07:59:12.272694
1441	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-03 07:59:12.298657	2020-10-03 07:59:12.298657
1442	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 07:59:12.305881	2020-10-03 07:59:12.305881
1443	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-03 08:00:20.468708	2020-10-03 08:00:20.468708
1444	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 08:00:20.668364	2020-10-03 08:00:20.668364
1445	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-03 08:00:20.694527	2020-10-03 08:00:20.694527
1446	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 08:00:20.701784	2020-10-03 08:00:20.701784
1447	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-03 08:02:11.884296	2020-10-03 08:02:11.884296
1448	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 08:02:11.900215	2020-10-03 08:02:11.900215
1449	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-03 08:02:11.926196	2020-10-03 08:02:11.926196
1450	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 08:02:11.933251	2020-10-03 08:02:11.933251
1451	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-03 08:03:07.20315	2020-10-03 08:03:07.20315
1452	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 08:03:07.288667	2020-10-03 08:03:07.288667
1453	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-03 08:03:07.379206	2020-10-03 08:03:07.379206
1454	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 08:03:07.386654	2020-10-03 08:03:07.386654
1455	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-03 08:27:24.05567	2020-10-03 08:27:24.05567
1456	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 08:27:24.073098	2020-10-03 08:27:24.073098
1457	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-03 08:27:24.098825	2020-10-03 08:27:24.098825
1458	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 08:27:24.106298	2020-10-03 08:27:24.106298
1459	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-03 08:30:12.371647	2020-10-03 08:30:12.371647
1460	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 08:30:12.816528	2020-10-03 08:30:12.816528
1461	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-03 08:30:13.025969	2020-10-03 08:30:13.025969
1462	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 08:30:13.091642	2020-10-03 08:30:13.091642
1463	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-03 08:37:08.253994	2020-10-03 08:37:08.253994
1464	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 08:37:08.274509	2020-10-03 08:37:08.274509
1465	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-03 08:37:08.300727	2020-10-03 08:37:08.300727
1466	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 08:37:08.307545	2020-10-03 08:37:08.307545
1467	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-03 08:39:39.488815	2020-10-03 08:39:39.488815
1468	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 08:39:39.500205	2020-10-03 08:39:39.500205
1469	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-03 08:39:39.526044	2020-10-03 08:39:39.526044
1470	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 08:39:39.533458	2020-10-03 08:39:39.533458
1471	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-03 08:44:36.888309	2020-10-03 08:44:36.888309
1472	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 08:44:36.909436	2020-10-03 08:44:36.909436
1473	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-03 08:44:36.935523	2020-10-03 08:44:36.935523
1474	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 08:44:36.942735	2020-10-03 08:44:36.942735
1475	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-03 08:46:05.937278	2020-10-03 08:46:05.937278
1476	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 08:46:05.981134	2020-10-03 08:46:05.981134
1477	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-03 08:46:06.016015	2020-10-03 08:46:06.016015
1478	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 08:46:06.023048	2020-10-03 08:46:06.023048
1479	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-03 08:46:50.449679	2020-10-03 08:46:50.449679
1480	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 08:46:50.592818	2020-10-03 08:46:50.592818
1481	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-03 08:46:50.876834	2020-10-03 08:46:50.876834
1482	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 08:46:51.059301	2020-10-03 08:46:51.059301
1483	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-03 08:49:25.0387	2020-10-03 08:49:25.0387
1484	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 08:49:25.060268	2020-10-03 08:49:25.060268
1485	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-03 08:49:25.08621	2020-10-03 08:49:25.08621
1486	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 08:49:25.093329	2020-10-03 08:49:25.093329
1487	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-03 08:56:58.610657	2020-10-03 08:56:58.610657
1488	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 08:56:58.93737	2020-10-03 08:56:58.93737
1489	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-03 08:56:59.296771	2020-10-03 08:56:59.296771
1490	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 08:56:59.390575	2020-10-03 08:56:59.390575
1491	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-03 08:57:43.94307	2020-10-03 08:57:43.94307
1492	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 08:57:44.698518	2020-10-03 08:57:44.698518
1493	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-03 08:57:45.675198	2020-10-03 08:57:45.675198
1494	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 08:57:45.840312	2020-10-03 08:57:45.840312
1495	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-03 08:58:27.826267	2020-10-03 08:58:27.826267
1496	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 08:58:27.84265	2020-10-03 08:58:27.84265
1497	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-03 08:58:27.868819	2020-10-03 08:58:27.868819
1498	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 08:58:27.875632	2020-10-03 08:58:27.875632
1499	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-03 08:59:17.997782	2020-10-03 08:59:17.997782
1500	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 08:59:18.020949	2020-10-03 08:59:18.020949
1501	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-03 08:59:18.046636	2020-10-03 08:59:18.046636
1502	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 08:59:18.05365	2020-10-03 08:59:18.05365
1503	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-03 09:11:05.79084	2020-10-03 09:11:05.79084
1504	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 09:11:05.803862	2020-10-03 09:11:05.803862
1505	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-03 09:11:05.829077	2020-10-03 09:11:05.829077
1506	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 09:11:05.837444	2020-10-03 09:11:05.837444
1507	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-03 09:21:07.822835	2020-10-03 09:21:07.822835
1508	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 09:21:07.838977	2020-10-03 09:21:07.838977
1509	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-03 09:21:07.8665	2020-10-03 09:21:07.8665
1510	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 09:21:07.873877	2020-10-03 09:21:07.873877
1511	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-03 09:37:08.642053	2020-10-03 09:37:08.642053
1512	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 09:37:08.654259	2020-10-03 09:37:08.654259
1513	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-03 09:37:08.679846	2020-10-03 09:37:08.679846
1514	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 09:37:08.689334	2020-10-03 09:37:08.689334
1515	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-03 09:39:55.327351	2020-10-03 09:39:55.327351
1516	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 09:39:55.348784	2020-10-03 09:39:55.348784
1517	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-03 09:39:55.374783	2020-10-03 09:39:55.374783
1518	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 09:39:55.382102	2020-10-03 09:39:55.382102
1519	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-03 09:42:05.149343	2020-10-03 09:42:05.149343
1520	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 09:42:05.164854	2020-10-03 09:42:05.164854
1521	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-03 09:42:05.190817	2020-10-03 09:42:05.190817
1522	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 09:42:05.197847	2020-10-03 09:42:05.197847
1523	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-03 09:42:59.462382	2020-10-03 09:42:59.462382
1524	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 09:42:59.476335	2020-10-03 09:42:59.476335
1525	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-03 09:42:59.503583	2020-10-03 09:42:59.503583
1526	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 09:42:59.509565	2020-10-03 09:42:59.509565
1527	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-03 09:44:09.397298	2020-10-03 09:44:09.397298
1528	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 09:44:10.581751	2020-10-03 09:44:10.581751
1529	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-03 09:44:10.907518	2020-10-03 09:44:10.907518
1530	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 09:44:10.914471	2020-10-03 09:44:10.914471
1531	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-03 09:45:24.772276	2020-10-03 09:45:24.772276
1532	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 09:45:24.819132	2020-10-03 09:45:24.819132
1533	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-03 09:45:24.844796	2020-10-03 09:45:24.844796
1534	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 09:45:24.852227	2020-10-03 09:45:24.852227
1535	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-03 09:48:33.560056	2020-10-03 09:48:33.560056
1536	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 09:48:33.888515	2020-10-03 09:48:33.888515
1537	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-03 09:48:34.658894	2020-10-03 09:48:34.658894
1538	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 09:48:34.755259	2020-10-03 09:48:34.755259
1539	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-03 09:50:23.540292	2020-10-03 09:50:23.540292
1540	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 09:50:23.561685	2020-10-03 09:50:23.561685
1541	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-03 09:50:23.654146	2020-10-03 09:50:23.654146
1542	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 09:50:23.661624	2020-10-03 09:50:23.661624
1543	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-03 09:51:49.940607	2020-10-03 09:51:49.940607
1544	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 09:51:49.949188	2020-10-03 09:51:49.949188
1545	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-03 09:51:49.976159	2020-10-03 09:51:49.976159
1546	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 09:51:49.983534	2020-10-03 09:51:49.983534
1547	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-03 09:53:14.041417	2020-10-03 09:53:14.041417
1548	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 09:53:14.063395	2020-10-03 09:53:14.063395
1549	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-03 09:53:14.089345	2020-10-03 09:53:14.089345
1550	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 09:53:14.097926	2020-10-03 09:53:14.097926
1551	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-03 10:08:01.247438	2020-10-03 10:08:01.247438
1552	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 10:08:01.266549	2020-10-03 10:08:01.266549
1553	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-03 10:08:01.292739	2020-10-03 10:08:01.292739
1554	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 10:08:01.299945	2020-10-03 10:08:01.299945
1555	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-03 10:15:29.855915	2020-10-03 10:15:29.855915
1556	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 10:15:30.134525	2020-10-03 10:15:30.134525
1557	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-03 10:15:30.22193	2020-10-03 10:15:30.22193
1558	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 10:15:30.242929	2020-10-03 10:15:30.242929
1559	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-03 10:22:30.842274	2020-10-03 10:22:30.842274
1560	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 10:22:31.293021	2020-10-03 10:22:31.293021
1561	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-03 10:22:31.469088	2020-10-03 10:22:31.469088
1562	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 10:22:31.534683	2020-10-03 10:22:31.534683
1563	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-03 10:24:15.060899	2020-10-03 10:24:15.060899
1564	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 10:24:15.082414	2020-10-03 10:24:15.082414
1565	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-03 10:24:15.10854	2020-10-03 10:24:15.10854
1566	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 10:24:15.115708	2020-10-03 10:24:15.115708
1567	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-03 10:26:01.931057	2020-10-03 10:26:01.931057
1568	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 10:26:01.947132	2020-10-03 10:26:01.947132
1569	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-03 10:26:01.974576	2020-10-03 10:26:01.974576
1570	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 10:26:01.981532	2020-10-03 10:26:01.981532
1571	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-03 10:29:26.36804	2020-10-03 10:29:26.36804
1572	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 10:29:26.452014	2020-10-03 10:29:26.452014
1573	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-03 10:29:26.477541	2020-10-03 10:29:26.477541
1574	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 10:29:26.485441	2020-10-03 10:29:26.485441
1575	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-03 10:30:41.115674	2020-10-03 10:30:41.115674
1576	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 10:30:41.131521	2020-10-03 10:30:41.131521
1577	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-03 10:30:41.157351	2020-10-03 10:30:41.157351
1578	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 10:30:41.164766	2020-10-03 10:30:41.164766
1579	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-03 10:36:22.758002	2020-10-03 10:36:22.758002
1580	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 10:36:22.767567	2020-10-03 10:36:22.767567
1581	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-03 10:36:22.803032	2020-10-03 10:36:22.803032
1582	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 10:36:22.809888	2020-10-03 10:36:22.809888
1583	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-03 11:57:21.939871	2020-10-03 11:57:21.939871
1584	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 11:57:21.963843	2020-10-03 11:57:21.963843
1585	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-03 11:57:21.989864	2020-10-03 11:57:21.989864
1586	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 11:57:21.997032	2020-10-03 11:57:21.997032
1587	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-03 17:17:16.274792	2020-10-03 17:17:16.274792
1588	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 17:17:16.648082	2020-10-03 17:17:16.648082
1589	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-03 17:17:16.897769	2020-10-03 17:17:16.897769
1590	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 17:17:16.936342	2020-10-03 17:17:16.936342
1591	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-03 18:16:55.999326	2020-10-03 18:16:55.999326
1592	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 18:16:56.020479	2020-10-03 18:16:56.020479
1593	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-03 18:16:56.046342	2020-10-03 18:16:56.046342
1594	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 18:16:56.054282	2020-10-03 18:16:56.054282
1595	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-03 18:37:25.940707	2020-10-03 18:37:25.940707
1596	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 18:37:25.959669	2020-10-03 18:37:25.959669
1597	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-03 18:37:25.985507	2020-10-03 18:37:25.985507
1598	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 18:37:25.992789	2020-10-03 18:37:25.992789
1599	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-03 19:07:56.359387	2020-10-03 19:07:56.359387
1600	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 19:07:56.650502	2020-10-03 19:07:56.650502
1601	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-03 19:07:56.792689	2020-10-03 19:07:56.792689
1602	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 19:07:56.800361	2020-10-03 19:07:56.800361
1603	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-03 19:27:44.595182	2020-10-03 19:27:44.595182
1604	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 19:27:45.371267	2020-10-03 19:27:45.371267
1605	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-03 19:27:46.379495	2020-10-03 19:27:46.379495
1606	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-03 19:27:46.446011	2020-10-03 19:27:46.446011
1607	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-05 17:49:30.614374	2020-10-05 17:49:30.614374
1608	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-05 17:49:30.68513	2020-10-05 17:49:30.68513
1609	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-05 17:49:30.854125	2020-10-05 17:49:30.854125
1610	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-05 17:49:30.868664	2020-10-05 17:49:30.868664
1611	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-05 17:50:44.320393	2020-10-05 17:50:44.320393
1612	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-05 17:50:44.339187	2020-10-05 17:50:44.339187
1613	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-05 17:50:44.365508	2020-10-05 17:50:44.365508
1614	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-05 17:50:44.372744	2020-10-05 17:50:44.372744
1615	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-05 18:00:48.199541	2020-10-05 18:00:48.199541
1616	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-05 18:00:49.53689	2020-10-05 18:00:49.53689
1617	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-05 18:00:49.809722	2020-10-05 18:00:49.809722
1618	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-05 18:00:49.933429	2020-10-05 18:00:49.933429
1619	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-05 18:06:10.387782	2020-10-05 18:06:10.387782
1620	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-05 18:06:10.392788	2020-10-05 18:06:10.392788
1621	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-05 18:06:10.419729	2020-10-05 18:06:10.419729
1622	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-05 18:06:10.42733	2020-10-05 18:06:10.42733
1623	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-05 18:13:30.585425	2020-10-05 18:13:30.585425
1624	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-05 18:13:30.603527	2020-10-05 18:13:30.603527
1625	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-05 18:13:30.629769	2020-10-05 18:13:30.629769
1626	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-05 18:13:30.636848	2020-10-05 18:13:30.636848
1627	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-05 18:17:13.694	2020-10-05 18:17:13.694
1628	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-05 18:17:13.716522	2020-10-05 18:17:13.716522
1629	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-05 18:17:13.742451	2020-10-05 18:17:13.742451
1630	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-05 18:17:13.749694	2020-10-05 18:17:13.749694
1631	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-05 23:16:15.103247	2020-10-05 23:16:15.103247
1632	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-05 23:16:15.123744	2020-10-05 23:16:15.123744
1633	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-05 23:16:15.149686	2020-10-05 23:16:15.149686
1634	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-05 23:16:15.156876	2020-10-05 23:16:15.156876
1635	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-05 23:33:15.532891	2020-10-05 23:33:15.532891
1636	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-05 23:33:15.550677	2020-10-05 23:33:15.550677
1637	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-05 23:33:15.576482	2020-10-05 23:33:15.576482
1638	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-05 23:33:15.584038	2020-10-05 23:33:15.584038
1639	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-06 02:14:24.337415	2020-10-06 02:14:24.337415
1640	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-06 02:14:24.467307	2020-10-06 02:14:24.467307
1641	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-06 02:14:24.514496	2020-10-06 02:14:24.514496
1642	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-06 02:14:24.521811	2020-10-06 02:14:24.521811
1643	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-06 18:11:30.103021	2020-10-06 18:11:30.103021
1644	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-06 18:11:30.118577	2020-10-06 18:11:30.118577
1645	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-06 18:11:30.144923	2020-10-06 18:11:30.144923
1646	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-06 18:11:30.152128	2020-10-06 18:11:30.152128
1647	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-06 18:13:29.822068	2020-10-06 18:13:29.822068
1648	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-06 18:13:29.967156	2020-10-06 18:13:29.967156
1649	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-06 18:13:30.392271	2020-10-06 18:13:30.392271
1650	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-06 18:13:30.738449	2020-10-06 18:13:30.738449
1651	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-06 18:13:44.592212	2020-10-06 18:13:44.592212
1652	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-06 18:13:44.609997	2020-10-06 18:13:44.609997
1653	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-06 18:13:44.636013	2020-10-06 18:13:44.636013
1654	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-06 18:13:44.643161	2020-10-06 18:13:44.643161
1655	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-06 18:15:08.467864	2020-10-06 18:15:08.467864
1656	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-06 18:15:08.497344	2020-10-06 18:15:08.497344
1657	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-06 18:15:08.522776	2020-10-06 18:15:08.522776
1658	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-06 18:15:08.530775	2020-10-06 18:15:08.530775
1659	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-06 18:15:25.419855	2020-10-06 18:15:25.419855
1660	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-06 18:15:25.431942	2020-10-06 18:15:25.431942
1661	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-06 18:15:25.45713	2020-10-06 18:15:25.45713
1662	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-06 18:15:25.465031	2020-10-06 18:15:25.465031
1663	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-06 18:16:11.095707	2020-10-06 18:16:11.095707
1664	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-06 18:16:11.110536	2020-10-06 18:16:11.110536
1665	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-06 18:16:11.136402	2020-10-06 18:16:11.136402
1666	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-06 18:16:11.143794	2020-10-06 18:16:11.143794
1667	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-06 18:16:48.019863	2020-10-06 18:16:48.019863
1668	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-06 18:16:48.037785	2020-10-06 18:16:48.037785
1669	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-06 18:16:48.064068	2020-10-06 18:16:48.064068
1670	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-06 18:16:48.071183	2020-10-06 18:16:48.071183
1671	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-06 18:18:27.218487	2020-10-06 18:18:27.218487
1672	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-06 18:18:27.235355	2020-10-06 18:18:27.235355
1673	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-06 18:18:27.261327	2020-10-06 18:18:27.261327
1674	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-06 18:18:27.308515	2020-10-06 18:18:27.308515
1675	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-06 18:19:47.978104	2020-10-06 18:19:47.978104
1676	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-06 18:19:47.998423	2020-10-06 18:19:47.998423
1677	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-06 18:19:48.025108	2020-10-06 18:19:48.025108
1678	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-06 18:19:48.031995	2020-10-06 18:19:48.031995
1679	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-06 18:20:54.454935	2020-10-06 18:20:54.454935
1680	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-06 18:20:54.477337	2020-10-06 18:20:54.477337
1681	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-06 18:20:54.503168	2020-10-06 18:20:54.503168
1682	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-06 18:20:54.510614	2020-10-06 18:20:54.510614
1683	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-06 18:42:05.127872	2020-10-06 18:42:05.127872
1684	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-06 18:42:05.144256	2020-10-06 18:42:05.144256
1685	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-06 18:42:05.170493	2020-10-06 18:42:05.170493
1686	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-06 18:42:05.177607	2020-10-06 18:42:05.177607
1687	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-07 00:46:14.556494	2020-10-07 00:46:14.556494
1688	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-07 00:46:14.788064	2020-10-07 00:46:14.788064
1689	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-07 00:46:15.004411	2020-10-07 00:46:15.004411
1690	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-07 00:46:15.020632	2020-10-07 00:46:15.020632
1691	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-07 01:22:27.510658	2020-10-07 01:22:27.510658
1692	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-07 01:22:27.531053	2020-10-07 01:22:27.531053
1693	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-07 01:22:27.556746	2020-10-07 01:22:27.556746
1694	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-07 01:22:27.56425	2020-10-07 01:22:27.56425
1695	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-07 01:27:32.062487	2020-10-07 01:27:32.062487
1696	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-07 01:27:32.083228	2020-10-07 01:27:32.083228
1697	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-07 01:27:32.109219	2020-10-07 01:27:32.109219
1698	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-07 01:27:32.116341	2020-10-07 01:27:32.116341
1699	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-07 01:32:11.171464	2020-10-07 01:32:11.171464
1700	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-07 01:32:11.651263	2020-10-07 01:32:11.651263
1701	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-07 01:32:13.010626	2020-10-07 01:32:13.010626
1702	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-07 01:32:13.059633	2020-10-07 01:32:13.059633
1703	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-07 01:33:29.822275	2020-10-07 01:33:29.822275
1704	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-07 01:33:29.83957	2020-10-07 01:33:29.83957
1705	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-07 01:33:29.865203	2020-10-07 01:33:29.865203
1706	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-07 01:33:29.872416	2020-10-07 01:33:29.872416
1707	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-08 17:22:18.931983	2020-10-08 17:22:18.931983
1708	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-08 17:22:19.421607	2020-10-08 17:22:19.421607
1709	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-08 17:22:19.604657	2020-10-08 17:22:19.604657
1710	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-08 17:22:19.623365	2020-10-08 17:22:19.623365
1711	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-08 17:26:52.826059	2020-10-08 17:26:52.826059
1712	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-08 17:26:52.848547	2020-10-08 17:26:52.848547
1713	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-08 17:26:52.874729	2020-10-08 17:26:52.874729
1714	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-08 17:26:52.881681	2020-10-08 17:26:52.881681
1715	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-08 18:01:04.102417	2020-10-08 18:01:04.102417
1716	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-08 18:01:04.50262	2020-10-08 18:01:04.50262
1717	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-08 18:01:04.603003	2020-10-08 18:01:04.603003
1718	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-08 18:01:04.61	2020-10-08 18:01:04.61
1719	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-08 18:03:56.286659	2020-10-08 18:03:56.286659
1720	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-08 18:03:56.312501	2020-10-08 18:03:56.312501
1721	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-08 18:03:56.338346	2020-10-08 18:03:56.338346
1722	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-08 18:03:56.345444	2020-10-08 18:03:56.345444
1723	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-09 17:04:19.630049	2020-10-09 17:04:19.630049
1724	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-09 17:04:19.86975	2020-10-09 17:04:19.86975
1725	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-09 17:04:20.094424	2020-10-09 17:04:20.094424
1726	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-09 17:04:20.113758	2020-10-09 17:04:20.113758
1727	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-09 17:41:34.782166	2020-10-09 17:41:34.782166
1728	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-09 17:41:34.801081	2020-10-09 17:41:34.801081
1729	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-09 17:41:34.827086	2020-10-09 17:41:34.827086
1730	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-09 17:41:34.834252	2020-10-09 17:41:34.834252
1731	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-09 17:47:58.931231	2020-10-09 17:47:58.931231
1732	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-09 17:47:58.949203	2020-10-09 17:47:58.949203
1733	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-09 17:47:58.976288	2020-10-09 17:47:58.976288
1734	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-09 17:47:58.983192	2020-10-09 17:47:58.983192
1735	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-09 17:55:21.892401	2020-10-09 17:55:21.892401
1736	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-09 17:55:21.947831	2020-10-09 17:55:21.947831
1737	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-09 17:55:21.98697	2020-10-09 17:55:21.98697
1738	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-09 17:55:21.994154	2020-10-09 17:55:21.994154
1739	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-09 18:01:11.767705	2020-10-09 18:01:11.767705
1740	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-09 18:01:12.122614	2020-10-09 18:01:12.122614
1741	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-09 18:01:12.166987	2020-10-09 18:01:12.166987
1742	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-09 18:01:12.174332	2020-10-09 18:01:12.174332
1743	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-09 22:10:15.869662	2020-10-09 22:10:15.869662
1744	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-09 22:10:15.884902	2020-10-09 22:10:15.884902
1745	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-09 22:10:15.910759	2020-10-09 22:10:15.910759
1746	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-09 22:10:15.918148	2020-10-09 22:10:15.918148
1747	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-09 22:12:54.571656	2020-10-09 22:12:54.571656
1748	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-09 22:12:54.595352	2020-10-09 22:12:54.595352
1749	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-09 22:12:54.646144	2020-10-09 22:12:54.646144
1750	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-09 22:12:54.653269	2020-10-09 22:12:54.653269
1751	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-09 22:16:50.998507	2020-10-09 22:16:50.998507
1752	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-09 22:16:51.017977	2020-10-09 22:16:51.017977
1753	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-09 22:16:51.044096	2020-10-09 22:16:51.044096
1754	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-09 22:16:51.051311	2020-10-09 22:16:51.051311
1755	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-09 23:32:25.302119	2020-10-09 23:32:25.302119
1756	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-09 23:32:25.3233	2020-10-09 23:32:25.3233
1757	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-09 23:32:25.349146	2020-10-09 23:32:25.349146
1758	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-09 23:32:25.35648	2020-10-09 23:32:25.35648
1759	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-10 01:33:07.923692	2020-10-10 01:33:07.923692
1760	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-10 01:33:07.936875	2020-10-10 01:33:07.936875
1761	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-10 01:33:07.972022	2020-10-10 01:33:07.972022
1762	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-10 01:33:07.979402	2020-10-10 01:33:07.979402
1763	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-10 02:19:01.444757	2020-10-10 02:19:01.444757
1764	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-10 02:19:01.458032	2020-10-10 02:19:01.458032
1765	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-10 02:19:01.483705	2020-10-10 02:19:01.483705
1766	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-10 02:19:01.491367	2020-10-10 02:19:01.491367
1767	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-10 02:20:17.76013	2020-10-10 02:20:17.76013
1768	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-10 02:20:17.770823	2020-10-10 02:20:17.770823
1769	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-10 02:20:17.796698	2020-10-10 02:20:17.796698
1770	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-10 02:20:17.804134	2020-10-10 02:20:17.804134
1771	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-10 02:21:07.287765	2020-10-10 02:21:07.287765
1772	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-10 02:21:07.307554	2020-10-10 02:21:07.307554
1773	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-10 02:21:07.331949	2020-10-10 02:21:07.331949
1774	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-10 02:21:07.340551	2020-10-10 02:21:07.340551
1775	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-10 02:24:27.121023	2020-10-10 02:24:27.121023
1776	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-10 02:24:27.144909	2020-10-10 02:24:27.144909
1777	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-10 02:24:27.170846	2020-10-10 02:24:27.170846
1778	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-10 02:24:27.177964	2020-10-10 02:24:27.177964
1779	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-10 02:27:54.455441	2020-10-10 02:27:54.455441
1780	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-10 02:27:54.474531	2020-10-10 02:27:54.474531
1781	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-10 02:27:54.500502	2020-10-10 02:27:54.500502
1782	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-10 02:27:54.507515	2020-10-10 02:27:54.507515
1783	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-10 02:34:01.057004	2020-10-10 02:34:01.057004
1784	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-10 02:34:01.072306	2020-10-10 02:34:01.072306
1785	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-10 02:34:01.098641	2020-10-10 02:34:01.098641
1786	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-10 02:34:01.105438	2020-10-10 02:34:01.105438
1787	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-10 02:35:48.861558	2020-10-10 02:35:48.861558
1788	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-10 02:35:48.879092	2020-10-10 02:35:48.879092
1789	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-10 02:35:48.905271	2020-10-10 02:35:48.905271
1790	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-10 02:35:48.912206	2020-10-10 02:35:48.912206
1791	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-10 02:37:11.947765	2020-10-10 02:37:11.947765
1792	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-10 02:37:11.967418	2020-10-10 02:37:11.967418
1793	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-10 02:37:12.026473	2020-10-10 02:37:12.026473
1794	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-10 02:37:12.033872	2020-10-10 02:37:12.033872
1795	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-10 02:45:10.992105	2020-10-10 02:45:10.992105
1796	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-10 02:45:11.151266	2020-10-10 02:45:11.151266
1797	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-10 02:45:11.306845	2020-10-10 02:45:11.306845
1798	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-10 02:45:11.361366	2020-10-10 02:45:11.361366
1799	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-10 02:47:06.203974	2020-10-10 02:47:06.203974
1800	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-10 02:47:06.229395	2020-10-10 02:47:06.229395
1801	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-10 02:47:06.25557	2020-10-10 02:47:06.25557
1802	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-10 02:47:06.262485	2020-10-10 02:47:06.262485
1803	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-10 02:47:46.50617	2020-10-10 02:47:46.50617
1804	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-10 02:47:46.532095	2020-10-10 02:47:46.532095
1805	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-10 02:47:46.55792	2020-10-10 02:47:46.55792
1806	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-10 02:47:46.565044	2020-10-10 02:47:46.565044
1807	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-10 02:53:45.208187	2020-10-10 02:53:45.208187
1808	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-10 02:53:45.229344	2020-10-10 02:53:45.229344
1809	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-10 02:53:45.255709	2020-10-10 02:53:45.255709
1810	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-10 02:53:45.262401	2020-10-10 02:53:45.262401
1811	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-10 02:54:03.423293	2020-10-10 02:54:03.423293
1812	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-10 02:54:03.43883	2020-10-10 02:54:03.43883
1813	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-10 02:54:03.464904	2020-10-10 02:54:03.464904
1814	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-10 02:54:03.47204	2020-10-10 02:54:03.47204
1815	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-10 03:04:44.77615	2020-10-10 03:04:44.77615
1816	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-10 03:04:44.820879	2020-10-10 03:04:44.820879
1817	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-10 03:04:44.879841	2020-10-10 03:04:44.879841
1818	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-10 03:04:44.886456	2020-10-10 03:04:44.886456
1819	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-10 03:32:20.677038	2020-10-10 03:32:20.677038
1820	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-10 03:32:20.698313	2020-10-10 03:32:20.698313
1821	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-10 03:32:20.732728	2020-10-10 03:32:20.732728
1822	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-10 03:32:20.739694	2020-10-10 03:32:20.739694
1823	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-10 03:36:58.025739	2020-10-10 03:36:58.025739
1824	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-10 03:36:58.674127	2020-10-10 03:36:58.674127
1825	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-10 03:36:59.133556	2020-10-10 03:36:59.133556
1826	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-10 03:36:59.199023	2020-10-10 03:36:59.199023
1827	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-10 03:45:36.862893	2020-10-10 03:45:36.862893
1828	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-10 03:45:36.872562	2020-10-10 03:45:36.872562
1829	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-10 03:45:36.898168	2020-10-10 03:45:36.898168
1830	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-10 03:45:36.90592	2020-10-10 03:45:36.90592
1831	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-10 04:11:21.654777	2020-10-10 04:11:21.654777
1832	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-10 04:11:21.685302	2020-10-10 04:11:21.685302
1833	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-10 04:11:21.711034	2020-10-10 04:11:21.711034
1834	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-10 04:11:21.718543	2020-10-10 04:11:21.718543
1835	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-13 00:11:27.631425	2020-10-13 00:11:27.631425
1836	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-13 00:11:27.946287	2020-10-13 00:11:27.946287
1837	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-13 00:11:28.195791	2020-10-13 00:11:28.195791
1838	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-13 00:11:28.235545	2020-10-13 00:11:28.235545
1839	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-13 00:58:17.330243	2020-10-13 00:58:17.330243
1840	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-13 00:58:17.357329	2020-10-13 00:58:17.357329
1841	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-13 00:58:17.383054	2020-10-13 00:58:17.383054
1842	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-13 00:58:17.390434	2020-10-13 00:58:17.390434
1843	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-13 01:00:20.69138	2020-10-13 01:00:20.69138
1844	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-13 01:00:20.706407	2020-10-13 01:00:20.706407
1845	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-13 01:00:20.732449	2020-10-13 01:00:20.732449
1846	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-13 01:00:20.739611	2020-10-13 01:00:20.739611
1847	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-13 01:09:13.50635	2020-10-13 01:09:13.50635
1848	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-13 01:09:13.521081	2020-10-13 01:09:13.521081
1849	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-13 01:09:13.547151	2020-10-13 01:09:13.547151
1850	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-13 01:09:13.554283	2020-10-13 01:09:13.554283
1851	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-13 01:11:15.025279	2020-10-13 01:11:15.025279
1852	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-13 01:11:15.420308	2020-10-13 01:11:15.420308
1853	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-13 01:11:15.529839	2020-10-13 01:11:15.529839
1854	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-13 01:11:15.536775	2020-10-13 01:11:15.536775
1855	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-13 01:12:08.341289	2020-10-13 01:12:08.341289
1856	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-13 01:12:08.356477	2020-10-13 01:12:08.356477
1857	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-13 01:12:08.38275	2020-10-13 01:12:08.38275
1858	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-13 01:12:08.389886	2020-10-13 01:12:08.389886
1859	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-13 01:13:07.470498	2020-10-13 01:13:07.470498
1860	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-13 01:13:07.485223	2020-10-13 01:13:07.485223
1861	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-13 01:13:07.511229	2020-10-13 01:13:07.511229
1862	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-13 01:13:07.518471	2020-10-13 01:13:07.518471
1863	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-13 01:14:09.600811	2020-10-13 01:14:09.600811
1864	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-13 01:14:09.622203	2020-10-13 01:14:09.622203
1865	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-13 01:14:09.648022	2020-10-13 01:14:09.648022
1866	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-13 01:14:09.655312	2020-10-13 01:14:09.655312
1867	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-13 01:14:38.73299	2020-10-13 01:14:38.73299
1868	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-13 01:14:38.825177	2020-10-13 01:14:38.825177
1869	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-13 01:14:39.105699	2020-10-13 01:14:39.105699
1870	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-13 01:14:39.224193	2020-10-13 01:14:39.224193
1871	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-13 01:16:22.777042	2020-10-13 01:16:22.777042
1872	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-13 01:16:22.797098	2020-10-13 01:16:22.797098
1873	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-13 01:16:22.82312	2020-10-13 01:16:22.82312
1874	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-13 01:16:22.830129	2020-10-13 01:16:22.830129
1875	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-13 01:19:21.086877	2020-10-13 01:19:21.086877
1876	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-13 01:19:22.068103	2020-10-13 01:19:22.068103
1877	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-13 01:19:22.550985	2020-10-13 01:19:22.550985
1878	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-13 01:19:22.608223	2020-10-13 01:19:22.608223
1879	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-13 01:20:45.82455	2020-10-13 01:20:45.82455
1880	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-13 01:20:45.846236	2020-10-13 01:20:45.846236
1881	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-13 01:20:45.880743	2020-10-13 01:20:45.880743
1882	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-13 01:20:45.888035	2020-10-13 01:20:45.888035
1883	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-13 01:24:46.002415	2020-10-13 01:24:46.002415
1884	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-13 01:24:46.018193	2020-10-13 01:24:46.018193
1885	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-13 01:24:46.043654	2020-10-13 01:24:46.043654
1886	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-13 01:24:46.05145	2020-10-13 01:24:46.05145
1887	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-13 01:25:46.122257	2020-10-13 01:25:46.122257
1888	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-13 01:25:46.297662	2020-10-13 01:25:46.297662
1889	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-13 01:25:46.506911	2020-10-13 01:25:46.506911
1890	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-13 01:25:46.514249	2020-10-13 01:25:46.514249
1891	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-13 02:05:40.301603	2020-10-13 02:05:40.301603
1892	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-13 02:05:40.393174	2020-10-13 02:05:40.393174
1893	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-13 02:05:41.041423	2020-10-13 02:05:41.041423
1894	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-13 02:05:41.065003	2020-10-13 02:05:41.065003
1895	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-13 02:28:20.164529	2020-10-13 02:28:20.164529
1896	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-13 02:28:20.187446	2020-10-13 02:28:20.187446
1897	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-13 02:28:20.213241	2020-10-13 02:28:20.213241
1898	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-13 02:28:20.220582	2020-10-13 02:28:20.220582
1899	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-13 17:35:44.902768	2020-10-13 17:35:44.902768
1900	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-13 17:35:45.148816	2020-10-13 17:35:45.148816
1901	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-13 17:35:45.3985	2020-10-13 17:35:45.3985
1902	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-13 17:35:45.423211	2020-10-13 17:35:45.423211
1903	0	20	90	90	10	20	90	90	70	SNIPER	0.812999999999999945	7	2	19	3	450	0	13	4	4.29999999999999982	9	10	1818	500	SNIPER	6	Heckler and Koch	PSG1	8	COMMON	psg1.yml	2020-10-13 17:41:28.149347	2020-10-13 17:41:28.149347
1904	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-13 17:41:28.167986	2020-10-13 17:41:28.167986
1905	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-13 17:41:28.194007	2020-10-13 17:41:28.194007
1906	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-13 17:41:28.201105	2020-10-13 17:41:28.201105
1907	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-13 20:02:05.813475	2020-10-13 20:02:05.813475
1908	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-13 20:02:05.830666	2020-10-13 20:02:05.830666
1909	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-13 20:03:31.610077	2020-10-13 20:03:31.610077
1910	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-13 20:03:31.627562	2020-10-13 20:03:31.627562
1911	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-13 20:04:38.450104	2020-10-13 20:04:38.450104
1912	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-13 20:04:38.464564	2020-10-13 20:04:38.464564
1913	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-13 20:05:43.802237	2020-10-13 20:05:43.802237
1914	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-13 20:05:44.609804	2020-10-13 20:05:44.609804
1915	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-13 20:06:08.552757	2020-10-13 20:06:08.552757
1916	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-13 20:06:08.561674	2020-10-13 20:06:08.561674
1917	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-13 20:07:30.887975	2020-10-13 20:07:30.887975
1918	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-13 20:07:30.908244	2020-10-13 20:07:30.908244
1919	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-13 20:08:00.726166	2020-10-13 20:08:00.726166
1920	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-13 20:08:00.743102	2020-10-13 20:08:00.743102
1921	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-13 20:09:17.798584	2020-10-13 20:09:17.798584
1922	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-13 20:09:17.814703	2020-10-13 20:09:17.814703
1923	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-13 20:13:42.189007	2020-10-13 20:13:42.189007
1924	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-13 20:13:42.196882	2020-10-13 20:13:42.196882
1925	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-13 20:14:19.399955	2020-10-13 20:14:19.399955
1926	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-13 20:14:19.415803	2020-10-13 20:14:19.415803
1927	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-15 21:54:01.67069	2020-10-15 21:54:01.67069
1928	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-15 21:54:02.179378	2020-10-15 21:54:02.179378
1929	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-15 21:54:58.640853	2020-10-15 21:54:58.640853
1930	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-15 21:54:58.657401	2020-10-15 21:54:58.657401
1931	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-15 22:00:46.843445	2020-10-15 22:00:46.843445
1932	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-15 22:00:46.86151	2020-10-15 22:00:46.86151
1933	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-16 16:55:41.763239	2020-10-16 16:55:41.763239
1934	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-16 16:55:41.988558	2020-10-16 16:55:41.988558
1935	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-16 17:12:00.359038	2020-10-16 17:12:00.359038
1936	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-16 17:12:00.378208	2020-10-16 17:12:00.378208
1937	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-16 17:27:07.716073	2020-10-16 17:27:07.716073
1938	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-16 17:27:09.065092	2020-10-16 17:27:09.065092
1939	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-16 17:45:48.537846	2020-10-16 17:45:48.537846
1940	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-16 17:45:48.55561	2020-10-16 17:45:48.55561
1941	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-16 19:23:37.77145	2020-10-16 19:23:37.77145
1942	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-16 19:23:37.785648	2020-10-16 19:23:37.785648
1943	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-16 19:27:19.048557	2020-10-16 19:27:19.048557
1944	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-16 19:27:19.107438	2020-10-16 19:27:19.107438
1945	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-17 21:45:16.965405	2020-10-17 21:45:16.965405
1946	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-17 21:45:16.986897	2020-10-17 21:45:16.986897
1947	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-17 21:51:54.741272	2020-10-17 21:51:54.741272
1948	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-17 21:51:54.968678	2020-10-17 21:51:54.968678
1949	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-18 00:39:15.335609	2020-10-18 00:39:15.335609
1950	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-18 00:39:15.602685	2020-10-18 00:39:15.602685
1951	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-19 19:37:17.829132	2020-10-19 19:37:17.829132
1952	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-19 19:37:18.055667	2020-10-19 19:37:18.055667
1953	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-19 19:42:45.950045	2020-10-19 19:42:45.950045
1954	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-19 19:42:45.96743	2020-10-19 19:42:45.96743
1955	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-19 19:43:25.515048	2020-10-19 19:43:25.515048
1956	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-19 19:43:25.536344	2020-10-19 19:43:25.536344
1957	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-19 19:44:52.038084	2020-10-19 19:44:52.038084
1958	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-19 19:44:52.057779	2020-10-19 19:44:52.057779
1959	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-19 19:55:18.940926	2020-10-19 19:55:18.940926
1960	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-19 19:55:18.963308	2020-10-19 19:55:18.963308
1961	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-19 19:58:15.815842	2020-10-19 19:58:15.815842
1962	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-19 19:58:15.832181	2020-10-19 19:58:15.832181
1963	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-19 20:06:23.869979	2020-10-19 20:06:23.869979
1964	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-19 20:06:24.884777	2020-10-19 20:06:24.884777
1965	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-19 20:15:56.390337	2020-10-19 20:15:56.390337
1966	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-19 20:15:56.4069	2020-10-19 20:15:56.4069
1967	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-19 20:20:04.903999	2020-10-19 20:20:04.903999
1968	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-19 20:20:04.922106	2020-10-19 20:20:04.922106
1969	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-19 20:24:05.227954	2020-10-19 20:24:05.227954
1970	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-19 20:24:05.428803	2020-10-19 20:24:05.428803
1971	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-19 20:24:37.414418	2020-10-19 20:24:37.414418
1972	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-19 20:24:37.438915	2020-10-19 20:24:37.438915
1973	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-20 17:35:45.063245	2020-10-20 17:35:45.063245
1974	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-20 17:35:45.315596	2020-10-20 17:35:45.315596
1975	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-20 17:53:44.093525	2020-10-20 17:53:44.093525
1976	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-20 17:53:44.114536	2020-10-20 17:53:44.114536
1977	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-20 18:04:31.833743	2020-10-20 18:04:31.833743
1978	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-20 18:04:31.854154	2020-10-20 18:04:31.854154
1979	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-20 18:06:12.896446	2020-10-20 18:06:12.896446
1980	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-20 18:06:12.918583	2020-10-20 18:06:12.918583
1981	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-20 18:06:48.988781	2020-10-20 18:06:48.988781
1982	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-20 18:06:49.004284	2020-10-20 18:06:49.004284
1983	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-20 18:08:32.002606	2020-10-20 18:08:32.002606
1984	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-20 18:08:32.036417	2020-10-20 18:08:32.036417
1985	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-20 18:14:17.745014	2020-10-20 18:14:17.745014
1986	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-20 18:14:17.765932	2020-10-20 18:14:17.765932
1987	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-20 18:15:06.089327	2020-10-20 18:15:06.089327
1988	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-20 18:15:06.110576	2020-10-20 18:15:06.110576
1989	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-20 18:15:27.78513	2020-10-20 18:15:27.78513
1990	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-20 18:15:27.795386	2020-10-20 18:15:27.795386
1991	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-20 18:16:20.070368	2020-10-20 18:16:20.070368
1992	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-20 18:16:20.090239	2020-10-20 18:16:20.090239
1993	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-20 18:24:38.897485	2020-10-20 18:24:38.897485
1994	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-20 18:24:39.576928	2020-10-20 18:24:39.576928
1995	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-20 18:24:55.552191	2020-10-20 18:24:55.552191
1996	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-20 18:24:55.571955	2020-10-20 18:24:55.571955
1997	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-20 18:35:51.367441	2020-10-20 18:35:51.367441
1998	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-20 18:35:51.386989	2020-10-20 18:35:51.386989
1999	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-20 18:41:06.258258	2020-10-20 18:41:06.258258
2000	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-20 18:41:06.2814	2020-10-20 18:41:06.2814
2001	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-20 18:43:01.306045	2020-10-20 18:43:01.306045
2002	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-20 18:43:01.321578	2020-10-20 18:43:01.321578
2003	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-20 18:46:00.037983	2020-10-20 18:46:00.037983
2004	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-20 18:46:01.336626	2020-10-20 18:46:01.336626
2005	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-20 20:29:48.973121	2020-10-20 20:29:48.973121
2006	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-20 20:29:48.995834	2020-10-20 20:29:48.995834
2007	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-20 20:34:48.505003	2020-10-20 20:34:48.505003
2008	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-20 20:34:48.522442	2020-10-20 20:34:48.522442
2009	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-20 20:40:00.656168	2020-10-20 20:40:00.656168
2010	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-20 20:40:00.675396	2020-10-20 20:40:00.675396
2011	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-20 20:41:09.236597	2020-10-20 20:41:09.236597
2012	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-20 20:41:09.254733	2020-10-20 20:41:09.254733
2013	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-20 20:57:55.302097	2020-10-20 20:57:55.302097
2014	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-20 20:57:55.324875	2020-10-20 20:57:55.324875
2015	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-20 21:10:32.214661	2020-10-20 21:10:32.214661
2016	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-20 21:10:32.229205	2020-10-20 21:10:32.229205
2017	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-20 21:12:06.518498	2020-10-20 21:12:06.518498
2018	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-20 21:12:06.534573	2020-10-20 21:12:06.534573
2019	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-20 21:14:21.147377	2020-10-20 21:14:21.147377
2020	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-20 21:14:21.159858	2020-10-20 21:14:21.159858
2021	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-20 21:15:22.67928	2020-10-20 21:15:22.67928
2022	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-20 21:15:22.688046	2020-10-20 21:15:22.688046
2023	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-20 21:15:31.656741	2020-10-20 21:15:31.656741
2024	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-20 21:15:31.672795	2020-10-20 21:15:31.672795
2025	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-20 21:20:41.25006	2020-10-20 21:20:41.25006
2026	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-20 21:20:41.265963	2020-10-20 21:20:41.265963
2027	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-20 21:27:53.877344	2020-10-20 21:27:53.877344
2028	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-20 21:27:53.8939	2020-10-20 21:27:53.8939
2029	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-20 21:32:29.164371	2020-10-20 21:32:29.164371
2030	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-20 21:32:29.185476	2020-10-20 21:32:29.185476
2031	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-21 17:24:41.420796	2020-10-21 17:24:41.420796
2032	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-21 17:24:41.635443	2020-10-21 17:24:41.635443
2033	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-21 17:51:58.573932	2020-10-21 17:51:58.573932
2034	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-21 17:51:59.040205	2020-10-21 17:51:59.040205
2035	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-21 17:52:25.322731	2020-10-21 17:52:25.322731
2036	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-21 17:52:25.347356	2020-10-21 17:52:25.347356
2037	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-21 17:53:29.900768	2020-10-21 17:53:29.900768
2038	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-21 17:53:29.917576	2020-10-21 17:53:29.917576
2039	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-21 17:56:20.02234	2020-10-21 17:56:20.02234
2040	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-21 17:56:20.044161	2020-10-21 17:56:20.044161
2041	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-21 17:58:51.54542	2020-10-21 17:58:51.54542
2042	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-21 17:58:51.561293	2020-10-21 17:58:51.561293
2043	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-21 17:59:23.382283	2020-10-21 17:59:23.382283
2044	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-21 17:59:23.396535	2020-10-21 17:59:23.396535
2045	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-21 18:06:15.121603	2020-10-21 18:06:15.121603
2046	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-21 18:06:15.137335	2020-10-21 18:06:15.137335
2047	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-21 18:10:51.289341	2020-10-21 18:10:51.289341
2048	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-21 18:10:51.311616	2020-10-21 18:10:51.311616
2049	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-21 18:11:03.714421	2020-10-21 18:11:03.714421
2050	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-21 18:11:03.729253	2020-10-21 18:11:03.729253
2051	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-21 18:34:05.933249	2020-10-21 18:34:05.933249
2052	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-21 18:34:05.942807	2020-10-21 18:34:05.942807
2053	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-21 18:35:43.595709	2020-10-21 18:35:43.595709
2054	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-21 18:35:43.606989	2020-10-21 18:35:43.606989
2055	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-21 18:51:24.368734	2020-10-21 18:51:24.368734
2056	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-21 18:51:24.386981	2020-10-21 18:51:24.386981
2057	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-22 19:20:35.500314	2020-10-22 19:20:35.500314
2058	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-22 19:20:35.724444	2020-10-22 19:20:35.724444
2059	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-22 19:20:48.023256	2020-10-22 19:20:48.023256
2060	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-22 19:20:48.041498	2020-10-22 19:20:48.041498
2061	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-22 21:26:45.065699	2020-10-22 21:26:45.065699
2062	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-22 21:26:45.085418	2020-10-22 21:26:45.085418
2063	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-22 21:32:31.504534	2020-10-22 21:32:31.504534
2064	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-22 21:32:31.522556	2020-10-22 21:32:31.522556
2065	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-22 22:44:36.999739	2020-10-22 22:44:36.999739
2066	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-22 22:44:38.012547	2020-10-22 22:44:38.012547
2067	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-22 22:56:19.245379	2020-10-22 22:56:19.245379
2068	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-22 22:56:19.260854	2020-10-22 22:56:19.260854
2069	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-22 23:19:17.210623	2020-10-22 23:19:17.210623
2070	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-22 23:19:17.225676	2020-10-22 23:19:17.225676
2071	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-22 23:19:38.954453	2020-10-22 23:19:38.954453
2072	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-22 23:19:38.968737	2020-10-22 23:19:38.968737
2073	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-22 23:20:33.301939	2020-10-22 23:20:33.301939
2074	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-22 23:20:33.321813	2020-10-22 23:20:33.321813
2075	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-23 18:35:16.047091	2020-10-23 18:35:16.047091
2076	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-23 18:35:16.263489	2020-10-23 18:35:16.263489
2077	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-23 18:41:16.453917	2020-10-23 18:41:16.453917
2078	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-23 18:41:16.484049	2020-10-23 18:41:16.484049
2079	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-23 18:42:39.274126	2020-10-23 18:42:39.274126
2080	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-23 18:42:39.288425	2020-10-23 18:42:39.288425
2081	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-23 18:43:04.890706	2020-10-23 18:43:04.890706
2082	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-23 18:43:04.906611	2020-10-23 18:43:04.906611
2083	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-23 18:51:46.747306	2020-10-23 18:51:46.747306
2084	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-23 18:51:46.761929	2020-10-23 18:51:46.761929
2085	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-23 18:59:39.777094	2020-10-23 18:59:39.777094
2086	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-23 18:59:39.790361	2020-10-23 18:59:39.790361
2087	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-23 19:03:59.242587	2020-10-23 19:03:59.242587
2088	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-23 19:03:59.92257	2020-10-23 19:03:59.92257
2089	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-23 19:12:22.575319	2020-10-23 19:12:22.575319
2090	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-23 19:12:22.585215	2020-10-23 19:12:22.585215
2091	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-23 19:13:15.765281	2020-10-23 19:13:15.765281
2092	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-23 19:13:15.779263	2020-10-23 19:13:15.779263
2093	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-23 19:13:37.22004	2020-10-23 19:13:37.22004
2094	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-23 19:13:37.24004	2020-10-23 19:13:37.24004
2095	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-26 19:20:30.166035	2020-10-26 19:20:30.166035
2096	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-26 19:20:30.39979	2020-10-26 19:20:30.39979
2097	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-26 19:31:01.276288	2020-10-26 19:31:01.276288
2098	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-26 19:31:01.294343	2020-10-26 19:31:01.294343
2099	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-26 19:40:48.756634	2020-10-26 19:40:48.756634
2100	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-26 19:40:48.778551	2020-10-26 19:40:48.778551
2101	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-26 19:46:42.383603	2020-10-26 19:46:42.383603
2102	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-26 19:46:42.399194	2020-10-26 19:46:42.399194
2103	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-26 20:03:48.629653	2020-10-26 20:03:48.629653
2104	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-26 20:03:48.843019	2020-10-26 20:03:48.843019
2105	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-26 20:18:39.733492	2020-10-26 20:18:39.733492
2106	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-26 20:18:39.753709	2020-10-26 20:18:39.753709
2107	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-26 20:25:59.233602	2020-10-26 20:25:59.233602
2108	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-26 20:25:59.254601	2020-10-26 20:25:59.254601
2109	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-26 20:26:37.027127	2020-10-26 20:26:37.027127
2110	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-26 20:26:37.048559	2020-10-26 20:26:37.048559
2111	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-26 23:01:13.724224	2020-10-26 23:01:13.724224
2112	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-26 23:01:13.896122	2020-10-26 23:01:13.896122
2113	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-26 23:07:36.270856	2020-10-26 23:07:36.270856
2114	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-26 23:07:36.293201	2020-10-26 23:07:36.293201
2115	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-26 23:13:10.395645	2020-10-26 23:13:10.395645
2116	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-26 23:13:10.413437	2020-10-26 23:13:10.413437
2117	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-26 23:14:45.451272	2020-10-26 23:14:45.451272
2118	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-26 23:14:45.468853	2020-10-26 23:14:45.468853
2119	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-26 23:16:57.804608	2020-10-26 23:16:57.804608
2120	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-26 23:16:57.827073	2020-10-26 23:16:57.827073
2121	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-26 23:18:05.686695	2020-10-26 23:18:05.686695
2122	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-26 23:18:05.705965	2020-10-26 23:18:05.705965
2123	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-26 23:23:29.549399	2020-10-26 23:23:29.549399
2124	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-26 23:23:29.56697	2020-10-26 23:23:29.56697
2125	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-26 23:26:59.172582	2020-10-26 23:26:59.172582
2126	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-26 23:26:59.187978	2020-10-26 23:26:59.187978
2127	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-26 23:29:49.86345	2020-10-26 23:29:49.86345
2128	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-26 23:29:49.88157	2020-10-26 23:29:49.88157
2129	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-26 23:31:25.993076	2020-10-26 23:31:25.993076
2130	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-26 23:31:26.012396	2020-10-26 23:31:26.012396
2131	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-26 23:31:33.868378	2020-10-26 23:31:33.868378
2132	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-26 23:31:33.887917	2020-10-26 23:31:33.887917
2133	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-26 23:32:26.218356	2020-10-26 23:32:26.218356
2134	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-26 23:32:26.240909	2020-10-26 23:32:26.240909
2135	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-26 23:34:16.572663	2020-10-26 23:34:16.572663
2136	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-26 23:34:16.589079	2020-10-26 23:34:16.589079
2137	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-26 23:36:10.316574	2020-10-26 23:36:10.316574
2138	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-26 23:36:10.596155	2020-10-26 23:36:10.596155
2139	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-26 23:38:24.377765	2020-10-26 23:38:24.377765
2140	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-26 23:38:24.395308	2020-10-26 23:38:24.395308
2141	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-26 23:45:00.685896	2020-10-26 23:45:00.685896
2142	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-26 23:45:00.710632	2020-10-26 23:45:00.710632
2143	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-26 23:47:04.024545	2020-10-26 23:47:04.024545
2144	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-26 23:47:04.042781	2020-10-26 23:47:04.042781
2145	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-26 23:50:04.708148	2020-10-26 23:50:04.708148
2146	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-26 23:50:04.728758	2020-10-26 23:50:04.728758
2147	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-26 23:51:37.516486	2020-10-26 23:51:37.516486
2148	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-26 23:51:37.534148	2020-10-26 23:51:37.534148
2149	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-26 23:53:21.531208	2020-10-26 23:53:21.531208
2150	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-26 23:53:21.649123	2020-10-26 23:53:21.649123
2151	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-26 23:56:58.008152	2020-10-26 23:56:58.008152
2152	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-26 23:56:58.02814	2020-10-26 23:56:58.02814
2153	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-26 23:57:11.094839	2020-10-26 23:57:11.094839
2154	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-26 23:57:11.112409	2020-10-26 23:57:11.112409
2155	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-26 23:59:06.805183	2020-10-26 23:59:06.805183
2156	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-26 23:59:06.827832	2020-10-26 23:59:06.827832
2157	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 00:02:00.119494	2020-10-27 00:02:00.119494
2158	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 00:02:00.138019	2020-10-27 00:02:00.138019
2159	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 00:03:12.279087	2020-10-27 00:03:12.279087
2160	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 00:03:12.300971	2020-10-27 00:03:12.300971
2161	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 00:04:27.631012	2020-10-27 00:04:27.631012
2162	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 00:04:27.646688	2020-10-27 00:04:27.646688
2163	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 00:07:44.407703	2020-10-27 00:07:44.407703
2164	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 00:07:44.589804	2020-10-27 00:07:44.589804
2165	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 00:08:42.338523	2020-10-27 00:08:42.338523
2166	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 00:08:42.353553	2020-10-27 00:08:42.353553
2167	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 00:11:56.367985	2020-10-27 00:11:56.367985
2168	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 00:11:56.390294	2020-10-27 00:11:56.390294
2169	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 00:14:57.939202	2020-10-27 00:14:57.939202
2170	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 00:14:57.959578	2020-10-27 00:14:57.959578
2171	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 00:17:49.244621	2020-10-27 00:17:49.244621
2172	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 00:17:49.261556	2020-10-27 00:17:49.261556
2173	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 00:22:41.686789	2020-10-27 00:22:41.686789
2174	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 00:22:41.703934	2020-10-27 00:22:41.703934
2175	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 00:25:37.78295	2020-10-27 00:25:37.78295
2176	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 00:25:37.798051	2020-10-27 00:25:37.798051
2177	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 00:26:31.767543	2020-10-27 00:26:31.767543
2178	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 00:26:31.976416	2020-10-27 00:26:31.976416
2179	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 00:32:30.867773	2020-10-27 00:32:30.867773
2180	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 00:32:30.889329	2020-10-27 00:32:30.889329
2181	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 00:32:56.213821	2020-10-27 00:32:56.213821
2182	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 00:32:56.650908	2020-10-27 00:32:56.650908
2183	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 00:35:47.819084	2020-10-27 00:35:47.819084
2184	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 00:35:47.83408	2020-10-27 00:35:47.83408
2185	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 00:44:37.773325	2020-10-27 00:44:37.773325
2186	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 00:44:39.113729	2020-10-27 00:44:39.113729
2187	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 00:46:04.195424	2020-10-27 00:46:04.195424
2188	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 00:46:04.920429	2020-10-27 00:46:04.920429
2189	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 00:50:03.382098	2020-10-27 00:50:03.382098
2190	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 00:50:03.920926	2020-10-27 00:50:03.920926
2191	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 00:51:43.29596	2020-10-27 00:51:43.29596
2192	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 00:51:43.318795	2020-10-27 00:51:43.318795
2193	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 00:53:41.028421	2020-10-27 00:53:41.028421
2194	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 00:53:41.767021	2020-10-27 00:53:41.767021
2195	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 00:56:28.299423	2020-10-27 00:56:28.299423
2196	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 00:56:28.432159	2020-10-27 00:56:28.432159
2197	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 00:57:28.06818	2020-10-27 00:57:28.06818
2198	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 00:57:28.089701	2020-10-27 00:57:28.089701
2199	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 01:00:14.939075	2020-10-27 01:00:14.939075
2200	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 01:00:15.066638	2020-10-27 01:00:15.066638
2201	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 01:01:02.870573	2020-10-27 01:01:02.870573
2202	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 01:01:02.885439	2020-10-27 01:01:02.885439
2203	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 01:06:10.715289	2020-10-27 01:06:10.715289
2204	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 01:06:10.737385	2020-10-27 01:06:10.737385
2205	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 01:07:10.778061	2020-10-27 01:07:10.778061
2206	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 01:07:10.799483	2020-10-27 01:07:10.799483
2207	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 01:07:52.453534	2020-10-27 01:07:52.453534
2208	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 01:07:52.717871	2020-10-27 01:07:52.717871
2209	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 01:10:13.104835	2020-10-27 01:10:13.104835
2210	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 01:10:13.119118	2020-10-27 01:10:13.119118
2211	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 01:11:54.582713	2020-10-27 01:11:54.582713
2212	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 01:11:54.608264	2020-10-27 01:11:54.608264
2213	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 01:13:28.391823	2020-10-27 01:13:28.391823
2214	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 01:13:28.413734	2020-10-27 01:13:28.413734
2215	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 01:15:27.418056	2020-10-27 01:15:27.418056
2216	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 01:15:27.612785	2020-10-27 01:15:27.612785
2217	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 01:16:40.884297	2020-10-27 01:16:40.884297
2218	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 01:16:40.900275	2020-10-27 01:16:40.900275
2219	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 01:17:08.561041	2020-10-27 01:17:08.561041
2220	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 01:17:08.576899	2020-10-27 01:17:08.576899
2221	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 01:18:34.717054	2020-10-27 01:18:34.717054
2222	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 01:18:34.722771	2020-10-27 01:18:34.722771
2223	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 01:21:01.579883	2020-10-27 01:21:01.579883
2224	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 01:21:01.831343	2020-10-27 01:21:01.831343
2225	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 01:21:42.443903	2020-10-27 01:21:42.443903
2226	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 01:21:43.468953	2020-10-27 01:21:43.468953
2227	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 01:22:36.441029	2020-10-27 01:22:36.441029
2228	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 01:22:36.464041	2020-10-27 01:22:36.464041
2229	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 01:25:34.313446	2020-10-27 01:25:34.313446
2230	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 01:25:34.323759	2020-10-27 01:25:34.323759
2231	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 01:26:16.608886	2020-10-27 01:26:16.608886
2232	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 01:26:16.627406	2020-10-27 01:26:16.627406
2233	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 01:28:34.512808	2020-10-27 01:28:34.512808
2234	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 01:28:34.527108	2020-10-27 01:28:34.527108
2235	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 01:29:35.050265	2020-10-27 01:29:35.050265
2236	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 01:29:36.661967	2020-10-27 01:29:36.661967
2237	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 01:32:12.175107	2020-10-27 01:32:12.175107
2238	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 01:32:12.302391	2020-10-27 01:32:12.302391
2239	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 01:35:05.585672	2020-10-27 01:35:05.585672
2240	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 01:35:06.655556	2020-10-27 01:35:06.655556
2241	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 01:44:36.400318	2020-10-27 01:44:36.400318
2242	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 01:44:36.418575	2020-10-27 01:44:36.418575
2243	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 01:44:41.759192	2020-10-27 01:44:41.759192
2244	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 01:44:41.776733	2020-10-27 01:44:41.776733
2245	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 01:44:47.191218	2020-10-27 01:44:47.191218
2246	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 01:44:47.219071	2020-10-27 01:44:47.219071
2247	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 01:44:52.586592	2020-10-27 01:44:52.586592
2248	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 01:44:52.646977	2020-10-27 01:44:52.646977
2249	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 01:45:29.004643	2020-10-27 01:45:29.004643
2250	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 01:45:29.021338	2020-10-27 01:45:29.021338
2251	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 01:45:34.548797	2020-10-27 01:45:34.548797
2252	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 01:45:34.563068	2020-10-27 01:45:34.563068
2253	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 01:45:39.949678	2020-10-27 01:45:39.949678
2254	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 01:45:39.972005	2020-10-27 01:45:39.972005
2255	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 01:45:45.47966	2020-10-27 01:45:45.47966
2256	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 01:45:45.52234	2020-10-27 01:45:45.52234
2257	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 01:45:50.832397	2020-10-27 01:45:50.832397
2258	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 01:45:50.838299	2020-10-27 01:45:50.838299
2259	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 01:45:56.251313	2020-10-27 01:45:56.251313
2260	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 01:45:56.272906	2020-10-27 01:45:56.272906
2261	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 01:46:01.727543	2020-10-27 01:46:01.727543
2262	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 01:46:01.748282	2020-10-27 01:46:01.748282
2263	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 01:46:07.06106	2020-10-27 01:46:07.06106
2264	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 01:46:07.081917	2020-10-27 01:46:07.081917
2265	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 01:46:12.479383	2020-10-27 01:46:12.479383
2266	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 01:46:12.498965	2020-10-27 01:46:12.498965
2267	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 01:46:17.885615	2020-10-27 01:46:17.885615
2268	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 01:46:17.907611	2020-10-27 01:46:17.907611
2269	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 01:46:23.43816	2020-10-27 01:46:23.43816
2270	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 01:46:23.45799	2020-10-27 01:46:23.45799
2271	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 01:57:48.756834	2020-10-27 01:57:48.756834
2272	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 01:57:48.773833	2020-10-27 01:57:48.773833
2273	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 01:58:32.028151	2020-10-27 01:58:32.028151
2274	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 01:58:32.043056	2020-10-27 01:58:32.043056
2275	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 02:15:15.671706	2020-10-27 02:15:15.671706
2276	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 02:15:16.711347	2020-10-27 02:15:16.711347
2277	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 02:22:12.973046	2020-10-27 02:22:12.973046
2278	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 02:22:13.107973	2020-10-27 02:22:13.107973
2279	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 03:08:40.856984	2020-10-27 03:08:40.856984
2280	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 03:08:40.934412	2020-10-27 03:08:40.934412
2281	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 03:15:59.967008	2020-10-27 03:15:59.967008
2282	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 03:15:59.992702	2020-10-27 03:15:59.992702
2283	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 03:18:35.168371	2020-10-27 03:18:35.168371
2284	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 03:18:35.176133	2020-10-27 03:18:35.176133
2285	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 03:24:39.673879	2020-10-27 03:24:39.673879
2286	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 03:24:39.690385	2020-10-27 03:24:39.690385
2287	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 03:27:56.753973	2020-10-27 03:27:56.753973
2288	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 03:27:56.759408	2020-10-27 03:27:56.759408
2289	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 03:39:16.032076	2020-10-27 03:39:16.032076
2290	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 03:39:16.050816	2020-10-27 03:39:16.050816
2291	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 03:41:17.603151	2020-10-27 03:41:17.603151
2292	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 03:41:17.958063	2020-10-27 03:41:17.958063
2293	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 03:42:17.865011	2020-10-27 03:42:17.865011
2294	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 03:42:17.886515	2020-10-27 03:42:17.886515
2295	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 03:46:54.352436	2020-10-27 03:46:54.352436
2296	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 03:46:54.369591	2020-10-27 03:46:54.369591
2297	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 04:10:05.245267	2020-10-27 04:10:05.245267
2298	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 04:10:06.826481	2020-10-27 04:10:06.826481
2299	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 04:11:11.720852	2020-10-27 04:11:11.720852
2300	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 04:11:11.740021	2020-10-27 04:11:11.740021
2301	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 17:32:34.594259	2020-10-27 17:32:34.594259
2302	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 17:32:34.937072	2020-10-27 17:32:34.937072
2303	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 17:32:49.58395	2020-10-27 17:32:49.58395
2304	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 17:32:49.603814	2020-10-27 17:32:49.603814
2305	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 17:33:10.424063	2020-10-27 17:33:10.424063
2306	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 17:33:10.446601	2020-10-27 17:33:10.446601
2307	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 17:51:40.520499	2020-10-27 17:51:40.520499
2308	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 17:51:40.544778	2020-10-27 17:51:40.544778
2309	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 18:19:22.781323	2020-10-27 18:19:22.781323
2310	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 18:19:22.800364	2020-10-27 18:19:22.800364
2311	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 19:03:11.215046	2020-10-27 19:03:11.215046
2312	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 19:03:11.343386	2020-10-27 19:03:11.343386
2313	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 19:04:26.975032	2020-10-27 19:04:26.975032
2314	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 19:04:27.18575	2020-10-27 19:04:27.18575
2315	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 19:05:14.006855	2020-10-27 19:05:14.006855
2316	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 19:05:14.190766	2020-10-27 19:05:14.190766
2317	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 19:06:29.762752	2020-10-27 19:06:29.762752
2318	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 19:06:29.784509	2020-10-27 19:06:29.784509
2319	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 19:12:45.313934	2020-10-27 19:12:45.313934
2320	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 19:12:45.331849	2020-10-27 19:12:45.331849
2321	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 19:12:56.47303	2020-10-27 19:12:56.47303
2322	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 19:12:56.490938	2020-10-27 19:12:56.490938
2323	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 19:13:28.797408	2020-10-27 19:13:28.797408
2324	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 19:13:28.816835	2020-10-27 19:13:28.816835
2325	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 19:16:07.290891	2020-10-27 19:16:07.290891
2326	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 19:16:07.310431	2020-10-27 19:16:07.310431
2327	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 19:23:13.142582	2020-10-27 19:23:13.142582
2328	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 19:23:13.152348	2020-10-27 19:23:13.152348
2329	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 19:28:26.403359	2020-10-27 19:28:26.403359
2330	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 19:28:26.421997	2020-10-27 19:28:26.421997
2331	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 19:37:28.668023	2020-10-27 19:37:28.668023
2332	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 19:37:28.794742	2020-10-27 19:37:28.794742
2333	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 19:38:30.21993	2020-10-27 19:38:30.21993
2334	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 19:38:30.641101	2020-10-27 19:38:30.641101
2335	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 19:46:11.531894	2020-10-27 19:46:11.531894
2336	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 19:46:11.551205	2020-10-27 19:46:11.551205
2337	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 19:47:04.334019	2020-10-27 19:47:04.334019
2338	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 19:47:04.354412	2020-10-27 19:47:04.354412
2339	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 19:54:07.558337	2020-10-27 19:54:07.558337
2340	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 19:54:07.838614	2020-10-27 19:54:07.838614
2341	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 19:56:01.232341	2020-10-27 19:56:01.232341
2342	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 19:56:01.253046	2020-10-27 19:56:01.253046
2343	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-27 23:59:04.549148	2020-10-27 23:59:04.549148
2344	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-27 23:59:04.941499	2020-10-27 23:59:04.941499
2345	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-28 00:02:25.272111	2020-10-28 00:02:25.272111
2346	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-28 00:02:25.830488	2020-10-28 00:02:25.830488
2347	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-28 00:04:21.263157	2020-10-28 00:04:21.263157
2348	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-28 00:04:21.805115	2020-10-28 00:04:21.805115
2349	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-28 00:05:09.711002	2020-10-28 00:05:09.711002
2350	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-28 00:05:09.729772	2020-10-28 00:05:09.729772
2351	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-28 00:06:17.664343	2020-10-28 00:06:17.664343
2352	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-28 00:06:17.684108	2020-10-28 00:06:17.684108
2353	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-28 00:09:06.381831	2020-10-28 00:09:06.381831
2354	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-28 00:09:06.393975	2020-10-28 00:09:06.393975
2355	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-28 00:21:46.094843	2020-10-28 00:21:46.094843
2356	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-28 00:21:46.114368	2020-10-28 00:21:46.114368
2357	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-28 00:25:13.068515	2020-10-28 00:25:13.068515
2358	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-28 00:25:13.085019	2020-10-28 00:25:13.085019
2359	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-28 00:25:38.608389	2020-10-28 00:25:38.608389
2360	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-28 00:25:38.628244	2020-10-28 00:25:38.628244
2361	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-28 00:26:41.718007	2020-10-28 00:26:41.718007
2362	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-28 00:26:41.762879	2020-10-28 00:26:41.762879
2363	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-28 01:54:43.430935	2020-10-28 01:54:43.430935
2364	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-28 01:54:43.446137	2020-10-28 01:54:43.446137
2365	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-28 01:56:24.714887	2020-10-28 01:56:24.714887
2366	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-28 01:56:24.735646	2020-10-28 01:56:24.735646
2367	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-28 02:04:03.819864	2020-10-28 02:04:03.819864
2368	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-28 02:04:03.884404	2020-10-28 02:04:03.884404
2369	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-28 02:05:35.512284	2020-10-28 02:05:35.512284
2370	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-28 02:05:35.535025	2020-10-28 02:05:35.535025
2371	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-28 02:08:26.015816	2020-10-28 02:08:26.015816
2372	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-28 02:08:26.303657	2020-10-28 02:08:26.303657
2373	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-28 02:11:10.205485	2020-10-28 02:11:10.205485
2374	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-28 02:11:10.22178	2020-10-28 02:11:10.22178
2375	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-28 02:12:52.828808	2020-10-28 02:12:52.828808
2376	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-28 02:12:52.85296	2020-10-28 02:12:52.85296
2377	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-28 02:14:01.524553	2020-10-28 02:14:01.524553
2378	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-28 02:14:01.548988	2020-10-28 02:14:01.548988
2379	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-28 02:20:41.246391	2020-10-28 02:20:41.246391
2380	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-28 02:20:41.26407	2020-10-28 02:20:41.26407
2381	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-28 02:22:28.644011	2020-10-28 02:22:28.644011
2382	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-28 02:22:28.670709	2020-10-28 02:22:28.670709
2383	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-30 17:00:17.900799	2020-10-30 17:00:17.900799
2384	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-30 17:00:18.128072	2020-10-30 17:00:18.128072
2385	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-30 17:22:24.093608	2020-10-30 17:22:24.093608
2386	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-30 17:22:24.116452	2020-10-30 17:22:24.116452
2387	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-30 17:22:33.238453	2020-10-30 17:22:33.238453
2388	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-30 17:22:33.258539	2020-10-30 17:22:33.258539
2389	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-30 17:25:30.513713	2020-10-30 17:25:30.513713
2390	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-30 17:25:30.534958	2020-10-30 17:25:30.534958
2391	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-30 17:29:13.217193	2020-10-30 17:29:13.217193
2392	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-30 17:29:13.230526	2020-10-30 17:29:13.230526
2393	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-30 17:32:06.21879	2020-10-30 17:32:06.21879
2394	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-30 17:32:06.231234	2020-10-30 17:32:06.231234
2395	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-30 23:43:56.659032	2020-10-30 23:43:56.659032
2396	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-30 23:43:56.678768	2020-10-30 23:43:56.678768
2397	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-30 23:45:57.387543	2020-10-30 23:45:57.387543
2398	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-30 23:45:57.40226	2020-10-30 23:45:57.40226
2399	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-30 23:53:40.947291	2020-10-30 23:53:40.947291
2400	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-30 23:53:40.969528	2020-10-30 23:53:40.969528
2401	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-30 23:56:06.945887	2020-10-30 23:56:06.945887
2402	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-30 23:56:06.961293	2020-10-30 23:56:06.961293
2403	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-30 23:58:17.004906	2020-10-30 23:58:17.004906
2404	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-30 23:58:17.896968	2020-10-30 23:58:17.896968
2405	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-30 23:58:49.296461	2020-10-30 23:58:49.296461
2406	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-30 23:58:49.31185	2020-10-30 23:58:49.31185
2407	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 00:00:13.329586	2020-10-31 00:00:13.329586
2408	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 00:00:13.788945	2020-10-31 00:00:13.788945
2409	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 00:01:58.531728	2020-10-31 00:01:58.531728
2410	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 00:01:58.547496	2020-10-31 00:01:58.547496
2411	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 00:03:43.777594	2020-10-31 00:03:43.777594
2412	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 00:03:44.020283	2020-10-31 00:03:44.020283
2413	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 00:08:14.117206	2020-10-31 00:08:14.117206
2414	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 00:08:14.13592	2020-10-31 00:08:14.13592
2415	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 00:09:16.894325	2020-10-31 00:09:16.894325
2416	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 00:09:17.781586	2020-10-31 00:09:17.781586
2417	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 00:11:34.597066	2020-10-31 00:11:34.597066
2418	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 00:11:34.604814	2020-10-31 00:11:34.604814
2419	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 00:12:53.436015	2020-10-31 00:12:53.436015
2420	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 00:12:54.160223	2020-10-31 00:12:54.160223
2421	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 00:15:26.834657	2020-10-31 00:15:26.834657
2422	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 00:15:26.905808	2020-10-31 00:15:26.905808
2423	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 00:16:20.464723	2020-10-31 00:16:20.464723
2424	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 00:16:21.530298	2020-10-31 00:16:21.530298
2425	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 00:17:40.269271	2020-10-31 00:17:40.269271
2426	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 00:17:40.28458	2020-10-31 00:17:40.28458
2427	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 00:20:45.343311	2020-10-31 00:20:45.343311
2428	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 00:20:45.361724	2020-10-31 00:20:45.361724
2429	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 00:23:19.046279	2020-10-31 00:23:19.046279
2430	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 00:23:19.062082	2020-10-31 00:23:19.062082
2431	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 00:24:04.278817	2020-10-31 00:24:04.278817
2432	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 00:24:04.298104	2020-10-31 00:24:04.298104
2433	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 00:26:49.157807	2020-10-31 00:26:49.157807
2434	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 00:26:49.173849	2020-10-31 00:26:49.173849
2435	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 00:28:43.64606	2020-10-31 00:28:43.64606
2436	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 00:28:43.663578	2020-10-31 00:28:43.663578
2437	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 00:32:19.684846	2020-10-31 00:32:19.684846
2438	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 00:32:19.691589	2020-10-31 00:32:19.691589
2439	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 00:35:08.009871	2020-10-31 00:35:08.009871
2440	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 00:35:08.02679	2020-10-31 00:35:08.02679
2441	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 00:36:11.241665	2020-10-31 00:36:11.241665
2442	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 00:36:12.050577	2020-10-31 00:36:12.050577
2443	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 00:37:23.784463	2020-10-31 00:37:23.784463
2444	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 00:37:24.254254	2020-10-31 00:37:24.254254
2445	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 00:37:55.369013	2020-10-31 00:37:55.369013
2446	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 00:37:55.378208	2020-10-31 00:37:55.378208
2447	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 00:40:23.629687	2020-10-31 00:40:23.629687
2448	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 00:40:23.811712	2020-10-31 00:40:23.811712
2449	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 00:41:50.194344	2020-10-31 00:41:50.194344
2450	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 00:41:50.216036	2020-10-31 00:41:50.216036
2451	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 00:44:04.767123	2020-10-31 00:44:04.767123
2452	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 00:44:05.528509	2020-10-31 00:44:05.528509
2453	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 00:48:56.358418	2020-10-31 00:48:56.358418
2454	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 00:48:57.957395	2020-10-31 00:48:57.957395
2455	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 00:52:24.893739	2020-10-31 00:52:24.893739
2456	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 00:52:24.9114	2020-10-31 00:52:24.9114
2457	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 00:53:08.061462	2020-10-31 00:53:08.061462
2458	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 00:53:08.350889	2020-10-31 00:53:08.350889
2459	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 00:55:45.804954	2020-10-31 00:55:45.804954
2460	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 00:55:46.289391	2020-10-31 00:55:46.289391
2461	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 00:57:33.269054	2020-10-31 00:57:33.269054
2462	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 00:57:33.528667	2020-10-31 00:57:33.528667
2463	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 00:58:47.775259	2020-10-31 00:58:47.775259
2464	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 00:58:47.791455	2020-10-31 00:58:47.791455
2465	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 01:00:49.472781	2020-10-31 01:00:49.472781
2466	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 01:00:49.489918	2020-10-31 01:00:49.489918
2467	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 01:02:53.074759	2020-10-31 01:02:53.074759
2468	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 01:02:53.380098	2020-10-31 01:02:53.380098
2469	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 01:04:51.434103	2020-10-31 01:04:51.434103
2470	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 01:04:51.719008	2020-10-31 01:04:51.719008
2471	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 01:07:45.986589	2020-10-31 01:07:45.986589
2472	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 01:07:46.138206	2020-10-31 01:07:46.138206
2473	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 01:10:13.404192	2020-10-31 01:10:13.404192
2474	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 01:10:13.421345	2020-10-31 01:10:13.421345
2475	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 01:11:11.955788	2020-10-31 01:11:11.955788
2476	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 01:11:11.974917	2020-10-31 01:11:11.974917
2477	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 01:12:10.938973	2020-10-31 01:12:10.938973
2478	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 01:12:10.961205	2020-10-31 01:12:10.961205
2479	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 01:22:04.504799	2020-10-31 01:22:04.504799
2480	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 01:22:04.51973	2020-10-31 01:22:04.51973
2481	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 01:22:35.630694	2020-10-31 01:22:35.630694
2482	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 01:22:35.654682	2020-10-31 01:22:35.654682
2483	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 01:28:32.99365	2020-10-31 01:28:32.99365
2484	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 01:28:33.016321	2020-10-31 01:28:33.016321
2485	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 01:28:51.46154	2020-10-31 01:28:51.46154
2486	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 01:28:51.484295	2020-10-31 01:28:51.484295
2487	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 01:33:01.775799	2020-10-31 01:33:01.775799
2488	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 01:33:02.962125	2020-10-31 01:33:02.962125
2489	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 01:34:51.016866	2020-10-31 01:34:51.016866
2490	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 01:34:51.037774	2020-10-31 01:34:51.037774
2491	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 01:39:13.884733	2020-10-31 01:39:13.884733
2492	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 01:39:13.902596	2020-10-31 01:39:13.902596
2493	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 01:40:05.63322	2020-10-31 01:40:05.63322
2494	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 01:40:05.647023	2020-10-31 01:40:05.647023
2495	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 01:41:02.18448	2020-10-31 01:41:02.18448
2496	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 01:41:02.200129	2020-10-31 01:41:02.200129
2497	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 01:43:39.371746	2020-10-31 01:43:39.371746
2498	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 01:43:39.384003	2020-10-31 01:43:39.384003
2499	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 01:44:31.132363	2020-10-31 01:44:31.132363
2500	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 01:44:31.154182	2020-10-31 01:44:31.154182
2501	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 01:45:26.244604	2020-10-31 01:45:26.244604
2502	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 01:45:26.257363	2020-10-31 01:45:26.257363
2503	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 01:48:09.730636	2020-10-31 01:48:09.730636
2504	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 01:48:09.749842	2020-10-31 01:48:09.749842
2505	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 01:50:16.787577	2020-10-31 01:50:16.787577
2506	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 01:50:16.80703	2020-10-31 01:50:16.80703
2507	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 01:52:27.067138	2020-10-31 01:52:27.067138
2508	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 01:52:27.632307	2020-10-31 01:52:27.632307
2509	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 01:52:36.927542	2020-10-31 01:52:36.927542
2510	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 01:52:36.931453	2020-10-31 01:52:36.931453
2511	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 01:53:04.466655	2020-10-31 01:53:04.466655
2512	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 01:53:04.483024	2020-10-31 01:53:04.483024
2513	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 01:54:10.094209	2020-10-31 01:54:10.094209
2514	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 01:54:11.274145	2020-10-31 01:54:11.274145
2515	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 01:56:02.767105	2020-10-31 01:56:02.767105
2516	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 01:56:04.081376	2020-10-31 01:56:04.081376
2517	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 01:56:37.761612	2020-10-31 01:56:37.761612
2518	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 01:56:37.778442	2020-10-31 01:56:37.778442
2519	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 01:58:14.748368	2020-10-31 01:58:14.748368
2520	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 01:58:14.975626	2020-10-31 01:58:14.975626
2521	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 02:03:25.586483	2020-10-31 02:03:25.586483
2522	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 02:03:25.592212	2020-10-31 02:03:25.592212
2523	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 02:16:38.424599	2020-10-31 02:16:38.424599
2524	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 02:16:38.445918	2020-10-31 02:16:38.445918
2525	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 02:29:24.914867	2020-10-31 02:29:24.914867
2526	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 02:29:24.930435	2020-10-31 02:29:24.930435
2527	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 02:29:52.229666	2020-10-31 02:29:52.229666
2528	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 02:29:52.248692	2020-10-31 02:29:52.248692
2529	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 02:33:50.19045	2020-10-31 02:33:50.19045
2530	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 02:33:50.211973	2020-10-31 02:33:50.211973
2531	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 02:45:08.162752	2020-10-31 02:45:08.162752
2532	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 02:45:08.18352	2020-10-31 02:45:08.18352
2533	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 02:45:38.220212	2020-10-31 02:45:38.220212
2534	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 02:45:38.235158	2020-10-31 02:45:38.235158
2535	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 02:51:16.673649	2020-10-31 02:51:16.673649
2536	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 02:51:17.479734	2020-10-31 02:51:17.479734
2537	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 03:00:45.553702	2020-10-31 03:00:45.553702
2538	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 03:00:45.569958	2020-10-31 03:00:45.569958
2539	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 03:03:45.940827	2020-10-31 03:03:45.940827
2540	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 03:03:45.963398	2020-10-31 03:03:45.963398
2541	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 03:09:32.500113	2020-10-31 03:09:32.500113
2542	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 03:09:32.516106	2020-10-31 03:09:32.516106
2543	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 03:12:11.064517	2020-10-31 03:12:11.064517
2544	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 03:12:11.083131	2020-10-31 03:12:11.083131
2545	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 03:15:38.267341	2020-10-31 03:15:38.267341
2546	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 03:15:38.286767	2020-10-31 03:15:38.286767
2547	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 03:20:32.51433	2020-10-31 03:20:32.51433
2548	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 03:20:32.536193	2020-10-31 03:20:32.536193
2549	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 03:24:36.154039	2020-10-31 03:24:36.154039
2550	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 03:24:36.17523	2020-10-31 03:24:36.17523
2551	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 03:28:06.085092	2020-10-31 03:28:06.085092
2552	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 03:28:06.103229	2020-10-31 03:28:06.103229
2553	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 03:29:15.687713	2020-10-31 03:29:15.687713
2554	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 03:29:15.707291	2020-10-31 03:29:15.707291
2555	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 03:32:21.977125	2020-10-31 03:32:21.977125
2556	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 03:32:22.273378	2020-10-31 03:32:22.273378
2557	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 03:33:15.54413	2020-10-31 03:33:15.54413
2558	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 03:33:15.562399	2020-10-31 03:33:15.562399
2559	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 03:36:15.709549	2020-10-31 03:36:15.709549
2560	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 03:36:15.730953	2020-10-31 03:36:15.730953
2561	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 03:36:49.253248	2020-10-31 03:36:49.253248
2562	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 03:36:49.265201	2020-10-31 03:36:49.265201
2563	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 03:38:35.471917	2020-10-31 03:38:35.471917
2564	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 03:38:35.488692	2020-10-31 03:38:35.488692
2565	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 03:41:24.279749	2020-10-31 03:41:24.279749
2566	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 03:41:24.290009	2020-10-31 03:41:24.290009
2567	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 03:45:30.324106	2020-10-31 03:45:30.324106
2568	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 03:45:30.345528	2020-10-31 03:45:30.345528
2569	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 03:50:39.058199	2020-10-31 03:50:39.058199
2570	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 03:50:39.0783	2020-10-31 03:50:39.0783
2571	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 03:50:45.590146	2020-10-31 03:50:45.590146
2572	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 03:50:45.595841	2020-10-31 03:50:45.595841
2573	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 03:51:54.089491	2020-10-31 03:51:54.089491
2574	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 03:51:54.108488	2020-10-31 03:51:54.108488
2575	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 03:53:26.112461	2020-10-31 03:53:26.112461
2576	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 03:53:26.130992	2020-10-31 03:53:26.130992
2577	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 03:55:09.136461	2020-10-31 03:55:09.136461
2578	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 03:55:09.217749	2020-10-31 03:55:09.217749
2579	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 03:56:40.606758	2020-10-31 03:56:40.606758
2580	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 03:56:40.848865	2020-10-31 03:56:40.848865
2581	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 03:57:27.697475	2020-10-31 03:57:27.697475
2582	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 03:57:27.719124	2020-10-31 03:57:27.719124
2583	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 03:57:54.231986	2020-10-31 03:57:54.231986
2584	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 03:57:54.25395	2020-10-31 03:57:54.25395
2585	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 03:59:19.101957	2020-10-31 03:59:19.101957
2586	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 03:59:19.116867	2020-10-31 03:59:19.116867
2587	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 04:05:05.469785	2020-10-31 04:05:05.469785
2588	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 04:05:06.527405	2020-10-31 04:05:06.527405
2589	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 04:07:42.449586	2020-10-31 04:07:42.449586
2590	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 04:07:42.470114	2020-10-31 04:07:42.470114
2591	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 04:14:38.718356	2020-10-31 04:14:38.718356
2592	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 04:14:38.743232	2020-10-31 04:14:38.743232
2593	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 04:15:08.243612	2020-10-31 04:15:08.243612
2594	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 04:15:08.261547	2020-10-31 04:15:08.261547
2595	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 04:16:06.334973	2020-10-31 04:16:06.334973
2596	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 04:16:06.356675	2020-10-31 04:16:06.356675
2597	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 04:18:45.370976	2020-10-31 04:18:45.370976
2598	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 04:18:45.382037	2020-10-31 04:18:45.382037
2599	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 04:20:33.97171	2020-10-31 04:20:33.97171
2600	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 04:20:35.503271	2020-10-31 04:20:35.503271
2601	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 04:21:25.29033	2020-10-31 04:21:25.29033
2602	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 04:21:25.308534	2020-10-31 04:21:25.308534
2603	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 04:21:41.838473	2020-10-31 04:21:41.838473
2604	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 04:21:41.858518	2020-10-31 04:21:41.858518
2605	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 04:23:15.596113	2020-10-31 04:23:15.596113
2606	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 04:23:15.613881	2020-10-31 04:23:15.613881
2607	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 04:27:49.570558	2020-10-31 04:27:49.570558
2608	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 04:27:49.587496	2020-10-31 04:27:49.587496
2609	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 04:30:07.825272	2020-10-31 04:30:07.825272
2610	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 04:30:08.045349	2020-10-31 04:30:08.045349
2611	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 04:33:00.94818	2020-10-31 04:33:00.94818
2612	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 04:33:00.963414	2020-10-31 04:33:00.963414
2613	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 04:38:00.69367	2020-10-31 04:38:00.69367
2614	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 04:38:00.952165	2020-10-31 04:38:00.952165
2615	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 04:39:49.019874	2020-10-31 04:39:49.019874
2616	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 04:39:49.230361	2020-10-31 04:39:49.230361
2617	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 04:41:26.132839	2020-10-31 04:41:26.132839
2618	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 04:41:26.150069	2020-10-31 04:41:26.150069
2619	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 04:42:52.613775	2020-10-31 04:42:52.613775
2620	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 04:42:52.83808	2020-10-31 04:42:52.83808
2621	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 04:48:40.18341	2020-10-31 04:48:40.18341
2622	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 04:48:40.566235	2020-10-31 04:48:40.566235
2623	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 04:50:21.982226	2020-10-31 04:50:21.982226
2624	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 04:50:22.357822	2020-10-31 04:50:22.357822
2625	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 04:52:00.539695	2020-10-31 04:52:00.539695
2626	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 04:52:00.560645	2020-10-31 04:52:00.560645
2627	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 04:54:25.600729	2020-10-31 04:54:25.600729
2628	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 04:54:25.61882	2020-10-31 04:54:25.61882
2629	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 04:55:29.971265	2020-10-31 04:55:29.971265
2630	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 04:55:29.989152	2020-10-31 04:55:29.989152
2631	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 04:59:47.129613	2020-10-31 04:59:47.129613
2632	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 04:59:47.144242	2020-10-31 04:59:47.144242
2633	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 04:59:55.519437	2020-10-31 04:59:55.519437
2634	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 04:59:55.537274	2020-10-31 04:59:55.537274
2635	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 05:03:03.857352	2020-10-31 05:03:03.857352
2636	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 05:03:03.872692	2020-10-31 05:03:03.872692
2637	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 05:04:35.656326	2020-10-31 05:04:35.656326
2638	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 05:04:35.677864	2020-10-31 05:04:35.677864
2639	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 05:06:08.426142	2020-10-31 05:06:08.426142
2640	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 05:06:08.44169	2020-10-31 05:06:08.44169
2641	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 05:06:33.369644	2020-10-31 05:06:33.369644
2642	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 05:06:33.426436	2020-10-31 05:06:33.426436
2643	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 05:07:31.002506	2020-10-31 05:07:31.002506
2644	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 05:07:31.021417	2020-10-31 05:07:31.021417
2645	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 05:09:22.798726	2020-10-31 05:09:22.798726
2646	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 05:09:22.818961	2020-10-31 05:09:22.818961
2647	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 05:11:58.311487	2020-10-31 05:11:58.311487
2648	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 05:11:58.462747	2020-10-31 05:11:58.462747
2649	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 05:18:35.66827	2020-10-31 05:18:35.66827
2650	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 05:18:35.700057	2020-10-31 05:18:35.700057
2651	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 05:20:04.432759	2020-10-31 05:20:04.432759
2652	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 05:20:04.455052	2020-10-31 05:20:04.455052
2653	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 05:21:10.286843	2020-10-31 05:21:10.286843
2654	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 05:21:10.658854	2020-10-31 05:21:10.658854
2655	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 05:22:24.026063	2020-10-31 05:22:24.026063
2656	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 05:22:24.046053	2020-10-31 05:22:24.046053
2657	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 05:26:56.507397	2020-10-31 05:26:56.507397
2658	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 05:26:56.526943	2020-10-31 05:26:56.526943
2659	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 05:29:49.465198	2020-10-31 05:29:49.465198
2660	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 05:29:49.471066	2020-10-31 05:29:49.471066
2661	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 05:30:18.67308	2020-10-31 05:30:18.67308
2662	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 05:30:18.689479	2020-10-31 05:30:18.689479
2663	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 05:30:47.849393	2020-10-31 05:30:47.849393
2664	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 05:30:47.857301	2020-10-31 05:30:47.857301
2665	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 05:32:30.71844	2020-10-31 05:32:30.71844
2666	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 05:32:31.205475	2020-10-31 05:32:31.205475
2667	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 05:33:07.690419	2020-10-31 05:33:07.690419
2668	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 05:33:07.994425	2020-10-31 05:33:07.994425
2669	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 05:33:22.789659	2020-10-31 05:33:22.789659
2670	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 05:33:22.808314	2020-10-31 05:33:22.808314
2671	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 05:34:18.937966	2020-10-31 05:34:18.937966
2672	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 05:34:18.952226	2020-10-31 05:34:18.952226
2673	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 05:34:34.326386	2020-10-31 05:34:34.326386
2674	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 05:34:34.337391	2020-10-31 05:34:34.337391
2675	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 05:35:59.701233	2020-10-31 05:35:59.701233
2676	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 05:36:00.655994	2020-10-31 05:36:00.655994
2677	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 05:38:10.852305	2020-10-31 05:38:10.852305
2678	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 05:38:10.87449	2020-10-31 05:38:10.87449
2679	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 05:40:09.424707	2020-10-31 05:40:09.424707
2680	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 05:40:09.439267	2020-10-31 05:40:09.439267
2681	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 05:43:28.492288	2020-10-31 05:43:28.492288
2682	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 05:43:28.499726	2020-10-31 05:43:28.499726
2683	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 05:45:45.038412	2020-10-31 05:45:45.038412
2684	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 05:45:45.058317	2020-10-31 05:45:45.058317
2685	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 05:47:10.870611	2020-10-31 05:47:10.870611
2686	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 05:47:10.88795	2020-10-31 05:47:10.88795
2687	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 05:49:11.913584	2020-10-31 05:49:11.913584
2688	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 05:49:12.003785	2020-10-31 05:49:12.003785
2689	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 05:50:31.191123	2020-10-31 05:50:31.191123
2690	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 05:50:31.207466	2020-10-31 05:50:31.207466
2691	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 05:51:11.319749	2020-10-31 05:51:11.319749
2692	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 05:51:11.334724	2020-10-31 05:51:11.334724
2693	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 05:52:55.818651	2020-10-31 05:52:55.818651
2694	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 05:52:56.332297	2020-10-31 05:52:56.332297
2695	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 05:55:16.53763	2020-10-31 05:55:16.53763
2696	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 05:55:16.557242	2020-10-31 05:55:16.557242
2697	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 05:57:02.24626	2020-10-31 05:57:02.24626
2698	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 05:57:03.215205	2020-10-31 05:57:03.215205
2699	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 06:00:03.8441	2020-10-31 06:00:03.8441
2700	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 06:00:03.972902	2020-10-31 06:00:03.972902
2701	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 06:01:26.750524	2020-10-31 06:01:26.750524
2702	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 06:01:26.769246	2020-10-31 06:01:26.769246
2703	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 06:03:18.574986	2020-10-31 06:03:18.574986
2704	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 06:03:19.009941	2020-10-31 06:03:19.009941
2705	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 06:04:35.137461	2020-10-31 06:04:35.137461
2706	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 06:04:35.190272	2020-10-31 06:04:35.190272
2707	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 06:07:34.350146	2020-10-31 06:07:34.350146
2708	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 06:07:34.798154	2020-10-31 06:07:34.798154
2709	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 06:08:28.016462	2020-10-31 06:08:28.016462
2710	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 06:08:28.034394	2020-10-31 06:08:28.034394
2711	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 06:09:48.165566	2020-10-31 06:09:48.165566
2712	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 06:09:48.180401	2020-10-31 06:09:48.180401
2713	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 06:12:04.578295	2020-10-31 06:12:04.578295
2714	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 06:12:04.712975	2020-10-31 06:12:04.712975
2715	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 06:16:59.4805	2020-10-31 06:16:59.4805
2716	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 06:16:59.49634	2020-10-31 06:16:59.49634
2717	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 06:17:48.308241	2020-10-31 06:17:48.308241
2718	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 06:17:48.323884	2020-10-31 06:17:48.323884
2719	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 06:19:36.455786	2020-10-31 06:19:36.455786
2720	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 06:19:37.440454	2020-10-31 06:19:37.440454
2721	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 06:21:21.021428	2020-10-31 06:21:21.021428
2722	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 06:21:21.211148	2020-10-31 06:21:21.211148
2723	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 06:36:55.286969	2020-10-31 06:36:55.286969
2724	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 06:36:55.542673	2020-10-31 06:36:55.542673
2725	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 06:40:21.864873	2020-10-31 06:40:21.864873
2726	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 06:40:22.284646	2020-10-31 06:40:22.284646
2727	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 06:42:07.169788	2020-10-31 06:42:07.169788
2728	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 06:42:07.197175	2020-10-31 06:42:07.197175
2729	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 06:42:58.755124	2020-10-31 06:42:58.755124
2730	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 06:43:00.442708	2020-10-31 06:43:00.442708
2731	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 06:47:18.904715	2020-10-31 06:47:18.904715
2732	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 06:47:18.922602	2020-10-31 06:47:18.922602
2733	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 06:52:51.23176	2020-10-31 06:52:51.23176
2734	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 06:52:51.249731	2020-10-31 06:52:51.249731
2735	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 06:56:01.024869	2020-10-31 06:56:01.024869
2736	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 06:56:01.043696	2020-10-31 06:56:01.043696
2737	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 07:04:41.217867	2020-10-31 07:04:41.217867
2738	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 07:04:41.964609	2020-10-31 07:04:41.964609
2739	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 07:05:58.747612	2020-10-31 07:05:58.747612
2740	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 07:05:58.760374	2020-10-31 07:05:58.760374
2741	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 07:07:03.269005	2020-10-31 07:07:03.269005
2742	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 07:07:04.120677	2020-10-31 07:07:04.120677
2743	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 07:13:46.462006	2020-10-31 07:13:46.462006
2744	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 07:13:46.478131	2020-10-31 07:13:46.478131
2745	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 07:14:13.441074	2020-10-31 07:14:13.441074
2746	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 07:14:13.463545	2020-10-31 07:14:13.463545
2747	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 07:15:07.343235	2020-10-31 07:15:07.343235
2748	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 07:15:07.366248	2020-10-31 07:15:07.366248
2749	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 07:17:28.633974	2020-10-31 07:17:28.633974
2750	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 07:17:28.648997	2020-10-31 07:17:28.648997
2751	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 07:18:44.432707	2020-10-31 07:18:44.432707
2752	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 07:18:44.453322	2020-10-31 07:18:44.453322
2753	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 07:19:20.61643	2020-10-31 07:19:20.61643
2754	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 07:19:20.7087	2020-10-31 07:19:20.7087
2755	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 07:21:27.105341	2020-10-31 07:21:27.105341
2756	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 07:21:27.413221	2020-10-31 07:21:27.413221
2757	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 07:26:15.080752	2020-10-31 07:26:15.080752
2758	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 07:26:16.372683	2020-10-31 07:26:16.372683
2759	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 07:26:36.043371	2020-10-31 07:26:36.043371
2760	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 07:26:36.06288	2020-10-31 07:26:36.06288
2761	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 07:27:24.489028	2020-10-31 07:27:24.489028
2762	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 07:27:24.507268	2020-10-31 07:27:24.507268
2763	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 07:27:40.977101	2020-10-31 07:27:40.977101
2764	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 07:27:40.999237	2020-10-31 07:27:40.999237
2765	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 07:28:15.374921	2020-10-31 07:28:15.374921
2766	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 07:28:15.393502	2020-10-31 07:28:15.393502
2767	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 07:29:20.650487	2020-10-31 07:29:20.650487
2768	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 07:29:20.672407	2020-10-31 07:29:20.672407
2769	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 07:42:22.174557	2020-10-31 07:42:22.174557
2770	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 07:42:22.199285	2020-10-31 07:42:22.199285
2771	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 07:45:29.519838	2020-10-31 07:45:29.519838
2772	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 07:45:29.663511	2020-10-31 07:45:29.663511
2773	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 07:48:03.496619	2020-10-31 07:48:03.496619
2774	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 07:48:03.518584	2020-10-31 07:48:03.518584
2775	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 07:49:10.679311	2020-10-31 07:49:10.679311
2776	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 07:49:10.6985	2020-10-31 07:49:10.6985
2777	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 07:50:22.9204	2020-10-31 07:50:22.9204
2778	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 07:50:23.572241	2020-10-31 07:50:23.572241
2779	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 07:51:12.539979	2020-10-31 07:51:12.539979
2780	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 07:51:12.555381	2020-10-31 07:51:12.555381
2781	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 08:01:40.224592	2020-10-31 08:01:40.224592
2782	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 08:01:40.987065	2020-10-31 08:01:40.987065
2783	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 08:03:43.105391	2020-10-31 08:03:43.105391
2784	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 08:03:43.778545	2020-10-31 08:03:43.778545
2785	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 08:05:34.664911	2020-10-31 08:05:34.664911
2786	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 08:05:34.687037	2020-10-31 08:05:34.687037
2787	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 08:44:50.834108	2020-10-31 08:44:50.834108
2788	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 08:44:50.851608	2020-10-31 08:44:50.851608
2789	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 08:49:02.286055	2020-10-31 08:49:02.286055
2790	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 08:49:03.322022	2020-10-31 08:49:03.322022
2791	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 08:50:10.498901	2020-10-31 08:50:10.498901
2792	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 08:50:10.594885	2020-10-31 08:50:10.594885
2793	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-10-31 09:18:52.50589	2020-10-31 09:18:52.50589
2794	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-10-31 09:18:52.523035	2020-10-31 09:18:52.523035
2795	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-03 19:11:22.433131	2020-11-03 19:11:22.433131
2796	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-03 19:11:22.758349	2020-11-03 19:11:22.758349
2797	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-03 19:11:32.239208	2020-11-03 19:11:32.239208
2798	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-03 19:11:32.27446	2020-11-03 19:11:32.27446
2799	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-03 19:28:47.007038	2020-11-03 19:28:47.007038
2800	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-03 19:28:47.034864	2020-11-03 19:28:47.034864
2801	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-03 19:45:34.253432	2020-11-03 19:45:34.253432
2802	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-03 19:45:34.285509	2020-11-03 19:45:34.285509
2803	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-04 19:17:00.986153	2020-11-04 19:17:00.986153
2804	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-04 19:17:01.186598	2020-11-04 19:17:01.186598
2805	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-04 19:23:34.053689	2020-11-04 19:23:34.053689
2806	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-04 19:23:34.076208	2020-11-04 19:23:34.076208
2807	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-04 19:25:32.516787	2020-11-04 19:25:32.516787
2808	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-04 19:25:32.533404	2020-11-04 19:25:32.533404
2809	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-04 19:27:29.922322	2020-11-04 19:27:29.922322
2810	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-04 19:27:29.998925	2020-11-04 19:27:29.998925
2811	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-04 19:28:15.278329	2020-11-04 19:28:15.278329
2812	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-04 19:28:16.773952	2020-11-04 19:28:16.773952
2813	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-04 19:30:25.739059	2020-11-04 19:30:25.739059
2814	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-04 19:30:26.375533	2020-11-04 19:30:26.375533
2815	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-04 19:31:08.236351	2020-11-04 19:31:08.236351
2816	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-04 19:31:08.261871	2020-11-04 19:31:08.261871
2817	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-04 19:37:05.032826	2020-11-04 19:37:05.032826
2818	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-04 19:37:05.041247	2020-11-04 19:37:05.041247
2819	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-04 19:38:39.869327	2020-11-04 19:38:39.869327
2820	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-04 19:38:41.4611	2020-11-04 19:38:41.4611
2821	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-04 19:42:26.113507	2020-11-04 19:42:26.113507
2822	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-04 19:42:26.126927	2020-11-04 19:42:26.126927
2823	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-04 19:51:43.667915	2020-11-04 19:51:43.667915
2824	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-04 19:51:43.685053	2020-11-04 19:51:43.685053
2825	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-05 00:11:28.95707	2020-11-05 00:11:28.95707
2826	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-05 00:11:28.97387	2020-11-05 00:11:28.97387
2827	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-05 05:00:18.892405	2020-11-05 05:00:18.892405
2828	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-05 05:00:19.331609	2020-11-05 05:00:19.331609
2829	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-05 05:09:27.453788	2020-11-05 05:09:27.453788
2830	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-05 05:09:27.570485	2020-11-05 05:09:27.570485
2831	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-05 05:15:18.217537	2020-11-05 05:15:18.217537
2832	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-05 05:15:18.278833	2020-11-05 05:15:18.278833
2833	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-05 05:17:00.031047	2020-11-05 05:17:00.031047
2834	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-05 05:17:01.241506	2020-11-05 05:17:01.241506
2835	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-05 05:17:55.138707	2020-11-05 05:17:55.138707
2836	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-05 05:17:55.156203	2020-11-05 05:17:55.156203
2837	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-05 05:19:47.64457	2020-11-05 05:19:47.64457
2838	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-05 05:19:47.674182	2020-11-05 05:19:47.674182
2839	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-05 05:22:00.417991	2020-11-05 05:22:00.417991
2840	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-05 05:22:00.437476	2020-11-05 05:22:00.437476
2841	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-05 05:23:56.685069	2020-11-05 05:23:56.685069
2842	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-05 05:23:56.702735	2020-11-05 05:23:56.702735
2843	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-05 05:30:19.26872	2020-11-05 05:30:19.26872
2844	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-05 05:30:19.283662	2020-11-05 05:30:19.283662
2845	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-05 05:31:59.22112	2020-11-05 05:31:59.22112
2846	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-05 05:31:59.239679	2020-11-05 05:31:59.239679
2847	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-05 05:33:43.468253	2020-11-05 05:33:43.468253
2848	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-05 05:33:43.487547	2020-11-05 05:33:43.487547
2849	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-05 05:48:14.221543	2020-11-05 05:48:14.221543
2850	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-05 05:48:14.239296	2020-11-05 05:48:14.239296
2851	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-05 05:53:30.230009	2020-11-05 05:53:30.230009
2852	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-05 05:53:30.249867	2020-11-05 05:53:30.249867
2853	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-05 06:01:00.317927	2020-11-05 06:01:00.317927
2854	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-05 06:01:00.335318	2020-11-05 06:01:00.335318
2855	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-05 18:44:13.983826	2020-11-05 18:44:13.983826
2856	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-05 18:44:14.210836	2020-11-05 18:44:14.210836
2857	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-05 19:12:05.254443	2020-11-05 19:12:05.254443
2858	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-05 19:12:05.265502	2020-11-05 19:12:05.265502
2859	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-05 19:14:32.16898	2020-11-05 19:14:32.16898
2860	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-05 19:14:32.190906	2020-11-05 19:14:32.190906
2861	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-05 19:39:45.260276	2020-11-05 19:39:45.260276
2862	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-05 19:39:45.329697	2020-11-05 19:39:45.329697
2863	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-05 21:48:47.952751	2020-11-05 21:48:47.952751
2864	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-05 21:48:49.343415	2020-11-05 21:48:49.343415
2865	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-05 21:48:57.232214	2020-11-05 21:48:57.232214
2866	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-05 21:48:57.253081	2020-11-05 21:48:57.253081
2867	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-05 21:49:49.675693	2020-11-05 21:49:49.675693
2868	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-05 21:49:49.832164	2020-11-05 21:49:49.832164
2869	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-05 21:51:35.119412	2020-11-05 21:51:35.119412
2870	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-05 21:51:35.13753	2020-11-05 21:51:35.13753
2871	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-06 00:53:13.933354	2020-11-06 00:53:13.933354
2872	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-06 00:53:13.947348	2020-11-06 00:53:13.947348
2873	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-06 01:32:14.906718	2020-11-06 01:32:14.906718
2874	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-06 01:32:14.919992	2020-11-06 01:32:14.919992
2875	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-06 03:21:45.956792	2020-11-06 03:21:45.956792
2876	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-06 03:21:45.97529	2020-11-06 03:21:45.97529
2877	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-07 02:21:55.636732	2020-11-07 02:21:55.636732
2878	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-07 02:21:55.872597	2020-11-07 02:21:55.872597
2879	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-07 04:23:38.583187	2020-11-07 04:23:38.583187
2880	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-07 04:23:38.602011	2020-11-07 04:23:38.602011
2881	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-07 04:30:12.674998	2020-11-07 04:30:12.674998
2882	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-07 04:30:13.027265	2020-11-07 04:30:13.027265
2883	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-07 06:39:59.440746	2020-11-07 06:39:59.440746
2884	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-07 06:39:59.463205	2020-11-07 06:39:59.463205
2885	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-07 07:30:19.924287	2020-11-07 07:30:19.924287
2886	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-07 07:30:19.940516	2020-11-07 07:30:19.940516
2887	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-07 07:57:51.360603	2020-11-07 07:57:51.360603
2888	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-07 07:57:51.380621	2020-11-07 07:57:51.380621
2889	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-07 08:46:09.769685	2020-11-07 08:46:09.769685
2890	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-07 08:46:09.776737	2020-11-07 08:46:09.776737
2891	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-07 08:46:28.788138	2020-11-07 08:46:28.788138
2892	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-07 08:46:28.811757	2020-11-07 08:46:28.811757
2893	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-07 23:36:09.349624	2020-11-07 23:36:09.349624
2894	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-07 23:36:09.585834	2020-11-07 23:36:09.585834
2895	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-07 23:43:51.701077	2020-11-07 23:43:51.701077
2896	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-07 23:43:52.903417	2020-11-07 23:43:52.903417
2897	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-07 23:51:24.53221	2020-11-07 23:51:24.53221
2898	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-07 23:51:24.559524	2020-11-07 23:51:24.559524
2899	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-08 00:22:26.134724	2020-11-08 00:22:26.134724
2900	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-08 00:22:26.15094	2020-11-08 00:22:26.15094
2901	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-08 00:38:38.833239	2020-11-08 00:38:38.833239
2902	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-08 00:38:38.85325	2020-11-08 00:38:38.85325
2903	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-08 00:54:31.137979	2020-11-08 00:54:31.137979
2904	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-08 00:54:31.154536	2020-11-08 00:54:31.154536
2905	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-08 01:09:04.232181	2020-11-08 01:09:04.232181
2906	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-08 01:09:04.250805	2020-11-08 01:09:04.250805
2907	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-08 05:44:07.467514	2020-11-08 05:44:07.467514
2908	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-08 05:44:07.490417	2020-11-08 05:44:07.490417
2909	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-09 19:42:46.495303	2020-11-09 19:42:46.495303
2910	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-09 19:42:46.729115	2020-11-09 19:42:46.729115
2911	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-09 20:06:44.019766	2020-11-09 20:06:44.019766
2912	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-09 20:06:44.041069	2020-11-09 20:06:44.041069
2913	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-09 20:12:42.057281	2020-11-09 20:12:42.057281
2914	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-09 20:12:42.161962	2020-11-09 20:12:42.161962
2915	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-09 20:13:21.425079	2020-11-09 20:13:21.425079
2916	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-09 20:13:21.439644	2020-11-09 20:13:21.439644
2917	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-10 01:23:27.530005	2020-11-10 01:23:27.530005
2918	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-10 01:23:27.542993	2020-11-10 01:23:27.542993
2919	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-10 19:09:52.601477	2020-11-10 19:09:52.601477
2920	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-10 19:09:53.576785	2020-11-10 19:09:53.576785
2921	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-10 19:13:04.585867	2020-11-10 19:13:04.585867
2922	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-10 19:13:04.599817	2020-11-10 19:13:04.599817
2923	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-10 19:17:11.687511	2020-11-10 19:17:11.687511
2924	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-10 19:17:11.706333	2020-11-10 19:17:11.706333
2925	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-10 19:21:13.300596	2020-11-10 19:21:13.300596
2926	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-10 19:21:13.320415	2020-11-10 19:21:13.320415
2927	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-10 19:35:30.534223	2020-11-10 19:35:30.534223
2928	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-10 19:35:30.545539	2020-11-10 19:35:30.545539
2929	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-10 19:37:59.534127	2020-11-10 19:37:59.534127
2930	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-10 19:38:00.680475	2020-11-10 19:38:00.680475
2931	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-10 19:38:25.191514	2020-11-10 19:38:25.191514
2932	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-10 19:38:25.214268	2020-11-10 19:38:25.214268
2933	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-10 19:38:46.819644	2020-11-10 19:38:46.819644
2934	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-10 19:38:47.343092	2020-11-10 19:38:47.343092
2935	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-10 19:44:04.877178	2020-11-10 19:44:04.877178
2936	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-10 19:44:04.900882	2020-11-10 19:44:04.900882
2937	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-10 19:45:44.932543	2020-11-10 19:45:44.932543
2938	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-10 19:45:45.223878	2020-11-10 19:45:45.223878
2939	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-11 00:12:37.711555	2020-11-11 00:12:37.711555
2940	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-11 00:12:37.728249	2020-11-11 00:12:37.728249
2941	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-11 00:14:13.773684	2020-11-11 00:14:13.773684
2942	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-11 00:14:13.792441	2020-11-11 00:14:13.792441
2943	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-11 00:15:17.231043	2020-11-11 00:15:17.231043
2944	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-11 00:15:17.246546	2020-11-11 00:15:17.246546
2945	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-11 00:17:36.277001	2020-11-11 00:17:36.277001
2946	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-11 00:17:36.458037	2020-11-11 00:17:36.458037
2947	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-11 00:32:43.045446	2020-11-11 00:32:43.045446
2948	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-11 00:32:43.093876	2020-11-11 00:32:43.093876
2949	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-11 00:35:16.382979	2020-11-11 00:35:16.382979
2950	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-11 00:35:16.392321	2020-11-11 00:35:16.392321
2951	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-11 00:36:25.253019	2020-11-11 00:36:25.253019
2952	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-11 00:36:25.271458	2020-11-11 00:36:25.271458
2953	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-11 00:42:22.877302	2020-11-11 00:42:22.877302
2954	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-11 00:42:22.892568	2020-11-11 00:42:22.892568
2955	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-11 00:43:25.744282	2020-11-11 00:43:25.744282
2956	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-11 00:43:25.763006	2020-11-11 00:43:25.763006
2957	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-11 01:02:26.823748	2020-11-11 01:02:26.823748
2958	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-11 01:02:26.838634	2020-11-11 01:02:26.838634
2959	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-11 01:03:27.03162	2020-11-11 01:03:27.03162
2960	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-11 01:03:27.184174	2020-11-11 01:03:27.184174
2961	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-11 01:05:33.6319	2020-11-11 01:05:33.6319
2962	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-11 01:05:33.648837	2020-11-11 01:05:33.648837
2963	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-11 02:15:34.617154	2020-11-11 02:15:34.617154
2964	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-11 02:15:34.630832	2020-11-11 02:15:34.630832
2965	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-11 02:17:54.455566	2020-11-11 02:17:54.455566
2966	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-11 02:17:54.47246	2020-11-11 02:17:54.47246
2967	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-11 02:19:46.33548	2020-11-11 02:19:46.33548
2968	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-11 02:19:46.529498	2020-11-11 02:19:46.529498
2969	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-11 02:20:02.574308	2020-11-11 02:20:02.574308
2970	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-11 02:20:02.596883	2020-11-11 02:20:02.596883
2971	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-11 02:22:03.327748	2020-11-11 02:22:03.327748
2972	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-11 02:22:03.345785	2020-11-11 02:22:03.345785
2973	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-11 02:25:19.280962	2020-11-11 02:25:19.280962
2974	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-11 02:25:19.361143	2020-11-11 02:25:19.361143
2975	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-11 02:27:02.24965	2020-11-11 02:27:02.24965
2976	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-11 02:27:02.271839	2020-11-11 02:27:02.271839
2977	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-11 05:05:05.347646	2020-11-11 05:05:05.347646
2978	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-11 05:05:05.363764	2020-11-11 05:05:05.363764
2979	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-12 19:36:32.075478	2020-11-12 19:36:32.075478
2980	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-12 19:36:32.314442	2020-11-12 19:36:32.314442
2981	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-12 19:49:54.945769	2020-11-12 19:49:54.945769
2982	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-12 19:49:54.962765	2020-11-12 19:49:54.962765
2983	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-12 19:58:37.764174	2020-11-12 19:58:37.764174
2984	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-12 19:58:38.396657	2020-11-12 19:58:38.396657
2985	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-12 20:00:33.502295	2020-11-12 20:00:33.502295
2986	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-12 20:00:33.518065	2020-11-12 20:00:33.518065
2987	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-12 20:02:33.279196	2020-11-12 20:02:33.279196
2988	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-12 20:02:33.300644	2020-11-12 20:02:33.300644
2989	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-12 20:10:29.012658	2020-11-12 20:10:29.012658
2990	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-12 20:10:29.028997	2020-11-12 20:10:29.028997
2991	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-12 20:18:08.422558	2020-11-12 20:18:08.422558
2992	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-12 20:18:08.440027	2020-11-12 20:18:08.440027
2993	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-12 20:18:46.963493	2020-11-12 20:18:46.963493
2994	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-12 20:18:46.985527	2020-11-12 20:18:46.985527
2995	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-12 20:21:25.887152	2020-11-12 20:21:25.887152
2996	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-12 20:21:26.361869	2020-11-12 20:21:26.361869
2997	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-12 20:22:33.069239	2020-11-12 20:22:33.069239
2998	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-12 20:22:34.271424	2020-11-12 20:22:34.271424
2999	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-12 20:30:55.393689	2020-11-12 20:30:55.393689
3000	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-12 20:30:55.412636	2020-11-12 20:30:55.412636
3001	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-12 20:33:04.377724	2020-11-12 20:33:04.377724
3002	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-12 20:33:05.223454	2020-11-12 20:33:05.223454
3003	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-12 20:34:23.757057	2020-11-12 20:34:23.757057
3004	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-12 20:34:23.775463	2020-11-12 20:34:23.775463
3005	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-13 17:26:36.034888	2020-11-13 17:26:36.034888
3006	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-13 17:26:36.046496	2020-11-13 17:26:36.046496
3007	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-13 17:47:13.189787	2020-11-13 17:47:13.189787
3008	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-13 17:47:13.203854	2020-11-13 17:47:13.203854
3009	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-13 18:50:38.163734	2020-11-13 18:50:38.163734
3010	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-13 18:50:39.072326	2020-11-13 18:50:39.072326
3011	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-13 19:03:01.985097	2020-11-13 19:03:01.985097
3012	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-13 19:03:01.999275	2020-11-13 19:03:01.999275
3013	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-13 19:11:56.065026	2020-11-13 19:11:56.065026
3014	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-13 19:11:56.0812	2020-11-13 19:11:56.0812
3015	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-13 19:13:03.456954	2020-11-13 19:13:03.456954
3016	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-13 19:13:03.47675	2020-11-13 19:13:03.47675
3017	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-13 19:22:14.650331	2020-11-13 19:22:14.650331
3018	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-13 19:22:14.667998	2020-11-13 19:22:14.667998
3019	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-13 19:25:23.76309	2020-11-13 19:25:23.76309
3020	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-13 19:25:23.779479	2020-11-13 19:25:23.779479
3021	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-13 19:29:56.139494	2020-11-13 19:29:56.139494
3022	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-13 19:29:56.246035	2020-11-13 19:29:56.246035
3023	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-13 19:36:19.558824	2020-11-13 19:36:19.558824
3024	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-13 19:36:20.693886	2020-11-13 19:36:20.693886
3025	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-13 19:53:48.525106	2020-11-13 19:53:48.525106
3026	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-13 19:53:48.547608	2020-11-13 19:53:48.547608
3027	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-13 20:04:47.091703	2020-11-13 20:04:47.091703
3028	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-13 20:04:47.245265	2020-11-13 20:04:47.245265
3029	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-13 20:06:14.866451	2020-11-13 20:06:14.866451
3030	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-13 20:06:14.883709	2020-11-13 20:06:14.883709
3031	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-13 20:18:32.042686	2020-11-13 20:18:32.042686
3032	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-13 20:18:32.052766	2020-11-13 20:18:32.052766
3033	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-13 21:17:18.732973	2020-11-13 21:17:18.732973
3034	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-13 21:17:18.754287	2020-11-13 21:17:18.754287
3035	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-14 00:20:57.057144	2020-11-14 00:20:57.057144
3036	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-14 00:20:57.557046	2020-11-14 00:20:57.557046
3037	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-15 12:53:57.617544	2020-11-15 12:53:57.617544
3038	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-15 12:53:57.957518	2020-11-15 12:53:57.957518
3039	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-16 01:17:14.196923	2020-11-16 01:17:14.196923
3040	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-16 01:17:14.44934	2020-11-16 01:17:14.44934
3041	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-16 03:57:54.101785	2020-11-16 03:57:54.101785
3042	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-16 03:57:54.15631	2020-11-16 03:57:54.15631
3043	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-16 04:54:22.32601	2020-11-16 04:54:22.32601
3044	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-16 04:54:22.345491	2020-11-16 04:54:22.345491
3045	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-16 04:59:44.87215	2020-11-16 04:59:44.87215
3046	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-16 04:59:45.355464	2020-11-16 04:59:45.355464
3047	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-16 05:11:03.387369	2020-11-16 05:11:03.387369
3048	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-16 05:11:03.429782	2020-11-16 05:11:03.429782
3049	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-16 05:13:45.256865	2020-11-16 05:13:45.256865
3050	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-16 05:13:45.293229	2020-11-16 05:13:45.293229
3051	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-16 18:36:48.718487	2020-11-16 18:36:48.718487
3052	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-16 18:36:49.077596	2020-11-16 18:36:49.077596
3053	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-16 18:55:11.142731	2020-11-16 18:55:11.142731
3054	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-16 18:55:11.177709	2020-11-16 18:55:11.177709
3055	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-16 19:48:07.312494	2020-11-16 19:48:07.312494
3056	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-16 19:48:07.473957	2020-11-16 19:48:07.473957
3057	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-16 19:49:19.075393	2020-11-16 19:49:19.075393
3058	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-16 19:49:19.475297	2020-11-16 19:49:19.475297
3059	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-16 20:16:17.198562	2020-11-16 20:16:17.198562
3060	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-16 20:16:17.2124	2020-11-16 20:16:17.2124
3061	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-16 20:43:07.380624	2020-11-16 20:43:07.380624
3062	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-16 20:43:07.48504	2020-11-16 20:43:07.48504
3063	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-16 21:36:55.634735	2020-11-16 21:36:55.634735
3064	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-16 21:36:55.656444	2020-11-16 21:36:55.656444
3065	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-16 21:45:17.41205	2020-11-16 21:45:17.41205
3066	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-16 21:45:17.769685	2020-11-16 21:45:17.769685
3067	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-16 21:46:22.630084	2020-11-16 21:46:22.630084
3068	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-16 21:46:22.648235	2020-11-16 21:46:22.648235
3069	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-16 21:55:30.878843	2020-11-16 21:55:30.878843
3070	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-16 21:55:30.905576	2020-11-16 21:55:30.905576
3071	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-16 22:01:34.103064	2020-11-16 22:01:34.103064
3072	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-16 22:01:34.695516	2020-11-16 22:01:34.695516
3073	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-16 22:04:38.450109	2020-11-16 22:04:38.450109
3074	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-16 22:04:38.4713	2020-11-16 22:04:38.4713
3075	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-16 22:09:53.037665	2020-11-16 22:09:53.037665
3076	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-16 22:09:53.056616	2020-11-16 22:09:53.056616
3077	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-16 22:14:02.59497	2020-11-16 22:14:02.59497
3078	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-16 22:14:02.61317	2020-11-16 22:14:02.61317
3079	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-16 22:14:32.936876	2020-11-16 22:14:32.936876
3080	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-16 22:14:33.148596	2020-11-16 22:14:33.148596
3081	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-16 22:17:36.482632	2020-11-16 22:17:36.482632
3082	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-16 22:17:37.543097	2020-11-16 22:17:37.543097
3083	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-16 23:16:24.802192	2020-11-16 23:16:24.802192
3084	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-16 23:16:24.819349	2020-11-16 23:16:24.819349
3085	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-17 00:00:06.645025	2020-11-17 00:00:06.645025
3086	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-17 00:00:06.66636	2020-11-17 00:00:06.66636
3087	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-17 00:17:15.361187	2020-11-17 00:17:15.361187
3088	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-17 00:17:15.627255	2020-11-17 00:17:15.627255
3089	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-17 00:26:56.423244	2020-11-17 00:26:56.423244
3090	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-17 00:26:56.769914	2020-11-17 00:26:56.769914
3091	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-17 00:27:22.991618	2020-11-17 00:27:22.991618
3092	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-17 00:27:23.013081	2020-11-17 00:27:23.013081
3093	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-17 00:30:07.171287	2020-11-17 00:30:07.171287
3094	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-17 00:30:07.189723	2020-11-17 00:30:07.189723
3095	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-17 18:50:04.090939	2020-11-17 18:50:04.090939
3096	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-17 18:50:04.122517	2020-11-17 18:50:04.122517
3097	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-17 18:51:24.853642	2020-11-17 18:51:24.853642
3098	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-17 18:51:24.875189	2020-11-17 18:51:24.875189
3099	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-17 19:33:50.974735	2020-11-17 19:33:50.974735
3100	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-17 19:33:51.754232	2020-11-17 19:33:51.754232
3101	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-17 20:14:14.391261	2020-11-17 20:14:14.391261
3102	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-17 20:14:14.411242	2020-11-17 20:14:14.411242
3103	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-17 20:18:09.706634	2020-11-17 20:18:09.706634
3104	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-17 20:18:09.734088	2020-11-17 20:18:09.734088
3105	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-18 01:43:32.508474	2020-11-18 01:43:32.508474
3106	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-18 01:43:32.706819	2020-11-18 01:43:32.706819
3107	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-18 01:47:38.570355	2020-11-18 01:47:38.570355
3108	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-18 01:47:38.596487	2020-11-18 01:47:38.596487
3109	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-18 20:52:40.454117	2020-11-18 20:52:40.454117
3110	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-18 20:52:40.462623	2020-11-18 20:52:40.462623
3111	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-18 21:05:11.771714	2020-11-18 21:05:11.771714
3112	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-18 21:05:11.791975	2020-11-18 21:05:11.791975
3113	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-18 22:54:39.227515	2020-11-18 22:54:39.227515
3114	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-18 22:54:39.237567	2020-11-18 22:54:39.237567
3115	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-18 22:59:34.576899	2020-11-18 22:59:34.576899
3116	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-18 22:59:35.640112	2020-11-18 22:59:35.640112
3117	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-18 23:27:44.633877	2020-11-18 23:27:44.633877
3118	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-18 23:27:44.998025	2020-11-18 23:27:44.998025
3119	10	10	10	10	10	10	10	10	220		4.23000000000000043	8	2	14	4	4.53000000000000025	3.14000000000000012	3	3	4.29999999999999982	5	80	24	2	ASSAULT_RIFLE	4	SK-10	An AUG-PARA Assault Rifle	29	COMMON	augpara.yml	2020-11-18 23:29:23.310309	2020-11-18 23:29:23.310309
3120	80	40	13	5	50	25	10	1	75	PISTOL	20.5009990000000002	9	1	5	1	20	0	33	1	0	3	80	40	2	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	RARE	czp10.yml	2020-11-18 23:29:24.103896	2020-11-18 23:29:24.103896
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
95	$2a$06$n4j.CLTvTCzOoSB3KtNH9emZe6HDr/uWQl8MvrZSLL3qfLIb.1BIi	0	0	butcher	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	M	0	0	0	0	0	0	12	0	0	0	0	PC	0	0	0	0	2020-11-08 02:20:53.16572	0	2020-11-08 02:20:53.16572	8388736
84	$2a$06$GJd2eoqdWK6Ii9VgSRJeJuh06vWIlC11kXu.ytZkRppIvvLstCjmC	0	0	ghost	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	M	0	0	0	0	0	0	9	0	0	0	0	PC	0	0	0	0	2020-10-03 05:56:41.102464	0	2020-10-03 05:56:41.102464	0
81	$2a$06$gsjiViXipt2xcNlcbRPvyufieozWa.c3NmNZjJZSo4K9RVRGZx3sK	0	0	sniper	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	M	0	0	0	0	0	0	1	0	0	0	0	PC	0	0	0	0	2020-09-03 00:43:59.88469	0	2020-09-03 00:43:59.88469	8388848
92	$2a$06$nDQOuWRvR7NyTKgN6Rvjj.1GbWCVfhSdH7wEqZ1RbGVPV8nnHWfve	0	0	zim	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	M	0	0	0	0	0	0	9	0	0	0	0	PC	0	0	0	0	2020-10-08 18:04:46.943319	0	2020-10-08 18:04:46.943319	8388848
80	$2a$06$rT6UNRQ2fIcTYlCj8uSyfu2SQEAmkOsWL1ITAYiPnEz7z6Q5fLBrW	0	0	medic	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	M	0	0	0	0	0	0	6	0	0	0	0	PC	0	0	0	0	2020-09-02 22:25:11.87892	0	2020-09-02 22:25:11.87892	8388736
82	$2a$06$HwsKn2sKzeT0y5kukiMpW.OYQgti2aHOazGQetNGDAFr59c7DgK6a	0	0	zip	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	M	0	0	0	0	0	0	1	0	0	0	0	PC	0	0	0	0	2020-09-11 19:25:54.833008	0	2020-09-11 19:25:54.833008	0
83	$2a$06$i/iixOQ9Hu8DG2VmoyudoOEYsccRGhYYPITt3GeZYT7/95PpBOAcG	0	0	grifter	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	M	0	0	0	0	0	0	3	0	0	0	0	PC	0	0	0	0	2020-09-12 02:09:58.045748	0	2020-09-12 02:09:58.045748	0
93	$2a$06$surjXkVKTplnqs580s8VMu5PoyfQj9Ru1cDHL3aGAWjQhzcfZgt8.	0	0	trent	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	M	0	0	0	0	0	0	12	0	0	0	0	PC	0	0	0	0	2020-10-13 01:26:24.968938	0	2020-10-13 01:26:24.968938	8388736
85	$2a$06$J2UkvS6kQt7gmfMyh5oKQ.xqEo5yom8PFbot88B3ghNXc.cnYS95i	0	0	ggg	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	M	0	0	0	0	0	0	9	0	0	0	0	PC	0	0	0	0	2020-10-03 08:03:15.164697	0	2020-10-03 08:03:15.164697	0
86	$2a$06$AlNLuWCx3I12ojkM.6gypeKwAMrEDsQHLoCXpxtwjW/h0jFlmkISS	0	0	zzz	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	M	0	0	0	0	0	0	11	0	0	0	0	PC	0	0	0	0	2020-10-03 08:04:23.530455	0	2020-10-03 08:04:23.530455	0
87	$2a$06$y9/OyYsLmvVHWN05xVm8R.oEzFiR9EAQmRZK/XKfnVFRERoe.ouyK	0	0	mmm	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	M	0	0	0	0	0	0	9	0	0	0	0	PC	0	0	0	0	2020-10-03 08:44:47.671052	0	2020-10-03 08:44:47.671052	0
88	$2a$06$eTsCnd2UGbNVdB1xdgVr5uTBRXCSqHe43Jxfd0bu0gHyHPpswEa.a	0	0	zfz	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	M	0	0	0	0	0	0	9	0	0	0	0	PC	0	0	0	0	2020-10-03 08:46:14.537936	0	2020-10-03 08:46:14.537936	0
89	$2a$06$1L1/9qoUqHFxY9GsYVPv0OGpGVBeLleh7rrxtNfP9xDhwdoBEsHtq	0	0	mmf	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	M	0	0	0	0	0	0	12	0	0	0	0	PC	0	0	0	0	2020-10-03 08:49:36.338635	0	2020-10-03 08:49:36.338635	0
90	$2a$06$0JdoKNw3nCsGrDbGRlMCN.xT7YNJiE24eGBQQgm9nqfKtx/fM.bRW	0	0	lol	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	M	0	0	0	0	0	0	13	0	0	0	0	PC	0	0	0	0	2020-10-03 08:59:06.485403	0	2020-10-03 08:59:06.485403	0
91	$2a$06$Rnq2F9ztRYNPORV4lQ2n/uylfCsM9oeFvUZsaVgcpoRBSy9Mb9yEq	0	0	m	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	M	0	0	0	0	0	0	20	0	0	0	0	PC	0	0	0	0	2020-10-03 09:37:38.046546	0	2020-10-03 09:37:38.046546	0
94	$2a$06$WIgdsRQ66oJU2mfN8/3Y6uWB/Tc.rCY3w8EklkFvuJjZ3F6qYMEgu	0	0	pyro	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	M	0	0	0	0	0	0	16	0	0	0	0	PC	0	0	0	0	2020-10-15 21:54:32.92138	0	2020-10-15 21:54:32.92138	8388608
1	foKntnEF3KSXA	0	0	far	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	M	0	0	0	0	0	0	0	0	0	0	0	PC	0	0	0	0	2019-03-20 22:38:47.454111	0	2019-03-20 22:38:47.454111	14680304
\.


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
-- Data for Name: player_object; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.player_object (po_id, po_player_id, po_type, po_type_vnum, po_type_id, po_type_load, po_wear_position, po_in_inventory, created_at, updated_at) FROM stdin;
164	33	rifle	21	21	id	16	0	2020-03-13 22:42:42.890086	2020-03-13 22:42:42.890086
166	33	rifle	22	22	id	18	0	2020-03-13 22:42:42.90687	2020-03-13 22:42:42.90687
148	33	object	0	0	id	0	0	2020-03-13 22:42:42.71526	2020-03-13 22:42:42.71526
149	33	object	0	0	id	1	0	2020-03-13 22:42:42.72343	2020-03-13 22:42:42.72343
150	33	object	0	0	id	2	0	2020-03-13 22:42:42.731774	2020-03-13 22:42:42.731774
151	33	object	0	0	id	3	0	2020-03-13 22:42:42.740212	2020-03-13 22:42:42.740212
152	33	object	0	0	id	4	0	2020-03-13 22:42:42.74856	2020-03-13 22:42:42.74856
153	33	object	0	0	id	5	0	2020-03-13 22:42:42.75677	2020-03-13 22:42:42.75677
154	33	object	0	0	id	6	0	2020-03-13 22:42:42.765216	2020-03-13 22:42:42.765216
155	33	object	0	0	id	7	0	2020-03-13 22:42:42.77346	2020-03-13 22:42:42.77346
156	33	object	0	0	id	8	0	2020-03-13 22:42:42.781862	2020-03-13 22:42:42.781862
157	33	object	0	0	id	9	0	2020-03-13 22:42:42.790191	2020-03-13 22:42:42.790191
158	33	object	0	0	id	10	0	2020-03-13 22:42:42.832423	2020-03-13 22:42:42.832423
159	33	object	0	0	id	11	0	2020-03-13 22:42:42.848583	2020-03-13 22:42:42.848583
160	33	object	0	0	id	12	0	2020-03-13 22:42:42.856725	2020-03-13 22:42:42.856725
161	33	object	0	0	id	13	0	2020-03-13 22:42:42.865149	2020-03-13 22:42:42.865149
162	33	object	0	0	id	14	0	2020-03-13 22:42:42.873602	2020-03-13 22:42:42.873602
163	33	object	0	0	id	15	0	2020-03-13 22:42:42.881893	2020-03-13 22:42:42.881893
165	33	object	0	0	id	17	0	2020-03-13 22:42:42.898575	2020-03-13 22:42:42.898575
167	33	object	0	0	id	19	0	2020-03-13 22:42:42.915237	2020-03-13 22:42:42.915237
633	32	object	0	0	id	0	0	2020-03-14 02:55:35.940487	2020-03-14 02:55:35.940487
634	32	object	0	0	id	1	0	2020-03-14 02:55:35.948597	2020-03-14 02:55:35.948597
635	32	object	0	0	id	2	0	2020-03-14 02:55:35.956816	2020-03-14 02:55:35.956816
636	32	object	0	0	id	3	0	2020-03-14 02:55:35.965189	2020-03-14 02:55:35.965189
637	32	object	0	0	id	4	0	2020-03-14 02:55:35.97382	2020-03-14 02:55:35.97382
638	32	object	0	0	id	5	0	2020-03-14 02:55:35.981973	2020-03-14 02:55:35.981973
639	32	object	0	0	id	6	0	2020-03-14 02:55:35.99026	2020-03-14 02:55:35.99026
640	32	object	0	0	id	7	0	2020-03-14 02:55:35.998535	2020-03-14 02:55:35.998535
641	32	object	0	0	id	8	0	2020-03-14 02:55:36.007047	2020-03-14 02:55:36.007047
642	32	object	0	0	id	9	0	2020-03-14 02:55:36.015233	2020-03-14 02:55:36.015233
643	32	object	0	0	id	10	0	2020-03-14 02:55:36.023469	2020-03-14 02:55:36.023469
644	32	object	0	0	id	11	0	2020-03-14 02:55:36.031886	2020-03-14 02:55:36.031886
645	32	object	0	0	id	12	0	2020-03-14 02:55:36.040456	2020-03-14 02:55:36.040456
646	32	object	0	0	id	13	0	2020-03-14 02:55:36.048606	2020-03-14 02:55:36.048606
647	32	object	0	0	id	14	0	2020-03-14 02:55:36.056845	2020-03-14 02:55:36.056845
648	32	object	0	0	id	15	0	2020-03-14 02:55:36.065346	2020-03-14 02:55:36.065346
650	32	object	0	0	id	17	0	2020-03-14 02:55:36.081987	2020-03-14 02:55:36.081987
652	32	object	0	0	id	19	0	2020-03-14 02:55:36.098669	2020-03-14 02:55:36.098669
653	32	rifle	0	1	id	0	1	2020-03-14 02:55:36.106961	2020-03-14 02:55:36.106961
654	32	rifle	0	1	id	0	1	2020-03-14 02:55:36.115174	2020-03-14 02:55:36.115174
655	32	rifle	0	1	id	0	1	2020-03-14 02:55:36.123431	2020-03-14 02:55:36.123431
656	32	rifle	0	1	id	0	1	2020-03-14 02:55:36.131913	2020-03-14 02:55:36.131913
657	32	rifle	0	1	id	0	1	2020-03-14 02:55:36.140251	2020-03-14 02:55:36.140251
649	32	rifle	19	19	id	16	0	2020-03-14 02:55:36.073979	2020-03-14 02:55:36.073979
651	32	rifle	20	20	id	18	0	2020-03-14 02:55:36.090223	2020-03-14 02:55:36.090223
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


--
-- Data for Name: room; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.room (room_number, zone, sector_type, name, description, ex_keyword, ex_description, light, room_flag, id) FROM stdin;
126	6	0	<default room name>	<default description>	\N	\N	0	0	1
129	7	0	<default room name>	<default description>	\N	\N	0	0	2
130	7	0	<default room name>	<default description>	\N	\N	0	0	3
131	7	0	<default room name>	<default description>	\N	\N	0	0	4
132	7	0	<default room name>	<default description>	\N	\N	0	0	5
133	7	0	<default room name>	<default description>	\N	\N	0	0	6
134	7	0	<default room name>	<default description>	\N	\N	0	0	7
135	7	0	<default room name>	<default description>	\N	\N	0	0	8
136	7	0	<default room name>	<default description>	\N	\N	0	0	9
137	7	0	<default room name>	<default description>	\N	\N	0	0	10
138	7	0	<default room name>	<default description>	\N	\N	0	0	11
139	7	0	<default room name>	<default description>	\N	\N	0	0	12
140	7	0	<default room name>	<default description>	\N	\N	0	0	13
141	7	0	<default room name>	<default description>	\N	\N	0	0	14
142	7	0	<default room name>	<default description>	\N	\N	0	0	15
143	7	0	<default room name>	<default description>	\N	\N	0	0	16
144	7	0	<default room name>	<default description>	\N	\N	0	0	17
145	7	0	<default room name>	<default description>	\N	\N	0	0	18
147	7	0	name	description	\N	\N	0	0	21
146	7	0	name	description	\N	\N	0	0	19
149	0	0	name	description	\N	\N	0	0	22
150	0	0	name	description	\N	\N	0	0	23
151	0	0	name	description	\N	\N	0	0	24
152	0	0	name	description	\N	\N	0	0	25
153	0	0	name	description	\N	\N	0	0	26
154	0	0	name	description	\N	\N	0	0	27
155	0	0	name	description	\N	\N	0	0	28
156	0	0	name	description	\N	\N	0	0	29
157	0	0	name	description	\N	\N	0	0	30
158	0	0	name	description	\N	\N	0	0	31
159	0	0	name	description	\N	\N	0	0	32
160	0	0	name	description	\N	\N	0	0	33
161	0	0	name	description	\N	\N	0	0	34
162	0	0	name	description	\N	\N	0	0	35
163	0	0	name	description	\N	\N	0	0	36
164	0	0	name	description	\N	\N	0	0	37
165	0	0	name	description	\N	\N	0	0	38
166	0	0	name	description	\N	\N	0	0	39
167	0	0	name	description	\N	\N	0	0	40
168	0	0	name	description	\N	\N	0	0	41
169	0	0	name	description	\N	\N	0	0	42
170	0	0	name	description	\N	\N	0	0	43
171	0	0	name	description	\N	\N	0	0	44
172	0	0	name	description	\N	\N	0	0	45
173	0	0	name	description	\N	\N	0	0	46
174	0	0	name	description	\N	\N	0	0	47
175	0	0	name	description	\N	\N	0	0	48
176	0	0	name	description	\N	\N	0	0	49
177	0	0	name	description	\N	\N	0	0	50
178	0	0	name	description	\N	\N	0	0	51
179	0	0	name	description	\N	\N	0	0	52
180	0	0	name	description	\N	\N	0	0	53
181	0	0	name	description	\N	\N	0	0	54
182	0	0	name	description	\N	\N	0	0	55
183	0	0	name	description	\N	\N	0	0	56
184	0	0	name	description	\N	\N	0	0	57
185	0	0	name	description	\N	\N	0	0	58
186	0	0	name	description	\N	\N	0	0	59
187	0	0	name	description	\N	\N	0	0	60
188	0	0	name	description	\N	\N	0	0	61
189	0	0	name	description	\N	\N	0	0	62
190	0	0	name	description	\N	\N	0	0	63
191	0	0	name	description	\N	\N	0	0	64
192	0	0	name	description	\N	\N	0	0	65
193	0	0	name	description	\N	\N	0	0	66
194	0	0	name	description	\N	\N	0	0	67
195	0	0	name	description	\N	\N	0	0	68
196	0	0	name	description	\N	\N	0	0	69
197	0	0	name	description	\N	\N	0	0	70
198	0	0	name	description	\N	\N	0	0	71
128	7	18	MP5 repo	The ebb and flow of traffic has worn down the tile floor here. You can't help but notice the pungent chemical smell that permeates every corner of this dingy hideout. The owner obviously doesn't care for appearances, but you can't really blame him. The idea of a clean weapons shop is a foreign concept. The very mention of the subject of cleanliness is instantly dismissed. A heavily armored door to the east is likely where all official business is conducted behind the scenes. You notice the stock loss prevention camera jutting from the corner of the room which has the best angle on every corner of the room.\r\n	\N	\N	0	16	20
410	1	0	Outer training courtyard - D.M.Z 0x4155	You stand in a square shaped courtyard amongs other new recruits. You can smell the reek of sweat and dirt everywhere you go. {yel}Corporal James Taggart{/yel} stands before you: a menacing authoritative presence. There is a heavy steel door to the {grn}North{/grn}.	\N	\N	\N	0	81
\.


--
-- Data for Name: room_direction_data; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.room_direction_data (id, room_number, exit_direction, general_description, keyword, exit_info, exit_key, to_room) FROM stdin;
347	126	2	general desc	keyword	1	0	124
349	129	0	general description	keyword	1	0	130
350	129	2	general desc	keyword	1	0	128
351	130	1	general description	keyword	1	0	131
352	130	2	general desc	keyword	1	0	129
353	131	1	general description	keyword	1	0	132
354	131	3	general desc	keyword	1	0	130
355	132	0	general description	keyword	1	0	133
356	132	3	general desc	keyword	1	0	131
357	133	0	general description	keyword	1	0	134
358	133	2	general desc	keyword	1	0	132
359	134	0	general description	keyword	1	0	135
360	134	2	general desc	keyword	1	0	133
361	135	1	general description	keyword	1	0	136
362	135	2	general desc	keyword	1	0	134
363	136	0	general description	keyword	1	0	137
364	136	3	general desc	keyword	1	0	135
365	137	2	general desc	keyword	1	0	136
366	137	3	general description	keyword	1	0	138
367	138	0	general description	keyword	1	0	139
368	138	1	general desc	keyword	1	0	137
369	139	0	general description	keyword	1	0	140
370	139	2	general desc	keyword	1	0	138
371	140	2	general desc	keyword	1	0	139
372	140	4	general description	keyword	1	0	141
373	141	4	general description	keyword	1	0	142
374	141	5	general desc	keyword	1	0	140
375	142	2	general description	keyword	1	0	143
376	142	5	general desc	keyword	1	0	141
377	143	0	general desc	keyword	1	0	142
378	143	1	general description	keyword	1	0	144
379	144	1	general description	keyword	1	0	145
380	144	3	general desc	keyword	1	0	143
381	145	0	general description	keyword	1	0	146
382	145	3	general desc	keyword	1	0	144
400	148	3	general desc	keyword	0	0	147
471	147	1	general_description	keyword	1	0	149
472	147	2	general desc	keyword	0	0	146
474	146	0	general_description	keyword	1	0	149
475	146	2	general desc	keyword	0	0	145
476	149	1	general_description	keyword	1	0	150
477	149	2	general description	keyword	1	0	146
478	150	1	general_description	keyword	1	0	151
479	150	3	general description	keyword	1	0	149
480	151	0	general_description	keyword	1	0	152
481	151	1	general_description	keyword	1	0	157
482	151	2	general_description	keyword	1	0	153
483	151	3	general description	keyword	1	0	150
484	152	1	general_description	keyword	1	0	154
485	152	2	general description	keyword	1	0	151
486	153	0	general description	keyword	1	0	151
487	154	2	general_description	keyword	1	0	155
488	154	3	general description	keyword	1	0	152
489	155	0	general description	keyword	1	0	154
490	155	3	general_description	keyword	1	0	156
491	156	1	general description	keyword	1	0	155
492	157	0	general_description	keyword	1	0	162
493	157	2	general_description	keyword	1	0	158
494	157	3	general description	keyword	1	0	151
495	158	0	general description	keyword	1	0	157
496	158	1	general_description	keyword	1	0	161
497	158	2	general_description	keyword	1	0	160
498	158	3	general_description	keyword	1	0	159
499	159	1	general description	keyword	1	0	158
500	160	0	general description	keyword	1	0	158
501	161	3	general description	keyword	1	0	158
502	162	0	general_description	keyword	1	0	164
503	162	1	general_description	keyword	1	0	163
504	162	2	general description	keyword	1	0	157
505	163	3	general description	keyword	1	0	162
506	164	0	general_description	keyword	1	0	165
507	164	2	general description	keyword	1	0	162
508	165	1	general_description	keyword	1	0	167
509	165	2	general description	keyword	1	0	164
510	165	3	general_description	keyword	1	0	166
511	166	1	general description	keyword	1	0	165
512	167	0	general_description	keyword	1	0	168
513	167	3	general description	keyword	1	0	165
514	168	0	general_description	keyword	1	0	169
515	168	2	general description	keyword	1	0	167
516	169	0	general_description	keyword	1	0	170
517	169	2	general description	keyword	1	0	168
518	170	0	general_description	keyword	1	0	171
519	170	2	general description	keyword	1	0	169
520	171	2	general description	keyword	1	0	170
521	171	3	general_description	keyword	1	0	172
522	172	1	general description	keyword	1	0	171
523	172	3	general_description	keyword	1	0	173
524	173	1	general description	keyword	1	0	172
525	173	2	general_description	keyword	1	0	174
526	174	0	general description	keyword	1	0	173
527	174	2	general_description	keyword	1	0	175
528	175	0	general description	keyword	1	0	174
529	175	2	general_description	keyword	1	0	176
530	175	3	general_description	keyword	1	0	179
531	176	0	general description	keyword	1	0	175
532	176	2	general_description	keyword	1	0	177
533	177	0	general description	keyword	1	0	176
534	177	1	general_description	keyword	1	0	178
535	178	3	general description	keyword	1	0	177
536	179	0	general_description	keyword	1	0	180
537	179	1	general description	keyword	1	0	175
538	180	2	general description	keyword	1	0	179
539	180	3	general_description	keyword	1	0	181
540	181	0	general_description	keyword	1	0	182
541	181	1	general description	keyword	1	0	180
542	181	2	general_description	keyword	1	0	188
543	182	2	general description	keyword	1	0	181
544	182	3	general_description	keyword	1	0	183
545	183	1	general description	keyword	1	0	182
546	183	3	general_description	keyword	1	0	184
547	184	1	general description	keyword	1	0	183
548	184	2	general_description	keyword	1	0	185
549	185	0	general description	keyword	1	0	184
550	185	2	general_description	keyword	1	0	186
551	186	0	general description	keyword	1	0	185
552	186	2	general_description	keyword	1	0	187
553	187	0	general description	keyword	1	0	186
554	188	0	general description	keyword	1	0	181
555	188	2	general_description	keyword	1	0	189
556	189	0	general description	keyword	1	0	188
557	189	2	general_description	keyword	1	0	190
558	190	0	general description	keyword	1	0	189
559	190	3	general_description	keyword	1	0	191
560	191	1	general description	keyword	1	0	190
561	191	3	general_description	keyword	1	0	192
562	192	0	general_description	keyword	1	0	193
563	192	1	general description	keyword	1	0	191
564	192	2	general_description	keyword	1	0	194
565	193	2	general description	keyword	1	0	192
566	194	0	general description	keyword	1	0	192
567	194	3	general_description	keyword	1	0	195
568	195	1	general description	keyword	1	0	194
569	195	3	general_description	keyword	1	0	196
570	196	1	general description	keyword	1	0	195
571	196	2	general_description	keyword	1	0	197
572	196	3	general_description	keyword	1	0	198
573	197	0	general description	keyword	1	0	196
574	198	1	general description	keyword	1	0	196
575	128	0	general description	keyword	0	0	129
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
2	2	129
\.


--
-- Data for Name: shops; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.shops (shop_id, shop_vnum, shop_title, shop_description, shop_profit_buy, shop_profit_sell, shop_type, shop_no_such_item1, shop_no_such_item2, shop_missing_cash1, shop_missing_cash2, shop_do_not_buy, shop_message_buy, shop_message_sell, shop_temper1, shop_bitvector, shop_keeper, shop_with_who, shop_open1, shop_open2, shop_close1, shop_close2, shop_bankaccount, shop_lastsort, shop_flags) FROM stdin;
2	2	MP5 repo	Feeling over burdened by money?	0	0	0	We don't carry that sort of thing here.	Look, we don't carry that...	No money, no product. Simple as that.	The precursor to buying guns is that you have to have money upfront...	I don't work with those types of items.	You got it. Here you go!	Nice. Maybe I can find some poor schmuck to sell this to...	0	0	0	0	0	0	0	0	0	0	0
3	3	MP5 repo	Feeling over burdened by money?	0	0	0	We don't carry that sort of thing here.	Look, we don't carry that...	No money, no product. Simple as that.	The precursor to buying guns is that you have to have money upfront...	I don't work with those types of items.	You got it. Here you go!	Nice. Maybe I can find some poor schmuck to sell this to...	0	0	0	0	0	0	0	0	0	0	0
\.


--
-- Data for Name: skill_trees; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.skill_trees (id, sktree_class, sktree_skill_name, sktree_skill_description, sktree_skill_level_unlock, sktree_skill_parent_id, sktree_skill_order) FROM stdin;
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
-- Data for Name: zone; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.zone (id, zone_virtual_number, zone_start, zone_end, zone_name, lifespan, reset_mode) FROM stdin;
7	1	124	120	D.O. Internals	15	2
151	7	127	227	alpha	100	2
\.


--
-- Data for Name: zone_data; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.zone_data (id, zone_id, zone_command, zone_if_flag, zone_arg1, zone_arg2, zone_arg3) FROM stdin;
6	0	M	1	0	10	27088
8	0	M	1	0	10	27088
13	0	M	0	0	10	27088
14	0	M	0	0	10	27088
15	7	M	0	0	10	27088
16	7	O	0	94	4	27088
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
-- Name: class_contagion_contagion_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.class_contagion_contagion_id_seq', 1, false);


--
-- Name: class_engineer_engineer_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.class_engineer_engineer_id_seq', 1, false);


--
-- Name: class_ghost_ghost_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.class_ghost_ghost_id_seq', 1, false);


--
-- Name: class_marine_marine_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.class_marine_marine_id_seq', 4, true);


--
-- Name: class_medic_medic_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.class_medic_medic_id_seq', 1064, true);


--
-- Name: class_psyop_psyop_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.class_psyop_psyop_id_seq', 1, false);


--
-- Name: class_sentinel_sentinel_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.class_sentinel_sentinel_id_seq', 10, true);


--
-- Name: class_sniper_sniper_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.class_sniper_sniper_id_seq', 472, true);


--
-- Name: class_support_support_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.class_support_support_id_seq', 1, false);


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

SELECT pg_catalog.setval('public.room_room_id_seq', 81, true);


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
-- Name: skill_trees_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.skill_trees_id_seq', 1, false);


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

ALTER TABLE ONLY public.computer_terminal
    ADD CONSTRAINT computer_terminal_pkey PRIMARY KEY (id);


--
-- Name: mini_gunner_sentinel mini_gunner_sentinel_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.mini_gunner_sentinel
    ADD CONSTRAINT mini_gunner_sentinel_pkey PRIMARY KEY (id);


--
-- Name: npc_dialogue npc_dialogue_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.npc_dialogue
    ADD CONSTRAINT npc_dialogue_pkey PRIMARY KEY (id);


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

ALTER TABLE ONLY public.player
    ADD CONSTRAINT player_pkey PRIMARY KEY (id);


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
-- Name: room_direction_data room_direction_data_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.room_direction_data
    ADD CONSTRAINT room_direction_data_pkey PRIMARY KEY (id);


--
-- Name: room room_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.room
    ADD CONSTRAINT room_pkey PRIMARY KEY (id);


--
-- Name: room room_room_number_key; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.room
    ADD CONSTRAINT room_room_number_key UNIQUE (room_number);


--
-- Name: skill_trees skill_trees_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.skill_trees
    ADD CONSTRAINT skill_trees_pkey PRIMARY KEY (id);


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
-- Name: skill_trees skill_trees_sktree_class_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.skill_trees
    ADD CONSTRAINT skill_trees_sktree_class_fkey FOREIGN KEY (sktree_class) REFERENCES public.player_classes(id);


--
-- Name: skill_trees skill_trees_sktree_skill_parent_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.skill_trees
    ADD CONSTRAINT skill_trees_sktree_skill_parent_id_fkey FOREIGN KEY (sktree_skill_parent_id) REFERENCES public.skill_trees(id);


--
-- Name: terminal_choices terminal_choices_choice_terminal_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.terminal_choices
    ADD CONSTRAINT terminal_choices_choice_terminal_id_fkey FOREIGN KEY (choice_terminal_id) REFERENCES public.computer_terminal(id);


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

ALTER TABLE public.class_sniper DROP COLUMN sniper_primary_type;
ALTER TABLE public.class_sniper DROP COLUMN sniper_primary_weapon_id;
ALTER TABLE public.class_sniper DROP COLUMN sniper_secondary_type;
ALTER TABLE public.class_sniper DROP COLUMN sniper_secondary_weapon_id;
DROP TABLE public.class_marine;
CREATE TABLE public.class_marine (
	marine_id SERIAL,
	marine_player_id INTEGER NOT NULL,
	created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);
CREATE TABLE public.class_breacher (
	breacher_id SERIAL,
	breacher_player_id INTEGER NOT NULL,
	created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);

DROP TABLE public.class_engineer;
CREATE TABLE public.class_engineer (
	engineer_id SERIAL,
	engineer_player_id INTEGER NOT NULL,
	created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);
DROP TABLE public.class_medic;
CREATE TABLE public.class_medic (
	medic_id SERIAL,
	medic_player_id INTEGER NOT NULL,
	created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);
DROP TABLE public.class_support;
CREATE TABLE public.class_support (
	support_id SERIAL,
	support_player_id INTEGER NOT NULL,
	created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);
DROP TABLE public.class_ghost;
CREATE TABLE public.class_ghost (
	ghost_id SERIAL,
	ghost_player_id INTEGER NOT NULL,
	created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);
