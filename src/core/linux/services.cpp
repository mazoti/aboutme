module;

#include <iostream>
#include <filesystem>
#include <set>

#include <memory>
#include <array>

module core;

import i18n;
import i18n_system;
import common;

std::wostream& services() noexcept{
	std::string result, line, service;

	std::array<char, 128> buffer;
	std::set<std::string> ordered_services;

	std::unique_ptr<FILE, decltype(&pclose)> pipe(
		popen("systemctl list-units --type=service --state=running", "r"),
		pclose);
	if(!pipe) return std::wcerr << ERROR_LINUX_PIPE << std::endl << std::endl;

	while(fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) result += buffer.data();

	// Ignore header
	std::istringstream stream(result);
	std::getline(stream, line);

	for(std::getline(stream, line); line != ""; std::getline(stream, line)){
		std::istringstream data(line);
		data >> service;
		ordered_services.emplace(service);
	}

	return std::wcout << SERVICES << std::endl << ordered_services;
}
