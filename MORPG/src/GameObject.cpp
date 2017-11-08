#include "GameObject.hpp"
#include "Utilities.hpp"

GameObject::GameObject(GameObjectID l_gameObjectID, bool l_isDrawable, bool l_isControllable, TextureManager* l_textureManager)
	: m_id(l_gameObjectID), m_isDrawable(l_isDrawable), m_isControllable(l_isControllable), m_position(sf::Vector2f(0, 0)) {
	m_textureManager = l_textureManager;
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

void GameObject::SetPosition(const sf::Vector2f & l_position) {
	m_position = l_position;
}

void GameObject::SetSprite(const std::string& l_texture) {
	if (m_textureManager->RequireTexture(l_texture)) {
		sf::Texture* texture = m_textureManager->GetTexture(l_texture);
		m_sprite.setTexture(*texture);
	} else {
		printf("Texture not found - %s%s\n", Utils::GetWorkingDirectory().c_str(), l_texture.c_str());
	}
}

bool GameObject::IsDrawable() {
	return m_isDrawable;
}

bool GameObject::IsControllable() {
	return m_isControllable;
}

