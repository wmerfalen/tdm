#ifndef __MENTOC_MODS_AFFECTS_HEADER__
#define  __MENTOC_MODS_AFFECTS_HEADER__

#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <forward_list>
#include <memory>

//#define __MENTOC_MODS_AFFECTS_SHOW_DEBUG_OUTPUT__
#ifdef __MENTOC_MODS_AFFECTS_SHOW_DEBUG_OUTPUT__
#define maffects_debug(a) std::cerr << "[mods::affects]" << __FILE__ << "|" << __LINE__ << "->" << a << "\n";
#else
#define maffects_debug(a) /**/
#endif
namespace mods {
	struct player;
};
using player_ptr_t = std::shared_ptr<mods::player>;

#include <functional>
namespace mods::affects {
	using amount_t = int;
	constexpr static amount_t DEFAULT_AMOUNT = 3;
	static constexpr float TRACKED_DAMAGE_BONUS_MULTIPLIER = 0.15;
	enum affect_t : uint16_t {
		NONE=0,
		BLIND,
		DISORIENT,
		POISON,
		INTIMIDATED,
		SCANNED,
		TRACKED,
		CORRODE,
		BLEED,
		SLOWED,
		MELT,
		__AFFECT_SIZE = 9,
	};
	constexpr static std::size_t AFFECT_DISSOLVE_COUNT = affect_t::__AFFECT_SIZE;
	/** i.e.: AFFECT_MAP[affect_t::BLIND] will give us AFF_BLIND */
	constexpr static int AFFECT_MAP[] = {
		(1 << 0),		/** AFF_BLIND */
		(1 << 22),	/** AFF_POISON */
		(1 << 11),		/** AFF_DISORIENT */
		(1 << 6),    /** AFF_INTIMIDATED */
		(1 << 23),		/** AFF_SCANNED */
		(1 << 24),   /** AFF_TRACKED */
	};
	const static std::map<std::string,affect_t> string_map = {
		{"blind",affect_t::BLIND},
		{"disorient",affect_t::DISORIENT},
		{"poison",affect_t::POISON},
		{"intimidated",affect_t::INTIMIDATED},
		{"scanned",affect_t::SCANNED},
		{"tracked",affect_t::TRACKED},
		{"corrode",affect_t::CORRODE},
		{"bleed",affect_t::BLEED},
		{"slowed",affect_t::SLOWED},
		{"melt",affect_t::MELT},
	};
	//using affect_dissolve_t = std::array<uint64_t,AFFECT_DISSOLVE_COUNT>;
	using affect_map_t = std::map<uint32_t,amount_t>;
	using affect_t = mods::affects::affect_t;
	using affect_vector_t = std::vector<affect_t>;

	int apply_tracked_bonus_damage(int damage);

	template <typename TAffects,typename TAffectsContainer,typename TAffectsMap,typename TEntityId>
	struct dissolver {
			using callback_t = std::function<void(TEntityId,TAffects,uint32_t)>;

			TEntityId entity_id;
			void on_zero(TAffects affect,callback_t cb) {
				m_zero_callbacks[affect] = cb;
			}

			void on_affect_change(TAffects affect,callback_t cb) {
				m_callbacks[affect] = cb;
			}
			dissolver(TAffectsMap affects) {
				m_has_affects = true;
				affect_map(affects);
			}
			dissolver(TAffectsContainer affects) {
				m_has_affects = true;
				std::fill(m_affects.begin(),m_affects.end(),0);
				for(auto& item : affects) {
					affect(item);
					d("Affecting item: " << item);
				}
			}
			~dissolver() = default;

			void affect_via(TAffectsContainer affects) {
				for(auto a : affects) {
					affect(a);
				}
			}

