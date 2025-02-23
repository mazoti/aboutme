cmake_minimum_required(VERSION 3.21)
project(aboutme
	VERSION 54.0.0
	DESCRIPTION "AboutMe by Marcos Mazoti"
	HOMEPAGE_URL "https://github.com/mazoti/aboutme"
	LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)

set(TRANSLATION "english" CACHE STRING "Translation language (check i18n folder)")
set_property(CACHE TRANSLATION PROPERTY STRINGS "english" "portuguese")

message(STATUS "Detected operating system: ${CMAKE_SYSTEM_NAME}")
message(STATUS "Using translation: ${TRANSLATION}")

string(TOLOWER "${CMAKE_SYSTEM_NAME}" SYSTEM)

add_library(all_modules)
target_sources(all_modules PUBLIC FILE_SET CXX_MODULES FILES
	src/common/common.cppm
	src/core/core.cppm
	src/i18n/${TRANSLATION}/${SYSTEM}.cppm
	src/i18n/${TRANSLATION}/all.cppm
)

add_executable(aboutme)

#==================================================== Common section ===================================================

option(ENABLE_ENVIRONMENT_VARIABLES "Display and check paths of environment variables" OFF)
if(ENABLE_ENVIRONMENT_VARIABLES)
	target_compile_definitions(aboutme PRIVATE ENABLE_ENVIRONMENT_VARIABLES)
	target_compile_definitions(all_modules PRIVATE ENABLE_ENVIRONMENT_VARIABLES)
	target_sources(aboutme PRIVATE src/common/environment_variables.cpp)
	message(STATUS "- Display and check paths of environment variables")
endif()

option(ENABLE_SYSTEM_CLOCK "Display system date, time and timezone" OFF)
if(ENABLE_SYSTEM_CLOCK)
	target_compile_definitions(aboutme PRIVATE ENABLE_SYSTEM_CLOCK)
	target_compile_definitions(all_modules PRIVATE ENABLE_SYSTEM_CLOCK)
	target_sources(aboutme PRIVATE src/common/system_clock.cpp)
	message(STATUS "- Display system date, time and timezone")
endif()

