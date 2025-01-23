module;

#include <iostream>
#include <fstream>

module core;

import i18n;
import i18n_system;

static std::string arch(const std::string& line){
	if(line.starts_with("cpu family")) return "AMD64";
	if(line.starts_with("CPU architecture")) return "ARM";
	if(line.starts_with("cpu") && line.find("POWER") != std::string::npos) return "PowerPC";
	if(line.starts_with("platform") && line.find("RISC-V") != std::string::npos) return "RISC-V";
	if(line.starts_with("cpu type") && line.find("MIPS") != std::string::npos) return "MIPS";

	return "";
}

// Displays CPU details
std::wostream& cpu(){
	int cores = 0;
	std::string model="", architecture = "";

	std::ifstream file("/proc/cpuinfo");
	if(!file) return std::wcerr << ERROR_CPU_INFO << std::endl << std::endl;

	for(std::string line; std::getline(file, line);) {
		if(model == ""){
			if(line.starts_with("model name")) model = line.substr(line.find(':') + 2);
		}
		if(architecture == "") architecture = arch(line);
		if(!line.size()) ++cores;
	}

	return std::wcout << CPU << std::endl
		<< L'\t' << MODEL << L' ' << std::wstring(model.begin(), model.end()) << std::endl
		<< L'\t' << ARCHITECTURE << L' ' << std::wstring(architecture.begin(), architecture.end()) << std::endl
		<< L'\t' << CORES << L' ' << cores << std::endl
		<< std::endl;
}
