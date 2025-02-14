module;

#include <iostream>

module core;

std::wostream& startup() noexcept{
	return std::wcout << L"STARTUP" << std::endl;
}
