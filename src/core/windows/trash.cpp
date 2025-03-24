module;

#include <iostream>
#include <memory>
#include <vector>

#include <Windows.h>
#include <ShlObj.h>
#include <Shlwapi.h>

#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "shlwapi.lib")

module core;

import common;

import i18n;
import i18n_system;

// Defines a custom deleter for smart pointers managing COM-like objects
template <typename T> struct releaser{ void operator()(T* ptr) const{if(ptr){ptr->Release();}}};

// Lists contents of the Windows Recycle Bin
std::wostream& trash() noexcept{
	wchar_t display_name[MAX_PATH];

	ULONG fetched;
	STRRET string_return;

	IEnumIDList *enum_ptr = nullptr;
	IShellFolder *desktop_folder_ptr = nullptr, *recycle_bin_folder_ptr = nullptr;
	LPITEMIDLIST recycle_bin_PID_ptr = nullptr, item_PID_ptr = nullptr;

	std::vector<std::wstring> trash_data;

	// Initializes COM library
	if(FAILED(CoInitialize(nullptr))) return std::wcerr << i18n_system::ERROR_TRASH_COM_INIT << L"\n\n";

	std::unique_ptr<void, decltype([](void*){
		CoUninitialize();
	})> couninit_ptr(reinterpret_cast<void*>(1));

	// Gets the desktop folder
	if(FAILED(SHGetDesktopFolder(&desktop_folder_ptr)))
		return std::wcerr << i18n_system::ERROR_TRASH_DESKTOP_FOLDER << L"\n\n";

	std::unique_ptr<IShellFolder, decltype([](IShellFolder *ptr){
		if(ptr) ptr->Release();
	})> df_ptr(desktop_folder_ptr);

	// Gets the Recycle Bin location
	if(FAILED(SHGetSpecialFolderLocation(nullptr, CSIDL_BITBUCKET, &recycle_bin_PID_ptr)))
		return std::wcerr << i18n_system::ERROR_TRASH_LOCATION << L"\n\n";

	std::unique_ptr<ITEMIDLIST, decltype([](LPITEMIDLIST ptr) { ILFree(ptr); })>
		rb_pid_ptr(static_cast<ITEMIDLIST*>(recycle_bin_PID_ptr));

	// Binds to the Recycle Bin folder
	if(FAILED(desktop_folder_ptr->BindToObject(recycle_bin_PID_ptr, nullptr, IID_IShellFolder, reinterpret_cast<void**>
		(&recycle_bin_folder_ptr)))) return std::wcerr << i18n_system::ERROR_TRASH_BIND << L"\n\n";

	std::unique_ptr<IShellFolder, decltype([](IShellFolder *ptr){
		if(ptr) ptr->Release();
	})> rb_folder_ptr(recycle_bin_folder_ptr);

	// Enumerates objects in the Recycle Bin
	if(FAILED(recycle_bin_folder_ptr->EnumObjects(nullptr, SHCONTF_FOLDERS | SHCONTF_NONFOLDERS, &enum_ptr)))
		return std::wcerr << i18n_system::ERROR_TRASH_ENUM << L"\n\n";

	std::unique_ptr<IEnumIDList, decltype([](IEnumIDList *ptr){
		if(ptr) ptr->Release();
	})> enum_list_ptr(enum_ptr);

	// Loops through all items in the Recycle Bin
	while((enum_ptr->Next(1, &item_PID_ptr, &fetched) == S_OK) && (fetched == 1)){
		std::unique_ptr<ITEMIDLIST, decltype([](LPITEMIDLIST ptr){
			ILFree(ptr);
		})> ipid_ptr(static_cast<ITEMIDLIST*>(item_PID_ptr));

		// Gets display name of the item
		if((FAILED(recycle_bin_folder_ptr->GetDisplayNameOf(item_PID_ptr, SHGDN_NORMAL, &string_return))) ||
		(FAILED(StrRetToBufW(&string_return, item_PID_ptr, display_name, MAX_PATH)))) continue;

		trash_data.emplace_back(display_name);
	}

	if(trash_data.empty()) return std::wcout << i18n::TRASH_EMPTY << L"\n\n";
	return std::wcout << i18n::TRASH << L'\n' << trash_data;
}
