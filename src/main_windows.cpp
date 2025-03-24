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

int main(int argc, char* argv[], const char* envp[]){
	DWORD original_mode;
	HANDLE std_handle;

	// Gets the standard output handle
	std_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	if(std_handle == INVALID_HANDLE_VALUE){
		std::wcerr << L"Failed to get standard output handle" << L'\n';
		return -1;
	}

	// Saves the current console mode
	if(!GetConsoleMode(std_handle, &original_mode)){
		std::wcerr << L"Failed to get console mode." << L'\n';
		return -2;
	}

	// Creates a unique_ptr with custom deleter to restore original mode
	auto restorer = [std_handle, original_mode](void*){ SetConsoleMode(std_handle, original_mode); };
	std::unique_ptr<void, decltype(restorer)> mode_guard(nullptr, restorer);

	// Checks Memory leaks in debug mode
	#ifndef NDEBUG
		_CrtMemState start_state, end_state, diff_state;
		_CrtMemCheckpoint(&start_state);
	#endif

	// Configures console for wide-character (UTF-16) output
	if(_setmode(_fileno(stdout), _O_U16TEXT) == -1){
		std::wcerr << L"Failed to set stdout to wide character mode." << L'\n';
		return -3;
	}

	if(_setmode(_fileno(stderr), _O_U16TEXT) == -1){
		std::wcerr << L"Failed to set stderr to wide character mode." << L'\n';
		return -4;
	}

	// Prints system header with version
	std::wcout << L'\n' << i18n::HEADER << L"\n\n";

	run(envp, ';');

	// Waits for any key to close if no arguments are provided
	if(argc == 1){
		std::wcout << i18n::ANY_KEY_TO_CLOSE << L'\n';
		_getch();
	}

	#ifndef NDEBUG
	_CrtMemCheckpoint(&end_state);
		if(_CrtMemDifference(&diff_state, &start_state, &end_state)){
			std::wcerr << L"ERROR: LEAK FOUND" << L'\n';
			_CrtMemDumpAllObjectsSince(&start_state);
			return -4;
		}
		std::wcout << L"*** No leak found ***" << L'\n';
	#endif

	return 0;
}
