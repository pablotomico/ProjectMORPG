#include "Network.hpp"
#include "Message.hpp"

Network::Network(MessageSystem * l_messageSystem) : Observer(System::S_Network, l_messageSystem), m_serverAddress(SERVERIP, SERVERPORT), m_client(-1), m_serverTimestep(100), m_username(std::string("Unknown")) {
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

Network::Network(MessageSystem * l_messageSystem, std::string l_username) : Observer(System::S_Network, l_messageSystem), m_serverAddress(SERVERIP, SERVERPORT), m_client(-1), m_serverTimestep(100), m_username(l_username) {
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
	ReadTCP();
	ReadUDP();
}

void Network::WriteNetwork() {
	if (m_client == -1) {
		return;
	}

	WriteUDP();
	WriteTCP();
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

void Network::ReadTCP() {
	int spaceLeft = (sizeof m_tcpReadBuffer) - m_tcpReadCount;
	for (;;) {
		int count = m_tcpSocket->Receive(&m_tcpReadBuffer, sizeof NetMessage);
		if (count == SOCKET_ERROR) {
			return;
		}
		m_tcpReadCount += count;
		if (m_tcpReadCount >= sizeof NetMessage) {
			ProcessMessage((const NetMessage *) m_tcpReadBuffer);
			m_tcpReadCount = 0;
		} else {
			return;
		}
	}
}

void Network::ReadUDP() {
	int aux = 0;
	NetMessage msg;

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

		ProcessMessage(&msg);
	} while (aux > 0);
}

void Network::WriteTCP() {

	bool popMessage = false;



	while (!m_tcpWriteQueue.empty()) {
		NetMessage message = m_tcpWriteQueue.front();

		memcpy(m_tcpWriteBuffer, &message, sizeof NetMessage);

		for (;;) {
			int count = m_tcpSocket->Send((const char *) &m_tcpWriteBuffer + m_tcpWriteCount, (sizeof NetMessage) - m_tcpWriteCount);
			if (count == SOCKET_ERROR) {
				return;
			}
			m_tcpWriteCount += count;

			if (m_tcpWriteCount >= sizeof NetMessage) {
				popMessage = true;
				break;
			} else {
				return;
			}
		}

		if (popMessage) {
			m_tcpWriteQueue.pop();
			m_tcpWriteCount = 0;
		}
	}
	return;
}

void Network::WriteUDP() {
	while (!m_udpWriteQueue.empty()) {
		NetMessage message = m_udpWriteQueue.front();
		int count = m_udpSocket->Send(&message, sizeof NetMessage, m_serverAddress);
		if (count == sizeof NetMessage) {
			m_udpWriteQueue.pop();
		} else {
			return;
		}
		//printf("ME: [%d] -> (%f, %f)\n", message.m_data.m_clientID, message.m_data.x, message.m_data.y);
	}
}

void Network::ProcessMessage(const NetMessage* l_message) {
	switch (l_message->m_type) {
	case NetMessage::Type::SET_CLIENT_ID:
		{
			m_client = l_message->m_serverData.m_clientID;
			m_serverTimestep = l_message->m_serverData.m_serverTimestep;
			m_tick = l_message->m_tick;
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

			memcpy(netmsg.m_initialData.m_username, m_username.c_str(), m_username.length());
			netmsg.m_initialData.m_username[m_username.length()] = '\0';

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
			spell.m_spellID = l_message->m_spellData.m_spellID;
			spell.m_endTick = l_message->m_spellData.m_endTick;

			message.m_gameObjectSpellData.second = spell;
			message.m_gameObjectSpellData.first = l_message->m_spellData.m_clientID;
			printf("[%d] Casting spell %d in TICK %d, finishing in TICK %d (we are in tick %d)\n", l_message->m_spellData.m_clientID, l_message->m_spellData.m_spellID, l_message->m_tick, l_message->m_spellData.m_endTick, m_tick);

			Send(message);

		}
		break;
	case NetMessage::Type::DATA:
		//printf("\t\t[T%d]SERVER: [%d] -> (%f, %f)\n", msg.m_tick, msg.m_data.m_clientID, msg.m_data.x, msg.m_data.y);
		if (l_message->m_data.m_clientID != m_client) {
			Message msg(MessageType::M_Player, System::S_NetworkControl, l_message->m_data.m_clientID, sf::Vector2f(l_message->m_data.x, l_message->m_data.y), l_message->m_tick);
			Send(msg);
		}
		break;

	case NetMessage::Type::PLAYER_CONNECTED:
		{
			printf("%s connected!\n", l_message->m_playerData.m_username);
			Message msg(MessageType::M_PlayerConnected, System::S_NetworkControl);
			msg.m_playerData.m_id = l_message->m_playerData.m_clientID;
			memcpy(msg.m_playerData.m_username, l_message->m_playerData.m_username, sizeof PlayerData::m_username);
			Send(msg);
		}
		break;
	case NetMessage::Type::PLAYER_DISCONNECTED:
		{
			Message message(MessageType::M_GameObjectDeleted, System::S_NetworkControl);
			message.m_integer = l_message->m_playerData.m_clientID;
			Send(message);
		}
		break;
	}
}
