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
    meq_profile_name character varying(32),
    meq_vnum integer NOT NULL,
    meq_light character varying(32),
    meq_finger_r character varying(32),
    meq_finger_l character varying(32),
    meq_neck_1 character varying(32),
    meq_neck_2 character varying(32),
    meq_body character varying(32),
    meq_head character varying(32),
    meq_legs character varying(32),
    meq_feet character varying(32),
    meq_hands character varying(32),
    meq_arms character varying(32),
    meq_shield character varying(32),
    meq_about character varying(32),
    meq_waist character varying(32),
    meq_wrist_r character varying(32),
    meq_wrist_l character varying(32),
    meq_wield character varying(32),
    meq_hold character varying(32),
    meq_secondary character varying(32),
    meq_shoulders_l character varying(32),
    meq_shoulders_r character varying(32),
    meq_backpack character varying(32),
    meq_goggles character varying(32),
    meq_vest_pack character varying(32),
    meq_elbow_l character varying(32),
    meq_elbow_r character varying(32),
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
    player_preferences character varying(2048)
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
    rifle_str_type character varying(16),
    rifle_type character varying(16),
    rifle_manufacturer character varying(32),
    rifle_name character varying(32),
    rifle_vnum integer,
    rifle_ammo_max integer,
    rifle_ammo_type character varying(16) DEFAULT 'SNIPER'::character varying NOT NULL,
    rifle_chance_to_injure double precision,
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
    room_flag integer NOT NULL
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
-- Name: armor_index id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.armor_index ALTER COLUMN id SET DEFAULT nextval('public.armor_index_id_seq'::regclass);


--
-- Name: camera_feed feed_id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.camera_feed ALTER COLUMN feed_id SET DEFAULT nextval('public.camera_feed_feed_id_seq'::regclass);


--
-- Name: class_breacher breacher_id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.class_breacher ALTER COLUMN breacher_id SET DEFAULT nextval('public.class_breacher_breacher_id_seq'::regclass);


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
-- Name: contract_steps id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.contract_steps ALTER COLUMN id SET DEFAULT nextval('public.contract_steps_id_seq'::regclass);


--
-- Name: contracts id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.contracts ALTER COLUMN id SET DEFAULT nextval('public.contracts_id_seq'::regclass);


--
-- Name: extra_description id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.extra_description ALTER COLUMN id SET DEFAULT nextval('public.extra_description_id_seq'::regclass);


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
-- Data for Name: armor_index; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.armor_index (id, armor_filename, armor_type, created_at, updated_at) FROM stdin;
1	baklava.yml	head	2021-04-02 05:25:19.429806	2021-04-02 05:25:19.429806
2	tactical-gas-mask.yml	head	2021-04-02 05:25:19.430309	2021-04-02 05:25:19.430309
3	thermal-goggles.yml	goggles	2021-04-02 05:25:19.430714	2021-04-02 05:25:19.430714
4	night-vision-goggles.yml	goggles	2021-04-02 05:25:19.431082	2021-04-02 05:25:19.431082
5	titan-shoulder-pads.yml	shoulders	2021-04-02 05:25:19.431465	2021-04-02 05:25:19.431465
6	xm-scorpio-shoulder-pads.yml	shoulders	2021-04-02 05:25:19.431835	2021-04-02 05:25:19.431835
7	viper-shoulder-pads.yml	shoulders	2021-04-02 05:25:19.432225	2021-04-02 05:25:19.432225
8	falcon-shoulder-pads.yml	shoulders	2021-04-02 05:25:19.432586	2021-04-02 05:25:19.432586
9	basic-ballistic-vest.yml	vest	2021-04-02 05:25:19.432957	2021-04-02 05:25:19.432957
10	falcon-ballistic-vest.yml	vest	2021-04-02 05:25:19.433348	2021-04-02 05:25:19.433348
11	raven-ballistic-vest.yml	vest	2021-04-02 05:25:19.433727	2021-04-02 05:25:19.433727
12	vulture-ballistic-vest.yml	vest	2021-04-02 05:25:19.434094	2021-04-02 05:25:19.434094
13	xm-scorpio-slotted-vest-pack.yml	vestpack	2021-04-02 05:25:19.434432	2021-04-02 05:25:19.434432
14	raven-ultralight-backpack.yml	backpack	2021-04-02 05:25:19.434789	2021-04-02 05:25:19.434789
15	titan-gauntlets.yml	arms	2021-04-02 05:25:19.435131	2021-04-02 05:25:19.435131
16	titan-elbow-guards.yml	elbow	2021-04-02 05:25:19.43547	2021-04-02 05:25:19.43547
17	razor-gps-wrist-watch.yml	wrist	2021-04-02 05:25:19.435811	2021-04-02 05:25:19.435811
18	xm-scorpio-tactical-gloves.yml	hands	2021-04-02 05:25:19.436208	2021-04-02 05:25:19.436208
19	xm-scorpio-belt.yml	waist	2021-04-02 05:25:19.436579	2021-04-02 05:25:19.436579
20	titan-shin-guards.yml	legs	2021-04-02 05:25:19.436942	2021-04-02 05:25:19.436942
21	viper-leg-guards.yml	legs	2021-04-02 05:25:19.437314	2021-04-02 05:25:19.437314
22	basic-boots.yml	feet	2021-04-02 05:25:19.437675	2021-04-02 05:25:19.437675
23	xm50-ultralight-boots.yml	feet	2021-04-02 05:25:19.438028	2021-04-02 05:25:19.438028
24	xm607-vulture-boots.yml	feet	2021-04-02 05:25:19.438399	2021-04-02 05:25:19.438399
25	xm8-panama-combat-boots.yml	feet	2021-04-02 05:25:19.438773	2021-04-02 05:25:19.438773
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
-- Data for Name: class_engineer; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.class_engineer (engineer_id, engineer_player_id, created_at, updated_at) FROM stdin;
\.


--
-- Data for Name: class_ghost; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.class_ghost (ghost_id, ghost_player_id, created_at, updated_at) FROM stdin;
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
1	1	2021-04-02 05:25:19.411452	2021-04-02 05:25:19.411452
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
-- Data for Name: contract_steps; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.contract_steps (id, s_contract_vnum, s_task_type, s_task_target, s_description, s_object_yaml, s_mob_vnum, s_room_vnum, s_quota, s_is_optional, s_order, s_reward_xp, s_reward_money, s_reward_1, s_reward_2, s_reward_3, s_reward_4, s_reward_5, s_reward_6, s_reward_7, s_reward_8, s_reward_9, s_reward_10, created_at, updated_at) FROM stdin;
\.


--
-- Data for Name: contracts; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.contracts (id, c_vnum, c_description, c_title, created_at, updated_at) FROM stdin;
\.


--
-- Data for Name: extra_description; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.extra_description (id, obj_fk_id, extra_keyword, extra_description) FROM stdin;
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
\.


--
-- Data for Name: mob_equipment_map; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.mob_equipment_map (id, mmap_mob_vnum, mmap_mob_equipment_vnum, created_at, updated_at) FROM stdin;
\.


--
-- Data for Name: mob_roam; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.mob_roam (id, mob_virtual_number, room_virtual_number, profile_name) FROM stdin;
\.


--
-- Data for Name: mob_zone; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.mob_zone (id, zone_virtual_number, mob_virtual_number, room_virtual_number, max_existing) FROM stdin;
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

COPY public.object_rifle (rifle_id, rifle_accuracy_map_0, rifle_accuracy_map_1, rifle_accuracy_map_2, rifle_accuracy_map_3, rifle_damage_map_0, rifle_damage_map_1, rifle_damage_map_2, rifle_damage_map_3, rifle_ammo_max, rifle_ammo_type, rifle_chance_to_injure, rifle_clip_size, rifle_cooldown_between_shots, rifle_critical_chance, rifle_critical_range, rifle_damage_per_second, rifle_disorient_amount, rifle_headshot_bonus, rifle_max_range, rifle_range_multiplier, rifle_reload_time, rifle_rounds_per_minute, rifle_muzzle_velocity, rifle_effective_firing_range, rifle_str_type, rifle_type, rifle_manufacturer, rifle_name, rifle_vnum, rifle_rarity, rifle_file, rifle_incendiary_damage, rifle_explosive_damage, rifle_shrapnel_damage, rifle_corrosive_damage, rifle_cryogenic_damage, rifle_radioactive_damage, rifle_emp_damage, rifle_shock_damage, rifle_anti_matter_damage, rifle_stat_strength, rifle_stat_intelligence, rifle_stat_wisdom, rifle_stat_dexterity, rifle_stat_constitution, rifle_stat_electronics, rifle_stat_armor, rifle_stat_marksmanship, rifle_stat_sniping, rifle_stat_demolitions, rifle_stat_chemistry, rifle_stat_weapon_handling, rifle_stat_strategy, rifle_stat_medical, created_at, updated_at) FROM stdin;
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
1	foKntnEF3KSXA	0	0	far	0	0	0	6500	6500	6500	6500	6500	6500	6500	6500	6500	6500	6500	6500	6500	6500	M	6500	6500	6500	6500	0	0	1	0	0	6500	6500	PC	0	1	6500	6500	2019-03-20 22:38:47.454111	0	2019-03-20 22:38:47.454111	14680304
\.


--
-- Data for Name: player_base_ability; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.player_base_ability (pba_id, pba_player_id, pba_str, pba_str_add, pba_intel, pba_wis, pba_dex, pba_con, pba_cha, pba_electronics, pba_armor, pba_marksmanship, pba_sniping, pba_demolitions, pba_chemistry, pba_weapon_handling, pba_strategy, pba_medical, created_at, updated_at) FROM stdin;
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

