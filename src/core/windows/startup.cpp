module;

#include <iostream>
#include <memory>
#include <map>
#include <vector>

#include <Windows.h>

#pragma comment(lib, "advapi32.lib")

module core;

import common;
import i18n;
import i18n_system;

// Enumerates startup applications from a given registry key
static void startup_apps(HKEY root_key, const wchar_t* subkey_path,
                         std::multimap<std::wstring, std::wstring>& startup_programs_ordered){

	LONG result;
	DWORD name_size, type, index, data_size;
	WCHAR value_name[256];
	HKEY app_key = nullptr;

	std::vector<BYTE> value_data(1024); // Initial buffer size for value data

	if(RegOpenKeyExW(root_key, subkey_path, 0, KEY_READ, &app_key) != ERROR_SUCCESS){
		std::wcerr << i18n_system::ERROR_REG_OPENKEYEX << L'\n';
		return;
	}

	// RAII handle for automatic registry key cleanup
	std::unique_ptr<std::remove_pointer_t<HKEY>, decltype([](HKEY handle){
		if(handle != nullptr) RegCloseKey(handle);
	})> app_key_ptr(app_key);

	// Enumerates all values under the registry key
	for(index = 0; ; ++index){
		name_size = 255; // Max characters excluding null terminator
		data_size = static_cast<DWORD>(value_data.size());

		result = RegEnumValueW(app_key, index, value_name, &name_size, nullptr, &type,
		                       value_data.data(), &data_size);

		if((result == ERROR_SUCCESS) && (type == REG_SZ || type == REG_EXPAND_SZ)){
			startup_programs_ordered.emplace(std::wstring(value_name),
				std::wstring(reinterpret_cast<const wchar_t*>(value_data.data())));
			continue;
		}

		if(result == ERROR_MORE_DATA){
			// Buffer too small; resize and retry
			value_data.resize(data_size);
			data_size = static_cast<DWORD>(value_data.size());
			result = RegEnumValueW(app_key, index, value_name, &name_size, nullptr, &type,
			                       value_data.data(), &data_size);
			if((result == ERROR_SUCCESS) && (type == REG_SZ || type == REG_EXPAND_SZ)){
			    startup_programs_ordered.emplace(std::wstring(value_name),
			    std::wstring(reinterpret_cast<const wchar_t*>(value_data.data())));
			}
			continue;
		}

		break; // Typically ERROR_NO_MORE_ITEMS, ending the enumeration
	}
}

// Retrieves and displays startup programs from the Windows registry
std::wostream& startup() noexcept{
	std::multimap<std::wstring, std::wstring> startup_programs_ordered;

	// Checks common startup locations
	startup_apps(HKEY_CURRENT_USER,  L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", startup_programs_ordered);
	startup_apps(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", startup_programs_ordered);

	if(startup_programs_ordered.size() > 0)
		return std::wcout << i18n::STARTUP << L'\n' << startup_programs_ordered;
	return std::wcout;
}
