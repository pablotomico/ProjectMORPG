#pragma once

#include "Socket.hpp"

namespace net {
	class TCPSocket : public Socket {
	public:
		TCPSocket();

		bool Listen();
		bool Connect(const Address& l_address);
		SOCKET Accept(Address& l_clientAddress, int* l_size);
		int Send(const void* l_data, const std::size_t& l_size);
		int Receive(void* l_data, const std::size_t& l_size);
	};
}