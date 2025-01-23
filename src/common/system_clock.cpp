module;

// Single thread, no problem on Windows
#ifdef _WINDOWS
	#define _CRT_SECURE_NO_WARNINGS
#endif

#include <iostream>
#include <sstream>
#include <iomanip>
#include <chrono>

#include <ctime>

module common;

import i18n;

std::wostream& system_clock() noexcept{
	// Get current time
	std::tm local_tm, gmt_tm, utc_tm;
	std::wstring date_value, time_value, gmt_value;
	std::wostringstream woss;

	std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

	// Convert to local time and GMT/UTC time
	local_tm  = *std::localtime(&time);
	utc_tm    = *std::gmtime(&time);
	gmt_tm    = *std::gmtime(&time);
	gmt_value = std::to_wstring(static_cast<int>(difftime(mktime(&local_tm), mktime(&gmt_tm)) / 3600)) + L" " + HOURS;

	// Determine if the system is using local time or UTC
	static_cast<int>(std::difftime(std::mktime(&local_tm), std::mktime(&utc_tm))) ?
		gmt_value += L" (local)" : gmt_value += L" (UTC)";

	woss << std::put_time(&local_tm, L"%d/%m/%y");
	date_value = woss.str();
	woss.str(L"");
	woss << std::put_time(&local_tm, L"%H:%M:%S");
	time_value = woss.str();

	// Check daylight saving time
	if(local_tm.tm_isdst > 0){
		return print_title(0, 1, L' ', SYSTEM_CLOCK,
			DATE, date_value,
			TIME, time_value,
			GMT, gmt_value,
			DAYLIGHT_SAVING_TIME, L"");
	}

	return print_title(0, 1, L' ', SYSTEM_CLOCK,
		DATE, date_value,
		TIME, time_value,
		GMT, gmt_value);
}
