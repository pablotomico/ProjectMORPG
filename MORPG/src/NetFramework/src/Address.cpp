#include "../include/Address.hpp"

net::Address::Address() {}

net::Address::Address(const std::string & l_address, const USHORT& l_portNumber) {
	m_address.sin_family = AF_INET;
	m_address.sin_port = htons(l_portNumber);
	m_address.sin_addr.s_addr = inet_addr(l_address.c_str());
}

net::Address::Address(const char * l_address, const USHORT& l_portNumber) {
	m_address.sin_family = AF_INET;
	m_address.sin_port = htons(l_portNumber);
	m_address.sin_addr.s_addr = inet_addr(l_address);
}
