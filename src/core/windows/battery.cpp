module;

#include <iostream>
#include <chrono>

#include <Windows.h>

module core;

import i18n;
import i18n_system;

std::wostream& battery() noexcept{
	int status_percent;

	std::chrono::seconds duration;
	std::chrono::duration<int, std::ratio<86400> > total_days;
	std::chrono::duration<int, std::ratio<3600>  > total_hours;
	std::chrono::duration<int, std::ratio<60>    > total_minutes;

	SYSTEM_POWER_STATUS power_status;

	if(!GetSystemPowerStatus(&power_status))
		return std::wcerr << i18n_system::ERROR_POWER_STATUS << std::endl << std::endl;

	// System does not have a battery
	status_percent = static_cast<int>(power_status.BatteryLifePercent);
	if(status_percent > 100) return std::wcerr << i18n_system::ERROR_BATTERY << std::endl << std::endl;

	duration = std::chrono::seconds(power_status.BatteryLifeTime);

	// Extract days
	total_days = std::chrono::duration_cast<std::chrono::days>(duration);
	duration -= total_days;

	// Extract hours
	total_hours = std::chrono::duration_cast<std::chrono::hours>(duration);
	duration -= total_hours;

	// Extract minutes
	total_minutes = std::chrono::duration_cast<std::chrono::minutes>(duration);
	duration -= total_minutes;

	std::wcout << i18n::BATTERY_STATUS << std::endl << L'\t' << i18n_system::AC_LINE_STATUS << L' ';

	if(power_status.ACLineStatus == 1) std::wcout << i18n::BATTERY_AC_POWER;
	else {
		std::wcout << i18n::BATTERY_AC_NOT_CONNECTED << std::endl << L'\t' << i18n_system::BATTERY_LIFE_TIME;
		if(total_days.count())    std::wcout << L' ' << total_days.count()    << L' ' << i18n::BATTERY_DAYS;
		if(total_hours.count())   std::wcout << L' ' << total_hours.count()   << L' ' << i18n::BATTERY_HOURS;
		if(total_minutes.count()) std::wcout << L' ' << total_minutes.count() << L' ' << i18n::BATTERY_MINUTES;
	}

	return std::wcout << L" (" << status_percent << L"%)" << std::endl << std::endl;
}
