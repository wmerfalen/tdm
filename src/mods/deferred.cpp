#include "deferred.hpp"
extern std::vector<room_data> world;
namespace mods {
	void deferred::push(uint64_t ticks_in_future,std::function<void()> lambda) {
		m_q.insert(std::make_pair(ticks_in_future + m_tick,lambda));
	}
	void deferred::push_secs(seconds secs,std::function<void()> lambda) {
		m_secs.insert(std::make_pair(time(NULL) + secs,lambda));
	}
	void deferred::push_chunk_affect(
			uuid_t player_uuid,
			std::size_t chunk,
			uint64_t bit,
			int64_t amount,
			std::size_t number_of_times,
			bool until_zero,
			uint64_t next_event_tick,
			uint64_t add){
		
		chunk_affect_t ca;
		ca.player_uuid = player_uuid;
		ca.chunk = chunk;
		ca.bit = bit;
		ca.amount = amount;
		ca.number_of_times = number_of_times;
		ca.until_zero = until_zero;
		ca.next_event_tick = next_event_tick;
		ca.add = add;
		m_chunk_affect.emplace_back(ca);
	}
	void deferred::tick() {
	//	for(auto & chunk : m_chunk_affect){
	//		if(m_tick == ca.next_event_tick){
	//			--ca.number_of_times;
	//			auto player = ptr_by_uuid(ca.player_uuid);
	//			player->


	//	}
	}
	void deferred::iteration() {
		{
			auto now = static_cast<unsigned long>(time(NULL));
			auto f = m_secs.find(now);

			if(f != m_secs.end()) {
				f->second();
				m_secs.erase(now);
			}
		}
		++m_iterations;

		if(m_iterations >= m_tres) {
			m_tick++;
			auto f = m_q.find(m_tick);

			if(f != m_q.end()) {
				f->second();
				m_q.erase(m_tick);
			}

			m_iterations = 0;
			// every tick, check the chunk affect
			this->tick();
		}
	}
	void deferred::detexturize_room(uint64_t ticks_in_future,room_rnum& room_id,room_data::texture_type_t texture){
		m_q.insert(std::make_pair(ticks_in_future + m_tick,[&](){
					world[room_id].remove_texture(texture);
					})
				);
	}
};

