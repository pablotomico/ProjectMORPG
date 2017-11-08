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

GameObjectID GameObjectManager::CreateGameObject(bool l_drawable, bool l_controllable, const std::string& l_texture) {
	GameObject* gameObject = new GameObject(m_nextAvailableID, l_drawable, l_controllable, m_textureManager);
	if (l_drawable) {
		gameObject->SetSprite(l_texture);
	}
	m_gameObjects[m_nextAvailableID] = gameObject;

	return m_nextAvailableID++; // returns the ID and adds 1
}

void GameObjectManager::UpdateGameObject(GameObjectID l_id) {}

void GameObjectManager::RemoveGameObject(GameObjectID l_id) {}

void GameObjectManager::Notify(Message l_message) {}
