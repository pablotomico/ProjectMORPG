#include "../include/TCPSocket.hpp"

net::TCPSocket::TCPSocket() {
	m_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_socket == INVALID_SOCKET) {
		std::cerr << "Error creating TCP Socket" << std::endl;
	}
}

bool net::TCPSocket::Bind(Address l_address) {

	if (bind(m_socket, (const sockaddr *) &l_address.m_address, sizeof l_address.m_address) != 0) {
		std::cerr << "bind failed\n";
		return false;
	}
	return true;
}

bool net::TCPSocket::Listen() {
	if (listen(m_socket, 1) != 0) {
		std::cerr << "listen failed\n";
		return false;
	}
	return true;
}

bool net::TCPSocket::Connect(Address l_address) {
	printf("Connecting to: %s:%d\n", inet_ntoa(l_address.m_address.sin_addr), ntohs(l_address.m_address.sin_port));
	if (connect(m_socket, (const sockaddr *) &(l_address.m_address), sizeof l_address.m_address) == SOCKET_ERROR) {
		if (WSAGetLastError() == WSAEWOULDBLOCK) {
			return true;
		}
		return false;
	}
	return true;
}

SOCKET net::TCPSocket::Accept(Address& l_clientAddress, int* l_size) {
	return accept(m_socket, (sockaddr *) &l_clientAddress.m_address, l_size);
}

int net::TCPSocket::Send(const void * l_data, std::size_t l_size) {
	return send(m_socket, (char *) l_data, l_size, 0);
}

int net::TCPSocket::Receive(void * l_data, std::size_t l_size) {
	return recv(m_socket, (char *) l_data, l_size, 0);
}

