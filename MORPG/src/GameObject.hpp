#pragma once

#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "Definitions.hpp"

using GameObjectID = unsigned int;

class GameObject {
public:
	GameObject(GameObjectID l_gameObjectID, bool l_isDrawable = false, bool l_isControllable = false);
	~GameObject();

	virtual void OnCreate() {}
	virtual void Update() {}
	virtual void OnDestroy() {}

	GameObjectID GetGameObjectID();

	sf::Vector2f GetPosition();
	sf::Sprite& GetSprite();

	bool IsDrawable();
	bool IsControllable();

	virtual void OnKeyDown(KeyCode l_keyCode) {}
	virtual void OnKey(KeyCode l_keyCode) {}
	virtual void OnKeyUp(KeyCode l_keyCode) {}
private:
	GameObjectID m_id;

	sf::Vector2f m_position;
	sf::Sprite m_sprite;

	bool m_isDrawable;
	bool m_isControllable;
};


using GameObjectContainer = std::unordered_map<GameObjectID, GameObject*>;