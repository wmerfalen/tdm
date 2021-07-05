#ifndef __MENTOC_MODS_deferred_HEADER__
#define __MENTOC_MODS_deferred_HEADER__

#include <iostream>
#include <functional>
#include <map>
#include <sys/socket.h>
#include <netdb.h>
#include "../conf.h"
#include "../sysdep.h"
#include "../structs.h"
#include "../types.hpp"
#include <tuple>

namespace mods::util {
	extern void texturize_room(room_rnum room_id, room_data::texture_type_t& texture_type);
	extern void detexturize_room(room_rnum room_id, room_data::texture_type_t& texture_type);
};
namespace mods::globals {
	extern std::vector<std::vector<std::shared_ptr<mods::player>>> room_list;
	void destruct_object(uuid_t);
};
namespace mods {
	struct chunk_affect_t {
		uuid_t player_uuid;
		std::size_t chunk;
		uint64_t bit;
		int64_t amount;
		std::size_t number_of_times;
		bool until_zero;
		uint64_t next_event_tick;
		uint64_t add;
	};
	class deferred {
		public:
			constexpr static uint32_t EVENT_OBJECT_DESTRUCT = 0;
			constexpr static uint32_t EVENT_PLAYER_UNBLOCK_INSTALLATION = 1;
			constexpr static uint32_t EVENT_GET_ATTACKED = 2;
			constexpr static uint32_t EVENT_PLAYER_UNBLOCK_BREACH = 3;
			constexpr static uint32_t EVENT_PLAYER_REVIVE_SUCCESSFUL = 4;
			constexpr static uint32_t EVENT_PLAYER_FINISHES_FEIGN_DEATH = 5;
			constexpr static uint32_t EVENT_PLAYER_GOES_VISIBLE = 6;
			constexpr static uint32_t EVENT_WEAPON_COOLDOWN_FINISHED = 7;
			constexpr static uint32_t EVENT_PLAYER_UNBLOCK_HEALING = 8;
			constexpr static uint64_t TICK_RESOLUTION = 3;
			using seconds = uint16_t;
			using lambda_queue_t = std::multimap<uint64_t,std::function<void()>>;
			using event_queue_t = std::multimap<uint32_t,std::tuple<uuid_t,uint32_t>>;
			using lambda_queue_iterator = lambda_queue_t::iterator;
			using event_queue_iterator = event_queue_t::iterator;

			deferred() = delete;
			deferred(uint64_t tick_resolution) :
				m_tres(tick_resolution),
				m_tick(0),
				m_iterations(0),
				m_time_tracker(time(nullptr)),
				m_ticks_per_minute(0),
				m_ticks_per_minute_sample(0) {
				/** initialized */
			}
			~deferred() = default;
			lambda_queue_iterator push(uint64_t ticks_in_future,std::function<void()> lambda);
			event_queue_iterator push_ticks_event(uint32_t ticks, std::tuple<uuid_t,uint32_t> type);
			event_queue_iterator push_ticks_event(uint32_t ticks, uuid_t,uint32_t type);
			void push_chunk_affect(
			    uuid_t player_uuid,
			    std::size_t chunk,
			    uint64_t bit,
			    int64_t amount,
			    std::size_t number_of_times,
			    bool until_zero,
			    uint64_t next_event_tick,
			    uint64_t add
			);
			void push_weapon_cooldown(const uint16_t& ticks, const uuid_t& player_uuid);
			void push_step(uint16_t ticks,std::size_t hash,const uuid_t& player_uuid);
			void iteration();
			void detexturize_room(uint64_t ticks_in_future,room_rnum room_id,room_data::texture_type_t texture);
			template <typename TTextureList>
			void texturize_room(uint64_t ticks_in_future,room_rnum& room_id,TTextureList& textures) {
				m_q.insert(
				std::make_pair(ticks_in_future + m_tick,[&]() {
					for(auto& texture : textures) {
						mods::util::texturize_room(room_id,texture);
					}
				})
				);//end insert
			}
			void cancel_lambda(lambda_queue_iterator it);
			void cancel_event(event_queue_iterator it);
			uint32_t get_ticks_per_minute();
		protected:
			lambda_queue_t m_q;
			event_queue_t m_ticks_event_type;
			uint64_t m_tres;
			uint64_t m_tick;
			uint64_t m_iterations;
			std::vector<chunk_affect_t> m_chunk_affect;
			time_t m_time_tracker;
			uint32_t m_ticks_per_minute;
			uint32_t m_ticks_per_minute_sample;
		private:
			void tick();
	};
};

#define DEFER_TICKS(ticks,lambda) mods::globals::defer_queue->push(ticks,lambda);
#define DEFER_STEP(ticks,step,player_uuid) mods::globals::defer_queue->push_step(ticks,step,player_uuid);

#endif
