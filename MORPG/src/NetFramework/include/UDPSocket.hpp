#pragma once

#include "Socket.hpp"

namespace net {
	class UDPSocket : public Socket {
	public:
		UDPSocket(Socket::SocketMode l_mode = Socket::SocketMode::None);

		char* Receive(int l_length);
		Address GetLastAddress();

		bool Send(net::Address l_address, const char * l_message, int l_length);
	private:
		Address m_lastAddress;
	};
}