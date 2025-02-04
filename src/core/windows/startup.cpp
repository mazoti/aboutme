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

struct reg_closer{ void operator()(HKEY handle) const { if(handle != nullptr) RegCloseKey(handle); }};

static void startup_programs(HKEY key_root, const char* sub_key, std::multimap<std::string, std::string>&
startup_programs_ordered){
	char value_name[256], value_data[1024];

	HKEY app_key = nullptr;
	DWORD name_size = sizeof(value_name), data_size = sizeof(value_data), index = 0;

	std::unique_ptr<std::remove_pointer_t<HKEY>, reg_closer> app_key_ptr(app_key);
	if(RegOpenKeyEx(key_root, sub_key, 0, KEY_READ, &app_key) != ERROR_SUCCESS){
		std::wcerr << ERROR_REG_OPENKEYEX << std::endl;
		return;
	}

	for(; RegEnumValue(app_key, index, value_name, &name_size, nullptr, nullptr,
		reinterpret_cast<LPBYTE>(value_data), &data_size) == ERROR_SUCCESS;
		++index){
		name_size = sizeof(value_name);
		data_size = sizeof(value_data);
		startup_programs_ordered.insert({value_name, value_data});
	}
}

std::wostream& startup() noexcept{
	std::multimap<std::string, std::string> startup_programs_ordered;

	startup_programs(HKEY_CURRENT_USER,  "Software\\Microsoft\\Windows\\CurrentVersion\\Run", startup_programs_ordered);
	startup_programs(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", startup_programs_ordered);

	if(startup_programs_ordered.size() > 0) return std::wcout << STARTUP << std::endl << startup_programs_ordered;
	return std::wcout;
}
