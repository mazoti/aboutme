module;

#include <iostream>
#include <memory>
#include <map>

#include <Windows.h>
#include <SetupAPI.h>

#pragma comment(lib, "setupapi.lib")

module core;

import i18n;
import i18n_system;

import common;

// Custom deleter for HDEVINFO
struct device_info_destroyer{ void operator()(HDEVINFO hDevInfo) const{ SetupDiDestroyDeviceInfoList(hDevInfo);}};

std::wostream& usb() noexcept{
	char key[64], value[64];
	DWORD device_index;
	SP_DEVINFO_DATA device_info;

	std::multimap<std::string, std::string> mmap_devices;

	// Create a unique_ptr with the custom deleter
	HDEVINFO device_info_set = SetupDiGetClassDevs(nullptr, "USB", nullptr, DIGCF_PRESENT | DIGCF_ALLCLASSES);
	if(device_info_set == INVALID_HANDLE_VALUE)
		return std::wcerr << ERROR_USB_DEVICE_INIT << std::endl << std::endl;

	std::unique_ptr<void, device_info_destroyer> device_info_set_ptr(SetupDiGetClassDevs(
		nullptr, "USB", nullptr, DIGCF_PRESENT | DIGCF_ALLCLASSES));

	device_info.cbSize = sizeof(SP_DEVINFO_DATA);
	for(device_index = 0; SetupDiEnumDeviceInfo(device_info_set, device_index, &device_info);){
		++device_index;

		// Get the device description
		if(!SetupDiGetDeviceRegistryProperty(device_info_set, &device_info, SPDRP_DEVICEDESC,
			nullptr, reinterpret_cast<PBYTE>(key), sizeof(key), nullptr)){
			std::wcerr << L'\t' << ERROR_USB_DEVICE_REG << std::endl << std::endl;
			continue;
		}

		// Get the device instance ID
		if(!SetupDiGetDeviceInstanceId(device_info_set, &device_info, value, sizeof(value), nullptr)){
			std::wcerr << L'\t' << ERROR_USB_DEVICE_INST << std::endl << std::endl;
			continue;
		}

		mmap_devices.insert({key, value});
	}

	return std::wcout << i18n::USB << std::endl << mmap_devices;
}
