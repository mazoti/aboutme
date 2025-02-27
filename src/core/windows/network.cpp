module;

#include <iostream>
#include <sstream>
#include <iomanip>
#include <memory>
#include <map>
#include <set>

#include <span>

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iphlpapi.h>

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")

module core;

import common;

import i18n;
import i18n_system;

struct wsa_cleanup { void operator()(void*){ WSACleanup(); }};
struct free_table  { void operator()(void* p){ if(p) free(p); }};
struct free_adapter{ void operator()(IP_ADAPTER_ADDRESSES* adapter) const{ if(adapter) free(adapter); }};

std::wostream& network() noexcept{
	size_t i;
	char host_name[MAX_COMPUTERNAME_LENGTH + 1], dns_address[INET6_ADDRSTRLEN],
		local_ip[INET_ADDRSTRLEN], remote_ip[INET_ADDRSTRLEN];
	struct in_addr local_address, remote_address;

	DWORD host_name_size, output_buffer_length, return_value, dword_size;
	ULONG buffer_length;
	WSADATA wsa_data;

	PIP_ADAPTER_ADDRESSES adapter_address;
	PIP_ADAPTER_INFO adapter_pointer;
	IP_ADAPTER_DNS_SERVER_ADDRESS* dns_server_pointer;

	std::wostringstream woss;
	std::wstring key, value;

	std::set<std::wstring> udp_ordered;
	std::set<std::wstring> tcp_established_ordered, tcp_listening_ordered, tcp_closed_ordered, tcp_other_ordered;
	std::set<std::string> dns_servers_ordered;
	std::multimap<std::wstring, std::wstring> network_devices_ordered;

	std::wcout << i18n::NETWORK << std::endl;

	// Displays gateway
	GetAdaptersInfo(nullptr, &buffer_length);
	std::unique_ptr<IP_ADAPTER_INFO, void(*)(void*)> adapter_pointer_info(static_cast<IP_ADAPTER_INFO*>
		(std::malloc(buffer_length)), std::free);

	if(!adapter_pointer_info) return std::wcerr << L'\t' << ERROR_MEMORY_ALLOCATION << std::endl << std::endl;

	adapter_pointer = adapter_pointer_info.get();
	if(GetAdaptersInfo(adapter_pointer, &buffer_length) != NO_ERROR)
		return std::wcerr << L'\t' << ERROR_ADAPTERS_INFO << std::endl << std::endl;

	for(;adapter_pointer;adapter_pointer = adapter_pointer->Next){
		woss << adapter_pointer->Description;
		key = woss.str();
		woss.str(L"");

		woss << L"IP: " << adapter_pointer->IpAddressList.IpAddress.String;
		value = woss.str();
		woss.str(L"");

		network_devices_ordered.insert({key, value});

		if(adapter_pointer->GatewayList.IpAddress.String[0] != '0'){
			woss << i18n::GATEWAY << " " << adapter_pointer->GatewayList.IpAddress.String;
			value = woss.str();
			woss.str(L"");
			network_devices_ordered.insert({key, value});	
		}
	}

	// Lists all Ethernet adapters and their MAC addresses
	output_buffer_length = sizeof(IP_ADAPTER_ADDRESSES);
	std::unique_ptr<IP_ADAPTER_ADDRESSES, free_adapter> adapter_addresses_ptr(static_cast<IP_ADAPTER_ADDRESSES*>
		(malloc(output_buffer_length)));
	if(!adapter_addresses_ptr) return std::wcerr << L'\t' << ERROR_MEMORY_ALLOCATION << std::endl << std::endl;

	return_value = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_ALL_COMPARTMENTS, nullptr,
		adapter_addresses_ptr.get(), &output_buffer_length);
	if(return_value == ERROR_BUFFER_OVERFLOW){
        adapter_addresses_ptr.reset(static_cast<IP_ADAPTER_ADDRESSES*>(malloc(output_buffer_length)));
		if(!adapter_addresses_ptr) return std::wcerr << ERROR_MEMORY_ALLOCATION << std::endl << std::endl;
		return_value = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_ALL_COMPARTMENTS, nullptr,
			adapter_addresses_ptr.get(), &output_buffer_length);
	}

	if(return_value != ERROR_SUCCESS) return std::wcerr << L'\t' << ERROR_ADAPTERS_ADDRESS << std::endl << std::endl;

	for(adapter_address = adapter_addresses_ptr.get();
		adapter_address != nullptr;
		adapter_address = adapter_address->Next){

		if(adapter_address->IfType == IF_TYPE_ETHERNET_CSMACD || adapter_address->IfType == IF_TYPE_IEEE80211){
			woss << L"MAC: ";
			for(i = 0; i < adapter_address->PhysicalAddressLength; ++i){
				woss << std::hex << std::setw(2) << std::setfill(L'0')
					<< static_cast<int>(adapter_address->PhysicalAddress[i]);
				if(i < adapter_address->PhysicalAddressLength - 1) woss << L':';
			}

			value = woss.str();
			woss.str(L"");

			woss << adapter_address->Description;
			key = woss.str();
			woss.str(L"");
			network_devices_ordered.insert({key, value});
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

	// Initialize Winsock with RAII
	if(WSAStartup(MAKEWORD(2, 2), &wsa_data)) return std::wcerr << L'\t'<< ERROR_WSA_STARTUP << std::endl << std::endl;
	std::unique_ptr<void, wsa_cleanup> wsa_cleanup_ptr(reinterpret_cast<void*>(1));

	// First call to get required size
	GetExtendedUdpTable(nullptr, &dword_size, TRUE, AF_INET, UDP_TABLE_OWNER_PID, 0);

	std::unique_ptr<MIB_UDPTABLE_OWNER_PID, free_table> udp_table_ptr(static_cast<PMIB_UDPTABLE_OWNER_PID>
		(malloc(dword_size)));
	if(!udp_table_ptr) return std::wcerr << L'\t' << ERROR_UDP_MALLOC << std::endl << std::endl;

	// Get the UDP table
	if(GetExtendedUdpTable(udp_table_ptr.get(), &dword_size, TRUE, AF_INET, UDP_TABLE_OWNER_PID, 0) != NO_ERROR)
		return std::wcerr << L'\t' << ERROR_EXTENDED_UDP_TABLE << std::endl << std::endl;

	// Create span over the table entries
	std::span<MIB_UDPROW_OWNER_PID> udp_table_ptr_span{udp_table_ptr->table,
		(dword_size - sizeof(DWORD)) / sizeof(MIB_UDPROW_OWNER_PID)};
	for(const _MIB_UDPROW_OWNER_PID& entry : udp_table_ptr_span){
		local_address.s_addr = entry.dwLocalAddr;
		inet_ntop(AF_INET, &local_address, local_ip, sizeof(local_ip));

		woss.str(L"");
		woss << local_ip << L":" << std::dec << ntohs(static_cast<u_short>(entry.dwLocalPort))
			<< L" (PID " << entry.dwOwningPid << L")";
		udp_ordered.insert(woss.str());
	}

	// First call to get required size
	GetExtendedTcpTable(nullptr, &dword_size, TRUE, AF_INET, TCP_TABLE_OWNER_PID_ALL, 0);

	std::unique_ptr<MIB_TCPTABLE_OWNER_PID, free_table> tcp_table_ptr
	(static_cast<PMIB_TCPTABLE_OWNER_PID>(malloc(dword_size)));
	if(!tcp_table_ptr) return std::wcerr << L'\t' << ERROR_TCP_MALLOC << std::endl << std::endl;

	// Get the TCP table
	if(GetExtendedTcpTable(tcp_table_ptr.get(), &dword_size, TRUE, AF_INET, TCP_TABLE_OWNER_PID_ALL, 0) != NO_ERROR)
		return std::wcerr << L'\t' << ERROR_EXTENDED_TCP_TABLE << std::endl << std::endl;

	// Create span over the table entries
	std::span<MIB_TCPROW_OWNER_PID> tcp_table_ptr_span{tcp_table_ptr->table, tcp_table_ptr->dwNumEntries};
	for(const _MIB_TCPROW_OWNER_PID& entry : tcp_table_ptr_span){
		local_address.s_addr = entry.dwLocalAddr;
		remote_address.s_addr = entry.dwRemoteAddr;

		inet_ntop(AF_INET, &local_address, local_ip, sizeof(local_ip));
		inet_ntop(AF_INET, &remote_address, remote_ip, sizeof(remote_ip));

		woss.str(L"");
		woss << local_ip << L":" << ntohs(static_cast<u_short>(entry.dwLocalPort)) << L" - "
			<< remote_ip << L":" << ntohs(static_cast<u_short>(entry.dwRemotePort))
			<< L" (PID: " << entry.dwOwningPid << L")";

		switch(entry.dwState){
			case MIB_TCP_STATE_CLOSED:
				tcp_closed_ordered.insert(woss.str());
				break;
			case MIB_TCP_STATE_LISTEN:
				tcp_listening_ordered.insert(woss.str());
				break;
			case MIB_TCP_STATE_ESTAB:
				tcp_established_ordered.insert(woss.str());
				break;
			default:
				tcp_other_ordered.insert(woss.str());
		}
	}

	std::wcout << network_devices_ordered;

	// Prints host name
	host_name_size = sizeof(host_name) / sizeof(host_name[0]);
	if(!GetComputerName(host_name, &host_name_size))
		return std::wcerr << L'\t' << ERROR_HOST_NAME << std::endl << std::endl;

	std::wcout << L'\t' << i18n::HOST_NAME << std::endl << L"\t\t" << host_name << std::endl << std::endl;

	// Prints DNS servers
	std::wcout << L'\t' << i18n::DNS_SERVERS << std::endl;
	for(const std::string& dns_server : dns_servers_ordered)
		std::wcout << L"\t\t" << std::wstring(dns_server.begin(), dns_server.end()) << std::endl;

	// Prints UDP endpoints
	std::wcout << std::endl << L'\t' << i18n::UDP_ENDPOINTS << std::endl;
	for(const std::wstring& udp_endpoint : udp_ordered)
		std::wcout << L"\t\t" << udp_endpoint << std::endl;

	// Prints TCP endpoints
	std::wcout << std::endl << L'\t' << i18n::TCP_ENDPOINTS << std::endl;

	if(!tcp_established_ordered.empty()){
		std::wcout << L"\t\t" << "ESTABLISHED:" << std::endl;
		for(const std::wstring& tcp_endpoint : tcp_established_ordered)
			std::wcout << L"\t\t\t" << tcp_endpoint << std::endl;
	}

	if(!tcp_listening_ordered.empty()){
		std::wcout << std::endl << L"\t\t" << "LISTENING:" << std::endl;
		for(const std::wstring& tcp_endpoint : tcp_listening_ordered)
			std::wcout << L"\t\t\t" << tcp_endpoint << std::endl;
	}

	if(!tcp_closed_ordered.empty()){
		std::wcout << std::endl << L"\t\t" << "CLOSED:" << std::endl;
		for(const std::wstring& tcp_endpoint : tcp_closed_ordered)
			std::wcout << L"\t\t\t" << tcp_endpoint << std::endl;
	}

	if(!tcp_other_ordered.empty()){
		std::wcout << std::endl << L"\t\t" << "OTHER:" << std::endl;
		for(const std::wstring& tcp_endpoint : tcp_other_ordered)
			std::wcout << L"\t\t\t" << tcp_endpoint << std::endl;
	}

	return std::wcout << std::endl;
}
