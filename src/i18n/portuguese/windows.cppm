module;

export module i18n_system;

// src/core/windows/battery.cpp
#if defined(ENABLE_BATTERY)
	export constexpr wchar_t AC_LINE_STATUS[]                = L"Status da linha AC:";
	export constexpr wchar_t BATTERY_FLAG[]                  = L"Indicador de bateria:";
	export constexpr wchar_t BATTERY_LIFE_TIME[]             = L"Vida \U000000FAtil:";
	export constexpr wchar_t ERROR_BATTERY[]                 = L"\U0000274C Nenhuma bateria encontrada";
	export constexpr wchar_t ERROR_POWER_STATUS[]            = L"\U0000274C Erro lendo informa\U000000E7\U000000F5es "
	                                                           "da bateria do sistema";
#endif

// src/core/windows/bluetooth.cpp
#if defined(ENABLE_BLUETOOTH)
	export constexpr wchar_t ERROR_NO_BLUETOOTH_ADAPTERS[]   = L"\U0000274C Nenhum adaptador Bluetooth encontrado";
#endif

// src/core/windows/features.cpp
#if defined(ENABLE_FEATURES)
	export constexpr wchar_t ERROR_FEATURES_COM_INIT[]       = L"\U0000274C Falha inicializando biblioteca COM";
	export constexpr wchar_t ERROR_FEATURES_QUERY[]          = L"\U0000274C Falha na query";
	export constexpr wchar_t ERROR_FEATURES_SECURITY_LEVEL[] = L"\U0000274C N\U000000E3o foi poss\U000000EDvel "
	                                                           "ativar n\U000000EDvel de seguran\U000000E7a";
	export constexpr wchar_t ERROR_FEATURES_WMI_CONNECT[]    = L"\U0000274C N\U000000E3o foi poss\U000000EDvel "
	                                                           "conectar ao WMI";
	export constexpr wchar_t ERROR_FEATURES_WMI_INIT[]       = L"\U0000274C Falha criando objeto IWbemLocator";
#endif

// src/core/windows/gpu.cpp
#if defined(ENABLE_GPU)
	export constexpr wchar_t ERROR_COM_INIT[]                = L"\U0000274C Falha inicializando biblioteca COM";
	export constexpr wchar_t ERROR_QUERY[]                   = L"\U0000274C Falha na query";
	export constexpr wchar_t ERROR_SECURITY_LEVEL[]          = L"\U0000274C N\U000000E3o foi poss\U000000EDvel "
	                                                           "ativar n\U000000EDvel de seguran\U000000E7a";
	export constexpr wchar_t ERROR_WMI_CONNECT[]             = L"\U0000274C N\U000000E3o foi poss\U000000EDvel "
	                                                           "conectar ao WMI";
	export constexpr wchar_t ERROR_WMI_INIT[]                = L"\U0000274C Falha criando objeto IWbemLocator";
#endif

// src/core/windows/hard_disks.cpp
#if defined(ENABLE_HARD_DISKS)
	export constexpr wchar_t ERROR_HARD_DISK[]               = L"\U0000274C GetLogicalDrives retornou o "
	                                                           "c\U000000F3digo de erro: ";
	export constexpr wchar_t ERROR_HARD_DISK_SPACE[]         = L"\U0000274C Erro lendo informa\U000000E7\U000000F5es "
	                                                           "de espa\U000000E7o de disco do drive ";
#endif

// src/core/windows/installed.cpp
#if defined(ENABLE_INSTALLED)
	export constexpr wchar_t ERROR_INSTALLED_REG_OPENKEYEX[] = L"\U0000274C Erro na fun\U000000E7\U000000E3o "
	                                                           "RegOpenKeyEx";
#endif

// src/core/windows/memory.cpp
#if defined(ENABLE_MEMORY)
	export constexpr wchar_t ERROR_MEMORY_COM_INIT[]         = L"\U0000274C Falha inicializando biblioteca COM";
	export constexpr wchar_t ERROR_MEMORY_QUERY[]            = L"\U0000274C Falha na query";
	export constexpr wchar_t ERROR_MEMORY_SECURITY_LEVEL[]   = L"\U0000274C N\U000000E3o foi poss\U000000EDvel "
	                                                           "ativar n\U000000EDvel de seguran\U000000E7a";
	export constexpr wchar_t ERROR_MEMORY_WMI_CONNECT[]      = L"\U0000274C N\U000000E3o foi poss\U000000EDvel "
	                                                           "conectar ao WMI";
	export constexpr wchar_t ERROR_MEMORY_WMI_INIT[]         = L"\U0000274C Falha criando objeto IWbemLocator";
