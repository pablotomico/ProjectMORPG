#include "GameObject.hpp"
#include "Util/Utilities.hpp"

GameObject::GameObject(GameObjectID l_gameObjectID, std::string l_name, bool l_isDrawable, bool l_isControllable, TextureManager* l_textureManager)
	: m_id(l_gameObjectID), m_isDrawable(l_isDrawable), m_isControllable(l_isControllable), m_position(sf::Vector2f(0, 0)), m_spriteOffset(sf::Vector2f(0, 50)), m_name(l_name) {
	m_textureManager = l_textureManager;
	m_nameText.setString(l_name);
	OnCreate();
}

GameObject::~GameObject() {
	OnDestroy();
}

void GameObject::Update(int l_tick, float l_deltaTime) {

	if (m_castingSpell && m_spellCast.first <= l_tick) {
		ThrowSpell();
	}

	if (!IsControllable()) {
		PredictPosition(l_tick, l_deltaTime);
	}

	if (IsDrawable()) {
		m_sprite.setPosition(m_position + m_spriteOffset);
		m_nameText.setPosition(m_position);
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

sf::Text & GameObject::GetNameText() {
	return m_nameText;
}

void GameObject::SetPosition(const sf::Vector2f & l_position, int l_tick) {
	if (l_tick > -1) {
		int deltaTick = l_tick - m_netPosition.first;
		m_netDeltaTime = 0;
		if (deltaTick <= 0) {
			return;
		}
		m_velocity.x = (l_position.x - m_netPosition.second.x) / (deltaTick * m_timestep);
		m_velocity.y = (l_position.y - m_netPosition.second.y) / (deltaTick * m_timestep);

		m_netPosition = std::pair<int, sf::Vector2f>(l_tick, l_position);
	}
	else {
		m_position = l_position;
	}
}

void GameObject::SetSprite(const std::string& l_texture) {
	if (m_textureManager->RequireTexture(l_texture)) {
		sf::Texture* texture = m_textureManager->GetTexture(l_texture);
		m_sprite.setTexture(*texture);
	}
	else {
		DEBUG("Texture not found - " << Utils::GetWorkingDirectory().c_str() << l_texture.c_str());
	}
}

void GameObject::SetSpriteScale(const float l_x, const float l_y) {
	m_sprite.setScale(l_x, l_y);
}

void GameObject::CastSpell(int l_endTick, int l_spellID) {
	if (l_spellID < 0) {
		m_castingSpell = false;
		return;
	}
	if (m_castingSpell) {
		LOG("Already casting a spell!");
		return;
	}
	m_castingSpell = true;

	m_spellCast.first = l_endTick;
	m_spellCast.second = l_spellID;
}

void GameObject::ThrowSpell() {
	m_castingSpell = false;

	LOG("Spell " << m_spellCast.second << " finished!");
}


void GameObject::PredictPosition(int l_tick, float l_deltaTime) {
	int deltaTick = l_tick - m_netPosition.first;
	m_netDeltaTime += l_deltaTime;
	m_position.x = m_netPosition.second.x + m_velocity.x * (l_deltaTime + m_timestep * deltaTick);
	m_position.y = m_netPosition.second.y + m_velocity.y * (l_deltaTime + m_timestep * deltaTick);
	//printf("Velocity = (%f, %f) l_deltaTime = %f, m_timestep = %f, deltaTick = %d\n", m_velocity.x, m_velocity.y, l_deltaTime, m_timestep, deltaTick);
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

