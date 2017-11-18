#pragma once

#include "Common.hpp"
#include "Address.hpp"

namespace net {

	class Socket {
	public:
		enum SocketMode {
			None,
			Read,
			Write
		};
	public:
		void Close() {
			closesocket(m_socket);
		}

	protected:
		SOCKET m_socket;
		SocketMode m_mode;
	};
}