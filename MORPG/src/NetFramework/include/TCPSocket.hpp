#pragma once

#include "Socket.hpp"

namespace net {
	class TCPSocket : public Socket {
	public:
		bool Bind(Address l_address);
		bool Listen();
		bool Connect();
		bool Accept(Address l_clientAddress);
		int Send(char* l_message);
		char* Receive();
	};
}