module;

#include <iostream>
#include <string>

#include <Windows.h>

module core;

import common;
import i18n;
import i18n_system;

// Function to display operating system details
std::wostream& operating_system() noexcept{
	wchar_t locale_name[LOCALE_NAME_MAX_LENGTH], locale_data[256];

	LCID user_locale;
	ULONGLONG seconds, minutes, hours, days;
	TCHAR buffer[MAX_COMPUTERNAME_LENGTH + 1];

	bool first = true;
	DWORD version_number = GetVersion();

	// Calculates uptime from system tick count (in milliseconds)
	seconds = (GetTickCount64() / 1000);
	minutes = seconds / 60;
	hours   = minutes / 60;
	days    = hours   / 24;

	seconds %= 60;
	minutes %= 60;
	hours   %= 24;

	// Outputs OS name and version
	std::wcout  << i18n::OPERATING_SYSTEM << std::endl
				<< L"\tMicrosoft Windows "
				<< static_cast<DWORD>(LOBYTE(LOWORD(version_number)))  // Major version
				<< L"." 
				<< static_cast<DWORD>(HIBYTE(LOWORD(version_number))); // Minor version

	// If not an NT-based system, skips build number; otherwise, includes it
	if(version_number < 0x80000000) std::wcout << L" (Build " << static_cast<DWORD>(HIWORD(version_number)) << L")";

	// Outputs uptime with commas
	std::wcout << std::endl << L"\t" << i18n::UPTIME;

	print_plural<ULONGLONG>(days,    i18n::DAY,         i18n::DAYS,         first);
	print_plural<ULONGLONG>(hours,   i18n::HOUR_UPTIME, i18n::HOURS_UPTIME, first);
	print_plural<ULONGLONG>(minutes, i18n::MINUTE,      i18n::MINUTES,      first);
	print_plural<ULONGLONG>(seconds, i18n::SECOND,      i18n::SECONDS,      first);

	// Gets and processes user's locale information
	user_locale = GetUserDefaultLCID();
	if(!LCIDToLocaleName(user_locale, locale_name, LOCALE_NAME_MAX_LENGTH, 0)){
		std::wcerr << std::endl << L'\t' << i18n_system::ERROR_LOCALE_NAME << std::endl;
	}
	else{
		// Gets country/region
		if(GetLocaleInfoEx(locale_name, LOCALE_SNATIVECOUNTRYNAME, locale_data, 256))
			std::wcout << std::endl << L"\t" << i18n::COUNTRY_REGION << L' ' << locale_data << std::endl;

		// Gets language
		if(GetLocaleInfoEx(locale_name, LOCALE_SNATIVELANGUAGENAME, locale_data, 256))
			std::wcout << L"\t" << i18n::LANGUAGE << L' ' << locale_data << std::endl;
	}

	// Gets and displays the computer's DNS hostname
	version_number = sizeof(buffer)/sizeof(buffer[0]);
	if(GetComputerNameEx(ComputerNameDnsHostname, buffer, &version_number))
		std::wcout << L'\t' << i18n::NAME << L' ' << buffer;

	return std::wcout << std::endl << std::endl;
}
