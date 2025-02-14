module;

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>

#include <limits.h>
#include <unistd.h>
#include <sys/utsname.h>

module core;

import i18n;
import i18n_system;

static std::wstring name(){
	std::wostringstream woss;
	char hostname[HOST_NAME_MAX];

	gethostname(hostname, HOST_NAME_MAX);
	woss << L'\t' << NAME << L' ' << hostname << std::endl;

	return woss.str();
}

static std::wstring uptime(){
	int days, hours, minutes, seconds;
	double uptime_seconds;
	std::wostringstream woss;

	std::wifstream file("/proc/uptime");

	if(file){
		file >> uptime_seconds;

		days    = static_cast<int>(uptime_seconds / 86400); // 60 * 60 * 24
		hours   = static_cast<int>(uptime_seconds / 3600 - days * 24);
		minutes = static_cast<int>(uptime_seconds / 60 - days * 1440 - hours * 60);
		seconds = static_cast<int>(uptime_seconds - days * 86400 - hours * 3600 - minutes * 60);

		woss << L"\t" << UPTIME;

		if(days)    woss << L" " << days    << L" " << DAYS;
		if(hours)   woss << L" " << hours   << L" " << HOURS_UPTIME;
		if(minutes) woss << L" " << minutes << L" " << MINUTES;
		if(seconds) woss << L" " << seconds << L" " << SECONDS;

		return woss.str();
	}

	return L"";
}

std::wostream& operating_system() noexcept{
	struct utsname buffer;
	std::wstring line, version_info;
	std::wifstream file("/etc/os-release");

	if(uname(&buffer)){
		return std::wcerr << ERROR_KERNEL_VERSION << std::endl << std::endl;
	}

	version_info = std::wstring(buffer.release, buffer.release + std::strlen(buffer.release));

	if(file){
		while(std::getline(file, line)){
			if(!line.rfind(L"NAME=", 0)){
				line = line.substr(5); // Skip 'NAME='
				if (line.front() == L'"') line.erase(line.begin());   // Remove starting quote
				if (line.back()  == L'"') line.erase(line.end() - 1); // Remove ending quote

				return std::wcout << OPERATING_SYSTEM << std::endl
						<< L"\t" << line << L' ' << version_info << std::endl
						<< uptime() << std::endl
						<< name() << std::endl;
			}
		}
	}

	return std::wcout << OPERATING_SYSTEM  << std::endl
			<< L"\tLinux " << version_info << std::endl 
			<< uptime() << std::endl
			<< name() << std::endl;
}
