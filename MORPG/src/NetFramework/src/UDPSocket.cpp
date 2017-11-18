#include "../include/UDPSocket.hpp"

net::UDPSocket::UDPSocket(Socket::SocketMode l_mode) {
	m_socket = socket(AF_INET, SOCK_DGRAM, 0);
	if (m_socket == INVALID_SOCKET) {
		std::cerr << "Error creating UDP Socket";
	}
	m_mode = l_mode;
}


char * net::UDPSocket::Receive(int l_length) {
	char* message;
	int size = sizeof m_lastAddress;
	int count = recvfrom(m_socket, (char *) &message, l_length, 0, (sockaddr *) &m_lastAddress, &size);
	if (count < 0) {
		std::cerr << "recvfrom failed";
	}
	return message;
}

net::Address net::UDPSocket::GetLastAddress() {
	return m_lastAddress;
}

bool net::UDPSocket::Send(net::Address l_address, const char * l_message, int l_length) {
	return (sendto(m_socket, l_message, l_length, 0, (const sockaddr *) &l_address.m_address, sizeof(l_address.m_address)) == l_length);
}

