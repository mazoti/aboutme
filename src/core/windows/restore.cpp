module;

#include <iostream>
#include <sstream>
#include <memory>
#include <vector>

#include <Windows.h>
#include <WbemIdl.h>
#include <comdef.h>

#pragma comment(lib, "wbemuuid.lib")

module core;

import common;

import i18n;
import i18n_system;

template <typename T> struct releaser{ void operator()(T* ptr) const{if(ptr){ptr->Release();}}};

static bool couninitialize_released_restore = false;

std::wostream& restore() noexcept{
	VARIANT variant_property;

	std::wstring tmp;
	std::wostringstream woss;
	std::vector<std::wstring> restore_points_ordered;

	IWbemClassObject *clsobj_pointer = nullptr;
	IWbemLocator *locator_pointer = nullptr;
	IWbemServices *svc_pointer = nullptr;
	IEnumWbemClassObject *enumerator_pointer = nullptr;

	ULONG return_result = 0;

	// Initialize COM and creates a smart pointer to CoUninitialize
	if(FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED)))
		return std::wcerr << i18n_system::ERROR_RESTORE_COM_INIT << std::endl << std::endl;

	std::unique_ptr<void, decltype([](void*){
		if(couninitialize_released_restore) return;
		couninitialize_released_restore = true;
		CoUninitialize();
	})> result_handle_ptr(reinterpret_cast<void*>(1));

	// Initialize WMI and creates a smart pointer to Release
	if(FAILED(CoCreateInstance(CLSID_WbemLocator, nullptr, CLSCTX_INPROC_SERVER, IID_IWbemLocator,
		reinterpret_cast<LPVOID*>(&locator_pointer))))
		return std::wcerr << i18n_system::ERROR_RESTORE_WMI_INIT << std::endl << std::endl;

	std::unique_ptr<IWbemLocator, releaser<IWbemLocator> > locator_pointer_ptr(locator_pointer);

	// Connect to WMI and creates a smart pointer to Release
	if(FAILED(locator_pointer->ConnectServer(_bstr_t(L"ROOT\\DEFAULT"), nullptr, nullptr, nullptr, 0, nullptr, nullptr,
		&svc_pointer))) return std::wcerr << i18n_system::ERROR_RESTORE_WMI_CONNECT << std::endl << std::endl;
	std::unique_ptr<IWbemServices, releaser<IWbemServices> > svc_pointer_ptr(svc_pointer);

	// Set security levels
	if(FAILED(CoSetProxyBlanket(svc_pointer, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, nullptr, RPC_C_AUTHN_LEVEL_CALL,
		RPC_C_IMP_LEVEL_IMPERSONATE, nullptr, EOAC_NONE)))
		return std::wcerr << i18n_system::ERROR_RESTORE_SECURITY_LEVEL << std::endl << std::endl;

	// Query data and creates a smart pointer to Release
	if(FAILED(svc_pointer->ExecQuery(bstr_t("WQL"), bstr_t(L"SELECT CreationTime, Description FROM SystemRestore"),
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, nullptr, &enumerator_pointer)))
		return std::wcerr << i18n_system::ERROR_RESTORE_QUERY << std::endl << std::endl;
	std::unique_ptr<IEnumWbemClassObject, releaser<IEnumWbemClassObject> > enumerator_pointer_ptr(enumerator_pointer);

	while(enumerator_pointer){
		enumerator_pointer->Next(WBEM_INFINITE, 1, &clsobj_pointer, &return_result);
		if(!return_result) break;
		std::unique_ptr<IWbemClassObject, releaser<IWbemClassObject> > class_object_ptr(clsobj_pointer);

		woss.str(L"");
		if(FAILED(clsobj_pointer->Get(L"CreationTime", 0, &variant_property, nullptr, nullptr))) continue;
		if(variant_property.vt == VT_BSTR){
			woss << variant_property.bstrVal;
			VariantClear(&variant_property);
		}

		// Format date and time
		tmp = woss.str();
		tmp = tmp.substr(0,4) + L'-' + tmp.substr(4,2) + L'-' + tmp.substr(6,2) + L' ' + tmp.substr(8,2) + L':'
			+ tmp.substr(10,2) + L':' + tmp.substr(12,2);

		woss.str(L"");
		if(FAILED(clsobj_pointer->Get(L"Description", 0, &variant_property, nullptr, nullptr))) continue;
		if(variant_property.vt == VT_BSTR){
			woss << variant_property.bstrVal;
			VariantClear(&variant_property);
		}

		// Insert in a vector to print in reverse order
		restore_points_ordered.emplace(restore_points_ordered.begin(), tmp + L" - " + woss.str());
	}

	if(restore_points_ordered.size() > 0){
		restore_points_ordered.size() == 1 ? std::wcout << i18n::RESTORE_POINT : std::wcout << i18n::RESTORE_POINTS;
		return std::wcout << std::endl << restore_points_ordered;
	}
	return std::wcout << i18n::NO_RESTORE_POINTS << std::endl << std::endl;
}
