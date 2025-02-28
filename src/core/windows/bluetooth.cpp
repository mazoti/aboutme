module;

#include <iostream>
#include <sstream>
#include <map>
#include <memory>

#include <Windows.h>
#include <bluetoothapis.h>

#pragma comment(lib, "Bthprops.lib")

module core;

import common;

import i18n;
import i18n_system;

struct bluetooth_find_device_close{
	void operator()(HBLUETOOTH_DEVICE_FIND handle) const{ if(handle) BluetoothFindDeviceClose(handle);}
};

std::wostream& bluetooth() noexcept{
	std::wostringstream bluetooth_address;
	std::multimap<std::wstring, std::wstring> bluetooth_devices_ordered;

	BLUETOOTH_DEVICE_INFO device_info;
	BLUETOOTH_DEVICE_SEARCH_PARAMS search_params;
	HBLUETOOTH_DEVICE_FIND device_find_handle;

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

	// Wrap the device find handle in a unique_ptr with the custom deleter
	device_find_handle = BluetoothFindFirstDevice(&search_params, &device_info);
	std::unique_ptr<void, bluetooth_find_device_close> device_find_handle_ptr(device_find_handle);

	if(device_find_handle == nullptr)
		return std::wcerr << i18n_system::ERROR_NO_BLUETOOTH_ADAPTERS << std::endl << std::endl;

	do{
		bluetooth_address << std::hex << std::uppercase
			<< ((device_info.Address.ullLong >> 40) & 0xFF) << L':'
			<< ((device_info.Address.ullLong >> 32) & 0xFF) << L':'
			<< ((device_info.Address.ullLong >> 24) & 0xFF) << L':'
			<< ((device_info.Address.ullLong >> 16) & 0xFF) << L':'
			<< ((device_info.Address.ullLong >> 8) & 0xFF)  << L':'
			<< (device_info.Address.ullLong & 0xFF);

		bluetooth_devices_ordered.insert({std::wstring(device_info.szName), bluetooth_address.str()});
		bluetooth_address.str(L"");
		bluetooth_address.clear();
	}while(BluetoothFindNextDevice(device_find_handle, &device_info));

	return std::wcout << i18n::BLUETOOTH << std::endl << bluetooth_devices_ordered;
}
