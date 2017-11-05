#pragma once

#include <SFML/Graphics.hpp>

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

	virtual void OnKeyDown(sf::Keyboard::Key l_keyCode) {}
	virtual void OnKey(sf::Keyboard::Key l_keyCode) {}
	virtual void OnKeyUp(sf::Keyboard::Key l_keyCode) {}
private:
	GameObjectID m_id;

	sf::Vector2f m_position;
	sf::Sprite m_sprite;

	bool m_isDrawable;
	bool m_isControllable;
};