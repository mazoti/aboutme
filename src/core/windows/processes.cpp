module;

#include <iostream>
#include <iomanip>
#include <string>

#include <vector>
#include <map>

#include <span>

#define UNICODE
#include <Windows.h>
#include <Psapi.h>

module core;

import common;
import i18n;
import i18n_system;

// Structure to hold process data
struct ProcessData{
	uint32_t pid;         // Process ID
	size_t ram_usage;     // RAM usage in bytes
	size_t cpu_time;      // CPU time in 100-nanosecond intervals
};

// Formats CPU time into appropriate units
std::wstring format_cpu_time(size_t cpu_time){
	size_t ms = cpu_time / 10000; // Convert from 100-ns to milliseconds

	if(ms < 1000) return std::to_wstring(ms) + L" ms";

	if(ms < 60000){
		double seconds = ms / 1000.0;
		return std::to_wstring(seconds) + L" s";
	}

	double minutes = ms / 60000.0;
	return std::to_wstring(minutes) + L" min";
}

// Formats RAM usage into appropriate units
std::wstring format_ram_usage(size_t ram_bytes){
	size_t kb = ram_bytes / 1024; // Convert bytes to KB

	if(kb < 1024) return std::to_wstring(kb) + L" KB";

	if(kb < 1024 * 1024){
		double mb = kb / 1024.0;
		return std::to_wstring(mb) + L" MB";
	}

	double gb = kb / (1024.0 * 1024.0);
	return std::to_wstring(gb) + L" GB";
}

// Lists running processes with PID, CPU, and RAM usage
std::wostream& processes() noexcept{
	// Map to group processes by name
	DWORD processes[1024], needed;
	std::map<std::wstring, std::vector<ProcessData>> process_map;

	// Enumerates processes
	if(!EnumProcesses(processes, sizeof(processes), &needed))
		return std::wcerr << i18n_system::ERROR_PROCESSES_ENUM << L'\n';

	// Collects process data
	for(const DWORD& process : std::span<DWORD>(processes, needed / sizeof(DWORD))){
		if(!process) continue;

		HANDLE process_handle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, process);
		if(process_handle == nullptr) continue;

		std::unique_ptr<void, decltype([](HANDLE h) { CloseHandle(h); })> proc_handle_ptr(process_handle);

		HMODULE module_handle;
		DWORD module_size_handle;
		TCHAR process_name[MAX_PATH] = TEXT("");
		PROCESS_MEMORY_COUNTERS process_memory_counters;
		FILETIME creation_time, exit_time, kernel_time, user_time;

		if(EnumProcessModules(process_handle, &module_handle, sizeof(module_handle), &module_size_handle) &&
			GetModuleBaseName(process_handle, module_handle, process_name, sizeof(process_name) / sizeof(TCHAR)) &&
			GetProcessMemoryInfo(process_handle, &process_memory_counters, sizeof(process_memory_counters)) &&
			GetProcessTimes(process_handle, &creation_time, &exit_time, &kernel_time, &user_time)){

			ULARGE_INTEGER kernel_time_large, user_time_large;
			kernel_time_large.LowPart = kernel_time.dwLowDateTime;
			kernel_time_large.HighPart = kernel_time.dwHighDateTime;
			user_time_large.LowPart = user_time.dwLowDateTime;
			user_time_large.HighPart = user_time.dwHighDateTime;

			std::wstring process_name_wstr = process_name;
			process_map[process_name_wstr].push_back({
				process,
				process_memory_counters.WorkingSetSize,
				kernel_time_large.QuadPart + user_time_large.QuadPart
			});
		}
	}

	// Calculate column widths and totals
	size_t max_pid_width = 0;
	size_t max_cpu_width = 0;
	size_t max_ram_width = 0;

	for(const auto& pair : process_map){
		const auto& processes = pair.second;

		// Handle totals for multiple instances
		if(processes.size() > 1){
			size_t total_ram = 0;
			size_t total_cpu = 0;

			for(const auto& proc : processes){
				total_ram += proc.ram_usage;
				total_cpu += proc.cpu_time;
			}

			std::wstring total_cpu_str = format_cpu_time(total_cpu);
			std::wstring total_ram_str = format_ram_usage(total_ram);

			max_cpu_width = (max_cpu_width > total_cpu_str.length() ? max_cpu_width : total_cpu_str.length());
			max_ram_width = (max_ram_width > total_ram_str.length() ? max_ram_width : total_ram_str.length());
		}

		// Calculates widths for individual processes
		for(const auto& proc : processes){
			std::wstring pid_str = std::to_wstring(proc.pid);
			std::wstring cpu_str = format_cpu_time(proc.cpu_time);
			std::wstring ram_str = format_ram_usage(proc.ram_usage);

			max_pid_width = (max_pid_width > pid_str.length() ? max_pid_width : pid_str.length());
			max_cpu_width = (max_cpu_width > cpu_str.length() ? max_cpu_width : cpu_str.length());
			max_ram_width = (max_ram_width > ram_str.length() ? max_ram_width : ram_str.length());
		}
	}

	// Adjusts widths for headers and labels
	std::wstring pid_header = L"PID";
	std::wstring cpu_header = L"CPU";
	std::wstring ram_header = L"RAM";
	std::wstring total_label = L"Total:";

	max_pid_width = (max_pid_width > pid_header.length()  ? max_pid_width : pid_header.length());
	max_pid_width = (max_pid_width > total_label.length() ? max_pid_width : total_label.length());

	max_cpu_width = (max_cpu_width > cpu_header.length() ? max_cpu_width : cpu_header.length());
	max_ram_width = (max_ram_width > ram_header.length() ? max_ram_width : ram_header.length());

	// Prints process information
	std::wcout << i18n::RUNNING_PROCESSES << L'\n';

	for(const auto& pair : process_map){
		const auto& name = pair.first;
		const auto& processes = pair.second;

		std::wcout << L"\t" << name << L":" << L'\n'
			<< L"\t\t" << std::left << std::setw(max_pid_width) << pid_header
			<< L"  " << std::left << std::setw(max_cpu_width) << cpu_header
			<< L"  " << std::left << std::setw(max_ram_width) << ram_header << L'\n';

		for(const auto& proc : processes){
			std::wstring pid_str = std::to_wstring(proc.pid);
			std::wstring cpu_str = format_cpu_time(proc.cpu_time);
			std::wstring ram_str = format_ram_usage(proc.ram_usage);
			std::wcout << L"\t\t" << std::left << std::setw(max_pid_width) << pid_str
				<< L"  " << std::left << std::setw(max_cpu_width) << cpu_str
				<< L"  " << std::left << std::setw(max_ram_width) << ram_str << L'\n';
		}

		if(processes.size() > 1){
			size_t total_ram = 0;
			size_t total_cpu = 0;

			for(const auto& proc : processes){
				total_ram += proc.ram_usage;
				total_cpu += proc.cpu_time;
			}
			std::wstring total_cpu_str = format_cpu_time(total_cpu);
			std::wstring total_ram_str = format_ram_usage(total_ram);
			std::wcout << L"\t\t" << std::left << std::setw(max_pid_width) << total_label
				<< L"  " << std::left << std::setw(max_cpu_width) << total_cpu_str
				<< L"  " << std::left << std::setw(max_ram_width) << total_ram_str << L"\n\n";
		}

		if(processes.size() == 1) std::wcout << L'\n';
	}

	return std::wcout;
}
