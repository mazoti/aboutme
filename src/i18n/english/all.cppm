module;

export module i18n;

export namespace i18n{

	// src/main_$(SYSTEM).cpp
	inline constexpr wchar_t ANY_KEY_TO_CLOSE[] = L"Press any key to close...";

	// src/common/environment_variables.cpp
#if defined(ENABLE_ENVIRONMENT_VARIABLES)
	inline constexpr wchar_t ENVIRONMENT_VARIABLES[]         = L"\U0001F333 Environment variables:";
	inline constexpr wchar_t ENVIRONMENT_VARIABLES_WARNING[] = L"\U000026A0 Warning: check if this path is correct";
#endif

	// src/common/system_clock.cpp
#if defined(ENABLE_SYSTEM_CLOCK)
	inline constexpr wchar_t DATE[]                 = L"Date (DD/MM/YY):";
	inline constexpr wchar_t DAYLIGHT_SAVING_TIME[] = L"Daylight saving time";
	inline constexpr wchar_t GMT[]                  = L"Timezone:";
	inline constexpr wchar_t HOUR[]                 = L"hour";
	inline constexpr wchar_t HOURS[]                = L"hours";
	inline constexpr wchar_t SYSTEM_CLOCK[]         = L"\U0001F556 System clock:";
	inline constexpr wchar_t TIME[]                 = L"Time (HH:MM:SS):";
#endif

