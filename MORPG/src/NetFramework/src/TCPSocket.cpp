#include "../include/TCPSocket.hpp"
#include "../../Util/Utilities.hpp"

net::TCPSocket::TCPSocket() {
	m_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_socket == INVALID_SOCKET) {
		DEBUG("Error creating TCP Socket");
	}
}

bool net::TCPSocket::Listen() {
	if (listen(m_socket, 1) != 0) {
		DEBUG("listen failed");
		return false;
	}
	return true;
}

bool net::TCPSocket::Connect(const Address& l_address) {
	LOG("Connecting to: " << inet_ntoa(l_address.m_address.sin_addr) << ":" << ntohs(l_address.m_address.sin_port));
	if (connect(m_socket, (const sockaddr *) &(l_address.m_address), sizeof l_address.m_address) == SOCKET_ERROR) {
		if (WSAGetLastError() == WSAEWOULDBLOCK) {
			return true;
		}
		return false;
	}
	return true;
}

SOCKET net::TCPSocket::Accept(Address& l_clientAddress, int* l_size) {
	return accept(m_socket, (sockaddr *)&l_clientAddress.m_address, l_size);
}

int net::TCPSocket::Send(const void * l_data, const std::size_t& l_size) {
	return send(m_socket, (char *)l_data, l_size, 0);
}

int net::TCPSocket::Receive(void * l_data, const std::size_t& l_size) {
	return recv(m_socket, (char *)l_data, l_size, 0);
}

