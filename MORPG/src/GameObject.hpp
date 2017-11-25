#pragma once

#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "Definitions.hpp"
#include "TextureManager.hpp"

using GameObjectID = unsigned int;

class GameObject {
public:
	GameObject(GameObjectID l_gameObjectID, bool l_isDrawable = false, bool l_isControllable = false, TextureManager* l_textureManager = nullptr);
	~GameObject();

	virtual void OnCreate() {}
	virtual void Update() {}
	virtual void OnDestroy() {}

	GameObjectID GetGameObjectID();

	sf::Vector2f GetPosition();
	std::pair<int, sf::Vector2f> GetNetPosition();
	sf::Sprite& GetSprite();

	void SetPosition(const sf::Vector2f& l_position, int l_tick);
	void SetSprite(const std::string& l_texture);
	void SetSpriteScale(const float l_x, const float l_y);

	void PredictPosition(int l_tick, float l_deltaTime);

	bool IsDrawable();
	bool IsControllable();

	virtual void Move(float l_x, float l_y) {
		m_position.x += l_x;
		m_position.y += l_y;
		//printf("[%d] at (%f, %f)\n", m_id, m_position.x, m_position.y);
	}

	virtual void Move(sf::Vector2f l_move) {
		Move(l_move.x, l_move.y);
	}\

public:
	float m_timestep;

private:
	GameObjectID m_id;

	sf::Vector2f m_position;
	std::pair<int, sf::Vector2f> m_netPosition;
	sf::Vector2f m_velocity;
	sf::Sprite m_sprite;
	std::string m_texture;

	

	TextureManager* m_textureManager;

	bool m_isDrawable;
	bool m_isControllable;
};


using GameObjectContainer = std::unordered_map<GameObjectID, GameObject*>;