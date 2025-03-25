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

static void tcp_endpoints(const std::span<MIB_TCPROW_OWNER_PID>& tcp_table_ptr_span, std::set<std::wstring>& tcp_closed_ordered, std::set<std::wstring>& tcp_listening_ordered, std::set<std::wstring>& tcp_established_ordered, std::set<std::wstring>& tcp_other_ordered){
	char local_ip[INET_ADDRSTRLEN], remote_ip[INET_ADDRSTRLEN];
	struct in_addr local_address, remote_address;

	std::wostringstream woss;

	for(const _MIB_TCPROW_OWNER_PID& entry : tcp_table_ptr_span){
		local_address.s_addr = entry.dwLocalAddr;
		remote_address.s_addr = entry.dwRemoteAddr;

		inet_ntop(AF_INET, &local_address,  local_ip,  sizeof(local_ip));
		inet_ntop(AF_INET, &remote_address, remote_ip, sizeof(remote_ip));

		woss.str(L"");
		woss << local_ip << L":" << ntohs(static_cast<u_short>(entry.dwLocalPort)) << L" - "
			<< remote_ip << L":" << ntohs(static_cast<u_short>(entry.dwRemotePort))
			<< L" (PID: " << entry.dwOwningPid << L")";

		switch(entry.dwState){
			case MIB_TCP_STATE_CLOSED: tcp_closed_ordered.insert(woss.str());      break;
			case MIB_TCP_STATE_LISTEN: tcp_listening_ordered.insert(woss.str());   break;
			case MIB_TCP_STATE_ESTAB:  tcp_established_ordered.insert(woss.str()); break;
			default: tcp_other_ordered.insert(woss.str());
		}
	}
}

static const std::set<std::wstring> udp_endpoints(const std::span<MIB_UDPROW_OWNER_PID>& udp_table_ptr_span){
	char local_ip[INET_ADDRSTRLEN];
	struct in_addr local_address;

	std::wostringstream woss;
	std::set<std::wstring> udp_ordered;

	for(const _MIB_UDPROW_OWNER_PID& entry : udp_table_ptr_span){
		local_address.s_addr = entry.dwLocalAddr;
		inet_ntop(AF_INET, &local_address, local_ip, sizeof(local_ip));

		woss.str(L"");
		woss << local_ip << L":" << std::dec << ntohs(static_cast<u_short>(entry.dwLocalPort))
			<< L" (PID " << entry.dwOwningPid << L")";
		udp_ordered.insert(woss.str());
	}

	return udp_ordered;
}

static const std::multimap<std::wstring, std::wstring> network_devices(PIP_ADAPTER_INFO& adapter_pointer, PIP_ADAPTER_ADDRESSES adapter_address){
	size_t i;
	char dns_address[INET6_ADDRSTRLEN];

	std::string tmp;
	std::wstring key, value;
	std::wostringstream woss;

	IP_ADAPTER_DNS_SERVER_ADDRESS *dns_server_pointer;

	sockaddr_in  *socket_address;
	sockaddr_in6 *socket_address_ipv6;

	std::multimap<std::wstring, std::wstring> network_devices_ordered;

	for(; adapter_pointer; adapter_pointer = adapter_pointer->Next){
		tmp = std::string(adapter_pointer->Description);
		key = std::wstring(tmp.begin(), tmp.end());

		tmp = std::string(adapter_pointer->IpAddressList.IpAddress.String);
		value = std::format(L"IP: {}", std::wstring(tmp.begin(), tmp.end()));

		insert_if_unique<std::wstring, std::wstring>(network_devices_ordered, key, value);

		// Stores gateway if valid
		if(adapter_pointer->GatewayList.IpAddress.String[0] != '0'){
			tmp = std::string(adapter_pointer->GatewayList.IpAddress.String);
			value = std::format(L"{} {}", i18n::GATEWAY, std::wstring(tmp.begin(), tmp.end())); 
			insert_if_unique<std::wstring, std::wstring>(network_devices_ordered, key, value);
		}
	}

	for(; adapter_address != nullptr; adapter_address = adapter_address->Next){
		// Handles Ethernet and WiFi adapters
		if(adapter_address->IfType == IF_TYPE_ETHERNET_CSMACD || adapter_address->IfType == IF_TYPE_IEEE80211){
			woss.str(L"");
			woss << L"MAC: ";
			// Formats MAC address
			for(i = 0; i < adapter_address->PhysicalAddressLength; ++i){
				woss << std::hex << std::setw(2) << std::setfill(L'0')
					<< static_cast<int>(adapter_address->PhysicalAddress[i]);
				if(i < adapter_address->PhysicalAddressLength - 1) woss << L':';
			}

			value = woss.str();
			key = std::format(L"{}", adapter_address->Description);
			insert_if_unique<std::wstring, std::wstring>(network_devices_ordered, key, value);
		}

		// Gets all DNS servers
		for(dns_server_pointer = adapter_address->FirstDnsServerAddress; dns_server_pointer != nullptr; dns_server_pointer = dns_server_pointer->Next){
			if(dns_server_pointer->Address.lpSockaddr->sa_family == AF_INET){
				socket_address = reinterpret_cast<sockaddr_in*>(dns_server_pointer->Address.lpSockaddr);
				inet_ntop(AF_INET, &socket_address->sin_addr, dns_address, sizeof(dns_address));
				tmp = dns_address;
				insert_if_unique<std::wstring, std::wstring>(network_devices_ordered, key, L"DNS: " + std::wstring(tmp.begin(), tmp.end()));
				continue;
			}

			if(dns_server_pointer->Address.lpSockaddr->sa_family == AF_INET6){
				socket_address_ipv6 = reinterpret_cast<sockaddr_in6*>(dns_server_pointer->Address.lpSockaddr);
				inet_ntop(AF_INET6, &socket_address_ipv6->sin6_addr, dns_address, sizeof(dns_address));
				tmp = dns_address;
				insert_if_unique<std::wstring, std::wstring>(network_devices_ordered, key, L"DNS: " + std::wstring(tmp.begin(), tmp.end()));
			}
		}
	}
	return network_devices_ordered;
}

