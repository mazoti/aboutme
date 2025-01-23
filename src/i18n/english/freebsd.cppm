module;

export module i18n_system;

// src/core/freebsd/cpu.cpp
#if defined(ENABLE_CPU)
	export constexpr wchar_t ERROR_SYSCTLBYNAME[]         = L"\U0000274C Error: sysctlbyname failed";
#endif

// src/core/freebsd/hard_disks.cpp
#if defined(ENABLE_HARD_DISKS)
	export constexpr wchar_t ERROR_MEDIA_SIZE[]           = L"\U0000274C Failed to get media size";
	export constexpr wchar_t ERROR_OPEN_DEVICE[]          = L"\U0000274C Failed to open device:";
	export constexpr wchar_t ERROR_SPACE_INFO[]           = L"\U0000274C Error: std::filesystem::space() failed";
#endif

// src/core/freebsd/memory.cpp
#if defined(ENABLE_MEMORY)
	export constexpr wchar_t ERROR_SYSCTLBYNAME[]         = L"\U0000274C Error: sysctlbyname failed";
#endif

// src/core/freebsd/operating_system.cpp
#if defined(ENABLE_OPERATING_SYSTEM)
	export constexpr wchar_t ERROR_KERNEL_VERSION[]       = L"\U0000274C Failed to get kernel version";
	export constexpr wchar_t ERROR_SYSCTL_BOOT_TIME[]     = L"\U0000274C sysctl failed to get boot time";
#endif

// src/core/freebsd/services.cpp
#if defined(ENABLE_SERVICES)
	export constexpr wchar_t ERROR_SERVICES_PIPE[]        = L"\U0000274C Error: popen() failed";
#endif

// src/core/freebsd/usb.cpp
#if defined(ENABLE_USB)
	export constexpr wchar_t ERROR_PIPE[]                 = L"\U0000274C Error: popen() failed";
#endif
