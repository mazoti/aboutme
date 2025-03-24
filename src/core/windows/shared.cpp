module;

#include <iostream>
#include <memory>
#include <span>

#include <Windows.h>
#include <LM.h>

#pragma comment(lib, "netapi32.lib")

module core;

std::wostream& shared() noexcept{
	NET_API_STATUS status;

	SHARE_INFO_2 *share_info = nullptr;
	DWORD entriesRead = 0, totalEntries = 0, resumeHandle = 0, userSharesFound = 0;

	do{
		status = NetShareEnum(nullptr, 2, reinterpret_cast<LPBYTE*>(&share_info), MAX_PREFERRED_LENGTH, &entriesRead, &totalEntries, &resumeHandle);
		std::unique_ptr<SHARE_INFO_2, decltype([](SHARE_INFO_2 *share_info_pointer){
			NetApiBufferFree(share_info_pointer);
		})> share_info_ptr(share_info);

		if(status != NERR_Success && status != ERROR_MORE_DATA)
			return std::wcerr << L"Error enumerating shares: " << status << L"\n";

		for(SHARE_INFO_2& sh : std::span<SHARE_INFO_2>(share_info, entriesRead)){
			if(sh.shi2_type == STYPE_DISKTREE && !(sh.shi2_type & STYPE_SPECIAL) && wcscmp(sh.shi2_netname, L"IPC$")){
				std::wcout << std::format(L"Share Name: {}\nPath: {}\nRemark: {}\nMax Users: {}\nCurrent Users: {}\n\n", sh.shi2_netname, sh.shi2_path, sh.shi2_remark, sh.shi2_max_uses, sh.shi2_current_uses);
				++userSharesFound;
			}
		}

	} while(status == ERROR_MORE_DATA);

    if (userSharesFound == 0)
		return std::wcout << L"No user-created shared folders found.\n";

	return std::wcout << L"SHARED=============================" << L'\n';
}
