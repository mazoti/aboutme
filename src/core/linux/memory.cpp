module;

#include <iostream>
#include <sstream>

#include <sys/sysinfo.h>
#include <unistd.h>

module core;

import i18n;
import common;

std::wostream& memory() noexcept{
	struct sysinfo system_info;

	if(sysinfo(&system_info)) return std::wcerr << ERROR_MEMORY << std::endl << std::endl;

	return print_title(0, 1, L' ', MEMORY,
		MEMORY_TOTAL,     std::to_wstring( system_info.totalram / 1048576) + L" MB",
		MEMORY_USED,      std::to_wstring((system_info.totalram - system_info.freeram) / 1048576) + L" MB",
		MEMORY_AVAILABLE, std::to_wstring( system_info.freeram  / 1048576) + L" MB");
}
