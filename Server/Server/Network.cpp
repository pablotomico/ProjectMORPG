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

	printf("Server socket listening\n");

	if (WSAAsyncSelect(m_serverTCPSocket, m_window, WM_SOCKET, FD_CLOSE | FD_CONNECT | FD_READ | FD_ACCEPT) == SOCKET_ERROR) {
		std::cerr << "WSAAsyncSelect failed\n";
	}
	if (WSAAsyncSelect(m_serverUDPSocket, m_window, WM_SOCKET, FD_READ | FD_WRITE) == SOCKET_ERROR) {
		std::cerr << "WSAAsyncSelect failed\n";
	}
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
