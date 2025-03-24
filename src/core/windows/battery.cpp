module;

#include <iostream>
#include <chrono>

#include <Windows.h>

module core;

import common;
import i18n;
import i18n_system;

// Displays battery status
std::wostream& battery() noexcept{
	int status_percent;

	std::chrono::seconds duration;
	std::chrono::duration<int, std::ratio<86400>> total_days;
	std::chrono::duration<int, std::ratio<3600>>  total_hours;
	std::chrono::duration<int, std::ratio<60>>    total_minutes;

	SYSTEM_POWER_STATUS power_status;

	// Prints comma after description
	bool is_first = true;

	// Retrieves the system power status using the Windows API
	if(!GetSystemPowerStatus(&power_status))
		return std::wcerr << i18n_system::ERROR_POWER_STATUS << L"\n\n";

	// System does not have a battery
	status_percent = static_cast<int>(power_status.BatteryLifePercent);
	if(status_percent > 100) return std::wcerr << i18n_system::ERROR_BATTERY << L"\n\n";

	// Gets the remaining battery life time in seconds from the power status
	duration = std::chrono::seconds(power_status.BatteryLifeTime);

	// Calculates the number of days in the remaining time
	total_days = std::chrono::duration_cast<std::chrono::days>(duration);
	duration -= total_days;

	// Calculates the number of hours in the remaining time
	total_hours = std::chrono::duration_cast<std::chrono::hours>(duration);
	duration -= total_hours;

	// Calculates the number of minutes in the remaining time
	total_minutes = std::chrono::duration_cast<std::chrono::minutes>(duration);
	duration -= total_minutes;

	std::wcout << i18n::BATTERY_STATUS << L"\n\n" << i18n_system::AC_LINE_STATUS << L' ';

	// Checks if the system is connected to AC power
	if(power_status.ACLineStatus == 1){
		std::wcout << i18n::BATTERY_AC_POWER;
	}else{
		std::wcout << i18n::BATTERY_AC_NOT_CONNECTED << L"\n\t" << i18n_system::BATTERY_LIFE_TIME;

		print_plural<int>(total_days.count(),    i18n::BATTERY_DAY,    i18n::BATTERY_DAYS,    is_first);
		print_plural<int>(total_hours.count(),   i18n::BATTERY_HOUR,   i18n::BATTERY_HOURS,   is_first);
		print_plural<int>(total_minutes.count(), i18n::BATTERY_MINUTE, i18n::BATTERY_MINUTES, is_first);
	}

	return std::wcout << L" (" << status_percent << L"%)" << L"\n\n";
}
