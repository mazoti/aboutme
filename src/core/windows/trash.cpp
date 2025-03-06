module;

#include <iostream>
#include <memory>
#include <set>

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

static bool couninitialize_released_trash = false;

std::wostream& trash() noexcept{
	wchar_t display_name[MAX_PATH];

	ULONG fetched;
	STRRET string_return;

	std::set<std::wstring> trash_data_ordered;

	IEnumIDList *enum_ptr = nullptr;
	IShellFolder *desktop_folder_ptr = nullptr, *recycle_bin_folder_ptr = nullptr;
	LPITEMIDLIST recycle_bin_PID_ptr = nullptr, item_PID_ptr = nullptr;

	if(FAILED(CoInitialize(nullptr))) return std::wcerr << i18n_system::ERROR_TRASH_COM_INIT << std::endl << std::endl;

	std::unique_ptr<void, decltype([](void*){
		if(couninitialize_released_trash) return;
		couninitialize_released_trash = true;
		CoUninitialize();
	})> couninit_ptr(reinterpret_cast<void*>(1));

	if(FAILED(SHGetDesktopFolder(&desktop_folder_ptr)))
		return std::wcerr << i18n_system::ERROR_TRASH_DESKTOP_FOLDER << std::endl << std::endl;

	std::unique_ptr<IShellFolder, decltype([](IShellFolder *ptr){
		if(ptr) ptr->Release();
	})> df_ptr(desktop_folder_ptr);

	if(FAILED(SHGetSpecialFolderLocation(nullptr, CSIDL_BITBUCKET, &recycle_bin_PID_ptr)))
		return std::wcerr << i18n_system::ERROR_TRASH_LOCATION << std::endl << std::endl;

	std::unique_ptr<ITEMIDLIST, decltype([](LPITEMIDLIST ptr) { ILFree(ptr); })>
		rb_pid_ptr(static_cast<ITEMIDLIST*>(recycle_bin_PID_ptr));

	if(FAILED(desktop_folder_ptr->BindToObject(recycle_bin_PID_ptr, nullptr, IID_IShellFolder, reinterpret_cast<void**>
		(&recycle_bin_folder_ptr)))) return std::wcerr << i18n_system::ERROR_TRASH_BIND << std::endl << std::endl;

	std::unique_ptr<IShellFolder, decltype([](IShellFolder *ptr){
		if(ptr) ptr->Release();
	})> rb_folder_ptr(recycle_bin_folder_ptr);

	if(FAILED(recycle_bin_folder_ptr->EnumObjects(nullptr, SHCONTF_FOLDERS | SHCONTF_NONFOLDERS, &enum_ptr)))
		return std::wcerr << i18n_system::ERROR_TRASH_ENUM << std::endl << std::endl;

	std::unique_ptr<IEnumIDList, decltype([](IEnumIDList *ptr){
		if(ptr) ptr->Release();
	})> enum_list_ptr(enum_ptr);

	while(enum_ptr->Next(1, &item_PID_ptr, &fetched) == S_OK && fetched == 1){
		std::unique_ptr<ITEMIDLIST, decltype([](LPITEMIDLIST ptr){
			CoTaskMemFree(ptr);
		})> ipid_ptr(static_cast<ITEMIDLIST*>(item_PID_ptr));

		if(FAILED(recycle_bin_folder_ptr->GetDisplayNameOf(item_PID_ptr, SHGDN_NORMAL, &string_return))) continue;
		if(FAILED(StrRetToBufW(&string_return, item_PID_ptr, display_name, MAX_PATH))) continue;
		trash_data_ordered.emplace(display_name);
	}

	if(trash_data_ordered.empty()) return std::wcout << i18n::TRASH_EMPTY << std::endl << std::endl;
	return std::wcout << i18n::TRASH << std::endl << trash_data_ordered;
}
