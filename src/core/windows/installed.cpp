module;

#include <iostream>
#include <memory>
#include <set>

#include <Windows.h>

#pragma comment(lib, "advapi32.lib")

module core;

import i18n;
import i18n_system;

import common;

struct reg_closer{ void operator()(HKEY handle) const { if(handle != nullptr) RegCloseKey(handle); }};

std::wostream& installed() noexcept{
	std::string key_name;
	std::set<std::string> installed_programs_ordered;
 	TCHAR app_key_name[1024], display_name[1024];

	HKEY app_key = nullptr, uninstall_key = nullptr;
	DWORD index = 0, size = 1024;

	std::unique_ptr<std::remove_pointer_t<HKEY>, reg_closer> uninstall_key_ptr(uninstall_key);
	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall", 0, KEY_READ,
		&uninstall_key) != ERROR_SUCCESS) return std::wcerr << ERROR_INSTALLED_REG_OPENKEYEX << std::endl << std::endl;

	for(; RegEnumKeyEx(uninstall_key, index, app_key_name, &size, nullptr, nullptr, nullptr, nullptr) == ERROR_SUCCESS; ++index){
		key_name = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\" + std::string(app_key_name);

		std::unique_ptr<std::remove_pointer_t<HKEY>, reg_closer> app_key_ptr(app_key);
		if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, key_name.c_str(), 0, KEY_READ, &app_key) != ERROR_SUCCESS) continue;

		size = sizeof(display_name);
		if(RegQueryValueEx(app_key, "DisplayName", nullptr, nullptr, reinterpret_cast<LPBYTE>(display_name), &size) != ERROR_SUCCESS) continue;
		installed_programs_ordered.insert(display_name);
		size = 1024;
	}

	return std::wcout << INSTALLED_PROGRAMS << std::endl << installed_programs_ordered;
}
