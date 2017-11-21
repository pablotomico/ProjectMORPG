#include "Network.hpp"
#include <iostream>

Network::Network(HWND l_window) : m_availableID(0), m_window(l_window), m_clientCount(0), m_TCPWriteReady(false), m_UDPWriteReady(false) {
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

Client* Network::RegisterClient(SOCKET l_clientSocket, sockaddr_in l_address) {
	Client* client = new Client(m_availableID++, l_clientSocket, l_address);
	if (WSAAsyncSelect(client->m_tcpSocket, m_window, WM_SOCKET, FD_CLOSE | FD_READ | FD_WRITE) == SOCKET_ERROR) {
		printf("TCP Client WSAAsyncSelect failed\n");
		return nullptr;
	} else {
		printf("Registered new Client (id %d, SOCKET %d)\n", client->m_id, client->m_tcpSocket);
		m_clients[client->m_id] = client;
		m_clientSocket[client->m_tcpSocket] = client->m_id;
		++m_clientCount;
		return client;
	}
}

void Network::RemoveClient(ClientID l_clientID, SOCKET l_clientSocket) {
	Client* client = m_clients[l_clientID];
	m_clientSocket.erase(client->m_tcpSocket);
	delete client;
	m_clients.erase(l_clientID);
	--m_clientCount;
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
	for (;;) {
		sockaddr_in addr;
		int addrSize = sizeof(addr);
		int count = recvfrom(m_serverUDPSocket, m_udpReadBuffer + m_udpReadCount, spaceLeft, 0, (sockaddr *) &addr, &addrSize);
		if (count == SOCKET_ERROR) {
			if (WSAGetLastError() == WSAEWOULDBLOCK) {
				m_UDPWriteReady = false;
				break;
			} else {
				return true;
			}
		}
		m_udpReadCount += count;
	}

	if (m_udpReadCount < sizeof NetMessage) {
		return false;
	}
	ProcessMessage((const NetMessage *) m_udpReadBuffer);
	m_udpReadCount = 0;

	return false;
}

bool Network::WriteUDP() {

	while (!m_udpMsgQueue.empty()) {
		NetMessage message = m_udpMsgQueue.front();
		m_udpMsgQueue.pop();

		for (auto& itr = m_clients.begin(); itr != m_clients.end(); ++itr) {
			printf("TO->[%d]\tINFO:[%d] -> (%f, %f) Type %d\n", itr->first, message.m_data.m_clientID, message.m_data.x, message.m_data.y, (int) message.m_type);
			int count = sendto(m_serverUDPSocket, (const char*) &message, sizeof NetMessage, 0, (const sockaddr *) &(itr->second->m_address), sizeof(itr->second->m_address));
			if (count == SOCKET_ERROR) {
				if (WSAGetLastError() == WSAEWOULDBLOCK) {
					break;
				}
				printf("[!!!]Error sending updated to client!\n");
			}
		}

	}
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

		NetMessage message;
		message.m_type = l_message->m_type;
		message.m_data = l_message->m_data;
		m_udpMsgQueue.push(message);
	}
}


