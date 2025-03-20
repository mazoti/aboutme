module;

#include <iostream>
#include <format>
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

// Helper class to manage VARIANT objects
class VariantWrapper{
public:
	VariantWrapper()     { VariantInit(&var);  }
	~VariantWrapper()    { VariantClear(&var); }
	VARIANT* operator&() { return &var;        }
	VARIANT var;
};

// Retrieves and displays system restore points
std::wostream& restore() noexcept {
	IWbemLocator* locator = nullptr;
	IWbemServices* service = nullptr;
	IEnumWbemClassObject* enumerator = nullptr;
	IWbemClassObject* class_object = nullptr;
	ULONG return_result = 0;

	std::wstring time_str;
	std::vector<std::wstring> restore_points;

	// Initializes COM
	if(FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED)))
		return std::wcerr << i18n_system::ERROR_RESTORE_COM_INIT << std::endl << std::endl;

	std::unique_ptr<void, decltype([](void*) { CoUninitialize(); })> com_guard(reinterpret_cast<void*>(1));

	// Creates WMI locator
	if(FAILED(CoCreateInstance(CLSID_WbemLocator, nullptr, CLSCTX_INPROC_SERVER, IID_IWbemLocator,
	reinterpret_cast<LPVOID*>(&locator))))
		return std::wcerr << i18n_system::ERROR_RESTORE_WMI_INIT << std::endl << std::endl;

	std::unique_ptr<IWbemLocator, releaser<IWbemLocator>> locator_ptr(locator);

	// Connects to WMI namespace ROOT\DEFAULT
	if(FAILED(locator->ConnectServer(_bstr_t(L"ROOT\\DEFAULT"), nullptr, nullptr, nullptr, 0, nullptr,
	nullptr, &service)))
		return std::wcerr << i18n_system::ERROR_RESTORE_WMI_CONNECT << std::endl << std::endl;

	std::unique_ptr<IWbemServices, releaser<IWbemServices>> service_ptr(service);

	// Sets security levels for the WMI proxy to allow impersonation
	if(FAILED(CoSetProxyBlanket(service, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, nullptr, RPC_C_AUTHN_LEVEL_CALL,
	RPC_C_IMP_LEVEL_IMPERSONATE, nullptr, EOAC_NONE)))
		return std::wcerr << i18n_system::ERROR_RESTORE_SECURITY_LEVEL << std::endl << std::endl;

	// Executes WQL query to retrieve system restore points
	if(FAILED(service->ExecQuery(bstr_t("WQL"), bstr_t(L"SELECT CreationTime, Description FROM SystemRestore"),
	WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, nullptr, &enumerator)))
		return std::wcerr << i18n_system::ERROR_RESTORE_QUERY << std::endl << std::endl;

	std::unique_ptr<IEnumWbemClassObject, releaser<IEnumWbemClassObject>> enumerator_ptr(enumerator);

	// Iterates through query results
	while(enumerator){
		// Gets CreationTime property
		VariantWrapper creation_time, description;

		if(FAILED(enumerator->Next(WBEM_INFINITE, 1, &class_object, &return_result)) || return_result == 0) break;

		std::unique_ptr<IWbemClassObject, releaser<IWbemClassObject>> class_object_ptr(class_object);

		if(FAILED(class_object->Get(L"CreationTime", 0, &creation_time, nullptr, nullptr)) ||
			creation_time.var.vt != VT_BSTR) continue;

		// Ensures valid length
		time_str = creation_time.var.bstrVal;
		if(time_str.length() < 14) continue;

		// Gets Description property
		if(FAILED(class_object->Get(L"Description", 0, &description, nullptr, nullptr)) ||
			description.var.vt != VT_BSTR) continue;

		// Stores restore point
		restore_points.push_back(std::format(L"{} - {}", std::format(L"{}-{}-{} {}:{}:{}", time_str.substr(0, 4),
			time_str.substr(4, 2), time_str.substr(6, 2), time_str.substr(8, 2), time_str.substr(10, 2),
			time_str.substr(12, 2)), description.var.bstrVal));
	}

	if(restore_points.size() > 0){
		// Reverses to chronological order
		if(restore_points.size() > 1) std::reverse(restore_points.begin(), restore_points.end());

		// Uses singular or plural form of "restore point(s)" based on count
		restore_points.size() == 1 ? std::wcout << i18n::RESTORE_POINT : std::wcout << i18n::RESTORE_POINTS;
		return std::wcout << std::endl << restore_points;
	}
	return std::wcout << i18n::NO_RESTORE_POINTS << std::endl << std::endl;
}
