module;

#include <iostream>
#include <span>

import common;

module core;

void run(char* envp[], char separator) noexcept{
	#if defined(ENABLE_CPU)
		cpu();
	#endif

	#if defined(ENABLE_GPU)
		gpu();
	#endif

	#if defined(ENABLE_MEMORY)
		memory();
	#endif

	#if defined(ENABLE_HARD_DISKS)
		hard_disks();
	#endif

	#if defined(ENABLE_USB)
		usb();
	#endif

	#if defined(ENABLE_SYSTEM_CLOCK)
		system_clock();
	#endif

	#if defined(ENABLE_NETWORK)
		network();
	#endif

	#if defined(ENABLE_WIFI)
		wifi();
	#endif

	#if defined(ENABLE_BLUETOOTH)
		bluetooth();
	#endif

	#if defined(ENABLE_BATTERY)
		battery();
	#endif

	#if defined(ENABLE_OPERATING_SYSTEM)
		operating_system();
	#endif

	#if defined(ENABLE_FEATURES)
		features();
	#endif

	#if defined(ENABLE_INSTALLED)
		installed();
	#endif

	#if defined(ENABLE_RESTORE)
		restore();
	#endif

	#if defined(ENABLE_PROCESSES)
		processes();
	#endif

	#if defined(ENABLE_SERVICES)
		services();
	#endif

	#if defined(ENABLE_TASKS)
		tasks();
	#endif

	#if defined(ENABLE_STARTUP)
		startup();
	#endif

	#if defined(ENABLE_ENVIRONMENT_VARIABLES)
		size_t count = 0;
		while(envp[count] != nullptr) ++count;
		std::span<char*> envp_span(envp, count);
		environment_variables(envp_span, separator);
	#endif
}
