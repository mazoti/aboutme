module;

#include <iostream>
#include <memory>
#include <string>
#include <span>
#include <vector>

#define UNICODE
#include <Windows.h>
#include <LM.h>

#pragma comment(lib, "netapi32.lib")

module core;

import i18n;
import i18n_system;

// Lists all shared resources
std::wostream& shared() noexcept{
	NET_API_STATUS status;
	SHARE_INFO_2 *share_info = nullptr;
	TCHAR buffer[MAX_COMPUTERNAME_LENGTH + 1];
	DWORD entries_read = 0, total_entries = 0, resume_handle = 0, buffer_size = sizeof(buffer) / sizeof(buffer[0]);

	std::wstring domain, netname;
	std::vector<std::wstring> shared_vector;

	// Gets the local computer's DNS hostname
	if(!GetComputerNameExW(ComputerNameDnsHostname, buffer, &buffer_size))
		return std::wcerr << i18n_system::ERROR_HOSTNAME << L"\n\n";
	domain = std::wstring(buffer);

	// Enumerates shared resources
	do{

		// Retrieves share information at level 2 (SHARE_INFO_2)
		status = NetShareEnum(nullptr, 2, reinterpret_cast<LPBYTE*>(&share_info), MAX_PREFERRED_LENGTH, &entries_read,
			&total_entries, &resume_handle);

		std::unique_ptr<SHARE_INFO_2, decltype([](SHARE_INFO_2 *share_info_pointer){
			NetApiBufferFree(share_info_pointer); })> share_info_ptr(share_info);

		if((status != NERR_Success) && (status != ERROR_MORE_DATA))
			return std::wcerr << i18n_system::ERROR_ENUM_SHARES << L"\n\n";

		// Iterates over the retrieved SHARE_INFO_2 structures using a span for safe access
		for(SHARE_INFO_2& sh : std::span<SHARE_INFO_2>(share_info, entries_read)){
			netname = sh.shi2_netname;
			// Filters shares: must be non-empty, not hidden (ending with '$') and not special
			if(!netname.empty() && netname.back() != L'$' && !(sh.shi2_type & STYPE_SPECIAL))
				shared_vector.emplace_back(std::format(L"\\\\{}\\{} - {}", domain, sh.shi2_netname, sh.shi2_path));
		}
	}while(status == ERROR_MORE_DATA);

	if(shared_vector.empty()) return std::wcout << i18n::SHARED_EMPTY << L"\n\n";

	std::wcout << i18n::SHARED << L'\n';
	for(const std::wstring& data : shared_vector) std::wcout << L'\t' << data << L'\n';
	return std::wcout << L'\n';
}
