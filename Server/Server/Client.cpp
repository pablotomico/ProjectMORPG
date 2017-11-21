#include "Client.hpp"
#include <iostream>

Client::Client(ClientID l_id) : m_id(l_id) {
	m_tcpSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_tcpSocket == INVALID_SOCKET) {
		std::cerr << l_id << "] Failed creating tcp socket\n";
	}
}

Client::Client(ClientID l_id, SOCKET l_socket) : m_id(l_id) {
	m_tcpSocket = l_socket;
}

Client::~Client() {
	closesocket(m_tcpSocket);
}
