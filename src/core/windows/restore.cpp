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

// Retrieves and displays system restore points
std::wostream& restore() noexcept {
	IWbemLocator              *locator = nullptr;
	IWbemServices             *service = nullptr;
	IEnumWbemClassObject   *enumerator = nullptr;
	IWbemClassObject     *class_object = nullptr;

	ULONG return_result = 0;

	std::wstring time_str;
	std::vector<std::wstring> restore_points;

	// Initializes COM
	if(FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED)))
		return std::wcerr << i18n_system::ERROR_RESTORE_COM_INIT << L"\n\n";

	std::unique_ptr<void, decltype([](void*){ CoUninitialize(); })> com_guard(reinterpret_cast<void*>(1));

	// Creates WMI locator
	if(FAILED(CoCreateInstance(CLSID_WbemLocator, nullptr, CLSCTX_INPROC_SERVER, IID_IWbemLocator,
	reinterpret_cast<LPVOID*>(&locator))))
		return std::wcerr << i18n_system::ERROR_RESTORE_WMI_INIT << L"\n\n";

	std::unique_ptr<IWbemLocator, releaser<IWbemLocator>> locator_ptr(locator);

	// Connects to WMI namespace ROOT\DEFAULT
	if(FAILED(locator->ConnectServer(_bstr_t(L"ROOT\\DEFAULT"), nullptr, nullptr, nullptr, 0, nullptr,
	nullptr, &service)))
		return std::wcerr << i18n_system::ERROR_RESTORE_WMI_CONNECT << L"\n\n";

	std::unique_ptr<IWbemServices, releaser<IWbemServices>> service_ptr(service);

	// Sets security levels for the WMI proxy to allow impersonation
	if(FAILED(CoSetProxyBlanket(service, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, nullptr, RPC_C_AUTHN_LEVEL_CALL,
	RPC_C_IMP_LEVEL_IMPERSONATE, nullptr, EOAC_NONE)))
		return std::wcerr << i18n_system::ERROR_RESTORE_SECURITY_LEVEL << L"\n\n";

	// Executes WQL query to retrieve system restore points
	if(FAILED(service->ExecQuery(bstr_t("WQL"),
	bstr_t(L"SELECT CreationTime, Description FROM SystemRestore"),
	WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, nullptr, &enumerator)))
		return std::wcerr << i18n_system::ERROR_RESTORE_QUERY << L"\n\n";

	std::unique_ptr<IEnumWbemClassObject, releaser<IEnumWbemClassObject>> enumerator_ptr(enumerator);

	// Iterates through query results
	while(enumerator){
		// Gets CreationTime property
		VariantWrapper creation_time, description;

		if(FAILED(enumerator->Next(WBEM_INFINITE, 1, &class_object, &return_result)) || !return_result) break;

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
		restore_points.push_back(std::format(L"{} - {}", std::format(L"{}-{}-{} {}:{}:{}",
			time_str.substr(0,   4),
			time_str.substr(4,   2),
			time_str.substr(6,   2),
			time_str.substr(8,   2),
			time_str.substr(10,  2),
			time_str.substr(12, 2)),
			(description.var.bstrVal ? description.var.bstrVal : L"")));
	}

	if(restore_points.size() > 0){
		// Uses singular or plural form of "restore point(s)" based on count
		restore_points.size() == 1 ?
			std::wcout << i18n::RESTORE_POINT  << L'\n':
			std::wcout << i18n::RESTORE_POINTS << L'\n';

		for(const std::wstring& data : restore_points) std::wcout << L'\t' << data << L'\n';
		return std::wcout << L'\n';
	}

	return std::wcout << i18n::NO_RESTORE_POINTS << L"\n\n";
}
