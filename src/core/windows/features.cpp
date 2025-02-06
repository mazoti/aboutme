module;

#include <iostream>
#include <sstream>
#include <memory>
#include <set>

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

struct couninitializer_features{
	void operator()(void*) const{
		if(couninitialize_released) return;
		couninitialize_released = true;
		CoUninitialize();
	}
};

std::wostream& features() noexcept{
	std::wostringstream woss;
	std::set<std::wstring> features_ordered;
	VARIANT variant_property;

	IWbemLocator* locator_pointer = nullptr;
	IWbemServices* svc_pointer = nullptr;
	IEnumWbemClassObject* enumerator_pointer = nullptr;
	IWbemClassObject* clsobj_pointer = nullptr;
	ULONG return_result = 0;

	// Initialize COM and creates a smart pointer to CoUninitialize
	if(FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED)))
		return std::wcerr << ERROR_FEATURES_COM_INIT << std::endl << std::endl;
	std::unique_ptr<void, couninitializer_features> result_handle_ptr(reinterpret_cast<void*>(1));

	// Initialize WMI and creates a smart pointer to Release
	if(FAILED(CoCreateInstance(CLSID_WbemLocator, nullptr, CLSCTX_INPROC_SERVER, IID_IWbemLocator,
		reinterpret_cast<LPVOID*>(&locator_pointer))))
		return std::wcerr << ERROR_FEATURES_WMI_INIT << std::endl << std::endl;
	std::unique_ptr<IWbemLocator, releaser<IWbemLocator> > locator_pointer_ptr(locator_pointer);

	// Connect to WMI and creates a smart pointer to Release
	if(FAILED(locator_pointer->ConnectServer(_bstr_t(L"ROOT\\CIMV2"), nullptr, nullptr, nullptr, 0, nullptr, nullptr,
		&svc_pointer))) return std::wcerr << ERROR_FEATURES_WMI_CONNECT << std::endl << std::endl;
	std::unique_ptr<IWbemServices, releaser<IWbemServices> > svc_pointer_ptr(svc_pointer);

	// Set security levels
	if(FAILED(CoSetProxyBlanket(svc_pointer, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, nullptr, RPC_C_AUTHN_LEVEL_CALL,
		RPC_C_IMP_LEVEL_IMPERSONATE, nullptr, EOAC_NONE)))
		return std::wcerr << ERROR_FEATURES_SECURITY_LEVEL << std::endl << std::endl;

	// Query video details and creates a smart pointer to Release
	if(FAILED(svc_pointer->ExecQuery(bstr_t("WQL"),
		bstr_t(L"SELECT * FROM Win32_OptionalFeature WHERE InstallState = 1"),
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, nullptr, &enumerator_pointer)))
		return std::wcerr << ERROR_FEATURES_QUERY << std::endl << std::endl;
	std::unique_ptr<IEnumWbemClassObject, releaser<IEnumWbemClassObject> > enumerator_pointer_ptr(enumerator_pointer);

	while(enumerator_pointer){
		// Initialize class object and creates a smart pointer to Release
		enumerator_pointer->Next(WBEM_INFINITE, 1, &clsobj_pointer, &return_result);
		if(!return_result) break;
		std::unique_ptr<IWbemClassObject, releaser<IWbemClassObject> > class_object_ptr(clsobj_pointer);

		if(SUCCEEDED(clsobj_pointer->Get(L"Caption", 0, &variant_property, nullptr, nullptr))){
			if(variant_property.vt == VT_BSTR){
				woss << variant_property.bstrVal;
				features_ordered.emplace(woss.str());
				woss.str(L"");
				VariantClear(&variant_property);
			}
		}
	}

	if(features_ordered.size() > 0) return std::wcout << FEATURES << std::endl << features_ordered;
	return std::wcout;
}