			/**
			 * @brief used so that we can define if the affect is added or subtracted each iteration
			 *
			 * @param affect
			 * @param direction true means increment, false decrement
			 */
			void set_direction(TAffects affect, bool direction) {
				if(direction) {
					m_increment.insert(affect);
					return;
				}
				m_increment.erase(affect);
			}
			/**
			 * @brief value that signals the affect to stop
			 *
			 * @param affect
			 * @param max
			 */
			void set_max_amount(TAffects affect, uint32_t max) {
				m_max_amount[affect] = max;
			}

			bool has_all_affects(TAffectsContainer in_affects) {
				for(auto& affect : in_affects) {
					if(!m_affects[affect]) {
						return false;
					}
				}
				return true;
			}
			bool has_any(TAffectsContainer in_affects) {
				for(auto& affect : in_affects) {
					if(m_affects[affect]) {
						return true;
					}
				}
				return false;
			}
			bool has_affect(TAffects a) {
				if(m_affects[a]) {
					maffects_debug("[has_affect] " << a << "amount:" << m_affects[a]);
				}
				return m_affects[a] > 0;
			}
			dissolver() {
				for(auto& affect : m_affects) {
					m_affects[affect.first] = 0;
				}
				m_has_affects = 0;
			}
			void trigger_callback(TAffects affect) {
				if(m_callbacks[affect]) {
					maffects_debug("calling callback via trigger_callback");
					m_callbacks[affect](entity_id,affect,m_affects[affect]);
				}
			}
			void affect_every_n_ticks(TAffects affect,uint32_t starting_amount, uint32_t every_n_ticks) {
				std::cerr << "[affect_every_n_ticks]: starting_amt: " << starting_amount << ", every_n_ticks: " << every_n_ticks << "\n";
				/** this is the original.. i have a feeling it's wrong
				 * m_affects[affect] += starting_amount;
				 */
				m_affects[affect] = starting_amount;
				m_tick_resolution_map[affect] = every_n_ticks;
				m_tick_resolution_map_counter[affect] = 0;
#if 0
				if(m_affects[item.first] == 0) {
					/** FIXME: need to uncomment this->remove() calls and implement that function */
					//this->remove(item.first);
				}
#endif
			}
			void affect_map(TAffectsMap affects) {
				for(auto& item : affects) {
					m_affects[item.first] += item.second;
#if 0
					if(m_affects[item.first] == 0) {
						/** FIXME: need to uncomment this->remove() calls and implement that function */
						//this->remove(item.first);
					}
#endif
				}
			}
			void affect(TAffects aff_id) {
				affect(aff_id,mods::affects::DEFAULT_AMOUNT);
			}
			void clear_all() {
				for(auto pair : m_affects) {
					m_affects[pair.first] = 0;
				}

			}
			void clear(TAffectsContainer affects) {
				for(auto a : affects) {
					m_affects[a] = 0;
					/** FIXME: need to uncomment this->remove() calls and implement that function */
					//this->remove(a);
				}
			}
			void process_affect(TAffects affect) {
				if(m_increment.find(affect) != m_increment.end()) {
					++m_affects[affect];
					++m_processed;
				} else {
					uint64_t i = m_affects[affect];
					if(m_affects[affect] > 0) {
						i = --m_affects[affect];
						++m_processed;
					}
					if(i == 0 && m_zero_callbacks[affect]) {
						maffects_debug("calling zero callback");
						m_zero_callbacks[affect](entity_id,affect,m_affects[affect]);
						m_zero_callbacks.erase(affect);
					}
				}
				trigger_callback(affect);
			}
			bool has_max_amount(TAffects affect) {
				return m_max_amount.find(affect) != m_max_amount.end();
			}
			bool max_amount_reached(TAffects affect) {
				return m_affects[affect] >= m_max_amount[affect];
			}
			void affect(TAffects affect,int amount) {
				m_affects[affect] += amount;
#if 0
				if((m_affects[affect] += amount) == 0) {
					/** FIXME: need to uncomment this->remove() calls and implement that function */
					//this->remove(affect);
				}
#endif
			}
			/**
			 * @brief
			 *
			 * @return
			 */
			std::size_t tick() {
				m_processed = 0;
				std::vector<TAffects> erase_me;
				for(auto& affect : m_affects) {
					bool has_resolution = m_tick_resolution_map.find(affect.first) != m_tick_resolution_map.end();
					if(has_resolution) {
						m_tick_resolution_map_counter[affect.first] = m_tick_resolution_map_counter[affect.first] + 1;
					}
					if(has_resolution && m_tick_resolution_map_counter[affect.first] < m_tick_resolution_map[affect.first]) {
						continue;
					}
					if(has_resolution && m_tick_resolution_map_counter[affect.first] >= m_tick_resolution_map[affect.first]) {
						maffects_debug("processing because map counter equals tick resolution: " << affect.first);
						m_tick_resolution_map_counter[affect.first] = 0;
						process_affect(affect.first);
						continue;
					}
					if(!has_resolution) {
						process_affect(affect.first);
					}
					if(has_max_amount(affect.first) && max_amount_reached(affect.first)) {
						queue_item_to_be_erased(affect.first);
					}
					if(m_increment.find(affect.first) != m_increment.end() && m_max_amount[affect.first] <= affect.second) {
						queue_item_to_be_erased(affect.first);
					}
					if(m_increment.find(affect.first) == m_increment.end() && affect.second == 0) {
						queue_item_to_be_erased(affect.first);
					}
				}
				erase_queued_items();
				return m_affects.size();
			}
			void queue_item_to_be_erased(TAffects affect) {
				m_destroy_queue.emplace_back(affect);
			}
			void erase_queued_items() {
				for(auto& m : m_destroy_queue) {
					m_affects.erase(m);
				}
				m_has_affects = m_affects.size();
			}

