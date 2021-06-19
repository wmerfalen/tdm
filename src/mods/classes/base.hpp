#ifndef __MENTOC_MODS_CLASSES_BASE_HEADER__
#define  __MENTOC_MODS_CLASSES_BASE_HEADER__

#include "types.hpp"
#include "../weapon.hpp"
#include "../bugs-fixtures.hpp"
#include <map>
#include <string>
#include "../orm/util.hpp"
#include "../player.hpp"
#include "../rand.hpp"

namespace mods::classes {
	struct skill_t {
			static constexpr const char* table_name = "skill_usage";
			virtual float get_increment() {
				return 0.5;
			}
			virtual float get_usage_increment() {
				return 1.0;
			}
			skill_t() = default;

			std::tuple<bool,std::string> load_skill_by_player(std::string_view skill_name, player_ptr_t& player) {
				m_db_id = 0;
				m_name = skill_name;
				m_current_level = 0.0;
				m_loaded_properly = false;
				m_load_status = load(player,skill_name);
				return m_load_status;
			}
			const float& get_current_level() const {
				return m_current_level;
			}
			void use_skill(player_ptr_t& player) {
				m_current_level += get_usage_increment();
				save(player);
			}
			void practice(player_ptr_t& player) {
				m_current_level += get_increment();
				auto s = save(player);
				if(std::get<0>(s)) {
					player->sendln("You practice for awhile...");
				} else {
					player->sendln("You can't seem to practice right now.");
					log(CAT("SYSERR: practicing for player: ",player->name().c_str(),": ",red_str(std::get<1>(s))).c_str());
				}
			}
			std::map<std::string,std::string>& export_class() {
				m_data.clear();
				m_data["skill_name"] = m_name;
				m_data["skill_level"] = std::to_string(m_current_level);
				return m_data;
			}
			static constexpr float NOT_LEARNED = 0.0;
			static constexpr float AWFUL = 50.0;
			static constexpr float OKAY = 250.0;
			static constexpr float LEARNED = 1000.0;
			static constexpr float MASTER = 10000.0;
			static constexpr float ELITE = 50000.0;

			bool not_learned() const {
				return m_current_level == 0;
			}

			bool terrible() const {
				return m_current_level > NOT_LEARNED && m_current_level <= AWFUL;
			}
			bool awful() const {
				return m_current_level > AWFUL && m_current_level <= OKAY;
			}
			bool okay() const {
				return m_current_level > OKAY && m_current_level <= LEARNED;
			}
			bool learned() const {
				return m_current_level > LEARNED && m_current_level <= MASTER;
			}
			bool mastered() const {
				return m_current_level > MASTER && m_current_level <= ELITE;
			}
			bool elite() const {
				return m_current_level >= ELITE;
			}

			std::string get_proficiency() const {
				if(not_learned()) {
					return "not-learned";
				}
				if(terrible()) {
					return "terrible";
				}
				if(awful()) {
					return "awful";
				}
				if(okay()) {
					return "okay";
				}
				if(learned()) {
					return "learned";
				}
				if(mastered()) {
					return "mastered";
				}
				if(elite()) {
					return "elite";
				}
				return "not-learned";
			}
			int16_t feed(const pqxx::result::reference& row) {
				m_name = row["skill_name"].c_str();
				m_current_level = row["skill_level"].as<float>();
				return 0;
			}
			std::tuple<bool,std::string> load(player_ptr_t& player,std::string_view skill_name) {
				try {
					auto check_txn = txn();
					auto check_sql = sql_compositor(table_name,&check_txn)
					                 .select("*")
					                 .from(table_name)
					                 .where("player_id",player->db_id())
					                 .op_and("skill_name",skill_name)
					                 .sql();
					auto check_result = mods::pq::exec(check_txn,check_sql);
					mods::pq::commit(check_txn);
					if(check_result.size()) {
						m_current_level = check_result[0]["skill_level"].as<float>();
						m_db_id = check_result[0]["id"].as<uint64_t>();
						m_loaded_properly = true;
					} else {
						/* insert the record */
						auto t = txn();
						auto sql = sql_compositor(table_name,&t)
						           .insert()
						           .into(table_name)
						.values({
							{"skill_name",skill_name.data()},
							{"skill_level","0.0"},
							{"player_id",std::to_string(player->db_id())}
						})
						.returning("id")
						.sql();
						auto row = mods::pq::exec(t,sql);
						mods::pq::commit(t);
						if(row.size()) {
							m_db_id = row[0]["id"].as<uint64_t>();
							m_loaded_properly = true;
						} else {
							m_db_id = 0;
							m_loaded_properly = false;
							return {false,"pkid not returned!"};
						}
					}
				} catch(const std::exception& e) {
					return {false,e.what()};
				}
				return {true,"success"};
			}
			std::tuple<bool,std::string> save(player_ptr_t& player) {
				try {
					auto check_txn = txn();
					auto check_sql = sql_compositor(table_name,&check_txn)
					                 .select("id")
					                 .from(table_name)
					                 .where("player_id",player->db_id())
					                 .op_and("skill_name",m_name)
					                 .sql();
					auto check_result = mods::pq::exec(check_txn,check_sql);
					mods::pq::commit(check_txn);
					if(check_result.size()) {
						/* update the record */
						auto t = txn();
						auto sql = sql_compositor(table_name,&t)
						           .update(table_name)
						.set({{"skill_level",std::to_string(m_current_level)}})
						.where("id",check_result[0]["id"].c_str())
						.sql();
						mods::pq::exec(t,sql);
						mods::pq::commit(t);
					} else {
						/* insert the record */
						auto t = txn();
						auto sql = sql_compositor(table_name,&t)
						           .insert()
						           .into(table_name)
						.values({
							{"skill_name",m_name},
							{"skill_level",std::to_string(m_current_level)},
							{"player_id",std::to_string(player->db_id())}
						})
						.sql();
						mods::pq::exec(t,sql);
						mods::pq::commit(t);
					}
				} catch(const std::exception& e) {
					return {false,e.what()};
				}
				return {true,"success"};
			}

		private:
			std::string m_name;
			float m_current_level;
			std::map<std::string,std::string> m_data;
			uint64_t m_db_id;
			bool m_loaded_properly;
			std::tuple<bool,std::string> m_load_status;
	};
	struct base {
		virtual types kind() {
			return types::UNDEFINED;
		}
		virtual std::string skills_page() {
			return "[stub]";
		}
		virtual std::tuple<bool,std::string> roll_skill_success(std::string_view skill) {
			return {1,"[stub]"};
		}
		virtual std::tuple<bool,std::string> practice(std::string_view skill) {
			return {1,"[stub]"};
		}
		void report(std::vector<std::string> msgs) {
			mods::bugs::fixtures(mods::classes::to_string(this->kind()), IMPLODE(msgs,""));
		}
		void report(std::string_view msg) {
			mods::bugs::fixtures(mods::classes::to_string(this->kind()), msg.data());
		}
	};
	void unblock_event(uint32_t unblock_event,uuid_t player);
};

#endif
