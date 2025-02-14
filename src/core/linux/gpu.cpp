module;

#include <iostream>
#include <fstream>
#include <filesystem>

module core;

import i18n;
import i18n_system;

// Displays GPU details
std::wostream& gpu() noexcept{
	if(!std::filesystem::exists("/sys/bus/pci/devices")){
//TODO
		return std::wcerr << L"PCI sysfs path not found\n";
	}

	for(const auto& entry : std::filesystem::directory_iterator("/sys/bus/pci/devices")){
		std::string class_path = "/sys/bus/pci/devices/" + entry.path().filename().string() + "/class";

		std::ifstream file(class_path);
		if(file){
			std::string line;
			std::getline(file, line);

//			if(line == "0x0300"){
				std::wcout << L"\n===" << std::wstring(class_path.begin(), class_path.end()) << L" ===\n";
				std::wcout << L"\n===" << std::wstring(line.begin(), line.end()) << L" ===************************\n";
//			}
		}
		
	}		
	
		
		
		
		


	return std::wcout << L"GPU" << std::endl;
}
























