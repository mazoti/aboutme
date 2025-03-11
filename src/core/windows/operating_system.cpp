module;

#include <iostream>
#include <string>

#include <Windows.h>

module core;

import i18n;
import i18n_system;

static inline void print_plural(ULONGLONG time_number, const wchar_t* desc, const wchar_t* desc_plural){
	if(time_number){
		std::wcout << L" " << time_number << L" ";
		time_number == 1? std::wcout << desc : std::wcout << desc_plural;
	}
}

// Displays system kernel version
std::wostream& operating_system() noexcept{
	wchar_t locale_name[LOCALE_NAME_MAX_LENGTH], locale_data[256];

	LCID user_locale;
	ULONGLONG seconds, minutes, hours, days;
	TCHAR buffer[MAX_COMPUTERNAME_LENGTH + 1];
	DWORD version_number = GetVersion();

	seconds = (GetTickCount64() / 1000)    % 60;
	minutes = (GetTickCount64() / 60000)   % 60;
	hours   = (GetTickCount64() / 3600000) % 24;
	days    = GetTickCount64()  / 86400000;

	std::wcout  << i18n::OPERATING_SYSTEM << std::endl
				<< L"\tMicrosoft Windows "
				<< static_cast<DWORD>(LOBYTE(LOWORD(version_number)))  // Major version
				<< L"." 
				<< static_cast<DWORD>(HIBYTE(LOWORD(version_number))); // Minor version

	if(version_number < 0x80000000) std::wcout << L" (Build " << static_cast<DWORD>(HIWORD(version_number)) << L")";

	std::wcout << std::endl << L"\t" << i18n::UPTIME;

	print_plural(days, i18n::DAY, i18n::DAYS);
	print_plural(hours, i18n::HOUR_UPTIME, i18n::HOURS_UPTIME);
	print_plural(minutes, i18n::MINUTE, i18n::MINUTES);
	print_plural(seconds, i18n::SECOND, i18n::SECONDS);

	user_locale = GetUserDefaultLCID();
	if(!LCIDToLocaleName(user_locale, locale_name, LOCALE_NAME_MAX_LENGTH, 0)){
		std::wcerr << std::endl << L'\t' << i18n_system::ERROR_LOCALE_NAME << std::endl;
	}
	else{
		// Get country/region
		if(GetLocaleInfoEx(locale_name, LOCALE_SNATIVECOUNTRYNAME, locale_data, 256))
			std::wcout << std::endl << L"\t" << i18n::COUNTRY_REGION << L' ' << locale_data << std::endl;

		// Get language
		if(GetLocaleInfoEx(locale_name, LOCALE_SNATIVELANGUAGENAME, locale_data, 256))
			std::wcout << L"\t" << i18n::LANGUAGE << L' ' << locale_data << std::endl;
	}

	// version_number is used for size
	version_number = sizeof(buffer)/sizeof(buffer[0]);
	if(GetComputerNameEx(ComputerNameDnsHostname, buffer, &version_number))
		std::wcout << L'\t' << i18n::NAME << L' ' << buffer;

	return std::wcout << std::endl << std::endl;
}