			void remove(TAffects id) {
				/** FIXME: need to uncomment this->remove() calls and implement that function */
				/** FIXME: need to uncomment this->remove() calls and implement that function */
			}
			bool has_any_affect(TAffectsContainer in_affects) {
				for(auto& affect : in_affects) {
					if(m_affects[affect]) {
						return true;
					}
				}
				return false;
			}

			TAffectsMap& get_affects() {
				return m_affects;
			}

			void set_callback(TAffects affect, std::function<void(TEntityId,TAffects,uint32_t)> f) {
				m_callbacks[affect] = f;
			}

		protected:
			bool m_has_affects;
			TAffectsMap m_affects;
			std::map<TAffects,uint32_t> m_tick_resolution_map;
			std::map<TAffects,uint32_t> m_tick_resolution_map_counter;
			std::set<TAffects> m_increment;
			std::map<TAffects,uint32_t> m_max_amount;
			std::map<TAffects,std::function<void(TEntityId,TAffects,uint32_t)>> m_callbacks;
			std::map<TAffects,std::function<void(TEntityId,TAffects,uint32_t)>> m_zero_callbacks;
			std::size_t m_processed;
			std::vector<TAffects> m_destroy_queue;
	};

	void process();
	uint32_t get_ticks_per_minute();
	mods::affects::affect_vector_t to_affect(std::vector<std::string> a);
	void affect_player(affect_vector_t a,player_ptr_t p);
	void affect_player_for(affect_vector_t a,player_ptr_t p,uint64_t ticks);
	void affect_player_str(std::vector<std::string> a,player_ptr_t p);
	void str_affect_player(std::vector<std::string> a,player_ptr_t p);
	std::vector<std::string>& affect_string_list_impl();
	void player_died(player_ptr_t p);
	void blind_for(player_ptr_t& victim,uint16_t ticks);
};
std::vector<std::string>& affect_string_list();
void str_affect_player(std::vector<std::string> a, player_ptr_t p);
void str_queue_on_room(std::vector<std::string> affects,int room_id);
void str_queue_on_player(std::vector<std::string> affects,player_ptr_t player);
void queue_on_room(std::vector<mods::affects::affect_t> affects,int room_id);
void queue_on_player(std::vector<mods::affects::affect_t> affects,int room_id);
#define AFF(a) mods::affects::affect_t::a
#endif

