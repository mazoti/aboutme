module;

#include <iostream>
#include <sstream>

#include <memory>

#include <array>
#include <set>

module core;

import i18n;
import i18n_system;
import common;

std::wostream& services(){
	std::string result;
	std::array<char, 128> buffer;
	std::string service;

	std::set<std::string> ordered_services;

	std::unique_ptr<FILE, decltype(&pclose)> pipe(popen("service -e", "r"), pclose);
	if(!pipe) return std::wcerr << ERROR_SERVICES_PIPE << std::endl << std::endl;

	while(fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) result += buffer.data();

	for(std::istringstream stream(result); std::getline(stream, service);) ordered_services.emplace(service);

	return std::wcout << SERVICES << std::endl << ordered_services;
}
