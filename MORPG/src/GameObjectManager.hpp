#pragma once

#include "Observer.hpp"
#include "GameObject.hpp"

class GameObjectManager : public Observer {

public:
	GameObjectManager(const std::shared_ptr<MessageSystem>& l_messageSystem, const std::shared_ptr<TextureManager>& l_textureManager);
	~GameObjectManager();

	std::shared_ptr<GameObjectContainer> GetGameObjectContainer();
	GameObject* GetGameObject(const GameObjectID& l_id);

	GameObjectID CreateGameObject(const std::string& l_name, const bool& l_drawable = false, const bool& l_controllable = false, const std::string& l_texture = false);
	void UpdateGameObject(const GameObjectID& l_id, const sf::Vector2f& l_position);
	void UpdateGameObject(const GameObjectID& l_id, const sf::Vector2f& l_position, const int& l_tick);
	void RemoveGameObject(const GameObjectID& l_id);

	void Update(const int& l_tick, const float& l_deltaTime);

protected:
	void Notify(const Message& l_message);

private:
	GameObjectID m_nextAvailableID;
	std::shared_ptr<GameObjectContainer> m_gameObjects;

	std::shared_ptr<TextureManager> m_textureManager;
};