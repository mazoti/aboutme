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

import i18n;
import i18n_system;

struct process_data{ uint32_t pid; size_t ram_usage, cpu_time; };

// Formats CPU time into appropriate units
static std::wstring format_cpu_time(size_t cpu_time){
	const size_t ms = cpu_time / 10000; // Converts from 100-ns to milliseconds

	if(ms < 1000 ) return std::to_wstring(ms) + L" ms";
	if(ms < 60000) return std::to_wstring(ms / 1000) + L" s";

	return std::to_wstring(ms / 60000) + L" min";
}

// Formats RAM usage into appropriate units
static std::wstring format_ram_usage(size_t ram_bytes){
	const size_t kb = ram_bytes / 1024; // Convert bytes to KB

	if(kb < 1024)    return std::to_wstring(kb)        + L" KB";
	if(kb < 1048576) return std::to_wstring(kb / 1024) + L" MB";

	return std::to_wstring(kb / 1048576) + L" GB";
}

// Lists running processes with PID, CPU, and RAM usage
std::wostream& processes() noexcept{
	size_t max_pid_width, max_cpu_width, max_ram_width, total_ram, total_cpu;

	HANDLE process_handle;
	HMODULE module_handle;

	PROCESS_MEMORY_COUNTERS process_memory_counters;
	FILETIME creation_time, exit_time, kernel_time, user_time;

	DWORD module_size_handle, processes[1024], needed;
	ULARGE_INTEGER kernel_time_large, user_time_large;

	TCHAR process_name[MAX_PATH] = TEXT("");

	std::wstring total_cpu_str, total_ram_str, pid_str, cpu_str, ram_str;

	constexpr size_t pid_header_size  = std::size(i18n::PID_HEADER);
	constexpr size_t cpu_header_size  = std::size(i18n::CPU_HEADER);
	constexpr size_t ram_header_size  = std::size(i18n::RAM_HEADER);
	constexpr size_t total_label_size = std::size(i18n::TOTAL_PROCESSES);

	// Maps to group processes by name
	std::map<std::wstring, std::vector<process_data>> process_map;

	// Enumerates processes
	if(!EnumProcesses(processes, sizeof(processes), &needed))
		return std::wcerr << i18n_system::ERROR_PROCESSES_ENUM << L'\n';

	// Collects process data
	for(const DWORD& process : std::span<DWORD>(processes, needed / sizeof(DWORD))){
		if(!process) continue;

		process_handle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, process);
		if(process_handle == nullptr) continue;

		std::unique_ptr<void, decltype([](HANDLE h){ CloseHandle(h); })> proc_handle_ptr(process_handle);

		if(EnumProcessModules(process_handle, &module_handle, sizeof(module_handle), &module_size_handle)        &&
			GetModuleBaseName(process_handle, module_handle, process_name, sizeof(process_name) / sizeof(TCHAR)) &&
			GetProcessMemoryInfo(process_handle, &process_memory_counters, sizeof(process_memory_counters))      &&
			GetProcessTimes(process_handle, &creation_time, &exit_time, &kernel_time, &user_time)){

			kernel_time_large.LowPart  = kernel_time.dwLowDateTime;
			kernel_time_large.HighPart = kernel_time.dwHighDateTime;
			user_time_large.LowPart    = user_time.dwLowDateTime;
			user_time_large.HighPart   = user_time.dwHighDateTime;

			process_map[process_name].push_back({
				process,
				process_memory_counters.WorkingSetSize,
				kernel_time_large.QuadPart + user_time_large.QuadPart
			});
		}
	}

	// Calculates column widths and totals
	max_pid_width = max_cpu_width = max_ram_width = 0;

	for(const auto& pair : process_map){
		// Handles totals for multiple instances
		if(pair.second.size() > 1){
			total_ram = total_cpu = 0;

			for(const auto& proc : pair.second){
				total_ram += proc.ram_usage;
				total_cpu += proc.cpu_time;
			}

			total_cpu_str = format_cpu_time(total_cpu);
			total_ram_str = format_ram_usage(total_ram);

			max_cpu_width = (max_cpu_width > total_cpu_str.length() ? max_cpu_width : total_cpu_str.length());
			max_ram_width = (max_ram_width > total_ram_str.length() ? max_ram_width : total_ram_str.length());
		}

		// Calculates widths for individual processes
		for(const auto& proc : pair.second){
			pid_str = std::to_wstring(proc.pid);
			cpu_str = format_cpu_time(proc.cpu_time);
			ram_str = format_ram_usage(proc.ram_usage);

			max_pid_width = (max_pid_width > pid_str.length() ? max_pid_width : pid_str.length());
			max_cpu_width = (max_cpu_width > cpu_str.length() ? max_cpu_width : cpu_str.length());
			max_ram_width = (max_ram_width > ram_str.length() ? max_ram_width : ram_str.length());
		}
	}

	// Adjusts widths for headers and labels
	max_pid_width = (max_pid_width > pid_header_size ? max_pid_width : pid_header_size);

	max_pid_width = (max_pid_width > total_label_size ? max_pid_width : total_label_size);
	max_cpu_width = (max_cpu_width > cpu_header_size  ? max_cpu_width : cpu_header_size);
	max_ram_width = (max_ram_width > ram_header_size  ? max_ram_width : ram_header_size);

	// Prints process information
	std::wcout << i18n::RUNNING_PROCESSES << L'\n';

	for(const auto& pair : process_map){
		std::wcout << L'\t'   << pair.first << L":\n"
		           << L"\t\t" << std::left  << std::setw(static_cast<std::streamsize>(max_pid_width)) << i18n::PID_HEADER
		           << L"  "   << std::left  << std::setw(static_cast<std::streamsize>(max_cpu_width)) << i18n::CPU_HEADER
		           << L"  "   << std::left  << std::setw(static_cast<std::streamsize>(max_ram_width)) << i18n::RAM_HEADER
		           << L'\n';

		for(const auto& proc : pair.second){
			pid_str = std::to_wstring(proc.pid);
			cpu_str = format_cpu_time(proc.cpu_time);
			ram_str = format_ram_usage(proc.ram_usage);

			std::wcout << L"\t\t" << std::left << std::setw(static_cast<std::streamsize>(max_pid_width)) << pid_str
			           << L"  "   << std::left << std::setw(static_cast<std::streamsize>(max_cpu_width)) << cpu_str
			           << L"  "   << std::left << std::setw(static_cast<std::streamsize>(max_ram_width)) << ram_str
			           << L'\n';
		}

		if(pair.second.size() > 1){
			total_ram = total_cpu = 0;

			for(const auto& proc : pair.second){
				total_ram += proc.ram_usage;
				total_cpu += proc.cpu_time;
			}

			total_cpu_str = format_cpu_time(total_cpu);
			total_ram_str = format_ram_usage(total_ram);

			std::wcout << L"\n\t\t" << std::left << std::setw(static_cast<std::streamsize>(max_pid_width))
			           << i18n::TOTAL_PROCESSES
			           << L"  "     << std::left << std::setw(static_cast<std::streamsize>(max_cpu_width))
			           << total_cpu_str
			           << L"  "     << std::left << std::setw(static_cast<std::streamsize>(max_ram_width))
			           << total_ram_str << L"\n\n";
		}

		if(pair.second.size() == 1) std::wcout << L'\n';
	}

	return std::wcout;
}
