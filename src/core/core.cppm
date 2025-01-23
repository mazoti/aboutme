module;

#include <iostream>

export module core;

#if defined(ENABLE_BATTERY)
	export std::wostream& battery() noexcept;
#endif

#if defined(ENABLE_BLUETOOTH)
	export std::wostream& bluetooth() noexcept;
#endif

#if defined(ENABLE_CPU)
	export std::wostream& cpu() noexcept;
#endif

#if defined(ENABLE_GPU)
	export std::wostream& gpu() noexcept;
#endif

#if defined(ENABLE_HARD_DISKS)
	export std::wostream& hard_disks() noexcept;
#endif

#if defined(ENABLE_MEMORY)
	export std::wostream& memory() noexcept;
#endif

#if defined(ENABLE_OPERATING_SYSTEM)
	export std::wostream& operating_system() noexcept;
#endif

#if defined(ENABLE_PROCESSES)
	export std::wostream& processes() noexcept;
#endif

#if defined(ENABLE_SERVICES)
	export std::wostream& services() noexcept;
#endif

#if defined(ENABLE_STARTUP)
	export std::wostream& startup() noexcept;
#endif

#if defined(ENABLE_USB)
	export std::wostream& usb() noexcept;
#endif

#if defined(ENABLE_WIFI)
	export std::wostream& wifi() noexcept;
#endif

export void run(char* envp[], char separator) noexcept;
