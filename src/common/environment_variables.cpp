module;

#include <iostream>
#include <filesystem>
#include <set>
#include <span>

#include <string_view>

module common;

import i18n;

// Defines a static constant set of environment variable names that are expected to contain paths.
// This set is used to determine if a variable's value should be treated as a list of filesystem paths
static const std::set<std::string> environment_variable_paths = {
	"XDG_CONFIG_DIRS",
	"__EGL_EXTERNAL_PLATFORM_CONFIG_DIRS",
	"__VSCMD_PREINIT_PATH",
	"ALLUSERSPROFILE",
	"ALSA_CONFIG_DIR",
	"ALSA_CONFIG_PATH",
	"APPDATA",
	"CARGO_HOME",
	"CARGO_INSTALL_ROOT",
	"CHROME_WRAPPER",
	"CommonProgramFiles",
	"CommonProgramFiles(x86)",
	"CommonProgramW6432",
	"ComSpec",
	"DevEnvDir",
	"DriverData",
	"ENV",
	"ExtensionSdkDir",
	"EXTERNAL_INCLUDE",
	"FLATPAK_SANDBOX_DIR",
	"FrameworkDir",
	"FrameworkDir64",
	"GI_TYPELIB_PATH",
	"GIO_LAUNCHED_DESKTOP_FILE",
	"GIT_ASKPASS",
	"GNOME_KEYRING_CONTROL",
	"GST_PLUGIN_SYSTEM_PATH",
	"HOME",
	"HOMEDRIVE",
	"INCLUDE",
	"JAVA_HOME",
	"LD_LIBRARY_PATH",
	"LIB",
	"LIBPATH",
	"LOCALAPPDATA",
	"MAIL",
	"NPM_CONFIG_USERCONFIG",
	"Path",
	"PATH",
	"ProgramData",
	"ProgramFiles",
	"ProgramFiles(x86)",
	"ProgramW6432",
	"PSModulePath",
	"PUBLIC",
	"PULSE_CLIENTCONFIG",
	"PWD",
	"PYTHONUSERBASE",
	"SHELL",
	"SSH_AUTH_SOCK",
	"SystemDrive",
	"SystemRoot",
	"TEMP",
	"TMP",
	"UniversalCRTSdkDir",
	"USERPROFILE",
	"VCIDEInstallDir",
	"VCINSTALLDIR",
	"VCPKG_ROOT",
	"VCToolsInstallDir",
	"VCToolsRedistDir",
	"VS170COMNTOOLS",
	"VSCODE_GIT_ASKPASS_MAIN",
	"VSCODE_GIT_ASKPASS_NODE",
	"VSCODE_GIT_IPC_HANDLE",
	"VSINSTALLDIR",
	"windir",
	"WindowsLibPath",
	"WindowsSdkBinPath",
	"WindowsSdkDir",
	"WindowsSdkVerBinPath",
	"XAUTHORITY",
	"XCURSOR_PATH",
	"XDG_CACHE_HOME",
	"XDG_CONFIG_HOME",
	"XDG_DATA_DIRS",
	"XDG_DATA_HOME",
	"XDG_GREETER_DATA_DIR",
	"XDG_RUNTIME_DIR",
	"XDG_SEAT_PATH",
	"XDG_SESSION_PATH",
	"XDG_STATE_HOME",
	"ZYPAK_BIN",
	"ZYPAK_LIB"
};


// Defines a function to process and display environment variables.
void environment_variables(std::span<const char*>& env_vars, char path_separator) noexcept{
	size_t position;
	std::string_view data_view, variable_name, temp;

	std::wcout << i18n::ENVIRONMENT_VARIABLES << std::endl;

	// Iterates over each environment variable in the provided span
	for(const char *environment : env_vars){
		data_view = std::string_view(environment);

		// Finds the position of the '=' character, which separates the variable name from its value
		position = data_view.find('=');
		if(position == std::string::npos) continue;

		// Extracts the variable name from the start of the string up to the '=' position
		variable_name = data_view.substr(0, position);
		if(variable_name.size() < 1) continue;

		std::wcout << L'\t' << std::wstring(variable_name.begin(), variable_name.end()) << L':' << std::endl;

		// Removes the variable name and '=' from the view, leaving only the value
		data_view.remove_prefix(position + 1);

		// Checks if the variable is in the set of path-containing variables
		if(environment_variable_paths.find(std::string(variable_name)) != environment_variable_paths.end()){

			// Iterates over the value, splitting it by the path separator
			for(position = data_view.find(path_separator);
				position != std::string::npos;
				position = data_view.find(path_separator))
			{
				variable_name = data_view.substr(0, position);
				
				if(!variable_name.size()) continue;

				std::wcout << L"\t\t" << std::wstring(variable_name.begin(), variable_name.end()) << std::endl;

				// Checks if the path exists using the filesystem library
				if(!std::filesystem::exists(std::filesystem::path(variable_name)))
					std::wcout << L"\t\t\t" << i18n::ENVIRONMENT_VARIABLES_WARNING << std::endl;

				data_view.remove_prefix(position + 1);
			}

			// Handles the last path segment after the final separator
			if(!data_view.size()){
				std::wcout << std::endl;
				continue;
			}
			
			if(!std::filesystem::exists(std::filesystem::path(data_view))){
				std::wcout << L"\t\t" << std::wstring(data_view.begin(), data_view.end()) << std::endl
					<< L"\t\t\t" << i18n::ENVIRONMENT_VARIABLES_WARNING << std::endl << std::endl;
				continue;
			}

			std::wcout << L"\t\t" << std::wstring(data_view.begin(), data_view.end()) << std::endl << std::endl;
			continue;
		}

		// For non-path variables, splits the value by the path separator and prints each part
		for(position = data_view.find(path_separator);
			position != std::string::npos;
			position = data_view.find(path_separator))
		{
			temp = data_view.substr(0, position);
			std::wcout << L"\t\t" << std::wstring(temp.begin(), temp.end()) << std::endl;
			data_view.remove_prefix(position + 1);
		}

		std::wcout << L"\t\t" << std::wstring(data_view.begin(), data_view.end()) << std::endl << std::endl;
	}
}
