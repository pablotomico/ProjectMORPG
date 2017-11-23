#include "Client.hpp"
#include <iostream>

Client::Client(ClientID l_id) : m_id(l_id), m_tcpWriteCount(0) {
	m_tcpSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_tcpSocket == INVALID_SOCKET) {
		std::cerr << l_id << "] Failed creating tcp socket\n";
	}
}

Client::Client(ClientID l_id, SOCKET l_socket, sockaddr_in l_address) : m_id(l_id), m_address(l_address) {
	m_tcpSocket = l_socket;
}

Client::~Client() {
	closesocket(m_tcpSocket);
}
