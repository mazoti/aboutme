module;

#include <iostream>
#include <memory>

#include <string>
#include <string_view>
#include <array>

module core;

import i18n;
import i18n_system;

import common;

std::wostream& usb(){
	size_t position;

	std::string result;
	std::string_view result_view, data;
	std::array<char, 128> buffer;

	std::unique_ptr<FILE, decltype(&pclose)> pipe(popen("usbconfig", "r"), pclose);

	if(!pipe) return std::wcerr << ERROR_FREEBSD_PIPE << std::endl << std::endl;

	while(fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) result += buffer.data();

	std::wcout << USB << std::endl;

	result_view = result;

	for(position = result_view.find('<'); position != std::string::npos; position = result_view.find('<')){
		result_view.remove_prefix(position + 1);
		position = result_view.find('>');
		if(position == std::string::npos) break;

		data = result_view.substr(0, position);
		std::wcout << L'\t' << std::wstring(data.begin(), data.end()) << std::endl;
		result_view.remove_prefix(position + 1);
	}

	return std::wcout << std::endl;
}
