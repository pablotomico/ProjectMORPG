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

	virtual void Move(float l_x, float l_y) {}
	virtual void Move(sf::Vector2f l_move) {}
private:
	GameObjectID m_id;

	sf::Vector2f m_position;
	sf::Sprite m_sprite;

	bool m_isDrawable;
	bool m_isControllable;
};


using GameObjectContainer = std::unordered_map<GameObjectID, GameObject*>;