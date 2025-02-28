module;

#include <iostream>
#include <string>
#include <memory>
#include <map>

#include <Windows.h>
#include <wlanapi.h>

#pragma comment(lib, "wlanapi.lib")
#pragma comment(lib, "Ole32.lib")

module core;

import common;

import i18n;
import i18n_system;

template <typename T> struct wlan_free_memory{ void operator()(T* list) const{ if(list) WlanFreeMemory(*list);}};

struct wlan_handle_closer{
	void operator()(HANDLE client_handle) const{ if(client_handle) WlanCloseHandle(client_handle, nullptr);}
};

std::wostream& wifi() noexcept{
	std::wstring ssid;
	std::multimap<std::wstring, std::wstring> wifi_ordered;

	HANDLE client_handle = nullptr;
	DWORD current_version = 0, max_client = 2;
	PWLAN_INTERFACE_INFO_LIST wlan_interface_info_list_ptr = nullptr;
	PWLAN_INTERFACE_INFO interface_info_ptr = nullptr;
	PWLAN_AVAILABLE_NETWORK_LIST wlan_available_network_list_ptr = nullptr;
	PWLAN_AVAILABLE_NETWORK wlan_available_network = nullptr;

	if(WlanOpenHandle(max_client, nullptr, &current_version, &client_handle) != ERROR_SUCCESS)
		return std::wcerr << i18n_system::ERROR_WLAN_OPEN_HANDLE << std::endl << std::endl;
	std::unique_ptr<void, wlan_handle_closer> wlan_handle_ptr(client_handle);

	// Enumerate wireless interfaces
	if(WlanEnumInterfaces(client_handle, nullptr, &wlan_interface_info_list_ptr) != ERROR_SUCCESS)
		return std::wcerr << i18n_system::ERROR_WLAN_ENUM_INTERFACES << std::endl << std::endl;
	std::unique_ptr<PWLAN_INTERFACE_INFO_LIST, wlan_free_memory<PWLAN_INTERFACE_INFO_LIST> >
		wlan_ifinfo_lptr(&wlan_interface_info_list_ptr);

	// Retrieve the list of available networks
	for(size_t i = 0; i < wlan_interface_info_list_ptr->dwNumberOfItems; ++i){
		interface_info_ptr = &wlan_interface_info_list_ptr->InterfaceInfo[i];

		if(WlanGetAvailableNetworkList(client_handle, &interface_info_ptr->InterfaceGuid, 0, nullptr,
			&wlan_available_network_list_ptr) != ERROR_SUCCESS) continue;
		std::unique_ptr<PWLAN_AVAILABLE_NETWORK_LIST, wlan_free_memory<PWLAN_AVAILABLE_NETWORK_LIST> >
			wlan_net_list_ptr(&wlan_available_network_list_ptr);

		for(size_t j = 0; j < wlan_available_network_list_ptr->dwNumberOfItems; ++j){
			wlan_available_network = static_cast<WLAN_AVAILABLE_NETWORK*>(&wlan_available_network_list_ptr->Network[j]);
			ssid = std::wstring(wlan_available_network->dot11Ssid.ucSSID, wlan_available_network->dot11Ssid.ucSSID +
				wlan_available_network->dot11Ssid.uSSIDLength);
			ssid += L" (" + std::to_wstring(wlan_available_network->wlanSignalQuality) + L"%)";

			wifi_ordered.insert({interface_info_ptr->strInterfaceDescription, ssid});
		}
	}
	if(wifi_ordered.empty()) return std::wcerr << i18n_system::ERROR_WIFI << std::endl << std::endl;
	return std::wcout << i18n::WIFI << std::endl << wifi_ordered;
}
