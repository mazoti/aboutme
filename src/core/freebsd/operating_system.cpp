module;

#include <iostream>
#include <ctime>

#include <sys/param.h> 
#include <sys/sysctl.h>

module core;

import i18n;
import i18n_system;

std::wostream& operating_system(){
	int seconds, minutes, hours, days;
	char kernel_version[256], hostname[MAXHOSTNAMELEN];
	struct timespec boot_time;
	time_t now, uptime;

	size_t size = sizeof(kernel_version), hostname_length = sizeof(hostname);
	size_t bt_length = sizeof(boot_time);
	int mib[2] = { CTL_KERN, KERN_BOOTTIME };

	if(sysctlbyname("kern.osrelease", kernel_version, &size, nullptr, 0))
		return std::wcerr << ERROR_KERNEL_VERSION << std::endl << std::endl;

	if(sysctl(mib, 2, &boot_time, &bt_length, nullptr, 0) == -1){
		return std::wcerr << ERROR_SYSCTL_BOOT_TIME << std::endl << std::endl;
	}

	time(&now);
	uptime = now - boot_time.tv_sec;

	// Converts to days, hours, minutes, seconds
	days    = static_cast<int>(uptime / 86400);
	hours   = static_cast<int>(days / 3600);
	minutes = static_cast<int>((uptime % 3600) / 60);
	seconds = static_cast<int>(uptime % 60);

	// Displays uptime
	std::wcout << OPERATING_SYSTEM << std::endl 
		<< L"\tFreeBSD " << kernel_version << std::endl
		<< L"\t" << UPTIME;

	if(days)    std::wcout << L" " << days    << L" " << DAYS;
	if(hours)   std::wcout << L" " << hours   << L" " << HOURS_UPTIME;
	if(minutes) std::wcout << L" " << minutes << L" " << MINUTES;
	if(seconds) std::wcout << L" " << seconds << L" " << SECONDS;

	// Displays hostname
	mib[1] = KERN_HOSTNAME;
	if(sysctl(mib, 2, hostname, &hostname_length, nullptr, 0) == 0){
		hostname[sizeof(hostname) - 1] = '\0';
		if(hostname[0] != '\0') std::wcout << std::endl << L'\t' << NAME << L' ' << hostname;
	}

	return std::wcout << std::endl << std::endl;
}
