module;

#include <iostream>
#include <memory>
#include <set>

#include <Windows.h>

#pragma comment(lib, "advapi32.lib")

module core;

import common;

import i18n;
import i18n_system;

// Function to list installed programs from the Windows registry
std::wostream& installed() noexcept{
	TCHAR app_key_name[1024], display_name[1024];
	HKEY app_key = nullptr, uninstall_key = nullptr;
	DWORD index = 0, size = 1024;

	std::string key_name;
	std::set<std::string> installed_programs_ordered;

	// Smart pointer to manage the uninstall_key handle with a custom deleter
	std::unique_ptr<std::remove_pointer_t<HKEY>, decltype([](HKEY handle){
		if(handle != nullptr) RegCloseKey(handle);
	})> uninstall_key_ptr(uninstall_key);

	// Attempts to open the Uninstall registry key under HKEY_LOCAL_MACHINE
	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall", 0, KEY_READ,
		&uninstall_key) != ERROR_SUCCESS)
		return std::wcerr << i18n_system::ERROR_INSTALLED_REG_OPENKEYEX << std::endl << std::endl;

	// Enumerates all subkeys under the Uninstall key
	// RegEnumKeyEx retrieves each subkey name into app_key_name
	for(; RegEnumKeyEx(uninstall_key, index, app_key_name, &size, nullptr, nullptr, nullptr, nullptr) == ERROR_SUCCESS;
		++index){
		key_name = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\" + std::string(app_key_name);

		// Smart pointer to manage the app_key handle with a custom deleter
		std::unique_ptr<std::remove_pointer_t<HKEY>, decltype([](HKEY handle){
			if(handle != nullptr) RegCloseKey(handle);
		})> app_key_ptr(app_key);

		// Smart pointer to manage the app_key handle with a custom deleter
		if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, key_name.c_str(), 0, KEY_READ, &app_key) != ERROR_SUCCESS) continue;

		size = sizeof(display_name);

		// Queries the "DisplayName" value from the subkey
		if(RegQueryValueEx(app_key, "DisplayName", nullptr, nullptr, reinterpret_cast<LPBYTE>(display_name), &size)
			!= ERROR_SUCCESS) continue;

		installed_programs_ordered.insert(display_name);
		size = 1024;
	}

	return std::wcout << i18n::INSTALLED_PROGRAMS << std::endl << installed_programs_ordered;
}
