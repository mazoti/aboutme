module;

#include <iostream>
#include <format>
#include <map>
#include <memory>

#include <Windows.h>
#include <bluetoothapis.h>

#pragma comment(lib, "Bthprops.lib")

module core;

import common;
import i18n;
import i18n_system;

// Enumerates and displays Bluetooth devices
std::wostream& bluetooth() noexcept{
	BLUETOOTH_DEVICE_INFO device_info;
	BLUETOOTH_DEVICE_SEARCH_PARAMS search_params;
	HBLUETOOTH_DEVICE_FIND device_find_handle;

	std::wstring address;
	std::multimap<std::wstring, std::wstring> bluetooth_devices_ordered;

	// Initializes device_info with default values
	device_info = {
		.dwSize = sizeof(BLUETOOTH_DEVICE_INFO),
		.Address = {0},
		.ulClassofDevice = 0,
		.fConnected = FALSE,
		.fRemembered = FALSE,
		.fAuthenticated = FALSE,
		.stLastSeen = {
			.wYear = 0,
			.wMonth = 0,
			.wDayOfWeek = 0,
			.wDay = 0,
			.wHour = 0,
			.wMinute = 0,
			.wSecond = 0,
			.wMilliseconds = 0
		},
		.stLastUsed = {
			.wYear = 0,
			.wMonth = 0,
			.wDayOfWeek = 0,
			.wDay = 0,
			.wHour = 0,
			.wMinute = 0,
			.wSecond = 0,
			.wMilliseconds = 0
		},
		.szName = L""
	};

	// Initializes search parameters for Bluetooth device discovery
	search_params = {
		sizeof(BLUETOOTH_DEVICE_SEARCH_PARAMS),
		1,      // Return authenticated devices
		1,      // Return remembered devices
		1,      // Return unknown devices
		1,      // Return connected devices
		1,      // Issue in-progress inquiry
		3,      // Timeout multiplier
		nullptr // Radio handle
	};

	// Starts searching for Bluetooth devices and gets the first device
	device_find_handle = BluetoothFindFirstDevice(&search_params, &device_info);

	std::unique_ptr<void, decltype([](HBLUETOOTH_DEVICE_FIND handle){
		if(handle) BluetoothFindDeviceClose(handle);
	})> device_find_handle_ptr(device_find_handle);

	if(device_find_handle == nullptr)
		return std::wcerr << i18n_system::ERROR_NO_BLUETOOTH_ADAPTERS << std::endl << std::endl;

	// Enumerates all Bluetooth devices
	do{
		address = std::format(L"{:02X}:{:02X}:{:02X}:{:02X}:{:02X}:{:02X}",
			device_info.Address.rgBytes[0],
			device_info.Address.rgBytes[1],
			device_info.Address.rgBytes[2],
			device_info.Address.rgBytes[3],
			device_info.Address.rgBytes[4],
			device_info.Address.rgBytes[5]
		);

		insert_if_unique<std::wstring, std::wstring>(bluetooth_devices_ordered,
			std::wstring(device_info.szName), address);
	}while(BluetoothFindNextDevice(device_find_handle, &device_info));

	return std::wcout << i18n::BLUETOOTH << std::endl << bluetooth_devices_ordered;
}
