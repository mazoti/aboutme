#include <iostream>
#include <locale>

import i18n;
import core;

int main(int argc, const char* argv[], const char* envp[]) {
	// Use a UTF-8 locale
	std::setlocale(LC_ALL, "");

	// System header with version
	std::wcout << std::endl << i18n::HEADER << std::endl << std::endl;

	run(envp, ':');

	if(argc == 1){
		std::wcout << i18n::ANY_KEY_TO_CLOSE << std::endl;
		getchar();
	}

	return 0;
}
