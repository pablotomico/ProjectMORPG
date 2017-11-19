#pragma once

#include "Common.hpp"
#include "Address.hpp"

namespace net {

	class Socket {
	public:
		void Close() {
			closesocket(m_socket);
		}

		void SetBlocking(bool l_blocking) {
			unsigned long on = (l_blocking == false);
			if (0 != ioctlsocket(m_socket, FIONBIO, &on)) {
				/* Handle failure. */
			}
		}

	protected:
		SOCKET m_socket;
	};
}