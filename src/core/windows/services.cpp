module;

#include <iostream>
#include <memory>
#include <set>
#include <span>

#include <Windows.h>

#pragma comment(lib, "Advapi32.lib")

module core;

import common;

import i18n;
import i18n_system;

// Lists running Windows services in an ordered manner
std::wostream& services() noexcept{
	std::set<std::string> services_running_ordered;

	LPENUM_SERVICE_STATUS service_status;
	DWORD i, bytes_needed = 0, services_returned = 0, resume_handle = 0;

	// Opens a handle to the Service Control Manager (SCM) with enumeration privileges
	SC_HANDLE scm_handle = OpenSCManager(nullptr, nullptr, SC_MANAGER_ENUMERATE_SERVICE);
	if(scm_handle == nullptr) return std::wcerr << i18n_system::ERROR_SERVICES << std::endl << std::endl;

	std::unique_ptr<SC_HANDLE__, decltype([](SC_HANDLE handle){
		CloseServiceHandle(handle);
	})> scm_handle_ptr(scm_handle);	

	// First call to EnumServicesStatus to determine the required buffer size
	// This call fails intentionally (buffer size is 0) but sets bytes_needed
	EnumServicesStatus(scm_handle, SERVICE_WIN32, SERVICE_STATE_ALL, nullptr, 0,
		&bytes_needed, &services_returned, &resume_handle);

	i = bytes_needed;
	service_status = static_cast<LPENUM_SERVICE_STATUS>(std::malloc(bytes_needed));
	if(!service_status) return std::wcerr << i18n_system::ERROR_SERVICES_MALLOC << std::endl << std::endl;

	// Run free at any exit
	std::unique_ptr<ENUM_SERVICE_STATUS, void(*)(ENUM_SERVICE_STATUS*)> service_status_ptr(
		service_status, [](ENUM_SERVICE_STATUS* ptr){ std::free(ptr); });

	// Second call to EnumServicesStatus to actually retrieve the service data
	if(!EnumServicesStatus(scm_handle, SERVICE_WIN32, SERVICE_STATE_ALL, service_status, i,
		&bytes_needed, &services_returned, &resume_handle))
		return std::wcerr << i18n_system::ERROR_SERVICES_ENUM << std::endl << std::endl;

	// Uses std::span to create a safe, bounds-checked view of the service data
	// Iterates over the retrieved services
	for(const ENUM_SERVICE_STATUS& service : std::span<ENUM_SERVICE_STATUS>(service_status, services_returned)){
		if(service.ServiceStatus.dwCurrentState == SERVICE_RUNNING)
			services_running_ordered.emplace(service.lpDisplayName);
	}

	if(services_running_ordered.size() > 0)
		return std::wcout << i18n::SERVICES << std::endl << services_running_ordered;
	return std::wcout;
}
