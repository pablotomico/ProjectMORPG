#pragma once

#include "Socket.hpp"

namespace net {
	class TCPSocket : public Socket {
	public:
		TCPSocket();

		bool Bind(Address l_address);
		bool Listen();
		bool Connect(Address l_address);
		SOCKET Accept(Address& l_clientAddress, int* l_size);
		int Send(const void* l_data, std::size_t l_size);
		int Receive(void* l_data, std::size_t l_size);
	};
}