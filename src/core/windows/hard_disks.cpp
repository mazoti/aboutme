module;

#include <iostream>
#include <sstream>

#include <Windows.h>

module core;

import common;
import i18n;
import i18n_system;

// Helper function to format a disk size line and update the largest line length
static inline void print_hard_disk(std::wostringstream& woss, const wchar_t* description, size_t total,
const wchar_t* data_size, size_t& largest){
	woss.str(L"");
	woss << description << std::to_wstring(total) << data_size;
	if(woss.str().size() > largest) largest = woss.str().size();
}

// Displays all hard disks sizes and usages
std::wostream& hard_disks() noexcept{
	std::wostringstream woss;
	size_t total_tmp, used_tmp, available_tmp, largest = 0, total_drivers = 0;

	ULONGLONG all_total, all_used, all_available;
	ULARGE_INTEGER free_bytes, total_bytes, total_free_bytes;

	char drive_letter[4] = {'A', ':', '\\', 0}, drive = 'A';
	DWORD drives = GetLogicalDrives();

	if(!drives)	return std::wcerr << i18n_system::ERROR_HARD_DISK << GetLastError() << std::endl << std::endl;

	// Finds the largest line size and iterates again to print
	for(all_total = all_used = all_available = 0; drive <= 'Z'; ++drive){
		if(drives & 1){
			drive_letter[0] = drive;
			if(!GetDiskFreeSpaceEx(drive_letter, &free_bytes, &total_bytes, &total_free_bytes)){
				std::wcerr << L"\t\t" << i18n_system::ERROR_HARD_DISK_SPACE << drive << ": "
				<< GetLastError() << std::endl << std::endl;
				drives >>= 1;
				continue;
			}

			// Converts sizes from bytes to gigabytes (1 GB = 2^30 bytes)
			total_tmp     = (total_bytes.QuadPart      / 1073741824);
			available_tmp = (total_free_bytes.QuadPart / 1073741824);
			used_tmp      = total_tmp - available_tmp;

			// Accumulates totals across all drives
			all_total     += total_tmp;
			all_used      += used_tmp;
			all_available += available_tmp;

			// Calculates line lengths for this drive
			print_hard_disk(woss, i18n::TOTAL,         total_tmp, L" GB", largest);
			print_hard_disk(woss, i18n::USED,           used_tmp, L" GB", largest);	
			print_hard_disk(woss, i18n::AVAILABLE, available_tmp, L" GB", largest);
		}
		drives >>= 1;
	}

	// Calculates line lengths for overall totals
	print_hard_disk(woss, i18n::TOTAL,         all_total, L" GB", largest);
	print_hard_disk(woss, i18n::USED,           all_used, L" GB", largest);
	print_hard_disk(woss, i18n::AVAILABLE, all_available, L" GB", largest);

	std::wcout << i18n::HARD_DISKS << std::endl;

	// Second pass: prints details for each drive
	drive = 'A';
	drives = GetLogicalDrives();
	for(all_total = all_used = all_available = 0; drive <= 'Z'; ++drive){
		if(drives & 1){
			drive_letter[0] = drive;
			if(!GetDiskFreeSpaceEx(drive_letter, &free_bytes, &total_bytes, &total_free_bytes)){
				std::wcerr << L"\t\t" << i18n_system::ERROR_HARD_DISK_SPACE << drive << ": "
				<< GetLastError() << std::endl << std::endl;
				drives >>= 1;
				continue;
			}

			// Converts sizes from bytes to gigabytes
			total_tmp     = (total_bytes.QuadPart      / 1073741824);
			available_tmp = (total_free_bytes.QuadPart / 1073741824);
			used_tmp      = total_tmp - available_tmp;

			// Accumulates totals across all drives
			all_total     += total_tmp;
			all_used      += used_tmp;
			all_available += available_tmp;

			++total_drivers;

			// Prints formatted drive info using largest line length for alignment
			print_title_largest(1, 1, largest, L' ', std::wstring(L"Drive ") + std::wstring(1,
				static_cast<wchar_t>(drive)) + L':' ,
				i18n::TOTAL,     std::to_wstring(total_tmp)     + L" GB",
				i18n::USED,      std::to_wstring(used_tmp)      + L" GB",
				i18n::AVAILABLE, std::to_wstring(available_tmp) + L" GB");
		}
		drives >>= 1;
	}

	if(total_drivers > 1) return print_title_largest(1, 1, largest, L' ', i18n::ALL,
		i18n::TOTAL,     std::to_wstring(all_total)     + L" GB",
		i18n::USED,      std::to_wstring(all_used)      + L" GB",
		i18n::AVAILABLE, std::to_wstring(all_available) + L" GB");

	return std::wcout;
}