#endif

// src/core/windows/operating_system.cpp
#if defined(ENABLE_OPERATING_SYSTEM)
	export constexpr wchar_t ERROR_LOCALE_NAME[]             = L"\U0000274C Falha ao obter nome do local";
#endif

// src/core/windows/processes.cpp
#if defined(ENABLE_PROCESSES)
	export constexpr wchar_t ERROR_PROCESSES_ENUM[]          = L"\U0000274C Erro enumerando processos";
#endif

// src/core/windows/restore.cpp
#if defined(ENABLE_RESTORE)
	export constexpr wchar_t ERROR_RESTORE_COM_INIT[]         = L"\U0000274C Falha inicializando biblioteca COM";
	export constexpr wchar_t ERROR_RESTORE_QUERY[]            = L"\U0000274C Falha na query";
	export constexpr wchar_t ERROR_RESTORE_SECURITY_LEVEL[]   = L"\U0000274C N\U000000E3o foi poss\U000000EDvel "
	                                                           "ativar n\U000000EDvel de seguran\U000000E7a";
	export constexpr wchar_t ERROR_RESTORE_WMI_CONNECT[]      = L"\U0000274C N\U000000E3o foi poss\U000000EDvel "
	                                                           "conectar ao WMI";
	export constexpr wchar_t ERROR_RESTORE_WMI_INIT[]         = L"\U0000274C Falha criando objeto IWbemLocator";
#endif

// src/core/windows/services.cpp
#if defined(ENABLE_SERVICES)
	export constexpr wchar_t ERROR_SERVICES[]                = L"\U0000274C N\U000000E3o foi poss\U000000EDvel abrir "
	                                                           "o Service Control Manager (SCM)";
	export constexpr wchar_t ERROR_SERVICES_ENUM[]           = L"\U0000274C Erro enumerando servi\U000000E7os";
	export constexpr wchar_t ERROR_SERVICES_MALLOC[]         = L"\U0000274C N\U000000E3o foi poss\U000000EDvel "
	                                                           "alocar mem\U000000F3ria";
#endif

// src/core/windows/startup.cpp
#if defined(ENABLE_STARTUP)
	export constexpr wchar_t ERROR_REG_OPENKEYEX[]           = L"\U0000274C Erro na fun\U000000E7\U000000E3o "
	                                                           "RegOpenKeyEx";
#endif

// src/core/windows/tasks.cpp
#if defined(ENABLE_TASKS)
	export constexpr wchar_t ERROR_TASKS[]                   = L"\U0000274C Erro em";
#endif

// src/core/windows/usb.cpp
#if defined(ENABLE_USB)
	export constexpr wchar_t ERROR_USB_DEVICE_INIT[]         = L"\U0000274C Erro inicializando conjunto de informa"
	                                                           "\U000000E7\U000000F5es do dispositivo USB";
	export constexpr wchar_t ERROR_USB_DEVICE_INST[]         = L"\U0000274C Erro: SetupDiGetDeviceInstanceId "
	                                                           "retornou falso";
	export constexpr wchar_t ERROR_USB_DEVICE_REG[]          = L"\U0000274C Erro: SetupDiGetDeviceRegistryProperty "
	                                                           "retornou falso";
#endif

// src/core/windows/wifi.cpp
#if defined(ENABLE_WIFI)
	export constexpr wchar_t ERROR_WIFI[]                    = L"\U0000274C Nenhuma conex\U000000E3o Wi-Fi encontrada";
	export constexpr wchar_t ERROR_WLAN_ENUM_INTERFACES[]    = L"\U0000274C WlanEnumInterfaces retornou erro";
	export constexpr wchar_t ERROR_WLAN_OPEN_HANDLE[]        = L"\U0000274C WlanOpenHandle retornou erro";
#endif
