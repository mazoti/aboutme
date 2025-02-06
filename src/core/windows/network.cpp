module;

#include <iostream>
#include <sstream>
#include <iomanip>
#include <memory>
#include <map>
#include <set>

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iphlpapi.h>

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")

module core;

import common;

import i18n;
import i18n_system;

struct free_adapter{ void operator()(IP_ADAPTER_ADDRESSES* adapter) const{ if(adapter) free(adapter); }};

std::wostream& network() noexcept{
	size_t i;
	char host_name[MAX_COMPUTERNAME_LENGTH + 1], dns_address[INET6_ADDRSTRLEN];

	DWORD host_name_size, output_buffer_length, return_value;
	ULONG buffer_length;

	PIP_ADAPTER_ADDRESSES adapter_address;
	PIP_ADAPTER_INFO adapter_pointer;
	IP_ADAPTER_DNS_SERVER_ADDRESS* dns_server_pointer;

	std::wostringstream mac_address;
	std::set<std::string> dns_servers_ordered;
	std::multimap<std::wstring, std::wstring> network_devices_ordered;

	std::wcout << NETWORK << std::endl;

	// Displays host name
	host_name_size = sizeof(host_name) / sizeof(host_name[0]);
	if(!GetComputerName(host_name, &host_name_size))
		return std::wcerr << '\t' << ERROR_HOST_NAME << std::endl << std::endl;

	std::wcout << L'\t' << HOST_NAME << std::endl << L"\t\t" << host_name << std::endl << std::endl;

	// Displays gateway
	GetAdaptersInfo(nullptr, &buffer_length);
	std::unique_ptr<IP_ADAPTER_INFO, void(*)(void*)> adapter_pointer_info(static_cast<IP_ADAPTER_INFO*>
		(std::malloc(buffer_length)), std::free);

	if(!adapter_pointer_info) return std::wcerr << ERROR_MEMORY_ALLOCATION << std::endl << std::endl;

	adapter_pointer = adapter_pointer_info.get();
	if(GetAdaptersInfo(adapter_pointer, &buffer_length) != NO_ERROR)
		return std::wcerr << ERROR_ADAPTERS_INFO << std::endl << std::endl;

	for(;adapter_pointer;adapter_pointer = adapter_pointer->Next){
		if(adapter_pointer->GatewayList.IpAddress.String[0] != '0')
			std::wcout << L'\t' << GATEWAY << std::endl << L"\t\t"
			<< adapter_pointer->GatewayList.IpAddress.String
			<< std::endl << std::endl;
	}

	// Lists all Ethernet adapters and their MAC addresses
	output_buffer_length = sizeof(IP_ADAPTER_ADDRESSES);
	std::unique_ptr<IP_ADAPTER_ADDRESSES, free_adapter> adapter_addresses_ptr(static_cast<IP_ADAPTER_ADDRESSES*>
		(malloc(output_buffer_length)));
	if(!adapter_addresses_ptr) return std::wcerr << ERROR_MEMORY_ALLOCATION << std::endl << std::endl;

	return_value = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_ALL_COMPARTMENTS, nullptr,
		adapter_addresses_ptr.get(), &output_buffer_length);
	if(return_value == ERROR_BUFFER_OVERFLOW){
        adapter_addresses_ptr.reset(static_cast<IP_ADAPTER_ADDRESSES*>(malloc(output_buffer_length)));
		if(!adapter_addresses_ptr) return std::wcerr << ERROR_MEMORY_ALLOCATION << std::endl << std::endl;
		return_value = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_ALL_COMPARTMENTS, nullptr,
			adapter_addresses_ptr.get(), &output_buffer_length);
	}

	if(return_value != ERROR_SUCCESS) return std::wcerr << ERROR_ADAPTERS_ADDRESS << std::endl << std::endl;

	for(adapter_address = adapter_addresses_ptr.get();
		adapter_address != nullptr;
		adapter_address = adapter_address->Next){

		if(adapter_address->IfType == IF_TYPE_ETHERNET_CSMACD || adapter_address->IfType == IF_TYPE_IEEE80211){
			mac_address	<< L"MAC: ";
			for(i=0; i < adapter_address->PhysicalAddressLength; ++i){
				mac_address	<< std::hex << std::setw(2) << std::setfill(L'0')
					<< static_cast<int>(adapter_address->PhysicalAddress[i]);
				if(i < adapter_address->PhysicalAddressLength - 1) mac_address << L':';
			}
			network_devices_ordered.insert({adapter_address->FriendlyName, std::wstring(DESCRIPTION)
				+ std::wstring(L" ") + std::wstring(adapter_address->Description)});
			network_devices_ordered.insert({adapter_address->FriendlyName, mac_address.str()});

			mac_address.str(L"");
		}

		// Gets all DNS servers
		for(dns_server_pointer = adapter_address->FirstDnsServerAddress; dns_server_pointer != nullptr;
			dns_server_pointer = dns_server_pointer->Next){

			if (dns_server_pointer->Address.lpSockaddr->sa_family == AF_INET){
				sockaddr_in* sa_in = reinterpret_cast<sockaddr_in*>(dns_server_pointer->Address.lpSockaddr);
				inet_ntop(AF_INET, &sa_in->sin_addr, dns_address, sizeof(dns_address));
				dns_servers_ordered.insert(dns_address);
				continue;
			}
			if(dns_server_pointer->Address.lpSockaddr->sa_family == AF_INET6){
				sockaddr_in6* sa_in6 = reinterpret_cast<sockaddr_in6*>(dns_server_pointer->Address.lpSockaddr);
				inet_ntop(AF_INET6, &sa_in6->sin6_addr, dns_address, sizeof(dns_address));
				dns_servers_ordered.insert(dns_address);
			}
		}
	}

	// Prints all DNS servers
	std::wcout << L'\t' << DNS_SERVERS << std::endl;
	for(const std::string& dns_server : dns_servers_ordered)
		std::wcout << L"\t\t" << std::wstring(dns_server.begin(), dns_server.end()) << std::endl;

	return std::wcout << std::endl << network_devices_ordered;
}