COPY public.player_object (po_id, po_player_id, po_type, po_type_vnum, po_type_id, po_type_load, po_wear_position, po_in_inventory, created_at, updated_at) FROM stdin;
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
1	1	0	1	2021-04-02 05:26:08.23724	2021-04-02 05:26:08.23724
2	2	0	1	2021-04-02 05:26:08.238229	2021-04-02 05:26:08.238229
3	3	0	1	2021-04-02 05:26:08.238988	2021-04-02 05:26:08.238988
4	4	0	1	2021-04-02 05:26:08.239704	2021-04-02 05:26:08.239704
5	5	0	1	2021-04-02 05:26:08.24043	2021-04-02 05:26:08.24043
6	6	0	1	2021-04-02 05:26:08.241041	2021-04-02 05:26:08.241041
7	7	0	1	2021-04-02 05:26:08.241647	2021-04-02 05:26:08.241647
8	8	0	1	2021-04-02 05:26:08.242211	2021-04-02 05:26:08.242211
9	9	0	1	2021-04-02 05:26:08.242734	2021-04-02 05:26:08.242734
10	10	0	1	2021-04-02 05:26:08.243272	2021-04-02 05:26:08.243272
11	11	0	1	2021-04-02 05:26:08.243805	2021-04-02 05:26:08.243805
12	12	0	1	2021-04-02 05:26:08.244384	2021-04-02 05:26:08.244384
13	13	0	1	2021-04-02 05:26:08.244928	2021-04-02 05:26:08.244928
14	14	0	1	2021-04-02 05:26:08.245472	2021-04-02 05:26:08.245472
15	15	0	1	2021-04-02 05:26:08.246017	2021-04-02 05:26:08.246017
16	16	0	1	2021-04-02 05:26:08.246544	2021-04-02 05:26:08.246544
\.


--
-- Data for Name: player_skill_usage; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.player_skill_usage (id, ps_player_id, ps_skill_id, ps_usage_count, created_at, updated_at) FROM stdin;
97	1	1	1	2021-04-02 05:36:44.992995	2021-04-02 05:36:44.992995
98	1	2	1	2021-04-02 05:36:44.996208	2021-04-02 05:36:44.996208
99	1	3	1	2021-04-02 05:36:44.99863	2021-04-02 05:36:44.99863
100	1	4	1	2021-04-02 05:36:45.000751	2021-04-02 05:36:45.000751
101	1	5	1	2021-04-02 05:36:45.002769	2021-04-02 05:36:45.002769
102	1	6	1	2021-04-02 05:36:45.004494	2021-04-02 05:36:45.004494
103	1	7	1	2021-04-02 05:36:45.006287	2021-04-02 05:36:45.006287
104	1	8	1	2021-04-02 05:36:45.007868	2021-04-02 05:36:45.007868
105	1	9	1	2021-04-02 05:36:45.009333	2021-04-02 05:36:45.009333
106	1	10	1	2021-04-02 05:36:45.010734	2021-04-02 05:36:45.010734
107	1	11	1	2021-04-02 05:36:45.012129	2021-04-02 05:36:45.012129
108	1	12	1	2021-04-02 05:36:45.013382	2021-04-02 05:36:45.013382
109	1	13	1	2021-04-02 05:36:45.014553	2021-04-02 05:36:45.014553
110	1	14	1	2021-04-02 05:36:45.01552	2021-04-02 05:36:45.01552
111	1	15	1	2021-04-02 05:36:45.016401	2021-04-02 05:36:45.016401
112	1	16	1	2021-04-02 05:36:45.017321	2021-04-02 05:36:45.017321
\.


--
-- Data for Name: rifle_attachment; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.rifle_attachment (id, rifle_player_id, rifle_data, rifle_position, created_at, updated_at) FROM stdin;
\.


--
-- Data for Name: rifle_index; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.rifle_index (id, rifle_filename, rifle_type, created_at, updated_at) FROM stdin;
1	augpara.yml	smg	2021-04-02 05:25:19.415724	2021-04-02 05:25:19.415724
2	fmg9.yml	smg	2021-04-02 05:25:19.416273	2021-04-02 05:25:19.416273
3	mp5.yml	smg	2021-04-02 05:25:19.416726	2021-04-02 05:25:19.416726
4	mp9.yml	smg	2021-04-02 05:25:19.41717	2021-04-02 05:25:19.41717
5	p90.yml	smg	2021-04-02 05:25:19.417623	2021-04-02 05:25:19.417623
6	tar21.yml	smg	2021-04-02 05:25:19.418014	2021-04-02 05:25:19.418014
7	ump45.yml	smg	2021-04-02 05:25:19.418419	2021-04-02 05:25:19.418419
8	l96aw.yml	sniper	2021-04-02 05:25:19.418811	2021-04-02 05:25:19.418811
9	psg1.yml	sniper	2021-04-02 05:25:19.419222	2021-04-02 05:25:19.419222
10	xm109.yml	sniper	2021-04-02 05:25:19.419588	2021-04-02 05:25:19.419588
11	belt-fed-minigun.yml	lmg	2021-04-02 05:25:19.41996	2021-04-02 05:25:19.41996
12	hk21.yml	lmg	2021-04-02 05:25:19.420351	2021-04-02 05:25:19.420351
13	mk46.yml	lmg	2021-04-02 05:25:19.420745	2021-04-02 05:25:19.420745
14	czp10.yml	pistol	2021-04-02 05:25:19.421103	2021-04-02 05:25:19.421103
15	desert-eagle.yml	pistol	2021-04-02 05:25:19.421469	2021-04-02 05:25:19.421469
16	glock.yml	pistol	2021-04-02 05:25:19.42183	2021-04-02 05:25:19.42183
17	magnum-revolver.yml	pistol	2021-04-02 05:25:19.422187	2021-04-02 05:25:19.422187
18	ppk.yml	pistol	2021-04-02 05:25:19.422571	2021-04-02 05:25:19.422571
19	famas.yml	ar	2021-04-02 05:25:19.422933	2021-04-02 05:25:19.422933
20	g36c.yml	ar	2021-04-02 05:25:19.423316	2021-04-02 05:25:19.423316
21	m16a4.yml	ar	2021-04-02 05:25:19.423686	2021-04-02 05:25:19.423686
22	m3.yml	ar	2021-04-02 05:25:19.424047	2021-04-02 05:25:19.424047
23	m4.yml	ar	2021-04-02 05:25:19.424429	2021-04-02 05:25:19.424429
24	scarh.yml	ar	2021-04-02 05:25:19.424798	2021-04-02 05:25:19.424798
25	552-commando.yml	ar	2021-04-02 05:25:19.425176	2021-04-02 05:25:19.425176
26	aug-a3.yml	ar	2021-04-02 05:25:19.425552	2021-04-02 05:25:19.425552
27	saiga12.yml	shotgun	2021-04-02 05:25:19.42593	2021-04-02 05:25:19.42593
28	sasg12.yml	shotgun	2021-04-02 05:25:19.42633	2021-04-02 05:25:19.42633
29	uzi.yml	mp	2021-04-02 05:25:19.426702	2021-04-02 05:25:19.426702
\.


--
-- Data for Name: rifle_instance; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.rifle_instance (rifle_id, rifle_accuracy_map_0, rifle_accuracy_map_1, rifle_accuracy_map_2, rifle_accuracy_map_3, rifle_damage_map_0, rifle_damage_map_1, rifle_damage_map_2, rifle_damage_map_3, rifle_rarity, rifle_file, rifle_str_type, rifle_type, rifle_manufacturer, rifle_name, rifle_vnum, rifle_ammo_max, rifle_ammo_type, rifle_chance_to_injure, rifle_clip_size, rifle_cooldown_between_shots, rifle_critical_chance, rifle_critical_range, rifle_damage_per_second, rifle_disorient_amount, rifle_headshot_bonus, rifle_max_range, rifle_range_multiplier, rifle_reload_time, rifle_rounds_per_minute, rifle_muzzle_velocity, rifle_effective_firing_range, rifle_damage_dice_count, rifle_damage_dice_sides, rifle_incendiary_damage, rifle_explosive_damage, rifle_shrapnel_damage, rifle_corrosive_damage, rifle_cryogenic_damage, rifle_radioactive_damage, rifle_emp_damage, rifle_shock_damage, rifle_anti_matter_damage, rifle_stat_strength, rifle_stat_intelligence, rifle_stat_wisdom, rifle_stat_dexterity, rifle_stat_constitution, rifle_stat_electronics, rifle_stat_armor, rifle_stat_marksmanship, rifle_stat_sniping, rifle_stat_demolitions, rifle_stat_chemistry, rifle_stat_weapon_handling, rifle_stat_strategy, rifle_stat_medical, created_at, updated_at) FROM stdin;
\.


--
-- Data for Name: rifle_placements; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.rifle_placements (id, ip_room_vnum, ip_container_selector, ip_rifle_id, created_at, updated_at) FROM stdin;
\.


