#include <iostream>
#include <locale>

import i18n;
import core;

int main(int argc, char* argv[], char* envp[]){
	// Set the locale to support Unicode
	std::locale::global(std::locale("en_US.UTF-8"));
	std::wcout.imbue(std::locale());
	std::wcerr.imbue(std::locale());

	// System header with version
	std::wcout << std::endl << HEADER << std::endl << std::endl;

	run(envp, ':');

	if(argc == 1){
		std::wcout << ANY_KEY_TO_CLOSE << std::endl;
		getchar();
	}

	return 0;
}
