module;

export module i18n;

export namespace i18n{

	// src/main_$(SYSTEM).cpp
	inline constexpr wchar_t ANY_KEY_TO_CLOSE[] = L"Pressione qualquer tecla para fechar...";

	// src/common/environment_variables.cpp
#if defined(ENABLE_ENVIRONMENT_VARIABLES)
	inline constexpr wchar_t ENVIRONMENT_VARIABLES[]         = L"\U0001F333 Vari\U000000E1veis de ambiente:";
	inline constexpr wchar_t ENVIRONMENT_VARIABLES_WARNING[] = L"\U000026A0 Aten\U000000E7\U000000E3o: verifique se "
	                                                           L"esse caminho est\U000000E1 correto";
#endif

	// src/common/system_clock.cpp
#if defined(ENABLE_SYSTEM_CLOCK)
	inline constexpr wchar_t DATE[]                 = L"Data (DD/MM/YY):";
	inline constexpr wchar_t DAYLIGHT_SAVING_TIME[] = L"Hor\U000000E1rio de ver\U000000E3o";
	inline constexpr wchar_t GMT[]                  = L"Fuso:";
	inline constexpr wchar_t HOURS[]                = L"horas";
	inline constexpr wchar_t SYSTEM_CLOCK[]         = L"\U0001F556 Rel\U000000F3gio do sistema:";
	inline constexpr wchar_t TIME[]                 = L"Hora (HH:MM:SS):";
#endif

