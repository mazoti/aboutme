module;

#include <iostream>

module core;

std::wostream& startup(){
	return std::wcout << L"STARTUP" << std::endl;
}
