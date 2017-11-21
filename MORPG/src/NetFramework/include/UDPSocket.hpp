#pragma once

#include "Socket.hpp"

namespace net {
	class UDPSocket : public Socket {
	public:
		UDPSocket();

		int Receive(void* l_data, std::size_t l_size, net::Address& l_address, int* size);

		int Send(const void* l_data, std::size_t l_size, const net::Address& l_address);
	};
}