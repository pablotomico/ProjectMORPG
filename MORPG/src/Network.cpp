#include "Network.hpp"
#include "Message.hpp"

Network::Network(MessageSystem * l_messageSystem) : Observer(System::S_Network, l_messageSystem), m_serverAddress(SERVERIP, SERVERPORT) {
	StartWinSock();

	m_udpSocket = new net::UDPSocket();
	m_tcpSocket = new net::TCPSocket();

	m_udpSocket->SetBlocking(false);
	m_tcpSocket->SetBlocking(false);
	
	m_tcpSocket->Connect(m_serverAddress);

	// ASK SERVER FOR CLIENT ID
	m_client = 1;


	std::cout << "Network system initialized!" << std::endl;
}

Network::~Network() {
	delete m_udpSocket;
	delete m_tcpSocket;
}

void Network::Update() {}

void Network::ReadNetwork() {

	int aux = 0;
	NetMessage msg;
	do {
		aux = m_udpSocket->Receive(&msg, sizeof NetMessage, m_serverAddress);
		//printf("\t\t\tSERVER: [%d] -> (%f, %f)\n", msg.m_client, msg.m_x, msg.m_y);
	} while (aux > 0);

	do {
		aux = m_tcpSocket->Receive(&msg, sizeof NetMessage);
		if (msg.m_type == NetMessage::Type::SET_CLIENT_ID) {
			printf("SERVER: Set ClientID to %d\n", msg.m_clientID);
		}

		//printf("\t\t\tSERVER: [%d] -> (%f, %f)\n", msg.m_client, msg.m_x, msg.m_y);
	} while (aux > 0);
	
}

void Network::WriteNetwork() {
	/*
	NetMessage message;
	message.m_client = m_client;
	message.m_x = 0;
	message.m_y = 0;

	printf("ME: [%d] -> (%f, %f)\n", message.m_client, message.m_x, message.m_y);

	m_udpSocket->Send(&message, sizeof NetMessage, m_serverAddress);
	*/
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