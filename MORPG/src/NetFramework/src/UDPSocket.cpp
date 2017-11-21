#include "../include/UDPSocket.hpp"

net::UDPSocket::UDPSocket() {
	m_socket = socket(AF_INET, SOCK_DGRAM, 0);
	if (m_socket == INVALID_SOCKET) {
		std::cerr << "Error creating UDP Socket" << std::endl;
	}
}


int net::UDPSocket::Receive(void * l_data, std::size_t l_size, net::Address & l_address, int* size) {
	return recvfrom(m_socket, (char *) l_data, l_size, 0, (sockaddr *) &l_address.m_address, size);
}


int net::UDPSocket::Send(const void * l_data, std::size_t l_size, const net::Address & l_address) {
	return sendto(m_socket, (char *)l_data, l_size, 0, (const sockaddr *) &l_address.m_address, sizeof(l_address.m_address));
}

