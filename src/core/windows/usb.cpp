module;

#include <iostream>
#include <memory>
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
	char key[64], value[64];
	DWORD device_index;
	SP_DEVINFO_DATA device_info;

	std::multimap<std::string, std::string> mmap_devices;

	// Gets a handle to the device information set for USB devices
	HDEVINFO device_info_set = SetupDiGetClassDevs(nullptr, "USB", nullptr, DIGCF_PRESENT | DIGCF_ALLCLASSES);
	if(device_info_set == INVALID_HANDLE_VALUE)
		return std::wcerr << i18n_system::ERROR_USB_DEVICE_INIT << std::endl << std::endl;

	std::unique_ptr<void, decltype([](HDEVINFO hDevInfo){
			SetupDiDestroyDeviceInfoList(hDevInfo);
	})> device_info_set_ptr(SetupDiGetClassDevs(nullptr, "USB", nullptr, DIGCF_PRESENT | DIGCF_ALLCLASSES));

	// Enumerates all USB devices in the device info set
	device_info.cbSize = sizeof(SP_DEVINFO_DATA);
	for(device_index = 0; SetupDiEnumDeviceInfo(device_info_set, device_index, &device_info);){
		++device_index;

		// Gets the device description from the registry property
		if(!SetupDiGetDeviceRegistryProperty(device_info_set, &device_info, SPDRP_DEVICEDESC,
			nullptr, reinterpret_cast<PBYTE>(key), sizeof(key), nullptr)){
			std::wcerr << L'\t' << i18n_system::ERROR_USB_DEVICE_REG << std::endl << std::endl;
			continue;
		}

		// Gets the device instance ID
		if(!SetupDiGetDeviceInstanceId(device_info_set, &device_info, value, sizeof(value), nullptr)){
			std::wcerr << L'\t' << i18n_system::ERROR_USB_DEVICE_INST << std::endl << std::endl;
			continue;
		}

		insert_if_unique<std::string, std::string>(mmap_devices, key, value);
	}

	return std::wcout << i18n::USB << std::endl << mmap_devices;
}
