cmake_minimum_required(VERSION 3.21)
project(aboutme
	VERSION 54.0.0
	DESCRIPTION "AboutMe by Marcos Mazoti"
	HOMEPAGE_URL "https://github.com/mazoti/aboutme"
	LANGUAGES CXX
)

# Sets C++ standard and related options
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)

# Translation selection
set(TRANSLATION "english" CACHE STRING "Translation language (check i18n folder)")
set_property(CACHE TRANSLATION PROPERTY STRINGS "english" "portuguese")

# Detects and normalizes OS name
message(STATUS "Detected operating system: ${CMAKE_SYSTEM_NAME}")
message(STATUS "Using translation: ${TRANSLATION}")
string(TOLOWER "${CMAKE_SYSTEM_NAME}" SYSTEM)

# Checks if the selected translation files exist
if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/i18n/${TRANSLATION}/${SYSTEM}.cppm")
	message(FATAL_ERROR "Translation file for '${TRANSLATION}' on '${SYSTEM}' not found: ${CMAKE_CURRENT_SOURCE_DIR}/src/i18n/${TRANSLATION}/${SYSTEM}.cppm")
endif()

if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/i18n/${TRANSLATION}/all.cppm")
	message(FATAL_ERROR "Common translation file for '${TRANSLATION}' not found: ${CMAKE_CURRENT_SOURCE_DIR}/src/i18n/${TRANSLATION}/all.cppm")
endif()

# Defines the module library
add_library(all_modules)
target_sources(all_modules PUBLIC FILE_SET CXX_MODULES FILES
	src/common/common.cppm
	src/core/core.cppm
	src/i18n/${TRANSLATION}/${SYSTEM}.cppm
	src/i18n/${TRANSLATION}/all.cppm
)

# Defines the executable
add_executable(aboutme)
target_sources(aboutme PRIVATE
	src/core/main.cpp
	src/main_${SYSTEM}.cpp
)

#==================================================== Common section ===================================================

macro(add_common_module MODULE_NAME DESCRIPTION FILE)
	if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${FILE}")
		option(ENABLE_${MODULE_NAME} "${DESCRIPTION}" ON)
		if(ENABLE_${MODULE_NAME})
			target_compile_definitions(aboutme PRIVATE ENABLE_${MODULE_NAME})
			target_compile_definitions(all_modules PRIVATE ENABLE_${MODULE_NAME})
			target_sources(aboutme PRIVATE ${FILE})
			message(STATUS "- ${DESCRIPTION}")
		endif()
	else()
		message(WARNING "Common module file not found: ${FILE}")
	endif()
endmacro()

add_common_module(ENVIRONMENT_VARIABLES "Display and check paths of environment variables"
	src/common/environment_variables.cpp)
add_common_module(SYSTEM_CLOCK "Display system date, time, and timezone" src/common/system_clock.cpp)

#===================================================== Core section ====================================================

macro(add_core_module MODULE_NAME DESCRIPTION)
	if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/core/${SYSTEM}/${MODULE_NAME}.cpp")
		option(ENABLE_${MODULE_NAME} "${DESCRIPTION}" ON)
		if(ENABLE_${MODULE_NAME})
			target_compile_definitions(aboutme PRIVATE ENABLE_${MODULE_NAME})
			target_compile_definitions(all_modules PRIVATE ENABLE_${MODULE_NAME})
			target_sources(aboutme PRIVATE src/core/${SYSTEM}/${MODULE_NAME}.cpp)
			message(STATUS "- ${DESCRIPTION}")
		endif()
	else()
		message(STATUS "Module '${MODULE_NAME}' not available for '${SYSTEM}'")
	endif()
endmacro()

add_core_module(BATTERY          "Display battery details")
add_core_module(BLUETOOTH        "Display Bluetooth devices")
add_core_module(CPU              "Display CPU details")
add_core_module(FEATURES         "Display installed features")
add_core_module(GPU              "Display GPU details")
add_core_module(HARD_DISKS       "Display hard disk size, usage and free space")
add_core_module(INSTALLED        "Display installed applications")
add_core_module(MEMORY           "Display memory size, usage and available")
add_core_module(NETWORK          "Display network information")
add_core_module(OPERATING_SYSTEM "Display operating system information")
add_core_module(PROCESSES        "Display memory and CPU usage of each running process")
add_core_module(RESTORE          "Display restore points")
add_core_module(SERVICES         "Display system running services")
add_core_module(SHARED           "Display shared resources")
add_core_module(STARTUP          "Display applications loaded on startup")
add_core_module(TASKS            "Display scheduled tasks")
add_core_module(TRASH            "Display files and folders in the trash can")
add_core_module(USB              "Display connected USB device details")
add_core_module(WIFI             "Display Wi-Fi networks")

#===================================================== Link section ====================================================

# Links against the module library
target_link_libraries(aboutme PRIVATE all_modules)

# Adds icon and binary descriptions on Windows version
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	target_link_options(aboutme PRIVATE ${CMAKE_SOURCE_DIR}/resources/aboutme.res)
endif()

# Specify installation location
install(TARGETS aboutme RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})
