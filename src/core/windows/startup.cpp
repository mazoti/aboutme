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
static inline void startup_apps(HKEY key_root, const wchar_t* sub_key,
std::multimap<std::wstring, std::wstring>& startup_programs_ordered){
	WCHAR value_name[256];
	BYTE value_data[1024];
	DWORD name_size, data_size, type, index;

	HKEY app_key = nullptr;

	if(RegOpenKeyExW(key_root, sub_key, 0, KEY_READ, &app_key) != ERROR_SUCCESS){
		std::wcerr << i18n_system::ERROR_REG_OPENKEYEX << std::endl;
		return;
	}

	std::unique_ptr<std::remove_pointer_t<HKEY>, decltype([](HKEY handle){
		if (handle != nullptr) RegCloseKey(handle);
	})> app_key_ptr(app_key);

	// Enumerates all values under the opened registry key
	for(index = 0; ; ++index){
		name_size = 255; // Maximum number of characters excluding null terminator
		data_size = sizeof(value_data);
		if(RegEnumValueW(app_key, index, value_name, &name_size, nullptr, &type,
			value_data, &data_size) != ERROR_SUCCESS)
			break;

		if(type == REG_SZ || type == REG_EXPAND_SZ)
			startup_programs_ordered.emplace(std::wstring(value_name),std::wstring(reinterpret_cast<const wchar_t*>
			(value_data)));
	}
}

// Retrieves and displays startup programs from the Windows registry
std::wostream& startup() noexcept{
	std::multimap<std::wstring, std::wstring> startup_programs_ordered;

	startup_apps(HKEY_CURRENT_USER,  L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", startup_programs_ordered);
	startup_apps(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", startup_programs_ordered);

	if(startup_programs_ordered.size() > 0)
		return std::wcout << i18n::STARTUP << std::endl << startup_programs_ordered;
	return std::wcout;
}
