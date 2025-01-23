module;

export module i18n_system;

//src/core/linux/cpu.cpp
#if defined(ENABLE_CPU)
	export constexpr wchar_t ERROR_CPU_INFO[]                = L"\U0000274C Failed to open /proc/cpuinfo";
#endif

//src/core/linux/hard_disk.cpp
#if defined(ENABLE_HARD_DISKS)
	export constexpr wchar_t ERROR_HARD_DISK[]               = L"\U0000274C setmntent error";
	export constexpr wchar_t ERROR_HARD_DISK_STATVFS[]       = L"\U0000274C statvfs error";
#endif

//src/core/linux/operating_system.cpp
#if defined(ENABLE_OPERATING_SYSTEM)
	export constexpr wchar_t ERROR_KERNEL_VERSION[]          = L"\U0000274C Failed to get kernel version";
#endif

// src/core/linux/services.cpp
#if defined(ENABLE_SERVICES)
	export constexpr wchar_t ERROR_PIPE[]                    = L"\U0000274C Error: popen() failed";
#endif
