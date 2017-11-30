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

void GameObject::Update(int l_tick, float l_deltaTime) {

	if (m_castingSpell && m_spellCast.first == l_tick) {
		ThrowSpell();
	}

	if (!IsControllable()) {
		PredictPosition(l_tick, l_deltaTime);
	}


	//printf("SIZE: %d\n", m_gameObjects.size());
	
}

GameObjectID GameObject::GetGameObjectID() {
	return m_id;
}

sf::Vector2f GameObject::GetPosition() {
	return m_position;
}

std::pair<int, sf::Vector2f> GameObject::GetNetPosition() {
	return m_netPosition;
}

sf::Sprite& GameObject::GetSprite() {
	return m_sprite;
}

void GameObject::SetPosition(const sf::Vector2f & l_position, int l_tick) {
	if (l_tick > -1) {
		int deltaTick = l_tick - m_netPosition.first;
		if (deltaTick == 0) {
			return;
		}
		m_velocity.x = (l_position.x - m_netPosition.second.x) / (deltaTick * m_timestep);
		m_velocity.y = (l_position.y - m_netPosition.second.y) / (deltaTick * m_timestep);
		m_netPosition = std::pair<int, sf::Vector2f>(l_tick, l_position);
	} else {

		m_position = l_position;
	}
}

void GameObject::SetSprite(const std::string& l_texture) {
	if (m_textureManager->RequireTexture(l_texture)) {
		sf::Texture* texture = m_textureManager->GetTexture(l_texture);
		m_sprite.setTexture(*texture);
	} else {
		printf("Texture not found - %s%s\n", Utils::GetWorkingDirectory().c_str(), l_texture.c_str());
	}
}

void GameObject::SetSpriteScale(const float l_x, const float l_y) {
	m_sprite.setScale(l_x, l_y);
}

void GameObject::CastSpell(int l_endTick, int l_spellID) {
	if (m_castingSpell) {
		printf("Already casting a spell!\n");
		return;
	}
	m_castingSpell = true;

	m_spellCast.first = l_endTick;
	m_spellCast.second = l_spellID;
}

void GameObject::ThrowSpell() {
	m_castingSpell = false;

	printf("Spell %d finished!\n", m_spellCast.second);
}


void GameObject::PredictPosition(int l_tick, float l_deltaTime) {
	int deltaTick = l_tick - m_netPosition.first;
	m_position.x = m_netPosition.second.x + m_velocity.x * l_deltaTime;
	m_position.y = m_netPosition.second.y + m_velocity.y * l_deltaTime;

	if (m_velocity.x > 0 || m_velocity.y > 0) {

		/*printf("[%d] PREDICTION:\n", m_id);
		printf("\tLAST POSITION RECEIVED = (%f, %f) ON TICK %d\n", m_netPosition.second.x, m_netPosition.second.y, m_netPosition.first);
		printf("\tPOSITION PREDICTED     = (%f, %f) ON TICK %d VELOCITY = (%f, %f)\n\n", m_position.x, m_position.y, l_tick, m_velocity.x, m_velocity.y);*/
	}
}

bool GameObject::IsDrawable() {
	return m_isDrawable;
}

bool GameObject::IsControllable() {
	return m_isControllable;
}

