\c postgres;
DROP DATABASE mud;
CREATE DATABASE mud;
\c mud;

--
-- PostgreSQL database dump
--

-- Dumped from database version 13.5 (Debian 13.5-0+deb11u1)
-- Dumped by pg_dump version 13.5 (Debian 13.5-0+deb11u1)

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
-- Name: pgcrypto; Type: EXTENSION; Schema: -; Owner: -
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

SET default_table_access_method = heap;

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
-- Name: banned; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.banned (
    id integer NOT NULL,
    b_ip_address inet,
    b_hostname character varying(255),
    b_username character varying(16),
    b_enforce boolean DEFAULT true NOT NULL,
    created_at timestamp without time zone DEFAULT now() NOT NULL
);


ALTER TABLE public.banned OWNER TO postgres;

--
-- Name: banned_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.banned_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.banned_id_seq OWNER TO postgres;

--
-- Name: banned_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.banned_id_seq OWNED BY public.banned.id;


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
-- Name: frozen; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.frozen (
    id integer NOT NULL,
    f_ip_address inet,
    f_hostname character varying(255),
    f_username character varying(16),
    f_enforce boolean DEFAULT true NOT NULL,
    created_at timestamp without time zone DEFAULT now() NOT NULL
);


ALTER TABLE public.frozen OWNER TO postgres;

--
-- Name: frozen_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.frozen_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.frozen_id_seq OWNER TO postgres;

--
-- Name: frozen_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.frozen_id_seq OWNED BY public.frozen.id;


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
    mob_action_bitvector character varying(8) NOT NULL,
    mob_affection_bitvector character varying(8) NOT NULL,
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
    mob_ability_medical integer DEFAULT 1 NOT NULL,
    mob_raid_id integer,
    mob_scalable character varying(1) DEFAULT '0'::character varying NOT NULL
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
-- Name: muted; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.muted (
    id integer NOT NULL,
    m_ip_address inet,
    m_username character varying(16),
    m_hostname character varying(255),
    m_enforce boolean DEFAULT true NOT NULL,
    created_at timestamp without time zone DEFAULT now() NOT NULL
);


ALTER TABLE public.muted OWNER TO postgres;

--
-- Name: muted_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.muted_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.muted_id_seq OWNER TO postgres;

--
-- Name: muted_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.muted_id_seq OWNED BY public.muted.id;


--
-- Name: notch; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.notch (
    id integer NOT NULL,
    n_points integer DEFAULT 1 NOT NULL,
    n_name character varying NOT NULL,
    n_player_id integer NOT NULL,
    created_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
    updated_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL
);


ALTER TABLE public.notch OWNER TO postgres;

--
-- Name: notch_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.notch_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.notch_id_seq OWNER TO postgres;

--
-- Name: notch_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.notch_id_seq OWNED BY public.notch.id;


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
-- Name: raid; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.raid (
    id integer NOT NULL,
    r_name character varying(256) NOT NULL,
    r_level character varying(16) NOT NULL,
    r_type character varying(32) NOT NULL,
    r_status character varying(16) NOT NULL,
    created_at timestamp without time zone DEFAULT now() NOT NULL
);


ALTER TABLE public.raid OWNER TO postgres;

--
-- Name: raid_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.raid_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.raid_id_seq OWNER TO postgres;

--
-- Name: raid_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.raid_id_seq OWNED BY public.raid.id;


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
-- Name: rifle_attribute_limits; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.rifle_attribute_limits (
    id integer NOT NULL,
    ral_type character varying(16) NOT NULL,
    ral_attribute character varying(128) NOT NULL,
    ral_low integer NOT NULL,
    ral_high integer NOT NULL,
    ral_overpowered integer NOT NULL,
    ral_start_level integer NOT NULL,
    ral_end_level integer NOT NULL,
    created_at timestamp without time zone DEFAULT now() NOT NULL,
    updated_at timestamp without time zone DEFAULT now() NOT NULL
);


ALTER TABLE public.rifle_attribute_limits OWNER TO postgres;

--
-- Name: rifle_attribute_limits_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.rifle_attribute_limits_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.rifle_attribute_limits_id_seq OWNER TO postgres;

--
-- Name: rifle_attribute_limits_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.rifle_attribute_limits_id_seq OWNED BY public.rifle_attribute_limits.id;


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
    nickname text,
    textures text
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
-- Name: stay; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.stay (
    id integer NOT NULL,
    s_room_vnum integer NOT NULL,
    s_username character varying(16) NOT NULL,
    created_at timestamp without time zone DEFAULT now() NOT NULL
);


ALTER TABLE public.stay OWNER TO postgres;

--
-- Name: stay_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.stay_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.stay_id_seq OWNER TO postgres;

--
-- Name: stay_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.stay_id_seq OWNED BY public.stay.id;


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
-- Name: user_logins; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.user_logins (
    id integer NOT NULL,
    u_ip_address inet NOT NULL,
    u_username character varying(16) NOT NULL
);


ALTER TABLE public.user_logins OWNER TO postgres;

--
-- Name: user_logins_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.user_logins_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.user_logins_id_seq OWNER TO postgres;

--
-- Name: user_logins_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.user_logins_id_seq OWNED BY public.user_logins.id;


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
-- Name: banned id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.banned ALTER COLUMN id SET DEFAULT nextval('public.banned_id_seq'::regclass);


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
-- Name: frozen id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.frozen ALTER COLUMN id SET DEFAULT nextval('public.frozen_id_seq'::regclass);


--
-- Name: help_pages id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.help_pages ALTER COLUMN id SET DEFAULT nextval('public.help_pages_id_seq'::regclass);


--
-- Name: help_topics id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.help_topics ALTER COLUMN id SET DEFAULT nextval('public.help_topics_id_seq'::regclass);


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
-- Name: muted id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.muted ALTER COLUMN id SET DEFAULT nextval('public.muted_id_seq'::regclass);


--
-- Name: notch id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.notch ALTER COLUMN id SET DEFAULT nextval('public.notch_id_seq'::regclass);


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
-- Name: raid id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.raid ALTER COLUMN id SET DEFAULT nextval('public.raid_id_seq'::regclass);


--
-- Name: rifle_attachment id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.rifle_attachment ALTER COLUMN id SET DEFAULT nextval('public.rifle_attachment_id_seq'::regclass);


--
-- Name: rifle_attribute_limits id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.rifle_attribute_limits ALTER COLUMN id SET DEFAULT nextval('public.rifle_attribute_limits_id_seq'::regclass);


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
-- Name: stay id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.stay ALTER COLUMN id SET DEFAULT nextval('public.stay_id_seq'::regclass);


--
-- Name: terminal_choices id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.terminal_choices ALTER COLUMN id SET DEFAULT nextval('public.terminal_choices_id_seq'::regclass);


--
-- Name: user_logins id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.user_logins ALTER COLUMN id SET DEFAULT nextval('public.user_logins_id_seq'::regclass);


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
-- Data for Name: banned; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.banned (id, b_ip_address, b_hostname, b_username, b_enforce, created_at) FROM stdin;
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
13	111	2022-05-17 13:17:37.45463	2022-05-17 13:17:37.45463
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
15	24	GOAL_GIVE	TARGET_MOB	143	26
\.


--
-- Data for Name: contract_steps; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.contract_steps (id, s_contract_vnum, s_task_type, s_task_target, s_description, s_object_yaml, s_mob_vnum, s_room_vnum, s_quota, s_is_optional, s_order, s_reward_xp, s_reward_money, s_reward_1, s_reward_2, s_reward_3, s_reward_4, s_reward_5, s_reward_6, s_reward_7, s_reward_8, s_reward_9, s_reward_10, created_at, updated_at) FROM stdin;
41	24	0	0	description	\N	0	0	0	f	0	0	0	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2022-02-01 03:04:51.277468	2022-02-01 03:04:51.277468
26	1	1	2	You've got the scanner? Great! Now head to Allied Foods marketplace. It is northeast from here.\r\nYou will want to look for the {yel}Slaughter Isle{/yel}. Use your BIOMETRIC scanner to \r\ncheck the meat products for the presence of {red}HUMAN REMAINS{/red}\r\n\r\n\r\nOnce you've found positive results we will radio you with your next objective.	\N	0	266	0	f	0	120	0	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2021-09-18 01:54:46.849428	2021-09-18 01:54:46.849428
42	24	0	0	description	\N	0	0	0	f	0	0	0	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2022-02-01 03:04:51.293908	2022-02-01 03:04:51.293908
27	1	1024	1	Locate the slaughter isle to begin testing the foods using your biometric scanner.We need atleast 4 positive bioscan results.{blu}Hint: type {yel}bioscan <target>{/yel}	\N	0	653	0	f	1	1100	560	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2021-09-18 01:54:46.851098	2021-09-18 01:54:46.851098
43	24	0	0	description	\N	0	0	0	f	0	0	0	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2022-02-01 03:04:51.310587	2022-02-01 03:04:51.310587
28	1	1	2	Take the biometric scanner back to Doctor Magnus Ortem. We will need to decide next steps.	\N	0	143	0	f	2	850	500	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2021-09-18 01:54:46.85257	2021-09-18 01:54:46.85257
44	24	0	0	description	\N	0	0	0	f	0	0	0	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2022-02-01 03:04:51.327242	2022-02-01 03:04:51.327242
29	1	1	0	Go back to Doctor Land with the biometric scanner.	\N	603	0	0	f	3	50	50	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2021-09-18 01:54:46.854077	2021-09-18 01:54:46.854077
45	24	0	0	description	\N	0	0	0	f	0	0	0	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2022-02-01 03:04:51.343902	2022-02-01 03:04:51.343902
30	1	512	0	Give the biometric scanner to Doctor Land.	\N	603	0	4	f	4	1150	1150	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2021-09-18 01:54:46.855485	2021-09-18 01:54:46.855485
\.


--
-- Data for Name: contracts; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.contracts (id, c_vnum, c_description, c_title, created_at, updated_at) FROM stdin;
11	1	Good evening, soldier. According to recent advances in our {yel}TRITON{/yel} sponsored laboratory a new kind of ammunition is being built.\r\n\r\nThe blueprint for this ammunition contains heavy amounts of radioactive isotopes from recently spent Uranium. Find the scientist in the armory to east of {blu}COBALT{/blu} Main hallway. He will give you the magazines he has made so far. Once you've acquired the magazines, I will give you further instructions over radio.\r\n{yel}Here are the contract instructions:{/yel}\r\n{yel}1) {grn}Find {blu}Doctor Land{/blu} {grn}in the Gear Room near the shooting range.{/grn}\r\n{yel}2) {grn}Find the TRITON Labs scientist. He will give you the magazines you need.\r\n{yel}3) {grn}Take the magazines to {blu}Doctor Land{/blu}.\r\n{yel}4) {grn}Talk to {blu}Doctor Land{/blu} on next steps...\r\n	Experimental {yel}High Velocity{/yel} magazines.	2021-09-18 01:54:46.847285	2021-09-18 01:54:46.847285
17	24	We need you to go to the TRITON Lab and ask Doctor Land for the \r\nBIOMETRIC scanner. He should be expecting you.\r\n{yel}Here are the contract instructions:{/yel}\r\n{yel}1) {grn}Find {blu}Doctor Land{/blu} {grn}in the TRITON Lab{/grn}\r\n{yel}2) {grn}{yel}ask{/yel} {blu}Doctor Land{/blu}{grn} for the BIOMETRIC scanner{/grn}	Get the BIOMETRIC scanner from Doctor Land	2022-02-01 03:04:51.255491	2022-02-01 03:04:51.255491
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
-- Data for Name: frozen; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.frozen (id, f_ip_address, f_hostname, f_username, f_enforce, created_at) FROM stdin;
\.


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
115	car-thief	100	\N	\N	\N	\N	\N	armor/basic-ballistic-vest.yml	armor/baklava.yml	armor/blue-jeans.yml	armor/plat-basketball-shoes.yml	armor/mp-enforcer-gloves.yml	\N	\N	\N	\N	\N	\N	melee/crowbar.yml	\N	\N	\N	\N	\N	\N	\N	\N	\N	2022-02-01 01:43:27.252772	2022-02-01 01:43:27.252772
116	petty-thief	101	\N	\N	\N	\N	\N	\N	\N	armor/blue-jeans.yml	armor/plat-basketball-shoes.yml	\N	\N	\N	\N	\N	\N	\N	melee/cheap-crowbar.yml	\N	\N	\N	\N	\N	\N	\N	\N	\N	2022-02-01 01:43:27.384903	2022-02-01 01:43:27.384903
117	kidnapper	102	\N	\N	\N	armor/dark-throne-necklace.yml	\N	armor/leather-trenchcoat.yml	armor/baklava.yml	armor/black-jeans.yml	armor/atom-fade-shoes.yml	armor/brown-leather-gloves.yml	\N	\N	\N	\N	\N	\N	rifle/mp5.yml	\N	\N	\N	\N	\N	\N	\N	\N	\N	2022-02-01 01:43:27.517425	2022-02-01 01:43:27.517425
118	chaotic-meth-addict	103	\N	\N	\N	\N	\N	armor/leather-trenchcoat.yml	\N	armor/black-jeans.yml	armor/atom-fade-shoes.yml	\N	\N	\N	\N	\N	\N	\N	melee/sickening-knife.yml	\N	\N	\N	\N	\N	\N	\N	\N	\N	2022-02-01 01:43:27.676684	2022-02-01 01:43:27.676684
119	shoplifter	104	\N	\N	\N	\N	\N	armor/leather-trenchcoat.yml	armor/baklava.yml	armor/blue-jeans.yml	armor/atom-fade-shoes.yml	armor/brown-leather-gloves.yml	\N	\N	\N	\N	\N	\N	rifle/mp5.yml	\N	\N	\N	\N	\N	\N	\N	\N	\N	2022-02-01 01:43:27.809019	2022-02-01 01:43:27.809019
120	crackhead	105	\N	\N	\N	\N	\N	\N	\N	armor/blue-jeans.yml	armor/plat-basketball-shoes.yml	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2022-02-01 01:43:27.942669	2022-02-01 01:43:27.942669
121	armed-guard	106	\N	\N	\N	\N	\N	armor/falcon-ballistic-vest.yml	armor/baklava.yml	armor/mp-enforcer-pants.yml	armor/xm607-vulture-boots.yml	armor/forge-xm3-gloves.yml	\N	\N	\N	\N	\N	\N	rifle/mp5.yml	\N	\N	armor/titan-shoulder-pads.yml	armor/titan-shoulder-pads.yml	\N	\N	\N	armor/titan-elbow-guards.yml	armor/titan-elbow-guards.yml	2022-02-01 01:43:29.858195	2022-02-01 01:43:29.858195
122	teller	107	\N	\N	\N	\N	\N	\N	\N	armor/leggings.yml	armor/dress-shoes.yml	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2022-02-01 01:43:29.95304	2022-02-01 01:43:29.95304
293	dynamic-waypoint-ave-car-thief-level-10	700	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2022-06-10 23:45:11.351925	2022-06-10 23:45:11.351925
294	chaotic-meth-addict	701	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2022-06-10 23:45:11.371125	2022-06-10 23:45:11.371125
132	orthos-guard	503	\N	\N	\N	\N	\N	armor/vulture-ballistic-vest.yml	\N	armor/titan-shin-guards.yml	armor/xm50-ultralight-boots.yml	armor/mp-enforcer-gloves.yml	\N	\N	\N	\N	\N	\N	rifle/orthos-aug-a3.yml	\N	\N	\N	\N	\N	\N	\N	\N	\N	2022-02-01 16:49:10.066679	2022-02-01 16:49:10.066679
133	orthos-sniper	504	\N	\N	\N	\N	\N	armor/vulture-ballistic-vest.yml	\N	armor/titan-shin-guards.yml	armor/xm50-ultralight-boots.yml	armor/mp-enforcer-gloves.yml	\N	\N	\N	\N	\N	\N	rifle/orthos-l96aw.yml	\N	\N	\N	\N	\N	\N	\N	\N	\N	2022-02-01 16:49:10.216828	2022-02-01 16:49:10.216828
131	defiler	666	\N	\N	\N	\N	\N	armor/falcon-ballistic-vest.yml	\N	armor/mp-enforcer-pants.yml	armor/xm50-ultralight-boots.yml	armor/xm-scorpio-tactical-gloves.yml	\N	\N	\N	\N	\N	\N	rifle/defiler-scarh.yml	\N	\N	\N	\N	\N	\N	\N	\N	\N	2022-02-01 03:06:24.886311	2022-02-01 03:06:24.886311
134	orthos-menace	505	\N	\N	\N	\N	\N	armor/raven-ballistic-vest.yml	\N	armor/viper-leg-guards.yml	armor/xm50-ultralight-boots.yml	armor/mp-enforcer-gloves.yml	\N	\N	\N	\N	\N	\N	rifle/orthos-scarh.yml	\N	\N	\N	\N	\N	\N	\N	\N	\N	2022-02-01 16:49:10.325195	2022-02-01 16:49:10.325195
135	cofob-inventory-specialist	112	\N	\N	\N	\N	\N	armor/basic-ballistic-vest.yml	\N	armor/mp-enforcer-pants.yml	armor/xm50-ultralight-boots.yml	armor/brown-leather-gloves.yml	\N	\N	\N	\N	\N	\N	rifle/defiler-scarh.yml	\N	\N	armor/titan-shoulder-pads.yml	armor/titan-shoulder-pads.yml	\N	\N	\N	armor/titan-elbow-guards.yml	armor/titan-elbow-guards.yml	2022-05-12 23:13:18.320147	2022-05-12 23:13:18.320147
136	cofob-inventory-grunt	113	\N	\N	\N	\N	\N	armor/basic-ballistic-vest.yml	\N	armor/mp-enforcer-pants.yml	armor/xm50-ultralight-boots.yml	armor/brown-leather-gloves.yml	\N	\N	\N	\N	\N	\N	rifle/defiler-scarh.yml	\N	\N	armor/titan-shoulder-pads.yml	armor/titan-shoulder-pads.yml	\N	\N	\N	armor/titan-elbow-guards.yml	armor/titan-elbow-guards.yml	2022-05-12 23:13:18.33433	2022-05-12 23:13:18.33433
137	maintenance-guard	667	\N	\N	\N	\N	\N	armor/p5-vest.yml	\N	armor/p5-pants.yml	armor/p5-boots.yml	\N	\N	\N	\N	\N	\N	\N	rifle/glock.yml	\N	\N	\N	\N	\N	\N	\N	\N	\N	2022-05-12 23:13:19.009975	2022-05-12 23:13:19.009975
138	maintenance-guard	668	\N	\N	\N	\N	\N	armor/p5-vest.yml	\N	armor/p5-pants.yml	armor/p5-boots.yml	\N	\N	\N	\N	\N	\N	\N	rifle/glock.yml	\N	\N	\N	\N	\N	\N	\N	\N	\N	2022-05-12 23:13:19.068135	2022-05-12 23:13:19.068135
158	orthos-sentinel-spawn	669	\N	\N	\N	\N	\N	\N	\N	armor/mp-enforcer-pants.yml	armor/xm50-ultralight-boots.yml	armor/forge-xm3-gloves.yml	\N	\N	\N	\N	\N	\N	rifle/orthos-sentinel-scarh.yml	\N	\N	armor/titan-shoulder-pads.yml	armor/titan-shoulder-pads.yml	\N	\N	\N	armor/titan-elbow-guards.yml	armor/titan-elbow-guards.yml	2022-05-25 15:53:44.498592	2022-05-25 15:53:44.498592
159	infected-drone-lvl-2	114	\N	\N	\N	\N	\N	armor/leather-trenchcoat.yml	\N	armor/black-jeans.yml	armor/atom-fade-shoes.yml	\N	\N	\N	\N	\N	\N	\N	rifle/czp10.yml	\N	\N	\N	\N	\N	\N	\N	\N	\N	2022-05-25 15:53:44.573311	2022-05-25 15:53:44.573311
160	hustler-lvl-3	115	\N	\N	\N	\N	\N	armor/leather-trenchcoat.yml	\N	armor/black-jeans.yml	armor/atom-fade-shoes.yml	\N	\N	\N	\N	\N	\N	\N	rifle/czp10.yml	\N	\N	\N	\N	\N	\N	\N	\N	\N	2022-05-25 15:53:44.622028	2022-05-25 15:53:44.622028
295	crackhead	702	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2022-06-10 23:45:11.388911	2022-06-10 23:45:11.388911
162	hydro-proc-extremists-dynamic-level	670	\N	\N	\N	\N	\N	armor/leather-trenchcoat.yml	\N	armor/pale-brown-pants.yml	armor/plain-black-shoes.yml	\N	\N	\N	\N	\N	\N	\N	rifle/tar21.yml	\N	\N	\N	\N	\N	\N	\N	\N	\N	2022-05-28 19:47:34.979999	2022-05-28 19:47:34.979999
296	dynamic-defiler-level-10	703	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2022-06-10 23:45:11.405677	2022-06-10 23:45:11.405677
297	hydro-proc-extremists-dynamic-level	704	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2022-06-10 23:45:11.423145	2022-06-10 23:45:11.423145
298	dynamic-car-thief-level-10	705	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2022-06-10 23:45:11.43965	2022-06-10 23:45:11.43965
299	dynamic-mp-enforcer-level-10	706	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2022-06-10 23:45:11.457514	2022-06-10 23:45:11.457514
300	dynamic-mp-shotgunner-level-10	707	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2022-06-10 23:45:11.474898	2022-06-10 23:45:11.474898
301	dynamic-ops-shield-shotgunner-level-10	708	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2022-06-10 23:45:11.500835	2022-06-10 23:45:11.500835
302	petty-thief	709	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2022-06-10 23:45:11.527072	2022-06-10 23:45:11.527072
303	dynamic-rogue-mp-shotgunner-level-10	710	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2022-06-10 23:45:11.5404	2022-06-10 23:45:11.5404
171	dynamic-ops-shield-shotgunner-level-10	502	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2022-06-10 22:55:30.642144	2022-06-10 22:55:30.642144
304	shoplifter	711	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2022-06-10 23:45:11.555533	2022-06-10 23:45:11.555533
173	dynamic-rogue-mp-shotgunner-level-10	407	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2022-06-10 22:55:30.712947	2022-06-10 22:55:30.712947
305	kidnapper	712	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2022-06-10 23:45:11.571797	2022-06-10 23:45:11.571797
\.


--
-- Data for Name: mob_equipment_map; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.mob_equipment_map (id, mmap_mob_vnum, mmap_mob_equipment_vnum, created_at, updated_at) FROM stdin;
18	600	7	2021-09-04 04:23:26.981479	2021-09-04 04:23:26.981479
19	601	8	2021-09-04 04:23:26.988068	2021-09-04 04:23:26.988068
20	602	9	2021-09-04 04:23:26.993713	2021-09-04 04:23:26.993713
21	603	6	2021-09-04 04:23:27.008422	2021-09-04 04:23:27.008422
22	406	2	2021-09-04 04:23:27.04276	2021-09-04 04:23:27.04276
23	407	3	2021-09-04 04:23:27.047159	2021-09-04 04:23:27.047159
24	500	4	2021-09-04 04:23:27.068704	2021-09-04 04:23:27.068704
25	501	5	2021-09-04 04:23:27.074152	2021-09-04 04:23:27.074152
26	502	15	2021-09-04 04:23:27.079717	2021-09-04 04:23:27.079717
30	109	109	2021-09-04 04:23:27.352124	2021-09-04 04:23:27.352124
31	110	110	2021-09-04 04:23:27.364886	2021-09-04 04:23:27.364886
11	101	101	2021-09-04 04:12:22.396441	2021-09-04 04:12:22.396441
27	102	102	2021-09-04 04:23:27.155622	2021-09-04 04:23:27.155622
13	103	103	2021-09-04 04:12:22.417965	2021-09-04 04:12:22.417965
28	104	104	2021-09-04 04:23:27.177327	2021-09-04 04:23:27.177327
29	105	105	2021-09-04 04:23:27.182288	2021-09-04 04:23:27.182288
34	106	106	2021-09-06 01:20:27.887755	2021-09-06 01:20:27.887755
45	666	666	2022-02-01 03:06:24.986389	2022-02-01 03:06:24.986389
46	503	503	2022-02-01 16:49:10.091732	2022-02-01 16:49:10.091732
47	504	504	2022-02-01 16:49:10.233468	2022-02-01 16:49:10.233468
48	505	505	2022-02-01 16:49:10.341863	2022-02-01 16:49:10.341863
49	112	112	2022-05-12 23:13:18.323055	2022-05-12 23:13:18.323055
50	113	113	2022-05-12 23:13:18.338291	2022-05-12 23:13:18.338291
64	669	669	2022-05-25 15:53:44.504757	2022-05-25 15:53:44.504757
65	114	114	2022-05-25 15:53:44.579648	2022-05-25 15:53:44.579648
66	115	115	2022-05-25 15:53:44.627772	2022-05-25 15:53:44.627772
68	670	670	2022-05-28 19:47:34.981819	2022-05-28 19:47:34.981819
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
764	102	130	kidnapper
765	102	131	kidnapper
766	102	231	kidnapper
767	102	230	kidnapper
768	102	229	kidnapper
769	102	228	kidnapper
770	102	229	kidnapper
771	102	230	kidnapper
772	102	231	kidnapper
773	102	131	kidnapper
774	102	138	kidnapper
775	102	139	kidnapper
776	102	138	kidnapper
777	102	131	kidnapper
778	102	231	kidnapper
779	102	230	kidnapper
780	102	229	kidnapper
781	102	228	kidnapper
\.


--
-- Data for Name: mob_zone; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.mob_zone (id, zone_virtual_number, mob_virtual_number, room_virtual_number, max_existing) FROM stdin;
\.


--
-- Data for Name: mobile; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.mobile (mob_id, mob_virtual_number, mob_name, mob_short_description, mob_long_description, mob_description, mob_action_bitvector, mob_affection_bitvector, mob_ability_strength, mob_ability_strength_add, mob_ability_intelligence, mob_ability_wisdom, mob_ability_dexterity, mob_ability_constitution, mob_ability_charisma, mob_alignment, mob_attack_type, mob_level, mob_hitroll, mob_armor, mob_max_hitpoints, mob_max_mana, mob_max_move, mob_gold, mob_exp, mob_load_position, mob_default_position, mob_sex, mob_hitpoints, mob_mana, mob_move, mob_damnodice, mob_damsizedice, mob_damroll, mob_weight, mob_height, mob_class, mob_special_extended_type, mob_targets, mob_roam_pattern, mob_ability_electronics, mob_ability_armor, mob_ability_marksmanship, mob_ability_sniping, mob_ability_demolitions, mob_ability_chemistry, mob_ability_weapon_handling, mob_ability_strategy, mob_ability_medical, mob_raid_id, mob_scalable) FROM stdin;
11	410	Corporal James Taggart	Corporal James Tagger short description	Corporal James Tagger long description	Corporal James Tagger description	0	0	10	10	10	10	10	10	10	0	0	150	150	150	-1	-1	-1	0	50	0	0	0	-1	-1	-1	50	50	50	80	9	0	0	\N	\N	1	1	1	1	1	1	1	1	1	\N	0
1	1	chef  employee	A pissed looking Los  employee	A pissed looking Los  employee	This particular employee looks like he just got out of a federal penitentiary. He's most likely hiding some weapon in one of the many compartments that should be used for storing utensils and food paraphernalia.	8	0	25	0	25	0	25	0	25	0	0	0	0	0	250	250	250	5000	50	8	8	0	250	250	250	25	0	0	50	15	0	14	\N	\N	1	1	1	1	1	1	1	1	1	\N	0
12	600	A {yel}TRITON{/yel} {blu}LABS{/blu} Scientist	A {yel}TRITON{/yel} {blu}LABS{/blu} Scientist	A {yel}TRITON{/yel} {blu}LABS{/blu} Scientist	A {yel}TRITON{/yel} {blu}LABS{/blu} Scientist	8	0	85	83	82	0	84	0	1	0	0	40	0	0	58580	15	240	505000	30500	0	0	1	58580	15	140	84	86	215	10	4	0	0	\N	\N	10	10	10	10	10	10	10	10	10	\N	0
13	601	A volunteer patient	A volunteer patient	A volunteer patient	A volunteer patient stands here with dazed thousand mile stare.	8	0	185	183	10	0	184	0	1	0	0	15	0	0	880	25	240	0	3500	0	0	1	880	25	240	8	90	115	10	4	0	0	\N	\N	10	10	10	10	10	10	10	10	10	\N	0
14	602	A {yel}TRITON{/yel} {blu}LABS{/blu} Field Surgeon	A {yel}TRITON{/yel} {blu}LABS{/blu} Field Surgeon	A {yel}TRITON{/yel} {blu}LABS{/blu} Field Surgeon	A {yel}TRITON{/yel} {blu}LABS{/blu} Field Surgeon	8	0	185	183	182	0	184	0	85	0	0	80	0	0	8580	815	240	85000	8500	0	0	2	8580	815	240	-72	-70	215	10	4	0	0	\N	\N	10	10	10	10	10	10	10	10	10	\N	0
15	603	A Ballistics Expert	A Ballistics Expert	A Ballistics Expert	A Ballistics Expert	8	0	85	83	82	0	84	0	1	0	0	40	0	0	58580	15	240	505000	30500	0	0	1	58580	15	140	84	86	215	10	4	0	0	\N	\N	10	10	10	10	10	10	10	10	10	\N	0
16	406	A suspicious looking car thief	A suspicious looking car thief	A suspicious looking car thief	A car thief equipped with brass knuckles and a crowbar. He is obviously armed.	8	0	25	23	22	0	24	0	10	0	0	15	0	0	150	45	510	950	150	0	0	1	150	45	510	20	16	20	5	5	0	0	\N	\N	0	0	0	0	0	0	0	0	0	\N	0
17	407	A {red}Rogue{/red} Military Police shotgunner	A {red}Rogue{/red} Military Police shotgunner	A {red}Rogue{/red} Military Police shotgunner	A fit military police shotgunner. He looks armed.	8	0	35	33	32	0	34	0	10	0	0	25	0	0	5550	565	510	150	250	0	0	1	5550	565	510	15	18	30	10	6	0	11	\N	\N	0	0	0	0	0	0	0	0	0	\N	0
18	500	A Military Police enforcer	A Military Police enforcer	A Military Police enforcer	A fit military police enforcer. He looks armed.	8	0	25	23	22	0	24	0	10	0	0	10	0	0	250	45	110	50	7050	0	0	1	250	45	110	10	6	20	10	5	0	10	\N	\N	10	10	10	10	10	10	10	10	10	\N	0
19	501	A Military Police shotgunner	A Military Police shotgunner	A Military Police shotgunner	A fit military police shotgunner. He looks armed.	8	0	35	33	32	0	34	0	10	0	0	40	0	0	5550	565	510	150	7050	0	0	1	5550	565	510	15	18	30	10	6	0	11	\N	\N	10	10	10	10	10	10	10	10	10	\N	0
20	502	An Ops Shield shotgunner	An Ops Shield shotgunner	An Ops Shield shotgunner	An Ops Shield shotgunner	8	0	355	333	132	0	334	0	10	0	0	40	0	0	109550	1565	810	109150	20050	0	0	1	109550	1565	810	115	48	340	10	6	0	11	\N	\N	10	10	10	10	10	10	10	10	10	\N	0
24	109	A retail associate	A retail associate	A retail associate	A retail associate tirelessly stocks shelves...	8	0	15	1	2	0	4	0	4	0	0	3	0	0	120	50	60	85	580	0	0	1	120	50	60	2	5	2	10	5	0	19	\N	\N	2	1	1	1	1	0	1	2	1	\N	0
25	110	A shoplifter	A shoplifter	A shoplifter	A shoplifter looks around nervously...	8	0	30	1	5	0	8	0	8	0	0	6	0	0	240	100	128	150	580	0	0	1	240	100	128	5	10	5	10	5	0	17	\N	\N	5	3	2	2	3	0	3	5	2	\N	0
104	700	A suspicious looking car thief	A suspicious looking car thief	A suspicious looking car thief	A car thief equipped with brass knuckles and a crowbar.	4360	0	16	15	14	0	16	0	6	0	0	10	0	0	100	30	340	633	100	0	0	1	100	30	340	13	10	13	3	3	0	0	\N	\N	0	0	0	0	0	0	0	0	0	\N	1
3	101	A petty thief	A petty thief	A petty thief	A petty thief is stalking the area.	8	0	4	4	2	0	4	0	0	0	0	5	0	0	100	15	110	750	250	0	0	1	100	15	110	2	6	10	4	5	0	14	\N	\N	5	4	5	2	0	0	4	0	0	\N	0
21	102	a kidnapper	a kidnapper	a kidnapper	a kidnapper is stalking the area.	8	0	9	6	2	0	7	0	0	0	0	8	0	0	1450	25	510	1123	250	0	0	1	1450	25	510	10	25	20	4	5	0	15	\N	\N	0	10	8	3	0	0	5	0	0	\N	0
4	103	a chaotic meth addict	a chaotic meth addict	a chaotic meth addict	a chaotic meth addict is manically patrolling the area.	8	0	13	6	2	0	13	0	0	0	0	13	0	0	650	25	310	4123	250	0	0	1	650	25	310	8	25	30	4	5	0	16	\N	\N	0	4	1	1	2	0	8	0	9	\N	0
22	104	A shoplifter	A shoplifter	A shoplifter	A shoplifter is stalking the area.	8	0	1	1	1	0	1	0	0	0	0	1	0	0	10	15	60	10	250	0	0	1	10	15	60	1	3	1	4	5	0	15	\N	\N	1	0	1	0	0	0	1	0	0	\N	0
23	105	A crackhead	A crackhead	A crackhead	A crackhead is stalking the area.	8	0	1	1	1	0	3	0	0	0	0	2	0	0	35	15	60	10	250	0	0	1	35	15	60	2	6	1	4	5	0	0	\N	\N	0	0	0	0	0	0	1	0	0	\N	0
26	106	An armed security guard	An armed security guard	An armed security guard	An armed security guard watches you closely.	8	0	28	23	25	0	54	0	0	0	0	20	0	0	950	245	610	2150	20044	0	0	1	950	245	610	90	18	60	10	5	0	18	\N	\N	25	30	35	20	30	10	38	25	20	\N	0
27	107	A bank teller	A bank teller	A bank teller	A bank teller is obediently serving you	8	0	1	1	1	0	3	0	30	0	0	2	0	0	15	15	20	450	580	0	0	2	15	15	20	1	3	1	4	5	0	0	\N	\N	0	0	0	0	0	0	1	0	0	\N	0
105	701	a chaotic meth addict	a chaotic meth addict	a chaotic meth addict	a chaotic meth addict is manically patrolling the area.	4360	0	8	4	1	0	8	0	0	0	0	10	0	0	433	16	206	2748	166	0	0	1	433	16	206	5	16	20	2	3	0	16	\N	\N	0	2	0	0	1	0	5	0	6	\N	1
30	666	{red}DEFILER{/red}	{red}DEFILER{/red}	{red}DEFILER{/red}	{red}DEFILER{/red}	8	0	22	13	12	0	14	0	1	0	0	20	0	0	3820	830	160	42080	8050	0	0	1	3820	830	160	16	16	12	10	5	0	20	\N	Butcher	12	20	13	12	11	10	11	18	1	\N	0
29	113	An inventory grunt	An inventory grunt	An inventory grunt	An inventory grunt is here breaking a sweat. He hardly notices you.	8	0	99	500	500	0	500	0	500	0	0	200	0	0	98493	98100	9882	0	1	0	0	1	98493	98100	9882	-74	-74	950	10	5	0	15	\N	Inventory	500	500	500	500	500	500	500	500	500	\N	0
31	503	An Orthos guard	An Orthos guard	An Orthos guard	An Orthos guard regards you indifferently.	8	0	55	23	52	0	54	0	20	0	0	20	0	0	5850	245	110	250	8950	0	0	1	5850	245	110	20	16	30	10	5	0	21	\N	Psi-Tech	20	20	20	20	20	20	20	20	20	\N	0
32	504	An Orthos sniper	An Orthos sniper	An Orthos sniper	An Orthos sniper lies here... waiting..	8	0	35	33	32	0	34	0	10	0	0	20	0	0	6550	865	510	950	9050	0	0	1	6550	865	510	25	28	30	10	6	0	21	\N	Psi-Tech	80	10	110	90	10	10	90	10	10	\N	0
33	505	An Orthos menace	An Orthos menace	An Orthos menace	An Orthos menace	8	0	55	33	32	0	34	0	0	0	0	20	0	0	8550	1565	910	2150	5050	0	0	1	8050	1565	910	115	48	340	10	6	0	21	\N	Psi-Tech	80	50	40	30	80	0	40	20	0	\N	0
28	112	An inventory specialist	An inventory specialist	An inventory specialist	An inventory specialist takes a calculating look at the list of items on today's list.	8	0	99	500	500	0	500	0	500	0	0	200	0	0	98493	98100	9882	0	1	0	0	2	98493	98100	9882	-74	-74	950	10	5	0	15	\N	Inventory	500	500	500	500	500	500	500	500	500	\N	0
34	667	Maintenance guard	Maintenance guard	Maintenance guard	Maintenance guard	8	0	10	1	2	0	4	0	4	0	0	5	0	0	220	10	30	50	150	0	0	1	220	10	30	2	6	4	10	5	0	10	\N	\N	2	0	3	0	0	0	3	2	0	\N	0
35	668	Maintenance grunt	Maintenance grunt	Maintenance grunt	Maintenance grunt	8	0	8	1	1	0	7	0	5	0	0	5	0	0	180	20	40	25	100	0	0	1	180	20	40	4	4	3	10	5	0	15	\N	\N	0	2	6	0	0	0	4	1	0	\N	0
39	670	An extremist	An extremist	An extremist	An extremist spitefully makes eye contact with you.	8	0	5	0	5	0	10	0	10	0	0	5	0	0	1225	250	210	0	250	0	0	1	1225	250	210	5	20	25	10	5	0	15	\N	Hydro-Processing	5	5	10	5	5	5	5	5	5	\N	0
106	702	A crackhead	A crackhead	A crackhead	A crackhead is stalking the area.	4360	0	0	0	0	0	2	0	0	0	0	10	0	0	23	10	40	6	166	0	0	1	23	10	40	1	4	0	2	3	0	0	\N	\N	0	0	0	0	0	0	0	0	0	\N	1
107	703	{red}DEFILER{/red}	{red}DEFILER{/red}	{red}DEFILER{/red}	{red}DEFILER{/red}	8	0	11	6	6	0	7	0	0	0	0	10	0	0	1910	415	80	21040	4025	0	0	1	1910	415	80	8	8	6	10	5	0	20	\N	\N	6	10	6	6	5	5	5	9	0	\N	1
108	704	An extremist	An extremist	An extremist	An extremist spitefully makes eye contact with you.	8	0	10	10	10	0	20	0	20	0	0	10	0	0	2450	500	420	450	500	0	0	1	2450	500	420	10	40	50	10	5	0	15	\N	\N	10	10	20	10	10	10	10	10	10	\N	1
109	705	A rugged car thief	A rugged car thief	A rugged car thief	A car thief stalking the area. He is armed.	4616	0	18	13	2	0	34	0	2	0	0	10	0	0	350	45	610	750	250	0	0	1	350	45	610	30	6	20	10	5	0	14	\N	\N	15	5	15	8	2	2	18	2	2	\N	1
36	669	Orthos sentinel	Orthos sentinel	Orthos sentinel	Orthos sentinel	8	0	900	91	902	0	944	0	0	0	0	85	0	0	85000	9080	1500	0	2350	0	0	1	85000	9080	1500	-74	32	230	10	5	0	22	\N	\N	942	1000	944	944	1000	944	1000	944	1000	\N	0
37	114	An infected drone	An infected drone	An infected drone	An infected drone lumbers hugrily towards you...	8	0	2	2	2	0	4	0	5	0	0	2	0	0	493	100	82	95	100	0	0	1	493	100	82	1	8	10	10	5	0	15	\N	\N	2	3	3	2	3	1	3	2	2	\N	0
38	115	An adapted drone	An adapted drone 	An adapted drone 	An adapted drone looks for something to infect...	8	0	4	5	4	0	4	0	5	0	0	3	0	0	350	100	82	88	100	0	0	1	350	100	82	2	12	18	10	5	0	15	\N	\N	4	5	5	4	7	0	8	3	4	\N	0
110	706	A Military Police enforcer	A Military Police enforcer	A Military Police enforcer	A fit military police enforcer. He looks armed.	4616	0	25	23	22	0	24	0	10	0	0	10	0	0	25	45	110	50	7050	0	0	1	25	45	110	10	10	20	10	5	0	10	\N	\N	10	10	10	10	10	10	10	10	10	\N	1
111	707	A Military Police shotgunner	A Military Police shotgunner	A Military Police shotgunner	A fit military police shotgunner. He looks armed.	4616	0	35	33	32	0	34	0	10	0	0	10	0	0	5550	565	510	150	7050	0	0	1	5550	565	510	15	18	30	10	6	0	11	\N	\N	10	10	10	10	10	10	10	10	10	\N	1
112	708	An Ops Shield shotgunner	An Ops Shield shotgunner	An Ops Shield shotgunner	An Ops Shield shotgunner	4616	0	88	83	33	0	83	0	2	0	0	10	0	0	27387	391	202	27287	5012	0	0	1	27387	391	202	28	12	85	10	6	0	11	\N	\N	2	2	2	2	2	2	2	2	2	\N	1
113	709	A petty thief	A petty thief	A petty thief	A petty thief is stalking the area.	4360	0	2	2	1	0	2	0	0	0	0	10	0	0	66	10	73	500	166	0	0	1	66	10	73	1	4	6	2	3	0	14	\N	\N	3	2	3	1	0	0	2	0	0	\N	1
114	710	A {red}Rogue{/red} Military Police shotgunner	A {red}Rogue{/red} Military Police shotgunner	A {red}Rogue{/red} Military Police shotgunner	A fit military police shotgunner. He looks armed.	4360	0	14	13	12	0	13	0	4	0	0	10	0	0	2220	226	204	60	100	0	0	1	2220	226	204	6	7	12	10	6	0	11	\N	\N	0	0	0	0	0	0	0	0	0	\N	1
115	711	A shoplifter	A shoplifter	A shoplifter	A shoplifter is stalking the area.	4360	0	0	0	0	0	0	0	0	0	0	10	0	0	6	10	40	6	166	0	0	1	6	10	40	0	2	0	4	5	0	15	\N	\N	0	0	0	0	0	0	0	0	0	\N	1
116	712	a kidnapper	a kidnapper	a kidnapper	a kidnapper is stalking the area.	4360	0	6	4	1	0	4	0	0	0	0	10	0	0	966	16	340	748	166	0	0	1	966	16	340	6	16	13	4	5	0	15	\N	\N	0	6	5	2	0	0	3	0	0	\N	1
\.


--
-- Data for Name: muted; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.muted (id, m_ip_address, m_username, m_hostname, m_enforce, created_at) FROM stdin;
\.


--
-- Data for Name: notch; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.notch (id, n_points, n_name, n_player_id, created_at, updated_at) FROM stdin;
4	1	Sheesh	1	2022-05-25 18:52:55.314286	2022-05-25 18:52:55.314286
5	6	Janitorial services	1	2022-06-11 19:06:11.020393	2022-06-11 19:06:11.020393
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
111	$2a$06$tkZgj.i47ARkCbyx8ZixBuZqWRJjPKtgQuekkJ6ZRgoPJ.EtKtIeC	0	0	ghost	1	1	0	3	0	6	6	12	15	6	0	0	241	339	729	1108	2600	M	241	339	729	53	53	53	9	1	53	0	0	PC	0	3	53	0	2022-05-17 13:17:37.271391	0	2022-05-17 13:17:37.271391	8388736	5
1	foKntnEF3KSXA	0	0	far	1	1	0	20	0	41	41	82	102	41	3200	0	65000	2277	65000	7283	92840	M	65000	2277	65000	53	53	53	9	1	53	0	0	PC	3200	19	53	0	2019-03-20 22:38:47.454111	0	2019-03-20 22:38:47.454111	14680304	37
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
17	111	3	0	6	6	12	15	6	6	6	11	11	15	7	9	6	9	2022-05-17 13:17:37.452746	2022-05-17 13:17:37.452746
1	1	20	0	41	41	82	102	41	41	40	64	64	103	42	62	41	57	2021-09-04 04:17:39.714689	2021-09-04 04:17:39.714689
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
471	111	8	\N	sg3-sniper-ammunition.yml	2	\N	1	1	8
472	111	1	197	\N	1	\N	1	1	13
476	1	1	200	\N	1	\N	1	1	6
477	1	1	201	\N	1	\N	1	1	13
478	1	1	202	\N	1	\N	1	1	999
479	1	1	203	\N	1	\N	1	1	8
480	111	1	\N	psg1.yml	2	16	0	1	7
426	1	8	\N	sg3-shotgun-ammunition.yml	2	\N	1	1	17
437	111	7	\N	basic-ballistic-vest.yml	2	5	0	1	\N
438	111	7	\N	basic-boots.yml	2	8	0	1	\N
439	111	7	\N	titan-shin-guards.yml	2	7	0	1	\N
440	111	7	\N	titan-gauntlets.yml	2	10	0	1	\N
489	1	8	\N	sg3-sniper-ammunition.yml	2	\N	1	1	22
490	1	1	\N	psg1.yml	2	\N	1	1	3
492	1	8	\N	sg3-shotgun-ammunition.yml	2	\N	1	1	17
491	1	1	\N	dst7a.yml	2	16	0	1	7
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
-- Data for Name: raid; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.raid (id, r_name, r_level, r_type, r_status, created_at) FROM stdin;
14	defend the MP's	1-20	defend	INCOMPLETE	2022-06-09 22:43:12.819973
9	dire dire snare fest	everyone	CTF	completed	2022-06-03 22:55:42.954011
\.


--
-- Data for Name: rifle_attachment; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.rifle_attachment (id, rifle_player_id, rifle_data, rifle_position, created_at, updated_at) FROM stdin;
\.


--
-- Data for Name: rifle_attribute_limits; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.rifle_attribute_limits (id, ral_type, ral_attribute, ral_low, ral_high, ral_overpowered, ral_start_level, ral_end_level, created_at, updated_at) FROM stdin;
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
1	90	10	0	0	90	40	9	0	COMMON	sasg12.yml	SHOTGUN	3	R.S.S.	SASG-12	6	40	SHOTGUN	77	8	2	34	1	80	0	53	1	4.3	2	10	1	10	1	1	0	1	1	1	0	1	1	1	1	0	0	1	1	0	0	0	0	1	1	1	1	0	0	2021-09-06 01:28:51.675323	2021-09-06 01:28:51.675323
2	90	10	0	0	90	40	9	0	COMMON	saiga12.yml	SHOTGUN	3	R.S.S.	SAIGA-12	39	1	SHOTGUN	1	6	2	34	1	154	1	53	2	1	2	14	918	15	3	31	0	1	1	1	1	0	1	1	1	1	0	1	1	1	1	0	0	0	1	0	1	0	0	2021-09-06 01:57:11.43389	2021-09-06 01:57:11.43389
3	80	40	13	5	50	25	10	1	COMMON	magnum-revolver.yml	PISTOL	8	TN3 SMITH-x Industrial	Magnum Revolver	40	75	PISTOL	20.5	6	1	6	1	25	1	55	1	0	4	1	1	2	3	24	1	1	1	0	1	1	1	1	0	1	1	0	0	1	1	0	1	1	1	0	0	0	0	2021-09-06 01:57:28.204973	2021-09-06 01:57:28.204973
4	80	40	13	5	50	25	10	1	COMMON	czp10.yml	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	75	PISTOL	1	1	1	5	1	39	0	33	1	0	3	80	1	2	3	1	1	0	1	1	1	0	0	0	0	1	1	0	0	0	1	0	0	0	1	1	1	1	0	2021-09-06 01:59:56.640283	2021-09-06 01:59:56.640283
5	90	10	0	0	80	40	0	0	COMMON	ump45.yml	SUB_MACHINE_GUN	5	PN/P	UMP-45	32	400	SUB_MACHINE_GUN	30.81	31	1	1	1	30	1	29	3	4.3	5	1	1357	20	10	6	1	1	1	1	1	1	1	1	1	0	1	0	0	1	1	0	1	0	1	1	1	1	0	2021-09-06 02:00:02.293957	2021-09-06 02:00:02.293957
6	90	10	0	0	90	40	9	0	COMMON	sasg12.yml	SHOTGUN	3	R.S.S.	SASG-12	6	1	SHOTGUN	70	6	2	34	1	80	1	53	1	4.3	1	19	918	10	3	1	1	1	0	1	0	1	1	1	1	1	1	1	1	0	1	0	1	1	0	0	1	0	0	2021-09-06 02:08:25.716967	2021-09-06 02:08:25.716967
7	80	40	13	5	50	25	10	1	COMMON	glock.yml	PISTOL	8	TN3 SMITH-x Industrial	Glock	41	75	PISTOL	1	9	1	5	1	21	1	1	1	1	3	80	40	3	4	16	1	1	0	1	1	1	0	1	1	1	1	1	0	0	0	0	1	1	1	1	1	1	0	2021-09-06 02:08:49.453172	2021-09-06 02:08:49.453172
8	0	20	90	90	10	20	90	90	COMMON	psg1.yml	SNIPER	6	PF-TDN	PSG1	8	70	SNIPER	1	7	16	19	3	1	0	23	4	1	12	13	1818	512	2	53	1	1	0	1	1	1	0	0	1	0	1	1	0	0	1	0	1	1	0	0	1	1	0	2021-09-06 02:09:56.904971	2021-09-06 02:09:56.904971
9	90	10	0	0	80	40	0	0	COMMON	fmg9.yml	SUB_MACHINE_GUN	5	DXGR-1	FMG-9	5	888	SUB_MACHINE_GUN	33	1	1	14	1	30	1	13	1	1	5	180	1	33	6	6	1	0	1	0	1	1	1	0	1	1	1	0	0	1	1	0	1	0	1	1	1	1	0	2021-09-06 02:11:28.277761	2021-09-06 02:11:28.277761
10	90	10	0	0	90	40	9	0	COMMON	sasg12.yml	SHOTGUN	3	R.S.S.	SASG-12	6	40	SHOTGUN	1	9	2	34	1	1	1	73	2	7	2	1	918	12	3	20	1	0	1	1	0	0	0	1	1	0	0	0	1	1	1	0	0	1	1	1	0	1	0	2021-09-06 02:48:16.222825	2021-09-06 02:48:16.222825
11	80	40	13	5	50	25	10	1	COMMON	magnum-revolver.yml	PISTOL	8	TN3 SMITH-x Industrial	Magnum Revolver	40	75	PISTOL	20.5	6	1	5	1	36	1	33	1	0	1	1	40	3	1	26	1	1	1	1	0	0	0	1	1	0	1	1	1	1	0	0	1	0	0	1	1	0	0	2021-09-06 02:48:40.192837	2021-09-06 02:48:40.192837
12	90	10	0	0	90	40	9	0	COMMON	sasg12.yml	SHOTGUN	3	R.S.S.	SASG-12	6	40	SHOTGUN	70	6	2	1	1	129	0	66	2	4.3	2	1	920	10	1	20	0	0	1	1	1	1	1	1	1	1	1	1	0	0	0	0	1	0	1	1	0	0	0	2021-09-06 02:53:24.30776	2021-09-06 02:53:24.30776
13	90	10	0	0	90	40	9	0	COMMON	saiga12.yml	SHOTGUN	3	R.S.S.	SAIGA-12	39	40	SHOTGUN	70	8	2	34	1	80	0	1	1	4.3	3	10	918	1	3	20	1	1	0	1	1	1	0	1	1	1	0	0	0	1	0	0	1	1	1	0	1	0	0	2021-09-06 03:02:35.527086	2021-09-06 03:02:35.527086
14	80	40	13	5	50	25	10	1	COMMON	glock.yml	PISTOL	8	TN3 SMITH-x Industrial	Glock	41	1	PISTOL	20.5	1	1	9	1	20	1	33	1	0	5	80	1	1	1	16	1	0	1	0	1	1	1	1	1	1	1	1	0	0	0	0	1	1	0	1	1	0	0	2021-09-06 03:04:47.023351	2021-09-06 03:04:47.023351
15	90	10	0	0	90	40	9	0	COMMON	sasg12.yml	SHOTGUN	3	R.S.S.	SASG-12	6	1	SHOTGUN	73	6	2	1	1	80	0	1	4	1	2	10	918	10	3	20	1	1	1	1	1	1	1	0	1	1	0	1	1	1	1	0	1	1	0	1	1	0	0	2021-09-06 03:05:55.639243	2021-09-06 03:05:55.639243
16	90	10	0	0	90	40	9	0	COMMON	sasg12.yml	SHOTGUN	3	R.S.S.	SASG-12	6	40	SHOTGUN	70	6	2	34	1	80	1	53	1	4.3	2	1	918	1	5	1	1	0	1	1	0	1	1	1	1	0	1	1	1	0	1	0	0	1	1	0	1	1	0	2021-09-06 03:06:30.49336	2021-09-06 03:06:30.49336
17	23.010099	0	0	0	50	0	0	0	COMMON	desert-eagle.yml	PISTOL	8	LX Industries	Desert Eagle	47	50	PISTOL	0	7	6	1	1	35	1	61	1	0	5	8	1	2	5	20	1	0	1	1	1	0	1	1	1	1	1	0	1	1	1	0	0	1	0	0	0	1	0	2021-09-06 03:06:37.447102	2021-09-06 03:06:37.447102
18	80	40	13	5	50	25	10	1	COMMON	glock.yml	PISTOL	8	TN3 SMITH-x Industrial	Glock	41	1	PISTOL	1	1	1	5	1	20	1	42	1	1	3	1	1	2	3	16	1	0	0	1	1	0	1	1	0	0	1	0	1	1	1	0	0	0	0	1	0	1	0	2021-09-06 03:08:25.046276	2021-09-06 03:08:25.046276
19	0	20	90	90	10	20	90	90	COMMON	l96aw.yml	SNIPER	6	Heckler and Koch	L96 Arctic Warfare	24	70	SNIPER	1	7	2	1	5	1	0	20	4	4.3	11	10	1818	775	5	54	0	1	1	0	0	1	1	1	1	1	0	0	1	1	1	0	0	1	1	1	0	0	0	2021-09-06 03:08:36.809308	2021-09-06 03:08:36.809308
20	10	10	10	10	10	10	10	10	COMMON	552-commando.yml	ASSAULT_RIFLE	4	S1 Industries	552 Commando	69	220	ASSAULT_RIFLE	1	14	2	14	4	5	3.14	3	3	4.3	1	80	1	2	6	1	0	0	0	0	0	0	0	0	0	1	0	0	0	1	0	0	0	1	1	1	1	1	0	2021-09-06 03:13:23.838239	2021-09-06 03:13:23.838239
21	80	40	13	5	50	25	10	1	COMMON	uzi.yml	PISTOL	8	TN3 SMITH-x Industrial	A Uzi	43	75	MACHINE_PISTOL	10.5	1	1	5	1	1	1	33	1	0	3	80	40	3	3	16	0	1	0	1	0	1	1	1	1	1	1	1	0	1	1	0	1	1	0	0	1	1	0	2021-09-06 03:14:24.90574	2021-09-06 03:14:24.90574
22	0	20	90	90	10	20	90	90	COMMON	psg1.yml	SNIPER	6	PF-TDN	PSG1	8	1	SNIPER	0.81	11	16	19	1	450	0	1	4	1	1	1	1	500	3	49	1	1	0	1	1	1	0	1	1	1	0	0	1	1	1	0	0	1	0	1	0	0	0	2021-09-06 03:17:24.397361	2021-09-06 03:17:24.397361
23	90	10	0	0	90	40	9	0	COMMON	saiga12.yml	SHOTGUN	3	R.S.S.	SAIGA-12	39	40	SHOTGUN	115	11	2	34	1	91	0	1	2	4.3	3	1	918	1	1	20	1	1	0	1	1	0	0	0	1	0	0	0	1	0	1	0	1	0	1	1	1	1	0	2021-09-06 03:40:23.665692	2021-09-06 03:40:23.665692
24	90	10	0	0	90	40	9	0	COMMON	sasg12.yml	SHOTGUN	3	R.S.S.	SASG-12	6	40	SHOTGUN	70	1	2	34	1	1	0	1	3	7	2	10	918	10	3	1	1	0	0	1	1	1	0	0	0	1	0	1	1	0	1	0	1	1	1	0	0	1	0	2021-09-06 03:42:49.10483	2021-09-06 03:42:49.10483
25	90	10	0	0	90	40	9	0	COMMON	sasg12.yml	SHOTGUN	3	R.S.S.	SASG-12	6	40	SHOTGUN	70	8	2	34	1	117	1	53	2	1	3	10	918	13	1	20	0	1	1	1	1	0	1	1	1	0	1	0	0	1	1	0	0	1	1	0	0	1	0	2021-09-06 03:51:37.101185	2021-09-06 03:51:37.101185
26	90	10	0	0	80	40	0	0	COMMON	belt-fed-minigun.yml	LIGHT_MACHINE_GUN	10	TN-33Y	A TN-33Y Belt-Fed Minigun	23	999	LIGHT_MACHINE_GUN	3.81	999	20	23	1	30	1	1	11	4.3	33	180	1	26	1	140	1	1	0	1	0	1	1	0	1	0	0	1	1	1	0	0	1	1	1	0	1	1	0	2021-09-06 03:53:40.659097	2021-09-06 03:53:40.659097
27	90	10	0	0	80	40	0	0	COMMON	mp9.yml	MACHINE_PISTOL	9	Heckler and Koch	MP9	38	1	MACHINE_PISTOL	32	37	1	14	1	30	1	21	3	1	5	1	1	20	1	6	1	1	1	0	1	1	0	1	1	1	0	1	1	0	1	0	0	1	0	0	1	1	0	2021-09-06 03:54:00.480819	2021-09-06 03:54:00.480819
28	10	10	10	10	10	10	10	10	COMMON	552-commando.yml	ASSAULT_RIFLE	4	S1 Industries	552 Commando	69	220	ASSAULT_RIFLE	1	8	2	1	4	5	3.14	3	1	4	8	80	24	1	1	1	1	1	0	1	1	0	1	1	1	1	0	1	1	1	1	0	1	0	0	1	1	1	0	2021-09-06 03:54:39.313207	2021-09-06 03:54:39.313207
29	80	40	13	5	50	25	10	1	COMMON	magnum-revolver.yml	PISTOL	8	TN3 SMITH-x Industrial	Magnum Revolver	40	75	PISTOL	33	1	1	7	1	1	0	33	1	1	3	80	1	2	1	16	1	1	1	1	0	0	1	1	1	0	0	0	0	0	0	0	0	0	0	1	0	1	0	2021-09-06 03:54:48.391041	2021-09-06 03:54:48.391041
30	10	10	10	10	10	10	10	10	COMMON	aug-a3.yml	ASSAULT_RIFLE	4	VI-AUG-IX	AUG A3	44	220	ASSAULT_RIFLE	4.23	8	2	19	4	7	1	3	3	1	1	99	24	2	11	1	1	1	0	1	1	0	1	1	1	1	0	0	1	0	1	0	0	0	1	1	1	1	0	2021-09-06 03:58:07.458034	2021-09-06 03:58:07.458034
31	90	10	0	0	80	40	0	0	COMMON	belt-fed-minigun.yml	LIGHT_MACHINE_GUN	10	TN-33Y	A TN-33Y Belt-Fed Minigun	23	999	LIGHT_MACHINE_GUN	3.81	999	20	1	1	30	0	23	7	4.3	1	1	1	36	40	1	1	0	1	0	1	1	1	1	0	1	0	1	1	1	0	0	1	1	0	1	1	1	0	2021-09-06 04:00:21.032214	2021-09-06 04:00:21.032214
32	0	20	90	90	10	20	90	90	COMMON	l96aw.yml	SNIPER	6	Heckler and Koch	L96 Arctic Warfare	24	70	SNIPER	0.81	7	2	19	5	450	1	21	1	1	8	59	2	500	1	1	2	1	1	1	5	1	0	1	0	0	2	1	1	1	1	0	0	1	2	1	0	1	0	2021-09-06 04:00:25.350172	2021-09-06 04:00:25.350172
33	90	10	0	0	80	40	0	0	COMMON	mp5.yml	SUB_MACHINE_GUN	5	Heckler and Koch	MP5	5	400	SUB_MACHINE_GUN	30.81	2	1	14	1	1	0	16	3	4.3	5	1	1	44	29	1	0	1	0	1	1	1	2	1	2	1	1	1	0	1	0	0	1	1	1	0	0	1	0	2021-09-06 04:00:53.142172	2021-09-06 04:00:53.142172
34	80	40	13	5	50	25	10	1	COMMON	magnum-revolver.yml	PISTOL	8	TN3 SMITH-x Industrial	Magnum Revolver	40	1	PISTOL	20	10	1	1	1	1	0	33	1	0	1	80	40	2	5	16	1	1	5	0	1	1	1	1	0	2	1	0	2	1	0	0	1	0	9	2	1	0	0	2021-09-06 04:01:03.958174	2021-09-06 04:01:03.958174
35	90	10	0	0	80	40	0	0	COMMON	mp5.yml	SUB_MACHINE_GUN	5	Heckler and Koch	MP5	5	1162	SUB_MACHINE_GUN	1	82	1	14	2	30	0	66	4	4.3	10	1	1	1	6	6	1	1	1	1	1	1	2	1	1	0	2	0	1	0	2	0	1	0	1	2	2	0	0	2021-09-06 04:01:19.683189	2021-09-06 04:01:19.683189
36	80	40	13	5	50	25	10	1	COMMON	glock.yml	PISTOL	8	TN3 SMITH-x Industrial	Glock	41	75	PISTOL	1	1	1	5	1	20	2	1	1	2	3	1	1	2	3	16	1	2	1	0	2	1	1	1	2	1	1	1	4	0	1	0	1	2	0	1	0	1	0	2021-09-06 04:01:46.176273	2021-09-06 04:01:46.176273
37	80	40	13	5	50	25	10	1	COMMON	uzi.yml	PISTOL	8	TN3 SMITH-x Industrial	A Uzi	43	75	MACHINE_PISTOL	10.5	13	1	1	1	23	0	1	2	0	1	1	40	3	3	1	1	0	2	5	2	0	1	1	1	1	0	1	1	1	2	0	1	0	0	0	0	1	0	2021-09-06 04:01:51.777233	2021-09-06 04:01:51.777233
38	80	40	13	5	50	25	10	1	COMMON	czp10.yml	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	75	PISTOL	20.5	12	1	5	2	3	0	33	1	1	1	80	1	1	3	1	0	1	0	4	1	0	1	1	3	2	0	2	3	1	2	0	0	0	0	1	1	0	0	2021-09-06 04:03:22.246198	2021-09-06 04:03:22.246198
39	10	10	10	10	10	10	10	10	COMMON	g36c.yml	ASSAULT_RIFLE	4	Heckler & Koch	G36C Assault Rifle	46	1	ASSAULT_RIFLE	4	8	2	14	2	5	1	1	1	4.3	5	80	24	2	6	19	1	1	1	2	2	0	1	0	2	1	2	1	1	2	2	0	0	1	2	1	0	0	0	2021-09-06 04:03:32.066198	2021-09-06 04:03:32.066198
40	90	10	0	0	90	40	9	0	COMMON	saiga12.yml	SHOTGUN	3	R.S.S.	SAIGA-12	39	100	SHOTGUN	169	1	2	34	1	80	0	1	4	12	1	10	918	10	8	26	1	1	1	0	1	2	1	2	4	1	1	1	0	0	0	0	1	4	2	0	0	2	0	2021-09-06 04:11:31.980285	2021-09-06 04:11:31.980285
41	10	10	10	10	10	10	10	10	COMMON	m16a4.yml	ASSAULT_RIFLE	4	Standard Issue	M16A4 Assault Rifle	25	220	ASSAULT_RIFLE	4.23	1	2	14	1	5	3.14	1	3	7	7	1	1	4	6	1	1	1	1	1	1	0	2	2	2	1	2	1	1	1	2	0	1	1	0	1	1	5	0	2021-09-06 04:12:25.294101	2021-09-06 04:12:25.294101
42	23.010099	0	0	0	50	0	0	0	COMMON	desert-eagle.yml	PISTOL	8	LX Industries	Desert Eagle	47	189	PISTOL	0	14	6	2	1	2	2	57	1	8	5	9	1	2	1	36	2	1	7	1	2	1	3	7	2	2	2	0	1	3	1	0	9	1	0	3	1	0	0	2021-09-06 04:13:09.227471	2021-09-06 04:13:09.227471
43	90	10	0	0	80	40	0	0	COMMON	belt-fed-minigun.yml	LIGHT_MACHINE_GUN	10	TN-33Y	A TN-33Y Belt-Fed Minigun	23	999	LIGHT_MACHINE_GUN	3.81	2	20	14	1	30	2	13	2	9	72	180	1	1	47	2	2	2	2	1	0	4	2	1	2	1	1	0	1	0	1	0	1	0	1	2	2	2	0	2021-09-06 04:13:23.551378	2021-09-06 04:13:23.551378
44	80	40	13	5	50	25	10	1	COMMON	czp10.yml	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	75	PISTOL	20.5	26	1	5	1	2	1	33	1	1	1	6	40	2	1	16	4	1	9	0	2	3	1	0	0	1	0	1	0	2	1	0	2	3	0	1	3	1	0	2021-09-06 04:13:44.591935	2021-09-06 04:13:44.591935
45	90	10	0	0	90	40	9	0	COMMON	saiga12.yml	SHOTGUN	3	R.S.S.	SAIGA-12	39	40	SHOTGUN	586	6	2	1	2	1	2	2	2	3	2	29	918	10	4	20	1	2	1	3	2	0	0	2	7	1	0	1	2	2	2	0	0	6	0	1	0	4	0	2021-09-06 04:21:24.352496	2021-09-06 04:21:24.352496
46	10	10	10	10	10	10	10	10	COMMON	552-commando.yml	ASSAULT_RIFLE	4	S1 Industries	552 Commando	69	2	ASSAULT_RIFLE	16	2	2	2	4	5	3.14	6	2	4.3	1	80	24	6	54	31	2	2	2	3	0	2	5	0	0	2	1	0	1	2	1	0	0	0	0	1	2	2	0	2021-09-06 04:21:40.381543	2021-09-06 04:21:40.381543
47	23.010099	0	0	0	50	0	0	0	COMMON	desert-eagle.yml	PISTOL	8	LX Industries	Desert Eagle	47	50	PISTOL	0	7	6	11	3	35	1	125	1	1	2	10	2	2	2	1	0	0	0	0	0	0	0	0	0	0	2	0	2	2	1	0	6	3	2	0	0	0	0	2021-09-06 04:22:42.764595	2021-09-06 04:22:42.764595
48	0	20	90	90	10	20	90	90	COMMON	psg1.yml	SNIPER	6	PF-TDN	PSG1	8	70	SNIPER	2	2	16	1	3	1620	2	13	4	1	9	10	1818	1	2	49	1	0	1	27	1	1	3	4	0	0	0	3	2	2	0	0	2	2	3	1	2	3	0	2021-09-06 04:22:46.863401	2021-09-06 04:22:46.863401
49	80	40	13	5	50	25	10	1	COMMON	uzi.yml	PISTOL	8	TN3 SMITH-x Industrial	A Uzi	43	75	MACHINE_PISTOL	30	13	1	5	1	62	0	3	2	0	1	80	1	7	2	1	1	5	2	1	2	2	0	2	3	0	1	1	0	19	1	0	0	1	4	0	2	0	0	2021-09-06 04:23:01.641628	2021-09-06 04:23:01.641628
50	0	20	90	90	10	20	90	90	COMMON	psg1.yml	SNIPER	6	PF-TDN	PSG1	8	90	SNIPER	0.81	21	16	8	3	450	0	2	7	15	1	12	1818	500	2	49	5	2	1	2	0	1	1	1	1	0	0	2	6	2	2	0	0	2	2	1	2	1	0	2021-09-06 04:23:35.951147	2021-09-06 04:23:35.951147
51	90	10	0	0	90	40	9	0	COMMON	sasg12.yml	SHOTGUN	3	R.S.S.	SASG-12	6	40	SHOTGUN	115	17	2	34	1	80	1	53	4	10	3	20	918	10	4	2	2	1	0	0	2	10	2	1	1	0	1	0	2	1	1	0	0	3	2	0	2	7	0	2021-09-06 04:27:50.496627	2021-09-06 04:27:50.496627
52	90	10	0	0	80	40	0	0	COMMON	mp5.yml	SUB_MACHINE_GUN	5	Heckler and Koch	MP5	5	400	SUB_MACHINE_GUN	461	31	1	1	1	30	2	13	10	4.3	1	2	818	2	21	19	2	4	2	2	1	1	1	2	2	0	2	3	0	2	3	0	0	0	1	1	3	3	0	2021-09-06 04:28:20.129768	2021-09-06 04:28:20.129768
53	90	10	0	0	90	40	9	0	COMMON	saiga12.yml	SHOTGUN	3	R.S.S.	SAIGA-12	39	147	SHOTGUN	70	62	2	2	2	80	1	53	2	4.3	2	10	918	21	3	2	0	1	2	3	2	0	3	2	0	1	2	1	3	2	2	0	0	0	0	1	0	0	0	2021-09-07 10:38:58.671814	2021-09-07 10:38:58.671814
54	80	40	13	5	50	25	10	1	COMMON	czp10.yml	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	1	PISTOL	20.5	2	1	13	8	20	0	33	2	2	6	1	40	1	3	2	0	0	2	2	2	1	34	2	0	0	1	2	0	0	0	0	1	9	3	0	2	2	0	2021-09-07 10:39:50.158031	2021-09-07 10:39:50.158031
55	80	40	13	5	50	25	10	1	COMMON	uzi.yml	PISTOL	8	TN3 SMITH-x Industrial	A Uzi	43	3	MACHINE_PISTOL	20	13	1	2	5	20	0	42	1	0	9	290	40	1	3	3	2	1	1	2	0	11	1	0	1	0	2	2	0	1	1	0	7	2	0	1	2	2	0	2021-09-07 10:40:02.18301	2021-09-07 10:40:02.18301
56	90	10	0	0	90	40	9	0	COMMON	sasg12.yml	SHOTGUN	3	R.S.S.	SASG-12	6	47	SHOTGUN	2	6	2	34	3	1	0	309	4	4.3	2	1	918	82	1	59	1	1	1	1	0	2	0	1	1	2	2	0	2	0	0	0	2	2	1	1	3	1	0	2021-09-07 10:48:10.468071	2021-09-07 10:48:10.468071
57	90	10	0	0	90	40	9	0	COMMON	sasg12.yml	SHOTGUN	3	R.S.S.	SASG-12	6	40	SHOTGUN	112	2	2	34	1	147	0	1	2	17	2	26	12	10	3	62	2	0	7	2	2	11	7	0	0	2	0	1	2	3	0	0	1	3	1	0	1	2	0	2021-09-07 10:54:48.821086	2021-09-07 10:54:48.821086
58	90	10	0	0	80	40	0	0	COMMON	mp9.yml	MACHINE_PISTOL	9	Heckler and Koch	MP9	38	1	MACHINE_PISTOL	30.81	1	1	2	3	108	2	1	8	4.3	2	180	818	20	6	14	17	0	2	2	5	2	2	11	1	0	0	0	7	2	1	0	4	2	0	1	2	4	0	2021-09-07 10:55:09.614216	2021-09-07 10:55:09.614216
59	10	10	10	10	10	10	10	10	COMMON	scarh.yml	ASSAULT_RIFLE	4	SK-10	SCAR-H Assault Rifle	31	220	ASSAULT_RIFLE	16	1	2	14	16	1	3.14	1	2	4.3	2	1	77	4	6	2	3	0	1	1	22	2	0	14	0	2	5	7	1	2	2	0	0	2	0	0	3	0	0	2021-09-07 10:55:32.641118	2021-09-07 10:55:32.641118
60	10	10	10	10	10	10	10	10	COMMON	552-commando.yml	ASSAULT_RIFLE	4	S1 Industries	552 Commando	69	458	ASSAULT_RIFLE	1	8	2	14	2	2	3.14	1	2	4.3	18	2	75	3	6	1	1	1	1	3	1	2	0	1	2	0	0	0	0	0	0	0	0	2	0	2	0	0	0	2021-09-07 10:56:19.699137	2021-09-07 10:56:19.699137
61	10	10	10	10	10	10	10	10	COMMON	tar21.yml	ASSAULT_RIFLE	4	SK-10	TAR-21 Assault Rifle	30	2	ASSAULT_RIFLE	17	8	2	3	2	5	1	2	1	15	5	376	2	2	52	38	0	0	3	1	2	1	0	2	4	0	1	0	1	0	0	0	0	2	2	40	2	1	0	2021-09-07 10:57:40.930179	2021-09-07 10:57:40.930179
62	0	20	90	90	10	20	90	90	COMMON	l96aw.yml	SNIPER	6	Heckler and Koch	L96 Arctic Warfare	24	1	SNIPER	2	7	2	19	3	2	2	3	4	1	8	16	1	500	8	54	2	0	2	1	0	3	4	1	19	0	1	3	1	1	0	0	0	19	4	1	2	0	0	2021-09-07 10:57:55.801265	2021-09-07 10:57:55.801265
63	90	10	0	0	80	40	0	0	COMMON	mk46.yml	LIGHT_MACHINE_GUN	10	TN-33Y	MK-46 Light Machine Gun	78	3717	LIGHT_MACHINE_GUN	3.81	3278	20	21	3	30	0	2	2	3	33	2	818	149	186	1	2	9	2	8	7	0	2	3	49	2	1	2	0	0	2	0	9	0	2	0	0	1	0	2021-09-07 10:58:07.221686	2021-09-07 10:58:07.221686
64	23.010099	0	0	0	50	0	0	0	COMMON	desert-eagle.yml	PISTOL	8	LX Industries	Desert Eagle	47	1	PISTOL	0	7	6	11	1	35	1	2	1	0	2	23	2	2	18	20	0	2	1	2	4	0	1	17	1	1	7	0	1	2	1	0	6	1	0	13	2	0	0	2021-09-07 10:58:18.857191	2021-09-07 10:58:18.857191
65	90	10	0	0	90	40	9	0	COMMON	saiga12.yml	SHOTGUN	3	R.S.S.	SAIGA-12	39	40	SHOTGUN	70	6	2	34	1	1	0	1	7	4.3	1	1	1390	1	1	20	0	1	1	0	1	1	1	1	0	1	1	0	1	1	0	0	1	0	1	0	0	1	0	2021-09-07 11:35:16.81688	2021-09-07 11:35:16.81688
66	0	20	90	90	10	20	90	90	COMMON	psg1.yml	SNIPER	6	PF-TDN	PSG1	8	92	SNIPER	0.81	12	16	19	3	1	0	13	1	1	9	18	1818	500	3	49	1	1	1	1	0	1	1	1	0	1	0	0	1	1	1	0	1	0	1	1	1	0	0	2021-09-07 11:37:13.543964	2021-09-07 11:37:13.543964
67	90	10	0	0	90	40	9	0	COMMON	saiga12.yml	SHOTGUN	3	R.S.S.	SAIGA-12	39	1	SHOTGUN	70	1	2	34	1	1	1	53	2	4.3	3	10	918	10	1	34	0	0	1	1	0	1	1	0	1	0	0	0	0	0	0	0	0	0	0	0	0	0	0	2021-09-07 11:38:57.601084	2021-09-07 11:38:57.601084
68	80	40	13	5	50	25	10	1	COMMON	ppk.yml	PISTOL	8	TN3 SMITH-x Industrial	Silenced PPK	42	124	PISTOL	20.5	9	1	1	1	20	0	43	1	0	1	1	40	2	3	16	1	0	1	1	1	1	1	1	1	0	0	0	1	1	1	0	1	0	0	1	1	1	0	2021-09-07 11:40:06.99814	2021-09-07 11:40:06.99814
69	80	40	13	5	50	25	10	1	COMMON	uzi.yml	PISTOL	8	TN3 SMITH-x Industrial	A Uzi	43	75	MACHINE_PISTOL	14	13	1	8	1	20	1	35	1	0	3	1	40	1	3	16	0	1	1	1	1	0	1	1	1	1	1	0	0	1	1	0	1	1	1	1	0	0	0	2021-09-07 11:41:52.537095	2021-09-07 11:41:52.537095
70	90	10	0	0	90	40	9	0	COMMON	saiga12.yml	SHOTGUN	3	R.S.S.	SAIGA-12	39	66	SHOTGUN	1	7	2	1	1	80	0	53	2	5	2	10	918	10	1	20	1	1	0	1	1	0	0	1	0	1	1	0	0	0	1	0	1	1	1	1	1	0	0	2021-09-07 13:53:42.597594	2021-09-07 13:53:42.597594
71	90	10	0	0	80	40	0	0	COMMON	mk46.yml	LIGHT_MACHINE_GUN	10	TN-33Y	MK-46 Light Machine Gun	78	999	LIGHT_MACHINE_GUN	3.81	2564	20	1	1	30	0	24	13	4.3	44	180	818	53	40	1	1	0	1	1	1	0	1	0	1	0	1	1	1	1	0	0	1	1	0	0	1	1	0	2021-09-07 13:54:19.914426	2021-09-07 13:54:19.914426
72	90	10	0	0	90	40	9	0	COMMON	sasg12.yml	SHOTGUN	3	R.S.S.	SASG-12	6	1	SHOTGUN	136	6	2	34	1	80	1	53	1	4.3	2	1	1	10	1	1	1	0	1	1	1	1	0	1	1	0	1	1	1	0	0	0	0	1	1	0	1	1	0	2021-09-07 14:06:46.882602	2021-09-07 14:06:46.882602
73	90	10	0	0	80	40	0	0	COMMON	mk46.yml	LIGHT_MACHINE_GUN	10	TN-33Y	MK-46 Light Machine Gun	78	999	LIGHT_MACHINE_GUN	3.81	999	20	22	1	30	0	13	7	4.3	33	180	818	20	40	140	1	1	1	1	0	1	1	1	0	0	1	1	1	0	1	0	0	0	1	0	1	1	0	2021-09-07 14:07:05.080684	2021-09-07 14:07:05.080684
74	80	40	13	5	50	25	10	1	COMMON	ppk.yml	PISTOL	8	TN3 SMITH-x Industrial	Silenced PPK	42	75	PISTOL	20.5	9	1	1	1	1	0	33	1	0	1	80	40	4	7	19	2	0	1	1	0	2	1	2	1	0	1	1	0	0	0	0	1	2	1	2	1	0	0	2021-09-07 14:07:13.03584	2021-09-07 14:07:13.03584
75	10	10	10	10	10	10	10	10	COMMON	tar21.yml	ASSAULT_RIFLE	4	SK-10	TAR-21 Assault Rifle	30	220	ASSAULT_RIFLE	4.23	8	2	14	1	5	3.14	1	3	4.3	11	80	2	2	1	1	0	1	1	1	1	1	4	0	1	1	1	1	0	1	0	0	1	1	1	0	2	2	0	2021-09-07 14:08:06.897732	2021-09-07 14:08:06.897732
76	80	40	13	5	50	25	10	1	COMMON	uzi.yml	PISTOL	8	TN3 SMITH-x Industrial	A Uzi	43	1	MACHINE_PISTOL	10.5	13	1	5	1	20	1	33	1	1	1	80	2	2	1	1	1	1	2	1	1	1	1	1	1	2	2	0	1	2	1	0	0	2	1	0	1	1	0	2021-09-07 14:08:08.725752	2021-09-07 14:08:08.725752
77	90	10	0	0	80	40	0	0	COMMON	mk46.yml	LIGHT_MACHINE_GUN	10	TN-33Y	MK-46 Light Machine Gun	78	999	LIGHT_MACHINE_GUN	3.81	1	20	14	1	30	1	1	8	4.3	33	353	818	45	100	140	1	1	1	1	0	1	1	2	1	0	1	0	1	1	1	0	0	1	0	0	2	2	0	2021-09-07 14:09:34.865872	2021-09-07 14:09:34.865872
78	90	10	0	0	80	40	0	0	COMMON	p90.yml	SUB_MACHINE_GUN	5	Heckler and Koch	Heckler and Koch P90	21	1	SUB_MACHINE_GUN	1	1	1	14	1	84	0	30	3	4.3	1	1	818	20	10	6	1	1	1	2	1	1	1	1	1	1	1	1	0	1	1	0	4	0	1	1	1	0	0	2021-09-07 14:09:53.320895	2021-09-07 14:09:53.320895
79	80	40	13	5	50	25	10	1	COMMON	czp10.yml	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	2	PISTOL	20.5	21	1	5	3	20	1	33	1	0	8	276	40	2	4	16	2	1	1	1	0	0	1	1	2	1	1	1	2	1	1	0	0	0	1	0	1	0	0	2021-09-07 14:10:19.630963	2021-09-07 14:10:19.630963
80	90	10	0	0	90	40	9	0	COMMON	sasg12.yml	SHOTGUN	3	R.S.S.	SASG-12	6	154	SHOTGUN	70	6	2	8	1	2	0	2	5	4.3	8	2	918	10	2	20	2	2	87	1	1	9	0	8	9	4	0	0	3	1	0	0	2	0	1	6	1	1	0	2021-09-07 15:34:49.372159	2021-09-07 15:34:49.372159
81	90	10	0	0	90	40	9	0	COMMON	sasg12.yml	SHOTGUN	3	R.S.S.	SASG-12	6	40	SHOTGUN	1082	1	2	34	2	329	0	53	2	4.3	2	81	918	2	3	2	8	2	1	2	1	1	1	1	1	3	1	1	0	4	2	0	4	4	1	4	2	1	0	2021-09-07 15:44:26.523537	2021-09-07 15:44:26.523537
82	90	10	0	0	80	40	0	0	COMMON	ump45.yml	SUB_MACHINE_GUN	5	PN/P	UMP-45	32	400	SUB_MACHINE_GUN	131	120	1	14	1	2	2	13	2	1	5	180	2	1	6	58	1	2	1	3	0	0	2	1	3	1	1	0	1	3	4	0	0	1	0	3	5	2	0	2021-09-07 15:47:20.881538	2021-09-07 15:47:20.881538
83	90	10	0	0	90	40	9	0	COMMON	sasg12.yml	SHOTGUN	3	R.S.S.	SASG-12	6	40	SHOTGUN	70	1	2	1	1	80	1	1	5	2	2	2	918	7	11	20	2	3	7	0	6	1	2	0	18	0	2	3	2	1	0	0	2	2	4	0	0	2	0	2021-09-11 02:31:04.075813	2021-09-11 02:31:04.075813
84	80	40	13	5	50	25	10	1	COMMON	uzi.yml	PISTOL	8	TN3 SMITH-x Industrial	A Uzi	43	1	MACHINE_PISTOL	2	50	1	16	2	20	0	1	2	2	5	250	2	5	2	16	1	2	2	1	2	71	0	2	2	0	5	1	0	0	3	0	4	0	30	1	3	2	0	2021-09-11 02:31:11.598437	2021-09-11 02:31:11.598437
85	90	10	0	0	80	40	0	0	COMMON	p90.yml	SUB_MACHINE_GUN	5	Heckler and Koch	Heckler and Koch P90	21	400	SUB_MACHINE_GUN	2	31	1	14	2	2	0	18	1	1	2	180	818	76	10	2	3	0	7	0	22	3	1	21	1	2	0	1	3	6	0	0	4	11	0	2	0	17	0	2021-09-11 22:08:15.984399	2021-09-11 22:08:15.984399
86	90	10	0	0	90	40	9	0	COMMON	sasg12.yml	SHOTGUN	3	R.S.S.	SASG-12	6	166	SHOTGUN	2	26	2	2	2	80	3	53	2	2	2	15	2778	10	7	37	1	1	2	0	4	3	10	1	0	1	2	0	4	2	9	0	0	1	0	0	2	1	0	2021-09-11 22:34:57.993684	2021-09-11 22:34:57.993684
87	90	10	0	0	90	40	9	0	COMMON	sasg12.yml	SHOTGUN	3	R.S.S.	SASG-12	6	40	SHOTGUN	101	6	2	1	1	80	0	53	2	4.3	2	10	918	1	1	1	1	1	0	1	1	0	1	1	1	1	1	0	1	1	0	0	1	0	1	0	1	1	0	2021-09-11 22:43:30.863845	2021-09-11 22:43:30.863845
88	90	10	0	0	90	40	9	0	COMMON	sasg12.yml	SHOTGUN	3	R.S.S.	SASG-12	6	40	SHOTGUN	70	6	2	140	2	2	0	53	7	31	3	10	1	17	5	20	1	2	2	1	2	2	0	1	3	2	2	2	0	0	3	0	3	8	3	2	0	0	0	2021-09-14 23:05:31.476199	2021-09-14 23:05:31.476199
89	0	20	90	90	10	20	90	90	COMMON	psg1.yml	SNIPER	6	PF-TDN	PSG1	8	70	SNIPER	1	16	16	19	3	82	8	60	1	4.3	17	10	1818	2	2	29	2	12	0	0	2	4	4	3	1	1	2	4	2	9	2	0	0	10	2	0	2	0	0	2021-09-14 23:08:43.094794	2021-09-14 23:08:43.094794
90	90	10	0	0	90	40	9	0	COMMON	sasg12.yml	SHOTGUN	3	R.S.S.	SASG-12	6	2	SHOTGUN	7	46	2	34	1	80	0	53	6	4.3	5	10	2	10	7	811	0	0	0	0	0	0	0	0	0	4	4	9	2	4	0	0	2	0	0	2	2	1	0	2021-09-14 23:18:08.304107	2021-09-14 23:18:08.304107
114	90	10	0	0	90	40	9	0	COMMON	sasg12.yml	SHOTGUN	3	R.S.S.	SASG-12	6	100	SHOTGUN	70	2	2	34	1	80	2	1	2	2	1	10	1	10	8	20	2	1	2	2	1	2	1	2	0	0	0	1	0	0	2	0	1	1	1	1	1	1	0	2021-09-16 23:36:13.402217	2021-09-16 23:36:13.402217
91	23.010099	0	0	0	50	0	0	0	COMMON	desert-eagle.yml	PISTOL	8	LX Industries	Desert Eagle	47	50	PISTOL	0	2	6	2	1	146	2	2	1	4	4	2	8	2	5	20	1	2	2	8	6	2	1	12	0	1	3	0	1	0	1	0	2	0	1	1	1	15	0	2021-09-14 23:18:30.652122	2021-09-14 23:18:30.652122
92	90	10	0	0	90	40	9	0	COMMON	saiga12.yml	SHOTGUN	3	R.S.S.	SAIGA-12	39	40	SHOTGUN	1	6	2	34	1	118	0	53	2	1	2	10	1	12	3	20	1	0	1	1	1	1	1	1	1	1	0	0	0	1	0	0	0	0	1	0	1	0	0	2021-09-14 23:35:49.479445	2021-09-14 23:35:49.479445
93	80	40	13	5	50	25	10	1	COMMON	ppk.yml	PISTOL	8	TN3 SMITH-x Industrial	Silenced PPK	42	75	PISTOL	38	1	1	5	1	1	0	1	1	0	3	93	40	3	3	23	1	0	1	1	1	1	0	0	1	1	1	1	0	1	1	0	0	0	1	1	1	1	0	2021-09-14 23:35:59.910515	2021-09-14 23:35:59.910515
94	90	10	0	0	90	40	9	0	COMMON	m3.yml	SHOTGUN	3	GBNT-3	M3	28	40	SHOTGUN	125	6	2	34	1	144	0	53	2	1	1	1	918	10	1	1	1	1	1	1	0	0	1	0	1	0	0	0	0	0	0	0	0	1	0	0	1	0	0	2021-09-14 23:39:15.773517	2021-09-14 23:39:15.773517
95	90	10	0	0	90	40	9	0	COMMON	saiga12.yml	SHOTGUN	3	R.S.S.	SAIGA-12	39	40	SHOTGUN	70	6	2	1	1	1	2	1	2	6	2	10	1719	1	3	84	1	2	1	2	1	1	0	1	0	2	1	0	1	2	1	0	1	1	0	0	1	0	0	2021-09-14 23:42:41.011863	2021-09-14 23:42:41.011863
96	90	10	0	0	80	40	0	0	COMMON	ump45.yml	SUB_MACHINE_GUN	5	PN/P	UMP-45	32	400	SUB_MACHINE_GUN	30.81	46	1	14	1	30	1	13	3	4.3	2	8	2	20	1	19	2	8	0	2	1	1	2	2	2	1	0	2	43	3	1	0	2	0	0	1	3	0	0	2021-09-14 23:42:59.549552	2021-09-14 23:42:59.549552
97	90	10	0	0	90	40	9	0	COMMON	sasg12.yml	SHOTGUN	3	R.S.S.	SASG-12	6	40	SHOTGUN	70	1	2	34	3	80	0	53	2	10	4	198	2563	10	7	4	1	2	2	1	2	0	2	3	1	2	1	1	2	2	0	0	1	5	0	2	0	0	0	2021-09-14 23:43:28.098499	2021-09-14 23:43:28.098499
98	0	20	90	90	10	20	90	90	COMMON	xm109.yml	SNIPER	6	Heckler and Koch	XM109 Sniper Rifle	13	70	SNIPER	2	1	2	19	3	450	0	13	12	4.3	22	2	1818	6001	3	159	3	1	7	3	2	0	1	2	2	8	0	1	2	1	3	0	1	2	1	1	2	0	0	2021-09-14 23:43:44.450572	2021-09-14 23:43:44.450572
99	80	40	13	5	50	25	10	1	COMMON	glock.yml	PISTOL	8	TN3 SMITH-x Industrial	Glock	41	75	PISTOL	20.5	2	1	5	1	20	0	33	2	2	2	80	329	6	1	33	3	1	1	1	2	0	0	1	2	0	2	0	3	1	2	0	2	2	2	2	0	1	0	2021-09-14 23:44:17.215558	2021-09-14 23:44:17.215558
100	90	10	0	0	90	40	9	0	COMMON	sasg12.yml	SHOTGUN	3	R.S.S.	SASG-12	6	127	SHOTGUN	70	12	2	34	18	80	2	7	2	7	2	10	918	10	3	5	1	2	2	1	0	1	1	1	0	4	1	1	2	2	4	0	0	1	0	4	2	6	0	2021-09-14 23:44:37.180693	2021-09-14 23:44:37.180693
101	80	40	13	5	50	25	10	1	COMMON	uzi.yml	PISTOL	8	TN3 SMITH-x Industrial	A Uzi	43	75	MACHINE_PISTOL	51	13	1	5	1	2	1	33	8	1	3	80	144	1	3	72	4	2	4	2	2	1	3	0	0	2	3	2	1	2	1	0	0	1	1	2	1	2	0	2021-09-14 23:44:50.870736	2021-09-14 23:44:50.870736
102	90	10	0	0	80	40	0	0	COMMON	mp9.yml	MACHINE_PISTOL	9	Heckler and Koch	MP9	38	1373	MACHINE_PISTOL	30.81	31	1	1	2	30	2	13	2	4	12	1	2774	72	1	2	13	2	0	2	0	1	1	2	6	3	0	2	4	1	0	0	1	2	0	1	1	2	0	2021-09-14 23:44:58.487728	2021-09-14 23:44:58.487728
103	90	10	0	0	80	40	0	0	COMMON	p90.yml	SUB_MACHINE_GUN	5	Heckler and Koch	Heckler and Koch P90	21	400	SUB_MACHINE_GUN	2	89	1	62	1	30	0	13	3	33	2	180	1	67	10	2	1	3	15	2	2	2	1	9	2	2	0	3	0	2	4	0	2	0	2	2	2	2	0	2021-09-14 23:45:13.896704	2021-09-14 23:45:13.896704
104	80	40	13	5	50	25	10	1	COMMON	uzi.yml	PISTOL	8	TN3 SMITH-x Industrial	A Uzi	43	195	MACHINE_PISTOL	10.5	2	1	2	2	1	2	48	1	2	12	1	144	2	2	2	2	6	0	0	3	1	1	8	1	0	1	0	2	2	1	0	1	2	1	1	0	1	0	2021-09-14 23:45:23.846735	2021-09-14 23:45:23.846735
105	0	20	90	90	10	20	90	90	COMMON	xm109.yml	SNIPER	6	Heckler and Koch	XM109 Sniper Rifle	13	70	SNIPER	1	115	2	1	3	1540	1	23	4	4.3	9	10	1818	3138	3	7	0	1	1	0	0	4	1	2	1	0	1	2	7	1	0	0	5	2	0	4	10	1	0	2021-09-14 23:45:34.064608	2021-09-14 23:45:34.064608
106	90	10	0	0	90	40	9	0	COMMON	saiga12.yml	SHOTGUN	3	R.S.S.	SAIGA-12	39	2	SHOTGUN	70	6	2	93	24	24	5	53	2	2	3	40	918	8	2	3	3	9	12	3	35	2	3	0	6	80	4	0	2	0	12	0	3	2	3	2	2	18	0	2021-09-16 02:07:36.873686	2021-09-16 02:07:36.873686
107	10	10	10	10	10	10	10	10	COMMON	m4.yml	ASSAULT_RIFLE	4	Standard Issue	M4 Assault Rifle	26	3	ASSAULT_RIFLE	4	24	2	249	17	123	4	2	3	3	2	80	24	4	6	10	4	6	19	2	0	8	5	2	7	0	0	4	63	10	9	0	0	4	4	2	59	13	0	2021-09-16 02:07:57.898864	2021-09-16 02:07:57.898864
108	90	10	0	0	80	40	0	0	COMMON	mp5.yml	SUB_MACHINE_GUN	5	Heckler and Koch	MP5	5	2230	SUB_MACHINE_GUN	75	3	1	14	4	7	0	8	2	4.3	5	180	3825	54	15	199	2	4	89	3	17	4	4	6	6	4	2	0	4	3	25	0	3	5	2	175	3	0	0	2021-09-16 02:08:02.030809	2021-09-16 02:08:02.030809
109	90	10	0	0	90	40	9	0	COMMON	saiga12.yml	SHOTGUN	3	R.S.S.	SAIGA-12	39	141	SHOTGUN	4	2	2	34	1	3	4	53	2	4.3	81	2	4	31	9	123	3	4	0	3	2	3	17	3	0	3	0	5	3	33	18	0	3	3	4	0	161	4	0	2021-09-16 02:08:15.453854	2021-09-16 02:08:15.453854
110	0	20	90	90	10	20	90	90	COMMON	psg1.yml	SNIPER	6	PF-TDN	PSG1	8	47	SNIPER	0.81	7	21	549	7	61	6	65	19	4.3	4	10	1818	3218	21	11	3	3	3	4	2	3	0	3	19	0	5	3	4	4	0	0	3	3	2	2	2	0	0	2021-09-16 02:08:20.239637	2021-09-16 02:08:20.239637
111	90	10	0	0	90	40	9	0	COMMON	saiga12.yml	SHOTGUN	3	R.S.S.	SAIGA-12	39	40	SHOTGUN	70	1	2	43	1	80	0	1	2	1	2	10	1323	12	5	38	0	0	1	1	1	1	1	1	1	0	0	1	1	1	1	0	1	1	0	1	1	1	0	2021-09-16 23:27:45.188054	2021-09-16 23:27:45.188054
112	80	40	13	5	50	25	10	1	COMMON	czp10.yml	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	75	PISTOL	20.5	15	1	5	1	1	0	1	1	0	3	80	40	2	3	28	1	1	1	1	1	0	1	1	1	0	0	0	0	0	0	0	0	0	0	0	0	0	0	2021-09-16 23:35:43.931376	2021-09-16 23:35:43.931376
113	80	40	13	5	50	25	10	1	COMMON	ppk.yml	PISTOL	8	TN3 SMITH-x Industrial	Silenced PPK	42	206	PISTOL	29	2	1	5	1	20	0	33	2	1	1	1	40	2	3	1	0	0	1	1	2	1	1	1	1	1	0	0	1	4	2	0	1	0	1	1	1	1	0	2021-09-16 23:35:57.520318	2021-09-16 23:35:57.520318
116	10	10	10	10	10	10	10	10	COMMON	tar21.yml	ASSAULT_RIFLE	4	SK-10	TAR-21 Assault Rifle	30	1	ASSAULT_RIFLE	2	2	2	1	4	45	2	4	3	14	5	4	2	2	1	10	3	1	0	1	10	4	3	0	2	2	0	7	0	1	0	0	0	2	4	1	0	3	0	2021-09-16 23:36:44.521348	2021-09-16 23:36:44.521348
118	90	10	0	0	90	40	9	0	COMMON	sasg12.yml	SHOTGUN	3	R.S.S.	SASG-12	6	40	SHOTGUN	3	6	2	34	1	1	2	1	1	4.3	2	10	1	10	3	127	2	3	3	2	0	1	2	8	0	1	1	1	0	0	6	0	2	1	1	0	2	3	0	2021-09-16 23:37:05.772357	2021-09-16 23:37:05.772357
119	90	10	0	0	90	40	9	0	COMMON	m3.yml	SHOTGUN	3	GBNT-3	M3	28	2	SHOTGUN	70	6	2	1	1	155	1	3	3	4.3	2	34	918	74	13	67	4	4	0	15	0	2	0	5	6	0	1	1	1	1	1	0	2	2	1	4	4	2	0	2021-09-16 23:37:18.426344	2021-09-16 23:37:18.426344
120	23.010099	0	0	0	50	0	0	0	COMMON	desert-eagle.yml	PISTOL	8	LX Industries	Desert Eagle	47	50	PISTOL	4	1	6	11	1	34	2	33	1	0	21	8	1	1	2	20	0	4	2	2	0	2	13	2	1	0	2	2	2	0	4	0	13	3	2	0	4	2	0	2021-09-16 23:37:20.712351	2021-09-16 23:37:20.712351
121	90	10	0	0	80	40	0	0	COMMON	hk21.yml	LIGHT_MACHINE_GUN	10	TN-33Y	HK-21 Light Machine Gun	33	2	LIGHT_MACHINE_GUN	3.81	1	20	2	1	1	0	2	2	4.3	33	324	1344	20	80	1	0	1	6	0	1	5	2	1	1	2	1	1	1	0	2	0	2	1	0	2	4	0	0	2021-09-16 23:41:05.302512	2021-09-16 23:41:05.302512
122	90	10	0	0	80	40	0	0	COMMON	mk46.yml	LIGHT_MACHINE_GUN	10	TN-33Y	MK-46 Light Machine Gun	78	1	LIGHT_MACHINE_GUN	3.81	19206	20	14	9	30	0	13	27	4.3	1	438	818	1	40	558	1	2	0	1	9	2	1	5	1	14	0	2	4	0	1	0	0	1	2	4	4	2	0	2021-09-16 23:41:13.858541	2021-09-16 23:41:13.858541
123	0	20	90	90	10	20	90	90	COMMON	psg1.yml	SNIPER	6	PF-TDN	PSG1	8	1	SNIPER	0.81	2	21	2	3	252	4	13	4	4.3	8	23	1818	1	21	9	14	1	3	1	2	0	1	2	0	2	0	2	0	4	1	0	4	2	1	1	2	1	0	2021-09-16 23:41:23.490425	2021-09-16 23:41:23.490425
124	10	10	10	10	10	10	10	10	COMMON	g36c.yml	ASSAULT_RIFLE	4	Heckler & Koch	G36C Assault Rifle	46	220	ASSAULT_RIFLE	13	2	2	14	2	45	1	3	3	14	5	80	1	2	1	2	2	4	8	1	0	1	0	3	1	1	2	1	0	0	0	0	2	1	0	2	0	0	0	2021-09-16 23:41:37.665403	2021-09-16 23:41:37.665403
125	80	40	13	5	50	25	10	1	COMMON	uzi.yml	PISTOL	8	TN3 SMITH-x Industrial	A Uzi	43	75	MACHINE_PISTOL	10.5	13	1	5	1	1	3	1	5	3	5	308	1	3	12	1	1	2	2	5	0	2	21	13	1	2	4	2	2	9	2	0	0	1	4	0	2	6	0	2021-09-16 23:41:43.832588	2021-09-16 23:41:43.832588
126	80	40	13	5	50	25	10	1	COMMON	magnum-revolver.yml	PISTOL	8	TN3 SMITH-x Industrial	Magnum Revolver	40	75	PISTOL	20.5	6	1	2	2	3	3	2	1	2	60	80	40	7	2	187	8	3	14	3	3	2	11	0	15	0	3	4	2	12	5	0	0	3	24	0	3	0	0	2021-09-16 23:41:47.188541	2021-09-16 23:41:47.188541
127	10	10	10	10	10	10	10	10	COMMON	g36c.yml	ASSAULT_RIFLE	4	Heckler & Koch	G36C Assault Rifle	46	220	ASSAULT_RIFLE	518	91	2	2	4	223	2	3	3	4.3	5	340	79	2	3	10	16	6	0	0	5	3	3	0	4	2	5	0	5	2	3	0	0	3	3	8	0	0	0	2021-09-16 23:41:49.369475	2021-09-16 23:41:49.369475
128	90	10	0	0	80	40	0	0	COMMON	belt-fed-minigun.yml	LIGHT_MACHINE_GUN	10	TN-33Y	A TN-33Y Belt-Fed Minigun	23	999	LIGHT_MACHINE_GUN	19	999	20	14	2	2	2	2	39	11	33	3	2	8	3	3	9	3	2	8	2	8	0	5	3	0	7	0	4	2	0	0	3	0	14	0	0	0	0	2021-09-16 23:41:54.99946	2021-09-16 23:41:54.99946
129	80	40	13	5	50	25	10	1	COMMON	magnum-revolver.yml	PISTOL	8	TN3 SMITH-x Industrial	Magnum Revolver	40	779	PISTOL	2	6	1	5	5	20	2	140	3	3	3	80	3	5	15	3	12	0	3	2	3	10	2	4	2	2	1	0	4	42	3	0	3	3	3	0	3	2	0	2021-09-16 23:41:58.892622	2021-09-16 23:41:58.892622
130	10	10	10	10	10	10	10	10	COMMON	m4.yml	ASSAULT_RIFLE	4	Standard Issue	M4 Assault Rifle	26	9	ASSAULT_RIFLE	3	8	2	80	4	3	17	3	2	12	5	408	49	3	2	57	2	3	0	3	5	0	2	21	0	3	3	44	2	3	2	0	4	3	0	2	2	14	0	2021-09-16 23:42:00.832379	2021-09-16 23:42:00.832379
131	90	10	0	0	80	40	0	0	COMMON	mp5.yml	SUB_MACHINE_GUN	5	Heckler and Koch	MP5	5	400	SUB_MACHINE_GUN	2	31	1	63	3	2	2	3	2	4.3	5	613	2	20	17	6	2	5	0	54	2	2	3	0	10	3	3	19	7	0	12	0	2	2	3	0	2	2	0	2021-09-16 23:42:04.939434	2021-09-16 23:42:04.939434
132	80	40	13	5	50	25	10	1	COMMON	ppk.yml	PISTOL	8	TN3 SMITH-x Industrial	Silenced PPK	42	2	PISTOL	2	9	1	3	2	20	3	33	1	12	3	3	179	164	3	16	2	0	3	3	17	3	3	3	3	3	2	0	2	2	4	0	2	2	0	23	3	3	0	2021-09-16 23:42:07.309542	2021-09-16 23:42:07.309542
133	80	40	13	5	50	25	10	1	COMMON	magnum-revolver.yml	PISTOL	8	TN3 SMITH-x Industrial	Magnum Revolver	40	3	PISTOL	4	4	1	92	4	20	0	33	2	6	3	337	3	3	13	16	84	7	22	4	3	4	2	2	0	3	0	2	0	4	4	0	41	6	0	60	3	3	0	2021-09-16 23:42:08.832497	2021-09-16 23:42:08.832497
134	90	10	0	0	90	40	9	0	COMMON	saiga12.yml	SHOTGUN	3	R.S.S.	SAIGA-12	39	40	SHOTGUN	208	4	2	34	1	2	0	53	3	4.3	2	8	918	58	3	4	6	3	3	6	0	0	2	10	6	7	3	4	5	4	0	0	0	0	0	4	2	2	0	2021-09-16 23:42:10.128434	2021-09-16 23:42:10.128434
135	90	10	0	0	80	40	0	0	COMMON	hk21.yml	LIGHT_MACHINE_GUN	10	TN-33Y	HK-21 Light Machine Gun	33	2	LIGHT_MACHINE_GUN	2	999	20	69	3	64	0	4	7	3	33	180	818	126	9	4	0	0	2	17	21	5	112	24	3	0	4	6	4	4	0	0	5	116	2	0	3	2	0	2021-09-16 23:42:13.915874	2021-09-16 23:42:13.915874
136	80	40	13	5	50	25	10	1	COMMON	uzi.yml	PISTOL	8	TN3 SMITH-x Industrial	A Uzi	43	4	MACHINE_PISTOL	10.5	13	1	14	4	3	6	4	1	3	3	4	2	2	10	16	3	4	91	0	4	32	0	4	3	13	13	4	19	0	3	0	6	6	2	0	0	2	0	2021-09-16 23:42:15.757476	2021-09-16 23:42:15.757476
137	90	10	0	0	80	40	0	0	COMMON	fmg9.yml	SUB_MACHINE_GUN	5	DXGR-1	FMG-9	5	11531	SUB_MACHINE_GUN	170	31	1	3	1	30	4	14	4	9	34	180	818	6	6	3	2	2	4	5	11	2	4	2	13	0	3	24	0	3	2	0	6	0	3	2	3	0	0	2021-09-16 23:42:17.37751	2021-09-16 23:42:17.37751
138	80	40	13	5	50	25	10	1	COMMON	uzi.yml	PISTOL	8	TN3 SMITH-x Industrial	A Uzi	43	75	MACHINE_PISTOL	10.5	13	1	5	1	4	0	33	1	0	3	80	40	11	13	16	5	2	1367	4	4	0	21	0	3	0	3	2	0	0	76	0	2	2	4	0	57	3	0	2021-09-16 23:42:20.082517	2021-09-16 23:42:20.082517
139	90	10	0	0	80	40	0	0	COMMON	hk21.yml	LIGHT_MACHINE_GUN	10	TN-33Y	HK-21 Light Machine Gun	33	2	LIGHT_MACHINE_GUN	11	3	20	47	3	30	0	13	3	3	4	2	818	66	40	140	514	4	0	10	3	3	4	0	49	5	26	2	2	19	8	0	4	2	3	0	4	0	0	2021-09-16 23:42:21.261413	2021-09-16 23:42:21.261413
142	90	10	0	0	90	40	9	0	COMMON	saiga12.yml	SHOTGUN	3	R.S.S.	SAIGA-12	39	40	SHOTGUN	70	20	2	4	4	38	0	53	5	4.3	19	10	18	2	3	20	215	3	2	0	2	26	4	2	2	0	3	3	5	3	23	0	12	12	2	4	5	3	0	2021-09-17 00:06:22.501847	2021-09-17 00:06:22.501847
143	90	10	0	0	80	40	0	0	COMMON	fmg9.yml	SUB_MACHINE_GUN	5	DXGR-1	FMG-9	5	400	SUB_MACHINE_GUN	30.81	3	1	14	1	206	4	3	3	80	66	2	818	20	18	6	2	3	2	3	4	12	2	0	2	22	0	6	17	21	6	0	0	2	4	26	2	0	0	2021-09-17 00:06:28.107491	2021-09-17 00:06:28.107491
144	90	10	0	0	80	40	0	0	COMMON	hk21.yml	LIGHT_MACHINE_GUN	10	TN-33Y	HK-21 Light Machine Gun	33	999	LIGHT_MACHINE_GUN	2	13	20	271	1	5	4	4	7	3	3	5	2	4	5	140	2	0	0	2	3	4	4	5	14	0	16	16	0	6	7	0	4	2	9	3	4	0	0	2021-09-17 00:06:53.128907	2021-09-17 00:06:53.128907
145	90	10	0	0	80	40	0	0	COMMON	p90.yml	SUB_MACHINE_GUN	5	Heckler and Koch	Heckler and Koch P90	21	1746	SUB_MACHINE_GUN	10	190	1	3	1	30	0	13	47	4.3	6	4	818	20	57	2	4	10	4	13	6	72	5	2	0	4	0	2	3	4	4	0	29	3	5	4	4	6	0	2021-09-17 00:07:08.100927	2021-09-17 00:07:08.100927
146	90	10	0	0	90	40	9	0	COMMON	saiga12.yml	SHOTGUN	3	R.S.S.	SAIGA-12	39	311	SHOTGUN	5	32	2	34	2	4	0	3	4	4.3	2	4	19	10	2	12	0	74	4	0	2	17	3	32	20	2	16	546	4	3	5	0	2	6	2	2	4	2	0	2021-09-17 00:19:32.432101	2021-09-17 00:19:32.432101
147	90	10	0	0	90	40	9	0	COMMON	saiga12.yml	SHOTGUN	3	R.S.S.	SAIGA-12	39	4	SHOTGUN	489	44	2	4	3	2	0	53	6	19	8	3	3	5	3	4	3	4	5	20	0	21	3	19	4	4	0	5	3	281	5	0	3	5	3	0	2	7	0	2021-09-17 00:19:36.334133	2021-09-17 00:19:36.334133
148	90	10	0	0	80	40	0	0	COMMON	ump45.yml	SUB_MACHINE_GUN	5	PN/P	UMP-45	32	400	SUB_MACHINE_GUN	30.81	20	1	2	1	68	4	13	3	21	36	180	6479	20	2	208	3	11	56	15	0	0	2	2	3	0	0	4	45	22	0	0	5	5	0	4	5	3	0	2021-09-17 00:19:37.303205	2021-09-17 00:19:37.303205
149	90	10	0	0	90	40	9	0	COMMON	sasg12.yml	SHOTGUN	3	R.S.S.	SASG-12	6	3	SHOTGUN	3	3	2	34	3	80	4	178	26	3	2	181	918	3	4	20	4	6	3	4	3	51	4	25	3	4	3	2	3	0	2	0	4	3	0	0	0	3	0	2021-09-17 00:46:53.68857	2021-09-17 00:46:53.68857
150	90	10	0	0	90	40	9	0	COMMON	saiga12.yml	SHOTGUN	3	R.S.S.	SAIGA-12	39	146	SHOTGUN	387	6	2	34	1	80	4	53	3	4	2	10	918	21	11	3	0	0	4	2	12	0	14	3	6	2	3	3	2	2	4	0	4	4	0	0	2	24	0	2021-09-17 00:52:59.358813	2021-09-17 00:52:59.358813
151	90	10	0	0	90	40	9	0	COMMON	sasg12.yml	SHOTGUN	3	R.S.S.	SASG-12	6	2	SHOTGUN	70	47	2	34	1	628	3	53	2	5	2	5	7	65	3	4	15	6	6	2	3	0	9	0	0	0	197	4	0	0	2	0	38	2	21	2	5	2	0	2021-09-17 01:33:01.729627	2021-09-17 01:33:01.729627
152	10	10	10	10	10	10	10	10	COMMON	scarh.yml	ASSAULT_RIFLE	4	SK-10	SCAR-H Assault Rifle	31	5	ASSAULT_RIFLE	24	8	2	4	5	5	3.14	3	4	2	21	80	91	4	23	5	125	4	4	21	5	5	0	4	249	5	24	5	2	0	4	0	4	0	0	0	5	2	0	2021-09-17 01:33:04.75737	2021-09-17 01:33:04.75737
153	0	20	90	90	10	20	90	90	COMMON	xm109.yml	SNIPER	6	Heckler and Koch	XM109 Sniper Rifle	13	73	SNIPER	0.81	53	2	19	3	15	0	13	5	30	2	55	2	1843	15	40	16	3	3	2	3	12	0	21	2	3	145	4	0	2	2	0	4	4	15	21	2	3	0	2021-09-17 01:33:05.290392	2021-09-17 01:33:05.290392
140	0	20	90	90	10	20	90	90	COMMON	xm109.yml	SNIPER	6	Heckler and Koch	XM109 Sniper Rifle	13	70	SNIPER	15	2	39	19	4	450	0	80	4	2	246	4	1818	500	4	271	6	4	4	4	5	2	3	4	0	0	0	2	128	4	3	0	0	5	0	25	3	2	0	2021-09-16 23:42:22.548445	2021-09-16 23:42:22.548445
115	0	20	90	90	10	20	90	90	COMMON	l96aw.yml	SNIPER	6	Heckler and Koch	L96 Arctic Warfare	24	155	SNIPER	0.81	7	39	1	3	450	0	2	1	4.3	8	10	2590	6	4	209	2	1	1	2	1	1	0	7	0	2	7	2	0	3	1	0	1	5	0	2	0	2	0	2021-09-16 23:36:33.761333	2021-09-16 23:36:33.761333
117	0	20	90	90	10	20	90	90	COMMON	l96aw.yml	SNIPER	6	Heckler and Koch	L96 Arctic Warfare	24	70	SNIPER	1	8	39	19	3	1332	1	2	4	16	2	10	2	975	1	324	3	5	3	2	2	1	2	0	2	1	1	0	2	1	3	0	7	3	0	3	2	2	0	2021-09-16 23:36:54.692448	2021-09-16 23:36:54.692448
141	0	20	90	90	10	20	90	90	COMMON	xm109.yml	SNIPER	6	Heckler and Koch	XM109 Sniper Rifle	13	70	SNIPER	2	14	39	4	3	2	0	3	4	26	54	10	6340	7	8	40	19	5	3	10	88	3	21	4	4	2	2	2	4	6	4	0	5	0	4	4	0	3	0	2021-09-16 23:42:23.841449	2021-09-16 23:42:23.841449
154	90	10	0	0	90	40	9	0	COMMON	saiga12.yml	SHOTGUN	3	R.S.S.	SAIGA-12	39	40	SHOTGUN	381	6	2	34	2	80	5	53	7	4.3	2	10	2	2	13	20	4	4	4	3	3	4	0	4	3	5	7	0	3	15	18	0	2	3	18	8	4	13	0	2021-09-17 01:46:31.106286	2021-09-17 01:46:31.106286
155	90	10	0	0	90	40	9	0	COMMON	saiga12.yml	SHOTGUN	3	R.S.S.	SAIGA-12	39	254	SHOTGUN	5	25	2	34	7	2	0	25	6	19	2	18	918	10	4	20	5	3	41	4	0	5	5	5	0	0	4	2	4	5	7	0	3	6	2	34	10	5	0	2021-09-17 01:46:41.943902	2021-09-17 01:46:41.943902
156	0	20	90	90	10	20	90	90	COMMON	psg1.yml	SNIPER	6	PF-TDN	PSG1	8	70	SNIPER	4	3	21	3	129	61	6	5	34	5	18	4	1818	3159	939	33	22	0	0	38	40	120	0	30	12	2	703	0	19	0	17	0	12	0	0	14	2	2	0	2021-09-17 01:46:52.492051	2021-09-17 01:46:52.492051
157	10	10	10	10	10	10	10	10	COMMON	scarh.yml	ASSAULT_RIFLE	4	SK-10	SCAR-H Assault Rifle	31	220	ASSAULT_RIFLE	2	3	2	14	4	103	46	4	4	5	4	5	24	3	6	10	4	37	0	3	3	2	5	27	133	0	2	5	5	6	6	0	3	4	0	0	3	7	0	2021-09-17 01:47:03.78067	2021-09-17 01:47:03.78067
158	90	10	0	0	90	40	9	0	COMMON	sasg12.yml	SHOTGUN	3	R.S.S.	SASG-12	6	1	SHOTGUN	1	6	2	34	1	1	1	53	2	4.3	2	10	1453	10	1	34	1	1	1	1	0	1	0	1	1	1	0	1	0	1	1	0	1	1	0	1	0	1	0	2021-09-18 01:27:39.936028	2021-09-18 01:27:39.936028
159	0	20	90	90	10	20	90	90	COMMON	xm109.yml	SNIPER	6	Heckler and Koch	XM109 Sniper Rifle	13	70	SNIPER	0.81	7	2	35	3	450	1	17	4	1	1	10	1818	500	1	1	1	0	1	1	1	0	0	0	1	1	1	1	0	1	1	0	1	1	1	0	1	1	0	2021-09-18 01:28:53.380809	2021-09-18 01:28:53.380809
160	90	10	0	0	80	40	0	0	COMMON	mk46.yml	LIGHT_MACHINE_GUN	10	TN-33Y	MK-46 Light Machine Gun	78	999	LIGHT_MACHINE_GUN	4	999	20	14	1	2	0	13	1	2	33	180	1	1	40	173	1	1	1	0	1	0	0	1	1	1	1	1	1	0	0	0	1	1	1	1	0	0	0	2021-09-18 01:30:06.037856	2021-09-18 01:30:06.037856
161	10	10	10	10	10	10	10	10	COMMON	tar21.yml	ASSAULT_RIFLE	4	SK-10	TAR-21 Assault Rifle	30	220	ASSAULT_RIFLE	4.23	8	2	35	1	45	3.14	3	1	4.3	1	1	1	1	6	1	0	1	2	0	1	2	2	1	2	0	2	2	2	1	0	0	2	1	0	0	1	0	0	2021-09-18 01:30:12.186423	2021-09-18 01:30:12.186423
162	90	10	0	0	80	40	0	0	COMMON	mp5.yml	SUB_MACHINE_GUN	5	Heckler and Koch	MP5	5	400	SUB_MACHINE_GUN	30.81	31	1	2	1	30	0	2	1	1	1	471	818	51	2	6	2	1	2	1	0	2	0	2	4	0	6	0	0	2	6	0	2	1	3	0	0	1	0	2021-09-18 01:30:18.64502	2021-09-18 01:30:18.64502
163	90	10	0	0	80	40	0	0	COMMON	mk46.yml	LIGHT_MACHINE_GUN	10	TN-33Y	MK-46 Light Machine Gun	78	4	LIGHT_MACHINE_GUN	5	999	20	2	1	70	0	1	1	1	248	1438	818	20	40	1	1	1	1	2	4	2	3	0	3	0	0	0	2	1	0	0	0	1	0	0	0	2	0	2021-09-18 01:30:39.448915	2021-09-18 01:30:39.448915
164	90	10	0	0	80	40	0	0	COMMON	ump45.yml	SUB_MACHINE_GUN	5	PN/P	UMP-45	32	1091	SUB_MACHINE_GUN	2	31	1	144	1	1	2	13	3	14	5	2	4239	48	6	6	3	2	2	1	2	0	1	0	2	1	1	3	3	0	2	0	3	0	2	2	0	1	0	2021-09-18 01:30:45.796981	2021-09-18 01:30:45.796981
165	0	20	90	90	10	20	90	90	COMMON	psg1.yml	SNIPER	6	PF-TDN	PSG1	8	70	SNIPER	0.81	7	6	43	1	61	0	53	5	4.3	1	2	2	934	1	2	3	0	1	2	2	0	1	3	0	1	0	2	0	1	1	0	2	3	1	2	0	2	0	2021-09-18 01:31:39.066818	2021-09-18 01:31:39.066818
166	90	10	0	0	90	40	9	0	COMMON	sasg12.yml	SHOTGUN	3	R.S.S.	SASG-12	6	40	SHOTGUN	1	6	2	90	4	80	1	53	2	1	2	2	3169	2	1	2	1	8	0	1	6	2	4	1	1	1	0	0	0	4	1	0	2	2	1	1	4	0	0	2021-09-18 01:31:41.977839	2021-09-18 01:31:41.977839
167	90	10	0	0	90	40	9	0	COMMON	saiga12.yml	SHOTGUN	3	R.S.S.	SAIGA-12	39	40	SHOTGUN	2	281	2	97	1	80	2	53	2	1	2	2	918	2	48	20	4	2	2	1	1	2	2	0	14	2	6	0	6	1	0	0	2	1	1	1	0	4	0	2021-09-18 01:32:18.631009	2021-09-18 01:32:18.631009
168	80	40	13	5	50	25	10	1	COMMON	uzi.yml	PISTOL	8	TN3 SMITH-x Industrial	A Uzi	43	75	MACHINE_PISTOL	69	121	1	5	1	2	2	5	2	2	1	2	40	2	2	79	1	0	3	9	3	2	2	0	3	5	4	2	2	0	2	0	2	18	0	20	0	3	0	2021-09-18 01:32:25.682925	2021-09-18 01:32:25.682925
169	0	20	90	90	10	20	90	90	COMMON	psg1.yml	SNIPER	6	PF-TDN	PSG1	8	1	SNIPER	0.81	10	6	19	10	1	1	1	5	4.3	2	10	1818	500	4	43	2	0	2	0	1	7	37	3	2	2	6	4	5	4	0	0	0	4	1	0	2	3	0	2021-09-18 01:32:40.296917	2021-09-18 01:32:40.296917
170	90	10	0	0	90	40	9	0	COMMON	sasg12.yml	SHOTGUN	3	R.S.S.	SASG-12	6	1	SHOTGUN	70	6	2	1	2	155	15	2	2	4.3	2	10	918	1	1	20	8	1	2	0	2	2	1	2	0	4	1	0	2	16	0	0	4	1	2	0	2	4	0	2021-09-18 01:34:54.528826	2021-09-18 01:34:54.528826
171	80	40	13	5	50	25	10	1	COMMON	czp10.yml	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	1	PISTOL	20.5	1	1	8	1	20	0	2	1	2	3	1	40	2	3	16	3	2	2	1	1	4	15	2	0	2	4	5	0	2	1	0	2	5	2	1	1	1	0	2021-09-18 01:35:00.363254	2021-09-18 01:35:00.363254
172	10	10	10	10	10	10	10	10	COMMON	m4.yml	ASSAULT_RIFLE	4	Standard Issue	M4 Assault Rifle	26	475	ASSAULT_RIFLE	2	1	2	2	5	194	1	1	20	4.3	5	80	5	9	6	10	0	6	2	3	1	2	1	4	1	1	3	2	2	4	3	0	1	0	2	0	4	0	0	2021-09-18 01:35:25.11697	2021-09-18 01:35:25.11697
173	90	10	0	0	90	40	9	0	COMMON	sasg12.yml	SHOTGUN	3	R.S.S.	SASG-12	6	47	SHOTGUN	70	6	2	34	1	1	1	1	1	4.3	2	1	1577	10	5	1	0	1	0	1	1	1	0	1	1	1	1	0	1	1	0	0	1	1	0	1	0	1	0	2021-09-18 01:43:24.324088	2021-09-18 01:43:24.324088
174	90	10	0	0	90	40	9	0	COMMON	sasg12.yml	SHOTGUN	3	R.S.S.	SASG-12	6	1	SHOTGUN	70	1	2	1	1	80	0	53	1	1	2	1	918	10	5	1	1	0	0	1	1	0	1	1	1	1	1	0	1	0	1	0	0	1	0	1	1	1	0	2021-09-18 01:46:20.098382	2021-09-18 01:46:20.098382
175	80	40	13	5	50	25	10	1	COMMON	magnum-revolver.yml	PISTOL	8	TN3 SMITH-x Industrial	Magnum Revolver	40	3	PISTOL	8	2	1	3	0	43	0	3	1	0	3	60	4	2	3	2	3	0	11	0	0	0	2	0	0	0	6	2	4	7	4	0	8	0	4	3	13	6	0	2022-05-19 01:26:48.903852	2022-05-19 01:26:48.903852
176	80	40	13	5	50	25	10	1	COMMON	glock.yml	PISTOL	8	TN3 SMITH-x Industrial	Glock	41	303	PISTOL	3	4	1	1	0	11	2	4	1	0	3	34	40	1	3	8	3	0	4	0	0	0	13	0	0	5	2	3	12	2	0	0	2	3	15	3	13	0	0	2022-05-19 01:38:42.487015	2022-05-19 01:38:42.487015
177	80	40	13	5	50	25	10	1	COMMON	magnum-revolver.yml	PISTOL	8	TN3 SMITH-x Industrial	Magnum Revolver	40	3	PISTOL	3	4	1	1	0	26	2	3	1	0	4	1	40	1	4	8	2	0	2	0	0	0	4	0	0	0	45	2	2	257	5	0	2	0	0	0	12	16	0	2022-05-19 04:16:57.683916	2022-05-19 04:16:57.683916
178	90	10	0	0	90	40	9	0	COMMON	sasg12.yml	SHOTGUN	3	R.S.S.	SASG-12	6	3	SHOTGUN	10	2	2	4	0	7	5	2	1	0	3	4	71	2	3	10	4	0	2	0	0	0	4	0	0	12	0	0	3	2	5	0	0	5	0	5	2	4	0	2022-05-19 14:34:23.500623	2022-05-19 14:34:23.500623
179	90	10	0	0	80	40	0	0	COMMON	mp9.yml	MACHINE_PISTOL	9	Heckler and Koch	MP9	38	11	MACHINE_PISTOL	6	1	1	4	1	30	6	5	11	0	17	5	57	2	22	2	5	0	3	0	0	0	5	0	0	0	0	0	0	0	0	0	25	3	405	0	0	4	0	2022-05-19 14:36:05.610468	2022-05-19 14:36:05.610468
180	80	40	13	5	50	25	10	1	COMMON	uzi.yml	PISTOL	8	TN3 SMITH-x Industrial	A Uzi	43	7	MACHINE_PISTOL	10	2	1	1	0	3	0	4	1	0	3	9	4	2	2	4	13	0	22	0	0	0	4	0	0	0	0	4	3	3	4	0	0	3	18	5	5	396	0	2022-05-19 14:37:20.330694	2022-05-19 14:37:20.330694
181	90	10	0	0	80	40	0	0	COMMON	belt-fed-minigun.yml	LIGHT_MACHINE_GUN	10	TN-33Y	A TN-33Y Belt-Fed Minigun	23	3	LIGHT_MACHINE_GUN	3	3	20	34	2	99	6	13	719	0	33	23	63	1	1	4	3	0	21	0	0	0	3	0	0	4	22	5	3	0	681	0	4	28	41	0	6	5	0	2022-05-20 13:31:58.231794	2022-05-20 13:31:58.231794
182	90	10	0	0	90	40	9	0	COMMON	sasg12.yml	SHOTGUN	3	R.S.S.	SASG-12	6	1	SHOTGUN	2	4	2	14	0	84	6	3	2	17	20	10	4	1	26	2	75	0	3	0	0	0	0	0	0	6	6	3	4	512	6	0	4	7	45	18	50	3	0	2022-05-20 13:34:16.536765	2022-05-20 13:34:16.536765
183	23.010099	0	0	0	50	0	0	0	COMMON	desert-eagle.yml	PISTOL	8	LX Industries	Desert Eagle	47	2	PISTOL	0	2	3	1	0	1	0	1	1	0	5	8	1	2	5	1	1	0	1	0	0	0	1	0	0	0	1	1	1	1	1	0	1	0	0	0	0	0	0	2022-05-27 13:18:08.172337	2022-05-27 13:18:08.172337
184	80	40	13	5	50	25	10	1	COMMON	czp10.yml	PISTOL	8	TN3 SMITH-x Industrial	CZP10 pistol	7	3	PISTOL	1	4	1	1	0	20	0	1	1	0	1	1	41	1	3	8	1	0	1	0	0	0	1	0	0	1	0	0	1	1	1	0	0	0	0	0	1	1	0	2022-05-27 13:19:03.762106	2022-05-27 13:19:03.762106
185	80	40	13	5	50	25	10	1	COMMON	uzi.yml	PISTOL	8	TN3 SMITH-x Industrial	A Uzi	43	1	MACHINE_PISTOL	2	1	1	5	0	24	0	1	1	0	3	20	40	1	1	2	1	0	1	0	0	0	0	0	0	1	0	0	0	1	1	0	0	1	1	1	1	1	0	2022-05-27 13:19:43.924731	2022-05-27 13:19:43.924731
186	80	40	13	5	50	25	10	1	COMMON	ppk.yml	PISTOL	8	TN3 SMITH-x Industrial	Silenced PPK	42	3	PISTOL	1	1	1	5	0	20	1	1	1	0	3	20	40	2	3	1	1	0	0	0	0	0	1	0	0	1	1	0	1	1	1	0	0	1	1	1	0	1	0	2022-05-27 13:20:03.239651	2022-05-27 13:20:03.239651
187	90	10	0	0	80	40	0	0	COMMON	belt-fed-minigun.yml	LIGHT_MACHINE_GUN	10	TN-33Y	A TN-33Y Belt-Fed Minigun	23	1	LIGHT_MACHINE_GUN	1	1	20	1	1	54	1	13	1	0	33	3	48	1	1	1	1	0	1	0	0	0	0	0	0	1	0	1	1	1	1	0	1	1	0	1	0	0	0	2022-05-27 13:20:34.200975	2022-05-27 13:20:34.200975
188	80	40	13	5	50	25	10	1	COMMON	glock.yml	PISTOL	8	TN3 SMITH-x Industrial	Glock	41	1	PISTOL	1	4	1	3	0	20	0	3	1	0	1	20	1	2	3	8	1	0	1	0	0	0	1	0	0	0	0	1	1	1	1	0	1	0	2	0	0	2	0	2022-05-27 13:22:48.092889	2022-05-27 13:22:48.092889
189	23.010099	0	0	0	50	0	0	0	COMMON	desert-eagle.yml	PISTOL	8	LX Industries	Desert Eagle	47	1	PISTOL	1	2	3	1	0	7	0	1	1	0	5	1	1	2	1	1	1	0	1	0	0	0	1	0	0	1	1	0	2	1	1	0	2	1	1	0	0	1	0	2022-05-27 13:24:34.260664	2022-05-27 13:24:34.260664
190	90	10	0	0	90	40	9	0	COMMON	sasg12.yml	SHOTGUN	3	R.S.S.	SASG-12	6	4	SHOTGUN	6	1	2	5	0	32	1	4	2	0	2	1	1	1	4	1	0	0	0	0	0	0	0	0	0	1	1	2	1	1	1	0	0	1	0	1	0	1	0	2022-05-27 13:29:40.764863	2022-05-27 13:29:40.764863
191	90	10	0	0	80	40	0	0	COMMON	belt-fed-minigun.yml	LIGHT_MACHINE_GUN	10	TN-33Y	A TN-33Y Belt-Fed Minigun	23	69	LIGHT_MACHINE_GUN	3	2	20	4	1	1	0	5	10	0	1	10	48	2	49	1	1	0	0	0	0	0	1	0	0	0	0	2	0	1	2	0	1	0	1	1	1	0	0	2022-05-27 13:30:41.341445	2022-05-27 13:30:41.341445
192	80	40	13	5	50	25	10	1	COMMON	ppk.yml	PISTOL	8	TN3 SMITH-x Industrial	Silenced PPK	42	3	PISTOL	2	4	1	1	0	1	1	1	1	0	8	20	1	1	3	8	1	0	0	0	0	0	1	0	0	0	1	2	2	0	2	0	1	2	2	0	1	1	0	2022-05-27 13:31:00.281053	2022-05-27 13:31:00.281053
193	80	40	13	5	50	25	10	1	COMMON	magnum-revolver.yml	PISTOL	8	TN3 SMITH-x Industrial	Magnum Revolver	40	2	PISTOL	1	1	1	5	0	1	0	1	1	0	3	20	40	2	2	1	0	0	0	0	0	0	2	0	0	1	1	0	0	0	2	0	0	0	1	1	2	1	0	2022-05-27 13:31:38.185849	2022-05-27 13:31:38.185849
194	23.010099	0	0	0	50	0	0	0	COMMON	desert-eagle.yml	PISTOL	8	LX Industries	Desert Eagle	47	2	PISTOL	0	2	3	1	0	1	1	1	1	0	5	43	1	2	5	10	2	0	1	0	0	0	0	0	0	0	1	1	1	0	1	0	0	1	1	1	1	2	0	2022-05-27 13:32:20.781361	2022-05-27 13:32:20.781361
195	80	40	13	5	50	25	10	1	COMMON	uzi.yml	PISTOL	8	TN3 SMITH-x Industrial	A Uzi	43	1	MACHINE_PISTOL	1	3	1	1	0	26	1	2	2	0	3	32	40	2	3	1	1	0	1	0	0	0	1	0	0	0	0	1	1	1	1	0	0	1	1	1	1	0	0	2022-05-27 19:49:47.149139	2022-05-27 19:49:47.149139
196	80	40	13	5	50	25	10	1	COMMON	uzi.yml	PISTOL	8	TN3 SMITH-x Industrial	A Uzi	43	10	MACHINE_PISTOL	1	3	1	5	0	1	1	1	1	0	3	1	1	2	3	1	0	0	1	0	0	0	1	0	0	1	0	1	1	1	1	0	0	3	1	0	1	0	0	2022-05-27 19:50:40.916734	2022-05-27 19:50:40.916734
197	0	20	90	90	10	20	90	90	COMMON	l96aw.yml	SNIPER	6	Heckler and Koch	L96 Arctic Warfare	24	1	SNIPER	1	3	9	8	3	24	1	1	11	0	1	10	1	2	3	2	0	0	0	0	0	0	8	0	0	0	0	1	2	0	1	0	0	0	1	1	2	1	0	2022-05-27 19:50:54.566603	2022-05-27 19:50:54.566603
198	80	40	13	5	50	25	10	1	COMMON	uzi.yml	PISTOL	8	TN3 SMITH-x Industrial	A Uzi	43	10	MACHINE_PISTOL	2	3	1	1	0	1	1	1	1	0	3	1	40	2	1	1	2	0	1	0	0	0	2	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	2022-05-27 19:52:17.03676	2022-05-27 19:52:17.03676
199	80	40	13	5	50	25	10	1	COMMON	glock.yml	PISTOL	8	TN3 SMITH-x Industrial	Glock	41	5	PISTOL	7	1	1	5	0	3	3	3	1	0	6	4	26	10	4	8	35	0	27	0	0	0	99	0	0	6	4	13	30	0	5	0	6	3	0	3	6	6	0	2022-05-27 23:51:17.037355	2022-05-27 23:51:17.037355
200	80	40	13	5	50	25	10	1	COMMON	magnum-revolver.yml	PISTOL	8	TN3 SMITH-x Industrial	Magnum Revolver	40	3	PISTOL	8	1	1	5	0	6	3	4	1	0	3	41	4	1	4	50	19	0	4	0	0	0	18	0	0	360	5	0	3	5	8	0	5	3	5	63	0	3	0	2022-05-28 00:01:15.525107	2022-05-28 00:01:15.525107
201	80	40	13	5	50	25	10	1	COMMON	uzi.yml	PISTOL	8	TN3 SMITH-x Industrial	A Uzi	43	6	MACHINE_PISTOL	3	3	1	4	0	62	0	5	1	0	3	1	18	1	1	3	4	0	11	0	0	0	5	0	0	16	3	0	6	6	4	0	4	7	6	9	4	3	0	2022-05-28 00:01:16.257918	2022-05-28 00:01:16.257918
202	90	10	0	0	80	40	0	0	COMMON	belt-fed-minigun.yml	LIGHT_MACHINE_GUN	10	TN-33Y	A TN-33Y Belt-Fed Minigun	23	5	LIGHT_MACHINE_GUN	3	4	20	3	2	16	0	13	17	0	33	15	6	1	8	4	69	0	69	0	0	0	0	0	0	0	0	0	0	0	0	0	0	4	0	6	0	0	0	2022-05-28 00:01:16.674515	2022-05-28 00:01:16.674515
203	10	10	10	10	10	10	10	10	COMMON	augpara.yml	ASSAULT_RIFLE	4	SK-10	AUG-PARA Assault Rifle	29	6	ASSAULT_RIFLE	10	3	2	6	4	46	3	6	1	0	5	5	24	1	1	2	161	0	4	0	0	0	4	0	0	8	11	303	3	0	4	0	72	4	4	0	0	174	0	2022-05-28 00:01:17.090572	2022-05-28 00:01:17.090572
204	80	40	13	5	50	25	10	1	COMMON	ppk.yml	PISTOL	8	TN3 SMITH-x Industrial	Silenced PPK	42	4	PISTOL	8	2	1	9	0	36	4	1	3	2	4	99	84	1	2	14	8	0	83	0	0	0	113	0	0	4	46	5	7	31	5	0	4	0	85	5	6	11	0	2022-06-11 17:39:00.066237	2022-06-11 17:39:00.066237
205	80	40	13	5	50	25	10	1	COMMON	ppk.yml	PISTOL	8	TN3 SMITH-x Industrial	Silenced PPK	42	8	PISTOL	13	2	1	6	0	20	0	2	1	0	2	76	111	4	1	1	5	0	65	0	0	0	7	0	0	96	7	199	1198	54	42	0	955	9	7	5	4	9	0	2022-06-11 19:06:11.034459	2022-06-11 19:06:11.034459
206	80	40	13	5	50	25	10	1	COMMON	magnum-revolver.yml	PISTOL	8	TN3 SMITH-x Industrial	Magnum Revolver	40	8	PISTOL	5	6	1	9	0	30	0	1	1	1	7	10	10	2	1	34	14	0	33	0	0	0	0	0	0	9	5	6	5	0	73	0	7	8	8	6	5	10	0	2022-06-11 19:07:17.162553	2022-06-11 19:07:17.162553
207	80	40	13	5	50	25	10	1	COMMON	ppk.yml	PISTOL	8	TN3 SMITH-x Industrial	Silenced PPK	42	3	PISTOL	3	1	1	11	0	44	6	1	2	1	4	28	40	4	1	1	217	0	7	0	0	0	0	0	0	0	37	8	43	45	14	0	952	6	8	42	9	0	0	2022-06-11 19:12:23.61176	2022-06-11 19:12:23.61176
208	90	10	0	0	80	40	0	0	COMMON	mp5.yml	SUB_MACHINE_GUN	5	Heckler and Koch	MP5	5	6	SUB_MACHINE_GUN	2	3	2	10	7	5	6	7	1	187	5	49	10	5	2	1	7	0	10	0	0	0	10	0	0	5	73	10	7	55	0	0	36	5	6	0	12020	54	0	2022-06-11 19:13:39.932315	2022-06-11 19:13:39.932315
209	80	40	13	5	50	25	10	1	COMMON	glock.yml	PISTOL	8	TN3 SMITH-x Industrial	Glock	41	3	PISTOL	10	2	1	1	0	51	0	1	1	4	3	10	40	60	1	2	55	0	11	0	0	0	7	0	0	8	0	7	7	976	58	0	5	10	7	12	87	7	0	2022-06-11 19:14:01.81407	2022-06-11 19:14:01.81407
210	90	10	0	0	80	40	0	0	COMMON	mk46.yml	LIGHT_MACHINE_GUN	10	TN-33Y	MK-46 Light Machine Gun	78	7	LIGHT_MACHINE_GUN	11	5	20	2	3	6	22	9	1	5	5	70	136	1	6	1	8	0	7	0	0	0	69	0	0	0	0	6	0	7	104	0	11	595	83	10	13	451	0	2022-06-11 19:16:19.65204	2022-06-11 19:16:19.65204
211	90	10	0	0	90	40	9	0	COMMON	sasg12.yml	SHOTGUN	3	R.S.S.	SASG-12	6	2	SHOTGUN	1	2	2	14	0	80	0	3	1	5	10	6	6	4	1	1	0	0	0	0	0	0	0	0	0	7	0	0	4	8	10	0	0	14	11	7	1647	0	0	2022-06-11 19:27:43.034582	2022-06-11 19:27:43.034582
212	90	10	0	0	90	40	9	0	COMMON	saiga12.yml	SHOTGUN	3	R.S.S.	SAIGA-12	39	1	SHOTGUN	10	2	2	14	0	112	0	1	2	5	13	10	10	1	2	2	89	0	118	0	0	0	0	0	0	9	7	40	6	5	9	0	10	102	6	7	11	0	0	2022-06-11 20:10:24.192818	2022-06-11 20:10:24.192818
213	80	40	13	5	50	25	10	1	COMMON	uzi.yml	PISTOL	8	TN3 SMITH-x Industrial	A Uzi	43	10	MACHINE_PISTOL	11	1	1	2	0	8	11	3	3	6	3	11	8	1	1	2	133	0	10	0	0	0	6	0	0	5	172	7	6	5	9	0	11	828	79	2655	11	0	0	2022-06-11 20:12:55.725468	2022-06-11 20:12:55.725468
214	80	40	13	5	50	25	10	1	COMMON	glock.yml	PISTOL	8	TN3 SMITH-x Industrial	Glock	41	3	PISTOL	1	2	1	4	0	20	0	3	1	1	1	80	50	5	3	1	156	0	6	0	0	0	91	0	0	7	7	11	295	10	8	0	9	9	11	0	6	12	0	2022-06-12 00:36:37.892652	2022-06-12 00:36:37.892652
215	90	10	0	0	80	40	0	0	COMMON	mp5.yml	SUB_MACHINE_GUN	5	Heckler and Koch	MP5	5	11	SUB_MACHINE_GUN	13	1	2	14	13	6	12	3	1	2	6	83	11	3374	3	5	245	0	11	0	0	0	6	0	0	9	222	172	52	8	7	0	170	11	94	9	8	21199	0	2022-06-13 12:06:43.324058	2022-06-13 12:06:43.324058
\.


--
-- Data for Name: rifle_placements; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.rifle_placements (id, ip_room_vnum, ip_container_selector, ip_rifle_id, created_at, updated_at) FROM stdin;
\.


--
-- Data for Name: room; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.room (id, room_number, zone, sector_type, name, description, ex_keyword, ex_description, light, room_flag, nickname, textures) FROM stdin;
3	132	1	21	{blu}C.O.F.O.B:{/blu} - Southern Hallway	The cement floor is a perfect complement to the reflection of the incandescent lights that are installed into the the ceiling. {gld}To the East{/gld}, you will find the Armory. {gld}To the West{/gld}, you will find the training grounds. Just around the corner to the East and North a ways is the Weapons Locker. Feel free to get acquainted with the layout, recruit.\r\n	\N	\N	1	0	\N	\N
4	133	1	21	{blu}C.O.F.O.B:{/blu} - Southern Hallway	The cement floor is a perfect complement to the reflection of the incandescent lights that are installed into the the ceiling. {gld}To the East{/gld}, you will find the Armory. {gld}To the West{/gld}, you will find the training grounds. Just around the corner to the East and North a ways is the Weapons Locker. Feel free to get acquainted with the layout, recruit.\r\n	\N	\N	1	0	\N	\N
5	134	1	21	{blu}C.O.F.O.B:{/blu} - Southern Hallway	The cement floor is a perfect complement to the reflection of the incandescent lights that are installed into the the ceiling. {gld}To the East{/gld}, you will find the Armory. {gld}To the West{/gld}, you will find the training grounds. Just around the corner to the East and North a ways is the Weapons Locker. Feel free to get acquainted with the layout, recruit.\r\n	\N	\N	1	0	\N	\N
6	135	1	21	{blu}C.O.F.O.B:{/blu} - Southern Hallway	The cement floor is a perfect complement to the reflection of the incandescent lights that are installed into the the ceiling. {gld}To the East{/gld}, you will find the Armory. {gld}To the West{/gld}, you will find the training grounds. Just around the corner to the East and North a ways is the Weapons Locker. Feel free to get acquainted with the layout, recruit.\r\n	\N	\N	1	0	\N	\N
7	136	1	21	{blu}C.O.F.O.B:{/blu} - Southern Hallway	The cement floor is a perfect complement to the reflection of the incandescent lights that are installed into the the ceiling. {gld}To the East{/gld}, you will find the Armory. {gld}To the West{/gld}, you will find the training grounds. Just around the corner to the East and North a ways is the Weapons Locker. Feel free to get acquainted with the layout, recruit.\r\n	\N	\N	1	0	\N	\N
8	137	1	21	{blu}C.O.F.O.B:{/blu} - Southern Hallway	The cement floor is a perfect complement to the reflection of the incandescent lights that are installed into the the ceiling. {gld}To the East{/gld}, you will find the Armory. {gld}To the West{/gld}, you will find the training grounds. Just around the corner to the East and North a ways is the Weapons Locker. Feel free to get acquainted with the layout, recruit.\r\n	\N	\N	1	0	\N	\N
19	148	1	21	{blu}C.O.F.O.B:{/blu} - Weapons Cache stairs - TOP	@FILL_ME@\r\n	\N	\N	1	0	\N	\N
21	150	1	21	{blu}C.O.F.O.B:{/blu} - Weapons Cache stairs - TOP	@FILL_ME@\r\n	\N	\N	1	0	\N	\N
22	151	1	21	{blu}C.O.F.O.B:{/blu} - Weapons Cache stairs - Basement 1	@FILL_ME@\r\n	\N	\N	1	0	\N	\N
24	153	1	21	{blu}C.O.F.O.B:{/blu} - Weapons Cache stairs - Basement 1	@FILL_ME@\r\n	\N	\N	1	0	\N	\N
25	154	1	21	{blu}C.O.F.O.B:{/blu} - Weapons Cache stairs - Basement 2	@FILL_ME@\r\n	\N	\N	1	0	\N	\N
26	155	1	21	{blu}C.O.F.O.B:{/blu} - Weapons Cache stairs - Basement 2	@FILL_ME@\r\n	\N	\N	1	0	\N	\N
27	156	1	21	{blu}C.O.F.O.B:{/blu} - Weapons Cache stairs - Basement 2	@FILL_ME@\r\n	\N	\N	1	0	cofob-armory-basement-2	\N
9	138	1	21	{blu}C.O.F.O.B{/blu} - Southern Hallway	The southern hallway of the COFOB aligns exactly with a series of huge sewer pipes. Several years ago, a recruit went AWOL and hijacked a tank. He drove it through the southern hallway and other parts of the FOB. As a result, the sewage drains burst open and flooded the entire first level of the FOB. It is why the common nickname for the southern hallway is "el bano".\r\n	\N	\N	1	0	\N	,CEMENT,INSIDE
11	140	1	21	{blu}C.O.F.O.B{/blu} - East Hallway	Many complaints have been lodged against the higher ups about the lack of cleanliness of the eastern hallway. An odd dampness is prevalent throughout. The temperature outside somehow affects this part of the FOB more than others. During sand storms, the eastern hallway turns to a sandy damp mess.\r\n	\N	\N	1	0	\N	,CEMENT,INSIDE
12	141	1	21	{blu}C.O.F.O.B{/blu} - East Hallway - TRITON Entrance	A sign above the door to the East says "{yel}TRITON LABS{/yel}" in big bold lettering. The placement of the laboratory is an odd one, as it is one of the only ways to reach the civilian portion of the FOB via the East.\r\n	\N	\N	1	0	cofob-armory-entrance	,CEMENT,INSIDE
13	142	1	21	{blu}C.O.F.O.B{/blu} - TRITON - Decontamination	In order for the lab to maintain a clean and sterile atmosphere, a decontamination area was installed. That is the room you find yourself in. A station not unlike the body scanners you find at an airport sprays you up and down with a cool mist of chemicals. A vacuum below your feet sucks wind and your ears pop as various hidden but audible devices clamor to eliminate any hazardous bacteria.\r\n	\N	\N	1	0	\N	,CEMENT,INSIDE
14	143	1	21	{blu}C.O.F.O.B{/blu} - TRITON - Research	An array of server racks blocks the center of the room from where you are standing.TRITON Labs is a sub-contractor of the incredibly murky organization known as {blu}ORTHOS CORP{/blu}. The lab specializes in HIGHLY experimental ammunition which blurs the line between ethical and immoral conduct. TRITON Labs is believed to have developed an extreme pathogen with the power to wipe out an entire city block with just a few drops of a highly secretive chemical agent.\r\n	\N	\N	1	0	\N	,CEMENT,INSIDE
18	147	1	21	{blu}C.O.F.O.B{/blu} - TRITON - Human Experiments	There used to be a time when clients would pay for highly specialized medical products.During the apex of that booming underground market, scientists were paid top dollar to secure the targetted product under any means necessary. The TRTION labs human experimentation sector is located somewhere beneath the FOB, though some say it was forever closed off forever. The leftover and forgotten human experiments reside in this room indefinitely. To have survived those experiments is a greater hell than to have perished.\r\n	\N	\N	1	0	\N	,CEMENT,INSIDE
15	144	1	21	{blu}C.O.F.O.B{/blu} - TRITON - Experimental Ammunition	TRITON labs has been home to the most advanced bleeding edge experimental ammunition on the planet. Racks of hermetically sealed boxes with TRITON's menacing logo reach almost to the full height of the extended ceiling. There is a single computer terminal here asking for biometric credentials. A camera adjusts it's focus and magnification as you move.\r\n	\N	\N	1	0	\N	,CEMENT,INSIDE
28	157	1	21	{blu}C.O.F.O.B:{/blu} - Weapons Cache stairs - Basement 2	@FILL_ME@\r\n	\N	\N	1	0	\N	\N
29	158	1	21	{blu}C.O.F.O.B:{/blu} - Weapons Cache stairs - Basement 2	@FILL_ME@\r\n	\N	\N	1	0	\N	\N
30	159	1	21	{blu}C.O.F.O.B:{/blu} - Weapons Cache stairs - Basement 2	@FILL_ME@\r\n	\N	\N	1	0	\N	\N
31	160	1	21	{blu}C.O.F.O.B:{/blu} - Weapons Cache stairs - Basement 2	@FILL_ME@\r\n	\N	\N	1	0	\N	\N
32	161	1	21	{blu}C.O.F.O.B:{/blu} - Weapons Cache stairs - Basement 2	@FILL_ME@\r\n	\N	\N	1	0	\N	\N
33	162	1	21	{blu}C.O.F.O.B:{/blu} - Weapons Cache stairs - Basement 2	@FILL_ME@\r\n	\N	\N	1	0	\N	\N
34	163	1	21	{blu}C.O.F.O.B:{/blu} - Weapons Cache stairs - Basement 2	@FILL_ME@\r\n	\N	\N	1	0	\N	\N
35	164	1	21	{blu}C.O.F.O.B:{/blu} - Weapons Cache stairs - Basement 2	@FILL_ME@\r\n	\N	\N	1	0	\N	\N
36	165	1	21	{blu}C.O.F.O.B:{/blu} - Weapons Cache stairs - Basement 2	@FILL_ME@\r\n	\N	\N	1	0	\N	\N
37	166	1	21	{blu}C.O.F.O.B:{/blu} - Weapons Cache stairs - Basement 2	@FILL_ME@\r\n	\N	\N	1	0	\N	\N
38	167	1	21	{blu}C.O.F.O.B:{/blu} - Weapons Cache stairs - Basement 2	@FILL_ME@\r\n	\N	\N	1	0	\N	\N
39	168	1	21	{blu}C.O.F.O.B:{/blu} - Weapons Cache stairs - Basement 2	@FILL_ME@\r\n	\N	\N	1	0	\N	\N
40	169	1	21	{blu}C.O.F.O.B:{/blu} - Weapons Cache stairs - Basement 2	@FILL_ME@\r\n	\N	\N	1	0	cofob-armory-basement-2	\N
41	170	1	21	{blu}C.O.F.O.B:{/blu} - Weapons Cache stairs - Basement 2	@FILL_ME@\r\n	\N	\N	1	0	\N	\N
42	171	1	21	{blu}C.O.F.O.B:{/blu} - Weapons Cache stairs - Basement 2	@FILL_ME@\r\n	\N	\N	1	0	\N	\N
43	172	1	21	{blu}C.O.F.O.B:{/blu} - Basement 2 - Breach charges	@FILL_ME@\r\n	\N	\N	1	0	\N	\N
44	173	1	21	{blu}C.O.F.O.B:{/blu} - Basement 2 - Breach charges	@FILL_ME@\r\n	\N	\N	1	0	\N	\N
45	174	1	0	name	description	\N	\N	1	0	\N	\N
46	175	1	0	name	description	\N	\N	1	0	\N	\N
47	176	1	0	name	description	\N	\N	1	0	\N	\N
49	178	1	0	{blu}C.O.F.O.B:{/blu} - Basement 2 - Car Garage	@FILL_ME@\r\n	\N	\N	1	0	\N	\N
50	179	1	0	{blu}C.O.F.O.B:{/blu} - Basement 2 - Car Garage	@FILL_ME@\r\n	\N	\N	1	0	\N	\N
51	180	1	0	{blu}C.O.F.O.B:{/blu} - Basement 2 - Car Garage - 1A	@FILL_ME@\r\n	\N	\N	1	0	\N	\N
52	181	1	0	{blu}C.O.F.O.B:{/blu} - Basement 2 - Car Garage - 1A	@FILL_ME@\r\n	\N	\N	1	0	\N	\N
53	182	1	0	{blu}C.O.F.O.B:{/blu} - Basement 2 - Car Garage - 1B	@FILL_ME@\r\n	\N	\N	1	0	\N	\N
54	183	1	0	{blu}C.O.F.O.B:{/blu} - Basement 2 - Car Garage - 1B	@FILL_ME@\r\n	\N	\N	1	0	\N	\N
55	184	1	0	{blu}C.O.F.O.B:{/blu} - Basement 2 - Car Garage	@FILL_ME@\r\n	\N	\N	1	0	\N	\N
56	185	1	0	{blu}C.O.F.O.B:{/blu} - Basement 2 - Car Garage	@FILL_ME@\r\n	\N	\N	1	0	\N	\N
57	186	1	0	{blu}C.O.F.O.B:{/blu} - Basement 2 - Car Garage - 2A	@FILL_ME@\r\n	\N	\N	1	0	\N	\N
58	187	1	0	{blu}C.O.F.O.B:{/blu} - Basement 2 - Car Garage - 2A	@FILL_ME@\r\n	\N	\N	1	0	\N	\N
59	188	1	0	{blu}C.O.F.O.B:{/blu} - Basement 2 - Car Garage - 2B	@FILL_ME@\r\n	\N	\N	1	0	\N	\N
60	189	1	0	{blu}C.O.F.O.B:{/blu} - Basement 2 - Car Garage - 2B	@FILL_ME@\r\n	\N	\N	1	0	\N	\N
61	190	1	0	{blu}C.O.F.O.B:{/blu} - Basement 2 - Car Garage	@FILL_ME@\r\n	\N	\N	1	0	\N	\N
62	191	1	0	{blu}C.O.F.O.B:{/blu} - Basement 2 - Car Garage	@FILL_ME@\r\n	\N	\N	1	0	\N	\N
85	214	1	0	{blu}C.O.F.O.B [OUTER]:{/blu} - Western Hallway	The reinforced metal walls are briefly interrupted by the occasional senior officer door. As you make your way north and south, you notice that the entirety of the western wall consists of metal walls with doors that lead to the underground portion of the base. \r\n	\N	\N	1	0	\N	\N
86	215	1	0	{blu}C.O.F.O.B [OUTER]:{/blu} - Western Hallway	The reinforced metal walls are briefly interrupted by the occasional senior officer door. As you make your way north and south, you notice that the entirety of the western wall consists of metal walls with doors that lead to the underground portion of the base. \r\n	\N	\N	1	0	\N	\N
87	216	1	0	{blu}C.O.F.O.B [OUTER]:{/blu} - Western Hallway	The reinforced metal walls are briefly interrupted by the occasional senior officer door. As you make your way north and south, you notice that the entirety of the western wall consists of metal walls with doors that lead to the underground portion of the base. \r\n	\N	\N	1	0	\N	\N
67	196	1	21	{blu}C.O.F.O.B{/blu} - East Hallway	Many complaints have been lodged against the higher ups about the lack of cleanliness of the eastern hallway. An odd dampness is prevalent throughout. The temperature outside somehow affects this part of the FOB more than others. During sand storms, the eastern hallway turns to a sandy damp mess.\r\n	\N	\N	1	0	\N	,CEMENT,INSIDE
72	201	1	21	{blu}C.O.F.O.B{/blu} - East Hallway	Many complaints have been lodged against the higher ups about the lack of cleanliness of the eastern hallway. An odd dampness is prevalent throughout. The temperature outside somehow affects this part of the FOB more than others. During sand storms, the eastern hallway turns to a sandy damp mess.\r\n	\N	\N	1	0	\N	,CEMENT,INSIDE
76	205	1	21	{blu}C.O.F.O.B{/blu} - North Hallway	The north end of the FOB has always been a much nicer and cleaner place than the rest of the base. Primarily due to the frequency of senior officer suites. The north hallway will also bring you to the second floor where a large majority of the engineering work is done.\r\n	\N	\N	1	0	\N	,CEMENT,INSIDE
127	256	1	21	{blu}C.O.F.O.B{/blu} - Engineering - Infrastructure Hallway	It's almost comical how clean the engineering level of the FOB is compared to the lower levels. The carpet and walls are in a prestine -- no, IMMACULATE state. You almost feel like you should take off your shoes. The climate is controlled by a different air-conditioning system than the rest of the base. The room is cool and devoid of any outside noise.\r\n	\N	\N	1	0	\N	,CEMENT,INSIDE
80	209	1	21	{blu}C.O.F.O.B{/blu} - North Hallway	The north end of the FOB has always been a much nicer and cleaner place than the rest of the base. Primarily due to the frequency of senior officer suites. The north hallway will also bring you to the second floor where a large majority of the engineering work is done.\r\n	\N	\N	1	0	\N	,CEMENT,INSIDE
88	217	1	0	{blu}C.O.F.O.B [OUTER]:{/blu} - Western Hallway	The reinforced metal walls are briefly interrupted by the occasional senior officer door. As you make your way north and south, you notice that the entirety of the western wall consists of metal walls with doors that lead to the underground portion of the base. \r\n	\N	\N	1	0	\N	\N
185	314	1	0	Market Apartments - Building 3 - Unit 303 - Master Bedroom	undefined\r\n	\N	\N	1	0	\N	\N
89	218	1	0	{blu}C.O.F.O.B [OUTER]:{/blu} - Western Hallway	The reinforced metal walls are briefly interrupted by the occasional senior officer door. As you make your way north and south, you notice that the entirety of the western wall consists of metal walls with doors that lead to the underground portion of the base. \r\n	\N	\N	1	0	\N	\N
90	219	1	0	{blu}C.O.F.O.B [OUTER]:{/blu} - Western Hallway	The reinforced metal walls are briefly interrupted by the occasional senior officer door. As you make your way north and south, you notice that the entirety of the western wall consists of metal walls with doors that lead to the underground portion of the base. \r\n	\N	\N	1	0	\N	\N
91	220	1	0	{blu}C.O.F.O.B [OUTER]:{/blu} - Western Hallway	The reinforced metal walls are briefly interrupted by the occasional senior officer door. As you make your way north and south, you notice that the entirety of the western wall consists of metal walls with doors that lead to the underground portion of the base. \r\n	\N	\N	1	0	\N	\N
92	221	1	0	{blu}C.O.F.O.B [OUTER]:{/blu} - Western Hallway	The reinforced metal walls are briefly interrupted by the occasional senior officer door. As you make your way north and south, you notice that the entirety of the western wall consists of metal walls with doors that lead to the underground portion of the base. \r\n	\N	\N	1	0	\N	\N
93	222	1	0	{blu}C.O.F.O.B [OUTER]:{/blu} - Western Hallway	The reinforced metal walls are briefly interrupted by the occasional senior officer door. As you make your way north and south, you notice that the entirety of the western wall consists of metal walls with doors that lead to the underground portion of the base. \r\n	\N	\N	1	0	\N	\N
94	223	1	0	{blu}C.O.F.O.B [OUTER]:{/blu} - Western Hallway	The reinforced metal walls are briefly interrupted by the occasional senior officer door. As you make your way north and south, you notice that the entirety of the western wall consists of metal walls with doors that lead to the underground portion of the base. \r\n	\N	\N	1	0	\N	\N
95	224	1	0	{blu}C.O.F.O.B [OUTER]:{/blu} - Atop the stairs	The floor deviates from the other rooms in that it is made up of a reinforced steel grating. The choice for such a floor remains a mystery. The door to the west leads to the stairs that will take you to the underground portion of the base.\r\n	\N	\N	1	0	cofob-west-atop-stairs	\N
96	225	1	0	{blu}C.O.F.O.B [OUTER]:{/blu} - Atop the stairs	The floor deviates from the other rooms in that it is made up of a reinforced steel grating. The choice for such a floor remains a mystery. The door to the west leads to the stairs that will take you to the underground portion of the base.\r\n	\N	\N	1	0	\N	\N
114	243	1	0	{blu}C.O.F.O.B:{/blu} - Second floor Foremast	@FILL_ME@\r\n	\N	\N	1	0	\N	\N
115	244	1	0	{blu}C.O.F.O.B:{/blu} - Second floor Foremast	@FILL_ME@\r\n	\N	\N	1	0	\N	\N
116	245	1	0	{blu}C.O.F.O.B:{/blu} - Second floor Foremast	@FILL_ME@\r\n	\N	\N	1	0	\N	\N
117	246	1	0	{blu}C.O.F.O.B:{/blu} - Second floor Foremast	@FILL_ME@\r\n	\N	\N	1	0	\N	\N
118	247	1	0	{blu}C.O.F.O.B:{/blu} - Second floor Foremast	@FILL_ME@\r\n	\N	\N	1	0	\N	\N
119	248	1	0	{blu}C.O.F.O.B:{/blu} - Second floor Foremast	@FILL_ME@\r\n	\N	\N	1	0	\N	\N
120	249	1	21	{blu}C.O.F.O.B:{/blu} - Second floor Foremast	@FILL_ME@\r\n	\N	\N	1	0	\N	\N
121	250	1	21	{blu}C.O.F.O.B:{/blu} - Second floor Foremast	@FILL_ME@\r\n	\N	\N	1	0	\N	\N
122	251	1	21	{blu}C.O.F.O.B:{/blu} - Second floor Foremast	@FILL_ME@\r\n	\N	\N	1	0	\N	\N
123	252	1	21	{blu}C.O.F.O.B:{/blu} - Second floor Foremast	@FILL_ME@\r\n	\N	\N	1	0	\N	\N
98	227	1	0	{blu}C.O.F.O.B{/blu} - Southern Hallway	The southern hallway of the COFOB aligns exactly with a series of huge sewer pipes. Several years ago, a recruit went AWOL and hijacked a tank. He drove it through the southern hallway and other parts of the FOB. As a result, the sewage drains burst open and flooded the entire first level of the FOB. It is why the common nickname for the southern hallway is "el bano".\r\n	\N	\N	1	0	\N	\N
97	226	1	0	{blu}C.O.F.O.B{/blu} - Southern Hallway	The southern hallway of the COFOB aligns exactly with a series of huge sewer pipes. Several years ago, a recruit went AWOL and hijacked a tank. He drove it through the southern hallway and other parts of the FOB. As a result, the sewage drains burst open and flooded the entire first level of the FOB. It is why the common nickname for the southern hallway is "el bano".\r\n	\N	\N	1	0	\N	\N
103	232	1	21	{blu}C.O.F.O.B{/blu} - Up the Staircase	The walls are a drab shade of grey and the stairs are an odd combination of carpet and metal. \r\n	\N	\N	1	0	\N	,CEMENT,INSIDE
104	233	1	21	{blu}C.O.F.O.B{/blu} - Up the Staircase	The walls are a drab shade of grey and the stairs are an odd combination of carpet and metal. \r\n	\N	\N	1	0	\N	,CEMENT,INSIDE
105	234	1	21	{blu}C.O.F.O.B{/blu} - Up the Staircase	The walls are a drab shade of grey and the stairs are an odd combination of carpet and metal. \r\n	\N	\N	1	0	\N	,CEMENT,INSIDE
108	237	1	21	{blu}C.O.F.O.B{/blu} - Engineering - Atop the stairs	The computing industry has progressed to the point where every individual has their own computer in the palm of their hand. But those advances dont't come without a cost. The engineering level of the FOB focusses on the invisible threat of state actors and the growing need for tighter security. \r\n	\N	\N	1	0	cofob-secondfloor-center	,CEMENT,INSIDE
113	242	1	21	{blu}C.O.F.O.B{/blu} - Engineering - Threat Surveillance	There are a couple high profile incidents where law enforcement were locked out of the personal devices of a deceased criminal. A hard lesson in cryptography was learned that day. As a result, funding instead went into massive amounts of global surveillance and threat tracking. The advent of Artificial Intelligence gave another possible avenue: automated tracking. The {red}Threat Surveillance{/red} sector of the FOB is the most advanced and funded aspect of every FOB to date.\r\n	\N	\N	1	0	\N	,CEMENT,INSIDE
130	259	1	0	{blu}C.O.F.O.B:{/blu} - Engineering hallway	The doors on the eastern wall are all closed and most likely locked. You see the commons area at the end of the hallway. You notice various surveillance cameras strategically placed at the corners of the ceiling. \r\n	\N	\N	1	0	\N	\N
131	260	1	0	{blu}C.O.F.O.B:{/blu} - Engineering hallway	The doors on the eastern wall are all closed and most likely locked. You see the commons area at the end of the hallway. You notice various surveillance cameras strategically placed at the corners of the ceiling. \r\n	\N	\N	1	0	\N	\N
132	261	1	0	{blu}C.O.F.O.B:{/blu} - Engineering hallway	The doors on the eastern wall are all closed and most likely locked. You see the commons area at the end of the hallway. You notice various surveillance cameras strategically placed at the corners of the ceiling. \r\n	\N	\N	1	0	\N	\N
134	263	1	0	{blu}C.O.F.O.B:{/blu} - Server Room 1A	It's noisy and loud in here. The rack mounted servers are fervently whirring under the immense workloads. The ceiling is a grate with a large fan behind it. A complex looking locking mechanism protects rack mounted servers here. \r\n	\N	\N	1	0	\N	\N
135	264	1	0	{blu}C.O.F.O.B:{/blu} - Server Room 1B	Wall to wall rack mounted servers take up the entirety of this room. There is almost nowhere to stand. A haphazard array of ethernet cables snake chaotically to and from each server. Maintaining this must be a headache. \r\n	\N	\N	1	0	\N	\N
136	265	1	21	{blu}C.O.F.O.B:{/blu} - Eastern Hallway	@FILL_ME@\r\n	\N	\N	1	0	\N	\N
137	266	1	0	{blu}C.O.F.O.B:{/blu} - Gear room	A huge weapons rack is attached to the wall here. The rack contains rifles, pistols, ammunition, explosives, and breach charges.\r\n	\N	\N	1	0	cofob-gear-room	\N
138	267	1	0	{blu}C.O.F.O.B:{/blu} - Shooting Range Area Falcon	@FILL_ME@\r\n	\N	\N	1	0	\N	\N
139	268	1	0	{blu}C.O.F.O.B:{/blu} - Shooting Range Area Falcon	@FILL_ME@\r\n	\N	\N	1	0	\N	\N
140	269	1	0	{blu}C.O.F.O.B:{/blu} - Shooting Range Area Falcon	@FILL_ME@\r\n	\N	\N	1	0	\N	\N
141	270	1	0	{blu}C.O.F.O.B:{/blu} - Shooting Range Area Falcon	@FILL_ME@\r\n	\N	\N	1	0	\N	\N
142	271	1	0	{blu}C.O.F.O.B:{/blu} - Shooting Range Area Falcon	@FILL_ME@\r\n	\N	\N	1	0	\N	\N
143	272	1	0	{blu}C.O.F.O.B:{/blu} - Shooting Range Area Falcon	@FILL_ME@\r\n	\N	\N	1	0	\N	\N
144	273	1	0	{blu}C.O.F.O.B:{/blu} - Shooting Range Area Falcon	@FILL_ME@\r\n	\N	\N	1	0	\N	\N
145	274	1	0	{blu}C.O.F.O.B [OUTER]:{/blu} - Eastern DMZ corridor	Dirt stretches to the east towards a heavy containment door. Behind that door must be where you can leave the building. The corridor is dimly lit. The air is dry and smells of a combination of sweat and gasoline for some reason.\r\n	\N	\N	1	0	\N	\N
146	275	1	0	{blu}C.O.F.O.B [OUTER]:{/blu} - Eastern DMZ corridor	Dirt stretches to the east towards a heavy containment door. Behind that door must be where you can leave the building. The corridor is dimly lit. The air is dry and smells of a combination of sweat and gasoline for some reason.\r\n	\N	\N	1	0	\N	\N
147	276	1	0	{blu}C.O.F.O.B [OUTER]:{/blu} - Eastern DMZ corridor	Dirt stretches to the east towards a heavy containment door. Behind that door must be where you can leave the building. The corridor is dimly lit. The air is dry and smells of a combination of sweat and gasoline for some reason.\r\n	\N	\N	1	0	\N	\N
148	277	1	0	{blu}C.O.F.O.B [OUTER]:{/blu} - Fenced off DMZ corridor	Beneath the floor here is an entrance to an underground tunnel. The only problem is that you can't seem to get it to move. To the east is the door to the outside world.A falcon engraving is above the door frame to the east.\r\n	\N	\N	1	0	\N	\N
149	278	1	0	Destroyed overpass - Abott Market South	Crushed and flattened by endless shells, the overpass that you just passed under creaks eternally. Now and then you hear the crumble of asphault and be bend of metal rods. The overpass connects the once thriving market place of Abott Market, but now all you see is rubble and a shell of what used to be a thriving market.\r\n	\N	\N	1	0	abbotstart	\N
152	281	1	0	Destroyed overpass - Abott Market South	Crushed and flattened by endless shells, the overpass that you just passed under creaks eternally. Now and then you hear the crumble of asphault and be bend of metal rods. The overpass connects the once thriving market place of Abott Market, but now all you see is rubble and a shell of what used to be a thriving market.\r\n	\N	\N	1	0	\N	\N
153	282	1	0	Destroyed overpass - Abott Market South	Crushed and flattened by endless shells, the overpass that you just passed under creaks eternally. Now and then you hear the crumble of asphault and be bend of metal rods. The overpass connects the once thriving market place of Abott Market, but now all you see is rubble and a shell of what used to be a thriving market.\r\n	\N	\N	1	0	\N	\N
154	283	1	0	Destroyed overpass - Abott Market South	Crushed and flattened by endless shells, the overpass that you just passed under creaks eternally. Now and then you hear the crumble of asphault and be bend of metal rods. The overpass connects the once thriving market place of Abott Market, but now all you see is rubble and a shell of what used to be a thriving market.\r\n	\N	\N	1	0	\N	\N
155	284	1	0	Destroyed overpass - Abott Market South	Crushed and flattened by endless shells, the overpass that you just passed under creaks eternally. Now and then you hear the crumble of asphault and be bend of metal rods. The overpass connects the once thriving market place of Abott Market, but now all you see is rubble and a shell of what used to be a thriving market.\r\n	\N	\N	1	0	\N	\N
156	285	1	0	Destroyed overpass - Abott Market South	Crushed and flattened by endless shells, the overpass that you just passed under creaks eternally. Now and then you hear the crumble of asphault and be bend of metal rods. The overpass connects the once thriving market place of Abott Market, but now all you see is rubble and a shell of what used to be a thriving market.\r\n	\N	\N	1	0	\N	\N
157	286	1	0	Destroyed overpass - Abott Market South	Crushed and flattened by endless shells, the overpass that you just passed under creaks eternally. Now and then you hear the crumble of asphault and be bend of metal rods. The overpass connects the once thriving market place of Abott Market, but now all you see is rubble and a shell of what used to be a thriving market.\r\n	\N	\N	1	0	\N	\N
237	365	1	0	{blu}Crenshaw{/blu} Highway overpass - North	The ramp curves to the north. To the east you see a long stretch of highway that disappears over the horizon. You see a shipyard far off to the northeast. \r\n	\N	\N	1	0	\N	\N
158	287	1	0	Destroyed overpass - Abott Market South	Crushed and flattened by endless shells, the overpass that you just passed under creaks eternally. Now and then you hear the crumble of asphault and be bend of metal rods. The overpass connects the once thriving market place of Abott Market, but now all you see is rubble and a shell of what used to be a thriving market.\r\n	\N	\N	1	0	\N	\N
159	288	1	0	Destroyed overpass - Abott Market South	Crushed and flattened by endless shells, the overpass that you just passed under creaks eternally. Now and then you hear the crumble of asphault and be bend of metal rods. The overpass connects the once thriving market place of Abott Market, but now all you see is rubble and a shell of what used to be a thriving market.\r\n	\N	\N	1	0	\N	\N
160	289	1	0	Destroyed overpass - Abott Market South	Crushed and flattened by endless shells, the overpass that you just passed under creaks eternally. Now and then you hear the crumble of asphault and be bend of metal rods. The overpass connects the once thriving market place of Abott Market, but now all you see is rubble and a shell of what used to be a thriving market.\r\n	\N	\N	1	0	abbot-market-east-divergence	\N
161	290	1	0	Destroyed overpass - Abott Market South	Crushed and flattened by endless shells, the overpass that you just passed under creaks eternally. Now and then you hear the crumble of asphault and be bend of metal rods. The overpass connects the once thriving market place of Abott Market, but now all you see is rubble and a shell of what used to be a thriving market.\r\n	\N	\N	1	0	\N	\N
162	291	1	0	Market Apartments - East entrance	An iron rod gate can be seen laying discarded near the entrance. In another time, that gate would have been used to keep intruders out. Far off to the west is what used to be the tenant parking lot. You'd be surprised to find any tenant using that lot seeing as how low the income requirements were for this apartment complex.\r\n	\N	\N	1	0	\N	\N
163	292	1	0	Market Apartments - East entrance	An iron rod gate can be seen laying discarded near the entrance. In another time, that gate would have been used to keep intruders out. Far off to the west is what used to be the tenant parking lot. You'd be surprised to find any tenant using that lot seeing as how low the income requirements were for this apartment complex.\r\n	\N	\N	1	0	\N	\N
164	293	1	0	Market Apartments - Building Way	Building 2 is a two story building with 16 units. The stairs leading to the second story are completely demolished. The top 4 units to the east are completely exposed to the elements. You could make it upstairs but it would require some sort of rope. \r\n	\N	\N	1	0	\N	\N
165	294	1	0	Market Apartments - Building Way	Building 2 is a two story building with 16 units. The stairs leading to the second story are completely demolished. The top 4 units to the east are completely exposed to the elements. You could make it upstairs but it would require some sort of rope. \r\n	\N	\N	1	0	\N	\N
166	295	1	0	Market Apartments - Building Way	Building 2 is a two story building with 16 units. The stairs leading to the second story are completely demolished. The top 4 units to the east are completely exposed to the elements. You could make it upstairs but it would require some sort of rope. \r\n	\N	\N	1	0	\N	\N
167	296	1	0	Market Apartments - Building 3	You see a two story apartment building with 8 units. Each apartment is a corner unit, but neither home could possibly be more than a studio unit. A few of the doors are closed, which you find peculiar. There can't possibly be anyone living there. Right?\r\n	\N	\N	1	0	\N	\N
168	297	1	0	Market Apartments - Stairs	Before you lie the stairs that will help you reach the second floor of building 3. The handrails are extremely hot to the touch due to baking here under the steady gaze of the sun over your shoulder.\r\n	\N	\N	1	0	\N	\N
169	298	1	0	Market Apartments - Stairs	Before you lie the stairs that will help you reach the second floor of building 3. The handrails are extremely hot to the touch due to baking here under the steady gaze of the sun over your shoulder.\r\n	\N	\N	1	0	\N	\N
171	300	1	0	Market Apartments - Building 3 Second floor hallway	A thin layer of dust and dirt cover the cement walkway here. You hear the crackle of dirt and millions of pieces of broken glass under your feet as you make your way about. Oddly enough, there are footprints here.\r\n	\N	\N	1	0	\N	\N
172	301	1	0	Market Apartments - Building 3 Second floor hallway	A thin layer of dust and dirt cover the cement walkway here. You hear the crackle of dirt and millions of pieces of broken glass under your feet as you make your way about. Oddly enough, there are footprints here.\r\n	\N	\N	1	0	\N	\N
173	302	1	0	Market Apartments - Building 3 Second floor hallway	A thin layer of dust and dirt cover the cement walkway here. You hear the crackle of dirt and millions of pieces of broken glass under your feet as you make your way about. Oddly enough, there are footprints here.\r\n	\N	\N	1	0	\N	\N
174	303	1	0	Market Apartments - Building 3 Second floor hallway	A thin layer of dust and dirt cover the cement walkway here. You hear the crackle of dirt and millions of pieces of broken glass under your feet as you make your way about. Oddly enough, there are footprints here.\r\n	\N	\N	1	0	\N	\N
175	304	1	0	Market Apartments - Building 3 Second floor hallway	A thin layer of dust and dirt cover the cement walkway here. You hear the crackle of dirt and millions of pieces of broken glass under your feet as you make your way about. Oddly enough, there are footprints here.\r\n	\N	\N	1	0	\N	\N
176	305	1	0	Market Apartments - Building 3 - Unit 301	A mostly empty apartment. You notice what looks like sleeping bags in the corner but that could just be trash. The air smells like cigarette smoke and dry desert air. The light from outside illuminates trillions of dust particles floating in the in perpetual motion.\r\n	\N	\N	1	0	\N	\N
177	306	1	0	Market Apartments - Building 3 - Unit 301	A mostly empty apartment. You notice what looks like sleeping bags in the corner but that could just be trash. The air smells like cigarette smoke and dry desert air. The light from outside illuminates trillions of dust particles floating in the in perpetual motion.\r\n	\N	\N	1	0	\N	\N
178	307	1	0	Market Apartments - Building 3 - Unit 301 - Master Bedroom	A D.I.Y. home stove sits in the middle of the room. Aside from that, there's nothing to look at. The carpet has been pulled up in places. The walls are covered with massive holes and random burn marks.\r\n	\N	\N	1	0	\N	\N
179	308	1	0	Market Apartments - Building 3 - Unit 301 - Bathroom	The sink seems to still be intact and usable. How any water made its way up here would be nothing short of miraculous. It does appear that the sink has been used recently, which only affirms your suspicion that there are still inhabitants who dwell here.\r\n	\N	\N	1	0	\N	\N
180	309	1	0	Market Apartments - Building 3 Second floor hallway	A thin layer of dust and dirt cover the cement walkway here. You hear the crackle of dirt and millions of pieces of broken glass under your feet as you make your way about. Oddly enough, there are footprints here.\r\n	\N	\N	1	0	\N	\N
181	310	1	0	Market Apartments - Building 3 Second floor hallway	A thin layer of dust and dirt cover the cement walkway here. You hear the crackle of dirt and millions of pieces of broken glass under your feet as you make your way about. Oddly enough, there are footprints here.\r\n	\N	\N	1	0	\N	\N
210	339	1	0	{blu}C.O.F.O.B [OUTER]:{/blu} - Descending the stairs	fill me\r\n	\N	\N	1	0	\N	\N
182	311	1	0	Market Apartments - Building 3 Second floor hallway	A thin layer of dust and dirt cover the cement walkway here. You hear the crackle of dirt and millions of pieces of broken glass under your feet as you make your way about. Oddly enough, there are footprints here.\r\n	\N	\N	1	0	\N	\N
184	313	1	0	Market Apartments - Building 3 - Unit 303	A mostly empty apartment. You notice what looks like sleeping bags in the corner but that could just be trash. The air smells like cigarette smoke and dry desert air. The light from outside illuminates trillions of dust particles floating in the in perpetual motion.\r\n	\N	\N	1	0	\N	\N
186	315	1	0	Market Apartments - Building 3 - Unit 303 - Bathroom	The sink seems to still be intact and usable. How any water made its way up here would be nothing short of miraculous. It does appear that the sink has been used recently, which only affirms your suspicion that there are still inhabitants who dwell here.\r\n	\N	\N	1	0	\N	\N
187	316	1	0	Market Apartments - Building 3 Second floor hallway	A thin layer of dust and dirt cover the cement walkway here. You hear the crackle of dirt and millions of pieces of broken glass under your feet as you make your way about. Oddly enough, there are footprints here.\r\n	\N	\N	1	0	\N	\N
188	317	1	0	Market Apartments - Building 3 Second floor hallway	A thin layer of dust and dirt cover the cement walkway here. You hear the crackle of dirt and millions of pieces of broken glass under your feet as you make your way about. Oddly enough, there are footprints here.\r\n	\N	\N	1	0	\N	\N
189	318	1	0	Market Apartments - Building 3 - Unit 305	A mostly empty apartment. You notice what looks like sleeping bags in the corner but that could just be trash. The air smells like cigarette smoke and dry desert air. The light from outside illuminates trillions of dust particles floating in the in perpetual motion.\r\n	\N	\N	1	0	\N	\N
190	319	1	0	Market Apartments - Building 3 - Unit 305	A mostly empty apartment. You notice what looks like sleeping bags in the corner but that could just be trash. The air smells like cigarette smoke and dry desert air. The light from outside illuminates trillions of dust particles floating in the in perpetual motion.\r\n	\N	\N	1	0	\N	\N
191	320	1	0	Market Apartments - Building 3 - Unit 305 - Master Bedroom	undefined\r\n	\N	\N	1	0	\N	\N
192	321	1	0	Market Apartments - Building 3 - Unit 305 - Bathroom	The sink seems to still be intact and usable. How any water made its way up here would be nothing short of miraculous. It does appear that the sink has been used recently, which only affirms your suspicion that there are still inhabitants who dwell here.\r\n	\N	\N	1	0	\N	\N
193	322	1	0	name	description	\N	\N	1	0	\N	\N
194	323	1	0	Market Apartments - Building 3 Second floor hallway	A thin layer of dust and dirt cover the cement walkway here. You hear the crackle of dirt and millions of pieces of broken glass under your feet as you make your way about. Oddly enough, there are footprints here.\r\n	\N	\N	1	0	\N	\N
197	326	1	0	Market Apartments - Building 3 - Unit 307	A mostly empty apartment. You notice what looks like sleeping bags in the corner but that could just be trash. The air smells like cigarette smoke and dry desert air. The light from outside illuminates trillions of dust particles floating in the in perpetual motion.\r\n	\N	\N	1	0	\N	\N
198	327	1	0	Market Apartments - Building 3 - Unit 307 - Master Bedroom	undefined\r\n	\N	\N	1	0	\N	\N
199	328	1	0	Market Apartments - Building 3 - Unit 307 - Bathroom	The sink seems to still be intact and usable. How any water made its way up here would be nothing short of miraculous. It does appear that the sink has been used recently, which only affirms your suspicion that there are still inhabitants who dwell here.\r\n	\N	\N	1	0	\N	\N
200	329	1	0	Market Apartments - Building Way	Building 2 is a two story building with 16 units. The stairs leading to the second story are completely demolished. The top 4 units to the east are completely exposed to the elements. You could make it upstairs but it would require some sort of rope. \r\n	\N	\N	1	0	\N	\N
201	330	1	0	Market Apartments - Building Way	Building 2 is a two story building with 16 units. The stairs leading to the second story are completely demolished. The top 4 units to the east are completely exposed to the elements. You could make it upstairs but it would require some sort of rope. \r\n	\N	\N	1	0	\N	\N
202	331	1	0	Market Apartments - Building Way	Building 2 is a two story building with 16 units. The stairs leading to the second story are completely demolished. The top 4 units to the east are completely exposed to the elements. You could make it upstairs but it would require some sort of rope. \r\n	\N	\N	1	0	\N	\N
203	332	1	0	Market Apartments - Building 2	Building 1 is a two story building with 8 units with the added addition of an exercise room and what looks like the property manager's office. Both the exercise room and the office look surprisingly intact. You notice lots of fresh footprints to and from building 1 which disappear into the destruction of building 2. Despite the abandoned nature of the complex as a whole, someone is still finding use for these units.\r\n	\N	\N	1	0	\N	\N
204	333	1	0	Abandoned Two way street - Abott Market	A trio of burning cars have become part of the debris scattered along the street. Way off to the north, you spot a working overpass. Deep long scars of blackened concrete tell a tale of destruction. To the west lies a small set of apartments once owned by the only property company to operate in this desolate portion of town. A giant construction crane is resting in the middle of the street to the north.\r\n	\N	\N	1	0	\N	\N
205	334	1	0	Abandoned Two way street - Abott Market	A trio of burning cars have become part of the debris scattered along the street. Way off to the north, you spot a working overpass. Deep long scars of blackened concrete tell a tale of destruction. To the west lies a small set of apartments once owned by the only property company to operate in this desolate portion of town. A giant construction crane is resting in the middle of the street to the north.\r\n	\N	\N	1	0	\N	\N
206	335	1	0	Abandoned Two way street - Abott Market	A trio of burning cars have become part of the debris scattered along the street. Way off to the north, you spot a working overpass. Deep long scars of blackened concrete tell a tale of destruction. To the west lies a small set of apartments once owned by the only property company to operate in this desolate portion of town. A giant construction crane is resting in the middle of the street to the north.\r\n	\N	\N	1	0	\N	\N
208	337	1	0	Abandoned Two way street - Abott Market	A trio of burning cars have become part of the debris scattered along the street. Way off to the north, you spot a working overpass. Deep long scars of blackened concrete tell a tale of destruction. To the west lies a small set of apartments once owned by the only property company to operate in this desolate portion of town. A giant construction crane is resting in the middle of the street to the north.\r\n	\N	\N	1	0	\N	\N
209	338	1	0	{blu}C.O.F.O.B [OUTER]:{/blu} - Descending the stairs	fill me\r\n	\N	\N	1	0	\N	\N
211	340	1	0	{blu}C.O.F.O.B:{/blu} - Mess Hall stairs	Metal railings accompany the stairs leading down to the mess hall.\r\n	\N	\N	1	0	\N	\N
212	341	1	0	{blu}C.O.F.O.B:{/blu} - Mess Hall stairs	Metal railings accompany the stairs leading down to the mess hall.\r\n	\N	\N	1	0	\N	\N
213	342	1	0	{blu}C.O.F.O.B:{/blu} - Mess Hall stairs	Metal railings accompany the stairs leading down to the mess hall.\r\n	\N	\N	1	0	\N	\N
214	343	1	0	{blu}C.O.F.O.B:{/blu} - Mess Hall stairs	Metal railings accompany the stairs leading down to the mess hall.\r\n	\N	\N	1	0	\N	\N
215	344	1	0	{blu}C.O.F.O.B:{/blu} - Mess Hall stairs	Metal railings accompany the stairs leading down to the mess hall.\r\n	\N	\N	1	0	\N	\N
216	345	1	0	{blu}C.O.F.O.B:{/blu} - Mess Hall stairs	Metal railings accompany the stairs leading down to the mess hall.\r\n	\N	\N	1	0	\N	\N
217	346	1	0	{blu}C.O.F.O.B:{/blu} - Mess Hall stairs	Metal railings accompany the stairs leading down to the mess hall.\r\n	\N	\N	1	0	\N	\N
218	347	1	0	{blu}C.O.F.O.B:{/blu} - Mess Hall serving area	The tile floor is immaculate in presentation. Plain grey metal chairs attend each table obediently in a neat formation of 2 chairs to each cardinal side.A few familiar dishes invite you to sit down and enjoy the hastily prepared meals. The chefs can be seen in the kitchen looking down as they prepare the food. The remote promise of coffee is enticing.\r\n	\N	\N	1	0	\N	\N
219	348	1	0	{blu}C.O.F.O.B:{/blu} - Mess Hall serving area	The tile floor is immaculate in presentation. Plain grey metal chairs attend each table obediently in a neat formation of 2 chairs to each cardinal side.A few familiar dishes invite you to sit down and enjoy the hastily prepared meals. The chefs can be seen in the kitchen looking down as they prepare the food. The remote promise of coffee is enticing.\r\n	\N	\N	1	0	\N	\N
232	149	1	21	{blu}C.O.F.O.B:{/blu} - Weapons Cache stairs - TOP	@FILL_ME@\r\n	\N	\N	1	0	\N	\N
23	152	1	21	{blu}C.O.F.O.B:{/blu} - Weapons Cache stairs - Basement 1	@FILL_ME@\r\n	\N	\N	1	0	\N	\N
48	177	1	21	{blu}C.O.F.O.B:{/blu} - Basement 2 - Waypoint Avenue North Exit	@FILL_ME@\r\n	\N	\N	1	0	cofob-armory-waypoint-avenue-exit	\N
70	199	1	21	{blu}C.O.F.O.B:{/blu} - Eastern Exit	Before you is a sturdy metal door that prevents the outside elements from making their way inside. The door simply says {yel}Eastern Exit{/yel}. The air is less cool as the corridor leading to the east lacks the sufficient air flow. There are no ventilation shafts leading to the east, but you can see through the tempered glass window on the door that a few military police are gaurding the exit to the city outside. \r\n	\N	\N	1	0	eastexit	\N
84	213	1	0	{blu}C.O.F.O.B [OUTER]:{/blu} - Western Hallway	The reinforced metal walls are briefly interrupted by the occasional senior officer door. As you make your way north and south, you notice that the entirety of the western wall consists of metal walls with doors that lead to the underground portion of the base. \r\n	\N	\N	1	0	\N	\N
221	350	1	0	{blu}C.O.F.O.B:{/blu} - Mess Hall serving area	The tile floor is immaculate in presentation. Plain grey metal chairs attend each table obediently in a neat formation of 2 chairs to each cardinal side.A few familiar dishes invite you to sit down and enjoy the hastily prepared meals. The chefs can be seen in the kitchen looking down as they prepare the food. The remote promise of coffee is enticing.\r\n	\N	\N	1	0	\N	\N
222	351	1	0	{blu}C.O.F.O.B:{/blu} - Mess Hall serving area	The tile floor is immaculate in presentation. Plain grey metal chairs attend each table obediently in a neat formation of 2 chairs to each cardinal side.A few familiar dishes invite you to sit down and enjoy the hastily prepared meals. The chefs can be seen in the kitchen looking down as they prepare the food. The remote promise of coffee is enticing.\r\n	\N	\N	1	0	\N	\N
223	352	1	0	{blu}C.O.F.O.B:{/blu} - Mess Hall serving area	The tile floor is immaculate in presentation. Plain grey metal chairs attend each table obediently in a neat formation of 2 chairs to each cardinal side.A few familiar dishes invite you to sit down and enjoy the hastily prepared meals. The chefs can be seen in the kitchen looking down as they prepare the food. The remote promise of coffee is enticing.\r\n	\N	\N	1	0	\N	\N
224	353	1	0	{blu}C.O.F.O.B:{/blu} - Mess Hall serving area	The tile floor is immaculate in presentation. Plain grey metal chairs attend each table obediently in a neat formation of 2 chairs to each cardinal side.A few familiar dishes invite you to sit down and enjoy the hastily prepared meals. The chefs can be seen in the kitchen looking down as they prepare the food. The remote promise of coffee is enticing.\r\n	\N	\N	1	0	\N	\N
225	354	1	0	{blu}C.O.F.O.B:{/blu} - Mess Hall serving area	The tile floor is immaculate in presentation. Plain grey metal chairs attend each table obediently in a neat formation of 2 chairs to each cardinal side.A few familiar dishes invite you to sit down and enjoy the hastily prepared meals. The chefs can be seen in the kitchen looking down as they prepare the food. The remote promise of coffee is enticing.\r\n	\N	\N	1	0	\N	\N
226	355	1	0	{blu}C.O.F.O.B:{/blu} - Mess Hall serving area	The tile floor is immaculate in presentation. Plain grey metal chairs attend each table obediently in a neat formation of 2 chairs to each cardinal side.A few familiar dishes invite you to sit down and enjoy the hastily prepared meals. The chefs can be seen in the kitchen looking down as they prepare the food. The remote promise of coffee is enticing.\r\n	\N	\N	1	0	\N	\N
227	356	1	0	{blu}C.O.F.O.B:{/blu} - Mess Hall serving area	The tile floor is immaculate in presentation. Plain grey metal chairs attend each table obediently in a neat formation of 2 chairs to each cardinal side.A few familiar dishes invite you to sit down and enjoy the hastily prepared meals. The chefs can be seen in the kitchen looking down as they prepare the food. The remote promise of coffee is enticing.\r\n	\N	\N	1	0	\N	\N
228	357	1	0	{blu}C.O.F.O.B:{/blu} - Mess Hall serving area	The tile floor is immaculate in presentation. Plain grey metal chairs attend each table obediently in a neat formation of 2 chairs to each cardinal side.A few familiar dishes invite you to sit down and enjoy the hastily prepared meals. The chefs can be seen in the kitchen looking down as they prepare the food. The remote promise of coffee is enticing.\r\n	\N	\N	1	0	\N	\N
128	257	1	21	{blu}C.O.F.O.B{/blu} - Engineering - Infrastructure Hallway	It's almost comical how clean the engineering level of the FOB is compared to the lower levels. The carpet and walls are in a prestine -- no, IMMACULATE state. You almost feel like you should take off your shoes. The climate is controlled by a different air-conditioning system than the rest of the base. The room is cool and devoid of any outside noise.\r\n	\N	\N	1	0	\N	,CEMENT,INSIDE
333	461	1	0	{blu}La Mesa{/blu} Kenwood Drive North	@FILL_ME@\r\n	\N	\N	1	0	\N	\N
334	462	1	0	{blu}La Mesa{/blu} Kenwood Drive North	@FILL_ME@\r\n	\N	\N	1	0	\N	\N
229	358	1	0	{blu}C.O.F.O.B:{/blu} - Mess Hall serving area	The tile floor is immaculate in presentation. Plain grey metal chairs attend each table obediently in a neat formation of 2 chairs to each cardinal side.A few familiar dishes invite you to sit down and enjoy the hastily prepared meals. The chefs can be seen in the kitchen looking down as they prepare the food. The remote promise of coffee is enticing.\r\n	\N	\N	1	0	\N	\N
230	359	1	0	{blu}C.O.F.O.B:{/blu} - Mess Hall serving area	The tile floor is immaculate in presentation. Plain grey metal chairs attend each table obediently in a neat formation of 2 chairs to each cardinal side.A few familiar dishes invite you to sit down and enjoy the hastily prepared meals. The chefs can be seen in the kitchen looking down as they prepare the food. The remote promise of coffee is enticing.\r\n	\N	\N	1	0	\N	\N
231	360	1	0	{blu}C.O.F.O.B:{/blu} - Mess Hall serving area	The tile floor is immaculate in presentation. Plain grey metal chairs attend each table obediently in a neat formation of 2 chairs to each cardinal side.A few familiar dishes invite you to sit down and enjoy the hastily prepared meals. The chefs can be seen in the kitchen looking down as they prepare the food. The remote promise of coffee is enticing.\r\n	\N	\N	1	0	\N	\N
129	258	1	0	{blu}C.O.F.O.B:{/blu} - Engineering hallway	The doors on the eastern wall are all closed and most likely locked. You see the commons area at the end of the hallway. You notice various surveillance cameras strategically placed at the corners of the ceiling. \r\n	\N	\N	1	0	\N	\N
133	262	1	0	{blu}C.O.F.O.B:{/blu} - Commons area	A comfy looking couch and recliner welcome you to the engineering commons area. Three television screens mounted on the west wall display various graphs and statistics. A ping pong table is in the center of the room. Two large refridgerators are humming quietly in the corner. The north wall consists of large one-way tinted windows that overlook the training fields below. To the east is the entrance to server room 1B.\r\n	\N	\N	1	0	cofob-engineering-commons	\N
150	279	1	0	Destroyed overpass - Abott Market South	Crushed and flattened by endless shells, the overpass that you just passed under creaks eternally. Now and then you hear the crumble of asphault and be bend of metal rods. The overpass connects the once thriving market place of Abott Market, but now all you see is rubble and a shell of what used to be a thriving market.\r\n	\N	\N	1	0	\N	\N
170	299	1	0	Market Apartments - Stairs	Before you lie the stairs that will help you reach the second floor of building 3. The handrails are extremely hot to the touch due to baking here under the steady gaze of the sun over your shoulder.\r\n	\N	\N	1	0	\N	\N
183	312	1	0	Market Apartments - Building 3 - Unit 303	A mostly empty apartment. You notice what looks like sleeping bags in the corner but that could just be trash. The air smells like cigarette smoke and dry desert air. The light from outside illuminates trillions of dust particles floating in the in perpetual motion.\r\n	\N	\N	1	0	\N	\N
195	324	1	0	Market Apartments - Building 3 Second floor hallway	A thin layer of dust and dirt cover the cement walkway here. You hear the crackle of dirt and millions of pieces of broken glass under your feet as you make your way about. Oddly enough, there are footprints here.\r\n	\N	\N	1	0	\N	\N
196	325	1	0	Market Apartments - Building 3 - Unit 307	A mostly empty apartment. You notice what looks like sleeping bags in the corner but that could just be trash. The air smells like cigarette smoke and dry desert air. The light from outside illuminates trillions of dust particles floating in the in perpetual motion.\r\n	\N	\N	1	0	\N	\N
220	349	1	0	{blu}C.O.F.O.B:{/blu} - Mess Hall serving area	The tile floor is immaculate in presentation. Plain grey metal chairs attend each table obediently in a neat formation of 2 chairs to each cardinal side.A few familiar dishes invite you to sit down and enjoy the hastily prepared meals. The chefs can be seen in the kitchen looking down as they prepare the food. The remote promise of coffee is enticing.\r\n	\N	\N	1	0	\N	\N
233	361	1	0	{blu}Crenshaw{/blu} Highway overpass	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	crenshaw-overpass-start	\N
234	362	1	0	{blu}Crenshaw{/blu} Highway overpass - Ramp	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	\N
235	363	1	0	{blu}Crenshaw{/blu} Highway overpass - Ramp	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	\N
236	364	1	0	{blu}Crenshaw{/blu} Highway overpass - Ramp	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	\N
432	560	1	0	{blu}Saint Vale Church{/blu} Inside - Isle	A dark colored wooden bench is here. There is a retractable padded kneeling device below the hanging bookshelf of psalms and prayers. \r\n	\N	\N	1	0	\N	\N
238	366	1	0	{blu}Crenshaw{/blu} Highway overpass - North	The ramp curves to the north. To the east you see a long stretch of highway that disappears over the horizon. You see a shipyard far off to the northeast. \r\n	\N	\N	1	0	\N	\N
239	367	1	0	{blu}Crenshaw{/blu} Highway overpass - North	The ramp curves to the north. To the east you see a long stretch of highway that disappears over the horizon. You see a shipyard far off to the northeast. \r\n	\N	\N	1	0	\N	\N
240	368	1	0	{blu}Crenshaw{/blu} Highway 94 East	Six lanes of traffic move for off to the east as far as the eye can see. You see hills of varying heights hugging the highway's north and south sides. There is sparse greenery along the freeway. \r\n	\N	\N	1	0	\N	\N
241	369	1	0	{blu}Crenshaw{/blu} Highway 94 East	Six lanes of traffic move for off to the east as far as the eye can see. You see hills of varying heights hugging the highway's north and south sides. There is sparse greenery along the freeway. \r\n	\N	\N	1	0	\N	\N
242	370	1	0	{blu}Crenshaw{/blu} Highway 94 East	Six lanes of traffic move for off to the east as far as the eye can see. You see hills of varying heights hugging the highway's north and south sides. There is sparse greenery along the freeway. \r\n	\N	\N	1	0	\N	\N
243	371	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	\N
244	372	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	\N
340	468	1	0	{blu}La Mesa{/blu} Campo Road 9600	Campo Road is a bland introduction to the hot and boring town of Spruce Valley. The roads are decorated with a Mexican food restaurant every block or so. A few signs advertise the presence of the {red}Saint Vale Church{/red}.\r\n	\N	\N	1	0	campo-conrad-drive-intersection	\N
245	373	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	\N
246	374	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	\N
247	375	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	\N
248	376	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	\N
249	377	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	\N
250	378	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	\N
251	379	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	\N
252	380	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	\N
253	381	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	\N
254	382	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	\N
255	383	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	\N
433	561	1	0	{blu}Saint Vale Church{/blu} Inside - Isle	A dark colored wooden bench is here. There is a retractable padded kneeling device below the hanging bookshelf of psalms and prayers. \r\n	\N	\N	1	0	\N	\N
256	384	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	\N
257	385	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	\N
258	386	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	\N
335	463	1	0	{blu}La Mesa{/blu} Kenwood Drive North	@FILL_ME@\r\n	\N	\N	1	0	\N	\N
259	387	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	\N
260	388	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	\N
261	389	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	\N
262	390	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	\N
263	391	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	\N
264	392	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	\N
265	393	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	\N
266	394	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	\N
267	395	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	\N
268	396	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	\N
269	397	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	\N
270	398	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	\N
271	399	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	\N
272	400	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	crenshaw-northern-shipping-entrance	\N
273	401	1	0	{blu}Shipyard{/blu} Entrance	Shipping containers stacked 30 to 40 stories high blot out the sun's rays coming from the east and even overhead during lunch time. Despite the time of day, artificial lighting is needed everywhere. As you make your way deeper into the shipyard, you notice several highly armed individuals patrolling the area. These individuals are wearing masks and have extensive radio communication devices that are resistant to E.M.P.. \r\n	\N	\N	1	0	\N	\N
422	550	1	0	{blu}Saint Vale Church{/blu} Inside - Center Isle	An elaborate purple and yellow pattern decorates the isle which leads to the altar. The corners of each isle have divine sigils engraved into the dark stain of the wood.\r\n	\N	\N	1	0	\N	\N
274	402	1	0	{blu}Shipyard{/blu} Entrance	Shipping containers stacked 30 to 40 stories high blot out the sun's rays coming from the east and even overhead during lunch time. Despite the time of day, artificial lighting is needed everywhere. As you make your way deeper into the shipyard, you notice several highly armed individuals patrolling the area. These individuals are wearing masks and have extensive radio communication devices that are resistant to E.M.P.. \r\n	\N	\N	1	0	\N	\N
275	403	1	0	{blu}Shipyard{/blu} Entrance	Shipping containers stacked 30 to 40 stories high blot out the sun's rays coming from the east and even overhead during lunch time. Despite the time of day, artificial lighting is needed everywhere. As you make your way deeper into the shipyard, you notice several highly armed individuals patrolling the area. These individuals are wearing masks and have extensive radio communication devices that are resistant to E.M.P.. \r\n	\N	\N	1	0	shipyard-row-a	\N
276	404	1	0	{blu}Shipyard{/blu} Shipment Row A	All around you are red and blue shipping containers with varying amounts of rust and outer wear. A few have severe indentations from mishaps. Oddly enough, containers with giant abrasions are bent back into shape and reused. You notice what seems to be burn marks, bullet holes, and dark splattered brownish red stains on a few containers. \r\n	\N	\N	1	0	\N	\N
277	405	1	0	{blu}Shipyard{/blu} Shipment Row A	All around you are red and blue shipping containers with varying amounts of rust and outer wear. A few have severe indentations from mishaps. Oddly enough, containers with giant abrasions are bent back into shape and reused. You notice what seems to be burn marks, bullet holes, and dark splattered brownish red stains on a few containers. \r\n	\N	\N	1	0	\N	\N
278	406	1	0	{blu}Shipyard{/blu} Shipment Row A	All around you are red and blue shipping containers with varying amounts of rust and outer wear. A few have severe indentations from mishaps. Oddly enough, containers with giant abrasions are bent back into shape and reused. You notice what seems to be burn marks, bullet holes, and dark splattered brownish red stains on a few containers. \r\n	\N	\N	1	0	\N	\N
279	407	1	0	{blu}Shipyard{/blu} Shipment Row A	All around you are red and blue shipping containers with varying amounts of rust and outer wear. A few have severe indentations from mishaps. Oddly enough, containers with giant abrasions are bent back into shape and reused. You notice what seems to be burn marks, bullet holes, and dark splattered brownish red stains on a few containers. \r\n	\N	\N	1	0	\N	\N
280	408	1	0	{blu}Shipyard{/blu} Shipment Row A	All around you are red and blue shipping containers with varying amounts of rust and outer wear. A few have severe indentations from mishaps. Oddly enough, containers with giant abrasions are bent back into shape and reused. You notice what seems to be burn marks, bullet holes, and dark splattered brownish red stains on a few containers. \r\n	\N	\N	1	0	\N	\N
281	409	1	0	{blu}Shipyard{/blu} Shipment Row A	All around you are red and blue shipping containers with varying amounts of rust and outer wear. A few have severe indentations from mishaps. Oddly enough, containers with giant abrasions are bent back into shape and reused. You notice what seems to be burn marks, bullet holes, and dark splattered brownish red stains on a few containers. \r\n	\N	\N	1	0	\N	\N
282	410	1	0	{blu}Shipyard{/blu} Shipment Row A	All around you are red and blue shipping containers with varying amounts of rust and outer wear. A few have severe indentations from mishaps. Oddly enough, containers with giant abrasions are bent back into shape and reused. You notice what seems to be burn marks, bullet holes, and dark splattered brownish red stains on a few containers. \r\n	\N	\N	1	0	\N	\N
283	411	1	0	{blu}Shipyard{/blu} Shipment Row A	All around you are red and blue shipping containers with varying amounts of rust and outer wear. A few have severe indentations from mishaps. Oddly enough, containers with giant abrasions are bent back into shape and reused. You notice what seems to be burn marks, bullet holes, and dark splattered brownish red stains on a few containers. \r\n	\N	\N	1	0	\N	\N
284	412	1	0	{blu}Shipyard{/blu} Shipment Row A	All around you are red and blue shipping containers with varying amounts of rust and outer wear. A few have severe indentations from mishaps. Oddly enough, containers with giant abrasions are bent back into shape and reused. You notice what seems to be burn marks, bullet holes, and dark splattered brownish red stains on a few containers. \r\n	\N	\N	1	0	\N	\N
285	413	1	0	{blu}Shipyard{/blu} Shipment Row A	All around you are red and blue shipping containers with varying amounts of rust and outer wear. A few have severe indentations from mishaps. Oddly enough, containers with giant abrasions are bent back into shape and reused. You notice what seems to be burn marks, bullet holes, and dark splattered brownish red stains on a few containers. \r\n	\N	\N	1	0	\N	\N
286	414	1	0	{blu}Shipyard{/blu} Shipment Row A	All around you are red and blue shipping containers with varying amounts of rust and outer wear. A few have severe indentations from mishaps. Oddly enough, containers with giant abrasions are bent back into shape and reused. You notice what seems to be burn marks, bullet holes, and dark splattered brownish red stains on a few containers. \r\n	\N	\N	1	0	\N	\N
287	415	1	0	{blu}Shipyard{/blu} Shipment Row A	All around you are red and blue shipping containers with varying amounts of rust and outer wear. A few have severe indentations from mishaps. Oddly enough, containers with giant abrasions are bent back into shape and reused. You notice what seems to be burn marks, bullet holes, and dark splattered brownish red stains on a few containers. \r\n	\N	\N	1	0	\N	\N
288	416	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	\N
289	417	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	\N
423	551	1	0	{blu}Saint Vale Church{/blu} Inside - Center Isle	An elaborate purple and yellow pattern decorates the isle which leads to the altar. The corners of each isle have divine sigils engraved into the dark stain of the wood.\r\n	\N	\N	1	0	\N	\N
290	418	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	\N
291	419	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	\N
292	420	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	\N
336	464	1	0	{blu}La Mesa{/blu} Kenwood Drive North	@FILL_ME@\r\n	\N	\N	1	0	kenwood-drive-campo-road	\N
293	421	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	\N
294	422	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	\N
295	423	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	\N
441	569	1	0	{blu}Saint Vale Church{/blu} Inside - Altar	A table is here with a decorative cloth draped over it. On each side of the table are tall stands that hold long white candles. There is a stand with a microphone embedded into the wood.\r\n	\N	\N	1	0	\N	\N
296	424	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	\N
297	425	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	\N
298	426	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	\N
299	427	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	\N
300	428	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	\N
301	429	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	\N
302	430	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	\N
303	431	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	\N
304	432	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	\N
305	433	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	\N
424	552	1	0	{blu}Saint Vale Church{/blu} Inside - Center Isle	An elaborate purple and yellow pattern decorates the isle which leads to the altar. The corners of each isle have divine sigils engraved into the dark stain of the wood.\r\n	\N	\N	1	0	\N	\N
434	562	1	0	{blu}Saint Vale Church{/blu} Inside - Isle	A dark colored wooden bench is here. There is a retractable padded kneeling device below the hanging bookshelf of psalms and prayers. \r\n	\N	\N	1	0	\N	\N
306	434	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	\N
307	435	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	\N
308	436	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	\N
309	437	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	\N
310	438	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	\N
311	439	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	\N
312	440	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	\N
313	441	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	\N
314	442	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	\N
315	443	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	\N
316	444	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	\N
317	445	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	\N
318	446	1	0	{blu}La Mesa{/blu} Campo Road Exit	Exit 10B to Campo Road.\r\n	\N	\N	1	0	\N	\N
319	447	1	0	{blu}La Mesa{/blu} Campo Road Exit	Exit 10B to Campo Road.\r\n	\N	\N	1	0	\N	\N
320	448	1	0	{blu}La Mesa{/blu} Campo Road Exit	Exit 10B to Campo Road.\r\n	\N	\N	1	0	\N	\N
321	449	1	0	{blu}La Mesa{/blu} Campo Road Exit	Exit 10B to Campo Road.\r\n	\N	\N	1	0	\N	\N
322	450	1	0	{blu}La Mesa{/blu} Exit 10B On Ramp	A bright orange sign on the right hand shoulder says {yel}END ROAD WORK{/yel}. You see a diamond shaped yellow sign with a black arrow pointing upwards and a {red}red{/red} octagon.The road slopes downwards and on the road you see 'STOP AHEAD' written in white painted on the ground. A narrow shoulder on your right is guarded by a grey metal and wooden railing.\r\n	\N	\N	1	0	\N	\N
323	451	1	0	{blu}La Mesa{/blu} Exit 10B On Ramp	A bright orange sign on the right hand shoulder says {yel}END ROAD WORK{/yel}. You see a diamond shaped yellow sign with a black arrow pointing upwards and a {red}red{/red} octagon.The road slopes downwards and on the road you see 'STOP AHEAD' written in white painted on the ground. A narrow shoulder on your right is guarded by a grey metal and wooden railing.\r\n	\N	\N	1	0	\N	\N
324	452	1	0	{blu}La Mesa{/blu} Exit 10B On Ramp	A bright orange sign on the right hand shoulder says {yel}END ROAD WORK{/yel}. You see a diamond shaped yellow sign with a black arrow pointing upwards and a {red}red{/red} octagon.The road slopes downwards and on the road you see 'STOP AHEAD' written in white painted on the ground. A narrow shoulder on your right is guarded by a grey metal and wooden railing.\r\n	\N	\N	1	0	\N	\N
325	453	1	0	{blu}La Mesa{/blu} Exit 10B On Ramp	A bright orange sign on the right hand shoulder says {yel}END ROAD WORK{/yel}. You see a diamond shaped yellow sign with a black arrow pointing upwards and a {red}red{/red} octagon.The road slopes downwards and on the road you see 'STOP AHEAD' written in white painted on the ground. A narrow shoulder on your right is guarded by a grey metal and wooden railing.\r\n	\N	\N	1	0	\N	\N
326	454	1	0	{blu}La Mesa{/blu} Exit 10B On Ramp	A bright orange sign on the right hand shoulder says {yel}END ROAD WORK{/yel}. You see a diamond shaped yellow sign with a black arrow pointing upwards and a {red}red{/red} octagon.The road slopes downwards and on the road you see 'STOP AHEAD' written in white painted on the ground. A narrow shoulder on your right is guarded by a grey metal and wooden railing.\r\n	\N	\N	1	0	\N	\N
327	455	1	0	{blu}La Mesa{/blu} Exit 10B On Ramp	A bright orange sign on the right hand shoulder says {yel}END ROAD WORK{/yel}. You see a diamond shaped yellow sign with a black arrow pointing upwards and a {red}red{/red} octagon.The road slopes downwards and on the road you see 'STOP AHEAD' written in white painted on the ground. A narrow shoulder on your right is guarded by a grey metal and wooden railing.\r\n	\N	\N	1	0	\N	\N
328	456	1	0	{blu}La Mesa{/blu} Kenwood Drive Intersection	You can take a left or right and you would be on Kenwood Drive. Go straight ahead and you can get back on Highway 94 East.\r\n	\N	\N	1	0	kenwood-drive-intersection	\N
329	457	1	0	{blu}La Mesa{/blu} Kenwood Drive North	@FILL_ME@\r\n	\N	\N	1	0	\N	\N
330	458	1	0	{blu}La Mesa{/blu} Kenwood Drive North	@FILL_ME@\r\n	\N	\N	1	0	\N	\N
331	459	1	0	{blu}La Mesa{/blu} Kenwood Drive North	@FILL_ME@\r\n	\N	\N	1	0	\N	\N
332	460	1	0	{blu}La Mesa{/blu} Kenwood Drive North	@FILL_ME@\r\n	\N	\N	1	0	\N	\N
337	465	1	0	{blu}La Mesa{/blu} Campo Road 9600	Campo Road is a bland introduction to the hot and boring town of Spruce Valley. The roads are decorated with a Mexican food restaurant every block or so. A few signs advertise the presence of the {red}Saint Vale Church{/red}.\r\n	\N	\N	1	0	\N	\N
338	466	1	0	{blu}La Mesa{/blu} Campo Road 9600	Campo Road is a bland introduction to the hot and boring town of Spruce Valley. The roads are decorated with a Mexican food restaurant every block or so. A few signs advertise the presence of the {red}Saint Vale Church{/red}.\r\n	\N	\N	1	0	\N	\N
339	467	1	0	{blu}La Mesa{/blu} Campo Road 9600	Campo Road is a bland introduction to the hot and boring town of Spruce Valley. The roads are decorated with a Mexican food restaurant every block or so. A few signs advertise the presence of the {red}Saint Vale Church{/red}.\r\n	\N	\N	1	0	\N	\N
341	469	1	0	{blu}La Mesa{/blu} Conrad Road	Nearly every building has the same Aztec style rooftops. The same construction company that worked on every building on Campo Road also worked on the apartment buildings here. That same company was shutdown due to activities related to money laundering. As low income as this neighborhood is, there are no signs of an obvious wealth inequality here. The residents here are like every other community of hard working parents trying to provide for their family.\r\n	\N	\N	1	0	\N	\N
342	470	1	0	{blu}La Mesa{/blu} Conrad Road	Nearly every building has the same Aztec style rooftops. The same construction company that worked on every building on Campo Road also worked on the apartment buildings here. That same company was shutdown due to activities related to money laundering. As low income as this neighborhood is, there are no signs of an obvious wealth inequality here. The residents here are like every other community of hard working parents trying to provide for their family.\r\n	\N	\N	1	0	\N	\N
343	471	1	0	{blu}La Mesa{/blu} Conrad Road	Nearly every building has the same Aztec style rooftops. The same construction company that worked on every building on Campo Road also worked on the apartment buildings here. That same company was shutdown due to activities related to money laundering. As low income as this neighborhood is, there are no signs of an obvious wealth inequality here. The residents here are like every other community of hard working parents trying to provide for their family.\r\n	\N	\N	1	0	\N	\N
344	472	1	0	{blu}La Mesa{/blu} Conrad Road	Nearly every building has the same Aztec style rooftops. The same construction company that worked on every building on Campo Road also worked on the apartment buildings here. That same company was shutdown due to activities related to money laundering. As low income as this neighborhood is, there are no signs of an obvious wealth inequality here. The residents here are like every other community of hard working parents trying to provide for their family.\r\n	\N	\N	1	0	\N	\N
345	473	1	0	{blu}La Mesa{/blu} Conrad Road	Nearly every building has the same Aztec style rooftops. The same construction company that worked on every building on Campo Road also worked on the apartment buildings here. That same company was shutdown due to activities related to money laundering. As low income as this neighborhood is, there are no signs of an obvious wealth inequality here. The residents here are like every other community of hard working parents trying to provide for their family.\r\n	\N	\N	1	0	\N	\N
346	474	1	0	{blu}La Mesa{/blu} Conrad Road	Nearly every building has the same Aztec style rooftops. The same construction company that worked on every building on Campo Road also worked on the apartment buildings here. That same company was shutdown due to activities related to money laundering. As low income as this neighborhood is, there are no signs of an obvious wealth inequality here. The residents here are like every other community of hard working parents trying to provide for their family.\r\n	\N	\N	1	0	\N	\N
347	475	1	0	{blu}La Mesa{/blu} Conrad Road	Nearly every building has the same Aztec style rooftops. The same construction company that worked on every building on Campo Road also worked on the apartment buildings here. That same company was shutdown due to activities related to money laundering. As low income as this neighborhood is, there are no signs of an obvious wealth inequality here. The residents here are like every other community of hard working parents trying to provide for their family.\r\n	\N	\N	1	0	\N	\N
348	476	1	0	{blu}La Mesa{/blu} Conrad Road	Nearly every building has the same Aztec style rooftops. The same construction company that worked on every building on Campo Road also worked on the apartment buildings here. That same company was shutdown due to activities related to money laundering. As low income as this neighborhood is, there are no signs of an obvious wealth inequality here. The residents here are like every other community of hard working parents trying to provide for their family.\r\n	\N	\N	1	0	\N	\N
425	553	1	0	{blu}Saint Vale Church{/blu} Inside - Center Isle	An elaborate purple and yellow pattern decorates the isle which leads to the altar. The corners of each isle have divine sigils engraved into the dark stain of the wood.\r\n	\N	\N	1	0	saint-vale-altar	\N
349	477	1	0	{blu}La Mesa{/blu} Conrad Road	Nearly every building has the same Aztec style rooftops. The same construction company that worked on every building on Campo Road also worked on the apartment buildings here. That same company was shutdown due to activities related to money laundering. As low income as this neighborhood is, there are no signs of an obvious wealth inequality here. The residents here are like every other community of hard working parents trying to provide for their family.\r\n	\N	\N	1	0	\N	\N
350	478	1	0	{blu}La Mesa{/blu} Conrad Road	Nearly every building has the same Aztec style rooftops. The same construction company that worked on every building on Campo Road also worked on the apartment buildings here. That same company was shutdown due to activities related to money laundering. As low income as this neighborhood is, there are no signs of an obvious wealth inequality here. The residents here are like every other community of hard working parents trying to provide for their family.\r\n	\N	\N	1	0	\N	\N
351	479	1	0	{blu}La Mesa{/blu} Conrad Road	Nearly every building has the same Aztec style rooftops. The same construction company that worked on every building on Campo Road also worked on the apartment buildings here. That same company was shutdown due to activities related to money laundering. As low income as this neighborhood is, there are no signs of an obvious wealth inequality here. The residents here are like every other community of hard working parents trying to provide for their family.\r\n	\N	\N	1	0	\N	\N
352	480	1	0	{blu}La Mesa{/blu} Conrad Road	Nearly every building has the same Aztec style rooftops. The same construction company that worked on every building on Campo Road also worked on the apartment buildings here. That same company was shutdown due to activities related to money laundering. As low income as this neighborhood is, there are no signs of an obvious wealth inequality here. The residents here are like every other community of hard working parents trying to provide for their family.\r\n	\N	\N	1	0	\N	\N
353	481	1	0	{blu}La Mesa{/blu} Conrad Road	Nearly every building has the same Aztec style rooftops. The same construction company that worked on every building on Campo Road also worked on the apartment buildings here. That same company was shutdown due to activities related to money laundering. As low income as this neighborhood is, there are no signs of an obvious wealth inequality here. The residents here are like every other community of hard working parents trying to provide for their family.\r\n	\N	\N	1	0	\N	\N
354	482	1	0	{blu}La Mesa{/blu} Conrad Road	Nearly every building has the same Aztec style rooftops. The same construction company that worked on every building on Campo Road also worked on the apartment buildings here. That same company was shutdown due to activities related to money laundering. As low income as this neighborhood is, there are no signs of an obvious wealth inequality here. The residents here are like every other community of hard working parents trying to provide for their family.\r\n	\N	\N	1	0	\N	\N
355	483	1	0	{blu}La Mesa{/blu} Conrad Road	Nearly every building has the same Aztec style rooftops. The same construction company that worked on every building on Campo Road also worked on the apartment buildings here. That same company was shutdown due to activities related to money laundering. As low income as this neighborhood is, there are no signs of an obvious wealth inequality here. The residents here are like every other community of hard working parents trying to provide for their family.\r\n	\N	\N	1	0	\N	\N
356	484	1	0	{blu}La Mesa{/blu} Conrad Road	Nearly every building has the same Aztec style rooftops. The same construction company that worked on every building on Campo Road also worked on the apartment buildings here. That same company was shutdown due to activities related to money laundering. As low income as this neighborhood is, there are no signs of an obvious wealth inequality here. The residents here are like every other community of hard working parents trying to provide for their family.\r\n	\N	\N	1	0	\N	\N
357	485	1	0	{blu}La Mesa{/blu} Conrad Road	Nearly every building has the same Aztec style rooftops. The same construction company that worked on every building on Campo Road also worked on the apartment buildings here. That same company was shutdown due to activities related to money laundering. As low income as this neighborhood is, there are no signs of an obvious wealth inequality here. The residents here are like every other community of hard working parents trying to provide for their family.\r\n	\N	\N	1	0	\N	\N
358	486	1	0	{blu}La Mesa{/blu} Conrad Road	Nearly every building has the same Aztec style rooftops. The same construction company that worked on every building on Campo Road also worked on the apartment buildings here. That same company was shutdown due to activities related to money laundering. As low income as this neighborhood is, there are no signs of an obvious wealth inequality here. The residents here are like every other community of hard working parents trying to provide for their family.\r\n	\N	\N	1	0	\N	\N
359	487	1	0	{blu}La Mesa{/blu} Conrad Road	Nearly every building has the same Aztec style rooftops. The same construction company that worked on every building on Campo Road also worked on the apartment buildings here. That same company was shutdown due to activities related to money laundering. As low income as this neighborhood is, there are no signs of an obvious wealth inequality here. The residents here are like every other community of hard working parents trying to provide for their family.\r\n	\N	\N	1	0	\N	\N
360	488	1	0	{blu}La Mesa{/blu} Conrad Road	Nearly every building has the same Aztec style rooftops. The same construction company that worked on every building on Campo Road also worked on the apartment buildings here. That same company was shutdown due to activities related to money laundering. As low income as this neighborhood is, there are no signs of an obvious wealth inequality here. The residents here are like every other community of hard working parents trying to provide for their family.\r\n	\N	\N	1	0	\N	\N
361	489	1	0	{blu}La Mesa{/blu} Conrad Road	Nearly every building has the same Aztec style rooftops. The same construction company that worked on every building on Campo Road also worked on the apartment buildings here. That same company was shutdown due to activities related to money laundering. As low income as this neighborhood is, there are no signs of an obvious wealth inequality here. The residents here are like every other community of hard working parents trying to provide for their family.\r\n	\N	\N	1	0	\N	\N
362	490	1	0	{blu}La Mesa{/blu} Conrad Road	Nearly every building has the same Aztec style rooftops. The same construction company that worked on every building on Campo Road also worked on the apartment buildings here. That same company was shutdown due to activities related to money laundering. As low income as this neighborhood is, there are no signs of an obvious wealth inequality here. The residents here are like every other community of hard working parents trying to provide for their family.\r\n	\N	\N	1	0	\N	\N
426	554	1	0	{blu}Saint Vale Church{/blu} Inside - Isle	A dark colored wooden bench is here. There is a retractable padded kneeling device below the hanging bookshelf of psalms and prayers. \r\n	\N	\N	1	0	\N	\N
363	491	1	0	{blu}La Mesa{/blu} Conrad Road	Nearly every building has the same Aztec style rooftops. The same construction company that worked on every building on Campo Road also worked on the apartment buildings here. That same company was shutdown due to activities related to money laundering. As low income as this neighborhood is, there are no signs of an obvious wealth inequality here. The residents here are like every other community of hard working parents trying to provide for their family.\r\n	\N	\N	1	0	\N	\N
364	492	1	0	{blu}La Mesa{/blu} Conrad Road	Nearly every building has the same Aztec style rooftops. The same construction company that worked on every building on Campo Road also worked on the apartment buildings here. That same company was shutdown due to activities related to money laundering. As low income as this neighborhood is, there are no signs of an obvious wealth inequality here. The residents here are like every other community of hard working parents trying to provide for their family.\r\n	\N	\N	1	0	\N	\N
365	493	1	0	{blu}La Mesa{/blu} Conrad Road	Nearly every building has the same Aztec style rooftops. The same construction company that worked on every building on Campo Road also worked on the apartment buildings here. That same company was shutdown due to activities related to money laundering. As low income as this neighborhood is, there are no signs of an obvious wealth inequality here. The residents here are like every other community of hard working parents trying to provide for their family.\r\n	\N	\N	1	0	campo-back-alley-church-entrance	\N
366	494	1	0	{blu}La Mesa{/blu} Vale church back alley entrance	An extremely narrow alleyway leads to the Vale Church parking lot. A wall made of concrete and topped with a high iron fence is to the North. Directly to the south are cars jam packed next to each other in a feeble attempt to add more parking spaces to the apartments directly south of here. \r\n	\N	\N	1	0	\N	\N
367	495	1	0	{blu}La Mesa{/blu} Vale church back alley entrance	An extremely narrow alleyway leads to the Vale Church parking lot. A wall made of concrete and topped with a high iron fence is to the North. Directly to the south are cars jam packed next to each other in a feeble attempt to add more parking spaces to the apartments directly south of here. \r\n	\N	\N	1	0	\N	\N
368	496	1	0	{blu}La Mesa{/blu} Vale church back alley entrance	An extremely narrow alleyway leads to the Vale Church parking lot. A wall made of concrete and topped with a high iron fence is to the North. Directly to the south are cars jam packed next to each other in a feeble attempt to add more parking spaces to the apartments directly south of here. \r\n	\N	\N	1	0	saint-vale-nw-parking-lot	\N
369	497	1	0	{blu}Saint Vale Church{/blu} Parking Lot	A huge parking lot for a giant congregation. There are administration and community buildings to the north where people often hold weddings and other communal gatherings. To the south you spot a building where the staff go when services are over. It seems to be heavily fortified, which seems odd, given that this is a place of worship. The Church itself is to the East and it's main bell tower casts a looming shadow over the rest of the building and parking lot.\r\n	\N	\N	1	0	\N	\N
370	498	1	0	{blu}Saint Vale Church{/blu} Parking Lot	A huge parking lot for a giant congregation. There are administration and community buildings to the north where people often hold weddings and other communal gatherings. To the south you spot a building where the staff go when services are over. It seems to be heavily fortified, which seems odd, given that this is a place of worship. The Church itself is to the East and it's main bell tower casts a looming shadow over the rest of the building and parking lot.\r\n	\N	\N	1	0	\N	\N
371	499	1	0	{blu}Saint Vale Church{/blu} Parking Lot	A huge parking lot for a giant congregation. There are administration and community buildings to the north where people often hold weddings and other communal gatherings. To the south you spot a building where the staff go when services are over. It seems to be heavily fortified, which seems odd, given that this is a place of worship. The Church itself is to the East and it's main bell tower casts a looming shadow over the rest of the building and parking lot.\r\n	\N	\N	1	0	\N	\N
372	500	1	0	{blu}Saint Vale Church{/blu} Parking Lot	A huge parking lot for a giant congregation. There are administration and community buildings to the north where people often hold weddings and other communal gatherings. To the south you spot a building where the staff go when services are over. It seems to be heavily fortified, which seems odd, given that this is a place of worship. The Church itself is to the East and it's main bell tower casts a looming shadow over the rest of the building and parking lot.\r\n	\N	\N	1	0	\N	\N
373	501	1	0	{blu}Saint Vale Church{/blu} Parking Lot	A huge parking lot for a giant congregation. There are administration and community buildings to the north where people often hold weddings and other communal gatherings. To the south you spot a building where the staff go when services are over. It seems to be heavily fortified, which seems odd, given that this is a place of worship. The Church itself is to the East and it's main bell tower casts a looming shadow over the rest of the building and parking lot.\r\n	\N	\N	1	0	\N	\N
374	502	1	0	{blu}Saint Vale Church{/blu} Parking Lot	A huge parking lot for a giant congregation. There are administration and community buildings to the north where people often hold weddings and other communal gatherings. To the south you spot a building where the staff go when services are over. It seems to be heavily fortified, which seems odd, given that this is a place of worship. The Church itself is to the East and it's main bell tower casts a looming shadow over the rest of the building and parking lot.\r\n	\N	\N	1	0	\N	\N
375	503	1	0	{blu}Saint Vale Church{/blu} Parking Lot	A huge parking lot for a giant congregation. There are administration and community buildings to the north where people often hold weddings and other communal gatherings. To the south you spot a building where the staff go when services are over. It seems to be heavily fortified, which seems odd, given that this is a place of worship. The Church itself is to the East and it's main bell tower casts a looming shadow over the rest of the building and parking lot.\r\n	\N	\N	1	0	\N	\N
376	504	1	0	{blu}Saint Vale Church{/blu} Parking Lot	A huge parking lot for a giant congregation. There are administration and community buildings to the north where people often hold weddings and other communal gatherings. To the south you spot a building where the staff go when services are over. It seems to be heavily fortified, which seems odd, given that this is a place of worship. The Church itself is to the East and it's main bell tower casts a looming shadow over the rest of the building and parking lot.\r\n	\N	\N	1	0	\N	\N
427	555	1	0	{blu}Saint Vale Church{/blu} Inside - Isle	A dark colored wooden bench is here. There is a retractable padded kneeling device below the hanging bookshelf of psalms and prayers. \r\n	\N	\N	1	0	\N	\N
428	556	1	0	{blu}Saint Vale Church{/blu} Inside - Isle	A dark colored wooden bench is here. There is a retractable padded kneeling device below the hanging bookshelf of psalms and prayers. \r\n	\N	\N	1	0	\N	\N
377	505	1	0	{blu}Saint Vale Church{/blu} Parking Lot	A huge parking lot for a giant congregation. There are administration and community buildings to the north where people often hold weddings and other communal gatherings. To the south you spot a building where the staff go when services are over. It seems to be heavily fortified, which seems odd, given that this is a place of worship. The Church itself is to the East and it's main bell tower casts a looming shadow over the rest of the building and parking lot.\r\n	\N	\N	1	0	\N	\N
378	506	1	0	{blu}Saint Vale Church{/blu} Parking Lot	A huge parking lot for a giant congregation. There are administration and community buildings to the north where people often hold weddings and other communal gatherings. To the south you spot a building where the staff go when services are over. It seems to be heavily fortified, which seems odd, given that this is a place of worship. The Church itself is to the East and it's main bell tower casts a looming shadow over the rest of the building and parking lot.\r\n	\N	\N	1	0	\N	\N
379	507	1	0	{blu}Saint Vale Church{/blu} Parking Lot	A huge parking lot for a giant congregation. There are administration and community buildings to the north where people often hold weddings and other communal gatherings. To the south you spot a building where the staff go when services are over. It seems to be heavily fortified, which seems odd, given that this is a place of worship. The Church itself is to the East and it's main bell tower casts a looming shadow over the rest of the building and parking lot.\r\n	\N	\N	1	0	\N	\N
444	572	1	0	{blu}Saint Vale Church{/blu} Inside - Altar	A table is here with a decorative cloth draped over it. On each side of the table are tall stands that hold long white candles. There is a stand with a microphone embedded into the wood.\r\n	\N	\N	1	0	\N	\N
597	726	1	0	{blu}Hartford Bank{/blu} - Tellers	To the east and west are bank tellers at their respective kiosks. The marble floor has a prestine aura to it. The distant promise of coffee hangs in the air.\r\n	\N	\N	1	0	\N	\N
380	508	1	0	{blu}Saint Vale Church{/blu} Parking Lot	A huge parking lot for a giant congregation. There are administration and community buildings to the north where people often hold weddings and other communal gatherings. To the south you spot a building where the staff go when services are over. It seems to be heavily fortified, which seems odd, given that this is a place of worship. The Church itself is to the East and it's main bell tower casts a looming shadow over the rest of the building and parking lot.\r\n	\N	\N	1	0	saint-vale-west-entrance	\N
381	509	1	0	{blu}Saint Vale Church{/blu} Parking Lot - Row A	A huge parking lot for a giant congregation. There are administration and community buildings to the north where people often hold weddings and other communal gatherings. To the south you spot a building where the staff go when services are over. It seems to be heavily fortified, which seems odd, given that this is a place of worship. The Church itself is to the East and it's main bell tower casts a looming shadow over the rest of the building and parking lot.\r\n	\N	\N	1	0	\N	\N
382	510	1	0	{blu}Saint Vale Church{/blu} Parking Lot - Row A	A huge parking lot for a giant congregation. There are administration and community buildings to the north where people often hold weddings and other communal gatherings. To the south you spot a building where the staff go when services are over. It seems to be heavily fortified, which seems odd, given that this is a place of worship. The Church itself is to the East and it's main bell tower casts a looming shadow over the rest of the building and parking lot.\r\n	\N	\N	1	0	\N	\N
383	511	1	0	{blu}Saint Vale Church{/blu} Parking Lot - Row A	A huge parking lot for a giant congregation. There are administration and community buildings to the north where people often hold weddings and other communal gatherings. To the south you spot a building where the staff go when services are over. It seems to be heavily fortified, which seems odd, given that this is a place of worship. The Church itself is to the East and it's main bell tower casts a looming shadow over the rest of the building and parking lot.\r\n	\N	\N	1	0	\N	\N
384	512	1	0	{blu}Saint Vale Church{/blu} Parking Lot - Row A	A huge parking lot for a giant congregation. There are administration and community buildings to the north where people often hold weddings and other communal gatherings. To the south you spot a building where the staff go when services are over. It seems to be heavily fortified, which seems odd, given that this is a place of worship. The Church itself is to the East and it's main bell tower casts a looming shadow over the rest of the building and parking lot.\r\n	\N	\N	1	0	\N	\N
385	513	1	0	{blu}Saint Vale Church{/blu} Parking Lot - Row B	A huge parking lot for a giant congregation. There are administration and community buildings to the north where people often hold weddings and other communal gatherings. To the south you spot a building where the staff go when services are over. It seems to be heavily fortified, which seems odd, given that this is a place of worship. The Church itself is to the East and it's main bell tower casts a looming shadow over the rest of the building and parking lot.\r\n	\N	\N	1	0	\N	\N
386	514	1	0	{blu}Saint Vale Church{/blu} Parking Lot - Row B	A huge parking lot for a giant congregation. There are administration and community buildings to the north where people often hold weddings and other communal gatherings. To the south you spot a building where the staff go when services are over. It seems to be heavily fortified, which seems odd, given that this is a place of worship. The Church itself is to the East and it's main bell tower casts a looming shadow over the rest of the building and parking lot.\r\n	\N	\N	1	0	\N	\N
387	515	1	0	{blu}Saint Vale Church{/blu} Parking Lot - Row B	A huge parking lot for a giant congregation. There are administration and community buildings to the north where people often hold weddings and other communal gatherings. To the south you spot a building where the staff go when services are over. It seems to be heavily fortified, which seems odd, given that this is a place of worship. The Church itself is to the East and it's main bell tower casts a looming shadow over the rest of the building and parking lot.\r\n	\N	\N	1	0	\N	\N
388	516	1	0	{blu}Saint Vale Church{/blu} Parking Lot - Row B	A huge parking lot for a giant congregation. There are administration and community buildings to the north where people often hold weddings and other communal gatherings. To the south you spot a building where the staff go when services are over. It seems to be heavily fortified, which seems odd, given that this is a place of worship. The Church itself is to the East and it's main bell tower casts a looming shadow over the rest of the building and parking lot.\r\n	\N	\N	1	0	\N	\N
389	517	1	0	{blu}Saint Vale Church{/blu} Parking Lot - Row B	A huge parking lot for a giant congregation. There are administration and community buildings to the north where people often hold weddings and other communal gatherings. To the south you spot a building where the staff go when services are over. It seems to be heavily fortified, which seems odd, given that this is a place of worship. The Church itself is to the East and it's main bell tower casts a looming shadow over the rest of the building and parking lot.\r\n	\N	\N	1	0	\N	\N
429	557	1	0	{blu}Saint Vale Church{/blu} Inside - Isle	A dark colored wooden bench is here. There is a retractable padded kneeling device below the hanging bookshelf of psalms and prayers. \r\n	\N	\N	1	0	\N	\N
430	558	1	0	{blu}Saint Vale Church{/blu} Inside - Isle	A dark colored wooden bench is here. There is a retractable padded kneeling device below the hanging bookshelf of psalms and prayers. \r\n	\N	\N	1	0	\N	\N
390	518	1	0	{blu}Saint Vale Church{/blu} Parking Lot - Row B	A huge parking lot for a giant congregation. There are administration and community buildings to the north where people often hold weddings and other communal gatherings. To the south you spot a building where the staff go when services are over. It seems to be heavily fortified, which seems odd, given that this is a place of worship. The Church itself is to the East and it's main bell tower casts a looming shadow over the rest of the building and parking lot.\r\n	\N	\N	1	0	\N	\N
391	519	1	0	{blu}Saint Vale Church{/blu} Parking Lot - Row B	A huge parking lot for a giant congregation. There are administration and community buildings to the north where people often hold weddings and other communal gatherings. To the south you spot a building where the staff go when services are over. It seems to be heavily fortified, which seems odd, given that this is a place of worship. The Church itself is to the East and it's main bell tower casts a looming shadow over the rest of the building and parking lot.\r\n	\N	\N	1	0	\N	\N
392	520	1	0	{blu}Saint Vale Church{/blu} Parking Lot - Row B	A huge parking lot for a giant congregation. There are administration and community buildings to the north where people often hold weddings and other communal gatherings. To the south you spot a building where the staff go when services are over. It seems to be heavily fortified, which seems odd, given that this is a place of worship. The Church itself is to the East and it's main bell tower casts a looming shadow over the rest of the building and parking lot.\r\n	\N	\N	1	0	\N	\N
393	521	1	0	{blu}Saint Vale Church{/blu} Parking Lot - Row C	A huge parking lot for a giant congregation. There are administration and community buildings to the north where people often hold weddings and other communal gatherings. To the south you spot a building where the staff go when services are over. It seems to be heavily fortified, which seems odd, given that this is a place of worship. The Church itself is to the East and it's main bell tower casts a looming shadow over the rest of the building and parking lot.\r\n	\N	\N	1	0	\N	\N
394	522	1	0	{blu}Saint Vale Church{/blu} Parking Lot - Row C	A huge parking lot for a giant congregation. There are administration and community buildings to the north where people often hold weddings and other communal gatherings. To the south you spot a building where the staff go when services are over. It seems to be heavily fortified, which seems odd, given that this is a place of worship. The Church itself is to the East and it's main bell tower casts a looming shadow over the rest of the building and parking lot.\r\n	\N	\N	1	0	\N	\N
395	523	1	0	{blu}Saint Vale Church{/blu} Parking Lot - Row C	A huge parking lot for a giant congregation. There are administration and community buildings to the north where people often hold weddings and other communal gatherings. To the south you spot a building where the staff go when services are over. It seems to be heavily fortified, which seems odd, given that this is a place of worship. The Church itself is to the East and it's main bell tower casts a looming shadow over the rest of the building and parking lot.\r\n	\N	\N	1	0	\N	\N
396	524	1	0	{blu}Saint Vale Church{/blu} Parking Lot - Row C	A huge parking lot for a giant congregation. There are administration and community buildings to the north where people often hold weddings and other communal gatherings. To the south you spot a building where the staff go when services are over. It seems to be heavily fortified, which seems odd, given that this is a place of worship. The Church itself is to the East and it's main bell tower casts a looming shadow over the rest of the building and parking lot.\r\n	\N	\N	1	0	\N	\N
442	570	1	0	{blu}Saint Vale Church{/blu} Inside - Altar	A table is here with a decorative cloth draped over it. On each side of the table are tall stands that hold long white candles. There is a stand with a microphone embedded into the wood.\r\n	\N	\N	1	0	\N	\N
397	525	1	0	{blu}Saint Vale Church{/blu} Inside	Sanctuary. A well ventilated and air conditioned cathedral. The west entrance of Saint Vale Church lets a little bit of outside light leak inside as the door closes automatically behind you. The smell of incense and blown out candles is hard to ignore, yet it's a very pleasant and calming feeling. Each of the fifty rows of seating is 100 feet across. Whoever built this Church had significant amounts of money, possibly even ludicrous amounts. \r\n	\N	\N	1	0	\N	\N
398	526	1	0	{blu}Saint Vale Church{/blu} Inside	Sanctuary. A well ventilated and air conditioned cathedral. The west entrance of Saint Vale Church lets a little bit of outside light leak inside as the door closes automatically behind you. The smell of incense and blown out candles is hard to ignore, yet it's a very pleasant and calming feeling. Each of the fifty rows of seating is 100 feet across. Whoever built this Church had significant amounts of money, possibly even ludicrous amounts. \r\n	\N	\N	1	0	\N	\N
399	527	1	0	{blu}Saint Vale Church{/blu} Inside	Sanctuary. A well ventilated and air conditioned cathedral. The west entrance of Saint Vale Church lets a little bit of outside light leak inside as the door closes automatically behind you. The smell of incense and blown out candles is hard to ignore, yet it's a very pleasant and calming feeling. Each of the fifty rows of seating is 100 feet across. Whoever built this Church had significant amounts of money, possibly even ludicrous amounts. \r\n	\N	\N	1	0	\N	\N
400	528	1	0	{blu}Saint Vale Church{/blu} Inside	Sanctuary. A well ventilated and air conditioned cathedral. The west entrance of Saint Vale Church lets a little bit of outside light leak inside as the door closes automatically behind you. The smell of incense and blown out candles is hard to ignore, yet it's a very pleasant and calming feeling. Each of the fifty rows of seating is 100 feet across. Whoever built this Church had significant amounts of money, possibly even ludicrous amounts. \r\n	\N	\N	1	0	\N	\N
401	529	1	0	{blu}Saint Vale Church{/blu} Inside	Sanctuary. A well ventilated and air conditioned cathedral. The west entrance of Saint Vale Church lets a little bit of outside light leak inside as the door closes automatically behind you. The smell of incense and blown out candles is hard to ignore, yet it's a very pleasant and calming feeling. Each of the fifty rows of seating is 100 feet across. Whoever built this Church had significant amounts of money, possibly even ludicrous amounts. \r\n	\N	\N	1	0	\N	\N
402	530	1	0	{blu}Saint Vale Church{/blu} Inside	Sanctuary. A well ventilated and air conditioned cathedral. The west entrance of Saint Vale Church lets a little bit of outside light leak inside as the door closes automatically behind you. The smell of incense and blown out candles is hard to ignore, yet it's a very pleasant and calming feeling. Each of the fifty rows of seating is 100 feet across. Whoever built this Church had significant amounts of money, possibly even ludicrous amounts. \r\n	\N	\N	1	0	\N	\N
431	559	1	0	{blu}Saint Vale Church{/blu} Inside - Isle	A dark colored wooden bench is here. There is a retractable padded kneeling device below the hanging bookshelf of psalms and prayers. \r\n	\N	\N	1	0	\N	\N
403	531	1	0	{blu}Saint Vale Church{/blu} Inside - Rear Hall	The wall to the south is made of a repurposed sort of brick layering that makes it almost look as if it was a design decision that came long after the architecture of the church was already decided. Something seems very off about this wall and you almost feel like you are being watched here. A door leading to the bathroom hallway is to the south of here. You can take a seat in one of the many rows of seating to the north of here.\r\n	\N	\N	1	0	\N	\N
404	532	1	0	{blu}Saint Vale Church{/blu} Inside - Rear Hall	The wall to the south is made of a repurposed sort of brick layering that makes it almost look as if it was a design decision that came long after the architecture of the church was already decided. Something seems very off about this wall and you almost feel like you are being watched here. A door leading to the bathroom hallway is to the south of here. You can take a seat in one of the many rows of seating to the north of here.\r\n	\N	\N	1	0	\N	\N
405	533	1	0	{blu}Saint Vale Church{/blu} Inside - Rear Hall	The wall to the south is made of a repurposed sort of brick layering that makes it almost look as if it was a design decision that came long after the architecture of the church was already decided. Something seems very off about this wall and you almost feel like you are being watched here. A door leading to the bathroom hallway is to the south of here. You can take a seat in one of the many rows of seating to the north of here.\r\n	\N	\N	1	0	\N	\N
445	573	1	0	{blu}Saint Vale Church{/blu} Inside - Altar	A table is here with a decorative cloth draped over it. On each side of the table are tall stands that hold long white candles. There is a stand with a microphone embedded into the wood.\r\n	\N	\N	1	0	\N	\N
406	534	1	0	{blu}Saint Vale Church{/blu} Inside - Rear Hall	The wall to the south is made of a repurposed sort of brick layering that makes it almost look as if it was a design decision that came long after the architecture of the church was already decided. Something seems very off about this wall and you almost feel like you are being watched here. A door leading to the bathroom hallway is to the south of here. You can take a seat in one of the many rows of seating to the north of here.\r\n	\N	\N	1	0	saint-vale-rear-east	\N
407	535	1	0	{blu}Saint Vale Church{/blu} Inside - Bathroom Hall	Oddly enough, this hallway is not air conditioned. The male bathroom is just to the west and the female bathroom is to the east.\r\n	\N	\N	1	0	\N	\N
408	536	1	0	{blu}Saint Vale Church{/blu} Inside - Bathroom Hall	Oddly enough, this hallway is not air conditioned. The male bathroom is just to the west and the female bathroom is to the east.\r\n	\N	\N	1	0	\N	\N
409	537	1	0	{blu}Saint Vale Church{/blu} Inside - Male Bathroom	The walls are painted a soothing orange and white color and the ceiling fan generates a constant white noise hum that lulls you into a meditative state.\r\n	\N	\N	1	0	\N	\N
410	538	1	0	{blu}Saint Vale Church{/blu} Inside - Male Bathroom	The walls are painted a soothing orange and white color and the ceiling fan generates a constant white noise hum that lulls you into a meditative state.\r\n	\N	\N	1	0	\N	\N
411	539	1	0	{blu}Saint Vale Church{/blu} Inside - Female Bathroom	The walls are painted a soothing orange and white color and the ceiling fan generates a constant white noise hum that lulls you into a meditative state.\r\n	\N	\N	1	0	\N	\N
412	540	1	0	{blu}Saint Vale Church{/blu} Inside - Female Bathroom	The walls are painted a soothing orange and white color and the ceiling fan generates a constant white noise hum that lulls you into a meditative state.\r\n	\N	\N	1	0	\N	\N
413	541	1	0	{blu}Saint Vale Church{/blu} Inside - Rear Hall	The wall to the south is made of a repurposed sort of brick layering that makes it almost look as if it was a design decision that came long after the architecture of the church was already decided. Something seems very off about this wall and you almost feel like you are being watched here. A door leading to the bathroom hallway is to the south of here. You can take a seat in one of the many rows of seating to the north of here.\r\n	\N	\N	1	0	\N	\N
443	571	1	0	{blu}Saint Vale Church{/blu} Inside - Altar	A table is here with a decorative cloth draped over it. On each side of the table are tall stands that hold long white candles. There is a stand with a microphone embedded into the wood.\r\n	\N	\N	1	0	\N	\N
414	542	1	0	{blu}Saint Vale Church{/blu} Inside - Rear Hall	The wall to the south is made of a repurposed sort of brick layering that makes it almost look as if it was a design decision that came long after the architecture of the church was already decided. Something seems very off about this wall and you almost feel like you are being watched here. A door leading to the bathroom hallway is to the south of here. You can take a seat in one of the many rows of seating to the north of here.\r\n	\N	\N	1	0	\N	\N
415	543	1	0	{blu}Saint Vale Church{/blu} Inside - Rear Hall	The wall to the south is made of a repurposed sort of brick layering that makes it almost look as if it was a design decision that came long after the architecture of the church was already decided. Something seems very off about this wall and you almost feel like you are being watched here. A door leading to the bathroom hallway is to the south of here. You can take a seat in one of the many rows of seating to the north of here.\r\n	\N	\N	1	0	\N	\N
416	544	1	0	{blu}Saint Vale Church{/blu} Inside - Rear Hall	The wall to the south is made of a repurposed sort of brick layering that makes it almost look as if it was a design decision that came long after the architecture of the church was already decided. Something seems very off about this wall and you almost feel like you are being watched here. A door leading to the bathroom hallway is to the south of here. You can take a seat in one of the many rows of seating to the north of here.\r\n	\N	\N	1	0	\N	\N
417	545	1	0	{blu}Saint Vale Church{/blu} Inside - Center Isle	An elaborate purple and yellow pattern decorates the isle which leads to the altar. The corners of each isle have divine sigils engraved into the dark stain of the wood.\r\n	\N	\N	1	0	\N	\N
418	546	1	0	{blu}Saint Vale Church{/blu} Inside - Center Isle	An elaborate purple and yellow pattern decorates the isle which leads to the altar. The corners of each isle have divine sigils engraved into the dark stain of the wood.\r\n	\N	\N	1	0	\N	\N
419	547	1	0	{blu}Saint Vale Church{/blu} Inside - Center Isle	An elaborate purple and yellow pattern decorates the isle which leads to the altar. The corners of each isle have divine sigils engraved into the dark stain of the wood.\r\n	\N	\N	1	0	\N	\N
420	548	1	0	{blu}Saint Vale Church{/blu} Inside - Center Isle	An elaborate purple and yellow pattern decorates the isle which leads to the altar. The corners of each isle have divine sigils engraved into the dark stain of the wood.\r\n	\N	\N	1	0	\N	\N
421	549	1	0	{blu}Saint Vale Church{/blu} Inside - Center Isle	An elaborate purple and yellow pattern decorates the isle which leads to the altar. The corners of each isle have divine sigils engraved into the dark stain of the wood.\r\n	\N	\N	1	0	\N	\N
435	563	1	0	{blu}Saint Vale Church{/blu} Inside - Isle	A dark colored wooden bench is here. There is a retractable padded kneeling device below the hanging bookshelf of psalms and prayers. \r\n	\N	\N	1	0	\N	\N
436	564	1	0	{blu}Saint Vale Church{/blu} Inside - Isle	A dark colored wooden bench is here. There is a retractable padded kneeling device below the hanging bookshelf of psalms and prayers. \r\n	\N	\N	1	0	\N	\N
437	565	1	0	{blu}Saint Vale Church{/blu} Inside - Isle	A dark colored wooden bench is here. There is a retractable padded kneeling device below the hanging bookshelf of psalms and prayers. \r\n	\N	\N	1	0	\N	\N
438	566	1	0	{blu}Saint Vale Church{/blu} Inside - Altar	A table is here with a decorative cloth draped over it. On each side of the table are tall stands that hold long white candles. There is a stand with a microphone embedded into the wood.\r\n	\N	\N	1	0	\N	\N
439	567	1	0	{blu}Saint Vale Church{/blu} Inside - Altar	A table is here with a decorative cloth draped over it. On each side of the table are tall stands that hold long white candles. There is a stand with a microphone embedded into the wood.\r\n	\N	\N	1	0	\N	\N
440	568	1	0	{blu}Saint Vale Church{/blu} Inside - Altar	A table is here with a decorative cloth draped over it. On each side of the table are tall stands that hold long white candles. There is a stand with a microphone embedded into the wood.\r\n	\N	\N	1	0	\N	\N
446	574	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Entrance	A large green and white sign looms above your head. {grn}Allied Foods{/grn} is the main food source for the local community. It's central location is within walking distance for most of the residents.\r\n	\N	\N	1	0	\N	\N
447	575	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Carts	Four rows of black shopping carts are off to the left. The automatic door in front of you gives you a nice gust of cool air-conditioned air.\r\n	\N	\N	1	0	\N	\N
448	576	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Pharmacy	A Pharmacy\r\n	\N	\N	1	0	\N	\N
449	577	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Pharmacy	A Pharmacy\r\n	\N	\N	1	0	\N	\N
450	578	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Pharmacy	A Pharmacy\r\n	\N	\N	1	0	allied-foods-pharmacy-bend	\N
451	579	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Pharmacy	A Pharmacy\r\n	\N	\N	1	0	\N	\N
452	580	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Pharmacy	A Pharmacy\r\n	\N	\N	1	0	\N	\N
453	581	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Pharmacy	A Pharmacy\r\n	\N	\N	1	0	\N	\N
454	582	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Shower Isle	A shower isle\r\n	\N	\N	1	0	\N	\N
455	583	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Shower Isle	A shower isle\r\n	\N	\N	1	0	\N	\N
456	584	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Shower Isle	A shower isle\r\n	\N	\N	1	0	\N	\N
457	585	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Shower Isle	A shower isle\r\n	\N	\N	1	0	\N	\N
458	586	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Shower Isle	A shower isle\r\n	\N	\N	1	0	\N	\N
459	587	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Shower Isle	A shower isle\r\n	\N	\N	1	0	\N	\N
460	588	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Shower Isle	A shower isle\r\n	\N	\N	1	0	\N	\N
461	589	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Shower Isle	A shower isle\r\n	\N	\N	1	0	allied-foods-shower-end	\N
462	590	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Dairy	The isle is dimly lit but that doesn't affect your vision of the contents of this isle. Every product here is in a white container. Milks, butters, cream cheese.. all sorts of kinds of almond milks.. At the end of the isle you notice an end cap displaying a high grade vokda. How appropriate.\r\n	\N	\N	1	0	\N	\N
463	591	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Dairy	The isle is dimly lit but that doesn't affect your vision of the contents of this isle. Every product here is in a white container. Milks, butters, cream cheese.. all sorts of kinds of almond milks.. At the end of the isle you notice an end cap displaying a high grade vokda. How appropriate.\r\n	\N	\N	1	0	\N	\N
464	592	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Dairy	The isle is dimly lit but that doesn't affect your vision of the contents of this isle. Every product here is in a white container. Milks, butters, cream cheese.. all sorts of kinds of almond milks.. At the end of the isle you notice an end cap displaying a high grade vokda. How appropriate.\r\n	\N	\N	1	0	allied-foods-dairy-end	\N
465	593	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Alcohol Isle	Both sides of the isle contain huge amounts of name brand alcoholic beverages. You notice that there aren't any beers. The only types of alcohol are high strength variants like vodka, rum, and whiskey. An {red}explosion{/red} in this isle would be {grn}fatal{/grn}.\r\n	\N	\N	1	0	\N	\N
466	594	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Alcohol Isle	Both sides of the isle contain huge amounts of name brand alcoholic beverages. You notice that there aren't any beers. The only types of alcohol are high strength variants like vodka, rum, and whiskey. An {red}explosion{/red} in this isle would be {grn}fatal{/grn}.\r\n	\N	\N	1	0	\N	\N
467	595	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Alcohol Isle	Both sides of the isle contain huge amounts of name brand alcoholic beverages. You notice that there aren't any beers. The only types of alcohol are high strength variants like vodka, rum, and whiskey. An {red}explosion{/red} in this isle would be {grn}fatal{/grn}.\r\n	\N	\N	1	0	\N	\N
468	596	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Alcohol Isle	Both sides of the isle contain huge amounts of name brand alcoholic beverages. You notice that there aren't any beers. The only types of alcohol are high strength variants like vodka, rum, and whiskey. An {red}explosion{/red} in this isle would be {grn}fatal{/grn}.\r\n	\N	\N	1	0	\N	\N
469	597	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Alcohol Isle	Both sides of the isle contain huge amounts of name brand alcoholic beverages. You notice that there aren't any beers. The only types of alcohol are high strength variants like vodka, rum, and whiskey. An {red}explosion{/red} in this isle would be {grn}fatal{/grn}.\r\n	\N	\N	1	0	\N	\N
470	598	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Alcohol Isle	Both sides of the isle contain huge amounts of name brand alcoholic beverages. You notice that there aren't any beers. The only types of alcohol are high strength variants like vodka, rum, and whiskey. An {red}explosion{/red} in this isle would be {grn}fatal{/grn}.\r\n	\N	\N	1	0	\N	\N
758	890	1	0	{blu}Psi-Tech H.Q.{/blu} - Outside - VIP Drop Off	This area contains many high end vehicles fit for the high value targets that use them.This place is heavily guarded and monitored at all times.\r\n	\N	\N	1	0	psi-tech-car-2	,DRY,OUTSIDE,STREET,PARKING_LOT,PARKING_STALL
471	599	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Alcohol Isle	Both sides of the isle contain huge amounts of name brand alcoholic beverages. You notice that there aren't any beers. The only types of alcohol are high strength variants like vodka, rum, and whiskey. An {red}explosion{/red} in this isle would be {grn}fatal{/grn}.\r\n	\N	\N	1	0	\N	\N
472	600	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Alcohol Isle	Both sides of the isle contain huge amounts of name brand alcoholic beverages. You notice that there aren't any beers. The only types of alcohol are high strength variants like vodka, rum, and whiskey. An {red}explosion{/red} in this isle would be {grn}fatal{/grn}.\r\n	\N	\N	1	0	allied-foods-alcohol-end	\N
473	601	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Dairy	The isle is dimly lit but that doesn't affect your vision of the contents of this isle. Every product here is in a white container. Milks, butters, cream cheese.. all sorts of kinds of almond milks.. At the end of the isle you notice an end cap displaying a high grade vokda. How appropriate.\r\n	\N	\N	1	0	\N	\N
474	602	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Dairy	The isle is dimly lit but that doesn't affect your vision of the contents of this isle. Every product here is in a white container. Milks, butters, cream cheese.. all sorts of kinds of almond milks.. At the end of the isle you notice an end cap displaying a high grade vokda. How appropriate.\r\n	\N	\N	1	0	\N	\N
475	603	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Dairy	The isle is dimly lit but that doesn't affect your vision of the contents of this isle. Every product here is in a white container. Milks, butters, cream cheese.. all sorts of kinds of almond milks.. At the end of the isle you notice an end cap displaying a high grade vokda. How appropriate.\r\n	\N	\N	1	0	allied-foods-dairy-end	\N
476	604	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Alcohol Isle 2	Both sides of the isle contain huge amounts of name brand alcoholic beverages. You notice that there aren't any beers. The only types of alcohol are high strength variants like vodka, rum, and whiskey. An {red}explosion{/red} in this isle would be {grn}fatal{/grn}.\r\n	\N	\N	1	0	\N	\N
477	605	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Alcohol Isle 2	Both sides of the isle contain huge amounts of name brand alcoholic beverages. You notice that there aren't any beers. The only types of alcohol are high strength variants like vodka, rum, and whiskey. An {red}explosion{/red} in this isle would be {grn}fatal{/grn}.\r\n	\N	\N	1	0	\N	\N
478	606	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Alcohol Isle 2	Both sides of the isle contain huge amounts of name brand alcoholic beverages. You notice that there aren't any beers. The only types of alcohol are high strength variants like vodka, rum, and whiskey. An {red}explosion{/red} in this isle would be {grn}fatal{/grn}.\r\n	\N	\N	1	0	\N	\N
479	607	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Alcohol Isle 2	Both sides of the isle contain huge amounts of name brand alcoholic beverages. You notice that there aren't any beers. The only types of alcohol are high strength variants like vodka, rum, and whiskey. An {red}explosion{/red} in this isle would be {grn}fatal{/grn}.\r\n	\N	\N	1	0	\N	\N
480	608	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Alcohol Isle 2	Both sides of the isle contain huge amounts of name brand alcoholic beverages. You notice that there aren't any beers. The only types of alcohol are high strength variants like vodka, rum, and whiskey. An {red}explosion{/red} in this isle would be {grn}fatal{/grn}.\r\n	\N	\N	1	0	\N	\N
481	609	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Alcohol Isle 2	Both sides of the isle contain huge amounts of name brand alcoholic beverages. You notice that there aren't any beers. The only types of alcohol are high strength variants like vodka, rum, and whiskey. An {red}explosion{/red} in this isle would be {grn}fatal{/grn}.\r\n	\N	\N	1	0	\N	\N
482	610	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Alcohol Isle 2	Both sides of the isle contain huge amounts of name brand alcoholic beverages. You notice that there aren't any beers. The only types of alcohol are high strength variants like vodka, rum, and whiskey. An {red}explosion{/red} in this isle would be {grn}fatal{/grn}.\r\n	\N	\N	1	0	\N	\N
483	611	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Alcohol Isle 2	Both sides of the isle contain huge amounts of name brand alcoholic beverages. You notice that there aren't any beers. The only types of alcohol are high strength variants like vodka, rum, and whiskey. An {red}explosion{/red} in this isle would be {grn}fatal{/grn}.\r\n	\N	\N	1	0	allied-foods-alcohol-end2	\N
484	612	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Dairy	The isle is dimly lit but that doesn't affect your vision of the contents of this isle. Every product here is in a white container. Milks, butters, cream cheese.. all sorts of kinds of almond milks.. At the end of the isle you notice an end cap displaying a high grade vokda. How appropriate.\r\n	\N	\N	1	0	\N	\N
485	613	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Dairy	The isle is dimly lit but that doesn't affect your vision of the contents of this isle. Every product here is in a white container. Milks, butters, cream cheese.. all sorts of kinds of almond milks.. At the end of the isle you notice an end cap displaying a high grade vokda. How appropriate.\r\n	\N	\N	1	0	\N	\N
486	614	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Dairy	The isle is dimly lit but that doesn't affect your vision of the contents of this isle. Every product here is in a white container. Milks, butters, cream cheese.. all sorts of kinds of almond milks.. At the end of the isle you notice an end cap displaying a high grade vokda. How appropriate.\r\n	\N	\N	1	0	allied-foods-dairy-end2	\N
487	615	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Alcohol Isle 3	Both sides of the isle contain huge amounts of name brand alcoholic beverages. You notice that there aren't any beers. The only types of alcohol are high strength variants like vodka, rum, and whiskey. An {red}explosion{/red} in this isle would be {grn}fatal{/grn}.\r\n	\N	\N	1	0	\N	\N
488	616	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Alcohol Isle 3	Both sides of the isle contain huge amounts of name brand alcoholic beverages. You notice that there aren't any beers. The only types of alcohol are high strength variants like vodka, rum, and whiskey. An {red}explosion{/red} in this isle would be {grn}fatal{/grn}.\r\n	\N	\N	1	0	\N	\N
489	617	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Alcohol Isle 3	Both sides of the isle contain huge amounts of name brand alcoholic beverages. You notice that there aren't any beers. The only types of alcohol are high strength variants like vodka, rum, and whiskey. An {red}explosion{/red} in this isle would be {grn}fatal{/grn}.\r\n	\N	\N	1	0	\N	\N
490	618	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Alcohol Isle 3	Both sides of the isle contain huge amounts of name brand alcoholic beverages. You notice that there aren't any beers. The only types of alcohol are high strength variants like vodka, rum, and whiskey. An {red}explosion{/red} in this isle would be {grn}fatal{/grn}.\r\n	\N	\N	1	0	\N	\N
491	619	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Alcohol Isle 3	Both sides of the isle contain huge amounts of name brand alcoholic beverages. You notice that there aren't any beers. The only types of alcohol are high strength variants like vodka, rum, and whiskey. An {red}explosion{/red} in this isle would be {grn}fatal{/grn}.\r\n	\N	\N	1	0	\N	\N
492	620	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Alcohol Isle 3	Both sides of the isle contain huge amounts of name brand alcoholic beverages. You notice that there aren't any beers. The only types of alcohol are high strength variants like vodka, rum, and whiskey. An {red}explosion{/red} in this isle would be {grn}fatal{/grn}.\r\n	\N	\N	1	0	\N	\N
493	621	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Alcohol Isle 3	Both sides of the isle contain huge amounts of name brand alcoholic beverages. You notice that there aren't any beers. The only types of alcohol are high strength variants like vodka, rum, and whiskey. An {red}explosion{/red} in this isle would be {grn}fatal{/grn}.\r\n	\N	\N	1	0	\N	\N
494	622	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Alcohol Isle 3	Both sides of the isle contain huge amounts of name brand alcoholic beverages. You notice that there aren't any beers. The only types of alcohol are high strength variants like vodka, rum, and whiskey. An {red}explosion{/red} in this isle would be {grn}fatal{/grn}.\r\n	\N	\N	1	0	allied-foods-alcohol-end3	\N
495	623	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Dairy	The isle is dimly lit but that doesn't affect your vision of the contents of this isle. Every product here is in a white container. Milks, butters, cream cheese.. all sorts of kinds of almond milks.. At the end of the isle you notice an end cap displaying a high grade vokda. How appropriate.\r\n	\N	\N	1	0	\N	\N
496	624	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Dairy	The isle is dimly lit but that doesn't affect your vision of the contents of this isle. Every product here is in a white container. Milks, butters, cream cheese.. all sorts of kinds of almond milks.. At the end of the isle you notice an end cap displaying a high grade vokda. How appropriate.\r\n	\N	\N	1	0	\N	\N
497	625	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Dairy	The isle is dimly lit but that doesn't affect your vision of the contents of this isle. Every product here is in a white container. Milks, butters, cream cheese.. all sorts of kinds of almond milks.. At the end of the isle you notice an end cap displaying a high grade vokda. How appropriate.\r\n	\N	\N	1	0	allied-foods-dairy-end3	\N
498	626	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Alcohol Isle 4	Both sides of the isle contain huge amounts of name brand alcoholic beverages. You notice that there aren't any beers. The only types of alcohol are high strength variants like vodka, rum, and whiskey. An {red}explosion{/red} in this isle would be {grn}fatal{/grn}.\r\n	\N	\N	1	0	\N	\N
499	627	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Alcohol Isle 4	Both sides of the isle contain huge amounts of name brand alcoholic beverages. You notice that there aren't any beers. The only types of alcohol are high strength variants like vodka, rum, and whiskey. An {red}explosion{/red} in this isle would be {grn}fatal{/grn}.\r\n	\N	\N	1	0	\N	\N
500	628	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Alcohol Isle 4	Both sides of the isle contain huge amounts of name brand alcoholic beverages. You notice that there aren't any beers. The only types of alcohol are high strength variants like vodka, rum, and whiskey. An {red}explosion{/red} in this isle would be {grn}fatal{/grn}.\r\n	\N	\N	1	0	\N	\N
501	629	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Alcohol Isle 4	Both sides of the isle contain huge amounts of name brand alcoholic beverages. You notice that there aren't any beers. The only types of alcohol are high strength variants like vodka, rum, and whiskey. An {red}explosion{/red} in this isle would be {grn}fatal{/grn}.\r\n	\N	\N	1	0	\N	\N
502	630	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Alcohol Isle 4	Both sides of the isle contain huge amounts of name brand alcoholic beverages. You notice that there aren't any beers. The only types of alcohol are high strength variants like vodka, rum, and whiskey. An {red}explosion{/red} in this isle would be {grn}fatal{/grn}.\r\n	\N	\N	1	0	\N	\N
503	631	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Alcohol Isle 4	Both sides of the isle contain huge amounts of name brand alcoholic beverages. You notice that there aren't any beers. The only types of alcohol are high strength variants like vodka, rum, and whiskey. An {red}explosion{/red} in this isle would be {grn}fatal{/grn}.\r\n	\N	\N	1	0	\N	\N
504	632	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Alcohol Isle 4	Both sides of the isle contain huge amounts of name brand alcoholic beverages. You notice that there aren't any beers. The only types of alcohol are high strength variants like vodka, rum, and whiskey. An {red}explosion{/red} in this isle would be {grn}fatal{/grn}.\r\n	\N	\N	1	0	\N	\N
505	633	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Alcohol Isle 4	Both sides of the isle contain huge amounts of name brand alcoholic beverages. You notice that there aren't any beers. The only types of alcohol are high strength variants like vodka, rum, and whiskey. An {red}explosion{/red} in this isle would be {grn}fatal{/grn}.\r\n	\N	\N	1	0	\N	\N
506	634	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Dairy	The isle is dimly lit but that doesn't affect your vision of the contents of this isle. Every product here is in a white container. Milks, butters, cream cheese.. all sorts of kinds of almond milks.. At the end of the isle you notice an end cap displaying a high grade vokda. How appropriate.\r\n	\N	\N	1	0	\N	\N
507	635	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Dairy	The isle is dimly lit but that doesn't affect your vision of the contents of this isle. Every product here is in a white container. Milks, butters, cream cheese.. all sorts of kinds of almond milks.. At the end of the isle you notice an end cap displaying a high grade vokda. How appropriate.\r\n	\N	\N	1	0	\N	\N
508	636	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Dairy	The isle is dimly lit but that doesn't affect your vision of the contents of this isle. Every product here is in a white container. Milks, butters, cream cheese.. all sorts of kinds of almond milks.. At the end of the isle you notice an end cap displaying a high grade vokda. How appropriate.\r\n	\N	\N	1	0	allied-foods-dairy-end4	\N
509	637	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Slaughter Isle	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	\N	\N
586	715	1	0	{blu}Hartford Bank{/blu} - Atrium	Hartford bank, like most of the businesses in downtown metro, is owned and operated by the Hexos Cartel. The lavishly decorated atrium is a testament to the massive amounts of income that is pumped through thousands of fake accounts in a sophisticated money laundering mechanism.\r\n	\N	\N	1	0	\N	\N
510	638	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Slaughter Isle	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	\N	\N
511	639	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Slaughter Isle	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	\N	\N
512	640	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Slaughter Isle	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	\N	\N
513	641	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Slaughter Isle	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	\N	\N
514	642	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Slaughter Isle	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	\N	\N
595	724	1	0	{blu}Hartford Bank{/blu} - Tellers	To the east and west are bank tellers at their respective kiosks. The marble floor has a prestine aura to it. The distant promise of coffee hangs in the air.\r\n	\N	\N	1	0	\N	\N
515	643	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Slaughter Isle	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	\N	\N
516	644	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Slaughter Isle	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	\N	\N
517	645	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Slaughter Isle	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	\N	\N
518	646	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Slaughter Isle	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	\N	\N
519	647	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Slaughter Isle	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	\N	\N
520	648	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Slaughter Isle	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	\N	\N
521	649	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Slaughter Isle	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	\N	\N
522	650	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Slaughter Isle	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	\N	\N
523	651	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Slaughter Isle	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	\N	\N
524	652	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Slaughter Isle	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	\N	\N
525	653	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Slaughter Isle	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	defiler-isle	\N
526	654	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Butcher	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	\N	\N
527	655	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Butcher	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	\N	\N
528	656	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Butcher	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	\N	\N
529	657	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Butcher	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	defiler-corner-1	\N
530	658	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Butcher	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	\N	\N
531	659	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Butcher	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	\N	\N
532	660	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Butcher	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	\N	\N
533	661	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Butcher	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	defiler-east-1	\N
534	662	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Butcher	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	\N	\N
535	663	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Butcher	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	\N	\N
536	664	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Butcher	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	defiler-corner-2	\N
537	665	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Butcher	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	\N	\N
538	666	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Butcher	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	\N	\N
539	667	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Butcher	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	defiler-east-tie	\N
540	668	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Butcher	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	\N	\N
541	669	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Butcher	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	\N	\N
542	670	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Butcher	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	\N	\N
543	671	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Butcher	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	defiler-corner-3	\N
544	672	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Butcher	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	\N	\N
545	673	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Butcher	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	\N	\N
757	889	1	0	{blu}Psi-Tech H.Q.{/blu} - Outside - VIP Drop Off	This area contains many high end vehicles fit for the high value targets that use them.This place is heavily guarded and monitored at all times.\r\n	\N	\N	1	0	psi-tech-car-1	,DRY,OUTSIDE,STREET,PARKING_LOT,PARKING_STALL
546	674	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Butcher	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	\N	\N
547	675	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Butcher	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	defiler-west-1	\N
548	676	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Butcher	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	\N	\N
549	677	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Butcher	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	\N	\N
550	678	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Butcher	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	defiler-corner-4	\N
551	679	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Butcher	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	\N	\N
552	680	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Butcher	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	\N	\N
553	681	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Butcher	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	defiler-corner-5	\N
554	682	1	0	{grn}Abbot Market East{/grn} - Allied Foods - Butcher	The frigid air of a freezer section assaults you into a new state of wakefulness. The white tiled floor has turned to an uneven amount of pink and black due to a large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven patterns that get heavier the further south you go.\r\n	\N	\N	1	0	defiler-spawn	\N
63	192	1	0	Waypoint North	@FILL_ME@\r\n	\N	\N	1	0	waypoint-car-garage-north-1	\N
556	685	1	0	{blu}Metro{/blu} Bornald Road	The road to the heart of Metro City is a bustling hive of busy upper city residents. While the majority of the people who work in Downtown Metro are mid to upper class citizens, a large disproportionate chunk of the people who gather here are drugged out degenerates. Several homeless families make camp right in the middle of the sidewalk: their life for all to judge. \r\n	\N	\N	1	0	\N	\N
557	686	1	0	{blu}Metro{/blu} Henley Fountain	A tribute to the Henley Corporation, an elaborately crafted fountain was erected in the early stages of development of Downtwon Metro City. The water is cleaned several times before it's recycled and spat back out. The fountain has a rectangular cement wall that keeps the water inside. There are elaborately chiseled patterns at the bottom of the pool and on the inner walls.\r\n	\N	\N	1	0	metrofountain-starting-point	\N
558	687	1	0	{blu}Metro{/blu} Henley Fountain	A tribute to the Henley Corporation, an elaborately crafted fountain was erected in the early stages of development of Downtwon Metro City. The water is cleaned several times before it's recycled and spat back out. The fountain has a rectangular cement wall that keeps the water inside. There are elaborately chiseled patterns at the bottom of the pool and on the inner walls.\r\n	\N	\N	1	0	metrofountain-3x3	\N
559	688	1	0	{blu}Metro{/blu} Henley Fountain	A tribute to the Henley Corporation, an elaborately crafted fountain was erected in the early stages of development of Downtwon Metro City. The water is cleaned several times before it's recycled and spat back out. The fountain has a rectangular cement wall that keeps the water inside. There are elaborately chiseled patterns at the bottom of the pool and on the inner walls.\r\n	\N	\N	1	0	metrofountain-3x4	\N
560	689	1	0	{blu}Metro{/blu} Henley Fountain	A tribute to the Henley Corporation, an elaborately crafted fountain was erected in the early stages of development of Downtwon Metro City. The water is cleaned several times before it's recycled and spat back out. The fountain has a rectangular cement wall that keeps the water inside. There are elaborately chiseled patterns at the bottom of the pool and on the inner walls.\r\n	\N	\N	1	0	metrofountain-2x4	\N
561	690	1	0	{blu}Metro{/blu} Henley Fountain	A tribute to the Henley Corporation, an elaborately crafted fountain was erected in the early stages of development of Downtwon Metro City. The water is cleaned several times before it's recycled and spat back out. The fountain has a rectangular cement wall that keeps the water inside. There are elaborately chiseled patterns at the bottom of the pool and on the inner walls.\r\n	\N	\N	1	0	metrofountain-1x4	\N
563	692	1	0	{blu}Metro{/blu} Henley Fountain	A tribute to the Henley Corporation, an elaborately crafted fountain was erected in the early stages of development of Downtwon Metro City. The water is cleaned several times before it's recycled and spat back out. The fountain has a rectangular cement wall that keeps the water inside. There are elaborately chiseled patterns at the bottom of the pool and on the inner walls.\r\n	\N	\N	1	0	metrofountain-1x2	\N
564	693	1	0	{blu}Metro{/blu} Henley Fountain	A tribute to the Henley Corporation, an elaborately crafted fountain was erected in the early stages of development of Downtwon Metro City. The water is cleaned several times before it's recycled and spat back out. The fountain has a rectangular cement wall that keeps the water inside. There are elaborately chiseled patterns at the bottom of the pool and on the inner walls.\r\n	\N	\N	1	0	metrofountain-1x1	\N
565	694	1	0	{blu}Metro{/blu} Henley Fountain	A tribute to the Henley Corporation, an elaborately crafted fountain was erected in the early stages of development of Downtwon Metro City. The water is cleaned several times before it's recycled and spat back out. The fountain has a rectangular cement wall that keeps the water inside. There are elaborately chiseled patterns at the bottom of the pool and on the inner walls.\r\n	\N	\N	1	0	metrofountain-1x0	\N
567	696	1	0	{blu}Metro{/blu} Inside the Henley Fountain	You are up to your knees in water. All of your equipment is starting to soak. That soggy feeling is creeping into your clothes and you feel less agile.\r\n	\N	\N	1	0	metrofountain-2x1	\N
568	697	1	0	{blu}Metro{/blu} Inside the Henley Fountain	You are up to your knees in water. All of your equipment is starting to soak. That soggy feeling is creeping into your clothes and you feel less agile.\r\n	\N	\N	1	0	metrofountain-2x2	\N
569	698	1	0	{blu}Metro{/blu} Inside the Henley Fountain	You are up to your knees in water. All of your equipment is starting to soak. That soggy feeling is creeping into your clothes and you feel less agile.\r\n	\N	\N	1	0	metrofountain-2x3	\N
570	699	1	0	{blu}Metro{/blu} Henley Fountain	A tribute to the Henley Corporation, an elaborately crafted fountain was erected in the early stages of development of Downtwon Metro City. The water is cleaned several times before it's recycled and spat back out. The fountain has a rectangular cement wall that keeps the water inside. There are elaborately chiseled patterns at the bottom of the pool and on the inner walls.\r\n	\N	\N	1	0	metrofountain-3x1	\N
571	700	1	0	{blu}Metro{/blu} Henley Fountain	A tribute to the Henley Corporation, an elaborately crafted fountain was erected in the early stages of development of Downtwon Metro City. The water is cleaned several times before it's recycled and spat back out. The fountain has a rectangular cement wall that keeps the water inside. There are elaborately chiseled patterns at the bottom of the pool and on the inner walls.\r\n	\N	\N	1	0	metrofountain-3x0	\N
572	701	1	0	name	description	\N	\N	1	0	\N	\N
573	702	1	0	{blu}Metro{/blu} Haven Street sidewalk	Along the paved sidewalks of Haven street are all the hallmarks of a busy metropolis. Except from the fact that the homeless denizens outnumber the working class citizens. The sidewalk paints a grim picture of the grip that drugs have on those who are poverty-striken. A testament to their suffering in the form of man made tents and makeshift sleeping bags.\r\n	\N	\N	1	0	\N	\N
596	725	1	0	{blu}Hartford Bank{/blu} - Tellers	To the east and west are bank tellers at their respective kiosks. The marble floor has a prestine aura to it. The distant promise of coffee hangs in the air.\r\n	\N	\N	1	0	\N	\N
574	703	1	0	{blu}Metro{/blu} Haven Street sidewalk	Along the paved sidewalks of Haven street are all the hallmarks of a busy metropolis. Except from the fact that the homeless denizens outnumber the working class citizens. The sidewalk paints a grim picture of the grip that drugs have on those who are poverty-striken. A testament to their suffering in the form of man made tents and makeshift sleeping bags.\r\n	\N	\N	1	0	\N	\N
575	704	1	0	{blu}Metro{/blu} Haven Street sidewalk	Along the paved sidewalks of Haven street are all the hallmarks of a busy metropolis. Except from the fact that the homeless denizens outnumber the working class citizens. The sidewalk paints a grim picture of the grip that drugs have on those who are poverty-striken. A testament to their suffering in the form of man made tents and makeshift sleeping bags.\r\n	\N	\N	1	0	\N	\N
576	705	1	0	{blu}Metro{/blu} Haven Street sidewalk	Along the paved sidewalks of Haven street are all the hallmarks of a busy metropolis. Except from the fact that the homeless denizens outnumber the working class citizens. The sidewalk paints a grim picture of the grip that drugs have on those who are poverty-striken. A testament to their suffering in the form of man made tents and makeshift sleeping bags.\r\n	\N	\N	1	0	bank-west-alley-start	\N
577	706	1	0	{blu}Metro{/blu} Haven Street sidewalk	Along the paved sidewalks of Haven street are all the hallmarks of a busy metropolis. Except from the fact that the homeless denizens outnumber the working class citizens. The sidewalk paints a grim picture of the grip that drugs have on those who are poverty-striken. A testament to their suffering in the form of man made tents and makeshift sleeping bags.\r\n	\N	\N	1	0	\N	\N
578	707	1	0	{blu}Metro{/blu} Hartford Bank entrance	A delightful awning above your head provides sanctuary from the blazing heat of the sun. Two automatic doors to the north welcome you to Hartford bank.\r\n	\N	\N	1	0	\N	\N
579	708	1	0	{blu}Metro{/blu} Hartford Bank entrance	A delightful awning above your head provides sanctuary from the blazing heat of the sun. Two automatic doors to the north welcome you to Hartford bank.\r\n	\N	\N	1	0	\N	\N
580	709	1	0	{blu}Hartford Bank{/blu} - Atrium	Hartford bank, like most of the businesses in downtown metro, is owned and operated by the Hexos Cartel. The lavishly decorated atrium is a testament to the massive amounts of income that is pumped through thousands of fake accounts in a sophisticated money laundering mechanism.\r\n	\N	\N	1	0	\N	\N
581	710	1	0	{blu}Hartford Bank{/blu} - Atrium	Hartford bank, like most of the businesses in downtown metro, is owned and operated by the Hexos Cartel. The lavishly decorated atrium is a testament to the massive amounts of income that is pumped through thousands of fake accounts in a sophisticated money laundering mechanism.\r\n	\N	\N	1	0	\N	\N
555	684	1	0	{blu}Metro{/blu} Bornald Road	The road to the heart of Metro City is a bustling hive of busy upper city residents. While the majority of the people who work in Downtown Metro are mid to upper class citizens, a large disproportionate chunk of the people who gather here are drugged out degenerates. Several homeless families make camp right in the middle of the sidewalk: their life for all to judge. \r\n	\N	\N	1	0	\N	\N
583	712	1	0	{blu}Hartford Bank{/blu} - Atrium	Hartford bank, like most of the businesses in downtown metro, is owned and operated by the Hexos Cartel. The lavishly decorated atrium is a testament to the massive amounts of income that is pumped through thousands of fake accounts in a sophisticated money laundering mechanism.\r\n	\N	\N	1	0	\N	\N
584	713	1	0	{blu}Hartford Bank{/blu} - Atrium	Hartford bank, like most of the businesses in downtown metro, is owned and operated by the Hexos Cartel. The lavishly decorated atrium is a testament to the massive amounts of income that is pumped through thousands of fake accounts in a sophisticated money laundering mechanism.\r\n	\N	\N	1	0	\N	\N
585	714	1	0	{blu}Hartford Bank{/blu} - Atrium	Hartford bank, like most of the businesses in downtown metro, is owned and operated by the Hexos Cartel. The lavishly decorated atrium is a testament to the massive amounts of income that is pumped through thousands of fake accounts in a sophisticated money laundering mechanism.\r\n	\N	\N	1	0	\N	\N
587	716	1	0	{blu}Hartford Bank{/blu} - Atrium	Hartford bank, like most of the businesses in downtown metro, is owned and operated by the Hexos Cartel. The lavishly decorated atrium is a testament to the massive amounts of income that is pumped through thousands of fake accounts in a sophisticated money laundering mechanism.\r\n	\N	\N	1	0	hartford-atrium-west	\N
588	717	1	0	{blu}Hartford Bank{/blu} - Atrium	Hartford bank, like most of the businesses in downtown metro, is owned and operated by the Hexos Cartel. The lavishly decorated atrium is a testament to the massive amounts of income that is pumped through thousands of fake accounts in a sophisticated money laundering mechanism.\r\n	\N	\N	1	0	\N	\N
589	718	1	0	{blu}Hartford Bank{/blu} - Atrium	Hartford bank, like most of the businesses in downtown metro, is owned and operated by the Hexos Cartel. The lavishly decorated atrium is a testament to the massive amounts of income that is pumped through thousands of fake accounts in a sophisticated money laundering mechanism.\r\n	\N	\N	1	0	hartford-atrium-center	\N
590	719	1	0	{blu}Hartford Bank{/blu} - Atrium	Hartford bank, like most of the businesses in downtown metro, is owned and operated by the Hexos Cartel. The lavishly decorated atrium is a testament to the massive amounts of income that is pumped through thousands of fake accounts in a sophisticated money laundering mechanism.\r\n	\N	\N	1	0	\N	\N
591	720	1	0	{blu}Hartford Bank{/blu} - Atrium	Hartford bank, like most of the businesses in downtown metro, is owned and operated by the Hexos Cartel. The lavishly decorated atrium is a testament to the massive amounts of income that is pumped through thousands of fake accounts in a sophisticated money laundering mechanism.\r\n	\N	\N	1	0	hartford-atrium-east	\N
592	721	1	0	{blu}Hartford Bank{/blu} - Atrium	Hartford bank, like most of the businesses in downtown metro, is owned and operated by the Hexos Cartel. The lavishly decorated atrium is a testament to the massive amounts of income that is pumped through thousands of fake accounts in a sophisticated money laundering mechanism.\r\n	\N	\N	1	0	\N	\N
593	722	1	0	{blu}Hartford Bank{/blu} - Atrium	Hartford bank, like most of the businesses in downtown metro, is owned and operated by the Hexos Cartel. The lavishly decorated atrium is a testament to the massive amounts of income that is pumped through thousands of fake accounts in a sophisticated money laundering mechanism.\r\n	\N	\N	1	0	\N	\N
594	723	1	0	{blu}Hartford Bank{/blu} - Tellers	To the east and west are bank tellers at their respective kiosks. The marble floor has a prestine aura to it. The distant promise of coffee hangs in the air.\r\n	\N	\N	1	0	\N	\N
598	727	1	0	{blu}Hartford Bank{/blu} - Tellers	To the east and west are bank tellers at their respective kiosks. The marble floor has a prestine aura to it. The distant promise of coffee hangs in the air.\r\n	\N	\N	1	0	\N	\N
599	728	1	0	{blu}Hartford Bank{/blu} - Tellers	To the east and west are bank tellers at their respective kiosks. The marble floor has a prestine aura to it. The distant promise of coffee hangs in the air.\r\n	\N	\N	1	0	\N	\N
600	729	1	0	{blu}Hartford Bank{/blu} - Teller A	You stand at a teller kiosk. There is a atm card scanner on the desk in front of you On the opposite side of the desk is a large monitor that is angled away from you. \r\n	\N	\N	1	0	hartford-teller-a	\N
601	730	1	0	{blu}Hartford Bank{/blu} - Teller B	You stand at a teller kiosk. There is a atm card scanner on the desk in front of you On the opposite side of the desk is a large monitor that is angled away from you. \r\n	\N	\N	1	0	hartford-teller-b	\N
602	731	1	0	{blu}Hartford Bank{/blu} - Teller C	You stand at a teller kiosk. There is a atm card scanner on the desk in front of you On the opposite side of the desk is a large monitor that is angled away from you. \r\n	\N	\N	1	0	hartford-teller-c	\N
603	732	1	0	{blu}Hartford Bank{/blu} - Teller D	You stand at a teller kiosk. There is a atm card scanner on the desk in front of you On the opposite side of the desk is a large monitor that is angled away from you. \r\n	\N	\N	1	0	hartford-teller-d	\N
562	691	1	0	{blu}Metro{/blu} Henley Fountain	A tribute to the Henley Corporation, an elaborately crafted fountain was erected in the early stages of development of Downtwon Metro City. The water is cleaned several times before it's recycled and spat back out. The fountain has a rectangular cement wall that keeps the water inside. There are elaborately chiseled patterns at the bottom of the pool and on the inner walls.\r\n	\N	\N	1	0	metrofountain-1x3	\N
582	711	1	0	{blu}Hartford Bank{/blu} - Atrium	Hartford bank, like most of the businesses in downtown metro, is owned and operated by the Hexos Cartel. The lavishly decorated atrium is a testament to the massive amounts of income that is pumped through thousands of fake accounts in a sophisticated money laundering mechanism.\r\n	\N	\N	1	0	\N	\N
604	733	1	0	{blu}Hartford Bank West Alley{/blu}	If you hadn't known there was a bank just to the east of here, you'd think you were in the worst time of dystopian nightmare. The ground is littered with used needles, broken glass, and human feces. The very thought of sleeping here makes you cringe, yet you see that many homeless people call this home by evidence of the makeshift shelters made of cardboard and newspaper fragments.\r\n	\N	\N	1	0	\N	\N
605	734	1	0	{blu}Hartford Bank West Alley{/blu}	If you hadn't known there was a bank just to the east of here, you'd think you were in the worst time of dystopian nightmare. The ground is littered with used needles, broken glass, and human feces. The very thought of sleeping here makes you cringe, yet you see that many homeless people call this home by evidence of the makeshift shelters made of cardboard and newspaper fragments.\r\n	\N	\N	1	0	\N	\N
606	735	1	0	{blu}Hartford Bank West Alley{/blu}	If you hadn't known there was a bank just to the east of here, you'd think you were in the worst time of dystopian nightmare. The ground is littered with used needles, broken glass, and human feces. The very thought of sleeping here makes you cringe, yet you see that many homeless people call this home by evidence of the makeshift shelters made of cardboard and newspaper fragments.\r\n	\N	\N	1	0	\N	\N
607	736	1	0	{blu}Hartford Bank West Alley{/blu}	If you hadn't known there was a bank just to the east of here, you'd think you were in the worst time of dystopian nightmare. The ground is littered with used needles, broken glass, and human feces. The very thought of sleeping here makes you cringe, yet you see that many homeless people call this home by evidence of the makeshift shelters made of cardboard and newspaper fragments.\r\n	\N	\N	1	0	\N	\N
608	737	1	0	{blu}Hartford Bank West Alley{/blu}	If you hadn't known there was a bank just to the east of here, you'd think you were in the worst time of dystopian nightmare. The ground is littered with used needles, broken glass, and human feces. The very thought of sleeping here makes you cringe, yet you see that many homeless people call this home by evidence of the makeshift shelters made of cardboard and newspaper fragments.\r\n	\N	\N	1	0	\N	\N
609	738	1	0	{blu}Hartford Bank West Alley{/blu}	If you hadn't known there was a bank just to the east of here, you'd think you were in the worst time of dystopian nightmare. The ground is littered with used needles, broken glass, and human feces. The very thought of sleeping here makes you cringe, yet you see that many homeless people call this home by evidence of the makeshift shelters made of cardboard and newspaper fragments.\r\n	\N	\N	1	0	\N	\N
610	739	1	0	{blu}Hartford Bank West Alley{/blu}	If you hadn't known there was a bank just to the east of here, you'd think you were in the worst time of dystopian nightmare. The ground is littered with used needles, broken glass, and human feces. The very thought of sleeping here makes you cringe, yet you see that many homeless people call this home by evidence of the makeshift shelters made of cardboard and newspaper fragments.\r\n	\N	\N	1	0	\N	\N
611	740	1	0	{blu}Hartford Bank West Alley{/blu}	If you hadn't known there was a bank just to the east of here, you'd think you were in the worst time of dystopian nightmare. The ground is littered with used needles, broken glass, and human feces. The very thought of sleeping here makes you cringe, yet you see that many homeless people call this home by evidence of the makeshift shelters made of cardboard and newspaper fragments.\r\n	\N	\N	1	0	\N	\N
612	741	1	0	{blu}Hartford Bank West Alley{/blu}	If you hadn't known there was a bank just to the east of here, you'd think you were in the worst time of dystopian nightmare. The ground is littered with used needles, broken glass, and human feces. The very thought of sleeping here makes you cringe, yet you see that many homeless people call this home by evidence of the makeshift shelters made of cardboard and newspaper fragments.\r\n	\N	\N	1	0	\N	\N
613	742	1	0	{blu}Hartford Bank West Alley{/blu}	If you hadn't known there was a bank just to the east of here, you'd think you were in the worst time of dystopian nightmare. The ground is littered with used needles, broken glass, and human feces. The very thought of sleeping here makes you cringe, yet you see that many homeless people call this home by evidence of the makeshift shelters made of cardboard and newspaper fragments.\r\n	\N	\N	1	0	\N	\N
614	743	1	0	{blu}Hartford Bank West Alley{/blu}	If you hadn't known there was a bank just to the east of here, you'd think you were in the worst time of dystopian nightmare. The ground is littered with used needles, broken glass, and human feces. The very thought of sleeping here makes you cringe, yet you see that many homeless people call this home by evidence of the makeshift shelters made of cardboard and newspaper fragments.\r\n	\N	\N	1	0	\N	\N
616	745	1	0	{blu}Hartford Bank Vault{/blu}	You find yourself in a heavily fortified bank vault. A scene that few people have the privilege of witnessing. Heavy bags of money line the walls. Giant stacks of money are organized into a neat pile on several tables in the middle of the room.\r\n	\N	\N	1	0	\N	\N
617	746	1	0	{blu}Hartford Bank Vault{/blu}	You find yourself in a heavily fortified bank vault. A scene that few people have the privilege of witnessing. Heavy bags of money line the walls. Giant stacks of money are organized into a neat pile on several tables in the middle of the room.\r\n	\N	\N	1	0	\N	\N
618	748	1	0	name	description	\N	\N	1	0	\N	\N
619	749	1	0	name	description	\N	\N	1	0	\N	\N
620	750	1	0	{blu}Market Street{/blu} - Thompson Avenue	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	thompson-north-1	\N
615	744	1	0	{blu}Hartford Bank Vault{/blu}	You find yourself in a heavily fortified bank vault. A scene that few people have the privilege of witnessing. Heavy bags of money line the walls. Giant stacks of money are organized into a neat pile on several tables in the middle of the room.\r\n	\N	\N	1	0	hartford-bank-west-valley-hidden-wall	\N
621	751	1	0	{blu}Market Street{/blu} - Thompson Avenue	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	\N	\N
623	753	1	0	{blu}Market Street{/blu} - Thompson Avenue	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	\N	\N
624	754	1	0	{blu}Market Street{/blu} - Thompson Avenue	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	thompson-north-2	\N
644	774	1	0	{blu}Market Street{/blu} - Trolley Station	Thanks to hundreds of thousands of dollars in federal and state funding, the Trolley station on Market Street is a reliable source of dependable transportation. The lower income areas tend to be the most reliant on this form of transportation. With that, you get all sorts of different individuals. \r\n	\N	\N	1	0	trolley-section-2	\N
625	755	1	0	{blu}Market Street{/blu} - Thompson Avenue	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	\N	\N
626	756	1	0	{blu}Market Street{/blu} - Thompson Avenue	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	\N	\N
628	758	1	0	{blu}Market Street{/blu} - Thompson Avenue	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	\N	\N
629	759	1	0	{blu}Market Street{/blu} - Thompson Avenue	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	\N	\N
630	760	1	0	{blu}Market Street{/blu} - Thompson Avenue	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	\N	\N
631	761	1	0	{blu}Market Street{/blu} - Thompson Avenue	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	thompson-north-4	\N
632	762	1	0	{blu}Market Street{/blu} - Thompson Avenue	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	\N	\N
633	763	1	0	{blu}Market Street{/blu} - Thompson Avenue	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	\N	\N
634	764	1	0	{blu}Market Street{/blu} - Thompson Avenue	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	thompson-north-5	\N
635	765	1	0	{blu}Market Street{/blu} - Trolley Station	Thanks to hundreds of thousands of dollars in federal and state funding, the Trolley station on Market Street is a reliable source of dependable transportation. The lower income areas tend to be the most reliant on this form of transportation. With that, you get all sorts of different individuals. \r\n	\N	\N	1	0	\N	\N
636	766	1	0	{blu}Market Street{/blu} - Trolley Station	Thanks to hundreds of thousands of dollars in federal and state funding, the Trolley station on Market Street is a reliable source of dependable transportation. The lower income areas tend to be the most reliant on this form of transportation. With that, you get all sorts of different individuals. \r\n	\N	\N	1	0	\N	\N
637	767	1	0	{blu}Market Street{/blu} - Trolley Station	Thanks to hundreds of thousands of dollars in federal and state funding, the Trolley station on Market Street is a reliable source of dependable transportation. The lower income areas tend to be the most reliant on this form of transportation. With that, you get all sorts of different individuals. \r\n	\N	\N	1	0	\N	\N
639	769	1	0	{blu}Market Street{/blu} - Trolley Station	Thanks to hundreds of thousands of dollars in federal and state funding, the Trolley station on Market Street is a reliable source of dependable transportation. The lower income areas tend to be the most reliant on this form of transportation. With that, you get all sorts of different individuals. \r\n	\N	\N	1	0	trolley-section-1	\N
640	770	1	0	{blu}Market Street{/blu} - Trolley Station	Thanks to hundreds of thousands of dollars in federal and state funding, the Trolley station on Market Street is a reliable source of dependable transportation. The lower income areas tend to be the most reliant on this form of transportation. With that, you get all sorts of different individuals. \r\n	\N	\N	1	0	\N	\N
641	771	1	0	{blu}Market Street{/blu} - Trolley Station	Thanks to hundreds of thousands of dollars in federal and state funding, the Trolley station on Market Street is a reliable source of dependable transportation. The lower income areas tend to be the most reliant on this form of transportation. With that, you get all sorts of different individuals. \r\n	\N	\N	1	0	\N	\N
642	772	1	0	{blu}Market Street{/blu} - Trolley Station	Thanks to hundreds of thousands of dollars in federal and state funding, the Trolley station on Market Street is a reliable source of dependable transportation. The lower income areas tend to be the most reliant on this form of transportation. With that, you get all sorts of different individuals. \r\n	\N	\N	1	0	\N	\N
643	773	1	0	{blu}Market Street{/blu} - Trolley Station	Thanks to hundreds of thousands of dollars in federal and state funding, the Trolley station on Market Street is a reliable source of dependable transportation. The lower income areas tend to be the most reliant on this form of transportation. With that, you get all sorts of different individuals. \r\n	\N	\N	1	0	\N	\N
645	775	1	0	{blu}Market Street{/blu} - Trolley Station	Thanks to hundreds of thousands of dollars in federal and state funding, the Trolley station on Market Street is a reliable source of dependable transportation. The lower income areas tend to be the most reliant on this form of transportation. With that, you get all sorts of different individuals. \r\n	\N	\N	1	0	\N	\N
646	776	1	0	{blu}Market Street{/blu} - Trolley Station	Thanks to hundreds of thousands of dollars in federal and state funding, the Trolley station on Market Street is a reliable source of dependable transportation. The lower income areas tend to be the most reliant on this form of transportation. With that, you get all sorts of different individuals. \r\n	\N	\N	1	0	\N	\N
647	777	1	0	{blu}Market Street{/blu} - Trolley Station	Thanks to hundreds of thousands of dollars in federal and state funding, the Trolley station on Market Street is a reliable source of dependable transportation. The lower income areas tend to be the most reliant on this form of transportation. With that, you get all sorts of different individuals. \r\n	\N	\N	1	0	\N	\N
648	778	1	0	{blu}Market Street{/blu} - Trolley Station	Thanks to hundreds of thousands of dollars in federal and state funding, the Trolley station on Market Street is a reliable source of dependable transportation. The lower income areas tend to be the most reliant on this form of transportation. With that, you get all sorts of different individuals. \r\n	\N	\N	1	0	\N	\N
649	779	1	0	{blu}Market Street{/blu} - Trolley Station	Thanks to hundreds of thousands of dollars in federal and state funding, the Trolley station on Market Street is a reliable source of dependable transportation. The lower income areas tend to be the most reliant on this form of transportation. With that, you get all sorts of different individuals. \r\n	\N	\N	1	0	trolley-section-3	\N
650	780	1	0	{blu}Market Street{/blu} - Trolley Station	Thanks to hundreds of thousands of dollars in federal and state funding, the Trolley station on Market Street is a reliable source of dependable transportation. The lower income areas tend to be the most reliant on this form of transportation. With that, you get all sorts of different individuals. \r\n	\N	\N	1	0	\N	\N
651	781	1	0	{blu}Market Street{/blu} - Trolley Station	Thanks to hundreds of thousands of dollars in federal and state funding, the Trolley station on Market Street is a reliable source of dependable transportation. The lower income areas tend to be the most reliant on this form of transportation. With that, you get all sorts of different individuals. \r\n	\N	\N	1	0	\N	\N
652	782	1	0	{blu}Market Street{/blu} - Trolley Station	Thanks to hundreds of thousands of dollars in federal and state funding, the Trolley station on Market Street is a reliable source of dependable transportation. The lower income areas tend to be the most reliant on this form of transportation. With that, you get all sorts of different individuals. \r\n	\N	\N	1	0	\N	\N
653	783	1	0	{blu}Market Street{/blu} - Trolley Station	Thanks to hundreds of thousands of dollars in federal and state funding, the Trolley station on Market Street is a reliable source of dependable transportation. The lower income areas tend to be the most reliant on this form of transportation. With that, you get all sorts of different individuals. \r\n	\N	\N	1	0	\N	\N
566	695	1	0	{blu}Metro{/blu} Henley Fountain	A tribute to the Henley Corporation, an elaborately crafted fountain was erected in the early stages of development of Downtwon Metro City. The water is cleaned several times before it's recycled and spat back out. The fountain has a rectangular cement wall that keeps the water inside. There are elaborately chiseled patterns at the bottom of the pool and on the inner walls.\r\n	\N	\N	1	0	metrofountain-2x0	\N
622	752	1	0	{blu}Market Street{/blu} - Thompson Avenue	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	\N	\N
627	757	1	0	{blu}Market Street{/blu} - Thompson Avenue	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	thompson-north-3	\N
638	768	1	0	{blu}Market Street{/blu} - Trolley Station	Thanks to hundreds of thousands of dollars in federal and state funding, the Trolley station on Market Street is a reliable source of dependable transportation. The lower income areas tend to be the most reliant on this form of transportation. With that, you get all sorts of different individuals. \r\n	\N	\N	1	0	\N	\N
654	784	1	0	{blu}Market Street{/blu} - Trolley Station	Thanks to hundreds of thousands of dollars in federal and state funding, the Trolley station on Market Street is a reliable source of dependable transportation. The lower income areas tend to be the most reliant on this form of transportation. With that, you get all sorts of different individuals. \r\n	\N	\N	1	0	trolley-section-4	\N
655	785	1	0	{blu}Market Street{/blu} - Thompson Avenue - EAST	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	thompson-east-1	\N
656	786	1	0	{blu}Market Street{/blu} - Thompson Avenue - EAST	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	\N	\N
657	787	1	0	{blu}Market Street{/blu} - Thompson Avenue - EAST	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	\N	\N
658	788	1	0	{blu}Market Street{/blu} - Thompson Avenue - EAST	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	thompson-east-2	\N
659	789	1	0	{blu}Market Street{/blu} - Thompson Avenue - EAST	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	\N	\N
660	790	1	0	{blu}Market Street{/blu} - Thompson Avenue - EAST	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	\N	\N
661	791	1	0	{blu}Market Street{/blu} - Thompson Avenue - EAST	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	thompson-east-3	\N
662	792	1	0	{blu}Market Street{/blu} - Thompson Avenue - EAST	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	\N	\N
663	793	1	0	{blu}Market Street{/blu} - Thompson Avenue - EAST	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	\N	\N
664	794	1	0	{blu}Market Street{/blu} - Thompson Avenue - EAST	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	thompson-east-4	\N
665	795	1	0	{blu}Market Street{/blu} - Thompson Avenue - EAST	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	\N	\N
666	796	1	0	{blu}Market Street{/blu} - Thompson Avenue - EAST	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	\N	\N
667	797	1	0	{blu}Market Street{/blu} - Thompson Avenue - EAST	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	thompson-east-5	\N
668	798	1	0	{blu}Market Street{/blu} - Thompson Avenue - EAST	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	\N	\N
669	799	1	0	{blu}Market Street{/blu} - Thompson Avenue - EAST	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	\N	\N
670	800	1	0	{blu}Market Street{/blu} - Thompson Avenue - EAST	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	thompson-east-6	\N
671	801	1	0	{blu}Market Street{/blu} - Thompson Avenue - EAST	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	\N	\N
672	802	1	0	{blu}Market Street{/blu} - Thompson Avenue - EAST	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	\N	\N
673	803	1	0	{blu}Market Street{/blu} - Thompson Avenue - EAST	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	thompson-east-7	\N
674	804	1	0	{blu}Market Street{/blu} - Thompson Avenue - EAST	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	\N	\N
675	805	1	0	{blu}Market Street{/blu} - Thompson Avenue - EAST	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	\N	\N
676	806	1	0	{blu}Market Street{/blu} - Thompson Avenue - EAST	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	thompson-east-8	\N
677	807	1	0	{blu}Market Street{/blu} - Thompson Avenue - EAST	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	\N	\N
678	808	1	0	{blu}Market Street{/blu} - Thompson Avenue - EAST	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	\N	\N
679	809	1	0	{blu}Market Street{/blu} - Thompson Avenue - EAST	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	thompson-east-9	\N
680	810	1	0	{blu}Market Street{/blu} - Thompson Avenue - EAST	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	\N	\N
681	811	1	0	{blu}Market Street{/blu} - Thompson Avenue - EAST	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	\N	\N
682	812	1	0	{blu}Market Street{/blu} - Thompson Avenue - EAST	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	thompson-east-10	\N
683	813	1	0	{blu}Market Street{/blu} - Thompson Avenue - EAST	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	\N	\N
684	814	1	0	{blu}Market Street{/blu} - Thompson Avenue - EAST	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	\N	\N
685	815	1	0	{blu}Market Street{/blu} - Thompson Avenue - EAST	Thompson Avenue is home to a recent spurt in development that started with the redesign and rebranding of the Market Street area. With new leadership and a focus on invigorating and supporting the working class, Thompson Avenue transformed from small farm town, to a semi-thriving metropolis. While the area still has it's problems, it's by far a lot safer than it used to be. \r\n	\N	\N	1	0	thompson-east-11	\N
689	821	1	0	{blu}Crenshaw{/blu} Highway overpass - Ramp	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	,AIR,DESERT,DRY,OUTSIDE
690	822	1	0	{blu}Crenshaw{/blu} Highway overpass - North	The ramp curves to the north. To the east you see a long stretch of highway that disappears over the horizon. You see a shipyard far off to the northeast. \r\n	\N	\N	1	0	\N	,AIR,DESERT,DRY,OUTSIDE
691	823	1	0	{blu}Crenshaw{/blu} Highway overpass - North	The ramp curves to the north. To the east you see a long stretch of highway that disappears over the horizon. You see a shipyard far off to the northeast. \r\n	\N	\N	1	0	\N	,AIR,DESERT,DRY,OUTSIDE
692	824	1	0	{blu}Crenshaw{/blu} Highway overpass - North	The ramp curves to the north. To the east you see a long stretch of highway that disappears over the horizon. You see a shipyard far off to the northeast. \r\n	\N	\N	1	0	\N	,AIR,DESERT,DRY,OUTSIDE
693	825	1	0	{blu}Crenshaw{/blu} Highway 94 East	Six lanes of traffic move for off to the east as far as the eye can see. You see hills of varying heights hugging the highway's north and south sides. There is sparse greenery along the freeway. \r\n	\N	\N	1	0	\N	,AIR,DESERT,DRY,OUTSIDE
694	826	1	0	{blu}Crenshaw{/blu} Highway 94 East	Six lanes of traffic move for off to the east as far as the eye can see. You see hills of varying heights hugging the highway's north and south sides. There is sparse greenery along the freeway. \r\n	\N	\N	1	0	\N	,AIR,DESERT,DRY,OUTSIDE
695	827	1	0	{blu}Crenshaw{/blu} Highway 94 East	Six lanes of traffic move for off to the east as far as the eye can see. You see hills of varying heights hugging the highway's north and south sides. There is sparse greenery along the freeway. \r\n	\N	\N	1	0	\N	,AIR,DESERT,DRY,OUTSIDE
696	828	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	,AIR,DESERT,DRY,OUTSIDE
697	829	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	,AIR,DESERT,DRY,OUTSIDE
698	830	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	,AIR,DESERT,DRY,OUTSIDE
699	831	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	,AIR,DESERT,DRY,OUTSIDE
700	832	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	,AIR,DESERT,DRY,OUTSIDE
764	896	1	25	{blu}Psi-Tech H.Q.{/blu} - Inside - Atrium	You find yourself in a perfectly air conditioned atrium with very high ceilings.Classical music is being played through a pair of hidden speakers somewhere above you. The obsidian colored marble floor is smooth yet slip-resistant.\r\n	\N	\N	1	0	psi-tech-atrium-2	,INSIDE,METAL_WALL,TILE
151	280	1	0	Destroyed overpass - Abott Market South	Crushed and flattened by endless shells, the overpass that you just passed under creaks eternally. Now and then you hear the crumble of asphault and be bend of metal rods. The overpass connects the once thriving market place of Abott Market, but now all you see is rubble and a shell of what used to be a thriving market.\r\n	\N	\N	1	0	\N	\N
687	819	1	0	{blu}Crenshaw{/blu} Highway overpass - Ramp	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	,AIR,DESERT,DRY,OUTSIDE
688	820	1	0	{blu}Crenshaw{/blu} Highway overpass - Ramp	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	,AIR,DESERT,DRY,OUTSIDE
765	897	1	25	{blu}Psi-Tech H.Q.{/blu} - Inside - Atrium	You find yourself in a perfectly air conditioned atrium with very high ceilings.Classical music is being played through a pair of hidden speakers somewhere above you. The obsidian colored marble floor is smooth yet slip-resistant.\r\n	\N	\N	1	0	\N	,INSIDE,METAL_WALL,TILE
705	837	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	,AIR,DESERT,DRY,OUTSIDE
706	838	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	,AIR,DESERT,DRY,OUTSIDE
707	839	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	,AIR,DESERT,DRY,OUTSIDE
708	840	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	,AIR,DESERT,DRY,OUTSIDE
709	841	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	,AIR,DESERT,DRY,OUTSIDE
710	842	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	,AIR,DESERT,DRY,OUTSIDE
711	843	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	,AIR,DESERT,DRY,OUTSIDE
712	844	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	,AIR,DESERT,DRY,OUTSIDE
713	845	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	,AIR,DESERT,DRY,OUTSIDE
714	846	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	,AIR,DESERT,DRY,OUTSIDE
715	847	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	,AIR,DESERT,DRY,OUTSIDE
716	848	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	,AIR,DESERT,DRY,OUTSIDE
702	834	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	,AIR,DESERT,DRY,OUTSIDE
703	835	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	,AIR,DESERT,DRY,OUTSIDE
704	836	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	,AIR,DESERT,DRY,OUTSIDE
721	853	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	,AIR,DESERT,DRY,OUTSIDE
722	854	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	,AIR,DESERT,DRY,OUTSIDE
723	855	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	,AIR,DESERT,DRY,OUTSIDE
724	856	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	,AIR,DESERT,DRY,OUTSIDE
725	857	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	crenshaw-northern-shipping-entrance	,AIR,DESERT,DRY,OUTSIDE
726	858	1	0	{blu}Shipyard{/blu} Entrance	Shipping containers stacked 30 to 40 stories high blot out the sun's rays coming from the east and even overhead during lunch time. Despite the time of day, artificial lighting is needed everywhere. As you make your way deeper into the shipyard, you notice several highly armed individuals patrolling the area. These individuals are wearing masks and have extensive radio communication devices that are resistant to E.M.P.. \r\n	\N	\N	1	0	\N	,AIR,DESERT,DRY,OUTSIDE
727	859	1	0	{blu}Shipyard{/blu} Entrance	Shipping containers stacked 30 to 40 stories high blot out the sun's rays coming from the east and even overhead during lunch time. Despite the time of day, artificial lighting is needed everywhere. As you make your way deeper into the shipyard, you notice several highly armed individuals patrolling the area. These individuals are wearing masks and have extensive radio communication devices that are resistant to E.M.P.. \r\n	\N	\N	1	0	\N	,AIR,DESERT,DRY,OUTSIDE
728	860	1	0	{blu}Shipyard{/blu} Entrance	Shipping containers stacked 30 to 40 stories high blot out the sun's rays coming from the east and even overhead during lunch time. Despite the time of day, artificial lighting is needed everywhere. As you make your way deeper into the shipyard, you notice several highly armed individuals patrolling the area. These individuals are wearing masks and have extensive radio communication devices that are resistant to E.M.P.. \r\n	\N	\N	1	0	shipyard-row-a	,AIR,DESERT,DRY,OUTSIDE
729	861	1	0	{blu}Shipyard{/blu} Shipment Row A	All around you are red and blue shipping containers with varying amounts of rust and outer wear. A few have severe indentations from mishaps. Oddly enough, containers with giant abrasions are bent back into shape and reused. You notice what seems to be burn marks, bullet holes, and dark splattered brownish red stains on a few containers. \r\n	\N	\N	1	0	\N	,AIR,DESERT,DRY,OUTSIDE
730	862	1	0	{blu}Shipyard{/blu} Shipment Row A	All around you are red and blue shipping containers with varying amounts of rust and outer wear. A few have severe indentations from mishaps. Oddly enough, containers with giant abrasions are bent back into shape and reused. You notice what seems to be burn marks, bullet holes, and dark splattered brownish red stains on a few containers. \r\n	\N	\N	1	0	\N	,AIR,DESERT,DRY,OUTSIDE
731	863	1	0	{blu}Shipyard{/blu} Shipment Row A	All around you are red and blue shipping containers with varying amounts of rust and outer wear. A few have severe indentations from mishaps. Oddly enough, containers with giant abrasions are bent back into shape and reused. You notice what seems to be burn marks, bullet holes, and dark splattered brownish red stains on a few containers. \r\n	\N	\N	1	0	\N	,AIR,DESERT,DRY,OUTSIDE
732	864	1	0	{blu}Shipyard{/blu} Shipment Row A	All around you are red and blue shipping containers with varying amounts of rust and outer wear. A few have severe indentations from mishaps. Oddly enough, containers with giant abrasions are bent back into shape and reused. You notice what seems to be burn marks, bullet holes, and dark splattered brownish red stains on a few containers. \r\n	\N	\N	1	0	\N	,AIR,DESERT,DRY,OUTSIDE
718	850	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	,AIR,DESERT,DRY,OUTSIDE
719	851	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	,AIR,DESERT,DRY,OUTSIDE
720	852	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	,AIR,DESERT,DRY,OUTSIDE
738	870	1	0	{blu}Psi-Tech H.Q.{/blu} - Outside - West courtyard	The entrance to {blu}Psi-Tech{/blu} is a stark contrast to the poverty of the surrounding area. The outside of the building is a bland grey-blue mix. Each walkway has stairs and a futuristic looking angular railing. Each corner of the paved sidewalk looks like a sleek cutout from a futuristic jet.The walkway curves down towards the main entrance. You notice a few cameras watching your movement.\r\n	\N	\N	1	0	psi-tech-sniper-post-west	,GLASS_WINDOWS,OUTSIDE,NARROW_EAST_WEST,SIDEWALK
686	818	1	0	{blu}Crenshaw{/blu} Highway overpass	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	crenshaw-overpass-start	,AIR,DESERT,DRY,OUTSIDE
701	833	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	,AIR,DESERT,DRY,OUTSIDE
717	849	1	0	{blu}Crenshaw{/blu} Highway 94 East	A newly paved road stretching to the east and curving towards the north. Speed limit signs are present but they aren't taken seriously by the civilians. Admittedly, the only residents that use the highways are the ones with disproportionate amounts of wealth. You notice a few carcasses of dead animals that made the dire decision to cross this hellish landscape.\r\n	\N	\N	1	0	\N	,AIR,DESERT,DRY,OUTSIDE
733	865	1	0	{blu}Shipyard{/blu} Shipment Row A	All around you are red and blue shipping containers with varying amounts of rust and outer wear. A few have severe indentations from mishaps. Oddly enough, containers with giant abrasions are bent back into shape and reused. You notice what seems to be burn marks, bullet holes, and dark splattered brownish red stains on a few containers. \r\n	\N	\N	1	0	\N	,AIR,DESERT,DRY,OUTSIDE
734	866	1	0	{blu}Shipyard{/blu} Shipment Row A	All around you are red and blue shipping containers with varying amounts of rust and outer wear. A few have severe indentations from mishaps. Oddly enough, containers with giant abrasions are bent back into shape and reused. You notice what seems to be burn marks, bullet holes, and dark splattered brownish red stains on a few containers. \r\n	\N	\N	1	0	\N	,AIR,DESERT,DRY,OUTSIDE
739	871	1	0	{blu}Psi-Tech H.Q.{/blu} - Outside - Main courtyard	The entrance to {blu}Psi-Tech{/blu} is a stark contrast to the poverty of the surrounding area. The outside of the building is a bland grey-blue mix. Each walkway has stairs and a futuristic looking angular railing. Each corner of the paved sidewalk looks like a sleek cutout from a futuristic jet.The walkway curves down towards the main entrance. You notice a few cameras watching your movement.\r\n	\N	\N	1	0	\N	,GLASS_WINDOWS,OUTSIDE,NARROW_EAST_WEST,SIDEWALK
740	872	1	0	{blu}Psi-Tech H.Q.{/blu} - Outside - Main courtyard	The entrance to {blu}Psi-Tech{/blu} is a stark contrast to the poverty of the surrounding area. The outside of the building is a bland grey-blue mix. Each walkway has stairs and a futuristic looking angular railing. Each corner of the paved sidewalk looks like a sleek cutout from a futuristic jet.The walkway curves down towards the main entrance. You notice a few cameras watching your movement.\r\n	\N	\N	1	0	\N	,GLASS_WINDOWS,OUTSIDE,NARROW_EAST_WEST,SIDEWALK
741	873	1	0	{blu}Psi-Tech H.Q.{/blu} - Outside - Main courtyard	The entrance to {blu}Psi-Tech{/blu} is a stark contrast to the poverty of the surrounding area. The outside of the building is a bland grey-blue mix. Each walkway has stairs and a futuristic looking angular railing. Each corner of the paved sidewalk looks like a sleek cutout from a futuristic jet.The walkway curves down towards the main entrance. You notice a few cameras watching your movement.\r\n	\N	\N	1	0	psi-tech-sentry-gun-a	,GLASS_WINDOWS,OUTSIDE,NARROW_EAST_WEST,SIDEWALK
742	874	1	0	{blu}Psi-Tech H.Q.{/blu} - Outside - South entrance	Large marble pillars 6 feet in diameter flank you on both sides. Every ten feet a new row of pillars. The entrance to {blu}Psi-Tech{/blu} is heavily guarded. An array of cameras at the end of the atrium monitor your movements.\r\n	\N	\N	1	0	psi-tech-guard-a	,DRY,OUTSIDE
743	875	1	0	{blu}Psi-Tech H.Q.{/blu} - Outside - South entrance	Large marble pillars 6 feet in diameter flank you on both sides. Every ten feet a new row of pillars. The entrance to {blu}Psi-Tech{/blu} is heavily guarded. An array of cameras at the end of the atrium monitor your movements.\r\n	\N	\N	1	0	psi-tech-south-door	,DRY,OUTSIDE
207	336	1	0	Abandoned Two way street - Abott Market	A trio of burning cars have become part of the debris scattered along the street. Way off to the north, you spot a working overpass. Deep long scars of blackened concrete tell a tale of destruction. To the west lies a small set of apartments once owned by the only property company to operate in this desolate portion of town. A giant construction crane is resting in the middle of the street to the north.\r\n	\N	\N	1	0	psi-tech-start	\N
735	867	1	0	{blu}Psi-Tech H.Q.{/blu} - Outside - West courtyard entrance	The entrance to {blu}Psi-Tech{/blu} is a stark contrast to the poverty of the surrounding area. The outside of the building is a bland grey-blue mix. Each walkway has stairs and a futuristic looking angular railing. Each corner of the paved sidewalk looks like a sleek cutout from a futuristic jet.The walkway curves down towards the main entrance. You notice a few cameras watching your movement.\r\n	\N	\N	1	0	\N	,GLASS_WINDOWS,OUTSIDE,NARROW_EAST_WEST,SIDEWALK
736	868	1	0	{blu}Psi-Tech H.Q.{/blu} - Outside - West courtyard entrance	The entrance to {blu}Psi-Tech{/blu} is a stark contrast to the poverty of the surrounding area. The outside of the building is a bland grey-blue mix. Each walkway has stairs and a futuristic looking angular railing. Each corner of the paved sidewalk looks like a sleek cutout from a futuristic jet.The walkway curves down towards the main entrance. You notice a few cameras watching your movement.\r\n	\N	\N	1	0	\N	,GLASS_WINDOWS,OUTSIDE,NARROW_EAST_WEST,SIDEWALK
737	869	1	0	{blu}Psi-Tech H.Q.{/blu} - Outside - West courtyard	The entrance to {blu}Psi-Tech{/blu} is a stark contrast to the poverty of the surrounding area. The outside of the building is a bland grey-blue mix. Each walkway has stairs and a futuristic looking angular railing. Each corner of the paved sidewalk looks like a sleek cutout from a futuristic jet.The walkway curves down towards the main entrance. You notice a few cameras watching your movement.\r\n	\N	\N	1	0	\N	,GLASS_WINDOWS,OUTSIDE,NARROW_EAST_WEST,SIDEWALK
744	876	1	0	{blu}Psi-Tech H.Q.{/blu} - Outside - South entrance	Large marble pillars 6 feet in diameter flank you on both sides. Every ten feet a new row of pillars. The entrance to {blu}Psi-Tech{/blu} is heavily guarded. An array of cameras at the end of the atrium monitor your movements.\r\n	\N	\N	1	0	psi-tech-guard-b	,DRY,OUTSIDE
745	877	1	0	{blu}Psi-Tech H.Q.{/blu} - Outside - Main courtyard	The entrance to {blu}Psi-Tech{/blu} is a stark contrast to the poverty of the surrounding area. The outside of the building is a bland grey-blue mix. Each walkway has stairs and a futuristic looking angular railing. Each corner of the paved sidewalk looks like a sleek cutout from a futuristic jet.The walkway curves down towards the main entrance. You notice a few cameras watching your movement.\r\n	\N	\N	1	0	psi-tech-sentry-gun-b	,GLASS_WINDOWS,OUTSIDE,NARROW_EAST_WEST,SIDEWALK
746	878	1	0	{blu}Psi-Tech H.Q.{/blu} - Outside - Main courtyard	The entrance to {blu}Psi-Tech{/blu} is a stark contrast to the poverty of the surrounding area. The outside of the building is a bland grey-blue mix. Each walkway has stairs and a futuristic looking angular railing. Each corner of the paved sidewalk looks like a sleek cutout from a futuristic jet.The walkway curves down towards the main entrance. You notice a few cameras watching your movement.\r\n	\N	\N	1	0	\N	,GLASS_WINDOWS,OUTSIDE,NARROW_EAST_WEST,SIDEWALK
747	879	1	0	{blu}Psi-Tech H.Q.{/blu} - Outside - Main courtyard	The entrance to {blu}Psi-Tech{/blu} is a stark contrast to the poverty of the surrounding area. The outside of the building is a bland grey-blue mix. Each walkway has stairs and a futuristic looking angular railing. Each corner of the paved sidewalk looks like a sleek cutout from a futuristic jet.The walkway curves down towards the main entrance. You notice a few cameras watching your movement.\r\n	\N	\N	1	0	\N	,GLASS_WINDOWS,OUTSIDE,NARROW_EAST_WEST,SIDEWALK
748	880	1	0	{blu}Psi-Tech H.Q.{/blu} - Outside - Main courtyard	The entrance to {blu}Psi-Tech{/blu} is a stark contrast to the poverty of the surrounding area. The outside of the building is a bland grey-blue mix. Each walkway has stairs and a futuristic looking angular railing. Each corner of the paved sidewalk looks like a sleek cutout from a futuristic jet.The walkway curves down towards the main entrance. You notice a few cameras watching your movement.\r\n	\N	\N	1	0	\N	,GLASS_WINDOWS,OUTSIDE,NARROW_EAST_WEST,SIDEWALK
749	881	1	0	{blu}Psi-Tech H.Q.{/blu} - Outside - Main courtyard	The entrance to {blu}Psi-Tech{/blu} is a stark contrast to the poverty of the surrounding area. The outside of the building is a bland grey-blue mix. Each walkway has stairs and a futuristic looking angular railing. Each corner of the paved sidewalk looks like a sleek cutout from a futuristic jet.The walkway curves down towards the main entrance. You notice a few cameras watching your movement.\r\n	\N	\N	1	0	\N	,GLASS_WINDOWS,OUTSIDE,NARROW_EAST_WEST,SIDEWALK
750	882	1	0	{blu}Psi-Tech H.Q.{/blu} - Outside - Main courtyard	The entrance to {blu}Psi-Tech{/blu} is a stark contrast to the poverty of the surrounding area. The outside of the building is a bland grey-blue mix. Each walkway has stairs and a futuristic looking angular railing. Each corner of the paved sidewalk looks like a sleek cutout from a futuristic jet.The walkway curves down towards the main entrance. You notice a few cameras watching your movement.\r\n	\N	\N	1	0	psi-tech-sniper-post-east	,GLASS_WINDOWS,OUTSIDE,NARROW_EAST_WEST,SIDEWALK
751	883	1	0	{blu}Psi-Tech H.Q.{/blu} - Outside - East courtyard	This side of the {blu}Psi-Tech{/blu} building has a peculiar lack of windows and entrances. A soft humming sound gives you the impression that a power generator is nearby, though you cannot possibly imagine where.\r\n	\N	\N	1	0	\N	,DRY,OUTSIDE,NARROW_NORTH_SOUTH,SIDEWALK
752	884	1	0	{blu}Psi-Tech H.Q.{/blu} - Outside - East courtyard	This side of the {blu}Psi-Tech{/blu} building has a peculiar lack of windows and entrances. A soft humming sound gives you the impression that a power generator is nearby, though you cannot possibly imagine where.\r\n	\N	\N	1	0	psi-tech-drop-off-start	,DRY,OUTSIDE,NARROW_NORTH_SOUTH,SIDEWALK
753	885	1	0	{blu}Psi-Tech H.Q.{/blu} - Outside - East courtyard	This side of the {blu}Psi-Tech{/blu} building has a peculiar lack of windows and entrances. A soft humming sound gives you the impression that a power generator is nearby, though you cannot possibly imagine where.\r\n	\N	\N	1	0	\N	,DRY,OUTSIDE,NARROW_NORTH_SOUTH,SIDEWALK
754	886	1	0	{blu}Psi-Tech H.Q.{/blu} - Outside - East courtyard	This side of the {blu}Psi-Tech{/blu} building has a peculiar lack of windows and entrances. A soft humming sound gives you the impression that a power generator is nearby, though you cannot possibly imagine where.\r\n	\N	\N	1	0	\N	,DRY,OUTSIDE,NARROW_NORTH_SOUTH,SIDEWALK
755	887	1	0	{blu}Psi-Tech H.Q.{/blu} - Outside - East courtyard	This side of the {blu}Psi-Tech{/blu} building has a peculiar lack of windows and entrances. A soft humming sound gives you the impression that a power generator is nearby, though you cannot possibly imagine where.\r\n	\N	\N	1	0	\N	,DRY,OUTSIDE,NARROW_NORTH_SOUTH,SIDEWALK
756	888	1	0	{blu}Psi-Tech H.Q.{/blu} - Outside - East courtyard	This side of the {blu}Psi-Tech{/blu} building has a peculiar lack of windows and entrances. A soft humming sound gives you the impression that a power generator is nearby, though you cannot possibly imagine where.\r\n	\N	\N	1	0	\N	,DRY,OUTSIDE,NARROW_NORTH_SOUTH,SIDEWALK
759	891	1	0	{blu}Psi-Tech H.Q.{/blu} - Outside - VIP Drop Off	This area contains many high end vehicles fit for the high value targets that use them.This place is heavily guarded and monitored at all times.\r\n	\N	\N	1	0	psi-tech-car-3	,DRY,OUTSIDE,STREET,PARKING_LOT,PARKING_STALL
760	892	1	0	{blu}Psi-Tech H.Q.{/blu} - Outside - VIP Drop Off	This area contains many high end vehicles fit for the high value targets that use them.This place is heavily guarded and monitored at all times.\r\n	\N	\N	1	0	psi-tech-car-4	,DRY,OUTSIDE,STREET,PARKING_LOT,PARKING_STALL
761	893	1	0	{blu}Psi-Tech H.Q.{/blu} - Outside - VIP Drop Off	This area contains many high end vehicles fit for the high value targets that use them.This place is heavily guarded and monitored at all times.\r\n	\N	\N	1	0	psi-tech-car-5	,DRY,OUTSIDE,STREET,PARKING_LOT,PARKING_STALL
762	894	1	25	{blu}Psi-Tech H.Q.{/blu} - Inside - Atrium	You find yourself in a perfectly air conditioned atrium with very high ceilings.Classical music is being played through a pair of hidden speakers somewhere above you. The obsidian colored marble floor is smooth yet slip-resistant.\r\n	\N	\N	1	0	psi-tech-atrium	,INSIDE,METAL_WALL,TILE
763	895	1	25	{blu}Psi-Tech H.Q.{/blu} - Inside - Atrium	You find yourself in a perfectly air conditioned atrium with very high ceilings.Classical music is being played through a pair of hidden speakers somewhere above you. The obsidian colored marble floor is smooth yet slip-resistant.\r\n	\N	\N	1	0	\N	,INSIDE,METAL_WALL,TILE
766	898	1	25	{blu}Psi-Tech H.Q.{/blu} - Inside - Atrium	You find yourself in a perfectly air conditioned atrium with very high ceilings.Classical music is being played through a pair of hidden speakers somewhere above you. The obsidian colored marble floor is smooth yet slip-resistant.\r\n	\N	\N	1	0	\N	,INSIDE,METAL_WALL,TILE
767	899	1	25	{blu}Psi-Tech H.Q.{/blu} - Inside - Atrium	You find yourself in a perfectly air conditioned atrium with very high ceilings.Classical music is being played through a pair of hidden speakers somewhere above you. The obsidian colored marble floor is smooth yet slip-resistant.\r\n	\N	\N	1	0	\N	,INSIDE,METAL_WALL,TILE
768	900	1	25	{blu}Psi-Tech H.Q.{/blu} - Inside - Atrium	You find yourself in a perfectly air conditioned atrium with very high ceilings.Classical music is being played through a pair of hidden speakers somewhere above you. The obsidian colored marble floor is smooth yet slip-resistant.\r\n	\N	\N	1	0	\N	,INSIDE,METAL_WALL,TILE
769	901	1	25	{blu}Psi-Tech H.Q.{/blu} - Inside - Atrium	You find yourself in a perfectly air conditioned atrium with very high ceilings.Classical music is being played through a pair of hidden speakers somewhere above you. The obsidian colored marble floor is smooth yet slip-resistant.\r\n	\N	\N	1	0	\N	,INSIDE,METAL_WALL,TILE
770	902	1	25	{blu}Psi-Tech H.Q.{/blu} - Inside - Atrium	You find yourself in a perfectly air conditioned atrium with very high ceilings.Classical music is being played through a pair of hidden speakers somewhere above you. The obsidian colored marble floor is smooth yet slip-resistant.\r\n	\N	\N	1	0	\N	,INSIDE,METAL_WALL,TILE
771	903	1	0	name	description	\N	\N	1	0	\N	\N
772	904	1	0	name	description	\N	\N	1	0	psi-tech-hatch	,INSIDE,METAL_HATCH,TILE
773	905	1	25	{blu}Psi-Tech H.Q.{/blu} - Inside - Atrium	You find yourself in a perfectly air conditioned atrium with very high ceilings.Classical music is being played through a pair of hidden speakers somewhere above you. The obsidian colored marble floor is smooth yet slip-resistant.\r\n	\N	\N	1	0	\N	,INSIDE,METAL_WALL,TILE
774	906	1	25	{blu}Psi-Tech H.Q.{/blu} - Inside - Atrium	You find yourself in a perfectly air conditioned atrium with very high ceilings.Classical music is being played through a pair of hidden speakers somewhere above you. The obsidian colored marble floor is smooth yet slip-resistant.\r\n	\N	\N	1	0	\N	,INSIDE,METAL_WALL,TILE
1	130	1	21	C.O.F.O.B - Hallway entrance	There is a single door here leading to the inner hallway of the FOB. From there, you can make your way to the different areas like the TRITON Laboratory, the kitchen, the engineering level, and the exit to the outlying city surrounding the FOB.\r\n	\N	\N	1	0	\N	,CEMENT,DAMP,INSIDE,ROOFTOP
2	131	1	21	{blu}C.O.F.O.B{/blu} - Southern Hallway	The southern hallway of the COFOB aligns exactly with a series of huge sewer pipes. Several years ago, a recruit went AWOL and hijacked a tank. He drove it through the southern hallway and other parts of the FOB. As a result, the sewage drains burst open and flooded the entire first level of the FOB. It is why the common nickname for the southern hallway is "el bano".\r\n	\N	\N	1	0	cofobcenter	,CEMENT,INSIDE
102	231	1	0	{blu}C.O.F.O.B{/blu} - Southern Hallway	The southern hallway of the COFOB aligns exactly with a series of huge sewer pipes. Several years ago, a recruit went AWOL and hijacked a tank. He drove it through the southern hallway and other parts of the FOB. As a result, the sewage drains burst open and flooded the entire first level of the FOB. It is why the common nickname for the southern hallway is "el bano".\r\n	\N	\N	1	0	cofob-bind-to-center	\N
100	229	1	0	{blu}C.O.F.O.B{/blu} - Southern Hallway	The southern hallway of the COFOB aligns exactly with a series of huge sewer pipes. Several years ago, a recruit went AWOL and hijacked a tank. He drove it through the southern hallway and other parts of the FOB. As a result, the sewage drains burst open and flooded the entire first level of the FOB. It is why the common nickname for the southern hallway is "el bano".\r\n	\N	\N	1	0	\N	\N
99	228	1	0	{blu}C.O.F.O.B{/blu} - Southern Hallway	The southern hallway of the COFOB aligns exactly with a series of huge sewer pipes. Several years ago, a recruit went AWOL and hijacked a tank. He drove it through the southern hallway and other parts of the FOB. As a result, the sewage drains burst open and flooded the entire first level of the FOB. It is why the common nickname for the southern hallway is "el bano".\r\n	\N	\N	1	0	\N	\N
10	139	1	21	{blu}C.O.F.O.B{/blu} - East Hallway	Many complaints have been lodged against the higher ups about the lack of cleanliness of the eastern hallway. An odd dampness is prevalent throughout. The temperature outside somehow affects this part of the FOB more than others. During sand storms, the eastern hallway turns to a sandy damp mess.\r\n	\N	\N	1	0	\N	,CEMENT,INSIDE
815	949	1	0	{blu}On Base{/blu} - Hydro-Processing	For as long as you can see (east and west), a blue, black, and green tubing the size of a small dog are pinned to the bottom north and south corners. It is noisey in here as whatever is inside that tubing is being fiercely pushed through.\r\n	\N	\N	1	0	\N	,DAMP,INSIDE,METAL_HATCH,METAL_WALL,SEWER,TUNNEL,SHALLOW_WATER,NARROW_EAST_WEST,METAL_FLOORS
17	146	1	21	{blu}C.O.F.O.B{/blu} - TRITON - Research	An array of server racks blocks the center of the room from where you are standing.TRITON Labs is a sub-contractor of the incredibly murky organization known as {blu}ORTHOS CORP{/blu}. The lab specializes in HIGHLY experimental ammunition which blurs the line between ethical and immoral conduct. TRITON Labs is believed to have developed an extreme pathogen with the power to wipe out an entire city block with just a few drops of a highly secretive chemical agent.\r\n	\N	\N	1	0	\N	,CEMENT,INSIDE
16	145	1	21	{blu}C.O.F.O.B{/blu} - TRITON - Experimental Ammunition	TRITON labs has been home to the most advanced bleeding edge experimental ammunition on the planet. Racks of hermetically sealed boxes with TRITON's menacing logo reach almost to the full height of the extended ceiling. There is a single computer terminal here asking for biometric credentials. A camera adjusts it's focus and magnification as you move.\r\n	\N	\N	1	0	\N	,CEMENT,INSIDE
64	193	1	21	{blu}C.O.F.O.B{/blu} - East Hallway	Many complaints have been lodged against the higher ups about the lack of cleanliness of the eastern hallway. An odd dampness is prevalent throughout. The temperature outside somehow affects this part of the FOB more than others. During sand storms, the eastern hallway turns to a sandy damp mess.\r\n	\N	\N	1	0	\N	,CEMENT,INSIDE
65	194	1	21	{blu}C.O.F.O.B{/blu} - East Hallway	Many complaints have been lodged against the higher ups about the lack of cleanliness of the eastern hallway. An odd dampness is prevalent throughout. The temperature outside somehow affects this part of the FOB more than others. During sand storms, the eastern hallway turns to a sandy damp mess.\r\n	\N	\N	1	0	\N	,CEMENT,INSIDE
66	195	1	21	{blu}C.O.F.O.B{/blu} - East Hallway	Many complaints have been lodged against the higher ups about the lack of cleanliness of the eastern hallway. An odd dampness is prevalent throughout. The temperature outside somehow affects this part of the FOB more than others. During sand storms, the eastern hallway turns to a sandy damp mess.\r\n	\N	\N	1	0	\N	,CEMENT,INSIDE
68	197	1	21	{blu}C.O.F.O.B{/blu} - East Hallway	Many complaints have been lodged against the higher ups about the lack of cleanliness of the eastern hallway. An odd dampness is prevalent throughout. The temperature outside somehow affects this part of the FOB more than others. During sand storms, the eastern hallway turns to a sandy damp mess.\r\n	\N	\N	1	0	\N	,CEMENT,INSIDE
69	198	1	21	{blu}C.O.F.O.B{/blu} - East Hallway	Many complaints have been lodged against the higher ups about the lack of cleanliness of the eastern hallway. An odd dampness is prevalent throughout. The temperature outside somehow affects this part of the FOB more than others. During sand storms, the eastern hallway turns to a sandy damp mess.\r\n	\N	\N	1	0	\N	,CEMENT,INSIDE
71	200	1	21	{blu}C.O.F.O.B{/blu} - East Hallway	Many complaints have been lodged against the higher ups about the lack of cleanliness of the eastern hallway. An odd dampness is prevalent throughout. The temperature outside somehow affects this part of the FOB more than others. During sand storms, the eastern hallway turns to a sandy damp mess.\r\n	\N	\N	1	0	\N	,CEMENT,INSIDE
73	202	1	21	{blu}C.O.F.O.B{/blu} - East Hallway	Many complaints have been lodged against the higher ups about the lack of cleanliness of the eastern hallway. An odd dampness is prevalent throughout. The temperature outside somehow affects this part of the FOB more than others. During sand storms, the eastern hallway turns to a sandy damp mess.\r\n	\N	\N	1	0	cofob-mess-hall-upstairs	,CEMENT,INSIDE
74	203	1	21	{blu}C.O.F.O.B{/blu} - East Hallway	Many complaints have been lodged against the higher ups about the lack of cleanliness of the eastern hallway. An odd dampness is prevalent throughout. The temperature outside somehow affects this part of the FOB more than others. During sand storms, the eastern hallway turns to a sandy damp mess.\r\n	\N	\N	1	0	\N	,CEMENT,INSIDE
77	206	1	21	{blu}C.O.F.O.B{/blu} - North Hallway	The north end of the FOB has always been a much nicer and cleaner place than the rest of the base. Primarily due to the frequency of senior officer suites. The north hallway will also bring you to the second floor where a large majority of the engineering work is done.\r\n	\N	\N	1	0	\N	,CEMENT,INSIDE
78	207	1	21	{blu}C.O.F.O.B{/blu} - Staircase	A remarkably plain staircase is here. The second floor of the FOB is home to the {yel}Engineering{/yel} team. Already you can tell that the air quality is much better upstairs.\r\n	\N	\N	1	0	cofob-stairs-A	,CEMENT,INSIDE
106	235	1	21	{blu}C.O.F.O.B{/blu} - Engineering - Atop the stairs	The computing industry has progressed to the point where every individual has their own computer in the palm of their hand. But those advances dont't come without a cost. The engineering level of the FOB focusses on the invisible threat of state actors and the growing need for tighter security. \r\n	\N	\N	1	0	\N	,CEMENT,INSIDE
107	236	1	21	{blu}C.O.F.O.B{/blu} - Engineering - Atop the stairs	The computing industry has progressed to the point where every individual has their own computer in the palm of their hand. But those advances dont't come without a cost. The engineering level of the FOB focusses on the invisible threat of state actors and the growing need for tighter security. \r\n	\N	\N	1	0	\N	,CEMENT,INSIDE
124	253	1	21	{blu}C.O.F.O.B{/blu} - Engineering - Infrastructure Hallway	It's almost comical how clean the engineering level of the FOB is compared to the lower levels. The carpet and walls are in a prestine -- no, IMMACULATE state. You almost feel like you should take off your shoes. The climate is controlled by a different air-conditioning system than the rest of the base. The room is cool and devoid of any outside noise.\r\n	\N	\N	1	0	\N	,CEMENT,INSIDE
125	254	1	21	{blu}C.O.F.O.B{/blu} - Engineering - Infrastructure Hallway	It's almost comical how clean the engineering level of the FOB is compared to the lower levels. The carpet and walls are in a prestine -- no, IMMACULATE state. You almost feel like you should take off your shoes. The climate is controlled by a different air-conditioning system than the rest of the base. The room is cool and devoid of any outside noise.\r\n	\N	\N	1	0	\N	,CEMENT,INSIDE
126	255	1	21	{blu}C.O.F.O.B{/blu} - Engineering - Infrastructure Hallway	It's almost comical how clean the engineering level of the FOB is compared to the lower levels. The carpet and walls are in a prestine -- no, IMMACULATE state. You almost feel like you should take off your shoes. The climate is controlled by a different air-conditioning system than the rest of the base. The room is cool and devoid of any outside noise.\r\n	\N	\N	1	0	\N	,CEMENT,INSIDE
109	238	1	21	{blu}C.O.F.O.B{/blu} - Engineering - Threat Surveillance	There are a couple high profile incidents where law enforcement were locked out of the personal devices of a deceased criminal. A hard lesson in cryptography was learned that day. As a result, funding instead went into massive amounts of global surveillance and threat tracking. The advent of Artificial Intelligence gave another possible avenue: automated tracking. The {red}Threat Surveillance{/red} sector of the FOB is the most advanced and funded aspect of every FOB to date.\r\n	\N	\N	1	0	\N	,CEMENT,INSIDE
110	239	1	21	{blu}C.O.F.O.B{/blu} - Engineering - Threat Surveillance	There are a couple high profile incidents where law enforcement were locked out of the personal devices of a deceased criminal. A hard lesson in cryptography was learned that day. As a result, funding instead went into massive amounts of global surveillance and threat tracking. The advent of Artificial Intelligence gave another possible avenue: automated tracking. The {red}Threat Surveillance{/red} sector of the FOB is the most advanced and funded aspect of every FOB to date.\r\n	\N	\N	1	0	\N	,CEMENT,INSIDE
111	240	1	21	{blu}C.O.F.O.B{/blu} - Engineering - Threat Surveillance	There are a couple high profile incidents where law enforcement were locked out of the personal devices of a deceased criminal. A hard lesson in cryptography was learned that day. As a result, funding instead went into massive amounts of global surveillance and threat tracking. The advent of Artificial Intelligence gave another possible avenue: automated tracking. The {red}Threat Surveillance{/red} sector of the FOB is the most advanced and funded aspect of every FOB to date.\r\n	\N	\N	1	0	\N	,CEMENT,INSIDE
112	241	1	21	{blu}C.O.F.O.B{/blu} - Engineering - Threat Surveillance	There are a couple high profile incidents where law enforcement were locked out of the personal devices of a deceased criminal. A hard lesson in cryptography was learned that day. As a result, funding instead went into massive amounts of global surveillance and threat tracking. The advent of Artificial Intelligence gave another possible avenue: automated tracking. The {red}Threat Surveillance{/red} sector of the FOB is the most advanced and funded aspect of every FOB to date.\r\n	\N	\N	1	0	\N	,CEMENT,INSIDE
75	204	1	21	{blu}C.O.F.O.B{/blu} - North Hallway	The north end of the FOB has always been a much nicer and cleaner place than the rest of the base. Primarily due to the frequency of senior officer suites. The north hallway will also bring you to the second floor where a large majority of the engineering work is done.\r\n	\N	\N	1	0	cofob-northeast-corner	,CEMENT,INSIDE
79	208	1	21	{blu}C.O.F.O.B{/blu} - North Hallway	The north end of the FOB has always been a much nicer and cleaner place than the rest of the base. Primarily due to the frequency of senior officer suites. The north hallway will also bring you to the second floor where a large majority of the engineering work is done.\r\n	\N	\N	1	0	\N	,CEMENT,INSIDE
81	210	1	21	{blu}C.O.F.O.B{/blu} - North Hallway	The north end of the FOB has always been a much nicer and cleaner place than the rest of the base. Primarily due to the frequency of senior officer suites. The north hallway will also bring you to the second floor where a large majority of the engineering work is done.\r\n	\N	\N	1	0	\N	,CEMENT,INSIDE
82	211	1	21	{blu}C.O.F.O.B{/blu} - North Hallway	The north end of the FOB has always been a much nicer and cleaner place than the rest of the base. Primarily due to the frequency of senior officer suites. The north hallway will also bring you to the second floor where a large majority of the engineering work is done.\r\n	\N	\N	1	0	\N	,CEMENT,INSIDE
83	212	1	21	{blu}C.O.F.O.B{/blu} - North Hallway	The north end of the FOB has always been a much nicer and cleaner place than the rest of the base. Primarily due to the frequency of senior officer suites. The north hallway will also bring you to the second floor where a large majority of the engineering work is done.\r\n	\N	\N	1	0	\N	,CEMENT,INSIDE
778	912	1	0	{blu}C.O.F.O.B{/blu} - Maintenance	You make your way through the southern maintenance hallway. Your feet clank on the reinforced steel grating, except when you make contact with the solid metal portions which glue each segment together. There are heavily enforced windows about the size of a dinner plate every 10 feet. The red and orange stains of sand and soot stain the windows. \r\n	\N	\N	1	0	\N	,INSIDE,METAL_WALL,ROOFTOP,METAL_FLOORS
779	913	1	0	{blu}C.O.F.O.B{/blu} - Maintenance	You make your way through the southern maintenance hallway. Your feet clank on the reinforced steel grating, except when you make contact with the solid metal portions which glue each segment together. There are heavily enforced windows about the size of a dinner plate every 10 feet. The red and orange stains of sand and soot stain the windows. \r\n	\N	\N	1	0	maintenance_guard-roaming-0	,INSIDE,METAL_WALL,ROOFTOP,METAL_FLOORS
780	914	1	0	{blu}C.O.F.O.B{/blu} - Maintenance	You make your way through the southern maintenance hallway. Your feet clank on the reinforced steel grating, except when you make contact with the solid metal portions which glue each segment together. There are heavily enforced windows about the size of a dinner plate every 10 feet. The red and orange stains of sand and soot stain the windows. \r\n	\N	\N	1	0	maintenance_guard-roaming-1	,INSIDE,METAL_WALL,ROOFTOP,METAL_FLOORS
781	915	1	0	{blu}C.O.F.O.B{/blu} - Maintenance	You make your way through the southern maintenance hallway. Your feet clank on the reinforced steel grating, except when you make contact with the solid metal portions which glue each segment together. There are heavily enforced windows about the size of a dinner plate every 10 feet. The red and orange stains of sand and soot stain the windows. \r\n	\N	\N	1	0	maintenance_guard-roaming-2	,INSIDE,METAL_WALL,ROOFTOP,METAL_FLOORS
782	916	1	0	{blu}C.O.F.O.B{/blu} - Maintenance	You make your way through the southern maintenance hallway. Your feet clank on the reinforced steel grating, except when you make contact with the solid metal portions which glue each segment together. There are heavily enforced windows about the size of a dinner plate every 10 feet. The red and orange stains of sand and soot stain the windows. \r\n	\N	\N	1	0	maintenance_guard-roaming-3	,INSIDE,METAL_WALL,ROOFTOP,METAL_FLOORS
777	911	1	0	{blu}C.O.F.O.B{/blu} - Maintenance	You make your way through the southern maintenance hallway. Your feet clank on the reinforced steel grating, except when you make contact with the solid metal portions which glue each segment together. There are heavily enforced windows about the size of a dinner plate every 10 feet. The red and orange stains of sand and soot stain the windows. \r\n	\N	\N	1	0	\N	,INSIDE,METAL_WALL,ROOFTOP,METAL_FLOORS
788	922	1	0	{blu}On Base{/blu} - Hydro-Processing	For as long as you can see (east and west), a blue, black, and green tubing the size of a small dog are pinned to the bottom north and south corners. It is noisey in here as whatever is inside that tubing is being fiercely pushed through.\r\n	\N	\N	1	0	maintenance-grunt-roaming-2	,DAMP,INSIDE,METAL_HATCH,METAL_WALL,SEWER,TUNNEL,SHALLOW_WATER,NARROW_EAST_WEST,METAL_FLOORS
789	923	1	0	{blu}On Base{/blu} - Hydro-Processing	For as long as you can see (east and west), a blue, black, and green tubing the size of a small dog are pinned to the bottom north and south corners. It is noisey in here as whatever is inside that tubing is being fiercely pushed through.\r\n	\N	\N	1	0	maintenance-grunt-roaming-3	,DAMP,INSIDE,METAL_HATCH,METAL_WALL,SEWER,TUNNEL,SHALLOW_WATER,NARROW_EAST_WEST,METAL_FLOORS
791	925	1	0	{blu}On Base{/blu} - Hydro-Processing	For as long as you can see (east and west), a blue, black, and green tubing the size of a small dog are pinned to the bottom north and south corners. It is noisey in here as whatever is inside that tubing is being fiercely pushed through.\r\n	\N	\N	1	0	maintenance-grunt-roaming-5	,DAMP,INSIDE,METAL_HATCH,METAL_WALL,SEWER,TUNNEL,SHALLOW_WATER,NARROW_EAST_WEST,METAL_FLOORS
775	909	1	0	{blu}C.O.F.O.B{/blu} - Equipment Inventory	Three huge lockers are attached to the wall, each with their own category of equipment. The most appalling detail is the haphazard array of bloodied medical equipment sitting on a nearby desk.\r\n	\N	\N	1	88	cofob-equipment-inventory	,INSIDE,METAL_WALL,ROOFTOP,TILE
790	924	1	0	{blu}On Base{/blu} - Hydro-Processing	For as long as you can see (east and west), a blue, black, and green tubing the size of a small dog are pinned to the bottom north and south corners. It is noisey in here as whatever is inside that tubing is being fiercely pushed through.\r\n	\N	\N	1	0	maintenance-grunt-roaming-4	,DAMP,INSIDE,METAL_HATCH,METAL_WALL,SEWER,TUNNEL,SHALLOW_WATER,NARROW_EAST_WEST,METAL_FLOORS
787	921	1	0	{blu}On Base{/blu} - Hydro-Processing	For as long as you can see (east and west), a blue, black, and green tubing the size of a small dog are pinned to the bottom north and south corners. It is noisey in here as whatever is inside that tubing is being fiercely pushed through.\r\n	\N	\N	1	0	maintenance-grunt-roaming-1	,DAMP,INSIDE,METAL_HATCH,METAL_WALL,SEWER,TUNNEL,SHALLOW_WATER,NARROW_EAST_WEST,METAL_FLOORS
792	926	1	0	{blu}On Base{/blu} - Hydro-Processing	For as long as you can see (east and west), a blue, black, and green tubing the size of a small dog are pinned to the bottom north and south corners. It is noisey in here as whatever is inside that tubing is being fiercely pushed through.\r\n	\N	\N	1	0	maintenance-grunt-roaming-6	,DAMP,INSIDE,METAL_HATCH,METAL_WALL,SEWER,TUNNEL,SHALLOW_WATER,NARROW_EAST_WEST,METAL_FLOORS
793	927	1	0	{blu}On Base{/blu} - Hydro-Processing	For as long as you can see (east and west), a blue, black, and green tubing the size of a small dog are pinned to the bottom north and south corners. It is noisey in here as whatever is inside that tubing is being fiercely pushed through.\r\n	\N	\N	1	0	maintenance-grunt-roaming-7	,DAMP,INSIDE,METAL_HATCH,METAL_WALL,SEWER,TUNNEL,SHALLOW_WATER,NARROW_EAST_WEST,METAL_FLOORS
794	928	1	0	{blu}On Base{/blu} - Hydro-Processing	For as long as you can see (east and west), a blue, black, and green tubing the size of a small dog are pinned to the bottom north and south corners. It is noisey in here as whatever is inside that tubing is being fiercely pushed through.\r\n	\N	\N	1	0	maintenance-grunt-roaming-8	,DAMP,INSIDE,METAL_HATCH,METAL_WALL,SEWER,TUNNEL,SHALLOW_WATER,NARROW_EAST_WEST,METAL_FLOORS
795	929	1	0	{blu}On Base{/blu} - Hydro-Processing	For as long as you can see (east and west), a blue, black, and green tubing the size of a small dog are pinned to the bottom north and south corners. It is noisey in here as whatever is inside that tubing is being fiercely pushed through.\r\n	\N	\N	1	0	maintenance-grunt-roaming-9	,DAMP,INSIDE,METAL_HATCH,METAL_WALL,SEWER,TUNNEL,SHALLOW_WATER,NARROW_EAST_WEST,METAL_FLOORS
783	917	1	0	{blu}C.O.F.O.B{/blu} - Maintenance	You make your way through the southern maintenance hallway. Your feet clank on the reinforced steel grating, except when you make contact with the solid metal portions which glue each segment together. There are heavily enforced windows about the size of a dinner plate every 10 feet. The red and orange stains of sand and soot stain the windows. \r\n	\N	\N	1	0	maintenance_guard-roaming-4	,INSIDE,METAL_WALL,ROOFTOP,METAL_FLOORS
784	918	1	0	{blu}C.O.F.O.B{/blu} - Maintenance	You make your way through the southern maintenance hallway. Your feet clank on the reinforced steel grating, except when you make contact with the solid metal portions which glue each segment together. There are heavily enforced windows about the size of a dinner plate every 10 feet. The red and orange stains of sand and soot stain the windows. \r\n	\N	\N	1	0	maintenance_guard-roaming-5	,INSIDE,METAL_WALL,ROOFTOP,METAL_FLOORS
776	910	1	0	{blu}C.O.F.O.B{/blu} - Maintenance	You make your way through the southern maintenance hallway. Your feet clank on the reinforced steel grating, except when you make contact with the solid metal portions which glue each segment together. There are heavily enforced windows about the size of a dinner plate every 10 feet. The red and orange stains of sand and soot stain the windows. \r\n	\N	\N	1	0	orthos-spawn-sentinel-1	,INSIDE,METAL_WALL,ROOFTOP,METAL_FLOORS
786	920	1	0	{blu}On Base{/blu} - Hydro-Processing	For as long as you can see (east and west), a blue, black, and green tubing the size of a small dog are pinned to the bottom north and south corners. It is noisey in here as whatever is inside that tubing is being fiercely pushed through.\r\n	\N	\N	1	0	maintenance-grunt-roaming-0	,DAMP,INSIDE,METAL_HATCH,METAL_WALL,SEWER,TUNNEL,SHALLOW_WATER,NARROW_EAST_WEST,METAL_FLOORS
796	930	1	0	{blu}On Base{/blu} - Hydro-Processing	The ladder is reinforced with large protroduing bolts on both sides. There are some points that need to be re-welded as the connections to the sturdiness of the wall seems to have been severed somehow. You notice that the ladder should wiped down due to the massive amount of grease, oil, and somehow blood. \r\n	\N	\N	1	0	\N	,DAMP,INSIDE,METAL_HATCH,METAL_WALL,SEWER,TUNNEL,SHALLOW_WATER,NARROW_EAST_WEST,METAL_FLOORS
808	942	1	0	{blu}On Base{/blu} - Hydro-Processing	For as long as you can see (east and west), a blue, black, and green tubing the size of a small dog are pinned to the bottom north and south corners. It is noisey in here as whatever is inside that tubing is being fiercely pushed through.\r\n	\N	\N	1	0	\N	,DAMP,INSIDE,METAL_HATCH,METAL_WALL,SEWER,TUNNEL,SHALLOW_WATER,NARROW_EAST_WEST,METAL_FLOORS
798	932	1	0	{blu}On Base{/blu} - Hydro-Processing	The ladder is reinforced with large protroduing bolts on both sides. There are some points that need to be re-welded as the connections to the sturdiness of the wall seems to have been severed somehow. You notice that the ladder should wiped down due to the massive amount of grease, oil, and somehow blood. \r\n	\N	\N	1	0	\N	,DAMP,INSIDE,METAL_HATCH,METAL_WALL,SEWER,TUNNEL,SHALLOW_WATER,NARROW_EAST_WEST,METAL_FLOORS
799	933	1	0	{blu}On Base{/blu} - Hydro-Processing	The ladder is reinforced with large protroduing bolts on both sides. There are some points that need to be re-welded as the connections to the sturdiness of the wall seems to have been severed somehow. You notice that the ladder should wiped down due to the massive amount of grease, oil, and somehow blood. \r\n	\N	\N	1	0	\N	,DAMP,INSIDE,METAL_HATCH,METAL_WALL,SEWER,TUNNEL,SHALLOW_WATER,NARROW_EAST_WEST,METAL_FLOORS
800	934	1	0	{blu}On Base{/blu} - Hydro-Processing	The ladder is reinforced with large protroduing bolts on both sides. There are some points that need to be re-welded as the connections to the sturdiness of the wall seems to have been severed somehow. You notice that the ladder should wiped down due to the massive amount of grease, oil, and somehow blood. \r\n	\N	\N	1	0	\N	,DAMP,INSIDE,METAL_HATCH,METAL_WALL,SEWER,TUNNEL,SHALLOW_WATER,NARROW_EAST_WEST,METAL_FLOORS
803	937	1	0	{blu}On Base{/blu} - Hydro-Processing	For as long as you can see (east and west), a blue, black, and green tubing the size of a small dog are pinned to the bottom north and south corners. It is noisey in here as whatever is inside that tubing is being fiercely pushed through.\r\n	\N	\N	1	0	hydro-proc-extremists-roaming-2	,INSIDE,METAL_WALL,NARROW_NORTH_SOUTH,METAL_FLOORS
804	938	1	0	{blu}On Base{/blu} - Hydro-Processing	For as long as you can see (east and west), a blue, black, and green tubing the size of a small dog are pinned to the bottom north and south corners. It is noisey in here as whatever is inside that tubing is being fiercely pushed through.\r\n	\N	\N	1	0	hydro-proc-extremists-roaming-3	,INSIDE,METAL_WALL,NARROW_NORTH_SOUTH,METAL_FLOORS
801	935	1	0	{blu}On Base{/blu} - Hydro-Processing	For as long as you can see (east and west), a blue, black, and green tubing the size of a small dog are pinned to the bottom north and south corners. It is noisey in here as whatever is inside that tubing is being fiercely pushed through.\r\n	\N	\N	1	0	hydro-proc-extremists-roaming-0	,INSIDE,METAL_WALL,NARROW_NORTH_SOUTH,METAL_FLOORS
802	936	1	0	{blu}On Base{/blu} - Hydro-Processing	For as long as you can see (east and west), a blue, black, and green tubing the size of a small dog are pinned to the bottom north and south corners. It is noisey in here as whatever is inside that tubing is being fiercely pushed through.\r\n	\N	\N	1	0	hydro-proc-extremists-roaming-1	,INSIDE,METAL_WALL,NARROW_NORTH_SOUTH,METAL_FLOORS
806	940	1	0	{blu}On Base{/blu} - Hydro-Processing	For as long as you can see (east and west), a blue, black, and green tubing the size of a small dog are pinned to the bottom north and south corners. It is noisey in here as whatever is inside that tubing is being fiercely pushed through.\r\n	\N	\N	1	0	\N	,DAMP,INSIDE,METAL_HATCH,METAL_WALL,SEWER,TUNNEL,SHALLOW_WATER,NARROW_EAST_WEST,METAL_FLOORS
807	941	1	0	{blu}On Base{/blu} - Hydro-Processing	For as long as you can see (east and west), a blue, black, and green tubing the size of a small dog are pinned to the bottom north and south corners. It is noisey in here as whatever is inside that tubing is being fiercely pushed through.\r\n	\N	\N	1	0	hydro-proc-start-point	,DAMP,INSIDE,METAL_HATCH,METAL_WALL,SEWER,TUNNEL,SHALLOW_WATER,NARROW_EAST_WEST,METAL_FLOORS
809	943	1	0	{blu}On Base{/blu} - Hydro-Processing	For as long as you can see (east and west), a blue, black, and green tubing the size of a small dog are pinned to the bottom north and south corners. It is noisey in here as whatever is inside that tubing is being fiercely pushed through.\r\n	\N	\N	1	0	\N	,DAMP,INSIDE,METAL_HATCH,METAL_WALL,SEWER,TUNNEL,SHALLOW_WATER,NARROW_EAST_WEST,METAL_FLOORS
810	944	1	0	{blu}On Base{/blu} - Hydro-Processing	For as long as you can see (east and west), a blue, black, and green tubing the size of a small dog are pinned to the bottom north and south corners. It is noisey in here as whatever is inside that tubing is being fiercely pushed through.\r\n	\N	\N	1	0	\N	,DAMP,INSIDE,METAL_HATCH,METAL_WALL,SEWER,TUNNEL,SHALLOW_WATER,NARROW_EAST_WEST,METAL_FLOORS
811	945	1	0	{blu}On Base{/blu} - Hydro-Processing	For as long as you can see (east and west), a blue, black, and green tubing the size of a small dog are pinned to the bottom north and south corners. It is noisey in here as whatever is inside that tubing is being fiercely pushed through.\r\n	\N	\N	1	0	\N	,DAMP,INSIDE,METAL_HATCH,METAL_WALL,SEWER,TUNNEL,SHALLOW_WATER,NARROW_EAST_WEST,METAL_FLOORS
812	946	1	0	{blu}On Base{/blu} - Hydro-Processing	For as long as you can see (east and west), a blue, black, and green tubing the size of a small dog are pinned to the bottom north and south corners. It is noisey in here as whatever is inside that tubing is being fiercely pushed through.\r\n	\N	\N	1	0	\N	,DAMP,INSIDE,METAL_HATCH,METAL_WALL,SEWER,TUNNEL,SHALLOW_WATER,NARROW_EAST_WEST,METAL_FLOORS
813	947	1	0	{blu}On Base{/blu} - Hydro-Processing	For as long as you can see (east and west), a blue, black, and green tubing the size of a small dog are pinned to the bottom north and south corners. It is noisey in here as whatever is inside that tubing is being fiercely pushed through.\r\n	\N	\N	1	0	\N	,DAMP,INSIDE,METAL_HATCH,METAL_WALL,SEWER,TUNNEL,SHALLOW_WATER,NARROW_EAST_WEST,METAL_FLOORS
814	948	1	0	{blu}On Base{/blu} - Hydro-Processing	For as long as you can see (east and west), a blue, black, and green tubing the size of a small dog are pinned to the bottom north and south corners. It is noisey in here as whatever is inside that tubing is being fiercely pushed through.\r\n	\N	\N	1	0	\N	,DAMP,INSIDE,METAL_HATCH,METAL_WALL,SEWER,TUNNEL,SHALLOW_WATER,NARROW_EAST_WEST,METAL_FLOORS
101	230	1	0	{blu}C.O.F.O.B{/blu} - Southern Hallway	The southern hallway of the COFOB aligns exactly with a series of huge sewer pipes. Several years ago, a recruit went AWOL and hijacked a tank. He drove it through the southern hallway and other parts of the FOB. As a result, the sewage drains burst open and flooded the entire first level of the FOB. It is why the common nickname for the southern hallway is "el bano".\r\n	\N	\N	1	0	\N	\N
20	128	1	18	{blu}C.O.F.O.B{/blu} - Equipment	Three huge lockers are attached to the wall, each with their own category of equipment. The most appalling detail is the haphazard array of bloodied medical equipment sitting on a nearby desk.\r\n	\N	\N	0	16	cofob-spawn-point-a	,CEMENT,DAMP,INSIDE,OUTSIDE,ROOFTOP,SHALLOW_WATER
797	931	1	0	{blu}On Base{/blu} - Hydro-Processing	The ladder is reinforced with large protroduing bolts on both sides. There are some points that need to be re-welded as the connections to the sturdiness of the wall seems to have been severed somehow. You notice that the ladder should wiped down due to the massive amount of grease, oil, and somehow blood. \r\n	\N	\N	1	0	\N	,DAMP,INSIDE,METAL_HATCH,METAL_WALL,SEWER,TUNNEL,SHALLOW_WATER,NARROW_EAST_WEST,METAL_FLOORS
817	951	1	0	{blu}On Base{/blu} - Hydro-Processing - Terminal	A large proportion of the room consists of a locked down computer terminal with old hardware. But it doesn't seem to be just old hardware: everything about it is built to withstand several attempts at trying to access the hardware inside. Things like the hard drives or the device in charge of the biometric interface. It's been said that this terminal can survive several grenade blasts. As of yet, nobody has tried to compromise the  machine, but never say never.\r\n	\N	\N	1	0	\N	,INSIDE,METAL_WALL,SERVER_ROOM,NARROW_NORTH_SOUTH,METAL_FLOORS
818	952	1	0	{blu}On Base{/blu} - Hydro-Processing	For as long as you can see (east and west), a blue, black, and green tubing the size of a small dog are pinned to the bottom north and south corners. It is noisey in here as whatever is inside that tubing is being fiercely pushed through.\r\n	\N	\N	1	0	\N	,DAMP,INSIDE,METAL_HATCH,METAL_WALL,SEWER,TUNNEL,SHALLOW_WATER,NARROW_EAST_WEST,METAL_FLOORS
819	953	1	0	{blu}On Base{/blu} - Hydro-Processing	For as long as you can see (east and west), a blue, black, and green tubing the size of a small dog are pinned to the bottom north and south corners. It is noisey in here as whatever is inside that tubing is being fiercely pushed through.\r\n	\N	\N	1	0	\N	,DAMP,INSIDE,METAL_HATCH,METAL_WALL,SEWER,TUNNEL,SHALLOW_WATER,NARROW_EAST_WEST,METAL_FLOORS
820	954	1	0	{blu}On Base{/blu} - Hydro-Processing	For as long as you can see (east and west), a blue, black, and green tubing the size of a small dog are pinned to the bottom north and south corners. It is noisey in here as whatever is inside that tubing is being fiercely pushed through.\r\n	\N	\N	1	0	\N	,DAMP,INSIDE,METAL_HATCH,METAL_WALL,SEWER,TUNNEL,SHALLOW_WATER,NARROW_EAST_WEST,METAL_FLOORS
821	955	1	0	{blu}On Base{/blu} - Hydro-Processing	For as long as you can see (east and west), a blue, black, and green tubing the size of a small dog are pinned to the bottom north and south corners. It is noisey in here as whatever is inside that tubing is being fiercely pushed through.\r\n	\N	\N	1	0	\N	,DAMP,INSIDE,METAL_HATCH,METAL_WALL,SEWER,TUNNEL,SHALLOW_WATER,NARROW_EAST_WEST,METAL_FLOORS
822	956	1	0	{blu}On Base{/blu} - Hydro-Processing - Terminal	A large proportion of the room consists of a locked down computer terminal with old hardware. But it doesn't seem to be just old hardware: everything about it is built to withstand several attempts at trying to access the hardware inside. Things like the hard drives or the device in charge of the biometric interface. It's been said that this terminal can survive several grenade blasts. As of yet, nobody has tried to compromise the  machine, but never say never.\r\n	\N	\N	1	0	\N	,INSIDE,METAL_WALL,SERVER_ROOM,NARROW_NORTH_SOUTH,METAL_FLOORS
823	957	1	0	{blu}On Base{/blu} - Hydro-Processing - Terminal	A large proportion of the room consists of a locked down computer terminal with old hardware. But it doesn't seem to be just old hardware: everything about it is built to withstand several attempts at trying to access the hardware inside. Things like the hard drives or the device in charge of the biometric interface. It's been said that this terminal can survive several grenade blasts. As of yet, nobody has tried to compromise the  machine, but never say never.\r\n	\N	\N	1	0	\N	,INSIDE,METAL_WALL,SERVER_ROOM,NARROW_NORTH_SOUTH,METAL_FLOORS
824	958	1	0	{blu}On Base{/blu} - Hydro-Processing	For as long as you can see (east and west), a blue, black, and green tubing the size of a small dog are pinned to the bottom north and south corners. It is noisey in here as whatever is inside that tubing is being fiercely pushed through.\r\n	\N	\N	1	0	\N	,INSIDE,METAL_WALL,NARROW_NORTH_SOUTH,METAL_FLOORS
825	959	1	0	{blu}On Base{/blu} - Hydro-Processing	For as long as you can see (east and west), a blue, black, and green tubing the size of a small dog are pinned to the bottom north and south corners. It is noisey in here as whatever is inside that tubing is being fiercely pushed through.\r\n	\N	\N	1	0	\N	,INSIDE,METAL_WALL,NARROW_NORTH_SOUTH,METAL_FLOORS
826	960	1	0	{blu}On Base{/blu} - Hydro-Processing	For as long as you can see (east and west), a blue, black, and green tubing the size of a small dog are pinned to the bottom north and south corners. It is noisey in here as whatever is inside that tubing is being fiercely pushed through.\r\n	\N	\N	1	0	\N	,INSIDE,METAL_WALL,NARROW_EAST_WEST,METAL_FLOORS
827	961	1	0	{blu}On Base{/blu} - Hydro-Processing	For as long as you can see (east and west), a blue, black, and green tubing the size of a small dog are pinned to the bottom north and south corners. It is noisey in here as whatever is inside that tubing is being fiercely pushed through.\r\n	\N	\N	1	0	\N	,INSIDE,METAL_WALL,NARROW_EAST_WEST,METAL_FLOORS
828	962	1	0	{blu}On Base{/blu} - Hydro-Processing	For as long as you can see (east and west), a blue, black, and green tubing the size of a small dog are pinned to the bottom north and south corners. It is noisey in here as whatever is inside that tubing is being fiercely pushed through.\r\n	\N	\N	1	0	\N	,INSIDE,METAL_WALL,NARROW_EAST_WEST,METAL_FLOORS
816	950	1	0	{blu}On Base{/blu} - Hydro-Processing - Terminal	A large proportion of the room consists of a locked down computer terminal with old hardware. But it doesn't seem to be just old hardware: everything about it is built to withstand several attempts at trying to access the hardware inside. Things like the hard drives or the device in charge of the biometric interface. It's been said that this terminal can survive several grenade blasts. As of yet, nobody has tried to compromise the  machine, but never say never.\r\n	\N	\N	1	0	\N	,INSIDE,METAL_WALL,SERVER_ROOM,NARROW_NORTH_SOUTH,METAL_FLOORS
785	919	1	0	{blu}C.O.F.O.B{/blu} - Maintenance	You make your way through the southern maintenance hallway. Your feet clank on the reinforced steel grating, except when you make contact with the solid metal portions which glue each segment together. There are heavily enforced windows about the size of a dinner plate every 10 feet. The red and orange stains of sand and soot stain the windows. \r\n	\N	\N	1	0	maintenance_guard-roaming-6	,INSIDE,METAL_WALL,ROOFTOP,METAL_FLOORS
805	939	1	0	{blu}On Base{/blu} - Hydro-Processing	For as long as you can see (east and west), a blue, black, and green tubing the size of a small dog are pinned to the bottom north and south corners. It is noisey in here as whatever is inside that tubing is being fiercely pushed through.\r\n	\N	\N	1	0	hydro-proc-extremists-roaming-4	,INSIDE,METAL_WALL,NARROW_NORTH_SOUTH,METAL_FLOORS
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
2651	280	0	general_description	keyword	1	0	281
2652	280	1	general_description	keyword	1	0	818
2653	280	2	general description	keyword	1	0	279
2654	818	1	general_description	keyword	1	0	819
2655	818	3	general description	keyword	1	0	280
2656	819	1	general_description	keyword	1	0	820
2657	819	3	general description	keyword	1	0	818
2658	820	1	general_description	keyword	1	0	821
2659	820	3	general description	keyword	1	0	819
2660	821	0	general_description	keyword	1	0	822
2661	821	3	general description	keyword	1	0	820
2662	822	0	general_description	keyword	1	0	823
2663	822	2	general description	keyword	1	0	821
2664	823	0	general_description	keyword	1	0	824
2665	823	2	general description	keyword	1	0	822
2666	824	1	general_description	keyword	1	0	825
2667	824	2	general description	keyword	1	0	823
2668	825	1	general_description	keyword	1	0	826
2669	825	3	general description	keyword	1	0	824
2670	826	1	general_description	keyword	1	0	827
2671	826	3	general description	keyword	1	0	825
2672	827	1	general_description	keyword	1	0	828
2673	827	3	general description	keyword	1	0	826
2674	828	1	general_description	keyword	1	0	829
2675	828	3	general description	keyword	1	0	827
2676	829	1	general_description	keyword	1	0	830
2677	829	3	general description	keyword	1	0	828
2678	830	1	general_description	keyword	1	0	831
2679	830	3	general description	keyword	1	0	829
2680	831	1	general_description	keyword	1	0	832
2681	831	3	general description	keyword	1	0	830
2682	832	1	general_description	keyword	1	0	833
2683	832	3	general description	keyword	1	0	831
2684	833	1	general_description	keyword	1	0	834
2685	833	3	general description	keyword	1	0	832
2686	834	1	general_description	keyword	1	0	835
2687	834	3	general description	keyword	1	0	833
2688	835	1	general_description	keyword	1	0	836
2689	835	3	general description	keyword	1	0	834
2690	836	1	general_description	keyword	1	0	837
2691	836	3	general description	keyword	1	0	835
2692	837	1	general_description	keyword	1	0	838
2693	837	3	general description	keyword	1	0	836
2694	838	1	general_description	keyword	1	0	839
2695	838	3	general description	keyword	1	0	837
2696	839	1	general_description	keyword	1	0	840
2697	839	3	general description	keyword	1	0	838
2698	840	1	general_description	keyword	1	0	841
2699	840	3	general description	keyword	1	0	839
2700	841	1	general_description	keyword	1	0	842
2701	841	3	general description	keyword	1	0	840
2702	842	1	general_description	keyword	1	0	843
2703	842	3	general description	keyword	1	0	841
2704	843	1	general_description	keyword	1	0	844
2705	843	3	general description	keyword	1	0	842
2706	844	1	general_description	keyword	1	0	845
2707	844	3	general description	keyword	1	0	843
2708	845	1	general_description	keyword	1	0	846
2709	845	3	general description	keyword	1	0	844
2710	846	1	general_description	keyword	1	0	847
2711	846	3	general description	keyword	1	0	845
2712	847	1	general_description	keyword	1	0	848
2713	847	3	general description	keyword	1	0	846
2714	848	1	general_description	keyword	1	0	849
2715	848	3	general description	keyword	1	0	847
2716	849	1	general_description	keyword	1	0	850
2717	849	3	general description	keyword	1	0	848
2718	850	1	general_description	keyword	1	0	851
2719	850	3	general description	keyword	1	0	849
2720	851	1	general_description	keyword	1	0	852
2721	851	3	general description	keyword	1	0	850
2722	852	1	general_description	keyword	1	0	853
2723	852	3	general description	keyword	1	0	851
2724	853	1	general_description	keyword	1	0	854
2725	853	3	general description	keyword	1	0	852
2726	854	1	general_description	keyword	1	0	855
2727	854	3	general description	keyword	1	0	853
2728	855	1	general_description	keyword	1	0	856
2729	855	3	general description	keyword	1	0	854
2730	856	1	general_description	keyword	1	0	857
2731	856	3	general description	keyword	1	0	855
2732	857	0	general_description	keyword	1	0	858
2733	857	3	general description	keyword	1	0	856
2734	858	0	general_description	keyword	1	0	859
2735	858	2	general description	keyword	1	0	857
2736	859	0	general_description	keyword	1	0	860
2737	859	2	general description	keyword	1	0	858
2738	860	1	general_description	keyword	1	0	861
2739	860	2	general description	keyword	1	0	859
2740	861	1	general_description	keyword	1	0	862
2741	861	3	general description	keyword	1	0	860
2742	862	0	general_description	keyword	1	0	863
2743	862	3	general description	keyword	1	0	861
2744	863	0	general_description	keyword	1	0	864
2745	863	2	general description	keyword	1	0	862
2746	864	1	general_description	keyword	1	0	865
2747	864	2	general description	keyword	1	0	863
2748	865	1	general_description	keyword	1	0	866
2749	865	3	general description	keyword	1	0	864
2750	866	3	general description	keyword	1	0	865
2751	280	0	general_description	keyword	1	0	281
2752	280	1	general_description	keyword	1	0	818
2753	280	2	general description	keyword	1	0	279
2754	818	1	general_description	keyword	1	0	819
2755	818	3	general description	keyword	1	0	280
2756	819	1	general_description	keyword	1	0	820
2757	819	3	general description	keyword	1	0	818
2758	820	1	general_description	keyword	1	0	821
2759	820	3	general description	keyword	1	0	819
2760	821	0	general_description	keyword	1	0	822
2761	821	3	general description	keyword	1	0	820
2762	822	0	general_description	keyword	1	0	823
2763	822	2	general description	keyword	1	0	821
2764	823	0	general_description	keyword	1	0	824
2765	823	2	general description	keyword	1	0	822
2766	824	1	general_description	keyword	1	0	825
2767	824	2	general description	keyword	1	0	823
2768	825	1	general_description	keyword	1	0	826
2769	825	3	general description	keyword	1	0	824
2770	826	1	general_description	keyword	1	0	827
2771	826	3	general description	keyword	1	0	825
2772	827	1	general_description	keyword	1	0	828
2773	827	3	general description	keyword	1	0	826
2774	828	1	general_description	keyword	1	0	829
2775	828	3	general description	keyword	1	0	827
2776	829	1	general_description	keyword	1	0	830
2777	829	3	general description	keyword	1	0	828
2778	830	1	general_description	keyword	1	0	831
2779	830	3	general description	keyword	1	0	829
2780	831	1	general_description	keyword	1	0	832
2781	831	3	general description	keyword	1	0	830
2782	832	1	general_description	keyword	1	0	833
2783	832	3	general description	keyword	1	0	831
2784	833	1	general_description	keyword	1	0	834
2785	833	3	general description	keyword	1	0	832
2786	834	1	general_description	keyword	1	0	835
2787	834	3	general description	keyword	1	0	833
2788	835	1	general_description	keyword	1	0	836
2789	835	3	general description	keyword	1	0	834
2790	836	1	general_description	keyword	1	0	837
2791	836	3	general description	keyword	1	0	835
2792	837	1	general_description	keyword	1	0	838
2793	837	3	general description	keyword	1	0	836
2794	838	1	general_description	keyword	1	0	839
2795	838	3	general description	keyword	1	0	837
2796	839	1	general_description	keyword	1	0	840
2797	839	3	general description	keyword	1	0	838
2798	840	1	general_description	keyword	1	0	841
2799	840	3	general description	keyword	1	0	839
2800	841	1	general_description	keyword	1	0	842
2801	841	3	general description	keyword	1	0	840
2802	842	1	general_description	keyword	1	0	843
2803	842	3	general description	keyword	1	0	841
2804	843	1	general_description	keyword	1	0	844
2805	843	3	general description	keyword	1	0	842
2806	844	1	general_description	keyword	1	0	845
2807	844	3	general description	keyword	1	0	843
2808	845	1	general_description	keyword	1	0	846
2809	845	3	general description	keyword	1	0	844
2810	846	1	general_description	keyword	1	0	847
2811	846	3	general description	keyword	1	0	845
2812	847	1	general_description	keyword	1	0	848
2813	847	3	general description	keyword	1	0	846
2814	848	1	general_description	keyword	1	0	849
2815	848	3	general description	keyword	1	0	847
2816	849	1	general_description	keyword	1	0	850
2817	849	3	general description	keyword	1	0	848
2818	850	1	general_description	keyword	1	0	851
2819	850	3	general description	keyword	1	0	849
2820	851	1	general_description	keyword	1	0	852
2821	851	3	general description	keyword	1	0	850
2822	852	1	general_description	keyword	1	0	853
2823	852	3	general description	keyword	1	0	851
2824	853	1	general_description	keyword	1	0	854
2825	853	3	general description	keyword	1	0	852
2826	854	1	general_description	keyword	1	0	855
2827	854	3	general description	keyword	1	0	853
2828	855	1	general_description	keyword	1	0	856
2829	855	3	general description	keyword	1	0	854
2830	856	1	general_description	keyword	1	0	857
2831	856	3	general description	keyword	1	0	855
2832	857	0	general_description	keyword	1	0	858
2833	857	3	general description	keyword	1	0	856
2834	858	0	general_description	keyword	1	0	859
2835	858	2	general description	keyword	1	0	857
2836	859	0	general_description	keyword	1	0	860
2837	859	2	general description	keyword	1	0	858
2838	860	1	general_description	keyword	1	0	861
2839	860	2	general description	keyword	1	0	859
2840	861	1	general_description	keyword	1	0	862
2841	861	3	general description	keyword	1	0	860
2842	862	0	general_description	keyword	1	0	863
2843	862	3	general description	keyword	1	0	861
2844	863	0	general_description	keyword	1	0	864
2845	863	2	general description	keyword	1	0	862
2846	864	1	general_description	keyword	1	0	865
2847	864	2	general description	keyword	1	0	863
2848	865	1	general_description	keyword	1	0	866
2849	865	3	general description	keyword	1	0	864
2850	866	3	general description	keyword	1	0	865
2851	280	0	general_description	keyword	1	0	281
2852	280	1	general_description	keyword	1	0	818
2853	280	2	general description	keyword	1	0	279
2854	818	1	general_description	keyword	1	0	819
2855	818	3	general description	keyword	1	0	280
2856	819	1	general_description	keyword	1	0	820
2857	819	3	general description	keyword	1	0	818
2858	820	1	general_description	keyword	1	0	821
2859	820	3	general description	keyword	1	0	819
2860	821	0	general_description	keyword	1	0	822
2861	821	3	general description	keyword	1	0	820
2862	822	0	general_description	keyword	1	0	823
2863	822	2	general description	keyword	1	0	821
2864	823	0	general_description	keyword	1	0	824
2865	823	2	general description	keyword	1	0	822
2866	824	1	general_description	keyword	1	0	825
2867	824	2	general description	keyword	1	0	823
2868	825	1	general_description	keyword	1	0	826
2869	825	3	general description	keyword	1	0	824
2870	826	1	general_description	keyword	1	0	827
2871	826	3	general description	keyword	1	0	825
2872	827	1	general_description	keyword	1	0	828
2873	827	3	general description	keyword	1	0	826
2874	828	1	general_description	keyword	1	0	829
2875	828	3	general description	keyword	1	0	827
2876	829	1	general_description	keyword	1	0	830
2877	829	3	general description	keyword	1	0	828
2878	830	1	general_description	keyword	1	0	831
2879	830	3	general description	keyword	1	0	829
2880	831	1	general_description	keyword	1	0	832
2881	831	3	general description	keyword	1	0	830
2882	832	1	general_description	keyword	1	0	833
2883	832	3	general description	keyword	1	0	831
2884	833	1	general_description	keyword	1	0	834
2885	833	3	general description	keyword	1	0	832
2886	834	1	general_description	keyword	1	0	835
2887	834	3	general description	keyword	1	0	833
2888	835	1	general_description	keyword	1	0	836
2889	835	3	general description	keyword	1	0	834
2890	836	1	general_description	keyword	1	0	837
2891	836	3	general description	keyword	1	0	835
2892	837	1	general_description	keyword	1	0	838
2893	837	3	general description	keyword	1	0	836
2894	838	1	general_description	keyword	1	0	839
2895	838	3	general description	keyword	1	0	837
2896	839	1	general_description	keyword	1	0	840
2897	839	3	general description	keyword	1	0	838
2898	840	1	general_description	keyword	1	0	841
2899	840	3	general description	keyword	1	0	839
2900	841	1	general_description	keyword	1	0	842
2901	841	3	general description	keyword	1	0	840
2902	842	1	general_description	keyword	1	0	843
2903	842	3	general description	keyword	1	0	841
2904	843	1	general_description	keyword	1	0	844
2905	843	3	general description	keyword	1	0	842
2906	844	1	general_description	keyword	1	0	845
2907	844	3	general description	keyword	1	0	843
2908	845	1	general_description	keyword	1	0	846
2909	845	3	general description	keyword	1	0	844
2910	846	1	general_description	keyword	1	0	847
2911	846	3	general description	keyword	1	0	845
2912	847	1	general_description	keyword	1	0	848
2913	847	3	general description	keyword	1	0	846
2914	848	1	general_description	keyword	1	0	849
2915	848	3	general description	keyword	1	0	847
2916	849	1	general_description	keyword	1	0	850
2917	849	3	general description	keyword	1	0	848
2918	850	1	general_description	keyword	1	0	851
2919	850	3	general description	keyword	1	0	849
2920	851	1	general_description	keyword	1	0	852
2921	851	3	general description	keyword	1	0	850
2922	852	1	general_description	keyword	1	0	853
2923	852	3	general description	keyword	1	0	851
2924	853	1	general_description	keyword	1	0	854
2925	853	3	general description	keyword	1	0	852
2926	854	1	general_description	keyword	1	0	855
2927	854	3	general description	keyword	1	0	853
2928	855	1	general_description	keyword	1	0	856
2929	855	3	general description	keyword	1	0	854
2930	856	1	general_description	keyword	1	0	857
2931	856	3	general description	keyword	1	0	855
2932	857	0	general_description	keyword	1	0	858
2933	857	3	general description	keyword	1	0	856
2934	858	0	general_description	keyword	1	0	859
2935	858	2	general description	keyword	1	0	857
2936	859	0	general_description	keyword	1	0	860
2937	859	2	general description	keyword	1	0	858
2938	860	1	general_description	keyword	1	0	861
2939	860	2	general description	keyword	1	0	859
2940	861	1	general_description	keyword	1	0	862
2941	861	3	general description	keyword	1	0	860
2942	862	0	general_description	keyword	1	0	863
2943	862	3	general description	keyword	1	0	861
2944	863	0	general_description	keyword	1	0	864
2945	863	2	general description	keyword	1	0	862
2946	864	1	general_description	keyword	1	0	865
2947	864	2	general description	keyword	1	0	863
2948	865	1	general_description	keyword	1	0	866
2949	865	3	general description	keyword	1	0	864
2950	866	3	general description	keyword	1	0	865
2951	280	0	general_description	keyword	1	0	281
2952	280	1	general_description	keyword	1	0	818
2953	280	2	general description	keyword	1	0	279
2954	818	1	general_description	keyword	1	0	819
2955	818	3	general description	keyword	1	0	280
2956	819	1	general_description	keyword	1	0	820
2957	819	3	general description	keyword	1	0	818
2958	820	1	general_description	keyword	1	0	821
2959	820	3	general description	keyword	1	0	819
2960	821	0	general_description	keyword	1	0	822
2961	821	3	general description	keyword	1	0	820
2962	822	0	general_description	keyword	1	0	823
2963	822	2	general description	keyword	1	0	821
2964	823	0	general_description	keyword	1	0	824
2965	823	2	general description	keyword	1	0	822
2966	824	1	general_description	keyword	1	0	825
2967	824	2	general description	keyword	1	0	823
2968	825	1	general_description	keyword	1	0	826
2969	825	3	general description	keyword	1	0	824
2970	826	1	general_description	keyword	1	0	827
2971	826	3	general description	keyword	1	0	825
2972	827	1	general_description	keyword	1	0	828
2973	827	3	general description	keyword	1	0	826
2974	828	1	general_description	keyword	1	0	829
2975	828	3	general description	keyword	1	0	827
2976	829	1	general_description	keyword	1	0	830
2977	829	3	general description	keyword	1	0	828
2978	830	1	general_description	keyword	1	0	831
2979	830	3	general description	keyword	1	0	829
2980	831	1	general_description	keyword	1	0	832
2981	831	3	general description	keyword	1	0	830
2982	832	1	general_description	keyword	1	0	833
2983	832	3	general description	keyword	1	0	831
2984	833	1	general_description	keyword	1	0	834
2985	833	3	general description	keyword	1	0	832
2986	834	1	general_description	keyword	1	0	835
2987	834	3	general description	keyword	1	0	833
2988	835	1	general_description	keyword	1	0	836
2989	835	3	general description	keyword	1	0	834
2990	836	1	general_description	keyword	1	0	837
2991	836	3	general description	keyword	1	0	835
2992	837	1	general_description	keyword	1	0	838
2993	837	3	general description	keyword	1	0	836
2994	838	1	general_description	keyword	1	0	839
2995	838	3	general description	keyword	1	0	837
2996	839	1	general_description	keyword	1	0	840
2997	839	3	general description	keyword	1	0	838
2998	840	1	general_description	keyword	1	0	841
2999	840	3	general description	keyword	1	0	839
3000	841	1	general_description	keyword	1	0	842
3001	841	3	general description	keyword	1	0	840
3002	842	1	general_description	keyword	1	0	843
3003	842	3	general description	keyword	1	0	841
3004	843	1	general_description	keyword	1	0	844
3005	843	3	general description	keyword	1	0	842
3006	844	1	general_description	keyword	1	0	845
3007	844	3	general description	keyword	1	0	843
3008	845	1	general_description	keyword	1	0	846
3009	845	3	general description	keyword	1	0	844
3010	846	1	general_description	keyword	1	0	847
3011	846	3	general description	keyword	1	0	845
3012	847	1	general_description	keyword	1	0	848
3013	847	3	general description	keyword	1	0	846
3014	848	1	general_description	keyword	1	0	849
3015	848	3	general description	keyword	1	0	847
3016	849	1	general_description	keyword	1	0	850
3017	849	3	general description	keyword	1	0	848
3018	850	1	general_description	keyword	1	0	851
3019	850	3	general description	keyword	1	0	849
3020	851	1	general_description	keyword	1	0	852
3021	851	3	general description	keyword	1	0	850
3022	852	1	general_description	keyword	1	0	853
3023	852	3	general description	keyword	1	0	851
3024	853	1	general_description	keyword	1	0	854
3025	853	3	general description	keyword	1	0	852
3026	854	1	general_description	keyword	1	0	855
3027	854	3	general description	keyword	1	0	853
3028	855	1	general_description	keyword	1	0	856
3029	855	3	general description	keyword	1	0	854
3030	856	1	general_description	keyword	1	0	857
3031	856	3	general description	keyword	1	0	855
3032	857	0	general_description	keyword	1	0	858
3033	857	3	general description	keyword	1	0	856
3034	858	0	general_description	keyword	1	0	859
3035	858	2	general description	keyword	1	0	857
3036	859	0	general_description	keyword	1	0	860
3037	859	2	general description	keyword	1	0	858
3038	860	1	general_description	keyword	1	0	861
3039	860	2	general description	keyword	1	0	859
3040	861	1	general_description	keyword	1	0	862
3041	861	3	general description	keyword	1	0	860
3042	862	0	general_description	keyword	1	0	863
3043	862	3	general description	keyword	1	0	861
3044	863	0	general_description	keyword	1	0	864
3045	863	2	general description	keyword	1	0	862
3046	864	1	general_description	keyword	1	0	865
3047	864	2	general description	keyword	1	0	863
3048	865	1	general_description	keyword	1	0	866
3049	865	3	general description	keyword	1	0	864
3050	866	3	general description	keyword	1	0	865
3051	280	0	general_description	keyword	1	0	281
3052	280	1	general_description	keyword	1	0	818
3053	280	2	general description	keyword	1	0	279
3054	818	1	general_description	keyword	1	0	819
3055	818	3	general description	keyword	1	0	280
3056	819	1	general_description	keyword	1	0	820
3057	819	3	general description	keyword	1	0	818
3058	820	1	general_description	keyword	1	0	821
3059	820	3	general description	keyword	1	0	819
3060	821	0	general_description	keyword	1	0	822
3061	821	3	general description	keyword	1	0	820
3062	822	0	general_description	keyword	1	0	823
3063	822	2	general description	keyword	1	0	821
3064	823	0	general_description	keyword	1	0	824
3065	823	2	general description	keyword	1	0	822
3066	824	1	general_description	keyword	1	0	825
3067	824	2	general description	keyword	1	0	823
3068	825	1	general_description	keyword	1	0	826
3069	825	3	general description	keyword	1	0	824
3070	826	1	general_description	keyword	1	0	827
3071	826	3	general description	keyword	1	0	825
3072	827	1	general_description	keyword	1	0	828
3073	827	3	general description	keyword	1	0	826
3074	828	1	general_description	keyword	1	0	829
3075	828	3	general description	keyword	1	0	827
3076	829	1	general_description	keyword	1	0	830
3077	829	3	general description	keyword	1	0	828
3078	830	1	general_description	keyword	1	0	831
3079	830	3	general description	keyword	1	0	829
3080	831	1	general_description	keyword	1	0	832
3081	831	3	general description	keyword	1	0	830
3082	832	1	general_description	keyword	1	0	833
3083	832	3	general description	keyword	1	0	831
3084	833	1	general_description	keyword	1	0	834
3085	833	3	general description	keyword	1	0	832
3086	834	1	general_description	keyword	1	0	835
3087	834	3	general description	keyword	1	0	833
3088	835	1	general_description	keyword	1	0	836
3089	835	3	general description	keyword	1	0	834
3090	836	1	general_description	keyword	1	0	837
3091	836	3	general description	keyword	1	0	835
3092	837	1	general_description	keyword	1	0	838
3093	837	3	general description	keyword	1	0	836
3094	838	1	general_description	keyword	1	0	839
3095	838	3	general description	keyword	1	0	837
3096	839	1	general_description	keyword	1	0	840
3097	839	3	general description	keyword	1	0	838
3098	840	1	general_description	keyword	1	0	841
3099	840	3	general description	keyword	1	0	839
3100	841	1	general_description	keyword	1	0	842
3101	841	3	general description	keyword	1	0	840
3102	842	1	general_description	keyword	1	0	843
3103	842	3	general description	keyword	1	0	841
3104	843	1	general_description	keyword	1	0	844
3105	843	3	general description	keyword	1	0	842
3106	844	1	general_description	keyword	1	0	845
3107	844	3	general description	keyword	1	0	843
3108	845	1	general_description	keyword	1	0	846
3109	845	3	general description	keyword	1	0	844
3110	846	1	general_description	keyword	1	0	847
3111	846	3	general description	keyword	1	0	845
3112	847	1	general_description	keyword	1	0	848
3113	847	3	general description	keyword	1	0	846
3114	848	1	general_description	keyword	1	0	849
3115	848	3	general description	keyword	1	0	847
3116	849	1	general_description	keyword	1	0	850
3117	849	3	general description	keyword	1	0	848
3118	850	1	general_description	keyword	1	0	851
3119	850	3	general description	keyword	1	0	849
3120	851	1	general_description	keyword	1	0	852
3121	851	3	general description	keyword	1	0	850
3122	852	1	general_description	keyword	1	0	853
3123	852	3	general description	keyword	1	0	851
3124	853	1	general_description	keyword	1	0	854
3125	853	3	general description	keyword	1	0	852
3126	854	1	general_description	keyword	1	0	855
3127	854	3	general description	keyword	1	0	853
3128	855	1	general_description	keyword	1	0	856
3129	855	3	general description	keyword	1	0	854
3130	856	1	general_description	keyword	1	0	857
3131	856	3	general description	keyword	1	0	855
3132	857	0	general_description	keyword	1	0	858
3133	857	3	general description	keyword	1	0	856
3134	858	0	general_description	keyword	1	0	859
3135	858	2	general description	keyword	1	0	857
3136	859	0	general_description	keyword	1	0	860
3137	859	2	general description	keyword	1	0	858
3138	860	1	general_description	keyword	1	0	861
3139	860	2	general description	keyword	1	0	859
3140	861	1	general_description	keyword	1	0	862
3141	861	3	general description	keyword	1	0	860
3142	862	0	general_description	keyword	1	0	863
3143	862	3	general description	keyword	1	0	861
3144	863	0	general_description	keyword	1	0	864
3145	863	2	general description	keyword	1	0	862
3146	864	1	general_description	keyword	1	0	865
3147	864	2	general description	keyword	1	0	863
3148	865	1	general_description	keyword	1	0	866
3149	865	3	general description	keyword	1	0	864
3150	866	3	general description	keyword	1	0	865
3151	336	0	general_description	keyword	1	0	337
3152	336	1	general_description	keyword	1	0	867
3153	336	2	general description	keyword	1	0	335
3154	867	1	general_description	keyword	1	0	868
3155	867	3	general description	keyword	1	0	336
3156	868	0	general_description	keyword	1	0	869
3157	868	3	general description	keyword	1	0	867
3158	869	0	general_description	keyword	1	0	870
3159	869	2	general description	keyword	1	0	868
3160	870	1	general_description	keyword	1	0	871
3161	870	2	general description	keyword	1	0	869
3162	871	1	general_description	keyword	1	0	872
3163	871	3	general description	keyword	1	0	870
3164	872	1	general_description	keyword	1	0	873
3165	872	3	general description	keyword	1	0	871
3166	873	0	general_description	keyword	1	0	874
3167	873	3	general description	keyword	1	0	872
3168	874	1	general_description	keyword	1	0	875
3169	874	2	general description	keyword	1	0	873
3170	875	0	general_description	keyword	1	0	894
3171	875	1	general_description	keyword	1	0	876
3172	875	3	general description	keyword	1	0	874
3173	876	2	general_description	keyword	1	0	877
3174	876	3	general description	keyword	1	0	875
3175	877	0	general description	keyword	1	0	876
3176	877	1	general_description	keyword	1	0	878
3177	878	1	general_description	keyword	1	0	879
3178	878	3	general description	keyword	1	0	877
3179	879	1	general_description	keyword	1	0	880
3180	879	3	general description	keyword	1	0	878
3181	880	1	general_description	keyword	1	0	881
3182	880	3	general description	keyword	1	0	879
3183	881	1	general_description	keyword	1	0	882
3184	881	3	general description	keyword	1	0	880
3185	882	0	general_description	keyword	1	0	883
3186	882	3	general description	keyword	1	0	881
3187	883	0	general_description	keyword	1	0	884
3188	883	2	general description	keyword	1	0	882
3189	884	0	general_description	keyword	1	0	885
3190	884	1	general_description	keyword	1	0	889
3191	884	2	general description	keyword	1	0	883
3192	885	0	general_description	keyword	1	0	886
3193	885	2	general description	keyword	1	0	884
3194	886	1	general_description	keyword	1	0	887
3195	886	2	general description	keyword	1	0	885
3196	887	0	general_description	keyword	1	0	888
3197	887	3	general description	keyword	1	0	886
3198	888	2	general description	keyword	1	0	887
3199	889	1	general_description	keyword	1	0	890
3200	889	3	general description	keyword	1	0	884
3201	890	1	general_description	keyword	1	0	891
3202	890	3	general description	keyword	1	0	889
3203	891	0	general_description	keyword	1	0	892
3204	891	3	general description	keyword	1	0	890
3205	892	1	general_description	keyword	1	0	893
3206	892	2	general description	keyword	1	0	891
3207	893	3	general description	keyword	1	0	892
3208	894	0	general_description	keyword	1	0	895
3209	894	2	general description	keyword	1	0	875
3210	895	0	general_description	keyword	1	0	896
3211	895	2	general description	keyword	1	0	894
3212	896	0	general_description	keyword	1	0	897
3213	896	2	general description	keyword	1	0	895
3214	897	0	general_description	keyword	1	0	898
3215	897	1	general_description	keyword	1	0	903
3216	897	2	general description	keyword	1	0	896
3217	897	3	general_description	keyword	1	0	899
3218	898	2	general description	keyword	1	0	897
3219	899	1	general description	keyword	1	0	897
3220	899	3	general_description	keyword	1	0	900
3221	900	1	general description	keyword	1	0	899
3222	900	3	general_description	keyword	1	0	901
3223	901	1	general description	keyword	1	0	900
3224	901	3	general_description	keyword	1	0	902
3225	902	1	general description	keyword	1	0	901
3226	903	1	general_description	keyword	1	0	904
3227	903	3	general description	keyword	1	0	897
3228	904	1	general_description	keyword	1	0	905
3229	904	3	general description	keyword	1	0	903
3230	904	5	A	metal	0	4	128
3231	905	1	general_description	keyword	1	0	906
3232	905	3	general description	keyword	1	0	904
3233	906	3	general description	keyword	1	0	905
3234	128	0	general_description	keyword	1	0	130
3235	130	0	general_description	keyword	1	0	131
3236	130	2	general description	keyword	1	0	128
3237	131	1	general_description	keyword	1	0	138
3238	131	2	general description	keyword	1	0	130
3239	131	3	general description	keyword	1	0	231
3240	138	1	general_description	keyword	1	0	139
3241	138	3	general description	keyword	1	0	131
3242	139	0	general_description	keyword	1	0	140
3243	139	3	general description	keyword	1	0	138
3244	231	1	general description	keyword	1	0	131
3245	231	3	general description	keyword	1	0	230
3246	230	1	general_description	keyword	1	0	231
3247	230	3	general description	keyword	1	0	229
3248	229	1	general_description	keyword	1	0	230
3249	229	3	general description	keyword	1	0	228
3250	228	1	general_description	keyword	1	0	229
3251	228	3	general description	keyword	1	0	227
3252	227	1	general_description	keyword	1	0	228
3253	227	3	general description	keyword	1	0	226
3254	226	0	general description	keyword	1	0	225
3255	226	1	general_description	keyword	1	0	227
3256	139	0	general_description	keyword	1	0	140
3257	139	3	general description	keyword	1	0	138
3258	140	0	general_description	keyword	1	0	141
3259	140	2	general description	keyword	1	0	139
3260	141	0	general_description	keyword	1	0	193
3261	141	1	general_description	keyword	1	0	142
3262	141	2	general description	keyword	1	0	140
3263	142	1	general_description	keyword	1	0	143
3264	142	3	general description	keyword	1	0	141
3265	143	0	general_description	keyword	1	0	144
3266	143	1	general_description	keyword	1	0	146
3267	143	2	general_description	keyword	1	0	145
3268	143	3	general description	keyword	1	0	142
3269	146	1	general_description	keyword	1	0	147
3270	146	3	general description	keyword	1	0	143
3271	147	3	general description	keyword	1	0	146
3272	147	5	general_description	keyword	1	0	148
3273	144	2	general description	keyword	1	0	143
3274	145	0	general description	keyword	1	0	143
3275	128	0	general_description	keyword	1	0	130
3276	909	2	general description	door	3	0	230
3277	193	0	general_description	keyword	1	0	194
3278	193	2	general description	keyword	1	0	141
3279	194	0	general_description	keyword	1	0	195
3280	194	2	general description	keyword	1	0	193
3281	195	0	general_description	keyword	1	0	196
3282	195	2	general description	keyword	1	0	194
3283	196	0	general_description	keyword	1	0	197
3284	196	2	general description	keyword	1	0	195
3285	197	0	general_description	keyword	1	0	198
3286	197	2	general description	keyword	1	0	196
3287	198	0	general_description	keyword	1	0	199
3288	198	2	general description	keyword	1	0	197
3289	200	0	general_description	keyword	1	0	201
3290	200	2	general description	keyword	1	0	199
3291	201	0	general_description	keyword	1	0	202
3292	201	2	general description	keyword	1	0	200
3293	202	0	general_description	keyword	1	0	203
3294	202	2	general description	keyword	1	0	201
3295	202	3	general_description	keyword	1	0	340
3296	203	0	general_description	keyword	1	0	204
3297	203	1	general_description	keyword	1	0	265
3298	203	2	general description	keyword	1	0	202
3299	205	1	general description	keyword	1	0	204
3300	205	3	general_description	keyword	1	0	206
3301	206	1	general description	keyword	1	0	205
3302	206	3	general_description	keyword	1	0	207
3303	207	0	general_description	keyword	1	0	232
3304	207	1	general description	keyword	1	0	206
3305	207	3	general_description	keyword	1	0	208
3306	232	2	general description	keyword	1	0	207
3307	232	4	general_description	keyword	1	0	233
3308	233	0	general_description	keyword	1	0	234
3309	233	5	general description	keyword	1	0	232
3310	234	2	general description	keyword	1	0	233
3311	234	4	general_description	keyword	1	0	235
3312	235	0	general_description	keyword	1	0	236
3313	235	5	general description	keyword	1	0	234
3314	236	0	general_description	keyword	1	0	237
3315	236	2	general description	keyword	1	0	235
3316	237	0	general_description	keyword	1	0	243
3317	237	1	general_description	keyword	1	0	253
3318	237	2	general description	keyword	1	0	236
3319	237	3	general_description	keyword	1	0	238
3320	253	1	general_description	keyword	1	0	254
3321	253	3	general description	keyword	1	0	237
3322	254	1	general_description	keyword	1	0	255
3323	254	3	general description	keyword	1	0	253
3324	255	1	general_description	keyword	1	0	256
3325	255	3	general description	keyword	1	0	254
3326	256	0	general_description	keyword	1	0	258
3327	256	1	general_description	keyword	1	0	257
3328	256	3	general description	keyword	1	0	255
3329	257	3	general description	keyword	1	0	256
3330	238	1	general description	keyword	1	0	237
3331	238	3	general_description	keyword	1	0	239
3332	239	1	general description	keyword	1	0	238
3333	239	3	general_description	keyword	1	0	240
3334	240	1	general description	keyword	1	0	239
3335	240	3	general_description	keyword	1	0	241
3336	241	1	general description	keyword	1	0	240
3337	241	3	general_description	keyword	1	0	242
3338	242	1	general description	keyword	1	0	241
3339	204	2	general description	keyword	1	0	203
3340	204	3	general_description	keyword	1	0	205
3341	208	1	general description	keyword	1	0	207
3342	208	3	general_description	keyword	1	0	209
3343	209	1	general description	keyword	1	0	208
3344	209	3	general_description	keyword	1	0	210
3345	210	1	general description	keyword	1	0	209
3346	210	3	general_description	keyword	1	0	211
3347	211	1	general description	keyword	1	0	210
3348	211	3	general_description	keyword	1	0	212
3349	212	1	general description	keyword	1	0	211
3350	212	2	general_description	keyword	1	0	213
3351	910	0	general description	keyword	1	0	128
3352	911	0	general description	keyword	1	0	910
3353	912	0	general description	keyword	1	0	911
3354	913	0	general description	keyword	1	0	912
3355	914	0	general description	keyword	1	0	913
3356	915	0	general description	keyword	1	0	914
3357	916	0	general description	keyword	1	0	915
3358	917	0	general description	keyword	1	0	916
3359	918	0	general description	keyword	1	0	917
3360	919	0	general description	keyword	1	0	918
3361	230	0	general_description	keyword	1	0	909
3362	230	1	general_description	keyword	1	0	231
3363	230	3	general description	keyword	1	0	229
3364	909	2	general description	door	3	0	230
3365	128	0	general_description	keyword	1	0	130
3366	128	2	general_description	keyword	1	0	910
3367	910	0	general description	keyword	1	0	128
3368	910	2	general_description	keyword	1	0	911
3369	911	0	general description	keyword	1	0	910
3370	911	2	general_description	keyword	1	0	912
3371	912	0	general description	keyword	1	0	911
3372	912	2	general_description	keyword	1	0	913
3373	913	0	general description	keyword	1	0	912
3374	913	2	general_description	keyword	1	0	914
3375	914	0	general description	keyword	1	0	913
3376	914	2	general_description	keyword	1	0	915
3377	915	0	general description	keyword	1	0	914
3378	915	2	general_description	keyword	1	0	916
3379	916	0	general description	keyword	1	0	915
3380	916	2	general_description	keyword	1	0	917
3381	917	0	general description	keyword	1	0	916
3382	917	2	general_description	keyword	1	0	918
3383	918	0	general description	keyword	1	0	917
3384	918	2	general_description	keyword	1	0	919
3385	919	0	general description	keyword	1	0	918
3386	920	1	general description	keyword	1	0	912
3387	921	1	general description	keyword	1	0	920
3388	922	1	general description	keyword	1	0	921
3389	923	1	general description	keyword	1	0	922
3390	924	1	general description	keyword	1	0	923
3391	925	1	general description	keyword	1	0	924
3392	926	1	general description	keyword	1	0	925
3393	927	1	general description	keyword	1	0	926
3394	928	1	general description	keyword	1	0	927
3395	929	1	general description	keyword	1	0	928
3396	930	4	general description	keyword	1	0	921
3397	931	4	general description	keyword	1	0	930
3398	932	4	general description	keyword	1	0	931
3399	933	4	general description	keyword	1	0	932
3400	934	4	general description	keyword	1	0	933
3401	935	0	general description	keyword	1	0	934
3402	936	0	general description	keyword	1	0	935
3403	937	0	general description	keyword	1	0	936
3404	938	0	general description	keyword	1	0	937
3405	939	0	general description	keyword	1	0	938
3406	940	1	general description	keyword	1	0	939
3407	941	1	general description	keyword	1	0	940
3408	941	1	general description	keyword	1	0	940
3409	942	1	general description	keyword	1	0	941
3410	943	1	general description	keyword	1	0	942
3411	944	1	general description	keyword	1	0	943
3412	945	1	general description	keyword	1	0	944
3413	946	1	general description	keyword	1	0	945
3414	947	1	general description	keyword	1	0	946
3415	948	1	general description	keyword	1	0	947
3416	949	1	general description	keyword	1	0	948
3417	950	2	general description	keyword	1	0	949
3418	951	0	general description	keyword	1	0	949
3419	952	1	general description	keyword	1	0	949
3420	953	1	general description	keyword	1	0	952
3421	954	1	general description	keyword	1	0	953
3422	955	1	general description	keyword	1	0	954
3423	956	2	general description	keyword	1	0	953
3424	957	0	general description	keyword	1	0	953
3425	958	2	general description	keyword	1	0	955
3426	959	2	general description	keyword	1	0	958
3427	960	1	general description	keyword	1	0	959
3428	961	1	general description	keyword	1	0	960
3429	962	1	general description	keyword	1	0	961
3430	230	0	general_description	keyword	1	0	909
3431	230	1	general_description	keyword	1	0	231
3432	230	3	general description	keyword	1	0	229
3433	909	2	general description	door	3	0	230
3434	128	0	general_description	keyword	1	0	130
3435	128	2	general_description	keyword	1	0	910
3436	910	0	general description	keyword	1	0	128
3437	910	2	general_description	keyword	1	0	911
3438	911	0	general description	keyword	1	0	910
3439	911	2	general_description	keyword	1	0	912
3440	912	0	general description	keyword	1	0	911
3441	912	2	general_description	keyword	1	0	913
3442	912	3	general_description	keyword	1	0	920
3443	913	0	general description	keyword	1	0	912
3444	913	2	general_description	keyword	1	0	914
3445	914	0	general description	keyword	1	0	913
3446	914	2	general_description	keyword	1	0	915
3447	915	0	general description	keyword	1	0	914
3448	915	2	general_description	keyword	1	0	916
3449	916	0	general description	keyword	1	0	915
3450	916	2	general_description	keyword	1	0	917
3451	917	0	general description	keyword	1	0	916
3452	917	2	general_description	keyword	1	0	918
3453	918	0	general description	keyword	1	0	917
3454	918	2	general_description	keyword	1	0	919
3455	919	0	general description	keyword	1	0	918
3456	920	1	general description	keyword	1	0	912
3457	920	3	general_description	keyword	1	0	921
3458	921	1	general description	keyword	1	0	920
3459	921	3	general_description	keyword	1	0	922
3460	921	5	general_description	keyword	1	0	930
3461	922	1	general description	keyword	1	0	921
3462	922	3	general_description	keyword	1	0	923
3463	923	1	general description	keyword	1	0	922
3464	923	3	general_description	keyword	1	0	924
3465	924	1	general description	keyword	1	0	923
3466	924	3	general_description	keyword	1	0	925
3467	925	1	general description	keyword	1	0	924
3468	925	3	general_description	keyword	1	0	926
3469	926	1	general description	keyword	1	0	925
3470	926	3	general_description	keyword	1	0	927
3471	927	1	general description	keyword	1	0	926
3472	927	3	general_description	keyword	1	0	928
3473	928	1	general description	keyword	1	0	927
3474	928	3	general_description	keyword	1	0	929
3475	929	1	general description	keyword	1	0	928
3476	930	4	general description	keyword	1	0	921
3477	930	5	general_description	keyword	1	0	931
3478	931	4	general description	keyword	1	0	930
3479	931	5	general_description	keyword	1	0	932
3480	932	4	general description	keyword	1	0	931
3481	932	5	general_description	keyword	1	0	933
3482	933	4	general description	keyword	1	0	932
3483	933	5	general_description	keyword	1	0	934
3484	934	2	general_description	keyword	1	0	935
3485	934	4	general description	keyword	1	0	933
3486	935	0	general description	keyword	1	0	934
3487	935	2	general_description	keyword	1	0	936
3488	936	0	general description	keyword	1	0	935
3489	936	2	general_description	keyword	1	0	937
3490	937	0	general description	keyword	1	0	936
3491	937	2	general_description	keyword	1	0	938
3492	938	0	general description	keyword	1	0	937
3493	938	2	general_description	keyword	1	0	939
3494	939	0	general description	keyword	1	0	938
3495	939	3	general_description	keyword	1	0	940
3496	940	1	general description	keyword	1	0	939
3497	940	3	general_description	keyword	1	0	941
3498	941	1	general description	keyword	1	0	940
3499	941	3	general_description	keyword	1	0	942
3500	942	1	general description	keyword	1	0	941
3501	942	3	general_description	keyword	1	0	943
3502	943	1	general description	keyword	1	0	942
3503	943	3	general_description	keyword	1	0	944
3504	944	1	general description	keyword	1	0	943
3505	944	3	general_description	keyword	1	0	945
3506	945	1	general description	keyword	1	0	944
3507	945	3	general_description	keyword	1	0	946
3508	946	1	general description	keyword	1	0	945
3509	946	3	general_description	keyword	1	0	947
3510	947	1	general description	keyword	1	0	946
3511	947	3	general_description	keyword	1	0	948
3512	948	1	general description	keyword	1	0	947
3513	948	3	general_description	keyword	1	0	949
3514	949	0	general_description	keyword	1	0	950
3515	949	1	general description	keyword	1	0	948
3516	949	2	general_description	keyword	1	0	951
3517	949	3	general_description	keyword	1	0	952
3518	950	2	general description	keyword	1	0	949
3519	951	0	general description	keyword	1	0	949
3520	952	1	general description	keyword	1	0	949
3521	952	3	general_description	keyword	1	0	953
3522	953	0	general_description	keyword	1	0	956
3523	953	1	general description	keyword	1	0	952
3524	953	2	general_description	keyword	1	0	957
3525	953	3	general_description	keyword	1	0	954
3526	954	1	general description	keyword	1	0	953
3527	954	3	general_description	keyword	1	0	955
3528	955	0	general_description	keyword	1	0	958
3529	955	1	general description	keyword	1	0	954
3530	956	2	general description	keyword	1	0	953
3531	957	0	general description	keyword	1	0	953
3532	958	0	general_description	keyword	1	0	959
3533	958	2	general description	keyword	1	0	955
3534	959	2	general description	keyword	1	0	958
3535	959	3	general_description	keyword	1	0	960
3536	960	1	general description	keyword	1	0	959
3537	960	3	general_description	keyword	1	0	961
3538	961	1	general description	keyword	1	0	960
3539	961	3	general_description	keyword	1	0	962
3540	962	1	general description	keyword	1	0	961
3541	920	1	general description	keyword	1	0	912
3542	920	3	general_description	keyword	1	0	921
3543	921	1	general description	keyword	1	0	920
3544	921	3	general_description	keyword	1	0	922
3545	921	5	general_description	keyword	1	0	930
3546	922	1	general description	keyword	1	0	921
3547	922	3	general_description	keyword	1	0	923
3548	923	1	general description	keyword	1	0	922
3549	923	3	general_description	keyword	1	0	924
3550	924	1	general description	keyword	1	0	923
3551	924	3	general_description	keyword	1	0	925
3552	925	1	general description	keyword	1	0	924
3553	925	3	general_description	keyword	1	0	926
3554	926	1	general description	keyword	1	0	925
3555	926	3	general_description	keyword	1	0	927
3556	927	1	general description	keyword	1	0	926
3557	927	3	general_description	keyword	1	0	928
3558	928	1	general description	keyword	1	0	927
3559	928	3	general_description	keyword	1	0	929
3560	929	1	general description	keyword	1	0	928
3561	913	0	general description	keyword	1	0	912
3562	913	2	general_description	keyword	1	0	914
3563	914	0	general description	keyword	1	0	913
3564	914	2	general_description	keyword	1	0	915
3565	915	0	general description	keyword	1	0	914
3566	915	2	general_description	keyword	1	0	916
3567	916	0	general description	keyword	1	0	915
3568	916	2	general_description	keyword	1	0	917
3569	917	0	general description	keyword	1	0	916
3570	917	2	general_description	keyword	1	0	918
3571	918	0	general description	keyword	1	0	917
3572	918	2	general_description	keyword	1	0	919
3573	919	0	general description	keyword	1	0	918
3574	910	0	general description	keyword	1	0	128
3575	910	2	general_description	keyword	1	0	911
3576	920	1	general description	keyword	1	0	912
3577	920	3	general_description	keyword	1	0	921
3578	921	1	general description	keyword	1	0	920
3579	921	3	general_description	keyword	1	0	922
3580	921	5	general_description	keyword	1	0	930
3581	922	1	general description	keyword	1	0	921
3582	922	3	general_description	keyword	1	0	923
3583	923	1	general description	keyword	1	0	922
3584	923	3	general_description	keyword	1	0	924
3585	924	1	general description	keyword	1	0	923
3586	924	3	general_description	keyword	1	0	925
3587	925	1	general description	keyword	1	0	924
3588	925	3	general_description	keyword	1	0	926
3589	926	1	general description	keyword	1	0	925
3590	926	3	general_description	keyword	1	0	927
3591	927	1	general description	keyword	1	0	926
3592	927	3	general_description	keyword	1	0	928
3593	928	1	general description	keyword	1	0	927
3594	928	3	general_description	keyword	1	0	929
3595	929	1	general description	keyword	1	0	928
3596	913	0	general description	keyword	1	0	912
3597	913	2	general_description	keyword	1	0	914
3598	914	0	general description	keyword	1	0	913
3599	914	2	general_description	keyword	1	0	915
3600	915	0	general description	keyword	1	0	914
3601	915	2	general_description	keyword	1	0	916
3602	916	0	general description	keyword	1	0	915
3603	916	2	general_description	keyword	1	0	917
3604	917	0	general description	keyword	1	0	916
3605	917	2	general_description	keyword	1	0	918
3606	918	0	general description	keyword	1	0	917
3607	918	2	general_description	keyword	1	0	919
3608	919	0	general description	keyword	1	0	918
3609	910	0	general description	keyword	1	0	128
3610	910	2	general_description	keyword	1	0	911
3611	920	1	general description	keyword	1	0	912
3612	920	3	general_description	keyword	1	0	921
3613	921	1	general description	keyword	1	0	920
3614	921	3	general_description	keyword	1	0	922
3615	921	5	general_description	keyword	1	0	930
3616	922	1	general description	keyword	1	0	921
3617	922	3	general_description	keyword	1	0	923
3618	923	1	general description	keyword	1	0	922
3619	923	3	general_description	keyword	1	0	924
3620	924	1	general description	keyword	1	0	923
3621	924	3	general_description	keyword	1	0	925
3622	925	1	general description	keyword	1	0	924
3623	925	3	general_description	keyword	1	0	926
3624	926	1	general description	keyword	1	0	925
3625	926	3	general_description	keyword	1	0	927
3626	927	1	general description	keyword	1	0	926
3627	927	3	general_description	keyword	1	0	928
3628	928	1	general description	keyword	1	0	927
3629	928	3	general_description	keyword	1	0	929
3630	929	1	general description	keyword	1	0	928
3631	913	0	general description	keyword	1	0	912
3632	913	2	general_description	keyword	1	0	914
3633	914	0	general description	keyword	1	0	913
3634	914	2	general_description	keyword	1	0	915
3635	915	0	general description	keyword	1	0	914
3636	915	2	general_description	keyword	1	0	916
3637	916	0	general description	keyword	1	0	915
3638	916	2	general_description	keyword	1	0	917
3639	917	0	general description	keyword	1	0	916
3640	917	2	general_description	keyword	1	0	918
3641	918	0	general description	keyword	1	0	917
3642	918	2	general_description	keyword	1	0	919
3643	919	0	general description	keyword	1	0	918
3644	910	0	general description	keyword	1	0	128
3645	910	2	general_description	keyword	1	0	911
3646	920	1	general description	keyword	1	0	912
3647	920	3	general_description	keyword	1	0	921
3648	921	1	general description	keyword	1	0	920
3649	921	3	general_description	keyword	1	0	922
3650	921	5	general_description	keyword	1	0	930
3651	922	1	general description	keyword	1	0	921
3652	922	3	general_description	keyword	1	0	923
3653	923	1	general description	keyword	1	0	922
3654	923	3	general_description	keyword	1	0	924
3655	924	1	general description	keyword	1	0	923
3656	924	3	general_description	keyword	1	0	925
3657	925	1	general description	keyword	1	0	924
3658	925	3	general_description	keyword	1	0	926
3659	926	1	general description	keyword	1	0	925
3660	926	3	general_description	keyword	1	0	927
3661	927	1	general description	keyword	1	0	926
3662	927	3	general_description	keyword	1	0	928
3663	928	1	general description	keyword	1	0	927
3664	928	3	general_description	keyword	1	0	929
3665	929	1	general description	keyword	1	0	928
3666	913	0	general description	keyword	1	0	912
3667	913	2	general_description	keyword	1	0	914
3668	914	0	general description	keyword	1	0	913
3669	914	2	general_description	keyword	1	0	915
3670	915	0	general description	keyword	1	0	914
3671	915	2	general_description	keyword	1	0	916
3672	916	0	general description	keyword	1	0	915
3673	916	2	general_description	keyword	1	0	917
3674	917	0	general description	keyword	1	0	916
3675	917	2	general_description	keyword	1	0	918
3676	918	0	general description	keyword	1	0	917
3677	918	2	general_description	keyword	1	0	919
3678	919	0	general description	keyword	1	0	918
3679	910	0	general description	keyword	1	0	128
3680	910	2	general_description	keyword	1	0	911
3681	920	1	general description	keyword	1	0	912
3682	920	3	general_description	keyword	1	0	921
3683	921	1	general description	keyword	1	0	920
3684	921	3	general_description	keyword	1	0	922
3685	921	5	general_description	keyword	1	0	930
3686	922	1	general description	keyword	1	0	921
3687	922	3	general_description	keyword	1	0	923
3688	923	1	general description	keyword	1	0	922
3689	923	3	general_description	keyword	1	0	924
3690	924	1	general description	keyword	1	0	923
3691	924	3	general_description	keyword	1	0	925
3692	925	1	general description	keyword	1	0	924
3693	925	3	general_description	keyword	1	0	926
3694	926	1	general description	keyword	1	0	925
3695	926	3	general_description	keyword	1	0	927
3696	927	1	general description	keyword	1	0	926
3697	927	3	general_description	keyword	1	0	928
3698	928	1	general description	keyword	1	0	927
3699	928	3	general_description	keyword	1	0	929
3700	929	1	general description	keyword	1	0	928
3701	913	0	general description	keyword	1	0	912
3702	913	2	general_description	keyword	1	0	914
3703	914	0	general description	keyword	1	0	913
3704	914	2	general_description	keyword	1	0	915
3705	915	0	general description	keyword	1	0	914
3706	915	2	general_description	keyword	1	0	916
3707	916	0	general description	keyword	1	0	915
3708	916	2	general_description	keyword	1	0	917
3709	917	0	general description	keyword	1	0	916
3710	917	2	general_description	keyword	1	0	918
3711	918	0	general description	keyword	1	0	917
3712	918	2	general_description	keyword	1	0	919
3713	919	0	general description	keyword	1	0	918
3714	910	0	general description	keyword	1	0	128
3715	910	2	general_description	keyword	1	0	911
3716	920	1	general description	keyword	1	0	912
3717	920	3	general_description	keyword	1	0	921
3718	921	1	general description	keyword	1	0	920
3719	921	3	general_description	keyword	1	0	922
3720	921	5	general_description	keyword	1	0	930
3721	922	1	general description	keyword	1	0	921
3722	922	3	general_description	keyword	1	0	923
3723	923	1	general description	keyword	1	0	922
3724	923	3	general_description	keyword	1	0	924
3725	924	1	general description	keyword	1	0	923
3726	924	3	general_description	keyword	1	0	925
3727	925	1	general description	keyword	1	0	924
3728	925	3	general_description	keyword	1	0	926
3729	926	1	general description	keyword	1	0	925
3730	926	3	general_description	keyword	1	0	927
3731	927	1	general description	keyword	1	0	926
3732	927	3	general_description	keyword	1	0	928
3733	928	1	general description	keyword	1	0	927
3734	928	3	general_description	keyword	1	0	929
3735	929	1	general description	keyword	1	0	928
3736	913	0	general description	keyword	1	0	912
3737	913	2	general_description	keyword	1	0	914
3738	914	0	general description	keyword	1	0	913
3739	914	2	general_description	keyword	1	0	915
3740	915	0	general description	keyword	1	0	914
3741	915	2	general_description	keyword	1	0	916
3742	916	0	general description	keyword	1	0	915
3743	916	2	general_description	keyword	1	0	917
3744	917	0	general description	keyword	1	0	916
3745	917	2	general_description	keyword	1	0	918
3746	918	0	general description	keyword	1	0	917
3747	918	2	general_description	keyword	1	0	919
3748	919	0	general description	keyword	1	0	918
3749	910	0	general description	keyword	1	0	128
3750	910	2	general_description	keyword	1	0	911
3751	920	1	general description	keyword	1	0	912
3752	920	3	general_description	keyword	1	0	921
3753	921	1	general description	keyword	1	0	920
3754	921	3	general_description	keyword	1	0	922
3755	921	5	general_description	keyword	1	0	930
3756	922	1	general description	keyword	1	0	921
3757	922	3	general_description	keyword	1	0	923
3758	923	1	general description	keyword	1	0	922
3759	923	3	general_description	keyword	1	0	924
3760	924	1	general description	keyword	1	0	923
3761	924	3	general_description	keyword	1	0	925
3762	925	1	general description	keyword	1	0	924
3763	925	3	general_description	keyword	1	0	926
3764	926	1	general description	keyword	1	0	925
3765	926	3	general_description	keyword	1	0	927
3766	927	1	general description	keyword	1	0	926
3767	927	3	general_description	keyword	1	0	928
3768	928	1	general description	keyword	1	0	927
3769	928	3	general_description	keyword	1	0	929
3770	929	1	general description	keyword	1	0	928
3771	913	0	general description	keyword	1	0	912
3772	913	2	general_description	keyword	1	0	914
3773	914	0	general description	keyword	1	0	913
3774	914	2	general_description	keyword	1	0	915
3775	915	0	general description	keyword	1	0	914
3776	915	2	general_description	keyword	1	0	916
3777	916	0	general description	keyword	1	0	915
3778	916	2	general_description	keyword	1	0	917
3779	917	0	general description	keyword	1	0	916
3780	917	2	general_description	keyword	1	0	918
3781	918	0	general description	keyword	1	0	917
3782	918	2	general_description	keyword	1	0	919
3783	919	0	general description	keyword	1	0	918
3784	910	0	general description	keyword	1	0	128
3785	910	2	general_description	keyword	1	0	911
3786	920	1	general description	keyword	1	0	912
3787	920	3	general_description	keyword	1	0	921
3788	921	1	general description	keyword	1	0	920
3789	921	3	general_description	keyword	1	0	922
3790	921	5	general_description	keyword	1	0	930
3791	922	1	general description	keyword	1	0	921
3792	922	3	general_description	keyword	1	0	923
3793	923	1	general description	keyword	1	0	922
3794	923	3	general_description	keyword	1	0	924
3795	924	1	general description	keyword	1	0	923
3796	924	3	general_description	keyword	1	0	925
3797	925	1	general description	keyword	1	0	924
3798	925	3	general_description	keyword	1	0	926
3799	926	1	general description	keyword	1	0	925
3800	926	3	general_description	keyword	1	0	927
3801	927	1	general description	keyword	1	0	926
3802	927	3	general_description	keyword	1	0	928
3803	928	1	general description	keyword	1	0	927
3804	928	3	general_description	keyword	1	0	929
3805	929	1	general description	keyword	1	0	928
3806	913	0	general description	keyword	1	0	912
3807	913	2	general_description	keyword	1	0	914
3808	914	0	general description	keyword	1	0	913
3809	914	2	general_description	keyword	1	0	915
3810	915	0	general description	keyword	1	0	914
3811	915	2	general_description	keyword	1	0	916
3812	916	0	general description	keyword	1	0	915
3813	916	2	general_description	keyword	1	0	917
3814	917	0	general description	keyword	1	0	916
3815	917	2	general_description	keyword	1	0	918
3816	918	0	general description	keyword	1	0	917
3817	918	2	general_description	keyword	1	0	919
3818	919	0	general description	keyword	1	0	918
3819	910	0	general description	keyword	1	0	128
3820	910	2	general_description	keyword	1	0	911
3821	920	1	general description	keyword	1	0	912
3822	920	3	general_description	keyword	1	0	921
3823	921	1	general description	keyword	1	0	920
3824	921	3	general_description	keyword	1	0	922
3825	921	5	general_description	keyword	1	0	930
3826	922	1	general description	keyword	1	0	921
3827	922	3	general_description	keyword	1	0	923
3828	923	1	general description	keyword	1	0	922
3829	923	3	general_description	keyword	1	0	924
3830	924	1	general description	keyword	1	0	923
3831	924	3	general_description	keyword	1	0	925
3832	925	1	general description	keyword	1	0	924
3833	925	3	general_description	keyword	1	0	926
3834	926	1	general description	keyword	1	0	925
3835	926	3	general_description	keyword	1	0	927
3836	927	1	general description	keyword	1	0	926
3837	927	3	general_description	keyword	1	0	928
3838	928	1	general description	keyword	1	0	927
3839	928	3	general_description	keyword	1	0	929
3840	929	1	general description	keyword	1	0	928
3841	913	0	general description	keyword	1	0	912
3842	913	2	general_description	keyword	1	0	914
3843	914	0	general description	keyword	1	0	913
3844	914	2	general_description	keyword	1	0	915
3845	915	0	general description	keyword	1	0	914
3846	915	2	general_description	keyword	1	0	916
3847	916	0	general description	keyword	1	0	915
3848	916	2	general_description	keyword	1	0	917
3849	917	0	general description	keyword	1	0	916
3850	917	2	general_description	keyword	1	0	918
3851	918	0	general description	keyword	1	0	917
3852	918	2	general_description	keyword	1	0	919
3853	919	0	general description	keyword	1	0	918
3854	910	0	general description	keyword	1	0	128
3855	910	2	general_description	keyword	1	0	911
3856	920	1	general description	keyword	1	0	912
3857	920	3	general_description	keyword	1	0	921
3858	921	1	general description	keyword	1	0	920
3859	921	3	general_description	keyword	1	0	922
3860	921	5	general_description	keyword	1	0	930
3861	922	1	general description	keyword	1	0	921
3862	922	3	general_description	keyword	1	0	923
3863	923	1	general description	keyword	1	0	922
3864	923	3	general_description	keyword	1	0	924
3865	924	1	general description	keyword	1	0	923
3866	924	3	general_description	keyword	1	0	925
3867	925	1	general description	keyword	1	0	924
3868	925	3	general_description	keyword	1	0	926
3869	926	1	general description	keyword	1	0	925
3870	926	3	general_description	keyword	1	0	927
3871	927	1	general description	keyword	1	0	926
3872	927	3	general_description	keyword	1	0	928
3873	928	1	general description	keyword	1	0	927
3874	928	3	general_description	keyword	1	0	929
3875	929	1	general description	keyword	1	0	928
3876	913	0	general description	keyword	1	0	912
3877	913	2	general_description	keyword	1	0	914
3878	914	0	general description	keyword	1	0	913
3879	914	2	general_description	keyword	1	0	915
3880	915	0	general description	keyword	1	0	914
3881	915	2	general_description	keyword	1	0	916
3882	916	0	general description	keyword	1	0	915
3883	916	2	general_description	keyword	1	0	917
3884	917	0	general description	keyword	1	0	916
3885	917	2	general_description	keyword	1	0	918
3886	918	0	general description	keyword	1	0	917
3887	918	2	general_description	keyword	1	0	919
3888	919	0	general description	keyword	1	0	918
3889	910	0	general description	keyword	1	0	128
3890	910	2	general_description	keyword	1	0	911
3891	920	1	general description	keyword	1	0	912
3892	920	3	general_description	keyword	1	0	921
3893	921	1	general description	keyword	1	0	920
3894	921	3	general_description	keyword	1	0	922
3895	921	5	general_description	keyword	1	0	930
3896	922	1	general description	keyword	1	0	921
3897	922	3	general_description	keyword	1	0	923
3898	923	1	general description	keyword	1	0	922
3899	923	3	general_description	keyword	1	0	924
3900	924	1	general description	keyword	1	0	923
3901	924	3	general_description	keyword	1	0	925
3902	925	1	general description	keyword	1	0	924
3903	925	3	general_description	keyword	1	0	926
3904	926	1	general description	keyword	1	0	925
3905	926	3	general_description	keyword	1	0	927
3906	927	1	general description	keyword	1	0	926
3907	927	3	general_description	keyword	1	0	928
3908	928	1	general description	keyword	1	0	927
3909	928	3	general_description	keyword	1	0	929
3910	929	1	general description	keyword	1	0	928
3911	913	0	general description	keyword	1	0	912
3912	913	2	general_description	keyword	1	0	914
3913	914	0	general description	keyword	1	0	913
3914	914	2	general_description	keyword	1	0	915
3915	915	0	general description	keyword	1	0	914
3916	915	2	general_description	keyword	1	0	916
3917	916	0	general description	keyword	1	0	915
3918	916	2	general_description	keyword	1	0	917
3919	917	0	general description	keyword	1	0	916
3920	917	2	general_description	keyword	1	0	918
3921	918	0	general description	keyword	1	0	917
3922	918	2	general_description	keyword	1	0	919
3923	919	0	general description	keyword	1	0	918
3924	910	0	general description	keyword	1	0	128
3925	910	2	general_description	keyword	1	0	911
3926	920	1	general description	keyword	1	0	912
3927	920	3	general_description	keyword	1	0	921
3928	921	1	general description	keyword	1	0	920
3929	921	3	general_description	keyword	1	0	922
3930	921	5	general_description	keyword	1	0	930
3931	922	1	general description	keyword	1	0	921
3932	922	3	general_description	keyword	1	0	923
3933	923	1	general description	keyword	1	0	922
3934	923	3	general_description	keyword	1	0	924
3935	924	1	general description	keyword	1	0	923
3936	924	3	general_description	keyword	1	0	925
3937	925	1	general description	keyword	1	0	924
3938	925	3	general_description	keyword	1	0	926
3939	926	1	general description	keyword	1	0	925
3940	926	3	general_description	keyword	1	0	927
3941	927	1	general description	keyword	1	0	926
3942	927	3	general_description	keyword	1	0	928
3943	928	1	general description	keyword	1	0	927
3944	928	3	general_description	keyword	1	0	929
3945	929	1	general description	keyword	1	0	928
3946	913	0	general description	keyword	1	0	912
3947	913	2	general_description	keyword	1	0	914
3948	914	0	general description	keyword	1	0	913
3949	914	2	general_description	keyword	1	0	915
3950	915	0	general description	keyword	1	0	914
3951	915	2	general_description	keyword	1	0	916
3952	916	0	general description	keyword	1	0	915
3953	916	2	general_description	keyword	1	0	917
3954	917	0	general description	keyword	1	0	916
3955	917	2	general_description	keyword	1	0	918
3956	918	0	general description	keyword	1	0	917
3957	918	2	general_description	keyword	1	0	919
3958	919	0	general description	keyword	1	0	918
3959	910	0	general description	keyword	1	0	128
3960	910	2	general_description	keyword	1	0	911
3961	920	1	general description	keyword	1	0	912
3962	920	3	general_description	keyword	1	0	921
3963	921	1	general description	keyword	1	0	920
3964	921	3	general_description	keyword	1	0	922
3965	921	5	general_description	keyword	1	0	930
3966	922	1	general description	keyword	1	0	921
3967	922	3	general_description	keyword	1	0	923
3968	923	1	general description	keyword	1	0	922
3969	923	3	general_description	keyword	1	0	924
3970	924	1	general description	keyword	1	0	923
3971	924	3	general_description	keyword	1	0	925
3972	925	1	general description	keyword	1	0	924
3973	925	3	general_description	keyword	1	0	926
3974	926	1	general description	keyword	1	0	925
3975	926	3	general_description	keyword	1	0	927
3976	927	1	general description	keyword	1	0	926
3977	927	3	general_description	keyword	1	0	928
3978	928	1	general description	keyword	1	0	927
3979	928	3	general_description	keyword	1	0	929
3980	929	1	general description	keyword	1	0	928
3981	913	0	general description	keyword	1	0	912
3982	913	2	general_description	keyword	1	0	914
3983	914	0	general description	keyword	1	0	913
3984	914	2	general_description	keyword	1	0	915
3985	915	0	general description	keyword	1	0	914
3986	915	2	general_description	keyword	1	0	916
3987	916	0	general description	keyword	1	0	915
3988	916	2	general_description	keyword	1	0	917
3989	917	0	general description	keyword	1	0	916
3990	917	2	general_description	keyword	1	0	918
3991	918	0	general description	keyword	1	0	917
3992	918	2	general_description	keyword	1	0	919
3993	919	0	general description	keyword	1	0	918
3994	910	0	general description	keyword	1	0	128
3995	910	2	general_description	keyword	1	0	911
3996	920	1	general description	keyword	1	0	912
3997	920	3	general_description	keyword	1	0	921
3998	921	1	general description	keyword	1	0	920
3999	921	3	general_description	keyword	1	0	922
4000	921	5	general_description	keyword	1	0	930
4001	922	1	general description	keyword	1	0	921
4002	922	3	general_description	keyword	1	0	923
4003	923	1	general description	keyword	1	0	922
4004	923	3	general_description	keyword	1	0	924
4005	924	1	general description	keyword	1	0	923
4006	924	3	general_description	keyword	1	0	925
4007	925	1	general description	keyword	1	0	924
4008	925	3	general_description	keyword	1	0	926
4009	926	1	general description	keyword	1	0	925
4010	926	3	general_description	keyword	1	0	927
4011	927	1	general description	keyword	1	0	926
4012	927	3	general_description	keyword	1	0	928
4013	928	1	general description	keyword	1	0	927
4014	928	3	general_description	keyword	1	0	929
4015	929	1	general description	keyword	1	0	928
4016	913	0	general description	keyword	1	0	912
4017	913	2	general_description	keyword	1	0	914
4018	914	0	general description	keyword	1	0	913
4019	914	2	general_description	keyword	1	0	915
4020	915	0	general description	keyword	1	0	914
4021	915	2	general_description	keyword	1	0	916
4022	916	0	general description	keyword	1	0	915
4023	916	2	general_description	keyword	1	0	917
4024	917	0	general description	keyword	1	0	916
4025	917	2	general_description	keyword	1	0	918
4026	918	0	general description	keyword	1	0	917
4027	918	2	general_description	keyword	1	0	919
4028	919	0	general description	keyword	1	0	918
4029	910	0	general description	keyword	1	0	128
4030	910	2	general_description	keyword	1	0	911
4031	920	1	general description	keyword	1	0	912
4032	920	3	general_description	keyword	1	0	921
4033	921	1	general description	keyword	1	0	920
4034	921	3	general_description	keyword	1	0	922
4035	921	5	general_description	keyword	1	0	930
4036	922	1	general description	keyword	1	0	921
4037	922	3	general_description	keyword	1	0	923
4038	923	1	general description	keyword	1	0	922
4039	923	3	general_description	keyword	1	0	924
4040	924	1	general description	keyword	1	0	923
4041	924	3	general_description	keyword	1	0	925
4042	925	1	general description	keyword	1	0	924
4043	925	3	general_description	keyword	1	0	926
4044	926	1	general description	keyword	1	0	925
4045	926	3	general_description	keyword	1	0	927
4046	927	1	general description	keyword	1	0	926
4047	927	3	general_description	keyword	1	0	928
4048	928	1	general description	keyword	1	0	927
4049	928	3	general_description	keyword	1	0	929
4050	929	1	general description	keyword	1	0	928
4051	913	0	general description	keyword	1	0	912
4052	913	2	general_description	keyword	1	0	914
4053	914	0	general description	keyword	1	0	913
4054	914	2	general_description	keyword	1	0	915
4055	915	0	general description	keyword	1	0	914
4056	915	2	general_description	keyword	1	0	916
4057	916	0	general description	keyword	1	0	915
4058	916	2	general_description	keyword	1	0	917
4059	917	0	general description	keyword	1	0	916
4060	917	2	general_description	keyword	1	0	918
4061	918	0	general description	keyword	1	0	917
4062	918	2	general_description	keyword	1	0	919
4063	919	0	general description	keyword	1	0	918
4064	910	0	general description	keyword	1	0	128
4065	910	2	general_description	keyword	1	0	911
4066	920	1	general description	keyword	1	0	912
4067	920	3	general_description	keyword	1	0	921
4068	921	1	general description	keyword	1	0	920
4069	921	3	general_description	keyword	1	0	922
4070	921	5	general_description	keyword	1	0	930
4071	922	1	general description	keyword	1	0	921
4072	922	3	general_description	keyword	1	0	923
4073	923	1	general description	keyword	1	0	922
4074	923	3	general_description	keyword	1	0	924
4075	924	1	general description	keyword	1	0	923
4076	924	3	general_description	keyword	1	0	925
4077	925	1	general description	keyword	1	0	924
4078	925	3	general_description	keyword	1	0	926
4079	926	1	general description	keyword	1	0	925
4080	926	3	general_description	keyword	1	0	927
4081	927	1	general description	keyword	1	0	926
4082	927	3	general_description	keyword	1	0	928
4083	928	1	general description	keyword	1	0	927
4084	928	3	general_description	keyword	1	0	929
4085	929	1	general description	keyword	1	0	928
4086	913	0	general description	keyword	1	0	912
4087	913	2	general_description	keyword	1	0	914
4088	914	0	general description	keyword	1	0	913
4089	914	2	general_description	keyword	1	0	915
4090	915	0	general description	keyword	1	0	914
4091	915	2	general_description	keyword	1	0	916
4092	916	0	general description	keyword	1	0	915
4093	916	2	general_description	keyword	1	0	917
4094	917	0	general description	keyword	1	0	916
4095	917	2	general_description	keyword	1	0	918
4096	918	0	general description	keyword	1	0	917
4097	918	2	general_description	keyword	1	0	919
4098	919	0	general description	keyword	1	0	918
4099	910	0	general description	keyword	1	0	128
4100	910	2	general_description	keyword	1	0	911
4101	920	1	general description	keyword	1	0	912
4102	920	3	general_description	keyword	1	0	921
4103	921	1	general description	keyword	1	0	920
4104	921	3	general_description	keyword	1	0	922
4105	921	5	general_description	keyword	1	0	930
4106	922	1	general description	keyword	1	0	921
4107	922	3	general_description	keyword	1	0	923
4108	923	1	general description	keyword	1	0	922
4109	923	3	general_description	keyword	1	0	924
4110	924	1	general description	keyword	1	0	923
4111	924	3	general_description	keyword	1	0	925
4112	925	1	general description	keyword	1	0	924
4113	925	3	general_description	keyword	1	0	926
4114	926	1	general description	keyword	1	0	925
4115	926	3	general_description	keyword	1	0	927
4116	927	1	general description	keyword	1	0	926
4117	927	3	general_description	keyword	1	0	928
4118	928	1	general description	keyword	1	0	927
4119	928	3	general_description	keyword	1	0	929
4120	929	1	general description	keyword	1	0	928
4121	913	0	general description	keyword	1	0	912
4122	913	2	general_description	keyword	1	0	914
4123	914	0	general description	keyword	1	0	913
4124	914	2	general_description	keyword	1	0	915
4125	915	0	general description	keyword	1	0	914
4126	915	2	general_description	keyword	1	0	916
4127	916	0	general description	keyword	1	0	915
4128	916	2	general_description	keyword	1	0	917
4129	917	0	general description	keyword	1	0	916
4130	917	2	general_description	keyword	1	0	918
4131	918	0	general description	keyword	1	0	917
4132	918	2	general_description	keyword	1	0	919
4133	919	0	general description	keyword	1	0	918
4134	910	0	general description	keyword	1	0	128
4135	910	2	general_description	keyword	1	0	911
4136	920	1	general description	keyword	1	0	912
4137	920	3	general_description	keyword	1	0	921
4138	921	1	general description	keyword	1	0	920
4139	921	3	general_description	keyword	1	0	922
4140	921	5	general_description	keyword	1	0	930
4141	922	1	general description	keyword	1	0	921
4142	922	3	general_description	keyword	1	0	923
4143	923	1	general description	keyword	1	0	922
4144	923	3	general_description	keyword	1	0	924
4145	924	1	general description	keyword	1	0	923
4146	924	3	general_description	keyword	1	0	925
4147	925	1	general description	keyword	1	0	924
4148	925	3	general_description	keyword	1	0	926
4149	926	1	general description	keyword	1	0	925
4150	926	3	general_description	keyword	1	0	927
4151	927	1	general description	keyword	1	0	926
4152	927	3	general_description	keyword	1	0	928
4153	928	1	general description	keyword	1	0	927
4154	928	3	general_description	keyword	1	0	929
4155	929	1	general description	keyword	1	0	928
4156	913	0	general description	keyword	1	0	912
4157	913	2	general_description	keyword	1	0	914
4158	914	0	general description	keyword	1	0	913
4159	914	2	general_description	keyword	1	0	915
4160	915	0	general description	keyword	1	0	914
4161	915	2	general_description	keyword	1	0	916
4162	916	0	general description	keyword	1	0	915
4163	916	2	general_description	keyword	1	0	917
4164	917	0	general description	keyword	1	0	916
4165	917	2	general_description	keyword	1	0	918
4166	918	0	general description	keyword	1	0	917
4167	918	2	general_description	keyword	1	0	919
4168	919	0	general description	keyword	1	0	918
4169	910	0	general description	keyword	1	0	128
4170	910	2	general_description	keyword	1	0	911
4171	920	1	general description	keyword	1	0	912
4172	920	3	general_description	keyword	1	0	921
4173	921	1	general description	keyword	1	0	920
4174	921	3	general_description	keyword	1	0	922
4175	921	5	general_description	keyword	1	0	930
4176	922	1	general description	keyword	1	0	921
4177	922	3	general_description	keyword	1	0	923
4178	923	1	general description	keyword	1	0	922
4179	923	3	general_description	keyword	1	0	924
4180	924	1	general description	keyword	1	0	923
4181	924	3	general_description	keyword	1	0	925
4182	925	1	general description	keyword	1	0	924
4183	925	3	general_description	keyword	1	0	926
4184	926	1	general description	keyword	1	0	925
4185	926	3	general_description	keyword	1	0	927
4186	927	1	general description	keyword	1	0	926
4187	927	3	general_description	keyword	1	0	928
4188	928	1	general description	keyword	1	0	927
4189	928	3	general_description	keyword	1	0	929
4190	929	1	general description	keyword	1	0	928
4191	913	0	general description	keyword	1	0	912
4192	913	2	general_description	keyword	1	0	914
4193	914	0	general description	keyword	1	0	913
4194	914	2	general_description	keyword	1	0	915
4195	915	0	general description	keyword	1	0	914
4196	915	2	general_description	keyword	1	0	916
4197	916	0	general description	keyword	1	0	915
4198	916	2	general_description	keyword	1	0	917
4199	917	0	general description	keyword	1	0	916
4200	917	2	general_description	keyword	1	0	918
4201	918	0	general description	keyword	1	0	917
4202	918	2	general_description	keyword	1	0	919
4203	919	0	general description	keyword	1	0	918
4204	910	0	general description	keyword	1	0	128
4205	910	2	general_description	keyword	1	0	911
4206	935	0	general description	keyword	1	0	934
4207	935	2	general_description	keyword	1	0	936
4208	936	0	general description	keyword	1	0	935
4209	936	2	general_description	keyword	1	0	937
4210	937	0	general description	keyword	1	0	936
4211	937	2	general_description	keyword	1	0	938
4212	938	0	general description	keyword	1	0	937
4213	938	2	general_description	keyword	1	0	939
4214	939	0	general description	keyword	1	0	938
4215	939	3	general_description	keyword	1	0	940
4216	935	0	general description	keyword	1	0	934
4217	935	2	general_description	keyword	1	0	936
4218	936	0	general description	keyword	1	0	935
4219	936	2	general_description	keyword	1	0	937
4220	937	0	general description	keyword	1	0	936
4221	937	2	general_description	keyword	1	0	938
4222	938	0	general description	keyword	1	0	937
4223	938	2	general_description	keyword	1	0	939
4224	939	0	general description	keyword	1	0	938
4225	939	3	general_description	keyword	1	0	940
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
27	26
\.


--
-- Data for Name: scripted_steps; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.scripted_steps (id, s_sequence_vnum, s_wait_ticks, s_mob, s_obj, s_room, s_quantity, s_order, s_interpret, s_yaml, s_type, s_dialogue) FROM stdin;
99	26	9	600	0	143	0	0	\N	\N	act	$n looks up from a messy desk full of detailed readouts..
100	26	9	600	0	143	0	1	\N	\N	dialogue	The scanner? Give it here...
101	26	3	600	0	143	0	2	\N	\N	act	$n attaches a long orange cable to the bottom of the biometric scanner
102	26	3	600	0	143	0	3	\N	\N	dialogue	Yes... these are definitely human remains. Though I do see an advanced type of mutation occurring. It's almost as if this sample were taken off of a living breathing human. I don't understand how this sort of thing could be possible. It defies all scientific explanation.
103	26	3	600	0	143	0	4	\N	\N	act	$n types a few commands into a computer terminal.
104	26	3	600	0	143	0	5	\N	\N	dialogue	I will get these results to my international colleagues for further study. I will give Corporal Crawford the results of my studies. That is all for now...
105	26	3	600	0	143	0	6	\N	\N	act	$n rubs his temples in an effort to quell a severe migraine...
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
332	1	wpn-assault-rifles	0
333	1	wpn-sniper-rifles	0
334	1	wpn-sub-machine-guns	0
336	1	wpn-pistols	0
337	1	wpn-machine-pistols	0
338	1	wpn-light-machine-guns	0
339	1	cqc-jab-to-head	0
340	1	cqc-jab-to-body	0
341	1	cqc-cross-to-head	0
342	1	cqc-cross-to-body	0
343	1	cqc-left-hook-to-head	0
344	1	cqc-right-hook-to-head	0
345	1	cqc-left-hook-to-body	0
346	1	cqc-right-hook-to-body	0
347	1	cqc-left-uppercut	0
348	1	cqc-right-uppercut	0
349	1	cqc-left-elbow	0
350	1	cqc-right-elbow	0
351	1	cqc-right-upward-elbow	0
352	1	cqc-left-upward-elbow	0
353	1	cqc-right-oblique	0
354	1	cqc-left-oblique	0
355	1	cqc-left-teep	0
356	1	cqc-right-teep	0
357	1	cqc-left-front-kick	0
358	1	cqc-right-front-kick	0
359	1	cqc-left-knee-to-head	0
360	1	cqc-right-knee-to-head	0
361	1	cqc-left-knee-to-body	0
362	1	cqc-right-knee-to-body	0
363	1	cqc-knife-disarm	0
364	1	cqc-pistol-disarm	0
365	1	cqc-right-leg-kick	0
366	1	cqc-left-leg-kick	0
367	1	cqc-right-kick-to-head	0
368	1	cqc-left-kick-to-head	0
369	1	cqc-right-stomp-to-head	0
370	1	cqc-left-stomp-to-head	0
371	1	wpn-ar	0
373	1	wpn-smg	0
374	1	wpn-mp	0
375	1	wpn-lmg	0
376	111	ads	0
377	111	stealth	0
378	111	summon	0
379	111	xray	0
380	111	feign	0
381	111	claymore	0
382	111	intimidation	0
383	111	cryo	0
384	111	flash	0
385	111	ts	0
386	111	lb	0
387	111	suture	0
388	111	as	0
389	111	sensor	0
390	111	ubs	0
391	111	ubf	0
392	111	gm	0
393	111	limb	0
394	111	smine	0
395	111	cmine	0
396	111	recon	0
397	111	wpn-ar	0
401	111	wpn-pistols	8
399	111	wpn-smg	0
402	111	wpn-mp	0
404	111	cqc-jab-to-head	0
405	111	cqc-jab-to-body	0
406	111	cqc-cross-to-head	0
407	111	cqc-cross-to-body	0
408	111	cqc-left-hook-to-head	0
409	111	cqc-right-hook-to-head	0
410	111	cqc-left-hook-to-body	0
411	111	cqc-right-hook-to-body	0
412	111	cqc-left-uppercut	0
413	111	cqc-right-uppercut	0
414	111	cqc-left-elbow	0
415	111	cqc-right-elbow	0
416	111	cqc-right-upward-elbow	0
417	111	cqc-left-upward-elbow	0
418	111	cqc-right-oblique	0
419	111	cqc-left-oblique	0
420	111	cqc-left-teep	0
421	111	cqc-right-teep	0
422	111	cqc-left-front-kick	0
423	111	cqc-right-front-kick	0
424	111	cqc-left-knee-to-head	0
403	111	wpn-lmg	19
372	1	wpn-sniper	44
400	111	wpn-shotguns	2
425	111	cqc-right-knee-to-head	0
426	111	cqc-left-knee-to-body	0
427	111	cqc-right-knee-to-body	0
428	111	cqc-knife-disarm	0
429	111	cqc-pistol-disarm	0
430	111	cqc-right-leg-kick	0
431	111	cqc-left-leg-kick	0
432	111	cqc-right-kick-to-head	0
433	111	cqc-left-kick-to-head	0
434	111	cqc-right-stomp-to-head	0
435	111	cqc-left-stomp-to-head	0
398	111	wpn-sniper	52
335	1	wpn-shotguns	73
\.


--
-- Data for Name: stay; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.stay (id, s_room_vnum, s_username, created_at) FROM stdin;
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
-- Data for Name: user_logins; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.user_logins (id, u_ip_address, u_username) FROM stdin;
1	127.0.0.1	far
2	127.0.0.1	ghost
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
5	128	128	2022-02-01 01:40:03.476734	t	0	0
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
971	1	Y	0	0	402	1	#yaml|vehicle/p3-hunchbak.yml
972	1	Y	0	0	400	1	#yaml|vehicle/p3-offroad-mx3.yml
973	1	Y	0	0	399	1	#yaml|vehicle/prime-town-suv.yml
974	1	Y	0	0	395	1	#yaml|vehicle/lxr-sunrise.yml
975	1	Y	0	0	393	1	#yaml|vehicle/lxr-sport.yml
976	1	Y	0	0	394	1	#yaml|vehicle/lxr-sport.yml
1022	1	M	0	106	729	1	\N
1023	1	M	0	106	730	1	\N
1024	1	M	0	106	731	1	\N
1025	1	M	0	106	732	1	\N
1026	1	M	0	107	729	1	\N
1027	1	M	0	107	730	1	\N
1028	1	M	0	107	731	1	\N
1029	1	M	0	107	732	1	\N
1151	1	M	0	669	910	6	\N
1152	1	M	0	114	697	10	\N
1153	1	M	0	114	698	10	\N
1154	1	M	0	114	689	10	\N
1155	1	M	0	114	696	10	\N
1156	1	M	0	114	151	10	\N
1157	1	M	0	115	697	10	\N
1158	1	M	0	115	698	10	\N
1159	1	M	0	115	689	10	\N
1160	1	M	0	115	696	10	\N
1161	1	M	0	115	151	10	\N
1167	1	M	0	670	935	10	\N
1168	1	M	0	670	936	10	\N
1169	1	M	0	670	937	10	\N
1170	1	M	0	670	938	10	\N
1171	1	M	0	670	939	10	\N
1079	1	Y	0	0	889	1	#yaml|vehicle/p3-hunchbak.yml
1080	1	Y	0	0	890	1	#yaml|vehicle/p3-offroad-mx3.yml
1081	1	Y	0	0	891	1	#yaml|vehicle/prime-town-suv.yml
1082	1	Y	0	0	892	1	#yaml|vehicle/lxr-sunrise.yml
1083	1	Y	0	0	893	1	#yaml|vehicle/lxr-sport.yml
1086	1	M	0	666	682	1	\N
1087	1	M	0	503	874	1	\N
1088	1	M	0	503	876	1	\N
1089	1	M	0	504	870	2	\N
1090	1	M	0	504	882	2	\N
1091	1	M	0	505	894	1	\N
1092	1	M	0	505	896	1	\N
1093	1	M	0	505	0	1	\N
1094	1	M	0	505	0	1	\N
1095	1	M	0	112	909	1	\N
1096	1	M	0	113	909	3	\N
1514	1	M	0	103	407	1	\N
1515	1	M	0	103	409	1	\N
1516	1	M	0	103	413	1	\N
1517	1	M	0	103	282	1	\N
1518	1	M	0	103	284	1	\N
1519	1	M	0	103	286	1	\N
1520	1	M	0	103	292	1	\N
1607	1	M	0	707	348	1	\N
1608	1	M	0	707	349	1	\N
1609	1	M	0	707	350	1	\N
1610	1	M	0	707	353	1	\N
1611	1	M	0	707	354	1	\N
1612	1	M	0	707	357	1	\N
1613	1	M	0	708	348	1	\N
1614	1	M	0	708	349	1	\N
1615	1	M	0	708	350	1	\N
1616	1	M	0	708	353	1	\N
1617	1	M	0	708	354	1	\N
1618	1	M	0	708	357	1	\N
1521	1	M	0	103	293	1	\N
1543	1	M	0	101	316	5	\N
1544	1	M	0	101	317	5	\N
1545	1	M	0	101	322	5	\N
1546	1	M	0	101	323	5	\N
1547	1	M	0	101	324	5	\N
1548	1	M	0	101	325	5	\N
1549	1	M	0	101	306	5	\N
1550	1	M	0	101	308	5	\N
1551	1	M	0	104	589	5	\N
1552	1	M	0	104	592	5	\N
1553	1	M	0	104	600	5	\N
1554	1	M	0	104	603	5	\N
1555	1	M	0	104	611	5	\N
1556	1	M	0	104	614	5	\N
1557	1	M	0	104	622	5	\N
1558	1	M	0	104	625	5	\N
1559	1	M	0	104	633	5	\N
1560	1	M	0	102	290	2	\N
1561	1	M	0	102	291	2	\N
1562	1	M	0	102	292	2	\N
1563	1	M	0	102	294	2	\N
1564	1	M	0	102	295	2	\N
1565	1	M	0	102	331	2	\N
1566	1	M	0	102	298	2	\N
1567	1	M	0	102	303	2	\N
1568	1	M	0	102	310	2	\N
1569	1	M	0	102	317	2	\N
1570	1	M	0	102	324	2	\N
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
-- Name: banned_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.banned_id_seq', 1, false);


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

SELECT pg_catalog.setval('public.class_ghost_ghost_id_seq', 13, true);


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

SELECT pg_catalog.setval('public.contract_step_callback_id_seq', 15, true);


--
-- Name: contract_steps_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.contract_steps_id_seq', 45, true);


--
-- Name: contracts_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.contracts_id_seq', 18, true);


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
-- Name: frozen_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.frozen_id_seq', 1, false);


--
-- Name: help_pages_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.help_pages_id_seq', 1434, true);


--
-- Name: help_topics_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.help_topics_id_seq', 149, true);


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

SELECT pg_catalog.setval('public.mob_equipment_id_seq', 305, true);


--
-- Name: mob_equipment_map_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.mob_equipment_map_id_seq', 68, true);


--
-- Name: mob_roam_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.mob_roam_id_seq', 781, true);


--
-- Name: mob_zone_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.mob_zone_id_seq', 1, false);


--
-- Name: mobile_mob_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.mobile_mob_id_seq', 116, true);


--
-- Name: muted_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.muted_id_seq', 1, false);


--
-- Name: notch_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.notch_id_seq', 5, true);


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

SELECT pg_catalog.setval('public.player_base_ability_pba_id_seq', 17, true);


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

SELECT pg_catalog.setval('public.player_id_seq', 111, true);


--
-- Name: player_object_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.player_object_id_seq', 492, true);


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
-- Name: raid_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.raid_id_seq', 14, true);


--
-- Name: rifle_attachment_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.rifle_attachment_id_seq', 4980, true);


--
-- Name: rifle_attribute_limits_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.rifle_attribute_limits_id_seq', 1, false);


--
-- Name: rifle_index_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.rifle_index_id_seq', 29, true);


--
-- Name: rifle_instance_rifle_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.rifle_instance_rifle_id_seq', 215, true);


--
-- Name: rifle_placements_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.rifle_placements_id_seq', 1, false);


--
-- Name: room_direction_data_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.room_direction_data_id_seq', 4225, true);


--
-- Name: room_extra_descriptions_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.room_extra_descriptions_id_seq', 1, false);


--
-- Name: room_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.room_id_seq', 828, true);


--
-- Name: room_virtual_number_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.room_virtual_number_seq', 1, false);


--
-- Name: scripted_sequences_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.scripted_sequences_id_seq', 27, true);


--
-- Name: scripted_steps_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.scripted_steps_id_seq', 105, true);


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

SELECT pg_catalog.setval('public.skill_usage_id_seq', 435, true);


--
-- Name: stay_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.stay_id_seq', 1, false);


--
-- Name: terminal_choices_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.terminal_choices_id_seq', 6, true);


--
-- Name: user_logins_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.user_logins_id_seq', 2, true);


--
-- Name: weapon_locker_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.weapon_locker_id_seq', 1, false);


--
-- Name: world_configuration_start_rooms_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.world_configuration_start_rooms_id_seq', 5, true);


--
-- Name: zone_data_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.zone_data_id_seq', 1618, true);


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
-- Name: banned banned_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.banned
    ADD CONSTRAINT banned_pkey PRIMARY KEY (id);


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
-- Name: frozen frozen_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.frozen
    ADD CONSTRAINT frozen_pkey PRIMARY KEY (id);


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
-- Name: muted muted_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.muted
    ADD CONSTRAINT muted_pkey PRIMARY KEY (id);


--
-- Name: notch notch_primary_key; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.notch
    ADD CONSTRAINT notch_primary_key PRIMARY KEY (id);


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
-- Name: raid raid_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.raid
    ADD CONSTRAINT raid_pkey PRIMARY KEY (id);


--
-- Name: rifle_attachment rifle_attachment_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.rifle_attachment
    ADD CONSTRAINT rifle_attachment_pkey PRIMARY KEY (id);


--
-- Name: rifle_attribute_limits rifle_attribute_limits_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.rifle_attribute_limits
    ADD CONSTRAINT rifle_attribute_limits_pkey PRIMARY KEY (id);


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
-- Name: stay stay_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.stay
    ADD CONSTRAINT stay_pkey PRIMARY KEY (id);


--
-- Name: stay stay_s_username_key; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.stay
    ADD CONSTRAINT stay_s_username_key UNIQUE (s_username);


--
-- Name: terminal_choices terminal_choices_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.terminal_choices
    ADD CONSTRAINT terminal_choices_pkey PRIMARY KEY (id);


--
-- Name: user_logins user_logins_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.user_logins
    ADD CONSTRAINT user_logins_pkey PRIMARY KEY (id);


--
-- Name: user_logins user_logins_u_ip_address_u_username_key; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.user_logins
    ADD CONSTRAINT user_logins_u_ip_address_u_username_key UNIQUE (u_ip_address, u_username);


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
-- Name: mobile fk_mob_raid_id; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.mobile
    ADD CONSTRAINT fk_mob_raid_id FOREIGN KEY (mob_raid_id) REFERENCES public.raid(id) ON UPDATE CASCADE ON DELETE CASCADE;


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
-- Name: notch fk_player_id; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.notch
    ADD CONSTRAINT fk_player_id FOREIGN KEY (n_player_id) REFERENCES public.player(id) ON UPDATE CASCADE ON DELETE CASCADE;


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

CREATE TABLE public.radio_station (
    id SERIAL PRIMARY KEY,
    rs_room_vnum integer NOT NULL,
		rs_power integer NOT NULL,
		rs_name varchar(256) NOT NULL
);

ALTER TABLE ONLY public.radio_station
    ADD CONSTRAINT radio_station_room_vnum_fkey FOREIGN KEY (rs_room_vnum) REFERENCES public.room(room_number) ON UPDATE CASCADE ON DELETE CASCADE;

--
-- PostgreSQL database dump complete
--


CREATE TABLE public.zone_mob (
    id SERIAL PRIMARY KEY,
    zm_room_vnum integer NOT NULL,
    zm_mob_vnum integer NOT NULL,
		zm_max integer NOT NULL DEFAULT 5
);
ALTER TABLE ONLY public.zone_mob
    ADD CONSTRAINT zone_mob_room_vnum_fkey FOREIGN KEY (zm_room_vnum) REFERENCES public.room(room_number) ON UPDATE CASCADE ON DELETE CASCADE;
ALTER TABLE ONLY public.zone_mob
    ADD CONSTRAINT zone_mob_mob_vnum_fkey FOREIGN KEY (zm_mob_vnum) REFERENCES public.mobile(mob_virtual_number) ON UPDATE CASCADE ON DELETE CASCADE;



CREATE TABLE public.zone_yaml (
    id SERIAL PRIMARY KEY,
    zy_room_vnum integer NOT NULL,
		zy_yaml TEXT NOT NULL,
		zy_max integer NOT NULL DEFAULT 1
);
ALTER TABLE ONLY public.zone_yaml
    ADD CONSTRAINT zone_yaml_room_vnum_fkey FOREIGN KEY (zy_room_vnum) REFERENCES public.room(room_number) ON UPDATE CASCADE ON DELETE CASCADE;



CREATE TABLE public.zone_random_item (
    id SERIAL PRIMARY KEY,
    zri_room_vnum integer NOT NULL,
		zri_yaml VARCHAR(256) NOT NULL,
		zri_max integer NOT NULL DEFAULT 1
);

ALTER TABLE ONLY public.zone_random_item
    ADD CONSTRAINT zone_random_item_room_vnum_fkey FOREIGN KEY (zri_room_vnum) REFERENCES public.room(room_number) ON UPDATE CASCADE ON DELETE CASCADE;


INSERT INTO public.zone_yaml (zy_room_vnum, zy_max, zy_yaml) ( select zone_arg2, zone_arg3, zone_yaml from zone_data where zone_command='Y');
insert into public.zone_mob (zm_mob_vnum, zm_room_vnum, zm_max)  (SELECT zone_arg1, zone_arg2, zone_arg3 FROM public.zone_data where zone_command='M' AND zone_arg2 != 0);
