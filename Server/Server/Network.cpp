#include "Network.hpp"
#include <iostream>

Network::Network(HWND l_window) : m_availableID(0), m_window(l_window) {
	StartWinSock();
	m_serverTCPSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_serverTCPSocket == INVALID_SOCKET) {
		std::cerr << "Failed creating server TCP socket\n";
	}
	m_serverUDPSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if (m_serverUDPSocket == INVALID_SOCKET) {
		std::cerr << "Failed creating server UDP socket\n";
	}


	struct addrinfo *ai = nullptr;
	if (getaddrinfo(SERVERIP, SERVERPORT, nullptr, &ai) != 0) {
		std::cerr << "getaddrinfo failed\n";
	}
	if (ai->ai_family != AF_INET) {
		std::cerr << "getaddinfo didn't return an AF_INET address\n";
	}
	if (bind(m_serverTCPSocket, ai->ai_addr, ai->ai_addrlen) != 0) {
		std::cerr << "bind failed\n";
	}
	if (listen(m_serverTCPSocket, 1) != 0) {
		std::cerr << "listen failed\n";
	}

	if (bind(m_serverUDPSocket, ai->ai_addr, ai->ai_addrlen) != 0) {
		std::cerr << "bind failed\n";
	}

	if (WSAAsyncSelect(m_serverTCPSocket, m_window, WM_SOCKET, FD_CLOSE | FD_CONNECT | FD_READ | FD_ACCEPT) == SOCKET_ERROR) {
		std::cerr << "WSAAsyncSelect failed\n";
	}
	if (WSAAsyncSelect(m_serverUDPSocket, m_window, WM_SOCKET, FD_READ | FD_WRITE) == SOCKET_ERROR) {
		std::cerr << "WSAAsyncSelect failed\n";
	}


	printf("Server ready!\n TCPSocket %d, UDPSocket %d\n", m_serverTCPSocket, m_serverUDPSocket);
}

Network::~Network() {
	while (m_clients.begin() != m_clients.end()) {
		delete m_clients.begin()->second;
		m_clients.erase(m_clients.begin()->first);
	}
	WSACleanup();
}

Client* Network::RegisterClient(SOCKET l_clientSocket) {
	Client* client = new Client(m_availableID++, l_clientSocket);
	if (WSAAsyncSelect(client->m_tcpSocket, m_window, WM_SOCKET, FD_CLOSE | FD_READ | FD_WRITE) == SOCKET_ERROR) {
		printf("TCP Client WSAAsyncSelect failed\n");
		return nullptr;
	} else {
		printf("Registered new Client (id %d, SOCKET %d)\n", client->m_id, client->m_tcpSocket);
		m_clients[client->m_id] = client;
		m_clientSocket[client->m_tcpSocket] = client->m_id;
		return client;
	}
}

void Network::RemoveClient(ClientID l_clientID, SOCKET l_clientSocket) {
	Client* client = m_clients[l_clientID];
	m_clientSocket.erase(client->m_tcpSocket);
	delete client;
	m_clients.erase(l_clientID);
}

ClientID Network::GetClientID(SOCKET l_clientSocket) {
	if (m_clientSocket.find(l_clientSocket) == m_clientSocket.end()) {
		return -1;
	}
	return m_clientSocket[l_clientSocket];
}

Client * Network::GetClient(ClientID l_clientID) {
	if (m_clients.find(l_clientID) == m_clients.end()) {
		return nullptr;
	}
	return m_clients[l_clientID];
}

bool Network::ReadUDP() {
	// Receive as much data from the client as will fit in the buffer.
	int spaceLeft = (sizeof m_udpReadBuffer) - m_udpReadCount;
	bool done = false;
	for (;;) {
		int count = recv(m_serverUDPSocket, m_udpReadBuffer + m_udpReadCount, spaceLeft, 0);
		if (count == SOCKET_ERROR) {
			if (WSAGetLastError() == WSAEWOULDBLOCK) {
				// That's as much data as we're going to get this time!
				// We need to wait for another FD_READ message.
				break;
			} else {
				return true;
			}
		}
		// We've successfully read some more data into the buffer.
		m_udpReadCount += count;
	}

	if (m_udpReadCount < sizeof NetMessage) {
		// ... but we've not received a complete message yet.
		// So we can't do anything until we receive some more.
		return false;
	}

	// We've got a complete message.
	ProcessMessage((const NetMessage *) m_udpReadBuffer);

	// Clear the buffer, ready for the next message.
	m_udpReadCount = 0;

	return false;

}


void Network::StartWinSock() {
	WSADATA w;
	int error = WSAStartup(0x0202, &w);
	if (error != 0) {
		std::cerr << "WSAStartup failed\n";
	}
	if (w.wVersion != 0x0202) {
		std::cerr << "Wrong WinSock version\n";
	}
}

void Network::ProcessMessage(const NetMessage* l_message) {
	if (l_message->m_type == NetMessage::Type::DATA) {
		printf("[%d] -> (%f, %f)\n", l_message->m_data.m_clientID, l_message->m_data.x, l_message->m_data.y);
	}

}
