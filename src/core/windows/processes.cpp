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

// Helper function to format a process metric and tracks the largest line size
static inline void print_process(std::wostringstream& woss, const wchar_t* description, size_t total,
const wchar_t* data_size, size_t& largest){
	woss.str(L"");
	woss << description << std::to_wstring(total) << data_size;
	if(woss.str().size() > largest) largest = woss.str().size();
}

// Lists running processes with their PID, CPU and RAM usage
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

	// Multimap to store process data: name -> (PID, RAM, CPU)
	std::multimap<std::string, triple<std::string, size_t, size_t> > ordered_processes;

	// Enumerates all running processes
	if(!EnumProcesses(processes, sizeof(processes), &needed))
		return std::wcerr << i18n_system::ERROR_PROCESSES_ENUM << std::endl;

	// Use std::span to safely iterate over process IDs
	for(const DWORD& process : std::span<DWORD>(processes, (needed / sizeof(DWORD)))){
		if(process){
			// Opens the process and wraps the handle with unique_ptr
			process_handle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, process);
		    if(process_handle == nullptr) continue;

			std::unique_ptr<void, decltype([](HANDLE handle){CloseHandle(handle);})> proc_handle_ptr(process_handle);

			// Gets module info, name, memory usage and time
			if(!(EnumProcessModules(process_handle, &module_handle, sizeof(module_handle), &module_size_handle) &&
				GetModuleBaseName(process_handle, module_handle, process_name, sizeof(process_name) / sizeof(TCHAR)) &&
				GetProcessMemoryInfo(process_handle, &process_memory_counters, sizeof(process_memory_counters)) &&
				GetProcessTimes(process_handle, &creation_time, &exit_time, &kernel_time, &user_time)))	continue;

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

				// Calculate size of PID line
				woss.str(L"");
				woss << L"PID:" << std::wstring(iterator->second.first.begin(), iterator->second.first.end());
				if(woss.str().size() > largest) largest = woss.str().size();

				print_process(woss, L"CPU:", iterator->second.third / 10000, L" ms", largest);
				print_process(woss, L"RAM:", iterator->second.second / 1024, L" KB", largest);		
			}

			if(count > 1){
				print_process(woss, L"CPU:", cpu_total, L" ms", largest);
				print_process(woss, L"RAM:", ram_total, L" KB", largest);
			}
		}
	}

	// Second pass: prints formatted process informations
	std::wcout << i18n::RUNNING_PROCESSES << std::endl;

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

				// Prints PID, CPU, and RAM with assumed print_largest function
				print_largest(2, 1, largest, L' ',
					L"PID:", std::wstring(iterator->second.first.begin(), iterator->second.first.end()),
					L"CPU:", std::to_wstring(iterator->second.third / 10000) + L" ms",
					L"RAM:", std::to_wstring(iterator->second.second / 1024) + L" KB");
			}

			if(count > 1){
				print_largest(2, 1, largest, L' ', i18n::TOTAL_PROCESSES, L"",
					L"CPU:", std::to_wstring(cpu_total) + L" ms",
					L"RAM:", std::to_wstring(ram_total) + L" KB");
			}
		}
	}
	return std::wcout;
}