--
-- Data for Name: room; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.room (id, room_number, zone, sector_type, name, description, ex_keyword, ex_description, light, room_flag) FROM stdin;
20	128	1	18	MP5 repo	Feeling over burdened by money?	\N	\N	0	16
1	130	1	21	{blu}COBALT:{/blu} {grn}Forward Operating Base{/grn} - Main Hallway	COBALT Air Force base. Ground zero for basic training. All initiates must follow rules and guidelines in your New Recruit handbook. Proceed {grn}North{/grn} recruit!\r\n	\N	\N	1	0
2	131	1	21	{blu}C.O.F.O.B:{/blu} - Southern Hallway	The cement floor is a perfect complement to the reflection of the incandescent lights that are installed into the the ceiling. {gld}To the East{/gld}, you will find the Armory. {gld}To the West{/gld}, you will find the training grounds. Just around the corner to the East and North a ways is the Weapons Locker. Feel free to get acquainted with the layout, recruit.\r\n	\N	\N	1	0
3	132	1	21	{blu}C.O.F.O.B:{/blu} - Southern Hallway	The cement floor is a perfect complement to the reflection of the incandescent lights that are installed into the the ceiling. {gld}To the East{/gld}, you will find the Armory. {gld}To the West{/gld}, you will find the training grounds. Just around the corner to the East and North a ways is the Weapons Locker. Feel free to get acquainted with the layout, recruit.\r\n	\N	\N	1	0
4	133	1	21	{blu}C.O.F.O.B:{/blu} - Southern Hallway	The cement floor is a perfect complement to the reflection of the incandescent lights that are installed into the the ceiling. {gld}To the East{/gld}, you will find the Armory. {gld}To the West{/gld}, you will find the training grounds. Just around the corner to the East and North a ways is the Weapons Locker. Feel free to get acquainted with the layout, recruit.\r\n	\N	\N	1	0
5	134	1	21	{blu}C.O.F.O.B:{/blu} - Southern Hallway	The cement floor is a perfect complement to the reflection of the incandescent lights that are installed into the the ceiling. {gld}To the East{/gld}, you will find the Armory. {gld}To the West{/gld}, you will find the training grounds. Just around the corner to the East and North a ways is the Weapons Locker. Feel free to get acquainted with the layout, recruit.\r\n	\N	\N	1	0
6	135	1	21	{blu}C.O.F.O.B:{/blu} - Southern Hallway	The cement floor is a perfect complement to the reflection of the incandescent lights that are installed into the the ceiling. {gld}To the East{/gld}, you will find the Armory. {gld}To the West{/gld}, you will find the training grounds. Just around the corner to the East and North a ways is the Weapons Locker. Feel free to get acquainted with the layout, recruit.\r\n	\N	\N	1	0
7	136	1	21	{blu}C.O.F.O.B:{/blu} - Southern Hallway	The cement floor is a perfect complement to the reflection of the incandescent lights that are installed into the the ceiling. {gld}To the East{/gld}, you will find the Armory. {gld}To the West{/gld}, you will find the training grounds. Just around the corner to the East and North a ways is the Weapons Locker. Feel free to get acquainted with the layout, recruit.\r\n	\N	\N	1	0
8	137	1	21	{blu}C.O.F.O.B:{/blu} - Southeast Corner	The hallway reaches north and south from here. A reduced temperature is like the result of the industrial grade internal air cooling system. It isn't much, but it beats the outside desert climate. The promise of coffee entices you, but you can't tell which direction it's coming from. \r\n	\N	\N	1	0
9	138	1	21	{blu}C.O.F.O.B:{/blu} - Armory Entrance	A cool draft moves through the bottom crack of the door to the Armory Entrance to the East.It seems the quality of air drastically differs depending on the people in charge of each department. The Sign above the door says in bold letters "Armory".\r\n	\N	\N	1	0
10	139	1	21	{blu}C.O.F.O.B:{/blu} - Armory	As you push through to the East, you notice a few recruits putting on standard issue gear. They ignore you as you take a look around. To the East is the buy station where you can make your purchases.\r\n	\N	\N	1	0
11	140	1	21	{blu}C.O.F.O.B:{/blu} - Armory Buy Station	You see an armor locker with standard issue equipment. Behind the counter is a {gld}list{/gld} of all the various items for sell. You can spend {grn}MP{/grn} (Mission Points) here to upgrade your loadout. To buy something, simply type {grn}"Buy ID"{/grn} where ID is the number next to the item you want in the output of the list command.\r\n	\N	\N	1	0
12	141	1	21	{blu}C.O.F.O.B:{/blu} - Armory Storage Room North	Standard issue armor and defensive utilities line the walls; none of which you can take as they are behind metal cages. There is, however, an Armor locker here with standard issue gear for anyone to take. \r\n	\N	\N	1	0
13	142	1	21	{blu}C.O.F.O.B:{/blu} - Armory Storage Room South	You enter the storage room and immediately notice the strong scent of sand, grime, and gasoline. A few bits of ammunition are strewn across the floor haphazardly. The Armory personnell either recently dug through the piles of ammo crates, or nobody bothered to clean this mess up. There seems to be a computer terminal on the East wall.\r\n	\N	\N	1	0
14	143	1	21	{blu}C.O.F.O.B:{/blu} - Armory - Secluded Room	A musty room with several freshly smoked cigars laying inside a deep ash tray the size of your fist. Someone was loading a Mossberg shotgun and haphazardly left it laying upon the couch as if it were a visitor. The T.V. appears to still be warm. Whoever was here is likely coming back soon. You have a feeling whoever was here will be back shortly.\r\n	\N	\N	1	0
15	144	1	21	{blu}C.O.F.O.B:{/blu} - Eastern Hallway	@FILL_ME@\r\n	\N	\N	1	0
16	145	1	21	{blu}C.O.F.O.B:{/blu} - Eastern Hallway	@FILL_ME@\r\n	\N	\N	1	0
17	146	1	21	{blu}C.O.F.O.B:{/blu} - Eastern Exit	Before you is a sturdy metal door that prevents the outside elements from making their way inside. The engraving on the door simply says {yel}Eastern Exit{/yel}.\r\n	\N	\N	1	0
18	147	1	21	{blu}C.O.F.O.B:{/blu} - Eastern Hallway	@FILL_ME@\r\n	\N	\N	1	0
21	150	1	0	{blu}C.O.F.O.B:{/blu} - Mess Hall	@FILL_ME@\r\n	\N	\N	1	0
22	151	1	21	{blu}C.O.F.O.B:{/blu} - Eastern Hallway	@FILL_ME@\r\n	\N	\N	1	0
23	152	1	21	{blu}C.O.F.O.B:{/blu} - Eastern Hallway	@FILL_ME@\r\n	\N	\N	1	0
24	153	1	21	{blu}C.O.F.O.B:{/blu} - North Hallway	@FILL_ME@\r\n	\N	\N	1	0
25	154	1	21	{blu}C.O.F.O.B:{/blu} - North Hallway	@FILL_ME@\r\n	\N	\N	1	0
26	155	1	21	{blu}C.O.F.O.B:{/blu} - North Hallway Stairs	@FILL_ME@\r\n	\N	\N	1	0
27	156	1	21	{blu}C.O.F.O.B:{/blu} - North Hallway Stairs	@FILL_ME@\r\n	\N	\N	1	0
28	157	1	21	{blu}C.O.F.O.B:{/blu} - North Hallway Stairs	@FILL_ME@\r\n	\N	\N	1	0
29	158	1	21	{blu}C.O.F.O.B:{/blu} - North Hallway Stairs	@FILL_ME@\r\n	\N	\N	1	0
30	159	1	21	{blu}C.O.F.O.B:{/blu} - North Hallway Stairs	@FILL_ME@\r\n	\N	\N	1	0
31	160	1	21	{blu}C.O.F.O.B:{/blu} - Second floor hallway	@FILL_ME@\r\n	\N	\N	1	0
32	161	1	21	{blu}C.O.F.O.B:{/blu} - Second floor hallway	@FILL_ME@\r\n	\N	\N	1	0
33	162	1	21	{blu}C.O.F.O.B:{/blu} - Second floor hallway	@FILL_ME@\r\n	\N	\N	1	0
34	163	1	21	{blu}C.O.F.O.B:{/blu} - Second floor hallway	@FILL_ME@\r\n	\N	\N	1	0
35	164	1	21	{blu}C.O.F.O.B:{/blu} - Second floor hallway	@FILL_ME@\r\n	\N	\N	1	0
36	165	1	21	{blu}C.O.F.O.B:{/blu} - Second floor hallway	@FILL_ME@\r\n	\N	\N	1	0
37	166	1	21	{blu}C.O.F.O.B:{/blu} - Second floor hallway	@FILL_ME@\r\n	\N	\N	1	0
38	167	1	0	{blu}C.O.F.O.B:{/blu} - Second floor Foremast	@FILL_ME@\r\n	\N	\N	1	0
39	168	1	0	{blu}C.O.F.O.B:{/blu} - Second floor Foremast	@FILL_ME@\r\n	\N	\N	1	0
40	169	1	0	{blu}C.O.F.O.B:{/blu} - Second floor Foremast	@FILL_ME@\r\n	\N	\N	1	0
41	170	1	0	{blu}C.O.F.O.B:{/blu} - Second floor Foremast	@FILL_ME@\r\n	\N	\N	1	0
42	171	1	0	{blu}C.O.F.O.B:{/blu} - Second floor Foremast	@FILL_ME@\r\n	\N	\N	1	0
43	172	1	0	{blu}C.O.F.O.B:{/blu} - Second floor Foremast	@FILL_ME@\r\n	\N	\N	1	0
44	173	1	21	{blu}C.O.F.O.B:{/blu} - Second floor Foremast	@FILL_ME@\r\n	\N	\N	1	0
45	174	1	21	{blu}C.O.F.O.B:{/blu} - Second floor Foremast	@FILL_ME@\r\n	\N	\N	1	0
46	175	1	21	{blu}C.O.F.O.B:{/blu} - Second floor Foremast	@FILL_ME@\r\n	\N	\N	1	0
47	176	1	21	{blu}C.O.F.O.B:{/blu} - Second floor Foremast	@FILL_ME@\r\n	\N	\N	1	0
48	177	1	21	{blu}C.O.F.O.B:{/blu} - Second floor hallway	@FILL_ME@\r\n	\N	\N	1	0
49	178	1	21	{blu}C.O.F.O.B:{/blu} - Second floor hallway	@FILL_ME@\r\n	\N	\N	1	0
50	179	1	21	{blu}C.O.F.O.B:{/blu} - Second floor hallway	@FILL_ME@\r\n	\N	\N	1	0
51	180	1	21	{blu}C.O.F.O.B:{/blu} - Second floor hallway	@FILL_ME@\r\n	\N	\N	1	0
52	181	1	21	{blu}C.O.F.O.B:{/blu} - Second floor hallway	@FILL_ME@\r\n	\N	\N	1	0
53	182	1	0	{blu}C.O.F.O.B:{/blu} - Engineering hallway	The doors on the eastern wall are all closed and most likely locked. You see the commons area at the end of the hallway. You notice various surveillance cameras strategically placed at the corners of the ceiling. \r\n	\N	\N	1	0
54	183	1	0	{blu}C.O.F.O.B:{/blu} - Engineering hallway	The doors on the eastern wall are all closed and most likely locked. You see the commons area at the end of the hallway. You notice various surveillance cameras strategically placed at the corners of the ceiling. \r\n	\N	\N	1	0
55	184	1	0	{blu}C.O.F.O.B:{/blu} - Engineering hallway	The doors on the eastern wall are all closed and most likely locked. You see the commons area at the end of the hallway. You notice various surveillance cameras strategically placed at the corners of the ceiling. \r\n	\N	\N	1	0
56	185	1	0	{blu}C.O.F.O.B:{/blu} - Engineering hallway	The doors on the eastern wall are all closed and most likely locked. You see the commons area at the end of the hallway. You notice various surveillance cameras strategically placed at the corners of the ceiling. \r\n	\N	\N	1	0
57	186	1	0	{blu}C.O.F.O.B:{/blu} - Commons area	A comfy looking couch and recliner welcome you to the engineering commons area. Three television screens mounted on the west wall display various graphs and statistics. A ping pong table is in the center of the room. Two large refridgerators are humming quietly in the corner. The north wall consists of large one-way tinted windows that overlook the training fields below. To the east is the entrance to server room 1B.\r\n	\N	\N	1	0
58	187	1	0	{blu}C.O.F.O.B:{/blu} - Server Room 1A	It's noisy and loud in here. The rack mounted servers are fervently whirring under the immense workloads. The ceiling is a grate with a large fan behind it. A complex looking locking mechanism protects rack mounted servers here. \r\n	\N	\N	1	0
59	188	1	0	{blu}C.O.F.O.B:{/blu} - Server Room 1B	Wall to wall rack mounted servers take up the entirety of this room. There is almost nowhere to stand. A haphazard array of ethernet cables snake chaotically to and from each server. Maintaining this must be a headache. \r\n	\N	\N	1	0
60	189	1	21	{blu}C.O.F.O.B:{/blu} - Eastern Hallway	@FILL_ME@\r\n	\N	\N	1	0
61	190	1	0	{blu}C.O.F.O.B:{/blu} - Shooting Range Entrance (east)	@FILL_ME@\r\n	\N	\N	1	0
62	191	1	0	{blu}C.O.F.O.B:{/blu} - Shooting Range Area Falcon	@FILL_ME@\r\n	\N	\N	1	0
63	192	1	0	{blu}C.O.F.O.B:{/blu} - Shooting Range Area Falcon	@FILL_ME@\r\n	\N	\N	1	0
64	193	1	0	{blu}C.O.F.O.B [OUTER]:{/blu} - Eastern DMZ corridor	Dirt stretches to the east towards a heavy containment door. Behind that door must be where you can leave the building. The corridor is dimly lit. The air is dry and smells of a combination of sweat and gasoline for some reason.\r\n	\N	\N	1	0
65	194	1	0	{blu}C.O.F.O.B [OUTER]:{/blu} - Eastern DMZ corridor	Dirt stretches to the east towards a heavy containment door. Behind that door must be where you can leave the building. The corridor is dimly lit. The air is dry and smells of a combination of sweat and gasoline for some reason.\r\n	\N	\N	1	0
66	195	1	0	{blu}C.O.F.O.B [OUTER]:{/blu} - Eastern DMZ corridor	Dirt stretches to the east towards a heavy containment door. Behind that door must be where you can leave the building. The corridor is dimly lit. The air is dry and smells of a combination of sweat and gasoline for some reason.\r\n	\N	\N	1	0
67	196	1	0	{blu}C.O.F.O.B [OUTER]:{/blu} - Fenced off DMZ corridor	Beneath the floor here is an entrance to an underground tunnel. The only problem is that you can't seem to get it to move. To the east is the door to the outside world.A falcon engraving is above the door frame to the east.\r\n	\N	\N	1	0
68	197	1	0	Destroyed overpass - Abott Market South	Crushed and flattened by endless shells, the overpass that you just passed under creaks eternally. Now and then you hear the crumble of asphault and be bend of metal rods. The overpass connects the once thriving market place of Abott Market, but now all you see is rubble and a shell of what used to be a thriving market.\r\n	\N	\N	1	0
69	198	1	0	Destroyed overpass - Abott Market South	Crushed and flattened by endless shells, the overpass that you just passed under creaks eternally. Now and then you hear the crumble of asphault and be bend of metal rods. The overpass connects the once thriving market place of Abott Market, but now all you see is rubble and a shell of what used to be a thriving market.\r\n	\N	\N	1	0
145	273	1	21	{blu}C.O.F.O.B:{/blu} - North Hallway Stairs	@FILL_ME@\r\n	\N	\N	1	0
70	199	1	0	Destroyed overpass - Abott Market South	Crushed and flattened by endless shells, the overpass that you just passed under creaks eternally. Now and then you hear the crumble of asphault and be bend of metal rods. The overpass connects the once thriving market place of Abott Market, but now all you see is rubble and a shell of what used to be a thriving market.\r\n	\N	\N	1	0
71	200	1	0	Destroyed overpass - Abott Market South	Crushed and flattened by endless shells, the overpass that you just passed under creaks eternally. Now and then you hear the crumble of asphault and be bend of metal rods. The overpass connects the once thriving market place of Abott Market, but now all you see is rubble and a shell of what used to be a thriving market.\r\n	\N	\N	1	0
72	201	1	0	Destroyed overpass - Abott Market South	Crushed and flattened by endless shells, the overpass that you just passed under creaks eternally. Now and then you hear the crumble of asphault and be bend of metal rods. The overpass connects the once thriving market place of Abott Market, but now all you see is rubble and a shell of what used to be a thriving market.\r\n	\N	\N	1	0
73	202	1	0	Destroyed overpass - Abott Market South	Crushed and flattened by endless shells, the overpass that you just passed under creaks eternally. Now and then you hear the crumble of asphault and be bend of metal rods. The overpass connects the once thriving market place of Abott Market, but now all you see is rubble and a shell of what used to be a thriving market.\r\n	\N	\N	1	0
74	203	1	0	Destroyed overpass - Abott Market South	Crushed and flattened by endless shells, the overpass that you just passed under creaks eternally. Now and then you hear the crumble of asphault and be bend of metal rods. The overpass connects the once thriving market place of Abott Market, but now all you see is rubble and a shell of what used to be a thriving market.\r\n	\N	\N	1	0
75	204	1	0	Destroyed overpass - Abott Market South	Crushed and flattened by endless shells, the overpass that you just passed under creaks eternally. Now and then you hear the crumble of asphault and be bend of metal rods. The overpass connects the once thriving market place of Abott Market, but now all you see is rubble and a shell of what used to be a thriving market.\r\n	\N	\N	1	0
76	205	1	0	Destroyed overpass - Abott Market South	Crushed and flattened by endless shells, the overpass that you just passed under creaks eternally. Now and then you hear the crumble of asphault and be bend of metal rods. The overpass connects the once thriving market place of Abott Market, but now all you see is rubble and a shell of what used to be a thriving market.\r\n	\N	\N	1	0
77	206	1	0	Destroyed overpass - Abott Market South	Crushed and flattened by endless shells, the overpass that you just passed under creaks eternally. Now and then you hear the crumble of asphault and be bend of metal rods. The overpass connects the once thriving market place of Abott Market, but now all you see is rubble and a shell of what used to be a thriving market.\r\n	\N	\N	1	0
78	207	1	0	Destroyed overpass - Abott Market South	Crushed and flattened by endless shells, the overpass that you just passed under creaks eternally. Now and then you hear the crumble of asphault and be bend of metal rods. The overpass connects the once thriving market place of Abott Market, but now all you see is rubble and a shell of what used to be a thriving market.\r\n	\N	\N	1	0
79	208	1	0	Destroyed overpass - Abott Market South	Crushed and flattened by endless shells, the overpass that you just passed under creaks eternally. Now and then you hear the crumble of asphault and be bend of metal rods. The overpass connects the once thriving market place of Abott Market, but now all you see is rubble and a shell of what used to be a thriving market.\r\n	\N	\N	1	0
80	209	1	0	Destroyed overpass - Abott Market South	Crushed and flattened by endless shells, the overpass that you just passed under creaks eternally. Now and then you hear the crumble of asphault and be bend of metal rods. The overpass connects the once thriving market place of Abott Market, but now all you see is rubble and a shell of what used to be a thriving market.\r\n	\N	\N	1	0
81	210	1	0	Destroyed overpass - Abott Market South	Crushed and flattened by endless shells, the overpass that you just passed under creaks eternally. Now and then you hear the crumble of asphault and be bend of metal rods. The overpass connects the once thriving market place of Abott Market, but now all you see is rubble and a shell of what used to be a thriving market.\r\n	\N	\N	1	0
82	211	1	0	Market Apartments - East entrance	An iron rod gate can be seen laying discarded near the entrance. In another time, that gate would have been used to keep intruders out. Far off to the west is what used to be the tenant parking lot. You'd be surprised to find any tenant using that lot seeing as how low the income requirements were for this apartment complex.\r\n	\N	\N	1	0
83	212	1	0	Market Apartments - East entrance	An iron rod gate can be seen laying discarded near the entrance. In another time, that gate would have been used to keep intruders out. Far off to the west is what used to be the tenant parking lot. You'd be surprised to find any tenant using that lot seeing as how low the income requirements were for this apartment complex.\r\n	\N	\N	1	0
84	213	1	0	Market Apartments - Building Way	Building 2 is a two story building with 16 units. The stairs leading to the second story are completely demolished. The top 4 units to the east are completely exposed to the elements. You could make it upstairs but it would require some sort of rope. \r\n	\N	\N	1	0
85	214	1	0	Market Apartments - Building Way	Building 2 is a two story building with 16 units. The stairs leading to the second story are completely demolished. The top 4 units to the east are completely exposed to the elements. You could make it upstairs but it would require some sort of rope. \r\n	\N	\N	1	0
86	215	1	0	Market Apartments - Building Way	Building 2 is a two story building with 16 units. The stairs leading to the second story are completely demolished. The top 4 units to the east are completely exposed to the elements. You could make it upstairs but it would require some sort of rope. \r\n	\N	\N	1	0
87	216	1	0	Market Apartments - Building 3	You see a two story apartment building with 8 units. Each apartment is a corner unit, but neither home could possibly be more than a studio unit. A few of the doors are closed, which you find peculiar. There can't possibly be anyone living there. Right?\r\n	\N	\N	1	0
88	217	1	0	Market Apartments - Stairs	Before you lie the stairs that will help you reach the second floor of building 3. The handrails are extremely hot to the touch due to baking here under the steady gaze of the sun over your shoulder.\r\n	\N	\N	1	0
89	218	1	0	Market Apartments - Stairs	Before you lie the stairs that will help you reach the second floor of building 3. The handrails are extremely hot to the touch due to baking here under the steady gaze of the sun over your shoulder.\r\n	\N	\N	1	0
146	274	1	21	{blu}C.O.F.O.B:{/blu} - Second floor hallway	@FILL_ME@\r\n	\N	\N	1	0
90	219	1	0	Market Apartments - Stairs	Before you lie the stairs that will help you reach the second floor of building 3. The handrails are extremely hot to the touch due to baking here under the steady gaze of the sun over your shoulder.\r\n	\N	\N	1	0
91	220	1	0	Market Apartments - Building 3 Second floor hallway	A thin layer of dust and dirt cover the cement walkway here. You hear the crackle of dirt and millions of pieces of broken glass under your feet as you make your way about. Oddly enough, there are footprints here.\r\n	\N	\N	1	0
92	221	1	0	Market Apartments - Building 3 Second floor hallway	A thin layer of dust and dirt cover the cement walkway here. You hear the crackle of dirt and millions of pieces of broken glass under your feet as you make your way about. Oddly enough, there are footprints here.\r\n	\N	\N	1	0
93	222	1	0	Market Apartments - Building 3 Second floor hallway	A thin layer of dust and dirt cover the cement walkway here. You hear the crackle of dirt and millions of pieces of broken glass under your feet as you make your way about. Oddly enough, there are footprints here.\r\n	\N	\N	1	0
94	223	1	0	Market Apartments - Building 3 Second floor hallway	A thin layer of dust and dirt cover the cement walkway here. You hear the crackle of dirt and millions of pieces of broken glass under your feet as you make your way about. Oddly enough, there are footprints here.\r\n	\N	\N	1	0
95	224	1	0	Market Apartments - Building 3 Second floor hallway	A thin layer of dust and dirt cover the cement walkway here. You hear the crackle of dirt and millions of pieces of broken glass under your feet as you make your way about. Oddly enough, there are footprints here.\r\n	\N	\N	1	0
96	225	1	0	Market Apartments - Building 3 - Unit 301	A mostly empty apartment. You notice what looks like sleeping bags in the corner but that could just be trash. The air smells like cigarette smoke and dry desert air. The light from outside illuminates trillions of dust particles floating in the in perpetual motion.\r\n	\N	\N	1	0
97	226	1	0	Market Apartments - Building 3 - Unit 301	A mostly empty apartment. You notice what looks like sleeping bags in the corner but that could just be trash. The air smells like cigarette smoke and dry desert air. The light from outside illuminates trillions of dust particles floating in the in perpetual motion.\r\n	\N	\N	1	0
98	227	1	0	Market Apartments - Building 3 - Unit 301 - Master Bedroom	A D.I.Y. home stove sits in the middle of the room. Aside from that, there's nothing to look at. The carpet has been pulled up in places. The walls are covered with massive holes and random burn marks.\r\n	\N	\N	1	0
99	228	1	0	Market Apartments - Building 3 - Unit 301 - Bathroom	The sink seems to still be intact and usable. How any water made its way up here would be nothing short of miraculous. It does appear that the sink has been used recently, which only affirms your suspicion that there are still inhabitants who dwell here.\r\n	\N	\N	1	0
100	229	1	0	Market Apartments - Building 3 Second floor hallway	A thin layer of dust and dirt cover the cement walkway here. You hear the crackle of dirt and millions of pieces of broken glass under your feet as you make your way about. Oddly enough, there are footprints here.\r\n	\N	\N	1	0
101	230	1	0	Market Apartments - Building 3 Second floor hallway	A thin layer of dust and dirt cover the cement walkway here. You hear the crackle of dirt and millions of pieces of broken glass under your feet as you make your way about. Oddly enough, there are footprints here.\r\n	\N	\N	1	0
102	231	1	0	Market Apartments - Building 3 Second floor hallway	A thin layer of dust and dirt cover the cement walkway here. You hear the crackle of dirt and millions of pieces of broken glass under your feet as you make your way about. Oddly enough, there are footprints here.\r\n	\N	\N	1	0
103	232	1	0	Market Apartments - Building 3 - Unit 303	A mostly empty apartment. You notice what looks like sleeping bags in the corner but that could just be trash. The air smells like cigarette smoke and dry desert air. The light from outside illuminates trillions of dust particles floating in the in perpetual motion.\r\n	\N	\N	1	0
104	233	1	0	Market Apartments - Building 3 - Unit 303	A mostly empty apartment. You notice what looks like sleeping bags in the corner but that could just be trash. The air smells like cigarette smoke and dry desert air. The light from outside illuminates trillions of dust particles floating in the in perpetual motion.\r\n	\N	\N	1	0
105	234	1	0	Market Apartments - Building 3 - Unit 303 - Master Bedroom	undefined\r\n	\N	\N	1	0
106	235	1	0	Market Apartments - Building 3 - Unit 303 - Bathroom	The sink seems to still be intact and usable. How any water made its way up here would be nothing short of miraculous. It does appear that the sink has been used recently, which only affirms your suspicion that there are still inhabitants who dwell here.\r\n	\N	\N	1	0
107	236	1	0	Market Apartments - Building 3 Second floor hallway	A thin layer of dust and dirt cover the cement walkway here. You hear the crackle of dirt and millions of pieces of broken glass under your feet as you make your way about. Oddly enough, there are footprints here.\r\n	\N	\N	1	0
108	237	1	0	Market Apartments - Building 3 Second floor hallway	A thin layer of dust and dirt cover the cement walkway here. You hear the crackle of dirt and millions of pieces of broken glass under your feet as you make your way about. Oddly enough, there are footprints here.\r\n	\N	\N	1	0
109	238	1	0	Market Apartments - Building 3 - Unit 305	A mostly empty apartment. You notice what looks like sleeping bags in the corner but that could just be trash. The air smells like cigarette smoke and dry desert air. The light from outside illuminates trillions of dust particles floating in the in perpetual motion.\r\n	\N	\N	1	0
110	239	1	0	Market Apartments - Building 3 - Unit 305	A mostly empty apartment. You notice what looks like sleeping bags in the corner but that could just be trash. The air smells like cigarette smoke and dry desert air. The light from outside illuminates trillions of dust particles floating in the in perpetual motion.\r\n	\N	\N	1	0
111	240	1	0	Market Apartments - Building 3 - Unit 305 - Master Bedroom	undefined\r\n	\N	\N	1	0
112	241	1	0	Market Apartments - Building 3 - Unit 305 - Bathroom	The sink seems to still be intact and usable. How any water made its way up here would be nothing short of miraculous. It does appear that the sink has been used recently, which only affirms your suspicion that there are still inhabitants who dwell here.\r\n	\N	\N	1	0
113	242	1	0	name	description	\N	\N	1	0
114	243	1	0	Market Apartments - Building 3 Second floor hallway	A thin layer of dust and dirt cover the cement walkway here. You hear the crackle of dirt and millions of pieces of broken glass under your feet as you make your way about. Oddly enough, there are footprints here.\r\n	\N	\N	1	0
115	244	1	0	Market Apartments - Building 3 Second floor hallway	A thin layer of dust and dirt cover the cement walkway here. You hear the crackle of dirt and millions of pieces of broken glass under your feet as you make your way about. Oddly enough, there are footprints here.\r\n	\N	\N	1	0
116	245	1	0	Market Apartments - Building 3 - Unit 307	A mostly empty apartment. You notice what looks like sleeping bags in the corner but that could just be trash. The air smells like cigarette smoke and dry desert air. The light from outside illuminates trillions of dust particles floating in the in perpetual motion.\r\n	\N	\N	1	0
117	246	1	0	Market Apartments - Building 3 - Unit 307	A mostly empty apartment. You notice what looks like sleeping bags in the corner but that could just be trash. The air smells like cigarette smoke and dry desert air. The light from outside illuminates trillions of dust particles floating in the in perpetual motion.\r\n	\N	\N	1	0
118	247	1	0	Market Apartments - Building 3 - Unit 307 - Master Bedroom	undefined\r\n	\N	\N	1	0
119	248	1	0	Market Apartments - Building 3 - Unit 307 - Bathroom	The sink seems to still be intact and usable. How any water made its way up here would be nothing short of miraculous. It does appear that the sink has been used recently, which only affirms your suspicion that there are still inhabitants who dwell here.\r\n	\N	\N	1	0
120	249	1	0	Market Apartments - Building Way	Building 2 is a two story building with 16 units. The stairs leading to the second story are completely demolished. The top 4 units to the east are completely exposed to the elements. You could make it upstairs but it would require some sort of rope. \r\n	\N	\N	1	0
121	250	1	0	Market Apartments - Building Way	Building 2 is a two story building with 16 units. The stairs leading to the second story are completely demolished. The top 4 units to the east are completely exposed to the elements. You could make it upstairs but it would require some sort of rope. \r\n	\N	\N	1	0
122	251	1	0	Market Apartments - Building Way	Building 2 is a two story building with 16 units. The stairs leading to the second story are completely demolished. The top 4 units to the east are completely exposed to the elements. You could make it upstairs but it would require some sort of rope. \r\n	\N	\N	1	0
123	252	1	0	Market Apartments - Building 2	Building 1 is a two story building with 8 units with the added addition of an exercise room and what looks like the property manager's office. Both the exercise room and the office look surprisingly intact. You notice lots of fresh footprints to and from building 1 which disappear into the destruction of building 2. Despite the abandoned nature of the complex as a whole, someone is still finding use for these units.\r\n	\N	\N	1	0
124	253	1	0	Abandoned Two way street - Abott Market	A trio of burning cars have become part of the debris scattered along the street. Way off to the north, you spot a working overpass. Deep long scars of blackened concrete tell a tale of destruction. To the west lies a small set of apartments once owned by the only property company to operate in this desolate portion of town. A giant construction crane is resting in the middle of the street to the north.\r\n	\N	\N	1	0
125	254	1	0	Abandoned Two way street - Abott Market	A trio of burning cars have become part of the debris scattered along the street. Way off to the north, you spot a working overpass. Deep long scars of blackened concrete tell a tale of destruction. To the west lies a small set of apartments once owned by the only property company to operate in this desolate portion of town. A giant construction crane is resting in the middle of the street to the north.\r\n	\N	\N	1	0
126	255	1	0	Abandoned Two way street - Abott Market	A trio of burning cars have become part of the debris scattered along the street. Way off to the north, you spot a working overpass. Deep long scars of blackened concrete tell a tale of destruction. To the west lies a small set of apartments once owned by the only property company to operate in this desolate portion of town. A giant construction crane is resting in the middle of the street to the north.\r\n	\N	\N	1	0
127	256	1	0	Abandoned Two way street - Abott Market	A trio of burning cars have become part of the debris scattered along the street. Way off to the north, you spot a working overpass. Deep long scars of blackened concrete tell a tale of destruction. To the west lies a small set of apartments once owned by the only property company to operate in this desolate portion of town. A giant construction crane is resting in the middle of the street to the north.\r\n	\N	\N	1	0
128	257	1	0	Abandoned Two way street - Abott Market	A trio of burning cars have become part of the debris scattered along the street. Way off to the north, you spot a working overpass. Deep long scars of blackened concrete tell a tale of destruction. To the west lies a small set of apartments once owned by the only property company to operate in this desolate portion of town. A giant construction crane is resting in the middle of the street to the north.\r\n	\N	\N	1	0
129	258	1	0	{blu}C.O.F.O.B [OUTER]:{/blu} - Western Hallway	The reinforced metal walls are briefly interrupted by the occasional senior officer door. As you make your way north and south, you notice that the entirety of the western wall consists of metal walls with doors that lead to the underground portion of the base. \r\n	\N	\N	1	0
130	259	1	0	{blu}C.O.F.O.B [OUTER]:{/blu} - Atop the stairs	The floor deviates from the other rooms in that it is made up of a reinforced steel grating. The choice for such a floor remains a mystery. The door to the west leads to the stairs that will take you to the underground portion of the base.\r\n	\N	\N	1	0
131	260	1	0	{blu}C.O.F.O.B [OUTER]:{/blu} - Descending the stairs	fill me\r\n	\N	\N	1	0
132	261	1	0	{blu}C.O.F.O.B [OUTER]:{/blu} - Descending the stairs	fill me\r\n	\N	\N	1	0
134	149	1	0	Mess hall	@FILL_ME@	\N	\N	\N	0
135	263	1	21	{blu}C.O.F.O.B:{/blu} - Eastern Hallway	@FILL_ME@\r\n	\N	\N	1	0
136	264	1	0	{blu}C.O.F.O.B:{/blu} - Mess Hall	@FILL_ME@\r\n	\N	\N	1	0
137	265	1	21	{blu}C.O.F.O.B:{/blu} - Eastern Hallway	@FILL_ME@\r\n	\N	\N	1	0
138	266	1	21	{blu}C.O.F.O.B:{/blu} - Eastern Hallway	@FILL_ME@\r\n	\N	\N	1	0
139	267	1	21	{blu}C.O.F.O.B:{/blu} - North Hallway	@FILL_ME@\r\n	\N	\N	1	0
140	268	1	21	{blu}C.O.F.O.B:{/blu} - North Hallway	@FILL_ME@\r\n	\N	\N	1	0
141	269	1	21	{blu}C.O.F.O.B:{/blu} - North Hallway Stairs	@FILL_ME@\r\n	\N	\N	1	0
142	270	1	21	{blu}C.O.F.O.B:{/blu} - North Hallway Stairs	@FILL_ME@\r\n	\N	\N	1	0
143	271	1	21	{blu}C.O.F.O.B:{/blu} - North Hallway Stairs	@FILL_ME@\r\n	\N	\N	1	0
144	272	1	21	{blu}C.O.F.O.B:{/blu} - North Hallway Stairs	@FILL_ME@\r\n	\N	\N	1	0
147	275	1	21	{blu}C.O.F.O.B:{/blu} - Second floor hallway	@FILL_ME@\r\n	\N	\N	1	0
148	276	1	21	{blu}C.O.F.O.B:{/blu} - Second floor hallway	@FILL_ME@\r\n	\N	\N	1	0
149	277	1	21	{blu}C.O.F.O.B:{/blu} - Second floor hallway	@FILL_ME@\r\n	\N	\N	1	0
150	278	1	21	{blu}C.O.F.O.B:{/blu} - Second floor hallway	@FILL_ME@\r\n	\N	\N	1	0
151	279	1	21	{blu}C.O.F.O.B:{/blu} - Second floor hallway	@FILL_ME@\r\n	\N	\N	1	0
152	280	1	21	{blu}C.O.F.O.B:{/blu} - Second floor hallway	@FILL_ME@\r\n	\N	\N	1	0
153	281	1	0	{blu}C.O.F.O.B:{/blu} - Second floor Foremast	@FILL_ME@\r\n	\N	\N	1	0
154	282	1	0	{blu}C.O.F.O.B:{/blu} - Second floor Foremast	@FILL_ME@\r\n	\N	\N	1	0
155	283	1	0	{blu}C.O.F.O.B:{/blu} - Second floor Foremast	@FILL_ME@\r\n	\N	\N	1	0
156	284	1	0	{blu}C.O.F.O.B:{/blu} - Second floor Foremast	@FILL_ME@\r\n	\N	\N	1	0
157	285	1	0	{blu}C.O.F.O.B:{/blu} - Second floor Foremast	@FILL_ME@\r\n	\N	\N	1	0
158	286	1	0	{blu}C.O.F.O.B:{/blu} - Second floor Foremast	@FILL_ME@\r\n	\N	\N	1	0
159	287	1	21	{blu}C.O.F.O.B:{/blu} - Second floor Foremast	@FILL_ME@\r\n	\N	\N	1	0
160	288	1	21	{blu}C.O.F.O.B:{/blu} - Second floor Foremast	@FILL_ME@\r\n	\N	\N	1	0
161	289	1	21	{blu}C.O.F.O.B:{/blu} - Second floor Foremast	@FILL_ME@\r\n	\N	\N	1	0
162	290	1	21	{blu}C.O.F.O.B:{/blu} - Second floor Foremast	@FILL_ME@\r\n	\N	\N	1	0
163	291	1	21	{blu}C.O.F.O.B:{/blu} - Second floor hallway	@FILL_ME@\r\n	\N	\N	1	0
164	292	1	21	{blu}C.O.F.O.B:{/blu} - Second floor hallway	@FILL_ME@\r\n	\N	\N	1	0
165	293	1	21	{blu}C.O.F.O.B:{/blu} - Second floor hallway	@FILL_ME@\r\n	\N	\N	1	0
166	294	1	21	{blu}C.O.F.O.B:{/blu} - Second floor hallway	@FILL_ME@\r\n	\N	\N	1	0
167	295	1	21	{blu}C.O.F.O.B:{/blu} - Second floor hallway	@FILL_ME@\r\n	\N	\N	1	0
168	296	1	0	{blu}C.O.F.O.B:{/blu} - Engineering hallway	The doors on the eastern wall are all closed and most likely locked. You see the commons area at the end of the hallway. You notice various surveillance cameras strategically placed at the corners of the ceiling. \r\n	\N	\N	1	0
169	297	1	0	{blu}C.O.F.O.B:{/blu} - Engineering hallway	The doors on the eastern wall are all closed and most likely locked. You see the commons area at the end of the hallway. You notice various surveillance cameras strategically placed at the corners of the ceiling. \r\n	\N	\N	1	0
170	298	1	0	{blu}C.O.F.O.B:{/blu} - Engineering hallway	The doors on the eastern wall are all closed and most likely locked. You see the commons area at the end of the hallway. You notice various surveillance cameras strategically placed at the corners of the ceiling. \r\n	\N	\N	1	0
171	299	1	0	{blu}C.O.F.O.B:{/blu} - Engineering hallway	The doors on the eastern wall are all closed and most likely locked. You see the commons area at the end of the hallway. You notice various surveillance cameras strategically placed at the corners of the ceiling. \r\n	\N	\N	1	0
172	300	1	0	{blu}C.O.F.O.B:{/blu} - Commons area	A comfy looking couch and recliner welcome you to the engineering commons area. Three television screens mounted on the west wall display various graphs and statistics. A ping pong table is in the center of the room. Two large refridgerators are humming quietly in the corner. The north wall consists of large one-way tinted windows that overlook the training fields below. To the east is the entrance to server room 1B.\r\n	\N	\N	1	0
173	301	1	0	{blu}C.O.F.O.B:{/blu} - Server Room 1A	It's noisy and loud in here. The rack mounted servers are fervently whirring under the immense workloads. The ceiling is a grate with a large fan behind it. A complex looking locking mechanism protects rack mounted servers here. \r\n	\N	\N	1	0
174	302	1	0	{blu}C.O.F.O.B:{/blu} - Server Room 1B	Wall to wall rack mounted servers take up the entirety of this room. There is almost nowhere to stand. A haphazard array of ethernet cables snake chaotically to and from each server. Maintaining this must be a headache. \r\n	\N	\N	1	0
19	148	1	21	{blu}C.O.F.O.B:{/blu} - Eastern Hallway	@FILL_ME@\r\n	\N	\N	1	0
\.


