#pragma once
#include <string>
#include "Common.hpp"

namespace net {

	class Address {
	public:
		Address();
		Address(const std::string& l_address, const USHORT& l_portNumber);
		Address(const char* l_address, const USHORT& l_portNumber);

	public:
		sockaddr_in m_address;
	};
}