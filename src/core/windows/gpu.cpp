// This code module provides functionality to query and display detailed information about installed GPU(s) on a
// Windows system using WMI (Windows Management Instrumentation)

module;

#include <iostream>
#include <memory>

#include <Windows.h>
#include <comdef.h>
#include <WbemIdl.h>

#pragma comment(lib, "wbemuuid.lib")

module core;

import common;

import i18n;
import i18n_system;

// Function to query and display GPU details
std::wostream& gpu() noexcept{
	size_t width;
	VARIANT variant_property;

	ULONG return_result = 0;
	IWbemClassObject* clsobj_pointer = nullptr;
	IEnumWbemClassObject* enumerator_pointer = nullptr;
	IWbemLocator* locator_pointer = nullptr;
	IWbemServices* svc_pointer = nullptr;

	// Initializes COM and creates a smart pointer to CoUninitialize
	if(FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED)))
		return std::wcerr << i18n_system::ERROR_COM_INIT << std::endl << std::endl;

	std::unique_ptr<void, decltype([](void*){
		CoUninitialize();
	})> couninit_ptr(reinterpret_cast<void*>(1));

	// Initializes WMI and creates a smart pointer to release resources
	if(FAILED(CoCreateInstance(CLSID_WbemLocator, nullptr, CLSCTX_INPROC_SERVER, IID_IWbemLocator,
		reinterpret_cast<LPVOID*>(&locator_pointer))))
		return std::wcerr << i18n_system::ERROR_WMI_INIT << std::endl << std::endl;
	std::unique_ptr<IWbemLocator, releaser<IWbemLocator> > locator_pointer_ptr(locator_pointer);

	// Connect to WMI and creates a smart pointer to release resources
	if(FAILED(locator_pointer->ConnectServer(_bstr_t(L"ROOT\\CIMV2"), nullptr, nullptr, nullptr, 0, nullptr, nullptr,
		&svc_pointer))) return std::wcerr << i18n_system::ERROR_WMI_CONNECT << std::endl << std::endl;
	std::unique_ptr<IWbemServices, releaser<IWbemServices> > svc_pointer_ptr(svc_pointer);

	// Configures security settings for WMI connection
	if(FAILED(CoSetProxyBlanket(svc_pointer, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, nullptr, RPC_C_AUTHN_LEVEL_CALL,
		RPC_C_IMP_LEVEL_IMPERSONATE, nullptr, EOAC_NONE)))
			return std::wcerr << i18n_system::ERROR_SECURITY_LEVEL << std::endl << std::endl;

	// Executes WQL query to get video controller information
	if(FAILED(svc_pointer->ExecQuery(bstr_t("WQL"), bstr_t(L"SELECT * FROM Win32_VideoController"), 
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, nullptr, &enumerator_pointer)))
			return std::wcerr << i18n_system::ERROR_QUERY << std::endl << std::endl;
	std::unique_ptr<IEnumWbemClassObject, releaser<IEnumWbemClassObject> > enumerator_pointer_ptr(enumerator_pointer);

	// Main loop to process all GPU devices
	for(std::wcout << i18n::GPU << std::endl; enumerator_pointer;){
		// Initialize class object and creates a smart pointer to Release
		enumerator_pointer->Next(WBEM_INFINITE, 1, &clsobj_pointer, &return_result);
		if(!return_result) break;
		std::unique_ptr<IWbemClassObject, releaser<IWbemClassObject> > class_object_ptr(clsobj_pointer);

		// Gets and displays GPU name
		if(SUCCEEDED(clsobj_pointer->Get(L"Name", 0, &variant_property, nullptr, nullptr))){
			std::wcout << L'\t' << variant_property.bstrVal << std::endl;
			VariantClear(&variant_property);
		}

		// Gets and displays driver version
		if(SUCCEEDED(clsobj_pointer->Get(L"DriverVersion", 0, &variant_property, nullptr, nullptr))){
			std::wcout << L"\tDriver: " << variant_property.bstrVal << std::endl;
			VariantClear(&variant_property);
		}

		// Gets and displays resolution (horizontal x vertical)
		if(SUCCEEDED(clsobj_pointer->Get(L"CurrentHorizontalResolution", 0, &variant_property, nullptr, nullptr))){
			width = variant_property.uintVal;
			VariantClear(&variant_property);

			if(SUCCEEDED(clsobj_pointer->Get(L"CurrentVerticalResolution", 0, &variant_property, nullptr, nullptr))){
				std::wcout << L'\t' << i18n::VIDEO_RESOLUTION << L' ' << width
				<< "x" << variant_property.uintVal << std::endl;
				VariantClear(&variant_property);
			}
		}

		// Gets and displays GPU memory in MB
		if(SUCCEEDED(clsobj_pointer->Get(L"AdapterRAM", 0, &variant_property, nullptr, nullptr))){
			std::wcout << L'\t' << i18n::GPU_MEMORY << L' ' << (variant_property.ullVal / 1048576) << L" MB" << std::endl;
			VariantClear(&variant_property);
		}
	}

	return std::wcout << std::endl;
}
