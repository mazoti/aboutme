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

// Enumerates and displays available WiFi networks
std::wostream& wifi() noexcept{
	size_t i, j;

	DWORD current_version = 0, max_client = 2;
	HANDLE client_handle                                         = nullptr;
	PWLAN_INTERFACE_INFO_LIST wlan_interface_info_list_ptr       = nullptr;
	PWLAN_INTERFACE_INFO interface_info_ptr                      = nullptr;
	PWLAN_AVAILABLE_NETWORK_LIST wlan_available_network_list_ptr = nullptr;
	PWLAN_AVAILABLE_NETWORK wlan_available_network               = nullptr;

	std::wstring ssid;
	std::multimap<std::wstring, std::wstring> wifi_ordered;

	// Attempts to open WLAN handle
	if(WlanOpenHandle(max_client, nullptr, &current_version, &client_handle) != ERROR_SUCCESS)
		return std::wcerr << i18n_system::ERROR_WLAN_OPEN_HANDLE << L"\n\n";

	std::unique_ptr<void, decltype([](HANDLE ptr){
		if(ptr) WlanCloseHandle(ptr, nullptr);
	})> wlan_handle_ptr(client_handle);

	// Enumerates wireless interfaces
	if(WlanEnumInterfaces(client_handle, nullptr, &wlan_interface_info_list_ptr) != ERROR_SUCCESS)
		return std::wcerr << i18n_system::ERROR_WLAN_ENUM_INTERFACES << L"\n\n";

	std::unique_ptr<PWLAN_INTERFACE_INFO_LIST, decltype([](PWLAN_INTERFACE_INFO_LIST* ptr){
		if(ptr) WlanFreeMemory(*ptr);
	})> wlan_ifinfo_lptr(&wlan_interface_info_list_ptr);

	// Loops through each wireless interface
	for(i = 0; i < wlan_interface_info_list_ptr->dwNumberOfItems; ++i){
		interface_info_ptr = &wlan_interface_info_list_ptr->InterfaceInfo[i];

		// Gets a list of available networks for this interface
		if(WlanGetAvailableNetworkList(client_handle, &interface_info_ptr->InterfaceGuid, 0, nullptr,
			&wlan_available_network_list_ptr) != ERROR_SUCCESS) continue;

		std::unique_ptr<PWLAN_AVAILABLE_NETWORK_LIST, decltype([](PWLAN_AVAILABLE_NETWORK_LIST* ptr){
			if(ptr) WlanFreeMemory(*ptr);
		})> wlan_net_list_ptr(&wlan_available_network_list_ptr);

		// Loops through each available network
		for(j = 0; j < wlan_available_network_list_ptr->dwNumberOfItems; ++j){
			wlan_available_network = &wlan_available_network_list_ptr->Network[j];

			// Constructs SSID string with signal quality percentage
			ssid = std::wstring(wlan_available_network->dot11Ssid.ucSSID, wlan_available_network->dot11Ssid.ucSSID +
				wlan_available_network->dot11Ssid.uSSIDLength);
			ssid += L" (" + std::to_wstring(wlan_available_network->wlanSignalQuality) + L"%)";

			insert_if_unique<std::wstring, std::wstring>(wifi_ordered,
				interface_info_ptr->strInterfaceDescription, ssid);
		}
	}

	if(wifi_ordered.empty()) return std::wcerr << i18n_system::ERROR_WIFI << L"\n\n";

	return std::wcout << i18n::WIFI << L'\n' << wifi_ordered;
}