--
-- Data for Name: room_direction_data; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.room_direction_data (id, room_number, exit_direction, general_description, keyword, exit_info, exit_key, to_room) FROM stdin;
576	128	0	general_description	keyword	1	0	130
577	130	0	general_description	keyword	1	0	131
578	130	2	general description	keyword	1	0	128
579	131	1	general_description	keyword	1	0	135
580	131	2	general description	keyword	1	0	130
581	131	3	general_description	keyword	1	0	132
582	132	1	general description	keyword	1	0	131
583	132	3	general_description	keyword	1	0	133
584	133	1	general description	keyword	1	0	132
585	133	3	general_description	keyword	1	0	134
586	134	0	general_description	keyword	1	0	258
587	134	1	general description	keyword	1	0	133
588	135	1	general_description	keyword	1	0	136
589	135	3	general description	keyword	1	0	131
590	136	0	general_description	keyword	1	0	137
591	136	3	general description	keyword	1	0	135
592	137	0	general_description	keyword	1	0	138
593	137	2	general description	keyword	1	0	136
594	138	0	general_description	keyword	1	0	144
595	138	1	general_description	keyword	1	0	139
596	138	2	general description	keyword	1	0	137
597	139	1	general_description	keyword	1	0	140
598	139	3	general description	keyword	1	0	138
599	140	0	general_description	keyword	1	0	141
600	140	1	general_description	keyword	1	0	143
601	140	2	general_description	keyword	1	0	142
602	140	3	general description	keyword	1	0	139
603	141	2	general description	keyword	1	0	140
604	142	0	general description	keyword	1	0	140
605	143	3	general description	keyword	1	0	140
606	144	0	general_description	keyword	1	0	145
607	144	2	general description	keyword	1	0	138
608	145	0	general_description	keyword	1	0	146
609	145	2	general description	keyword	1	0	144
610	146	0	general_description	keyword	1	0	147
611	146	1	general_description	keyword	1	0	193
612	146	2	general description	keyword	1	0	145
613	147	0	general_description	keyword	1	0	148
614	147	2	general description	keyword	1	0	146
615	147	3	general_description	keyword	1	0	189
618	150	1	general description	keyword	1	0	149
619	151	0	general_description	keyword	1	0	152
620	151	2	general description	keyword	1	0	149
621	152	2	general description	keyword	1	0	151
622	152	3	general_description	keyword	1	0	153
623	153	1	general description	keyword	1	0	152
624	153	3	general_description	keyword	1	0	154
625	154	1	general description	keyword	1	0	153
626	154	3	general_description	keyword	1	0	155
627	155	0	general_description	keyword	1	0	156
628	155	1	general description	keyword	1	0	154
629	156	2	general description	keyword	1	0	155
630	156	4	general_description	keyword	1	0	157
631	157	0	general_description	keyword	1	0	158
632	157	5	general description	keyword	1	0	156
633	158	2	general description	keyword	1	0	157
634	158	4	general_description	keyword	1	0	159
635	159	0	general_description	keyword	1	0	160
636	159	5	general description	keyword	1	0	158
637	160	0	general_description	keyword	1	0	161
638	160	2	general description	keyword	1	0	159
639	161	0	general_description	keyword	1	0	167
640	161	1	general_description	keyword	1	0	177
641	161	2	general description	keyword	1	0	160
642	161	3	general_description	keyword	1	0	162
643	162	1	general description	keyword	1	0	161
644	162	3	general_description	keyword	1	0	163
645	163	1	general description	keyword	1	0	162
646	163	3	general_description	keyword	1	0	164
647	164	1	general description	keyword	1	0	163
648	164	3	general_description	keyword	1	0	165
649	165	1	general description	keyword	1	0	164
650	165	3	general_description	keyword	1	0	166
651	166	1	general description	keyword	1	0	165
652	167	0	general_description	keyword	1	0	168
653	167	2	general description	keyword	1	0	161
654	168	1	general_description	keyword	1	0	169
655	168	2	general description	keyword	1	0	167
656	168	3	general_description	keyword	1	0	173
657	169	0	general_description	keyword	1	0	170
658	169	3	general description	keyword	1	0	168
659	170	0	general_description	keyword	1	0	171
660	170	2	general description	keyword	1	0	169
661	171	0	general_description	keyword	1	0	172
662	171	2	general description	keyword	1	0	170
663	172	2	general description	keyword	1	0	171
664	173	0	general_description	keyword	1	0	174
665	173	1	general description	keyword	1	0	168
666	174	0	general_description	keyword	1	0	175
667	174	2	general description	keyword	1	0	173
668	175	0	general_description	keyword	1	0	176
669	175	2	general description	keyword	1	0	174
670	176	2	general description	keyword	1	0	175
671	177	1	general_description	keyword	1	0	178
672	177	3	general description	keyword	1	0	161
673	178	1	general_description	keyword	1	0	179
674	178	3	general description	keyword	1	0	177
675	179	1	general_description	keyword	1	0	180
676	179	3	general description	keyword	1	0	178
677	180	0	general_description	keyword	1	0	182
678	180	1	general_description	keyword	1	0	181
679	180	3	general description	keyword	1	0	179
680	181	3	general description	keyword	1	0	180
681	182	0	general_description	keyword	1	0	183
682	182	2	general description	keyword	1	0	180
683	183	0	general_description	keyword	1	0	184
684	183	1	general_description	keyword	1	0	187
685	183	2	general description	keyword	1	0	182
686	184	0	general_description	keyword	1	0	185
687	184	2	general description	keyword	1	0	183
688	185	0	general_description	keyword	1	0	186
689	185	2	general description	keyword	1	0	184
690	186	1	general_description	keyword	1	0	188
691	186	2	general description	keyword	1	0	185
692	187	3	general description	keyword	1	0	183
693	188	3	general description	keyword	1	0	186
694	189	1	general description	keyword	1	0	147
695	189	3	general_description	keyword	1	0	190
696	190	1	general description	keyword	1	0	189
697	190	2	general_description	keyword	1	0	191
698	191	0	general description	keyword	1	0	190
699	191	2	general_description	keyword	1	0	192
700	192	0	general description	keyword	1	0	191
701	193	1	general_description	keyword	1	0	194
702	193	3	general description	keyword	1	0	146
703	194	1	general_description	keyword	1	0	195
704	194	3	general description	keyword	1	0	193
705	195	1	general_description	keyword	1	0	196
706	195	3	general description	keyword	1	0	194
707	196	1	general_description	keyword	1	0	197
708	196	3	general description	keyword	1	0	195
709	197	0	general_description	keyword	1	0	198
710	197	3	general description	keyword	1	0	196
711	198	1	general_description	keyword	1	0	199
712	198	2	general description	keyword	1	0	197
713	199	0	general_description	keyword	1	0	200
714	199	3	general description	keyword	1	0	198
715	200	0	general_description	keyword	1	0	201
716	200	2	general description	keyword	1	0	199
717	201	0	general_description	keyword	1	0	202
718	201	2	general description	keyword	1	0	200
719	202	0	general_description	keyword	1	0	203
720	202	2	general description	keyword	1	0	201
721	203	0	general_description	keyword	1	0	204
722	203	2	general description	keyword	1	0	202
723	204	0	general_description	keyword	1	0	205
724	204	2	general description	keyword	1	0	203
725	205	0	general_description	keyword	1	0	206
726	205	2	general description	keyword	1	0	204
727	206	0	general_description	keyword	1	0	207
728	206	2	general description	keyword	1	0	205
729	207	0	general_description	keyword	1	0	208
730	207	2	general description	keyword	1	0	206
731	208	0	general_description	keyword	1	0	209
732	208	2	general description	keyword	1	0	207
733	209	0	general_description	keyword	1	0	210
734	209	2	general description	keyword	1	0	208
735	210	0	general_description	keyword	1	0	253
736	210	2	general description	keyword	1	0	209
737	210	3	general_description	keyword	1	0	211
738	211	1	general description	keyword	1	0	210
739	211	3	general_description	keyword	1	0	212
740	212	0	general_description	keyword	1	0	213
741	212	1	general description	keyword	1	0	211
742	213	0	general_description	keyword	1	0	214
743	213	2	general description	keyword	1	0	212
744	214	2	general description	keyword	1	0	213
745	214	3	general_description	keyword	1	0	215
746	215	0	general_description	keyword	1	0	216
747	215	1	general description	keyword	1	0	214
748	215	3	general_description	keyword	1	0	249
749	216	0	general_description	keyword	1	0	217
750	216	2	general description	keyword	1	0	215
751	217	2	general description	keyword	1	0	216
752	217	4	general_description	keyword	1	0	218
753	218	0	general_description	keyword	1	0	219
754	218	5	general description	keyword	1	0	217
755	219	2	general description	keyword	1	0	218
756	219	4	general_description	keyword	1	0	220
757	220	0	general_description	keyword	1	0	221
758	220	5	general description	keyword	1	0	219
759	221	1	general_description	keyword	1	0	236
760	221	2	general description	keyword	1	0	220
761	221	3	general_description	keyword	1	0	222
762	222	1	general description	keyword	1	0	221
763	222	3	general_description	keyword	1	0	223
764	223	1	general description	keyword	1	0	222
765	223	3	general_description	keyword	1	0	224
766	224	0	general_description	keyword	1	0	225
767	224	1	general description	keyword	1	0	223
768	224	3	general_description	keyword	1	0	229
769	225	0	general_description	keyword	1	0	226
770	225	2	general description	keyword	1	0	224
771	226	0	general_description	keyword	1	0	227
772	226	1	general_description	keyword	1	0	228
773	226	2	general description	keyword	1	0	225
774	227	2	general description	keyword	1	0	226
775	228	3	general description	keyword	1	0	226
776	229	1	general description	keyword	1	0	224
777	229	3	general_description	keyword	1	0	230
778	230	1	general description	keyword	1	0	229
779	230	3	general_description	keyword	1	0	231
780	231	0	general_description	keyword	1	0	232
781	231	1	general description	keyword	1	0	230
782	232	0	general_description	keyword	1	0	233
783	232	2	general description	keyword	1	0	231
784	233	0	general_description	keyword	1	0	234
785	233	1	general_description	keyword	1	0	235
786	233	2	general description	keyword	1	0	232
787	234	2	general description	keyword	1	0	233
788	235	3	general description	keyword	1	0	233
789	236	1	general_description	keyword	1	0	237
790	236	3	general description	keyword	1	0	221
791	237	0	general_description	keyword	1	0	238
792	237	1	general_description	keyword	1	0	242
793	237	3	general description	keyword	1	0	236
794	238	0	general_description	keyword	1	0	239
795	238	2	general description	keyword	1	0	237
796	239	0	general_description	keyword	1	0	240
797	239	1	general_description	keyword	1	0	241
798	239	2	general description	keyword	1	0	238
799	240	2	general description	keyword	1	0	239
800	241	3	general description	keyword	1	0	239
801	242	1	general_description	keyword	1	0	243
802	242	3	general description	keyword	1	0	237
803	243	1	general_description	keyword	1	0	244
804	243	3	general description	keyword	1	0	242
805	244	0	general_description	keyword	1	0	245
806	244	3	general description	keyword	1	0	243
807	245	0	general_description	keyword	1	0	246
808	245	2	general description	keyword	1	0	244
809	246	0	general_description	keyword	1	0	247
810	246	1	general_description	keyword	1	0	248
811	246	2	general description	keyword	1	0	245
812	247	2	general description	keyword	1	0	246
813	248	3	general description	keyword	1	0	246
814	249	1	general description	keyword	1	0	215
815	249	3	general_description	keyword	1	0	250
816	250	1	general description	keyword	1	0	249
817	250	3	general_description	keyword	1	0	251
818	251	0	general_description	keyword	1	0	252
819	251	1	general description	keyword	1	0	250
820	252	2	general description	keyword	1	0	251
821	253	0	general_description	keyword	1	0	254
822	253	2	general description	keyword	1	0	210
823	254	0	general_description	keyword	1	0	255
824	254	2	general description	keyword	1	0	253
825	255	0	general_description	keyword	1	0	256
826	255	2	general description	keyword	1	0	254
827	256	0	general_description	keyword	1	0	257
828	256	2	general description	keyword	1	0	255
829	257	2	general description	keyword	1	0	256
830	258	0	general_description	keyword	1	0	259
831	258	2	general description	keyword	1	0	134
832	259	2	general description	keyword	1	0	258
833	259	3	general_description	keyword	1	0	260
834	260	1	general description	keyword	1	0	259
835	260	5	general_description	keyword	1	0	261
836	261	4	general description	keyword	1	0	260
838	263	0	general_description	keyword	1	0	265
839	263	2	general description	keyword	1	0	148
840	263	3	general_description	keyword	1	0	264
841	264	1	general description	keyword	1	0	263
842	265	0	general_description	keyword	1	0	266
843	265	2	general description	keyword	1	0	263
844	266	2	general description	keyword	1	0	265
845	266	3	general_description	keyword	1	0	267
846	267	1	general description	keyword	1	0	266
847	267	3	general_description	keyword	1	0	268
848	268	1	general description	keyword	1	0	267
849	268	3	general_description	keyword	1	0	269
850	269	0	general_description	keyword	1	0	270
851	269	1	general description	keyword	1	0	268
852	270	2	general description	keyword	1	0	269
853	270	4	general_description	keyword	1	0	271
854	271	0	general_description	keyword	1	0	272
855	271	5	general description	keyword	1	0	270
856	272	2	general description	keyword	1	0	271
857	272	4	general_description	keyword	1	0	273
858	273	0	general_description	keyword	1	0	274
859	273	5	general description	keyword	1	0	272
860	274	0	general_description	keyword	1	0	275
861	274	2	general description	keyword	1	0	273
862	275	0	general_description	keyword	1	0	281
863	275	1	general_description	keyword	1	0	291
864	275	2	general description	keyword	1	0	274
865	275	3	general_description	keyword	1	0	276
866	276	1	general description	keyword	1	0	275
867	276	3	general_description	keyword	1	0	277
868	277	1	general description	keyword	1	0	276
869	277	3	general_description	keyword	1	0	278
870	278	1	general description	keyword	1	0	277
871	278	3	general_description	keyword	1	0	279
872	279	1	general description	keyword	1	0	278
873	279	3	general_description	keyword	1	0	280
874	280	1	general description	keyword	1	0	279
875	281	0	general_description	keyword	1	0	282
876	281	2	general description	keyword	1	0	275
877	282	1	general_description	keyword	1	0	283
878	282	2	general description	keyword	1	0	281
879	282	3	general_description	keyword	1	0	287
880	283	0	general_description	keyword	1	0	284
881	283	3	general description	keyword	1	0	282
882	284	0	general_description	keyword	1	0	285
883	284	2	general description	keyword	1	0	283
884	285	0	general_description	keyword	1	0	286
885	285	2	general description	keyword	1	0	284
886	286	2	general description	keyword	1	0	285
887	287	0	general_description	keyword	1	0	288
888	287	1	general description	keyword	1	0	282
889	288	0	general_description	keyword	1	0	289
890	288	2	general description	keyword	1	0	287
891	289	0	general_description	keyword	1	0	290
892	289	2	general description	keyword	1	0	288
893	290	2	general description	keyword	1	0	289
894	291	1	general_description	keyword	1	0	292
895	291	3	general description	keyword	1	0	275
896	292	1	general_description	keyword	1	0	293
897	292	3	general description	keyword	1	0	291
898	293	1	general_description	keyword	1	0	294
899	293	3	general description	keyword	1	0	292
900	294	0	general_description	keyword	1	0	296
901	294	1	general_description	keyword	1	0	295
902	294	3	general description	keyword	1	0	293
903	295	3	general description	keyword	1	0	294
904	296	0	general_description	keyword	1	0	297
905	296	2	general description	keyword	1	0	294
906	297	0	general_description	keyword	1	0	298
907	297	1	general_description	keyword	1	0	301
908	297	2	general description	keyword	1	0	296
909	298	0	general_description	keyword	1	0	299
910	298	2	general description	keyword	1	0	297
911	299	0	general_description	keyword	1	0	300
912	299	2	general description	keyword	1	0	298
913	300	1	general_description	keyword	1	0	302
914	300	2	general description	keyword	1	0	299
915	301	3	general description	keyword	1	0	297
916	302	3	general description	keyword	1	0	300
917	148	0	general_description	keyword	1	0	263
918	148	2	general description	keyword	1	0	147
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
1	1	1	2021-04-02 05:25:19.440977	2021-04-02 05:25:19.440977
2	2	1	2021-04-02 05:25:19.441474	2021-04-02 05:25:19.441474
3	3	1	2021-04-02 05:25:19.441862	2021-04-02 05:25:19.441862
4	4	1	2021-04-02 05:25:19.442238	2021-04-02 05:25:19.442238
5	5	1	2021-04-02 05:25:19.442602	2021-04-02 05:25:19.442602
6	6	1	2021-04-02 05:25:19.443017	2021-04-02 05:25:19.443017
7	7	1	2021-04-02 05:25:19.443384	2021-04-02 05:25:19.443384
8	8	1	2021-04-02 05:25:19.443746	2021-04-02 05:25:19.443746
9	9	1	2021-04-02 05:25:19.444136	2021-04-02 05:25:19.444136
10	10	1	2021-04-02 05:25:19.444506	2021-04-02 05:25:19.444506
11	11	1	2021-04-02 05:25:19.444893	2021-04-02 05:25:19.444893
12	12	1	2021-04-02 05:25:19.445259	2021-04-02 05:25:19.445259
13	13	1	2021-04-02 05:25:19.445633	2021-04-02 05:25:19.445633
14	14	1	2021-04-02 05:25:19.446018	2021-04-02 05:25:19.446018
15	15	1	2021-04-02 05:25:19.446364	2021-04-02 05:25:19.446364
16	16	1	2021-04-02 05:25:19.44672	2021-04-02 05:25:19.44672
17	17	1	2021-04-02 05:25:19.447093	2021-04-02 05:25:19.447093
18	18	1	2021-04-02 05:25:19.447467	2021-04-02 05:25:19.447467
19	19	1	2021-04-02 05:25:19.447827	2021-04-02 05:25:19.447827
20	20	1	2021-04-02 05:25:19.448239	2021-04-02 05:25:19.448239
21	21	1	2021-04-02 05:25:19.448598	2021-04-02 05:25:19.448598
22	22	1	2021-04-02 05:25:19.448994	2021-04-02 05:25:19.448994
23	23	1	2021-04-02 05:25:19.449345	2021-04-02 05:25:19.449345
24	24	1	2021-04-02 05:25:19.449721	2021-04-02 05:25:19.449721
25	25	1	2021-04-02 05:25:19.450076	2021-04-02 05:25:19.450076
26	26	1	2021-04-02 05:25:19.450429	2021-04-02 05:25:19.450429
27	27	1	2021-04-02 05:25:19.450802	2021-04-02 05:25:19.450802
28	28	1	2021-04-02 05:25:19.451154	2021-04-02 05:25:19.451154
29	29	1	2021-04-02 05:25:19.451522	2021-04-02 05:25:19.451522
30	30	1	2021-04-02 05:25:19.451885	2021-04-02 05:25:19.451885
\.