#===================================================== Core section ====================================================

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/core/${SYSTEM}/battery.cpp")
	option(ENABLE_BATTERY "Display battery details" OFF)
	if(ENABLE_BATTERY)
		target_compile_definitions(aboutme PRIVATE ENABLE_BATTERY)
		target_compile_definitions(all_modules PRIVATE ENABLE_BATTERY)
		target_sources(aboutme PRIVATE src/core/${SYSTEM}/battery.cpp)
		message(STATUS "- Display battery details")
	endif()
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/core/${SYSTEM}/bluetooth.cpp")
	option(ENABLE_BLUETOOTH "Display Bluetooth devices" OFF)
	if(ENABLE_BLUETOOTH)
		target_compile_definitions(aboutme PRIVATE ENABLE_BLUETOOTH)
		target_compile_definitions(all_modules PRIVATE ENABLE_BLUETOOTH)
		target_sources(aboutme PRIVATE src/core/${SYSTEM}/bluetooth.cpp)
		message(STATUS "- Display Bluetooth devices")
	endif()
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/core/${SYSTEM}/cpu.cpp")
	option(ENABLE_CPU "Display CPU details" OFF)
	if(ENABLE_CPU)
		target_compile_definitions(aboutme PRIVATE ENABLE_CPU)
		target_compile_definitions(all_modules PRIVATE ENABLE_CPU)
		target_sources(aboutme PRIVATE src/core/${SYSTEM}/cpu.cpp)
		message(STATUS "- Display CPU details")
	endif()
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/core/${SYSTEM}/features.cpp")
	option(ENABLE_FEATURES "Display installed features" OFF)
	if(ENABLE_FEATURES)
		target_compile_definitions(aboutme PRIVATE ENABLE_FEATURES)
		target_compile_definitions(all_modules PRIVATE ENABLE_FEATURES)
		target_sources(aboutme PRIVATE src/core/${SYSTEM}/features.cpp)
		message(STATUS "- Display installed features")
	endif()
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/core/${SYSTEM}/gpu.cpp")
	option(ENABLE_GPU "Display GPU details" OFF)
	if(ENABLE_GPU)
		target_compile_definitions(aboutme PRIVATE ENABLE_GPU)
		target_compile_definitions(all_modules PRIVATE ENABLE_GPU)
		target_sources(aboutme PRIVATE src/core/${SYSTEM}/gpu.cpp)
		message(STATUS "- Display GPU details")
	endif()
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/core/${SYSTEM}/hard_disks.cpp")
	option(ENABLE_HARD_DISKS "Display hard disks size, usage and free space" OFF)
	if(ENABLE_HARD_DISKS)
		target_compile_definitions(aboutme PRIVATE ENABLE_HARD_DISKS)
		target_compile_definitions(all_modules PRIVATE ENABLE_HARD_DISKS)
		target_sources(aboutme PRIVATE src/core/${SYSTEM}/hard_disks.cpp)
		message(STATUS "- Display hard disks size, usage and free space")
	endif()
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/core/${SYSTEM}/installed.cpp")
	option(ENABLE_INSTALLED "Display installed applications" OFF)
	if(ENABLE_INSTALLED)
		target_compile_definitions(aboutme PRIVATE ENABLE_INSTALLED)
		target_compile_definitions(all_modules PRIVATE ENABLE_INSTALLED)
		target_sources(aboutme PRIVATE src/core/${SYSTEM}/installed.cpp)
		message(STATUS "- Display installed applications")
	endif()
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/core/${SYSTEM}/memory.cpp")
	option(ENABLE_MEMORY "Display memory size, usage and available" OFF)
	if(ENABLE_MEMORY)
		target_compile_definitions(aboutme PRIVATE ENABLE_MEMORY)
		target_compile_definitions(all_modules PRIVATE ENABLE_MEMORY)
		target_sources(aboutme PRIVATE src/core/${SYSTEM}/memory.cpp)
		message(STATUS "- Display memory size, usage and available")
	endif()
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/core/${SYSTEM}/network.cpp")
	option(ENABLE_NETWORK "Display network information" OFF)
	if(ENABLE_NETWORK)
		target_compile_definitions(aboutme PRIVATE ENABLE_NETWORK)
		target_compile_definitions(all_modules PRIVATE ENABLE_NETWORK)
		target_sources(aboutme PRIVATE src/core/${SYSTEM}/network.cpp)
		message(STATUS "- Display network information")
	endif()
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/core/${SYSTEM}/operating_system.cpp")
	option(ENABLE_OPERATING_SYSTEM "Display operating system information" OFF)
	if(ENABLE_OPERATING_SYSTEM)
		target_compile_definitions(aboutme PRIVATE ENABLE_OPERATING_SYSTEM)
		target_compile_definitions(all_modules PRIVATE ENABLE_OPERATING_SYSTEM)
		target_sources(aboutme PRIVATE src/core/${SYSTEM}/operating_system.cpp)
		message(STATUS "- Display operating system information")
	endif()
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/core/${SYSTEM}/processes.cpp")
	option(ENABLE_PROCESSES "Display memory and CPU usage of each running process" OFF)
	if(ENABLE_PROCESSES)
		target_compile_definitions(aboutme PRIVATE ENABLE_PROCESSES)
		target_compile_definitions(all_modules PRIVATE ENABLE_PROCESSES)
		target_sources(aboutme PRIVATE src/core/${SYSTEM}/processes.cpp)
		message(STATUS "- Display memory and CPU usage of each running process")
	endif()
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/core/${SYSTEM}/restore.cpp")
	option(ENABLE_RESTORE "Display restore points" OFF)
	if(ENABLE_RESTORE)
		target_compile_definitions(aboutme PRIVATE ENABLE_RESTORE)
		target_compile_definitions(all_modules PRIVATE ENABLE_RESTORE)
		target_sources(aboutme PRIVATE src/core/${SYSTEM}/restore.cpp)
		message(STATUS "- Display restore points")
	endif()
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/core/${SYSTEM}/services.cpp")
	option(ENABLE_SERVICES "Display system running services" OFF)
	if(ENABLE_SERVICES)
		target_compile_definitions(aboutme PRIVATE ENABLE_SERVICES)
		target_compile_definitions(all_modules PRIVATE ENABLE_SERVICES)
		target_sources(aboutme PRIVATE src/core/${SYSTEM}/services.cpp)
		message(STATUS "- Display system running services")
	endif()
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/core/${SYSTEM}/startup.cpp")
	option(ENABLE_STARTUP "Display applications loaded on startup" OFF)
	if(ENABLE_STARTUP)
		target_compile_definitions(aboutme PRIVATE ENABLE_STARTUP)
		target_compile_definitions(all_modules PRIVATE ENABLE_STARTUP)
		target_sources(aboutme PRIVATE src/core/${SYSTEM}/startup.cpp)
		message(STATUS "- Display applications loaded on startup")
	endif()
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/core/${SYSTEM}/tasks.cpp")
	option(ENABLE_TASKS "Display scheduled tasks" OFF)
	if(ENABLE_TASKS)
		target_compile_definitions(aboutme PRIVATE ENABLE_TASKS)
		target_compile_definitions(all_modules PRIVATE ENABLE_TASKS)
		target_sources(aboutme PRIVATE src/core/${SYSTEM}/tasks.cpp)
		message(STATUS "- Display scheduled tasks")
	endif()
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/core/${SYSTEM}/trash.cpp")
	option(ENABLE_TRASH "Display files and folders in the trash can" OFF)
	if(ENABLE_TRASH)
		target_compile_definitions(aboutme PRIVATE ENABLE_TRASH)
		target_compile_definitions(all_modules PRIVATE ENABLE_TRASH)
		target_sources(aboutme PRIVATE src/core/${SYSTEM}/trash.cpp)
		message(STATUS "- Display files and folders in the trash can")
	endif()
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/core/${SYSTEM}/usb.cpp")
	option(ENABLE_USB "Display connected USB devices detais" OFF)
	if(ENABLE_USB)
		target_compile_definitions(aboutme PRIVATE ENABLE_USB)
		target_compile_definitions(all_modules PRIVATE ENABLE_USB)
		target_sources(aboutme PRIVATE src/core/${SYSTEM}/usb.cpp)
		message(STATUS "- Display connected USB devices detais")
	endif()
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/core/${SYSTEM}/wifi.cpp")
	option(ENABLE_WIFI "Display Wi-Fi networks" OFF)
	if(ENABLE_WIFI)
		target_compile_definitions(aboutme PRIVATE ENABLE_WIFI)
		target_compile_definitions(all_modules PRIVATE ENABLE_WIFI)
		target_sources(aboutme PRIVATE src/core/${SYSTEM}/wifi.cpp)
		message(STATUS "- Display Wi-Fi networks")
	endif()
endif()

target_sources(aboutme PRIVATE
	src/core/main.cpp
	src/main_${SYSTEM}.cpp
)

#===================================================== Link section ====================================================

# Link against the module library
target_link_libraries(aboutme PRIVATE all_modules)

# Adds icon and binary descriptions on Windows version
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	target_link_options(aboutme PRIVATE ${CMAKE_SOURCE_DIR}/resources/aboutme.res)
endif()

# Specify installation location
install(TARGETS aboutme RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})
