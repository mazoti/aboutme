module;

export module i18n_system;

// src/core/freebsd/cpu.cpp
#if defined(ENABLE_CPU)
	export constexpr wchar_t ERROR_SYSCTLBYNAME[]         = L"\U0000274C Erro: falha em sysctlbyname";
#endif

// src/core/freebsd/hard_disks.cpp
#if defined(ENABLE_HARD_DISKS)
	export constexpr wchar_t ERROR_FREEBSD_MEDIA_SIZE[]   = L"\U0000274C Falha ao ler tamanho do disco";
	export constexpr wchar_t ERROR_FREEBSD_OPEN_DEVICE[]  = L"\U0000274C Falha ao ler dispositivo:";
	export constexpr wchar_t ERROR_FREEBSD_SPACE_INFO[]   = L"\U0000274C Erro: falha em std::filesystem::space()";
#endif

// src/core/freebsd/memory.cpp
#if defined(ENABLE_MEMORY)
	export constexpr wchar_t ERROR_FREEBSD_SYSCTLBYNAME[] = L"\U0000274C Erro: falha em sysctlbyname";
#endif

// src/core/freebsd/operating_system.cpp
#if defined(ENABLE_OPERATING_SYSTEM)
	export constexpr wchar_t ERROR_KERNEL_VERSION[]       = L"\U0000274C N\U000000E3o foi poss\U000000EDvel ler a "
	                                                         "vers\U000000E3o do kernel";

	export constexpr wchar_t ERROR_SYSCTL_BOOT_TIME[]     = L"\U0000274C sysctl falhou em ler o hor\U000000E1rio "
	                                                         "de inicializa\U000000E7\U000000E3o";
#endif

// src/core/freebsd/services.cpp
#if defined(ENABLE_SERVICES)
	export constexpr wchar_t ERROR_SERVICES_PIPE[]        = L"\U0000274C Erro: falha em popen()";
#endif

// src/core/freebsd/usb.cpp
#if defined(ENABLE_USB)
	export constexpr wchar_t ERROR_FREEBSD_PIPE[]         = L"\U0000274C Erro: falha em popen()";
#endif
