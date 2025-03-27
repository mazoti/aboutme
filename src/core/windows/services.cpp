module;

#include <iostream>
#include <memory>
#include <set>
#include <span>
#include <vector>
#include <Windows.h>

#pragma comment(lib, "Advapi32.lib")

module core;

import i18n;
import i18n_system;

// Lists running Windows services ordered
std::wostream& services() noexcept {
	SC_HANDLE scm_handle;
	LPENUM_SERVICE_STATUSW service_status;
	DWORD bytes_needed = 0, services_returned = 0, resume_handle = 0;

	std::vector<BYTE> buffer;
	std::set<std::wstring> services_running_ordered;

	// Opens a handle to the SCM with enumeration privileges
	scm_handle = OpenSCManagerW(nullptr, nullptr, SC_MANAGER_ENUMERATE_SERVICE);
	if(scm_handle == nullptr) return std::wcerr << i18n_system::ERROR_SERVICES << L"\n\n";

	std::unique_ptr<SC_HANDLE__, decltype([](SC_HANDLE handle){ CloseServiceHandle(handle); })>
		scm_handle_ptr(scm_handle);

	// Determines the required buffer size
	if((EnumServicesStatusW(scm_handle, SERVICE_WIN32, SERVICE_ACTIVE, nullptr, 0, &bytes_needed,
	&services_returned, &resume_handle)) || (GetLastError() != ERROR_MORE_DATA))
		return std::wcerr << i18n_system::ERROR_SERVICES_ENUM << L"\n\n";

	// Allocates buffer and enumerates services
	buffer.resize(bytes_needed);
	service_status = reinterpret_cast<LPENUM_SERVICE_STATUSW>(buffer.data());

	if(!EnumServicesStatusW(scm_handle, SERVICE_WIN32, SERVICE_ACTIVE, service_status,
	static_cast<DWORD>(buffer.size()), &bytes_needed, &services_returned, &resume_handle))
		return std::wcerr << i18n_system::ERROR_SERVICES_ENUM << L"\n\n";

	// Collects running services into an ordered set
	for(const ENUM_SERVICE_STATUSW& service : std::span<ENUM_SERVICE_STATUSW>(service_status, services_returned)){
		if(service.ServiceStatus.dwCurrentState == SERVICE_RUNNING)
			services_running_ordered.emplace(service.lpDisplayName);
	}

	// Prints the running services
	if(!services_running_ordered.empty()){
		std::wcout << i18n::SERVICES << L'\n';
		for(const std::wstring& data : services_running_ordered) std::wcout << L'\t' << data << L'\n';
	}

	return std::wcout << L'\n';
}
