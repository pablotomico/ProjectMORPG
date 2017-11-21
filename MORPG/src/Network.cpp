#include "Network.hpp"
#include "Message.hpp"

Network::Network(MessageSystem * l_messageSystem) : Observer(System::S_Network, l_messageSystem), m_serverAddress(SERVERIP, SERVERPORT), m_client(-1) {
	StartWinSock();

	m_tcpSocket = new net::TCPSocket();
	m_udpSocket = new net::UDPSocket();

	m_tcpSocket->Bind();
	net::Address addr;
	m_tcpSocket->GetBindedAddress(addr);
	m_udpSocket->Bind(addr);

	m_tcpSocket->SetBlocking(false);
	m_udpSocket->SetBlocking(false);

	m_tcpSocket->Connect(m_serverAddress);

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
		aux = m_tcpSocket->Receive(&msg, sizeof NetMessage);
		if (msg.m_type == NetMessage::Type::SET_CLIENT_ID) {
			printf("SERVER: Set ClientID to %d\n", msg.m_clientID);
			m_client = msg.m_clientID;
		}

		//printf("\t\t\tSERVER: [%d] -> (%f, %f)\n", msg.m_client, msg.m_x, msg.m_y);
	} while (aux > 0);

	do {
		net::Address addr;
		int i = sizeof addr.m_address;
		aux = m_udpSocket->Receive(&msg, sizeof NetMessage, addr, &i);
		if (aux == SOCKET_ERROR) {
			if (WSAGetLastError() == WSAEWOULDBLOCK) {
				break;
			}
			std::cerr << "Error receiving!\n";
		} 
		printf("\nReceived UDP Message - Type %d - %d bytes ", (int) msg.m_type, aux);
		printf("from: %s:%d\n", inet_ntoa(addr.m_address.sin_addr), ntohs(addr.m_address.sin_port));

		if (msg.m_type == NetMessage::Type::DATA) {
			printf("\t\tSERVER: [%d] -> (%f, %f)\n", msg.m_data.m_clientID, msg.m_data.x, msg.m_data.y);
		}
	} while (aux > 0);


}

void Network::WriteNetwork() {
	if (m_client == -1) {
		return;
	}
	NetMessage message;
	message.m_type = NetMessage::Type::DATA;
	message.m_data.m_clientID = m_client;
	message.m_data.x = 0;
	message.m_data.y = 0;


	//printf("ME: [%d] -> (%f, %f)\n", message.m_data.m_clientID, message.m_data.x, message.m_data.y);

	m_udpSocket->Send(&message, sizeof NetMessage, m_serverAddress);
	m_tcpSocket->Send(&message, sizeof NetMessage);
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