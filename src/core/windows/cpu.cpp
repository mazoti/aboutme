module;

#include <iostream>
#include <array>
#include <bitset>

#include <Windows.h>
#include <intrin.h>

module core;

import i18n;

// Displays all CPU supported instructions
inline const static std::wstring instructions(const std::array<int, 4>& cpu_information,
const std::array<int, 4>& cpu_information_extended){
	std::wstring all_instructions;

	// Converts feature flags from registers to bitsets for easy bit testing
	std::bitset<32> features_edx(static_cast<unsigned int>(cpu_information[3]));
	std::bitset<32> features_ecx(static_cast<unsigned int>(cpu_information[2]));
	std::bitset<32> features_ebx(static_cast<unsigned int>(cpu_information_extended[1]));

	if(features_ecx[25]) all_instructions.append(L"AES ");
	if(features_ecx[28]) all_instructions.append(L"AVX ");
	if(features_ebx[5])  all_instructions.append(L"AVX2 ");
	if(features_ebx[30]) all_instructions.append(L"AVX512BW ");
	if(features_ebx[28]) all_instructions.append(L"AVX512CD ");
	if(features_ebx[17]) all_instructions.append(L"AVX512DQ ");
	if(features_ebx[27]) all_instructions.append(L"AVX512ER ");
	if(features_ebx[16]) all_instructions.append(L"AVX512F ");
	if(features_ebx[21]) all_instructions.append(L"AVX512IFMA ");
	if(features_ebx[26]) all_instructions.append(L"AVX512PF ");
	if(features_ebx[31]) all_instructions.append(L"AVX512VL ");
	if(features_edx[15]) all_instructions.append(L"CMOV ");
	if(features_ecx[13]) all_instructions.append(L"CMPXCHG16B ");
	if(features_edx[23]) all_instructions.append(L"MMX ");
	if(features_edx[25]) all_instructions.append(L"SSE ");
	if(features_edx[26]) all_instructions.append(L"SSE2 ");
	if(features_ecx[0])  all_instructions.append(L"SSE3 ");
	if(features_ecx[9])  all_instructions.append(L"SSSE3 ");
	if(features_ecx[19]) all_instructions.append(L"SSE4.1 ");
	if(features_ecx[20]) all_instructions.append(L"SSE4.2 ");
	if(features_edx[4])  all_instructions.append(L"TSC ");

	// Remove trailing space
	if(!all_instructions.empty()) all_instructions.pop_back();
	return all_instructions;
}

// Converts a processor architecture code (WORD) to a human-readable wide string
inline const static std::wstring arch(WORD code){
	switch(code){
		case  0: return L"x86";	
		case  5: return L"ARM";	
		case  6: return L"Itanium";	
		case  9: return L"AMD64";
		case 12: return L"ARM64";
		default: return L"Unknown";
	}
}

// Displays CPU details
std::wostream& cpu() noexcept{
	size_t start;
	SYSTEM_INFO sys_info;

	std::array<char, 64> cpu_brand{};
	std::array<int,   4> cpu_information, cpu_information_extended;

	// Retrieves CPU brand string using CPUID leaf 0x80000002 through 0x80000004
	int *cpu_info = reinterpret_cast<int*>(&cpu_brand[0]);
	__cpuid(cpu_info, -2147483646); // 0x80000002

	cpu_info = reinterpret_cast<int*>(&cpu_brand[15]);
	__cpuid(cpu_info, -2147483645); // 0x80000003

	cpu_info = reinterpret_cast<int*>(&cpu_brand[31]);
	__cpuid(cpu_info, -2147483644); // 0x80000004

	// Populates sys_info with details like architecture and processor count
	GetSystemInfo(&sys_info);

	// Trims leading spaces from the CPU brand string
	start = 0;
	for(const char c: cpu_brand){
		if(!std::isspace(c)) break;
		++start;
	}

	// Gets vendor string, results stored in EBX, EDX, ECX registers
	__cpuid(cpu_information.data(), 0);
 
	// Gets feature flags, results stored in EAX(stepping), EBX(misc), ECX(features), EDX(features)
	__cpuid(cpu_information.data(), 1);

	// Retrieves extended feature flags using CPUID leaf 7, subleaf 0, results in EBX(extended features)
	__cpuidex(cpu_information_extended.data(), 7, 0);

	// Converts feature flags from registers to bitsets for easy bit testing
	std::bitset<32> features_edx(static_cast<unsigned int>(cpu_information[3]));
	std::bitset<32> features_ecx(static_cast<unsigned int>(cpu_information[2]));
	std::bitset<32> features_ebx(static_cast<unsigned int>(cpu_information_extended[1]));

	// Outputs CPU information formatted
	return std::wcout << i18n::CPU        << std::endl 
		<< L'\t' << &cpu_brand[start]     << std::endl
		<< L'\t' << i18n::ARCHITECTURE    << L' ' << arch(sys_info.wProcessorArchitecture) << std::endl
		<< L'\t' << i18n::THREADS << L' ' << sys_info.dwNumberOfProcessors << std::endl
		<< L'\t' << i18n::INSTRUCTIONS_SUPPORTED << L' ' << instructions(cpu_information, cpu_information_extended)
		<< std::endl << std::endl;
}
