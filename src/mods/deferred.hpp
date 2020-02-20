#ifndef __MENTOC_MODS_deferred_HEADER__
#define  __MENTOC_MODS_deferred_HEADER__

#include <iostream>
#include <functional>
#include <map>
#include <sys/socket.h>
#include <netdb.h>
#include "../conf.h"
#include "../sysdep.h"
#include "../structs.h"
#include "../types.hpp"

namespace mods::util {
	extern void texturize_room(room_rnum room_id, room_data::texture_type_t& texture_type);
	extern void detexturize_room(room_rnum room_id, room_data::texture_type_t& texture_type);
};
namespace mods::globals {
	extern std::vector<std::vector<std::shared_ptr<mods::player>>> room_list;
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
			constexpr static uint64_t TICK_RESOLUTION = 3;
			using seconds = uint16_t;
			deferred() = delete;
			deferred(uint64_t tick_resolution) : m_tres(tick_resolution),
				m_tick(0), m_iterations(0) {

			}
			~deferred() = default;
			/* TODO: Operator << for sending to the character */
			deferred& operator<<(const char* m) {
				return *this;
			}
			deferred& operator<<(const std::string m) {
				return *this;
			}
			deferred& operator<<(int m) {
				return *this;
			}
			void push(uint64_t ticks_in_future,std::function<void()> lambda);
			void push_secs(seconds secs,std::function<void()> lambda);
			void push_chunk_affect(
		uuid_t player_uuid,
		std::size_t chunk,
		uint64_t bit,
		int64_t amount,
		std::size_t number_of_times,
		bool until_zero,
		uint64_t next_event_tick,
		uint64_t add);
			/*
			void push_chunk_affect(uint64_t ticks_in_future,uuid_t player,std::size chunk, uint64_t bit,int64_t amount, std::size_t number_of_times);
			void push_interval_chunk_affect(uint64_t every_tick,uuid_t player,std::size chunk, uint64_t bit,int64_t amount, bool until_zero);
			*/
			void tick();
			void iteration();
			void detexturize_room(uint64_t ticks_in_future,room_rnum& room_id,room_data::texture_type_t texture);
			template <typename TTextureList>
			void texturize_room(uint64_t ticks_in_future,room_rnum& room_id,TTextureList& textures){
				m_q.insert(std::make_pair(ticks_in_future + m_tick,[&](){
					for(auto & texture : textures){
						mods::util::texturize_room(room_id,texture);
					}
				})
				);//end insert
			}

		protected:
			std::multimap<uint64_t,std::function<void()>> m_q;
			std::multimap<seconds,std::function<void()>> m_secs;
			uint64_t m_tres;
			uint64_t m_tick;
			uint64_t m_iterations;
			std::vector<chunk_affect_t> m_chunk_affect;
	};
};

#endif
