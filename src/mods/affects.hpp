#ifndef __MENTOC_MODS_AFFECTS_HEADER__
#define  __MENTOC_MODS_AFFECTS_HEADER__

#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <forward_list>
#include <memory>

#define __MENTOC_MODS_AFFECTS_SHOW_DEBUG_OUTPUT__
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
	enum affect_t {
		NONE = -1,
		BLIND = 0,
		DISORIENT = 1,
		POISON = 2,
		INTIMIDATED = 3,
		SCANNED = 4,
		__AFFECT_SIZE = 5
	};
	constexpr static std::size_t AFFECT_DISSOLVE_COUNT = affect_t::__AFFECT_SIZE;
	/** i.e.: AFFECT_MAP[affect_t::BLIND] will give us AFF_BLIND */
	constexpr static int AFFECT_MAP[] = {
		(1 << 0),		/** AFF_BLIND */
		(1 << 22),	/** AFF_POISON */
		(1 << 11),		/** AFF_DISORIENT */
		(1 << 6 ),   /** AFF_INTIMIDATED */
		(1 << 23)		/** AFF_SCANNED */
	};
	const static std::map<std::string,affect_t> string_map = {
		{"blind",affect_t::BLIND},
		{"disorient",affect_t::DISORIENT},
		{"poison",affect_t::POISON},
		{"intimidated",affect_t::INTIMIDATED},
		{"scanned",affect_t::SCANNED}
	};
	//using affect_dissolve_t = std::array<uint64_t,AFFECT_DISSOLVE_COUNT>;
	using affect_map_t = std::map<uint32_t,amount_t>;
	using affect_t = mods::affects::affect_t;
	using affect_vector_t = std::vector<affect_t>;

	template <typename TAffects,typename TAffectsContainer,typename TAffectsMap,typename TEntityId>
		struct dissolver {
			using callback_t = std::function<void(TEntityId,TAffects,uint32_t)>;

			TEntityId entity_id;

			void on_affect_change(TAffects affect,callback_t cb){
				m_callbacks[affect] = cb;
			}
			dissolver(TAffectsMap affects){
				m_has_affects = true;
				affect_map(affects);
			}
			dissolver(TAffectsContainer affects){
				m_has_affects = true;
				std::fill(m_affects.begin(),m_affects.end(),0);
				for(auto& item : affects){
					affect(item);
					d("Affecting item: " << item);
				}
			}
			~dissolver() = default;

			void affect_via(TAffectsContainer affects){
				for(auto a : affects){
					affect(a);
				}
			}

			/**
			 * @brief set affect direction
			 *
			 * @param affect
			 * @param direction true means increment, false decrement
			 */
			void set_direction(TAffects affect, bool direction){
				if(direction){
					m_increment.insert(affect);
					return;
				}
				m_increment.erase(affect);
			}
			void set_max_amount(TAffects affect, uint32_t max){
				m_max_amount[affect] = max;
			}

			bool has_all_affects(TAffectsContainer in_affects){
				for(auto & affect : in_affects){
					if(!m_affects[affect]) {
						return false;
					}
				}
				return true;
			}
			bool has_affect(TAffects a){
				maffects_debug("[has_affect] " << a);
				return m_affects[a];
			}
			dissolver() {
				for(auto & affect : m_affects){
					m_affects[affect.first] = 0;
				}
				m_has_affects = 0;
			}
			void trigger_callback(TAffects affect){
				if(m_callbacks[affect]){
					maffects_debug("calling callback via trigger_callback");
					m_callbacks[affect](entity_id,affect,m_affects[affect]);
				}
			}
			void affect_every_n_ticks(TAffects affect,uint32_t starting_amount, uint32_t every_n_ticks){
				std::cerr << "[affect_every_n_ticks]: starting_amt: " << starting_amount << ", every_n_ticks: " << every_n_ticks << "\n";
				/** this is the original.. i have a feeling it's wrong
				 * m_affects[affect] += starting_amount;
				 */
				  m_affects[affect] = starting_amount;
					m_tick_resolution_map[affect] = every_n_ticks;
					m_tick_resolution_map_counter[affect] = 0;
#if 0
					if(m_affects[item.first] == 0){
	/** FIXME: need to uncomment this->remove() calls and implement that function */
						//this->remove(item.first);
					}
#endif
				}
			void affect_map(TAffectsMap affects){
				for(auto& item : affects){
					m_affects[item.first] += item.second;
#if 0
					if(m_affects[item.first] == 0){
	/** FIXME: need to uncomment this->remove() calls and implement that function */
						//this->remove(item.first);
					}
#endif
				}
			}
			void affect(TAffects aff_id){
				affect(aff_id,mods::affects::DEFAULT_AMOUNT);
			}
			void clear(TAffectsContainer affects){
				for(auto a : affects){
					m_affects[a] = 0;
	/** FIXME: need to uncomment this->remove() calls and implement that function */
					//this->remove(a);
				}
			}
			void process_affect(TAffects affect){
				if(m_increment.find(affect) != m_increment.end()){
					++m_affects[affect];
				}else{
					--m_affects[affect];
				}
				++m_processed;
				trigger_callback(affect);
			}
			void affect(TAffects affect,int amount){
				m_affects[affect] += amount;
#if 0
				if((m_affects[affect] += amount) == 0){
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
			std::size_t tick(){
				m_processed = 0;
				std::vector<TAffects> erase_me;
				for(auto & affect : m_affects){
					bool has_resolution = m_tick_resolution_map.find(affect.first) != m_tick_resolution_map.end();
					if(has_resolution){
						m_tick_resolution_map_counter[affect.first] = m_tick_resolution_map_counter[affect.first] + 1;
					}
					if(has_resolution && m_tick_resolution_map_counter[affect.first] < m_tick_resolution_map[affect.first]){
						continue;
					}
					if(has_resolution && m_tick_resolution_map_counter[affect.first] >= m_tick_resolution_map[affect.first]){
							maffects_debug("processing because map counter equals tick resolution: " << affect.first );
							m_tick_resolution_map_counter[affect.first] = 0;
							process_affect(affect.first);
							continue;
					}
					if(!has_resolution){
						process_affect(affect.first);
					}
					if(m_increment.find(affect.first) != m_increment.end() && m_max_amount[affect.first] <= affect.second){
						erase_me.push_back(affect.first);
					}
					if(m_increment.find(affect.first) == m_increment.end() && affect.second == 0){
						erase_me.push_back(affect.first);
					}
				}
				for(auto && item : erase_me){
					if(m_affects.find(item) != m_affects.end()){
						m_affects.erase(item);
					}
				}
				m_has_affects = m_affects.size();
				return m_affects.size();
			}
			void remove(TAffects id){
	/** FIXME: need to uncomment this->remove() calls and implement that function */
	/** FIXME: need to uncomment this->remove() calls and implement that function */
			}
	bool has_any_affect(TAffectsContainer in_affects){
			for(auto & affect : in_affects){
				if(m_affects[affect]) {
					return true;
				}
			}
			return false;
		}

		TAffectsMap& get_affects(){ return m_affects; }

		void set_callback(TAffects affect, std::function<void(TEntityId,TAffects,uint32_t)> f){
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
			std::size_t m_processed;
		};

	void process();
	uint32_t get_ticks_per_minute();
	mods::affects::affect_vector_t to_affect(std::vector<std::string> a);
	void affect_player(affect_vector_t a,player_ptr_t p);
	void affect_player_for(affect_vector_t a,player_ptr_t p,uint64_t ticks);
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

