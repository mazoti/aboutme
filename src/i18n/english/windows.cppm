module;

export module i18n_system;

// src/core/windows/battery.cpp
#if defined(ENABLE_BATTERY)
	export constexpr wchar_t AC_LINE_STATUS[]                = L"AC Line Status:";
	export constexpr wchar_t BATTERY_FLAG[]                  = L"Battery Flag:";
	export constexpr wchar_t BATTERY_LIFE_TIME[]             = L"Battery Life Time:";
	export constexpr wchar_t ERROR_BATTERY[]                 = L"\U0000274C No battery found";
	export constexpr wchar_t ERROR_POWER_STATUS[]            = L"\U0000274C Failed to get system power status";
#endif

// src/core/windows/bluetooth.cpp
#if defined(ENABLE_BLUETOOTH)
	export constexpr wchar_t ERROR_NO_BLUETOOTH_ADAPTERS[]   = L"\U0000274C No Bluetooth adapters found";
#endif

// src/core/windows/features.cpp
#if defined(ENABLE_FEATURES)
	export constexpr wchar_t ERROR_FEATURES_COM_INIT[]       = L"\U0000274C Failed to initialize COM library";
	export constexpr wchar_t ERROR_FEATURES_QUERY[]          = L"\U0000274C Query failed";
	export constexpr wchar_t ERROR_FEATURES_SECURITY_LEVEL[] = L"\U0000274C Could not set security level";
	export constexpr wchar_t ERROR_FEATURES_WMI_CONNECT[]    = L"\U0000274C Could not connect to WMI";
	export constexpr wchar_t ERROR_FEATURES_WMI_INIT[]       = L"\U0000274C Failed to create IWbemLocator object";
#endif

// src/core/windows/gpu.cpp
#if defined(ENABLE_GPU)
	export constexpr wchar_t ERROR_COM_INIT[]                = L"\U0000274C Failed to initialize COM library";
	export constexpr wchar_t ERROR_QUERY[]                   = L"\U0000274C Query failed";
	export constexpr wchar_t ERROR_SECURITY_LEVEL[]          = L"\U0000274C Could not set security level";
	export constexpr wchar_t ERROR_WMI_CONNECT[]             = L"\U0000274C Could not connect to WMI";
	export constexpr wchar_t ERROR_WMI_INIT[]                = L"\U0000274C Failed to create IWbemLocator object";
#endif

// src/core/windows/hard_disks.cpp
#if defined(ENABLE_HARD_DISKS)
	export constexpr wchar_t ERROR_HARD_DISK[]               = L"\U0000274C GetLogicalDrives failed with error: ";
	export constexpr wchar_t ERROR_HARD_DISK_SPACE[]         = L"\U0000274C Error getting disk space "
	                                                            "information of the drive ";
#endif

// src/core/windows/installed.cpp
#if defined(ENABLE_INSTALLED)
	export constexpr wchar_t ERROR_INSTALLED_REG_OPENKEYEX[] = L"\U0000274C Error on RegOpenKeyEx";
#endif

// src/core/windows/memory.cpp
#if defined(ENABLE_MEMORY)
	export constexpr wchar_t ERROR_MEMORY_COM_INIT[]         = L"\U0000274C Failed to initialize COM library";
	export constexpr wchar_t ERROR_MEMORY_QUERY[]            = L"\U0000274C Query failed";
	export constexpr wchar_t ERROR_MEMORY_SECURITY_LEVEL[]   = L"\U0000274C Could not set security level";
	export constexpr wchar_t ERROR_MEMORY_WMI_CONNECT[]      = L"\U0000274C Could not connect to WMI";
	export constexpr wchar_t ERROR_MEMORY_WMI_INIT[]         = L"\U0000274C Failed to create IWbemLocator object";
#endif

