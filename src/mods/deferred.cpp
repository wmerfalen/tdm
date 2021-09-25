#include "deferred.hpp"
#include "player.hpp"
#include "scripted-sequence-runner.hpp"

extern std::deque<room_data> world;
extern player_ptr_t ptr_by_uuid(uuid_t);
namespace mods::corpse {
	extern void explode(const uuid_t&);
};
namespace mods {
	deferred::lambda_queue_iterator deferred::push(uint64_t ticks_in_future,std::function<void()> lambda) {
		return m_q.insert(std::make_pair(ticks_in_future + m_tick,lambda));
	}
	deferred::event_queue_iterator  deferred::push_ticks_event(uint32_t ticks, uuid_t player_uuid,uint32_t ui_type) {
		std::tuple<uuid_t,uint32_t> type = {player_uuid,ui_type};
		return m_ticks_event_type.insert(std::make_pair(m_tick + ticks, type));
	}
	deferred::event_queue_iterator  deferred::push_ticks_event(uint32_t ticks, std::tuple<uuid_t,uint32_t> type) {
		return m_ticks_event_type.insert(std::make_pair(m_tick + ticks, type));
	}
	void deferred::cancel_lambda(lambda_queue_iterator it) {
		m_q.erase(it);
	}
	void deferred::cancel_event(event_queue_iterator it) {
		m_ticks_event_type.erase(it);
	}
	void deferred::push_weapon_cooldown(const uint16_t& ticks, const uuid_t& player_uuid) {
		push_ticks_event(ticks,player_uuid,EVENT_WEAPON_COOLDOWN_FINISHED);
	}
	uint32_t deferred::get_ticks_per_minute() {
		return m_ticks_per_minute_sample;
	}
	void deferred::push_chunk_affect(
	    uuid_t player_uuid,
	    std::size_t chunk,
	    uint64_t bit,
	    int64_t amount,
	    std::size_t number_of_times,
	    bool until_zero,
	    uint64_t next_event_tick,
	    uint64_t add) {

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
		auto seconds = time(nullptr);
		if((seconds - m_time_tracker) >= 60) {
			m_ticks_per_minute_sample = m_ticks_per_minute;
			m_ticks_per_minute = 0;
			m_time_tracker = seconds;
		}
		m_ticks_per_minute++;
		m_tick++;
		auto fe = m_ticks_event_type.find(m_tick);
		if(fe != m_ticks_event_type.end()) {
			auto fe_tuple = fe->second;
			std::shared_ptr<mods::player> player = nullptr;
			switch(std::get<1>(fe_tuple)) {
				case deferred::EVENT_CORPSE_EXPLODE:
					mods::corpse::explode(std::get<0>(fe_tuple));
					break;
				case deferred::EVENT_OBJECT_DESTRUCT:
					mods::globals::destruct_object(std::get<0>(fe_tuple));
					break;
				case deferred::EVENT_PLAYER_UNBLOCK_INSTALLATION:
				default:
					player = ptr_by_uuid(std::get<0>(fe_tuple));
					if(!player) {
						break;
					}
					player->unblock_event(std::get<1>(fe_tuple));
					break;
			}
			m_ticks_event_type.erase(m_tick);
		}
		auto f = m_q.find(m_tick);

		if(f != m_q.end()) {
			f->second();
			m_q.erase(m_tick);
		}
	}
	void deferred::iteration() {
		++m_iterations;
		if(m_iterations >= m_tres) {
			this->tick();
			m_iterations = 0;
		}
	}
	void push_ticks_event(uint32_t ticks, std::tuple<uuid_t,uint32_t> type) {

	}
	void deferred::detexturize_room(uint64_t ticks_in_future,room_rnum room_id,room_data::texture_type_t texture) {
		m_q.insert(
		std::make_pair(ticks_in_future + m_tick,[&]() {
			world[room_id].remove_texture(texture);
		})
		);
	}
	void deferred::push_step(uint16_t ticks, std::size_t hash,const uuid_t& player_uuid) {
		m_q.insert(
		std::make_pair(ticks + m_tick,[=]() {
#ifdef __MENTOC_SHOW_SCRIPTED_STEP_RUNNER_DEBUG__
			std::cerr << green_str("running scripted step runner...") << "\n";
#endif
			mods::scripted_sequence_runner::step_runner(hash,player_uuid);
		})
		);

	}
};

