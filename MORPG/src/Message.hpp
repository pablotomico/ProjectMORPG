#pragma once

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
	M_Player
	// TODO: Come up with more types
};

struct GameObjectMessage {
	GameObjectMessage(GameObjectID l_id, sf::Vector2f l_position, sf::Time l_time)
		: m_gameObjectID(l_id), m_position(l_position), m_time(l_time) {}

	void Print() {
		printf("[%d] (%f, %f)\n", m_gameObjectID, m_position.x, m_position.y);
	}

	GameObjectID m_gameObjectID;
	sf::Vector2f m_position;
	sf::Time m_time;
};

class Message {
public:
	Message(MessageType l_type, System l_systemReceiver);
	Message(MessageType l_type, System l_systemReceiver, const std::string& l_string);
	Message(MessageType l_type, System l_systemReceiver, KeyCode l_keyCode);
	Message(MessageType l_type, System l_systemReceiver, GameObjectID l_id, sf::Vector2f l_position, sf::Time l_time);

	~Message();
public:
	MessageType m_type;
	System m_systemReceiver;
	union {
		std::string* m_string;
		int m_integer;
		KeyCode m_keyCode;
		std::pair<int, GameObjectID> m_gameObjectIDs;
		GameObjectMessage m_gameObject;
	};
};