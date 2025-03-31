module;

export module i18n_system;

export namespace i18n_system{

// src/core/windows/battery.cpp
#if defined(ENABLE_BATTERY)
	inline constexpr wchar_t AC_LINE_STATUS[]                = L"Status da linha AC:";
	inline constexpr wchar_t BATTERY_FLAG[]                  = L"Indicador de bateria:";
	inline constexpr wchar_t BATTERY_LIFE_TIME[]             = L"Vida \U000000FAtil:";
	inline constexpr wchar_t ERROR_BATTERY[]                 = L"\U0000274C Nenhuma bateria encontrada";
	inline constexpr wchar_t ERROR_POWER_STATUS[]            = L"\U0000274C Erro lendo informa\U000000E7\U000000F5es "
	                                                           "da bateria do sistema";
#endif

// src/core/windows/bluetooth.cpp
#if defined(ENABLE_BLUETOOTH)
	inline constexpr wchar_t ERROR_NO_BLUETOOTH_ADAPTERS[]   = L"\U0000274C Nenhum adaptador Bluetooth encontrado";
#endif

// src/core/windows/features.cpp
#if defined(ENABLE_FEATURES)
	inline constexpr wchar_t ERROR_FEATURES_COM_INIT[]       = L"\U0000274C Falha inicializando biblioteca COM";
	inline constexpr wchar_t ERROR_FEATURES_QUERY[]          = L"\U0000274C Falha na query";
	inline constexpr wchar_t ERROR_FEATURES_SECURITY_LEVEL[] = L"\U0000274C N\U000000E3o foi poss\U000000EDvel "
	                                                           "ativar n\U000000EDvel de seguran\U000000E7a";
	inline constexpr wchar_t ERROR_FEATURES_WMI_CONNECT[]    = L"\U0000274C N\U000000E3o foi poss\U000000EDvel "
	                                                           "conectar ao WMI";
	inline constexpr wchar_t ERROR_FEATURES_WMI_INIT[]       = L"\U0000274C Falha criando objeto IWbemLocator";
#endif

// src/core/windows/gpu.cpp
#if defined(ENABLE_GPU)
	inline constexpr wchar_t ERROR_COM_INIT[]                = L"\U0000274C Falha inicializando biblioteca COM";
	inline constexpr wchar_t ERROR_QUERY[]                   = L"\U0000274C Falha na query";
	inline constexpr wchar_t ERROR_SECURITY_LEVEL[]          = L"\U0000274C N\U000000E3o foi poss\U000000EDvel "
	                                                           "ativar n\U000000EDvel de seguran\U000000E7a";
	inline constexpr wchar_t ERROR_WMI_CONNECT[]             = L"\U0000274C N\U000000E3o foi poss\U000000EDvel "
	                                                           "conectar ao WMI";
	inline constexpr wchar_t ERROR_WMI_INIT[]                = L"\U0000274C Falha criando objeto IWbemLocator";
#endif

// src/core/windows/hard_disks.cpp
#if defined(ENABLE_HARD_DISKS)
	inline constexpr wchar_t ERROR_HARD_DISK[]               = L"\U0000274C GetLogicalDrives retornou o "
	                                                           "c\U000000F3digo de erro: ";
	inline constexpr wchar_t ERROR_HARD_DISK_SPACE[]         = L"\U0000274C Erro lendo informa\U000000E7\U000000F5es "
	                                                           "de espa\U000000E7o de disco do drive ";
#endif

// src/core/windows/installed.cpp
#if defined(ENABLE_INSTALLED)
	inline constexpr wchar_t ERROR_INSTALLED_REG_OPENKEYEX[] = L"\U0000274C Erro na fun\U000000E7\U000000E3o "
	                                                           "RegOpenKeyEx";
#endif

// src/core/windows/memory.cpp
#if defined(ENABLE_MEMORY)
	inline constexpr wchar_t ERROR_MEMORY_COM_INIT[]         = L"\U0000274C Falha inicializando biblioteca COM";
	inline constexpr wchar_t ERROR_MEMORY_QUERY[]            = L"\U0000274C Falha na query";
	inline constexpr wchar_t ERROR_MEMORY_SECURITY_LEVEL[]   = L"\U0000274C N\U000000E3o foi poss\U000000EDvel "
	                                                           "ativar n\U000000EDvel de seguran\U000000E7a";
	inline constexpr wchar_t ERROR_MEMORY_WMI_CONNECT[]      = L"\U0000274C N\U000000E3o foi poss\U000000EDvel "
	                                                           "conectar ao WMI";
	inline constexpr wchar_t ERROR_MEMORY_WMI_INIT[]         = L"\U0000274C Falha criando objeto IWbemLocator";
#endif

// src/core/windows/network.cpp
#if defined(ENABLE_NETWORK)
	inline constexpr wchar_t ERROR_HOST_NAME[]               = L"\U0000274C Falha lendo nome do host";
	inline constexpr wchar_t ERROR_MEMORY_ALLOCATION[]       = L"\U0000274C Falha alocando mem\U000000F3ria";
	inline constexpr wchar_t ERROR_ADAPTERS_ADDRESS[]        = L"\U0000274C Falha em GetAdaptersAddresses";
	inline constexpr wchar_t ERROR_ADAPTERS_INFO[]           = L"\U0000274C Falha em GetAdaptersInfo";
	inline constexpr wchar_t ERROR_WSA_STARTUP[]             = L"\U0000274C Falha em WSAStartup";
	inline constexpr wchar_t ERROR_TCP_MALLOC[]              = L"\U0000274C Falha alocando mem\U000000F3ria "
	                                                            "para a tabela TCP";
	inline constexpr wchar_t ERROR_UDP_MALLOC[]              = L"\U0000274C Falha alocando mem\U000000F3ria "
	                                                            "para a tabela UDP";
	inline constexpr wchar_t ERROR_EXTENDED_TCP_TABLE[]      = L"\U0000274C Falha em GetExtendedTcpTable";
	inline constexpr wchar_t ERROR_EXTENDED_UDP_TABLE[]      = L"\U0000274C Falha em GetExtendedUdpTable";
#endif

// src/core/windows/operating_system.cpp
#if defined(ENABLE_OPERATING_SYSTEM)
	inline constexpr wchar_t ERROR_LOCALE_NAME[]             = L"\U0000274C Falha ao obter nome do local";
#endif

// src/core/windows/processes.cpp
#if defined(ENABLE_PROCESSES)
	inline constexpr wchar_t ERROR_PROCESSES_ENUM[]          = L"\U0000274C Erro enumerando processos";
#endif

// src/core/windows/restore.cpp
#if defined(ENABLE_RESTORE)
	inline constexpr wchar_t ERROR_RESTORE_COM_INIT[]         = L"\U0000274C Falha inicializando biblioteca COM";
	inline constexpr wchar_t ERROR_RESTORE_QUERY[]            = L"\U0000274C Falha na query";
	inline constexpr wchar_t ERROR_RESTORE_SECURITY_LEVEL[]   = L"\U0000274C N\U000000E3o foi poss\U000000EDvel "
	                                                           "ativar n\U000000EDvel de seguran\U000000E7a";
	inline constexpr wchar_t ERROR_RESTORE_WMI_CONNECT[]      = L"\U0000274C N\U000000E3o foi poss\U000000EDvel "
	                                                           "conectar ao WMI";
	inline constexpr wchar_t ERROR_RESTORE_WMI_INIT[]         = L"\U0000274C Falha criando objeto IWbemLocator";
#endif

// src/core/windows/scheduled_tasks.cpp
#if defined(ENABLE_SCHEDULED_TASKS)
	inline constexpr wchar_t ERROR_TASKS[]                   = L"\U0000274C Erro em";
#endif

// src/core/windows/services.cpp
#if defined(ENABLE_SERVICES)
	inline constexpr wchar_t ERROR_SERVICES[]                = L"\U0000274C N\U000000E3o foi poss\U000000EDvel abrir "
	                                                           "o Service Control Manager (SCM)";
	inline constexpr wchar_t ERROR_SERVICES_ENUM[]           = L"\U0000274C Erro enumerando servi\U000000E7os";
	inline constexpr wchar_t ERROR_SERVICES_MALLOC[]         = L"\U0000274C N\U000000E3o foi poss\U000000EDvel "
	                                                           "alocar mem\U000000F3ria";
#endif

// src/core/windows/startup.cpp
#if defined(ENABLE_STARTUP)
	inline constexpr wchar_t ERROR_REG_OPENKEYEX[]           = L"\U0000274C Erro na fun\U000000E7\U000000E3o "
	                                                           "RegOpenKeyEx";
#endif

// src/core/windows/trash.cpp
#if defined(ENABLE_TRASH)
	inline constexpr wchar_t ERROR_TRASH_BIND[]             = L"\U0000274C Falha lendo a lixeira";
	inline constexpr wchar_t ERROR_TRASH_COM_INIT[]         = L"\U0000274C Falha inicializando biblioteca COM";
	inline constexpr wchar_t ERROR_TRASH_DESKTOP_FOLDER[]   = L"\U0000274C Falha lendo pasta do desktop";
	inline constexpr wchar_t ERROR_TRASH_ENUM[]             = L"\U0000274C Falha enumerando objetos";
	inline constexpr wchar_t ERROR_TRASH_LOCATION[]         = L"\U0000274C Falha lendo local da lixeira";
#endif

// src/core/windows/usb.cpp
#if defined(ENABLE_USB)
	inline constexpr wchar_t ERROR_USB_DEVICE_INIT[]         = L"\U0000274C Erro inicializando conjunto de informa"
	                                                           "\U000000E7\U000000F5es do dispositivo USB";
	inline constexpr wchar_t ERROR_USB_DEVICE_INST[]         = L"\U0000274C Erro: SetupDiGetDeviceInstanceId "
	                                                           "retornou falso";
	inline constexpr wchar_t ERROR_USB_DEVICE_REG[]          = L"\U0000274C Erro: SetupDiGetDeviceRegistryProperty "
	                                                           "retornou falso";
#endif

// src/core/windows/wifi.cpp
#if defined(ENABLE_WIFI)
	inline constexpr wchar_t ERROR_WIFI[]                    = L"\U0000274C Nenhuma conex\U000000E3o Wi-Fi encontrada";
	inline constexpr wchar_t ERROR_WLAN_ENUM_INTERFACES[]    = L"\U0000274C WlanEnumInterfaces retornou erro";
	inline constexpr wchar_t ERROR_WLAN_OPEN_HANDLE[]        = L"\U0000274C WlanOpenHandle retornou erro";
#endif
}
