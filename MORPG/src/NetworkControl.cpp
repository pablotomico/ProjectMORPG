#include "NetworkControl.hpp"

NetworkControl::NetworkControl(const std::shared_ptr<MessageSystem>& l_messageSystem)
	: Observer(System::S_NetworkControl, l_messageSystem) {}

void NetworkControl::RegisterClient(const int& l_clientID) {
	m_clientGameObjectMap[l_clientID] = -1;
	Message msg(MessageType::M_GameObjectCreated, System::S_GameObjectManager);
	msg.m_gameObjectCreated.m_gameObjectIDs.first = l_clientID;
	msg.m_gameObjectCreated.m_timestep = m_timestep;
	Send(msg);
}
void NetworkControl::RegisterClient(const int& l_clientID, const std::string& l_username) {
	m_clientGameObjectMap[l_clientID] = -1;
	Message msg(MessageType::M_GameObjectCreated, System::S_GameObjectManager);
	msg.m_gameObjectCreated.m_gameObjectIDs.first = l_clientID;
	msg.m_gameObjectCreated.m_timestep = m_timestep;
	memcpy(msg.m_gameObjectCreated.m_name, l_username.c_str(), l_username.size());
	Send(msg);
}

void NetworkControl::AddClientGameObject(const int& l_clientID, const GameObjectID& l_gameObjectID) {
	//printf("Adding Client GameObject %d-%d\n", l_clientID, l_gameObjectID);
	m_clientGameObjectMap[l_clientID] = l_gameObjectID;
}

void NetworkControl::UpdateClient(const int& l_clientID, const float& l_x, const float& l_y, const int& l_tick) {
	GameObjectID goID = m_clientGameObjectMap[l_clientID];
	//printf("Updating Client %d GameObject %d\n", l_clientID, goID);
	Message msg(MessageType::M_GameObject, System::S_GameObjectManager, goID, sf::Vector2f(l_x, l_y), l_tick);
	Send(msg);
}


void NetworkControl::RemoveClient(const int& l_clientID) {
	if (!HasClient(l_clientID)) {
		return;
	}

	Message msg(MessageType::M_GameObjectDeleted, System::S_GameObjectManager);
	msg.m_integer = m_clientGameObjectMap[l_clientID];
	Send(msg);

	m_clientGameObjectMap.erase(l_clientID);
}

bool NetworkControl::HasClient(const int& l_clientID) {
	return (m_clientGameObjectMap.find(l_clientID) != m_clientGameObjectMap.end());
}

void NetworkControl::Notify(const Message& l_message) {
	switch (l_message.m_type) {
	case MessageType::M_Player:
		//printf("\t\tSERVER: [%d] -> (%f, %f)\n", l_message.m_gameObject.m_gameObjectID, l_message.m_gameObject.m_position.x, l_message.m_gameObject.m_position.y);
		if (!HasClient(l_message.m_gameObject.m_gameObjectID)) {
			//printf("Registering Client %d\n", l_message.m_gameObject.m_gameObjectID);
			//RegisterClient(l_message.m_gameObject.m_gameObjectID);
		}

		if (HasClient(l_message.m_gameObject.m_gameObjectID) && m_clientGameObjectMap[l_message.m_gameObject.m_gameObjectID] != -1) {
			UpdateClient(l_message.m_gameObject.m_gameObjectID, l_message.m_gameObject.m_position.x, l_message.m_gameObject.m_position.y, l_message.m_gameObject.m_tick);
		}

		break;
	case MessageType::M_GameObjectCreated:
		AddClientGameObject(l_message.m_gameObjectCreated.m_gameObjectIDs.first, l_message.m_gameObjectCreated.m_gameObjectIDs.second);
		break;
	case MessageType::M_GameObjectDeleted:
		RemoveClient(l_message.m_integer);
		break;
	case MessageType::M_SetServerTimestep:
		m_timestep = l_message.m_float;
		break;
	case MessageType::M_CastSpell:
	{
		Message message(l_message);
		message.m_systemReceiver = System::S_GameObjectManager;
		GameObjectID id = m_clientGameObjectMap[message.m_gameObjectSpellData.first];
		message.m_gameObjectSpellData.first = id;

		Send(message);
	}
	break;
	case MessageType::M_PlayerConnected:
	{
		if (!HasClient(l_message.m_playerData.m_id)) {
			//printf("Registering Client %d\n", l_message.m_gameObject.m_gameObjectID);
			RegisterClient(l_message.m_playerData.m_id, std::string(l_message.m_playerData.m_username));
		}
	}
	break;
	}
}
