module;

#include <iostream>
#include <sstream>

#include <sys/sysctl.h>
#include <unistd.h>

module core;

import i18n;
import i18n_system;
import common;

std::wostream& memory(void){
	size_t length_memory;
	int64_t total_memory, free_memory;

	// Get total memory
	length_memory = sizeof(total_memory);
	if(sysctlbyname("hw.realmem", &total_memory, &length_memory, nullptr, 0) == -1)
		return std::wcerr << ERROR_FREEBSD_SYSCTLBYNAME << std::endl << std::endl;

	// Get available memory
	free_memory = 0;
	length_memory = sizeof(free_memory);
	if(sysctlbyname("vm.stats.vm.v_free_count", &free_memory, &length_memory, nullptr, 0) == -1)
		return std::wcerr << ERROR_FREEBSD_SYSCTLBYNAME << std::endl << std::endl;

	// Convert free memory pages to bytes
	total_memory /= 1048576;
	free_memory = free_memory * getpagesize() / 1048576;

	return print_title(0, 1, L' ', MEMORY,
		MEMORY_TOTAL,     std::to_wstring(total_memory)               + L" MB",
		MEMORY_USED,      std::to_wstring(total_memory - free_memory) + L" MB",
		MEMORY_AVAILABLE, std::to_wstring(free_memory)                + L" MB");
}
