#include "Network.hpp"
#include "Message.hpp"

Network::Network(MessageSystem * l_messageSystem) : Observer(System::S_Network, l_messageSystem) {
	StartWinSock();

	// creating UDP socket
	m_socket = socket(AF_INET, SOCK_DGRAM, 0);
	if (m_socket == INVALID_SOCKET) {
		std::cerr << "Error creating socket";
	}

	// Fill the server address
	m_serverAddr.sin_family = AF_INET;
	m_serverAddr.sin_port = htons(SERVERPORT);
	m_serverAddr.sin_addr.s_addr = inet_addr(SERVERIP);

	// Set socket in the socketset
	FD_ZERO(&m_socketSetWrite);
	FD_ZERO(&m_socketSetRead);
	

	// Define the timeout (0 = non-blocking)
	m_timeout.tv_sec = 0;
	m_timeout.tv_usec = 1;


	// ASK SERVER FOR CLIENT ID
	m_client = 1;


	std::cout << "Network system initialized!" << std::endl;
}

Network::~Network() {}

void Network::Update() {}

void Network::ReadNetwork() {
	FD_ZERO(&m_socketSetRead);
	FD_SET(m_socket, &m_socketSetRead);
	// Select socket for reading
	int count = select(0, &m_socketSetRead, NULL, NULL, &m_timeout);
	if (count == SOCKET_ERROR) {
		std::cerr << "select failed";
	}
	if (FD_ISSET(m_socket, &m_socketSetRead)) {
		NetMessage message;
		sockaddr_in fromAddr;
		int fromAddrSize = sizeof(fromAddr);
		int count = recvfrom(m_socket, (char *) &message, sizeof NetMessage, 0, (sockaddr *) &fromAddr, &fromAddrSize);
		if (count < 0) {
			std::cerr << "recvfrom failed";
		}
		if (count != sizeof Message) {
			std::cerr << "received odd-sized message";
		}
		printf("\t\t\tSERVER: [%d] -> (%f, %f)\n", message.m_client, message.m_x, message.m_y);
	}
}

void Network::WriteNetwork() {
	FD_ZERO(&m_socketSetWrite);
	FD_SET(m_socket, &m_socketSetWrite);
	// Select socket for writing
	int count = select(0, NULL, &m_socketSetWrite, NULL, &m_timeout);
	if (count == SOCKET_ERROR) {
		std::cerr << "select failed";
	}
	if (FD_ISSET(m_socket, &m_socketSetWrite)) {
		NetMessage message;
		message.m_client = m_client;
		message.m_x = 0;
		message.m_y = 0;

		printf("ME: [%d] -> (%f, %f)\n", message.m_client, message.m_x, message.m_y);

		// Send the message to the server.
		if (sendto(m_socket, (const char *) &message, sizeof NetMessage, 0, (const sockaddr *) &m_serverAddr, sizeof(m_serverAddr)) != sizeof NetMessage) {
			std::cerr << "sendto failed";
		}
	}
}

void Network::Notify(Message l_message) {
	if (l_message.m_type == MessageType::M_GameObject) {
		//l_message.m_gameObject.Print();
	}
}


void Network::StartWinSock() {
	// We want version 2.2.
	WSADATA w;
	int error = WSAStartup(0x0202, &w);
	if (error != 0) {
		std::cerr << "WSAStartup failed";
	}
	if (w.wVersion != 0x0202) {
		std::cerr << "Wrong WinSock version";
	}

}