#include <iostream>
#include <memory>

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
	DWORD mode;
	HANDLE std_handle = nullptr;

	// Saves the console state
	GetConsoleMode(std_handle, &mode);

	// Restores the console to the initial state
	std::unique_ptr<void, decltype([](HANDLE console_handle){
		SetConsoleMode(console_handle, ENABLE_PROCESSED_OUTPUT | ENABLE_WRAP_AT_EOL_OUTPUT);
	})> console_mode_ptr(std_handle);

	// Memory leaks check in debug mode
	#ifndef NDEBUG
		_CrtMemState start_state, end_state, diff_state;
		_CrtMemCheckpoint(&start_state);
	#endif

	// Configures console for wide character (UTF-16) output
	_setmode(_fileno(stdout), _O_U16TEXT);
	_setmode(_fileno(stderr), _O_U16TEXT);

	// System header with version
	std::wcout << std::endl << HEADER << std::endl << std::endl;

	run(envp, ';');

	// Waits for any key to close if no arguments are provided
	if(argc == 1){
		std::wcout << i18n::ANY_KEY_TO_CLOSE << std::endl;
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
