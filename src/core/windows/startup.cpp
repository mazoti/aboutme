module;

#include <iostream>
#include <memory>
#include <map>

#include <Windows.h>

#pragma comment(lib, "advapi32.lib")

module core;

import common;

import i18n;
import i18n_system;

// Enumerates startup applications from a given registry key
static inline void startup_apps(HKEY key_root, const char* sub_key, std::multimap<std::string, std::string>&
startup_programs_ordered){
	char value_name[256], value_data[1024];

	HKEY app_key = nullptr;
	DWORD name_size = sizeof(value_name), data_size = sizeof(value_data), index = 0;

	std::unique_ptr<std::remove_pointer_t<HKEY>, decltype([](HKEY handle){
		if(handle != nullptr) RegCloseKey(handle);
	})> app_key_ptr(app_key);

	if(RegOpenKeyEx(key_root, sub_key, 0, KEY_READ, &app_key) != ERROR_SUCCESS){
		std::wcerr << i18n_system::ERROR_REG_OPENKEYEX << std::endl;
		return;
	}

	// Enumerates all values under the opened registry key
	for(; RegEnumValue(app_key, index, value_name, &name_size, nullptr, nullptr,
		reinterpret_cast<LPBYTE>(value_data), &data_size) == ERROR_SUCCESS;
		++index){

		// Resets buffer sizes for the next iteration
		name_size = sizeof(value_name);
		data_size = sizeof(value_data);

		insert_if_unique<std::string, std::string>(startup_programs_ordered, value_name, value_data);
	}
}

// Retrieves and displays startup programs from the Windows registry
std::wostream& startup() noexcept{
	std::multimap<std::string, std::string> startup_programs_ordered;

	startup_apps(HKEY_CURRENT_USER,  "Software\\Microsoft\\Windows\\CurrentVersion\\Run", startup_programs_ordered);
	startup_apps(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", startup_programs_ordered);

	if(startup_programs_ordered.size() > 0)
		return std::wcout << i18n::STARTUP << std::endl << startup_programs_ordered;
	return std::wcout;
}
