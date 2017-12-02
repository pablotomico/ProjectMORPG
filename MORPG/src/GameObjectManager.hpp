#pragma once

#include "Observer.hpp"
#include "GameObject.hpp"

class GameObjectManager : public Observer {

public:
	GameObjectManager(MessageSystem* l_messageSystem, TextureManager* l_textureManager);
	~GameObjectManager();

	GameObjectContainer* GetGameObjectContainer();
	GameObject* GetGameObject(GameObjectID l_id);

	GameObjectID CreateGameObject(std::string l_name, bool l_drawable = false, bool l_controllable = false, const std::string& l_texture = false);
	void UpdateGameObject(GameObjectID l_id, sf::Vector2f l_position);
	void UpdateGameObject(GameObjectID l_id, sf::Vector2f l_position, int l_tick);
	void RemoveGameObject(GameObjectID l_id);

	void Update(int l_tick, float l_deltaTime);

protected:
	void Notify(Message l_message);

private:
	GameObjectID m_nextAvailableID;
	GameObjectContainer m_gameObjects;

	TextureManager* m_textureManager;
};