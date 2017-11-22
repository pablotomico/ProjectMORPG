#include "GameObjectManager.hpp"

GameObjectManager::GameObjectManager(MessageSystem* l_messageSystem, TextureManager* l_textureManager)
	: Observer(System::S_GameObjectManager, l_messageSystem), m_nextAvailableID(0) {
	m_textureManager = l_textureManager;
}

GameObjectManager::~GameObjectManager() {
	// Destroy GameObjects
	while (m_gameObjects.begin() != m_gameObjects.end()) {
		delete m_gameObjects.begin()->second;
		m_gameObjects.erase(m_gameObjects.begin());
	}
}

GameObjectContainer * GameObjectManager::GetGameObjectContainer() {
	return &m_gameObjects;
}

GameObject * GameObjectManager::GetGameObject(GameObjectID l_id) {
	return m_gameObjects.at(l_id);
}

GameObjectID GameObjectManager::CreateGameObject(bool l_drawable, bool l_controllable, const std::string& l_texture) {
	GameObject* gameObject = new GameObject(m_nextAvailableID, l_drawable, l_controllable, m_textureManager);
	if (l_drawable) {
		gameObject->SetSprite(l_texture);
	}
	m_gameObjects[m_nextAvailableID] = gameObject;
	printf("[SYSTEM] Creating gameobject %d\n", m_nextAvailableID);
	return m_nextAvailableID++; // returns the ID and adds 1
}

void GameObjectManager::UpdateGameObject(GameObjectID l_id, sf::Vector2f l_position) {
	GameObject* gameObject = GetGameObject(l_id);
	gameObject->SetPosition(l_position);
}

void GameObjectManager::RemoveGameObject(GameObjectID l_id) {}

void GameObjectManager::Notify(Message l_message) {
	switch (l_message.m_type) {
	case MessageType::M_GameObjectCreated:
		{
			GameObjectID id = CreateGameObject(true, true, "character_1.png");
			GameObject* gameobject = GetGameObject(id);
			gameobject->SetSpriteScale(0.25f, 0.25f);

			Message msg(MessageType::M_GameObjectCreated, System::S_NetworkControl);
			msg.m_gameObjectIDs.first = l_message.m_gameObjectIDs.first;
			msg.m_gameObjectIDs.second = id;
			Send(msg);
			break;
		}
	case MessageType::M_GameObject:
		{
			UpdateGameObject(l_message.m_gameObject.m_gameObjectID, l_message.m_gameObject.m_position); // TODO: time stamp here
		}
		break;
	}
}
