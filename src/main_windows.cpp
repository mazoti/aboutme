#include <iostream>

// Windows API
#include <Windows.h>
#include <conio.h>
#include <fcntl.h>
#include <io.h>

// Memory leak check in debug mode
#ifndef NDEBUG
	#define _CRTDBG_MAP_ALLOC
	#include <crtdbg.h>
#endif

import i18n;
import core;

int main(int argc, char* argv[], char* envp[]){
	// Memory leaks check in debug mode
	#ifndef NDEBUG
		_CrtMemState start_state, end_state, diff_state;
		_CrtMemCheckpoint(&start_state);
	#endif

	// Sets the console to handle wide characters (32 bits)
	_setmode(_fileno(stdout), _O_U16TEXT);
	_setmode(_fileno(stderr), _O_U16TEXT);

	// System header with version
	std::wcout << std::endl << HEADER << std::endl << std::endl;

	run(envp, ';');

	// Waits for any key to close if no additional arguments are provided
	if(argc == 1){
		std::wcout << ANY_KEY_TO_CLOSE << std::endl;
		_getch();
	}

	#ifndef NDEBUG
		_CrtMemCheckpoint(&end_state);
		if(_CrtMemDifference(&diff_state, &start_state, &end_state)){
			std::wcerr << L"ERROR: LEAK FOUND" << std::endl;
			return 1;
		}
		std::wcout << L"*** No leak found ***" << std::endl;
	#endif

	return 0;
}
