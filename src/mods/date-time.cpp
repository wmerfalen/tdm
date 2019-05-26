#include "date-time.hpp"

namespace mods::date_time {
	static moon_phase_t moon;
	std::array<moon_phase_t,12> phase_ring_buffer = {
		NEW_MOON, WANING_CRESCENT,THIRD_QUARTER,
		WANING_GIBBOUS,FULL_MOON, WAXING_GIBBOUS,
		FIRST_QUARTER, WAXING_CRESCENT
	};
	static uint8_t phase_ring_buffer_index;
	static uint8_t hour;
	static uint8_t day;
	static uint8_t month;
	static uint8_t year;

	void initialize_all(uint8_t month,uint8_t day,uint8_t year,uint8_t hour,uint8_t moon_phase_index) {
		mods::date_time::month = month;
		mods::date_time::day = day;
		mods::date_time::year = year;
		mods::date_time::hour = hour;
		mods::date_time::phase_ring_buffer_index = moon_phase_index;
	}

	void initialize_date(uint8_t month,uint8_t day,uint8_t year) {
		mods::date_time::month = month;
		mods::date_time::day = day;
		mods::date_time::year = year;
		mods::date_time::hour = 0;
		mods::date_time::phase_ring_buffer_index = 0;
	}

	uint8_t get_ihour() {
		return mods::date_time::hour;
	}
	uint8_t get_iday() {
		return mods::date_time::day;
	}
	uint8_t get_iyear(){
		return mods::date_time::year;
	}
	std::string get_day(){
		switch(mods::date_time::day){
			case 0: return "sunday";
			case 1: return "monday";
			case 2: return "tuesday";
			case 3: return "wednesday";
			case 4: return "thursday";
			case 5: return "friday";
			case 6: return "saturday";
			default: return "unknown";
		}
	}
	std::string get_month(){
		switch(mods::date_time::month){
			case 1: return "january";
			case 2: return "february";
			case 3: return "march";
			case 4: return "april";
			case 5: return "may";
			case 6: return "june";
			case 7: return "july";
			case 8: return "august";
			case 9: return "september";
			case 10: return "october";
			case 11: return "november";
			case 12: return "december";
			default: return "unknown";
		}
	}
	std::string get_hour() {
		return std::to_string(mods::date_time::hour);
	}
	void increment_hour() {
		mods::date_time::hour += 1;
		if(mods::date_time::hour == 25){
			mods::date_time::hour = 0;
			mods::date_time::day += 1;
		}
		if(mods::date_time::day >= 30){
			mods::date_time::month += 1;
		}
		if(mods::date_time::month > 12){
			mods::date_time::month = 1;
		}
	}
	std::string get_moon_phase() {
#define CASE_MOON(a) case a: return #a; break; 
		switch(mods::date_time::moon){
			CASE_MOON(NEW_MOON);
			CASE_MOON(WAXING_CRESCENT);
			CASE_MOON(FIRST_QUARTER);
			CASE_MOON(WAXING_GIBBOUS);
			CASE_MOON(FULL_MOON);
			CASE_MOON(THIRD_QUARTER);
			CASE_MOON(WANING_CRESCENT);
			default:
				return "UNKNOWN";
		}
	}
};
