module;

#include <iostream>
#include <format>
#include <memory>
#include <set>

#include <Windows.h>
#include <bluetoothapis.h>

#pragma comment(lib, "Bthprops.lib")

module core;

import i18n;
import i18n_system;

// Enumerates and displays Bluetooth devices
std::wostream& bluetooth() noexcept{
	BLUETOOTH_DEVICE_INFO device_info;
	BLUETOOTH_DEVICE_SEARCH_PARAMS search_params;
	HBLUETOOTH_DEVICE_FIND device_find_handle;

	std::set<std::wstring> bluetooth_devices_set;

	// Initializes device_info with default values
	device_info = { sizeof(BLUETOOTH_DEVICE_INFO) };

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
		bluetooth_devices_set.emplace(std::format(L"{} - {:02X}:{:02X}:{:02X}:{:02X}:{:02X}:{:02X}{}{}",
			device_info.szName,

			device_info.Address.rgBytes[0],
			device_info.Address.rgBytes[1],
			device_info.Address.rgBytes[2],
			device_info.Address.rgBytes[3],
			device_info.Address.rgBytes[4],
			device_info.Address.rgBytes[5],

			(device_info.fAuthenticated ? i18n::AUTHENTICATED : L""),
			(device_info.fConnected     ? i18n::CONNECTED     : L"")
		));
	}while(BluetoothFindNextDevice(device_find_handle, &device_info));

	std::wcout << i18n::BLUETOOTH << std::endl;
	for(const std::wstring& data : bluetooth_devices_set) std::wcout << L'\t' << data << std::endl;
	return std::wcout << std::endl;
}
