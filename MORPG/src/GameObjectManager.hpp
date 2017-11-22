#pragma once

#include "Observer.hpp"
#include "GameObject.hpp"

class GameObjectManager : public Observer {

public:
	GameObjectManager(MessageSystem* l_messageSystem, TextureManager* l_textureManager);
	~GameObjectManager();

	GameObjectContainer* GetGameObjectContainer();
	GameObject* GetGameObject(GameObjectID l_id);

	GameObjectID CreateGameObject(bool l_drawable = false, bool l_controllable = false, const std::string& l_texture = false);
	void UpdateGameObject(GameObjectID l_id, sf::Vector2f l_position);
	void RemoveGameObject(GameObjectID l_id);

protected:
	void Notify(Message l_message);

private:
	GameObjectID m_nextAvailableID;
	GameObjectContainer m_gameObjects;

	TextureManager* m_textureManager;
};