module;

#include <iostream>

import common;

module core;

// Runs all defined modules
void run(const char* envp[], char separator) noexcept{
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

	#if defined(ENABLE_SHARED)
		shared();
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

	#if defined(ENABLE_TRASH)
		trash();
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

	#if defined(ENABLE_SCHEDULED_TASKS)
		scheduled_tasks();
	#endif

	#if defined(ENABLE_STARTUP)
		startup();
	#endif

	#if defined(ENABLE_ENVIRONMENT_VARIABLES)
		environment_variables(envp, separator);
	#endif
}
