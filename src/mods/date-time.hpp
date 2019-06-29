#ifndef __MENTOC_MODS_DATE_TIME_HEADER__
#define __MENTOC_MODS_DATE_TIME_HEADER__
#include <iostream>
#include <array>
#include <string>
#include <time.h>

namespace mods::date_time {
	enum moon_phase {
		NEW_MOON = 0,
		WAXING_CRESCENT,
		FIRST_QUARTER,
		WAXING_GIBBOUS,
		FULL_MOON,
		THIRD_QUARTER,
		WANING_GIBBOUS,
		WANING_CRESCENT,
		__FIRST = NEW_MOON,
		__LAST = WANING_CRESCENT
	};
	using moon_phase_t = moon_phase;
	uint8_t get_imonth();
	uint8_t get_iday();
	uint8_t get_ihour();
	uint8_t get_iyear();
	std::string get_month();
	std::string get_day();
	std::string get_hour();
	std::string get_moon_phase();
	void heartbeat();
	void increment_hour();
	/**
	 * ! \brief initializes all date/time integers
	 *  \param uint8_t month
	 *  \param uint8_t day
	 *  \param uint8_t year
	 *  \param uint8_t hour
	 *  \param uint8_t moon_phase_index
	 *  \return void
	 */
	void initialize_all(uint8_t month,uint8_t day,uint8_t year,uint8_t hour,uint8_t moon_phase_index);
	/**
	 * ! \brief initializes month, day, year
	 *  \param uint8_t month
	 *  \param uint8_t day
	 *  \param uint8_t year
	 *  \return void
	 */
	void initialize_date(uint8_t month,uint8_t day,uint8_t year);
};
#endif
