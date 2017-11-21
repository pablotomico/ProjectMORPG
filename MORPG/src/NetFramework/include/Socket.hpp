#pragma once

#include "Common.hpp"
#include "Address.hpp"

namespace net {

	class Socket {
	public:
		void Close() {
			closesocket(m_socket);
		}

		SOCKET GetSocket() {
			return m_socket;
		}

		void Bind() {
			sockaddr_in addr;
			addr.sin_family = AF_INET;
			addr.sin_addr.s_addr = INADDR_ANY;
			addr.sin_port = htons(INADDR_ANY);
			printf("Bind(): %s:%d\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
			if (bind(m_socket, (const sockaddr *) &addr, sizeof(addr)) < 0) {
				std::cerr << "Bind() failed\n";
			}
		}

		void Bind(Address l_address) {
			printf("Bind(Address): %s:%d\n", inet_ntoa(l_address.m_address.sin_addr), ntohs(l_address.m_address.sin_port));
			if (bind(m_socket, (const sockaddr *) &l_address.m_address, sizeof(l_address.m_address)) < 0) {
				std::cerr << "Bind(Address) failed\n";
			}
		} 

		void GetBindedAddress(Address& l_address) {
			int size = sizeof(l_address.m_address);
			getsockname(m_socket, (sockaddr *) &l_address.m_address, &size);
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