module;

#include <iostream>
#include <sstream>
#include <memory>

#include <Windows.h>
#include <WbemIdl.h>
#include <comdef.h>

#pragma comment(lib, "wbemuuid.lib")

module core;

import common;

import i18n;
import i18n_system;

template <typename T> struct releaser{ void operator()(T* ptr) const{if(ptr){ptr->Release();}}};

static bool couninitialize_released = false;

struct couninitializer_memory{
	void operator()(void*) const{
		if(couninitialize_released) return;
		couninitialize_released = true;
		CoUninitialize();
	}
};

std::wostream& memory() noexcept{
	MEMORYSTATUSEX memory_state;
	VARIANT variant_property;

	ULONG return_result = 0;
	IWbemClassObject* clsobj_pointer = nullptr;
	IEnumWbemClassObject* enumerator_pointer = nullptr;
	IWbemLocator* locator_pointer = nullptr;
	IWbemServices* svc_pointer = nullptr;

	memory_state.dwLength = sizeof(memory_state);
	if(!GlobalMemoryStatusEx(&memory_state)) return std::wcerr << i18n::ERROR_MEMORY << std::endl << std::endl;

	// Initialize COM and creates a smart pointer to CoUninitialize
	if(FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED)))
		return std::wcerr << i18n_system::ERROR_MEMORY_COM_INIT << std::endl << std::endl;
	std::unique_ptr<void, couninitializer_memory> result_handle_ptr(reinterpret_cast<void*>(1));

	// Initialize WMI and creates a smart pointer to Release
	if(FAILED(CoCreateInstance(CLSID_WbemLocator, nullptr, CLSCTX_INPROC_SERVER, IID_IWbemLocator,
		reinterpret_cast<LPVOID*>(&locator_pointer))))
			return std::wcerr << i18n_system::ERROR_MEMORY_WMI_INIT << std::endl << std::endl;
	std::unique_ptr<IWbemLocator, releaser<IWbemLocator> > locator_pointer_ptr(locator_pointer);

	// Connect to WMI and creates a smart pointer to Release
	if(FAILED(locator_pointer->ConnectServer(_bstr_t(L"ROOT\\CIMV2"), nullptr, nullptr, nullptr, 0, nullptr, nullptr,
		&svc_pointer))) return std::wcerr << i18n_system::ERROR_MEMORY_WMI_CONNECT << std::endl << std::endl;
	std::unique_ptr<IWbemServices, releaser<IWbemServices> > svc_pointer_ptr(svc_pointer);

	// Set security levels
	if(FAILED(CoSetProxyBlanket(svc_pointer, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, nullptr, RPC_C_AUTHN_LEVEL_CALL,
		RPC_C_IMP_LEVEL_IMPERSONATE, nullptr, EOAC_NONE)))
			return std::wcerr << i18n_system::ERROR_MEMORY_SECURITY_LEVEL << std::endl << std::endl;

	// Query data and creates a smart pointer to Release
	if(FAILED(svc_pointer->ExecQuery(bstr_t("WQL"), bstr_t(L"SELECT * FROM Win32_PhysicalMemory"),
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, nullptr, &enumerator_pointer)))
			return std::wcerr << i18n_system::ERROR_MEMORY_QUERY << std::endl << std::endl;
	std::unique_ptr<IEnumWbemClassObject, releaser<IEnumWbemClassObject> > enumerator_pointer_ptr(enumerator_pointer);

	for(std::wcout << i18n::MEMORY << std::endl; enumerator_pointer;){
		// Initialize class object and creates a smart pointer to Release
		enumerator_pointer->Next(WBEM_INFINITE, 1, &clsobj_pointer, &return_result);
		if(!return_result) break;
		std::unique_ptr<IWbemClassObject, releaser<IWbemClassObject> > class_object_ptr(clsobj_pointer);

		if(SUCCEEDED(clsobj_pointer->Get(L"Manufacturer", 0, &variant_property, nullptr, nullptr))){
			if(variant_property.vt == VT_BSTR){
				std::wcout << L'\t' << i18n::MEMORY_MANUFACTURER << L' ' << variant_property.bstrVal << std::endl;
				VariantClear(&variant_property);
			}
		}

		if(SUCCEEDED(clsobj_pointer->Get(L"Capacity", 0, &variant_property, nullptr, nullptr))){
			if(variant_property.vt == VT_BSTR){
				std::wcout << L'\t' << i18n::MEMORY_CAPACITY << L' ' << std::stoull(variant_property.bstrVal) / 1048576
				<< " MB" << std::endl;
				VariantClear(&variant_property);
			}
		}

		if(SUCCEEDED(clsobj_pointer->Get(L"Speed", 0, &variant_property, nullptr, nullptr))){
			std::wcout << L'\t' << i18n::MEMORY_SPEED << L' ' << variant_property.uintVal << " MHz"<< std::endl;
			VariantClear(&variant_property);
		}

		if(SUCCEEDED(clsobj_pointer->Get(L"MemoryType", 0, &variant_property, nullptr, nullptr))){
			std::wcout << L'\t' << i18n::MEMORY_TYPE << L' ';
			switch(static_cast<unsigned int>(variant_property.uintVal)){
				case 20: std::wcout << "DDR"  << std::endl; break;
				case 21: std::wcout << "DDR2" << std::endl; break;
				case 24: std::wcout << "DDR3" << std::endl; break;
				case 26: std::wcout << "DDR4" << std::endl; break;
				default: std::wcout << "Unknown" << std::endl; break;
			}
			VariantClear(&variant_property);
		}

		std::wcout << std::endl;
	}

	return print(1, 1, L' ',
		i18n::MEMORY_TOTAL,     std::to_wstring( memory_state.ullTotalPhys / 1048576) + L" MB",
		i18n::MEMORY_USED,      std::to_wstring((memory_state.ullTotalPhys - memory_state.ullAvailPhys)/1048576) +
			L" MB",
		i18n::MEMORY_AVAILABLE, std::to_wstring( memory_state.ullAvailPhys / 1048576) + L" MB");
}
