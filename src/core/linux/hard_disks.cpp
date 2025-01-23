module;

#include <iostream>
#include <sstream>
#include <memory>
#include <map>

#include <mntent.h>
#include <sys/statvfs.h>

module core;

import i18n;
import i18n_system;

import common;

std::wostream& hard_disks(){
	size_t all_total, all_used, all_available, total_tmp, used_tmp, available_tmp;

	struct mntent *mnt;
	struct statvfs disk_stat;

	std::string fs_name;
	std::map<std::string, std::string> mount_points;

	FILE *mnt_file = setmntent("/proc/mounts", "r");
	if(!mnt_file) return std::wcerr << ERROR_LINUX_HARD_DISK << std::endl << std::endl;

	// Create a unique_ptr with a custom deleter to close the mntfile on any return
	std::unique_ptr<FILE, decltype(&endmntent)> endmntent_ptr(mnt_file, endmntent);

	while((mnt = getmntent(mnt_file)) != nullptr){
		fs_name = mnt->mnt_fsname;
		if((fs_name.starts_with("/dev/")) && (mount_points.find(fs_name) == mount_points.end())){
			mount_points.try_emplace(fs_name, mnt->mnt_dir);
		}
	}

	std::wcout << HARD_DISKS << std::endl;

	all_total = all_used = all_available = 0;

	// Iterates and prints the map
	for(const auto& [key, value] : mount_points){
		if(statvfs(value.c_str(), &disk_stat)){
			std::wcerr << L"\t\t" << ERROR_LINUX_HARD_DISK_STATVFS << std::endl << std::endl;
			continue;
		}

		total_tmp     = disk_stat.f_blocks * disk_stat.f_frsize / 1073741824;
		available_tmp = disk_stat.f_bsize * disk_stat.f_bfree   / 1073741824;
		used_tmp = total_tmp - available_tmp;

		all_total     += total_tmp;
		all_used      += used_tmp;
		all_available += available_tmp;

		print_title(1, 1, L' ', std::wstring(key.begin(), key.end()) + L':' ,
			TOTAL,     std::to_wstring(total_tmp)     + L" GB",
			USED,      std::to_wstring(used_tmp)      + L" GB",
			AVAILABLE, std::to_wstring(available_tmp) + L" GB");
	}

	return print_title(1, 1, L' ', ALL,
		TOTAL,     std::to_wstring(all_total)     + L" GB",
		USED,      std::to_wstring(all_used)      + L" GB",
		AVAILABLE, std::to_wstring(all_available) + L" GB");
}
