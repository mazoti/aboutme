module;

#include <iostream>
#include <sstream>
#include <memory>

#include <array>
#include <map>

module core;

import i18n;
import i18n_system;
import common;

std::wostream& processes(){
	std::string result, line, tmp, PID;
	std::array<char, 128> buffer;

	std::multimap<std::string, triple<std::string, float, float> > ordered_processes;

	std::unique_ptr<FILE, decltype(&pclose)> pipe(popen("top -b", "r"), pclose);
//	if(!pipe) return std::wcerr << ERROR_FREEBSD_PIPE << std::endl << std::endl;

	while(fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) result += buffer.data();

	// Ignore header
	std::istringstream stream(result);
	for(int i=0; i< 6; ++i) std::getline(stream, line);

	for(std::getline(stream, line); line != ""; std::getline(stream, line)){
		std::istringstream data(line);

		// Gets PID;
		data >> PID;

		// Gets USER
		data >> tmp;

		// Gets THR
		data >> tmp;

		// Gets PRI
		data >> tmp;

		// Gets NICE
		data >> tmp;

		// Gets SIZE
		data >> tmp;

		// Gets RES
		data >> tmp;




		// Gets STATE
		data >> tmp;


		// Gets C
		data >> tmp;

		// Gets TIME
		data >> tmp;

		// Gets WCPU
		data >> tmp;


		// Gets COMMAND
		data >> tmp;



		std::wcout << std::wstring(line.begin(), line.end()) << L" COMMAND: "
		<< std::wstring(tmp.begin(), tmp.end()) << std::endl;

	}


	return	std::wcout << RUNNING_PROCESSES << std::endl;
}
