module;

#include <iostream>
#include <sstream>
#include <memory>
#include <filesystem>
#include <set>

#include <unistd.h>
#include <fcntl.h>
#include <sys/disk.h>

module core;

import i18n;
import i18n_system;
import common;

// Custom closer for file descriptors
struct fd_closer{
	void operator()(int* file_descriptor) const{
		if(file_descriptor != nullptr && *file_descriptor >= 0) close(*file_descriptor);
	}
};

static void print_mounted(const char* path, size_t* all_total, size_t* all_used, size_t* all_available){
	size_t total_tmp, used_tmp, available_tmp;
	std::error_code err_code;

	for(const auto& entry : std::filesystem::directory_iterator(path)){
		std::string canonical_path = std::filesystem::canonical(entry);
		std::filesystem::space_info space = std::filesystem::space(canonical_path.c_str(), err_code);

		if(err_code){
			std::wcerr << ERROR_FREEBSD_SPACE_INFO << std::endl;
			continue;
		}

		total_tmp     = space.capacity / 1073741824;
		available_tmp = space.free     / 1073741824;
		used_tmp = total_tmp - available_tmp;

		*all_total += total_tmp;
		*all_available += available_tmp;
		*all_used += used_tmp;

		print_title(1, 1, L' ', std::wstring(canonical_path.begin(), canonical_path.end()),
			TOTAL,        std::to_wstring(total_tmp)     + L" GB",
			USED,         std::to_wstring(used_tmp)      + L" GB",
			AVAILABLE,    std::to_wstring(available_tmp) + L" GB");
	}
}

std::wostream& hard_disks(){
	int number;
	off_t mediasize;
	size_t all_total, all_used, all_available;

	std::string filename, fullpath;
	std::error_code err_code;
	std::set<std::string> ordered_disks;

	std::wcout << HARD_DISKS << std::endl;

	// Gets all physical disks
	for(const auto& entry : std::filesystem::directory_iterator("/dev")){
		filename = entry.path().filename().string();

		if(filename.starts_with("ada")){
			number = std::stoi(filename.substr(3));
			fullpath = "/dev/ada" + std::to_string(number);
			ordered_disks.emplace(fullpath);
			continue;
		}

		if(filename.starts_with("nvd")){
			number = std::stoi(filename.substr(3));
			fullpath = "/dev/nvd" + std::to_string(number);
			ordered_disks.emplace(fullpath);
			continue;
		}

		if(filename.starts_with("da")){
			number = std::stoi(filename.substr(2));
			fullpath = "/dev/da" + std::to_string(number);
			ordered_disks.emplace(fullpath);
		}
	}

	// Gets disk sizes
	for(const auto& entry : ordered_disks){
		number = open(entry.c_str(), O_RDONLY);
		if(number < 0){
			std::wcerr << L'\t' << ERROR_FREEBSD_OPEN_DEVICE << L' ' << std::wstring(entry.begin(), entry.end())
			<< std::endl << std::endl;
			continue;
		}

		// Create a unique_ptr
		std::unique_ptr<int, fd_closer> number_fd_Ptr(&number);

		if(ioctl(number, DIOCGMEDIASIZE, &mediasize) == -1){
			std::wcerr << L'\t' << ERROR_FREEBSD_MEDIA_SIZE << L' ' << std::wstring(entry.begin(), entry.end())
			<< std::endl << std::endl;
			continue;
		}

		std::wcout << L'\t' << std::wstring(entry.begin(), entry.end()) << std::endl
			<< L"\t\t" << TOTAL << L' ' <<  mediasize / 1073741824 << L" GB" << std::endl
			<< std::endl;
	}

	// Displays mounted sizes
	all_total = all_used = all_available = 0;

	print_mounted("/mnt",   &all_total, &all_used, &all_available);
	print_mounted("/media", &all_total, &all_used, &all_available);

	return print_title(1, 1, L' ', ALL,
		TOTAL,     std::to_wstring(all_total)     + L" GB",
		USED,      std::to_wstring(all_used)      + L" GB",
		AVAILABLE, std::to_wstring(all_available) + L" GB");
}