// Function to display network information
std::wostream& network() noexcept{
	char host_name[MAX_COMPUTERNAME_LENGTH + 1];

	DWORD host_name_size, output_buffer_length, return_value, dword_size;
	ULONG buffer_length;
	WSADATA wsa_data;
	PIP_ADAPTER_INFO adapter_pointer;

	std::set<std::wstring> tcp_established_ordered, tcp_listening_ordered, tcp_closed_ordered, tcp_other_ordered;

	// Gets gateway information
	GetAdaptersInfo(nullptr, &buffer_length);
	std::unique_ptr<IP_ADAPTER_INFO, void(*)(void*)> adapter_pointer_info(static_cast<IP_ADAPTER_INFO*>
		(std::malloc(buffer_length)), std::free);

	if(!adapter_pointer_info)
		return std::wcerr << L'\t' << i18n_system::ERROR_MEMORY_ALLOCATION << L"\n\n";

	// Gets adapter information
	adapter_pointer = adapter_pointer_info.get();
	if(GetAdaptersInfo(adapter_pointer, &buffer_length) != NO_ERROR)
		return std::wcerr << L'\t' << i18n_system::ERROR_ADAPTERS_INFO << L"\n\n";

	// Gets Ethernet adapters and MAC addresses
	output_buffer_length = sizeof(IP_ADAPTER_ADDRESSES);

	std::unique_ptr<IP_ADAPTER_ADDRESSES, void(*)(void*)>adapter_addresses_ptr(static_cast<IP_ADAPTER_ADDRESSES*>
		(malloc(output_buffer_length)), std::free);

	if(!adapter_addresses_ptr)
		return std::wcerr << L'\t' << i18n_system::ERROR_MEMORY_ALLOCATION << L"\n\n";

	// Gets adapter addresses
	return_value = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_ALL_COMPARTMENTS, nullptr,
		adapter_addresses_ptr.get(), &output_buffer_length);
	if(return_value == ERROR_BUFFER_OVERFLOW){
        adapter_addresses_ptr.reset(static_cast<IP_ADAPTER_ADDRESSES*>(malloc(output_buffer_length)));
		if(!adapter_addresses_ptr) return std::wcerr << i18n_system::ERROR_MEMORY_ALLOCATION << L"\n\n";
		return_value = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_ALL_COMPARTMENTS, nullptr,
			adapter_addresses_ptr.get(), &output_buffer_length);
	}

	if(return_value != ERROR_SUCCESS)
		return std::wcerr << L'\t' << i18n_system::ERROR_ADAPTERS_ADDRESS << L"\n\n";

	// Initialize Winsock with RAII
	if(WSAStartup(MAKEWORD(2, 2), &wsa_data))
		return std::wcerr << L'\t'<< i18n_system::ERROR_WSA_STARTUP << L"\n\n";

	std::unique_ptr<void, decltype([](void*){ WSACleanup(); })> wsa_cleanup_ptr(reinterpret_cast<void*>(1));

	// First call to get required size
	GetExtendedUdpTable(nullptr, &dword_size, TRUE, AF_INET, UDP_TABLE_OWNER_PID, 0);

	std::unique_ptr<MIB_UDPTABLE_OWNER_PID, void(*)(void*)> udp_table_ptr(static_cast<PMIB_UDPTABLE_OWNER_PID>
		(malloc(dword_size)), std::free);

	if(!udp_table_ptr) return std::wcerr << L'\t' << i18n_system::ERROR_UDP_MALLOC << L"\n\n";

	// Gets the UDP table
	if(GetExtendedUdpTable(udp_table_ptr.get(), &dword_size, TRUE, AF_INET, UDP_TABLE_OWNER_PID, 0) != NO_ERROR)
		return std::wcerr << L'\t' << i18n_system::ERROR_EXTENDED_UDP_TABLE << L"\n\n";

	// Create span over the table entries
	std::span<MIB_UDPROW_OWNER_PID> udp_table_ptr_span{udp_table_ptr->table,
		(dword_size - sizeof(DWORD)) / sizeof(MIB_UDPROW_OWNER_PID)};

	// First call to get required size
	GetExtendedTcpTable(nullptr, &dword_size, TRUE, AF_INET, TCP_TABLE_OWNER_PID_ALL, 0);

	std::unique_ptr<MIB_TCPTABLE_OWNER_PID, void(*)(void*)> tcp_table_ptr(static_cast<PMIB_TCPTABLE_OWNER_PID>
		(malloc(dword_size)), std::free);

	if(!tcp_table_ptr) return std::wcerr << L'\t' << i18n_system::ERROR_TCP_MALLOC << L"\n\n";

	// Gets the TCP table
	if(GetExtendedTcpTable(tcp_table_ptr.get(), &dword_size, TRUE, AF_INET, TCP_TABLE_OWNER_PID_ALL, 0) != NO_ERROR)
		return std::wcerr << L'\t' << i18n_system::ERROR_EXTENDED_TCP_TABLE << L"\n\n";

	// Create span over the table entries
	std::span<MIB_TCPROW_OWNER_PID> tcp_table_ptr_span{tcp_table_ptr->table, tcp_table_ptr->dwNumEntries};

	// Prints adapters with MACs
	std::wcout << i18n::NETWORK << L'\n' << network_devices(adapter_pointer, adapter_addresses_ptr.get());

	// Prints host name
	host_name_size = sizeof(host_name) / sizeof(host_name[0]);
	if(!GetComputerName(host_name, &host_name_size))
		return std::wcerr << L'\t' << i18n_system::ERROR_HOST_NAME << L"\n\n";

	std::wcout << L'\t' << i18n::HOST_NAME << L"\n\t\t" << host_name

	// Prints UDP endpoints
	<< L"\n\n\t" << i18n::UDP_ENDPOINTS << L'\n';
	for(const std::wstring& udp_endpoint : udp_endpoints(udp_table_ptr_span))
		std::wcout << L"\t\t" << udp_endpoint << L'\n';

	// Prints TCP endpoints
	tcp_endpoints(tcp_table_ptr_span, tcp_closed_ordered, tcp_listening_ordered, tcp_established_ordered, tcp_other_ordered);
	std::wcout << L"\n\t" << i18n::TCP_ENDPOINTS << L"\n\t\t" << "ESTABLISHED:\n";
	for(const std::wstring& tcp_endpoint : tcp_established_ordered) std::wcout << L"\t\t\t" << tcp_endpoint << L'\n';

	std::wcout << L"\n\t\t" << "LISTENING:\n";
	for(const std::wstring& tcp_endpoint : tcp_listening_ordered) std::wcout << L"\t\t\t" << tcp_endpoint << L'\n';

	std::wcout << L"\n\t\t" << "CLOSED:\n";
	for(const std::wstring& tcp_endpoint : tcp_closed_ordered) std::wcout << L"\t\t\t" << tcp_endpoint << L'\n';

	std::wcout << L"\n\t\t" << "OTHER:\n";
	for(const std::wstring& tcp_endpoint : tcp_other_ordered) std::wcout << L"\t\t\t" << tcp_endpoint << L'\n';

	return std::wcout << L'\n';
}
