module;

#include <iostream>
#include <string>

#include <Windows.h>

module core;

import i18n;
import i18n_system;

// Displays system kernel version
std::wostream& operating_system() noexcept{
    wchar_t locale_name[LOCALE_NAME_MAX_LENGTH], locale_data[256];

	LCID user_locale;
	ULONGLONG seconds, minutes, hours, days;
	TCHAR buffer[MAX_COMPUTERNAME_LENGTH + 1];
	DWORD version_number = GetVersion();

	seconds = GetTickCount64() / 1000;
	minutes = seconds / 60;
	hours = minutes / 60;
	days = hours / 24;

	hours   %= 24;
	minutes %= 60;
	seconds %= 60;

	std::wcout  << i18n::OPERATING_SYSTEM << std::endl
				<< L"\tMicrosoft Windows "
				<< static_cast<DWORD>(LOBYTE(LOWORD(version_number)))  // Major version
				<< L"." 
				<< static_cast<DWORD>(HIBYTE(LOWORD(version_number))); // Minor version

	if(version_number < 0x80000000) std::wcout << L" (Build " << static_cast<DWORD>(HIWORD(version_number)) << L")";

	std::wcout << std::endl << L"\t" << i18n::UPTIME;
	if(days)    std::wcout << L" " << days    << L" " << i18n::DAYS;
	if(hours)   std::wcout << L" " << hours   << L" " << i18n::HOURS_UPTIME;
	if(minutes) std::wcout << L" " << minutes << L" " << i18n::MINUTES;
	if(seconds) std::wcout << L" " << seconds << L" " << i18n::SECONDS;

	user_locale = GetUserDefaultLCID();
	if(!LCIDToLocaleName(user_locale, locale_name, LOCALE_NAME_MAX_LENGTH, 0))
		std::wcerr << std::endl << L'\t' << i18n_system::ERROR_LOCALE_NAME << std::endl;
	else{

		// Get country/region
		if(GetLocaleInfoEx(locale_name, LOCALE_SENGCOUNTRY, locale_data, 256))
			std::wcout << std::endl << L"\t" << i18n::COUNTRY_REGION << L' ' << locale_data << std::endl;

		// Get language
		if(GetLocaleInfoEx(locale_name, LOCALE_SENGLISHLANGUAGENAME, locale_data, 256))
			std::wcout << L"\t" << i18n::LANGUAGE << L' ' << locale_data << std::endl;
	}

	// version_number is used for size
	version_number = sizeof(buffer)/sizeof(buffer[0]);
	if(GetComputerNameEx(ComputerNameDnsHostname, buffer, &version_number))
		std::wcout << L'\t' << i18n::NAME << L' ' << buffer;

	return std::wcout << std::endl << std::endl;
}
