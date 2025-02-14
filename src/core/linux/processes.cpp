module;

#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <charconv>
#include <map>

#include <unistd.h>

module core;

import i18n;
import common;

std::wostream& processes() noexcept{
	int value;
	long stime, utime;
	size_t vmsize, total_ms, ram_total, cpu_total, count;

	std::string name, line, temp, process_key;
	std::fstream file;

	std::multimap<std::string, triple<std::string, size_t, size_t> > ordered_processes;

	for(const auto& entry : std::filesystem::directory_iterator("/proc")){
		if(entry.is_directory()){

			const auto& PID = entry.path().filename().string();

			auto result = std::from_chars(PID.data(), PID.data() + PID.size(), value);

			if(result.ec == std::errc()){
				file.open("/proc/" + PID + "/cmdline", std::ios::in);
				if(!file.is_open()) continue;
				std::getline(file, name, ' ');
				file.close();

				if(name == "") continue;

				file.open("/proc/" + PID + "/status", std::ios::in);
				if(!file.is_open()) continue;

				while(std::getline(file, line)){
					std::size_t pos = line.find("VmSize:");
					if(pos != std::string::npos){
						pos += 8;
						std::istringstream iss(line.substr(pos));
						iss >> vmsize;
						vmsize /= 1024;
						break;
					}
				}
 				file.close();

				file.open("/proc/" + PID + "/stat", std::ios::in);
				if(!file.is_open()) continue;
				std::getline(file, line);
				std::istringstream iss(line);

				// Skip first 13 fields
				for(int i = 0; i < 13; ++i) iss >> temp;

				// Read utime and stime
				iss >> utime >> stime;

				total_ms = (size_t)(((utime + stime) * 1000.0) / sysconf(_SC_CLK_TCK));
 				file.close();
				ordered_processes.insert({name, triple<std::string, size_t, size_t>(PID, vmsize, total_ms)});
			}
		}
	}

	std::wcout << RUNNING_PROCESSES << std::endl;

	for(auto it = ordered_processes.begin(); it != ordered_processes.end(); ++it){
		if(process_key != it->first){
			process_key = it->first;
			std::wcout << L'\t' << std::wstring(it->first.begin(), it->first.end()) << L':' << std::endl;

			auto range = ordered_processes.equal_range(process_key);

			ram_total = cpu_total = count = 0;

			for(auto iterator = range.first; iterator != range.second; ++iterator){
				ram_total += iterator->second.second;
				cpu_total += iterator->second.third;
				++count;

				print(2, 1, L' ',
					L"PID: " , std::wstring(iterator->second.first.begin(), iterator->second.first.end()),
					L"RAM: ",  std::to_wstring(iterator->second.second) + L" KB",
					L"CPU: ",  std::to_wstring(iterator->second.third) + L" ms");
			}

			if(count > 1){
				print(2, 1, L' ', TOTAL_PROCESSES, L"",
					L"RAM: ", std::to_wstring(ram_total) + L" KB",
					L"CPU: ", std::to_wstring(cpu_total) + L" ms");
			}
		}
	}
	return std::wcout;
}
