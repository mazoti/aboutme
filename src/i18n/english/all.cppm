module;

export module i18n;

// src/main_$(SYSTEM).cpp
export constexpr wchar_t ANY_KEY_TO_CLOSE[] = L"Press any key to close...";

// src/common/environment_variables.cpp
#if defined(ENABLE_ENVIRONMENT_VARIABLES)
	export constexpr wchar_t ENVIRONMENT_VARIABLES[]         = L"\U0001F333 Environment variables:";
	export constexpr wchar_t ENVIRONMENT_VARIABLES_WARNING[] = L"\U000026A0 Warning: check if this path is correct";
#endif

// src/common/system_clock.cpp
#if defined(ENABLE_SYSTEM_CLOCK)
	export constexpr wchar_t DATE[]                          = L"Date (dd/mm/yy):";
	export constexpr wchar_t DAYLIGHT_SAVING_TIME[]          = L"Daylight saving time";
	export constexpr wchar_t GMT[]                           = L"Timezone:";
	export constexpr wchar_t HOURS[]                         = L"hours";
	export constexpr wchar_t SYSTEM_CLOCK[]                  = L"\U0001F556 System clock:";
	export constexpr wchar_t TIME[]                          = L"Time (hh:mm:ss):";
#endif

// src/core/$(SYSTEM)/battery.cpp
#if defined(ENABLE_BATTERY)
	export constexpr wchar_t BATTERY_AC_NOT_CONNECTED[]      = L"running on battery";
	export constexpr wchar_t BATTERY_AC_POWER[]              = L"running on AC power";
	export constexpr wchar_t BATTERY_DAYS[]                  = L"days";
	export constexpr wchar_t BATTERY_HOURS[]                 = L"hours";
	export constexpr wchar_t BATTERY_MINUTES[]               = L"minutes";
	export constexpr wchar_t BATTERY_STATUS[]                = L"\U0001F50B Battery:";
#endif

// src/core/$(SYSTEM)/bluetooth.cpp
#if defined(ENABLE_BLUETOOTH)
	export constexpr wchar_t BLUETOOTH[]                     = L"\U0001F91D Bluetooth:";
#endif

// src/core/$(SYSTEM)/cpu.cpp
#if defined(ENABLE_CPU)
	export constexpr wchar_t ARCHITECTURE[]                  = L"Architecture:";
	export constexpr wchar_t INSTRUCTIONS_SUPPORTED[]        = L"Instructions supported:";
	export constexpr wchar_t THREADS[]                       = L"Threads:";
	export constexpr wchar_t CPU[]                           = L"\U0001F9EE CPU:";
#endif

// src/core/$(SYSTEM)/features.cpp
#if defined(ENABLE_FEATURES)
	export constexpr wchar_t FEATURES[]                      = L"\U0001F527 Features:";
#endif

// src/core/$(SYSTEM)/gpu.cpp
#if defined(ENABLE_GPU)
	export constexpr wchar_t GPU[]                           = L"\U0001F31F GPU:";
	export constexpr wchar_t GPU_MEMORY[]                    = L"Memory:";
	export constexpr wchar_t VIDEO_RESOLUTION[]              = L"Resolution:";
#endif

// src/core/$(SYSTEM)/hard_disks.cpp
#if defined(ENABLE_HARD_DISKS)
	export constexpr wchar_t ALL[]                           = L"All:";
	export constexpr wchar_t AVAILABLE[]                     = L"Free:";
	export constexpr wchar_t HARD_DISKS[]                    = L"\U0001F4BF Hard disks:";
	export constexpr wchar_t TOTAL[]                         = L"Total:";
	export constexpr wchar_t USED[]                          = L"Used:";
#endif

// src/core/$(SYSTEM)/installed.cpp
#if defined(ENABLE_INSTALLED)
	export constexpr wchar_t INSTALLED_PROGRAMS[]            = L"\U0001F4BE Installed programs:";
#endif

// src/core/$(SYSTEM)/memory.cpp
#if defined(ENABLE_MEMORY)
	export constexpr wchar_t ERROR_MEMORY[]                  = L"\U0000274C Error retrieving memory information";
	export constexpr wchar_t MEMORY[]                        = L"\U0001F418 Memory (RAM):";
	export constexpr wchar_t MEMORY_AVAILABLE[]              = L"Free:";
	export constexpr wchar_t MEMORY_CAPACITY[]               = L"Capacity:";
	export constexpr wchar_t MEMORY_MANUFACTURER[]           = L"Manufacturer:";
	export constexpr wchar_t MEMORY_SPEED[]                  = L"Speed:";
	export constexpr wchar_t MEMORY_TOTAL[]                  = L"Total:";
	export constexpr wchar_t MEMORY_TYPE[]                   = L"Type:";
	export constexpr wchar_t MEMORY_USED[]                   = L"Used:";
#endif

// src/core/$(SYSTEM)/operating_system.cpp
#if defined(ENABLE_OPERATING_SYSTEM)
	export constexpr wchar_t COUNTRY_REGION[]                = L"Country/Region:";
	export constexpr wchar_t DAYS[]                          = L"days";
	export constexpr wchar_t HOURS_UPTIME[]                  = L"hours";
	export constexpr wchar_t LANGUAGE[]                      = L"Language:";
	export constexpr wchar_t MINUTES[]                       = L"minutes";
	export constexpr wchar_t NAME[]                          = L"Name:";
	export constexpr wchar_t OPERATING_SYSTEM[]              = L"\U0001F5A5 Operating system:";
	export constexpr wchar_t SECONDS[]                       = L"seconds";
	export constexpr wchar_t UPTIME[]                        = L"Uptime:";
#endif

// src/core/$(SYSTEM)/processes.cpp
#if defined(ENABLE_PROCESSES)
	export constexpr wchar_t RUNNING_PROCESSES[]             = L"\U0001F3C3 Processes:";
	export constexpr wchar_t TOTAL_PROCESSES[]               = L"Total:";
#endif

// src/core/$(SYSTEM)/restore.cpp
#if defined(ENABLE_RESTORE)
	export constexpr wchar_t RESTORE_POINTS[]                = L"\U0001F504 Restore points:";
	export constexpr wchar_t NO_RESTORE_POINTS[]             = L"\U0000274C No restore points found\n"
	                                                            "\t(Admin privileges needed)";
#endif

// src/core/$(SYSTEM)/services.cpp
#if defined(ENABLE_SERVICES)
	export constexpr wchar_t SERVICES[]                      = L"\U0001F3E2 Services:";
#endif

// src/core/$(SYSTEM)/startup.cpp
#if defined(ENABLE_STARTUP)
	export constexpr wchar_t STARTUP[]                       = L"\U0001F680 Startup programs:";
#endif

// src/core/$(SYSTEM)/tasks.cpp
#if defined(ENABLE_TASKS)
	export constexpr wchar_t TASKS[]                         = L"\U00002705 Tasks:";
#endif

// src/core/$(SYSTEM)/usb.cpp
#if defined(ENABLE_USB)
	export constexpr wchar_t USB[]                           = L"\U0001F50C USB devices:";
#endif

// src/core/$(SYSTEM)/wifi.cpp
#if defined(ENABLE_WIFI)
	export constexpr wchar_t WIFI[]                          = L"\U0001F4F6 Wi-Fi:";
#endif
