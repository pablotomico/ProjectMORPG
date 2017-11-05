#include "GameObject.hpp"

GameObject::GameObject(GameObjectID l_gameObjectID, bool l_isDrawable, bool l_isControllable)
	: m_id(l_gameObjectID), m_isDrawable(l_isDrawable), m_isControllable(l_isControllable), m_position(sf::Vector2f(0, 0)) {
	OnCreate();
}

GameObject::~GameObject() {
	OnDestroy();
}

GameObjectID GameObject::GetGameObjectID() {
	return m_id;
}

sf::Vector2f GameObject::GetPosition() {
	return m_position;
}

sf::Sprite& GameObject::GetSprite() {
	return m_sprite;
}

bool GameObject::IsDrawable() {
	return m_isDrawable;
}

bool GameObject::IsControllable() {
	return m_isControllable;
}

