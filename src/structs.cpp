#include "utils.h"
#include "structs.h"
#include "comm.h"
#include <array>
#include "mods/behaviour_tree_impl.hpp"

namespace mods {
	struct extra_desc_data;
};

static inline std::string default_yaml_file(const std::string& type) {
	return type + ".yml";
}

#include "mods/util.hpp"
#include "mods/rooms.hpp"

obj_data::~obj_data() {
	name.clear();
	str_type.clear();
	str_sub_type.clear();
	description.clear();
	short_description.clear();
	action_description.clear();
	ex_description.clear();
	carried_by = nullptr;
	worn_by = nullptr;
	worn_on = -1;
	in_obj = nullptr;
	contains = nullptr;
	if(next_content) {
		auto head = next_content;
		auto tmp = head;
		while(head != NULL) {
			tmp = head;
			head = head->next;
			free(tmp);
		}
		next_content = nullptr;
	}
	next = nullptr;
	ai_state = 0;
}
void obj_data::init() {
	d("[debug]obj_data::init()\n");
	forged = false;
	visibility = STARTING_VISIBILITY;
	feed_status = 0;
	m_location_data = 0;
	std::fill(m_capabilities.begin(),m_capabilities.end(),0);
	item_number = 0;
	in_room = 0;
	name = "<default-1>";
	str_type = "object";
	str_sub_type = "";
	description = name;
	short_description = name;
	action_description = name;
	ex_description.clear();
	carried_by = nullptr;
	worn_by = nullptr;
	worn_on = -1;
	in_obj = nullptr;
	contains = nullptr;
	if(next_content) {
		auto head = next_content;
		auto tmp = head;
		while(head != NULL) {
			tmp = head;
			head = head->next;
			free(tmp);
		}
		next_content = nullptr;
	}
	next = nullptr;
	ai_state = 0;
	obj_flags.init();
	type = 0;
	this->uuid = mods::globals::obj_uuid();
	from_direction = 0;
	m_db_id = 0;

#define MENTOC_OBJ_INITIALIZE_CONSTRUCTOR(r,data,CLASS_TYPE) \
			this->BOOST_PP_CAT(m_,CLASS_TYPE) = nullptr;
	BOOST_PP_SEQ_FOR_EACH(MENTOC_OBJ_INITIALIZE_CONSTRUCTOR, ~, MENTOC_ITEM_TYPES_SEQ)
}
obj_data::obj_data(std::string item_type,std::string_view feed_file) {
#ifdef __MENTOC_OBJ_DATA_DEBUG_OUTPUT__
	std::cerr << "obj_data::obj_data(string item_type, string feed_file)\n";
#endif
	int type = txt::yaml_string_to_int(item_type);
	obj_data(type,feed_file);
}
int16_t obj_data::feed(int16_t in_type,std::string_view feed_file) {
#ifdef __MENTOC_OBJ_DATA_DEBUG_OUTPUT__
	std::cerr << "obj_data::feed(int item_type, string feed_file)\n";
#endif
	this->feed_status = 2;
	this->m_feed_file = feed_file.data();
	this->type = in_type;
	this->str_type = mods::util::yaml_int_to_string(in_type);
	m_db_id = 0;
	std::string s_type = this->str_type.substr(strlen("ITEM_"));
	std::transform(s_type.begin(),s_type.end(),s_type.begin(),
	[](unsigned char c) {
		return std::tolower(c);
	});
#define MENTOC_OBJ_DATA_FEED_DUAL(r,data,CLASS_TYPE) \
			if(s_type.compare( BOOST_PP_STRINGIZE(CLASS_TYPE) ) == 0){\
				this->CLASS_TYPE(feed_file); \
				this->post_feed(this->BOOST_PP_CAT(m_,CLASS_TYPE).get());\
			}
	BOOST_PP_SEQ_FOR_EACH(MENTOC_OBJ_DATA_FEED_DUAL, ~, MENTOC_ITEM_TYPES_SEQ)
	return this->feed_status;
}
obj_data::obj_data(const obj_data& other) {
	forged = other.forged;
	feed(other.type,other.m_feed_file);
	action_description = other.action_description;
#ifdef __MENTOC_OBJ_DATA_DEBUG_OUTPUT__
	std::cerr << ("obj_data copy constructor") << ", name:'" << name.c_str() << "'\n";
#endif
}
obj_data& obj_data::operator=(obj_data& other) {
	forged = other.forged;
	feed(other.type,other.m_feed_file);
	action_description = other.action_description;
#ifdef __MENTOC_OBJ_DATA_DEBUG_OUTPUT__
	std::cerr << ("obj_data operator equals") << ", name:'" << name.c_str() << "'\n";
#endif
	return *this;
}

