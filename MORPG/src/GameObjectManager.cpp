#include "GameObjectManager.hpp"
#include "GameObject.hpp"


GameObjectManager::GameObjectManager(const std::shared_ptr<MessageSystem>& l_messageSystem, const std::shared_ptr<TextureManager>& l_textureManager)
	: Observer(System::S_GameObjectManager, l_messageSystem)
	, m_nextAvailableID(0)
	, m_textureManager(l_textureManager) {
	m_gameObjects = std::make_shared<GameObjectContainer>();
}

GameObjectManager::~GameObjectManager() {
	// Destroy GameObjects
	while (m_gameObjects->begin() != m_gameObjects->end()) {
		delete m_gameObjects->begin()->second;
		m_gameObjects->erase(m_gameObjects->begin());
	}
}

std::shared_ptr<GameObjectContainer> GameObjectManager::GetGameObjectContainer() {
	return m_gameObjects;
}

GameObject * GameObjectManager::GetGameObject(const GameObjectID& l_id) {
	return m_gameObjects->at(l_id);
}

GameObjectID GameObjectManager::CreateGameObject(const std::string& l_name, const bool& l_drawable, const bool& l_controllable, const std::string& l_texture) {
	GameObject* gameObject = new GameObject(m_nextAvailableID, l_name, l_drawable, l_controllable, m_textureManager);
	if (l_drawable) {
		gameObject->SetSprite(l_texture);
	}
	m_gameObjects->emplace(m_nextAvailableID, gameObject);
	//printf("[SYSTEM] Creating gameobject %d\n", m_nextAvailableID);
	return m_nextAvailableID++; // returns the ID and adds 1
}

void GameObjectManager::UpdateGameObject(const GameObjectID& l_id, const sf::Vector2f& l_position) {
	GameObject* gameObject = GetGameObject(l_id);
	gameObject->SetPosition(l_position, -1);
}

void GameObjectManager::UpdateGameObject(const GameObjectID& l_id, const sf::Vector2f& l_position, const int& l_tick) {
	GameObject* gameObject = GetGameObject(l_id);
	gameObject->SetPosition(l_position, l_tick);
}

void GameObjectManager::RemoveGameObject(const GameObjectID& l_id) {
	GameObject* gameObject = GetGameObject(l_id);
	m_gameObjects->erase(l_id);
	delete gameObject;
}

void GameObjectManager::Update(const int& l_tick, const float& l_deltaTime) {
	for (auto& itr = m_gameObjects->begin(); itr != m_gameObjects->end(); ++itr) {
		GameObject* gameObject = itr->second;
		gameObject->Update(l_tick, l_deltaTime);
	}
}

void GameObjectManager::Notify(const Message& l_message) {
	switch (l_message.m_type) {
	case MessageType::M_GameObjectCreated:
	{
		GameObjectID id = CreateGameObject(std::string(l_message.m_gameObjectCreated.m_name), true, false, "character_1.png");
		GameObject* gameobject = GetGameObject(id);
		gameobject->SetSpriteScale(0.25f, 0.25f);
		gameobject->m_timestep = l_message.m_gameObjectCreated.m_timestep;

		Message msg(MessageType::M_GameObjectCreated, System::S_NetworkControl);
		msg.m_gameObjectCreated.m_gameObjectIDs.first = l_message.m_gameObjectCreated.m_gameObjectIDs.first;
		msg.m_gameObjectCreated.m_gameObjectIDs.second = id;
		Send(msg);
		break;
	}
	case MessageType::M_GameObjectDeleted:
	{
		RemoveGameObject(l_message.m_integer);
	}
	break;
	case MessageType::M_GameObject:
	{
		UpdateGameObject(l_message.m_gameObject.m_gameObjectID, l_message.m_gameObject.m_position, l_message.m_gameObject.m_tick);
	}
	break;

	case MessageType::M_CastSpell:
	{
		GameObject* gameobject = GetGameObject(l_message.m_gameObjectSpellData.first);
		gameobject->CastSpell(l_message.m_gameObjectSpellData.second.m_endTick, l_message.m_gameObjectSpellData.second.m_spellID);
	}
	break;
	}
}
