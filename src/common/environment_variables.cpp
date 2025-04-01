module;

#include <iostream>
#include <filesystem>
#include <unordered_set>
#include <span>
#include <string>
#include <string_view>
#include <ranges>

module common;

import i18n;

// Defines a static constant set of environment variable names that are expected to contain paths.
// This set is used to determine if a variable's value should be treated as a list of filesystem paths
static const std::unordered_set<std::wstring> environment_variable_paths = {
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
	L"XDG_CONFIG_DIRS",
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

// Function to process and display environment variables
void environment_variables(const char* envp[], char separator) noexcept{
	std::string temp;
	std::wstring tmp;
	std::wstring_view wdata_view, wvariable_name, part;

	size_t position, count = 0;
	wchar_t wpath_separator = static_cast<wchar_t>(separator);

	// Counts the number of environment variables
	if(envp){
		while(envp[count] != nullptr) ++count;
	}

	std::wcout << i18n::ENVIRONMENT_VARIABLES << L'\n';

	// Processes each environment variable
	for(const char* environment : std::span<const char*>(envp, count)){
		// Converts char* to wstring for wide-character output
		temp = environment;
		tmp = std::wstring(temp.begin(), temp.end());
		wdata_view = tmp;

		// Finds the '=' separator between name and value
		position = wdata_view.find(L'=');
		if(position == std::wstring::npos) continue;

		// Extracts the variable name
		wvariable_name = wdata_view.substr(0, position);
		if(wvariable_name.empty()) continue;

		// Prints the variable name
		std::wcout << L'\t' << wvariable_name << L":\n";

		// Extracts the value
		wdata_view.remove_prefix(position + 1);

		// Splits the value by the separator and processes each part
		for(auto part_range : std::ranges::split_view(wdata_view, wpath_separator)){
			part = std::wstring_view(part_range.begin(), part_range.end());
			if(part.empty()) continue;

			std::wcout << L"\t\t" << part << L'\n';
			if(environment_variable_paths.contains(wvariable_name.data()) &&
			!std::filesystem::exists(std::filesystem::path(part)))
				std::wcout << L"\t\t\t" << i18n::ENVIRONMENT_VARIABLES_WARNING << L'\n';
		}
		std::wcout << L'\n';
	}
}
