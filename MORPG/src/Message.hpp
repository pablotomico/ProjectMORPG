#pragma once
#include <utility>
#include <iostream>
#include <SFML/Window.hpp>
#include "Systems.hpp"
#include "Definitions.hpp"
#include "GameObject.hpp"

#define MULTICAST -1

enum  MessageType {
	String,
	Int,
	M_KeyPressed,
	M_KeyHold,
	M_KeyReleased,
	M_GameObject,
	M_GameObjectCreated,
	M_GameObjectDeleted,
	M_Player,
	M_PlayerConnected,
	M_SetServerTimestep,
	M_CastSpell
	// TODO: Come up with more types
};

struct GameObjectMessage {
	GameObjectMessage(GameObjectID l_id, sf::Vector2f l_position)
		: m_gameObjectID(l_id), m_position(l_position) {}
	GameObjectMessage(GameObjectID l_id, sf::Vector2f l_position, int l_tick)
		: m_gameObjectID(l_id), m_position(l_position), m_tick(l_tick) {}

	void Print() {
		printf("[%d] (%f, %f)\n", m_gameObjectID, m_position.x, m_position.y);
	}

	GameObjectID m_gameObjectID;
	sf::Vector2f m_position;
	int m_tick;
};

struct GameObjectCreated {
	char m_name[16];
	std::pair<int, GameObjectID> m_gameObjectIDs;
	float m_timestep;
};

struct SpellData {
	int m_spellID;
	union {
		float m_duration;
		int m_endTick;
	};
};

struct PlayerData {
	int m_id;
	char m_username[16];
};

class Message {
public:
	Message(MessageType l_type, System l_systemReceiver);
	Message(MessageType l_type, System l_systemReceiver, const std::string& l_string);
	Message(MessageType l_type, System l_systemReceiver, KeyCode l_keyCode);
	Message(MessageType l_type, System l_systemReceiver, GameObjectID l_id, sf::Vector2f l_position);
	Message(MessageType l_type, System l_systemReceiver, GameObjectID l_id, sf::Vector2f l_position, int l_tick);


	~Message();
public:
	MessageType m_type;
	System m_systemReceiver;
	union {
		std::string* m_string;
		int m_integer;
		float m_float;
		KeyCode m_keyCode;
		GameObjectCreated m_gameObjectCreated;
		GameObjectMessage m_gameObject;
		SpellData m_spellData;
		std::pair<GameObjectID, SpellData> m_gameObjectSpellData;
		PlayerData m_playerData;
	};
};