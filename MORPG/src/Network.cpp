#include "Network.hpp"
#include "Message.hpp"

Network::Network(MessageSystem * l_messageSystem) : Observer(System::S_Network, l_messageSystem), m_serverAddress(SERVERIP, SERVERPORT), m_client(-1), m_serverTimestep(100) {
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
		if (aux == -1) {
			break;
		}
		switch (msg.m_type) {
		case NetMessage::Type::SET_CLIENT_ID:
			{
				m_client = msg.m_serverData.m_clientID;
				m_serverTimestep = msg.m_serverData.m_serverTimestep;
				m_tick = msg.m_tick;
				printf("SERVER: \n\tClientID = %d\n\tTimeStep = %f\n\tTick = %d\n", m_client, m_serverTimestep, m_tick);

				Message msg(MessageType::M_SetServerTimestep, System::S_NetworkControl);
				msg.m_float = m_serverTimestep;
				Send(msg);

				msg.m_type = MessageType::M_GameObjectCreated;
				msg.m_gameObjectCreated.m_gameObjectIDs.first = m_client;
				msg.m_gameObjectCreated.m_gameObjectIDs.second = m_controlledGameObject;
				Send(msg);

				NetMessage netmsg;
				netmsg.m_type = NetMessage::Type::INITIAL_DATA;
				netmsg.m_initialData.m_clientID = m_client;
				std::string username("XxmusikitoxX");
				memcpy(netmsg.m_initialData.m_username, username.c_str(), username.length());
				netmsg.m_initialData.m_username[username.length()] = '\0';

				netmsg.m_initialData.x = 0;
				netmsg.m_initialData.y = 0;

				netmsg.m_tick = m_tick;

				printf("Sending Initial Data...\n");
				m_tcpWriteQueue.push(netmsg);
			}
			break;
		case NetMessage::Type::CAST_SPELL:
			{
				Message message(MessageType::M_CastSpell, System::S_NetworkControl);
				SpellData spell;
				spell.m_spellID = msg.m_spellData.m_spellID;
				spell.m_endTick = msg.m_spellData.m_endTick;

				message.m_gameObjectSpellData.second = spell;
				message.m_gameObjectSpellData.first = msg.m_spellData.m_clientID;
				printf("[%d] Casting spell %d in TICK %d, finishing in TICK %d (we are in tick %d)\n", msg.m_spellData.m_clientID, msg.m_spellData.m_spellID, msg.m_tick, msg.m_spellData.m_endTick, m_tick);

				Send(message);

			}
			break;
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
		//printf("\nReceived UDP Message - Type %d - %d bytes ", (int) msg.m_type, aux);
		//printf("from: %s:%d\n", inet_ntoa(addr.m_address.sin_addr), ntohs(addr.m_address.sin_port));

		if (msg.m_type == NetMessage::Type::DATA) {
			//printf("\t\t[T%d]SERVER: [%d] -> (%f, %f)\n", msg.m_tick, msg.m_data.m_clientID, msg.m_data.x, msg.m_data.y);
			if (msg.m_data.m_clientID != m_client) {
				Message msg(MessageType::M_Player, System::S_NetworkControl, msg.m_data.m_clientID, sf::Vector2f(msg.m_data.x, msg.m_data.y), msg.m_tick);
				Send(msg);
			}
		}
	} while (aux > 0);


}

void Network::WriteNetwork() {
	if (m_client == -1) {
		return;
	}

	while (!m_udpWriteQueue.empty()) {
		NetMessage message = m_udpWriteQueue.front();
		m_udpWriteQueue.pop();

		// TODO: check return value
		m_udpSocket->Send(&message, sizeof NetMessage, m_serverAddress);
		//printf("ME: [%d] -> (%f, %f)\n", message.m_data.m_clientID, message.m_data.x, message.m_data.y);
	}

	while (!m_tcpWriteQueue.empty()) {
		NetMessage message = m_tcpWriteQueue.front();
		m_tcpWriteQueue.pop();

		if (message.m_type == NetMessage::Type::INITIAL_DATA) {
			printf("INITIALDATA\n");
		}
		// TODO: check return value
		m_tcpSocket->Send(&message, sizeof NetMessage);
	}



}

void Network::SetControlledGameObject(GameObjectID l_id) {
	m_controlledGameObject = l_id;
}

void Network::Notify(Message l_message) {
	switch (l_message.m_type) {
	case MessageType::M_GameObject:
		{
			NetMessage message;
			message.m_type = NetMessage::Type::DATA;
			message.m_data.m_clientID = m_client;
			message.m_data.x = l_message.m_gameObject.m_position.x;
			message.m_data.y = l_message.m_gameObject.m_position.y;
			message.m_tick = m_tick;

			m_udpWriteQueue.push(message);
		}
		break;
	case MessageType::M_CastSpell:
		NetMessage message;
		message.m_type = NetMessage::Type::CAST_SPELL;
		message.m_spellData.m_clientID = m_client;
		message.m_spellData.m_spellID = l_message.m_spellData.m_spellID;
		message.m_spellData.m_duration = l_message.m_spellData.m_duration;
		message.m_tick = m_tick;
		printf("Cast spell %d! (%f seconds)\n", message.m_spellData.m_spellID, message.m_spellData.m_duration);
		m_tcpWriteQueue.push(message);
		break;
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