--
-- Data for Name: skill_trees; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.skill_trees (id, skill_name, skill_category, skill_player_class, skill_parent_id, skill_order, skill_description, created_at, updated_at) FROM stdin;
1	intel:1-tracking-shot	Technology	sniper	\N	0	\N	2021-04-02 05:25:19.456247	2021-04-02 05:25:19.456247
2	intel:2-sensor-grenade	Technology	sniper	1	0	\N	2021-04-02 05:25:19.456849	2021-04-02 05:25:19.456849
3	intel:3-xray-shot	Technology	sniper	2	1	\N	2021-04-02 05:25:19.45743	2021-04-02 05:25:19.45743
4	intel:4-area-scan	Technology	sniper	\N	2	\N	2021-04-02 05:25:19.457965	2021-04-02 05:25:19.457965
5	disrupt:1-chaff-grenade	Technology	sniper	\N	0	\N	2021-04-02 05:25:19.458432	2021-04-02 05:25:19.458432
6	disrupt:2-emp-grenade	Technology	sniper	5	1	\N	2021-04-02 05:25:19.458862	2021-04-02 05:25:19.458862
7	claymore:1-plant	Demolitions	sniper	\N	0	\N	2021-04-02 05:25:19.459363	2021-04-02 05:25:19.459363
8	claymore:2-shrapnel	Demolitions	sniper	7	1	\N	2021-04-02 05:25:19.459801	2021-04-02 05:25:19.459801
9	claymore:3-corrosive	Demolitions	sniper	8	1	\N	2021-04-02 05:25:19.460325	2021-04-02 05:25:19.460325
10	guided-missile	Demolitions	sniper	\N	1	\N	2021-04-02 05:25:19.460838	2021-04-02 05:25:19.460838
11	light-bandage	Medical	sniper	\N	1	\N	2021-04-02 05:25:19.461282	2021-04-02 05:25:19.461282
12	suture	Medical	sniper	\N	1	\N	2021-04-02 05:25:19.461716	2021-04-02 05:25:19.461716
13	adrenaline-shot	Medical	sniper	\N	1	\N	2021-04-02 05:25:19.462111	2021-04-02 05:25:19.462111
14	underbarrel-grenade-launcher	Sniping	sniper	\N	1	\N	2021-04-02 05:25:19.4625	2021-04-02 05:25:19.4625
15	underbarrel-shotgun	Sniping	sniper	\N	1	\N	2021-04-02 05:25:19.462886	2021-04-02 05:25:19.462886
16	target-limb	Sniping	sniper	\N	1	\N	2021-04-02 05:25:19.463271	2021-04-02 05:25:19.463271
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
1	1	128	228	Hereford Base	10	0
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
-- Name: armor_index_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.armor_index_id_seq', 25, true);