// src/core/windows/network.cpp
#if defined(ENABLE_NETWORK)
	export constexpr wchar_t ERROR_HOST_NAME[]               = L"\U0000274C Failed get host name";
	export constexpr wchar_t ERROR_MEMORY_ALLOCATION[]       = L"\U0000274C Memory allocation failed";
	export constexpr wchar_t ERROR_ADAPTERS_ADDRESS[]        = L"\U0000274C GetAdaptersAddresses failed";
	export constexpr wchar_t ERROR_ADAPTERS_INFO[]           = L"\U0000274C GetAdaptersInfo failed";
	export constexpr wchar_t ERROR_WSA_STARTUP[]             = L"\U0000274C WSAStartup failed";
	export constexpr wchar_t ERROR_TCP_MALLOC[]              = L"\U0000274C Memory allocation failed for TCP table";
	export constexpr wchar_t ERROR_UDP_MALLOC[]              = L"\U0000274C Memory allocation failed for UDP table";
	export constexpr wchar_t ERROR_EXTENDED_TCP_TABLE[]      = L"\U0000274C GetExtendedTcpTable failed";
	export constexpr wchar_t ERROR_EXTENDED_UDP_TABLE[]      = L"\U0000274C GetExtendedUdpTable failed";
#endif

// src/core/windows/operating_system.cpp
#if defined(ENABLE_OPERATING_SYSTEM)
	export constexpr wchar_t ERROR_LOCALE_NAME[]             = L"\U0000274C Failed to get locale name";
#endif

// src/core/windows/processes.cpp
#if defined(ENABLE_PROCESSES)
	export constexpr wchar_t ERROR_PROCESSES_ENUM[]          = L"\U0000274C Failed to enumerate processes";
#endif

// src/core/windows/restore.cpp
#if defined(ENABLE_RESTORE)
	export constexpr wchar_t ERROR_RESTORE_COM_INIT[]        = L"\U0000274C Failed to initialize COM library";
	export constexpr wchar_t ERROR_RESTORE_QUERY[]           = L"\U0000274C Query failed";
	export constexpr wchar_t ERROR_RESTORE_SECURITY_LEVEL[]  = L"\U0000274C Could not set security level";
	export constexpr wchar_t ERROR_RESTORE_WMI_CONNECT[]     = L"\U0000274C Could not connect to WMI";
	export constexpr wchar_t ERROR_RESTORE_WMI_INIT[]        = L"\U0000274C Failed to create IWbemLocator object";
#endif

// src/core/windows/services.cpp
#if defined(ENABLE_SERVICES)
	export constexpr wchar_t ERROR_SERVICES[]                = L"\U0000274C Failed to open Service "
	                                                            "Control Manager (SCM)";
	export constexpr wchar_t ERROR_SERVICES_ENUM[]           = L"\U0000274C Failed to enumerate services";
	export constexpr wchar_t ERROR_SERVICES_MALLOC[]         = L"\U0000274C Failed to allocate memory";
#endif

// src/core/windows/startup.cpp
#if defined(ENABLE_STARTUP)
	export constexpr wchar_t ERROR_REG_OPENKEYEX[]           = L"\U0000274C Error on RegOpenKeyEx";
#endif

// src/core/windows/tasks.cpp
#if defined(ENABLE_TASKS)
	export constexpr wchar_t ERROR_TASKS[]                   = L"\U0000274C Error on";
#endif

// src/core/windows/usb.cpp
#if defined(ENABLE_USB)
	export constexpr wchar_t ERROR_USB_DEVICE_INIT[]         = L"\U0000274C Error initializing USB "
	                                                            "device information set";
	export constexpr wchar_t ERROR_USB_DEVICE_INST[]         = L"\U0000274C Error: SetupDiGetDeviceInstanceId "
	                                                            "returned false";
	export constexpr wchar_t ERROR_USB_DEVICE_REG[]          = L"\U0000274C Error: SetupDiGetDeviceRegistryProperty "
	                                                            "returned false";
#endif

// src/core/windows/wifi.cpp
#if defined(ENABLE_WIFI)
	export constexpr wchar_t ERROR_WIFI[]                    = L"\U0000274C No Wi-Fi found";
	export constexpr wchar_t ERROR_WLAN_ENUM_INTERFACES[]    = L"\U0000274C WlanEnumInterfaces failed";
	export constexpr wchar_t ERROR_WLAN_OPEN_HANDLE[]        = L"\U0000274C WlanOpenHandle failed";
#endif
