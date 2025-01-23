module;

export module i18n_system;

//src/core/linux/cpu.cpp
#if defined(ENABLE_CPU)
	export constexpr wchar_t ERROR_CPU_INFO[]                = L"\U0000274C N\U000000E3o foi poss\U000000EDvel "
	                                                            "ler /proc/cpuinfo";
#endif

//src/core/linux/hard_disk.cpp
#if defined(ENABLE_HARD_DISKS)
	export constexpr wchar_t ERROR_LINUX_HARD_DISK[]         = L"\U0000274C Erro na fun\U000000E7\U000000E3o setmntent";
	export constexpr wchar_t ERROR_LINUX_HARD_DISK_STATVFS[] = L"\U0000274C Erro na fun\U000000E7\U000000E3o statvfs";
#endif

//src/core/linux/operating_system.cpp
#if defined(ENABLE_OPERATING_SYSTEM)
	export constexpr wchar_t ERROR_KERNEL_VERSION[]          = L"\U0000274C N\U000000E3o foi poss\U000000EDvel ler a "
	                                                            "vers\U000000E3o do kernel";
#endif

// src/core/linux/services.cpp
#if defined(ENABLE_SERVICES)
	export constexpr wchar_t ERROR_LINUX_PIPE[]              = L"\U0000274C Erro: falha em popen()";
#endif
