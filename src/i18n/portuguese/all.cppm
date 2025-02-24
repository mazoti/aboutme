module;

export module i18n;

// src/main_$(SYSTEM).cpp
export constexpr wchar_t ANY_KEY_TO_CLOSE[] = L"Pressione qualquer tecla para fechar...";

// src/common/environment_variables.cpp
#if defined(ENABLE_ENVIRONMENT_VARIABLES)
	export constexpr wchar_t ENVIRONMENT_VARIABLES[]         = L"\U0001F333 Vari\U000000E1veis de ambiente:";
	export constexpr wchar_t ENVIRONMENT_VARIABLES_WARNING[] = L"\U000026A0 Aten\U000000E7\U000000E3o: verifique se "
	                                                            "esse caminho est\U000000E1 correto";
#endif

// src/common/system_clock.cpp
#if defined(ENABLE_SYSTEM_CLOCK)
	export constexpr wchar_t DATE[]                          = L"Data (dd/mm/aa):";
	export constexpr wchar_t DAYLIGHT_SAVING_TIME[]          = L"Hor\U000000E1rio de ver\U000000E3o";
	export constexpr wchar_t GMT[]                           = L"Fuso:";
	export constexpr wchar_t HOURS[]                         = L"horas";
	export constexpr wchar_t SYSTEM_CLOCK[]                  = L"\U0001F556 Rel\U000000F3gio do sistema:";
	export constexpr wchar_t TIME[]                          = L"Hora (hh:mm:ss):";
#endif

// src/core/$(SYSTEM)/battery.cpp
#if defined(ENABLE_BATTERY)
	export constexpr wchar_t BATTERY_AC_NOT_CONNECTED[]      = L"funcionando na bateria";
	export constexpr wchar_t BATTERY_AC_POWER[]              = L"conectado \u00E0 energia el\u00E9trica";
	export constexpr wchar_t BATTERY_DAYS[]                  = L"dias";
	export constexpr wchar_t BATTERY_HOURS[]                 = L"horas";
	export constexpr wchar_t BATTERY_MINUTES[]               = L"minutos";
	export constexpr wchar_t BATTERY_STATUS[]                = L"\U0001F50B Bateria:";
#endif

// src/core/$(SYSTEM)/bluetooth.cpp
#if defined(ENABLE_BLUETOOTH)
	export constexpr wchar_t BLUETOOTH[]                     = L"\U0001F91D Bluetooth:";
#endif

// src/core/$(SYSTEM)/cpu.cpp
#if defined(ENABLE_CPU)
	export constexpr wchar_t ARCHITECTURE[]                  = L"Arquitetura:";
	export constexpr wchar_t INSTRUCTIONS_SUPPORTED[]        = L"Instru\U000000E7\U000000F5es suportadas:";
	export constexpr wchar_t THREADS[]                       = L"Threads:";
	export constexpr wchar_t CPU[]                           = L"\U0001F9EE Processador:";
#endif

// src/core/$(SYSTEM)/features.cpp
#if defined(ENABLE_FEATURES)
	export constexpr wchar_t FEATURES[]                      = L"\U0001F527 Recursos:";
#endif

// src/core/$(SYSTEM)/gpu.cpp
#if defined(ENABLE_GPU)
	export constexpr wchar_t GPU[]                           = L"\U0001F31F GPU:";
	export constexpr wchar_t GPU_MEMORY[]                    = L"Mem\U000000F3ria:";
	export constexpr wchar_t VIDEO_RESOLUTION[]              = L"Resolu\U000000E7\U000000E3o:";
#endif

// src/core/$(SYSTEM)/hard_disks.cpp
#if defined(ENABLE_HARD_DISKS)
	export constexpr wchar_t ALL[]                           = L"Todos:";
	export constexpr wchar_t AVAILABLE[]                     = L"Dispon\U000000EDvel:";
	export constexpr wchar_t HARD_DISKS[]                    = L"\U0001F4BF Discos r\U000000EDgidos:";
	export constexpr wchar_t TOTAL[]                         = L"Total:";
	export constexpr wchar_t USED[]                          = L"Em uso:";
#endif

// src/core/$(SYSTEM)/installed.cpp
#if defined(ENABLE_INSTALLED)
	export constexpr wchar_t INSTALLED_PROGRAMS[]            = L"\U0001F4BE Programas instalados:";
#endif