std::string obj_data::generate_stat_page() {
#define MENTOC_OBJ_DATA_STAT_GEN(r,data,CLASS_TYPE) \
			if(BOOST_PP_CAT(m_, CLASS_TYPE)){\
				BOOST_PP_CAT(m_,CLASS_TYPE)->attributes->generate_map();\
				return mods::util::map2str(\
				BOOST_PP_CAT(m_,CLASS_TYPE)->attributes->exported\
				);\
			}
	BOOST_PP_SEQ_FOR_EACH(MENTOC_OBJ_DATA_STAT_GEN, ~, MENTOC_ITEM_TYPES_SEQ)
	return "<no stat page>";
}

void obj_flag_data::init() {
	memset(value,0,sizeof(value));
	holds_ammo = 0;
	is_ammo = 0;
	type = 0;
	type_flag = 0;
	weapon_flags = 0;
	ammo_max = 0;
	ammo = 0;
	clip_size = 0;
	wear_flags = 0;
	extra_flags = 0;
	weight = 0;
	cost = 0;
	cost_per_day = 0;
	timer = 0;
	bitvector = 0;
}
bool obj_data::flagged(int value) {
	return ((obj_flags.extra_flags) & (value));
}

#ifdef __MENTOC_USE_PQXX_RESULT__
void obj_flag_data::feed(const pqxx::result::reference& row) {
#else
void obj_flag_data::feed(pqxx::row row) {
#endif
	if(mods::string(row["value_0"]).length()) {
		value[0] = mods::util::stoi<int>(row["value_0"]);
	}

	if(mods::string(row["value_1"]).length()) {
		value[1] = mods::util::stoi<int>(row["value_1"]);
	}

	if(mods::string(row["value_2"]).length()) {
		value[2] = mods::util::stoi<int>(row["value_2"]);
	}

	if(mods::string(row["value_3"]).length()) {
		value[3] = mods::util::stoi<int>(row["value_3"]);
	}
	ammo_max = mods::util::stoi<int>(row["ammo_max"]);
	clip_size = mods::util::stoi<int>(row["clip_size"]);
	weapon_flags =mods::util::stoi<weapon_type_t>(row["weapon_flags"]);
	bitvector = mods::util::stoi<int>(row["bitvector"]);
	type_flag =mods::util::stoi<int>(row["type_flag"]);
	wear_flags = mods::util::stoi<int>(row["wear_flags"]);
	extra_flags =mods::util::stoi<int>(row["extra_flags"]);
	weight = mods::util::stoi<int>(row["weight"]);
	cost = mods::util::stoi<int>(row["cost"]);
	cost_per_day =mods::util::stoi<int>(row["cost_per_day"]);
	timer = mods::util::stoi<int>(row["timer"]);
}
std::vector<mods::extra_desc_data>& room_data::ex_descriptions() {
	log("DEPRECATED: implement room_data::ex_descriptions or stop calling it");
	return m_ex_descriptions;
}
#ifdef __MENTOC_SHOW_STRUCTS_CPP_DEBUG_OUTPUT__
char_player_data::~char_player_data() {
#ifdef __MENTOC_PLAYER_DATA_DESTRUCTOR_OUTPUT__
	std::cerr << green_str("[char_player_data] DESTRUCTOR HIT\n");
#endif
}
#endif
char_player_data::char_player_data() {
#ifdef __MENTOC_SHOW_STRUCTS_CPP_DEBUG_OUTPUT__
	std::cerr << "[char_player_data()] default constructor\n";
#endif
	name.assign("");         /* PC / NPC s name (kill ...  )         */
	short_descr.assign("");  /* for NPC 'actions'                    */
	long_descr.assign("");   /* for 'look'             */
	description.assign("");  /* Extra descriptions                   */
	title.assign("");        /* PC / NPC's title                     */
	sex = 0;
	chclass = 0;
	level = 0;
	hometown = 0;
	memset(&time,0,sizeof(time));
	weight = 0;
	height = 0;
	/** TODO: phase this out */
	passwd.assign("");
	std::fill(m_passwd.begin(),m_passwd.end(),0);
}
char_data::char_data(char_data* o) {
#ifdef __MENTOC_SHOW_STRUCTS_CPP_DEBUG_OUTPUT__
	std::cerr << "[char_data] ptr copy constructor\n";
#endif
	this->import(o);
}
void char_data::import(const char_data* o) {
#ifdef __MENTOC_SHOW_STRUCTS_CPP_DEBUG_OUTPUT__
	std::cerr << "[char_data::import] using ptr\n";
#endif
	has_desc = false;
	desc = nullptr;
	pfilepos = o->pfilepos;
	uuid = o->uuid;
	last_fight_timestamp = o->last_fight_timestamp;
	nr = o->nr;
	mob_specials.vnum = o->mob_specials.vnum;
	in_room = o->in_room;
	was_in_room = o->was_in_room;
	wait = o->wait;
	drone = o->drone;
	drone_owner = o->drone_owner;
	drone_simulate = o->drone_simulate;
	drone_uuid = o->drone_uuid;

	player = o->player;       /* Normal data                   */
	if(o->player.level < 0) {
		player.level = 0;
	}
	real_abils = o->real_abils;  /* Abilities without modifiers   */
	aff_abils = o->aff_abils;   /* Abils with spells/stones/etc  */


	points = o->points;        /* Points                        */
	char_specials = o->char_specials;  /* PC/NPC specials    */
	player_specials = o->player_specials; /* PC specials      */
	mob_specials.import(o->mob_specials);  /* NPC specials     */
	affected = o->affected;       /* affected by what spells       */
	for(unsigned i = 0; i < NUM_WEARS; i++) {
		equipment[i] = o->equipment[i];/* Equipment array               */
	}
	carrying = o->carrying;            /* Head of list                  */
	desc = o->desc;         /* NULL for mobiles              */
	next = o->next;             /* For either monster or ppl-list  */
	next_fighting = o->next_fighting;    /* For fighting list               */
	followers = o->followers;        /* List of chars followers       */
	master = o->master;             /* Who is char following?        */
	player_ptr = o->player_ptr;
	goal = o->goal;
	disorient = o->disorient;
	state = o->state;
	builder_data = o->builder_data;
}
char_data::~char_data() {
#ifdef __MENTOC_SHOW_STRUCTS_CPP_DEBUG_OUTPUT__
	std::cerr << "~char_data[" << this->uuid << "\n";
#endif
	has_desc = false;
	desc.reset();
	pfilepos = 0;
	uuid = 0;
	last_fight_timestamp = 0;
	nr = 0;
	in_room = 0;
	was_in_room = 0;
	wait = 0;
	drone = 0;
	drone_owner = 0;
	drone_simulate = 0;
	drone_uuid = 0;
	mob_specials.init();
	affected = nullptr;
	for(unsigned i = 0; i < NUM_WEARS; i++) {
		equipment[i] = nullptr;
	}
	carrying = nullptr;
	/** TODO: m_carrying */
	next = nullptr;
	next_fighting = nullptr;
	followers = nullptr;
	master = nullptr;
	player_ptr = nullptr;
	goal = 0;
	disorient = 0;
	state = 0;
	if(this->builder_data) {
		this->builder_data.reset();
	}
	if(this->player_specials) {
		this->player_specials.reset();
	}
}
void char_data::init() {
	visibility = STARTING_VISIBILITY;
	has_desc = false;
	desc.reset();
	pfilepos = 0;
	uuid = 0;
	last_fight_timestamp = 0;
	nr = 0;
	in_room = 0;
	was_in_room = 0;
	wait = 0;
	drone = 0;
	drone_owner = 0;
	drone_simulate = 0;
	drone_uuid = 0;
	mob_specials.init();
	/** TODO: do something smart with this.. maybe call .init() on them
		memset(&real_abils,0,sizeof(real_abils));
		memset(&aff_abils,0,sizeof(aff_abils));
		memset(&char_specials,0,sizeof(char_specials));
		memset(&mob_specials,0,sizeof(mob_specials));
		*/
	affected = nullptr;
	for(unsigned i = 0; i < NUM_WEARS; i++) {
		equipment[i] = nullptr;
	}
	carrying = nullptr;
	/** TODO: m_carrying */
	next = nullptr;
	next_fighting = nullptr;
	followers = nullptr;
	master = nullptr;
	player_ptr = nullptr;
	goal = 0;
	disorient = 0;
	state = 0;
	builder_data.reset();
	player_specials = std::make_shared<player_special_data>();
}

std::string_view room_data::overhead(const lense_type_t& lense) {
	switch(lense) {
		case NORMAL_SIGHT:
			if(std::find(m_textures.begin(),m_textures.end(),texture_type_t::GRASS) != m_textures.end()) {
				return "{grn}==={/grn}";
			}
			if(std::find(m_textures.begin(),m_textures.end(),texture_type_t::CEMENT) != m_textures.end()) {
				return "{grey}[ ]{/grey}";
			}
			if(std::find(m_textures.begin(),m_textures.end(),texture_type_t::OUTSIDE) != m_textures.end()) {
				return "{blu}[ ]{/blu}";
			}
			if(std::find(m_textures.begin(),m_textures.end(),texture_type_t::INSIDE) != m_textures.end()) {
				return "{wht}[ ]{/wht}";
			}
			return "[ ]";
			break;

		case THERMAL_GOGGLES:
			if(std::find(m_textures.begin(),m_textures.end(),texture_type_t::GRASS) != m_textures.end()) {
				return "{blu}...{/blu}";
			}
			if(std::find(m_textures.begin(),m_textures.end(),texture_type_t::CEMENT) != m_textures.end()) {
				return "{blu}...{/blu}";
			}
			if(std::find(m_textures.begin(),m_textures.end(),texture_type_t::OUTSIDE) != m_textures.end()) {
				return "{red}...{/red}";
			}
			/** TODO: if we have mobs in this room, mark it with a red indicator to simulate heat signatures */
			break;

		case NIGHT_VISION_GOGGLES:
			break;


		case AERIAL_DRONE:
			break;

		case AERIAL_DRONE_THERMAL:
			break;

		case AERIAL_DRONE_NIGHT_VISION:
			break;


		case RC_DRONE:
			break;

		case RC_DRONE_THERMAL:
			break;

		case RC_DRONE_NIGHT_VISION:
			break;

		default:
			return "[ ]";
	}
	return "[ ]";
}
std::set<room_data::texture_type_t>& room_data::textures() {
	return m_textures;
}

void room_data::add_texture(texture_type_t t) {
	m_textures.insert(t);
}
void room_data::remove_texture(texture_type_t t) {
	decltype(m_textures) final_textures;
	for(auto&& texture : m_textures) {
		if(texture == t) {
			continue;
		}
		final_textures.insert(texture);
	}
	m_textures = std::move(final_textures);
}

void room_data::init() {
	starting_point = false;
	x = 0;
	y = 0;
	z = 0;
	this->shop_vnum = 0;
	watching = 0;
	number = 0;
	zone = 0;
	sector_type = 0;
	contents = nullptr;
	people = nullptr;
	for(unsigned i = 0; i < NUM_OF_DIRS; i++) {
		this->dir_option[i] = nullptr;
	}
	m_texture_levels[ON_FIRE] = 0;
	name = "name";
	description = "description";
	room_flags = 0;
	light = 1;                  /* Number of lightsources in room     */
	REMOVE_BIT(room_flags, ROOM_DARK);
	func = 0;
}

room_data::room_data() {
	init();
}
room_data::room_data(const room_data& r) {
	this->init();
	number = r.number;
	zone = r.zone;
	sector_type = r.sector_type;
	contents = r.contents;
	people = r.people;
	this->shop_vnum = r.shop_vnum;
	description.assign(r.description.str());
	name.assign(r.name.str());

	this->shop_vnum = r.shop_vnum;
	watching = r.watching;
	for(unsigned i = 0; i < NUM_OF_DIRS; i++) {
		this->dir_option[i] =r.dir_option[i];
	}
	room_flags = r.room_flags;
	light = r.light;
	func = r.func;
	m_texture_levels = r.m_texture_levels;
	m_directions = r.m_directions;
	m_ex_descriptions = r.m_ex_descriptions;
	m_textures = r.m_textures;
	m_contents = r.m_contents;
}
room_data::~room_data() {
	for(unsigned i = 0; i < NUM_OF_DIRS; i++) {
		if(dir_option[i] != nullptr) {
			free(dir_option[i]);
		}
	}
}

/** !zone
 * !integral-objects !integral_objects
 */
#ifdef __MENTOC_SHOW_MATCHES_QUERY_DEBUG_OUTPUT__
#define mq_debug(A) std::cerr << "[matches_query]:'" << A << "'\n";
#else
#define mq_debug(A) /** - */
#endif
/** for the most part, this is crap. clean it up */
bool obj_data::matches_query(std::string_view query) {
	if(query.compare("armor-locker") == 0) {
		mq_debug("AL entry");
	}

	mq_debug("entry");
	mq_debug("feed_file: " << this->feed_file());
	if(has_container()) {
		mq_debug("has container");
		mq_debug("type: " << this->container()->attributes->type);
		mq_debug("locker constant:" << mods::weapon::type::container::WEAPON_LOCKER);
		mq_debug("this->feed_file: " << this->feed_file());
	}
	if(query.compare("weapon-locker") == 0 && has_container() && this->feed_file().compare("weapon-locker.yml") == 0) {
		mq_debug("is a weapon locker");
		return true;
	}
	if(query.compare("armor-locker") == 0 && has_container() && this->feed_file().compare("armor-locker.yml") == 0) {
		mq_debug("is a armor locker");
		return true;
	}
	return false;
#undef mq_debug
}
void room_data::set_dir_option(byte i,
                               const std::string& gen_desc,
                               const std::string& keyword,
                               const int& ex_info,
                               const int& key,
                               const room_rnum to_room) {
	if(i >= NUM_OF_DIRS) {
		return;
	}
	if(this->dir_option[i] == nullptr) {
		/** free is handled properly in room_data destructor */
		this->dir_option[i] = reinterpret_cast<room_direction_data*>(calloc(sizeof(room_direction_data),1));
	} else {
		/** free is handled properly in room_data destructor */
		free(this->dir_option[i]);
		this->dir_option[i] = reinterpret_cast<room_direction_data*>(calloc(sizeof(room_direction_data),1));
	}
	/** FIXME: replace strdup'd members with mods::string */
	this->dir_option[i]->general_description = gen_desc.c_str();
	this->dir_option[i]->keyword = keyword.c_str();
	this->dir_option[i]->exit_info = ex_info;
	/** FIXME: make this dynamic when bugs with builder.cpp are cleaned up */
	this->dir_option[i]->key = 0;
	this->dir_option[i]->to_room = to_room;
	m_directions.emplace_back(i);
}
const std::vector<uint8_t>& room_data::directions() const {
	return m_directions;
}
namespace mods {
	void descriptor_data::queue_output(std::string_view msg, bool newline, bool plain) {
		switch(m_queue_behaviour) {
			case queue_behaviour_enum_t::NORMAL:
				if(plain) {
					output += msg;
				} else {
					output += mods::globals::color_eval(msg);
				}
				if(newline) {
#ifdef __MENTOC_OUTPUT_DEBUGGUNG__
					std::cerr << "descriptor_data::queue_output: newline requested\n";
#endif
					output += "\r\n";
				}
				has_output = true;
				return;
			case queue_behaviour_enum_t::IGNORE_ALL:
				has_output = false;
				return;
			case queue_behaviour_enum_t::REDIRECT_TO_PLAYER:
			case queue_behaviour_enum_t::REDIRECT_TO_FILESYSTEM:
			case queue_behaviour_enum_t::REDIRECT_TO_DB:
			default:
				return;
		}
	}
	size_t mods::descriptor_data::flush_output() {
		std::size_t result;
		switch(m_queue_behaviour) {
			default:
			case queue_behaviour_enum_t::NORMAL:
				if(output.size() == 0) {
					has_output = false;
					return 0;
				}

				result = write_to_descriptor(descriptor,output.c_str());

				/* Handle snooping: prepend "% " and send to snooper. */
				if(snoop_by) {
					write_to_output(*snoop_by, "%% %*s%%%%", static_cast<int>(result), output.c_str());
				}
				output.clear();
				has_output = false;
				return (result);
				break;
			case queue_behaviour_enum_t::IGNORE_ALL:
				has_output = false;
				return 0;
			case queue_behaviour_enum_t::REDIRECT_TO_PLAYER:
			case queue_behaviour_enum_t::REDIRECT_TO_FILESYSTEM:
			case queue_behaviour_enum_t::REDIRECT_TO_DB:
				return 0;
		}
	}

};
bool room_data::has_texture(texture_type_t t) {
	if(m_textures.size() == 0) {
		return false;
	}
	return std::find(m_textures.begin(),m_textures.end(),t) != m_textures.end();
}
void mob_special_data::init() {
	this->memory.clear();
	this->attack_type = 0;
	this->default_pos = 0;
	this->damnodice = 0;
	this->damsizedice = 0;
	this->snipe_tracking = 0;
	this->behaviour_tree = 0;
	this->behaviour_tree_flags = 0;
	this->extended_mob_type = mods::mobs::extended_types_t::NONE;
	this->heading = 0;
	this->previous_room = 0;
}
void mob_special_data::import(const mob_special_data& other) {
	this->watching_room = other.watching_room;
	this->memory = other.memory;
	this->attack_type = other.attack_type;
	this->default_pos = other.default_pos;
	this->damnodice = other.damnodice;
	this->damsizedice = other.damsizedice;
	this->snipe_tracking = other.snipe_tracking;
	this->behaviour_tree = other.behaviour_tree;
	this->behaviour_tree_flags = other.behaviour_tree_flags;
	this->extended_mob_type = other.extended_mob_type;
	this->heading = other.heading;
	this->previous_room = other.previous_room;
	this->vnum = other.vnum;
}
void mob_special_data::report(room_rnum room) {
	for(auto& uuid : this->memory) {
		send_to_room(room, "Report: %s\r\n", CAT("memory:",(uuid)).c_str());
	}
	std::vector<std::string> msg;
#define MR(MSG,VALUE) msg.push_back(std::string(MSG) + std::string(":") + std::to_string(VALUE));
	MR("attack_type",this->attack_type);
	MR("default_pos",this->default_pos);
	MR("damnodice",this->damnodice);
	MR("damsizedice",this->damsizedice);
	MR("snipe_tracking",this->snipe_tracking);
	MR("behaviour_tree",this->behaviour_tree);
	MR("behaviour_tree_flags",this->behaviour_tree_flags);
	MR("extended_mob_type",this->extended_mob_type);
	MR("heading",this->heading);
	MR("previous_room",this->previous_room);
#undef MR
	for(auto m : msg) {
		send_to_room(room, "Report: %s\r\n", m.c_str());
	}
}
void mob_special_data::clear_behaviour_tree() {
	this->behaviour_tree = 0;
}
void mob_special_data::set_behaviour_tree(std::string_view name) {
	this->behaviour_tree = mods::behaviour_tree_impl::grab_tree_by_name(name.data());
}
mob_special_data::~mob_special_data() {
#ifdef __MENTOC_SHOW_STRUCTS_CPP_DEBUG_OUTPUT__
	std::cerr << "[~mob_special_data]\n";
#endif
	this->clear_behaviour_tree();
	this->memory.clear();
	this->attack_type = 0;
	this->default_pos = 0;
	this->damnodice = 0;
	this->damsizedice = 0;
	this->snipe_tracking = 0;
	this->behaviour_tree = 0;
	this->behaviour_tree_flags = 0;
	this->extended_mob_type = mods::mobs::extended_types_t::NONE;
}
char_point_data::char_point_data(const char_point_data& other) {
	std::cerr << "[char_point_data]::copy constructor\n";
	this->mana = other.mana;
	this->max_mana = other.max_mana;
	this->hit = other.hit;
	this->max_hit = other.max_hit;
	this->move = other.move;
	this->max_move = other.max_move;
	this->armor = other.armor;
	this->gold = other.gold;
	this->bank_gold = other.bank_gold;
	this->exp = other.exp;
	this->hitroll = other.hitroll;
	this->damroll = other.damroll;
}
