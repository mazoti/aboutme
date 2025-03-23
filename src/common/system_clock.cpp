module;

// Single thread, no problem on Windows
#ifdef _WINDOWS
	#define _CRT_SECURE_NO_WARNINGS
#endif

#include <iostream>
#include <iomanip>
#include <format>

#include <chrono>
#include <ctime>

module common;

import i18n;

// Outputs system clock information
std::wostream& system_clock() noexcept{
	int offset;
	std::tm local_tm, gmt_tm;
	std::wstring date_value, time_value, gmt_value;

	// Gets the current system time as a time_t value
	std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

	// Converts the time_t to local time and GMT/UTC time
	local_tm  = *std::localtime(&time);
	gmt_tm    = *std::gmtime(&time);

	// Calculates GMT offset in hours and append an internationalized "hours" string
	offset = static_cast<int>(difftime(mktime(&local_tm), mktime(&gmt_tm)) / 3600);
	gmt_value = std::to_wstring(offset) + L" ";

	((offset >= -1) && (offset <= 1)) ? gmt_value += i18n::HOUR : gmt_value += i18n::HOURS;

	// Checks if the system is using local time or UTC based on the offset
	static_cast<int>(std::difftime(std::mktime(&local_tm), std::mktime(&gmt_tm))) ?
		gmt_value += L" (local)" : gmt_value += L" (UTC)";

	// Formats the local date as "DD/MM/YY" (e.g., 25/12/23)
	date_value = std::format(L"{:02}/{:02}/{:02}", local_tm.tm_mday, local_tm.tm_mon + 1, local_tm.tm_year % 100);

	// Formats the local time as "HH:MM:SS" (e.g., 14:30:45)
	time_value = std::format(L"{:02}:{:02}:{:02}", local_tm.tm_hour, local_tm.tm_min, local_tm.tm_sec);

	// Checks daylight saving time
	if(local_tm.tm_isdst > 0){
		return print_title(0, 1, L' ', i18n::SYSTEM_CLOCK,
			i18n::DATE, date_value,
			i18n::TIME, time_value,
			i18n::GMT, gmt_value,
			i18n::DAYLIGHT_SAVING_TIME, L"");
	}

	return print_title(0, 1, L' ', i18n::SYSTEM_CLOCK,
		i18n::DATE, date_value,
		i18n::TIME, time_value,
		i18n::GMT, gmt_value);
}
