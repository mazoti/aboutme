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

struct service_handle_closer{ void operator()(SC_HANDLE handle) const{ CloseServiceHandle(handle); }};

std::wostream& services() noexcept{
	std::set<std::string> services_running_ordered;

	LPENUM_SERVICE_STATUS service_status;
	DWORD i, bytes_needed = 0, services_returned = 0, resume_handle = 0;

	SC_HANDLE scm_handle = OpenSCManager(nullptr, nullptr, SC_MANAGER_ENUMERATE_SERVICE);
	if(scm_handle == nullptr) return std::wcerr << ERROR_SERVICES << std::endl << std::endl;

	// Wrap the handle in a unique_ptr with the custom deleter
	std::unique_ptr<SC_HANDLE__, service_handle_closer> scm_handle_ptr(scm_handle);

	// First call to get the required buffer size, it will return an error
	EnumServicesStatus(scm_handle, SERVICE_WIN32, SERVICE_STATE_ALL, nullptr, 0,
		&bytes_needed, &services_returned, &resume_handle);

	i = bytes_needed;
	service_status = static_cast<LPENUM_SERVICE_STATUS>(std::malloc(bytes_needed));
	if(!service_status) return std::wcerr << ERROR_SERVICES_MALLOC << std::endl << std::endl;

	// Run free at any exit
	std::unique_ptr<ENUM_SERVICE_STATUS, void(*)(ENUM_SERVICE_STATUS*)> service_status_ptr(
		service_status, [](ENUM_SERVICE_STATUS* ptr){ std::free(ptr); });

	if(!EnumServicesStatus(scm_handle, SERVICE_WIN32, SERVICE_STATE_ALL, service_status, i, 
		&bytes_needed, &services_returned, &resume_handle))
		return std::wcerr << ERROR_SERVICES_ENUM << std::endl << std::endl;

	// Create span for safe access and safely iterate
	for(const ENUM_SERVICE_STATUS& service : std::span<ENUM_SERVICE_STATUS>(service_status, services_returned)){
		if(service.ServiceStatus.dwCurrentState == SERVICE_RUNNING){
			services_running_ordered.emplace(std::string(service.lpDisplayName));
		}
	}

	if(services_running_ordered.size() > 0)
		return std::wcout << i18n::SERVICES << std::endl << services_running_ordered;
	return std::wcout;
}
