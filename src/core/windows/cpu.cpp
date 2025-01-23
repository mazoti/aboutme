module;

#include <iostream>
#include <array>
#include <bitset>

#include <Windows.h>
#include <intrin.h>

module core;

import i18n;

static std::wstring arch(WORD code){
	switch(code){
		case 0:  return L"x86";	
		case 5:  return L"ARM";	
		case 6:  return L"Itanium";	
		case 9:  return L"AMD64";
		case 12: return L"ARM64";
		default: return L"Unknown";
	}
}

// Displays CPU details
std::wostream& cpu() noexcept{
	size_t start;
	std::array<char, 64> cpu_brand{};
	std::array<int, 4> cpu_information, cpu_information_extended;
	SYSTEM_INFO sys_info;

	int* cpu_info = reinterpret_cast<int*>(&cpu_brand[0]);
	__cpuid(cpu_info, -2147483646); // 0x80000002

	cpu_info = reinterpret_cast<int*>(&cpu_brand[15]);
	__cpuid(cpu_info, -2147483645); // 0x80000003

	cpu_info = reinterpret_cast<int*>(&cpu_brand[31]);
	__cpuid(cpu_info, -2147483644); // 0x80000004

	GetSystemInfo(&sys_info);

	// Left trim string
	start = 0;
	for(const char c: cpu_brand){
		if(!std::isspace(c)) break;
		++start;
	}

	// Get vendor string
	__cpuid(cpu_information.data(), 0);
 
	// Get feature flags
	__cpuid(cpu_information.data(), 1);
	__cpuidex(cpu_information_extended.data(), 7, 0);

	// EDX flags
	std::bitset<32> features_edx(static_cast<unsigned int>(cpu_information[3]));

	// ECX flags
	std::bitset<32> features_ecx(static_cast<unsigned int>(cpu_information[2]));

	// EBX flags for extended features
	std::bitset<32> features_ebx(static_cast<unsigned int>(cpu_information_extended[1]));

	std::wcout << CPU << std::endl 
		<< L'\t' << &cpu_brand[start] << std::endl
		<< L'\t' << ARCHITECTURE << L' ' << arch(sys_info.wProcessorArchitecture) << std::endl
		<< L'\t' << CORES << L' ' << sys_info.dwNumberOfProcessors << std::endl
		<< L'\t' << INSTRUCTIONS_SUPPORTED << L' ';

	if(features_ecx[25]) std::wcout << "AES ";
	if(features_ecx[28]) std::wcout << "AVX ";
	if(features_ebx[5])  std::wcout << "AVX2 ";
	if(features_ebx[30]) std::wcout << "AVX512BW ";
	if(features_ebx[28]) std::wcout << "AVX512CD ";
	if(features_ebx[17]) std::wcout << "AVX512DQ ";
	if(features_ebx[27]) std::wcout << "AVX512ER ";
	if(features_ebx[16]) std::wcout << "AVX512F ";
	if(features_ebx[21]) std::wcout << "AVX512IFMA ";
	if(features_ebx[26]) std::wcout << "AVX512PF ";
	if(features_ebx[31]) std::wcout << "AVX512VL ";
	if(features_edx[15]) std::wcout << "CMOV ";
	if(features_ecx[13]) std::wcout << "CMPXCHG16B ";
	if(features_edx[23]) std::wcout << "MMX ";
	if(features_edx[25]) std::wcout << "SSE ";
	if(features_edx[26]) std::wcout << "SSE2 ";
	if(features_ecx[0])  std::wcout << "SSE3 ";
	if(features_ecx[9])  std::wcout << "SSSE3 ";
	if(features_ecx[19]) std::wcout << "SSE4.1 ";
	if(features_ecx[20]) std::wcout << "SSE4.2 ";
	if(features_edx[4])  std::wcout << "TSC ";

	return std::wcout << std::endl << std::endl;
}
