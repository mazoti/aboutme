module;

#include <iostream>

module core;


std::wostream& network() noexcept{

	return std::wcout << L"NETWORK" << std::endl;

}
