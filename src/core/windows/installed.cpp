module;

#include <iostream>
#include <memory>
#include <set>
#include <string>

#include <Windows.h>

#pragma comment(lib, "advapi32.lib")

module core;

import i18n;
import i18n_system;

// Lists installed programs from the Windows registry
std::wostream& installed() noexcept{
	DWORD size, index;
	WCHAR app_key_name[1024], display_name[1024];
	HKEY app_key, uninstall_key = nullptr;

	std::wstring subkey_path;
	std::set<std::wstring> installed_programs;

	// Opens the Uninstall registry key under HKEY_LOCAL_MACHINE
	if(RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall", 0, KEY_READ,
	&uninstall_key) != ERROR_SUCCESS) return std::wcerr << i18n_system::ERROR_INSTALLED_REG_OPENKEYEX << L"\n\n";

	// Manages the uninstall_key handle
	std::unique_ptr<std::remove_pointer_t<HKEY>, decltype([](HKEY h){ if(h) RegCloseKey(h); })>
	uninstall_key_ptr(uninstall_key);

	// Enumerates all subkeys under the Uninstall key
	for(index = 0;; ++index){
		size = 1024;
		if(RegEnumKeyExW(uninstall_key, index, app_key_name, &size, nullptr,
			nullptr, nullptr, nullptr) != ERROR_SUCCESS) break;

		// Constructs the full subkey path
		subkey_path = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\" + std::wstring(app_key_name);

		app_key = nullptr;
		if(RegOpenKeyExW(HKEY_LOCAL_MACHINE, subkey_path.c_str(), 0, KEY_READ, &app_key) != ERROR_SUCCESS) continue;

		// Manages the app_key handle
		std::unique_ptr<std::remove_pointer_t<HKEY>, decltype([](HKEY h){if(h) RegCloseKey(h);})> app_key_ptr(app_key);

		size = sizeof(display_name);
		if(RegQueryValueExW(app_key, L"DisplayName", nullptr, nullptr, reinterpret_cast<LPBYTE>(display_name),
			&size) == ERROR_SUCCESS) installed_programs.insert(display_name);
	}

	std::wcout << i18n::INSTALLED_PROGRAMS << L'\n';
	for(const auto& program : installed_programs) std::wcout << L'\t' << program << L'\n';
	return std::wcout << L'\n';
}
