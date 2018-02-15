#pragma once

#include <unordered_map>
#include <SFML/Graphics.hpp>
#include <memory>
#include "Definitions.hpp"
#include "TextureManager.hpp"
#include "Spell.hpp"

using GameObjectID = unsigned int;

class GameObject {
public:
	GameObject(const GameObjectID& l_gameObjectID, const std::string& l_name, const bool& l_isDrawable = false,
		const bool& l_isControllable = false, const std::shared_ptr<TextureManager>& l_textureManager = nullptr);
	~GameObject();

	virtual void OnCreate() {}
	virtual void OnDestroy() {}
	void Update(const int& l_tick, const float& l_deltaTime);

	GameObjectID GetGameObjectID();

	sf::Vector2f GetPosition();
	std::pair<int, sf::Vector2f> GetNetPosition();
	sf::Sprite& GetSprite();
	sf::Text& GetNameText();

	void SetPosition(const sf::Vector2f& l_position, const int& l_tick);
	void SetSprite(const std::string& l_texture);
	void SetSpriteScale(const float& l_x, const float& l_y);

	void CastSpell(const int& l_endTick, const int& l_spellID);
	void ThrowSpell();

	void PredictPosition(const int& l_tick, const float& l_deltaTime);

	bool IsDrawable();
	bool IsControllable();

	virtual void Move(const float& l_x, const float& l_y) {
		m_position.x += l_x;
		m_position.y += l_y;
		//printf("[%d] at (%f, %f)\n", m_id, m_position.x, m_position.y);
	}

	virtual void Move(const sf::Vector2f& l_move) {
		Move(l_move.x, l_move.y);
	}

public:
	float m_timestep;
	sf::Vector2f m_spriteOffset;

	std::string m_name;

private:
	GameObjectID m_id;

	sf::Vector2f m_position;
	std::pair<int, sf::Vector2f> m_netPosition;
	float m_netDeltaTime;
	sf::Vector2f m_velocity;
	sf::Sprite m_sprite;
	sf::Text m_nameText;

	std::string m_texture;

	bool m_castingSpell;
	SpellCast m_spellCast;

	std::shared_ptr<TextureManager> m_textureManager;

	bool m_isDrawable;
	bool m_isControllable;
};


using GameObjectContainer = std::unordered_map<GameObjectID, GameObject*>;