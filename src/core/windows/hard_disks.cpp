module;

#include <iostream>
#include <sstream>

#include <Windows.h>

module core;

import i18n;
import i18n_system;
import common;

// Displays all hard disks sizes and usages
std::wostream& hard_disks() noexcept{
	std::wostringstream woss;
	size_t total_tmp, used_tmp, available_tmp, largest = 0, total_drivers = 0;
	ULONGLONG all_total, all_used, all_available;
	ULARGE_INTEGER free_bytes, total_bytes, total_free_bytes;

	char drive_letter[4] = {'A', ':', '\\', 0}, drive = 'A';
	DWORD drives = GetLogicalDrives();

	if(!drives)	return std::wcerr << ERROR_HARD_DISK << GetLastError() << std::endl << std::endl;

	// Finds the largest line size and iterates again to print
	for(all_total = all_used = all_available = 0; drive <= 'Z'; ++drive){
		if(drives & 1){
			drive_letter[0] = drive;
			if(!GetDiskFreeSpaceEx(drive_letter, &free_bytes, &total_bytes, &total_free_bytes)){
				std::wcerr << L"\t\t" << ERROR_HARD_DISK_SPACE << drive << ": "
				<< GetLastError() << std::endl << std::endl;
				drives >>= 1;
				continue;
			}

			total_tmp     = (total_bytes.QuadPart      / 1073741824);
			available_tmp = (total_free_bytes.QuadPart / 1073741824);
			used_tmp = total_tmp - available_tmp;

			all_total     += total_tmp;
			all_used      += used_tmp;
			all_available += available_tmp;

			woss.str(L"");
			woss << TOTAL << std::to_wstring(total_tmp) << L" GB";
			if(woss.str().size() > largest) largest = woss.str().size();

			woss.str(L"");
			woss << USED << std::to_wstring(used_tmp) << L" GB";
			if(woss.str().size() > largest) largest = woss.str().size();

			woss.str(L"");
			woss << AVAILABLE << std::to_wstring(available_tmp) << L" GB";
			if(woss.str().size() > largest) largest = woss.str().size();
		}
		drives >>= 1;
	}

	woss.str(L"");
	woss << TOTAL << std::to_wstring(all_total) << L" GB";
	if(woss.str().size() > largest) largest = woss.str().size();

	woss.str(L"");
	woss << USED << std::to_wstring(all_used) << L" GB";
	if(woss.str().size() > largest) largest = woss.str().size();

	woss.str(L"");
	woss << AVAILABLE << std::to_wstring(all_available) << L" GB";
	if(woss.str().size() > largest) largest = woss.str().size();

	std::wcout << HARD_DISKS << std::endl;

	drive = 'A';
	drives = GetLogicalDrives();
	for(all_total = all_used = all_available = 0; drive <= 'Z'; ++drive){
		if(drives & 1){
			drive_letter[0] = drive;
			if(!GetDiskFreeSpaceEx(drive_letter, &free_bytes, &total_bytes, &total_free_bytes)){
				std::wcerr << L"\t\t" << ERROR_HARD_DISK_SPACE << drive << ": "
				<< GetLastError() << std::endl << std::endl;
				drives >>= 1;
				continue;
			}

			total_tmp     = (total_bytes.QuadPart      / 1073741824);
			available_tmp = (total_free_bytes.QuadPart / 1073741824);
			used_tmp = total_tmp - available_tmp;

			all_total     += total_tmp;
			all_used      += used_tmp;
			all_available += available_tmp;

			++total_drivers;

			print_title_largest(1, 1, largest, L' ', std::wstring(L"Drive ") + std::wstring(1, static_cast<wchar_t>(drive)) + L':' ,
				TOTAL,     std::to_wstring(total_tmp)     + L" GB",
				USED,      std::to_wstring(used_tmp)      + L" GB",
				AVAILABLE, std::to_wstring(available_tmp) + L" GB");
		}
		drives >>= 1;
	}

	if(total_drivers > 1) return print_title_largest(1, 1, largest, L' ', ALL,
		TOTAL,     std::to_wstring(all_total)     + L" GB",
		USED,      std::to_wstring(all_used)      + L" GB",
		AVAILABLE, std::to_wstring(all_available) + L" GB");

	return std::wcout;

}