// src/core/$(SYSTEM)/memory.cpp
#if defined(ENABLE_MEMORY)
	export constexpr wchar_t ERROR_MEMORY[]                  = L"\U0000274C Erro lendo informa\U000000E7\U000000F5es "
	                                                            "de mem\U000000F3ria";
	export constexpr wchar_t MEMORY[]                        = L"\U0001F418 Mem\U000000F3ria (RAM):";
	export constexpr wchar_t MEMORY_AVAILABLE[]              = L"Dispon\U000000EDvel:";
	export constexpr wchar_t MEMORY_CAPACITY[]               = L"Capacidade:";
	export constexpr wchar_t MEMORY_MANUFACTURER[]           = L"Fabricante:";
	export constexpr wchar_t MEMORY_SPEED[]                  = L"Velocidade:";
	export constexpr wchar_t MEMORY_TOTAL[]                  = L"Total:";
	export constexpr wchar_t MEMORY_TYPE[]                   = L"Tipo:";
	export constexpr wchar_t MEMORY_USED[]                   = L"Em uso:";
#endif

// src/core/$(SYSTEM)/network.cpp
#if defined(ENABLE_NETWORK)
	export constexpr wchar_t DESCRIPTION[]                   = L"Descri\U000000E7\U000000E3o:";
	export constexpr wchar_t DNS_SERVERS[]                   = L"Servidores DNS:";
	export constexpr wchar_t GATEWAY[]                       = L"Gateway:";
	export constexpr wchar_t NETWORK[]                       = L"\U0001F310 Rede:";
	export constexpr wchar_t HOST_NAME[]                     = L"Host:";
	export constexpr wchar_t TCP_ENDPOINTS[]                 = L"TCP endpoints:";
	export constexpr wchar_t UDP_ENDPOINTS[]                 = L"UDP endpoints:";
#endif

// src/core/$(SYSTEM)/operating_system.cpp
#if defined(ENABLE_OPERATING_SYSTEM)
	export constexpr wchar_t COUNTRY_REGION[]                = L"Pa\U000000EDs/Regi\U000000E3o:";
	export constexpr wchar_t DAYS[]                          = L"dias";
	export constexpr wchar_t HOURS_UPTIME[]                  = L"horas";
	export constexpr wchar_t LANGUAGE[]                      = L"Idioma:";
	export constexpr wchar_t MINUTES[]                       = L"minutos";
	export constexpr wchar_t NAME[]                          = L"Nome:";
	export constexpr wchar_t OPERATING_SYSTEM[]              = L"\U0001F5A5 Sistema operacional:";
	export constexpr wchar_t SECONDS[]                       = L"segundos";
	export constexpr wchar_t UPTIME[]                        = L"Tempo de atividade:";
#endif

// src/core/$(SYSTEM)/processes.cpp
#if defined(ENABLE_PROCESSES)
	export constexpr wchar_t RUNNING_PROCESSES[]             = L"\U0001F3C3 Processos:";
	export constexpr wchar_t TOTAL_PROCESSES[]               = L"Total:";
#endif

// src/core/$(SYSTEM)/restore.cpp
#if defined(ENABLE_RESTORE)
	export constexpr wchar_t RESTORE_POINTS[]                = L"\U0001F504 Pontos de restaura\U000000E7\U000000E3o:";
	export constexpr wchar_t NO_RESTORE_POINTS[]             = L"\U0000274C Nenhum ponto de restaura"
	                                                            "\U000000E7\U000000E3o encontrado\n"
	                                                            "\t(Requer permiss\U000000E3o de administrador)";
#endif

// src/core/$(SYSTEM)/services.cpp
#if defined(ENABLE_SERVICES)
	export constexpr wchar_t SERVICES[]                      = L"\U0001F3E2 Servi\U000000E7os:";
#endif

// src/core/$(SYSTEM)/startup.cpp
#if defined(ENABLE_STARTUP)
	export constexpr wchar_t STARTUP[]                       = L"\U0001F680 Programas executados na "
	                                                            "inicializa\U000000E7\U000000E3o:";
#endif

// src/core/$(SYSTEM)/tasks.cpp
#if defined(ENABLE_TASKS)
	export constexpr wchar_t TASKS[]                         = L"\U00002705 Tarefas:";
#endif

// src/core/windows/trash.cpp
#if defined(ENABLE_TRASH)
	export constexpr wchar_t TRASH[]                         = L"\U0001F5D1 Lixeira:";
	export constexpr wchar_t TRASH_EMPY[]                    = L"\U0000274C Lixeira vazia";
#endif

// src/core/$(SYSTEM)/usb.cpp
#if defined(ENABLE_USB)
	export constexpr wchar_t USB[]                           = L"\U0001F50C Dispositivos USB:";
#endif

// src/core/$(SYSTEM)/wifi.cpp
#if defined(ENABLE_WIFI)
	export constexpr wchar_t WIFI[]                          = L"\U0001F4F6 Wi-Fi:";
#endif
