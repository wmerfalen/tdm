\c postgres
DROP DATABASE mud;
CREATE DATABASE mud;
\c mud;

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
-- Name: armor_index; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.armor_index (
    id integer NOT NULL,
    armor_filename character varying NOT NULL,
    armor_type character varying NOT NULL,
    created_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
    updated_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL
);


ALTER TABLE public.armor_index OWNER TO postgres;

--
-- Name: armor_index_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.armor_index_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.armor_index_id_seq OWNER TO postgres;

--
-- Name: armor_index_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.armor_index_id_seq OWNED BY public.armor_index.id;


--
-- Name: armor_locker; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.armor_locker (
    id integer NOT NULL,
    a_room_vnum integer[] NOT NULL,
    a_yaml text[] NOT NULL
);


ALTER TABLE public.armor_locker OWNER TO postgres;

--
-- Name: armor_locker_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.armor_locker_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.armor_locker_id_seq OWNER TO postgres;

--
-- Name: armor_locker_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.armor_locker_id_seq OWNED BY public.armor_locker.id;


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
-- Name: class_breacher; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.class_breacher (
    breacher_id integer NOT NULL,
    breacher_player_id integer NOT NULL,
    created_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
    updated_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL
);


ALTER TABLE public.class_breacher OWNER TO postgres;

--
-- Name: class_breacher_breacher_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.class_breacher_breacher_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.class_breacher_breacher_id_seq OWNER TO postgres;

--
-- Name: class_breacher_breacher_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.class_breacher_breacher_id_seq OWNED BY public.class_breacher.breacher_id;


--
-- Name: class_contagion; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.class_contagion (
    contagion_id integer NOT NULL,
    contagion_player_id integer NOT NULL,
    created_at timestamp without time zone DEFAULT now(),
    updated_at timestamp without time zone DEFAULT now()
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
-- Name: class_sniper; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.class_sniper (
    sniper_id integer NOT NULL,
    sniper_player_id integer NOT NULL,
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
-- Name: contract_step_callback; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.contract_step_callback (
    id integer NOT NULL,
    s_contract_vnum integer NOT NULL,
    s_task_type text,
    s_task_target text,
    s_task_vnum integer,
    s_sequence_vnum integer NOT NULL
);


ALTER TABLE public.contract_step_callback OWNER TO postgres;

--
-- Name: contract_step_callback_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.contract_step_callback_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.contract_step_callback_id_seq OWNER TO postgres;

--
-- Name: contract_step_callback_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.contract_step_callback_id_seq OWNED BY public.contract_step_callback.id;


--
-- Name: contract_steps; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.contract_steps (
    id integer NOT NULL,
    s_contract_vnum integer NOT NULL,
    s_task_type integer NOT NULL,
    s_task_target integer NOT NULL,
    s_description text NOT NULL,
    s_object_yaml text,
    s_mob_vnum integer,
    s_room_vnum integer,
    s_quota integer,
    s_is_optional boolean DEFAULT false NOT NULL,
    s_order integer NOT NULL,
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
    created_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
    updated_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL
);


ALTER TABLE public.contract_steps OWNER TO postgres;

--
-- Name: contract_steps_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.contract_steps_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.contract_steps_id_seq OWNER TO postgres;

--
-- Name: contract_steps_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.contract_steps_id_seq OWNED BY public.contract_steps.id;


--
-- Name: contracts; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.contracts (
    id integer NOT NULL,
    c_vnum integer NOT NULL,
    c_description text NOT NULL,
    c_title text NOT NULL,
    created_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
    updated_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL
);


ALTER TABLE public.contracts OWNER TO postgres;

--
-- Name: contracts_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.contracts_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.contracts_id_seq OWNER TO postgres;

--
-- Name: contracts_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.contracts_id_seq OWNED BY public.contracts.id;


--
-- Name: event_messages; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.event_messages (
    id integer NOT NULL,
    em_msg text NOT NULL
);


ALTER TABLE public.event_messages OWNER TO postgres;

--
-- Name: event_messages_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.event_messages_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.event_messages_id_seq OWNER TO postgres;

--
-- Name: event_messages_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.event_messages_id_seq OWNED BY public.event_messages.id;


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
-- Name: friendly_reminders; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.friendly_reminders (
    id integer NOT NULL,
    fr_msg text NOT NULL
);


ALTER TABLE public.friendly_reminders OWNER TO postgres;

--
-- Name: friendly_reminders_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.friendly_reminders_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.friendly_reminders_id_seq OWNER TO postgres;

--
-- Name: friendly_reminders_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.friendly_reminders_id_seq OWNED BY public.friendly_reminders.id;


--
-- Name: hq_locations; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.hq_locations (
    id integer NOT NULL,
    hq_affiliation character varying(32),
    hq_room_vnum integer NOT NULL,
    hq_level integer DEFAULT 1 NOT NULL,
    hq_basic_mob_count integer DEFAULT 10 NOT NULL,
    hq_advanced_mob_count integer DEFAULT 10 NOT NULL,
    hq_elite_mob_count integer DEFAULT 10 NOT NULL,
    hq_suv_count integer DEFAULT 10 NOT NULL,
    hq_sedan_count integer DEFAULT 20 NOT NULL,
    hq_armored_van_count integer DEFAULT 2 NOT NULL,
    hq_replenish_ticks integer DEFAULT 150 NOT NULL,
    hq_replenish_basic_count integer DEFAULT 15 NOT NULL,
    hq_replenish_advanced_count integer DEFAULT 10 NOT NULL,
    hq_replenish_elite_count integer DEFAULT 2 NOT NULL,
    created_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
    updated_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL
);


ALTER TABLE public.hq_locations OWNER TO postgres;

--
-- Name: hq_locations_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.hq_locations_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.hq_locations_id_seq OWNER TO postgres;

--
-- Name: hq_locations_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.hq_locations_id_seq OWNED BY public.hq_locations.id;


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
-- Name: mob_equipment; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.mob_equipment (
    id integer NOT NULL,
    meq_profile_name character varying(1024),
    meq_vnum integer NOT NULL,
    meq_light character varying(1024),
    meq_finger_r character varying(1024),
    meq_finger_l character varying(1024),
    meq_neck_1 character varying(1024),
    meq_neck_2 character varying(1024),
    meq_body character varying(1024),
    meq_head character varying(1024),
    meq_legs character varying(1024),
    meq_feet character varying(1024),
    meq_hands character varying(1024),
    meq_arms character varying(1024),
    meq_shield character varying(1024),
    meq_about character varying(1024),
    meq_waist character varying(1024),
    meq_wrist_r character varying(1024),
    meq_wrist_l character varying(1024),
    meq_wield character varying(1024),
    meq_hold character varying(1024),
    meq_secondary character varying(1024),
    meq_shoulders_l character varying(1024),
    meq_shoulders_r character varying(1024),
    meq_backpack character varying(1024),
    meq_goggles character varying(1024),
    meq_vest_pack character varying(1024),
    meq_elbow_l character varying(1024),
    meq_elbow_r character varying(1024),
    created_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
    updated_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL
);


ALTER TABLE public.mob_equipment OWNER TO postgres;

--
-- Name: mob_equipment_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.mob_equipment_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.mob_equipment_id_seq OWNER TO postgres;

--
-- Name: mob_equipment_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.mob_equipment_id_seq OWNED BY public.mob_equipment.id;


--
-- Name: mob_equipment_map; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.mob_equipment_map (
    id integer NOT NULL,
    mmap_mob_vnum integer NOT NULL,
    mmap_mob_equipment_vnum integer NOT NULL,
    created_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
    updated_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL
);


ALTER TABLE public.mob_equipment_map OWNER TO postgres;

--
-- Name: mob_equipment_map_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.mob_equipment_map_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.mob_equipment_map_id_seq OWNER TO postgres;

--
-- Name: mob_equipment_map_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.mob_equipment_map_id_seq OWNED BY public.mob_equipment_map.id;


--
-- Name: mob_roam; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.mob_roam (
    id integer NOT NULL,
    mob_virtual_number integer NOT NULL,
    room_virtual_number integer NOT NULL,
    profile_name character varying(255)
);


ALTER TABLE public.mob_roam OWNER TO postgres;

--
-- Name: mob_roam_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.mob_roam_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.mob_roam_id_seq OWNER TO postgres;

--
-- Name: mob_roam_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.mob_roam_id_seq OWNED BY public.mob_roam.id;


--
-- Name: mob_zone; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.mob_zone (
    id integer NOT NULL,
    zone_virtual_number integer NOT NULL,
    mob_virtual_number integer NOT NULL,
    room_virtual_number integer NOT NULL,
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
    mob_special_extended_type integer DEFAULT 0 NOT NULL,
    mob_targets text,
    mob_roam_pattern text,
    mob_ability_electronics integer DEFAULT 1 NOT NULL,
    mob_ability_armor integer DEFAULT 1 NOT NULL,
    mob_ability_marksmanship integer DEFAULT 1 NOT NULL,
    mob_ability_sniping integer DEFAULT 1 NOT NULL,
    mob_ability_demolitions integer DEFAULT 1 NOT NULL,
    mob_ability_chemistry integer DEFAULT 1 NOT NULL,
    mob_ability_weapon_handling integer DEFAULT 1 NOT NULL,
    mob_ability_strategy integer DEFAULT 1 NOT NULL,
    mob_ability_medical integer DEFAULT 1 NOT NULL
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
    armor_incendiary_resistance_percent integer DEFAULT 0 NOT NULL,
    armor_explosive_resistance_percent integer DEFAULT 0 NOT NULL,
    armor_shrapnel_resistance_percent integer DEFAULT 0 NOT NULL,
    armor_corrosive_resistance_percent integer DEFAULT 0 NOT NULL,
    armor_cryogenic_resistance_percent integer DEFAULT 0 NOT NULL,
    armor_radiation_resistance_percent integer DEFAULT 0 NOT NULL,
    armor_emp_resistance_percent integer DEFAULT 0 NOT NULL,
    armor_shock_resistance_percent integer DEFAULT 0 NOT NULL,
    armor_anti_matter_resistance_percent integer DEFAULT 0 NOT NULL,
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
    explosive_incendiary_damage_dice_count integer DEFAULT 0 NOT NULL,
    explosive_incendiary_damage_dice_sides integer DEFAULT 0 NOT NULL,
    explosive_radiation_damage_dice_count integer DEFAULT 0 NOT NULL,
    explosive_radiation_damage_dice_sides integer DEFAULT 0 NOT NULL,
    explosive_chemical_damage_dice_count integer DEFAULT 0 NOT NULL,
    explosive_chemical_damage_dice_sides integer DEFAULT 0 NOT NULL,
    explosive_electric_damage_dice_count integer DEFAULT 0 NOT NULL,
    explosive_electric_damage_dice_sides integer DEFAULT 0 NOT NULL,
    explosive_armor_penetration_damage_dice_count integer DEFAULT 0 NOT NULL,
    explosive_armor_penetration_damage_dice_sides integer DEFAULT 0 NOT NULL,
    explosive_damage_dice_count integer DEFAULT 0 NOT NULL,
    explosive_damage_dice_sides integer DEFAULT 0 NOT NULL,
    explosive_incendiary_damage_percent integer DEFAULT 0 NOT NULL,
    explosive_explosive_damage_percent integer DEFAULT 0 NOT NULL,
    explosive_shrapnel_damage_percent integer DEFAULT 0 NOT NULL,
    explosive_corrosive_damage_percent integer DEFAULT 0 NOT NULL,
    explosive_cryogenic_damage_percent integer DEFAULT 0 NOT NULL,
    explosive_radiation_damage_percent integer DEFAULT 0 NOT NULL,
    explosive_emp_damage_percent integer DEFAULT 0 NOT NULL,
    explosive_shock_damage_percent integer DEFAULT 0 NOT NULL,
    explosive_anti_matter_damage_percent integer DEFAULT 0 NOT NULL,
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
    rifle_base_damage integer,
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
    rifle_incendiary_damage integer DEFAULT 0 NOT NULL,
    rifle_explosive_damage integer DEFAULT 0 NOT NULL,
    rifle_shrapnel_damage integer DEFAULT 0 NOT NULL,
    rifle_corrosive_damage integer DEFAULT 0 NOT NULL,
    rifle_cryogenic_damage integer DEFAULT 0 NOT NULL,
    rifle_radioactive_damage integer DEFAULT 0 NOT NULL,
    rifle_emp_damage integer DEFAULT 0 NOT NULL,
    rifle_shock_damage integer DEFAULT 0 NOT NULL,
    rifle_anti_matter_damage integer DEFAULT 0 NOT NULL,
    rifle_stat_strength integer DEFAULT 0 NOT NULL,
    rifle_stat_intelligence integer DEFAULT 0 NOT NULL,
    rifle_stat_wisdom integer DEFAULT 0 NOT NULL,
    rifle_stat_dexterity integer DEFAULT 0 NOT NULL,
    rifle_stat_constitution integer DEFAULT 0 NOT NULL,
    rifle_stat_electronics integer DEFAULT 0 NOT NULL,
    rifle_stat_armor integer DEFAULT 0 NOT NULL,
    rifle_stat_marksmanship integer DEFAULT 0 NOT NULL,
    rifle_stat_sniping integer DEFAULT 0 NOT NULL,
    rifle_stat_demolitions integer DEFAULT 0 NOT NULL,
    rifle_stat_chemistry integer DEFAULT 0 NOT NULL,
    rifle_stat_weapon_handling integer DEFAULT 0 NOT NULL,
    rifle_stat_strategy integer DEFAULT 0 NOT NULL,
    rifle_stat_medical integer DEFAULT 0 NOT NULL,
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
    player_preferences character varying(2048),
    player_practice_sessions integer DEFAULT 1 NOT NULL
);


ALTER TABLE public.player OWNER TO postgres;

--
-- Name: player_base_ability; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.player_base_ability (
    pba_id integer NOT NULL,
    pba_player_id integer NOT NULL,
    pba_str integer DEFAULT 0 NOT NULL,
    pba_str_add integer DEFAULT 0 NOT NULL,
    pba_intel integer DEFAULT 0 NOT NULL,
    pba_wis integer DEFAULT 0 NOT NULL,
    pba_dex integer DEFAULT 0 NOT NULL,
    pba_con integer DEFAULT 0 NOT NULL,
    pba_cha integer DEFAULT 0 NOT NULL,
    pba_electronics integer DEFAULT 0 NOT NULL,
    pba_armor integer DEFAULT 0 NOT NULL,
    pba_marksmanship integer DEFAULT 0 NOT NULL,
    pba_sniping integer DEFAULT 0 NOT NULL,
    pba_demolitions integer DEFAULT 0 NOT NULL,
    pba_chemistry integer DEFAULT 0 NOT NULL,
    pba_weapon_handling integer DEFAULT 0 NOT NULL,
    pba_strategy integer DEFAULT 0 NOT NULL,
    pba_medical integer DEFAULT 0 NOT NULL,
    created_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
    updated_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL
);


ALTER TABLE public.player_base_ability OWNER TO postgres;

--
-- Name: player_base_ability_pba_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.player_base_ability_pba_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.player_base_ability_pba_id_seq OWNER TO postgres;

--
-- Name: player_base_ability_pba_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.player_base_ability_pba_id_seq OWNED BY public.player_base_ability.pba_id;


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
-- Name: player_contract_state; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.player_contract_state (
    id integer NOT NULL,
    pc_player_id integer NOT NULL,
    pc_contract_vnum integer NOT NULL,
    pc_state_data text,
    created_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
    updated_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL
);


ALTER TABLE public.player_contract_state OWNER TO postgres;

--
-- Name: player_contract_state_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.player_contract_state_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.player_contract_state_id_seq OWNER TO postgres;

--
-- Name: player_contract_state_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.player_contract_state_id_seq OWNED BY public.player_contract_state.id;


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
    id integer NOT NULL,
    po_player_id integer NOT NULL,
    po_type integer NOT NULL,
    po_type_id integer,
    po_yaml text,
    po_load_type integer NOT NULL,
    po_wear_position integer,
    po_in_inventory integer,
    po_quantity integer DEFAULT 1 NOT NULL,
    po_ammunition integer
);


ALTER TABLE public.player_object OWNER TO postgres;

--
-- Name: player_object_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.player_object_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.player_object_id_seq OWNER TO postgres;

--
-- Name: player_object_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.player_object_id_seq OWNED BY public.player_object.id;


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
-- Name: player_skill_points; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.player_skill_points (
    id integer NOT NULL,
    ps_skill_id integer NOT NULL,
    ps_points integer DEFAULT 0 NOT NULL,
    ps_player_id integer NOT NULL,
    created_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
    updated_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL
);


ALTER TABLE public.player_skill_points OWNER TO postgres;

--
-- Name: player_skill_points_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.player_skill_points_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.player_skill_points_id_seq OWNER TO postgres;

--
-- Name: player_skill_points_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.player_skill_points_id_seq OWNED BY public.player_skill_points.id;


--
-- Name: player_skill_usage; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.player_skill_usage (
    id integer NOT NULL,
    ps_player_id integer NOT NULL,
    ps_skill_id integer NOT NULL,
    ps_usage_count integer NOT NULL,
    created_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
    updated_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL
);


ALTER TABLE public.player_skill_usage OWNER TO postgres;

--
-- Name: player_skill_usage_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.player_skill_usage_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.player_skill_usage_id_seq OWNER TO postgres;

--
-- Name: player_skill_usage_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.player_skill_usage_id_seq OWNED BY public.player_skill_usage.id;


--
-- Name: rifle_attachment; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.rifle_attachment (
    id integer NOT NULL,
    rifle_player_id integer NOT NULL,
    rifle_data text NOT NULL,
    rifle_position text DEFAULT 'inventory'::text NOT NULL,
    created_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
    updated_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL
);


ALTER TABLE public.rifle_attachment OWNER TO postgres;

--
-- Name: rifle_attachment_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.rifle_attachment_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.rifle_attachment_id_seq OWNER TO postgres;

--
-- Name: rifle_attachment_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.rifle_attachment_id_seq OWNED BY public.rifle_attachment.id;


--
-- Name: rifle_index; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.rifle_index (
    id integer NOT NULL,
    rifle_filename character varying NOT NULL,
    rifle_type character varying NOT NULL,
    created_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
    updated_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL
);


ALTER TABLE public.rifle_index OWNER TO postgres;

--
-- Name: rifle_index_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.rifle_index_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.rifle_index_id_seq OWNER TO postgres;

--
-- Name: rifle_index_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.rifle_index_id_seq OWNED BY public.rifle_index.id;


--
-- Name: rifle_instance; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.rifle_instance (
    rifle_id integer NOT NULL,
    rifle_accuracy_map_0 double precision DEFAULT 10.0,
    rifle_accuracy_map_1 double precision DEFAULT 10.0,
    rifle_accuracy_map_2 double precision DEFAULT 10.0,
    rifle_accuracy_map_3 double precision DEFAULT 10.0,
    rifle_damage_map_0 double precision DEFAULT 10.0,
    rifle_damage_map_1 double precision DEFAULT 10.0,
    rifle_damage_map_2 double precision DEFAULT 10.0,
    rifle_damage_map_3 double precision DEFAULT 10.0,
    rifle_rarity public.rarity_t DEFAULT 'COMMON'::public.rarity_t NOT NULL,
    rifle_file character varying(32),
    rifle_str_type character varying(32),
    rifle_type character varying(32),
    rifle_manufacturer character varying(32),
    rifle_name character varying(32),
    rifle_vnum integer,
    rifle_ammo_max integer,
    rifle_ammo_type character varying(32) DEFAULT 'SNIPER'::character varying NOT NULL,
    rifle_chance_to_injure double precision,
    rifle_clip_size integer,
    rifle_cooldown_between_shots integer,
    rifle_critical_chance integer,
    rifle_critical_range integer,
    rifle_base_damage integer,
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
    created_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
    updated_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL
);


ALTER TABLE public.rifle_instance OWNER TO postgres;

--
-- Name: rifle_instance_rifle_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.rifle_instance_rifle_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.rifle_instance_rifle_id_seq OWNER TO postgres;

--
-- Name: rifle_instance_rifle_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.rifle_instance_rifle_id_seq OWNED BY public.rifle_instance.rifle_id;


--
-- Name: rifle_placements; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.rifle_placements (
    id integer NOT NULL,
    ip_room_vnum integer NOT NULL,
    ip_container_selector text,
    ip_rifle_id integer NOT NULL,
    created_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
    updated_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL
);


ALTER TABLE public.rifle_placements OWNER TO postgres;

--
-- Name: rifle_placements_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.rifle_placements_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.rifle_placements_id_seq OWNER TO postgres;

--
-- Name: rifle_placements_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.rifle_placements_id_seq OWNED BY public.rifle_placements.id;


--
-- Name: room; Type: TABLE; Schema: public; Owner: postgres
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
    room_flag integer NOT NULL,
    nickname text
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
-- Name: room_extra_descriptions; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.room_extra_descriptions (
    id integer NOT NULL,
    red_room_vnum integer NOT NULL,
    red_keyword text NOT NULL,
    red_description text NOT NULL
);


ALTER TABLE public.room_extra_descriptions OWNER TO postgres;

--
-- Name: room_extra_descriptions_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.room_extra_descriptions_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.room_extra_descriptions_id_seq OWNER TO postgres;

--
-- Name: room_extra_descriptions_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.room_extra_descriptions_id_seq OWNED BY public.room_extra_descriptions.id;


--
-- Name: room_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.room_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.room_id_seq OWNER TO postgres;

--
-- Name: room_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.room_id_seq OWNED BY public.room.id;


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
-- Name: scripted_sequences; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.scripted_sequences (
    id integer NOT NULL,
    s_sequence_vnum integer NOT NULL
);


ALTER TABLE public.scripted_sequences OWNER TO postgres;

--
-- Name: scripted_sequences_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.scripted_sequences_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.scripted_sequences_id_seq OWNER TO postgres;

--
-- Name: scripted_sequences_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.scripted_sequences_id_seq OWNED BY public.scripted_sequences.id;


--
-- Name: scripted_steps; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.scripted_steps (
    id integer NOT NULL,
    s_sequence_vnum integer NOT NULL,
    s_wait_ticks integer DEFAULT 0 NOT NULL,
    s_mob integer,
    s_obj integer,
    s_room integer,
    s_quantity integer,
    s_order integer DEFAULT 0 NOT NULL,
    s_interpret text,
    s_yaml text,
    s_type text NOT NULL,
    s_dialogue text
);


ALTER TABLE public.scripted_steps OWNER TO postgres;

--
-- Name: scripted_steps_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.scripted_steps_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.scripted_steps_id_seq OWNER TO postgres;

--
-- Name: scripted_steps_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.scripted_steps_id_seq OWNED BY public.scripted_steps.id;


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
-- Name: skill_points; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.skill_points (
    id integer NOT NULL,
    sp_level integer NOT NULL,
    sp_points integer DEFAULT 0 NOT NULL,
    created_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
    updated_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL
);


ALTER TABLE public.skill_points OWNER TO postgres;

--
-- Name: skill_points_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.skill_points_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.skill_points_id_seq OWNER TO postgres;

--
-- Name: skill_points_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.skill_points_id_seq OWNED BY public.skill_points.id;


--
-- Name: skill_trees; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.skill_trees (
    id integer NOT NULL,
    skill_name character varying(255) NOT NULL,
    skill_category character varying(255) NOT NULL,
    skill_player_class character varying(255) NOT NULL,
    skill_parent_id integer,
    skill_order integer DEFAULT 0 NOT NULL,
    skill_description text,
    created_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
    updated_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL
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
-- Name: skill_usage; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.skill_usage (
    id integer NOT NULL,
    player_id integer NOT NULL,
    skill_name character varying(32) NOT NULL,
    skill_level double precision DEFAULT 0.0 NOT NULL
);


ALTER TABLE public.skill_usage OWNER TO postgres;

--
-- Name: skill_usage_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.skill_usage_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.skill_usage_id_seq OWNER TO postgres;

--
-- Name: skill_usage_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.skill_usage_id_seq OWNED BY public.skill_usage.id;


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
-- Name: weapon_locker; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.weapon_locker (
    id integer NOT NULL,
    w_room_vnum integer[] NOT NULL,
    w_yaml text[] NOT NULL
);


ALTER TABLE public.weapon_locker OWNER TO postgres;

--
-- Name: weapon_locker_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.weapon_locker_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.weapon_locker_id_seq OWNER TO postgres;

--
-- Name: weapon_locker_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.weapon_locker_id_seq OWNED BY public.weapon_locker.id;


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
    zone_arg3 integer NOT NULL,
    zone_yaml text
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
-- Name: armor_index id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.armor_index ALTER COLUMN id SET DEFAULT nextval('public.armor_index_id_seq'::regclass);


--
-- Name: armor_locker id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.armor_locker ALTER COLUMN id SET DEFAULT nextval('public.armor_locker_id_seq'::regclass);


--
-- Name: camera_feed feed_id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.camera_feed ALTER COLUMN feed_id SET DEFAULT nextval('public.camera_feed_feed_id_seq'::regclass);


--
-- Name: class_breacher breacher_id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.class_breacher ALTER COLUMN breacher_id SET DEFAULT nextval('public.class_breacher_breacher_id_seq'::regclass);


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
-- Name: contract_step_callback id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.contract_step_callback ALTER COLUMN id SET DEFAULT nextval('public.contract_step_callback_id_seq'::regclass);


--
-- Name: contract_steps id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.contract_steps ALTER COLUMN id SET DEFAULT nextval('public.contract_steps_id_seq'::regclass);


--
-- Name: contracts id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.contracts ALTER COLUMN id SET DEFAULT nextval('public.contracts_id_seq'::regclass);


--
-- Name: event_messages id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.event_messages ALTER COLUMN id SET DEFAULT nextval('public.event_messages_id_seq'::regclass);


--
-- Name: extra_description id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.extra_description ALTER COLUMN id SET DEFAULT nextval('public.extra_description_id_seq'::regclass);


--
-- Name: friendly_reminders id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.friendly_reminders ALTER COLUMN id SET DEFAULT nextval('public.friendly_reminders_id_seq'::regclass);


--
-- Name: hq_locations id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.hq_locations ALTER COLUMN id SET DEFAULT nextval('public.hq_locations_id_seq'::regclass);


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
-- Name: mob_equipment id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.mob_equipment ALTER COLUMN id SET DEFAULT nextval('public.mob_equipment_id_seq'::regclass);


--
-- Name: mob_equipment_map id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.mob_equipment_map ALTER COLUMN id SET DEFAULT nextval('public.mob_equipment_map_id_seq'::regclass);


--
-- Name: mob_roam id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.mob_roam ALTER COLUMN id SET DEFAULT nextval('public.mob_roam_id_seq'::regclass);


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
-- Name: player_base_ability pba_id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.player_base_ability ALTER COLUMN pba_id SET DEFAULT nextval('public.player_base_ability_pba_id_seq'::regclass);


--
-- Name: player_classes id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.player_classes ALTER COLUMN id SET DEFAULT nextval('public.player_classes_id_seq'::regclass);


--
-- Name: player_contract_state id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.player_contract_state ALTER COLUMN id SET DEFAULT nextval('public.player_contract_state_id_seq'::regclass);


--
-- Name: player_flags id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.player_flags ALTER COLUMN id SET DEFAULT nextval('public.player_flags_id_seq'::regclass);


--
-- Name: player_object id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.player_object ALTER COLUMN id SET DEFAULT nextval('public.player_object_id_seq'::regclass);


--
-- Name: player_race_perks id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.player_race_perks ALTER COLUMN id SET DEFAULT nextval('public.player_race_perks_id_seq'::regclass);


--
-- Name: player_races id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.player_races ALTER COLUMN id SET DEFAULT nextval('public.player_races_id_seq'::regclass);


--
-- Name: player_skill_points id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.player_skill_points ALTER COLUMN id SET DEFAULT nextval('public.player_skill_points_id_seq'::regclass);


--
-- Name: player_skill_usage id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.player_skill_usage ALTER COLUMN id SET DEFAULT nextval('public.player_skill_usage_id_seq'::regclass);


--
-- Name: rifle_attachment id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.rifle_attachment ALTER COLUMN id SET DEFAULT nextval('public.rifle_attachment_id_seq'::regclass);


--
-- Name: rifle_index id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.rifle_index ALTER COLUMN id SET DEFAULT nextval('public.rifle_index_id_seq'::regclass);


--
-- Name: rifle_instance rifle_id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.rifle_instance ALTER COLUMN rifle_id SET DEFAULT nextval('public.rifle_instance_rifle_id_seq'::regclass);


--
-- Name: rifle_placements id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.rifle_placements ALTER COLUMN id SET DEFAULT nextval('public.rifle_placements_id_seq'::regclass);


--
-- Name: room id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.room ALTER COLUMN id SET DEFAULT nextval('public.room_id_seq'::regclass);


--
-- Name: room_direction_data id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.room_direction_data ALTER COLUMN id SET DEFAULT nextval('public.room_direction_data_id_seq'::regclass);


--
-- Name: room_extra_descriptions id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.room_extra_descriptions ALTER COLUMN id SET DEFAULT nextval('public.room_extra_descriptions_id_seq'::regclass);


--
-- Name: scripted_sequences id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.scripted_sequences ALTER COLUMN id SET DEFAULT nextval('public.scripted_sequences_id_seq'::regclass);


--
-- Name: scripted_steps id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.scripted_steps ALTER COLUMN id SET DEFAULT nextval('public.scripted_steps_id_seq'::regclass);


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
-- Name: skill_points id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.skill_points ALTER COLUMN id SET DEFAULT nextval('public.skill_points_id_seq'::regclass);


--
-- Name: skill_trees id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.skill_trees ALTER COLUMN id SET DEFAULT nextval('public.skill_trees_id_seq'::regclass);


--
-- Name: skill_usage id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.skill_usage ALTER COLUMN id SET DEFAULT nextval('public.skill_usage_id_seq'::regclass);


--
-- Name: terminal_choices id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.terminal_choices ALTER COLUMN id SET DEFAULT nextval('public.terminal_choices_id_seq'::regclass);


--
-- Name: weapon_locker id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.weapon_locker ALTER COLUMN id SET DEFAULT nextval('public.weapon_locker_id_seq'::regclass);


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
-- Data for Name: armor_index; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.armor_index (id, armor_filename, armor_type, created_at, updated_at) FROM stdin;
1	baklava.yml	head	2021-09-04 04:12:07.150447	2021-09-04 04:12:07.150447
2	tactical-gas-mask.yml	head	2021-09-04 04:12:07.150913	2021-09-04 04:12:07.150913
3	thermal-goggles.yml	goggles	2021-09-04 04:12:07.151313	2021-09-04 04:12:07.151313
4	night-vision-goggles.yml	goggles	2021-09-04 04:12:07.151694	2021-09-04 04:12:07.151694
5	titan-shoulder-pads.yml	shoulders	2021-09-04 04:12:07.152031	2021-09-04 04:12:07.152031
6	xm-scorpio-shoulder-pads.yml	shoulders	2021-09-04 04:12:07.152365	2021-09-04 04:12:07.152365
7	viper-shoulder-pads.yml	shoulders	2021-09-04 04:12:07.152703	2021-09-04 04:12:07.152703
8	falcon-shoulder-pads.yml	shoulders	2021-09-04 04:12:07.153036	2021-09-04 04:12:07.153036
9	basic-ballistic-vest.yml	vest	2021-09-04 04:12:07.153366	2021-09-04 04:12:07.153366
10	falcon-ballistic-vest.yml	vest	2021-09-04 04:12:07.153696	2021-09-04 04:12:07.153696
11	raven-ballistic-vest.yml	vest	2021-09-04 04:12:07.154141	2021-09-04 04:12:07.154141
12	vulture-ballistic-vest.yml	vest	2021-09-04 04:12:07.154551	2021-09-04 04:12:07.154551
13	xm-scorpio-slotted-vest-pack.yml	vestpack	2021-09-04 04:12:07.154923	2021-09-04 04:12:07.154923
14	raven-ultralight-backpack.yml	backpack	2021-09-04 04:12:07.155321	2021-09-04 04:12:07.155321
15	titan-gauntlets.yml	arms	2021-09-04 04:12:07.155703	2021-09-04 04:12:07.155703
16	titan-elbow-guards.yml	elbow	2021-09-04 04:12:07.156063	2021-09-04 04:12:07.156063
17	razor-gps-wrist-watch.yml	wrist	2021-09-04 04:12:07.156437	2021-09-04 04:12:07.156437
18	xm-scorpio-tactical-gloves.yml	hands	2021-09-04 04:12:07.156781	2021-09-04 04:12:07.156781
19	xm-scorpio-belt.yml	waist	2021-09-04 04:12:07.157173	2021-09-04 04:12:07.157173
20	titan-shin-guards.yml	legs	2021-09-04 04:12:07.157519	2021-09-04 04:12:07.157519
21	viper-leg-guards.yml	legs	2021-09-04 04:12:07.157893	2021-09-04 04:12:07.157893
22	basic-boots.yml	feet	2021-09-04 04:12:07.158231	2021-09-04 04:12:07.158231
23	xm50-ultralight-boots.yml	feet	2021-09-04 04:12:07.158612	2021-09-04 04:12:07.158612
24	xm607-vulture-boots.yml	feet	2021-09-04 04:12:07.158979	2021-09-04 04:12:07.158979
25	xm8-panama-combat-boots.yml	feet	2021-09-04 04:12:07.159364	2021-09-04 04:12:07.159364
\.


--
-- Data for Name: armor_locker; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.armor_locker (id, a_room_vnum, a_yaml) FROM stdin;
\.


--
-- Data for Name: camera_feed; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.camera_feed (feed_id, feed_type, feed_vnum, feed_room_vnum, feed_order, created_at, updated_at) FROM stdin;
\.


--
-- Data for Name: class_breacher; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.class_breacher (breacher_id, breacher_player_id, created_at, updated_at) FROM stdin;
\.


--
-- Data for Name: class_contagion; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.class_contagion (contagion_id, contagion_player_id, created_at, updated_at) FROM stdin;
\.


--
-- Data for Name: class_engineer; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.class_engineer (engineer_id, engineer_player_id, created_at, updated_at) FROM stdin;
\.


--
-- Data for Name: class_ghost; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.class_ghost (ghost_id, ghost_player_id, created_at, updated_at) FROM stdin;
12	110	2021-09-18 01:42:45.198142	2021-09-18 01:42:45.198142
\.


--
-- Data for Name: class_marine; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.class_marine (marine_id, marine_player_id, created_at, updated_at) FROM stdin;
\.


--
-- Data for Name: class_medic; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.class_medic (medic_id, medic_player_id, created_at, updated_at) FROM stdin;
\.


--
-- Data for Name: class_sniper; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.class_sniper (sniper_id, sniper_player_id, created_at, updated_at) FROM stdin;
1	1	2021-09-04 04:12:07.132625	2021-09-04 04:12:07.132625
\.


--
-- Data for Name: class_support; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.class_support (support_id, support_player_id, created_at, updated_at) FROM stdin;
\.


--
-- Data for Name: computer_terminal; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.computer_terminal (id, terminal_room_vnum, terminal_type, terminal_attaches_to, terminal_name, terminal_description, created_at, updated_at) FROM stdin;
\.


--
-- Data for Name: contract_step_callback; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.contract_step_callback (id, s_contract_vnum, s_task_type, s_task_target, s_task_vnum, s_sequence_vnum) FROM stdin;
11	1	GOAL_FIND	TARGET_ROOM	143	1
12	1	GOAL_FIND	TARGET_ROOM	144	2
\.


--
-- Data for Name: contract_steps; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.contract_steps (id, s_contract_vnum, s_task_type, s_task_target, s_description, s_object_yaml, s_mob_vnum, s_room_vnum, s_quota, s_is_optional, s_order, s_reward_xp, s_reward_money, s_reward_1, s_reward_2, s_reward_3, s_reward_4, s_reward_5, s_reward_6, s_reward_7, s_reward_8, s_reward_9, s_reward_10, created_at, updated_at) FROM stdin;
26	1	1	2	Find Doctor Land. Doctor Land is a ballistics expert and as such runs the majority of the new recruit shooting range\r\nactivities. You can find him in the gear room.	\N	0	266	0	f	0	850	1150	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2021-09-18 01:54:46.849428	2021-09-18 01:54:46.849428
27	1	1	2	Locate the TRITON Labs research scientist. He can give you the magazines that Doctor Land requires.\r\n	\N	0	143	0	f	1	850	1150	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2021-09-18 01:54:46.851098	2021-09-18 01:54:46.851098
28	1	1	2	Follow the TRITON Labs research scientist north to get the magazines.\r\n	\N	0	144	0	f	2	850	5000	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2021-09-18 01:54:46.85257	2021-09-18 01:54:46.85257
29	1	1	0	Go back to Doctor Land with the magazines.\r\n	\N	603	0	0	f	3	1150	1150	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2021-09-18 01:54:46.854077	2021-09-18 01:54:46.854077
30	1	512	0	Give all 4 magazines to Doctor Land so that he can find a way to build more.\r\n	\N	603	0	4	f	4	1150	1150	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2021-09-18 01:54:46.855485	2021-09-18 01:54:46.855485
\.


--
-- Data for Name: contracts; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.contracts (id, c_vnum, c_description, c_title, created_at, updated_at) FROM stdin;
11	1	Good evening, soldier. According to recent advances in our {yel}TRITON{/yel} sponsored laboratory a new kind of ammunition is being built.\r\n\r\nThe blueprint for this ammunition contains heavy amounts of radioactive isotopes from recently spent Uranium. Find the scientist in the armory to east of {blu}COBALT{/blu} Main hallway. He will give you the magazines he has made so far. Once you've acquired the magazines, I will give you further instructions over radio.\r\n{yel}Here are the contract instructions:{/yel}\r\n{yel}1) {grn}Find {blu}Doctor Land{/blu} {grn}in the Gear Room near the shooting range.{/grn}\r\n{yel}2) {grn}Find the TRITON Labs scientist. He will give you the magazines you need.\r\n{yel}3) {grn}Take the magazines to {blu}Doctor Land{/blu}.\r\n{yel}4) {grn}Talk to {blu}Doctor Land{/blu} on next steps...\r\n	Experimental {yel}High Velocity{/yel} magazines.	2021-09-18 01:54:46.847285	2021-09-18 01:54:46.847285
\.


--
-- Data for Name: event_messages; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.event_messages (id, em_msg) FROM stdin;
\.


--
-- Data for Name: extra_description; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.extra_description (id, obj_fk_id, extra_keyword, extra_description) FROM stdin;
\.


--
-- Data for Name: friendly_reminders; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.friendly_reminders (id, fr_msg) FROM stdin;
\.


--
-- Data for Name: hq_locations; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.hq_locations (id, hq_affiliation, hq_room_vnum, hq_level, hq_basic_mob_count, hq_advanced_mob_count, hq_elite_mob_count, hq_suv_count, hq_sedan_count, hq_armored_van_count, hq_replenish_ticks, hq_replenish_basic_count, hq_replenish_advanced_count, hq_replenish_elite_count, created_at, updated_at) FROM stdin;
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
\.


--
-- Data for Name: mini_gunner_sentinel; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.mini_gunner_sentinel (id, mgs_mob_vnum, mgs_face_direction, mgs_room_vnum) FROM stdin;
\.


--
-- Data for Name: mob_equipment; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.mob_equipment (id, meq_profile_name, meq_vnum, meq_light, meq_finger_r, meq_finger_l, meq_neck_1, meq_neck_2, meq_body, meq_head, meq_legs, meq_feet, meq_hands, meq_arms, meq_shield, meq_about, meq_waist, meq_wrist_r, meq_wrist_l, meq_wield, meq_hold, meq_secondary, meq_shoulders_l, meq_shoulders_r, meq_backpack, meq_goggles, meq_vest_pack, meq_elbow_l, meq_elbow_r, created_at, updated_at) FROM stdin;
1	triton-scientist	7	\N	\N	\N	\N	\N	armor/plain-white-lab-coat.yml	\N	armor/pale-brown-pants.yml	armor/static-free-shoes.yml	armor/rubber-gloves.yml	\N	\N	\N	\N	\N	\N	\N	gadget/triton-labs-yellow-key-fob.yml	\N	\N	\N	\N	armor/thick-glasses.yml	\N	\N	\N	2021-09-04 04:12:22.241931	2021-09-04 04:12:22.241931
2	triton-volunteer-patient	8	\N	\N	\N	\N	\N	armor/hospital-gown.yml	\N	\N	armor/hospital-shoes.yml	\N	\N	\N	\N	\N	\N	\N	melee/improvised-shank.yml	\N	\N	\N	\N	\N	\N	\N	\N	\N	2021-09-04 04:12:22.247385	2021-09-04 04:12:22.247385
3	triton-field-surgeon	9	\N	\N	\N	\N	\N	armor/plain-white-lab-coat.yml	\N	armor/pale-brown-pants.yml	armor/static-free-shoes.yml	armor/rubber-gloves.yml	\N	\N	\N	\N	\N	\N	melee/operating-scalpel.yml	\N	\N	\N	\N	\N	armor/operating-goggles.yml	\N	\N	\N	2021-09-04 04:12:22.252503	2021-09-04 04:12:22.252503
4	doctor-land	6	\N	\N	\N	\N	\N	armor/vulture-pk3-ballistic-vest.yml	\N	armor/heavy-armament-tactical-pants.yml	armor/vulture-pk3-ballistic-boots.yml	armor/forge-xm3-gloves.yml	\N	\N	\N	\N	\N	\N	rifle/ump45-vulture-modded.yml	gadget/ballistics-lab-blue-key-fob.yml	rifle/blackhawk-50cal-pistol.yml	\N	\N	\N	\N	\N	\N	\N	2021-09-04 04:12:22.26565	2021-09-04 04:12:22.26565
5	waypoint-ave-car-thief	2	\N	\N	\N	\N	\N	armor/basic-ballistic-vest.yml	armor/baklava.yml	armor/dark-smithies-pants.yml	armor/red-brug-sneakers.yml	melee/brass-knuckles.yml	\N	\N	\N	\N	\N	\N	melee/crowbar.yml	\N	\N	\N	\N	\N	\N	\N	\N	\N	2021-09-04 04:12:22.295644	2021-09-04 04:12:22.295644
6	rogue-mp-shotgunner	3	\N	\N	\N	\N	\N	armor/basic-ballistic-vest.yml	\N	armor/mp-enforcer-pants.yml	armor/basic-boots.yml	armor/mp-enforcer-gloves.yml	\N	\N	\N	\N	\N	\N	rifle/bf-39-shotgun.yml	\N	\N	\N	\N	\N	\N	\N	\N	\N	2021-09-04 04:12:22.299795	2021-09-04 04:12:22.299795
7	mp-enforcer	4	\N	\N	\N	\N	\N	armor/basic-ballistic-vest.yml	\N	armor/mp-enforcer-pants.yml	armor/basic-boots.yml	armor/mp-enforcer-gloves.yml	\N	\N	\N	\N	\N	\N	rifle/hk45.yml	\N	\N	\N	\N	\N	\N	\N	\N	\N	2021-09-04 04:12:22.320894	2021-09-04 04:12:22.320894
8	mp-shotgunner	5	\N	\N	\N	\N	\N	armor/basic-ballistic-vest.yml	\N	armor/mp-enforcer-pants.yml	armor/basic-boots.yml	armor/mp-enforcer-gloves.yml	\N	\N	\N	\N	\N	\N	rifle/bf-39-shotgun.yml	\N	\N	\N	\N	\N	\N	\N	\N	\N	2021-09-04 04:12:22.326344	2021-09-04 04:12:22.326344
9	ops-shield-shotgunner	15	\N	\N	\N	\N	\N	armor/basic-ballistic-vest.yml	\N	armor/mp-enforcer-pants.yml	armor/basic-boots.yml	armor/mp-enforcer-gloves.yml	\N	\N	\N	\N	\N	\N	rifle/bf-39-shotgun.yml	\N	\N	\N	\N	\N	\N	\N	\N	\N	2021-09-04 04:12:22.331595	2021-09-04 04:12:22.331595
10	car-thief	10	\N	\N	\N	\N	\N	armor/basic-ballistic-vest.yml	armor/baklava.yml	armor/blue-jeans.yml	armor/plat-basketball-shoes.yml	armor/mp-enforcer-gloves.yml	\N	\N	\N	\N	\N	\N	melee/crowbar.yml	\N	\N	\N	\N	\N	\N	\N	\N	\N	2021-09-04 04:12:22.383806	2021-09-04 04:12:22.383806
11	petty-thief	11	\N	\N	\N	\N	\N	\N	\N	armor/blue-jeans.yml	armor/plat-basketball-shoes.yml	\N	\N	\N	\N	\N	\N	\N	melee/cheap-crowbar.yml	\N	\N	\N	\N	\N	\N	\N	\N	\N	2021-09-04 04:12:22.395097	2021-09-04 04:12:22.395097
12	kidnapper	12	\N	\N	\N	armor/dark-throne-necklace.yml	\N	armor/leather-trenchcoat.yml	armor/baklava.yml	armor/black-jeans.yml	armor/atom-fade-shoes.yml	armor/brown-leather-gloves.yml	\N	\N	\N	\N	\N	\N	melee/sickening-knife.yml	\N	\N	\N	\N	\N	\N	\N	\N	\N	2021-09-04 04:12:22.405272	2021-09-04 04:12:22.405272
13	chaotic-meth-addict	13	\N	\N	\N	\N	\N	armor/leather-trenchcoat.yml	\N	armor/black-jeans.yml	armor/atom-fade-shoes.yml	\N	\N	\N	\N	\N	\N	\N	melee/sickening-knife.yml	\N	\N	\N	\N	\N	\N	\N	\N	\N	2021-09-04 04:12:22.416633	2021-09-04 04:12:22.416633
16	retail-associate	109	\N	\N	\N	\N	\N	armor/allied-foods-shirt.yml	\N	armor/black-jeans.yml	armor/plain-black-shoes.yml	\N	\N	\N	\N	\N	\N	\N	melee/retractable-knife.yml	\N	\N	\N	\N	\N	\N	\N	\N	\N	2021-09-04 04:12:23.883086	2021-09-04 04:12:23.883086
17	shoplifter	110	\N	\N	\N	\N	\N	armor/blue-hoodie.yml	\N	armor/dusty-jeans.yml	armor/sloth-martins.yml	\N	\N	\N	\N	\N	\N	\N	rifle/mp5.yml	\N	\N	\N	\N	\N	\N	\N	\N	\N	2021-09-04 04:12:23.894169	2021-09-04 04:12:23.894169
69	armed-guard	14	\N	\N	\N	\N	\N	armor/falcon-ballistic-vest.yml	armor/baklava.yml	armor/mp-enforcer-pants.yml	armor/xm607-vulture-boots.yml	armor/forge-xm3-gloves.yml	\N	\N	\N	\N	\N	\N	rifle/mp5.yml	\N	\N	armor/titan-shoulder-pads.yml	armor/titan-shoulder-pads.yml	\N	\N	\N	armor/titan-elbow-guards.yml	armor/titan-elbow-guards.yml	2021-09-04 22:42:19.155625	2021-09-04 22:42:19.155625
70	teller	107	\N	\N	\N	\N	\N	\N	\N	armor/leggings.yml	armor/dress-shoes.yml	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2021-09-04 22:42:19.163143	2021-09-04 22:42:19.163143
90	thug-lvl-2	112	\N	\N	\N	\N	\N	armor/leather-trenchcoat.yml	\N	armor/black-jeans.yml	armor/atom-fade-shoes.yml	\N	\N	\N	\N	\N	\N	\N	melee/flimsy-knife.yml	\N	\N	\N	\N	\N	\N	\N	\N	\N	2021-09-06 01:20:28.213973	2021-09-06 01:20:28.213973
91	hustler-lvl-3	113	\N	\N	\N	\N	\N	armor/leather-trenchcoat.yml	\N	armor/black-jeans.yml	armor/atom-fade-shoes.yml	\N	\N	\N	\N	\N	\N	\N	melee/weak-brass-knuckles.yml	\N	\N	\N	\N	\N	\N	\N	\N	\N	2021-09-06 01:20:28.231383	2021-09-06 01:20:28.231383
\.


--
-- Data for Name: mob_equipment_map; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.mob_equipment_map (id, mmap_mob_vnum, mmap_mob_equipment_vnum, created_at, updated_at) FROM stdin;
38	112	112	2021-09-06 02:04:34.330576	2021-09-06 02:04:34.330576
39	113	113	2021-09-06 02:04:34.385077	2021-09-06 02:04:34.385077
18	600	7	2021-09-04 04:23:26.981479	2021-09-04 04:23:26.981479
19	601	8	2021-09-04 04:23:26.988068	2021-09-04 04:23:26.988068
20	602	9	2021-09-04 04:23:26.993713	2021-09-04 04:23:26.993713
21	603	6	2021-09-04 04:23:27.008422	2021-09-04 04:23:27.008422
22	406	2	2021-09-04 04:23:27.04276	2021-09-04 04:23:27.04276
23	407	3	2021-09-04 04:23:27.047159	2021-09-04 04:23:27.047159
24	500	4	2021-09-04 04:23:27.068704	2021-09-04 04:23:27.068704
25	501	5	2021-09-04 04:23:27.074152	2021-09-04 04:23:27.074152
26	502	15	2021-09-04 04:23:27.079717	2021-09-04 04:23:27.079717
10	100	10	2021-09-04 04:12:22.385266	2021-09-04 04:12:22.385266
11	101	11	2021-09-04 04:12:22.396441	2021-09-04 04:12:22.396441
27	102	12	2021-09-04 04:23:27.155622	2021-09-04 04:23:27.155622
13	103	13	2021-09-04 04:12:22.417965	2021-09-04 04:12:22.417965
28	104	12	2021-09-04 04:23:27.177327	2021-09-04 04:23:27.177327
29	105	13	2021-09-04 04:23:27.182288	2021-09-04 04:23:27.182288
30	109	109	2021-09-04 04:23:27.352124	2021-09-04 04:23:27.352124
31	110	110	2021-09-04 04:23:27.364886	2021-09-04 04:23:27.364886
34	106	14	2021-09-06 01:20:27.887755	2021-09-06 01:20:27.887755
35	107	107	2021-09-06 01:20:27.896475	2021-09-06 01:20:27.896475
\.


--
-- Data for Name: mob_roam; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.mob_roam (id, mob_virtual_number, room_virtual_number, profile_name) FROM stdin;
634	601	147	volunteer-patient
635	601	148	volunteer-patient
636	601	149	volunteer-patient
637	601	150	volunteer-patient
638	601	151	volunteer-patient
639	601	152	volunteer-patient
640	601	153	volunteer-patient
641	601	154	volunteer-patient
642	601	155	volunteer-patient
643	601	156	volunteer-patient
644	407	157	rogue-mp-shotgunner
645	407	158	rogue-mp-shotgunner
646	407	159	rogue-mp-shotgunner
647	407	160	rogue-mp-shotgunner
648	407	161	rogue-mp-shotgunner
649	407	162	rogue-mp-shotgunner
650	407	163	rogue-mp-shotgunner
651	407	164	rogue-mp-shotgunner
652	407	165	rogue-mp-shotgunner
653	407	164	rogue-mp-shotgunner
654	407	163	rogue-mp-shotgunner
655	407	162	rogue-mp-shotgunner
656	407	161	rogue-mp-shotgunner
657	407	162	rogue-mp-shotgunner
658	407	163	rogue-mp-shotgunner
659	407	164	rogue-mp-shotgunner
660	407	165	rogue-mp-shotgunner
661	407	164	rogue-mp-shotgunner
662	407	163	rogue-mp-shotgunner
663	407	162	rogue-mp-shotgunner
664	407	161	rogue-mp-shotgunner
665	407	162	rogue-mp-shotgunner
666	407	163	rogue-mp-shotgunner
667	407	164	rogue-mp-shotgunner
668	501	130	mp-shotgunner
669	501	131	mp-shotgunner
670	501	138	mp-shotgunner
671	501	131	mp-shotgunner
672	501	231	mp-shotgunner
673	500	130	mp-enforcer
674	500	131	mp-enforcer
675	500	138	mp-enforcer
676	500	131	mp-enforcer
677	500	231	mp-enforcer
678	500	230	mp-enforcer
679	500	229	mp-enforcer
680	500	228	mp-enforcer
681	500	227	mp-enforcer
682	500	226	mp-enforcer
683	500	225	mp-enforcer
684	500	224	mp-enforcer
685	500	223	mp-enforcer
686	500	222	mp-enforcer
687	500	221	mp-enforcer
688	500	220	mp-enforcer
689	500	219	mp-enforcer
690	500	218	mp-enforcer
691	500	217	mp-enforcer
692	500	216	mp-enforcer
693	500	215	mp-enforcer
694	500	214	mp-enforcer
695	500	213	mp-enforcer
696	500	212	mp-enforcer
697	500	211	mp-enforcer
698	500	210	mp-enforcer
699	500	209	mp-enforcer
700	500	208	mp-enforcer
701	500	207	mp-enforcer
702	500	206	mp-enforcer
703	500	205	mp-enforcer
704	500	204	mp-enforcer
705	500	203	mp-enforcer
706	500	202	mp-enforcer
707	500	201	mp-enforcer
708	500	200	mp-enforcer
709	500	199	mp-enforcer
710	500	198	mp-enforcer
711	500	197	mp-enforcer
712	500	196	mp-enforcer
713	500	195	mp-enforcer
714	500	194	mp-enforcer
715	500	193	mp-enforcer
716	500	141	mp-enforcer
717	500	140	mp-enforcer
718	500	139	mp-enforcer
719	100	392	car-thief
720	100	393	car-thief
721	100	394	car-thief
722	100	395	car-thief
723	100	396	car-thief
724	100	395	car-thief
725	100	394	car-thief
726	100	393	car-thief
727	100	392	car-thief
728	100	393	car-thief
729	100	394	car-thief
730	100	395	car-thief
731	100	396	car-thief
732	100	395	car-thief
733	100	394	car-thief
734	100	393	car-thief
735	100	392	car-thief
736	102	291	kidnapper
737	102	292	kidnapper
738	102	293	kidnapper
739	102	294	kidnapper
740	102	295	kidnapper
741	102	329	kidnapper
742	102	330	kidnapper
743	102	331	kidnapper
744	102	330	kidnapper
745	102	329	kidnapper
746	102	295	kidnapper
747	102	296	kidnapper
748	102	297	kidnapper
749	102	298	kidnapper
750	102	299	kidnapper
751	102	300	kidnapper
752	102	301	kidnapper
753	102	316	kidnapper
754	102	317	kidnapper
755	102	322	kidnapper
756	102	323	kidnapper
757	102	324	kidnapper
758	102	323	kidnapper
759	102	322	kidnapper
760	102	317	kidnapper
761	102	316	kidnapper
762	102	301	kidnapper
763	102	302	kidnapper
\.


--
-- Data for Name: mob_zone; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.mob_zone (id, zone_virtual_number, mob_virtual_number, room_virtual_number, max_existing) FROM stdin;
\.


--
-- Data for Name: mobile; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.mobile (mob_id, mob_virtual_number, mob_name, mob_short_description, mob_long_description, mob_description, mob_action_bitvector, mob_affection_bitvector, mob_ability_strength, mob_ability_strength_add, mob_ability_intelligence, mob_ability_wisdom, mob_ability_dexterity, mob_ability_constitution, mob_ability_charisma, mob_alignment, mob_attack_type, mob_level, mob_hitroll, mob_armor, mob_max_hitpoints, mob_max_mana, mob_max_move, mob_gold, mob_exp, mob_load_position, mob_default_position, mob_sex, mob_hitpoints, mob_mana, mob_move, mob_damnodice, mob_damsizedice, mob_damroll, mob_weight, mob_height, mob_class, mob_special_extended_type, mob_targets, mob_roam_pattern, mob_ability_electronics, mob_ability_armor, mob_ability_marksmanship, mob_ability_sniping, mob_ability_demolitions, mob_ability_chemistry, mob_ability_weapon_handling, mob_ability_strategy, mob_ability_medical) FROM stdin;
11	410	Corporal James Taggart	Corporal James Tagger short description	Corporal James Tagger long description	Corporal James Tagger description	0	0	10	10	10	10	10	10	10	0	0	150	150	150	-1	-1	-1	0	50	0	0	0	-1	-1	-1	50	50	50	80	9	0	0	\N	\N	1	1	1	1	1	1	1	1	1
1	1	chef  employee	A pissed looking Los  employee	A pissed looking Los  employee	This particular employee looks like he just got out of a federal penitentiary. He's most likely hiding some weapon in one of the many compartments that should be used for storing utensils and food paraphernalia.	8	0	25	0	25	0	25	0	25	0	0	0	0	0	250	250	250	5000	50	8	8	0	250	250	250	25	0	0	50	15	0	14	\N	\N	1	1	1	1	1	1	1	1	1
28	112	An infected drone	An infected drone	An infected drone	An infected drone looks for something to infect...	8	0	2	2	2	0	4	0	5	0	0	2	0	0	413	100	82	90	50	0	0	1	413	100	82	1	8	10	10	5	0	15	\N	\N	2	3	3	2	3	1	3	2	2
29	113	An adapted drone	An adapted drone	An adapted drone	An adapted drone looks for something to infect...	8	0	4	5	4	0	4	0	5	0	0	3	0	0	350	100	82	48	50	0	0	1	350	100	82	2	12	18	10	5	0	15	\N	\N	4	5	5	4	7	0	8	3	4
12	600	A {yel}TRITON{/yel} {blu}LABS{/blu} Scientist	A {yel}TRITON{/yel} {blu}LABS{/blu} Scientist	A {yel}TRITON{/yel} {blu}LABS{/blu} Scientist	A {yel}TRITON{/yel} {blu}LABS{/blu} Scientist	8	0	85	83	82	0	84	0	1	0	0	40	0	0	58580	15	240	505000	30500	0	0	1	58580	15	140	84	86	215	10	4	0	0	\N	\N	10	10	10	10	10	10	10	10	10
13	601	A volunteer patient	A volunteer patient	A volunteer patient	A volunteer patient stands here with dazed thousand mile stare.	8	0	185	183	10	0	184	0	1	0	0	15	0	0	880	25	240	0	3500	0	0	1	880	25	240	8	90	115	10	4	0	0	\N	\N	10	10	10	10	10	10	10	10	10
14	602	A {yel}TRITON{/yel} {blu}LABS{/blu} Field Surgeon	A {yel}TRITON{/yel} {blu}LABS{/blu} Field Surgeon	A {yel}TRITON{/yel} {blu}LABS{/blu} Field Surgeon	A {yel}TRITON{/yel} {blu}LABS{/blu} Field Surgeon	8	0	185	183	182	0	184	0	85	0	0	80	0	0	8580	815	240	85000	8500	0	0	2	8580	815	240	-72	-70	215	10	4	0	0	\N	\N	10	10	10	10	10	10	10	10	10
15	603	A Ballistics Expert	A Ballistics Expert	A Ballistics Expert	A Ballistics Expert	8	0	85	83	82	0	84	0	1	0	0	40	0	0	58580	15	240	505000	30500	0	0	1	58580	15	140	84	86	215	10	4	0	0	\N	\N	10	10	10	10	10	10	10	10	10
16	406	A suspicious looking car thief	A suspicious looking car thief	A suspicious looking car thief	A car thief equipped with brass knuckles and a crowbar. He is obviously armed.	8	0	25	23	22	0	24	0	10	0	0	15	0	0	150	45	510	950	150	0	0	1	150	45	510	20	16	20	5	5	0	0	\N	\N	0	0	0	0	0	0	0	0	0
17	407	A {red}Rogue{/red} Military Police shotgunner	A {red}Rogue{/red} Military Police shotgunner	A {red}Rogue{/red} Military Police shotgunner	A fit military police shotgunner. He looks armed.	8	0	35	33	32	0	34	0	10	0	0	25	0	0	5550	565	510	150	250	0	0	1	5550	565	510	15	18	30	10	6	0	11	\N	\N	0	0	0	0	0	0	0	0	0
18	500	A Military Police enforcer	A Military Police enforcer	A Military Police enforcer	A fit military police enforcer. He looks armed.	8	0	25	23	22	0	24	0	10	0	0	10	0	0	250	45	110	50	7050	0	0	1	250	45	110	10	6	20	10	5	0	10	\N	\N	10	10	10	10	10	10	10	10	10
19	501	A Military Police shotgunner	A Military Police shotgunner	A Military Police shotgunner	A fit military police shotgunner. He looks armed.	8	0	35	33	32	0	34	0	10	0	0	40	0	0	5550	565	510	150	7050	0	0	1	5550	565	510	15	18	30	10	6	0	11	\N	\N	10	10	10	10	10	10	10	10	10
20	502	An Ops Shield shotgunner	An Ops Shield shotgunner	An Ops Shield shotgunner	An Ops Shield shotgunner	8	0	355	333	132	0	334	0	10	0	0	40	0	0	109550	1565	810	109150	20050	0	0	1	109550	1565	810	115	48	340	10	6	0	11	\N	\N	10	10	10	10	10	10	10	10	10
2	100	A rugged car thief	A rugged car thief	A rugged car thief	A car thief stalking the area. He is armed.	8	0	18	13	2	0	34	0	0	0	0	10	0	0	350	45	610	750	250	0	0	1	350	45	610	30	6	20	10	5	0	14	\N	\N	15	5	15	8	0	0	18	0	0
3	101	A petty thief	A petty thief	A petty thief	A petty thief is stalking the area.	8	0	4	4	2	0	4	0	0	0	0	5	0	0	100	15	110	750	250	0	0	1	100	15	110	2	6	10	4	5	0	14	\N	\N	5	4	5	2	0	0	4	0	0
21	102	a kidnapper	a kidnapper	a kidnapper	a kidnapper is stalking the area.	8	0	9	6	2	0	7	0	0	0	0	8	0	0	450	25	110	1123	250	0	0	1	450	25	110	3	25	20	4	5	0	15	\N	\N	0	10	8	3	0	0	5	0	0
4	103	a chaotic meth addict	a chaotic meth addict	a chaotic meth addict	a chaotic meth addict is manically patrolling the area.	8	0	13	6	2	0	13	0	0	0	0	13	0	0	650	25	310	4123	250	0	0	1	650	25	310	8	25	30	4	5	0	16	\N	\N	0	4	1	1	2	0	8	0	9
22	104	A shoplifter	A shoplifter	A shoplifter	A shoplifter is stalking the area.	8	0	1	1	1	0	1	0	0	0	0	1	0	0	10	15	60	10	250	0	0	1	10	15	60	1	3	1	4	5	0	15	\N	\N	1	0	1	0	0	0	1	0	0
23	105	A crackhead	A crackhead	A crackhead	A crackhead is stalking the area.	8	0	1	1	1	0	3	0	0	0	0	2	0	0	35	15	60	10	250	0	0	1	35	15	60	2	6	1	4	5	0	0	\N	\N	0	0	0	0	0	0	1	0	0
24	109	A retail associate	A retail associate	A retail associate	A retail associate tirelessly stocks shelves...	8	0	15	1	2	0	4	0	4	0	0	3	0	0	120	50	60	85	580	0	0	1	120	50	60	2	5	2	10	5	0	19	\N	\N	2	1	1	1	1	0	1	2	1
25	110	A shoplifter	A shoplifter	A shoplifter	A shoplifter looks around nervously...	8	0	30	1	5	0	8	0	8	0	0	6	0	0	240	100	128	150	580	0	0	1	240	100	128	5	10	5	10	5	0	17	\N	\N	5	3	2	2	3	0	3	5	2
26	106	An armed security guard	An armed security guard	An armed security guard	An armed security guard watches you closely.	8	0	28	23	25	0	54	0	0	0	0	20	0	0	950	245	610	2150	20044	0	0	1	950	245	610	90	18	60	10	5	0	18	\N	\N	25	30	35	20	30	10	38	25	20
27	107	A bank teller	A bank teller	A bank teller	A bank teller is obediently serving you	8	0	1	1	1	0	3	0	30	0	0	2	0	0	15	15	20	450	580	0	0	2	15	15	20	1	3	1	4	5	0	0	\N	\N	0	0	0	0	0	0	1	0	0
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

COPY public.object_armor (armor_id, armor_csv_capabilities, armor_csv_attach_to, armor_thac0, armor_weight_in_lbs, armor_fire_resistance_percent, armor_balistic_resistance_percent, armor_speed_profile, armor_offensive_damage_amount, armor_durability_profile, armor_file, armor_str_type, armor_manufacturer, armor_name, armor_vnum, armor_rarity, armor_type, armor_hp, armor_classification, armor_worth, armor_incendiary_resistance_percent, armor_explosive_resistance_percent, armor_shrapnel_resistance_percent, armor_corrosive_resistance_percent, armor_cryogenic_resistance_percent, armor_radiation_resistance_percent, armor_emp_resistance_percent, armor_shock_resistance_percent, armor_anti_matter_resistance_percent, created_at, updated_at) FROM stdin;
\.


--
-- Data for Name: object_explosive; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.object_explosive (explosive_id, explosive_alternate_explosion_type, explosive_chance_to_injure, explosive_critical_chance, explosive_critical_range, explosive_blast_radius, explosive_damage_per_second, explosive_disorient_amount, explosive_loudness_type, explosive_str_type, explosive_type, explosive_manufacturer, explosive_name, explosive_vnum, explosive_rarity, explosive_file, explosive_incendiary_damage_dice_count, explosive_incendiary_damage_dice_sides, explosive_radiation_damage_dice_count, explosive_radiation_damage_dice_sides, explosive_chemical_damage_dice_count, explosive_chemical_damage_dice_sides, explosive_electric_damage_dice_count, explosive_electric_damage_dice_sides, explosive_armor_penetration_damage_dice_count, explosive_armor_penetration_damage_dice_sides, explosive_damage_dice_count, explosive_damage_dice_sides, explosive_incendiary_damage_percent, explosive_explosive_damage_percent, explosive_shrapnel_damage_percent, explosive_corrosive_damage_percent, explosive_cryogenic_damage_percent, explosive_radiation_damage_percent, explosive_emp_damage_percent, explosive_shock_damage_percent, explosive_anti_matter_damage_percent, created_at, updated_at) FROM stdin;
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

COPY public.object_rifle (rifle_id, rifle_accuracy_map_0, rifle_accuracy_map_1, rifle_accuracy_map_2, rifle_accuracy_map_3, rifle_damage_map_0, rifle_damage_map_1, rifle_damage_map_2, rifle_damage_map_3, rifle_ammo_max, rifle_ammo_type, rifle_chance_to_injure, rifle_clip_size, rifle_cooldown_between_shots, rifle_critical_chance, rifle_critical_range, rifle_base_damage, rifle_disorient_amount, rifle_headshot_bonus, rifle_max_range, rifle_range_multiplier, rifle_reload_time, rifle_rounds_per_minute, rifle_muzzle_velocity, rifle_effective_firing_range, rifle_str_type, rifle_type, rifle_manufacturer, rifle_name, rifle_vnum, rifle_rarity, rifle_file, rifle_incendiary_damage, rifle_explosive_damage, rifle_shrapnel_damage, rifle_corrosive_damage, rifle_cryogenic_damage, rifle_radioactive_damage, rifle_emp_damage, rifle_shock_damage, rifle_anti_matter_damage, rifle_stat_strength, rifle_stat_intelligence, rifle_stat_wisdom, rifle_stat_dexterity, rifle_stat_constitution, rifle_stat_electronics, rifle_stat_armor, rifle_stat_marksmanship, rifle_stat_sniping, rifle_stat_demolitions, rifle_stat_chemistry, rifle_stat_weapon_handling, rifle_stat_strategy, rifle_stat_medical, created_at, updated_at) FROM stdin;
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

COPY public.player (id, player_password, player_affection_plr_bitvector, player_affection_bitvector, player_name, player_short_description, player_long_description, player_action_bitvector, player_ability_strength, player_ability_strength_add, player_ability_intelligence, player_ability_wisdom, player_ability_dexterity, player_ability_constitution, player_ability_charisma, player_ability_alignment, player_attack_type, player_max_hitpoints, player_max_mana, player_max_move, player_gold, player_exp, player_sex, player_hitpoints, player_mana, player_move, player_damroll, player_weight, player_height, player_class, player_title, player_hometown, player_damnodice, player_damsizedice, player_type, player_alignment, player_level, player_hitroll, player_armor, player_birth, player_time_played, player_logon, player_preferences, player_practice_sessions) FROM stdin;
110	$2a$06$V3cNhHZegxU40gLh/I8w1.dR7IaNHnNyzBSUCuv80W1EWiehiajD.	0	0	sniper	1	1	0	3	0	9	9	12	15	9	0	0	241	436	729	48	561	M	241	436	729	52	52	52	9	1	52	0	0	PC	0	3	52	0	2021-09-18 01:42:45.163952	0	2021-09-18 01:42:45.163952	8388736	7
1	foKntnEF3KSXA	0	0	far	1	1	0	231	0	693	519	924	1155	866	4710	0	65000	1212	65000	6752	8740	M	65000	1212	65000	53	53	53	9	1	53	0	0	PC	4710	7	53	0	2019-03-20 22:38:47.454111	0	2019-03-20 22:38:47.454111	14680304	13
\.


--
-- Data for Name: player_base_ability; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.player_base_ability (pba_id, pba_player_id, pba_str, pba_str_add, pba_intel, pba_wis, pba_dex, pba_con, pba_cha, pba_electronics, pba_armor, pba_marksmanship, pba_sniping, pba_demolitions, pba_chemistry, pba_weapon_handling, pba_strategy, pba_medical, created_at, updated_at) FROM stdin;
2	96	50	0	150	112	200	250	187	150	87	137	100	25	150	137	150	225	2021-09-04 04:25:30.214342	2021-09-04 04:25:30.214342
3	97	60	0	60	45	120	180	75	60	105	75	60	15	60	75	60	90	2021-09-07 11:11:48.984162	2021-09-07 11:11:48.984162
4	98	50	0	150	112	200	250	187	150	87	137	100	25	150	137	150	225	2021-09-14 23:33:16.630307	2021-09-14 23:33:16.630307
5	99	330	0	2970	2970	3300	3630	2970	2970	660	4290	1320	2079	2970	2062	2970	3712	2021-09-16 02:18:01.678329	2021-09-16 02:18:01.678329
14	108	6	0	19	19	25	31	19	19	12	15	15	32	20	13	19	18	2021-09-18 01:25:34.696188	2021-09-18 01:25:34.696188
9	103	11	0	33	33	44	55	33	33	22	55	22	34	33	30	33	41	2021-09-17 02:13:34.628945	2021-09-17 02:13:34.628945
10	104	1	0	3	3	4	5	3	3	2	5	2	4	3	10	3	11	2021-09-17 02:18:35.567437	2021-09-17 02:18:35.567437
15	109	1	0	3	3	4	5	3	3	2	4	4	5	4	2	3	3	2021-09-18 01:37:53.530041	2021-09-18 01:37:53.530041
12	106	15	0	46	46	61	77	46	46	30	77	30	69	46	146	46	161	2021-09-17 02:27:46.192218	2021-09-17 02:27:46.192218
11	105	2	0	6	6	8	11	6	6	4	11	4	9	6	20	6	23	2021-09-17 02:23:06.268849	2021-09-17 02:23:06.268849
8	102	99	0	11	11	110	209	11	11	99	52	44	9	11	52	11	19	2021-09-17 00:53:51.810107	2021-09-17 00:53:51.810107
6	100	33	0	99	74	132	165	123	99	57	90	66	16	99	90	99	148	2021-09-16 03:18:37.232244	2021-09-16 03:18:37.232244
13	107	3	0	9	9	12	15	9	9	6	8	8	15	10	6	9	15	2021-09-17 19:58:07.021327	2021-09-17 19:58:07.021327
7	101	44	0	44	44	88	132	44	44	44	60	44	9	44	60	44	60	2021-09-17 00:53:22.86459	2021-09-17 00:53:22.86459
16	110	3	0	9	9	12	15	9	9	6	8	8	15	10	6	9	9	2021-09-18 01:42:45.197498	2021-09-18 01:42:45.197498
1	1	231	0	693	519	924	1155	866	693	404	635	462	115	693	635	693	1039	2021-09-04 04:17:39.714689	2021-09-04 04:17:39.714689
\.


--
-- Data for Name: player_classes; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.player_classes (id, pclass_name, pclass_description) FROM stdin;
\.


--
-- Data for Name: player_contract_state; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.player_contract_state (id, pc_player_id, pc_contract_vnum, pc_state_data, created_at, updated_at) FROM stdin;
\.


--
-- Data for Name: player_flags; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.player_flags (id, player_id, chunk_index, flag_value) FROM stdin;
\.


--
-- Data for Name: player_object; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.player_object (id, po_player_id, po_type, po_type_id, po_yaml, po_load_type, po_wear_position, po_in_inventory, po_quantity, po_ammunition) FROM stdin;
263	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
264	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
274	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
275	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
283	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
289	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
295	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
298	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
301	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
305	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
306	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
323	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
324	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
325	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
328	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
332	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
160	1	8	\N	sg3-sniper-ammunition.yml	2	\N	1	1	36
351	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
276	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
280	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
284	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
293	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
296	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
299	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
302	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
261	1	8	\N	sg3-sniper-ammunition.yml	2	\N	1	1	36
326	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
329	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
161	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
162	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
352	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
163	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
164	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
165	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
166	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
167	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
168	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
169	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
170	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
171	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
172	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
173	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
174	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
175	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
176	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
177	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
178	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
114	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
115	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
116	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
153	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
154	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
155	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
179	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
180	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
181	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
182	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
183	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
184	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
185	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
186	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
187	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
188	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
189	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
252	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
253	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
254	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
255	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
256	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
257	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
258	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
259	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
260	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
245	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
246	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
247	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
248	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
249	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
250	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
251	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
262	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
282	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
288	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
294	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
297	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
300	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
303	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
304	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
322	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
327	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
330	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
331	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
350	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
367	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
368	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
369	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
375	110	7	\N	titan-elbow-guards.yml	2	26	0	1	\N
376	110	7	\N	titan-gauntlets.yml	2	10	0	1	\N
377	110	7	\N	titan-shin-guards.yml	2	7	0	1	\N
378	110	7	\N	basic-boots.yml	2	8	0	1	\N
379	110	7	\N	basic-ballistic-vest.yml	2	5	0	1	\N
381	110	1	\N	psg1.yml	2	16	0	1	7
382	110	8	\N	sg3-sniper-ammunition.yml	2	\N	1	1	50
383	110	8	\N	sg3-sniper-ammunition.yml	2	\N	1	1	50
384	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
385	1	1	\N	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	3	16	0	1	7
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
-- Data for Name: player_skill_points; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.player_skill_points (id, ps_skill_id, ps_points, ps_player_id, created_at, updated_at) FROM stdin;
\.


--
-- Data for Name: player_skill_usage; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.player_skill_usage (id, ps_player_id, ps_skill_id, ps_usage_count, created_at, updated_at) FROM stdin;
\.


--
-- Data for Name: rifle_attachment; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.rifle_attachment (id, rifle_player_id, rifle_data, rifle_position, created_at, updated_at) FROM stdin;
4890	1	psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}#level:30	primary	2021-09-18 04:04:21.874679	2021-09-18 04:04:21.874679
\.


--
-- Data for Name: rifle_index; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.rifle_index (id, rifle_filename, rifle_type, created_at, updated_at) FROM stdin;
1	augpara.yml	smg	2021-09-04 04:12:07.136657	2021-09-04 04:12:07.136657
2	fmg9.yml	smg	2021-09-04 04:12:07.137141	2021-09-04 04:12:07.137141
3	mp5.yml	smg	2021-09-04 04:12:07.137541	2021-09-04 04:12:07.137541
4	mp9.yml	smg	2021-09-04 04:12:07.137923	2021-09-04 04:12:07.137923
5	p90.yml	smg	2021-09-04 04:12:07.138269	2021-09-04 04:12:07.138269
6	tar21.yml	smg	2021-09-04 04:12:07.138675	2021-09-04 04:12:07.138675
7	ump45.yml	smg	2021-09-04 04:12:07.139059	2021-09-04 04:12:07.139059
8	l96aw.yml	sniper	2021-09-04 04:12:07.139481	2021-09-04 04:12:07.139481
9	psg1.yml	sniper	2021-09-04 04:12:07.139872	2021-09-04 04:12:07.139872
10	xm109.yml	sniper	2021-09-04 04:12:07.140248	2021-09-04 04:12:07.140248
11	belt-fed-minigun.yml	lmg	2021-09-04 04:12:07.140632	2021-09-04 04:12:07.140632
12	hk21.yml	lmg	2021-09-04 04:12:07.141027	2021-09-04 04:12:07.141027
13	mk46.yml	lmg	2021-09-04 04:12:07.141378	2021-09-04 04:12:07.141378
14	czp10.yml	pistol	2021-09-04 04:12:07.141737	2021-09-04 04:12:07.141737
15	desert-eagle.yml	pistol	2021-09-04 04:12:07.142118	2021-09-04 04:12:07.142118
16	glock.yml	pistol	2021-09-04 04:12:07.142496	2021-09-04 04:12:07.142496
17	magnum-revolver.yml	pistol	2021-09-04 04:12:07.142846	2021-09-04 04:12:07.142846
18	ppk.yml	pistol	2021-09-04 04:12:07.143192	2021-09-04 04:12:07.143192
19	famas.yml	ar	2021-09-04 04:12:07.143541	2021-09-04 04:12:07.143541
20	g36c.yml	ar	2021-09-04 04:12:07.143885	2021-09-04 04:12:07.143885
21	m16a4.yml	ar	2021-09-04 04:12:07.144361	2021-09-04 04:12:07.144361
22	m3.yml	ar	2021-09-04 04:12:07.1447	2021-09-04 04:12:07.1447
23	m4.yml	ar	2021-09-04 04:12:07.145054	2021-09-04 04:12:07.145054
24	scarh.yml	ar	2021-09-04 04:12:07.145397	2021-09-04 04:12:07.145397
25	552-commando.yml	ar	2021-09-04 04:12:07.145735	2021-09-04 04:12:07.145735
26	aug-a3.yml	ar	2021-09-04 04:12:07.146069	2021-09-04 04:12:07.146069
27	saiga12.yml	shotgun	2021-09-04 04:12:07.146463	2021-09-04 04:12:07.146463
28	sasg12.yml	shotgun	2021-09-04 04:12:07.146818	2021-09-04 04:12:07.146818
29	uzi.yml	mp	2021-09-04 04:12:07.147184	2021-09-04 04:12:07.147184
\.


--
-- Data for Name: rifle_instance; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.rifle_instance (rifle_id, rifle_accuracy_map_0, rifle_accuracy_map_1, rifle_accuracy_map_2, rifle_accuracy_map_3, rifle_damage_map_0, rifle_damage_map_1, rifle_damage_map_2, rifle_damage_map_3, rifle_rarity, rifle_file, rifle_str_type, rifle_type, rifle_manufacturer, rifle_name, rifle_vnum, rifle_ammo_max, rifle_ammo_type, rifle_chance_to_injure, rifle_clip_size, rifle_cooldown_between_shots, rifle_critical_chance, rifle_critical_range, rifle_base_damage, rifle_disorient_amount, rifle_headshot_bonus, rifle_max_range, rifle_range_multiplier, rifle_reload_time, rifle_rounds_per_minute, rifle_muzzle_velocity, rifle_effective_firing_range, rifle_damage_dice_count, rifle_damage_dice_sides, rifle_incendiary_damage, rifle_explosive_damage, rifle_shrapnel_damage, rifle_corrosive_damage, rifle_cryogenic_damage, rifle_radioactive_damage, rifle_emp_damage, rifle_shock_damage, rifle_anti_matter_damage, rifle_stat_strength, rifle_stat_intelligence, rifle_stat_wisdom, rifle_stat_dexterity, rifle_stat_constitution, rifle_stat_electronics, rifle_stat_armor, rifle_stat_marksmanship, rifle_stat_sniping, rifle_stat_demolitions, rifle_stat_chemistry, rifle_stat_weapon_handling, rifle_stat_strategy, rifle_stat_medical, created_at, updated_at) FROM stdin;
1	90	10	0	0	90	40	9	0	COMMON	sasg12.yml	SHOTGUN	3	R.S.S.	SASG-12	6	40	SHOTGUN	77	8	2	34	1	80	0	53	1	4.29999999999999982	2	10	1	10	1	1	0	1	1	1	0	1	1	1	1	0	0	1	1	0	0	0	0	1	1	1	1	0	0	2021-09-06 01:28:51.675323	2021-09-06 01:28:51.675323
2	90	10	0	0	90	40	9	0	COMMON	saiga12.yml	SHOTGUN	3	R.S.S.	SAIGA-12	39	1	SHOTGUN	1	6	2	34	1	154	1	53	2	1	2	14	918	15	3	31	0	1	1	1	1	0	1	1	1	1	0	1	1	1	1	0	0	0	1	0	1	0	0	2021-09-06 01:57:11.43389	2021-09-06 01:57:11.43389
3	80	40	13	5	50	25	10	1	COMMON	magnum-revolver.yml	PISTOL	8	TN3 SMITH-x Industrial	Magnum Revolver	40	75	PISTOL	20.5	6	1	6	1	25	1	55	1	0	4	1	1	2	3	24	1	1	1	0	1	1	1	1	0	1	1	0	0	1	1	0	1	1	1	0	0	0	0	2021-09-06 01:57:28.204973	2021-09-06 01:57:28.204973
4	80	40	13	5	50	25	10	1	COMMON	czp10.yml	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	75	PISTOL	1	1	1	5	1	39	0	33	1	0	3	80	1	2	3	1	1	0	1	1	1	0	0	0	0	1	1	0	0	0	1	0	0	0	1	1	1	1	0	2021-09-06 01:59:56.640283	2021-09-06 01:59:56.640283
5	90	10	0	0	80	40	0	0	COMMON	ump45.yml	SUB_MACHINE_GUN	5	PN/P	UMP-45	32	400	SUB_MACHINE_GUN	30.8099999999999987	31	1	1	1	30	1	29	3	4.29999999999999982	5	1	1357	20	10	6	1	1	1	1	1	1	1	1	1	0	1	0	0	1	1	0	1	0	1	1	1	1	0	2021-09-06 02:00:02.293957	2021-09-06 02:00:02.293957
6	90	10	0	0	90	40	9	0	COMMON	sasg12.yml	SHOTGUN	3	R.S.S.	SASG-12	6	1	SHOTGUN	70	6	2	34	1	80	1	53	1	4.29999999999999982	1	19	918	10	3	1	1	1	0	1	0	1	1	1	1	1	1	1	1	0	1	0	1	1	0	0	1	0	0	2021-09-06 02:08:25.716967	2021-09-06 02:08:25.716967
7	80	40	13	5	50	25	10	1	COMMON	glock.yml	PISTOL	8	TN3 SMITH-x Industrial	Glock	41	75	PISTOL	1	9	1	5	1	21	1	1	1	1	3	80	40	3	4	16	1	1	0	1	1	1	0	1	1	1	1	1	0	0	0	0	1	1	1	1	1	1	0	2021-09-06 02:08:49.453172	2021-09-06 02:08:49.453172
8	0	20	90	90	10	20	90	90	COMMON	psg1.yml	SNIPER	6	PF-TDN	PSG1	8	70	SNIPER	1	7	16	19	3	1	0	23	4	1	12	13	1818	512	2	53	1	1	0	1	1	1	0	0	1	0	1	1	0	0	1	0	1	1	0	0	1	1	0	2021-09-06 02:09:56.904971	2021-09-06 02:09:56.904971
9	90	10	0	0	80	40	0	0	COMMON	fmg9.yml	SUB_MACHINE_GUN	5	DXGR-1	FMG-9	5	888	SUB_MACHINE_GUN	33	1	1	14	1	30	1	13	1	1	5	180	1	33	6	6	1	0	1	0	1	1	1	0	1	1	1	0	0	1	1	0	1	0	1	1	1	1	0	2021-09-06 02:11:28.277761	2021-09-06 02:11:28.277761
10	90	10	0	0	90	40	9	0	COMMON	sasg12.yml	SHOTGUN	3	R.S.S.	SASG-12	6	40	SHOTGUN	1	9	2	34	1	1	1	73	2	7	2	1	918	12	3	20	1	0	1	1	0	0	0	1	1	0	0	0	1	1	1	0	0	1	1	1	0	1	0	2021-09-06 02:48:16.222825	2021-09-06 02:48:16.222825
11	80	40	13	5	50	25	10	1	COMMON	magnum-revolver.yml	PISTOL	8	TN3 SMITH-x Industrial	Magnum Revolver	40	75	PISTOL	20.5	6	1	5	1	36	1	33	1	0	1	1	40	3	1	26	1	1	1	1	0	0	0	1	1	0	1	1	1	1	0	0	1	0	0	1	1	0	0	2021-09-06 02:48:40.192837	2021-09-06 02:48:40.192837
12	90	10	0	0	90	40	9	0	COMMON	sasg12.yml	SHOTGUN	3	R.S.S.	SASG-12	6	40	SHOTGUN	70	6	2	1	1	129	0	66	2	4.29999999999999982	2	1	920	10	1	20	0	0	1	1	1	1	1	1	1	1	1	1	0	0	0	0	1	0	1	1	0	0	0	2021-09-06 02:53:24.30776	2021-09-06 02:53:24.30776
13	90	10	0	0	90	40	9	0	COMMON	saiga12.yml	SHOTGUN	3	R.S.S.	SAIGA-12	39	40	SHOTGUN	70	8	2	34	1	80	0	1	1	4.29999999999999982	3	10	918	1	3	20	1	1	0	1	1	1	0	1	1	1	0	0	0	1	0	0	1	1	1	0	1	0	0	2021-09-06 03:02:35.527086	2021-09-06 03:02:35.527086
14	80	40	13	5	50	25	10	1	COMMON	glock.yml	PISTOL	8	TN3 SMITH-x Industrial	Glock	41	1	PISTOL	20.5	1	1	9	1	20	1	33	1	0	5	80	1	1	1	16	1	0	1	0	1	1	1	1	1	1	1	1	0	0	0	0	1	1	0	1	1	0	0	2021-09-06 03:04:47.023351	2021-09-06 03:04:47.023351
15	90	10	0	0	90	40	9	0	COMMON	sasg12.yml	SHOTGUN	3	R.S.S.	SASG-12	6	1	SHOTGUN	73	6	2	1	1	80	0	1	4	1	2	10	918	10	3	20	1	1	1	1	1	1	1	0	1	1	0	1	1	1	1	0	1	1	0	1	1	0	0	2021-09-06 03:05:55.639243	2021-09-06 03:05:55.639243
16	90	10	0	0	90	40	9	0	COMMON	sasg12.yml	SHOTGUN	3	R.S.S.	SASG-12	6	40	SHOTGUN	70	6	2	34	1	80	1	53	1	4.29999999999999982	2	1	918	1	5	1	1	0	1	1	0	1	1	1	1	0	1	1	1	0	1	0	0	1	1	0	1	1	0	2021-09-06 03:06:30.49336	2021-09-06 03:06:30.49336
17	23.0100990000000003	0	0	0	50	0	0	0	COMMON	desert-eagle.yml	PISTOL	8	LX Industries	Desert Eagle	47	50	PISTOL	0	7	6	1	1	35	1	61	1	0	5	8	1	2	5	20	1	0	1	1	1	0	1	1	1	1	1	0	1	1	1	0	0	1	0	0	0	1	0	2021-09-06 03:06:37.447102	2021-09-06 03:06:37.447102
18	80	40	13	5	50	25	10	1	COMMON	glock.yml	PISTOL	8	TN3 SMITH-x Industrial	Glock	41	1	PISTOL	1	1	1	5	1	20	1	42	1	1	3	1	1	2	3	16	1	0	0	1	1	0	1	1	0	0	1	0	1	1	1	0	0	0	0	1	0	1	0	2021-09-06 03:08:25.046276	2021-09-06 03:08:25.046276
19	0	20	90	90	10	20	90	90	COMMON	l96aw.yml	SNIPER	6	Heckler and Koch	L96 Arctic Warfare	24	70	SNIPER	1	7	2	1	5	1	0	20	4	4.29999999999999982	11	10	1818	775	5	54	0	1	1	0	0	1	1	1	1	1	0	0	1	1	1	0	0	1	1	1	0	0	0	2021-09-06 03:08:36.809308	2021-09-06 03:08:36.809308
20	10	10	10	10	10	10	10	10	COMMON	552-commando.yml	ASSAULT_RIFLE	4	S1 Industries	552 Commando	69	220	ASSAULT_RIFLE	1	14	2	14	4	5	3.14000000000000012	3	3	4.29999999999999982	1	80	1	2	6	1	0	0	0	0	0	0	0	0	0	1	0	0	0	1	0	0	0	1	1	1	1	1	0	2021-09-06 03:13:23.838239	2021-09-06 03:13:23.838239
21	80	40	13	5	50	25	10	1	COMMON	uzi.yml	PISTOL	8	TN3 SMITH-x Industrial	A Uzi	43	75	MACHINE_PISTOL	10.5	1	1	5	1	1	1	33	1	0	3	80	40	3	3	16	0	1	0	1	0	1	1	1	1	1	1	1	0	1	1	0	1	1	0	0	1	1	0	2021-09-06 03:14:24.90574	2021-09-06 03:14:24.90574
22	0	20	90	90	10	20	90	90	COMMON	psg1.yml	SNIPER	6	PF-TDN	PSG1	8	1	SNIPER	0.810000000000000053	11	16	19	1	450	0	1	4	1	1	1	1	500	3	49	1	1	0	1	1	1	0	1	1	1	0	0	1	1	1	0	0	1	0	1	0	0	0	2021-09-06 03:17:24.397361	2021-09-06 03:17:24.397361
23	90	10	0	0	90	40	9	0	COMMON	saiga12.yml	SHOTGUN	3	R.S.S.	SAIGA-12	39	40	SHOTGUN	115	11	2	34	1	91	0	1	2	4.29999999999999982	3	1	918	1	1	20	1	1	0	1	1	0	0	0	1	0	0	0	1	0	1	0	1	0	1	1	1	1	0	2021-09-06 03:40:23.665692	2021-09-06 03:40:23.665692
24	90	10	0	0	90	40	9	0	COMMON	sasg12.yml	SHOTGUN	3	R.S.S.	SASG-12	6	40	SHOTGUN	70	1	2	34	1	1	0	1	3	7	2	10	918	10	3	1	1	0	0	1	1	1	0	0	0	1	0	1	1	0	1	0	1	1	1	0	0	1	0	2021-09-06 03:42:49.10483	2021-09-06 03:42:49.10483
25	90	10	0	0	90	40	9	0	COMMON	sasg12.yml	SHOTGUN	3	R.S.S.	SASG-12	6	40	SHOTGUN	70	8	2	34	1	117	1	53	2	1	3	10	918	13	1	20	0	1	1	1	1	0	1	1	1	0	1	0	0	1	1	0	0	1	1	0	0	1	0	2021-09-06 03:51:37.101185	2021-09-06 03:51:37.101185
26	90	10	0	0	80	40	0	0	COMMON	belt-fed-minigun.yml	LIGHT_MACHINE_GUN	10	TN-33Y	A TN-33Y Belt-Fed Minigun	23	999	LIGHT_MACHINE_GUN	3.81000000000000005	999	20	23	1	30	1	1	11	4.29999999999999982	33	180	1	26	1	140	1	1	0	1	0	1	1	0	1	0	0	1	1	1	0	0	1	1	1	0	1	1	0	2021-09-06 03:53:40.659097	2021-09-06 03:53:40.659097
27	90	10	0	0	80	40	0	0	COMMON	mp9.yml	MACHINE_PISTOL	9	Heckler and Koch	MP9	38	1	MACHINE_PISTOL	32	37	1	14	1	30	1	21	3	1	5	1	1	20	1	6	1	1	1	0	1	1	0	1	1	1	0	1	1	0	1	0	0	1	0	0	1	1	0	2021-09-06 03:54:00.480819	2021-09-06 03:54:00.480819
28	10	10	10	10	10	10	10	10	COMMON	552-commando.yml	ASSAULT_RIFLE	4	S1 Industries	552 Commando	69	220	ASSAULT_RIFLE	1	8	2	1	4	5	3.14000000000000012	3	1	4	8	80	24	1	1	1	1	1	0	1	1	0	1	1	1	1	0	1	1	1	1	0	1	0	0	1	1	1	0	2021-09-06 03:54:39.313207	2021-09-06 03:54:39.313207
29	80	40	13	5	50	25	10	1	COMMON	magnum-revolver.yml	PISTOL	8	TN3 SMITH-x Industrial	Magnum Revolver	40	75	PISTOL	33	1	1	7	1	1	0	33	1	1	3	80	1	2	1	16	1	1	1	1	0	0	1	1	1	0	0	0	0	0	0	0	0	0	0	1	0	1	0	2021-09-06 03:54:48.391041	2021-09-06 03:54:48.391041
30	10	10	10	10	10	10	10	10	COMMON	aug-a3.yml	ASSAULT_RIFLE	4	VI-AUG-IX	AUG A3	44	220	ASSAULT_RIFLE	4.23000000000000043	8	2	19	4	7	1	3	3	1	1	99	24	2	11	1	1	1	0	1	1	0	1	1	1	1	0	0	1	0	1	0	0	0	1	1	1	1	0	2021-09-06 03:58:07.458034	2021-09-06 03:58:07.458034
31	90	10	0	0	80	40	0	0	COMMON	belt-fed-minigun.yml	LIGHT_MACHINE_GUN	10	TN-33Y	A TN-33Y Belt-Fed Minigun	23	999	LIGHT_MACHINE_GUN	3.81000000000000005	999	20	1	1	30	0	23	7	4.29999999999999982	1	1	1	36	40	1	1	0	1	0	1	1	1	1	0	1	0	1	1	1	0	0	1	1	0	1	1	1	0	2021-09-06 04:00:21.032214	2021-09-06 04:00:21.032214
32	0	20	90	90	10	20	90	90	COMMON	l96aw.yml	SNIPER	6	Heckler and Koch	L96 Arctic Warfare	24	70	SNIPER	0.810000000000000053	7	2	19	5	450	1	21	1	1	8	59	2	500	1	1	2	1	1	1	5	1	0	1	0	0	2	1	1	1	1	0	0	1	2	1	0	1	0	2021-09-06 04:00:25.350172	2021-09-06 04:00:25.350172
33	90	10	0	0	80	40	0	0	COMMON	mp5.yml	SUB_MACHINE_GUN	5	Heckler and Koch	MP5	5	400	SUB_MACHINE_GUN	30.8099999999999987	2	1	14	1	1	0	16	3	4.29999999999999982	5	1	1	44	29	1	0	1	0	1	1	1	2	1	2	1	1	1	0	1	0	0	1	1	1	0	0	1	0	2021-09-06 04:00:53.142172	2021-09-06 04:00:53.142172
34	80	40	13	5	50	25	10	1	COMMON	magnum-revolver.yml	PISTOL	8	TN3 SMITH-x Industrial	Magnum Revolver	40	1	PISTOL	20	10	1	1	1	1	0	33	1	0	1	80	40	2	5	16	1	1	5	0	1	1	1	1	0	2	1	0	2	1	0	0	1	0	9	2	1	0	0	2021-09-06 04:01:03.958174	2021-09-06 04:01:03.958174
35	90	10	0	0	80	40	0	0	COMMON	mp5.yml	SUB_MACHINE_GUN	5	Heckler and Koch	MP5	5	1162	SUB_MACHINE_GUN	1	82	1	14	2	30	0	66	4	4.29999999999999982	10	1	1	1	6	6	1	1	1	1	1	1	2	1	1	0	2	0	1	0	2	0	1	0	1	2	2	0	0	2021-09-06 04:01:19.683189	2021-09-06 04:01:19.683189
36	80	40	13	5	50	25	10	1	COMMON	glock.yml	PISTOL	8	TN3 SMITH-x Industrial	Glock	41	75	PISTOL	1	1	1	5	1	20	2	1	1	2	3	1	1	2	3	16	1	2	1	0	2	1	1	1	2	1	1	1	4	0	1	0	1	2	0	1	0	1	0	2021-09-06 04:01:46.176273	2021-09-06 04:01:46.176273
37	80	40	13	5	50	25	10	1	COMMON	uzi.yml	PISTOL	8	TN3 SMITH-x Industrial	A Uzi	43	75	MACHINE_PISTOL	10.5	13	1	1	1	23	0	1	2	0	1	1	40	3	3	1	1	0	2	5	2	0	1	1	1	1	0	1	1	1	2	0	1	0	0	0	0	1	0	2021-09-06 04:01:51.777233	2021-09-06 04:01:51.777233
38	80	40	13	5	50	25	10	1	COMMON	czp10.yml	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	75	PISTOL	20.5	12	1	5	2	3	0	33	1	1	1	80	1	1	3	1	0	1	0	4	1	0	1	1	3	2	0	2	3	1	2	0	0	0	0	1	1	0	0	2021-09-06 04:03:22.246198	2021-09-06 04:03:22.246198
39	10	10	10	10	10	10	10	10	COMMON	g36c.yml	ASSAULT_RIFLE	4	Heckler & Koch	G36C Assault Rifle	46	1	ASSAULT_RIFLE	4	8	2	14	2	5	1	1	1	4.29999999999999982	5	80	24	2	6	19	1	1	1	2	2	0	1	0	2	1	2	1	1	2	2	0	0	1	2	1	0	0	0	2021-09-06 04:03:32.066198	2021-09-06 04:03:32.066198
40	90	10	0	0	90	40	9	0	COMMON	saiga12.yml	SHOTGUN	3	R.S.S.	SAIGA-12	39	100	SHOTGUN	169	1	2	34	1	80	0	1	4	12	1	10	918	10	8	26	1	1	1	0	1	2	1	2	4	1	1	1	0	0	0	0	1	4	2	0	0	2	0	2021-09-06 04:11:31.980285	2021-09-06 04:11:31.980285
41	10	10	10	10	10	10	10	10	COMMON	m16a4.yml	ASSAULT_RIFLE	4	Standard Issue	M16A4 Assault Rifle	25	220	ASSAULT_RIFLE	4.23000000000000043	1	2	14	1	5	3.14000000000000012	1	3	7	7	1	1	4	6	1	1	1	1	1	1	0	2	2	2	1	2	1	1	1	2	0	1	1	0	1	1	5	0	2021-09-06 04:12:25.294101	2021-09-06 04:12:25.294101
42	23.0100990000000003	0	0	0	50	0	0	0	COMMON	desert-eagle.yml	PISTOL	8	LX Industries	Desert Eagle	47	189	PISTOL	0	14	6	2	1	2	2	57	1	8	5	9	1	2	1	36	2	1	7	1	2	1	3	7	2	2	2	0	1	3	1	0	9	1	0	3	1	0	0	2021-09-06 04:13:09.227471	2021-09-06 04:13:09.227471
43	90	10	0	0	80	40	0	0	COMMON	belt-fed-minigun.yml	LIGHT_MACHINE_GUN	10	TN-33Y	A TN-33Y Belt-Fed Minigun	23	999	LIGHT_MACHINE_GUN	3.81000000000000005	2	20	14	1	30	2	13	2	9	72	180	1	1	47	2	2	2	2	1	0	4	2	1	2	1	1	0	1	0	1	0	1	0	1	2	2	2	0	2021-09-06 04:13:23.551378	2021-09-06 04:13:23.551378
44	80	40	13	5	50	25	10	1	COMMON	czp10.yml	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	75	PISTOL	20.5	26	1	5	1	2	1	33	1	1	1	6	40	2	1	16	4	1	9	0	2	3	1	0	0	1	0	1	0	2	1	0	2	3	0	1	3	1	0	2021-09-06 04:13:44.591935	2021-09-06 04:13:44.591935
45	90	10	0	0	90	40	9	0	COMMON	saiga12.yml	SHOTGUN	3	R.S.S.	SAIGA-12	39	40	SHOTGUN	586	6	2	1	2	1	2	2	2	3	2	29	918	10	4	20	1	2	1	3	2	0	0	2	7	1	0	1	2	2	2	0	0	6	0	1	0	4	0	2021-09-06 04:21:24.352496	2021-09-06 04:21:24.352496
46	10	10	10	10	10	10	10	10	COMMON	552-commando.yml	ASSAULT_RIFLE	4	S1 Industries	552 Commando	69	2	ASSAULT_RIFLE	16	2	2	2	4	5	3.14000000000000012	6	2	4.29999999999999982	1	80	24	6	54	31	2	2	2	3	0	2	5	0	0	2	1	0	1	2	1	0	0	0	0	1	2	2	0	2021-09-06 04:21:40.381543	2021-09-06 04:21:40.381543
47	23.0100990000000003	0	0	0	50	0	0	0	COMMON	desert-eagle.yml	PISTOL	8	LX Industries	Desert Eagle	47	50	PISTOL	0	7	6	11	3	35	1	125	1	1	2	10	2	2	2	1	0	0	0	0	0	0	0	0	0	0	2	0	2	2	1	0	6	3	2	0	0	0	0	2021-09-06 04:22:42.764595	2021-09-06 04:22:42.764595
48	0	20	90	90	10	20	90	90	COMMON	psg1.yml	SNIPER	6	PF-TDN	PSG1	8	70	SNIPER	2	2	16	1	3	1620	2	13	4	1	9	10	1818	1	2	49	1	0	1	27	1	1	3	4	0	0	0	3	2	2	0	0	2	2	3	1	2	3	0	2021-09-06 04:22:46.863401	2021-09-06 04:22:46.863401
49	80	40	13	5	50	25	10	1	COMMON	uzi.yml	PISTOL	8	TN3 SMITH-x Industrial	A Uzi	43	75	MACHINE_PISTOL	30	13	1	5	1	62	0	3	2	0	1	80	1	7	2	1	1	5	2	1	2	2	0	2	3	0	1	1	0	19	1	0	0	1	4	0	2	0	0	2021-09-06 04:23:01.641628	2021-09-06 04:23:01.641628
50	0	20	90	90	10	20	90	90	COMMON	psg1.yml	SNIPER	6	PF-TDN	PSG1	8	90	SNIPER	0.810000000000000053	21	16	8	3	450	0	2	7	15	1	12	1818	500	2	49	5	2	1	2	0	1	1	1	1	0	0	2	6	2	2	0	0	2	2	1	2	1	0	2021-09-06 04:23:35.951147	2021-09-06 04:23:35.951147
51	90	10	0	0	90	40	9	0	COMMON	sasg12.yml	SHOTGUN	3	R.S.S.	SASG-12	6	40	SHOTGUN	115	17	2	34	1	80	1	53	4	10	3	20	918	10	4	2	2	1	0	0	2	10	2	1	1	0	1	0	2	1	1	0	0	3	2	0	2	7	0	2021-09-06 04:27:50.496627	2021-09-06 04:27:50.496627
52	90	10	0	0	80	40	0	0	COMMON	mp5.yml	SUB_MACHINE_GUN	5	Heckler and Koch	MP5	5	400	SUB_MACHINE_GUN	461	31	1	1	1	30	2	13	10	4.29999999999999982	1	2	818	2	21	19	2	4	2	2	1	1	1	2	2	0	2	3	0	2	3	0	0	0	1	1	3	3	0	2021-09-06 04:28:20.129768	2021-09-06 04:28:20.129768
53	90	10	0	0	90	40	9	0	COMMON	saiga12.yml	SHOTGUN	3	R.S.S.	SAIGA-12	39	147	SHOTGUN	70	62	2	2	2	80	1	53	2	4.29999999999999982	2	10	918	21	3	2	0	1	2	3	2	0	3	2	0	1	2	1	3	2	2	0	0	0	0	1	0	0	0	2021-09-07 10:38:58.671814	2021-09-07 10:38:58.671814
54	80	40	13	5	50	25	10	1	COMMON	czp10.yml	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	1	PISTOL	20.5	2	1	13	8	20	0	33	2	2	6	1	40	1	3	2	0	0	2	2	2	1	34	2	0	0	1	2	0	0	0	0	1	9	3	0	2	2	0	2021-09-07 10:39:50.158031	2021-09-07 10:39:50.158031
55	80	40	13	5	50	25	10	1	COMMON	uzi.yml	PISTOL	8	TN3 SMITH-x Industrial	A Uzi	43	3	MACHINE_PISTOL	20	13	1	2	5	20	0	42	1	0	9	290	40	1	3	3	2	1	1	2	0	11	1	0	1	0	2	2	0	1	1	0	7	2	0	1	2	2	0	2021-09-07 10:40:02.18301	2021-09-07 10:40:02.18301
56	90	10	0	0	90	40	9	0	COMMON	sasg12.yml	SHOTGUN	3	R.S.S.	SASG-12	6	47	SHOTGUN	2	6	2	34	3	1	0	309	4	4.29999999999999982	2	1	918	82	1	59	1	1	1	1	0	2	0	1	1	2	2	0	2	0	0	0	2	2	1	1	3	1	0	2021-09-07 10:48:10.468071	2021-09-07 10:48:10.468071
57	90	10	0	0	90	40	9	0	COMMON	sasg12.yml	SHOTGUN	3	R.S.S.	SASG-12	6	40	SHOTGUN	112	2	2	34	1	147	0	1	2	17	2	26	12	10	3	62	2	0	7	2	2	11	7	0	0	2	0	1	2	3	0	0	1	3	1	0	1	2	0	2021-09-07 10:54:48.821086	2021-09-07 10:54:48.821086
58	90	10	0	0	80	40	0	0	COMMON	mp9.yml	MACHINE_PISTOL	9	Heckler and Koch	MP9	38	1	MACHINE_PISTOL	30.8099999999999987	1	1	2	3	108	2	1	8	4.29999999999999982	2	180	818	20	6	14	17	0	2	2	5	2	2	11	1	0	0	0	7	2	1	0	4	2	0	1	2	4	0	2021-09-07 10:55:09.614216	2021-09-07 10:55:09.614216
59	10	10	10	10	10	10	10	10	COMMON	scarh.yml	ASSAULT_RIFLE	4	SK-10	SCAR-H Assault Rifle	31	220	ASSAULT_RIFLE	16	1	2	14	16	1	3.14000000000000012	1	2	4.29999999999999982	2	1	77	4	6	2	3	0	1	1	22	2	0	14	0	2	5	7	1	2	2	0	0	2	0	0	3	0	0	2021-09-07 10:55:32.641118	2021-09-07 10:55:32.641118
60	10	10	10	10	10	10	10	10	COMMON	552-commando.yml	ASSAULT_RIFLE	4	S1 Industries	552 Commando	69	458	ASSAULT_RIFLE	1	8	2	14	2	2	3.14000000000000012	1	2	4.29999999999999982	18	2	75	3	6	1	1	1	1	3	1	2	0	1	2	0	0	0	0	0	0	0	0	2	0	2	0	0	0	2021-09-07 10:56:19.699137	2021-09-07 10:56:19.699137
61	10	10	10	10	10	10	10	10	COMMON	tar21.yml	ASSAULT_RIFLE	4	SK-10	TAR-21 Assault Rifle	30	2	ASSAULT_RIFLE	17	8	2	3	2	5	1	2	1	15	5	376	2	2	52	38	0	0	3	1	2	1	0	2	4	0	1	0	1	0	0	0	0	2	2	40	2	1	0	2021-09-07 10:57:40.930179	2021-09-07 10:57:40.930179
62	0	20	90	90	10	20	90	90	COMMON	l96aw.yml	SNIPER	6	Heckler and Koch	L96 Arctic Warfare	24	1	SNIPER	2	7	2	19	3	2	2	3	4	1	8	16	1	500	8	54	2	0	2	1	0	3	4	1	19	0	1	3	1	1	0	0	0	19	4	1	2	0	0	2021-09-07 10:57:55.801265	2021-09-07 10:57:55.801265
63	90	10	0	0	80	40	0	0	COMMON	mk46.yml	LIGHT_MACHINE_GUN	10	TN-33Y	MK-46 Light Machine Gun	78	3717	LIGHT_MACHINE_GUN	3.81000000000000005	3278	20	21	3	30	0	2	2	3	33	2	818	149	186	1	2	9	2	8	7	0	2	3	49	2	1	2	0	0	2	0	9	0	2	0	0	1	0	2021-09-07 10:58:07.221686	2021-09-07 10:58:07.221686
64	23.0100990000000003	0	0	0	50	0	0	0	COMMON	desert-eagle.yml	PISTOL	8	LX Industries	Desert Eagle	47	1	PISTOL	0	7	6	11	1	35	1	2	1	0	2	23	2	2	18	20	0	2	1	2	4	0	1	17	1	1	7	0	1	2	1	0	6	1	0	13	2	0	0	2021-09-07 10:58:18.857191	2021-09-07 10:58:18.857191
65	90	10	0	0	90	40	9	0	COMMON	saiga12.yml	SHOTGUN	3	R.S.S.	SAIGA-12	39	40	SHOTGUN	70	6	2	34	1	1	0	1	7	4.29999999999999982	1	1	1390	1	1	20	0	1	1	0	1	1	1	1	0	1	1	0	1	1	0	0	1	0	1	0	0	1	0	2021-09-07 11:35:16.81688	2021-09-07 11:35:16.81688
66	0	20	90	90	10	20	90	90	COMMON	psg1.yml	SNIPER	6	PF-TDN	PSG1	8	92	SNIPER	0.810000000000000053	12	16	19	3	1	0	13	1	1	9	18	1818	500	3	49	1	1	1	1	0	1	1	1	0	1	0	0	1	1	1	0	1	0	1	1	1	0	0	2021-09-07 11:37:13.543964	2021-09-07 11:37:13.543964
67	90	10	0	0	90	40	9	0	COMMON	saiga12.yml	SHOTGUN	3	R.S.S.	SAIGA-12	39	1	SHOTGUN	70	1	2	34	1	1	1	53	2	4.29999999999999982	3	10	918	10	1	34	0	0	1	1	0	1	1	0	1	0	0	0	0	0	0	0	0	0	0	0	0	0	0	2021-09-07 11:38:57.601084	2021-09-07 11:38:57.601084
68	80	40	13	5	50	25	10	1	COMMON	ppk.yml	PISTOL	8	TN3 SMITH-x Industrial	Silenced PPK	42	124	PISTOL	20.5	9	1	1	1	20	0	43	1	0	1	1	40	2	3	16	1	0	1	1	1	1	1	1	1	0	0	0	1	1	1	0	1	0	0	1	1	1	0	2021-09-07 11:40:06.99814	2021-09-07 11:40:06.99814
69	80	40	13	5	50	25	10	1	COMMON	uzi.yml	PISTOL	8	TN3 SMITH-x Industrial	A Uzi	43	75	MACHINE_PISTOL	14	13	1	8	1	20	1	35	1	0	3	1	40	1	3	16	0	1	1	1	1	0	1	1	1	1	1	0	0	1	1	0	1	1	1	1	0	0	0	2021-09-07 11:41:52.537095	2021-09-07 11:41:52.537095
70	90	10	0	0	90	40	9	0	COMMON	saiga12.yml	SHOTGUN	3	R.S.S.	SAIGA-12	39	66	SHOTGUN	1	7	2	1	1	80	0	53	2	5	2	10	918	10	1	20	1	1	0	1	1	0	0	1	0	1	1	0	0	0	1	0	1	1	1	1	1	0	0	2021-09-07 13:53:42.597594	2021-09-07 13:53:42.597594
71	90	10	0	0	80	40	0	0	COMMON	mk46.yml	LIGHT_MACHINE_GUN	10	TN-33Y	MK-46 Light Machine Gun	78	999	LIGHT_MACHINE_GUN	3.81000000000000005	2564	20	1	1	30	0	24	13	4.29999999999999982	44	180	818	53	40	1	1	0	1	1	1	0	1	0	1	0	1	1	1	1	0	0	1	1	0	0	1	1	0	2021-09-07 13:54:19.914426	2021-09-07 13:54:19.914426
72	90	10	0	0	90	40	9	0	COMMON	sasg12.yml	SHOTGUN	3	R.S.S.	SASG-12	6	1	SHOTGUN	136	6	2	34	1	80	1	53	1	4.29999999999999982	2	1	1	10	1	1	1	0	1	1	1	1	0	1	1	0	1	1	1	0	0	0	0	1	1	0	1	1	0	2021-09-07 14:06:46.882602	2021-09-07 14:06:46.882602
73	90	10	0	0	80	40	0	0	COMMON	mk46.yml	LIGHT_MACHINE_GUN	10	TN-33Y	MK-46 Light Machine Gun	78	999	LIGHT_MACHINE_GUN	3.81000000000000005	999	20	22	1	30	0	13	7	4.29999999999999982	33	180	818	20	40	140	1	1	1	1	0	1	1	1	0	0	1	1	1	0	1	0	0	0	1	0	1	1	0	2021-09-07 14:07:05.080684	2021-09-07 14:07:05.080684
74	80	40	13	5	50	25	10	1	COMMON	ppk.yml	PISTOL	8	TN3 SMITH-x Industrial	Silenced PPK	42	75	PISTOL	20.5	9	1	1	1	1	0	33	1	0	1	80	40	4	7	19	2	0	1	1	0	2	1	2	1	0	1	1	0	0	0	0	1	2	1	2	1	0	0	2021-09-07 14:07:13.03584	2021-09-07 14:07:13.03584
75	10	10	10	10	10	10	10	10	COMMON	tar21.yml	ASSAULT_RIFLE	4	SK-10	TAR-21 Assault Rifle	30	220	ASSAULT_RIFLE	4.23000000000000043	8	2	14	1	5	3.14000000000000012	1	3	4.29999999999999982	11	80	2	2	1	1	0	1	1	1	1	1	4	0	1	1	1	1	0	1	0	0	1	1	1	0	2	2	0	2021-09-07 14:08:06.897732	2021-09-07 14:08:06.897732
76	80	40	13	5	50	25	10	1	COMMON	uzi.yml	PISTOL	8	TN3 SMITH-x Industrial	A Uzi	43	1	MACHINE_PISTOL	10.5	13	1	5	1	20	1	33	1	1	1	80	2	2	1	1	1	1	2	1	1	1	1	1	1	2	2	0	1	2	1	0	0	2	1	0	1	1	0	2021-09-07 14:08:08.725752	2021-09-07 14:08:08.725752
77	90	10	0	0	80	40	0	0	COMMON	mk46.yml	LIGHT_MACHINE_GUN	10	TN-33Y	MK-46 Light Machine Gun	78	999	LIGHT_MACHINE_GUN	3.81000000000000005	1	20	14	1	30	1	1	8	4.29999999999999982	33	353	818	45	100	140	1	1	1	1	0	1	1	2	1	0	1	0	1	1	1	0	0	1	0	0	2	2	0	2021-09-07 14:09:34.865872	2021-09-07 14:09:34.865872
78	90	10	0	0	80	40	0	0	COMMON	p90.yml	SUB_MACHINE_GUN	5	Heckler and Koch	Heckler and Koch P90	21	1	SUB_MACHINE_GUN	1	1	1	14	1	84	0	30	3	4.29999999999999982	1	1	818	20	10	6	1	1	1	2	1	1	1	1	1	1	1	1	0	1	1	0	4	0	1	1	1	0	0	2021-09-07 14:09:53.320895	2021-09-07 14:09:53.320895
79	80	40	13	5	50	25	10	1	COMMON	czp10.yml	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	2	PISTOL	20.5	21	1	5	3	20	1	33	1	0	8	276	40	2	4	16	2	1	1	1	0	0	1	1	2	1	1	1	2	1	1	0	0	0	1	0	1	0	0	2021-09-07 14:10:19.630963	2021-09-07 14:10:19.630963
80	90	10	0	0	90	40	9	0	COMMON	sasg12.yml	SHOTGUN	3	R.S.S.	SASG-12	6	154	SHOTGUN	70	6	2	8	1	2	0	2	5	4.29999999999999982	8	2	918	10	2	20	2	2	87	1	1	9	0	8	9	4	0	0	3	1	0	0	2	0	1	6	1	1	0	2021-09-07 15:34:49.372159	2021-09-07 15:34:49.372159
81	90	10	0	0	90	40	9	0	COMMON	sasg12.yml	SHOTGUN	3	R.S.S.	SASG-12	6	40	SHOTGUN	1082	1	2	34	2	329	0	53	2	4.29999999999999982	2	81	918	2	3	2	8	2	1	2	1	1	1	1	1	3	1	1	0	4	2	0	4	4	1	4	2	1	0	2021-09-07 15:44:26.523537	2021-09-07 15:44:26.523537
82	90	10	0	0	80	40	0	0	COMMON	ump45.yml	SUB_MACHINE_GUN	5	PN/P	UMP-45	32	400	SUB_MACHINE_GUN	131	120	1	14	1	2	2	13	2	1	5	180	2	1	6	58	1	2	1	3	0	0	2	1	3	1	1	0	1	3	4	0	0	1	0	3	5	2	0	2021-09-07 15:47:20.881538	2021-09-07 15:47:20.881538
83	90	10	0	0	90	40	9	0	COMMON	sasg12.yml	SHOTGUN	3	R.S.S.	SASG-12	6	40	SHOTGUN	70	1	2	1	1	80	1	1	5	2	2	2	918	7	11	20	2	3	7	0	6	1	2	0	18	0	2	3	2	1	0	0	2	2	4	0	0	2	0	2021-09-11 02:31:04.075813	2021-09-11 02:31:04.075813
84	80	40	13	5	50	25	10	1	COMMON	uzi.yml	PISTOL	8	TN3 SMITH-x Industrial	A Uzi	43	1	MACHINE_PISTOL	2	50	1	16	2	20	0	1	2	2	5	250	2	5	2	16	1	2	2	1	2	71	0	2	2	0	5	1	0	0	3	0	4	0	30	1	3	2	0	2021-09-11 02:31:11.598437	2021-09-11 02:31:11.598437
85	90	10	0	0	80	40	0	0	COMMON	p90.yml	SUB_MACHINE_GUN	5	Heckler and Koch	Heckler and Koch P90	21	400	SUB_MACHINE_GUN	2	31	1	14	2	2	0	18	1	1	2	180	818	76	10	2	3	0	7	0	22	3	1	21	1	2	0	1	3	6	0	0	4	11	0	2	0	17	0	2021-09-11 22:08:15.984399	2021-09-11 22:08:15.984399
86	90	10	0	0	90	40	9	0	COMMON	sasg12.yml	SHOTGUN	3	R.S.S.	SASG-12	6	166	SHOTGUN	2	26	2	2	2	80	3	53	2	2	2	15	2778	10	7	37	1	1	2	0	4	3	10	1	0	1	2	0	4	2	9	0	0	1	0	0	2	1	0	2021-09-11 22:34:57.993684	2021-09-11 22:34:57.993684
87	90	10	0	0	90	40	9	0	COMMON	sasg12.yml	SHOTGUN	3	R.S.S.	SASG-12	6	40	SHOTGUN	101	6	2	1	1	80	0	53	2	4.29999999999999982	2	10	918	1	1	1	1	1	0	1	1	0	1	1	1	1	1	0	1	1	0	0	1	0	1	0	1	1	0	2021-09-11 22:43:30.863845	2021-09-11 22:43:30.863845
88	90	10	0	0	90	40	9	0	COMMON	sasg12.yml	SHOTGUN	3	R.S.S.	SASG-12	6	40	SHOTGUN	70	6	2	140	2	2	0	53	7	31	3	10	1	17	5	20	1	2	2	1	2	2	0	1	3	2	2	2	0	0	3	0	3	8	3	2	0	0	0	2021-09-14 23:05:31.476199	2021-09-14 23:05:31.476199
89	0	20	90	90	10	20	90	90	COMMON	psg1.yml	SNIPER	6	PF-TDN	PSG1	8	70	SNIPER	1	16	16	19	3	82	8	60	1	4.29999999999999982	17	10	1818	2	2	29	2	12	0	0	2	4	4	3	1	1	2	4	2	9	2	0	0	10	2	0	2	0	0	2021-09-14 23:08:43.094794	2021-09-14 23:08:43.094794
90	90	10	0	0	90	40	9	0	COMMON	sasg12.yml	SHOTGUN	3	R.S.S.	SASG-12	6	2	SHOTGUN	7	46	2	34	1	80	0	53	6	4.29999999999999982	5	10	2	10	7	811	0	0	0	0	0	0	0	0	0	4	4	9	2	4	0	0	2	0	0	2	2	1	0	2021-09-14 23:18:08.304107	2021-09-14 23:18:08.304107
91	23.0100990000000003	0	0	0	50	0	0	0	COMMON	desert-eagle.yml	PISTOL	8	LX Industries	Desert Eagle	47	50	PISTOL	0	2	6	2	1	146	2	2	1	4	4	2	8	2	5	20	1	2	2	8	6	2	1	12	0	1	3	0	1	0	1	0	2	0	1	1	1	15	0	2021-09-14 23:18:30.652122	2021-09-14 23:18:30.652122
92	90	10	0	0	90	40	9	0	COMMON	saiga12.yml	SHOTGUN	3	R.S.S.	SAIGA-12	39	40	SHOTGUN	1	6	2	34	1	118	0	53	2	1	2	10	1	12	3	20	1	0	1	1	1	1	1	1	1	1	0	0	0	1	0	0	0	0	1	0	1	0	0	2021-09-14 23:35:49.479445	2021-09-14 23:35:49.479445
93	80	40	13	5	50	25	10	1	COMMON	ppk.yml	PISTOL	8	TN3 SMITH-x Industrial	Silenced PPK	42	75	PISTOL	38	1	1	5	1	1	0	1	1	0	3	93	40	3	3	23	1	0	1	1	1	1	0	0	1	1	1	1	0	1	1	0	0	0	1	1	1	1	0	2021-09-14 23:35:59.910515	2021-09-14 23:35:59.910515
94	90	10	0	0	90	40	9	0	COMMON	m3.yml	SHOTGUN	3	GBNT-3	M3	28	40	SHOTGUN	125	6	2	34	1	144	0	53	2	1	1	1	918	10	1	1	1	1	1	1	0	0	1	0	1	0	0	0	0	0	0	0	0	1	0	0	1	0	0	2021-09-14 23:39:15.773517	2021-09-14 23:39:15.773517
95	90	10	0	0	90	40	9	0	COMMON	saiga12.yml	SHOTGUN	3	R.S.S.	SAIGA-12	39	40	SHOTGUN	70	6	2	1	1	1	2	1	2	6	2	10	1719	1	3	84	1	2	1	2	1	1	0	1	0	2	1	0	1	2	1	0	1	1	0	0	1	0	0	2021-09-14 23:42:41.011863	2021-09-14 23:42:41.011863
96	90	10	0	0	80	40	0	0	COMMON	ump45.yml	SUB_MACHINE_GUN	5	PN/P	UMP-45	32	400	SUB_MACHINE_GUN	30.8099999999999987	46	1	14	1	30	1	13	3	4.29999999999999982	2	8	2	20	1	19	2	8	0	2	1	1	2	2	2	1	0	2	43	3	1	0	2	0	0	1	3	0	0	2021-09-14 23:42:59.549552	2021-09-14 23:42:59.549552
97	90	10	0	0	90	40	9	0	COMMON	sasg12.yml	SHOTGUN	3	R.S.S.	SASG-12	6	40	SHOTGUN	70	1	2	34	3	80	0	53	2	10	4	198	2563	10	7	4	1	2	2	1	2	0	2	3	1	2	1	1	2	2	0	0	1	5	0	2	0	0	0	2021-09-14 23:43:28.098499	2021-09-14 23:43:28.098499
98	0	20	90	90	10	20	90	90	COMMON	xm109.yml	SNIPER	6	Heckler and Koch	XM109 Sniper Rifle	13	70	SNIPER	2	1	2	19	3	450	0	13	12	4.29999999999999982	22	2	1818	6001	3	159	3	1	7	3	2	0	1	2	2	8	0	1	2	1	3	0	1	2	1	1	2	0	0	2021-09-14 23:43:44.450572	2021-09-14 23:43:44.450572
99	80	40	13	5	50	25	10	1	COMMON	glock.yml	PISTOL	8	TN3 SMITH-x Industrial	Glock	41	75	PISTOL	20.5	2	1	5	1	20	0	33	2	2	2	80	329	6	1	33	3	1	1	1	2	0	0	1	2	0	2	0	3	1	2	0	2	2	2	2	0	1	0	2021-09-14 23:44:17.215558	2021-09-14 23:44:17.215558
100	90	10	0	0	90	40	9	0	COMMON	sasg12.yml	SHOTGUN	3	R.S.S.	SASG-12	6	127	SHOTGUN	70	12	2	34	18	80	2	7	2	7	2	10	918	10	3	5	1	2	2	1	0	1	1	1	0	4	1	1	2	2	4	0	0	1	0	4	2	6	0	2021-09-14 23:44:37.180693	2021-09-14 23:44:37.180693
101	80	40	13	5	50	25	10	1	COMMON	uzi.yml	PISTOL	8	TN3 SMITH-x Industrial	A Uzi	43	75	MACHINE_PISTOL	51	13	1	5	1	2	1	33	8	1	3	80	144	1	3	72	4	2	4	2	2	1	3	0	0	2	3	2	1	2	1	0	0	1	1	2	1	2	0	2021-09-14 23:44:50.870736	2021-09-14 23:44:50.870736
102	90	10	0	0	80	40	0	0	COMMON	mp9.yml	MACHINE_PISTOL	9	Heckler and Koch	MP9	38	1373	MACHINE_PISTOL	30.8099999999999987	31	1	1	2	30	2	13	2	4	12	1	2774	72	1	2	13	2	0	2	0	1	1	2	6	3	0	2	4	1	0	0	1	2	0	1	1	2	0	2021-09-14 23:44:58.487728	2021-09-14 23:44:58.487728
103	90	10	0	0	80	40	0	0	COMMON	p90.yml	SUB_MACHINE_GUN	5	Heckler and Koch	Heckler and Koch P90	21	400	SUB_MACHINE_GUN	2	89	1	62	1	30	0	13	3	33	2	180	1	67	10	2	1	3	15	2	2	2	1	9	2	2	0	3	0	2	4	0	2	0	2	2	2	2	0	2021-09-14 23:45:13.896704	2021-09-14 23:45:13.896704
104	80	40	13	5	50	25	10	1	COMMON	uzi.yml	PISTOL	8	TN3 SMITH-x Industrial	A Uzi	43	195	MACHINE_PISTOL	10.5	2	1	2	2	1	2	48	1	2	12	1	144	2	2	2	2	6	0	0	3	1	1	8	1	0	1	0	2	2	1	0	1	2	1	1	0	1	0	2021-09-14 23:45:23.846735	2021-09-14 23:45:23.846735
105	0	20	90	90	10	20	90	90	COMMON	xm109.yml	SNIPER	6	Heckler and Koch	XM109 Sniper Rifle	13	70	SNIPER	1	115	2	1	3	1540	1	23	4	4.29999999999999982	9	10	1818	3138	3	7	0	1	1	0	0	4	1	2	1	0	1	2	7	1	0	0	5	2	0	4	10	1	0	2021-09-14 23:45:34.064608	2021-09-14 23:45:34.064608
106	90	10	0	0	90	40	9	0	COMMON	saiga12.yml	SHOTGUN	3	R.S.S.	SAIGA-12	39	2	SHOTGUN	70	6	2	93	24	24	5	53	2	2	3	40	918	8	2	3	3	9	12	3	35	2	3	0	6	80	4	0	2	0	12	0	3	2	3	2	2	18	0	2021-09-16 02:07:36.873686	2021-09-16 02:07:36.873686
107	10	10	10	10	10	10	10	10	COMMON	m4.yml	ASSAULT_RIFLE	4	Standard Issue	M4 Assault Rifle	26	3	ASSAULT_RIFLE	4	24	2	249	17	123	4	2	3	3	2	80	24	4	6	10	4	6	19	2	0	8	5	2	7	0	0	4	63	10	9	0	0	4	4	2	59	13	0	2021-09-16 02:07:57.898864	2021-09-16 02:07:57.898864
108	90	10	0	0	80	40	0	0	COMMON	mp5.yml	SUB_MACHINE_GUN	5	Heckler and Koch	MP5	5	2230	SUB_MACHINE_GUN	75	3	1	14	4	7	0	8	2	4.29999999999999982	5	180	3825	54	15	199	2	4	89	3	17	4	4	6	6	4	2	0	4	3	25	0	3	5	2	175	3	0	0	2021-09-16 02:08:02.030809	2021-09-16 02:08:02.030809
109	90	10	0	0	90	40	9	0	COMMON	saiga12.yml	SHOTGUN	3	R.S.S.	SAIGA-12	39	141	SHOTGUN	4	2	2	34	1	3	4	53	2	4.29999999999999982	81	2	4	31	9	123	3	4	0	3	2	3	17	3	0	3	0	5	3	33	18	0	3	3	4	0	161	4	0	2021-09-16 02:08:15.453854	2021-09-16 02:08:15.453854
110	0	20	90	90	10	20	90	90	COMMON	psg1.yml	SNIPER	6	PF-TDN	PSG1	8	47	SNIPER	0.810000000000000053	7	21	549	7	61	6	65	19	4.29999999999999982	4	10	1818	3218	21	11	3	3	3	4	2	3	0	3	19	0	5	3	4	4	0	0	3	3	2	2	2	0	0	2021-09-16 02:08:20.239637	2021-09-16 02:08:20.239637
111	90	10	0	0	90	40	9	0	COMMON	saiga12.yml	SHOTGUN	3	R.S.S.	SAIGA-12	39	40	SHOTGUN	70	1	2	43	1	80	0	1	2	1	2	10	1323	12	5	38	0	0	1	1	1	1	1	1	1	0	0	1	1	1	1	0	1	1	0	1	1	1	0	2021-09-16 23:27:45.188054	2021-09-16 23:27:45.188054
112	80	40	13	5	50	25	10	1	COMMON	czp10.yml	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	75	PISTOL	20.5	15	1	5	1	1	0	1	1	0	3	80	40	2	3	28	1	1	1	1	1	0	1	1	1	0	0	0	0	0	0	0	0	0	0	0	0	0	0	2021-09-16 23:35:43.931376	2021-09-16 23:35:43.931376
113	80	40	13	5	50	25	10	1	COMMON	ppk.yml	PISTOL	8	TN3 SMITH-x Industrial	Silenced PPK	42	206	PISTOL	29	2	1	5	1	20	0	33	2	1	1	1	40	2	3	1	0	0	1	1	2	1	1	1	1	1	0	0	1	4	2	0	1	0	1	1	1	1	0	2021-09-16 23:35:57.520318	2021-09-16 23:35:57.520318
114	90	10	0	0	90	40	9	0	COMMON	sasg12.yml	SHOTGUN	3	R.S.S.	SASG-12	6	100	SHOTGUN	70	2	2	34	1	80	2	1	2	2	1	10	1	10	8	20	2	1	2	2	1	2	1	2	0	0	0	1	0	0	2	0	1	1	1	1	1	1	0	2021-09-16 23:36:13.402217	2021-09-16 23:36:13.402217
116	10	10	10	10	10	10	10	10	COMMON	tar21.yml	ASSAULT_RIFLE	4	SK-10	TAR-21 Assault Rifle	30	1	ASSAULT_RIFLE	2	2	2	1	4	45	2	4	3	14	5	4	2	2	1	10	3	1	0	1	10	4	3	0	2	2	0	7	0	1	0	0	0	2	4	1	0	3	0	2021-09-16 23:36:44.521348	2021-09-16 23:36:44.521348
118	90	10	0	0	90	40	9	0	COMMON	sasg12.yml	SHOTGUN	3	R.S.S.	SASG-12	6	40	SHOTGUN	3	6	2	34	1	1	2	1	1	4.29999999999999982	2	10	1	10	3	127	2	3	3	2	0	1	2	8	0	1	1	1	0	0	6	0	2	1	1	0	2	3	0	2021-09-16 23:37:05.772357	2021-09-16 23:37:05.772357
119	90	10	0	0	90	40	9	0	COMMON	m3.yml	SHOTGUN	3	GBNT-3	M3	28	2	SHOTGUN	70	6	2	1	1	155	1	3	3	4.29999999999999982	2	34	918	74	13	67	4	4	0	15	0	2	0	5	6	0	1	1	1	1	1	0	2	2	1	4	4	2	0	2021-09-16 23:37:18.426344	2021-09-16 23:37:18.426344
120	23.0100990000000003	0	0	0	50	0	0	0	COMMON	desert-eagle.yml	PISTOL	8	LX Industries	Desert Eagle	47	50	PISTOL	4	1	6	11	1	34	2	33	1	0	21	8	1	1	2	20	0	4	2	2	0	2	13	2	1	0	2	2	2	0	4	0	13	3	2	0	4	2	0	2021-09-16 23:37:20.712351	2021-09-16 23:37:20.712351
121	90	10	0	0	80	40	0	0	COMMON	hk21.yml	LIGHT_MACHINE_GUN	10	TN-33Y	HK-21 Light Machine Gun	33	2	LIGHT_MACHINE_GUN	3.81000000000000005	1	20	2	1	1	0	2	2	4.29999999999999982	33	324	1344	20	80	1	0	1	6	0	1	5	2	1	1	2	1	1	1	0	2	0	2	1	0	2	4	0	0	2021-09-16 23:41:05.302512	2021-09-16 23:41:05.302512
122	90	10	0	0	80	40	0	0	COMMON	mk46.yml	LIGHT_MACHINE_GUN	10	TN-33Y	MK-46 Light Machine Gun	78	1	LIGHT_MACHINE_GUN	3.81000000000000005	19206	20	14	9	30	0	13	27	4.29999999999999982	1	438	818	1	40	558	1	2	0	1	9	2	1	5	1	14	0	2	4	0	1	0	0	1	2	4	4	2	0	2021-09-16 23:41:13.858541	2021-09-16 23:41:13.858541
123	0	20	90	90	10	20	90	90	COMMON	psg1.yml	SNIPER	6	PF-TDN	PSG1	8	1	SNIPER	0.810000000000000053	2	21	2	3	252	4	13	4	4.29999999999999982	8	23	1818	1	21	9	14	1	3	1	2	0	1	2	0	2	0	2	0	4	1	0	4	2	1	1	2	1	0	2021-09-16 23:41:23.490425	2021-09-16 23:41:23.490425
124	10	10	10	10	10	10	10	10	COMMON	g36c.yml	ASSAULT_RIFLE	4	Heckler & Koch	G36C Assault Rifle	46	220	ASSAULT_RIFLE	13	2	2	14	2	45	1	3	3	14	5	80	1	2	1	2	2	4	8	1	0	1	0	3	1	1	2	1	0	0	0	0	2	1	0	2	0	0	0	2021-09-16 23:41:37.665403	2021-09-16 23:41:37.665403
125	80	40	13	5	50	25	10	1	COMMON	uzi.yml	PISTOL	8	TN3 SMITH-x Industrial	A Uzi	43	75	MACHINE_PISTOL	10.5	13	1	5	1	1	3	1	5	3	5	308	1	3	12	1	1	2	2	5	0	2	21	13	1	2	4	2	2	9	2	0	0	1	4	0	2	6	0	2021-09-16 23:41:43.832588	2021-09-16 23:41:43.832588
126	80	40	13	5	50	25	10	1	COMMON	magnum-revolver.yml	PISTOL	8	TN3 SMITH-x Industrial	Magnum Revolver	40	75	PISTOL	20.5	6	1	2	2	3	3	2	1	2	60	80	40	7	2	187	8	3	14	3	3	2	11	0	15	0	3	4	2	12	5	0	0	3	24	0	3	0	0	2021-09-16 23:41:47.188541	2021-09-16 23:41:47.188541
127	10	10	10	10	10	10	10	10	COMMON	g36c.yml	ASSAULT_RIFLE	4	Heckler & Koch	G36C Assault Rifle	46	220	ASSAULT_RIFLE	518	91	2	2	4	223	2	3	3	4.29999999999999982	5	340	79	2	3	10	16	6	0	0	5	3	3	0	4	2	5	0	5	2	3	0	0	3	3	8	0	0	0	2021-09-16 23:41:49.369475	2021-09-16 23:41:49.369475
128	90	10	0	0	80	40	0	0	COMMON	belt-fed-minigun.yml	LIGHT_MACHINE_GUN	10	TN-33Y	A TN-33Y Belt-Fed Minigun	23	999	LIGHT_MACHINE_GUN	19	999	20	14	2	2	2	2	39	11	33	3	2	8	3	3	9	3	2	8	2	8	0	5	3	0	7	0	4	2	0	0	3	0	14	0	0	0	0	2021-09-16 23:41:54.99946	2021-09-16 23:41:54.99946
129	80	40	13	5	50	25	10	1	COMMON	magnum-revolver.yml	PISTOL	8	TN3 SMITH-x Industrial	Magnum Revolver	40	779	PISTOL	2	6	1	5	5	20	2	140	3	3	3	80	3	5	15	3	12	0	3	2	3	10	2	4	2	2	1	0	4	42	3	0	3	3	3	0	3	2	0	2021-09-16 23:41:58.892622	2021-09-16 23:41:58.892622
130	10	10	10	10	10	10	10	10	COMMON	m4.yml	ASSAULT_RIFLE	4	Standard Issue	M4 Assault Rifle	26	9	ASSAULT_RIFLE	3	8	2	80	4	3	17	3	2	12	5	408	49	3	2	57	2	3	0	3	5	0	2	21	0	3	3	44	2	3	2	0	4	3	0	2	2	14	0	2021-09-16 23:42:00.832379	2021-09-16 23:42:00.832379
131	90	10	0	0	80	40	0	0	COMMON	mp5.yml	SUB_MACHINE_GUN	5	Heckler and Koch	MP5	5	400	SUB_MACHINE_GUN	2	31	1	63	3	2	2	3	2	4.29999999999999982	5	613	2	20	17	6	2	5	0	54	2	2	3	0	10	3	3	19	7	0	12	0	2	2	3	0	2	2	0	2021-09-16 23:42:04.939434	2021-09-16 23:42:04.939434
132	80	40	13	5	50	25	10	1	COMMON	ppk.yml	PISTOL	8	TN3 SMITH-x Industrial	Silenced PPK	42	2	PISTOL	2	9	1	3	2	20	3	33	1	12	3	3	179	164	3	16	2	0	3	3	17	3	3	3	3	3	2	0	2	2	4	0	2	2	0	23	3	3	0	2021-09-16 23:42:07.309542	2021-09-16 23:42:07.309542
133	80	40	13	5	50	25	10	1	COMMON	magnum-revolver.yml	PISTOL	8	TN3 SMITH-x Industrial	Magnum Revolver	40	3	PISTOL	4	4	1	92	4	20	0	33	2	6	3	337	3	3	13	16	84	7	22	4	3	4	2	2	0	3	0	2	0	4	4	0	41	6	0	60	3	3	0	2021-09-16 23:42:08.832497	2021-09-16 23:42:08.832497
134	90	10	0	0	90	40	9	0	COMMON	saiga12.yml	SHOTGUN	3	R.S.S.	SAIGA-12	39	40	SHOTGUN	208	4	2	34	1	2	0	53	3	4.29999999999999982	2	8	918	58	3	4	6	3	3	6	0	0	2	10	6	7	3	4	5	4	0	0	0	0	0	4	2	2	0	2021-09-16 23:42:10.128434	2021-09-16 23:42:10.128434
135	90	10	0	0	80	40	0	0	COMMON	hk21.yml	LIGHT_MACHINE_GUN	10	TN-33Y	HK-21 Light Machine Gun	33	2	LIGHT_MACHINE_GUN	2	999	20	69	3	64	0	4	7	3	33	180	818	126	9	4	0	0	2	17	21	5	112	24	3	0	4	6	4	4	0	0	5	116	2	0	3	2	0	2021-09-16 23:42:13.915874	2021-09-16 23:42:13.915874
136	80	40	13	5	50	25	10	1	COMMON	uzi.yml	PISTOL	8	TN3 SMITH-x Industrial	A Uzi	43	4	MACHINE_PISTOL	10.5	13	1	14	4	3	6	4	1	3	3	4	2	2	10	16	3	4	91	0	4	32	0	4	3	13	13	4	19	0	3	0	6	6	2	0	0	2	0	2021-09-16 23:42:15.757476	2021-09-16 23:42:15.757476
137	90	10	0	0	80	40	0	0	COMMON	fmg9.yml	SUB_MACHINE_GUN	5	DXGR-1	FMG-9	5	11531	SUB_MACHINE_GUN	170	31	1	3	1	30	4	14	4	9	34	180	818	6	6	3	2	2	4	5	11	2	4	2	13	0	3	24	0	3	2	0	6	0	3	2	3	0	0	2021-09-16 23:42:17.37751	2021-09-16 23:42:17.37751
138	80	40	13	5	50	25	10	1	COMMON	uzi.yml	PISTOL	8	TN3 SMITH-x Industrial	A Uzi	43	75	MACHINE_PISTOL	10.5	13	1	5	1	4	0	33	1	0	3	80	40	11	13	16	5	2	1367	4	4	0	21	0	3	0	3	2	0	0	76	0	2	2	4	0	57	3	0	2021-09-16 23:42:20.082517	2021-09-16 23:42:20.082517
139	90	10	0	0	80	40	0	0	COMMON	hk21.yml	LIGHT_MACHINE_GUN	10	TN-33Y	HK-21 Light Machine Gun	33	2	LIGHT_MACHINE_GUN	11	3	20	47	3	30	0	13	3	3	4	2	818	66	40	140	514	4	0	10	3	3	4	0	49	5	26	2	2	19	8	0	4	2	3	0	4	0	0	2021-09-16 23:42:21.261413	2021-09-16 23:42:21.261413
142	90	10	0	0	90	40	9	0	COMMON	saiga12.yml	SHOTGUN	3	R.S.S.	SAIGA-12	39	40	SHOTGUN	70	20	2	4	4	38	0	53	5	4.29999999999999982	19	10	18	2	3	20	215	3	2	0	2	26	4	2	2	0	3	3	5	3	23	0	12	12	2	4	5	3	0	2021-09-17 00:06:22.501847	2021-09-17 00:06:22.501847
143	90	10	0	0	80	40	0	0	COMMON	fmg9.yml	SUB_MACHINE_GUN	5	DXGR-1	FMG-9	5	400	SUB_MACHINE_GUN	30.8099999999999987	3	1	14	1	206	4	3	3	80	66	2	818	20	18	6	2	3	2	3	4	12	2	0	2	22	0	6	17	21	6	0	0	2	4	26	2	0	0	2021-09-17 00:06:28.107491	2021-09-17 00:06:28.107491
144	90	10	0	0	80	40	0	0	COMMON	hk21.yml	LIGHT_MACHINE_GUN	10	TN-33Y	HK-21 Light Machine Gun	33	999	LIGHT_MACHINE_GUN	2	13	20	271	1	5	4	4	7	3	3	5	2	4	5	140	2	0	0	2	3	4	4	5	14	0	16	16	0	6	7	0	4	2	9	3	4	0	0	2021-09-17 00:06:53.128907	2021-09-17 00:06:53.128907
145	90	10	0	0	80	40	0	0	COMMON	p90.yml	SUB_MACHINE_GUN	5	Heckler and Koch	Heckler and Koch P90	21	1746	SUB_MACHINE_GUN	10	190	1	3	1	30	0	13	47	4.29999999999999982	6	4	818	20	57	2	4	10	4	13	6	72	5	2	0	4	0	2	3	4	4	0	29	3	5	4	4	6	0	2021-09-17 00:07:08.100927	2021-09-17 00:07:08.100927
146	90	10	0	0	90	40	9	0	COMMON	saiga12.yml	SHOTGUN	3	R.S.S.	SAIGA-12	39	311	SHOTGUN	5	32	2	34	2	4	0	3	4	4.29999999999999982	2	4	19	10	2	12	0	74	4	0	2	17	3	32	20	2	16	546	4	3	5	0	2	6	2	2	4	2	0	2021-09-17 00:19:32.432101	2021-09-17 00:19:32.432101
147	90	10	0	0	90	40	9	0	COMMON	saiga12.yml	SHOTGUN	3	R.S.S.	SAIGA-12	39	4	SHOTGUN	489	44	2	4	3	2	0	53	6	19	8	3	3	5	3	4	3	4	5	20	0	21	3	19	4	4	0	5	3	281	5	0	3	5	3	0	2	7	0	2021-09-17 00:19:36.334133	2021-09-17 00:19:36.334133
148	90	10	0	0	80	40	0	0	COMMON	ump45.yml	SUB_MACHINE_GUN	5	PN/P	UMP-45	32	400	SUB_MACHINE_GUN	30.8099999999999987	20	1	2	1	68	4	13	3	21	36	180	6479	20	2	208	3	11	56	15	0	0	2	2	3	0	0	4	45	22	0	0	5	5	0	4	5	3	0	2021-09-17 00:19:37.303205	2021-09-17 00:19:37.303205
149	90	10	0	0	90	40	9	0	COMMON	sasg12.yml	SHOTGUN	3	R.S.S.	SASG-12	6	3	SHOTGUN	3	3	2	34	3	80	4	178	26	3	2	181	918	3	4	20	4	6	3	4	3	51	4	25	3	4	3	2	3	0	2	0	4	3	0	0	0	3	0	2021-09-17 00:46:53.68857	2021-09-17 00:46:53.68857
150	90	10	0	0	90	40	9	0	COMMON	saiga12.yml	SHOTGUN	3	R.S.S.	SAIGA-12	39	146	SHOTGUN	387	6	2	34	1	80	4	53	3	4	2	10	918	21	11	3	0	0	4	2	12	0	14	3	6	2	3	3	2	2	4	0	4	4	0	0	2	24	0	2021-09-17 00:52:59.358813	2021-09-17 00:52:59.358813
151	90	10	0	0	90	40	9	0	COMMON	sasg12.yml	SHOTGUN	3	R.S.S.	SASG-12	6	2	SHOTGUN	70	47	2	34	1	628	3	53	2	5	2	5	7	65	3	4	15	6	6	2	3	0	9	0	0	0	197	4	0	0	2	0	38	2	21	2	5	2	0	2021-09-17 01:33:01.729627	2021-09-17 01:33:01.729627
152	10	10	10	10	10	10	10	10	COMMON	scarh.yml	ASSAULT_RIFLE	4	SK-10	SCAR-H Assault Rifle	31	5	ASSAULT_RIFLE	24	8	2	4	5	5	3.14000000000000012	3	4	2	21	80	91	4	23	5	125	4	4	21	5	5	0	4	249	5	24	5	2	0	4	0	4	0	0	0	5	2	0	2021-09-17 01:33:04.75737	2021-09-17 01:33:04.75737
153	0	20	90	90	10	20	90	90	COMMON	xm109.yml	SNIPER	6	Heckler and Koch	XM109 Sniper Rifle	13	73	SNIPER	0.810000000000000053	53	2	19	3	15	0	13	5	30	2	55	2	1843	15	40	16	3	3	2	3	12	0	21	2	3	145	4	0	2	2	0	4	4	15	21	2	3	0	2021-09-17 01:33:05.290392	2021-09-17 01:33:05.290392
140	0	20	90	90	10	20	90	90	COMMON	xm109.yml	SNIPER	6	Heckler and Koch	XM109 Sniper Rifle	13	70	SNIPER	15	2	39	19	4	450	0	80	4	2	246	4	1818	500	4	271	6	4	4	4	5	2	3	4	0	0	0	2	128	4	3	0	0	5	0	25	3	2	0	2021-09-16 23:42:22.548445	2021-09-16 23:42:22.548445
115	0	20	90	90	10	20	90	90	COMMON	l96aw.yml	SNIPER	6	Heckler and Koch	L96 Arctic Warfare	24	155	SNIPER	0.810000000000000053	7	39	1	3	450	0	2	1	4.29999999999999982	8	10	2590	6	4	209	2	1	1	2	1	1	0	7	0	2	7	2	0	3	1	0	1	5	0	2	0	2	0	2021-09-16 23:36:33.761333	2021-09-16 23:36:33.761333
117	0	20	90	90	10	20	90	90	COMMON	l96aw.yml	SNIPER	6	Heckler and Koch	L96 Arctic Warfare	24	70	SNIPER	1	8	39	19	3	1332	1	2	4	16	2	10	2	975	1	324	3	5	3	2	2	1	2	0	2	1	1	0	2	1	3	0	7	3	0	3	2	2	0	2021-09-16 23:36:54.692448	2021-09-16 23:36:54.692448
141	0	20	90	90	10	20	90	90	COMMON	xm109.yml	SNIPER	6	Heckler and Koch	XM109 Sniper Rifle	13	70	SNIPER	2	14	39	4	3	2	0	3	4	26	54	10	6340	7	8	40	19	5	3	10	88	3	21	4	4	2	2	2	4	6	4	0	5	0	4	4	0	3	0	2021-09-16 23:42:23.841449	2021-09-16 23:42:23.841449
154	90	10	0	0	90	40	9	0	COMMON	saiga12.yml	SHOTGUN	3	R.S.S.	SAIGA-12	39	40	SHOTGUN	381	6	2	34	2	80	5	53	7	4.29999999999999982	2	10	2	2	13	20	4	4	4	3	3	4	0	4	3	5	7	0	3	15	18	0	2	3	18	8	4	13	0	2021-09-17 01:46:31.106286	2021-09-17 01:46:31.106286
155	90	10	0	0	90	40	9	0	COMMON	saiga12.yml	SHOTGUN	3	R.S.S.	SAIGA-12	39	254	SHOTGUN	5	25	2	34	7	2	0	25	6	19	2	18	918	10	4	20	5	3	41	4	0	5	5	5	0	0	4	2	4	5	7	0	3	6	2	34	10	5	0	2021-09-17 01:46:41.943902	2021-09-17 01:46:41.943902
156	0	20	90	90	10	20	90	90	COMMON	psg1.yml	SNIPER	6	PF-TDN	PSG1	8	70	SNIPER	4	3	21	3	129	61	6	5	34	5	18	4	1818	3159	939	33	22	0	0	38	40	120	0	30	12	2	703	0	19	0	17	0	12	0	0	14	2	2	0	2021-09-17 01:46:52.492051	2021-09-17 01:46:52.492051
157	10	10	10	10	10	10	10	10	COMMON	scarh.yml	ASSAULT_RIFLE	4	SK-10	SCAR-H Assault Rifle	31	220	ASSAULT_RIFLE	2	3	2	14	4	103	46	4	4	5	4	5	24	3	6	10	4	37	0	3	3	2	5	27	133	0	2	5	5	6	6	0	3	4	0	0	3	7	0	2021-09-17 01:47:03.78067	2021-09-17 01:47:03.78067
158	90	10	0	0	90	40	9	0	COMMON	sasg12.yml	SHOTGUN	3	R.S.S.	SASG-12	6	1	SHOTGUN	1	6	2	34	1	1	1	53	2	4.29999999999999982	2	10	1453	10	1	34	1	1	1	1	0	1	0	1	1	1	0	1	0	1	1	0	1	1	0	1	0	1	0	2021-09-18 01:27:39.936028	2021-09-18 01:27:39.936028
159	0	20	90	90	10	20	90	90	COMMON	xm109.yml	SNIPER	6	Heckler and Koch	XM109 Sniper Rifle	13	70	SNIPER	0.810000000000000053	7	2	35	3	450	1	17	4	1	1	10	1818	500	1	1	1	0	1	1	1	0	0	0	1	1	1	1	0	1	1	0	1	1	1	0	1	1	0	2021-09-18 01:28:53.380809	2021-09-18 01:28:53.380809
160	90	10	0	0	80	40	0	0	COMMON	mk46.yml	LIGHT_MACHINE_GUN	10	TN-33Y	MK-46 Light Machine Gun	78	999	LIGHT_MACHINE_GUN	4	999	20	14	1	2	0	13	1	2	33	180	1	1	40	173	1	1	1	0	1	0	0	1	1	1	1	1	1	0	0	0	1	1	1	1	0	0	0	2021-09-18 01:30:06.037856	2021-09-18 01:30:06.037856
161	10	10	10	10	10	10	10	10	COMMON	tar21.yml	ASSAULT_RIFLE	4	SK-10	TAR-21 Assault Rifle	30	220	ASSAULT_RIFLE	4.23000000000000043	8	2	35	1	45	3.14000000000000012	3	1	4.29999999999999982	1	1	1	1	6	1	0	1	2	0	1	2	2	1	2	0	2	2	2	1	0	0	2	1	0	0	1	0	0	2021-09-18 01:30:12.186423	2021-09-18 01:30:12.186423
162	90	10	0	0	80	40	0	0	COMMON	mp5.yml	SUB_MACHINE_GUN	5	Heckler and Koch	MP5	5	400	SUB_MACHINE_GUN	30.8099999999999987	31	1	2	1	30	0	2	1	1	1	471	818	51	2	6	2	1	2	1	0	2	0	2	4	0	6	0	0	2	6	0	2	1	3	0	0	1	0	2021-09-18 01:30:18.64502	2021-09-18 01:30:18.64502
163	90	10	0	0	80	40	0	0	COMMON	mk46.yml	LIGHT_MACHINE_GUN	10	TN-33Y	MK-46 Light Machine Gun	78	4	LIGHT_MACHINE_GUN	5	999	20	2	1	70	0	1	1	1	248	1438	818	20	40	1	1	1	1	2	4	2	3	0	3	0	0	0	2	1	0	0	0	1	0	0	0	2	0	2021-09-18 01:30:39.448915	2021-09-18 01:30:39.448915
164	90	10	0	0	80	40	0	0	COMMON	ump45.yml	SUB_MACHINE_GUN	5	PN/P	UMP-45	32	1091	SUB_MACHINE_GUN	2	31	1	144	1	1	2	13	3	14	5	2	4239	48	6	6	3	2	2	1	2	0	1	0	2	1	1	3	3	0	2	0	3	0	2	2	0	1	0	2021-09-18 01:30:45.796981	2021-09-18 01:30:45.796981
165	0	20	90	90	10	20	90	90	COMMON	psg1.yml	SNIPER	6	PF-TDN	PSG1	8	70	SNIPER	0.810000000000000053	7	6	43	1	61	0	53	5	4.29999999999999982	1	2	2	934	1	2	3	0	1	2	2	0	1	3	0	1	0	2	0	1	1	0	2	3	1	2	0	2	0	2021-09-18 01:31:39.066818	2021-09-18 01:31:39.066818
166	90	10	0	0	90	40	9	0	COMMON	sasg12.yml	SHOTGUN	3	R.S.S.	SASG-12	6	40	SHOTGUN	1	6	2	90	4	80	1	53	2	1	2	2	3169	2	1	2	1	8	0	1	6	2	4	1	1	1	0	0	0	4	1	0	2	2	1	1	4	0	0	2021-09-18 01:31:41.977839	2021-09-18 01:31:41.977839
167	90	10	0	0	90	40	9	0	COMMON	saiga12.yml	SHOTGUN	3	R.S.S.	SAIGA-12	39	40	SHOTGUN	2	281	2	97	1	80	2	53	2	1	2	2	918	2	48	20	4	2	2	1	1	2	2	0	14	2	6	0	6	1	0	0	2	1	1	1	0	4	0	2021-09-18 01:32:18.631009	2021-09-18 01:32:18.631009
168	80	40	13	5	50	25	10	1	COMMON	uzi.yml	PISTOL	8	TN3 SMITH-x Industrial	A Uzi	43	75	MACHINE_PISTOL	69	121	1	5	1	2	2	5	2	2	1	2	40	2	2	79	1	0	3	9	3	2	2	0	3	5	4	2	2	0	2	0	2	18	0	20	0	3	0	2021-09-18 01:32:25.682925	2021-09-18 01:32:25.682925
169	0	20	90	90	10	20	90	90	COMMON	psg1.yml	SNIPER	6	PF-TDN	PSG1	8	1	SNIPER	0.810000000000000053	10	6	19	10	1	1	1	5	4.29999999999999982	2	10	1818	500	4	43	2	0	2	0	1	7	37	3	2	2	6	4	5	4	0	0	0	4	1	0	2	3	0	2021-09-18 01:32:40.296917	2021-09-18 01:32:40.296917
170	90	10	0	0	90	40	9	0	COMMON	sasg12.yml	SHOTGUN	3	R.S.S.	SASG-12	6	1	SHOTGUN	70	6	2	1	2	155	15	2	2	4.29999999999999982	2	10	918	1	1	20	8	1	2	0	2	2	1	2	0	4	1	0	2	16	0	0	4	1	2	0	2	4	0	2021-09-18 01:34:54.528826	2021-09-18 01:34:54.528826
171	80	40	13	5	50	25	10	1	COMMON	czp10.yml	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	1	PISTOL	20.5	1	1	8	1	20	0	2	1	2	3	1	40	2	3	16	3	2	2	1	1	4	15	2	0	2	4	5	0	2	1	0	2	5	2	1	1	1	0	2021-09-18 01:35:00.363254	2021-09-18 01:35:00.363254
172	10	10	10	10	10	10	10	10	COMMON	m4.yml	ASSAULT_RIFLE	4	Standard Issue	M4 Assault Rifle	26	475	ASSAULT_RIFLE	2	1	2	2	5	194	1	1	20	4.29999999999999982	5	80	5	9	6	10	0	6	2	3	1	2	1	4	1	1	3	2	2	4	3	0	1	0	2	0	4	0	0	2021-09-18 01:35:25.11697	2021-09-18 01:35:25.11697
173	90	10	0	0	90	40	9	0	COMMON	sasg12.yml	SHOTGUN	3	R.S.S.	SASG-12	6	47	SHOTGUN	70	6	2	34	1	1	1	1	1	4.29999999999999982	2	1	1577	10	5	1	0	1	0	1	1	1	0	1	1	1	1	0	1	1	0	0	1	1	0	1	0	1	0	2021-09-18 01:43:24.324088	2021-09-18 01:43:24.324088
174	90	10	0	0	90	40	9	0	COMMON	sasg12.yml	SHOTGUN	3	R.S.S.	SASG-12	6	1	SHOTGUN	70	1	2	1	1	80	0	53	1	1	2	1	918	10	5	1	1	0	0	1	1	0	1	1	1	1	1	0	1	0	1	0	0	1	0	1	1	1	0	2021-09-18 01:46:20.098382	2021-09-18 01:46:20.098382
\.


--
-- Data for Name: rifle_placements; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.rifle_placements (id, ip_room_vnum, ip_container_selector, ip_rifle_id, created_at, updated_at) FROM stdin;
\.


--
-- Data for Name: room; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.room (id, room_number, zone, sector_type, name, description, ex_keyword, ex_description, light, room_flag, nickname) FROM stdin;
2	131	1	21	{blu}C.O.F.O.B:{/blu} - Southern Hallway	The cement floor is a perfect complement to the reflection of the incandescent lights that are installed into the the ceiling. {gld}To the East{/gld}, you will find the Armory. {gld}To the West{/gld}, you will find the training grounds. Just around the corner to the East and North a ways is the Weapons Locker. Feel free to get acquainted with the layout, recruit.\r\n	\N	\N	1	0	cofobcenter
3	132	1	21	{blu}C.O.F.O.B:{/blu} - Southern Hallway	The cement floor is a perfect complement to the reflection of the incandescent lights that are installed into the the ceiling. {gld}To the East{/gld}, you will find the Armory. {gld}To the West{/gld}, you will find the training grounds. Just around the corner to the East and North a ways is the Weapons Locker. Feel free to get acquainted with the layout, recruit.\r\n	\N	\N	1	0	\N
4	133	1	21	{blu}C.O.F.O.B:{/blu} - Southern Hallway	The cement floor is a perfect complement to the reflection of the incandescent lights that are installed into the the ceiling. {gld}To the East{/gld}, you will find the Armory. {gld}To the West{/gld}, you will find the training grounds. Just around the corner to the East and North a ways is the Weapons Locker. Feel free to get acquainted with the layout, recruit.\r\n	\N	\N	1	0	\N
5	134	1	21	{blu}C.O.F.O.B:{/blu} - Southern Hallway	The cement floor is a perfect complement to the reflection of the incandescent lights that are installed into the the ceiling. {gld}To the East{/gld}, you will find the Armory. {gld}To the West{/gld}, you will find the training grounds. Just around the corner to the East and North a ways is the Weapons Locker. Feel free to get acquainted with the layout, recruit.\r\n	\N	\N	1	0	\N
6	135	1	21	{blu}C.O.F.O.B:{/blu} - Southern Hallway	The cement floor is a perfect complement to the reflection of the incandescent lights that are installed into the the ceiling. {gld}To the East{/gld}, you will find the Armory. {gld}To the West{/gld}, you will find the training grounds. Just around the corner to the East and North a ways is the Weapons Locker. Feel free to get acquainted with the layout, recruit.\r\n	\N	\N	1	0	\N
7	136	1	21	{blu}C.O.F.O.B:{/blu} - Southern Hallway	The cement floor is a perfect complement to the reflection of the incandescent lights that are installed into the the ceiling. {gld}To the East{/gld}, you will find the Armory. {gld}To the West{/gld}, you will find the training grounds. Just around the corner to the East and North a ways is the Weapons Locker. Feel free to get acquainted with the layout, recruit.\r\n	\N	\N	1	0	\N
8	137	1	21	{blu}C.O.F.O.B:{/blu} - Southern Hallway	The cement floor is a perfect complement to the reflection of the incandescent lights that are installed into the the ceiling. {gld}To the East{/gld}, you will find the Armory. {gld}To the West{/gld}, you will find the training grounds. Just around the corner to the East and North a ways is the Weapons Locker. Feel free to get acquainted with the layout, recruit.\r\n	\N	\N	1	0	\N
9	138	1	21	{blu}C.O.F.O.B:{/blu} - Southern Hallway	The cement floor is a perfect complement to the reflection of the incandescent lights that are installed into the the ceiling. {gld}To the East{/gld}, you will find the Armory. {gld}To the West{/gld}, you will find the training grounds. Just around the corner to the East and North a ways is the Weapons Locker. Feel free to get acquainted with the layout, recruit.\r\n	\N	\N	1	0	\N
10	139	1	21	{blu}C.O.F.O.B:{/blu} - Southern Hallway	The cement floor is a perfect complement to the reflection of the incandescent lights that are installed into the the ceiling. {gld}To the East{/gld}, you will find the Armory. {gld}To the West{/gld}, you will find the training grounds. Just around the corner to the East and North a ways is the Weapons Locker. Feel free to get acquainted with the layout, recruit.\r\n	\N	\N	1	0	\N
11	140	1	21	{blu}C.O.F.O.B:{/blu} - Southeast Corner	The hallway reaches north and south from here. A reduced temperature is like the result of the industrial grade internal air cooling system. It isn't much, but it beats the outside desert climate. The promise of coffee entices you, but you can't tell which direction it's coming from. \r\n	\N	\N	1	0	\N
12	141	1	21	{blu}C.O.F.O.B:{/blu} - Armory Entrance	A cool draft moves through the bottom crack of the door to the Armory Entrance to the East.It seems the quality of air drastically differs depending on the people in charge of each department. The Sign above the door says in bold letters "Armory".\r\n	\N	\N	1	0	cofob-armory-entrance
13	142	1	21	{blu}C.O.F.O.B:{/blu} - Armory	As you push through to the East, you notice a few recruits putting on standard issue gear. They ignore you as you take a look around. To the East is the buy station where you can make your purchases.\r\n	\N	\N	1	0	\N
14	143	1	21	{blu}C.O.F.O.B:{/blu} - Armory Buy Station	You see an armor locker with standard issue equipment. Behind the counter is a {gld}list{/gld} of all the various items for sell. You can spend {grn}MP{/grn} (Mission Points) here to upgrade your loadout. To buy something, simply type {grn}"Buy ID"{/grn} where ID is the number next to the item you want in the output of the list command.\r\n	\N	\N	1	0	\N
15	144	1	21	{blu}C.O.F.O.B:{/blu} - Armory Storage Room North	Standard issue armor and defensive utilities line the walls; none of which you can take as they are behind metal cages. There is, however, an Armor locker here with standard issue gear for anyone to take. \r\n	\N	\N	1	0	\N
16	145	1	21	{blu}C.O.F.O.B:{/blu} - Armory Storage Room South	You enter the storage room and immediately notice the strong scent of sand, grime, and gasoline. A few bits of ammunition are strewn across the floor haphazardly. The Armory personnell either recently dug through the piles of ammo crates, or nobody bothered to clean this mess up. There seems to be a computer terminal on the East wall.\r\n	\N	\N	1	0	\N
17	146	1	21	{blu}C.O.F.O.B:{/blu} - Armory - Secluded Room	A musty room with several freshly smoked cigars laying inside a deep ash tray the size of your fist. Someone was loading a Mossberg shotgun and haphazardly left it laying upon the couch as if it were a visitor. The T.V. appears to still be warm. Whoever was here is likely coming back soon. You have a feeling whoever was here will be back shortly.\r\n	\N	\N	1	0	\N
18	147	1	21	{blu}C.O.F.O.B:{/blu} - Armory - Secluded Room - Weapons Cache	@FILL_ME@\r\n	\N	\N	1	0	\N
19	148	1	21	{blu}C.O.F.O.B:{/blu} - Weapons Cache stairs - TOP	@FILL_ME@\r\n	\N	\N	1	0	\N
21	150	1	21	{blu}C.O.F.O.B:{/blu} - Weapons Cache stairs - TOP	@FILL_ME@\r\n	\N	\N	1	0	\N
22	151	1	21	{blu}C.O.F.O.B:{/blu} - Weapons Cache stairs - Basement 1	@FILL_ME@\r\n	\N	\N	1	0	\N
24	153	1	21	{blu}C.O.F.O.B:{/blu} - Weapons Cache stairs - Basement 1	@FILL_ME@\r\n	\N	\N	1	0	\N
25	154	1	21	{blu}C.O.F.O.B:{/blu} - Weapons Cache stairs - Basement 2	@FILL_ME@\r\n	\N	\N	1	0	\N
26	155	1	21	{blu}C.O.F.O.B:{/blu} - Weapons Cache stairs - Basement 2	@FILL_ME@\r\n	\N	\N	1	0	\N
27	156	1	21	{blu}C.O.F.O.B:{/blu} - Weapons Cache stairs - Basement 2	@FILL_ME@\r\n	\N	\N	1	0	cofob-armory-basement-2
28	157	1	21	{blu}C.O.F.O.B:{/blu} - Weapons Cache stairs - Basement 2	@FILL_ME@\r\n	\N	\N	1	0	\N
29	158	1	21	{blu}C.O.F.O.B:{/blu} - Weapons Cache stairs - Basement 2	@FILL_ME@\r\n	\N	\N	1	0	\N
30	159	1	21	{blu}C.O.F.O.B:{/blu} - Weapons Cache stairs - Basement 2	@FILL_ME@\r\n	\N	\N	1	0	\N
31	160	1	21	{blu}C.O.F.O.B:{/blu} - Weapons Cache stairs - Basement 2	@FILL_ME@\r\n	\N	\N	1	0	\N
32	161	1	21	{blu}C.O.F.O.B:{/blu} - Weapons Cache stairs - Basement 2	@FILL_ME@\r\n	\N	\N	1	0	\N
33	162	1	21	{blu}C.O.F.O.B:{/blu} - Weapons Cache stairs - Basement 2	@FILL_ME@\r\n	\N	\N	1	0	\N
34	163	1	21	{blu}C.O.F.O.B:{/blu} - Weapons Cache stairs - Basement 2	@FILL_ME@\r\n	\N	\N	1	0	\N
35	164	1	21	{blu}C.O.F.O.B:{/blu} - Weapons Cache stairs - Basement 2	@FILL_ME@\r\n	\N	\N	1	0	\N
36	165	1	21	{blu}C.O.F.O.B:{/blu} - Weapons Cache stairs - Basement 2	@FILL_ME@\r\n	\N	\N	1	0	\N
37	166	1	21	{blu}C.O.F.O.B:{/blu} - Weapons Cache stairs - Basement 2	@FILL_ME@\r\n	\N	\N	1	0	\N
38	167	1	21	{blu}C.O.F.O.B:{/blu} - Weapons Cache stairs - Basement 2	@FILL_ME@\r\n	\N	\N	1	0	\N
39	168	1	21	{blu}C.O.F.O.B:{/blu} - Weapons Cache stairs - Basement 2	@FILL_ME@\r\n	\N	\N	1	0	\N
40	169	1	21	{blu}C.O.F.O.B:{/blu} - Weapons Cache stairs - Basement 2	@FILL_ME@\r\n	\N	\N	1	0	cofob-armory-basement-2
41	170	1	21	{blu}C.O.F.O.B:{/blu} - Weapons Cache stairs - Basement 2	@FILL_ME@\r\n	\N	\N	1	0	\N
42	171	1	21	{blu}C.O.F.O.B:{/blu} - Weapons Cache stairs - Basement 2	@FILL_ME@\r\n	\N	\N	1	0	\N
43	172	1	21	{blu}C.O.F.O.B:{/blu} - Basement 2 - Breach charges	@FILL_ME@\r\n	\N	\N	1	0	\N
44	173	1	21	{blu}C.O.F.O.B:{/blu} - Basement 2 - Breach charges	@FILL_ME@\r\n	\N	\N	1	0	\N
45	174	1	0	name	description	\N	\N	1	0	\N
46	175	1	0	name	description	\N	\N	1	0	\N
47	176	1	0	name	description	\N	\N	1	0	\N
49	178	1	0	{blu}C.O.F.O.B:{/blu} - Basement 2 - Car Garage	@FILL_ME@\r\n	\N	\N	1	0	\N
50	179	1	0	{blu}C.O.F.O.B:{/blu} - Basement 2 - Car Garage	@FILL_ME@\r\n	\N	\N	1	0	\N
51	180	1	0	{blu}C.O.F.O.B:{/blu} - Basement 2 - Car Garage - 1A	@FILL_ME@\r\n	\N	\N	1	0	\N
52	181	1	0	{blu}C.O.F.O.B:{/blu} - Basement 2 - Car Garage - 1A	@FILL_ME@\r\n	\N	\N	1	0	\N
53	182	1	0	{blu}C.O.F.O.B:{/blu} - Basement 2 - Car Garage - 1B	@FILL_ME@\r\n	\N	\N	1	0	\N
54	183	1	0	{blu}C.O.F.O.B:{/blu} - Basement 2 - Car Garage - 1B	@FILL_ME@\r\n	\N	\N	1	0	\N
55	184	1	0	{blu}C.O.F.O.B:{/blu} - Basement 2 - Car Garage	@FILL_ME@\r\n	\N	\N	1	0	\N
56	185	1	0	{blu}C.O.F.O.B:{/blu} - Basement 2 - Car Garage	@FILL_ME@\r\n	\N	\N	1	0	\N
57	186	1	0	{blu}C.O.F.O.B:{/blu} - Basement 2 - Car Garage - 2A	@FILL_ME@\r\n	\N	\N	1	0	\N
58	187	1	0	{blu}C.O.F.O.B:{/blu} - Basement 2 - Car Garage - 2A	@FILL_ME@\r\n	\N	\N	1	0	\N
59	188	1	0	{blu}C.O.F.O.B:{/blu} - Basement 2 - Car Garage - 2B	@FILL_ME@\r\n	\N	\N	1	0	\N
60	189	1	0	{blu}C.O.F.O.B:{/blu} - Basement 2 - Car Garage - 2B	@FILL_ME@\r\n	\N	\N	1	0	\N
61	190	1	0	{blu}C.O.F.O.B:{/blu} - Basement 2 - Car Garage	@FILL_ME@\r\n	\N	\N	1	0	\N
62	191	1	0	{blu}C.O.F.O.B:{/blu} - Basement 2 - Car Garage	@FILL_ME@\r\n	\N	\N	1	0	\N
64	193	1	21	{blu}C.O.F.O.B:{/blu} - Eastern Hallway	@FILL_ME@\r\n	\N	\N	1	0	\N
65	194	1	21	{blu}C.O.F.O.B:{/blu} - Eastern Hallway	@FILL_ME@\r\n	\N	\N	1	0	\N
66	195	1	21	{blu}C.O.F.O.B:{/blu} - Eastern Hallway	@FILL_ME@\r\n	\N	\N	1	0	\N
67	196	1	21	{blu}C.O.F.O.B:{/blu} - Eastern Hallway	@FILL_ME@\r\n	\N	\N	1	0	\N
68	197	1	21	{blu}C.O.F.O.B:{/blu} - Eastern Hallway	@FILL_ME@\r\n	\N	\N	1	0	\N
69	198	1	21	{blu}C.O.F.O.B:{/blu} - Eastern Hallway	@FILL_ME@\r\n	\N	\N	1	0	\N
71	200	1	21	{blu}C.O.F.O.B:{/blu} - Eastern Hallway	@FILL_ME@\r\n	\N	\N	1	0	\N
72	201	1	21	{blu}C.O.F.O.B:{/blu} - Eastern Hallway	@FILL_ME@\r\n	\N	\N	1	0	\N
73	202	1	21	{blu}C.O.F.O.B:{/blu} - Eastern Hallway	@FILL_ME@\r\n	\N	\N	1	0	cofob-mess-hall-upstairs
74	203	1	21	{blu}C.O.F.O.B:{/blu} - Eastern Hallway	@FILL_ME@\r\n	\N	\N	1	0	\N
75	204	1	21	{blu}C.O.F.O.B:{/blu} - Eastern Hallway	@FILL_ME@\r\n	\N	\N	1	0	\N
76	205	1	21	{blu}C.O.F.O.B:{/blu} - North Hallway	@FILL_ME@\r\n	\N	\N	1	0	\N
77	206	1	21	{blu}C.O.F.O.B:{/blu} - North Hallway	@FILL_ME@\r\n	\N	\N	1	0	\N
78	207	1	21	{blu}C.O.F.O.B:{/blu} - North Hallway Stairs	@FILL_ME@\r\n	\N	\N	1	0	cofob-stairs-A
79	208	1	21	{blu}C.O.F.O.B:{/blu} - North Hallway	@FILL_ME@\r\n	\N	\N	1	0	\N
80	209	1	21	{blu}C.O.F.O.B:{/blu} - North Hallway	@FILL_ME@\r\n	\N	\N	1	0	\N
81	210	1	21	{blu}C.O.F.O.B:{/blu} - North Hallway	@FILL_ME@\r\n	\N	\N	1	0	\N
82	211	1	21	{blu}C.O.F.O.B:{/blu} - North Hallway	@FILL_ME@\r\n	\N	\N	1	0	\N
83	212	1	21	{blu}C.O.F.O.B:{/blu} - North Hallway	@FILL_ME@\r\n	\N	\N	1	0	\N
85	214	1	0	{blu}C.O.F.O.B [OUTER]:{/blu} - Western Hallway	The reinforced metal walls are briefly interrupted by the occasional senior officer door. As you make your way north and south, you notice that the entirety of the western wall consists of metal walls with doors that lead to the underground portion of the base. \r\n	\N	\N	1	0	\N
86	215	1	0	{blu}C.O.F.O.B [OUTER]:{/blu} - Western Hallway	The reinforced metal walls are briefly interrupted by the occasional senior officer door. As you make your way north and south, you notice that the entirety of the western wall consists of metal walls with doors that lead to the underground portion of the base. \r\n	\N	\N	1	0	\N
87	216	1	0	{blu}C.O.F.O.B [OUTER]:{/blu} - Western Hallway	The reinforced metal walls are briefly interrupted by the occasional senior officer door. As you make your way north and south, you notice that the entirety of the western wall consists of metal walls with doors that lead to the underground portion of the base. \r\n	\N	\N	1	0	\N
125	254	1	21	{blu}C.O.F.O.B:{/blu} - Second floor hallway	@FILL_ME@\r\n	\N	\N	1	0	\N
126	255	1	21	{blu}C.O.F.O.B:{/blu} - Second floor hallway	@FILL_ME@\r\n	\N	\N	1	0	\N
127	256	1	21	{blu}C.O.F.O.B:{/blu} - Second floor hallway	@FILL_ME@\r\n	\N	\N	1	0	\N
88	217	1	0	{blu}C.O.F.O.B [OUTER]:{/blu} - Western Hallway	The reinforced metal walls are briefly interrupted by the occasional senior officer door. As you make your way north and south, you notice that the entirety of the western wall consists of metal walls with doors that lead to the underground portion of the base. \r\n	\N	\N	1	0	\N
89	218	1	0	{blu}C.O.F.O.B [OUTER]:{/blu} - Western Hallway	The reinforced metal walls are briefly interrupted by the occasional senior officer door. As you make your way north and south, you notice that the entirety of the western wall consists of metal walls with doors that lead to the underground portion of the base. \r\n	\N	\N	1	0	\N
90	219	1	0	{blu}C.O.F.O.B [OUTER]:{/blu} - Western Hallway	The reinforced metal walls are briefly interrupted by the occasional senior officer door. As you make your way north and south, you notice that the entirety of the western wall consists of metal walls with doors that lead to the underground portion of the base. \r\n	\N	\N	1	0	\N
91	220	1	0	{blu}C.O.F.O.B [OUTER]:{/blu} - Western Hallway	The reinforced metal walls are briefly interrupted by the occasional senior officer door. As you make your way north and south, you notice that the entirety of the western wall consists of metal walls with doors that lead to the underground portion of the base. \r\n	\N	\N	1	0	\N
92	221	1	0	{blu}C.O.F.O.B [OUTER]:{/blu} - Western Hallway	The reinforced metal walls are briefly interrupted by the occasional senior officer door. As you make your way north and south, you notice that the entirety of the western wall consists of metal walls with doors that lead to the underground portion of the base. \r\n	\N	\N	1	0	\N
93	222	1	0	{blu}C.O.F.O.B [OUTER]:{/blu} - Western Hallway	The reinforced metal walls are briefly interrupted by the occasional senior officer door. As you make your way north and south, you notice that the entirety of the western wall consists of metal walls with doors that lead to the underground portion of the base. \r\n	\N	\N	1	0	\N
94	223	1	0	{blu}C.O.F.O.B [OUTER]:{/blu} - Western Hallway	The reinforced metal walls are briefly interrupted by the occasional senior officer door. As you make your way north and south, you notice that the entirety of the western wall consists of metal walls with doors that lead to the underground portion of the base. \r\n	\N	\N	1	0	\N
95	224	1	0	{blu}C.O.F.O.B [OUTER]:{/blu} - Atop the stairs	The floor deviates from the other rooms in that it is made up of a reinforced steel grating. The choice for such a floor remains a mystery. The door to the west leads to the stairs that will take you to the underground portion of the base.\r\n	\N	\N	1	0	cofob-west-atop-stairs
96	225	1	0	{blu}C.O.F.O.B [OUTER]:{/blu} - Atop the stairs	The floor deviates from the other rooms in that it is made up of a reinforced steel grating. The choice for such a floor remains a mystery. The door to the west leads to the stairs that will take you to the underground portion of the base.\r\n	\N	\N	1	0	\N
97	226	1	0	{blu}C.O.F.O.B [OUTER]:{/blu} - Western Hallway	The reinforced metal walls are briefly interrupted by the occasional senior officer door. As you make your way north and south, you notice that the entirety of the western wall consists of metal walls with doors that lead to the underground portion of the base. \r\n	\N	\N	1	0	\N
98	227	1	0	{blu}C.O.F.O.B [OUTER]:{/blu} - South Hallway	The reinforced metal walls are briefly interrupted by the occasional senior officer door. As you make your way north and south, you notice that the entirety of the western wall consists of metal walls with doors that lead to the underground portion of the base. \r\n	\N	\N	1	0	\N
100	229	1	0	{blu}C.O.F.O.B [OUTER]:{/blu} - South Hallway	The reinforced metal walls are briefly interrupted by the occasional senior officer door. As you make your way north and south, you notice that the entirety of the western wall consists of metal walls with doors that lead to the underground portion of the base. \r\n	\N	\N	1	0	\N
101	230	1	0	{blu}C.O.F.O.B [OUTER]:{/blu} - South Hallway	The reinforced metal walls are briefly interrupted by the occasional senior officer door. As you make your way north and south, you notice that the entirety of the western wall consists of metal walls with doors that lead to the underground portion of the base. \r\n	\N	\N	1	0	\N
102	231	1	0	{blu}C.O.F.O.B [OUTER]:{/blu} - South Hallway	The reinforced metal walls are briefly interrupted by the occasional senior officer door. As you make your way north and south, you notice that the entirety of the western wall consists of metal walls with doors that lead to the underground portion of the base. \r\n	\N	\N	1	0	cofob-bind-to-center
103	232	1	21	{blu}C.O.F.O.B:{/blu} - North Hallway Stairs	@FILL_ME@\r\n	\N	\N	1	0	\N
104	233	1	21	{blu}C.O.F.O.B:{/blu} - North Hallway Stairs	@FILL_ME@\r\n	\N	\N	1	0	\N
105	234	1	21	{blu}C.O.F.O.B:{/blu} - North Hallway Stairs	@FILL_ME@\r\n	\N	\N	1	0	\N
106	235	1	21	{blu}C.O.F.O.B:{/blu} - North Hallway Stairs	@FILL_ME@\r\n	\N	\N	1	0	\N
107	236	1	21	{blu}C.O.F.O.B:{/blu} - Second floor hallway	@FILL_ME@\r\n	\N	\N	1	0	\N
108	237	1	21	{blu}C.O.F.O.B:{/blu} - Second floor hallway	@FILL_ME@\r\n	\N	\N	1	0	cofob-secondfloor-center
109	238	1	21	{blu}C.O.F.O.B:{/blu} - Second floor hallway	@FILL_ME@\r\n	\N	\N	1	0	\N
110	239	1	21	{blu}C.O.F.O.B:{/blu} - Second floor hallway	@FILL_ME@\r\n	\N	\N	1	0	\N
111	240	1	21	{blu}C.O.F.O.B:{/blu} - Second floor hallway	@FILL_ME@\r\n	\N	\N	1	0	\N
112	241	1	21	{blu}C.O.F.O.B:{/blu} - Second floor hallway	@FILL_ME@\r\n	\N	\N	1	0	\N
113	242	1	21	{blu}C.O.F.O.B:{/blu} - Second floor hallway	@FILL_ME@\r\n	\N	\N	1	0	\N
114	243	1	0	{blu}C.O.F.O.B:{/blu} - Second floor Foremast	@FILL_ME@\r\n	\N	\N	1	0	\N
115	244	1	0	{blu}C.O.F.O.B:{/blu} - Second floor Foremast	@FILL_ME@\r\n	\N	\N	1	0	\N
116	245	1	0	{blu}C.O.F.O.B:{/blu} - Second floor Foremast	@FILL_ME@\r\n	\N	\N	1	0	\N
117	246	1	0	{blu}C.O.F.O.B:{/blu} - Second floor Foremast	@FILL_ME@\r\n	\N	\N	1	0	\N
118	247	1	0	{blu}C.O.F.O.B:{/blu} - Second floor Foremast	@FILL_ME@\r\n	\N	\N	1	0	\N
119	248	1	0	{blu}C.O.F.O.B:{/blu} - Second floor Foremast	@FILL_ME@\r\n	\N	\N	1	0	\N
120	249	1	21	{blu}C.O.F.O.B:{/blu} - Second floor Foremast	@FILL_ME@\r\n	\N	\N	1	0	\N
121	250	1	21	{blu}C.O.F.O.B:{/blu} - Second floor Foremast	@FILL_ME@\r\n	\N	\N	1	0	\N
122	251	1	21	{blu}C.O.F.O.B:{/blu} - Second floor Foremast	@FILL_ME@\r\n	\N	\N	1	0	\N
123	252	1	21	{blu}C.O.F.O.B:{/blu} - Second floor Foremast	@FILL_ME@\r\n	\N	\N	1	0	\N
124	253	1	21	{blu}C.O.F.O.B:{/blu} - Second floor hallway	@FILL_ME@\r\n	\N	\N	1	0	\N
130	259	1	0	{blu}C.O.F.O.B:{/blu} - Engineering hallway	The doors on the eastern wall are all closed and most likely locked. You see the commons area at the end of the hallway. You notice various surveillance cameras strategically placed at the corners of the ceiling. \r\n	\N	\N	1	0	\N
131	260	1	0	{blu}C.O.F.O.B:{/blu} - Engineering hallway	The doors on the eastern wall are all closed and most likely locked. You see the commons area at the end of the hallway. You notice various surveillance cameras strategically placed at the corners of the ceiling. \r\n	\N	\N	1	0	\N
132	261	1	0	{blu}C.O.F.O.B:{/blu} - Engineering hallway	The doors on the eastern wall are all closed and most likely locked. You see the commons area at the end of the hallway. You notice various surveillance cameras strategically placed at the corners of the ceiling. \r\n	\N	\N	1	0	\N
134	263	1	0	{blu}C.O.F.O.B:{/blu} - Server Room 1A	It's noisy and loud in here. The rack mounted servers are fervently whirring under the immense workloads. The ceiling is a grate with a large fan behind it. A complex looking locking mechanism protects rack mounted servers here. \r\n	\N	\N	1	0	\N
135	264	1	0	{blu}C.O.F.O.B:{/blu} - Server Room 1B	Wall to wall rack mounted servers take up the entirety of this room. There is almost nowhere to stand. A haphazard array of ethernet cables snake chaotically to and from each server. Maintaining this must be a headache. \r\n	\N	\N	1	0	\N
136	265	1	21	{blu}C.O.F.O.B:{/blu} - Eastern Hallway	@FILL_ME@\r\n	\N	\N	1	0	\N
137	266	1	0	{blu}C.O.F.O.B:{/blu} - Gear room	A huge weapons rack is attached to the wall here. The rack contains rifles, pistols, ammunition, explosives, and breach charges.\r\n	\N	\N	1	0	cofob-gear-room
138	267	1	0	{blu}C.O.F.O.B:{/blu} - Shooting Range Area Falcon	@FILL_ME@\r\n	\N	\N	1	0	\N
139	268	1	0	{blu}C.O.F.O.B:{/blu} - Shooting Range Area Falcon	@FILL_ME@\r\n	\N	\N	1	0	\N
140	269	1	0	{blu}C.O.F.O.B:{/blu} - Shooting Range Area Falcon	@FILL_ME@\r\n	\N	\N	1	0	\N
141	270	1	0	{blu}C.O.F.O.B:{/blu} - Shooting Range Area Falcon	@FILL_ME@\r\n	\N	\N	1	0	\N
142	271	1	0	{blu}C.O.F.O.B:{/blu} - Shooting Range Area Falcon	@FILL_ME@\r\n	\N	\N	1	0	\N
143	272	1	0	{blu}C.O.F.O.B:{/blu} - Shooting Range Area Falcon	@FILL_ME@\r\n	\N	\N	1	0	\N
144	273	1	0	{blu}C.O.F.O.B:{/blu} - Shooting Range Area Falcon	@FILL_ME@\r\n	\N	\N	1	0	\N
145	274	1	0	{blu}C.O.F.O.B [OUTER]:{/blu} - Eastern DMZ corridor	Dirt stretches to the east towards a heavy containment door. Behind that door must be where you can leave the building. The corridor is dimly lit. The air is dry and smells of a combination of sweat and gasoline for some reason.\r\n	\N	\N	1	0	\N
146	275	1	0	{blu}C.O.F.O.B [OUTER]:{/blu} - Eastern DMZ corridor	Dirt stretches to the east towards a heavy containment door. Behind that door must be where you can leave the building. The corridor is dimly lit. The air is dry and smells of a combination of sweat and gasoline for some reason.\r\n	\N	\N	1	0	\N
147	276	1	0	{blu}C.O.F.O.B [OUTER]:{/blu} - Eastern DMZ corridor	Dirt stretches to the east towards a heavy containment door. Behind that door must be where you can leave the building. The corridor is dimly lit. The air is dry and smells of a combination of sweat and gasoline for some reason.\r\n	\N	\N	1	0	\N
148	277	1	0	{blu}C.O.F.O.B [OUTER]:{/blu} - Fenced off DMZ corridor	Beneath the floor here is an entrance to an underground tunnel. The only problem is that you can't seem to get it to move. To the east is the door to the outside world.A falcon engraving is above the door frame to the east.\r\n	\N	\N	1	0	\N
149	278	1	0	Destroyed overpass - Abott Market South	Crushed and flattened by endless shells, the overpass that you just passed under creaks eternally. Now and then you hear the crumble of asphault and be bend of metal rods. The overpass connects the once thriving market place of Abott Market, but now all you see is rubble and a shell of what used to be a thriving market.\r\n	\N	\N	1	0	abbotstart
151	280	1	0	Destroyed overpass - Abott Market South	Crushed and flattened by endless shells, the overpass that you just passed under creaks eternally. Now and then you hear the crumble of asphault and be bend of metal rods. The overpass connects the once thriving market place of Abott Market, but now all you see is rubble and a shell of what used to be a thriving market.\r\n	\N	\N	1	0	\N
152	281	1	0	Destroyed overpass - Abott Market South	Crushed and flattened by endless shells, the overpass that you just passed under creaks eternally. Now and then you hear the crumble of asphault and be bend of metal rods. The overpass connects the once thriving market place of Abott Market, but now all you see is rubble and a shell of what used to be a thriving market.\r\n	\N	\N	1	0	\N
153	282	1	0	Destroyed overpass - Abott Market South	Crushed and flattened by endless shells, the overpass that you just passed under creaks eternally. Now and then you hear the crumble of asphault and be bend of metal rods. The overpass connects the once thriving market place of Abott Market, but now all you see is rubble and a shell of what used to be a thriving market.\r\n	\N	\N	1	0	\N
154	283	1	0	Destroyed overpass - Abott Market South	Crushed and flattened by endless shells, the overpass that you just passed under creaks eternally. Now and then you hear the crumble of asphault and be bend of metal rods. The overpass connects the once thriving market place of Abott Market, but now all you see is rubble and a shell of what used to be a thriving market.\r\n	\N	\N	1	0	\N
155	284	1	0	Destroyed overpass - Abott Market South	Crushed and flattened by endless shells, the overpass that you just passed under creaks eternally. Now and then you hear the crumble of asphault and be bend of metal rods. The overpass connects the once thriving market place of Abott Market, but now all you see is rubble and a shell of what used to be a thriving market.\r\n	\N	\N	1	0	\N
156	285	1	0	Destroyed overpass - Abott Market South	Crushed and flattened by endless shells, the overpass that you just passed under creaks eternally. Now and then you hear the crumble of asphault and be bend of metal rods. The overpass connects the once thriving market place of Abott Market, but now all you see is rubble and a shell of what used to be a thriving market.\r\n	\N	\N	1	0	\N
157	286	1	0	Destroyed overpass - Abott Market South	Crushed and flattened by endless shells, the overpass that you just passed under creaks eternally. Now and then you hear the crumble of asphault and be bend of metal rods. The overpass connects the once thriving market place of Abott Market, but now all you see is rubble and a shell of what used to be a thriving market.\r\n	\N	\N	1	0	\N
237	365	1	0	{blu}Crenshaw{/blu} Highway overpass - North	The ramp curves to the north. To the east you see a long stretch of highway that disappears over the horizon. You see a shipyard far off to the northeast. \r\n	\N	\N	1	0	\N
158	287	1	0	Destroyed overpass - Abott Market South	Crushed and flattened by endless shells, the overpass that you just passed under creaks eternally. Now and then you hear the crumble of asphault and be bend of metal rods. The overpass connects the once thriving market place of Abott Market, but now all you see is rubble and a shell of what used to be a thriving market.\r\n	\N	\N	1	0	\N
159	288	1	0	Destroyed overpass - Abott Market South	Crushed and flattened by endless shells, the overpass that you just passed under creaks eternally. Now and then you hear the crumble of asphault and be bend of metal rods. The overpass connects the once thriving market place of Abott Market, but now all you see is rubble and a shell of what used to be a thriving market.\r\n	\N	\N	1	0	\N
160	289	1	0	Destroyed overpass - Abott Market South	Crushed and flattened by endless shells, the overpass that you just passed under creaks eternally. Now and then you hear the crumble of asphault and be bend of metal rods. The overpass connects the once thriving market place of Abott Market, but now all you see is rubble and a shell of what used to be a thriving market.\r\n	\N	\N	1	0	abbot-market-east-divergence
161	290	1	0	Destroyed overpass - Abott Market South	Crushed and flattened by endless shells, the overpass that you just passed under creaks eternally. Now and then you hear the crumble of asphault and be bend of metal rods. The overpass connects the once thriving market place of Abott Market, but now all you see is rubble and a shell of what used to be a thriving market.\r\n	\N	\N	1	0	\N
162	291	1	0	Market Apartments - East entrance	An iron rod gate can be seen laying discarded near the entrance. In another time, that gate would have been used to keep intruders out. Far off to the west is what used to be the tenant parking lot. You'd be surprised to find any tenant using that lot seeing as how low the income requirements were for this apartment complex.\r\n	\N	\N	1	0	\N
163	292	1	0	Market Apartments - East entrance	An iron rod gate can be seen laying discarded near the entrance. In another time, that gate would have been used to keep intruders out. Far off to the west is what used to be the tenant parking lot. You'd be surprised to find any tenant using that lot seeing as how low the income requirements were for this apartment complex.\r\n	\N	\N	1	0	\N
164	293	1	0	Market Apartments - Building Way	Building 2 is a two story building with 16 units. The stairs leading to the second story are completely demolished. The top 4 units to the east are completely exposed to the elements. You could make it upstairs but it would require some sort of rope. \r\n	\N	\N	1	0	\N
165	294	1	0	Market Apartments - Building Way	Building 2 is a two story building with 16 units. The stairs leading to the second story are completely demolished. The top 4 units to the east are completely exposed to the elements. You could make it upstairs but it would require some sort of rope. \r\n	\N	\N	1	0	\N
166	295	1	0	Market Apartments - Building Way	Building 2 is a two story building with 16 units. The stairs leading to the second story are completely demolished. The top 4 units to the east are completely exposed to the elements. You could make it upstairs but it would require some sort of rope. \r\n	\N	\N	1	0	\N
167	296	1	0	Market Apartments - Building 3	You see a two story apartment building with 8 units. Each apartment is a corner unit, but neither home could possibly be more than a studio unit. A few of the doors are closed, which you find peculiar. There can't possibly be anyone living there. Right?\r\n	\N	\N	1	0	\N
168	297	1	0	Market Apartments - Stairs	Before you lie the stairs that will help you reach the second floor of building 3. The handrails are extremely hot to the touch due to baking here under the steady gaze of the sun over your shoulder.\r\n	\N	\N	1	0	\N
169	298	1	0	Market Apartments - Stairs	Before you lie the stairs that will help you reach the second floor of building 3. The handrails are extremely hot to the touch due to baking here under the steady gaze of the sun over your shoulder.\r\n	\N	\N	1	0	\N
171	300	1	0	Market Apartments - Building 3 Second floor hallway	A thin layer of dust and dirt cover the cement walkway here. You hear the crackle of dirt and millions of pieces of broken glass under your feet as you make your way about. Oddly enough, there are footprints here.\r\n	\N	\N	1	0	\N
172	301	1	0	Market Apartments - Building 3 Second floor hallway	A thin layer of dust and dirt cover the cement walkway here. You hear the crackle of dirt and millions of pieces of broken glass under your feet as you make your way about. Oddly enough, there are footprints here.\r\n	\N	\N	1	0	\N
173	302	1	0	Market Apartments - Building 3 Second floor hallway	A thin layer of dust and dirt cover the cement walkway here. You hear the crackle of dirt and millions of pieces of broken glass under your feet as you make your way about. Oddly enough, there are footprints here.\r\n	\N	\N	1	0	\N
174	303	1	0	Market Apartments - Building 3 Second floor hallway	A thin layer of dust and dirt cover the cement walkway here. You hear the crackle of dirt and millions of pieces of broken glass under your feet as you make your way about. Oddly enough, there are footprints here.\r\n	\N	\N	1	0	\N
175	304	1	0	Market Apartments - Building 3 Second floor hallway	A thin layer of dust and dirt cover the cement walkway here. You hear the crackle of dirt and millions of pieces of broken glass under your feet as you make your way about. Oddly enough, there are footprints here.\r\n	\N	\N	1	0	\N
176	305	1	0	Market Apartments - Building 3 - Unit 301	A mostly empty apartment. You notice what looks like sleeping bags in the corner but that could just be trash. The air smells like cigarette smoke and dry desert air. The light from outside illuminates trillions of dust particles floating in the in perpetual motion.\r\n	\N	\N	1	0	\N
177	306	1	0	Market Apartments - Building 3 - Unit 301	A mostly empty apartment. You notice what looks like sleeping bags in the corner but that could just be trash. The air smells like cigarette smoke and dry desert air. The light from outside illuminates trillions of dust particles floating in the in perpetual motion.\r\n	\N	\N	1	0	\N
178	307	1	0	Market Apartments - Building 3 - Unit 301 - Master Bedroom	A D.I.Y. home stove sits in the middle of the room. Aside from that, there's nothing to look at. The carpet has been pulled up in places. The walls are covered with massive holes and random burn marks.\r\n	\N	\N	1	0	\N
179	308	1	0	Market Apartments - Building 3 - Unit 301 - Bathroom	The sink seems to still be intact and usable. How any water made its way up here would be nothing short of miraculous. It does appear that the sink has been used recently, which only affirms your suspicion that there are still inhabitants who dwell here.\r\n	\N	\N	1	0	\N
180	309	1	0	Market Apartments - Building 3 Second floor hallway	A thin layer of dust and dirt cover the cement walkway here. You hear the crackle of dirt and millions of pieces of broken glass under your feet as you make your way about. Oddly enough, there are footprints here.\r\n	\N	\N	1	0	\N
181	310	1	0	Market Apartments - Building 3 Second floor hallway	A thin layer of dust and dirt cover the cement walkway here. You hear the crackle of dirt and millions of pieces of broken glass under your feet as you make your way about. Oddly enough, there are footprints here.\r\n	\N	\N	1	0	\N
182	311	1	0	Market Apartments - Building 3 Second floor hallway	A thin layer of dust and dirt cover the cement walkway here. You hear the crackle of dirt and millions of pieces of broken glass under your feet as you make your way about. Oddly enough, there are footprints here.\r\n	\N	\N	1	0	\N
184	313	1	0	Market Apartments - Building 3 - Unit 303	A mostly empty apartment. You notice what looks like sleeping bags in the corner but that could just be trash. The air smells like cigarette smoke and dry desert air. The light from outside illuminates trillions of dust particles floating in the in perpetual motion.\r\n	\N	\N	1	0	\N
185	314	1	0	Market Apartments - Building 3 - Unit 303 - Master Bedroom	undefined\r\n	\N	\N	1	0	\N
186	315	1	0	Market Apartments - Building 3 - Unit 303 - Bathroom	The sink seems to still be intact and usable. How any water made its way up here would be nothing short of miraculous. It does appear that the sink has been used recently, which only affirms your suspicion that there are still inhabitants who dwell here.\r\n	\N	\N	1	0	\N
187	316	1	0	Market Apartments - Building 3 Second floor hallway	A thin layer of dust and dirt cover the cement walkway here. You hear the crackle of dirt and millions of pieces of broken glass under your feet as you make your way about. Oddly enough, there are footprints here.\r\n	\N	\N	1	0	\N
188	317	1	0	Market Apartments - Building 3 Second floor hallway	A thin layer of dust and dirt cover the cement walkway here. You hear the crackle of dirt and millions of pieces of broken glass under your feet as you make your way about. Oddly enough, there are footprints here.\r\n	\N	\N	1	0	\N
189	318	1	0	Market Apartments - Building 3 - Unit 305	A mostly empty apartment. You notice what looks like sleeping bags in the corner but that could just be trash. The air smells like cigarette smoke and dry desert air. The light from outside illuminates trillions of dust particles floating in the in perpetual motion.\r\n	\N	\N	1	0	\N
190	319	1	0	Market Apartments - Building 3 - Unit 305	A mostly empty apartment. You notice what looks like sleeping bags in the corner but that could just be trash. The air smells like cigarette smoke and dry desert air. The light from outside illuminates trillions of dust particles floating in the in perpetual motion.\r\n	\N	\N	1	0	\N
191	320	1	0	Market Apartments - Building 3 - Unit 305 - Master Bedroom	undefined\r\n	\N	\N	1	0	\N
192	321	1	0	Market Apartments - Building 3 - Unit 305 - Bathroom	The sink seems to still be intact and usable. How any water made its way up here would be nothing short of miraculous. It does appear that the sink has been used recently, which only affirms your suspicion that there are still inhabitants who dwell here.\r\n	\N	\N	1	0	\N
193	322	1	0	name	description	\N	\N	1	0	\N
194	323	1	0	Market Apartments - Building 3 Second floor hallway	A thin layer of dust and dirt cover the cement walkway here. You hear the crackle of dirt and millions of pieces of broken glass under your feet as you make your way about. Oddly enough, there are footprints here.\r\n	\N	\N	1	0	\N
197	326	1	0	Market Apartments - Building 3 - Unit 307	A mostly empty apartment. You notice what looks like sleeping bags in the corner but that could just be trash. The air smells like cigarette smoke and dry desert air. The light from outside illuminates trillions of dust particles floating in the in perpetual motion.\r\n	\N	\N	1	0	\N
198	327	1	0	Market Apartments - Building 3 - Unit 307 - Master Bedroom	undefined\r\n	\N	\N	1	0	\N
199	328	1	0	Market Apartments - Building 3 - Unit 307 - Bathroom	The sink seems to still be intact and usable. How any water made its way up here would be nothing short of miraculous. It does appear that the sink has been used recently, which only affirms your suspicion that there are still inhabitants who dwell here.\r\n	\N	\N	1	0	\N
200	329	1	0	Market Apartments - Building Way	Building 2 is a two story building with 16 units. The stairs leading to the second story are completely demolished. The top 4 units to the east are completely exposed to the elements. You could make it upstairs but it would require some sort of rope. \r\n	\N	\N	1	0	\N
201	330	1	0	Market Apartments - Building Way	Building 2 is a two story building with 16 units. The stairs leading to the second story are completely demolished. The top 4 units to the east are completely exposed to the elements. You could make it upstairs but it would require some sort of rope. \r\n	\N	\N	1	0	\N
202	331	1	0	Market Apartments - Building Way	Building 2 is a two story building with 16 units. The stairs leading to the second story are completely demolished. The top 4 units to the east are completely exposed to the elements. You could make it upstairs but it would require some sort of rope. \r\n	\N	\N	1	0	\N
203	332	1	0	Market Apartments - Building 2	Building 1 is a two story building with 8 units with the added addition of an exercise room and what looks like the property manager's office. Both the exercise room and the office look surprisingly intact. You notice lots of fresh footprints to and from building 1 which disappear into the destruction of building 2. Despite the abandoned nature of the complex as a whole, someone is still finding use for these units.\r\n	\N	\N	1	0	\N
204	333	1	0	Abandoned Two way street - Abott Market	A trio of burning cars have become part of the debris scattered along the street. Way off to the north, you spot a working overpass. Deep long scars of blackened concrete tell a tale of destruction. To the west lies a small set of apartments once owned by the only property company to operate in this desolate portion of town. A giant construction crane is resting in the middle of the street to the north.\r\n	\N	\N	1	0	\N
205	334	1	0	Abandoned Two way street - Abott Market	A trio of burning cars have become part of the debris scattered along the street. Way off to the north, you spot a working overpass. Deep long scars of blackened concrete tell a tale of destruction. To the west lies a small set of apartments once owned by the only property company to operate in this desolate portion of town. A giant construction crane is resting in the middle of the street to the north.\r\n	\N	\N	1	0	\N
206	335	1	0	Abandoned Two way street - Abott Market	A trio of burning cars have become part of the debris scattered along the street. Way off to the north, you spot a working overpass. Deep long scars of blackened concrete tell a tale of destruction. To the west lies a small set of apartments once owned by the only property company to operate in this desolate portion of town. A giant construction crane is resting in the middle of the street to the north.\r\n	\N	\N	1	0	\N
207	336	1	0	Abandoned Two way street - Abott Market	A trio of burning cars have become part of the debris scattered along the street. Way off to the north, you spot a working overpass. Deep long scars of blackened concrete tell a tale of destruction. To the west lies a small set of apartments once owned by the only property company to operate in this desolate portion of town. A giant construction crane is resting in the middle of the street to the north.\r\n	\N	\N	1	0	\N
208	337	1	0	Abandoned Two way street - Abott Market	A trio of burning cars have become part of the debris scattered along the street. Way off to the north, you spot a working overpass. Deep long scars of blackened concrete tell a tale of destruction. To the west lies a small set of apartments once owned by the only property company to operate in this desolate portion of town. A giant construction crane is resting in the middle of the street to the north.\r\n	\N	\N	1	0	\N
209	338	1	0	{blu}C.O.F.O.B [OUTER]:{/blu} - Descending the stairs	fill me\r\n	\N	\N	1	0	\N
210	339	1	0	{blu}C.O.F.O.B [OUTER]:{/blu} - Descending the stairs	fill me\r\n	\N	\N	1	0	\N
211	340	1	0	{blu}C.O.F.O.B:{/blu} - Mess Hall stairs	Metal railings accompany the stairs leading down to the mess hall.\r\n	\N	\N	1	0	\N
212	341	1	0	{blu}C.O.F.O.B:{/blu} - Mess Hall stairs	Metal railings accompany the stairs leading down to the mess hall.\r\n	\N	\N	1	0	\N
213	342	1	0	{blu}C.O.F.O.B:{/blu} - Mess Hall stairs	Metal railings accompany the stairs leading down to the mess hall.\r\n	\N	\N	1	0	\N
214	343	1	0	{blu}C.O.F.O.B:{/blu} - Mess Hall stairs	Metal railings accompany the stairs leading down to the mess hall.\r\n	\N	\N	1	0	\N
215	344	1	0	{blu}C.O.F.O.B:{/blu} - Mess Hall stairs	Metal railings accompany the stairs leading down to the mess hall.\r\n	\N	\N	1	0	\N
216	345	1	0	{blu}C.O.F.O.B:{/blu} - Mess Hall stairs	Metal railings accompany the stairs leading down to the mess hall.\r\n	\N	\N	1	0	\N
217	346	1	0	{blu}C.O.F.O.B:{/blu} - Mess Hall stairs	Metal railings accompany the stairs leading down to the mess hall.\r\n	\N	\N	1	0	\N
218	347	1	0	{blu}C.O.F.O.B:{/blu} - Mess Hall serving area	The tile floor is immaculate in presentation. Plain grey metal chairs attend each table obediently in a neat formation of 2 chairs to each cardinal side.A few familiar dishes invite you to sit down and enjoy the hastily prepared meals. The chefs can be seen in the kitchen looking down as they prepare the food. The remote promise of coffee is enticing.\r\n	\N	\N	1	0	\N
219	348	1	0	{blu}C.O.F.O.B:{/blu} - Mess Hall serving area	The tile floor is immaculate in presentation. Plain grey metal chairs attend each table obediently in a neat formation of 2 chairs to each cardinal side.A few familiar dishes invite you to sit down and enjoy the hastily prepared meals. The chefs can be seen in the kitchen looking down as they prepare the food. The remote promise of coffee is enticing.\r\n	\N	\N	1	0	\N
20	128	1	18	MP5 repo	Feeling over burdened by money?	\N	\N	0	16	\N
1	130	1	21	{blu}COBALT:{/blu} {grn}Forward Operating Base{/grn} - Main Hallway	COBALT Air Force base. Ground zero for basic training. All initiates must follow rules and guidelines in your New Recruit handbook. Proceed {grn}North{/grn} recruit!\r\n	\N	\N	1	0	\N
232	149	1	21	{blu}C.O.F.O.B:{/blu} - Weapons Cache stairs - TOP	@FILL_ME@\r\n	\N	\N	1	0	\N
23	152	1	21	{blu}C.O.F.O.B:{/blu} - Weapons Cache stairs - Basement 1	@FILL_ME@\r\n	\N	\N	1	0	\N
48	177	1	21	{blu}C.O.F.O.B:{/blu} - Basement 2 - Waypoint Avenue North Exit	@FILL_ME@\r\n	\N	\N	1	0	cofob-armory-waypoint-avenue-exit
70	199	1	21	{blu}C.O.F.O.B:{/blu} - Eastern Exit	Before you is a sturdy metal door that prevents the outside elements from making their way inside. The door simply says {yel}Eastern Exit{/yel}. The air is less cool as the corridor leading to the east lacks the sufficient air flow. There are no ventilation shafts leading to the east, but you can see through the tempered glass window on the door that a few military police are gaurding the exit to the city outside. \r\n	\N	\N	1	0	eastexit
84	213	1	0	{blu}C.O.F.O.B [OUTER]:{/blu} - Western Hallway	The reinforced metal walls are briefly interrupted by the occasional senior officer door. As you make your way north and south, you notice that the entirety of the western wall consists of metal walls with doors that lead to the underground portion of the base. \r\n	\N	\N	1	0	\N
99	228	1	0	{blu}C.O.F.O.B [OUTER]:{/blu} - South Hallway	The reinforced metal walls are briefly interrupted by the occasional senior officer door. As you make your way north and south, you notice that the entirety of the western wall consists of metal walls with doors that lead to the underground portion of the base. \r\n	\N	\N	1	0	\N
128	257	1	21	{blu}C.O.F.O.B:{/blu} - Second floor hallway	@FILL_ME@\r\n	\N	\N	1	0	\N
221	350	1	0	{blu}C.O.F.O.B:{/blu} - Mess Hall serving area	The tile floor is immaculate in presentation. Plain grey metal chairs attend each table obediently in a neat formation of 2 chairs to each cardinal side.A few familiar dishes invite you to sit down and enjoy the hastily prepared meals. The chefs can be seen in the kitchen looking down as they prepare the food. The remote promise of coffee is enticing.\r\n	\N	\N	1	0	\N
222	351	1	0	{blu}C.O.F.O.B:{/blu} - Mess Hall serving area	The tile floor is immaculate in presentation. Plain grey metal chairs attend each table obediently in a neat formation of 2 chairs to each cardinal side.A few familiar dishes invite you to sit down and enjoy the hastily prepared meals. The chefs can be seen in the kitchen looking down as they prepare the food. The remote promise of coffee is enticing.\r\n	\N	\N	1	0	\N
223	352	1	0	{blu}C.O.F.O.B:{/blu} - Mess Hall serving area	The tile floor is immaculate in presentation. Plain grey metal chairs attend each table obediently in a neat formation of 2 chairs to each cardinal side.A few familiar dishes invite you to sit down and enjoy the hastily prepared meals. The chefs can be seen in the kitchen looking down as they prepare the food. The remote promise of coffee is enticing.\r\n	\N	\N	1	0	\N
224	353	1	0	{blu}C.O.F.O.B:{/blu} - Mess Hall serving area	The tile floor is immaculate in presentation. Plain grey metal chairs attend each table obediently in a neat formation of 2 chairs to each cardinal side.A few familiar dishes invite you to sit down and enjoy the hastily prepared meals. The chefs can be seen in the kitchen looking down as they prepare the food. The remote promise of coffee is enticing.\r\n	\N	\N	1	0	\N
225	354	1	0	{blu}C.O.F.O.B:{/blu} - Mess Hall serving area	The tile floor is immaculate in presentation. Plain grey metal chairs attend each table obediently in a neat formation of 2 chairs to each cardinal side.A few familiar dishes invite you to sit down and enjoy the hastily prepared meals. The chefs can be seen in the kitchen looking down as they prepare the food. The remote promise of coffee is enticing.\r\n	\N	\N	1	0	\N
226	355	1	0	{blu}C.O.F.O.B:{/blu} - Mess Hall serving area	The tile floor is immaculate in presentation. Plain grey metal chairs attend each table obediently in a neat formation of 2 chairs to each cardinal side.A few familiar dishes invite you to sit down and enjoy the hastily prepared meals. The chefs can be seen in the kitchen looking down as they prepare the food. The remote promise of coffee is enticing.\r\n	\N	\N	1	0	\N
227	356	1	0	{blu}C.O.F.O.B:{/blu} - Mess Hall serving area	The tile floor is immaculate in presentation. Plain grey metal chairs attend each table obediently in a neat formation of 2 chairs to each cardinal side.A few familiar dishes invite you to sit down and enjoy the hastily prepared meals. The chefs can be seen in the kitchen looking down as they prepare the food. The remote promise of coffee is enticing.\r\n	\N	\N	1	0	\N
228	357	1	0	{blu}C.O.F.O.B:{/blu} - Mess Hall serving area	The tile floor is immaculate in presentation. Plain grey metal chairs attend each table obediently in a neat formation of 2 chairs to each cardinal side.A few familiar dishes invite you to sit down and enjoy the hastily prepared meals. The chefs can be seen in the kitchen looking down as they prepare the food. The remote promise of coffee is enticing.\r\n	\N	\N	1	0	\N
229	358	1	0	{blu}C.O.F.O.B:{/blu} - Mess Hall serving area	The tile floor is immaculate in presentation. Plain grey metal chairs attend each table obediently in a neat formation of 2 chairs to each cardinal side.A few familiar dishes invite you to sit down and enjoy the hastily prepared meals. The chefs can be seen in the kitchen looking down as they prepare the food. The remote promise of coffee is enticing.\r\n	\N	\N	1	0	\N
230	359	1	0	{blu}C.O.F.O.B:{/blu} - Mess Hall serving area	The tile floor is immaculate in presentation. Plain grey metal chairs attend each table obediently in a neat formation of 2 chairs to each cardinal side.A few familiar dishes invite you to sit down and enjoy the hastily prepared meals. The chefs can be seen in the kitchen looking down as they prepare the food. The remote promise of coffee is enticing.\r\n	\N	\N	1	0	\N
231	360	1	0	{blu}C.O.F.O.B:{/blu} - Mess Hall serving area	The tile floor is immaculate in presentation. Plain grey metal chairs attend each table obediently in a neat formation of 2 chairs to each cardinal side.A few familiar dishes invite you to sit down and enjoy the hastily prepared meals. The chefs can be seen in the kitchen looking down as they prepare the food. The remote promise of coffee is enticing.\r\n	\N	\N	1	0	\N
129	258	1	0	{blu}C.O.F.O.B:{/blu} - Engineering hallway	The doors on the eastern wall are all closed and most likely locked. You see the commons area at the end of the hallway. You notice various surveillance cameras strategically placed at the corners of the ceiling. \r\n	\N	\N	1	0	\N
133	262	1	0	{blu}C.O.F.O.B:{/blu} - Commons area	A comfy looking couch and recliner welcome you to the engineering commons area. Three television screens mounted on the west wall display various graphs and statistics. A ping pong table is in the center of the room. Two large refridgerators are humming quietly in the corner. The north wall consists of large one-way tinted windows that overlook the training fields below. To the east is the entrance to server room 1B.\r\n	\N	\N	1	0	cofob-engineering-commons
150	279	1	0	Destroyed overpass - Abott Market South	Crushed and flattened by endless shells, the overpass that you just passed under creaks eternally. Now and then you hear the crumble of asphault and be bend of metal rods. The overpass connects the once thriving market place of Abott Market, but now all you see is rubble and a shell of what used to be a thriving market.\r\n	\N	\N	1	0	\N
170	299	1	0	Market Apartments - Stairs	Before you lie the stairs that will help you reach the second floor of building 3. The handrails are extremely hot to the touch due to baking here under the steady gaze of the sun over your shoulder.\r\n	\N	\N	1	0	\N
183	312	1	0	Market Apartments - Building 3 - Unit 303	A mostly empty apartment. You notice what looks like sleeping bags in the corner but that could just be trash. The air smells like cigarette smoke and dry desert air. The light from outside illuminates trillions of dust particles floating in the in perpetual motion.\r\n	\N	\N	1	0	\N
195	324	1	0	Market Apartments - Building 3 Second floor hallway	A thin layer of dust and dirt cover the cement walkway here. You hear the crackle of dirt and millions of pieces of broken glass under your feet as you make your way about. Oddly enough, there are footprints here.\r\n	\N	\N	1	0	\N
196	325	1	0	Market Apartments - Building 3 - Unit 307	A mostly empty apartment. You notice what looks like sleeping bags in the corner but that could just be trash. The air smells like cigarette smoke and dry desert air. The light from outside illuminates trillions of dust particles floating in the in perpetual motion.\r\n	\N	\N	1	0	\N
220	349	1	0	{blu}C.O.F.O.B:{/blu} - Mess Hall serving area	The tile floor is immaculate in presentation. Plain grey metal chairs attend each table obediently in a neat formation of 2 chairs to each cardinal side.A few familiar dishes invite you to sit down and enjoy the hastily prepared meals. The chefs can be seen in the kitchen looking down as they prepare the food. The remote promise of coffee is enticing.\r\n	\N	\N	1	0	\N
233	361	1	0	{blu}Crenshaw{/blu} Highway overpass	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	crenshaw-overpass-start
234	362	1	0	{blu}Crenshaw{/blu} Highway overpass - Ramp	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N
235	363	1	0	{blu}Crenshaw{/blu} Highway overpass - Ramp	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N
236	364	1	0	{blu}Crenshaw{/blu} Highway overpass - Ramp	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N
432	560	1	0	{blu}Saint Vale Church{/blu} Inside - Isle	A dark colored wooden bench is here. There is a retractable padded kneeling device below the hanging bookshelf of psalms and prayers. \r\n	\N	\N	1	0	\N
238	366	1	0	{blu}Crenshaw{/blu} Highway overpass - North	The ramp curves to the north. To the east you see a long stretch of highway that disappears over the horizon. You see a shipyard far off to the northeast. \r\n	\N	\N	1	0	\N
239	367	1	0	{blu}Crenshaw{/blu} Highway overpass - North	The ramp curves to the north. To the east you see a long stretch of highway that disappears over the horizon. You see a shipyard far off to the northeast. \r\n	\N	\N	1	0	\N
240	368	1	0	{blu}Crenshaw{/blu} Highway 94 East	Six lanes of traffic move for off to the east as far as the eye can see. You see hills of varying heights hugging the highway's north and south sides. There is sparse greenery along the freeway. \r\n	\N	\N	1	0	\N
241	369	1	0	{blu}Crenshaw{/blu} Highway 94 East	Six lanes of traffic move for off to the east as far as the eye can see. You see hills of varying heights hugging the highway's north and south sides. There is sparse greenery along the freeway. \r\n	\N	\N	1	0	\N
242	370	1	0	{blu}Crenshaw{/blu} Highway 94 East	Six lanes of traffic move for off to the east as far as the eye can see. You see hills of varying heights hugging the highway's north and south sides. There is sparse greenery along the freeway. \r\n	\N	\N	1	0	\N
243	371	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N
244	372	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N
340	468	1	0	{blu}La Mesa{/blu} Campo Road 9600	Campo Road is a bland introduction to the hot and boring town of Spruce Valley. The roads are decorated with a Mexican food restaurant every block or so. A few signs advertise the presence of the {red}Saint Vale Church{/red}.\r\n	\N	\N	1	0	campo-conrad-drive-intersection
245	373	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N
246	374	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N
247	375	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N
248	376	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N
249	377	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N
250	378	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N
251	379	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N
252	380	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N
253	381	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N
254	382	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N
255	383	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N
433	561	1	0	{blu}Saint Vale Church{/blu} Inside - Isle	A dark colored wooden bench is here. There is a retractable padded kneeling device below the hanging bookshelf of psalms and prayers. \r\n	\N	\N	1	0	\N
256	384	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N
257	385	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N
258	386	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N
259	387	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N
260	388	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N
261	389	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N
262	390	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N
263	391	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N
264	392	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N
265	393	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N
266	394	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N
267	395	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N
268	396	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N
269	397	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N
270	398	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N
271	399	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N
272	400	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	crenshaw-northern-shipping-entrance
273	401	1	0	{blu}Shipyard{/blu} Entrance	Shipping containers stacked 30 to 40 stories high blot out the sun's rays coming from the east and even overhead during lunch time. Despite the time of day, artificial lighting is needed everywhere. As you make your way deeper into the shipyard, you notice several highly armed individuals patrolling the area. These individuals are wearing masks and have extensive radio communication devices that are resistant to E.M.P.. \r\n	\N	\N	1	0	\N
274	402	1	0	{blu}Shipyard{/blu} Entrance	Shipping containers stacked 30 to 40 stories high blot out the sun's rays coming from the east and even overhead during lunch time. Despite the time of day, artificial lighting is needed everywhere. As you make your way deeper into the shipyard, you notice several highly armed individuals patrolling the area. These individuals are wearing masks and have extensive radio communication devices that are resistant to E.M.P.. \r\n	\N	\N	1	0	\N
275	403	1	0	{blu}Shipyard{/blu} Entrance	Shipping containers stacked 30 to 40 stories high blot out the sun's rays coming from the east and even overhead during lunch time. Despite the time of day, artificial lighting is needed everywhere. As you make your way deeper into the shipyard, you notice several highly armed individuals patrolling the area. These individuals are wearing masks and have extensive radio communication devices that are resistant to E.M.P.. \r\n	\N	\N	1	0	shipyard-row-a
276	404	1	0	{blu}Shipyard{/blu} Shipment Row A	All around you are red and blue shipping containers with varying amounts of rust and outer wear. A few have severe indentations from mishaps. Oddly enough, containers with giant abrasions are bent back into shape and reused. You notice what seems to be burn marks, bullet holes, and dark splattered brownish red stains on a few containers. \r\n	\N	\N	1	0	\N
277	405	1	0	{blu}Shipyard{/blu} Shipment Row A	All around you are red and blue shipping containers with varying amounts of rust and outer wear. A few have severe indentations from mishaps. Oddly enough, containers with giant abrasions are bent back into shape and reused. You notice what seems to be burn marks, bullet holes, and dark splattered brownish red stains on a few containers. \r\n	\N	\N	1	0	\N
278	406	1	0	{blu}Shipyard{/blu} Shipment Row A	All around you are red and blue shipping containers with varying amounts of rust and outer wear. A few have severe indentations from mishaps. Oddly enough, containers with giant abrasions are bent back into shape and reused. You notice what seems to be burn marks, bullet holes, and dark splattered brownish red stains on a few containers. \r\n	\N	\N	1	0	\N
279	407	1	0	{blu}Shipyard{/blu} Shipment Row A	All around you are red and blue shipping containers with varying amounts of rust and outer wear. A few have severe indentations from mishaps. Oddly enough, containers with giant abrasions are bent back into shape and reused. You notice what seems to be burn marks, bullet holes, and dark splattered brownish red stains on a few containers. \r\n	\N	\N	1	0	\N
280	408	1	0	{blu}Shipyard{/blu} Shipment Row A	All around you are red and blue shipping containers with varying amounts of rust and outer wear. A few have severe indentations from mishaps. Oddly enough, containers with giant abrasions are bent back into shape and reused. You notice what seems to be burn marks, bullet holes, and dark splattered brownish red stains on a few containers. \r\n	\N	\N	1	0	\N
281	409	1	0	{blu}Shipyard{/blu} Shipment Row A	All around you are red and blue shipping containers with varying amounts of rust and outer wear. A few have severe indentations from mishaps. Oddly enough, containers with giant abrasions are bent back into shape and reused. You notice what seems to be burn marks, bullet holes, and dark splattered brownish red stains on a few containers. \r\n	\N	\N	1	0	\N
282	410	1	0	{blu}Shipyard{/blu} Shipment Row A	All around you are red and blue shipping containers with varying amounts of rust and outer wear. A few have severe indentations from mishaps. Oddly enough, containers with giant abrasions are bent back into shape and reused. You notice what seems to be burn marks, bullet holes, and dark splattered brownish red stains on a few containers. \r\n	\N	\N	1	0	\N
283	411	1	0	{blu}Shipyard{/blu} Shipment Row A	All around you are red and blue shipping containers with varying amounts of rust and outer wear. A few have severe indentations from mishaps. Oddly enough, containers with giant abrasions are bent back into shape and reused. You notice what seems to be burn marks, bullet holes, and dark splattered brownish red stains on a few containers. \r\n	\N	\N	1	0	\N
284	412	1	0	{blu}Shipyard{/blu} Shipment Row A	All around you are red and blue shipping containers with varying amounts of rust and outer wear. A few have severe indentations from mishaps. Oddly enough, containers with giant abrasions are bent back into shape and reused. You notice what seems to be burn marks, bullet holes, and dark splattered brownish red stains on a few containers. \r\n	\N	\N	1	0	\N
285	413	1	0	{blu}Shipyard{/blu} Shipment Row A	All around you are red and blue shipping containers with varying amounts of rust and outer wear. A few have severe indentations from mishaps. Oddly enough, containers with giant abrasions are bent back into shape and reused. You notice what seems to be burn marks, bullet holes, and dark splattered brownish red stains on a few containers. \r\n	\N	\N	1	0	\N
286	414	1	0	{blu}Shipyard{/blu} Shipment Row A	All around you are red and blue shipping containers with varying amounts of rust and outer wear. A few have severe indentations from mishaps. Oddly enough, containers with giant abrasions are bent back into shape and reused. You notice what seems to be burn marks, bullet holes, and dark splattered brownish red stains on a few containers. \r\n	\N	\N	1	0	\N
287	415	1	0	{blu}Shipyard{/blu} Shipment Row A	All around you are red and blue shipping containers with varying amounts of rust and outer wear. A few have severe indentations from mishaps. Oddly enough, containers with giant abrasions are bent back into shape and reused. You notice what seems to be burn marks, bullet holes, and dark splattered brownish red stains on a few containers. \r\n	\N	\N	1	0	\N
288	416	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N
289	417	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N
423	551	1	0	{blu}Saint Vale Church{/blu} Inside - Center Isle	An elaborate purple and yellow pattern decorates the isle which leads to the altar. The corners of each isle have divine sigils engraved into the dark stain of the wood.\r\n	\N	\N	1	0	\N
290	418	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N
291	419	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N
292	420	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N
293	421	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N
294	422	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N
295	423	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N
441	569	1	0	{blu}Saint Vale Church{/blu} Inside - Altar	A table is here with a decorative cloth draped over it. On each side of the table are tall stands that hold long white candles. There is a stand with a microphone embedded into the wood.\r\n	\N	\N	1	0	\N
296	424	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N
297	425	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N
298	426	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N
299	427	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N
300	428	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N
301	429	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N
302	430	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N
303	431	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N
304	432	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N
305	433	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N
424	552	1	0	{blu}Saint Vale Church{/blu} Inside - Center Isle	An elaborate purple and yellow pattern decorates the isle which leads to the altar. The corners of each isle have divine sigils engraved into the dark stain of the wood.\r\n	\N	\N	1	0	\N
306	434	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N
307	435	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N
308	436	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N
309	437	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N
310	438	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N
311	439	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N
312	440	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N
313	441	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N
314	442	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N
315	443	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N
316	444	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N
317	445	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N
318	446	1	0	{blu}La Mesa{/blu} Campo Road Exit	Exit 10B to Campo Road.\r\n	\N	\N	1	0	\N
319	447	1	0	{blu}La Mesa{/blu} Campo Road Exit	Exit 10B to Campo Road.\r\n	\N	\N	1	0	\N
320	448	1	0	{blu}La Mesa{/blu} Campo Road Exit	Exit 10B to Campo Road.\r\n	\N	\N	1	0	\N
321	449	1	0	{blu}La Mesa{/blu} Campo Road Exit	Exit 10B to Campo Road.\r\n	\N	\N	1	0	\N
322	450	1	0	{blu}La Mesa{/blu} Exit 10B On Ramp	A bright orange sign on the right hand shoulder says {yel}END ROAD WORK{/yel}. You see a diamond shaped yellow sign with a black arrow pointing upwards and a {red}red{/red} octagon.The road slopes downwards and on the road you see 'STOP AHEAD' written in white painted on the ground. A narrow shoulder on your right is guarded by a grey metal and wooden railing.\r\n	\N	\N	1	0	\N
323	451	1	0	{blu}La Mesa{/blu} Exit 10B On Ramp	A bright orange sign on the right hand shoulder says {yel}END ROAD WORK{/yel}. You see a diamond shaped yellow sign with a black arrow pointing upwards and a {red}red{/red} octagon.The road slopes downwards and on the road you see 'STOP AHEAD' written in white painted on the ground. A narrow shoulder on your right is guarded by a grey metal and wooden railing.\r\n	\N	\N	1	0	\N
324	452	1	0	{blu}La Mesa{/blu} Exit 10B On Ramp	A bright orange sign on the right hand shoulder says {yel}END ROAD WORK{/yel}. You see a diamond shaped yellow sign with a black arrow pointing upwards and a {red}red{/red} octagon.The road slopes downwards and on the road you see 'STOP AHEAD' written in white painted on the ground. A narrow shoulder on your right is guarded by a grey metal and wooden railing.\r\n	\N	\N	1	0	\N
325	453	1	0	{blu}La Mesa{/blu} Exit 10B On Ramp	A bright orange sign on the right hand shoulder says {yel}END ROAD WORK{/yel}. You see a diamond shaped yellow sign with a black arrow pointing upwards and a {red}red{/red} octagon.The road slopes downwards and on the road you see 'STOP AHEAD' written in white painted on the ground. A narrow shoulder on your right is guarded by a grey metal and wooden railing.\r\n	\N	\N	1	0	\N
326	454	1	0	{blu}La Mesa{/blu} Exit 10B On Ramp	A bright orange sign on the right hand shoulder says {yel}END ROAD WORK{/yel}. You see a diamond shaped yellow sign with a black arrow pointing upwards and a {red}red{/red} octagon.The road slopes downwards and on the road you see 'STOP AHEAD' written in white painted on the ground. A narrow shoulder on your right is guarded by a grey metal and wooden railing.\r\n	\N	\N	1	0	\N
327	455	1	0	{blu}La Mesa{/blu} Exit 10B On Ramp	A bright orange sign on the right hand shoulder says {yel}END ROAD WORK{/yel}. You see a diamond shaped yellow sign with a black arrow pointing upwards and a {red}red{/red} octagon.The road slopes downwards and on the road you see 'STOP AHEAD' written in white painted on the ground. A narrow shoulder on your right is guarded by a grey metal and wooden railing.\r\n	\N	\N	1	0	\N
328	456	1	0	{blu}La Mesa{/blu} Kenwood Drive Intersection	You can take a left or right and you would be on Kenwood Drive. Go straight ahead and you can get back on Highway 94 East.\r\n	\N	\N	1	0	kenwood-drive-intersection
329	457	1	0	{blu}La Mesa{/blu} Kenwood Drive North	@FILL_ME@\r\n	\N	\N	1	0	\N
330	458	1	0	{blu}La Mesa{/blu} Kenwood Drive North	@FILL_ME@\r\n	\N	\N	1	0	\N
331	459	1	0	{blu}La Mesa{/blu} Kenwood Drive North	@FILL_ME@\r\n	\N	\N	1	0	\N
332	460	1	0	{blu}La Mesa{/blu} Kenwood Drive North	@FILL_ME@\r\n	\N	\N	1	0	\N
333	461	1	0	{blu}La Mesa{/blu} Kenwood Drive North	@FILL_ME@\r\n	\N	\N	1	0	\N
334	462	1	0	{blu}La Mesa{/blu} Kenwood Drive North	@FILL_ME@\r\n	\N	\N	1	0	\N
335	463	1	0	{blu}La Mesa{/blu} Kenwood Drive North	@FILL_ME@\r\n	\N	\N	1	0	\N
336	464	1	0	{blu}La Mesa{/blu} Kenwood Drive North	@FILL_ME@\r\n	\N	\N	1	0	kenwood-drive-campo-road
337	465	1	0	{blu}La Mesa{/blu} Campo Road 9600	Campo Road is a bland introduction to the hot and boring town of Spruce Valley. The roads are decorated with a Mexican food restaurant every block or so. A few signs advertise the presence of the {red}Saint Vale Church{/red}.\r\n	\N	\N	1	0	\N
338	466	1	0	{blu}La Mesa{/blu} Campo Road 9600	Campo Road is a bland introduction to the hot and boring town of Spruce Valley. The roads are decorated with a Mexican food restaurant every block or so. A few signs advertise the presence of the {red}Saint Vale Church{/red}.\r\n	\N	\N	1	0	\N
339	467	1	0	{blu}La Mesa{/blu} Campo Road 9600	Campo Road is a bland introduction to the hot and boring town of Spruce Valley. The roads are decorated with a Mexican food restaurant every block or so. A few signs advertise the presence of the {red}Saint Vale Church{/red}.\r\n	\N	\N	1	0	\N
341	469	1	0	{blu}La Mesa{/blu} Conrad Road	Nearly every building has the same Aztec style rooftops. The same construction company that worked on every building on Campo Road also worked on the apartment buildings here. That same company was shutdown due to activities related to money laundering. As low income as this neighborhood is, there are no signs of an obvious wealth inequality here. The residents here are like every other community of hard working parents trying to provide for their family.\r\n	\N	\N	1	0	\N
342	470	1	0	{blu}La Mesa{/blu} Conrad Road	Nearly every building has the same Aztec style rooftops. The same construction company that worked on every building on Campo Road also worked on the apartment buildings here. That same company was shutdown due to activities related to money laundering. As low income as this neighborhood is, there are no signs of an obvious wealth inequality here. The residents here are like every other community of hard working parents trying to provide for their family.\r\n	\N	\N	1	0	\N
343	471	1	0	{blu}La Mesa{/blu} Conrad Road	Nearly every building has the same Aztec style rooftops. The same construction company that worked on every building on Campo Road also worked on the apartment buildings here. That same company was shutdown due to activities related to money laundering. As low income as this neighborhood is, there are no signs of an obvious wealth inequality here. The residents here are like every other community of hard working parents trying to provide for their family.\r\n	\N	\N	1	0	\N
344	472	1	0	{blu}La Mesa{/blu} Conrad Road	Nearly every building has the same Aztec style rooftops. The same construction company that worked on every building on Campo Road also worked on the apartment buildings here. That same company was shutdown due to activities related to money laundering. As low income as this neighborhood is, there are no signs of an obvious wealth inequality here. The residents here are like every other community of hard working parents trying to provide for their family.\r\n	\N	\N	1	0	\N
345	473	1	0	{blu}La Mesa{/blu} Conrad Road	Nearly every building has the same Aztec style rooftops. The same construction company that worked on every building on Campo Road also worked on the apartment buildings here. That same company was shutdown due to activities related to money laundering. As low income as this neighborhood is, there are no signs of an obvious wealth inequality here. The residents here are like every other community of hard working parents trying to provide for their family.\r\n	\N	\N	1	0	\N
346	474	1	0	{blu}La Mesa{/blu} Conrad Road	Nearly every building has the same Aztec style rooftops. The same construction company that worked on every building on Campo Road also worked on the apartment buildings here. That same company was shutdown due to activities related to money laundering. As low income as this neighborhood is, there are no signs of an obvious wealth inequality here. The residents here are like every other community of hard working parents trying to provide for their family.\r\n	\N	\N	1	0	\N
347	475	1	0	{blu}La Mesa{/blu} Conrad Road	Nearly every building has the same Aztec style rooftops. The same construction company that worked on every building on Campo Road also worked on the apartment buildings here. That same company was shutdown due to activities related to money laundering. As low income as this neighborhood is, there are no signs of an obvious wealth inequality here. The residents here are like every other community of hard working parents trying to provide for their family.\r\n	\N	\N	1	0	\N
348	476	1	0	{blu}La Mesa{/blu} Conrad Road	Nearly every building has the same Aztec style rooftops. The same construction company that worked on every building on Campo Road also worked on the apartment buildings here. That same company was shutdown due to activities related to money laundering. As low income as this neighborhood is, there are no signs of an obvious wealth inequality here. The residents here are like every other community of hard working parents trying to provide for their family.\r\n	\N	\N	1	0	\N
425	553	1	0	{blu}Saint Vale Church{/blu} Inside - Center Isle	An elaborate purple and yellow pattern decorates the isle which leads to the altar. The corners of each isle have divine sigils engraved into the dark stain of the wood.\r\n	\N	\N	1	0	saint-vale-altar
349	477	1	0	{blu}La Mesa{/blu} Conrad Road	Nearly every building has the same Aztec style rooftops. The same construction company that worked on every building on Campo Road also worked on the apartment buildings here. That same company was shutdown due to activities related to money laundering. As low income as this neighborhood is, there are no signs of an obvious wealth inequality here. The residents here are like every other community of hard working parents trying to provide for their family.\r\n	\N	\N	1	0	\N
350	478	1	0	{blu}La Mesa{/blu} Conrad Road	Nearly every building has the same Aztec style rooftops. The same construction company that worked on every building on Campo Road also worked on the apartment buildings here. That same company was shutdown due to activities related to money laundering. As low income as this neighborhood is, there are no signs of an obvious wealth inequality here. The residents here are like every other community of hard working parents trying to provide for their family.\r\n	\N	\N	1	0	\N
351	479	1	0	{blu}La Mesa{/blu} Conrad Road	Nearly every building has the same Aztec style rooftops. The same construction company that worked on every building on Campo Road also worked on the apartment buildings here. That same company was shutdown due to activities related to money laundering. As low income as this neighborhood is, there are no signs of an obvious wealth inequality here. The residents here are like every other community of hard working parents trying to provide for their family.\r\n	\N	\N	1	0	\N
352	480	1	0	{blu}La Mesa{/blu} Conrad Road	Nearly every building has the same Aztec style rooftops. The same construction company that worked on every building on Campo Road also worked on the apartment buildings here. That same company was shutdown due to activities related to money laundering. As low income as this neighborhood is, there are no signs of an obvious wealth inequality here. The residents here are like every other community of hard working parents trying to provide for their family.\r\n	\N	\N	1	0	\N
353	481	1	0	{blu}La Mesa{/blu} Conrad Road	Nearly every building has the same Aztec style rooftops. The same construction company that worked on every building on Campo Road also worked on the apartment buildings here. That same company was shutdown due to activities related to money laundering. As low income as this neighborhood is, there are no signs of an obvious wealth inequality here. The residents here are like every other community of hard working parents trying to provide for their family.\r\n	\N	\N	1	0	\N
354	482	1	0	{blu}La Mesa{/blu} Conrad Road	Nearly every building has the same Aztec style rooftops. The same construction company that worked on every building on Campo Road also worked on the apartment buildings here. That same company was shutdown due to activities related to money laundering. As low income as this neighborhood is, there are no signs of an obvious wealth inequality here. The residents here are like every other community of hard working parents trying to provide for their family.\r\n	\N	\N	1	0	\N
355	483	1	0	{blu}La Mesa{/blu} Conrad Road	Nearly every building has the same Aztec style rooftops. The same construction company that worked on every building on Campo Road also worked on the apartment buildings here. That same company was shutdown due to activities related to money laundering. As low income as this neighborhood is, there are no signs of an obvious wealth inequality here. The residents here are like every other community of hard working parents trying to provide for their family.\r\n	\N	\N	1	0	\N
356	484	1	0	{blu}La Mesa{/blu} Conrad Road	Nearly every building has the same Aztec style rooftops. The same construction company that worked on every building on Campo Road also worked on the apartment buildings here. That same company was shutdown due to activities related to money laundering. As low income as this neighborhood is, there are no signs of an obvious wealth inequality here. The residents here are like every other community of hard working parents trying to provide for their family.\r\n	\N	\N	1	0	\N
357	485	1	0	{blu}La Mesa{/blu} Conrad Road	Nearly every building has the same Aztec style rooftops. The same construction company that worked on every building on Campo Road also worked on the apartment buildings here. That same company was shutdown due to activities related to money laundering. As low income as this neighborhood is, there are no signs of an obvious wealth inequality here. The residents here are like every other community of hard working parents trying to provide for their family.\r\n	\N	\N	1	0	\N
358	486	1	0	{blu}La Mesa{/blu} Conrad Road	Nearly every building has the same Aztec style rooftops. The same construction company that worked on every building on Campo Road also worked on the apartment buildings here. That same company was shutdown due to activities related to money laundering. As low income as this neighborhood is, there are no signs of an obvious wealth inequality here. The residents here are like every other community of hard working parents trying to provide for their family.\r\n	\N	\N	1	0	\N
359	487	1	0	{blu}La Mesa{/blu} Conrad Road	Nearly every building has the same Aztec style rooftops. The same construction company that worked on every building on Campo Road also worked on the apartment buildings here. That same company was shutdown due to activities related to money laundering. As low income as this neighborhood is, there are no signs of an obvious wealth inequality here. The residents here are like every other community of hard working parents trying to provide for their family.\r\n	\N	\N	1	0	\N
360	488	1	0	{blu}La Mesa{/blu} Conrad Road	Nearly every building has the same Aztec style rooftops. The same construction company that worked on every building on Campo Road also worked on the apartment buildings here. That same company was shutdown due to activities related to money laundering. As low income as this neighborhood is, there are no signs of an obvious wealth inequality here. The residents here are like every other community of hard working parents trying to provide for their family.\r\n	\N	\N	1	0	\N
361	489	1	0	{blu}La Mesa{/blu} Conrad Road	Nearly every building has the same Aztec style rooftops. The same construction company that worked on every building on Campo Road also worked on the apartment buildings here. That same company was shutdown due to activities related to money laundering. As low income as this neighborhood is, there are no signs of an obvious wealth inequality here. The residents here are like every other community of hard working parents trying to provide for their family.\r\n	\N	\N	1	0	\N
362	490	1	0	{blu}La Mesa{/blu} Conrad Road	Nearly every building has the same Aztec style rooftops. The same construction company that worked on every building on Campo Road also worked on the apartment buildings here. That same company was shutdown due to activities related to money laundering. As low income as this neighborhood is, there are no signs of an obvious wealth inequality here. The residents here are like every other community of hard working parents trying to provide for their family.\r\n	\N	\N	1	0	\N
426	554	1	0	{blu}Saint Vale Church{/blu} Inside - Isle	A dark colored wooden bench is here. There is a retractable padded kneeling device below the hanging bookshelf of psalms and prayers. \r\n	\N	\N	1	0	\N
363	491	1	0	{blu}La Mesa{/blu} Conrad Road	Nearly every building has the same Aztec style rooftops. The same construction company that worked on every building on Campo Road also worked on the apartment buildings here. That same company was shutdown due to activities related to money laundering. As low income as this neighborhood is, there are no signs of an obvious wealth inequality here. The residents here are like every other community of hard working parents trying to provide for their family.\r\n	\N	\N	1	0	\N
364	492	1	0	{blu}La Mesa{/blu} Conrad Road	Nearly every building has the same Aztec style rooftops. The same construction company that worked on every building on Campo Road also worked on the apartment buildings here. That same company was shutdown due to activities related to money laundering. As low income as this neighborhood is, there are no signs of an obvious wealth inequality here. The residents here are like every other community of hard working parents trying to provide for their family.\r\n	\N	\N	1	0	\N
365	493	1	0	{blu}La Mesa{/blu} Conrad Road	Nearly every building has the same Aztec style rooftops. The same construction company that worked on every building on Campo Road also worked on the apartment buildings here. That same company was shutdown due to activities related to money laundering. As low income as this neighborhood is, there are no signs of an obvious wealth inequality here. The residents here are like every other community of hard working parents trying to provide for their family.\r\n	\N	\N	1	0	campo-back-alley-church-entrance
366	494	1	0	{blu}La Mesa{/blu} Vale church back alley entrance	An extremely narrow alleyway leads to the Vale Church parking lot. A wall made of concrete and topped with a high iron fence is to the North. Directly to the south are cars jam packed next to each other in a feeble attempt to add more parking spaces to the apartments directly south of here. \r\n	\N	\N	1	0	\N
367	495	1	0	{blu}La Mesa{/blu} Vale church back alley entrance	An extremely narrow alleyway leads to the Vale Church parking lot. A wall made of concrete and topped with a high iron fence is to the North. Directly to the south are cars jam packed next to each other in a feeble attempt to add more parking spaces to the apartments directly south of here. \r\n	\N	\N	1	0	\N
368	496	1	0	{blu}La Mesa{/blu} Vale church back alley entrance	An extremely narrow alleyway leads to the Vale Church parking lot. A wall made of concrete and topped with a high iron fence is to the North. Directly to the south are cars jam packed next to each other in a feeble attempt to add more parking spaces to the apartments directly south of here. \r\n	\N	\N	1	0	saint-vale-nw-parking-lot
369	497	1	0	{blu}Saint Vale Church{/blu} Parking Lot	A huge parking lot for a giant congregation. There are administration and community buildings to the north where people often hold weddings and other communal gatherings. To the south you spot a building where the staff go when services are over. It seems to be heavily fortified, which seems odd, given that this is a place of worship. The Church itself is to the East and it's main bell tower casts a looming shadow over the rest of the building and parking lot.\r\n	\N	\N	1	0	\N
370	498	1	0	{blu}Saint Vale Church{/blu} Parking Lot	A huge parking lot for a giant congregation. There are administration and community buildings to the north where people often hold weddings and other communal gatherings. To the south you spot a building where the staff go when services are over. It seems to be heavily fortified, which seems odd, given that this is a place of worship. The Church itself is to the East and it's main bell tower casts a looming shadow over the rest of the building and parking lot.\r\n	\N	\N	1	0	\N
371	499	1	0	{blu}Saint Vale Church{/blu} Parking Lot	A huge parking lot for a giant congregation. There are administration and community buildings to the north where people often hold weddings and other communal gatherings. To the south you spot a building where the staff go when services are over. It seems to be heavily fortified, which seems odd, given that this is a place of worship. The Church itself is to the East and it's main bell tower casts a looming shadow over the rest of the building and parking lot.\r\n	\N	\N	1	0	\N
372	500	1	0	{blu}Saint Vale Church{/blu} Parking Lot	A huge parking lot for a giant congregation. There are administration and community buildings to the north where people often hold weddings and other communal gatherings. To the south you spot a building where the staff go when services are over. It seems to be heavily fortified, which seems odd, given that this is a place of worship. The Church itself is to the East and it's main bell tower casts a looming shadow over the rest of the building and parking lot.\r\n	\N	\N	1	0	\N
373	501	1	0	{blu}Saint Vale Church{/blu} Parking Lot	A huge parking lot for a giant congregation. There are administration and community buildings to the north where people often hold weddings and other communal gatherings. To the south you spot a building where the staff go when services are over. It seems to be heavily fortified, which seems odd, given that this is a place of worship. The Church itself is to the East and it's main bell tower casts a looming shadow over the rest of the building and parking lot.\r\n	\N	\N	1	0	\N
374	502	1	0	{blu}Saint Vale Church{/blu} Parking Lot	A huge parking lot for a giant congregation. There are administration and community buildings to the north where people often hold weddings and other communal gatherings. To the south you spot a building where the staff go when services are over. It seems to be heavily fortified, which seems odd, given that this is a place of worship. The Church itself is to the East and it's main bell tower casts a looming shadow over the rest of the building and parking lot.\r\n	\N	\N	1	0	\N
375	503	1	0	{blu}Saint Vale Church{/blu} Parking Lot	A huge parking lot for a giant congregation. There are administration and community buildings to the north where people often hold weddings and other communal gatherings. To the south you spot a building where the staff go when services are over. It seems to be heavily fortified, which seems odd, given that this is a place of worship. The Church itself is to the East and it's main bell tower casts a looming shadow over the rest of the building and parking lot.\r\n	\N	\N	1	0	\N
376	504	1	0	{blu}Saint Vale Church{/blu} Parking Lot	A huge parking lot for a giant congregation. There are administration and community buildings to the north where people often hold weddings and other communal gatherings. To the south you spot a building where the staff go when services are over. It seems to be heavily fortified, which seems odd, given that this is a place of worship. The Church itself is to the East and it's main bell tower casts a looming shadow over the rest of the building and parking lot.\r\n	\N	\N	1	0	\N
427	555	1	0	{blu}Saint Vale Church{/blu} Inside - Isle	A dark colored wooden bench is here. There is a retractable padded kneeling device below the hanging bookshelf of psalms and prayers. \r\n	\N	\N	1	0	\N
428	556	1	0	{blu}Saint Vale Church{/blu} Inside - Isle	A dark colored wooden bench is here. There is a retractable padded kneeling device below the hanging bookshelf of psalms and prayers. \r\n	\N	\N	1	0	\N
377	505	1	0	{blu}Saint Vale Church{/blu} Parking Lot	A huge parking lot for a giant congregation. There are administration and community buildings to the north where people often hold weddings and other communal gatherings. To the south you spot a building where the staff go when services are over. It seems to be heavily fortified, which seems odd, given that this is a place of worship. The Church itself is to the East and it's main bell tower casts a looming shadow over the rest of the building and parking lot.\r\n	\N	\N	1	0	\N
378	506	1	0	{blu}Saint Vale Church{/blu} Parking Lot	A huge parking lot for a giant congregation. There are administration and community buildings to the north where people often hold weddings and other communal gatherings. To the south you spot a building where the staff go when services are over. It seems to be heavily fortified, which seems odd, given that this is a place of worship. The Church itself is to the East and it's main bell tower casts a looming shadow over the rest of the building and parking lot.\r\n	\N	\N	1	0	\N
379	507	1	0	{blu}Saint Vale Church{/blu} Parking Lot	A huge parking lot for a giant congregation. There are administration and community buildings to the north where people often hold weddings and other communal gatherings. To the south you spot a building where the staff go when services are over. It seems to be heavily fortified, which seems odd, given that this is a place of worship. The Church itself is to the East and it's main bell tower casts a looming shadow over the rest of the building and parking lot.\r\n	\N	\N	1	0	\N
380	508	1	0	{blu}Saint Vale Church{/blu} Parking Lot	A huge parking lot for a giant congregation. There are administration and community buildings to the north where people often hold weddings and other communal gatherings. To the south you spot a building where the staff go when services are over. It seems to be heavily fortified, which seems odd, given that this is a place of worship. The Church itself is to the East and it's main bell tower casts a looming shadow over the rest of the building and parking lot.\r\n	\N	\N	1	0	saint-vale-west-entrance
381	509	1	0	{blu}Saint Vale Church{/blu} Parking Lot - Row A	A huge parking lot for a giant congregation. There are administration and community buildings to the north where people often hold weddings and other communal gatherings. To the south you spot a building where the staff go when services are over. It seems to be heavily fortified, which seems odd, given that this is a place of worship. The Church itself is to the East and it's main bell tower casts a looming shadow over the rest of the building and parking lot.\r\n	\N	\N	1	0	\N
382	510	1	0	{blu}Saint Vale Church{/blu} Parking Lot - Row A	A huge parking lot for a giant congregation. There are administration and community buildings to the north where people often hold weddings and other communal gatherings. To the south you spot a building where the staff go when services are over. It seems to be heavily fortified, which seems odd, given that this is a place of worship. The Church itself is to the East and it's main bell tower casts a looming shadow over the rest of the building and parking lot.\r\n	\N	\N	1	0	\N
383	511	1	0	{blu}Saint Vale Church{/blu} Parking Lot - Row A	A huge parking lot for a giant congregation. There are administration and community buildings to the north where people often hold weddings and other communal gatherings. To the south you spot a building where the staff go when services are over. It seems to be heavily fortified, which seems odd, given that this is a place of worship. The Church itself is to the East and it's main bell tower casts a looming shadow over the rest of the building and parking lot.\r\n	\N	\N	1	0	\N
384	512	1	0	{blu}Saint Vale Church{/blu} Parking Lot - Row A	A huge parking lot for a giant congregation. There are administration and community buildings to the north where people often hold weddings and other communal gatherings. To the south you spot a building where the staff go when services are over. It seems to be heavily fortified, which seems odd, given that this is a place of worship. The Church itself is to the East and it's main bell tower casts a looming shadow over the rest of the building and parking lot.\r\n	\N	\N	1	0	\N
385	513	1	0	{blu}Saint Vale Church{/blu} Parking Lot - Row B	A huge parking lot for a giant congregation. There are administration and community buildings to the north where people often hold weddings and other communal gatherings. To the south you spot a building where the staff go when services are over. It seems to be heavily fortified, which seems odd, given that this is a place of worship. The Church itself is to the East and it's main bell tower casts a looming shadow over the rest of the building and parking lot.\r\n	\N	\N	1	0	\N
386	514	1	0	{blu}Saint Vale Church{/blu} Parking Lot - Row B	A huge parking lot for a giant congregation. There are administration and community buildings to the north where people often hold weddings and other communal gatherings. To the south you spot a building where the staff go when services are over. It seems to be heavily fortified, which seems odd, given that this is a place of worship. The Church itself is to the East and it's main bell tower casts a looming shadow over the rest of the building and parking lot.\r\n	\N	\N	1	0	\N
387	515	1	0	{blu}Saint Vale Church{/blu} Parking Lot - Row B	A huge parking lot for a giant congregation. There are administration and community buildings to the north where people often hold weddings and other communal gatherings. To the south you spot a building where the staff go when services are over. It seems to be heavily fortified, which seems odd, given that this is a place of worship. The Church itself is to the East and it's main bell tower casts a looming shadow over the rest of the building and parking lot.\r\n	\N	\N	1	0	\N
388	516	1	0	{blu}Saint Vale Church{/blu} Parking Lot - Row B	A huge parking lot for a giant congregation. There are administration and community buildings to the north where people often hold weddings and other communal gatherings. To the south you spot a building where the staff go when services are over. It seems to be heavily fortified, which seems odd, given that this is a place of worship. The Church itself is to the East and it's main bell tower casts a looming shadow over the rest of the building and parking lot.\r\n	\N	\N	1	0	\N
389	517	1	0	{blu}Saint Vale Church{/blu} Parking Lot - Row B	A huge parking lot for a giant congregation. There are administration and community buildings to the north where people often hold weddings and other communal gatherings. To the south you spot a building where the staff go when services are over. It seems to be heavily fortified, which seems odd, given that this is a place of worship. The Church itself is to the East and it's main bell tower casts a looming shadow over the rest of the building and parking lot.\r\n	\N	\N	1	0	\N
429	557	1	0	{blu}Saint Vale Church{/blu} Inside - Isle	A dark colored wooden bench is here. There is a retractable padded kneeling device below the hanging bookshelf of psalms and prayers. \r\n	\N	\N	1	0	\N
430	558	1	0	{blu}Saint Vale Church{/blu} Inside - Isle	A dark colored wooden bench is here. There is a retractable padded kneeling device below the hanging bookshelf of psalms and prayers. \r\n	\N	\N	1	0	\N
390	518	1	0	{blu}Saint Vale Church{/blu} Parking Lot - Row B	A huge parking lot for a giant congregation. There are administration and community buildings to the north where people often hold weddings and other communal gatherings. To the south you spot a building where the staff go when services are over. It seems to be heavily fortified, which seems odd, given that this is a place of worship. The Church itself is to the East and it's main bell tower casts a looming shadow over the rest of the building and parking lot.\r\n	\N	\N	1	0	\N
391	519	1	0	{blu}Saint Vale Church{/blu} Parking Lot - Row B	A huge parking lot for a giant congregation. There are administration and community buildings to the north where people often hold weddings and other communal gatherings. To the south you spot a building where the staff go when services are over. It seems to be heavily fortified, which seems odd, given that this is a place of worship. The Church itself is to the East and it's main bell tower casts a looming shadow over the rest of the building and parking lot.\r\n	\N	\N	1	0	\N
392	520	1	0	{blu}Saint Vale Church{/blu} Parking Lot - Row B	A huge parking lot for a giant congregation. There are administration and community buildings to the north where people often hold weddings and other communal gatherings. To the south you spot a building where the staff go when services are over. It seems to be heavily fortified, which seems odd, given that this is a place of worship. The Church itself is to the East and it's main bell tower casts a looming shadow over the rest of the building and parking lot.\r\n	\N	\N	1	0	\N
393	521	1	0	{blu}Saint Vale Church{/blu} Parking Lot - Row C	A huge parking lot for a giant congregation. There are administration and community buildings to the north where people often hold weddings and other communal gatherings. To the south you spot a building where the staff go when services are over. It seems to be heavily fortified, which seems odd, given that this is a place of worship. The Church itself is to the East and it's main bell tower casts a looming shadow over the rest of the building and parking lot.\r\n	\N	\N	1	0	\N
394	522	1	0	{blu}Saint Vale Church{/blu} Parking Lot - Row C	A huge parking lot for a giant congregation. There are administration and community buildings to the north where people often hold weddings and other communal gatherings. To the south you spot a building where the staff go when services are over. It seems to be heavily fortified, which seems odd, given that this is a place of worship. The Church itself is to the East and it's main bell tower casts a looming shadow over the rest of the building and parking lot.\r\n	\N	\N	1	0	\N
395	523	1	0	{blu}Saint Vale Church{/blu} Parking Lot - Row C	A huge parking lot for a giant congregation. There are administration and community buildings to the north where people often hold weddings and other communal gatherings. To the south you spot a building where the staff go when services are over. It seems to be heavily fortified, which seems odd, given that this is a place of worship. The Church itself is to the East and it's main bell tower casts a looming shadow over the rest of the building and parking lot.\r\n	\N	\N	1	0	\N
396	524	1	0	{blu}Saint Vale Church{/blu} Parking Lot - Row C	A huge parking lot for a giant congregation. There are administration and community buildings to the north where people often hold weddings and other communal gatherings. To the south you spot a building where the staff go when services are over. It seems to be heavily fortified, which seems odd, given that this is a place of worship. The Church itself is to the East and it's main bell tower casts a looming shadow over the rest of the building and parking lot.\r\n	\N	\N	1	0	\N
442	570	1	0	{blu}Saint Vale Church{/blu} Inside - Altar	A table is here with a decorative cloth draped over it. On each side of the table are tall stands that hold long white candles. There is a stand with a microphone embedded into the wood.\r\n	\N	\N	1	0	\N
397	525	1	0	{blu}Saint Vale Church{/blu} Inside	Sanctuary. A well ventilated and air conditioned cathedral. The west entrance of Saint Vale Church lets a little bit of outside light leak inside as the door closes automatically behind you. The smell of incense and blown out candles is hard to ignore, yet it's a very pleasant and calming feeling. Each of the fifty rows of seating is 100 feet across. Whoever built this Church had significant amounts of money, possibly even ludicrous amounts. \r\n	\N	\N	1	0	\N
398	526	1	0	{blu}Saint Vale Church{/blu} Inside	Sanctuary. A well ventilated and air conditioned cathedral. The west entrance of Saint Vale Church lets a little bit of outside light leak inside as the door closes automatically behind you. The smell of incense and blown out candles is hard to ignore, yet it's a very pleasant and calming feeling. Each of the fifty rows of seating is 100 feet across. Whoever built this Church had significant amounts of money, possibly even ludicrous amounts. \r\n	\N	\N	1	0	\N
399	527	1	0	{blu}Saint Vale Church{/blu} Inside	Sanctuary. A well ventilated and air conditioned cathedral. The west entrance of Saint Vale Church lets a little bit of outside light leak inside as the door closes automatically behind you. The smell of incense and blown out candles is hard to ignore, yet it's a very pleasant and calming feeling. Each of the fifty rows of seating is 100 feet across. Whoever built this Church had significant amounts of money, possibly even ludicrous amounts. \r\n	\N	\N	1	0	\N
400	528	1	0	{blu}Saint Vale Church{/blu} Inside	Sanctuary. A well ventilated and air conditioned cathedral. The west entrance of Saint Vale Church lets a little bit of outside light leak inside as the door closes automatically behind you. The smell of incense and blown out candles is hard to ignore, yet it's a very pleasant and calming feeling. Each of the fifty rows of seating is 100 feet across. Whoever built this Church had significant amounts of money, possibly even ludicrous amounts. \r\n	\N	\N	1	0	\N
401	529	1	0	{blu}Saint Vale Church{/blu} Inside	Sanctuary. A well ventilated and air conditioned cathedral. The west entrance of Saint Vale Church lets a little bit of outside light leak inside as the door closes automatically behind you. The smell of incense and blown out candles is hard to ignore, yet it's a very pleasant and calming feeling. Each of the fifty rows of seating is 100 feet across. Whoever built this Church had significant amounts of money, possibly even ludicrous amounts. \r\n	\N	\N	1	0	\N
402	530	1	0	{blu}Saint Vale Church{/blu} Inside	Sanctuary. A well ventilated and air conditioned cathedral. The west entrance of Saint Vale Church lets a little bit of outside light leak inside as the door closes automatically behind you. The smell of incense and blown out candles is hard to ignore, yet it's a very pleasant and calming feeling. Each of the fifty rows of seating is 100 feet across. Whoever built this Church had significant amounts of money, possibly even ludicrous amounts. \r\n	\N	\N	1	0	\N
431	559	1	0	{blu}Saint Vale Church{/blu} Inside - Isle	A dark colored wooden bench is here. There is a retractable padded kneeling device below the hanging bookshelf of psalms and prayers. \r\n	\N	\N	1	0	\N
403	531	1	0	{blu}Saint Vale Church{/blu} Inside - Rear Hall	The wall to the south is made of a repurposed sort of brick layering that makes it almost look as if it was a design decision that came long after the architecture of the church was already decided. Something seems very off about this wall and you almost feel like you are being watched here. A door leading to the bathroom hallway is to the south of here. You can take a seat in one of the many rows of seating to the north of here.\r\n	\N	\N	1	0	\N
404	532	1	0	{blu}Saint Vale Church{/blu} Inside - Rear Hall	The wall to the south is made of a repurposed sort of brick layering that makes it almost look as if it was a design decision that came long after the architecture of the church was already decided. Something seems very off about this wall and you almost feel like you are being watched here. A door leading to the bathroom hallway is to the south of here. You can take a seat in one of the many rows of seating to the north of here.\r\n	\N	\N	1	0	\N
405	533	1	0	{blu}Saint Vale Church{/blu} Inside - Rear Hall	The wall to the south is made of a repurposed sort of brick layering that makes it almost look as if it was a design decision that came long after the architecture of the church was already decided. Something seems very off about this wall and you almost feel like you are being watched here. A door leading to the bathroom hallway is to the south of here. You can take a seat in one of the many rows of seating to the north of here.\r\n	\N	\N	1	0	\N
406	534	1	0	{blu}Saint Vale Church{/blu} Inside - Rear Hall	The wall to the south is made of a repurposed sort of brick layering that makes it almost look as if it was a design decision that came long after the architecture of the church was already decided. Something seems very off about this wall and you almost feel like you are being watched here. A door leading to the bathroom hallway is to the south of here. You can take a seat in one of the many rows of seating to the north of here.\r\n	\N	\N	1	0	saint-vale-rear-east
407	535	1	0	{blu}Saint Vale Church{/blu} Inside - Bathroom Hall	Oddly enough, this hallway is not air conditioned. The male bathroom is just to the west and the female bathroom is to the east.\r\n	\N	\N	1	0	\N
408	536	1	0	{blu}Saint Vale Church{/blu} Inside - Bathroom Hall	Oddly enough, this hallway is not air conditioned. The male bathroom is just to the west and the female bathroom is to the east.\r\n	\N	\N	1	0	\N
409	537	1	0	{blu}Saint Vale Church{/blu} Inside - Male Bathroom	The walls are painted a soothing orange and white color and the ceiling fan generates a constant white noise hum that lulls you into a meditative state.\r\n	\N	\N	1	0	\N
410	538	1	0	{blu}Saint Vale Church{/blu} Inside - Male Bathroom	The walls are painted a soothing orange and white color and the ceiling fan generates a constant white noise hum that lulls you into a meditative state.\r\n	\N	\N	1	0	\N
411	539	1	0	{blu}Saint Vale Church{/blu} Inside - Female Bathroom	The walls are painted a soothing orange and white color and the ceiling fan generates a constant white noise hum that lulls you into a meditative state.\r\n	\N	\N	1	0	\N
412	540	1	0	{blu}Saint Vale Church{/blu} Inside - Female Bathroom	The walls are painted a soothing orange and white color and the ceiling fan generates a constant white noise hum that lulls you into a meditative state.\r\n	\N	\N	1	0	\N
413	541	1	0	{blu}Saint Vale Church{/blu} Inside - Rear Hall	The wall to the south is made of a repurposed sort of brick layering that makes it almost look as if it was a design decision that came long after the architecture of the church was already decided. Something seems very off about this wall and you almost feel like you are being watched here. A door leading to the bathroom hallway is to the south of here. You can take a seat in one of the many rows of seating to the north of here.\r\n	\N	\N	1	0	\N
443	571	1	0	{blu}Saint Vale Church{/blu} Inside - Altar	A table is here with a decorative cloth draped over it. On each side of the table are tall stands that hold long white candles. There is a stand with a microphone embedded into the wood.\r\n	\N	\N	1	0	\N
414	542	1	0	{blu}Saint Vale Church{/blu} Inside - Rear Hall	The wall to the south is made of a repurposed sort of brick layering that makes it almost look as if it was a design decision that came long after the architecture of the church was already decided. Something seems very off about this wall and you almost feel like you are being watched here. A door leading to the bathroom hallway is to the south of here. You can take a seat in one of the many rows of seating to the north of here.\r\n	\N	\N	1	0	\N
415	543	1	0	{blu}Saint Vale Church{/blu} Inside - Rear Hall	The wall to the south is made of a repurposed sort of brick layering that makes it almost look as if it was a design decision that came long after the architecture of the church was already decided. Something seems very off about this wall and you almost feel like you are being watched here. A door leading to the bathroom hallway is to the south of here. You can take a seat in one of the many rows of seating to the north of here.\r\n	\N	\N	1	0	\N
416	544	1	0	{blu}Saint Vale Church{/blu} Inside - Rear Hall	The wall to the south is made of a repurposed sort of brick layering that makes it almost look as if it was a design decision that came long after the architecture of the church was already decided. Something seems very off about this wall and you almost feel like you are being watched here. A door leading to the bathroom hallway is to the south of here. You can take a seat in one of the many rows of seating to the north of here.\r\n	\N	\N	1	0	\N
417	545	1	0	{blu}Saint Vale Church{/blu} Inside - Center Isle	An elaborate purple and yellow pattern decorates the isle which leads to the altar. The corners of each isle have divine sigils engraved into the dark stain of the wood.\r\n	\N	\N	1	0	\N
418	546	1	0	{blu}Saint Vale Church{/blu} Inside - Center Isle	An elaborate purple and yellow pattern decorates the isle which leads to the altar. The corners of each isle have divine sigils engraved into the dark stain of the wood.\r\n	\N	\N	1	0	\N
419	547	1	0	{blu}Saint Vale Church{/blu} Inside - Center Isle	An elaborate purple and yellow pattern decorates the isle which leads to the altar. The corners of each isle have divine sigils engraved into the dark stain of the wood.\r\n	\N	\N	1	0	\N
420	548	1	0	{blu}Saint Vale Church{/blu} Inside - Center Isle	An elaborate purple and yellow pattern decorates the isle which leads to the altar. The corners of each isle have divine sigils engraved into the dark stain of the wood.\r\n	\N	\N	1	0	\N
421	549	1	0	{blu}Saint Vale Church{/blu} Inside - Center Isle	An elaborate purple and yellow pattern decorates the isle which leads to the altar. The corners of each isle have divine sigils engraved into the dark stain of the wood.\r\n	\N	\N	1	0	\N
422	550	1	0	{blu}Saint Vale Church{/blu} Inside - Center Isle	An elaborate purple and yellow pattern decorates the isle which leads to the altar. The corners of each isle have divine sigils engraved into the dark stain of the wood.\r\n	\N	\N	1	0	\N
434	562	1	0	{blu}Saint Vale Church{/blu} Inside - Isle	A dark colored wooden bench is here. There is a retractable padded kneeling device below the hanging bookshelf of psalms and prayers. \r\n	\N	\N	1	0	\N
435	563	1	0	{blu}Saint Vale Church{/blu} Inside - Isle	A dark colored wooden bench is here. There is a retractable padded kneeling device below the hanging bookshelf of psalms and prayers. \r\n	\N	\N	1	0	\N
436	564	1	0	{blu}Saint Vale Church{/blu} Inside - Isle	A dark colored wooden bench is here. There is a retractable padded kneeling device below the hanging bookshelf of psalms and prayers. \r\n	\N	\N	1	0	\N
437	565	1	0	{blu}Saint Vale Church{/blu} Inside - Isle	A dark colored wooden bench is here. There is a retractable padded kneeling device below the hanging bookshelf of psalms and prayers. \r\n	\N	\N	1	0	\N
438	566	1	0	{blu}Saint Vale Church{/blu} Inside - Altar	A table is here with a decorative cloth draped over it. On each side of the table are tall stands that hold long white candles. There is a stand with a microphone embedded into the wood.\r\n	\N	\N	1	0	\N
439	567	1	0	{blu}Saint Vale Church{/blu} Inside - Altar	A table is here with a decorative cloth draped over it. On each side of the table are tall stands that hold long white candles. There is a stand with a microphone embedded into the wood.\r\n	\N	\N	1	0	\N
440	568	1	0	{blu}Saint Vale Church{/blu} Inside - Altar	A table is here with a decorative cloth draped over it. On each side of the table are tall stands that hold long white candles. There is a stand with a microphone embedded into the wood.\r\n	\N	\N	1	0	\N
444	572	1	0	{blu}Saint Vale Church{/blu} Inside - Altar	A table is here with a decorative cloth draped over it. On each side of the table are tall stands that hold long white candles. There is a stand with a microphone embedded into the wood.\r\n	\N	\N	1	0	\N
445	573	1	0	{blu}Saint Vale Church{/blu} Inside - Altar	A table is here with a decorative cloth draped over it. On each side of the table are tall stands that hold long white candles. There is a stand with a microphone embedded into the wood.\r\n	\N	\N	1	0	\N
446	574	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Entrance	A large green and white sign looms above your head. {grn}Allied Foods{/grn} is the main food source for the local community. It's central location is within walking distance for most of the residents.\r\n	\N	\N	1	0	\N
447	575	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Carts	Four rows of black shopping carts are off to the left. The automatic door in front of you gives you a nice gust of cool air-conditioned air.\r\n	\N	\N	1	0	\N
448	576	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Pharmacy	A Pharmacy\r\n	\N	\N	1	0	\N
449	577	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Pharmacy	A Pharmacy\r\n	\N	\N	1	0	\N
450	578	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Pharmacy	A Pharmacy\r\n	\N	\N	1	0	allied-foods-pharmacy-bend
451	579	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Pharmacy	A Pharmacy\r\n	\N	\N	1	0	\N
452	580	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Pharmacy	A Pharmacy\r\n	\N	\N	1	0	\N
453	581	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Pharmacy	A Pharmacy\r\n	\N	\N	1	0	\N
454	582	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Shower Isle	A shower isle\r\n	\N	\N	1	0	\N
455	583	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Shower Isle	A shower isle\r\n	\N	\N	1	0	\N
456	584	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Shower Isle	A shower isle\r\n	\N	\N	1	0	\N
457	585	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Shower Isle	A shower isle\r\n	\N	\N	1	0	\N
458	586	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Shower Isle	A shower isle\r\n	\N	\N	1	0	\N
459	587	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Shower Isle	A shower isle\r\n	\N	\N	1	0	\N
460	588	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Shower Isle	A shower isle\r\n	\N	\N	1	0	\N
461	589	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Shower Isle	A shower isle\r\n	\N	\N	1	0	allied-foods-shower-end
462	590	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Dairy	The isle is dimly lit but that doesn't affect your vision of the contents of this isle. Every product here is in a white container. Milks, butters, cream cheese.. all sorts of kinds of almond milks.. At the end of the isle you notice an end cap displaying a high grade vokda. How appropriate.\r\n	\N	\N	1	0	\N
463	591	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Dairy	The isle is dimly lit but that doesn't affect your vision of the contents of this isle. Every product here is in a white container. Milks, butters, cream cheese.. all sorts of kinds of almond milks.. At the end of the isle you notice an end cap displaying a high grade vokda. How appropriate.\r\n	\N	\N	1	0	\N
464	592	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Dairy	The isle is dimly lit but that doesn't affect your vision of the contents of this isle. Every product here is in a white container. Milks, butters, cream cheese.. all sorts of kinds of almond milks.. At the end of the isle you notice an end cap displaying a high grade vokda. How appropriate.\r\n	\N	\N	1	0	allied-foods-dairy-end
465	593	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Alcohol Isle	Both sides of the isle contain huge amounts of name brand alcoholic beverages. You notice that there aren't any beers. The only types of alcohol are high strength variants like vodka, rum, and whiskey. An {red}explosion{/red} in this isle would be {grn}fatal{/grn}.\r\n	\N	\N	1	0	\N
466	594	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Alcohol Isle	Both sides of the isle contain huge amounts of name brand alcoholic beverages. You notice that there aren't any beers. The only types of alcohol are high strength variants like vodka, rum, and whiskey. An {red}explosion{/red} in this isle would be {grn}fatal{/grn}.\r\n	\N	\N	1	0	\N
467	595	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Alcohol Isle	Both sides of the isle contain huge amounts of name brand alcoholic beverages. You notice that there aren't any beers. The only types of alcohol are high strength variants like vodka, rum, and whiskey. An {red}explosion{/red} in this isle would be {grn}fatal{/grn}.\r\n	\N	\N	1	0	\N
468	596	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Alcohol Isle	Both sides of the isle contain huge amounts of name brand alcoholic beverages. You notice that there aren't any beers. The only types of alcohol are high strength variants like vodka, rum, and whiskey. An {red}explosion{/red} in this isle would be {grn}fatal{/grn}.\r\n	\N	\N	1	0	\N
469	597	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Alcohol Isle	Both sides of the isle contain huge amounts of name brand alcoholic beverages. You notice that there aren't any beers. The only types of alcohol are high strength variants like vodka, rum, and whiskey. An {red}explosion{/red} in this isle would be {grn}fatal{/grn}.\r\n	\N	\N	1	0	\N
470	598	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Alcohol Isle	Both sides of the isle contain huge amounts of name brand alcoholic beverages. You notice that there aren't any beers. The only types of alcohol are high strength variants like vodka, rum, and whiskey. An {red}explosion{/red} in this isle would be {grn}fatal{/grn}.\r\n	\N	\N	1	0	\N
471	599	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Alcohol Isle	Both sides of the isle contain huge amounts of name brand alcoholic beverages. You notice that there aren't any beers. The only types of alcohol are high strength variants like vodka, rum, and whiskey. An {red}explosion{/red} in this isle would be {grn}fatal{/grn}.\r\n	\N	\N	1	0	\N
472	600	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Alcohol Isle	Both sides of the isle contain huge amounts of name brand alcoholic beverages. You notice that there aren't any beers. The only types of alcohol are high strength variants like vodka, rum, and whiskey. An {red}explosion{/red} in this isle would be {grn}fatal{/grn}.\r\n	\N	\N	1	0	allied-foods-alcohol-end
473	601	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Dairy	The isle is dimly lit but that doesn't affect your vision of the contents of this isle. Every product here is in a white container. Milks, butters, cream cheese.. all sorts of kinds of almond milks.. At the end of the isle you notice an end cap displaying a high grade vokda. How appropriate.\r\n	\N	\N	1	0	\N
474	602	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Dairy	The isle is dimly lit but that doesn't affect your vision of the contents of this isle. Every product here is in a white container. Milks, butters, cream cheese.. all sorts of kinds of almond milks.. At the end of the isle you notice an end cap displaying a high grade vokda. How appropriate.\r\n	\N	\N	1	0	\N
475	603	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Dairy	The isle is dimly lit but that doesn't affect your vision of the contents of this isle. Every product here is in a white container. Milks, butters, cream cheese.. all sorts of kinds of almond milks.. At the end of the isle you notice an end cap displaying a high grade vokda. How appropriate.\r\n	\N	\N	1	0	allied-foods-dairy-end
476	604	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Alcohol Isle 2	Both sides of the isle contain huge amounts of name brand alcoholic beverages. You notice that there aren't any beers. The only types of alcohol are high strength variants like vodka, rum, and whiskey. An {red}explosion{/red} in this isle would be {grn}fatal{/grn}.\r\n	\N	\N	1	0	\N
477	605	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Alcohol Isle 2	Both sides of the isle contain huge amounts of name brand alcoholic beverages. You notice that there aren't any beers. The only types of alcohol are high strength variants like vodka, rum, and whiskey. An {red}explosion{/red} in this isle would be {grn}fatal{/grn}.\r\n	\N	\N	1	0	\N
478	606	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Alcohol Isle 2	Both sides of the isle contain huge amounts of name brand alcoholic beverages. You notice that there aren't any beers. The only types of alcohol are high strength variants like vodka, rum, and whiskey. An {red}explosion{/red} in this isle would be {grn}fatal{/grn}.\r\n	\N	\N	1	0	\N
479	607	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Alcohol Isle 2	Both sides of the isle contain huge amounts of name brand alcoholic beverages. You notice that there aren't any beers. The only types of alcohol are high strength variants like vodka, rum, and whiskey. An {red}explosion{/red} in this isle would be {grn}fatal{/grn}.\r\n	\N	\N	1	0	\N
480	608	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Alcohol Isle 2	Both sides of the isle contain huge amounts of name brand alcoholic beverages. You notice that there aren't any beers. The only types of alcohol are high strength variants like vodka, rum, and whiskey. An {red}explosion{/red} in this isle would be {grn}fatal{/grn}.\r\n	\N	\N	1	0	\N
481	609	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Alcohol Isle 2	Both sides of the isle contain huge amounts of name brand alcoholic beverages. You notice that there aren't any beers. The only types of alcohol are high strength variants like vodka, rum, and whiskey. An {red}explosion{/red} in this isle would be {grn}fatal{/grn}.\r\n	\N	\N	1	0	\N
482	610	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Alcohol Isle 2	Both sides of the isle contain huge amounts of name brand alcoholic beverages. You notice that there aren't any beers. The only types of alcohol are high strength variants like vodka, rum, and whiskey. An {red}explosion{/red} in this isle would be {grn}fatal{/grn}.\r\n	\N	\N	1	0	\N
483	611	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Alcohol Isle 2	Both sides of the isle contain huge amounts of name brand alcoholic beverages. You notice that there aren't any beers. The only types of alcohol are high strength variants like vodka, rum, and whiskey. An {red}explosion{/red} in this isle would be {grn}fatal{/grn}.\r\n	\N	\N	1	0	allied-foods-alcohol-end2
484	612	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Dairy	The isle is dimly lit but that doesn't affect your vision of the contents of this isle. Every product here is in a white container. Milks, butters, cream cheese.. all sorts of kinds of almond milks.. At the end of the isle you notice an end cap displaying a high grade vokda. How appropriate.\r\n	\N	\N	1	0	\N
485	613	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Dairy	The isle is dimly lit but that doesn't affect your vision of the contents of this isle. Every product here is in a white container. Milks, butters, cream cheese.. all sorts of kinds of almond milks.. At the end of the isle you notice an end cap displaying a high grade vokda. How appropriate.\r\n	\N	\N	1	0	\N
486	614	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Dairy	The isle is dimly lit but that doesn't affect your vision of the contents of this isle. Every product here is in a white container. Milks, butters, cream cheese.. all sorts of kinds of almond milks.. At the end of the isle you notice an end cap displaying a high grade vokda. How appropriate.\r\n	\N	\N	1	0	allied-foods-dairy-end2
487	615	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Alcohol Isle 3	Both sides of the isle contain huge amounts of name brand alcoholic beverages. You notice that there aren't any beers. The only types of alcohol are high strength variants like vodka, rum, and whiskey. An {red}explosion{/red} in this isle would be {grn}fatal{/grn}.\r\n	\N	\N	1	0	\N
488	616	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Alcohol Isle 3	Both sides of the isle contain huge amounts of name brand alcoholic beverages. You notice that there aren't any beers. The only types of alcohol are high strength variants like vodka, rum, and whiskey. An {red}explosion{/red} in this isle would be {grn}fatal{/grn}.\r\n	\N	\N	1	0	\N
489	617	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Alcohol Isle 3	Both sides of the isle contain huge amounts of name brand alcoholic beverages. You notice that there aren't any beers. The only types of alcohol are high strength variants like vodka, rum, and whiskey. An {red}explosion{/red} in this isle would be {grn}fatal{/grn}.\r\n	\N	\N	1	0	\N
490	618	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Alcohol Isle 3	Both sides of the isle contain huge amounts of name brand alcoholic beverages. You notice that there aren't any beers. The only types of alcohol are high strength variants like vodka, rum, and whiskey. An {red}explosion{/red} in this isle would be {grn}fatal{/grn}.\r\n	\N	\N	1	0	\N
491	619	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Alcohol Isle 3	Both sides of the isle contain huge amounts of name brand alcoholic beverages. You notice that there aren't any beers. The only types of alcohol are high strength variants like vodka, rum, and whiskey. An {red}explosion{/red} in this isle would be {grn}fatal{/grn}.\r\n	\N	\N	1	0	\N
492	620	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Alcohol Isle 3	Both sides of the isle contain huge amounts of name brand alcoholic beverages. You notice that there aren't any beers. The only types of alcohol are high strength variants like vodka, rum, and whiskey. An {red}explosion{/red} in this isle would be {grn}fatal{/grn}.\r\n	\N	\N	1	0	\N
493	621	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Alcohol Isle 3	Both sides of the isle contain huge amounts of name brand alcoholic beverages. You notice that there aren't any beers. The only types of alcohol are high strength variants like vodka, rum, and whiskey. An {red}explosion{/red} in this isle would be {grn}fatal{/grn}.\r\n	\N	\N	1	0	\N
494	622	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Alcohol Isle 3	Both sides of the isle contain huge amounts of name brand alcoholic beverages. You notice that there aren't any beers. The only types of alcohol are high strength variants like vodka, rum, and whiskey. An {red}explosion{/red} in this isle would be {grn}fatal{/grn}.\r\n	\N	\N	1	0	allied-foods-alcohol-end3
495	623	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Dairy	The isle is dimly lit but that doesn't affect your vision of the contents of this isle. Every product here is in a white container. Milks, butters, cream cheese.. all sorts of kinds of almond milks.. At the end of the isle you notice an end cap displaying a high grade vokda. How appropriate.\r\n	\N	\N	1	0	\N
496	624	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Dairy	The isle is dimly lit but that doesn't affect your vision of the contents of this isle. Every product here is in a white container. Milks, butters, cream cheese.. all sorts of kinds of almond milks.. At the end of the isle you notice an end cap displaying a high grade vokda. How appropriate.\r\n	\N	\N	1	0	\N
497	625	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Dairy	The isle is dimly lit but that doesn't affect your vision of the contents of this isle. Every product here is in a white container. Milks, butters, cream cheese.. all sorts of kinds of almond milks.. At the end of the isle you notice an end cap displaying a high grade vokda. How appropriate.\r\n	\N	\N	1	0	allied-foods-dairy-end3
498	626	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Alcohol Isle 4	Both sides of the isle contain huge amounts of name brand alcoholic beverages. You notice that there aren't any beers. The only types of alcohol are high strength variants like vodka, rum, and whiskey. An {red}explosion{/red} in this isle would be {grn}fatal{/grn}.\r\n	\N	\N	1	0	\N
499	627	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Alcohol Isle 4	Both sides of the isle contain huge amounts of name brand alcoholic beverages. You notice that there aren't any beers. The only types of alcohol are high strength variants like vodka, rum, and whiskey. An {red}explosion{/red} in this isle would be {grn}fatal{/grn}.\r\n	\N	\N	1	0	\N
500	628	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Alcohol Isle 4	Both sides of the isle contain huge amounts of name brand alcoholic beverages. You notice that there aren't any beers. The only types of alcohol are high strength variants like vodka, rum, and whiskey. An {red}explosion{/red} in this isle would be {grn}fatal{/grn}.\r\n	\N	\N	1	0	\N
501	629	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Alcohol Isle 4	Both sides of the isle contain huge amounts of name brand alcoholic beverages. You notice that there aren't any beers. The only types of alcohol are high strength variants like vodka, rum, and whiskey. An {red}explosion{/red} in this isle would be {grn}fatal{/grn}.\r\n	\N	\N	1	0	\N
502	630	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Alcohol Isle 4	Both sides of the isle contain huge amounts of name brand alcoholic beverages. You notice that there aren't any beers. The only types of alcohol are high strength variants like vodka, rum, and whiskey. An {red}explosion{/red} in this isle would be {grn}fatal{/grn}.\r\n	\N	\N	1	0	\N
503	631	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Alcohol Isle 4	Both sides of the isle contain huge amounts of name brand alcoholic beverages. You notice that there aren't any beers. The only types of alcohol are high strength variants like vodka, rum, and whiskey. An {red}explosion{/red} in this isle would be {grn}fatal{/grn}.\r\n	\N	\N	1	0	\N
504	632	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Alcohol Isle 4	Both sides of the isle contain huge amounts of name brand alcoholic beverages. You notice that there aren't any beers. The only types of alcohol are high strength variants like vodka, rum, and whiskey. An {red}explosion{/red} in this isle would be {grn}fatal{/grn}.\r\n	\N	\N	1	0	\N
505	633	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Alcohol Isle 4	Both sides of the isle contain huge amounts of name brand alcoholic beverages. You notice that there aren't any beers. The only types of alcohol are high strength variants like vodka, rum, and whiskey. An {red}explosion{/red} in this isle would be {grn}fatal{/grn}.\r\n	\N	\N	1	0	\N
506	634	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Dairy	The isle is dimly lit but that doesn't affect your vision of the contents of this isle. Every product here is in a white container. Milks, butters, cream cheese.. all sorts of kinds of almond milks.. At the end of the isle you notice an end cap displaying a high grade vokda. How appropriate.\r\n	\N	\N	1	0	\N
507	635	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Dairy	The isle is dimly lit but that doesn't affect your vision of the contents of this isle. Every product here is in a white container. Milks, butters, cream cheese.. all sorts of kinds of almond milks.. At the end of the isle you notice an end cap displaying a high grade vokda. How appropriate.\r\n	\N	\N	1	0	\N
508	636	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Dairy	The isle is dimly lit but that doesn't affect your vision of the contents of this isle. Every product here is in a white container. Milks, butters, cream cheese.. all sorts of kinds of almond milks.. At the end of the isle you notice an end cap displaying a high grade vokda. How appropriate.\r\n	\N	\N	1	0	allied-foods-dairy-end4
509	637	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Slaughter Isle	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	\N
510	638	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Slaughter Isle	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	\N
511	639	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Slaughter Isle	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	\N
512	640	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Slaughter Isle	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	\N
513	641	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Slaughter Isle	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	\N
514	642	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Slaughter Isle	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	\N
515	643	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Slaughter Isle	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	\N
516	644	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Slaughter Isle	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	\N
517	645	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Slaughter Isle	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	\N
518	646	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Slaughter Isle	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	\N
519	647	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Slaughter Isle	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	\N
520	648	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Slaughter Isle	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	\N
521	649	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Slaughter Isle	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	\N
522	650	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Slaughter Isle	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	\N
523	651	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Slaughter Isle	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	\N
524	652	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Slaughter Isle	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	\N
525	653	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Slaughter Isle	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	defiler-isle
526	654	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Butcher	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	\N
527	655	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Butcher	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	\N
528	656	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Butcher	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	\N
529	657	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Butcher	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	defiler-corner-1
530	658	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Butcher	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	\N
531	659	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Butcher	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	\N
532	660	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Butcher	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	\N
533	661	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Butcher	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	defiler-east-1
534	662	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Butcher	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	\N
535	663	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Butcher	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	\N
536	664	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Butcher	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	defiler-corner-2
537	665	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Butcher	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	\N
538	666	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Butcher	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	\N
539	667	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Butcher	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	defiler-east-tie
540	668	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Butcher	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	\N
541	669	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Butcher	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	\N
542	670	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Butcher	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	\N
543	671	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Butcher	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	defiler-corner-3
544	672	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Butcher	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	\N
545	673	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Butcher	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	\N
546	674	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Butcher	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	\N
547	675	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Butcher	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	defiler-west-1
548	676	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Butcher	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	\N
549	677	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Butcher	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	\N
550	678	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Butcher	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	defiler-corner-4
551	679	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Butcher	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	\N
552	680	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Butcher	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	\N
553	681	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Butcher	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	defiler-corner-5
554	682	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Butcher	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	defiler-spawn
63	192	1	0	Waypoint North	@FILL_ME@\r\n	\N	\N	1	0	waypoint-car-garage-north-1
556	685	1	0	{blu}Metro{/blu} Bornald Road	The road to the heart of Metro City is a bustling hive of busy upper city residents. While the majority of the people who work in Downtown Metro are mid to upper class citizens, a large disproportionate chunk of the people who gather here are drugged out degenerates. Several homeless families make camp right in the middle of the sidewalk: their life for all to judge. \r\n	\N	\N	1	0	\N
557	686	1	0	{blu}Metro{/blu} Henley Fountain	A tribute to the Henley Corporation, an elaborately crafted fountain was erected in the early stages of development of Downtwon Metro City. The water is cleaned several times before it's recycled and spat back out. The fountain has a rectangular cement wall that keeps the water inside. There are elaborately chiseled patterns at the bottom of the pool and on the inner walls.\r\n	\N	\N	1	0	metrofountain-starting-point
558	687	1	0	{blu}Metro{/blu} Henley Fountain	A tribute to the Henley Corporation, an elaborately crafted fountain was erected in the early stages of development of Downtwon Metro City. The water is cleaned several times before it's recycled and spat back out. The fountain has a rectangular cement wall that keeps the water inside. There are elaborately chiseled patterns at the bottom of the pool and on the inner walls.\r\n	\N	\N	1	0	metrofountain-3x3
559	688	1	0	{blu}Metro{/blu} Henley Fountain	A tribute to the Henley Corporation, an elaborately crafted fountain was erected in the early stages of development of Downtwon Metro City. The water is cleaned several times before it's recycled and spat back out. The fountain has a rectangular cement wall that keeps the water inside. There are elaborately chiseled patterns at the bottom of the pool and on the inner walls.\r\n	\N	\N	1	0	metrofountain-3x4
560	689	1	0	{blu}Metro{/blu} Henley Fountain	A tribute to the Henley Corporation, an elaborately crafted fountain was erected in the early stages of development of Downtwon Metro City. The water is cleaned several times before it's recycled and spat back out. The fountain has a rectangular cement wall that keeps the water inside. There are elaborately chiseled patterns at the bottom of the pool and on the inner walls.\r\n	\N	\N	1	0	metrofountain-2x4
561	690	1	0	{blu}Metro{/blu} Henley Fountain	A tribute to the Henley Corporation, an elaborately crafted fountain was erected in the early stages of development of Downtwon Metro City. The water is cleaned several times before it's recycled and spat back out. The fountain has a rectangular cement wall that keeps the water inside. There are elaborately chiseled patterns at the bottom of the pool and on the inner walls.\r\n	\N	\N	1	0	metrofountain-1x4
563	692	1	0	{blu}Metro{/blu} Henley Fountain	A tribute to the Henley Corporation, an elaborately crafted fountain was erected in the early stages of development of Downtwon Metro City. The water is cleaned several times before it's recycled and spat back out. The fountain has a rectangular cement wall that keeps the water inside. There are elaborately chiseled patterns at the bottom of the pool and on the inner walls.\r\n	\N	\N	1	0	metrofountain-1x2
564	693	1	0	{blu}Metro{/blu} Henley Fountain	A tribute to the Henley Corporation, an elaborately crafted fountain was erected in the early stages of development of Downtwon Metro City. The water is cleaned several times before it's recycled and spat back out. The fountain has a rectangular cement wall that keeps the water inside. There are elaborately chiseled patterns at the bottom of the pool and on the inner walls.\r\n	\N	\N	1	0	metrofountain-1x1
565	694	1	0	{blu}Metro{/blu} Henley Fountain	A tribute to the Henley Corporation, an elaborately crafted fountain was erected in the early stages of development of Downtwon Metro City. The water is cleaned several times before it's recycled and spat back out. The fountain has a rectangular cement wall that keeps the water inside. There are elaborately chiseled patterns at the bottom of the pool and on the inner walls.\r\n	\N	\N	1	0	metrofountain-1x0
567	696	1	0	{blu}Metro{/blu} Inside the Henley Fountain	You are up to your knees in water. All of your equipment is starting to soak. That soggy feeling is creeping into your clothes and you feel less agile.\r\n	\N	\N	1	0	metrofountain-2x1
568	697	1	0	{blu}Metro{/blu} Inside the Henley Fountain	You are up to your knees in water. All of your equipment is starting to soak. That soggy feeling is creeping into your clothes and you feel less agile.\r\n	\N	\N	1	0	metrofountain-2x2
569	698	1	0	{blu}Metro{/blu} Inside the Henley Fountain	You are up to your knees in water. All of your equipment is starting to soak. That soggy feeling is creeping into your clothes and you feel less agile.\r\n	\N	\N	1	0	metrofountain-2x3
570	699	1	0	{blu}Metro{/blu} Henley Fountain	A tribute to the Henley Corporation, an elaborately crafted fountain was erected in the early stages of development of Downtwon Metro City. The water is cleaned several times before it's recycled and spat back out. The fountain has a rectangular cement wall that keeps the water inside. There are elaborately chiseled patterns at the bottom of the pool and on the inner walls.\r\n	\N	\N	1	0	metrofountain-3x1
571	700	1	0	{blu}Metro{/blu} Henley Fountain	A tribute to the Henley Corporation, an elaborately crafted fountain was erected in the early stages of development of Downtwon Metro City. The water is cleaned several times before it's recycled and spat back out. The fountain has a rectangular cement wall that keeps the water inside. There are elaborately chiseled patterns at the bottom of the pool and on the inner walls.\r\n	\N	\N	1	0	metrofountain-3x0
572	701	1	0	name	description	\N	\N	1	0	\N
573	702	1	0	{blu}Metro{/blu} Haven Street sidewalk	Along the paved sidewalks of Haven street are all the hallmarks of a busy metropolis. Except from the fact that the homeless denizens outnumber the working class citizens. The sidewalk paints a grim picture of the grip that drugs have on those who are poverty-striken. A testament to their suffering in the form of man made tents and makeshift sleeping bags.\r\n	\N	\N	1	0	\N
574	703	1	0	{blu}Metro{/blu} Haven Street sidewalk	Along the paved sidewalks of Haven street are all the hallmarks of a busy metropolis. Except from the fact that the homeless denizens outnumber the working class citizens. The sidewalk paints a grim picture of the grip that drugs have on those who are poverty-striken. A testament to their suffering in the form of man made tents and makeshift sleeping bags.\r\n	\N	\N	1	0	\N
575	704	1	0	{blu}Metro{/blu} Haven Street sidewalk	Along the paved sidewalks of Haven street are all the hallmarks of a busy metropolis. Except from the fact that the homeless denizens outnumber the working class citizens. The sidewalk paints a grim picture of the grip that drugs have on those who are poverty-striken. A testament to their suffering in the form of man made tents and makeshift sleeping bags.\r\n	\N	\N	1	0	\N
576	705	1	0	{blu}Metro{/blu} Haven Street sidewalk	Along the paved sidewalks of Haven street are all the hallmarks of a busy metropolis. Except from the fact that the homeless denizens outnumber the working class citizens. The sidewalk paints a grim picture of the grip that drugs have on those who are poverty-striken. A testament to their suffering in the form of man made tents and makeshift sleeping bags.\r\n	\N	\N	1	0	bank-west-alley-start
577	706	1	0	{blu}Metro{/blu} Haven Street sidewalk	Along the paved sidewalks of Haven street are all the hallmarks of a busy metropolis. Except from the fact that the homeless denizens outnumber the working class citizens. The sidewalk paints a grim picture of the grip that drugs have on those who are poverty-striken. A testament to their suffering in the form of man made tents and makeshift sleeping bags.\r\n	\N	\N	1	0	\N
578	707	1	0	{blu}Metro{/blu} Hartford Bank entrance	A delightful awning above your head provides sanctuary from the blazing heat of the sun. Two automatic doors to the north welcome you to Hartford bank.\r\n	\N	\N	1	0	\N
579	708	1	0	{blu}Metro{/blu} Hartford Bank entrance	A delightful awning above your head provides sanctuary from the blazing heat of the sun. Two automatic doors to the north welcome you to Hartford bank.\r\n	\N	\N	1	0	\N
580	709	1	0	{blu}Hartford Bank{/blu} - Atrium	Hartford bank, like most of the businesses in downtown metro, is owned and operated by the Hexos Cartel. The lavishly decorated atrium is a testament to the massive amounts of income that is pumped through thousands of fake accounts in a sophisticated money laundering mechanism.\r\n	\N	\N	1	0	\N
581	710	1	0	{blu}Hartford Bank{/blu} - Atrium	Hartford bank, like most of the businesses in downtown metro, is owned and operated by the Hexos Cartel. The lavishly decorated atrium is a testament to the massive amounts of income that is pumped through thousands of fake accounts in a sophisticated money laundering mechanism.\r\n	\N	\N	1	0	\N
555	684	1	0	{blu}Metro{/blu} Bornald Road	The road to the heart of Metro City is a bustling hive of busy upper city residents. While the majority of the people who work in Downtown Metro are mid to upper class citizens, a large disproportionate chunk of the people who gather here are drugged out degenerates. Several homeless families make camp right in the middle of the sidewalk: their life for all to judge. \r\n	\N	\N	1	0	\N
583	712	1	0	{blu}Hartford Bank{/blu} - Atrium	Hartford bank, like most of the businesses in downtown metro, is owned and operated by the Hexos Cartel. The lavishly decorated atrium is a testament to the massive amounts of income that is pumped through thousands of fake accounts in a sophisticated money laundering mechanism.\r\n	\N	\N	1	0	\N
584	713	1	0	{blu}Hartford Bank{/blu} - Atrium	Hartford bank, like most of the businesses in downtown metro, is owned and operated by the Hexos Cartel. The lavishly decorated atrium is a testament to the massive amounts of income that is pumped through thousands of fake accounts in a sophisticated money laundering mechanism.\r\n	\N	\N	1	0	\N
585	714	1	0	{blu}Hartford Bank{/blu} - Atrium	Hartford bank, like most of the businesses in downtown metro, is owned and operated by the Hexos Cartel. The lavishly decorated atrium is a testament to the massive amounts of income that is pumped through thousands of fake accounts in a sophisticated money laundering mechanism.\r\n	\N	\N	1	0	\N
586	715	1	0	{blu}Hartford Bank{/blu} - Atrium	Hartford bank, like most of the businesses in downtown metro, is owned and operated by the Hexos Cartel. The lavishly decorated atrium is a testament to the massive amounts of income that is pumped through thousands of fake accounts in a sophisticated money laundering mechanism.\r\n	\N	\N	1	0	\N
587	716	1	0	{blu}Hartford Bank{/blu} - Atrium	Hartford bank, like most of the businesses in downtown metro, is owned and operated by the Hexos Cartel. The lavishly decorated atrium is a testament to the massive amounts of income that is pumped through thousands of fake accounts in a sophisticated money laundering mechanism.\r\n	\N	\N	1	0	hartford-atrium-west
588	717	1	0	{blu}Hartford Bank{/blu} - Atrium	Hartford bank, like most of the businesses in downtown metro, is owned and operated by the Hexos Cartel. The lavishly decorated atrium is a testament to the massive amounts of income that is pumped through thousands of fake accounts in a sophisticated money laundering mechanism.\r\n	\N	\N	1	0	\N
589	718	1	0	{blu}Hartford Bank{/blu} - Atrium	Hartford bank, like most of the businesses in downtown metro, is owned and operated by the Hexos Cartel. The lavishly decorated atrium is a testament to the massive amounts of income that is pumped through thousands of fake accounts in a sophisticated money laundering mechanism.\r\n	\N	\N	1	0	hartford-atrium-center
590	719	1	0	{blu}Hartford Bank{/blu} - Atrium	Hartford bank, like most of the businesses in downtown metro, is owned and operated by the Hexos Cartel. The lavishly decorated atrium is a testament to the massive amounts of income that is pumped through thousands of fake accounts in a sophisticated money laundering mechanism.\r\n	\N	\N	1	0	\N
591	720	1	0	{blu}Hartford Bank{/blu} - Atrium	Hartford bank, like most of the businesses in downtown metro, is owned and operated by the Hexos Cartel. The lavishly decorated atrium is a testament to the massive amounts of income that is pumped through thousands of fake accounts in a sophisticated money laundering mechanism.\r\n	\N	\N	1	0	hartford-atrium-east
592	721	1	0	{blu}Hartford Bank{/blu} - Atrium	Hartford bank, like most of the businesses in downtown metro, is owned and operated by the Hexos Cartel. The lavishly decorated atrium is a testament to the massive amounts of income that is pumped through thousands of fake accounts in a sophisticated money laundering mechanism.\r\n	\N	\N	1	0	\N
593	722	1	0	{blu}Hartford Bank{/blu} - Atrium	Hartford bank, like most of the businesses in downtown metro, is owned and operated by the Hexos Cartel. The lavishly decorated atrium is a testament to the massive amounts of income that is pumped through thousands of fake accounts in a sophisticated money laundering mechanism.\r\n	\N	\N	1	0	\N
594	723	1	0	{blu}Hartford Bank{/blu} - Tellers	To the east and west are bank tellers at their respective kiosks. The marble floor has a prestine aura to it. The distant promise of coffee hangs in the air.\r\n	\N	\N	1	0	\N
595	724	1	0	{blu}Hartford Bank{/blu} - Tellers	To the east and west are bank tellers at their respective kiosks. The marble floor has a prestine aura to it. The distant promise of coffee hangs in the air.\r\n	\N	\N	1	0	\N
596	725	1	0	{blu}Hartford Bank{/blu} - Tellers	To the east and west are bank tellers at their respective kiosks. The marble floor has a prestine aura to it. The distant promise of coffee hangs in the air.\r\n	\N	\N	1	0	\N
597	726	1	0	{blu}Hartford Bank{/blu} - Tellers	To the east and west are bank tellers at their respective kiosks. The marble floor has a prestine aura to it. The distant promise of coffee hangs in the air.\r\n	\N	\N	1	0	\N
598	727	1	0	{blu}Hartford Bank{/blu} - Tellers	To the east and west are bank tellers at their respective kiosks. The marble floor has a prestine aura to it. The distant promise of coffee hangs in the air.\r\n	\N	\N	1	0	\N
599	728	1	0	{blu}Hartford Bank{/blu} - Tellers	To the east and west are bank tellers at their respective kiosks. The marble floor has a prestine aura to it. The distant promise of coffee hangs in the air.\r\n	\N	\N	1	0	\N
600	729	1	0	{blu}Hartford Bank{/blu} - Teller A	You stand at a teller kiosk. There is a atm card scanner on the desk in front of you On the opposite side of the desk is a large monitor that is angled away from you. \r\n	\N	\N	1	0	hartford-teller-a
601	730	1	0	{blu}Hartford Bank{/blu} - Teller B	You stand at a teller kiosk. There is a atm card scanner on the desk in front of you On the opposite side of the desk is a large monitor that is angled away from you. \r\n	\N	\N	1	0	hartford-teller-b
602	731	1	0	{blu}Hartford Bank{/blu} - Teller C	You stand at a teller kiosk. There is a atm card scanner on the desk in front of you On the opposite side of the desk is a large monitor that is angled away from you. \r\n	\N	\N	1	0	hartford-teller-c
603	732	1	0	{blu}Hartford Bank{/blu} - Teller D	You stand at a teller kiosk. There is a atm card scanner on the desk in front of you On the opposite side of the desk is a large monitor that is angled away from you. \r\n	\N	\N	1	0	hartford-teller-d
562	691	1	0	{blu}Metro{/blu} Henley Fountain	A tribute to the Henley Corporation, an elaborately crafted fountain was erected in the early stages of development of Downtwon Metro City. The water is cleaned several times before it's recycled and spat back out. The fountain has a rectangular cement wall that keeps the water inside. There are elaborately chiseled patterns at the bottom of the pool and on the inner walls.\r\n	\N	\N	1	0	metrofountain-1x3
582	711	1	0	{blu}Hartford Bank{/blu} - Atrium	Hartford bank, like most of the businesses in downtown metro, is owned and operated by the Hexos Cartel. The lavishly decorated atrium is a testament to the massive amounts of income that is pumped through thousands of fake accounts in a sophisticated money laundering mechanism.\r\n	\N	\N	1	0	\N
604	733	1	0	{blu}Hartford Bank West Alley{/blu}	If you hadn't known there was a bank just to the east of here, you'd think you were in the worst time of dystopian nightmare. The ground is littered with used needles, broken glass, and human feces. The very thought of sleeping here makes you cringe, yet you see that many homeless people call this home by evidence of the makeshift shelters made of cardboard and newspaper fragments.\r\n	\N	\N	1	0	\N
605	734	1	0	{blu}Hartford Bank West Alley{/blu}	If you hadn't known there was a bank just to the east of here, you'd think you were in the worst time of dystopian nightmare. The ground is littered with used needles, broken glass, and human feces. The very thought of sleeping here makes you cringe, yet you see that many homeless people call this home by evidence of the makeshift shelters made of cardboard and newspaper fragments.\r\n	\N	\N	1	0	\N
606	735	1	0	{blu}Hartford Bank West Alley{/blu}	If you hadn't known there was a bank just to the east of here, you'd think you were in the worst time of dystopian nightmare. The ground is littered with used needles, broken glass, and human feces. The very thought of sleeping here makes you cringe, yet you see that many homeless people call this home by evidence of the makeshift shelters made of cardboard and newspaper fragments.\r\n	\N	\N	1	0	\N
607	736	1	0	{blu}Hartford Bank West Alley{/blu}	If you hadn't known there was a bank just to the east of here, you'd think you were in the worst time of dystopian nightmare. The ground is littered with used needles, broken glass, and human feces. The very thought of sleeping here makes you cringe, yet you see that many homeless people call this home by evidence of the makeshift shelters made of cardboard and newspaper fragments.\r\n	\N	\N	1	0	\N
608	737	1	0	{blu}Hartford Bank West Alley{/blu}	If you hadn't known there was a bank just to the east of here, you'd think you were in the worst time of dystopian nightmare. The ground is littered with used needles, broken glass, and human feces. The very thought of sleeping here makes you cringe, yet you see that many homeless people call this home by evidence of the makeshift shelters made of cardboard and newspaper fragments.\r\n	\N	\N	1	0	\N
609	738	1	0	{blu}Hartford Bank West Alley{/blu}	If you hadn't known there was a bank just to the east of here, you'd think you were in the worst time of dystopian nightmare. The ground is littered with used needles, broken glass, and human feces. The very thought of sleeping here makes you cringe, yet you see that many homeless people call this home by evidence of the makeshift shelters made of cardboard and newspaper fragments.\r\n	\N	\N	1	0	\N
610	739	1	0	{blu}Hartford Bank West Alley{/blu}	If you hadn't known there was a bank just to the east of here, you'd think you were in the worst time of dystopian nightmare. The ground is littered with used needles, broken glass, and human feces. The very thought of sleeping here makes you cringe, yet you see that many homeless people call this home by evidence of the makeshift shelters made of cardboard and newspaper fragments.\r\n	\N	\N	1	0	\N
611	740	1	0	{blu}Hartford Bank West Alley{/blu}	If you hadn't known there was a bank just to the east of here, you'd think you were in the worst time of dystopian nightmare. The ground is littered with used needles, broken glass, and human feces. The very thought of sleeping here makes you cringe, yet you see that many homeless people call this home by evidence of the makeshift shelters made of cardboard and newspaper fragments.\r\n	\N	\N	1	0	\N
612	741	1	0	{blu}Hartford Bank West Alley{/blu}	If you hadn't known there was a bank just to the east of here, you'd think you were in the worst time of dystopian nightmare. The ground is littered with used needles, broken glass, and human feces. The very thought of sleeping here makes you cringe, yet you see that many homeless people call this home by evidence of the makeshift shelters made of cardboard and newspaper fragments.\r\n	\N	\N	1	0	\N
613	742	1	0	{blu}Hartford Bank West Alley{/blu}	If you hadn't known there was a bank just to the east of here, you'd think you were in the worst time of dystopian nightmare. The ground is littered with used needles, broken glass, and human feces. The very thought of sleeping here makes you cringe, yet you see that many homeless people call this home by evidence of the makeshift shelters made of cardboard and newspaper fragments.\r\n	\N	\N	1	0	\N
614	743	1	0	{blu}Hartford Bank West Alley{/blu}	If you hadn't known there was a bank just to the east of here, you'd think you were in the worst time of dystopian nightmare. The ground is littered with used needles, broken glass, and human feces. The very thought of sleeping here makes you cringe, yet you see that many homeless people call this home by evidence of the makeshift shelters made of cardboard and newspaper fragments.\r\n	\N	\N	1	0	\N
616	745	1	0	{blu}Hartford Bank Vault{/blu}	You find yourself in a heavily fortified bank vault. A scene that few people have the privilege of witnessing. Heavy bags of money line the walls. Giant stacks of money are organized into a neat pile on several tables in the middle of the room.\r\n	\N	\N	1	0	\N
617	746	1	0	{blu}Hartford Bank Vault{/blu}	You find yourself in a heavily fortified bank vault. A scene that few people have the privilege of witnessing. Heavy bags of money line the walls. Giant stacks of money are organized into a neat pile on several tables in the middle of the room.\r\n	\N	\N	1	0	\N
618	748	1	0	name	description	\N	\N	1	0	\N
619	749	1	0	name	description	\N	\N	1	0	\N
620	750	1	0	{blu}Market Street{/blu} - Thompson Avenue	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	thompson-north-1
615	744	1	0	{blu}Hartford Bank Vault{/blu}	You find yourself in a heavily fortified bank vault. A scene that few people have the privilege of witnessing. Heavy bags of money line the walls. Giant stacks of money are organized into a neat pile on several tables in the middle of the room.\r\n	\N	\N	1	0	hartford-bank-west-valley-hidden-wall
621	751	1	0	{blu}Market Street{/blu} - Thompson Avenue	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	\N
623	753	1	0	{blu}Market Street{/blu} - Thompson Avenue	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	\N
624	754	1	0	{blu}Market Street{/blu} - Thompson Avenue	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	thompson-north-2
644	774	1	0	{blu}Market Street{/blu} - Trolley Station	Thanks to hundreds of thousands of dollars in federal and state funding, the Trolley station on Market Street is a reliable source of dependable transportation. The lower income areas tend to be the most reliant on this form of transportation. With that, you get all sorts of different individuals. \r\n	\N	\N	1	0	trolley-section-2
625	755	1	0	{blu}Market Street{/blu} - Thompson Avenue	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	\N
626	756	1	0	{blu}Market Street{/blu} - Thompson Avenue	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	\N
628	758	1	0	{blu}Market Street{/blu} - Thompson Avenue	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	\N
629	759	1	0	{blu}Market Street{/blu} - Thompson Avenue	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	\N
630	760	1	0	{blu}Market Street{/blu} - Thompson Avenue	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	\N
631	761	1	0	{blu}Market Street{/blu} - Thompson Avenue	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	thompson-north-4
632	762	1	0	{blu}Market Street{/blu} - Thompson Avenue	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	\N
633	763	1	0	{blu}Market Street{/blu} - Thompson Avenue	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	\N
634	764	1	0	{blu}Market Street{/blu} - Thompson Avenue	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	thompson-north-5
635	765	1	0	{blu}Market Street{/blu} - Trolley Station	Thanks to hundreds of thousands of dollars in federal and state funding, the Trolley station on Market Street is a reliable source of dependable transportation. The lower income areas tend to be the most reliant on this form of transportation. With that, you get all sorts of different individuals. \r\n	\N	\N	1	0	\N
636	766	1	0	{blu}Market Street{/blu} - Trolley Station	Thanks to hundreds of thousands of dollars in federal and state funding, the Trolley station on Market Street is a reliable source of dependable transportation. The lower income areas tend to be the most reliant on this form of transportation. With that, you get all sorts of different individuals. \r\n	\N	\N	1	0	\N
637	767	1	0	{blu}Market Street{/blu} - Trolley Station	Thanks to hundreds of thousands of dollars in federal and state funding, the Trolley station on Market Street is a reliable source of dependable transportation. The lower income areas tend to be the most reliant on this form of transportation. With that, you get all sorts of different individuals. \r\n	\N	\N	1	0	\N
639	769	1	0	{blu}Market Street{/blu} - Trolley Station	Thanks to hundreds of thousands of dollars in federal and state funding, the Trolley station on Market Street is a reliable source of dependable transportation. The lower income areas tend to be the most reliant on this form of transportation. With that, you get all sorts of different individuals. \r\n	\N	\N	1	0	trolley-section-1
640	770	1	0	{blu}Market Street{/blu} - Trolley Station	Thanks to hundreds of thousands of dollars in federal and state funding, the Trolley station on Market Street is a reliable source of dependable transportation. The lower income areas tend to be the most reliant on this form of transportation. With that, you get all sorts of different individuals. \r\n	\N	\N	1	0	\N
641	771	1	0	{blu}Market Street{/blu} - Trolley Station	Thanks to hundreds of thousands of dollars in federal and state funding, the Trolley station on Market Street is a reliable source of dependable transportation. The lower income areas tend to be the most reliant on this form of transportation. With that, you get all sorts of different individuals. \r\n	\N	\N	1	0	\N
642	772	1	0	{blu}Market Street{/blu} - Trolley Station	Thanks to hundreds of thousands of dollars in federal and state funding, the Trolley station on Market Street is a reliable source of dependable transportation. The lower income areas tend to be the most reliant on this form of transportation. With that, you get all sorts of different individuals. \r\n	\N	\N	1	0	\N
643	773	1	0	{blu}Market Street{/blu} - Trolley Station	Thanks to hundreds of thousands of dollars in federal and state funding, the Trolley station on Market Street is a reliable source of dependable transportation. The lower income areas tend to be the most reliant on this form of transportation. With that, you get all sorts of different individuals. \r\n	\N	\N	1	0	\N
645	775	1	0	{blu}Market Street{/blu} - Trolley Station	Thanks to hundreds of thousands of dollars in federal and state funding, the Trolley station on Market Street is a reliable source of dependable transportation. The lower income areas tend to be the most reliant on this form of transportation. With that, you get all sorts of different individuals. \r\n	\N	\N	1	0	\N
646	776	1	0	{blu}Market Street{/blu} - Trolley Station	Thanks to hundreds of thousands of dollars in federal and state funding, the Trolley station on Market Street is a reliable source of dependable transportation. The lower income areas tend to be the most reliant on this form of transportation. With that, you get all sorts of different individuals. \r\n	\N	\N	1	0	\N
647	777	1	0	{blu}Market Street{/blu} - Trolley Station	Thanks to hundreds of thousands of dollars in federal and state funding, the Trolley station on Market Street is a reliable source of dependable transportation. The lower income areas tend to be the most reliant on this form of transportation. With that, you get all sorts of different individuals. \r\n	\N	\N	1	0	\N
648	778	1	0	{blu}Market Street{/blu} - Trolley Station	Thanks to hundreds of thousands of dollars in federal and state funding, the Trolley station on Market Street is a reliable source of dependable transportation. The lower income areas tend to be the most reliant on this form of transportation. With that, you get all sorts of different individuals. \r\n	\N	\N	1	0	\N
649	779	1	0	{blu}Market Street{/blu} - Trolley Station	Thanks to hundreds of thousands of dollars in federal and state funding, the Trolley station on Market Street is a reliable source of dependable transportation. The lower income areas tend to be the most reliant on this form of transportation. With that, you get all sorts of different individuals. \r\n	\N	\N	1	0	trolley-section-3
650	780	1	0	{blu}Market Street{/blu} - Trolley Station	Thanks to hundreds of thousands of dollars in federal and state funding, the Trolley station on Market Street is a reliable source of dependable transportation. The lower income areas tend to be the most reliant on this form of transportation. With that, you get all sorts of different individuals. \r\n	\N	\N	1	0	\N
651	781	1	0	{blu}Market Street{/blu} - Trolley Station	Thanks to hundreds of thousands of dollars in federal and state funding, the Trolley station on Market Street is a reliable source of dependable transportation. The lower income areas tend to be the most reliant on this form of transportation. With that, you get all sorts of different individuals. \r\n	\N	\N	1	0	\N
652	782	1	0	{blu}Market Street{/blu} - Trolley Station	Thanks to hundreds of thousands of dollars in federal and state funding, the Trolley station on Market Street is a reliable source of dependable transportation. The lower income areas tend to be the most reliant on this form of transportation. With that, you get all sorts of different individuals. \r\n	\N	\N	1	0	\N
653	783	1	0	{blu}Market Street{/blu} - Trolley Station	Thanks to hundreds of thousands of dollars in federal and state funding, the Trolley station on Market Street is a reliable source of dependable transportation. The lower income areas tend to be the most reliant on this form of transportation. With that, you get all sorts of different individuals. \r\n	\N	\N	1	0	\N
566	695	1	0	{blu}Metro{/blu} Henley Fountain	A tribute to the Henley Corporation, an elaborately crafted fountain was erected in the early stages of development of Downtwon Metro City. The water is cleaned several times before it's recycled and spat back out. The fountain has a rectangular cement wall that keeps the water inside. There are elaborately chiseled patterns at the bottom of the pool and on the inner walls.\r\n	\N	\N	1	0	metrofountain-2x0
622	752	1	0	{blu}Market Street{/blu} - Thompson Avenue	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	\N
627	757	1	0	{blu}Market Street{/blu} - Thompson Avenue	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	thompson-north-3
638	768	1	0	{blu}Market Street{/blu} - Trolley Station	Thanks to hundreds of thousands of dollars in federal and state funding, the Trolley station on Market Street is a reliable source of dependable transportation. The lower income areas tend to be the most reliant on this form of transportation. With that, you get all sorts of different individuals. \r\n	\N	\N	1	0	\N
654	784	1	0	{blu}Market Street{/blu} - Trolley Station	Thanks to hundreds of thousands of dollars in federal and state funding, the Trolley station on Market Street is a reliable source of dependable transportation. The lower income areas tend to be the most reliant on this form of transportation. With that, you get all sorts of different individuals. \r\n	\N	\N	1	0	trolley-section-4
655	785	1	0	{blu}Market Street{/blu} - Thompson Avenue - EAST	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	thompson-east-1
656	786	1	0	{blu}Market Street{/blu} - Thompson Avenue - EAST	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	\N
657	787	1	0	{blu}Market Street{/blu} - Thompson Avenue - EAST	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	\N
658	788	1	0	{blu}Market Street{/blu} - Thompson Avenue - EAST	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	thompson-east-2
659	789	1	0	{blu}Market Street{/blu} - Thompson Avenue - EAST	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	\N
660	790	1	0	{blu}Market Street{/blu} - Thompson Avenue - EAST	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	\N
661	791	1	0	{blu}Market Street{/blu} - Thompson Avenue - EAST	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	thompson-east-3
662	792	1	0	{blu}Market Street{/blu} - Thompson Avenue - EAST	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	\N
663	793	1	0	{blu}Market Street{/blu} - Thompson Avenue - EAST	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	\N
664	794	1	0	{blu}Market Street{/blu} - Thompson Avenue - EAST	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	thompson-east-4
665	795	1	0	{blu}Market Street{/blu} - Thompson Avenue - EAST	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	\N
666	796	1	0	{blu}Market Street{/blu} - Thompson Avenue - EAST	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	\N
667	797	1	0	{blu}Market Street{/blu} - Thompson Avenue - EAST	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	thompson-east-5
668	798	1	0	{blu}Market Street{/blu} - Thompson Avenue - EAST	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	\N
669	799	1	0	{blu}Market Street{/blu} - Thompson Avenue - EAST	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	\N
670	800	1	0	{blu}Market Street{/blu} - Thompson Avenue - EAST	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	thompson-east-6
671	801	1	0	{blu}Market Street{/blu} - Thompson Avenue - EAST	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	\N
672	802	1	0	{blu}Market Street{/blu} - Thompson Avenue - EAST	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	\N
673	803	1	0	{blu}Market Street{/blu} - Thompson Avenue - EAST	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	thompson-east-7
674	804	1	0	{blu}Market Street{/blu} - Thompson Avenue - EAST	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	\N
675	805	1	0	{blu}Market Street{/blu} - Thompson Avenue - EAST	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	\N
676	806	1	0	{blu}Market Street{/blu} - Thompson Avenue - EAST	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	thompson-east-8
677	807	1	0	{blu}Market Street{/blu} - Thompson Avenue - EAST	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	\N
678	808	1	0	{blu}Market Street{/blu} - Thompson Avenue - EAST	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	\N
679	809	1	0	{blu}Market Street{/blu} - Thompson Avenue - EAST	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	thompson-east-9
680	810	1	0	{blu}Market Street{/blu} - Thompson Avenue - EAST	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	\N
681	811	1	0	{blu}Market Street{/blu} - Thompson Avenue - EAST	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	\N
682	812	1	0	{blu}Market Street{/blu} - Thompson Avenue - EAST	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	thompson-east-10
683	813	1	0	{blu}Market Street{/blu} - Thompson Avenue - EAST	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	\N
684	814	1	0	{blu}Market Street{/blu} - Thompson Avenue - EAST	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	\N
685	815	1	0	{blu}Market Street{/blu} - Thompson Avenue - EAST	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	thompson-east-11
\.


--
-- Data for Name: room_direction_data; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.room_direction_data (id, room_number, exit_direction, general_description, keyword, exit_info, exit_key, to_room) FROM stdin;
576	128	0	general_description	keyword	1	0	130
577	130	0	general_description	keyword	1	0	131
578	130	2	general description	keyword	1	0	128
579	131	1	general_description	keyword	1	0	138
580	131	2	general description	keyword	1	0	130
581	131	3	general description	keyword	1	0	231
582	132	1	general description	keyword	1	0	131
583	132	3	general_description	keyword	1	0	230
584	133	1	general description	keyword	1	0	132
585	133	3	general_description	keyword	1	0	229
586	134	1	general description	keyword	1	0	133
587	134	3	general_description	keyword	1	0	228
588	135	1	general description	keyword	1	0	134
589	135	3	general_description	keyword	1	0	227
590	136	1	general description	keyword	1	0	135
591	136	3	general_description	keyword	1	0	226
592	137	0	general_description	keyword	1	0	225
593	137	1	general description	keyword	1	0	136
594	138	1	general_description	keyword	1	0	139
595	138	3	general description	keyword	1	0	131
596	139	0	general_description	keyword	1	0	140
597	139	3	general description	keyword	1	0	138
598	140	0	general_description	keyword	1	0	141
599	140	2	general description	keyword	1	0	139
600	141	0	general_description	keyword	1	0	193
601	141	1	general_description	keyword	1	0	142
602	141	2	general description	keyword	1	0	140
603	142	1	general_description	keyword	1	0	143
604	142	3	general description	keyword	1	0	141
605	143	0	general_description	keyword	1	0	144
606	143	1	general_description	keyword	1	0	146
607	143	2	general_description	keyword	1	0	145
608	143	3	general description	keyword	1	0	142
609	144	2	general description	keyword	1	0	143
610	145	0	general description	keyword	1	0	143
611	146	1	general_description	keyword	1	0	147
612	146	3	general description	keyword	1	0	143
613	147	3	general description	keyword	1	0	146
614	147	5	general_description	keyword	1	0	148
615	148	1	general_description	keyword	1	0	149
616	148	4	general description	keyword	1	0	147
617	150	3	general description	keyword	1	0	149
618	150	5	general_description	keyword	1	0	151
619	151	1	general_description	keyword	1	0	152
620	151	4	general description	keyword	1	0	150
621	152	1	general_description	keyword	1	0	153
622	152	3	general description	keyword	1	0	151
623	153	3	general description	keyword	1	0	152
624	153	5	general_description	keyword	1	0	154
625	154	1	general_description	keyword	1	0	155
626	154	4	general description	keyword	1	0	153
627	155	1	general_description	keyword	1	0	156
628	155	3	general description	keyword	1	0	154
629	156	2	general_description	keyword	1	0	157
630	156	3	general description	keyword	1	0	155
631	157	0	general description	keyword	1	0	156
632	157	2	general_description	keyword	1	0	158
633	158	0	general description	keyword	1	0	157
634	158	2	general_description	keyword	1	0	159
635	159	0	general description	keyword	1	0	158
636	159	2	general_description	keyword	1	0	160
637	160	0	general description	keyword	1	0	159
638	160	2	general_description	keyword	1	0	161
639	161	0	general description	keyword	1	0	160
640	161	3	general_description	keyword	1	0	162
641	162	1	general description	keyword	1	0	161
642	162	3	general_description	keyword	1	0	163
643	163	1	general description	keyword	1	0	162
644	163	3	general_description	keyword	1	0	164
645	164	1	general description	keyword	1	0	163
646	164	3	general_description	keyword	1	0	165
647	165	1	general description	keyword	1	0	164
648	165	3	general_description	keyword	1	0	166
649	166	0	general_description	keyword	1	0	167
650	166	1	general description	keyword	1	0	165
651	166	2	general_description	keyword	1	0	174
652	167	0	general_description	keyword	1	0	168
653	167	2	general description	keyword	1	0	166
654	168	0	general_description	keyword	1	0	169
655	168	2	general description	keyword	1	0	167
656	169	0	general_description	keyword	1	0	170
657	169	1	general_description	keyword	1	0	172
658	169	2	general description	keyword	1	0	168
659	170	0	general_description	keyword	1	0	171
660	170	2	general description	keyword	1	0	169
661	171	2	general description	keyword	1	0	170
662	172	1	general_description	keyword	1	0	173
663	172	3	general description	keyword	1	0	169
664	173	3	general description	keyword	1	0	172
665	174	0	general description	keyword	1	0	166
666	174	2	general_description	keyword	1	0	175
667	175	0	general description	keyword	1	0	174
668	175	3	general_description	keyword	1	0	176
669	176	1	general description	keyword	1	0	175
670	176	3	general_description	keyword	1	0	177
671	177	1	general description	keyword	1	0	176
672	177	2	general_description	keyword	1	0	178
673	178	0	general description	keyword	1	0	177
674	178	2	general_description	keyword	1	0	179
675	179	0	general description	keyword	1	0	178
676	179	1	general_description	keyword	1	0	182
677	179	2	general_description	keyword	1	0	184
678	179	3	general_description	keyword	1	0	180
679	180	1	general description	keyword	1	0	179
680	180	3	general_description	keyword	1	0	181
681	181	1	general description	keyword	1	0	180
682	182	1	general_description	keyword	1	0	183
683	182	3	general description	keyword	1	0	179
684	183	3	general description	keyword	1	0	182
685	184	0	general description	keyword	1	0	179
686	184	2	general_description	keyword	1	0	185
687	185	0	general description	keyword	1	0	184
688	185	1	general_description	keyword	1	0	188
689	185	2	general_description	keyword	1	0	190
690	185	3	general_description	keyword	1	0	186
691	186	1	general description	keyword	1	0	185
692	186	3	general_description	keyword	1	0	187
693	187	1	general description	keyword	1	0	186
694	188	1	general_description	keyword	1	0	189
695	188	3	general description	keyword	1	0	185
696	189	3	general description	keyword	1	0	188
697	190	0	general description	keyword	1	0	185
698	190	2	general_description	keyword	1	0	191
699	191	0	general description	keyword	1	0	190
700	191	3	general_description	keyword	1	0	192
701	192	1	general description	keyword	1	0	191
702	193	0	general_description	keyword	1	0	194
703	193	2	general description	keyword	1	0	141
704	194	0	general_description	keyword	1	0	195
705	194	2	general description	keyword	1	0	193
706	195	0	general_description	keyword	1	0	196
707	195	2	general description	keyword	1	0	194
708	196	0	general_description	keyword	1	0	197
709	196	2	general description	keyword	1	0	195
710	197	0	general_description	keyword	1	0	198
711	197	2	general description	keyword	1	0	196
712	198	0	general_description	keyword	1	0	199
713	198	2	general description	keyword	1	0	197
714	199	0	general_description	keyword	1	0	200
715	199	1	general_description	keyword	1	0	274
716	199	2	general description	keyword	1	0	198
717	200	0	general_description	keyword	1	0	201
718	200	2	general description	keyword	1	0	199
719	201	0	general_description	keyword	1	0	202
720	201	2	general description	keyword	1	0	200
721	202	0	general_description	keyword	1	0	203
722	202	2	general description	keyword	1	0	201
723	202	3	general_description	keyword	1	0	340
724	203	0	general_description	keyword	1	0	204
725	203	1	general_description	keyword	1	0	265
726	203	2	general description	keyword	1	0	202
727	204	2	general description	keyword	1	0	203
728	204	3	general_description	keyword	1	0	205
729	205	1	general description	keyword	1	0	204
730	205	3	general_description	keyword	1	0	206
731	206	1	general description	keyword	1	0	205
732	206	3	general_description	keyword	1	0	207
733	207	0	general_description	keyword	1	0	232
734	207	1	general description	keyword	1	0	206
735	207	3	general_description	keyword	1	0	208
736	208	1	general description	keyword	1	0	207
737	208	3	general_description	keyword	1	0	209
738	209	1	general description	keyword	1	0	208
739	209	3	general_description	keyword	1	0	210
740	210	1	general description	keyword	1	0	209
741	210	3	general_description	keyword	1	0	211
742	211	1	general description	keyword	1	0	210
743	211	3	general_description	keyword	1	0	212
744	212	1	general description	keyword	1	0	211
745	212	2	general_description	keyword	1	0	213
746	213	0	general description	keyword	1	0	212
747	213	2	general_description	keyword	1	0	214
748	214	0	general description	keyword	1	0	213
749	214	2	general_description	keyword	1	0	215
750	215	0	general description	keyword	1	0	214
751	215	2	general_description	keyword	1	0	216
752	216	0	general description	keyword	1	0	215
753	216	2	general_description	keyword	1	0	217
754	217	0	general description	keyword	1	0	216
755	217	2	general_description	keyword	1	0	218
756	218	0	general description	keyword	1	0	217
757	218	2	general_description	keyword	1	0	219
758	219	0	general description	keyword	1	0	218
759	219	2	general_description	keyword	1	0	220
760	220	0	general description	keyword	1	0	219
761	220	2	general_description	keyword	1	0	221
762	221	0	general description	keyword	1	0	220
763	221	2	general_description	keyword	1	0	222
764	222	0	general description	keyword	1	0	221
765	222	2	general_description	keyword	1	0	223
766	223	0	general description	keyword	1	0	222
767	223	2	general_description	keyword	1	0	224
768	224	0	general description	keyword	1	0	223
769	224	2	general_description	keyword	1	0	225
770	224	3	general_description	keyword	1	0	338
771	225	0	general description	keyword	1	0	224
772	225	2	general_description	keyword	1	0	226
773	226	0	general description	keyword	1	0	225
774	226	1	general_description	keyword	1	0	227
775	227	1	general_description	keyword	1	0	228
776	227	3	general description	keyword	1	0	226
777	228	1	general_description	keyword	1	0	229
778	228	3	general description	keyword	1	0	227
779	229	1	general_description	keyword	1	0	230
780	229	3	general description	keyword	1	0	228
781	230	1	general_description	keyword	1	0	231
782	230	3	general description	keyword	1	0	229
783	231	1	general description	keyword	1	0	131
784	231	3	general description	keyword	1	0	230
785	232	2	general description	keyword	1	0	207
786	232	4	general_description	keyword	1	0	233
787	233	0	general_description	keyword	1	0	234
788	233	5	general description	keyword	1	0	232
789	234	2	general description	keyword	1	0	233
790	234	4	general_description	keyword	1	0	235
791	235	0	general_description	keyword	1	0	236
792	235	5	general description	keyword	1	0	234
793	236	0	general_description	keyword	1	0	237
794	236	2	general description	keyword	1	0	235
795	237	0	general_description	keyword	1	0	243
796	237	1	general_description	keyword	1	0	253
797	237	2	general description	keyword	1	0	236
798	237	3	general_description	keyword	1	0	238
799	238	1	general description	keyword	1	0	237
800	238	3	general_description	keyword	1	0	239
801	239	1	general description	keyword	1	0	238
802	239	3	general_description	keyword	1	0	240
803	240	1	general description	keyword	1	0	239
804	240	3	general_description	keyword	1	0	241
805	241	1	general description	keyword	1	0	240
806	241	3	general_description	keyword	1	0	242
807	242	1	general description	keyword	1	0	241
808	243	0	general_description	keyword	1	0	244
809	243	2	general description	keyword	1	0	237
810	244	1	general_description	keyword	1	0	245
811	244	2	general description	keyword	1	0	243
812	244	3	general_description	keyword	1	0	249
813	245	0	general_description	keyword	1	0	246
814	245	3	general description	keyword	1	0	244
815	246	0	general_description	keyword	1	0	247
816	246	2	general description	keyword	1	0	245
817	247	0	general_description	keyword	1	0	248
818	247	2	general description	keyword	1	0	246
819	248	2	general description	keyword	1	0	247
820	249	0	general_description	keyword	1	0	250
821	249	1	general description	keyword	1	0	244
822	250	0	general_description	keyword	1	0	251
823	250	2	general description	keyword	1	0	249
824	251	0	general_description	keyword	1	0	252
825	251	2	general description	keyword	1	0	250
826	252	2	general description	keyword	1	0	251
827	253	1	general_description	keyword	1	0	254
828	253	3	general description	keyword	1	0	237
829	254	1	general_description	keyword	1	0	255
830	254	3	general description	keyword	1	0	253
831	255	1	general_description	keyword	1	0	256
832	255	3	general description	keyword	1	0	254
833	256	0	general_description	keyword	1	0	258
834	256	1	general_description	keyword	1	0	257
835	256	3	general description	keyword	1	0	255
836	257	3	general description	keyword	1	0	256
837	258	0	general_description	keyword	1	0	259
838	258	2	general description	keyword	1	0	256
839	259	0	general_description	keyword	1	0	260
840	259	1	general_description	keyword	1	0	263
841	259	2	general description	keyword	1	0	258
842	260	0	general_description	keyword	1	0	261
843	260	2	general description	keyword	1	0	259
844	261	0	general_description	keyword	1	0	262
845	261	2	general description	keyword	1	0	260
846	262	1	general_description	keyword	1	0	264
847	262	2	general description	keyword	1	0	261
848	263	3	general description	keyword	1	0	259
849	264	3	general description	keyword	1	0	262
850	265	1	general_description	keyword	1	0	266
851	265	3	general description	keyword	1	0	203
852	266	0	general_description	keyword	1	0	267
853	266	3	general description	keyword	1	0	265
854	267	0	general_description	keyword	1	0	268
855	267	2	general description	keyword	1	0	266
856	268	0	general_description	keyword	1	0	269
857	268	1	general_description	keyword	1	0	271
858	268	2	general description	keyword	1	0	267
859	268	3	general_description	keyword	1	0	270
860	268	4	general_description	keyword	1	0	272
861	268	5	general_description	keyword	1	0	273
862	269	2	general description	keyword	1	0	268
863	270	1	general description	keyword	1	0	268
864	271	3	general description	keyword	1	0	268
865	272	5	general description	keyword	1	0	268
866	273	4	general description	keyword	1	0	268
867	274	1	general_description	keyword	1	0	275
868	274	3	general description	keyword	1	0	199
869	275	1	general_description	keyword	1	0	276
870	275	3	general description	keyword	1	0	274
871	276	1	general_description	keyword	1	0	277
872	276	3	general description	keyword	1	0	275
873	277	1	general_description	keyword	1	0	278
874	277	3	general description	keyword	1	0	276
875	278	1	general_description	keyword	1	0	279
876	278	3	general description	keyword	1	0	277
877	279	0	general_description	keyword	1	0	280
878	279	3	general description	keyword	1	0	278
879	280	0	general_description	keyword	1	0	281
880	280	2	general description	keyword	1	0	279
881	281	0	general_description	keyword	1	0	282
882	281	2	general description	keyword	1	0	280
883	282	0	general_description	keyword	1	0	283
884	282	2	general description	keyword	1	0	281
885	283	0	general_description	keyword	1	0	284
886	283	2	general description	keyword	1	0	282
887	284	0	general_description	keyword	1	0	285
888	284	2	general description	keyword	1	0	283
889	285	0	general_description	keyword	1	0	286
890	285	2	general description	keyword	1	0	284
891	286	0	general_description	keyword	1	0	287
892	286	2	general description	keyword	1	0	285
893	287	0	general_description	keyword	1	0	288
894	287	2	general description	keyword	1	0	286
895	288	0	general_description	keyword	1	0	289
896	288	2	general description	keyword	1	0	287
897	289	0	general_description	keyword	1	0	290
898	289	2	general description	keyword	1	0	288
899	290	0	general_description	keyword	1	0	333
900	290	2	general description	keyword	1	0	289
901	290	3	general_description	keyword	1	0	291
902	291	1	general description	keyword	1	0	290
903	291	3	general_description	keyword	1	0	292
904	292	0	general_description	keyword	1	0	293
905	292	1	general description	keyword	1	0	291
906	293	0	general_description	keyword	1	0	294
907	293	2	general description	keyword	1	0	292
908	294	2	general description	keyword	1	0	293
909	294	3	general_description	keyword	1	0	295
910	295	0	general_description	keyword	1	0	296
911	295	1	general description	keyword	1	0	294
912	295	3	general_description	keyword	1	0	329
913	296	0	general_description	keyword	1	0	297
914	296	2	general description	keyword	1	0	295
915	297	2	general description	keyword	1	0	296
916	297	4	general_description	keyword	1	0	298
917	298	0	general_description	keyword	1	0	299
918	298	5	general description	keyword	1	0	297
919	299	2	general description	keyword	1	0	298
920	299	4	general_description	keyword	1	0	300
921	300	0	general_description	keyword	1	0	301
922	300	5	general description	keyword	1	0	299
923	301	1	general_description	keyword	1	0	316
924	301	2	general description	keyword	1	0	300
925	301	3	general_description	keyword	1	0	302
926	302	1	general description	keyword	1	0	301
927	302	3	general_description	keyword	1	0	303
928	303	1	general description	keyword	1	0	302
929	303	3	general_description	keyword	1	0	304
930	304	0	general_description	keyword	1	0	305
931	304	1	general description	keyword	1	0	303
932	304	3	general_description	keyword	1	0	309
933	305	0	general_description	keyword	1	0	306
934	305	2	general description	keyword	1	0	304
935	306	0	general_description	keyword	1	0	307
936	306	1	general_description	keyword	1	0	308
937	306	2	general description	keyword	1	0	305
938	307	2	general description	keyword	1	0	306
939	308	3	general description	keyword	1	0	306
940	309	1	general description	keyword	1	0	304
941	309	3	general_description	keyword	1	0	310
942	310	1	general description	keyword	1	0	309
943	310	3	general_description	keyword	1	0	311
944	311	0	general_description	keyword	1	0	312
945	311	1	general description	keyword	1	0	310
946	312	0	general_description	keyword	1	0	313
947	312	2	general description	keyword	1	0	311
948	313	0	general_description	keyword	1	0	314
949	313	1	general_description	keyword	1	0	315
950	313	2	general description	keyword	1	0	312
951	314	2	general description	keyword	1	0	313
952	315	3	general description	keyword	1	0	313
953	316	1	general_description	keyword	1	0	317
954	316	3	general description	keyword	1	0	301
955	317	0	general_description	keyword	1	0	318
956	317	1	general_description	keyword	1	0	322
957	317	3	general description	keyword	1	0	316
958	318	0	general_description	keyword	1	0	319
959	318	2	general description	keyword	1	0	317
960	319	0	general_description	keyword	1	0	320
961	319	1	general_description	keyword	1	0	321
962	319	2	general description	keyword	1	0	318
963	320	2	general description	keyword	1	0	319
964	321	3	general description	keyword	1	0	319
965	322	1	general_description	keyword	1	0	323
966	322	3	general description	keyword	1	0	317
967	323	1	general_description	keyword	1	0	324
968	323	3	general description	keyword	1	0	322
969	324	0	general_description	keyword	1	0	325
970	324	3	general description	keyword	1	0	323
971	325	0	general_description	keyword	1	0	326
972	325	2	general description	keyword	1	0	324
973	326	0	general_description	keyword	1	0	327
974	326	1	general_description	keyword	1	0	328
975	326	2	general description	keyword	1	0	325
976	327	2	general description	keyword	1	0	326
977	328	3	general description	keyword	1	0	326
978	329	1	general description	keyword	1	0	295
979	329	3	general_description	keyword	1	0	330
980	330	1	general description	keyword	1	0	329
981	330	3	general_description	keyword	1	0	331
982	331	0	general_description	keyword	1	0	332
983	331	1	general description	keyword	1	0	330
984	332	2	general description	keyword	1	0	331
985	333	0	general_description	keyword	1	0	334
986	333	2	general description	keyword	1	0	290
987	334	0	general_description	keyword	1	0	335
988	334	2	general description	keyword	1	0	333
989	335	0	general_description	keyword	1	0	336
990	335	2	general description	keyword	1	0	334
991	336	0	general_description	keyword	1	0	337
992	336	2	general description	keyword	1	0	335
993	337	2	general description	keyword	1	0	336
994	338	1	general description	keyword	1	0	224
995	338	5	general_description	keyword	1	0	339
996	339	4	general description	keyword	1	0	338
997	340	1	general description	keyword	1	0	202
998	340	5	general_description	keyword	1	0	341
999	341	3	general_description	keyword	1	0	342
1000	341	4	general description	keyword	1	0	340
1001	342	1	general description	keyword	1	0	341
1002	342	3	general_description	keyword	1	0	343
1003	343	1	general description	keyword	1	0	342
1004	343	5	general_description	keyword	1	0	344
1005	344	3	general_description	keyword	1	0	345
1006	344	4	general description	keyword	1	0	343
1007	345	1	general description	keyword	1	0	344
1008	345	3	general_description	keyword	1	0	346
1009	346	1	general description	keyword	1	0	345
1010	346	2	general_description	keyword	1	0	347
1011	347	0	general description	keyword	1	0	346
1012	347	2	general_description	keyword	1	0	348
1013	348	0	general description	keyword	1	0	347
1014	348	1	general_description	keyword	1	0	349
1015	349	1	general_description	keyword	1	0	350
1016	349	3	general description	keyword	1	0	348
1017	350	1	general_description	keyword	1	0	353
1018	350	2	general_description	keyword	1	0	351
1019	350	3	general description	keyword	1	0	349
1020	351	0	general description	keyword	1	0	350
1021	351	2	general_description	keyword	1	0	352
1022	352	0	general description	keyword	1	0	351
1023	353	1	general_description	keyword	1	0	354
1024	353	3	general description	keyword	1	0	350
1025	354	1	general_description	keyword	1	0	357
1026	354	2	general_description	keyword	1	0	355
1027	354	3	general description	keyword	1	0	353
1028	355	0	general description	keyword	1	0	354
1029	355	2	general_description	keyword	1	0	356
1030	356	0	general description	keyword	1	0	355
1031	357	1	general_description	keyword	1	0	358
1032	357	3	general description	keyword	1	0	354
1033	358	2	general_description	keyword	1	0	359
1034	358	3	general description	keyword	1	0	357
1035	359	0	general description	keyword	1	0	358
1036	359	2	general_description	keyword	1	0	360
1037	360	0	general description	keyword	1	0	359
1038	128	0	general_description	keyword	1	0	130
1039	130	0	general_description	keyword	1	0	131
1040	130	2	general description	keyword	1	0	128
1041	131	1	general_description	keyword	1	0	138
1042	131	2	general description	keyword	1	0	130
1043	131	3	general description	keyword	1	0	231
1044	132	1	general description	keyword	1	0	131
1045	132	3	general_description	keyword	1	0	230
1046	133	1	general description	keyword	1	0	132
1047	133	3	general_description	keyword	1	0	229
1048	134	1	general description	keyword	1	0	133
1049	134	3	general_description	keyword	1	0	228
1050	135	1	general description	keyword	1	0	134
1051	135	3	general_description	keyword	1	0	227
1052	136	1	general description	keyword	1	0	135
1053	136	3	general_description	keyword	1	0	226
1054	137	0	general_description	keyword	1	0	225
1055	137	1	general description	keyword	1	0	136
1056	138	1	general_description	keyword	1	0	139
1057	138	3	general description	keyword	1	0	131
1058	139	0	general_description	keyword	1	0	140
1059	139	3	general description	keyword	1	0	138
1060	140	0	general_description	keyword	1	0	141
1061	140	2	general description	keyword	1	0	139
1062	141	0	general_description	keyword	1	0	193
1063	141	1	general_description	keyword	1	0	142
1064	141	2	general description	keyword	1	0	140
1065	142	1	general_description	keyword	1	0	143
1066	142	3	general description	keyword	1	0	141
1067	143	0	general_description	keyword	1	0	144
1068	143	1	general_description	keyword	1	0	146
1069	143	2	general_description	keyword	1	0	145
1070	143	3	general description	keyword	1	0	142
1071	144	2	general description	keyword	1	0	143
1072	145	0	general description	keyword	1	0	143
1073	146	1	general_description	keyword	1	0	147
1074	146	3	general description	keyword	1	0	143
1075	147	3	general description	keyword	1	0	146
1076	147	5	general_description	keyword	1	0	148
1077	148	1	general_description	keyword	1	0	149
1078	148	4	general description	keyword	1	0	147
1079	149	1	general_description	keyword	1	0	150
1080	149	3	general description	keyword	1	0	148
1081	150	3	general description	keyword	1	0	149
1082	150	5	general_description	keyword	1	0	151
1083	151	1	general_description	keyword	1	0	152
1084	151	4	general description	keyword	1	0	150
1085	152	1	general_description	keyword	1	0	153
1086	152	3	general description	keyword	1	0	151
1087	153	3	general description	keyword	1	0	152
1088	153	5	general_description	keyword	1	0	154
1089	154	1	general_description	keyword	1	0	155
1090	154	4	general description	keyword	1	0	153
1091	155	1	general_description	keyword	1	0	156
1092	155	3	general description	keyword	1	0	154
1093	156	2	general_description	keyword	1	0	157
1094	156	3	general description	keyword	1	0	155
1095	157	0	general description	keyword	1	0	156
1096	157	2	general_description	keyword	1	0	158
1097	158	0	general description	keyword	1	0	157
1098	158	2	general_description	keyword	1	0	159
1099	159	0	general description	keyword	1	0	158
1100	159	2	general_description	keyword	1	0	160
1101	160	0	general description	keyword	1	0	159
1102	160	2	general_description	keyword	1	0	161
1103	161	0	general description	keyword	1	0	160
1104	161	3	general_description	keyword	1	0	162
1105	162	1	general description	keyword	1	0	161
1106	162	3	general_description	keyword	1	0	163
1107	163	1	general description	keyword	1	0	162
1108	163	3	general_description	keyword	1	0	164
1109	164	1	general description	keyword	1	0	163
1110	164	3	general_description	keyword	1	0	165
1111	165	1	general description	keyword	1	0	164
1112	165	3	general_description	keyword	1	0	166
1113	166	0	general_description	keyword	1	0	167
1114	166	1	general description	keyword	1	0	165
1115	166	2	general_description	keyword	1	0	174
1116	167	0	general_description	keyword	1	0	168
1117	167	2	general description	keyword	1	0	166
1118	168	0	general_description	keyword	1	0	169
1119	168	2	general description	keyword	1	0	167
1120	169	0	general_description	keyword	1	0	170
1121	169	1	general_description	keyword	1	0	172
1122	169	2	general description	keyword	1	0	168
1123	170	0	general_description	keyword	1	0	171
1124	170	2	general description	keyword	1	0	169
1125	171	2	general description	keyword	1	0	170
1126	172	1	general_description	keyword	1	0	173
1127	172	3	general description	keyword	1	0	169
1128	173	3	general description	keyword	1	0	172
1129	174	0	general description	keyword	1	0	166
1130	174	2	general_description	keyword	1	0	175
1131	175	0	general description	keyword	1	0	174
1132	175	3	general_description	keyword	1	0	176
1133	176	1	general description	keyword	1	0	175
1134	176	3	general_description	keyword	1	0	177
1135	177	1	general description	keyword	1	0	176
1136	177	2	general_description	keyword	1	0	178
1137	178	0	general description	keyword	1	0	177
1138	178	2	general_description	keyword	1	0	179
1139	179	0	general description	keyword	1	0	178
1140	179	1	general_description	keyword	1	0	182
1141	179	2	general_description	keyword	1	0	184
1142	179	3	general_description	keyword	1	0	180
1143	180	1	general description	keyword	1	0	179
1144	180	3	general_description	keyword	1	0	181
1145	181	1	general description	keyword	1	0	180
1146	182	1	general_description	keyword	1	0	183
1147	182	3	general description	keyword	1	0	179
1148	183	3	general description	keyword	1	0	182
1149	184	0	general description	keyword	1	0	179
1150	184	2	general_description	keyword	1	0	185
1151	185	0	general description	keyword	1	0	184
1152	185	1	general_description	keyword	1	0	188
1153	185	2	general_description	keyword	1	0	190
1154	185	3	general_description	keyword	1	0	186
1155	186	1	general description	keyword	1	0	185
1156	186	3	general_description	keyword	1	0	187
1157	187	1	general description	keyword	1	0	186
1158	188	1	general_description	keyword	1	0	189
1159	188	3	general description	keyword	1	0	185
1160	189	3	general description	keyword	1	0	188
1161	190	0	general description	keyword	1	0	185
1162	190	2	general_description	keyword	1	0	191
1163	191	0	general description	keyword	1	0	190
1164	191	3	general_description	keyword	1	0	192
1165	192	1	general description	keyword	1	0	191
1166	193	0	general_description	keyword	1	0	194
1167	193	2	general description	keyword	1	0	141
1168	194	0	general_description	keyword	1	0	195
1169	194	2	general description	keyword	1	0	193
1170	195	0	general_description	keyword	1	0	196
1171	195	2	general description	keyword	1	0	194
1172	196	0	general_description	keyword	1	0	197
1173	196	2	general description	keyword	1	0	195
1174	197	0	general_description	keyword	1	0	198
1175	197	2	general description	keyword	1	0	196
1176	198	0	general_description	keyword	1	0	199
1177	198	2	general description	keyword	1	0	197
1178	199	0	general_description	keyword	1	0	200
1179	199	1	general_description	keyword	1	0	274
1180	199	2	general description	keyword	1	0	198
1181	200	0	general_description	keyword	1	0	201
1182	200	2	general description	keyword	1	0	199
1183	201	0	general_description	keyword	1	0	202
1184	201	2	general description	keyword	1	0	200
1185	202	0	general_description	keyword	1	0	203
1186	202	2	general description	keyword	1	0	201
1187	202	3	general_description	keyword	1	0	340
1188	203	0	general_description	keyword	1	0	204
1189	203	1	general_description	keyword	1	0	265
1190	203	2	general description	keyword	1	0	202
1191	204	2	general description	keyword	1	0	203
1192	204	3	general_description	keyword	1	0	205
1193	205	1	general description	keyword	1	0	204
1194	205	3	general_description	keyword	1	0	206
1195	206	1	general description	keyword	1	0	205
1196	206	3	general_description	keyword	1	0	207
1197	207	0	general_description	keyword	1	0	232
1198	207	1	general description	keyword	1	0	206
1199	207	3	general_description	keyword	1	0	208
1200	208	1	general description	keyword	1	0	207
1201	208	3	general_description	keyword	1	0	209
1202	209	1	general description	keyword	1	0	208
1203	209	3	general_description	keyword	1	0	210
1204	210	1	general description	keyword	1	0	209
1205	210	3	general_description	keyword	1	0	211
1206	211	1	general description	keyword	1	0	210
1207	211	3	general_description	keyword	1	0	212
1208	212	1	general description	keyword	1	0	211
1209	212	2	general_description	keyword	1	0	213
1210	213	0	general description	keyword	1	0	212
1211	213	2	general_description	keyword	1	0	214
1212	214	0	general description	keyword	1	0	213
1213	214	2	general_description	keyword	1	0	215
1214	215	0	general description	keyword	1	0	214
1215	215	2	general_description	keyword	1	0	216
1216	216	0	general description	keyword	1	0	215
1217	216	2	general_description	keyword	1	0	217
1218	217	0	general description	keyword	1	0	216
1219	217	2	general_description	keyword	1	0	218
1220	218	0	general description	keyword	1	0	217
1221	218	2	general_description	keyword	1	0	219
1222	219	0	general description	keyword	1	0	218
1223	219	2	general_description	keyword	1	0	220
1224	220	0	general description	keyword	1	0	219
1225	220	2	general_description	keyword	1	0	221
1226	221	0	general description	keyword	1	0	220
1227	221	2	general_description	keyword	1	0	222
1228	222	0	general description	keyword	1	0	221
1229	222	2	general_description	keyword	1	0	223
1230	223	0	general description	keyword	1	0	222
1231	223	2	general_description	keyword	1	0	224
1232	224	0	general description	keyword	1	0	223
1233	224	2	general_description	keyword	1	0	225
1234	224	3	general_description	keyword	1	0	338
1235	225	0	general description	keyword	1	0	224
1236	225	2	general_description	keyword	1	0	226
1237	226	0	general description	keyword	1	0	225
1238	226	1	general_description	keyword	1	0	227
1239	227	1	general_description	keyword	1	0	228
1240	227	3	general description	keyword	1	0	226
1241	228	1	general_description	keyword	1	0	229
1242	228	3	general description	keyword	1	0	227
1243	229	1	general_description	keyword	1	0	230
1244	229	3	general description	keyword	1	0	228
1245	230	1	general_description	keyword	1	0	231
1246	230	3	general description	keyword	1	0	229
1247	231	1	general description	keyword	1	0	131
1248	231	3	general description	keyword	1	0	230
1249	232	2	general description	keyword	1	0	207
1250	232	4	general_description	keyword	1	0	233
1251	233	0	general_description	keyword	1	0	234
1252	233	5	general description	keyword	1	0	232
1253	234	2	general description	keyword	1	0	233
1254	234	4	general_description	keyword	1	0	235
1255	235	0	general_description	keyword	1	0	236
1256	235	5	general description	keyword	1	0	234
1257	236	0	general_description	keyword	1	0	237
1258	236	2	general description	keyword	1	0	235
1259	237	0	general_description	keyword	1	0	243
1260	237	1	general_description	keyword	1	0	253
1261	237	2	general description	keyword	1	0	236
1262	237	3	general_description	keyword	1	0	238
1263	238	1	general description	keyword	1	0	237
1264	238	3	general_description	keyword	1	0	239
1265	239	1	general description	keyword	1	0	238
1266	239	3	general_description	keyword	1	0	240
1267	240	1	general description	keyword	1	0	239
1268	240	3	general_description	keyword	1	0	241
1269	241	1	general description	keyword	1	0	240
1270	241	3	general_description	keyword	1	0	242
1271	242	1	general description	keyword	1	0	241
1272	243	0	general_description	keyword	1	0	244
1273	243	2	general description	keyword	1	0	237
1274	244	1	general_description	keyword	1	0	245
1275	244	2	general description	keyword	1	0	243
1276	244	3	general_description	keyword	1	0	249
1277	245	0	general_description	keyword	1	0	246
1278	245	3	general description	keyword	1	0	244
1279	246	0	general_description	keyword	1	0	247
1280	246	2	general description	keyword	1	0	245
1281	247	0	general_description	keyword	1	0	248
1282	247	2	general description	keyword	1	0	246
1283	248	2	general description	keyword	1	0	247
1284	249	0	general_description	keyword	1	0	250
1285	249	1	general description	keyword	1	0	244
1286	250	0	general_description	keyword	1	0	251
1287	250	2	general description	keyword	1	0	249
1288	251	0	general_description	keyword	1	0	252
1289	251	2	general description	keyword	1	0	250
1290	252	2	general description	keyword	1	0	251
1291	253	1	general_description	keyword	1	0	254
1292	253	3	general description	keyword	1	0	237
1293	254	1	general_description	keyword	1	0	255
1294	254	3	general description	keyword	1	0	253
1295	255	1	general_description	keyword	1	0	256
1296	255	3	general description	keyword	1	0	254
1297	256	0	general_description	keyword	1	0	258
1298	256	1	general_description	keyword	1	0	257
1299	256	3	general description	keyword	1	0	255
1300	257	3	general description	keyword	1	0	256
1301	258	0	general_description	keyword	1	0	259
1302	258	2	general description	keyword	1	0	256
1303	259	0	general_description	keyword	1	0	260
1304	259	1	general_description	keyword	1	0	263
1305	259	2	general description	keyword	1	0	258
1306	260	0	general_description	keyword	1	0	261
1307	260	2	general description	keyword	1	0	259
1308	261	0	general_description	keyword	1	0	262
1309	261	2	general description	keyword	1	0	260
1310	262	1	general_description	keyword	1	0	264
1311	262	2	general description	keyword	1	0	261
1312	263	3	general description	keyword	1	0	259
1313	264	3	general description	keyword	1	0	262
1314	265	1	general_description	keyword	1	0	266
1315	265	3	general description	keyword	1	0	203
1316	266	0	general_description	keyword	1	0	267
1317	266	3	general description	keyword	1	0	265
1318	267	0	general_description	keyword	1	0	268
1319	267	2	general description	keyword	1	0	266
1320	268	0	general_description	keyword	1	0	269
1321	268	1	general_description	keyword	1	0	271
1322	268	2	general description	keyword	1	0	267
1323	268	3	general_description	keyword	1	0	270
1324	268	4	general_description	keyword	1	0	272
1325	268	5	general_description	keyword	1	0	273
1326	269	2	general description	keyword	1	0	268
1327	270	1	general description	keyword	1	0	268
1328	271	3	general description	keyword	1	0	268
1329	272	5	general description	keyword	1	0	268
1330	273	4	general description	keyword	1	0	268
1331	274	1	general_description	keyword	1	0	275
1332	274	3	general description	keyword	1	0	199
1333	275	1	general_description	keyword	1	0	276
1334	275	3	general description	keyword	1	0	274
1335	276	1	general_description	keyword	1	0	277
1336	276	3	general description	keyword	1	0	275
1337	277	1	general_description	keyword	1	0	278
1338	277	3	general description	keyword	1	0	276
1339	278	1	general_description	keyword	1	0	279
1340	278	3	general description	keyword	1	0	277
1341	279	0	general_description	keyword	1	0	280
1342	279	3	general description	keyword	1	0	278
1343	280	0	general_description	keyword	1	0	281
1344	280	2	general description	keyword	1	0	279
1345	281	0	general_description	keyword	1	0	282
1346	281	2	general description	keyword	1	0	280
1347	282	0	general_description	keyword	1	0	283
1348	282	2	general description	keyword	1	0	281
1349	283	0	general_description	keyword	1	0	284
1350	283	2	general description	keyword	1	0	282
1351	284	0	general_description	keyword	1	0	285
1352	284	2	general description	keyword	1	0	283
1353	285	0	general_description	keyword	1	0	286
1354	285	2	general description	keyword	1	0	284
1355	286	0	general_description	keyword	1	0	287
1356	286	2	general description	keyword	1	0	285
1357	287	0	general_description	keyword	1	0	288
1358	287	2	general description	keyword	1	0	286
1359	288	0	general_description	keyword	1	0	289
1360	288	2	general description	keyword	1	0	287
1361	289	0	general_description	keyword	1	0	290
1362	289	1	general_description	keyword	3137	0	574
1363	289	2	general description	keyword	1	0	288
1364	290	0	general_description	keyword	1	0	333
1365	290	2	general description	keyword	1	0	289
1366	290	3	general_description	keyword	1	0	291
1367	291	1	general description	keyword	1	0	290
1368	291	3	general_description	keyword	1	0	292
1369	292	0	general_description	keyword	1	0	293
1370	292	1	general description	keyword	1	0	291
1371	293	0	general_description	keyword	1	0	294
1372	293	2	general description	keyword	1	0	292
1373	294	2	general description	keyword	1	0	293
1374	294	3	general_description	keyword	1	0	295
1375	295	0	general_description	keyword	1	0	296
1376	295	1	general description	keyword	1	0	294
1377	295	3	general_description	keyword	1	0	329
1378	296	0	general_description	keyword	1	0	297
1379	296	2	general description	keyword	1	0	295
1380	297	2	general description	keyword	1	0	296
1381	297	4	general_description	keyword	1	0	298
1382	298	0	general_description	keyword	1	0	299
1383	298	5	general description	keyword	1	0	297
1384	299	2	general description	keyword	1	0	298
1385	299	4	general_description	keyword	1	0	300
1386	300	0	general_description	keyword	1	0	301
1387	300	5	general description	keyword	1	0	299
1388	301	1	general_description	keyword	1	0	316
1389	301	2	general description	keyword	1	0	300
1390	301	3	general_description	keyword	1	0	302
1391	302	1	general description	keyword	1	0	301
1392	302	3	general_description	keyword	1	0	303
1393	303	1	general description	keyword	1	0	302
1394	303	3	general_description	keyword	1	0	304
1395	304	0	general_description	keyword	1	0	305
1396	304	1	general description	keyword	1	0	303
1397	304	3	general_description	keyword	1	0	309
1398	305	0	general_description	keyword	1	0	306
1399	305	2	general description	keyword	1	0	304
1400	306	0	general_description	keyword	1	0	307
1401	306	1	general_description	keyword	1	0	308
1402	306	2	general description	keyword	1	0	305
1403	307	2	general description	keyword	1	0	306
1404	308	3	general description	keyword	1	0	306
1405	309	1	general description	keyword	1	0	304
1406	309	3	general_description	keyword	1	0	310
1407	310	1	general description	keyword	1	0	309
1408	310	3	general_description	keyword	1	0	311
1409	311	0	general_description	keyword	1	0	312
1410	311	1	general description	keyword	1	0	310
1411	312	0	general_description	keyword	1	0	313
1412	312	2	general description	keyword	1	0	311
1413	313	0	general_description	keyword	1	0	314
1414	313	1	general_description	keyword	1	0	315
1415	313	2	general description	keyword	1	0	312
1416	314	2	general description	keyword	1	0	313
1417	315	3	general description	keyword	1	0	313
1418	316	1	general_description	keyword	1	0	317
1419	316	3	general description	keyword	1	0	301
1420	317	0	general_description	keyword	1	0	318
1421	317	1	general_description	keyword	1	0	322
1422	317	3	general description	keyword	1	0	316
1423	318	0	general_description	keyword	1	0	319
1424	318	2	general description	keyword	1	0	317
1425	319	0	general_description	keyword	1	0	320
1426	319	1	general_description	keyword	1	0	321
1427	319	2	general description	keyword	1	0	318
1428	320	2	general description	keyword	1	0	319
1429	321	3	general description	keyword	1	0	319
1430	322	1	general_description	keyword	1	0	323
1431	322	3	general description	keyword	1	0	317
1432	323	1	general_description	keyword	1	0	324
1433	323	3	general description	keyword	1	0	322
1434	324	0	general_description	keyword	1	0	325
1435	324	3	general description	keyword	1	0	323
1436	325	0	general_description	keyword	1	0	326
1437	325	1	general_description	keyword	1	0	361
1438	325	2	general description	keyword	1	0	324
1439	326	0	general_description	keyword	1	0	327
1440	326	1	general_description	keyword	1	0	328
1441	326	2	general description	keyword	1	0	325
1442	327	2	general description	keyword	1	0	326
1443	328	2	general_description	keyword	1	0	361
1444	328	3	general description	keyword	1	0	326
1445	329	1	general description	keyword	1	0	295
1446	329	3	general_description	keyword	1	0	330
1447	330	1	general description	keyword	1	0	329
1448	330	3	general_description	keyword	1	0	331
1449	331	0	general_description	keyword	1	0	332
1450	331	1	general description	keyword	1	0	330
1451	332	2	general description	keyword	1	0	331
1452	333	0	general_description	keyword	1	0	334
1453	333	2	general description	keyword	1	0	290
1454	334	0	general_description	keyword	1	0	335
1455	334	2	general description	keyword	1	0	333
1456	335	0	general_description	keyword	1	0	336
1457	335	2	general description	keyword	1	0	334
1458	336	0	general_description	keyword	1	0	337
1459	336	2	general description	keyword	1	0	335
1460	337	2	general description	keyword	1	0	336
1461	338	1	general description	keyword	1	0	224
1462	338	5	general_description	keyword	1	0	339
1463	339	4	general description	keyword	1	0	338
1464	340	1	general description	keyword	1	0	202
1465	340	5	general_description	keyword	1	0	341
1466	341	3	general_description	keyword	1	0	342
1467	341	4	general description	keyword	1	0	340
1468	342	1	general description	keyword	1	0	341
1469	342	3	general_description	keyword	1	0	343
1470	343	1	general description	keyword	1	0	342
1471	343	5	general_description	keyword	1	0	344
1472	344	3	general_description	keyword	1	0	345
1473	344	4	general description	keyword	1	0	343
1474	345	1	general description	keyword	1	0	344
1475	345	3	general_description	keyword	1	0	346
1476	346	1	general description	keyword	1	0	345
1477	346	2	general_description	keyword	1	0	347
1478	347	0	general description	keyword	1	0	346
1479	347	2	general_description	keyword	1	0	348
1480	348	0	general description	keyword	1	0	347
1481	348	1	general_description	keyword	1	0	349
1482	349	1	general_description	keyword	1	0	350
1483	349	3	general description	keyword	1	0	348
1484	350	1	general_description	keyword	1	0	353
1485	350	2	general_description	keyword	1	0	351
1486	350	3	general description	keyword	1	0	349
1487	351	0	general description	keyword	1	0	350
1488	351	2	general_description	keyword	1	0	352
1489	352	0	general description	keyword	1	0	351
1490	353	1	general_description	keyword	1	0	354
1491	353	3	general description	keyword	1	0	350
1492	354	1	general_description	keyword	1	0	357
1493	354	2	general_description	keyword	1	0	355
1494	354	3	general description	keyword	1	0	353
1495	355	0	general description	keyword	1	0	354
1496	355	2	general_description	keyword	1	0	356
1497	356	0	general description	keyword	1	0	355
1498	357	1	general_description	keyword	1	0	358
1499	357	3	general description	keyword	1	0	354
1500	358	2	general_description	keyword	1	0	359
1501	358	3	general description	keyword	1	0	357
1502	359	0	general description	keyword	1	0	358
1503	359	2	general_description	keyword	1	0	360
1504	360	0	general description	keyword	1	0	359
1505	361	1	general_description	keyword	1	0	362
1506	361	3	general description	keyword	1	0	325
1507	362	1	general_description	keyword	1	0	363
1508	362	3	general description	keyword	1	0	361
1509	363	1	general_description	keyword	1	0	364
1510	363	3	general description	keyword	1	0	362
1511	364	0	general_description	keyword	1	0	365
1512	364	3	general description	keyword	1	0	363
1513	365	0	general_description	keyword	1	0	366
1514	365	2	general description	keyword	1	0	364
1515	366	0	general_description	keyword	1	0	367
1516	366	2	general description	keyword	1	0	365
1517	367	1	general_description	keyword	1	0	368
1518	367	2	general description	keyword	1	0	366
1519	368	1	general_description	keyword	1	0	369
1520	368	3	general description	keyword	1	0	367
1521	369	1	general_description	keyword	1	0	370
1522	369	3	general description	keyword	1	0	368
1523	370	1	general_description	keyword	1	0	371
1524	370	3	general description	keyword	1	0	369
1525	371	1	general_description	keyword	1	0	372
1526	371	3	general description	keyword	1	0	370
1527	372	1	general_description	keyword	1	0	373
1528	372	3	general description	keyword	1	0	371
1529	373	1	general_description	keyword	1	0	374
1530	373	3	general description	keyword	1	0	372
1531	374	1	general_description	keyword	1	0	375
1532	374	3	general description	keyword	1	0	373
1533	375	1	general_description	keyword	1	0	376
1534	375	3	general description	keyword	1	0	374
1535	376	1	general_description	keyword	1	0	377
1536	376	3	general description	keyword	1	0	375
1537	377	1	general_description	keyword	1	0	378
1538	377	3	general description	keyword	1	0	376
1539	378	1	general_description	keyword	1	0	379
1540	378	3	general description	keyword	1	0	377
1541	379	1	general_description	keyword	1	0	380
1542	379	3	general description	keyword	1	0	378
1543	380	1	general_description	keyword	1	0	381
1544	380	3	general description	keyword	1	0	379
1545	381	1	general_description	keyword	1	0	382
1546	381	3	general description	keyword	1	0	380
1547	382	1	general_description	keyword	1	0	383
1548	382	3	general description	keyword	1	0	381
1549	383	1	general_description	keyword	1	0	384
1550	383	3	general description	keyword	1	0	382
1551	384	1	general_description	keyword	1	0	385
1552	384	3	general description	keyword	1	0	383
1553	385	1	general_description	keyword	1	0	386
1554	385	3	general description	keyword	1	0	384
1555	386	1	general_description	keyword	1	0	387
1556	386	3	general description	keyword	1	0	385
1557	387	1	general_description	keyword	1	0	388
1558	387	3	general description	keyword	1	0	386
1559	388	1	general_description	keyword	1	0	389
1560	388	3	general description	keyword	1	0	387
1561	389	1	general_description	keyword	1	0	390
1562	389	3	general description	keyword	1	0	388
1563	390	1	general_description	keyword	1	0	391
1564	390	3	general description	keyword	1	0	389
1565	391	1	general_description	keyword	1	0	392
1566	391	3	general description	keyword	1	0	390
1567	392	1	general_description	keyword	1	0	393
1568	392	3	general description	keyword	1	0	391
1569	393	1	general_description	keyword	1	0	394
1570	393	3	general description	keyword	1	0	392
1571	394	1	general_description	keyword	1	0	395
1572	394	3	general description	keyword	1	0	393
1573	395	1	general_description	keyword	1	0	396
1574	395	3	general description	keyword	1	0	394
1575	396	1	general_description	keyword	1	0	397
1576	396	3	general description	keyword	1	0	395
1577	397	1	general_description	keyword	1	0	398
1578	397	3	general description	keyword	1	0	396
1579	398	1	general_description	keyword	1	0	399
1580	398	3	general description	keyword	1	0	397
1581	399	1	general_description	keyword	1	0	400
1582	399	3	general description	keyword	1	0	398
1583	400	0	general_description	keyword	1	0	401
1584	400	1	general_description	keyword	1	0	416
1585	400	3	general description	keyword	1	0	399
1586	401	0	general_description	keyword	1	0	402
1587	401	2	general description	keyword	1	0	400
1588	402	0	general_description	keyword	1	0	403
1589	402	2	general description	keyword	1	0	401
1590	403	1	general_description	keyword	1	0	404
1591	403	2	general description	keyword	1	0	402
1592	403	3	general_description	keyword	1	0	410
1593	404	1	general_description	keyword	1	0	405
1594	404	3	general description	keyword	1	0	403
1595	405	0	general_description	keyword	1	0	406
1596	405	3	general description	keyword	1	0	404
1597	406	0	general_description	keyword	1	0	407
1598	406	2	general description	keyword	1	0	405
1599	407	1	general_description	keyword	1	0	408
1600	407	2	general description	keyword	1	0	406
1601	408	1	general_description	keyword	1	0	409
1602	408	3	general description	keyword	1	0	407
1603	409	3	general description	keyword	1	0	408
1604	410	1	general description	keyword	1	0	403
1605	410	3	general_description	keyword	1	0	411
1606	411	0	general_description	keyword	1	0	412
1607	411	1	general description	keyword	1	0	410
1608	412	0	general_description	keyword	1	0	413
1609	412	2	general description	keyword	1	0	411
1610	413	2	general description	keyword	1	0	412
1611	413	3	general_description	keyword	1	0	414
1612	414	1	general description	keyword	1	0	413
1613	414	3	general_description	keyword	1	0	415
1614	415	1	general description	keyword	1	0	414
1615	416	1	general_description	keyword	1	0	417
1616	416	3	general description	keyword	1	0	400
1617	417	1	general_description	keyword	1	0	418
1618	417	3	general description	keyword	1	0	416
1619	418	1	general_description	keyword	1	0	419
1620	418	3	general description	keyword	1	0	417
1621	419	1	general_description	keyword	1	0	420
1622	419	3	general description	keyword	1	0	418
1623	420	1	general_description	keyword	1	0	421
1624	420	3	general description	keyword	1	0	419
1625	421	1	general_description	keyword	1	0	422
1626	421	3	general description	keyword	1	0	420
1627	422	1	general_description	keyword	1	0	423
1628	422	3	general description	keyword	1	0	421
1629	423	1	general_description	keyword	1	0	424
1630	423	3	general description	keyword	1	0	422
1631	424	1	general_description	keyword	1	0	425
1632	424	3	general description	keyword	1	0	423
1633	425	1	general_description	keyword	1	0	426
1634	425	3	general description	keyword	1	0	424
1635	426	1	general_description	keyword	1	0	427
1636	426	3	general description	keyword	1	0	425
1637	427	1	general_description	keyword	1	0	428
1638	427	3	general description	keyword	1	0	426
1639	428	1	general_description	keyword	1	0	429
1640	428	3	general description	keyword	1	0	427
1641	429	1	general_description	keyword	1	0	430
1642	429	3	general description	keyword	1	0	428
1643	430	1	general_description	keyword	1	0	431
1644	430	3	general description	keyword	1	0	429
1645	431	1	general_description	keyword	1	0	432
1646	431	3	general description	keyword	1	0	430
1647	432	1	general_description	keyword	1	0	433
1648	432	3	general description	keyword	1	0	431
1649	433	1	general_description	keyword	1	0	434
1650	433	3	general description	keyword	1	0	432
1651	434	1	general_description	keyword	1	0	435
1652	434	3	general description	keyword	1	0	433
1653	435	1	general_description	keyword	1	0	436
1654	435	3	general description	keyword	1	0	434
1655	436	1	general_description	keyword	1	0	437
1656	436	3	general description	keyword	1	0	435
1657	437	1	general_description	keyword	1	0	438
1658	437	3	general description	keyword	1	0	436
1659	438	1	general_description	keyword	1	0	439
1660	438	3	general description	keyword	1	0	437
1661	439	1	general_description	keyword	1	0	440
1662	439	3	general description	keyword	1	0	438
1663	440	1	general_description	keyword	1	0	441
1664	440	3	general description	keyword	1	0	439
1665	441	1	general_description	keyword	1	0	442
1666	441	3	general description	keyword	1	0	440
1667	442	1	general_description	keyword	1	0	443
1668	442	3	general description	keyword	1	0	441
1669	443	1	general_description	keyword	1	0	444
1670	443	3	general description	keyword	1	0	442
1671	444	1	general_description	keyword	1	0	445
1672	444	3	general description	keyword	1	0	443
1673	445	2	general_description	keyword	1	0	446
1674	445	3	general description	keyword	1	0	444
1675	446	0	general description	keyword	1	0	445
1676	446	2	general_description	keyword	1	0	447
1677	447	0	general description	keyword	1	0	446
1678	447	5	general_description	keyword	1	0	448
1679	448	1	general_description	keyword	1	0	449
1680	448	4	general description	keyword	1	0	447
1681	449	1	general_description	keyword	1	0	450
1682	449	3	general description	keyword	1	0	448
1683	450	1	general_description	keyword	1	0	451
1684	450	3	general description	keyword	1	0	449
1685	451	1	general_description	keyword	1	0	452
1686	451	3	general description	keyword	1	0	450
1687	452	1	general_description	keyword	1	0	453
1688	452	3	general description	keyword	1	0	451
1689	453	1	general_description	keyword	1	0	454
1690	453	3	general description	keyword	1	0	452
1691	454	1	general_description	keyword	1	0	455
1692	454	3	general description	keyword	1	0	453
1693	455	1	general_description	keyword	1	0	456
1694	455	3	general description	keyword	1	0	454
1695	456	0	general_description	keyword	1	0	457
1696	456	3	general description	keyword	1	0	455
1697	457	0	general_description	keyword	1	0	458
1698	457	2	general description	keyword	1	0	456
1699	458	0	general_description	keyword	1	0	459
1700	458	2	general description	keyword	1	0	457
1701	459	0	general_description	keyword	1	0	460
1702	459	2	general description	keyword	1	0	458
1703	460	0	general_description	keyword	1	0	461
1704	460	2	general description	keyword	1	0	459
1705	461	0	general_description	keyword	1	0	462
1706	461	2	general description	keyword	1	0	460
1707	462	0	general_description	keyword	1	0	463
1708	462	2	general description	keyword	1	0	461
1709	463	0	general_description	keyword	1	0	464
1710	463	2	general description	keyword	1	0	462
1711	464	1	general_description	keyword	1	0	465
1712	464	2	general description	keyword	1	0	463
1713	465	1	general_description	keyword	1	0	466
1714	465	3	general description	keyword	1	0	464
1715	466	1	general_description	keyword	1	0	467
1716	466	3	general description	keyword	1	0	465
1717	467	1	general_description	keyword	1	0	468
1718	467	3	general description	keyword	1	0	466
1719	468	0	general_description	keyword	1	0	469
1720	468	3	general description	keyword	1	0	467
1721	469	0	general_description	keyword	1	0	470
1722	469	2	general description	keyword	1	0	468
1723	470	0	general_description	keyword	1	0	471
1724	470	2	general description	keyword	1	0	469
1725	471	0	general_description	keyword	1	0	472
1726	471	2	general description	keyword	1	0	470
1727	472	0	general_description	keyword	1	0	473
1728	472	2	general description	keyword	1	0	471
1729	473	0	general_description	keyword	1	0	474
1730	473	2	general description	keyword	1	0	472
1731	474	0	general_description	keyword	1	0	475
1732	474	2	general description	keyword	1	0	473
1733	475	0	general_description	keyword	1	0	476
1734	475	2	general description	keyword	1	0	474
1735	476	0	general_description	keyword	1	0	477
1736	476	2	general description	keyword	1	0	475
1737	477	0	general_description	keyword	1	0	478
1738	477	2	general description	keyword	1	0	476
1739	478	0	general_description	keyword	1	0	479
1740	478	2	general description	keyword	1	0	477
1741	479	0	general_description	keyword	1	0	480
1742	479	2	general description	keyword	1	0	478
1743	480	0	general_description	keyword	1	0	481
1744	480	2	general description	keyword	1	0	479
1745	481	0	general_description	keyword	1	0	482
1746	481	2	general description	keyword	1	0	480
1747	482	0	general_description	keyword	1	0	483
1748	482	2	general description	keyword	1	0	481
1749	483	0	general_description	keyword	1	0	484
1750	483	2	general description	keyword	1	0	482
1751	484	0	general_description	keyword	1	0	485
1752	484	2	general description	keyword	1	0	483
1753	485	0	general_description	keyword	1	0	486
1754	485	2	general description	keyword	1	0	484
1755	486	0	general_description	keyword	1	0	487
1756	486	2	general description	keyword	1	0	485
1757	487	0	general_description	keyword	1	0	488
1758	487	2	general description	keyword	1	0	486
1759	488	0	general_description	keyword	1	0	489
1760	488	2	general description	keyword	1	0	487
1761	489	0	general_description	keyword	1	0	490
1762	489	2	general description	keyword	1	0	488
1763	490	0	general_description	keyword	1	0	491
1764	490	2	general description	keyword	1	0	489
1765	491	0	general_description	keyword	1	0	492
1766	491	2	general description	keyword	1	0	490
1767	492	0	general_description	keyword	1	0	493
1768	492	2	general description	keyword	1	0	491
1769	493	1	general_description	keyword	1	0	494
1770	493	2	general description	keyword	1	0	492
1771	494	1	general_description	keyword	1	0	495
1772	494	3	general description	keyword	1	0	493
1773	495	1	general_description	keyword	1	0	496
1774	495	3	general description	keyword	1	0	494
1775	496	1	general_description	keyword	1	0	497
1776	496	3	general description	keyword	1	0	495
1777	497	1	general_description	keyword	1	0	498
1778	497	3	general description	keyword	1	0	496
1779	498	1	general_description	keyword	1	0	499
1780	498	3	general description	keyword	1	0	497
1781	499	1	general_description	keyword	1	0	500
1782	499	3	general description	keyword	1	0	498
1783	500	1	general_description	keyword	1	0	501
1784	500	2	general_description	keyword	1	0	509
1785	500	3	general description	keyword	1	0	499
1786	501	1	general_description	keyword	1	0	502
1787	501	3	general description	keyword	1	0	500
1788	502	1	general_description	keyword	1	0	503
1789	502	2	general_description	keyword	1	0	513
1790	502	3	general description	keyword	1	0	501
1791	503	1	general_description	keyword	1	0	504
1792	503	3	general description	keyword	1	0	502
1793	504	1	general_description	keyword	1	0	505
1794	504	2	general_description	keyword	1	0	517
1795	504	3	general description	keyword	1	0	503
1796	505	1	general_description	keyword	1	0	506
1797	505	3	general description	keyword	1	0	504
1798	506	1	general_description	keyword	1	0	507
1799	506	2	general_description	keyword	1	0	521
1800	506	3	general description	keyword	1	0	505
1801	507	1	general_description	keyword	1	0	508
1802	507	3	general description	keyword	1	0	506
1803	508	2	general_description	keyword	1	0	525
1804	508	3	general description	keyword	1	0	507
1805	509	0	general description	keyword	1	0	500
1806	509	2	general_description	keyword	1	0	510
1807	510	0	general description	keyword	1	0	509
1808	510	2	general_description	keyword	1	0	511
1809	511	0	general description	keyword	1	0	510
1810	511	2	general_description	keyword	1	0	512
1811	512	0	general description	keyword	1	0	511
1812	513	0	general description	keyword	1	0	502
1813	513	2	general_description	keyword	1	0	514
1814	514	0	general description	keyword	1	0	513
1815	514	2	general_description	keyword	1	0	515
1816	515	0	general description	keyword	1	0	514
1817	515	2	general_description	keyword	1	0	516
1818	516	0	general description	keyword	1	0	515
1819	517	0	general description	keyword	1	0	504
1820	517	2	general_description	keyword	1	0	518
1821	518	0	general description	keyword	1	0	517
1822	518	2	general_description	keyword	1	0	519
1823	519	0	general description	keyword	1	0	518
1824	519	2	general_description	keyword	1	0	520
1825	520	0	general description	keyword	1	0	519
1826	521	0	general description	keyword	1	0	506
1827	521	2	general_description	keyword	1	0	522
1828	522	0	general description	keyword	1	0	521
1829	522	2	general_description	keyword	1	0	523
1830	523	0	general description	keyword	1	0	522
1831	523	2	general_description	keyword	1	0	524
1832	524	0	general description	keyword	1	0	523
1833	525	0	general description	keyword	1	0	508
1834	525	2	general_description	keyword	1	0	526
1835	526	0	general description	keyword	1	0	525
1836	526	2	general_description	keyword	1	0	527
1837	527	0	general description	keyword	1	0	526
1838	527	2	general_description	keyword	1	0	528
1839	528	0	general description	keyword	1	0	527
1840	528	2	general_description	keyword	1	0	529
1841	529	0	general description	keyword	1	0	528
1842	529	2	general_description	keyword	1	0	530
1843	530	0	general description	keyword	1	0	529
1844	530	1	general_description	keyword	1	0	531
1845	531	1	general_description	keyword	1	0	532
1846	531	3	general description	keyword	1	0	530
1847	532	1	general_description	keyword	1	0	533
1848	532	2	general_description	keyword	1	0	535
1849	532	3	general description	keyword	1	0	531
1850	533	1	general_description	keyword	1	0	534
1851	533	3	general description	keyword	1	0	532
1852	534	1	general_description	keyword	1	0	541
1853	534	3	general description	keyword	1	0	533
1854	535	0	general description	keyword	1	0	532
1855	535	2	general_description	keyword	1	0	536
1856	536	0	general description	keyword	1	0	535
1857	536	1	general_description	keyword	1	0	537
1858	536	3	general_description	keyword	1	0	539
1859	537	1	general_description	keyword	1	0	538
1860	537	3	general description	keyword	1	0	536
1861	538	3	general description	keyword	1	0	537
1862	539	1	general description	keyword	1	0	536
1863	539	3	general_description	keyword	1	0	540
1864	540	1	general description	keyword	1	0	539
1865	541	1	general_description	keyword	1	0	542
1866	541	3	general description	keyword	1	0	534
1867	542	0	general_description	keyword	1	0	545
1868	542	1	general_description	keyword	1	0	543
1869	542	3	general description	keyword	1	0	541
1870	543	1	general_description	keyword	1	0	544
1871	543	3	general description	keyword	1	0	542
1872	544	3	general description	keyword	1	0	543
1873	545	0	general_description	keyword	1	0	546
1874	545	2	general description	keyword	1	0	542
1875	546	0	general_description	keyword	1	0	547
1876	546	2	general description	keyword	1	0	545
1877	547	0	general_description	keyword	1	0	548
1878	547	1	general_description	keyword	1	0	556
1879	547	2	general description	keyword	1	0	546
1880	547	3	general_description	keyword	1	0	554
1881	548	0	general_description	keyword	1	0	549
1882	548	2	general description	keyword	1	0	547
1883	549	0	general_description	keyword	1	0	550
1884	549	1	general_description	keyword	1	0	560
1885	549	2	general description	keyword	1	0	548
1886	549	3	general_description	keyword	1	0	558
1887	550	0	general_description	keyword	1	0	551
1888	550	2	general description	keyword	1	0	549
1889	551	0	general_description	keyword	1	0	552
1890	551	1	general_description	keyword	1	0	564
1891	551	2	general description	keyword	1	0	550
1892	551	3	general_description	keyword	1	0	562
1893	552	0	general_description	keyword	1	0	553
1894	552	2	general description	keyword	1	0	551
1895	553	1	general_description	keyword	1	0	566
1896	553	2	general description	keyword	1	0	552
1897	553	3	general_description	keyword	1	0	568
1898	554	1	general description	keyword	1	0	547
1899	554	3	general_description	keyword	1	0	555
1900	555	1	general description	keyword	1	0	554
1901	556	1	general_description	keyword	1	0	557
1902	556	3	general description	keyword	1	0	547
1903	557	3	general description	keyword	1	0	556
1904	558	1	general description	keyword	1	0	549
1905	558	3	general_description	keyword	1	0	559
1906	559	1	general description	keyword	1	0	558
1907	560	1	general_description	keyword	1	0	561
1908	560	3	general description	keyword	1	0	549
1909	561	3	general description	keyword	1	0	560
1910	562	1	general description	keyword	1	0	551
1911	562	3	general_description	keyword	1	0	563
1912	563	1	general description	keyword	1	0	562
1913	564	1	general_description	keyword	1	0	565
1914	564	3	general description	keyword	1	0	551
1915	565	3	general description	keyword	1	0	564
1916	566	1	general_description	keyword	1	0	567
1917	566	3	general description	keyword	1	0	553
1918	567	0	general_description	keyword	1	0	571
1919	567	3	general description	keyword	1	0	566
1920	568	0	general_description	keyword	1	0	569
1921	568	1	general description	keyword	1	0	553
1922	569	2	general description	keyword	1	0	568
1923	569	3	general_description	keyword	1	0	570
1924	570	1	general description	keyword	1	0	569
1925	571	1	general_description	keyword	1	0	572
1926	571	2	general description	keyword	1	0	567
1927	572	0	general_description	keyword	1	0	573
1928	572	3	general description	keyword	1	0	571
1929	573	2	general description	keyword	1	0	572
1930	574	1	general_description	keyword	3137	0	575
1931	574	3	general description	keyword	1	0	289
1932	575	1	general_description	keyword	1	0	576
1933	575	3	general description	keyword	1	0	574
1934	576	1	general_description	keyword	1	0	577
1935	576	3	general description	keyword	1	0	575
1936	577	1	general_description	keyword	1	0	578
1937	577	3	general description	keyword	1	0	576
1938	578	0	general_description	keyword	1	0	579
1939	578	1	general_description	keyword	1	0	582
1940	578	3	general description	keyword	1	0	577
1941	579	0	general_description	keyword	1	0	580
1942	579	2	general description	keyword	1	0	578
1943	580	0	general_description	keyword	1	0	581
1944	580	2	general description	keyword	1	0	579
1945	581	2	general description	keyword	1	0	580
1946	582	1	general_description	keyword	1	0	583
1947	582	3	general description	keyword	1	0	578
1948	583	1	general_description	keyword	1	0	584
1949	583	3	general description	keyword	1	0	582
1950	584	1	general_description	keyword	1	0	585
1951	584	3	general description	keyword	1	0	583
1952	585	1	general_description	keyword	1	0	586
1953	585	3	general description	keyword	1	0	584
1954	586	1	general_description	keyword	1	0	587
1955	586	3	general description	keyword	1	0	585
1956	587	1	general_description	keyword	1	0	588
1957	587	3	general description	keyword	1	0	586
1958	588	1	general_description	keyword	1	0	589
1959	588	3	general description	keyword	1	0	587
1960	589	2	general_description	keyword	1	0	590
1961	589	3	general description	keyword	1	0	588
1962	590	0	general description	keyword	1	0	589
1963	590	2	general_description	keyword	1	0	591
1964	591	0	general description	keyword	1	0	590
1965	591	2	general_description	keyword	1	0	592
1966	592	0	general description	keyword	1	0	591
1967	592	3	general_description	keyword	1	0	593
1968	593	1	general description	keyword	1	0	592
1969	593	3	general_description	keyword	1	0	594
1970	594	1	general description	keyword	1	0	593
1971	594	3	general_description	keyword	1	0	595
1972	595	1	general description	keyword	1	0	594
1973	595	3	general_description	keyword	1	0	596
1974	596	1	general description	keyword	1	0	595
1975	596	3	general_description	keyword	1	0	597
1976	597	1	general description	keyword	1	0	596
1977	597	3	general_description	keyword	1	0	598
1978	598	1	general description	keyword	1	0	597
1979	598	3	general_description	keyword	1	0	599
1980	599	1	general description	keyword	1	0	598
1981	599	3	general_description	keyword	1	0	600
1982	600	1	general description	keyword	1	0	599
1983	600	2	general_description	keyword	1	0	601
1984	601	0	general description	keyword	1	0	600
1985	601	2	general_description	keyword	1	0	602
1986	602	0	general description	keyword	1	0	601
1987	602	2	general_description	keyword	1	0	603
1988	603	0	general description	keyword	1	0	602
1989	603	1	general_description	keyword	1	0	604
1990	604	1	general_description	keyword	1	0	605
1991	604	3	general description	keyword	1	0	603
1992	605	1	general_description	keyword	1	0	606
1993	605	3	general description	keyword	1	0	604
1994	606	1	general_description	keyword	1	0	607
1995	606	3	general description	keyword	1	0	605
1996	607	1	general_description	keyword	1	0	608
1997	607	3	general description	keyword	1	0	606
1998	608	1	general_description	keyword	1	0	609
1999	608	3	general description	keyword	1	0	607
2000	609	1	general_description	keyword	1	0	610
2001	609	3	general description	keyword	1	0	608
2002	610	1	general_description	keyword	1	0	611
2003	610	3	general description	keyword	1	0	609
2004	611	2	general_description	keyword	1	0	612
2005	611	3	general description	keyword	1	0	610
2006	612	0	general description	keyword	1	0	611
2007	612	2	general_description	keyword	1	0	613
2008	613	0	general description	keyword	1	0	612
2009	613	2	general_description	keyword	1	0	614
2010	614	0	general description	keyword	1	0	613
2011	614	3	general_description	keyword	1	0	615
2012	615	1	general description	keyword	1	0	614
2013	615	3	general_description	keyword	1	0	616
2014	616	1	general description	keyword	1	0	615
2015	616	3	general_description	keyword	1	0	617
2016	617	1	general description	keyword	1	0	616
2017	617	3	general_description	keyword	1	0	618
2018	618	1	general description	keyword	1	0	617
2019	618	3	general_description	keyword	1	0	619
2020	619	1	general description	keyword	1	0	618
2021	619	3	general_description	keyword	1	0	620
2022	620	1	general description	keyword	1	0	619
2023	620	3	general_description	keyword	1	0	621
2024	621	1	general description	keyword	1	0	620
2025	621	3	general_description	keyword	1	0	622
2026	622	1	general description	keyword	1	0	621
2027	622	2	general_description	keyword	1	0	623
2028	623	0	general description	keyword	1	0	622
2029	623	2	general_description	keyword	1	0	624
2030	624	0	general description	keyword	1	0	623
2031	624	2	general_description	keyword	1	0	625
2032	625	0	general description	keyword	1	0	624
2033	625	1	general_description	keyword	1	0	626
2034	626	1	general_description	keyword	1	0	627
2035	626	3	general description	keyword	1	0	625
2036	627	1	general_description	keyword	1	0	628
2037	627	3	general description	keyword	1	0	626
2038	628	1	general_description	keyword	1	0	629
2039	628	3	general description	keyword	1	0	627
2040	629	1	general_description	keyword	1	0	630
2041	629	3	general description	keyword	1	0	628
2042	630	1	general_description	keyword	1	0	631
2043	630	3	general description	keyword	1	0	629
2044	631	1	general_description	keyword	1	0	632
2045	631	3	general description	keyword	1	0	630
2046	632	1	general_description	keyword	1	0	633
2047	632	3	general description	keyword	1	0	631
2048	633	2	general_description	keyword	1	0	634
2049	633	3	general description	keyword	1	0	632
2050	634	0	general description	keyword	1	0	633
2051	634	2	general_description	keyword	1	0	635
2052	635	0	general description	keyword	1	0	634
2053	635	2	general_description	keyword	1	0	636
2054	636	0	general description	keyword	1	0	635
2055	636	2	general_description	keyword	1	0	637
2056	637	0	general description	keyword	1	0	636
2057	637	2	general_description	keyword	1	0	638
2058	638	0	general description	keyword	1	0	637
2059	638	2	general_description	keyword	1	0	639
2060	639	0	general description	keyword	1	0	638
2061	639	2	general_description	keyword	1	0	640
2062	640	0	general description	keyword	1	0	639
2063	640	2	general_description	keyword	1	0	641
2064	641	0	general description	keyword	1	0	640
2065	641	2	general_description	keyword	1	0	642
2066	642	0	general description	keyword	1	0	641
2067	642	2	general_description	keyword	1	0	643
2068	643	0	general description	keyword	1	0	642
2069	643	2	general_description	keyword	1	0	644
2070	644	0	general description	keyword	1	0	643
2071	644	2	general_description	keyword	1	0	645
2072	645	0	general description	keyword	1	0	644
2073	645	2	general_description	keyword	1	0	646
2074	646	0	general description	keyword	1	0	645
2075	646	2	general_description	keyword	1	0	647
2076	647	0	general description	keyword	1	0	646
2077	647	2	general_description	keyword	1	0	648
2078	648	0	general description	keyword	1	0	647
2079	648	2	general_description	keyword	1	0	649
2080	649	0	general description	keyword	1	0	648
2081	649	2	general_description	keyword	1	0	650
2082	650	0	general description	keyword	1	0	649
2083	650	2	general_description	keyword	1	0	651
2084	651	0	general description	keyword	1	0	650
2085	651	2	general_description	keyword	1	0	652
2086	652	0	general description	keyword	1	0	651
2087	652	2	general_description	keyword	1	0	653
2088	653	0	general description	keyword	1	0	652
2089	653	1	general_description	keyword	1	0	654
2090	653	2	general_description	keyword	1	0	682
2091	653	3	general_description	keyword	1	0	668
2092	654	1	general_description	keyword	1	0	655
2093	654	2	general_description	keyword	1	0	667
2094	654	3	general description	keyword	1	0	653
2095	655	1	general_description	keyword	1	0	656
2096	655	3	general description	keyword	1	0	654
2097	656	1	general_description	keyword	1	0	657
2098	656	3	general description	keyword	1	0	655
2099	657	2	general_description	keyword	1	0	658
2100	657	3	general description	keyword	1	0	656
2101	658	0	general description	keyword	1	0	657
2102	658	2	general_description	keyword	1	0	659
2103	659	0	general description	keyword	1	0	658
2104	659	2	general_description	keyword	1	0	660
2105	660	0	general description	keyword	1	0	659
2106	660	2	general_description	keyword	1	0	661
2107	661	0	general description	keyword	1	0	660
2108	661	3	general_description	keyword	1	0	662
2109	662	1	general description	keyword	1	0	661
2110	662	3	general_description	keyword	1	0	663
2111	663	1	general description	keyword	1	0	662
2112	663	3	general_description	keyword	1	0	664
2113	664	0	general_description	keyword	1	0	665
2114	664	1	general description	keyword	1	0	663
2115	665	0	general_description	keyword	1	0	666
2116	665	2	general description	keyword	1	0	664
2117	666	0	general_description	keyword	1	0	667
2118	666	2	general description	keyword	1	0	665
2119	667	2	general description	keyword	1	0	666
2120	667	3	general_description	keyword	1	0	682
2121	668	1	general description	keyword	1	0	653
2122	668	2	general_description	keyword	1	0	681
2123	668	3	general_description	keyword	1	0	669
2124	669	1	general description	keyword	1	0	668
2125	669	3	general_description	keyword	1	0	670
2126	670	1	general description	keyword	1	0	669
2127	670	3	general_description	keyword	1	0	671
2128	671	1	general description	keyword	1	0	670
2129	671	2	general_description	keyword	1	0	672
2130	672	0	general description	keyword	1	0	671
2131	672	2	general_description	keyword	1	0	673
2132	673	0	general description	keyword	1	0	672
2133	673	2	general_description	keyword	1	0	674
2134	674	0	general description	keyword	1	0	673
2135	674	2	general_description	keyword	1	0	675
2136	675	0	general description	keyword	1	0	674
2137	675	1	general_description	keyword	1	0	676
2138	676	1	general_description	keyword	1	0	677
2139	676	3	general description	keyword	1	0	675
2140	677	1	general_description	keyword	1	0	678
2141	677	3	general description	keyword	1	0	676
2142	678	0	general_description	keyword	1	0	679
2143	678	3	general description	keyword	1	0	677
2144	679	0	general_description	keyword	1	0	680
2145	679	2	general description	keyword	1	0	678
2146	680	0	general_description	keyword	1	0	681
2147	680	2	general description	keyword	1	0	679
2148	681	1	general_description	keyword	1	0	682
2149	681	2	general description	keyword	1	0	680
2150	682	0	general description	keyword	1	0	653
2151	682	1	general description	keyword	1	0	667
2152	682	3	general description	keyword	1	0	681
2153	192	1	general description	keyword	1	0	191
2154	192	3	general_description	keyword	1	0	684
2155	684	1	general description	keyword	1	0	192
2156	684	3	general_description	keyword	1	0	685
2157	685	1	general description	keyword	1	0	684
2158	685	3	general_description	keyword	1	0	686
2159	686	0	general_description	keyword	1	0	687
2160	686	1	general description	keyword	1	0	685
2161	686	2	general_description	keyword	1	0	699
2162	686	3	general description	keyword	1	0	697
2163	687	0	general_description	keyword	1	0	688
2164	687	2	general description	keyword	1	0	686
2165	687	3	general_description	keyword	1	0	698
2166	688	2	general description	keyword	1	0	687
2167	688	3	general_description	keyword	1	0	689
2168	689	1	general description	keyword	1	0	688
2169	689	2	general_description	keyword	1	0	698
2170	689	3	general_description	keyword	1	0	690
2171	690	1	general description	keyword	1	0	689
2172	690	2	general_description	keyword	1	0	691
2173	691	0	general description	keyword	1	0	690
2174	691	1	general_description	keyword	1	0	698
2175	691	2	general_description	keyword	1	0	692
2176	691	3	general_description	keyword	1	0	701
2177	692	0	general description	keyword	1	0	691
2178	692	1	general_description	keyword	1	0	697
2179	692	2	general_description	keyword	1	0	693
2180	693	0	general description	keyword	1	0	692
2181	693	1	general_description	keyword	1	0	696
2182	693	2	general_description	keyword	1	0	694
2183	694	0	general description	keyword	1	0	693
2184	694	1	general_description	keyword	1	0	695
2185	695	0	general_description	keyword	1	0	696
2186	695	1	general_description	keyword	1	0	700
2187	695	3	general description	keyword	1	0	694
2188	696	0	general_description	keyword	1	0	697
2189	696	1	general_description	keyword	1	0	699
2190	696	2	general description	keyword	1	0	695
2191	696	3	general description	keyword	1	0	693
2192	697	0	general_description	keyword	1	0	698
2193	697	1	general description	keyword	1	0	686
2194	697	2	general description	keyword	1	0	696
2195	697	3	general description	keyword	1	0	692
2196	697	5	A	metal	0	3	128
2197	698	0	general description	keyword	1	0	689
2198	698	1	general description	keyword	1	0	687
2199	698	2	general description	keyword	1	0	697
2200	698	3	general description	keyword	1	0	691
2201	699	0	general description	keyword	1	0	686
2202	699	2	general_description	keyword	1	0	700
2203	699	3	general description	keyword	1	0	696
2204	700	0	general description	keyword	1	0	699
2205	700	3	general description	keyword	1	0	695
2206	701	1	general description	keyword	1	0	691
2207	701	3	general_description	keyword	1	0	702
2208	702	0	general_description	keyword	1	0	707
2209	702	1	general description	keyword	1	0	701
2210	702	3	general_description	keyword	1	0	703
2211	703	1	general description	keyword	1	0	702
2212	703	3	general_description	keyword	1	0	704
2213	704	1	general description	keyword	1	0	703
2214	704	3	general_description	keyword	1	0	705
2215	705	1	general description	keyword	1	0	704
2216	705	3	general_description	keyword	1	0	706
2217	706	1	general description	keyword	1	0	705
2218	707	0	general_description	keyword	1	0	708
2219	707	2	general description	keyword	1	0	702
2220	708	0	general_description	keyword	1	0	709
2221	708	2	general description	keyword	1	0	707
2222	709	0	general_description	keyword	1	0	710
2223	709	2	general description	keyword	1	0	708
2224	710	0	general_description	keyword	1	0	717
2225	710	1	general_description	keyword	1	0	711
2226	710	2	general description	keyword	1	0	709
2227	710	3	general_description	keyword	1	0	713
2228	711	1	general_description	keyword	1	0	712
2229	711	3	general description	keyword	1	0	710
2230	712	0	general_description	keyword	1	0	719
2231	712	3	general description	keyword	1	0	711
2232	713	1	general description	keyword	1	0	710
2233	713	3	general_description	keyword	1	0	714
2234	714	0	general_description	keyword	1	0	715
2235	714	1	general description	keyword	1	0	713
2236	715	0	general_description	keyword	1	0	716
2237	715	2	general description	keyword	1	0	714
2238	716	1	general_description	keyword	1	0	722
2239	716	2	general description	keyword	1	0	715
2240	717	0	general_description	keyword	1	0	718
2241	717	2	general description	keyword	1	0	710
2242	718	0	general_description	keyword	1	0	723
2243	718	1	general_description	keyword	1	0	721
2244	718	2	general description	keyword	1	0	717
2245	718	3	general_description	keyword	1	0	722
2246	719	0	general_description	keyword	1	0	720
2247	719	2	general description	keyword	1	0	712
2248	720	2	general description	keyword	1	0	719
2249	720	3	general_description	keyword	1	0	721
2250	721	1	general description	keyword	1	0	720
2251	721	3	general description	keyword	1	0	718
2252	722	1	general description	keyword	1	0	718
2253	722	3	general description	keyword	1	0	716
2254	723	0	general_description	keyword	1	0	724
2255	723	2	general description	keyword	1	0	718
2256	724	0	general_description	keyword	1	0	725
2257	724	2	general description	keyword	1	0	723
2258	724	3	general_description	keyword	1	0	729
2259	725	0	general_description	keyword	1	0	726
2260	725	1	general_description	keyword	1	0	731
2261	725	2	general description	keyword	1	0	724
2262	726	0	general_description	keyword	1	0	727
2263	726	2	general description	keyword	1	0	725
2264	726	3	general_description	keyword	1	0	730
2265	727	0	general_description	keyword	1	0	728
2266	727	1	general_description	keyword	1	0	732
2267	727	2	general description	keyword	1	0	726
2268	728	2	general description	keyword	1	0	727
2269	729	1	general description	keyword	1	0	724
2270	730	1	general description	keyword	1	0	726
2271	731	3	general description	keyword	1	0	725
2272	732	3	general description	keyword	1	0	727
2273	192	1	general description	keyword	1	0	191
2274	192	3	general_description	keyword	1	0	684
2275	684	1	general description	keyword	1	0	192
2276	684	3	general_description	keyword	1	0	685
2277	685	1	general description	keyword	1	0	684
2278	685	3	general_description	keyword	1	0	686
2279	686	0	general_description	keyword	1	0	687
2280	686	1	general description	keyword	1	0	685
2281	686	2	general_description	keyword	1	0	699
2282	686	3	general description	keyword	1	0	697
2283	687	0	general_description	keyword	1	0	688
2284	687	2	general description	keyword	1	0	686
2285	687	3	general_description	keyword	1	0	698
2286	688	2	general description	keyword	1	0	687
2287	688	3	general_description	keyword	1	0	689
2288	689	1	general description	keyword	1	0	688
2289	689	2	general_description	keyword	1	0	698
2290	689	3	general_description	keyword	1	0	690
2291	690	1	general description	keyword	1	0	689
2292	690	2	general_description	keyword	1	0	691
2293	691	0	general description	keyword	1	0	690
2294	691	1	general_description	keyword	1	0	698
2295	691	2	general_description	keyword	1	0	692
2296	691	3	general_description	keyword	1	0	701
2297	692	0	general description	keyword	1	0	691
2298	692	1	general_description	keyword	1	0	697
2299	692	2	general_description	keyword	1	0	693
2300	693	0	general description	keyword	1	0	692
2301	693	1	general_description	keyword	1	0	696
2302	693	2	general_description	keyword	1	0	694
2303	694	0	general description	keyword	1	0	693
2304	694	1	general_description	keyword	1	0	695
2305	695	0	general_description	keyword	1	0	696
2306	695	1	general_description	keyword	1	0	700
2307	695	3	general description	keyword	1	0	694
2308	696	0	general_description	keyword	1	0	697
2309	696	1	general_description	keyword	1	0	699
2310	696	2	general description	keyword	1	0	695
2311	696	3	general description	keyword	1	0	693
2312	697	0	general_description	keyword	1	0	698
2313	697	1	general description	keyword	1	0	686
2314	697	2	general description	keyword	1	0	696
2315	697	3	general description	keyword	1	0	692
2316	697	5	A	metal	0	3	128
2317	698	0	general description	keyword	1	0	689
2318	698	1	general description	keyword	1	0	687
2319	698	2	general description	keyword	1	0	697
2320	698	3	general description	keyword	1	0	691
2321	699	0	general description	keyword	1	0	686
2322	699	2	general_description	keyword	1	0	700
2323	699	3	general description	keyword	1	0	696
2324	700	0	general description	keyword	1	0	699
2325	700	3	general description	keyword	1	0	695
2326	701	1	general description	keyword	1	0	691
2327	701	3	general_description	keyword	1	0	702
2328	702	0	general_description	keyword	1	0	707
2329	702	1	general description	keyword	1	0	701
2330	702	3	general_description	keyword	1	0	703
2331	703	1	general description	keyword	1	0	702
2332	703	3	general_description	keyword	1	0	704
2333	704	1	general description	keyword	1	0	703
2334	704	3	general_description	keyword	1	0	705
2335	705	0	general_description	keyword	1	0	733
2336	705	1	general description	keyword	1	0	704
2337	705	3	general_description	keyword	1	0	706
2338	706	1	general description	keyword	1	0	705
2339	707	0	general_description	keyword	1	0	708
2340	707	2	general description	keyword	1	0	702
2341	708	0	general_description	keyword	1	0	709
2342	708	2	general description	keyword	1	0	707
2343	709	0	general_description	keyword	1	0	710
2344	709	2	general description	keyword	1	0	708
2345	710	0	general_description	keyword	1	0	717
2346	710	1	general_description	keyword	1	0	711
2347	710	2	general description	keyword	1	0	709
2348	710	3	general_description	keyword	1	0	713
2349	711	1	general_description	keyword	1	0	712
2350	711	3	general description	keyword	1	0	710
2351	712	0	general_description	keyword	1	0	719
2352	712	3	general description	keyword	1	0	711
2353	713	1	general description	keyword	1	0	710
2354	713	3	general_description	keyword	1	0	714
2355	714	0	general_description	keyword	1	0	715
2356	714	1	general description	keyword	1	0	713
2357	714	3	general_description	keyword	1	0	736
2358	715	0	general_description	keyword	1	0	716
2359	715	2	general description	keyword	1	0	714
2360	715	3	general_description	keyword	1	0	737
2361	716	1	general_description	keyword	1	0	722
2362	716	2	general description	keyword	1	0	715
2363	716	3	general_description	keyword	1	0	738
2364	717	0	general_description	keyword	1	0	718
2365	717	2	general description	keyword	1	0	710
2366	718	0	general_description	keyword	1	0	723
2367	718	1	general_description	keyword	1	0	721
2368	718	2	general description	keyword	1	0	717
2369	718	3	general_description	keyword	1	0	722
2370	719	0	general_description	keyword	1	0	720
2371	719	2	general description	keyword	1	0	712
2372	720	2	general description	keyword	1	0	719
2373	720	3	general_description	keyword	1	0	721
2374	721	1	general description	keyword	1	0	720
2375	721	3	general description	keyword	1	0	718
2376	722	1	general description	keyword	1	0	718
2377	722	3	general description	keyword	1	0	716
2378	723	0	general_description	keyword	1	0	724
2379	723	2	general description	keyword	1	0	718
2380	724	0	general_description	keyword	1	0	725
2381	724	2	general description	keyword	1	0	723
2382	724	3	general_description	keyword	1	0	729
2383	725	0	general_description	keyword	1	0	726
2384	725	1	general_description	keyword	1	0	731
2385	725	2	general description	keyword	1	0	724
2386	726	0	general_description	keyword	1	0	727
2387	726	2	general description	keyword	1	0	725
2388	726	3	general_description	keyword	1	0	730
2389	727	0	general_description	keyword	1	0	728
2390	727	1	general_description	keyword	1	0	732
2391	727	2	general description	keyword	1	0	726
2392	728	2	general description	keyword	1	0	727
2393	728	3	general_description	keyword	1	0	746
2394	729	1	general description	keyword	1	0	724
2395	730	1	general description	keyword	1	0	726
2396	731	3	general description	keyword	1	0	725
2397	732	3	general description	keyword	1	0	727
2398	733	0	general_description	keyword	1	0	734
2399	733	2	general description	keyword	1	0	705
2400	734	0	general_description	keyword	1	0	735
2401	734	2	general description	keyword	1	0	733
2402	735	0	general_description	keyword	1	0	736
2403	735	2	general description	keyword	1	0	734
2404	736	0	general_description	keyword	1	0	737
2405	736	2	general description	keyword	1	0	735
2406	737	0	general_description	keyword	1	0	738
2407	737	2	general description	keyword	1	0	736
2408	738	0	general_description	keyword	1	0	739
2409	738	2	general description	keyword	1	0	737
2410	739	0	general_description	keyword	1	0	740
2411	739	2	general description	keyword	1	0	738
2412	740	0	general_description	keyword	1	0	741
2413	740	2	general description	keyword	1	0	739
2414	741	0	general_description	keyword	1	0	742
2415	741	2	general description	keyword	1	0	740
2416	742	0	general_description	keyword	1	0	743
2417	742	2	general description	keyword	1	0	741
2418	743	0	general_description	keyword	1	0	744
2419	743	2	general description	keyword	1	0	742
2420	744	1	general_description	keyword	347	0	745
2421	744	2	general description	keyword	1	0	743
2422	745	1	general_description	keyword	1	0	746
2423	745	3	general description	keyword	1	0	744
2424	746	3	general description	keyword	1	0	745
2425	744	1	general_description	keyword	347	0	745
2426	744	2	general description	keyword	1	0	743
2427	744	3	general_description	keyword	1	0	748
2428	748	1	general description	keyword	1	0	744
2429	748	3	general_description	keyword	1	0	749
2430	749	1	general description	keyword	347	0	748
2431	744	1	general_description	keyword	347	0	745
2432	744	2	general description	keyword	1	0	743
2433	744	3	general_description	keyword	1	0	748
2434	748	1	general description	keyword	1	0	744
2435	748	3	general_description	keyword	1	0	749
2436	749	1	general description	keyword	347	0	748
2437	695	0	general_description	keyword	1	0	696
2438	695	1	general_description	keyword	1	0	700
2439	695	2	general_description	keyword	1	0	750
2440	695	3	general description	keyword	1	0	694
2441	750	0	general description	keyword	1	0	695
2442	750	2	general_description	keyword	1	0	751
2443	751	0	general description	keyword	1	0	750
2444	751	2	general_description	keyword	1	0	752
2445	752	0	general description	keyword	1	0	751
2446	752	2	general_description	keyword	1	0	753
2447	753	0	general description	keyword	1	0	752
2448	753	2	general_description	keyword	1	0	754
2449	754	0	general description	keyword	1	0	753
2450	754	3	general_description	keyword	1	0	755
2451	755	1	general description	keyword	1	0	754
2452	755	3	general_description	keyword	1	0	756
2453	756	1	general description	keyword	1	0	755
2454	756	3	general_description	keyword	1	0	757
2455	757	1	general description	keyword	1	0	756
2456	757	2	general_description	keyword	1	0	758
2457	758	0	general description	keyword	1	0	757
2458	758	2	general_description	keyword	1	0	759
2459	759	0	general description	keyword	1	0	758
2460	759	2	general_description	keyword	1	0	760
2461	760	0	general description	keyword	1	0	759
2462	760	2	general_description	keyword	1	0	761
2463	761	0	general description	keyword	1	0	760
2464	761	3	general_description	keyword	1	0	762
2465	762	1	general description	keyword	1	0	761
2466	762	3	general_description	keyword	1	0	763
2467	763	1	general description	keyword	1	0	762
2468	763	3	general_description	keyword	1	0	764
2469	764	1	general description	keyword	1	0	763
2470	764	2	general_description	keyword	1	0	765
2471	765	0	general description	keyword	1	0	764
2472	765	2	general_description	keyword	1	0	766
2473	766	0	general description	keyword	1	0	765
2474	766	2	general_description	keyword	1	0	767
2475	767	0	general description	keyword	1	0	766
2476	767	2	general_description	keyword	1	0	768
2477	768	0	general description	keyword	1	0	767
2478	768	2	general_description	keyword	1	0	769
2479	769	0	general description	keyword	1	0	768
2480	769	3	general_description	keyword	1	0	770
2481	770	1	general description	keyword	1	0	769
2482	770	3	general_description	keyword	1	0	771
2483	771	1	general description	keyword	1	0	770
2484	771	3	general_description	keyword	1	0	772
2485	772	1	general description	keyword	1	0	771
2486	772	3	general_description	keyword	1	0	773
2487	773	1	general description	keyword	1	0	772
2488	773	3	general_description	keyword	1	0	774
2489	774	1	general description	keyword	1	0	773
2490	774	2	general_description	keyword	1	0	775
2491	775	0	general description	keyword	1	0	774
2492	775	2	general_description	keyword	1	0	776
2493	776	0	general description	keyword	1	0	775
2494	776	2	general_description	keyword	1	0	777
2495	777	0	general description	keyword	1	0	776
2496	777	2	general_description	keyword	1	0	778
2497	778	0	general description	keyword	1	0	777
2498	778	2	general_description	keyword	1	0	779
2499	779	0	general description	keyword	1	0	778
2500	779	3	general_description	keyword	1	0	780
2501	780	1	general description	keyword	1	0	779
2502	780	3	general_description	keyword	1	0	781
2503	781	1	general description	keyword	1	0	780
2504	781	3	general_description	keyword	1	0	782
2505	782	1	general description	keyword	1	0	781
2506	782	3	general_description	keyword	1	0	783
2507	783	1	general description	keyword	1	0	782
2508	783	3	general_description	keyword	1	0	784
2509	784	1	general description	keyword	1	0	783
2510	744	1	general_description	keyword	347	0	745
2511	744	2	general description	keyword	1	0	743
2512	744	3	general_description	keyword	1	0	748
2513	748	1	general description	keyword	1	0	744
2514	748	3	general_description	keyword	1	0	749
2515	749	1	general description	keyword	347	0	748
2516	695	0	general_description	keyword	1	0	696
2517	695	1	general_description	keyword	1	0	700
2518	695	2	general_description	keyword	1	0	750
2519	695	3	general description	keyword	1	0	694
2520	750	0	general description	keyword	1	0	695
2521	750	2	general_description	keyword	1	0	751
2522	751	0	general description	keyword	1	0	750
2523	751	2	general_description	keyword	1	0	752
2524	752	0	general description	keyword	1	0	751
2525	752	2	general_description	keyword	1	0	753
2526	753	0	general description	keyword	1	0	752
2527	753	2	general_description	keyword	1	0	754
2528	754	0	general description	keyword	1	0	753
2529	754	2	general_description	keyword	1	0	785
2530	754	3	general_description	keyword	1	0	755
2531	755	1	general description	keyword	1	0	754
2532	755	3	general_description	keyword	1	0	756
2533	756	1	general description	keyword	1	0	755
2534	756	3	general_description	keyword	1	0	757
2535	757	1	general description	keyword	1	0	756
2536	757	2	general_description	keyword	1	0	758
2537	758	0	general description	keyword	1	0	757
2538	758	2	general_description	keyword	1	0	759
2539	759	0	general description	keyword	1	0	758
2540	759	2	general_description	keyword	1	0	760
2541	760	0	general description	keyword	1	0	759
2542	760	2	general_description	keyword	1	0	761
2543	761	0	general description	keyword	1	0	760
2544	761	3	general_description	keyword	1	0	762
2545	762	1	general description	keyword	1	0	761
2546	762	3	general_description	keyword	1	0	763
2547	763	1	general description	keyword	1	0	762
2548	763	3	general_description	keyword	1	0	764
2549	764	1	general description	keyword	1	0	763
2550	764	2	general_description	keyword	1	0	765
2551	765	0	general description	keyword	1	0	764
2552	765	2	general_description	keyword	1	0	766
2553	766	0	general description	keyword	1	0	765
2554	766	2	general_description	keyword	1	0	767
2555	767	0	general description	keyword	1	0	766
2556	767	2	general_description	keyword	1	0	768
2557	768	0	general description	keyword	1	0	767
2558	768	2	general_description	keyword	1	0	769
2559	769	0	general description	keyword	1	0	768
2560	769	3	general_description	keyword	1	0	770
2561	770	1	general description	keyword	1	0	769
2562	770	3	general_description	keyword	1	0	771
2563	771	1	general description	keyword	1	0	770
2564	771	3	general_description	keyword	1	0	772
2565	772	1	general description	keyword	1	0	771
2566	772	3	general_description	keyword	1	0	773
2567	773	1	general description	keyword	1	0	772
2568	773	3	general_description	keyword	1	0	774
2569	774	1	general description	keyword	1	0	773
2570	774	2	general_description	keyword	1	0	775
2571	775	0	general description	keyword	1	0	774
2572	775	2	general_description	keyword	1	0	776
2573	776	0	general description	keyword	1	0	775
2574	776	2	general_description	keyword	1	0	777
2575	777	0	general description	keyword	1	0	776
2576	777	2	general_description	keyword	1	0	778
2577	778	0	general description	keyword	1	0	777
2578	778	2	general_description	keyword	1	0	779
2579	779	0	general description	keyword	1	0	778
2580	779	3	general_description	keyword	1	0	780
2581	780	1	general description	keyword	1	0	779
2582	780	3	general_description	keyword	1	0	781
2583	781	1	general description	keyword	1	0	780
2584	781	3	general_description	keyword	1	0	782
2585	782	1	general description	keyword	1	0	781
2586	782	3	general_description	keyword	1	0	783
2587	783	1	general description	keyword	1	0	782
2588	783	3	general_description	keyword	1	0	784
2589	784	1	general description	keyword	1	0	783
2590	785	0	general description	keyword	1	0	754
2591	785	2	general_description	keyword	1	0	786
2592	786	0	general description	keyword	1	0	785
2593	786	2	general_description	keyword	1	0	787
2594	787	0	general description	keyword	1	0	786
2595	787	2	general_description	keyword	1	0	788
2596	788	0	general description	keyword	1	0	787
2597	788	2	general_description	keyword	1	0	789
2598	789	0	general description	keyword	1	0	788
2599	789	2	general_description	keyword	1	0	790
2600	790	0	general description	keyword	1	0	789
2601	790	2	general_description	keyword	1	0	791
2602	791	0	general description	keyword	1	0	790
2603	791	2	general_description	keyword	1	0	792
2604	792	0	general description	keyword	1	0	791
2605	792	2	general_description	keyword	1	0	793
2606	793	0	general description	keyword	1	0	792
2607	793	2	general_description	keyword	1	0	794
2608	794	0	general description	keyword	1	0	793
2609	794	2	general_description	keyword	1	0	795
2610	795	0	general description	keyword	1	0	794
2611	795	2	general_description	keyword	1	0	796
2612	796	0	general description	keyword	1	0	795
2613	796	2	general_description	keyword	1	0	797
2614	797	0	general description	keyword	1	0	796
2615	797	2	general_description	keyword	1	0	798
2616	798	0	general description	keyword	1	0	797
2617	798	2	general_description	keyword	1	0	799
2618	799	0	general description	keyword	1	0	798
2619	799	2	general_description	keyword	1	0	800
2620	800	0	general description	keyword	1	0	799
2621	800	2	general_description	keyword	1	0	801
2622	801	0	general description	keyword	1	0	800
2623	801	2	general_description	keyword	1	0	802
2624	802	0	general description	keyword	1	0	801
2625	802	2	general_description	keyword	1	0	803
2626	803	0	general description	keyword	1	0	802
2627	803	2	general_description	keyword	1	0	804
2628	804	0	general description	keyword	1	0	803
2629	804	2	general_description	keyword	1	0	805
2630	805	0	general description	keyword	1	0	804
2631	805	2	general_description	keyword	1	0	806
2632	806	0	general description	keyword	1	0	805
2633	806	2	general_description	keyword	1	0	807
2634	807	0	general description	keyword	1	0	806
2635	807	2	general_description	keyword	1	0	808
2636	808	0	general description	keyword	1	0	807
2637	808	2	general_description	keyword	1	0	809
2638	809	0	general description	keyword	1	0	808
2639	809	2	general_description	keyword	1	0	810
2640	810	0	general description	keyword	1	0	809
2641	810	2	general_description	keyword	1	0	811
2642	811	0	general description	keyword	1	0	810
2643	811	2	general_description	keyword	1	0	812
2644	812	0	general description	keyword	1	0	811
2645	812	2	general_description	keyword	1	0	813
2646	813	0	general description	keyword	1	0	812
2647	813	2	general_description	keyword	1	0	814
2648	814	0	general description	keyword	1	0	813
2649	814	2	general_description	keyword	1	0	815
2650	815	0	general description	keyword	1	0	814
\.


--
-- Data for Name: room_extra_descriptions; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.room_extra_descriptions (id, red_room_vnum, red_keyword, red_description) FROM stdin;
\.


--
-- Data for Name: scripted_sequences; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.scripted_sequences (id, s_sequence_vnum) FROM stdin;
21	1
23	2
\.


--
-- Data for Name: scripted_steps; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.scripted_steps (id, s_sequence_vnum, s_wait_ticks, s_mob, s_obj, s_room, s_quantity, s_order, s_interpret, s_yaml, s_type, s_dialogue) FROM stdin;
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
-- Data for Name: skill_points; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.skill_points (id, sp_level, sp_points, created_at, updated_at) FROM stdin;
1	1	1	2021-09-04 04:12:07.161588	2021-09-04 04:12:07.161588
2	2	1	2021-09-04 04:12:07.162076	2021-09-04 04:12:07.162076
3	3	1	2021-09-04 04:12:07.162457	2021-09-04 04:12:07.162457
4	4	1	2021-09-04 04:12:07.162818	2021-09-04 04:12:07.162818
5	5	1	2021-09-04 04:12:07.163205	2021-09-04 04:12:07.163205
6	6	1	2021-09-04 04:12:07.163581	2021-09-04 04:12:07.163581
7	7	1	2021-09-04 04:12:07.163933	2021-09-04 04:12:07.163933
8	8	1	2021-09-04 04:12:07.164266	2021-09-04 04:12:07.164266
9	9	1	2021-09-04 04:12:07.164627	2021-09-04 04:12:07.164627
10	10	1	2021-09-04 04:12:07.165068	2021-09-04 04:12:07.165068
11	11	1	2021-09-04 04:12:07.165411	2021-09-04 04:12:07.165411
12	12	1	2021-09-04 04:12:07.165783	2021-09-04 04:12:07.165783
13	13	1	2021-09-04 04:12:07.166157	2021-09-04 04:12:07.166157
14	14	1	2021-09-04 04:12:07.166533	2021-09-04 04:12:07.166533
15	15	1	2021-09-04 04:12:07.166887	2021-09-04 04:12:07.166887
16	16	1	2021-09-04 04:12:07.16723	2021-09-04 04:12:07.16723
17	17	1	2021-09-04 04:12:07.167578	2021-09-04 04:12:07.167578
18	18	1	2021-09-04 04:12:07.167912	2021-09-04 04:12:07.167912
19	19	1	2021-09-04 04:12:07.168322	2021-09-04 04:12:07.168322
20	20	1	2021-09-04 04:12:07.168692	2021-09-04 04:12:07.168692
21	21	1	2021-09-04 04:12:07.169065	2021-09-04 04:12:07.169065
22	22	1	2021-09-04 04:12:07.169414	2021-09-04 04:12:07.169414
23	23	1	2021-09-04 04:12:07.169779	2021-09-04 04:12:07.169779
24	24	1	2021-09-04 04:12:07.170146	2021-09-04 04:12:07.170146
25	25	1	2021-09-04 04:12:07.170532	2021-09-04 04:12:07.170532
26	26	1	2021-09-04 04:12:07.170887	2021-09-04 04:12:07.170887
27	27	1	2021-09-04 04:12:07.1713	2021-09-04 04:12:07.1713
28	28	1	2021-09-04 04:12:07.171643	2021-09-04 04:12:07.171643
29	29	1	2021-09-04 04:12:07.172013	2021-09-04 04:12:07.172013
30	30	1	2021-09-04 04:12:07.172343	2021-09-04 04:12:07.172343
\.


--
-- Data for Name: skill_trees; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.skill_trees (id, skill_name, skill_category, skill_player_class, skill_parent_id, skill_order, skill_description, created_at, updated_at) FROM stdin;
1	intel:1-tracking-shot	Technology	sniper	\N	0	\N	2021-09-04 04:12:07.176871	2021-09-04 04:12:07.176871
2	intel:2-sensor-grenade	Technology	sniper	1	0	\N	2021-09-04 04:12:07.177414	2021-09-04 04:12:07.177414
3	intel:3-xray-shot	Technology	sniper	2	1	\N	2021-09-04 04:12:07.177917	2021-09-04 04:12:07.177917
4	intel:4-area-scan	Technology	sniper	\N	2	\N	2021-09-04 04:12:07.178419	2021-09-04 04:12:07.178419
5	disrupt:1-chaff-grenade	Technology	sniper	\N	0	\N	2021-09-04 04:12:07.178899	2021-09-04 04:12:07.178899
6	disrupt:2-emp-grenade	Technology	sniper	5	1	\N	2021-09-04 04:12:07.179282	2021-09-04 04:12:07.179282
7	claymore:1-plant	Demolitions	sniper	\N	0	\N	2021-09-04 04:12:07.17979	2021-09-04 04:12:07.17979
8	claymore:2-shrapnel	Demolitions	sniper	7	1	\N	2021-09-04 04:12:07.180228	2021-09-04 04:12:07.180228
9	claymore:3-corrosive	Demolitions	sniper	8	1	\N	2021-09-04 04:12:07.180698	2021-09-04 04:12:07.180698
10	guided-missile	Demolitions	sniper	\N	1	\N	2021-09-04 04:12:07.181205	2021-09-04 04:12:07.181205
11	light-bandage	Medical	sniper	\N	1	\N	2021-09-04 04:12:07.181575	2021-09-04 04:12:07.181575
12	suture	Medical	sniper	\N	1	\N	2021-09-04 04:12:07.181958	2021-09-04 04:12:07.181958
13	adrenaline-shot	Medical	sniper	\N	1	\N	2021-09-04 04:12:07.182341	2021-09-04 04:12:07.182341
14	underbarrel-grenade-launcher	Sniping	sniper	\N	1	\N	2021-09-04 04:12:07.182739	2021-09-04 04:12:07.182739
15	underbarrel-shotgun	Sniping	sniper	\N	1	\N	2021-09-04 04:12:07.183109	2021-09-04 04:12:07.183109
16	target-limb	Sniping	sniper	\N	1	\N	2021-09-04 04:12:07.1835	2021-09-04 04:12:07.1835
\.


--
-- Data for Name: skill_usage; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.skill_usage (id, player_id, skill_name, skill_level) FROM stdin;
1	1	ads	0
2	1	stealth	0
3	1	summon	0
4	1	xray	0
5	1	feign	0
6	1	claymore	0
7	1	intimidation	0
8	1	cryo	0
9	1	flash	0
10	1	ts	0
11	1	lb	0
12	1	suture	0
13	1	as	0
14	1	emp	0
15	1	chaff	0
16	1	sensor	0
17	1	ubs	0
18	1	ubf	0
19	1	gm	0
20	1	limb	0
21	1	smine	0
22	1	cmine	0
23	1	recon	0
309	110	ads	0
310	110	stealth	0
311	110	summon	0
312	110	xray	0
313	110	feign	0
314	110	claymore	0
315	110	intimidation	0
316	110	cryo	0
317	110	flash	0
318	110	ts	0
319	110	lb	0
320	110	suture	0
321	110	as	0
322	110	emp	0
323	110	chaff	0
324	110	sensor	0
325	110	ubs	0
326	110	ubf	0
327	110	gm	0
328	110	limb	0
329	110	smine	0
330	110	cmine	0
331	110	recon	0
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
-- Data for Name: weapon_locker; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.weapon_locker (id, w_room_vnum, w_yaml) FROM stdin;
\.


--
-- Data for Name: world_configuration_start_rooms; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.world_configuration_start_rooms (id, mortal_start_room, immortal_start_room, created_at, is_active, idle_room, frozen_room) FROM stdin;
4	128	128	2019-06-01 19:30:07.630823	t	45 46
\.


--
-- Data for Name: zone; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.zone (id, zone_virtual_number, zone_start, zone_end, zone_name, lifespan, reset_mode) FROM stdin;
1	1	128	228	Hereford Base	10	0
\.


--
-- Data for Name: zone_data; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.zone_data (id, zone_id, zone_command, zone_if_flag, zone_arg1, zone_arg2, zone_arg3, zone_yaml) FROM stdin;
828	1	M	0	600	143	1	\N
829	1	M	0	601	147	8	\N
830	1	M	0	602	143	1	\N
831	1	M	0	603	266	1	\N
832	1	M	0	500	131	10	\N
833	1	M	0	501	131	10	\N
834	1	M	0	502	212	2	\N
835	1	M	0	502	207	2	\N
836	1	M	0	502	204	2	\N
837	1	M	0	502	340	2	\N
838	1	M	0	502	199	2	\N
839	1	M	0	502	279	2	\N
840	1	M	0	502	139	2	\N
841	1	Y	0	0	402	1	#yaml|vehicle/p3-hunchbak.yml
842	1	Y	0	0	400	1	#yaml|vehicle/p3-offroad-mx3.yml
843	1	Y	0	0	399	1	#yaml|vehicle/prime-town-suv.yml
844	1	Y	0	0	395	1	#yaml|vehicle/lxr-sunrise.yml
845	1	Y	0	0	393	1	#yaml|vehicle/lxr-sport.yml
846	1	Y	0	0	394	1	#yaml|vehicle/lxr-sport.yml
847	1	M	0	100	393	2	\N
848	1	M	0	100	394	2	\N
849	1	M	0	100	395	2	\N
850	1	M	0	100	396	2	\N
851	1	M	0	100	398	2	\N
852	1	M	0	100	400	2	\N
853	1	M	0	100	399	2	\N
854	1	M	0	100	401	2	\N
855	1	M	0	100	402	2	\N
856	1	M	0	101	316	5	\N
857	1	M	0	101	317	5	\N
858	1	M	0	101	322	5	\N
859	1	M	0	101	323	5	\N
860	1	M	0	101	324	5	\N
861	1	M	0	101	325	5	\N
862	1	M	0	101	306	5	\N
863	1	M	0	101	308	5	\N
864	1	M	0	102	290	2	\N
865	1	M	0	102	291	2	\N
866	1	M	0	102	292	2	\N
867	1	M	0	102	294	2	\N
868	1	M	0	102	295	2	\N
869	1	M	0	102	331	2	\N
870	1	M	0	102	298	2	\N
871	1	M	0	102	303	2	\N
872	1	M	0	102	310	2	\N
873	1	M	0	102	317	2	\N
874	1	M	0	102	324	2	\N
875	1	M	0	103	407	1	\N
876	1	M	0	103	409	1	\N
877	1	M	0	103	413	1	\N
878	1	M	0	103	282	1	\N
879	1	M	0	103	284	1	\N
880	1	M	0	103	286	1	\N
881	1	M	0	103	292	1	\N
882	1	M	0	103	293	1	\N
883	1	R	0	575	1	0	melee/improvised-shank.yml|melee/screwdriver.yml|
884	1	R	0	576	1	0	melee/improvised-shank.yml|melee/screwdriver.yml|
885	1	R	0	577	1	0	melee/improvised-shank.yml|melee/screwdriver.yml|
886	1	R	0	578	1	0	melee/improvised-shank.yml|melee/screwdriver.yml|
887	1	R	0	579	1	0	melee/improvised-shank.yml|melee/screwdriver.yml|
888	1	R	0	580	1	0	melee/improvised-shank.yml|melee/screwdriver.yml|
889	1	R	0	581	1	0	melee/improvised-shank.yml|melee/screwdriver.yml|
890	1	R	0	582	1	0	melee/improvised-shank.yml|melee/screwdriver.yml|
891	1	R	0	583	1	0	melee/improvised-shank.yml|melee/screwdriver.yml|
892	1	R	0	584	1	0	melee/improvised-shank.yml|melee/screwdriver.yml|
893	1	R	0	585	1	0	melee/improvised-shank.yml|melee/screwdriver.yml|
894	1	R	0	586	1	0	melee/improvised-shank.yml|melee/screwdriver.yml|
895	1	R	0	587	1	0	melee/improvised-shank.yml|melee/screwdriver.yml|
896	1	R	0	588	1	0	melee/improvised-shank.yml|melee/screwdriver.yml|
897	1	R	0	589	1	0	melee/improvised-shank.yml|melee/screwdriver.yml|
898	1	R	0	590	1	0	melee/improvised-shank.yml|melee/screwdriver.yml|
899	1	R	0	591	1	0	melee/improvised-shank.yml|melee/screwdriver.yml|
900	1	R	0	592	1	0	melee/improvised-shank.yml|melee/screwdriver.yml|
901	1	R	0	593	1	0	melee/improvised-shank.yml|melee/screwdriver.yml|
902	1	R	0	594	1	0	melee/improvised-shank.yml|melee/screwdriver.yml|
903	1	R	0	595	1	0	melee/improvised-shank.yml|melee/screwdriver.yml|
904	1	R	0	596	1	0	melee/improvised-shank.yml|melee/screwdriver.yml|
905	1	R	0	597	1	0	melee/improvised-shank.yml|melee/screwdriver.yml|
906	1	R	0	598	1	0	melee/improvised-shank.yml|melee/screwdriver.yml|
907	1	R	0	599	1	0	melee/improvised-shank.yml|melee/screwdriver.yml|
908	1	R	0	600	1	0	melee/improvised-shank.yml|melee/screwdriver.yml|
909	1	R	0	601	1	0	melee/improvised-shank.yml|melee/screwdriver.yml|
910	1	R	0	602	1	0	melee/improvised-shank.yml|melee/screwdriver.yml|
911	1	R	0	603	1	0	melee/improvised-shank.yml|melee/screwdriver.yml|
912	1	R	0	604	1	0	melee/improvised-shank.yml|melee/screwdriver.yml|
913	1	R	0	605	1	0	melee/improvised-shank.yml|melee/screwdriver.yml|
914	1	R	0	606	1	0	melee/improvised-shank.yml|melee/screwdriver.yml|
915	1	R	0	607	1	0	melee/improvised-shank.yml|melee/screwdriver.yml|
916	1	R	0	608	1	0	melee/improvised-shank.yml|melee/screwdriver.yml|
917	1	R	0	609	1	0	melee/improvised-shank.yml|melee/screwdriver.yml|
918	1	R	0	610	1	0	melee/improvised-shank.yml|melee/screwdriver.yml|
919	1	R	0	611	1	0	melee/improvised-shank.yml|melee/screwdriver.yml|
920	1	R	0	612	1	0	melee/improvised-shank.yml|melee/screwdriver.yml|
921	1	R	0	613	1	0	melee/improvised-shank.yml|melee/screwdriver.yml|
922	1	R	0	614	1	0	melee/improvised-shank.yml|melee/screwdriver.yml|
923	1	R	0	615	1	0	melee/improvised-shank.yml|melee/screwdriver.yml|
924	1	R	0	616	1	0	melee/improvised-shank.yml|melee/screwdriver.yml|
925	1	R	0	617	1	0	melee/improvised-shank.yml|melee/screwdriver.yml|
926	1	R	0	618	1	0	melee/improvised-shank.yml|melee/screwdriver.yml|
927	1	R	0	619	1	0	melee/improvised-shank.yml|melee/screwdriver.yml|
928	1	R	0	620	1	0	melee/improvised-shank.yml|melee/screwdriver.yml|
929	1	R	0	621	1	0	melee/improvised-shank.yml|melee/screwdriver.yml|
930	1	R	0	622	1	0	melee/improvised-shank.yml|melee/screwdriver.yml|
931	1	R	0	623	1	0	melee/improvised-shank.yml|melee/screwdriver.yml|
932	1	R	0	624	1	0	melee/improvised-shank.yml|melee/screwdriver.yml|
933	1	R	0	625	1	0	melee/improvised-shank.yml|melee/screwdriver.yml|
934	1	R	0	626	1	0	melee/improvised-shank.yml|melee/screwdriver.yml|
935	1	R	0	627	1	0	melee/improvised-shank.yml|melee/screwdriver.yml|
936	1	R	0	628	1	0	melee/improvised-shank.yml|melee/screwdriver.yml|
937	1	R	0	629	1	0	melee/improvised-shank.yml|melee/screwdriver.yml|
938	1	R	0	630	1	0	melee/improvised-shank.yml|melee/screwdriver.yml|
939	1	R	0	631	1	0	melee/improvised-shank.yml|melee/screwdriver.yml|
940	1	R	0	632	1	0	melee/improvised-shank.yml|melee/screwdriver.yml|
941	1	R	0	633	1	0	melee/improvised-shank.yml|melee/screwdriver.yml|
942	1	R	0	634	1	0	melee/improvised-shank.yml|melee/screwdriver.yml|
943	1	R	0	635	1	0	melee/improvised-shank.yml|melee/screwdriver.yml|
944	1	R	0	636	1	0	melee/improvised-shank.yml|melee/screwdriver.yml|
945	1	M	0	109	578	16	\N
946	1	M	0	109	589	16	\N
947	1	M	0	109	600	16	\N
948	1	M	0	109	611	16	\N
949	1	M	0	109	622	16	\N
950	1	M	0	109	592	16	\N
951	1	M	0	109	614	16	\N
952	1	M	0	109	625	16	\N
953	1	M	0	109	636	16	\N
954	1	M	0	110	578	16	\N
955	1	M	0	110	589	16	\N
956	1	M	0	110	600	16	\N
957	1	M	0	110	611	16	\N
958	1	M	0	110	622	16	\N
959	1	M	0	110	592	16	\N
960	1	M	0	110	614	16	\N
961	1	M	0	110	625	16	\N
962	1	M	0	110	636	16	\N
963	1	M	0	106	729	1	\N
964	1	M	0	106	730	1	\N
965	1	M	0	106	731	1	\N
966	1	M	0	106	732	1	\N
967	1	M	0	107	729	1	\N
968	1	M	0	107	730	1	\N
969	1	M	0	107	731	1	\N
970	1	M	0	107	732	1	\N
\.


--
-- Name: affected_type_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.affected_type_id_seq', 1, false);


--
-- Name: armor_index_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.armor_index_id_seq', 25, true);


--
-- Name: armor_locker_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.armor_locker_id_seq', 1, false);


--
-- Name: camera_feed_feed_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.camera_feed_feed_id_seq', 1, false);


--
-- Name: class_breacher_breacher_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.class_breacher_breacher_id_seq', 1, true);


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

SELECT pg_catalog.setval('public.class_ghost_ghost_id_seq', 12, true);


--
-- Name: class_marine_marine_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.class_marine_marine_id_seq', 2, true);


--
-- Name: class_medic_medic_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.class_medic_medic_id_seq', 1, false);


--
-- Name: class_sniper_sniper_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.class_sniper_sniper_id_seq', 1, true);


--
-- Name: class_support_support_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.class_support_support_id_seq', 1, false);


--
-- Name: computer_terminal_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.computer_terminal_id_seq', 1, true);


--
-- Name: contract_step_callback_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.contract_step_callback_id_seq', 12, true);


--
-- Name: contract_steps_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.contract_steps_id_seq', 30, true);


--
-- Name: contracts_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.contracts_id_seq', 12, true);


--
-- Name: event_messages_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.event_messages_id_seq', 1, false);


--
-- Name: extra_description_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.extra_description_id_seq', 10, true);


--
-- Name: friendly_reminders_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.friendly_reminders_id_seq', 1, false);


--
-- Name: hq_locations_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.hq_locations_id_seq', 1, false);


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
-- Name: mob_equipment_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.mob_equipment_id_seq', 114, true);


--
-- Name: mob_equipment_map_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.mob_equipment_map_id_seq', 39, true);


--
-- Name: mob_roam_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.mob_roam_id_seq', 763, true);


--
-- Name: mob_zone_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.mob_zone_id_seq', 1, false);


--
-- Name: mobile_mob_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.mobile_mob_id_seq', 29, true);


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
-- Name: player_base_ability_pba_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.player_base_ability_pba_id_seq', 16, true);


--
-- Name: player_classes_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.player_classes_id_seq', 1, false);


--
-- Name: player_contract_state_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.player_contract_state_id_seq', 1, false);


--
-- Name: player_flags_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.player_flags_id_seq', 1, false);


--
-- Name: player_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.player_id_seq', 110, true);


--
-- Name: player_object_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.player_object_id_seq', 385, true);


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
-- Name: player_skill_points_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.player_skill_points_id_seq', 1, false);


--
-- Name: player_skill_usage_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.player_skill_usage_id_seq', 1, false);


--
-- Name: rifle_attachment_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.rifle_attachment_id_seq', 4890, true);


--
-- Name: rifle_index_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.rifle_index_id_seq', 29, true);


--
-- Name: rifle_instance_rifle_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.rifle_instance_rifle_id_seq', 174, true);


--
-- Name: rifle_placements_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.rifle_placements_id_seq', 1, false);


--
-- Name: room_direction_data_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.room_direction_data_id_seq', 2650, true);


--
-- Name: room_extra_descriptions_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.room_extra_descriptions_id_seq', 1, false);


--
-- Name: room_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.room_id_seq', 685, true);


--
-- Name: room_virtual_number_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.room_virtual_number_seq', 1, false);


--
-- Name: scripted_sequences_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.scripted_sequences_id_seq', 24, true);


--
-- Name: scripted_steps_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.scripted_steps_id_seq', 84, true);


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
-- Name: skill_points_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.skill_points_id_seq', 30, true);


--
-- Name: skill_trees_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.skill_trees_id_seq', 16, true);


--
-- Name: skill_usage_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.skill_usage_id_seq', 331, true);


--
-- Name: terminal_choices_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.terminal_choices_id_seq', 6, true);


--
-- Name: weapon_locker_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.weapon_locker_id_seq', 1, false);


--
-- Name: world_configuration_start_rooms_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.world_configuration_start_rooms_id_seq', 4, true);


--
-- Name: zone_data_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.zone_data_id_seq', 970, true);


--
-- Name: zone_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.zone_id_seq', 151, true);


--
-- Name: armor_index armor_index_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.armor_index
    ADD CONSTRAINT armor_index_pkey PRIMARY KEY (id);


--
-- Name: armor_locker armor_locker_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.armor_locker
    ADD CONSTRAINT armor_locker_pkey PRIMARY KEY (id);


--
-- Name: camera_feed camera_feed_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.camera_feed
    ADD CONSTRAINT camera_feed_pkey PRIMARY KEY (feed_id);


--
-- Name: class_breacher class_breacher_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.class_breacher
    ADD CONSTRAINT class_breacher_pkey PRIMARY KEY (breacher_id);


--
-- Name: class_contagion class_contagion_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.class_contagion
    ADD CONSTRAINT class_contagion_pkey PRIMARY KEY (contagion_id);


--
-- Name: class_engineer class_engineer_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.class_engineer
    ADD CONSTRAINT class_engineer_pkey PRIMARY KEY (engineer_id);


--
-- Name: class_ghost class_ghost_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.class_ghost
    ADD CONSTRAINT class_ghost_pkey PRIMARY KEY (ghost_id);


--
-- Name: class_marine class_marine_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.class_marine
    ADD CONSTRAINT class_marine_pkey PRIMARY KEY (marine_id);


--
-- Name: class_medic class_medic_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.class_medic
    ADD CONSTRAINT class_medic_pkey PRIMARY KEY (medic_id);


--
-- Name: class_sniper class_sniper_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.class_sniper
    ADD CONSTRAINT class_sniper_pkey PRIMARY KEY (sniper_id);


--
-- Name: class_support class_support_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.class_support
    ADD CONSTRAINT class_support_pkey PRIMARY KEY (support_id);


--
-- Name: computer_terminal computer_terminal_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.computer_terminal
    ADD CONSTRAINT computer_terminal_pkey PRIMARY KEY (id);


--
-- Name: contract_step_callback contract_step_callback_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.contract_step_callback
    ADD CONSTRAINT contract_step_callback_pkey PRIMARY KEY (id);


--
-- Name: contract_steps contract_steps_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.contract_steps
    ADD CONSTRAINT contract_steps_pkey PRIMARY KEY (id);


--
-- Name: contracts contracts_c_vnum_key; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.contracts
    ADD CONSTRAINT contracts_c_vnum_key UNIQUE (c_vnum);


--
-- Name: contracts contracts_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.contracts
    ADD CONSTRAINT contracts_pkey PRIMARY KEY (id);


--
-- Name: event_messages event_messages_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.event_messages
    ADD CONSTRAINT event_messages_pkey PRIMARY KEY (id);


--
-- Name: extra_description extra_description_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.extra_description
    ADD CONSTRAINT extra_description_pkey PRIMARY KEY (id);


--
-- Name: friendly_reminders friendly_reminders_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.friendly_reminders
    ADD CONSTRAINT friendly_reminders_pkey PRIMARY KEY (id);


--
-- Name: hq_locations hq_locations_hq_room_vnum_key; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.hq_locations
    ADD CONSTRAINT hq_locations_hq_room_vnum_key UNIQUE (hq_room_vnum);


--
-- Name: hq_locations hq_locations_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.hq_locations
    ADD CONSTRAINT hq_locations_pkey PRIMARY KEY (id);


--
-- Name: integral_object integral_object_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.integral_object
    ADD CONSTRAINT integral_object_pkey PRIMARY KEY (object_id);


--
-- Name: karma karma_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.karma
    ADD CONSTRAINT karma_pkey PRIMARY KEY (karma_id);


--
-- Name: mini_game mini_game_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.mini_game
    ADD CONSTRAINT mini_game_pkey PRIMARY KEY (game_id);


--
-- Name: mini_gunner_sentinel mini_gunner_sentinel_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.mini_gunner_sentinel
    ADD CONSTRAINT mini_gunner_sentinel_pkey PRIMARY KEY (id);


--
-- Name: mob_equipment_map mob_equipment_map_mmap_mob_vnum_key; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.mob_equipment_map
    ADD CONSTRAINT mob_equipment_map_mmap_mob_vnum_key UNIQUE (mmap_mob_vnum);


--
-- Name: mob_equipment_map mob_equipment_map_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.mob_equipment_map
    ADD CONSTRAINT mob_equipment_map_pkey PRIMARY KEY (id);


--
-- Name: mob_equipment mob_equipment_meq_vnum_key; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.mob_equipment
    ADD CONSTRAINT mob_equipment_meq_vnum_key UNIQUE (meq_vnum);


--
-- Name: mob_equipment mob_equipment_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.mob_equipment
    ADD CONSTRAINT mob_equipment_pkey PRIMARY KEY (id);


--
-- Name: mob_roam mob_roam_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.mob_roam
    ADD CONSTRAINT mob_roam_pkey PRIMARY KEY (id);


--
-- Name: mob_zone mob_zone_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.mob_zone
    ADD CONSTRAINT mob_zone_pkey PRIMARY KEY (id);


--
-- Name: mobile mobile_mob_id_key; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.mobile
    ADD CONSTRAINT mobile_mob_id_key UNIQUE (mob_id);


--
-- Name: mobile mobile_mob_virtual_number_key; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.mobile
    ADD CONSTRAINT mobile_mob_virtual_number_key UNIQUE (mob_virtual_number);


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
-- Name: player_contract_state player_contract_state_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.player_contract_state
    ADD CONSTRAINT player_contract_state_pkey PRIMARY KEY (id);


--
-- Name: player_flags player_flags_player_id_chunk_index_key; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.player_flags
    ADD CONSTRAINT player_flags_player_id_chunk_index_key UNIQUE (player_id, chunk_index);


--
-- Name: player_object player_object_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.player_object
    ADD CONSTRAINT player_object_pkey PRIMARY KEY (id);


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
-- Name: player_skill_points player_skill_points_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.player_skill_points
    ADD CONSTRAINT player_skill_points_pkey PRIMARY KEY (id);


--
-- Name: player_skill_usage player_skill_usage_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.player_skill_usage
    ADD CONSTRAINT player_skill_usage_pkey PRIMARY KEY (id);


--
-- Name: rifle_attachment rifle_attachment_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.rifle_attachment
    ADD CONSTRAINT rifle_attachment_pkey PRIMARY KEY (id);


--
-- Name: rifle_index rifle_index_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.rifle_index
    ADD CONSTRAINT rifle_index_pkey PRIMARY KEY (id);


--
-- Name: rifle_instance rifle_instance_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.rifle_instance
    ADD CONSTRAINT rifle_instance_pkey PRIMARY KEY (rifle_id);


--
-- Name: rifle_placements rifle_placements_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.rifle_placements
    ADD CONSTRAINT rifle_placements_pkey PRIMARY KEY (id);


--
-- Name: room_direction_data room_direction_data_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.room_direction_data
    ADD CONSTRAINT room_direction_data_pkey PRIMARY KEY (id);


--
-- Name: room_extra_descriptions room_extra_descriptions_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.room_extra_descriptions
    ADD CONSTRAINT room_extra_descriptions_pkey PRIMARY KEY (id);


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
-- Name: scripted_sequences scripted_sequences_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.scripted_sequences
    ADD CONSTRAINT scripted_sequences_pkey PRIMARY KEY (id);


--
-- Name: scripted_sequences scripted_sequences_s_sequence_vnum_key; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.scripted_sequences
    ADD CONSTRAINT scripted_sequences_s_sequence_vnum_key UNIQUE (s_sequence_vnum);


--
-- Name: scripted_steps scripted_steps_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.scripted_steps
    ADD CONSTRAINT scripted_steps_pkey PRIMARY KEY (id);


--
-- Name: shop_rooms shop_rooms_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.shop_rooms
    ADD CONSTRAINT shop_rooms_pkey PRIMARY KEY (shop_rooms_id);


--
-- Name: skill_points skill_points_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.skill_points
    ADD CONSTRAINT skill_points_pkey PRIMARY KEY (id);


--
-- Name: skill_trees skill_trees_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.skill_trees
    ADD CONSTRAINT skill_trees_pkey PRIMARY KEY (id);


--
-- Name: skill_trees skill_trees_skill_player_class_skill_category_skill_name_key; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.skill_trees
    ADD CONSTRAINT skill_trees_skill_player_class_skill_category_skill_name_key UNIQUE (skill_player_class, skill_category, skill_name);


--
-- Name: skill_usage skill_usage_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.skill_usage
    ADD CONSTRAINT skill_usage_pkey PRIMARY KEY (id);


--
-- Name: terminal_choices terminal_choices_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.terminal_choices
    ADD CONSTRAINT terminal_choices_pkey PRIMARY KEY (id);


--
-- Name: weapon_locker weapon_locker_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.weapon_locker
    ADD CONSTRAINT weapon_locker_pkey PRIMARY KEY (id);


--
-- Name: zone zone_id_key; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.zone
    ADD CONSTRAINT zone_id_key UNIQUE (id);


--
-- Name: zone zone_zone_virtual_number_key; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.zone
    ADD CONSTRAINT zone_zone_virtual_number_key UNIQUE (zone_virtual_number);


--
-- Name: contract_steps fk_contract_vnum; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.contract_steps
    ADD CONSTRAINT fk_contract_vnum FOREIGN KEY (s_contract_vnum) REFERENCES public.contracts(c_vnum) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- Name: contract_step_callback fk_contract_vnum; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.contract_step_callback
    ADD CONSTRAINT fk_contract_vnum FOREIGN KEY (s_contract_vnum) REFERENCES public.contracts(c_vnum) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- Name: player_contract_state fk_cvnum; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.player_contract_state
    ADD CONSTRAINT fk_cvnum FOREIGN KEY (pc_contract_vnum) REFERENCES public.contracts(c_vnum) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- Name: mob_equipment_map fk_meq_vnum; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.mob_equipment_map
    ADD CONSTRAINT fk_meq_vnum FOREIGN KEY (mmap_mob_equipment_vnum) REFERENCES public.mob_equipment(meq_vnum) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- Name: mob_zone fk_mob_virtual_number; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.mob_zone
    ADD CONSTRAINT fk_mob_virtual_number FOREIGN KEY (mob_virtual_number) REFERENCES public.mobile(mob_virtual_number) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- Name: mob_roam fk_mob_virtual_number; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.mob_roam
    ADD CONSTRAINT fk_mob_virtual_number FOREIGN KEY (mob_virtual_number) REFERENCES public.mobile(mob_virtual_number) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- Name: mini_gunner_sentinel fk_mob_vnum; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.mini_gunner_sentinel
    ADD CONSTRAINT fk_mob_vnum FOREIGN KEY (mgs_mob_vnum) REFERENCES public.mobile(mob_id) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- Name: mob_equipment_map fk_mob_vnum; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.mob_equipment_map
    ADD CONSTRAINT fk_mob_vnum FOREIGN KEY (mmap_mob_vnum) REFERENCES public.mobile(mob_virtual_number) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- Name: skill_trees fk_parent_id; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.skill_trees
    ADD CONSTRAINT fk_parent_id FOREIGN KEY (skill_parent_id) REFERENCES public.skill_trees(id) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- Name: karma fk_player_id; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.karma
    ADD CONSTRAINT fk_player_id FOREIGN KEY (karma_player_id) REFERENCES public.player(id) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- Name: class_marine fk_player_id; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.class_marine
    ADD CONSTRAINT fk_player_id FOREIGN KEY (marine_player_id) REFERENCES public.player(id) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- Name: class_breacher fk_player_id; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.class_breacher
    ADD CONSTRAINT fk_player_id FOREIGN KEY (breacher_player_id) REFERENCES public.player(id) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- Name: class_engineer fk_player_id; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.class_engineer
    ADD CONSTRAINT fk_player_id FOREIGN KEY (engineer_player_id) REFERENCES public.player(id) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- Name: class_medic fk_player_id; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.class_medic
    ADD CONSTRAINT fk_player_id FOREIGN KEY (medic_player_id) REFERENCES public.player(id) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- Name: class_support fk_player_id; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.class_support
    ADD CONSTRAINT fk_player_id FOREIGN KEY (support_player_id) REFERENCES public.player(id) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- Name: class_ghost fk_player_id; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.class_ghost
    ADD CONSTRAINT fk_player_id FOREIGN KEY (ghost_player_id) REFERENCES public.player(id) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- Name: class_sniper fk_player_id; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.class_sniper
    ADD CONSTRAINT fk_player_id FOREIGN KEY (sniper_player_id) REFERENCES public.player(id) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- Name: player_skill_usage fk_player_id; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.player_skill_usage
    ADD CONSTRAINT fk_player_id FOREIGN KEY (ps_player_id) REFERENCES public.player(id) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- Name: player_skill_points fk_player_id; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.player_skill_points
    ADD CONSTRAINT fk_player_id FOREIGN KEY (ps_player_id) REFERENCES public.player(id) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- Name: player_contract_state fk_player_id; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.player_contract_state
    ADD CONSTRAINT fk_player_id FOREIGN KEY (pc_player_id) REFERENCES public.player(id) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- Name: rifle_attachment fk_player_id; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.rifle_attachment
    ADD CONSTRAINT fk_player_id FOREIGN KEY (rifle_player_id) REFERENCES public.player(id) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- Name: player_object fk_player_id; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.player_object
    ADD CONSTRAINT fk_player_id FOREIGN KEY (po_player_id) REFERENCES public.player(id) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- Name: skill_usage fk_player_id; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.skill_usage
    ADD CONSTRAINT fk_player_id FOREIGN KEY (player_id) REFERENCES public.player(id) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- Name: class_contagion fk_player_id; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.class_contagion
    ADD CONSTRAINT fk_player_id FOREIGN KEY (contagion_player_id) REFERENCES public.player(id) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- Name: rifle_placements fk_rifle_id; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.rifle_placements
    ADD CONSTRAINT fk_rifle_id FOREIGN KEY (ip_rifle_id) REFERENCES public.rifle_instance(rifle_id) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- Name: mob_zone fk_room_virtual_number; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.mob_zone
    ADD CONSTRAINT fk_room_virtual_number FOREIGN KEY (room_virtual_number) REFERENCES public.room(room_number) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- Name: mob_roam fk_room_virtual_number; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.mob_roam
    ADD CONSTRAINT fk_room_virtual_number FOREIGN KEY (room_virtual_number) REFERENCES public.room(room_number) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- Name: room_direction_data fk_room_vnum; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.room_direction_data
    ADD CONSTRAINT fk_room_vnum FOREIGN KEY (room_number) REFERENCES public.room(room_number) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- Name: camera_feed fk_room_vnum; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.camera_feed
    ADD CONSTRAINT fk_room_vnum FOREIGN KEY (feed_room_vnum) REFERENCES public.room(room_number) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- Name: computer_terminal fk_room_vnum; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.computer_terminal
    ADD CONSTRAINT fk_room_vnum FOREIGN KEY (terminal_room_vnum) REFERENCES public.room(room_number) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- Name: integral_object fk_room_vnum; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.integral_object
    ADD CONSTRAINT fk_room_vnum FOREIGN KEY (object_room_vnum) REFERENCES public.room(room_number) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- Name: mini_game fk_room_vnum; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.mini_game
    ADD CONSTRAINT fk_room_vnum FOREIGN KEY (game_room_vnum) REFERENCES public.room(room_number) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- Name: mini_gunner_sentinel fk_room_vnum; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.mini_gunner_sentinel
    ADD CONSTRAINT fk_room_vnum FOREIGN KEY (mgs_room_vnum) REFERENCES public.room(room_number) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- Name: npc_dialogue fk_room_vnum; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.npc_dialogue
    ADD CONSTRAINT fk_room_vnum FOREIGN KEY (dialogue_mob_vnum) REFERENCES public.mobile(mob_virtual_number) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- Name: shop_rooms fk_room_vnum; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.shop_rooms
    ADD CONSTRAINT fk_room_vnum FOREIGN KEY (shop_room_vnum) REFERENCES public.room(room_number) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- Name: hq_locations fk_room_vnum; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.hq_locations
    ADD CONSTRAINT fk_room_vnum FOREIGN KEY (hq_room_vnum) REFERENCES public.room(room_number) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- Name: room_extra_descriptions fk_room_vnum; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.room_extra_descriptions
    ADD CONSTRAINT fk_room_vnum FOREIGN KEY (red_room_vnum) REFERENCES public.room(room_number) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- Name: contract_step_callback fk_sequence_vnum; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.contract_step_callback
    ADD CONSTRAINT fk_sequence_vnum FOREIGN KEY (s_sequence_vnum) REFERENCES public.scripted_sequences(s_sequence_vnum) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- Name: scripted_steps fk_sequence_vnum; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.scripted_steps
    ADD CONSTRAINT fk_sequence_vnum FOREIGN KEY (s_sequence_vnum) REFERENCES public.scripted_sequences(s_sequence_vnum) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- Name: player_skill_usage fk_skill_id; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.player_skill_usage
    ADD CONSTRAINT fk_skill_id FOREIGN KEY (ps_skill_id) REFERENCES public.skill_trees(id) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- Name: player_skill_points fk_skill_id; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.player_skill_points
    ADD CONSTRAINT fk_skill_id FOREIGN KEY (ps_skill_id) REFERENCES public.skill_trees(id) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- Name: room fk_zone; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.room
    ADD CONSTRAINT fk_zone FOREIGN KEY (zone) REFERENCES public.zone(id) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- Name: mob_zone fk_zone_virtual_number; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.mob_zone
    ADD CONSTRAINT fk_zone_virtual_number FOREIGN KEY (zone_virtual_number) REFERENCES public.zone(zone_virtual_number) ON UPDATE CASCADE ON DELETE CASCADE;


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

