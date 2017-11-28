#include "NetworkControl.hpp"

NetworkControl::NetworkControl(MessageSystem * l_messageSystem) : Observer(System::S_NetworkControl, l_messageSystem) {}

void NetworkControl::RegisterClient(int l_clientID) {
	m_clientGameObjectMap[l_clientID] = -1;
	Message msg(MessageType::M_GameObjectCreated, System::S_GameObjectManager);
	msg.m_gameObjectCreated.m_gameObjectIDs.first = l_clientID;
	msg.m_gameObjectCreated.m_timestep = m_timestep;
	Send(msg);
}

void NetworkControl::AddClientGameObject(int l_clientID, GameObjectID l_gameObjectID) {
	printf("Adding Client GameObject %d-%d\n", l_clientID, l_gameObjectID);
	m_clientGameObjectMap[l_clientID] = l_gameObjectID;
}

void NetworkControl::UpdateClient(int l_clientID, float l_x, float l_y, int l_tick) {
	GameObjectID goID = m_clientGameObjectMap[l_clientID];
	//printf("Updating Client %d GameObject %d\n", l_clientID, goID);
	Message msg(MessageType::M_GameObject, System::S_GameObjectManager, goID, sf::Vector2f(l_x, l_y), l_tick);
	Send(msg);
}


void NetworkControl::RemoveClient(int l_clientID) {}

bool NetworkControl::HasClient(int l_clientID) {
	return (m_clientGameObjectMap.find(l_clientID) != m_clientGameObjectMap.end());
}

void NetworkControl::Notify(Message l_message) {
	switch (l_message.m_type) {
	case MessageType::M_Player:
		//printf("\t\tSERVER: [%d] -> (%f, %f)\n", l_message.m_gameObject.m_gameObjectID, l_message.m_gameObject.m_position.x, l_message.m_gameObject.m_position.y);
		if (!HasClient(l_message.m_gameObject.m_gameObjectID)) {
			printf("Registering Client %d\n", l_message.m_gameObject.m_gameObjectID);
			RegisterClient(l_message.m_gameObject.m_gameObjectID);
		} else if (m_clientGameObjectMap[l_message.m_gameObject.m_gameObjectID] != -1) {
			UpdateClient(l_message.m_gameObject.m_gameObjectID, l_message.m_gameObject.m_position.x, l_message.m_gameObject.m_position.y, l_message.m_gameObject.m_tick);
		}

		break;
	case MessageType::M_GameObjectCreated:
		AddClientGameObject(l_message.m_gameObjectCreated.m_gameObjectIDs.first, l_message.m_gameObjectCreated.m_gameObjectIDs.second);
		break;
	case MessageType::M_SetServerTimestep:
		m_timestep = l_message.m_float;
		break;
	}
}