--
-- Name: camera_feed_feed_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.camera_feed_feed_id_seq', 1, false);


--
-- Name: class_breacher_breacher_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.class_breacher_breacher_id_seq', 1, false);


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

SELECT pg_catalog.setval('public.class_marine_marine_id_seq', 1, false);


--
-- Name: class_medic_medic_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.class_medic_medic_id_seq', 7, true);


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
-- Name: contract_steps_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.contract_steps_id_seq', 1, false);


--
-- Name: contracts_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.contracts_id_seq', 1, false);


--
-- Name: extra_description_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.extra_description_id_seq', 10, true);


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

SELECT pg_catalog.setval('public.mob_equipment_id_seq', 1, false);


--
-- Name: mob_equipment_map_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.mob_equipment_map_id_seq', 1, false);


--
-- Name: mob_roam_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.mob_roam_id_seq', 1, false);


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
-- Name: player_base_ability_pba_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.player_base_ability_pba_id_seq', 1, false);


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
-- Name: player_skill_points_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.player_skill_points_id_seq', 16, true);


--
-- Name: player_skill_usage_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.player_skill_usage_id_seq', 112, true);


--
-- Name: rifle_attachment_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.rifle_attachment_id_seq', 1, false);


--
-- Name: rifle_index_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.rifle_index_id_seq', 29, true);


--
-- Name: rifle_instance_rifle_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.rifle_instance_rifle_id_seq', 1, false);


--
-- Name: rifle_placements_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.rifle_placements_id_seq', 1, false);


--
-- Name: room_direction_data_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.room_direction_data_id_seq', 918, true);


--
-- Name: room_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.room_id_seq', 174, true);


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
-- Name: skill_points_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.skill_points_id_seq', 30, true);


--
-- Name: skill_trees_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.skill_trees_id_seq', 16, true);


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
-- Name: armor_index armor_index_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.armor_index
    ADD CONSTRAINT armor_index_pkey PRIMARY KEY (id);


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
-- Name: extra_description extra_description_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.extra_description
    ADD CONSTRAINT extra_description_pkey PRIMARY KEY (id);


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
-- Name: terminal_choices terminal_choices_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.terminal_choices
    ADD CONSTRAINT terminal_choices_pkey PRIMARY KEY (id);


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