	// src/core/$(SYSTEM)/battery.cpp
#if defined(ENABLE_BATTERY)
	inline constexpr wchar_t BATTERY_AC_NOT_CONNECTED[] = L"Funcionando na bateria";
	inline constexpr wchar_t BATTERY_AC_POWER[]         = L"Conectado \u00E0 energia el\u00E9trica";
	inline constexpr wchar_t BATTERY_DAYS[]             = L"dias";
	inline constexpr wchar_t BATTERY_HOURS[]            = L"horas";
	inline constexpr wchar_t BATTERY_MINUTES[]          = L"minutos";
	inline constexpr wchar_t BATTERY_STATUS[]           = L"\U0001F50B Bateria:";
#endif

// src/core/$(SYSTEM)/bluetooth.cpp
#if defined(ENABLE_BLUETOOTH)
	inline constexpr wchar_t BLUETOOTH[] = L"\U0001F91D Bluetooth:";
#endif

// src/core/$(SYSTEM)/cpu.cpp
#if defined(ENABLE_CPU)
	inline constexpr wchar_t ARCHITECTURE[]           = L"Arquitetura:";
	inline constexpr wchar_t INSTRUCTIONS_SUPPORTED[] = L"Instru\U000000E7\U000000F5es suportadas:";
	inline constexpr wchar_t THREADS[]                = L"Threads:";
	inline constexpr wchar_t CPU[]                    = L"\U0001F9EE Processador:";
#endif

// src/core/$(SYSTEM)/features.cpp
#if defined(ENABLE_FEATURES)
	inline constexpr wchar_t FEATURES[] = L"\U0001F527 Recursos:";
#endif

// src/core/$(SYSTEM)/gpu.cpp
#if defined(ENABLE_GPU)
	inline constexpr wchar_t GPU[]              = L"\U0001F31F GPU:";
	inline constexpr wchar_t GPU_MEMORY[]       = L"Mem\U000000F3ria:";
	inline constexpr wchar_t VIDEO_RESOLUTION[] = L"Resolu\U000000E7\U000000E3o:";
#endif

// src/core/$(SYSTEM)/hard_disks.cpp
#if defined(ENABLE_HARD_DISKS)
	inline constexpr wchar_t ALL[]        = L"Todos:";
	inline constexpr wchar_t AVAILABLE[]  = L"Dispon\U000000EDvel:";
	inline constexpr wchar_t HARD_DISKS[] = L"\U0001F4BF Discos r\U000000EDgidos:";
	inline constexpr wchar_t TOTAL[]      = L"Total:";
	inline constexpr wchar_t USED[]       = L"Em uso:";
#endif

// src/core/$(SYSTEM)/installed.cpp
#if defined(ENABLE_INSTALLED)
	inline constexpr wchar_t INSTALLED_PROGRAMS[] = L"\U0001F4BE Programas instalados:";
#endif

// src/core/$(SYSTEM)/memory.cpp
#if defined(ENABLE_MEMORY)
	inline constexpr wchar_t ERROR_MEMORY[]        = L"\U0000274C Erro lendo informa\U000000E7\U000000F5es "
	                                                 L"de mem\U000000F3ria";
	inline constexpr wchar_t MEMORY[]              = L"\U0001F418 Mem\U000000F3ria (RAM):";
	inline constexpr wchar_t MEMORY_AVAILABLE[]    = L"Dispon\U000000EDvel:";
	inline constexpr wchar_t MEMORY_CAPACITY[]     = L"Capacidade:";
	inline constexpr wchar_t MEMORY_MANUFACTURER[] = L"Fabricante:";
	inline constexpr wchar_t MEMORY_SPEED[]        = L"Velocidade:";
	inline constexpr wchar_t MEMORY_TOTAL[]        = L"Total:";
	inline constexpr wchar_t MEMORY_TYPE[]         = L"Tipo:";
	inline constexpr wchar_t MEMORY_USED[]         = L"Em uso:";
#endif

// src/core/$(SYSTEM)/network.cpp
#if defined(ENABLE_NETWORK)
	inline constexpr wchar_t DESCRIPTION[]   = L"Descri\U000000E7\U000000E3o:";
	inline constexpr wchar_t DNS_SERVERS[]   = L"Servidores DNS:";
	inline constexpr wchar_t GATEWAY[]       = L"Gateway:";
	inline constexpr wchar_t HOST_NAME[]     = L"Hostname:";
	inline constexpr wchar_t NETWORK[]       = L"\U0001F310 Rede:";
	inline constexpr wchar_t TCP_ENDPOINTS[] = L"TCP endpoints:";
	inline constexpr wchar_t UDP_ENDPOINTS[] = L"UDP endpoints:";
#endif

// src/core/$(SYSTEM)/operating_system.cpp
#if defined(ENABLE_OPERATING_SYSTEM)
	inline constexpr wchar_t COUNTRY_REGION[]   = L"Pa\U000000EDs/Regi\U000000E3o:";
	inline constexpr wchar_t DAY[]              = L"dia";
	inline constexpr wchar_t DAYS[]             = L"dias";
	inline constexpr wchar_t HOUR_UPTIME[]      = L"hora";
	inline constexpr wchar_t HOURS_UPTIME[]     = L"horas";
	inline constexpr wchar_t LANGUAGE[]         = L"Idioma:";
	inline constexpr wchar_t MINUTE[]           = L"minuto";
	inline constexpr wchar_t MINUTES[]          = L"minutos";
	inline constexpr wchar_t NAME[]             = L"Nome:";
	inline constexpr wchar_t OPERATING_SYSTEM[] = L"\U0001F5A5 Sistema operacional:";
	inline constexpr wchar_t SECOND[]           = L"segundo";
	inline constexpr wchar_t SECONDS[]          = L"segundos";
	inline constexpr wchar_t UPTIME[]           = L"Tempo de atividade:";
#endif

// src/core/$(SYSTEM)/processes.cpp
#if defined(ENABLE_PROCESSES)
	inline constexpr wchar_t RUNNING_PROCESSES[] = L"\U0001F3C3 Processos:";
	inline constexpr wchar_t TOTAL_PROCESSES[]   = L"Total:";
#endif

// src/core/$(SYSTEM)/restore.cpp
#if defined(ENABLE_RESTORE)
	inline constexpr wchar_t RESTORE_POINT[]     = L"\U0001F504 Ponto de restaura\U000000E7\U000000E3o:";
	inline constexpr wchar_t RESTORE_POINTS[]    = L"\U0001F504 Pontos de restaura\U000000E7\U000000E3o:";
	inline constexpr wchar_t NO_RESTORE_POINTS[] = L"\U0000274C Nenhum ponto de restaura"
	                                               L"\U000000E7\U000000E3o encontrado\n"
	                                               L"\t(Requer permiss\U000000E3o de administrador)";
#endif

// src/core/$(SYSTEM)/services.cpp
#if defined(ENABLE_SERVICES)
	inline constexpr wchar_t SERVICES[] = L"\U0001F3E2 Servi\U000000E7os:";
#endif

// src/core/$(SYSTEM)/startup.cpp
#if defined(ENABLE_STARTUP)
	inline constexpr wchar_t STARTUP[] = L"\U0001F680 Programas executados na inicializa\U000000E7\U000000E3o:";
#endif

// src/core/$(SYSTEM)/tasks.cpp
#if defined(ENABLE_TASKS)
	inline constexpr wchar_t TASKS[] = L"\U00002705 Tarefas:";
#endif

// src/core/windows/trash.cpp
#if defined(ENABLE_TRASH)
	inline constexpr wchar_t TRASH[]       = L"\U0001F5D1 Lixeira:";
	inline constexpr wchar_t TRASH_EMPTY[] = L"\U00002705 Lixeira vazia";
#endif

// src/core/$(SYSTEM)/usb.cpp
#if defined(ENABLE_USB)
	inline constexpr wchar_t USB[] = L"\U0001F50C Dispositivos USB:";
#endif

// src/core/$(SYSTEM)/wifi.cpp
#if defined(ENABLE_WIFI)
	inline constexpr wchar_t WIFI[] = L"\U0001F4F6 Wi-Fi:";
#endif

}
