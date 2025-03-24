module;

#include <iostream>
#include <array>
#include <string>
#include <cstdint>

#include <Windows.h>
#include <intrin.h>

module core;

import i18n;

// Displays all CPU supported instructions
inline static std::wstring instructions(std::uint32_t features_ebx, std::uint32_t features_ecx,
                                        std::uint32_t features_edx){
	std::wstring all_instructions;

	if(features_ecx & 0x02000000) all_instructions.append(L"AES ");
	if(features_ecx & 0x10000000) all_instructions.append(L"AVX ");

	if(features_ebx & 0x00000020) all_instructions.append(L"AVX2 ");
	if(features_ebx & 0x40000000) all_instructions.append(L"AVX512BW ");
	if(features_ebx & 0x10000000) all_instructions.append(L"AVX512CD ");
	if(features_ebx & 0x00020000) all_instructions.append(L"AVX512DQ ");
	if(features_ebx & 0x08000000) all_instructions.append(L"AVX512ER ");
	if(features_ebx & 0x00010000) all_instructions.append(L"AVX512F ");
	if(features_ebx & 0x00200000) all_instructions.append(L"AVX512IFMA ");
	if(features_ebx & 0x04000000) all_instructions.append(L"AVX512PF ");
	if(features_ebx & 0x80000000) all_instructions.append(L"AVX512VL ");

	if(features_edx & 0x00008000) all_instructions.append(L"CMOV ");
	if(features_ecx & 0x00002000) all_instructions.append(L"CMPXCHG16B ");
	if(features_edx & 0x00000800) all_instructions.append(L"MMX ");
	if(features_edx & 0x02000000) all_instructions.append(L"SSE ");
	if(features_edx & 0x04000000) all_instructions.append(L"SSE2 ");
	if(features_ecx & 0x00000001) all_instructions.append(L"SSE3 ");
	if(features_ecx & 0x00000200) all_instructions.append(L"SSSE3 ");
	if(features_ecx & 0x00080000) all_instructions.append(L"SSE4.1 ");
	if(features_ecx & 0x00100000) all_instructions.append(L"SSE4.2 ");
	if(features_edx & 0x00000010) all_instructions.append(L"TSC ");

	// Removes trailing space if the string is not empty
	if(!all_instructions.empty()) all_instructions.pop_back();
	return all_instructions;
}

// Converts a processor architecture code (WORD) to a human-readable wide string
inline static std::wstring arch(WORD code){
	switch(code){
		case PROCESSOR_ARCHITECTURE_INTEL: return L"x86";
		case PROCESSOR_ARCHITECTURE_ARM:   return L"ARM";
		case PROCESSOR_ARCHITECTURE_IA64:  return L"Itanium";
		case PROCESSOR_ARCHITECTURE_AMD64: return L"AMD64";
		case PROCESSOR_ARCHITECTURE_ARM64: return L"ARM64";
		default:                           return L"Unknown";
	}
}

// Displays CPU details
std::wostream& cpu() noexcept{
	int* cpu_info;
	size_t start;

	SYSTEM_INFO sys_info{};

	std::string brand_str;
	std::array<char, 64> cpu_brand{};
	std::array<int,  4> cpu_information{}, cpu_information_extended{};

	// Retrieves CPU brand string using CPUID leaves 0x80000002 to 0x80000004
	cpu_info = reinterpret_cast<int*>(&cpu_brand[0]);
	__cpuid(cpu_info, -2147483646);  // Bytes 0-15
	cpu_info = reinterpret_cast<int*>(&cpu_brand[16]);
	__cpuid(cpu_info, -2147483645);  // Bytes 16-31
	cpu_info = reinterpret_cast<int*>(&cpu_brand[32]);
	__cpuid(cpu_info, -2147483644);  // Bytes 32-47

	// Converts CPU brand to std::string and trim leading spaces
	brand_str = cpu_brand.data();
	start = brand_str.find_first_not_of(' ');
	if(start == std::string::npos) start = 0;
	brand_str = brand_str.substr(start);

	// Populates system information
	GetSystemInfo(&sys_info);

	// Gets CPU information: vendor string (leaf 0), feature flags (leaf 1), extended features (leaf 7)
	__cpuid(cpu_information.data(), 0);
	__cpuid(cpu_information.data(), 1);
	__cpuidex(cpu_information_extended.data(), 7, 0);

	// Outputs CPU details
	return std::wcout << i18n::CPU << L"\n\t"
	                  << std::wstring(brand_str.begin(), brand_str.end()) << L"\n\t"
	                  << i18n::ARCHITECTURE << L' ' << arch(sys_info.wProcessorArchitecture) << L"\n\t"
	                  << i18n::THREADS      << L' ' << sys_info.dwNumberOfProcessors         << L"\n\t"
	                  << i18n::INSTRUCTIONS_SUPPORTED << L' '
	                  << instructions(static_cast<std::uint32_t>(cpu_information_extended[1]),  // EBX from leaf 7
	                                  static_cast<std::uint32_t>(cpu_information[2]),           // ECX from leaf 1
	                                  static_cast<std::uint32_t>(cpu_information[3]))           // EDX from leaf 1
	                  << L"\n\n";
}
