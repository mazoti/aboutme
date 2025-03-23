module;

#include <iostream>
#include <filesystem>
#include <unordered_set>
#include <span>
#include <string>
#include <string_view>

module common;

import i18n;

// Defines a static constant set of environment variable names that are expected to contain paths.
// This set is used to determine if a variable's value should be treated as a list of filesystem paths
static const std::unordered_set<std::wstring> environment_variable_paths = {
	L"XDG_CONFIG_DIRS",
	L"__EGL_EXTERNAL_PLATFORM_CONFIG_DIRS",
	L"__VSCMD_PREINIT_PATH",
	L"ALLUSERSPROFILE",
	L"ALSA_CONFIG_DIR",
	L"ALSA_CONFIG_PATH",
	L"APPDATA",
	L"CARGO_HOME",
	L"CARGO_INSTALL_ROOT",
	L"CHROME_WRAPPER",
	L"CommonProgramFiles",
	L"CommonProgramFiles(x86)",
	L"CommonProgramW6432",
	L"ComSpec",
	L"DevEnvDir",
	L"DriverData",
	L"ENV",
	L"ExtensionSdkDir",
	L"EXTERNAL_INCLUDE",
	L"FLATPAK_SANDBOX_DIR",
	L"FrameworkDir",
	L"FrameworkDir64",
	L"GI_TYPELIB_PATH",
	L"GIO_LAUNCHED_DESKTOP_FILE",
	L"GIT_ASKPASS",
	L"GNOME_KEYRING_CONTROL",
	L"GST_PLUGIN_SYSTEM_PATH",
	L"HOME",
	L"HOMEDRIVE",
	L"INCLUDE",
	L"JAVA_HOME",
	L"LD_LIBRARY_PATH",
	L"LIB",
	L"LIBPATH",
	L"LOCALAPPDATA",
	L"MAIL",
	L"NPM_CONFIG_USERCONFIG",
	L"Path",
	L"PATH",
	L"ProgramData",
	L"ProgramFiles",
	L"ProgramFiles(x86)",
	L"ProgramW6432",
	L"PSModulePath",
	L"PUBLIC",
	L"PULSE_CLIENTCONFIG",
	L"PWD",
	L"PYTHONUSERBASE",
	L"SHELL",
	L"SSH_AUTH_SOCK",
	L"SystemDrive",
	L"SystemRoot",
	L"TEMP",
	L"TMP",
	L"UniversalCRTSdkDir",
	L"USERPROFILE",
	L"VCIDEInstallDir",
	L"VCINSTALLDIR",
	L"VCPKG_ROOT",
	L"VCToolsInstallDir",
	L"VCToolsRedistDir",
	L"VS170COMNTOOLS",
	L"VSCODE_GIT_ASKPASS_MAIN",
	L"VSCODE_GIT_ASKPASS_NODE",
	L"VSCODE_GIT_IPC_HANDLE",
	L"VSINSTALLDIR",
	L"windir",
	L"WindowsLibPath",
	L"WindowsSdkBinPath",
	L"WindowsSdkDir",
	L"WindowsSdkVerBinPath",
	L"XAUTHORITY",
	L"XCURSOR_PATH",
	L"XDG_CACHE_HOME",
	L"XDG_CONFIG_HOME",
	L"XDG_DATA_DIRS",
	L"XDG_DATA_HOME",
	L"XDG_GREETER_DATA_DIR",
	L"XDG_RUNTIME_DIR",
	L"XDG_SEAT_PATH",
	L"XDG_SESSION_PATH",
	L"XDG_STATE_HOME",
	L"ZYPAK_BIN",
	L"ZYPAK_LIB"
};

// Defines a function to process and display environment variables.
void environment_variables(std::span<const char*>& env_vars, char path_separator) noexcept{
	size_t position;
	std::string temp;
	std::wstring tmp;
	std::wstring_view wdata_view, wvariable_name;

	// Convert path separator to wide character
	wchar_t wpath_separator = static_cast<wchar_t>(path_separator);

	std::wcout << i18n::ENVIRONMENT_VARIABLES << std::endl;

	// Iterates over each environment variable in the provided span
	for(const char *environment : env_vars){
		temp = std::string(environment);
		tmp = std::wstring(temp.begin(), temp.end());
		wdata_view = tmp;

		// Finds the position of the '=' character, which separates the variable name from its value
		position = wdata_view.find('=');
		if(position == std::wstring::npos) continue;

		// Extracts the variable name from the start of the string up to the '=' position
		wvariable_name = wdata_view.substr(0, position);
		if(wvariable_name.size() < 1) continue;

		std::wcout << L'\t' << wvariable_name << L':' << std::endl;

		// Removes the variable name and '=' from the view, leaving only the value
		wdata_view.remove_prefix(position + 1);

		// Checks if the variable is in the set of path-containing variables
		if(environment_variable_paths.contains(wvariable_name.data())){
			// Iterates over the value, splitting it by the path separator
			for(position = wdata_view.find(wpath_separator); position != std::wstring::npos;
			position = wdata_view.find(wpath_separator)){
				wvariable_name = wdata_view.substr(0, position);

				if(!wvariable_name.size()) continue;
				std::wcout << L"\t\t" << wvariable_name << std::endl;

				// Checks if the path exists using the filesystem library
				if(!std::filesystem::exists(std::filesystem::path(wvariable_name)))
					std::wcout << L"\t\t\t" << i18n::ENVIRONMENT_VARIABLES_WARNING << std::endl;

				wdata_view.remove_prefix(position + 1);
			}

			// Handles the last path segment after the final separator
			if(!wdata_view.size()){
				std::wcout << std::endl;
				continue;
			}

			if(!std::filesystem::exists(std::filesystem::path(wdata_view))){
				std::wcout << L"\t\t" << wdata_view << std::endl
					<< L"\t\t\t" << i18n::ENVIRONMENT_VARIABLES_WARNING << std::endl << std::endl;
				continue;
			}

			std::wcout << L"\t\t" << wdata_view << std::endl << std::endl;
			continue;
		}

		for(position = wdata_view.find(wpath_separator); position != std::wstring::npos;
		position = wdata_view.find(wpath_separator)){
			std::wcout << L"\t\t" << wdata_view.substr(0, position) << std::endl;
			wdata_view.remove_prefix(position + 1);
		}

		std::wcout << L"\t\t" << wdata_view << std::endl << std::endl;
	}
}