	// src/core/$(SYSTEM)/battery.cpp
#if defined(ENABLE_BATTERY)
	inline constexpr wchar_t BATTERY_AC_NOT_CONNECTED[] = L"Running on battery";
	inline constexpr wchar_t BATTERY_AC_POWER[]         = L"Running on AC power";
	inline constexpr wchar_t BATTERY_DAYS[]             = L"days";
	inline constexpr wchar_t BATTERY_HOURS[]            = L"hours";
	inline constexpr wchar_t BATTERY_MINUTES[]          = L"minutes";
	inline constexpr wchar_t BATTERY_STATUS[]           = L"\U0001F50B Battery:";
#endif

// src/core/$(SYSTEM)/bluetooth.cpp
#if defined(ENABLE_BLUETOOTH)
	inline constexpr wchar_t BLUETOOTH[] = L"\U0001F91D Bluetooth:";
#endif

// src/core/$(SYSTEM)/cpu.cpp
#if defined(ENABLE_CPU)
	inline constexpr wchar_t ARCHITECTURE[]           = L"Architecture:";
	inline constexpr wchar_t INSTRUCTIONS_SUPPORTED[] = L"Supported instructions:";
	inline constexpr wchar_t THREADS[]                = L"Threads:";
	inline constexpr wchar_t CPU[]                    = L"\U0001F9EE CPU:";
#endif

// src/core/$(SYSTEM)/features.cpp
#if defined(ENABLE_FEATURES)
	inline constexpr wchar_t FEATURES[] = L"\U0001F527 Features:";
#endif

// src/core/$(SYSTEM)/gpu.cpp
#if defined(ENABLE_GPU)
	inline constexpr wchar_t GPU[]              = L"\U0001F31F GPU:";
	inline constexpr wchar_t GPU_MEMORY[]       = L"Memory:";
	inline constexpr wchar_t VIDEO_RESOLUTION[] = L"Resolution:";
#endif

// src/core/$(SYSTEM)/hard_disks.cpp
#if defined(ENABLE_HARD_DISKS)
	inline constexpr wchar_t ALL[]        = L"All:";
	inline constexpr wchar_t AVAILABLE[]  = L"Available:";
	inline constexpr wchar_t HARD_DISKS[] = L"\U0001F4BF Hard disks:";
	inline constexpr wchar_t TOTAL[]      = L"Total:";
	inline constexpr wchar_t USED[]       = L"Used:";
#endif

// src/core/$(SYSTEM)/installed.cpp
#if defined(ENABLE_INSTALLED)
	inline constexpr wchar_t INSTALLED_PROGRAMS[] = L"\U0001F4BE Installed:";
#endif

// src/core/$(SYSTEM)/memory.cpp
#if defined(ENABLE_MEMORY)
	inline constexpr wchar_t ERROR_MEMORY[]        = L"\U0000274C Failed to retrieve memory information";
	inline constexpr wchar_t MEMORY[]              = L"\U0001F418 Memory (RAM):";
	inline constexpr wchar_t MEMORY_AVAILABLE[]    = L"Available:";
	inline constexpr wchar_t MEMORY_CAPACITY[]     = L"Capacity:";
	inline constexpr wchar_t MEMORY_MANUFACTURER[] = L"Manufacturer:";
	inline constexpr wchar_t MEMORY_SPEED[]        = L"Speed:";
	inline constexpr wchar_t MEMORY_TOTAL[]        = L"Total:";
	inline constexpr wchar_t MEMORY_TYPE[]         = L"Type:";
	inline constexpr wchar_t MEMORY_USED[]         = L"Used:";
#endif

// src/core/$(SYSTEM)/network.cpp
#if defined(ENABLE_NETWORK)
	inline constexpr wchar_t DESCRIPTION[]   = L"Description:";
	inline constexpr wchar_t DNS_SERVERS[]   = L"DNS servers:";
	inline constexpr wchar_t GATEWAY[]       = L"Gateway:";
	inline constexpr wchar_t HOST_NAME[]     = L"Hostname:";
	inline constexpr wchar_t NETWORK[]       = L"\U0001F310 Network:";
	inline constexpr wchar_t TCP_ENDPOINTS[] = L"TCP endpoints:";
	inline constexpr wchar_t UDP_ENDPOINTS[] = L"UDP endpoints:";
#endif

// src/core/$(SYSTEM)/operating_system.cpp
#if defined(ENABLE_OPERATING_SYSTEM)
	inline constexpr wchar_t COUNTRY_REGION[]   = L"Country/Region:";
	inline constexpr wchar_t DAY[]              = L"day";
	inline constexpr wchar_t DAYS[]             = L"days";
	inline constexpr wchar_t HOUR_UPTIME[]      = L"hour";
	inline constexpr wchar_t HOURS_UPTIME[]     = L"hours";
	inline constexpr wchar_t LANGUAGE[]         = L"Language:";
	inline constexpr wchar_t MINUTE[]           = L"minute";
	inline constexpr wchar_t MINUTES[]          = L"minutes";
	inline constexpr wchar_t NAME[]             = L"Name:";
	inline constexpr wchar_t OPERATING_SYSTEM[] = L"\U0001F5A5 Operating system:";
	inline constexpr wchar_t SECOND[]           = L"second";
	inline constexpr wchar_t SECONDS[]          = L"seconds";
	inline constexpr wchar_t UPTIME[]           = L"Uptime:";
#endif

// src/core/$(SYSTEM)/processes.cpp
#if defined(ENABLE_PROCESSES)
	inline constexpr wchar_t RUNNING_PROCESSES[] = L"\U0001F3C3 Processes:";
	inline constexpr wchar_t TOTAL_PROCESSES[]   = L"Total:";
#endif

// src/core/$(SYSTEM)/restore.cpp
#if defined(ENABLE_RESTORE)
	inline constexpr wchar_t RESTORE_POINT[]     = L"\U0001F504 Restore point:";
	inline constexpr wchar_t RESTORE_POINTS[]    = L"\U0001F504 Restore points:";
	inline constexpr wchar_t NO_RESTORE_POINTS[] = L"\U0000274C No restore points found\n"
	                                               L"\t(Requires admin privileges)";
#endif

// src/core/$(SYSTEM)/services.cpp
#if defined(ENABLE_SERVICES)
	inline constexpr wchar_t SERVICES[] = L"\U0001F3E2 Services:";
#endif

// src/core/$(SYSTEM)/startup.cpp
#if defined(ENABLE_STARTUP)
	inline constexpr wchar_t STARTUP[] = L"\U0001F680 Startup programs:";
#endif

// src/core/$(SYSTEM)/tasks.cpp
#if defined(ENABLE_TASKS)
	inline constexpr wchar_t TASKS[] = L"\U00002705 Tasks:";
#endif

// src/core/windows/trash.cpp
#if defined(ENABLE_TRASH)
	inline constexpr wchar_t TRASH[]       = L"\U0001F5D1 Trash:";
	inline constexpr wchar_t TRASH_EMPTY[] = L"\U00002705 Trash is empty";
#endif

// src/core/$(SYSTEM)/usb.cpp
#if defined(ENABLE_USB)
	inline constexpr wchar_t USB[] = L"\U0001F50C USB devices:";
#endif

// src/core/$(SYSTEM)/wifi.cpp
#if defined(ENABLE_WIFI)
	inline constexpr wchar_t WIFI[] = L"\U0001F4F6 Wi-Fi:";
#endif

}
