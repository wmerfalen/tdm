#ifndef __MENTOC_MODS_AFFECTS_HEADER__
#define  __MENTOC_MODS_AFFECTS_HEADER__

#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <forward_list>
#include <memory>

namespace mods {
	struct player;
};
using player_ptr_t = std::shared_ptr<mods::player>;

namespace mods::affects {
using amount_t = int;
constexpr static amount_t DEFAULT_AMOUNT = 3;
enum affect_t {
	NONE = -1,
	BLIND = 0,
	DISORIENT = 1,
	POISON = 2,
	INTIMIDATED = 3,
	__AFFECT_SIZE = 4
};
constexpr static std::size_t AFFECT_DISSOLVE_COUNT = affect_t::__AFFECT_SIZE;
/** i.e.: AFFECT_MAP[affect_t::BLIND] will give us AFF_BLIND */
constexpr static int AFFECT_MAP[] = {
	(1 << 0),		/** AFF_BLIND */
	(1 << 22),	/** AFF_POISON */
	(1 << 11),		/** AFF_DISORIENT */
	(1 << 6 ),   /** AFF_INTIMIDATED */
};
const static std::map<std::string,affect_t> string_map = {
	{"blind",affect_t::BLIND},
	{"disorient",affect_t::DISORIENT},
	{"poison",affect_t::POISON},
	{"intimidated",affect_t::INTIMIDATED}
};
using affect_dissolve_t = std::array<uint64_t,AFFECT_DISSOLVE_COUNT>;
using affect_map_t = std::map<affect_t,amount_t>;
using affect_t = mods::affects::affect_t;
using affect_vector_t = std::vector<affect_t>;

	struct dissolver {
		void affect(affect_t affect);
		void affect(affect_t affect,amount_t amt);

		void remove(affect_t id);
		void clear(std::vector<affect_t> affects);

		template <typename T>
		void affect_via(T affects){
			for(auto a : affects){
				affect(a);
			}
		}
		void affect_map(affect_map_t afmap);

		std::size_t tick();
		dissolver();
		template <typename T>
		dissolver(T affects){
			m_has_affects = true;
			std::fill(m_affects.begin(),m_affects.end(),0);
			for(auto& item : affects){
				affect(item);
				d("Affecting item: " << item);
			}
		}
		~dissolver() = default;
		protected:
			bool m_has_affects;
			affect_dissolve_t m_affects;
	};

	void process();
	mods::affects::affect_vector_t to_affect(std::vector<std::string> a);
	void affect_player(affect_vector_t a,player_ptr_t p);
	void affect_player_str(std::vector<std::string> a,player_ptr_t p);
	void str_affect_player(std::vector<std::string> a,player_ptr_t p);
	std::vector<std::string>& affect_string_list_impl();
};
std::vector<std::string>& affect_string_list();
void str_affect_player(std::vector<std::string> a, player_ptr_t p);
void str_queue_on_room(std::vector<std::string> affects,int room_id);
void str_queue_on_player(std::vector<std::string> affects,player_ptr_t player);
void queue_on_room(std::vector<mods::affects::affect_t> affects,int room_id);
void queue_on_player(std::vector<mods::affects::affect_t> affects,int room_id);
#define AFF(a) mods::affects::affect_t::a
#endif

