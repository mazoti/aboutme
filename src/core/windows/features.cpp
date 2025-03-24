module;

#include <iostream>
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

// Retrieves and displays installed Windows optional features using WMI
std::wostream& features() noexcept{
	IWbemLocator* locator_pointer = nullptr;
	IWbemServices* svc_pointer = nullptr;
	IEnumWbemClassObject* enumerator_pointer = nullptr;
	IWbemClassObject* clsobj_pointer = nullptr;
	ULONG return_result = 0;

	std::set<std::wstring> features_ordered;

	// Initializes COM and creates a smart pointer to CoUninitialize
	if(FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED)))
		return std::wcerr << i18n_system::ERROR_FEATURES_COM_INIT << L"\n\n";

	std::unique_ptr<void, decltype([](void*){ CoUninitialize(); })> couninit_ptr(reinterpret_cast<void*>(1));

	// Initializes WMI and creates a smart pointer to release resources
	if(FAILED(CoCreateInstance(CLSID_WbemLocator, nullptr, CLSCTX_INPROC_SERVER, IID_IWbemLocator,
		reinterpret_cast<LPVOID*>(&locator_pointer))))
		return std::wcerr << i18n_system::ERROR_FEATURES_WMI_INIT << L"\n\n";

	std::unique_ptr<IWbemLocator, releaser<IWbemLocator>> locator_pointer_ptr(locator_pointer);

	// Connects to WMI and creates a smart pointer to Release
	if(FAILED(locator_pointer->ConnectServer(_bstr_t(L"ROOT\\CIMV2"), nullptr, nullptr, nullptr, 0, nullptr, nullptr,
		&svc_pointer))) return std::wcerr << i18n_system::ERROR_FEATURES_WMI_CONNECT << L"\n\n";

	std::unique_ptr<IWbemServices, releaser<IWbemServices>> svc_pointer_ptr(svc_pointer);

	// Sets security levels on the WMI proxy to allow impersonation
	if(FAILED(CoSetProxyBlanket(svc_pointer, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, nullptr, RPC_C_AUTHN_LEVEL_CALL,
		RPC_C_IMP_LEVEL_IMPERSONATE, nullptr, EOAC_NONE)))
		return std::wcerr << i18n_system::ERROR_FEATURES_SECURITY_LEVEL << L"\n\n";

	// Executes a WQL query to get installed optional features
	// Uses forward-only enumeration for efficiency
	if(FAILED(svc_pointer->ExecQuery(bstr_t("WQL"),
		bstr_t(L"SELECT * FROM Win32_OptionalFeature WHERE InstallState = 1"),
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, nullptr, &enumerator_pointer)))
		return std::wcerr << i18n_system::ERROR_FEATURES_QUERY << L"\n\n";

	std::unique_ptr<IEnumWbemClassObject, releaser<IEnumWbemClassObject>> enumerator_pointer_ptr(enumerator_pointer);

	// Iterates through query results
	while(enumerator_pointer){
		VariantWrapper variant_property;

		// Initializes class object and creates a smart pointer to release resources
		enumerator_pointer->Next(WBEM_INFINITE, 1, &clsobj_pointer, &return_result);
		if(!return_result) break;

		std::unique_ptr<IWbemClassObject, releaser<IWbemClassObject>> class_object_ptr(clsobj_pointer);

		// Retrieves the "Caption" property from the current object
		if((SUCCEEDED(clsobj_pointer->Get(L"Caption", 0, &variant_property, nullptr, nullptr))) &&
		(variant_property.var.vt == VT_BSTR)) features_ordered.emplace(variant_property.var.bstrVal);
	}

	if(!features_ordered.empty()){
		std::wcout << i18n::FEATURES << L'\n';
		for(const std::wstring& data : features_ordered) std::wcout << L'\t' << data << L'\n';
		return std::wcout << L'\n';
	}
	return std::wcout;
}
