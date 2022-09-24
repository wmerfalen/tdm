#include "room-data.hpp"

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
	(room_flags) &= ~(ROOM_DARK);
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
	for(const auto& c : r.containers) {
		containers.push_front(c);
	}
}
room_data::~room_data() {
	for(unsigned i = 0; i < NUM_OF_DIRS; i++) {
		if(dir_option[i] != nullptr) {
			dir_option[i]->general_description.clear();
			dir_option[i]->keyword.clear();
			free(dir_option[i]);
			dir_option[i] = nullptr;
		}
	}
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
std::vector<mods::extra_desc_data>& room_data::ex_descriptions() {
	return m_ex_descriptions;
}
void room_data::add_textures(std::set<texture_type_t> m) {
	for(auto t : m) {
		add_texture(t);
	}
}
bool room_data::has_texture(texture_type_t t) {
	if(m_textures.size() == 0) {
		return false;
	}
	return std::find(m_textures.begin(),m_textures.end(),t) != m_textures.end();
}

const std::vector<uint8_t>& room_data::directions() const {
	return m_directions;
}

room_data::fire_status_t& room_data::fire_status() {
	return reinterpret_cast<room_data::fire_status_t&>(m_texture_levels[ON_FIRE]);
}
void room_data::fire_status_start() {
	m_texture_levels[ON_FIRE] = (room_data::texture_level_t)room_data::fire_status_t::KINDLING;
}
/** END fire status */
room_data::texture_level_t& room_data::texture_level(texture_type_t type) {
	return m_texture_levels[type];
}

