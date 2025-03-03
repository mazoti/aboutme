module;

#include <iostream>

module core;

std::wostream& shared() noexcept{



	return std::wcout << L"SHARED=============================" << std::endl;
}




//#include <Windows.h>
//#include <lm.h>
//#include <iostream>
//#include <string>
//#include <format>
//
//#pragma comment(lib, "netapi32.lib")
//
//void DisplaySharedFolders() {
//    SHARE_INFO_2* shareInfo;
//    DWORD entriesRead = 0;
//    DWORD totalEntries = 0;
//    DWORD resumeHandle = 0;
//    NET_API_STATUS status;
//    DWORD userSharesFound = 0;
//
//    // Get local computer name
//    wchar_t computerName[MAX_COMPUTERNAME_LENGTH + 1];
//    DWORD nameSize = sizeof(computerName) / sizeof(computerName[0]);
//    GetComputerNameW(computerName, &nameSize);
//
//    std::wcout << L"User-created shared folders on " << computerName << L":\n\n";
//
//    do {
//        status = NetShareEnum(
//            nullptr,
//            2,
//            (LPBYTE*)&shareInfo,
//            MAX_PREFERRED_LENGTH,
//            &entriesRead,
//            &totalEntries,
//            &resumeHandle
//        );
//
//        if (status == NERR_Success || status == ERROR_MORE_DATA) {
//            for (DWORD i = 0; i < entriesRead; i++) {
//                // Skip special/administrative shares and non-disk shares
//                if (shareInfo[i].shi2_type == STYPE_DISKTREE && 
//                    !(shareInfo[i].shi2_type & STYPE_SPECIAL) &&
//                    wcscmp(shareInfo[i].shi2_netname, L"IPC$") != 0) {
//                    
//                    std::wcout << std::format(
//                        L"Share Name: {}\n"
//                        L"Path: {}\n"
//                        L"Remark: {}\n"
//                        L"Max Users: {}\n"
//                        L"Current Users: {}\n\n",
//                        shareInfo[i].shi2_netname,
//                        shareInfo[i].shi2_path,
//                        shareInfo[i].shi2_remark,
//                        shareInfo[i].shi2_max_uses,
//                        shareInfo[i].shi2_current_uses
//                    );
//                    userSharesFound++;
//                }
//            }
//            NetApiBufferFree(shareInfo);
//        }
//        else {
//            std::wcerr << L"Error enumerating shares: " << status << L"\n";
//            break;
//        }
//    } while (status == ERROR_MORE_DATA);
//
//    if (userSharesFound == 0) {
//        std::wcout << L"No user-created shared folders found.\n";
//    }
//}
//
//int main() {
//    SetConsoleOutputCP(CP_UTF8);
//    DisplaySharedFolders();
//    return 0;
//}