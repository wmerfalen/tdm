#include "deferred.hpp"
extern std::vector<room_data> world;
namespace mods {
	void deferred::push(uint64_t ticks_in_future,std::function<void()> lambda) {
		m_q.insert(std::make_pair(ticks_in_future + m_tick,lambda));
	}
	void deferred::push_secs(seconds secs,std::function<void()> lambda) {
		m_secs.insert(std::make_pair(time(NULL) + secs,lambda));
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
		}
	}
	void deferred::detexturize_room(uint64_t ticks_in_future,room_rnum& room_id,room_data::texture_type_t texture){
		m_q.insert(std::make_pair(ticks_in_future + m_tick,[&](){
				world[room_id].remove_texture(texture);
			})
		);
	}
};

