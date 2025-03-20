module;

#include <iostream>
#include <memory>
#include <set>
#include <span>
#include <vector>
#include <Windows.h>

#pragma comment(lib, "Advapi32.lib")

module core;

import common;
import i18n;
import i18n_system;

// Lists running Windows services in an ordered manner
std::wostream& services() noexcept {
	DWORD bytes_needed = 0, services_returned = 0, resume_handle = 0;
	SC_HANDLE scm_handle;

	std::string tmp;
	std::vector<BYTE> buffer;
	std::set<std::wstring> services_running_ordered;

	// Opens a handle to the SCM with enumeration privileges
	scm_handle = OpenSCManager(nullptr, nullptr, SC_MANAGER_ENUMERATE_SERVICE);
	if(scm_handle == nullptr) return std::wcerr << i18n_system::ERROR_SERVICES << std::endl << std::endl;
	std::unique_ptr<SC_HANDLE__, decltype([](SC_HANDLE handle) { CloseServiceHandle(handle); })>
		scm_handle_ptr(scm_handle);

	// Determines the required buffer size
	if((EnumServicesStatus(scm_handle, SERVICE_WIN32, SERVICE_STATE_ALL, nullptr, 0, &bytes_needed,
	&services_returned, &resume_handle)) || (GetLastError() != ERROR_MORE_DATA))
		return std::wcerr << i18n_system::ERROR_SERVICES_ENUM << std::endl << std::endl;

	// Allocates buffer and enumerate services
	buffer.resize(bytes_needed);
	LPENUM_SERVICE_STATUS service_status = reinterpret_cast<LPENUM_SERVICE_STATUS>(buffer.data());
	if(!EnumServicesStatus(scm_handle, SERVICE_WIN32, SERVICE_STATE_ALL, service_status,
	buffer.size(), &bytes_needed, &services_returned, &resume_handle))
		return std::wcerr << i18n_system::ERROR_SERVICES_ENUM << std::endl << std::endl;

	// Collects running services into an ordered set
	for(const ENUM_SERVICE_STATUS& service : std::span<ENUM_SERVICE_STATUS>(service_status, services_returned)){
		if(service.ServiceStatus.dwCurrentState == SERVICE_RUNNING) {
			tmp = std::string(service.lpDisplayName);
			services_running_ordered.emplace(std::wstring(tmp.begin(), tmp.end()));
		}
	}

	if(services_running_ordered.size() > 0)
		return std::wcout << i18n::SERVICES << std::endl << services_running_ordered;
	return std::wcout;
}
