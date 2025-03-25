module;

#include <iostream>
#include <sstream>
#include <memory>
#include <string>

#include <Windows.h>
#include <WbemIdl.h>
#include <comdef.h>

#pragma comment(lib, "wbemuuid.lib")

module core;

import common;

import i18n;
import i18n_system;

// Defines a custom deleter for smart pointers managing COM-like objects
template <typename T> struct releaser{ void operator()(T* ptr) const{if(ptr){ptr->Release();}}};

// Helper class to manage Variant objects
class VariantWrapper{
public:
	VariantWrapper()     { VariantInit(&var);  }
	~VariantWrapper()    { VariantClear(&var); }
	VARIANT* operator&() { return &var;        }
	VARIANT var;
};

// Function to display memory information
std::wostream& memory() noexcept{
	MEMORYSTATUSEX memory_state;

	ULONG return_result = 0;

	IWbemClassObject     *clsobj_pointer     = nullptr;
	IEnumWbemClassObject *enumerator_pointer = nullptr;
	IWbemLocator         *locator_pointer    = nullptr;
	IWbemServices        *svc_pointer        = nullptr;

	// Initializes memory status structure
	memory_state.dwLength = sizeof(memory_state);
	if(!GlobalMemoryStatusEx(&memory_state)) return std::wcerr << i18n::ERROR_MEMORY << L"\n\n";

	// Initializes COM and creates a smart pointer to CoUninitialize
	if(FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED)))
		return std::wcerr << i18n_system::ERROR_MEMORY_COM_INIT << L"\n\n";

	std::unique_ptr<void, decltype([](void*){ CoUninitialize(); })> result_handle_ptr(reinterpret_cast<void*>(1));

	// Initializes WMI and creates a smart pointer to Release
	if(FAILED(CoCreateInstance(CLSID_WbemLocator, nullptr, CLSCTX_INPROC_SERVER, IID_IWbemLocator,
		reinterpret_cast<LPVOID*>(&locator_pointer))))
			return std::wcerr << i18n_system::ERROR_MEMORY_WMI_INIT << L"\n\n";

	std::unique_ptr<IWbemLocator, releaser<IWbemLocator>> locator_pointer_ptr(locator_pointer);

	// Connects to WMI and creates a smart pointer to Release
	if(FAILED(locator_pointer->ConnectServer(_bstr_t(L"ROOT\\CIMV2"), nullptr, nullptr, nullptr, 0, nullptr, nullptr,
		&svc_pointer))) return std::wcerr << i18n_system::ERROR_MEMORY_WMI_CONNECT << L"\n\n";

	std::unique_ptr<IWbemServices, releaser<IWbemServices>> svc_pointer_ptr(svc_pointer);

	// Configures security settings for WMI connection
	if(FAILED(CoSetProxyBlanket(svc_pointer, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, nullptr, RPC_C_AUTHN_LEVEL_CALL,
		RPC_C_IMP_LEVEL_IMPERSONATE, nullptr, EOAC_NONE)))
			return std::wcerr << i18n_system::ERROR_MEMORY_SECURITY_LEVEL << L"\n\n";

	// Queries data and creates a smart pointer to Release
	if(FAILED(svc_pointer->ExecQuery(bstr_t("WQL"), bstr_t(L"SELECT * FROM Win32_PhysicalMemory"),
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, nullptr, &enumerator_pointer)))
			return std::wcerr << i18n_system::ERROR_MEMORY_QUERY << L"\n\n";

	std::unique_ptr<IEnumWbemClassObject, releaser<IEnumWbemClassObject>> enumerator_pointer_ptr(enumerator_pointer);

	// Loops through all physical memory modules
	for(std::wcout << i18n::MEMORY << L'\n'; enumerator_pointer; std::wcout << L'\n'){
		VariantWrapper manufacturer_property, capacity_property, speed_property, type_property;

		// Initializes class object and creates a smart pointer to Release
		if(FAILED(enumerator_pointer->Next(WBEM_INFINITE, 1, &clsobj_pointer, &return_result)) || (!return_result)) break;

		std::unique_ptr<IWbemClassObject, releaser<IWbemClassObject>> class_object_ptr(clsobj_pointer);

		// Gets and displays manufacturer
		if(SUCCEEDED(clsobj_pointer->Get(L"Manufacturer", 0, &manufacturer_property, nullptr, nullptr)) &&
			(manufacturer_property.var.vt == VT_BSTR))
				std::wcout << L'\t' << i18n::MEMORY_MANUFACTURER << L' ' << manufacturer_property.var.bstrVal << L'\n';

		// Gets and displays capacity in MB
		if(SUCCEEDED(clsobj_pointer->Get(L"Capacity", 0, &capacity_property, nullptr, nullptr)) &&
			(capacity_property.var.vt == VT_BSTR)) std::wcout << L'\t' << i18n::MEMORY_CAPACITY << L' '
				<< std::stoull(capacity_property.var.bstrVal) / 1048576 << L" MB\n";

		// Gets and displays speed in MHz
		if(SUCCEEDED(clsobj_pointer->Get(L"Speed", 0, &speed_property, nullptr, nullptr)))
			std::wcout << L'\t' << i18n::MEMORY_SPEED << L' ' << speed_property.var.ulVal << " MHz\n";

		// Gets and displays memory type
		if(SUCCEEDED(clsobj_pointer->Get(L"MemoryType", 0, &type_property, nullptr, nullptr))){
			std::wcout << L'\t' << i18n::MEMORY_TYPE << L' ';
			switch(static_cast<unsigned int>(type_property.var.uiVal)){
				case 20: std::wcout << "DDR"     << L'\n'; break;
				case 21: std::wcout << "DDR2"    << L'\n'; break;
				case 24: std::wcout << "DDR3"    << L'\n'; break;
				case 26: std::wcout << "DDR4"    << L'\n'; break;
				default: std::wcout << "Unknown" << L'\n'; break;
			}
		}
	}

	// Displays total memory statistics
	return print(1, 1, L' ',
		i18n::MEMORY_TOTAL, std::to_wstring(memory_state.ullTotalPhys / 1048576) + L" MB",
		i18n::MEMORY_USED, std::to_wstring((memory_state.ullTotalPhys - memory_state.ullAvailPhys)/1048576) + L" MB",
		i18n::MEMORY_AVAILABLE, std::to_wstring(memory_state.ullAvailPhys / 1048576) + L" MB");
}
