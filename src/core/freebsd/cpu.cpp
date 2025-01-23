module;

#include <iostream>

#include <sys/sysctl.h>

module core;

import i18n;
import i18n_system;

// Displays CPU details
std::wostream& cpu(){
	int cores;
	size_t len;
	char model[256], machine[256];

	// CPU model
	len = sizeof(model);
	if(sysctlbyname("hw.model", &model, &len, nullptr, 0))
		return std::wcerr << ERROR_SYSCTLBYNAME << std::endl << std::endl;

	// CPU architecture
	if(sysctlbyname("hw.machine_arch", &machine, &len, nullptr, 0))
		return std::wcerr << ERROR_SYSCTLBYNAME << std::endl << std::endl;

	// CPU cores
	len = sizeof(cores);
	if(sysctlbyname("hw.ncpu", &cores, &len, nullptr, 0))
		return std::wcerr << ERROR_SYSCTLBYNAME << std::endl << std::endl;

	return std::wcout << CPU << std::endl
		<< L'\t' << MODEL << L' ' << model << std::endl
		<< L'\t' << ARCHITECTURE << L' ' << machine << std::endl
		<< L'\t' << CORES << L' ' << cores << std::endl
		<< std::endl;
}
