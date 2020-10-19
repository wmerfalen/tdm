#ifndef __MENTOC_MODS_RIFLE_INSTANCE_DATA_HEADER__
#define __MENTOC_MODS_RIFLE_INSTANCE_DATA_HEADER__

#include <iostream>
#include <vector>
#include <map>

	using mw_rifle = mods::weapon::type::rifle;
	template <typename TObjPtr,typename TUUID>
	struct heat {
	heat(){
		m_temperature = 0;
		m_weapon_uuid = 0;
	}
	heat(TUUID weapon){
		set_weapon(weapon);
	}
	void set_weapon(TUUID weapon){
		m_weapon_uuid = weapon;
		m_temperature = 0;
		m_weapon = optr_by_uuid(m_weapon_uuid);
	}
	void fire_weapon(){
		int16_t add = 0;
		switch((mw_rifle)m_weapon->rifle()->type){
			default:
			case mw_rifle::SNIPER:
				add = 15;
				break;
			case mw_rifle::SUB_MACHINE_GUN:
				add = 10;
				break;

			case mw_rifle::SHOTGUN:
				add = 25;
				break;

			case mw_rifle::HANDGUN:
			case mw_rifle::PISTOL:
				add = 12;
				break;

			case mw_rifle::MACHINE_PISTOL:
				add = 25;
				break;

			case mw_rifle::ASSAULT_RIFLE:
				add = 45;
				break;
		}
		m_temperature += add;
	}
	~heat() = default;
		private:
			int16_t m_temperature;
			TUUID m_weapon_uuid;
			TObjPtr m_weapon;

	};

	template <typename TAttachmentData,typename TObjPtr,typename TUUID>
	struct rifle_instance_data {
		rifle_instance_data() : ammo(0) {

		}
		~rifle_instance_data() = default;
		uint16_t ammo;
		std::deque<TAttachmentData> attachments;
		std::shared_ptr<heat<TObjPtr,TUUID>> heat_data;
	};

#endif
