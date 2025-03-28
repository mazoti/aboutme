module;

#include <iostream>
#include <memory>
#include <vector>
#include <map>

#include <Windows.h>
#include <SetupAPI.h>

#pragma comment(lib, "setupapi.lib")

module core;

import common;

import i18n;
import i18n_system;

// Lists connected USB devices
std::wostream& usb() noexcept{
	DWORD device_index, required_size;
	HDEVINFO device_info_set;
	SP_DEVINFO_DATA device_info;

	std::vector<wchar_t> key, value;
	std::multimap<std::wstring, std::wstring> mmap_devices;

	// Gets a handle to the device information set for USB devices
	device_info_set = SetupDiGetClassDevs(nullptr, "USB", nullptr, DIGCF_PRESENT | DIGCF_ALLCLASSES);
	if(device_info_set == INVALID_HANDLE_VALUE)
		return std::wcerr << i18n_system::ERROR_USB_DEVICE_INIT << L"\n\n";

	std::unique_ptr<void, decltype([](HDEVINFO hDevInfo){
			SetupDiDestroyDeviceInfoList(hDevInfo);
	})> device_info_set_ptr(SetupDiGetClassDevs(nullptr, "USB", nullptr, DIGCF_PRESENT | DIGCF_ALLCLASSES));

	// Enumerates all USB devices in the device info set
	device_info.cbSize = sizeof(SP_DEVINFO_DATA);
	for(device_index = 0; SetupDiEnumDeviceInfo(device_info_set, device_index, &device_info); ++device_index){

		// Get device description
		SetupDiGetDeviceRegistryPropertyW(device_info_set, &device_info, SPDRP_DEVICEDESC, nullptr, nullptr,
			0, &required_size);

		key.resize(required_size / sizeof(wchar_t));

		if(!SetupDiGetDeviceRegistryPropertyW(device_info_set, &device_info, SPDRP_DEVICEDESC, nullptr,
			reinterpret_cast<PBYTE>(key.data()), required_size, nullptr)){
			std::wcerr << L'\t' << i18n_system::ERROR_USB_DEVICE_REG << L"\n\n";
			continue;
		}

		// Get device instance ID
		SetupDiGetDeviceInstanceIdW(device_info_set, &device_info, nullptr, 0, &required_size);

		value.resize(required_size);

		if(!SetupDiGetDeviceInstanceIdW(device_info_set, &device_info, value.data(), required_size, nullptr)){
			std::wcerr << L'\t' << i18n_system::ERROR_USB_DEVICE_INST << L"\n\n";
			continue;
		}

		mmap_devices.emplace(key.data(), value.data());
	}

	if(mmap_devices.empty()) return std::wcout << i18n::USB_EMPTY << L"\n\n";

	return std::wcout << i18n::USB << L'\n' << mmap_devices;
}
