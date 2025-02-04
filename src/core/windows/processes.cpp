module;

#include <iostream>
#include <sstream>
#include <string>
#include <span>
#include <map>

#include <Windows.h>
#include <Psapi.h>

module core;

import common;

import i18n;
import i18n_system;

struct handle_closer{ void operator()(HANDLE handle) const{ CloseHandle(handle); }};

std::wostream& processes() noexcept{
	HANDLE process_handle;
	HMODULE module_handle;
	PROCESS_MEMORY_COUNTERS process_memory_counters;
	DWORD processes[1024], needed, module_size_handle;
	ULARGE_INTEGER kernel_time_large, user_time_large;
	FILETIME creation_time, exit_time, kernel_time, user_time;
	TCHAR process_name[MAX_PATH] = TEXT("");

	std::wostringstream woss;
	std::string key, process_key;
	size_t ram_total, cpu_total, count, largest = 0;
	std::multimap<std::string, triple<std::string, size_t, size_t> > ordered_processes;

	if(!EnumProcesses(processes, sizeof(processes), &needed))
		return std::wcerr << ERROR_PROCESSES_ENUM << std::endl;

	// Create span for safe access and safely iterate
	for(const DWORD& process : std::span<DWORD>(processes, (needed / sizeof(DWORD)))){
		if(process){
			// Opens the process and wraps the handle with unique_ptr
			process_handle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, process);
		    if(process_handle == nullptr) continue;
			std::unique_ptr<void, handle_closer> process_handle_ptr(process_handle);

			if(!EnumProcessModules(process_handle, &module_handle, sizeof(module_handle), &module_size_handle))
				continue;
			if(!GetModuleBaseName(process_handle, module_handle, process_name, sizeof(process_name) / sizeof(TCHAR)))
				continue;
			if(!GetProcessMemoryInfo(process_handle, &process_memory_counters, sizeof(process_memory_counters)))
				continue;
			if(!GetProcessTimes(process_handle, &creation_time, &exit_time, &kernel_time, &user_time))
				continue;

			kernel_time_large.LowPart  = kernel_time.dwLowDateTime;
			kernel_time_large.HighPart = kernel_time.dwHighDateTime;
			user_time_large.LowPart    = user_time.dwLowDateTime;
			user_time_large.HighPart   = user_time.dwHighDateTime;

			ordered_processes.insert({
				std::string(process_name),
				triple<std::string, size_t, size_t>(std::to_string(process),
				process_memory_counters.WorkingSetSize,
				kernel_time_large.QuadPart + user_time_large.QuadPart)
			});
		}
	}

	// Finds the largest line size and iterates again to print
	for(typename std::multimap<std::string, triple<std::string, size_t,
		size_t> >::const_iterator it = ordered_processes.begin();
		it != ordered_processes.end(); ++it){
		if(process_key != it->first){
			process_key = it->first;

			std::pair<typename std::multimap<std::string, triple<std::string, size_t, size_t> >::const_iterator,
				typename std::multimap<std::string, triple<std::string, size_t, size_t> >::const_iterator>
				range = ordered_processes.equal_range(process_key);

			ram_total = cpu_total = count = 0;

			for(typename std::multimap<std::string, triple<std::string, size_t, size_t> >::const_iterator iterator
				= range.first; iterator != range.second; ++iterator){

				ram_total += iterator->second.second / 1024;
				cpu_total += iterator->second.third / 10000;
				++count;

				woss.str(L"");
				woss << L"PID:" << std::wstring(iterator->second.first.begin(), iterator->second.first.end());
				if(woss.str().size() > largest) largest = woss.str().size();
				

				woss.str(L"");
				woss << L"RAM:" << std::to_wstring(iterator->second.second / 1024) << L" KB";
				if(woss.str().size() > largest) largest = woss.str().size();

				woss.str(L"");
				woss << L"CPU:" << std::to_wstring(iterator->second.third / 10000) << L" ms";
				if(woss.str().size() > largest) largest = woss.str().size();
			}

			if(count > 1){
				woss.str(L"");
				woss << L"RAM:" << std::to_wstring(ram_total) << L" KB";
				if(woss.str().size() > largest) largest = woss.str().size();

				woss.str(L"");
				woss << L"CPU:" << std::to_wstring(cpu_total) << L" ms";
				if(woss.str().size() > largest) largest = woss.str().size();
			}
		}
	}

	std::wcout << RUNNING_PROCESSES << std::endl;

	for(typename std::multimap<std::string, triple<std::string, size_t,
		size_t> >::const_iterator it = ordered_processes.begin();
		it != ordered_processes.end(); ++it){
		if(process_key != it->first){
			process_key = it->first;
			std::wcout << L'\t' << std::wstring(it->first.begin(), it->first.end()) << L':' << std::endl;

			std::pair<typename std::multimap<std::string, triple<std::string, size_t, size_t> >::const_iterator,
				typename std::multimap<std::string, triple<std::string, size_t, size_t> >::const_iterator>
				range = ordered_processes.equal_range(process_key);

			ram_total = cpu_total = count = 0;

			for(typename std::multimap<std::string, triple<std::string, size_t, size_t> >::const_iterator iterator
				= range.first; iterator != range.second; ++iterator){

				ram_total += iterator->second.second / 1024;
				cpu_total += iterator->second.third / 10000;
				++count;

				print_largest(2, 1, largest, L' ',
					L"PID:" , std::wstring(iterator->second.first.begin(), iterator->second.first.end()),
					L"RAM:",  std::to_wstring(iterator->second.second / 1024) + L" KB",
					L"CPU:",  std::to_wstring(iterator->second.third / 10000) + L" ms");
			}

			if(count > 1){
				print_largest(2, 1, largest, L' ', TOTAL_PROCESSES, L"",
					L"RAM:", std::to_wstring(ram_total) + L" KB",
					L"CPU:", std::to_wstring(cpu_total) + L" ms");
			}
		}
	}
	return std::wcout;
}
