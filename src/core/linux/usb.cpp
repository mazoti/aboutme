module;

#include <iostream>
#include <fstream>
#include <filesystem>
#include <map>

module core;

import i18n;
import i18n_system;

import common;

std::wostream& usb() noexcept{
	std::fstream file;
	std::string path, value, data;
	std::multimap<std::wstring, std::wstring> mmap_devices;

	for(const auto& entry : std::filesystem::directory_iterator("/sys/bus/usb/devices")){
		value = "";
		path = entry.path();

		// PID
		file.open(path + "/idProduct", std::ios::in);
		if(!file.is_open()) continue;
		std::getline(file, data);
		value += "product ID: " + data;
		file.close();

		// Vendor ID
		file.open(path + "/idVendor", std::ios::in);
		if(!file.is_open()) continue;
		std::getline(file, data);
		value += "\n\t\t\tvendor ID:  " + data;
		file.close();

		// Product
		file.open(path + "/product", std::ios::in);
		if(file.is_open()){
			std::getline(file, data);
			file.close();
			value = data + "\n\t\t\t" + value;
		}else{
			value = "Unknown product\n\t\t\t" + value;
		}

		// Manufacturer
		file.open(path + "/manufacturer", std::ios::in);
		if(file.is_open()){
			std::getline(file, data);
			file.close();
			mmap_devices.insert({std::wstring(data.begin(), data.end()), std::wstring(value.begin(), value.end())});
		}else{
			mmap_devices.insert({L"Unknown manufacturer", std::wstring(value.begin(), value.end())});
		}
	}

	return std::wcout << USB << std::endl << mmap_devices;
}
